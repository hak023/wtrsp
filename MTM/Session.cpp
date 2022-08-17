#include <cstdlib>
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
		IFLOG(E_LOG_ERR, "QueueThread shutdown");
		m_pclsQ->shutdown();
		IFLOG(E_LOG_ERR, "QueueThread join");
		m_pclsQ->join();
		IFLOG(E_LOG_ERR, "QueueThread delete");
		delete m_pclsQ;
		IFLOG(E_LOG_ERR, "QueueThread end");
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

	Worker::m_pWorkerIdMgr->releaseId(m_nAllocWorker);
}
void Session::m_fnRecvTrsgCrtJobReq(KString &_rclsXml)
{
	/*
	 * Todo List when received CreateJobRequest
	 * 1. TargetContent Count
	 * 2. SourceContent
	 * 3. TargetContent
	 * 4. InputFile
	 * 5. Metadata
	 * 6. OutputFile
	 * 7. Codec Find
	 * 8. Command
	 * 9. Send "Created"
	 *
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
		// 내부NAS에 위치한 SourceFile
		KString clsPath = (KCSTR) pclsConf->m_clsNasInternal;
		m_clsInputFile.m_fnCat((KCSTR) clsPath);
		if (((KCSTR) clsPath)[clsPath.m_unRealLen - 1] != '/')
			m_clsInputFile.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.
		m_clsInputFile.m_fnCat((KCSTR) m_pclsSourceContent->m_clsBinaryData);

		if (access((KCSTR)m_clsInputFile, F_OK) < 0)
		{
			// 파일 존재하지 않음
			IFLOG(E_LOG_ERR, "InputFile[%s] is not Exist.", (KCSTR)m_clsInputFile);
			m_eSrcSt = E_NAS_ACCESS_FAILED; // 잘못된 파일(NAS)경로, 존재하지 않음, 접근경로실패
		}
	}
	else
	{
		if (m_pclsSourceContent->m_clsNasCode.m_unRealLen > 0)
		{
			pclsNasSystem->m_fnGetSourceDir(m_pclsSourceContent->m_clsNasCode, m_clsInputFile);
			if (m_clsInputFile == "UNKNOWN")
			{
				IFLOG(E_LOG_ERR, "Invalid SourceContent NAS CODE[%s]", (KCSTR)m_pclsSourceContent->m_clsNasCode);
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
					IFLOG(E_LOG_ERR, "InputFile[%s] is not Exist.", (KCSTR)m_clsInputFile);
					m_eSrcSt = E_NAS_ACCESS_FAILED; // 잘못된 파일(NAS)경로, 존재하지 않음, 접근경로실패
				}
			}
		}
		else
		{
			IFLOG(E_LOG_ERR, "Empty SourceContent NAS CODE[%s]", (KCSTR)m_pclsSourceContent->m_clsNasCode);
			m_eSrcSt = E_NO_SRC_NAS_CODE;
		}
	}
	IFLOG(E_LOG_INFO, "Input FilePath[%s]", (KCSTR)m_clsInputFile);

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
			IFLOG(E_LOG_ERR, "Metadata Image[%s] is File Not Exist", (KCSTR) m_pclsSourceContent->m_clsMetadata.m_clsImageData);
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
			clsCommand << "-metadata www=\"\" -metadata Comment=\"\" -metadata Encodedby=\"\" ";

			m_clsMetaCommand = clsCommand;
		}
		IFLOG(E_LOG_INFO, "Metadata ImageData FilePath[%s] & Metadata Command[%s]", (KCSTR)m_clsMetaImage, (KCSTR)m_clsMetaCommand);
	}
	IFLOG(E_LOG_DEBUG, "SourceState(After Received CrtJobReq) : %s", g_fnGetTrssCode(m_eSrcSt));

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
			KString clsFileName = m_fnGetFileName();
			pclsTarget->m_clsBinaryData = clsFileName;

			KString clsPath = (KCSTR) pclsConf->m_clsNasInternal;
			clsOutputFile.m_fnCat((KCSTR) clsPath);
			if (((KCSTR) clsPath)[clsPath.m_unRealLen - 1] != '/')
				clsOutputFile.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.

			// Output Directory 확인
			if(access((KCSTR)clsOutputFile, F_OK) < 0)
			{
				IFLOG(E_LOG_ERR, "No Such Directory[%s]", (KCSTR)clsOutputFile);
				pclsTarget->m_eTcSt = E_NAS_ACCESS_FAILED;
			}

			clsOutputFile.m_fnCat((KCSTR) clsFileName);
			pclsTarget->m_clsOutputFile = clsOutputFile;
		}
		else
		{
			// External NAS
			if (m_pclsSourceContent->m_clsNasCode.m_unRealLen > 0)
			{
				pclsNasSystem->m_fnGetSourceDir(pclsTarget->m_clsNasCode, clsOutputFile);
				if (clsOutputFile == "UNKNOWN")
				{
					IFLOG(E_LOG_ERR, "Invalid TargetContent NAS CODE[%s]", (KCSTR)pclsTarget->m_clsNasCode);
					pclsTarget->m_eTcSt = E_NAS_ACCESS_FAILED;
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
						IFLOG(E_LOG_ERR, "No Such Directory[%s]", (KCSTR)clsOutputDir);
						pclsTarget->m_eTcSt = E_NAS_ACCESS_FAILED;
					}

					clsOutputFile.m_fnCat((KCSTR) pclsTarget->m_clsFilePath);
					pclsTarget->m_clsOutputFile = clsOutputFile;
				}
			}
			else
			{
				IFLOG(E_LOG_ERR, "Empty TargetContent NAS CODE[%s]", (KCSTR)pclsTarget->m_clsNasCode);
				pclsTarget->m_eTcSt = E_NO_TARGET_NAS_CODE;
			}
		}
		IFLOG(E_LOG_INFO, "Output FilePath[%s]", (KCSTR)pclsTarget->m_clsOutputFile);
		IFLOG(E_LOG_DEBUG, "TargetState(After Making OutputFile) : %s", g_fnGetTrssCode(pclsTarget->m_eTcSt));

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

			IFLOG(E_LOG_DEBUG, "SourceContainerID:[%s], TargetContainerID:[%s], VideoCodec:[%s], AudioCodec:[%s]", (KSTR)clsSourceContainerID, (KSTR)clsTargetContainerID, (KSTR)clsTargetVideoCodec, (KSTR)clsTargetAudioCodec);

			// Font 변환 지원하지 않음
			if(clsSourceContainerID == "MDF" || clsSourceContainerID == "UTF" || clsTargetContainerID == "MDF" || clsTargetContainerID == "UTF" )
				pclsTarget->m_eTcSt = E_MEDIA_ENCODING_ERROR;

			// 1. Audio / Video Codec Each One Check
			pclsTarget->m_eTcSt = pclsConf->m_fnChkTargetCodec(clsTargetAudioCodec, clsTargetVideoCodec);
			IFLOG(E_LOG_DEBUG, "TargetState(After First Finding A/VCodec) : %s", g_fnGetTrssCode(pclsTarget->m_eTcSt));
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
					IFLOG(E_LOG_DEBUG, "Get TargetCodec : %s", (KSTR ) clsDebug);

					// Transcoding Command
					KString clsTcCommand;	clsTcCommand.m_fnReSize(BUFF_SIZE);
					if (clsTargetCodec.m_clsTool == "ffmpeg")
					{
						pclsTarget->m_clsTcType = E_TC_FFMPEG;

						clsTcCommand << clsTargetCodec.m_clsTool;
						clsTcCommand << " -i " << m_clsInputFile;

						if (clsTargetCodec.m_clsType == "Image")
						{
							if (pclsTarget->m_clsContainer.clsVideoCodec.m_clsWidth.m_unRealLen > 0 && pclsTarget->m_clsContainer.clsVideoCodec.m_clsHeight.m_unRealLen > 0)
								clsTcCommand << " -vf scale=" << pclsTarget->m_clsContainer.clsVideoCodec.m_clsWidth << ":" << pclsTarget->m_clsContainer.clsVideoCodec.m_clsHeight;
							else
								clsTcCommand.m_fnReSize(BUFF_SIZE);
						}
						else
						{
							// MetaImage Data -i file -map 0:0
							if ((KBOOL) clsTargetCodec.m_clsMetadata && m_clsMetaImage.m_unRealLen > 0)
								clsTcCommand << " " << m_clsMetaImage;

							// Format
							clsTcCommand << " -f " << clsTargetCodec.m_clsFormat;
							clsTcCommand << " -acodec " << clsTargetCodec.m_clsACodec;

							if (pclsTarget->m_clsContainer.clsAudioCodec.m_clsChannel.m_unRealLen > 0)
								clsTcCommand << " -ac " << pclsTarget->m_clsContainer.clsAudioCodec.m_clsChannel;

							if (pclsTarget->m_clsContainer.clsAudioCodec.m_clsSampleRate.m_unRealLen > 0)
								clsTcCommand << " -ar " << pclsTarget->m_clsContainer.clsAudioCodec.m_clsSampleRate;

							if (pclsTarget->m_clsContainer.clsAudioCodec.m_clsBitRate.m_unRealLen > 0)
								clsTcCommand << " -ab " << pclsTarget->m_clsContainer.clsAudioCodec.m_clsBitRate;

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

						}
						// -metadata album="" ...
						if ((KBOOL) clsTargetCodec.m_clsMetadata && m_clsMetaCommand.m_unRealLen > 0)
							clsTcCommand << " " << m_clsMetaCommand;

						// -y(overwrite) outputfile path
						clsTcCommand << " -y " << clsOutputFile;

						if (clsTargetCodec.m_clsEtc.m_unRealLen > 0)
							clsTcCommand << " " << clsTargetCodec.m_clsEtc;

					}
					else if (clsTargetCodec.m_clsTool == "MCONV")
					{
						// MCONV
						pclsTarget->m_clsTcType = E_TC_MCONV;

						clsTcCommand << clsTargetCodec.m_clsTool;

						if (clsSourceContainerID == "K3G")
							clsSourceContainerID = "3GP";
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
				}	// bFind == true
				else
				{
					// bFind == false
					IFLOG(E_LOG_ERR, "Not Supported Codec in SourceContent(%s) & TargetContent(C:%s,A:%s,V:%s)", (KCSTR)clsSourceContainerID, (KCSTR)clsTargetContainerID, (KCSTR)clsTargetAudioCodec, (KCSTR)clsTargetVideoCodec);
					pclsTarget->m_eTcSt = E_TRANSCODING_FAILED;
				}
				// Audio & Video Codec Map 에서 찾기 실패
			}
			// MSG 이상 없음
		}
		IFLOG(E_LOG_DEBUG, "TargetState(After Making TC Command) : %s", g_fnGetTrssCode(pclsTarget->m_eTcSt));

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
		// TC_STOPPED 에서 사용될 변환된 음원 길이
		KString clsResultDescription;	clsResultDescription.m_fnReSize(BUFF_SIZE);
		clsResultDescription = "0";

		// 이벤트 생성
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

//		KString clsNotify;	clsNotify.m_fnReSize(BUFF_SIZE);
//		m_pclsTrsgTr->m_fnSendNotiTcStarted(_rclsXml, m_pclsSourceContent, pclsTarget, unFail, unSuccess, unTargetIdx, clsNotify);
//		m_eSt = E_SESS_ST_TRSG_JOB_CHG_NOTI_TC_STARTED_SND;
//		m_clsLastSendXml = clsNotify;

		if (m_eSrcSt == E_TC_RES_OK && pclsTarget->m_eTcSt == E_TC_RES_OK)
		{
			pclsTarget->m_eTcSt = m_fnExecuteTranscoding(pclsTarget->m_clsTcCommand, pclsTarget->m_clsTcType, clsResultDescription);
			IFLOG(E_LOG_INFO, "First TC_RESULT: %s", g_fnGetTrssCode(pclsTarget->m_eTcSt));

			if(pclsTarget->m_eTcSt == E_TC_RES_OK)
			{
				// 변환 성공
				// PlayTime Get
				IFLOG(E_LOG_INFO, "Transcoding Result File PlayTime : %s", (KSTR)clsResultDescription);

				if(pclsTarget->m_clsContainer.clsAudioCodec.m_clsID == "VOX")
				{
					pclsTarget->m_clsTcType = E_TC_VOX;

					KString clsVoxCommand;
					clsVoxCommand << "vox -b 16 " << pclsTarget->m_clsOutputFile << " " << pclsTarget->m_clsOutputFile;

					pclsTarget->m_eTcSt = m_fnExecuteTranscoding(clsVoxCommand, pclsTarget->m_clsTcType, clsResultDescription);
					IFLOG(E_LOG_INFO, "Second TC_RESULT: %s", g_fnGetTrssCode(pclsTarget->m_eTcSt));
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
		IFLOG(E_LOG_INFO, "TC_RES:%s, ResultDescription:%s", g_fnGetTrssCode(pclsTarget->m_eTcSt), (KSTR)clsResultDescription);

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
		// Target 기준
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
      IFLOG(E_LOG_ERR, "Different Target Count! [TargetContent:%d / TranscodingList:%d]", m_unTargetContentList, m_unTranscodingList);

   if ((_unFail + _unSuccess) == m_unTargetContentList)
   {
      IFLOG(E_LOG_ERR, "Target Transcoding Completed! [Current:%d, Total:%d]", (_unFail + _unSuccess), m_unTargetContentList);
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

	// Session 만료
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

	KString::m_fnStrnCat((KSTR) clsFileName, clsFileName.m_unLen, "%04d%02d%02d%02d%02d%02d%08lld_encode.tmp", tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec, now_monotonic.tv_nsec / 10);

	return clsFileName;
}
void* Session::m_fnGetWorker()
{
	SessionMgr *pclsMgr = (SessionMgr*) m_pclsOwner;
	return pclsMgr->m_pclsOwner;
}
ETrssCodeSet_t Session::m_fnExecuteTranscoding(KString &_rclsTcCommand, ETcType_t _eTcType, KString & _rclsResultDescription)
{
	FILE *fp;
	int stTime = time(NULL);
	MainConfig *pclsConf = MainConfig::m_fnGetInstance();

	// 명령어 만들기
	KString clsTcBinary; clsTcBinary.m_fnReSize(BUFF_SIZE);
	clsTcBinary << "timeout " << pclsConf->m_unCmdTimeout << "s ";
	clsTcBinary.m_fnCat((KCSTR)_rclsTcCommand);
	clsTcBinary << " 2>&1 &";

	IFLOG(E_LOG_INFO, "Execute Command[%s]", (KSTR)clsTcBinary);

	KString clsBuff; clsBuff.m_fnReSize(BUFF_SIZE);
	KString clsSize; clsSize.m_fnReSize(BUFF_SIZE);
	KString clsTime; clsTime.m_fnReSize(BUFF_SIZE);
	fp = popen((KCSTR)clsTcBinary, "r");
	if(fp == NULL)
	{
		IFLOG(E_LOG_ERR, "m_fnExecuteTranscoding popen() fail");
		pclose(fp);
		return E_TRANSCODING_FAILED;
	}

	if(_eTcType == E_TC_FFMPEG)
	{
		while(fgets((KSTR)clsBuff, BUFF_SIZE, fp))
		{
			// Size 가져오기 (성공/실패 판단여부)
			if(KString::m_fnStrStr((KSTR)clsBuff, "WTRSP_FileSize:") != NULL)
			{
				clsSize = KString::m_fnSkipString((KCSTR)clsBuff, "WTRSP_FileSize:");

				if(KString::m_fnAtoi((KCSTR)clsSize) <= 0)
				{
					pclose(fp);
					return E_TRANSCODING_FAILED;   //Failed
				}
				else
				{
					clsBuff.m_fnReSize(BUFF_SIZE);
					continue;
				}
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Unknown encoder") != NULL)// FFMPEG 실패, 마지막 줄 Return 값에 따라서 처리
			{
				pclose(fp);
//				return E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE;
				return E_TRANSCODING_FAILED;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Unknown decoder") != NULL)
			{
				pclose(fp);
//				return E_NOT_SUPPORT_AUDIO_DECODEC_TYPE;
				return E_TRANSCODING_FAILED;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "No such file or directory") != NULL)
			{	// Output 경로 없는 경우
				pclose(fp);
				return E_NAS_ACCESS_FAILED;
			}
			// FFMPEG 오류 항목들 체크 (metadata map 0:0 명령에서 에러, metadata:true 넣기 전 )
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Automatic encoder selection failed for output stream #0:0.") != NULL)
			{
				pclose(fp);
				return E_TRANSCODING_FAILED;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Unsupported") != NULL)
			{
				pclose(fp);
				return E_TRANSCODING_FAILED;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Conversion failed!") != NULL) //bitrate too low 등.. 포함
			{
				pclose(fp);
				return E_PARAMETER_INVALID;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Is a directory") != NULL) //outputFile (파일이름 없을때)
			{
				pclose(fp);
				return E_NAS_ACCESS_FAILED;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "WTRSP_PlayTime:") != NULL && KString::m_fnAtoi((KCSTR)clsSize) > 0)
			{
				clsTime = KString::m_fnSkipString((KCSTR)clsBuff, "WTRSP_PlayTime:");
				_rclsResultDescription = clsTime;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "received signal") != NULL)
			{
				IFLOG(E_LOG_ERR, "Received Signal: Command TimeOut");
				clsSize.m_fnReSize(BUFF_SIZE);
				clsTime.m_fnReSize(BUFF_SIZE);
				_rclsResultDescription = "0";
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
		int endTime = time(NULL);
		// 명령어 실행시간
		IFLOG(E_LOG_INFO, "Command duration=%d\n", endTime - stTime);
		if(KString::m_fnAtoi((KCSTR)clsSize) <= 0)
			return E_TRANSCODING_FAILED;   //Failed
		else
			return E_TC_RES_OK;
	}
	else	// MCONV, VOX
	{
		while(fgets((KSTR)clsBuff, BUFF_SIZE, fp))
		{
			// 변환된 음원길이
			if(KString::m_fnStrStr((KSTR)clsBuff, "WTRSP_PlayTime:") != NULL)
			{
				clsTime = KString::m_fnSkipString((KCSTR)clsBuff, "WTRSP_PlayTime:");
				_rclsResultDescription = clsTime;
			}
			// Timeout
			else if(KString::m_fnStrStr((KSTR)clsBuff, "received signal") != NULL)
			{
				IFLOG(E_LOG_ERR, "Received Signal: Command TimeOut");
				clsSize.m_fnReSize(BUFF_SIZE);
				clsTime.m_fnReSize(BUFF_SIZE);
				_rclsResultDescription = "0";
				pclose(fp);
				return E_TRANSCODING_FAILED; // Timeout
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Code 0,") != NULL)
			{
				pclose(fp);
				return E_TC_RES_OK;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Code 50,") != NULL)
			{
				pclose(fp);
				// invalid License
				return E_LICENSE_KEY_INVALID;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Code 100,") != NULL)
			{
				pclose(fp);
				// Internal Error
				return E_TRANSCODING_FAILED;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Code 101,") != NULL)
			{
				pclose(fp);
				//internal init error
				return E_TRANSCODING_FAILED;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Code 102,") != NULL)
			{
				pclose(fp);
				// internal file open error
				return E_TRANSCODING_FAILED;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Code 200,") != NULL)
			{
				pclose(fp);
				//tc invalid argument
				return E_PARAMETER_INVALID;
			}
			else if(KString::m_fnStrStr((KSTR)clsBuff, "Code 201,") != NULL)
			{
				pclose(fp);
				// invalid argument file same
				return E_TRANSCODING_FAILED;
			}
			else
			{
				clsBuff.m_fnReSize(BUFF_SIZE);
				continue;
			}
			clsBuff.m_fnReSize(BUFF_SIZE);
		}
		int endTime = time(NULL);
		// 명령어 실행시간
		IFLOG(E_LOG_INFO, "Command Execution Time=%d\n", endTime - stTime);
		return E_TRANSCODING_FAILED;
	}
}
void Session::m_fnRecvSessionTimeOut()
{
	// 로직 중간에 멈추거나 Notify 보내는 중 등.. SessionTimer로부터 TimeOut 받았을때
	if (E_SESS_ST_TRSG_CREATE_JOB_REQ_RCV <= m_eSt && m_eSt <= E_SESS_ST_TRSG_JOB_CHG_NOTI_JOB_STOPPED_SND)
	{
		KString clsNotify;	clsNotify.m_fnReSize(BUFF_SIZE);
		// m_clsLastSendXml 마지막으로 보낸 메시지
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
		IFLOG(E_LOG_DEBUG, "SessionQueueThread CbkProcess");
		pclsMy->m_fnProcTrsgTranscoding();
	}

	delete _pclsEv;
}
