#include <cstdlib>
#include <time.h>
#include "Session.h"
#include "SessionMgr.h"
#include "Worker.h"
#include "SessionTimer.h"
#include "MainConfig.h"
#include "NasSystemTable.h"
#include "AppXmlParser.h"
#include "main.h"

#define BUFF_SIZE 10240
using namespace eSipUtil;
Session::Session()
{
	m_pclsOwner = NULL;
	m_pclsTrsgTr = NULL;
	m_unTid = 0;
	m_clsSessionID = KNULL;
	m_clsJobID = KNULL;
	m_eSt = E_SESS_ST_NONE;
	m_nAllocSes = -1;
	m_clsLastSendXml = KNULL;
	m_pclsSourceContent = new SourceContent;
	m_clsImageData = KNULL;
	m_clsMetaImage = KNULL;
	m_clsMetaCommand = KNULL;
	m_unTranscodingList = 0;
	m_unTargetContentList = 0;
	m_clsTargetList.m_fnClear();
	m_clsCodecList.m_fnClear();
	m_clsInputFile = KNULL;
	m_eSrcSt = E_TC_RES_OK;
	m_nAllocWorker = Worker::m_pWorkerIdMgr->getAvailableId();
	if (m_nAllocWorker != -1)
	{
		//int nTotalSessionNum = (MainConfig::m_fnGetInstance()->m_unWorkerNum) * (MainConfig::m_fnGetInstance()->m_unWorkerIdMgrNum);
		int nTotalSessionNum = (MainConfig::m_fnGetInstance()->m_unWorkerIdMgrNum);
		IFLOG(E_LOG_ERR, "New Session[Total Session:%d]", nTotalSessionNum - Worker::m_pWorkerIdMgr->getAvailableIdNum());
	}

	// Session QueueThread 만들기
	m_pclsQ = new QueueThread<AppBaseEvent>;
	m_pclsQ->setName("SessionQueue");
	m_pclsQ->setMaxQueueSize(1000);
	m_pclsQ->setObject(this);
	m_pclsQ->setProcessCb(m_fnCbkProcess);
	m_pclsQ->run();
}
Session::~Session()
{
	if (m_pclsQ)
	{
		m_pclsQ->shutdown();
		m_pclsQ->join();
		delete m_pclsQ;
	}

	if (m_pclsTrsgTr)
	{
		KString clsSessionID = m_pclsTrsgTr->m_clsSessionID;
		Worker *pclsWorker = (Worker*) m_fnGetWorker();
		if (pclsWorker->m_clsTrsgTrMgr.m_fnDel(m_unTid, m_clsJobID, m_clsSessionID))
			IFLOG(E_LOG_INFO, "Trsg Tr Del[Tid:%d][JobID:%s][SessionID:%s]", m_unTid, (KCSTR) m_clsJobID, (KCSTR) m_clsSessionID);
		else
			IFLOG(E_LOG_ERR, "Trsg Tr Del Fail[Tid:%d][JobID:%s][SessionID:%s]", m_unTid, (KCSTR) m_clsJobID, (KCSTR) m_clsSessionID);
	}

	if (m_pclsSourceContent)
	{
		delete m_pclsSourceContent;
	}

	m_clsTargetList.m_fnClear();
	m_clsCodecList.m_fnClear();

	if (m_nAllocWorker != -1)
	{
		Worker::m_pWorkerIdMgr->releaseId(m_nAllocWorker);
		//int nTotalSessionNum = (MainConfig::m_fnGetInstance()->m_unWorkerNum) * (MainConfig::m_fnGetInstance()->m_unWorkerIdMgrNum);
		int nTotalSessionNum = (MainConfig::m_fnGetInstance()->m_unWorkerIdMgrNum);
		IFLOG(E_LOG_ERR, "Delete Session[Total Session:%d]", nTotalSessionNum - Worker::m_pWorkerIdMgr->getAvailableIdNum());
	}
}
void Session::m_fnRecvTrsgCrtJobReq(KString &_rclsXml)
{
	/*
	 * CreateJobRequest 받았을 때 할일
	 * 1. TargetContent Count
	 * 2. SourceContent
	 * 3. TargetContent
	 * 4. InputFile
	 * 5. Metadata
	 * 6. OutputFile
	 * 7. Codec Find
	 * 8. Command 만들기
	 * 9. Command 실행
	 * 10. Send "Created" Event
	 */

	// CreateJobRequest MSG
	m_pclsTrsgTr->m_fnRecvCrtJobReq(_rclsXml);

	m_clsLastSendXml.m_fnReSize(BUFF_SIZE);
	m_clsLastSendXml = _rclsXml;

	m_eSt = E_SESS_ST_TRSG_CREATE_JOB_REQ_RCV;
	// Target Count
	m_unTranscodingList = AppXmlParser::m_fnGetTranscodingList(_rclsXml);

	// SourceContent
	AppXmlParser::m_fnGetSourceContentInfo(_rclsXml, m_pclsSourceContent);
	// TargetContent
	m_unTargetContentList = AppXmlParser::m_fnGetTargetContentList(_rclsXml, &m_clsTargetList);

	Worker * pclsWorker = (Worker*)m_fnGetWorker();
	MainConfig *pclsConf = MainConfig::m_fnGetInstance();
	NasSystemTable *pclsNasSystem = NasSystemTable::m_fnGetInstance();

	// SessionTimer 설정
	pclsWorker->m_clsSessionTimer.m_fnSetTimer((KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, pclsConf->m_unSesTmr);

	// InputFile
	if (m_pclsSourceContent->m_clsBinaryData.m_unRealLen > 0)
	{
		// Internal NAS Check
		KString clsPath = (KCSTR) pclsConf->m_clsNasInternal;
		m_clsInputFile.m_fnCat((KCSTR) clsPath);
		if (((KCSTR) clsPath)[clsPath.m_unRealLen - 1] != '/')
			m_clsInputFile.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.

		m_clsInputFile.m_fnCat((KCSTR) m_pclsSourceContent->m_clsBinaryData);

		if (access((KCSTR)m_clsInputFile, F_OK) < 0)
		{
			// 파일 존재하지 않음
			IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] InputFile[%s] is not Exist.", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, (KCSTR)m_clsInputFile);
			m_eSrcSt = E_NAS_ACCESS_FAILED; // 잘못된 파일(NAS)경로, 존재하지 않음, 접근경로실패
		}
	}
	else
	{
		// External NAS Check
		if (m_pclsSourceContent->m_clsNasCode.m_unRealLen > 0)
		{
			pclsNasSystem->m_fnGetSourceDir(m_pclsSourceContent->m_clsNasCode, m_clsInputFile);
			if (m_clsInputFile == "UNKNOWN")
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] Invalid SourceContent NAS CODE[%s]", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, (KCSTR)m_pclsSourceContent->m_clsNasCode);
				m_eSrcSt = E_NAS_ACCESS_FAILED;
			}
			else
			{
				if (((KCSTR) m_clsInputFile)[m_clsInputFile.m_unRealLen - 1] != '/')
					m_clsInputFile.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.

				m_clsInputFile.m_fnCat((KSTR) m_pclsSourceContent->m_clsFilePath);
				if (access((KCSTR)m_clsInputFile, F_OK) < 0)
				{
					// 파일 존재하지 않음
					IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] InputFile[%s] is not Exist.", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, (KCSTR)m_clsInputFile);
					m_eSrcSt = E_NAS_ACCESS_FAILED; // 잘못된 파일(NAS)경로, 존재하지 않음, 접근경로실패
				}
			}
		}
		else
		{
			IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s]  Empty SourceContent NAS CODE", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID);
			m_eSrcSt = E_NO_SRC_NAS_CODE;
		}
	}
	IFLOG(E_LOG_INFO, "[S:%s][T:%010u][J:%s] Input FilePath[%s]", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, (KCSTR)m_clsInputFile);

	// Metadata
	if (m_pclsSourceContent->m_clsMetadata.m_clsImageData.m_unRealLen > 0)
	{
		// Metadata 데이터 있는 경우
		// 내부 NAS 에서 ImageData 가져와서 변환에 사용
		KString clsPath = (KCSTR) pclsConf->m_clsNasInternal;
		m_clsImageData.m_fnCat((KCSTR) clsPath);
		if (((KCSTR) clsPath)[clsPath.m_unRealLen - 1] != '/')
			m_clsImageData.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.
		m_clsImageData.m_fnCat((KCSTR) m_pclsSourceContent->m_clsMetadata.m_clsImageData);

		if (access((KCSTR)m_clsImageData, F_OK) < 0)
		{
			IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] Metadata ImageFile[%s] is not Exist", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, (KCSTR) m_pclsSourceContent->m_clsMetadata.m_clsImageData);
			m_eSrcSt = E_NAS_ACCESS_FAILED;
		}
		else
		{
			// MetaData 의 Image
			KString clsImage;
			KString::m_fnStrnCat((KSTR)clsImage, BUFF_SIZE, "-i %s -map 0:0 -map 1:0", (KSTR)m_clsImageData);
			m_clsMetaImage = clsImage;

			// 그 외의 Metadata 정보
			KString clsCommand; clsCommand.m_fnReSize(BUFF_SIZE);
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetAlbum(), "") != 0)
				clsCommand << "-metadata Album=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetAlbum() << "\" ";
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetArtist(), "") != 0)
							clsCommand << "-metadata Artist=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetArtist() << "\" ";
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetCopyright(), "") != 0)
							clsCommand << "-metadata Copyright=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetCopyright() << "\" ";
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetDate(), "") != 0)
							clsCommand << "-metadata Date=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetDate() << "\" ";
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetGenre(), "") != 0)
							clsCommand << "-metadata Genre=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetGenre() << "\" ";
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetImageType(), "") != 0)
							clsCommand << "-metadata ImageType=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetImageType() << "\" ";
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetPlayDevice(), "") != 0)
							clsCommand << "-metadata PlayDevice=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetPlayDevice() << "\" ";
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetPlayTime(), "") != 0)
							clsCommand << "-metadata PlayTime=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetPlayTime() << "\" ";
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetSite(), "") != 0)
							clsCommand << "-metadata Site=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetSite() << "\" ";
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetTitle(), "") != 0)
							clsCommand << "-metadata Title=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetTitle() << "\" ";
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetTrack(), "") != 0)
							clsCommand << "-metadata Track=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetTrack() << "\" ";
			if(strcmp(m_pclsSourceContent->m_clsMetadata.m_fnGetYear(), "") != 0)
							clsCommand << "-metadata Year=\"" << m_pclsSourceContent->m_clsMetadata.m_fnGetYear() << "\" ";
			clsCommand << "-metadata www=\"\" -metadata Comment=\"\" -metadata Encodedby=\"\"";

			m_clsMetaCommand = clsCommand;
		}
		IFLOG(E_LOG_INFO, "[S:%s][T:%010u][J:%s] Metadata ImageData FilePath[%s] & Metadata Command[%s]", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, (KCSTR)m_clsMetaImage, (KCSTR)m_clsMetaCommand);
	}
	IFLOG(E_LOG_DEBUG, "SourceState(After Received CrtJobReq) : %s", g_fnGetTrssCodeDesc((KCSTR)g_fnGetTrssCode(m_eSrcSt)));

	// 3. OutputFile
	Iterator stTargetItor;
	TargetContent *pclsTarget = (TargetContent*)m_clsTargetList.m_fnGetNext(stTargetItor);
	while (pclsTarget)
	{
		KString clsOutputFile;	clsOutputFile.m_fnReSize(BUFF_SIZE);
		pclsTarget->m_eTcSt = E_TC_RES_OK;
		if (m_pclsSourceContent->m_clsBinaryData.m_unRealLen > 0)
		{
			//Internal NAS
			KString clsNasPath = (KCSTR) pclsConf->m_clsNasInternal;
			clsOutputFile.m_fnCat((KCSTR) clsNasPath);
			if (((KCSTR) clsNasPath)[clsNasPath.m_unRealLen - 1] != '/')
				clsOutputFile.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.

			if(pclsTarget->m_clsContainer.m_clsID == "VOX")
			{
				KString clsTempPath = clsOutputFile;
				KString clsTempFile = m_fnGetFileName();
				clsTempPath.m_fnCat((KCSTR)clsTempFile);
				pclsTarget->m_clsTempFile = clsTempPath;
				IFLOG(E_LOG_DEBUG, "[S:%s][T:%010u][J:%s] TEMP File For VOX[%s]", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, (KCSTR)pclsTarget->m_clsTempFile);
			}

			KString clsFileName = m_fnGetFileName();
			clsOutputFile.m_fnCat((KCSTR)clsFileName);
			pclsTarget->m_clsBinaryData = clsFileName;
			pclsTarget->m_clsOutputFile = clsOutputFile;
		}
		else
		{
			// External NAS
			if (m_pclsSourceContent->m_clsNasCode.m_unRealLen > 0)
			{
				// VOX 케이스
				if (pclsTarget->m_clsContainer.m_clsID == "VOX")
				{
					// VOX 의 중간변환 파일은 Internal NAS에 저장
					KString clsTempPath; clsTempPath.m_fnReSize(BUFF_SIZE);
					KString clsNasPath = (KCSTR) pclsConf->m_clsNasInternal;
					clsTempPath.m_fnCat((KCSTR) clsNasPath);
					if (((KCSTR) clsNasPath)[clsNasPath.m_unRealLen - 1] != '/')
						clsTempPath.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.

					KString clsTempFile = m_fnGetFileName();
					clsTempPath.m_fnCat((KCSTR) clsTempFile);
					pclsTarget->m_clsTempFile = clsTempPath;
					IFLOG(E_LOG_DEBUG, "[S:%s][T:%010u][J:%s] TEMP File For VOX[%s]", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, (KCSTR)pclsTarget->m_clsTempFile);
				}

				pclsNasSystem->m_fnGetSourceDir(pclsTarget->m_clsNasCode, clsOutputFile);
				if (clsOutputFile == "UNKNOWN")
				{
					if (pclsTarget->m_clsNasCode.m_unRealLen == 0)
					{
						IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] Empty TargetContent NAS CODE", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID);
						pclsTarget->m_eTcSt = E_NO_TARGET_NAS_CODE;
					}
					else
					{
						IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] Invalid TargetContent NAS CODE[%s]", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, (KCSTR)pclsTarget->m_clsNasCode);
						pclsTarget->m_eTcSt = E_NAS_ACCESS_FAILED;
					}
				}
				else
				{
					if (((KCSTR) clsOutputFile)[clsOutputFile.m_unRealLen - 1] != '/')
						clsOutputFile.m_fnCat("/");

					// Output Directory 확인
					KString clsOutputDir; clsOutputDir.m_fnReSize(BUFF_SIZE);
					clsOutputDir = clsOutputFile;	// NAS Source Dir
					clsOutputDir.m_fnCat((KCSTR) pclsTarget->m_clsPath);

					if(access((KCSTR)clsOutputDir, F_OK) < 0)
					{
						IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] Invalid Target Directory[%s]", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, (KCSTR)clsOutputDir);
						pclsTarget->m_eTcSt = E_NAS_ACCESS_FAILED;
					}

					clsOutputFile.m_fnCat((KCSTR) pclsTarget->m_clsFilePath);
					pclsTarget->m_clsOutputFile = clsOutputFile;
				}
			}
			else
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] Empty SourceContent NAS CODE", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID);
				pclsTarget->m_eTcSt = E_NO_SRC_NAS_CODE;
			}
		}

		KString clsDebug;
		pclsTarget->m_fnDebug(clsDebug);
		IFLOG(E_LOG_DEBUG, (KCSTR)clsDebug);

		// OutputFile Directory & NAS OK
		if (pclsTarget->m_eTcSt == E_TC_RES_OK)
		{
			// Codec Find
			KString clsSourceContainerID = m_pclsSourceContent->m_clsID;
			KString clsTargetContainerID = pclsTarget->m_clsContainer.m_clsID;
			KString clsTargetVideoCodec = pclsTarget->m_clsContainer.clsVideoCodec.m_clsID;
			if(clsTargetVideoCodec == "NONE")
				clsTargetVideoCodec = "";
			KString clsTargetAudioCodec = pclsTarget->m_clsContainer.clsAudioCodec.m_clsID;
			// Image 변환요청일 때나 AudioCodec이 NONE 일때 AudioCodec 빈값으로 처리
			if(clsTargetAudioCodec == "NONE" || clsTargetVideoCodec == "JPEG" || clsTargetVideoCodec == "PNG")
				clsTargetAudioCodec = "";

			// Font 변환 지원하지 않음
			if (clsSourceContainerID == "MDF" || clsSourceContainerID == "UTF" || clsTargetContainerID == "MDF" || clsTargetContainerID == "UTF")
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] Not Supported Codec(E_MEDIA_ENCODING_ERROR)", (KSTR) m_clsSessionID, m_unTid, (KSTR) m_clsJobID);
				pclsTarget->m_eTcSt = E_MEDIA_ENCODING_ERROR;
			}

			// 1. Audio / Video Codec Each One Check
			pclsTarget->m_eTcSt = pclsConf->m_fnChkTargetCodec(clsTargetAudioCodec, clsTargetVideoCodec);
			IFLOG(E_LOG_DEBUG, "TargetState(After First Finding A/VCodec) : %s", g_fnGetTrssCodeDesc((KCSTR)g_fnGetTrssCode(pclsTarget->m_eTcSt)));
			if (pclsTarget->m_eTcSt == E_TC_RES_OK)
			{
				TcCodec clsTargetCodec;
				bool bFind = false;
				// 완전히 같은 Codec 찾기
				bFind = pclsConf->m_fnGetTargetCodec(clsSourceContainerID, clsTargetContainerID, clsTargetAudioCodec, clsTargetVideoCodec, clsTargetCodec);
				if (bFind)
				{
					KString clsDebug;
					clsTargetCodec.m_fnDebug(clsDebug);
					IFLOG(E_LOG_DEBUG, "Get TargetCodec : %s", (KSTR) clsDebug);

					// Transcoding Command
					KString clsTcCommand;	clsTcCommand.m_fnReSize(BUFF_SIZE);
					if (clsTargetCodec.m_clsTool == "FCONV")
					{
						pclsTarget->m_clsTcType = E_TC_FFMPEG;

						clsTcCommand << clsTargetCodec.m_clsTool;
						clsTcCommand << " -i " << m_clsInputFile;

						if (clsTargetCodec.m_clsType == "Image")
						{
							if (pclsTarget->m_clsContainer.clsVideoCodec.m_clsWidth.m_unRealLen > 0 && pclsTarget->m_clsContainer.clsVideoCodec.m_clsHeight.m_unRealLen > 0)
							{
								clsTcCommand << " -vf scale=" << pclsTarget->m_clsContainer.clsVideoCodec.m_clsWidth << ":" << pclsTarget->m_clsContainer.clsVideoCodec.m_clsHeight;
								clsTcCommand << " -y " << pclsTarget->m_clsOutputFile;
							}
							else
							{
								IFLOG(E_LOG_ERR, "TC Command For JPEG Something Wrong! RESET Command");
								clsTcCommand.m_fnReSize(BUFF_SIZE);
							}
						}
						else
						{
							// Audio & Video Type
							// MetaImage Data -i [InputFile] -map 0:0 -map 1:0
							if ((KBOOL) clsTargetCodec.m_clsMetadata && m_clsMetaImage.m_unRealLen > 0)
								clsTcCommand << " " << m_clsMetaImage;

							// Format
							clsTcCommand << " -f " << clsTargetCodec.m_clsFormat;
							clsTcCommand << " -acodec " << clsTargetCodec.m_clsACodec;

							if (pclsTarget->m_clsContainer.clsAudioCodec.m_clsChannel.m_unRealLen > 0)
								clsTcCommand << " -ac " << pclsTarget->m_clsContainer.clsAudioCodec.m_clsChannel;

							if (pclsTarget->m_clsContainer.clsAudioCodec.m_clsSampleRate.m_unRealLen > 0)
								clsTcCommand << " -ar " << pclsTarget->m_clsContainer.clsAudioCodec.m_clsSampleRate;

							if (KString::m_fnStrCmp(pclsTarget->m_clsContainer.m_clsID, "VOX") != 0)
							{
								if (pclsTarget->m_clsContainer.clsAudioCodec.m_clsBitRate.m_unRealLen > 0)
									clsTcCommand << " -b:a " << pclsTarget->m_clsContainer.clsAudioCodec.m_clsBitRate;

								if (pclsTarget->m_clsSize.m_unRealLen > 0 && KString::m_fnStrCmp(pclsTarget->m_clsContainer.m_clsID, "MMF") == 0)
								{
									KString clsSize = "";
									clsSize << (KINT) pclsTarget->m_clsSize << "k";
									clsTcCommand << " -fs " << clsSize;
								}
							}

							if (clsTargetCodec.m_clsType == "Video")
							{
								clsTcCommand << " -vcodec " << clsTargetCodec.m_clsVCodec;
								if (pclsTarget->m_clsContainer.clsVideoCodec.m_clsBitRate.m_unRealLen > 0)
									clsTcCommand << " -b:v " << pclsTarget->m_clsContainer.clsVideoCodec.m_clsBitRate;

								if (pclsTarget->m_clsContainer.clsVideoCodec.m_clsMaxFrameRate.m_unRealLen > 0)
									clsTcCommand << " -r " << pclsTarget->m_clsContainer.clsVideoCodec.m_clsMaxFrameRate;

								if (pclsTarget->m_clsContainer.clsVideoCodec.m_clsWidth.m_unRealLen > 0 && pclsTarget->m_clsContainer.clsVideoCodec.m_clsHeight.m_unRealLen > 0)
									clsTcCommand << " -s " << pclsTarget->m_clsContainer.clsVideoCodec.m_clsWidth << "x" << pclsTarget->m_clsContainer.clsVideoCodec.m_clsHeight;
							}

							// Encoded Time
							if ((KBOOL) clsTargetCodec.m_clsTimeStamp)
								clsTcCommand << " -timestamp now";

							// -metadata Album=...
							if ((KBOOL) clsTargetCodec.m_clsMetadata && m_clsMetaCommand.m_unRealLen > 0)
							{
								clsTcCommand << " " << m_clsMetaCommand << " -y " << pclsTarget->m_clsOutputFile;

								// -map 0 -map -0:v
								if (clsTargetCodec.m_clsEtc.m_unRealLen > 0)
									clsTcCommand << " " << clsTargetCodec.m_clsEtc;
							}
							else
							{
								if (clsTargetCodec.m_clsEtc.m_unRealLen > 0)
									clsTcCommand << " " << clsTargetCodec.m_clsEtc;

								if (clsTargetContainerID == "VOX")
									clsTcCommand << " -y " << pclsTarget->m_clsTempFile;
								else
									clsTcCommand << " -y " << pclsTarget->m_clsOutputFile;
							}
						}
					}
					else if (clsTargetCodec.m_clsTool == "MCONV")
					{
						// MCONV
						pclsTarget->m_clsTcType = E_TC_MCONV;

						clsTcCommand << clsTargetCodec.m_clsTool;

						if (clsSourceContainerID == "K3G")	clsSourceContainerID = "3GP";

						clsTcCommand << " fc -sf " << clsSourceContainerID << " ";

						if (clsTargetCodec.m_clsType == "Video")
						{
							clsTcCommand << "NULL " << m_clsInputFile;
							clsTcCommand << " -df " << clsTargetCodec.m_clsFormat << " NULL " << clsOutputFile;
							clsTcCommand << " -dv " << clsTargetCodec.m_clsVCodec;

							// MaxFrameRate
							if (pclsTarget->m_clsContainer.clsVideoCodec.m_clsMaxFrameRate.m_unRealLen > 0)
								clsTcCommand << " " << pclsTarget->m_clsContainer.clsVideoCodec.m_clsMaxFrameRate;
							else
								clsTcCommand << " 0";

							// BitRate
							if (pclsTarget->m_clsContainer.clsVideoCodec.m_clsBitRate.m_unRealLen > 0)
								clsTcCommand << " " << pclsTarget->m_clsContainer.clsVideoCodec.m_clsBitRate;
							else
								clsTcCommand << " 0";

							// Width
							if (pclsTarget->m_clsContainer.clsVideoCodec.m_clsWidth.m_unRealLen > 0)
								clsTcCommand << " " << pclsTarget->m_clsContainer.clsVideoCodec.m_clsWidth;
							else
								clsTcCommand << " 0";

							//Height
							if (pclsTarget->m_clsContainer.clsVideoCodec.m_clsHeight.m_unRealLen > 0)
								clsTcCommand << " " << pclsTarget->m_clsContainer.clsVideoCodec.m_clsHeight;
							else
								clsTcCommand << " 0";
						}
						else	// Audio
						{
							clsTcCommand << m_clsInputFile << " NULL";
							clsTcCommand << " -df " << clsTargetCodec.m_clsFormat << " " << clsOutputFile << " NULL";
						}

						clsTcCommand << " -da " << clsTargetCodec.m_clsACodec;

						// SampleRate
						if (pclsTarget->m_clsContainer.clsAudioCodec.m_clsSampleRate.m_unRealLen > 0)
							clsTcCommand << " " << pclsTarget->m_clsContainer.clsAudioCodec.m_clsSampleRate;
						else
							clsTcCommand << " 0";

						// Channel
						if (pclsTarget->m_clsContainer.clsAudioCodec.m_clsChannel.m_unRealLen > 0)
							clsTcCommand << " " << pclsTarget->m_clsContainer.clsAudioCodec.m_clsChannel;
						else
							clsTcCommand << " 1";

						// BitPerSample
						if (pclsTarget->m_clsContainer.clsAudioCodec.m_clsBitPerSample.m_unRealLen > 0)
							clsTcCommand << " " << pclsTarget->m_clsContainer.clsAudioCodec.m_clsBitPerSample;
						else
							clsTcCommand << " 16";

						// BitRate
						if (pclsTarget->m_clsContainer.clsAudioCodec.m_clsBitRate.m_unRealLen > 0)
							clsTcCommand << " " << pclsTarget->m_clsContainer.clsAudioCodec.m_clsBitRate;
						else
							clsTcCommand << " 0";
					}
					else
					{
						// 예외
						IFLOG(E_LOG_ERR, "Unavailable Transcoding Tool(ONLY FFMPEG/MCONV)");
						pclsTarget->m_eTcSt = E_TRANSCODING_FAILED;
					}
					pclsTarget->m_clsTcCommand = clsTcCommand;
					// tc_codec.json 에서 일치하는 코덱 찾고, Command 만들기 완료
				}
				else
				{
					// bFind == false & No Src/Target ContainerID 등..
					IFLOG(E_LOG_ERR, "Not Supported Codec in SourceContent(%s) & TargetContent(C:%s,A:%s,V:%s)", (KCSTR)clsSourceContainerID, (KCSTR)clsTargetContainerID, (KCSTR)clsTargetAudioCodec, (KCSTR)clsTargetVideoCodec);
					pclsTarget->m_eTcSt = E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE;
				}
				// Audio & Video Codec Map 에서 찾기 실패
			}
			// MSG 이상 없음
		}
		IFLOG(E_LOG_DEBUG, "TargetState(After Making TC Command) : %s", g_fnGetTrssCodeDesc((KCSTR)g_fnGetTrssCode(pclsTarget->m_eTcSt)));

		// pclsTarget
		pclsTarget = (TargetContent*) m_clsTargetList.m_fnGetNext(stTargetItor);
	}

	m_fnCreateTrsgEv(E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_CREATED);
//	m_fnSendTrsgJobChgNotiCreated(m_clsCrtJobReqXml);
}

void Session::m_fnSendTrsgJobChgNotiCreated()
{
	if (m_eSt < E_SESS_ST_TRSG_CREATE_JOB_REQ_RCV)
			return;

	// _rclsXml == CRT_JOB_REQ
	KString clsNotify;	clsNotify.m_fnReSize(BUFF_SIZE);
	m_pclsTrsgTr->m_fnSendNotiCreated(m_clsLastSendXml, clsNotify);
	m_eSt = E_SESS_ST_TRSG_JOB_CHG_NOTI_CREATED_SND;
	m_clsLastSendXml = clsNotify;

	m_fnCreateTrsgEv(E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_WAITING);
//	m_fnSendTrsgJobChgNotiWaiting(clsNotify);
}
void Session::m_fnSendTrsgJobChgNotiWaiting()
{
	if (m_eSt < E_SESS_ST_TRSG_JOB_CHG_NOTI_CREATED_SND)
			return;

	// _rclsXml == JobNotify_Created
	KString clsNotify;	clsNotify.m_fnReSize(BUFF_SIZE);
	m_pclsTrsgTr->m_fnSendNotiWaiting(m_clsLastSendXml, clsNotify);
	m_eSt = E_SESS_ST_TRSG_JOB_CHG_NOTI_WAITING_SND;
	m_clsLastSendXml = clsNotify;

	m_fnCreateTrsgEv(E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED);
//	m_fnSendTrsgJobChgNotiJobStarted(clsNotify);
}
void Session::m_fnSendTrsgJobChgNotiJobStarted()
{
	if (m_eSt < E_SESS_ST_TRSG_JOB_CHG_NOTI_WAITING_SND)
		return;

	// _rclsXml == JobNotify_Waiting
	KString clsNotify;	clsNotify.m_fnReSize(BUFF_SIZE);
	m_pclsTrsgTr->m_fnSendNotiJobStarted(m_clsLastSendXml, m_unTargetContentList, clsNotify);
	m_eSt = E_SESS_ST_TRSG_JOB_CHG_NOTI_JOB_STARTED_SND;
	m_clsLastSendXml = clsNotify;

	// Session Queue Event Put
	AppTrsgEvent *pclsEv = new AppTrsgEvent;
	pclsEv->m_unTid = m_unTid;
	pclsEv->m_clsSessionID = m_clsSessionID;
	pclsEv->m_clsJobID = m_clsJobID;
	pclsEv->m_eT = E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED;
	pclsEv->m_clsXml = m_clsLastSendXml;
	m_fnPutSessionTcEv(pclsEv);
//	m_fnSendTrsgJobChgNotiTcStarted(clsNotify);
}
void Session::m_fnProcTrsgTranscoding()
{
	if(m_eSt < E_SESS_ST_TRSG_JOB_CHG_NOTI_JOB_STARTED_SND)
		return;

	// _rclsXml == JobNotify_JobStarted
	unsigned int unTargetIdx = 1;
	unsigned int unFail = 0;
	unsigned int unSuccess = 0;

	Iterator stTargetItor;
	TargetContent *pclsTarget = (TargetContent*) m_clsTargetList.m_fnGetNext(stTargetItor);
	while (pclsTarget)
	{
		KString clsResultDescription;	clsResultDescription.m_fnReSize(BUFF_SIZE);
		clsResultDescription = "0";

		// TranscodingStarted Event
		AppTrsgTcStartEvent *pclsStartEv = new AppTrsgTcStartEvent;
		pclsStartEv->m_eT = E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED;
		pclsStartEv->m_unTid = m_unTid;
		pclsStartEv->m_clsSessionID = m_clsSessionID;
		pclsStartEv->m_clsJobID = m_clsJobID;
		pclsStartEv->m_pclsTargetContent = pclsTarget;
		pclsStartEv->m_unFail = unFail;
		pclsStartEv->m_unSuccess = unSuccess;
		pclsStartEv->m_unTargetIdx = unTargetIdx;
		Worker::m_fnPutTrsgTcStartEv(pclsStartEv);

		if (m_eSrcSt == E_TC_RES_OK && pclsTarget->m_eTcSt == E_TC_RES_OK)
		{
			pclsTarget->m_eTcSt = m_fnExecuteTranscoding(pclsTarget->m_clsTcCommand, m_pclsSourceContent->m_clsID, pclsTarget, clsResultDescription);
			IFLOG(E_LOG_INFO, "[S:%s][T:%010u][J:%s] TC_RESULT: %s", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, g_fnGetTrssCodeDesc((KCSTR)g_fnGetTrssCode(pclsTarget->m_eTcSt)));

			if(pclsTarget->m_eTcSt == E_TC_RES_OK)
			{
				if(pclsTarget->m_clsContainer.clsAudioCodec.m_clsID == "VOX")
				{
					pclsTarget->m_clsTcType = E_TC_VOX;

					KString clsVoxCommand;
					clsVoxCommand << "vox -b 16 " << pclsTarget->m_clsTempFile << " " << pclsTarget->m_clsOutputFile;

					pclsTarget->m_eTcSt = m_fnExecuteTranscoding(clsVoxCommand, m_pclsSourceContent->m_clsID, pclsTarget, clsResultDescription);
					IFLOG(E_LOG_INFO, "[S:%s][T:%010u][J:%s] TC_RESULT : %s", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, g_fnGetTrssCodeDesc((KCSTR)g_fnGetTrssCode(pclsTarget->m_eTcSt)));
				}
			}
		}

		// Final Command Result
		if (m_eSrcSt == E_TC_RES_OK)
		{
			if (pclsTarget->m_eTcSt == E_TC_RES_OK)
				unSuccess++;
			else
				unFail++;
		}
		else
		{
			unFail++;
			if(pclsTarget->m_eTcSt == E_TC_RES_OK)
				pclsTarget->m_eTcSt = m_eSrcSt;
		}

		// TranscodingStopped Event Worker Put
		AppTrsgTcStopEvent *pclsStopEv = new AppTrsgTcStopEvent;
		pclsStopEv->m_eT = E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED;
		pclsStopEv->m_unTid = m_unTid;
		pclsStopEv->m_clsSessionID = m_clsSessionID;
		pclsStopEv->m_clsJobID = m_clsJobID;
		pclsStopEv->m_unFail = unFail;
		pclsStopEv->m_unSuccess = unSuccess;
		pclsStopEv->m_eTcSt = pclsTarget->m_eTcSt;
		pclsStopEv->m_clsResultDescription = clsResultDescription;
		Worker::m_fnPutTrsgTcStopEv(pclsStopEv);
//		m_fnSendTrsgJobChgNotiTcStopped(clsNotify, unFail, unSuccess, pclsTarget->m_eTcSt, clsResultDescription);

		unTargetIdx++;
		pclsTarget = (TargetContent*) m_clsTargetList.m_fnGetNext(stTargetItor);
	}
}
void Session::m_fnSendTrsgJobChgNotiTcStarted(TargetContent* _pclsTarget, unsigned int _unFail, unsigned int _unSuccess, unsigned int _unTargetIdx)
{
	if (m_eSt < E_SESS_ST_TRSG_JOB_CHG_NOTI_JOB_STARTED_SND)
		return;
	KString clsNotify;	clsNotify.m_fnReSize(BUFF_SIZE);
	m_pclsTrsgTr->m_fnSendNotiTcStarted(m_clsLastSendXml, m_pclsSourceContent, _pclsTarget, _unFail, _unSuccess, _unTargetIdx, clsNotify);
	m_eSt = E_SESS_ST_TRSG_JOB_CHG_NOTI_TC_STARTED_SND;
	m_clsLastSendXml = clsNotify;
}
void Session::m_fnSendTrsgJobChgNotiTcStopped(unsigned int _unFail, unsigned int _unSuccess, ETrssCodeSet_t _eSt, KString &_rclsResultDescription)
{
	if (m_eSt < E_SESS_ST_TRSG_JOB_CHG_NOTI_TC_STARTED_SND)
		return;

	// _rclsXml == JobNotify_TCStart
	KString clsNotify;	clsNotify.m_fnReSize(BUFF_SIZE);
	m_pclsTrsgTr->m_fnSendNotiTcStopped(m_clsLastSendXml, _unFail, _unSuccess, _eSt, _rclsResultDescription, clsNotify);
	m_eSt = E_SESS_ST_TRSG_JOB_CHG_NOTI_TC_STOPPED_SND;
	m_clsLastSendXml = clsNotify;

   // TranscodingList와 TargetContent 갯수 비교
   if(m_unTargetContentList != m_unTranscodingList)
      IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] Different Target Count! [TargetContent:%d / TranscodingList:%d]", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, m_unTargetContentList, m_unTranscodingList);

   if ((_unFail + _unSuccess) == m_unTargetContentList)
   {
      IFLOG(E_LOG_INFO, "[S:%s][T:%010u][J:%s] Target Transcoding Completed! [Current:%d, Total:%d]", (KCSTR)m_clsSessionID, m_unTid, (KCSTR)m_clsJobID, (_unFail + _unSuccess), m_unTargetContentList);
      m_fnCreateTrsgEv(E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED);
//      m_fnSendTrsgJobChgNotiJobStopped(clsNotify);
   }
}
void Session::m_fnSendTrsgJobChgNotiJobStopped()
{
	if (m_eSt < E_SESS_ST_TRSG_JOB_CHG_NOTI_TC_STOPPED_SND)
		return;

	// _rclsXml == JobNotify_TCStop
	KString clsNotify;	clsNotify.m_fnReSize(BUFF_SIZE);
	m_pclsTrsgTr->m_fnSendNotiJobStopped(m_clsLastSendXml, clsNotify);
	m_eSt = E_SESS_ST_TRSG_JOB_CHG_NOTI_JOB_STOPPED_SND;
	m_clsLastSendXml = clsNotify;

	m_fnCreateTrsgEv(E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED);
//	m_fnSendTrsgJobChgNotiJobDestroyed(clsNotify);
}
void Session::m_fnSendTrsgJobChgNotiJobDestroyed()
{
	// _rclsXml == JobNotify_JobStop
	KString clsNotify;	clsNotify.m_fnReSize(BUFF_SIZE);
	m_pclsTrsgTr->m_fnSendNotiJobDestroy(m_clsLastSendXml, m_unTargetContentList, E_TC_RES_OK, clsNotify);
	m_eSt = E_SESS_ST_TRSG_JOB_CHG_NOTI_JOB_DESTROYED_SND;
	m_clsLastSendXml = clsNotify;

	m_fnCreateSessionEndEv();
}
KString Session::m_fnGetFileName()
{
	KString clsFileName;
	clsFileName.m_fnReSize(BUFF_SIZE);

	time_t stTime;
	struct tm tmTime;
	stTime = time(NULL);
	localtime_r(&stTime, &tmTime);

	struct timespec now_monotonic;
	clock_gettime(CLOCK_REALTIME, &now_monotonic);

	KString::m_fnStrnCat((KSTR) clsFileName, clsFileName.m_unLen,
			"%04d%02d%02d/%02d/%02d/%02d/%04d%02d%02d%02d%02d%02d%08lld_encode.tmp",
			tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
			tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec,
			tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
			tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec, now_monotonic.tv_nsec / 10);

	// YYYYMMDD/HH/MM/SS/
	KString clsDir;
	KString::m_fnStrnCat((KSTR) clsDir, clsDir.m_unLen, "%s/%04d%02d%02d/%02d/%02d/%02d", (KCSTR)MainConfig::m_fnGetInstance()->m_clsNasInternal,
	         tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);

	CfgFile::m_fnCreateDir((KCSTR)clsDir);

	return clsFileName;
}

void* Session::m_fnGetWorker()
{
	SessionMgr *pclsMgr = (SessionMgr*) m_pclsOwner;
	return pclsMgr->m_pclsOwner;
}

ETrssCodeSet_t Session::m_fnExecuteTranscoding(KString &_rclsTcCommand, KString &_rclsSourceContainerID, TargetContent* _pclsTarget, KString & _rclsResultDescription)
{
	MainConfig *pclsConf = MainConfig::m_fnGetInstance();
	KString clsTcBinary;
	clsTcBinary.m_fnReSize(BUFF_SIZE);

	clsTcBinary << "python3 commandTimeout.py " << pclsConf->m_unCmdTimeout << " " << (KCSTR) _rclsTcCommand << " 2>&1";
	IFLOG(E_LOG_INFO, "[S:%s][T:%010u][J:%s]Execute Command: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KSTR) clsTcBinary);

	KString clsBuff;
	clsBuff.m_fnReSize(BUFF_SIZE);
	KString clsSize;
	clsSize.m_fnReSize(BUFF_SIZE);
	KString clsTime;
	clsTime.m_fnReSize(BUFF_SIZE);

	// 명령어 시작시간 설정
	double timeDiffMs = 0.0;
	struct timespec stTimespec, endTimespec;
	clock_gettime(CLOCK_MONOTONIC, &stTimespec);

	FILE *fp;
	fp = popen((KCSTR) clsTcBinary, "r");
	if (fp == NULL)
	{
		IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s]Execute Command(%s) FAILED", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KSTR) clsTcBinary);
		pclose(fp);
		return E_TRANSCODING_FAILED;
	}

	ETcType_t eTcType = _pclsTarget->m_clsTcType;
	if (eTcType == E_TC_FFMPEG)
	{
		while (fgets((KSTR) clsBuff, BUFF_SIZE, fp))
		{
			// Size > 0 면 변환성공
			if (KString::m_fnStrStr((KSTR) clsBuff, "WTRSP_FileSize:") != NULL)
			{
				clsSize = KString::m_fnSkipString((KCSTR) clsBuff, "WTRSP_FileSize:");
				IFLOG(E_LOG_INFO, "[S:%s][T:%010u][J:%s] %s ResultFileSize: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KCSTR) clsSize);

				clsBuff.m_fnReSize(BUFF_SIZE);
				continue;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "WTRSP_PlayTime:") != NULL)
			{
				clsTime = KString::m_fnSkipString((KCSTR) clsBuff, "WTRSP_PlayTime:");
				_rclsResultDescription = clsTime;
				IFLOG(E_LOG_INFO, "[S:%s][T:%010u][J:%s] %s ResultPlayTime: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KCSTR) clsTime);
         
				clsBuff.m_fnReSize(BUFF_SIZE);
				break;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Unknown encoder") != NULL) // FFMPEG 실패, 마지막 줄 Return 값에 따라서 처리
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_TRANSCODING_FAILED;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Unknown decoder") != NULL)
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_TRANSCODING_FAILED;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "No such file or directory") != NULL)
			{	// Output 경로 없는 경우
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_NAS_ACCESS_FAILED;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Automatic encoder selection failed for output stream #0:0.") != NULL)
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_TRANSCODING_FAILED;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Unsupported") != NULL)
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_TRANSCODING_FAILED;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Conversion failed!") != NULL)
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_PARAMETER_INVALID;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Is a directory") != NULL)
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_NAS_ACCESS_FAILED;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "received signal") != NULL)
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: Received Signal Command TimeOut", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType));
				clsSize.m_fnReSize(BUFF_SIZE);
				clsTime.m_fnReSize(BUFF_SIZE);
				_rclsResultDescription = "0";

				pclose(fp);
				return E_TRANSCODING_FAILED; // Timeout
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Permission denied") != NULL)
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_PERMISSON_FAILED", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType));

				pclose(fp);
				return E_TRANSCODING_FAILED; // Timeout
			}
			else
			{
				clsBuff.m_fnReSize(BUFF_SIZE);
				continue;
			}
			clsBuff.m_fnReSize(BUFF_SIZE);
		}

		if (KString::m_fnAtoi((KCSTR) clsSize) <= 0)
		{
			IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: ResultFile is Empty", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType));

			pclose(fp);
			return E_TRANSCODING_FAILED;
		}
		else
		{
			pclose(fp);
			// 변환 성공 CASE
			// 명령어 종료시간 설정
			clock_gettime(CLOCK_MONOTONIC, &endTimespec);
			timeDiffMs = (endTimespec.tv_sec - stTimespec.tv_sec) * 1000 + (endTimespec.tv_nsec - stTimespec.tv_nsec) / 1000000.0;
			IFLOG(E_LOG_INFO, "[S:%s][T:%010u][J:%s] %s_TC_SUCCESS Src:%s->Tgt:%s ExecutionTime:%.3lf(ms), ResultPlayTime:%d", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KCSTR) _rclsSourceContainerID,
					(KCSTR) _pclsTarget->m_clsContainer.m_clsID, timeDiffMs, (KUINT) _rclsResultDescription);

			KString clsChgMod = "chmod 777 ";
			// FCONV로 변환하는 VOX는 중간파일로 TempFile 변경
			if (_pclsTarget->m_clsContainer.m_clsID == "VOX" && _pclsTarget->m_clsTempFile.m_unRealLen > 0)
				clsChgMod << (KCSTR) _pclsTarget->m_clsTempFile;
			else
				clsChgMod << (KCSTR) _pclsTarget->m_clsOutputFile;
			IFLOG(E_LOG_DEBUG, "ChangeMod Command: %s", (KCSTR) clsChgMod);
			std::system((KCSTR) clsChgMod);

			return E_TC_RES_OK;
		}
	}
	else	// MCONV, VOX
	{
		while (fgets((KSTR) clsBuff, BUFF_SIZE, fp))
		{
			// 변환된 음원길이
			if (KString::m_fnStrStr((KSTR) clsBuff, "WTRSP_PlayTime:") != NULL)
			{
				clsTime = KString::m_fnSkipString((KCSTR) clsBuff, "WTRSP_PlayTime:");
				_rclsResultDescription = clsTime;
			}
			// Timeout
			else if (KString::m_fnStrStr((KSTR) clsBuff, "received signal") != NULL)
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: Received Signal Command TimeOut", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType));
				clsSize.m_fnReSize(BUFF_SIZE);
				clsTime.m_fnReSize(BUFF_SIZE);
				_rclsResultDescription = "0";

				pclose(fp);
				return E_TRANSCODING_FAILED;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Code 0,") != NULL)
			{
				pclose(fp);
				// 명령어 종료시간 설정
				clock_gettime(CLOCK_MONOTONIC, &endTimespec);
				timeDiffMs = (endTimespec.tv_sec - stTimespec.tv_sec) * 1000 + (endTimespec.tv_nsec - stTimespec.tv_nsec) / 1000000.0;
				IFLOG(E_LOG_INFO, "[S:%s][T:%010u][J:%s] %s_TC_SUCCESS Src:%s->Tgt:%s ExecutionTime:%.3lf(ms), ResultPlayTime:%d", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType),
						(KCSTR) _rclsSourceContainerID, (KCSTR) _pclsTarget->m_clsContainer.m_clsID, timeDiffMs, (KUINT) _rclsResultDescription);

				KString clsChgMod = "chmod 777 ";
				clsChgMod << (KCSTR) _pclsTarget->m_clsOutputFile;
				IFLOG(E_LOG_DEBUG, "ChangeMod Command: %s", (KCSTR) clsChgMod);
				std::system((KCSTR) clsChgMod);

				return E_TC_RES_OK;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Code 50,") != NULL)
			{
				// invalid License
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_LICENSE_KEY_INVALID;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Code 100,") != NULL)
			{
				// Internal Error
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_TRANSCODING_FAILED;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Code 101,") != NULL)
			{
				//internal init error
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_TRANSCODING_FAILED;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Code 102,") != NULL)
			{
				// internal file open error
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_TRANSCODING_FAILED;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Code 200,") != NULL)
			{
				//tc invalid argument
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_PARAMETER_INVALID;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Code 201,") != NULL)
			{
				// invalid argument file same
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_TC_FAILED: %s", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType), (KSTR) clsBuff);

				pclose(fp);
				return E_TRANSCODING_FAILED;
			}
			else if (KString::m_fnStrStr((KSTR) clsBuff, "Permission denied") != NULL)
			{
				IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] %s_PERMISSON_FAILED", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID, (KCSTR) g_fnGetTranscodingType(eTcType));

				pclose(fp);
				return E_TRANSCODING_FAILED; // Timeout
			}
			else
			{
				clsBuff.m_fnReSize(BUFF_SIZE);
				continue;
			}
			clsBuff.m_fnReSize(BUFF_SIZE);
		}
		IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] TC_FAILED: UNKNOWN", (KCSTR) m_clsSessionID, m_unTid, (KCSTR) m_clsJobID);

		pclose(fp);
		return E_TRANSCODING_FAILED;
	}
}
void Session::m_fnRecvSessionTimeOut()
{
	// 로직 중간에 멈추거나 Notify 보내는 중 등.. SessionTimer로부터 TimeOut 받았을때
	if (E_SESS_ST_TRSG_CREATE_JOB_REQ_RCV <= m_eSt && m_eSt <= E_SESS_ST_TRSG_JOB_CHG_NOTI_JOB_STOPPED_SND)
	{
		KString clsNotify;
		clsNotify.m_fnReSize(BUFF_SIZE);
		// m_clsLastSendXml 마지막으로 보낸 메시지
		IFLOG(E_LOG_ERR, "[S:%s][T:%010u][J:%s] Session Timeout(E_JOB_TRANSCODE_TIMEOUT)", (KSTR) m_clsSessionID, m_unTid, (KSTR) m_clsJobID);
		m_pclsTrsgTr->m_fnSendNotiJobDestroy(m_clsLastSendXml, m_unTargetContentList, E_JOB_TRANSCODE_TIMEOUT, clsNotify);
		// Session 만료
		m_fnCreateSessionEndEv();
	}
}
void Session::m_fnCreateSessionEndEv()
{
	AppSessionEndEvent *pclsEv = new AppSessionEndEvent;
	pclsEv->m_unTid = m_unTid;
	pclsEv->m_clsSessionID = m_clsSessionID;
	pclsEv->m_clsJobID = m_clsJobID;
	pclsEv->m_eT = E_MAIN_EV_SESSION_END;
	Worker::m_fnPutSessionEndEv(pclsEv);
}
void Session::m_fnCreateTrsgEv(EAppEvent_t _eT)
{
	AppTrsgEvent *pclsEv = new AppTrsgEvent;
	pclsEv->m_unTid = m_unTid;
	pclsEv->m_clsSessionID = m_clsSessionID;
	pclsEv->m_clsJobID = m_clsJobID;
	pclsEv->m_eT = _eT;
	pclsEv->m_clsXml = m_clsLastSendXml;
	Worker::m_fnPutTrsgEv(pclsEv);
}
void Session::m_fnPutSessionTcEv(AppTrsgEvent *_pclsEv)
{
	unsigned int unTid = (KUINT)_pclsEv->m_unTid;
	if(unTid == 0)
		delete _pclsEv;
	else
	{
		if(m_pclsQ->put(_pclsEv) == false)
			delete _pclsEv;
	}
}
void Session::m_fnCbkProcess(AppBaseEvent *_pclsEv, void *_pvOwner)
{
	Session * pclsMy = (Session*)_pvOwner;


	if(_pclsEv->m_eT == E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED)
	{
		pclsMy->m_fnProcTrsgTranscoding();
	}

	delete _pclsEv;
}
