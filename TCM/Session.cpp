#include "Session.h"
#include "SessionMgr.h"
#include "Worker.h"
#include "SessionTimer.h"
#include "MainConfig.h"
#include "AppXmlParser.h"
#include "TrsgCdr.h"
#include "main.h"

using namespace eSipUtil;
Session::Session()
{
	m_pclsOwner=NULL;
	m_pclsTrseTr=NULL;
	m_pclsTrssTr=NULL;
	m_unTid = 0;
	m_clsSessionID = KNULL;
	m_clsJobID = KNULL;
	m_eSt = E_SESS_ST_NONE;
	m_nAllocSes=-1;
	m_clsInternalNasFileName = KNULL;
	m_clsInternalNasImgFileName = KNULL;
	m_clsTranscodesCnt.m_fnInit();
	m_pclsCTrsgCdrInfo = new CTrsgCdrInfo;
	m_nAllocWorker = Worker::m_pWorkerIdMgr->getAvailableId();

	if(m_nAllocWorker!=-1)
	{
		int nTotalSessionNum = (MainConfig::m_fnGetInstance()->m_unWorkerNum) * (MainConfig::m_fnGetInstance()->m_unIdMgrNum);
		IFLOG(E_LOG_ERR,"[Session] New Session [Total:%d]", nTotalSessionNum - Worker::m_pWorkerIdMgr->getAvailableIdNum());
	}
}
Session::~Session()
{

	//20220428//01/01/01/2022042814150800484344_decode.tmp 파일명 규칙에 의해 최소 42자리
	if(m_clsInternalNasFileName.m_unRealLen >= 42)
	{
		if(MainConfig::m_fnGetInstance()->m_bDeleteDecodeFile)
		{
			KString clsInternalNasFile; clsInternalNasFile.m_fnReSize(10240);
			KString clsPath = (KCSTR)MainConfig::m_fnGetInstance()->m_clsNasInternal;
			clsInternalNasFile.m_fnCat((KCSTR)clsPath);
			if( ((KCSTR)clsPath)[clsPath.m_unRealLen-1] != '/') clsInternalNasFile.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.
			clsInternalNasFile.m_fnCat((KCSTR)m_clsInternalNasFileName);
			remove((KCSTR)clsInternalNasFile);//internal_nas_path + (/) + filename
		}
	}

	//20220428/01/01/01/2022042814150800484344_decode.img 파일명 규칙에 의해 최소 42자리
	if(m_clsInternalNasImgFileName.m_unRealLen >= 42)
	{
		if(MainConfig::m_fnGetInstance()->m_bDeleteDecodeFile)//binary delete 와 동일한 config 사용함.
		{
			KString clsInternalNasImgFile; clsInternalNasImgFile.m_fnReSize(10240);
			KString clsPath = (KCSTR)MainConfig::m_fnGetInstance()->m_clsNasInternal;
			clsInternalNasImgFile.m_fnCat((KCSTR)clsPath);
			if( ((KCSTR)clsPath)[clsPath.m_unRealLen-1] != '/') clsInternalNasImgFile.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.
			clsInternalNasImgFile.m_fnCat((KCSTR)m_clsInternalNasImgFileName);
			remove((KCSTR)clsInternalNasImgFile);//internal_nas_path + (/) + imgfilename
		}
	}

	if(m_pclsTrseTr)
	{
		Worker * pclsWorker = (Worker*)m_fnGetWorker();
		if(pclsWorker->m_clsTrseTrMgr.m_fnDel(m_unTid, m_clsSessionID))
			IFLOG(E_LOG_INFO, "Trse Tr Del[Tid:%d][SessionID:%s]", m_unTid, (KCSTR)m_clsSessionID);
		else
			IFLOG(E_LOG_ERR, "Trse Tr Del Fail[Tid:%d][SessionID:%s]", m_unTid, (KCSTR)m_clsSessionID);
	}
	if(m_pclsTrssTr)
	{
		KString clsSessionID = m_pclsTrssTr->m_clsSessionID;//trss node Sess ID
		Worker * pclsWorker = (Worker*)m_fnGetWorker();
		if(pclsWorker->m_clsTrssTrMgr.m_fnDel(m_unTid, m_clsJobID, clsSessionID))
			IFLOG(E_LOG_INFO, "Trss Tr Del[Tid:%d][SessionID:%s][JobID:%s]", m_unTid, (KCSTR)clsSessionID, (KCSTR)m_clsJobID);
		else
			IFLOG(E_LOG_ERR, "Trss Tr Del Fail[Tid:%d][SessionID:%s][JobID:%s]", m_unTid, (KCSTR)clsSessionID, (KCSTR)m_clsJobID);
	}
	if(m_pclsCTrsgCdrInfo)
	{
		m_pclsCTrsgCdrInfo->m_fnMakeCdrList(m_clsJobID);
		delete m_pclsCTrsgCdrInfo;
	}
	m_clsTranscodesCnt.m_fnInit();
	if(m_nAllocWorker!=-1)
	{
		Worker::m_pWorkerIdMgr->releaseId(m_nAllocWorker);
		int nTotalSessionNum = (MainConfig::m_fnGetInstance()->m_unWorkerNum) * (MainConfig::m_fnGetInstance()->m_unIdMgrNum);
		IFLOG(E_LOG_ERR,"[Session] Del Session [Total:%d]", nTotalSessionNum - Worker::m_pWorkerIdMgr->getAvailableIdNum());
	}

}
void * Session::m_fnGetWorker()
{
	SessionMgr * pclsMgr = (SessionMgr*)m_pclsOwner;
	return pclsMgr->m_pclsOwner;
}
KString Session::m_fnGetFileName()
{
	//TCP 방식으로 들어온 Base64 데이터를 decode 하여 저장하기 위한 파일명 생성 함수.
	//2022012713015782065284_decode.tmp
	//2022-01-27 13:01:57.82065284_decode.tmp
	KString clsFileName; clsFileName.m_fnReSize(10240);

	time_t stTime;
	struct tm tmTime;
	stTime = time(NULL);
	localtime_r(&stTime, &tmTime);

	struct timespec now_monotonic;
	clock_gettime(CLOCK_REALTIME, &now_monotonic);

	KString::m_fnStrnCat((KSTR) clsFileName, clsFileName.m_unLen,
			"%04d%02d%02d/%02d/%02d/%02d/%04d%02d%02d%02d%02d%02d%08lld_decode.tmp",
			tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
			tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec,
			tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
			tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec,
			now_monotonic.tv_nsec / 10);

	KString clsDir;
	KString::m_fnStrnCat((KSTR) clsDir, clsDir.m_unLen, "%s/%04d%02d%02d/%02d/%02d/%02d", (KCSTR)MainConfig::m_fnGetInstance()->m_clsNasInternal,
			tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);

	CfgFile::m_fnCreateDir((KCSTR)clsDir);

	return clsFileName;
}
KString Session::m_fnGetImgFileName()
{
	//ImageData  Base64 데이터를 decode 하여 저장하기 위한 파일명 생성 함수.
	//2022012713015782065284_decode.img
	//2022-01-27 13:01:57.82065284_decode.img
	KString clsFileName; clsFileName.m_fnReSize(10240);

	time_t stTime;
	struct tm tmTime;
	stTime = time(NULL);
	localtime_r(&stTime, &tmTime);

	struct timespec now_monotonic;
	clock_gettime(CLOCK_REALTIME, &now_monotonic);

	KString::m_fnStrnCat((KSTR) clsFileName, clsFileName.m_unLen,
			"%04d%02d%02d/%02d/%02d/%02d/%04d%02d%02d%02d%02d%02d%08lld_decode.img",
			tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
			tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec,
			tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
			tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec,
			now_monotonic.tv_nsec / 10);

	KString clsDir;
	KString::m_fnStrnCat((KSTR) clsDir, clsDir.m_unLen, "%s/%04d%02d%02d/%02d/%02d/%02d", (KCSTR)MainConfig::m_fnGetInstance()->m_clsNasInternal,
			tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);

	CfgFile::m_fnCreateDir((KCSTR)clsDir);

	return clsFileName;
}
void Session::m_fnRecvTrseCrtJobReq(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_NONE) return;

	m_eSt = E_SESS_ST_TRSE_CREATE_JOB_REQ_RCV;
	Worker * pclsWorker = (Worker*)m_fnGetWorker();
	MainConfig * pclsConf = MainConfig::m_fnGetInstance();
	TrssNodeMgr * pclsTrssNodeMgr = TrssNodeMgr::m_fnGetInstance();

	pclsWorker->m_clsSessionTimer.m_fnSetTimer((KCSTR)m_clsSessionID, m_unTid, pclsConf->m_unSesTmr);

	m_pclsTrseTr->m_fnRecvReq(_rclsXml, m_clsJobID);//Worker 에서 JobID 이미 생성 함.

	ETrsgCodeSet_t eSt = AppXmlParser::m_fnCreateJobValid(_rclsXml);
	KString clsTrsgIP = m_pclsTrseTr->m_stTrseAddr.m_szLocalIp;
	KString clsClientIP = m_pclsTrseTr->m_stTrseAddr.m_szRemoteIp;
	m_clsTranscodesCnt.nTotal = AppXmlParser::m_fnGetTranscodingList(_rclsXml);
   if(eSt == E_TRANSCODING_ERR ) m_clsTranscodesCnt.nTotal = 1;//Cdr 기록을 위해 강제로 한 줄을 생성 함.
	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnRecvTrseCrtJobReq(_rclsXml, m_clsSessionID, clsTrsgIP, clsClientIP, m_clsTranscodesCnt.nTotal);

	if(eSt != E_JOB_RES_OK)
	{
		//SOURCECONTENT_TAG_DOES_NOT_EXIST or TARGETCONTENT_TAG_DOES_NOT_EXIST or TRANSCODING_TAG_DOES_NOT_EXIST
		m_fnSendTrseCrtJobRes(_rclsXml, eSt);//실패로 응답해야 함.
		m_fnCreateSessionEndEv();
		return;
	}

	int nSrcNasCode = -1;
	AppXmlParser::m_fnGetNasCode(_rclsXml, &nSrcNasCode , NULL);
	KString clsInternalNasFile; clsInternalNasFile.m_fnReSize(10240);
	KString clsFileType;
	if(nSrcNasCode == -1)//TCP Type
	{
		clsFileType = "TCP";
		KString clsFileName = m_fnGetFileName();
		KString clsNasPath = (KCSTR)pclsConf->m_clsNasInternal;
		clsInternalNasFile.m_fnCat((KCSTR)clsNasPath);
		if( ((KCSTR)clsNasPath)[clsNasPath.m_unRealLen-1] != '/') clsInternalNasFile.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.
		clsInternalNasFile.m_fnCat((KCSTR)clsFileName);

		ETrsgCodeSet_t eStWrite = AppXmlParser::m_fnWriteSourceFile(_rclsXml, clsNasPath, clsFileName);
		if(eStWrite == E_JOB_RES_OK)
		{
			IFLOG(E_LOG_INFO, "File Write OK[%s]", (KCSTR)clsInternalNasFile);//full path
			m_clsInternalNasFileName = (KCSTR)clsFileName;//세션 종료 시 삭제를 위해 파일명만 저장.(지울때 내부 NAS 경로 더해줌)
		}
		else if(eStWrite == E_INTERNAL_NAS_ACCESS_ERR)
		{
			//파일 쓰기 실패 예외처리
			IFLOG(E_LOG_ERR, "File Write Fail[%s]", (KCSTR)clsInternalNasFile);
			m_fnSendTrseCrtJobRes(_rclsXml, E_INTERNAL_NAS_ACCESS_ERR);//실패로 응답해야 함.
			m_fnCreateSessionEndEv();
			return;
		}
		else if(eStWrite == E_TCP_BINARY_DECODING_FAILED)
		{
			//파일 쓰기 실패 예외처리
			IFLOG(E_LOG_ERR, "Binary Decode Fail(Base64)[%s]", (KCSTR)clsInternalNasFile);
			m_fnSendTrseCrtJobRes(_rclsXml, E_TCP_BINARY_DECODING_FAILED);//실패로 응답해야 함.
			m_fnCreateSessionEndEv();
			return;
		}
		else if(eStWrite == E_SOURCE_CONTENT_ERR)
		{
			//파일 쓰기 실패 예외처리
			IFLOG(E_LOG_ERR, "SourceContent Not exist[%s]", (KCSTR)clsInternalNasFile);
			m_fnSendTrseCrtJobRes(_rclsXml, E_SOURCE_CONTENT_ERR);//실패로 응답해야 함.
			m_fnCreateSessionEndEv();
			return;
		}
	}
	else//Nas Type
	{
		clsFileType = "NAS";
	}

	KString clsImgFileName = m_fnGetImgFileName();
	KString clsNasPath = (KCSTR)pclsConf->m_clsNasInternal;
	if(AppXmlParser::m_fnWriteImageFile(_rclsXml, clsNasPath, clsImgFileName) == true)
	{
		m_clsInternalNasImgFileName = (KCSTR)clsImgFileName;//세션 종료 시 삭제를 위해 파일명만 저장.(지울때 내부 NAS 경로 더해줌)
		IFLOG(E_LOG_INFO, "[Session] InternalNasImgFileName[%s]", (KCSTR)m_clsInternalNasImgFileName);
	}
	KString clsPath, clsFile, clsContainer;
	AppXmlParser::m_fnGetSourceContentInfo(_rclsXml, clsPath, clsFile, clsContainer);
	//Source Data 정보 CDR 기록
	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnSetInputData(nSrcNasCode, clsPath, clsFile, clsContainer);
	//Target Data 정보 CDR 기록
	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnSetOutputData(_rclsXml);
	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnSetFileType(clsFileType);


	if(pclsTrssNodeMgr->m_fnIsAllDeactive())
	{
		IFLOG(E_LOG_ERR, "[Session] Trss All Deactive");
		//연결된 TRSS가 없으면 예외처리 해야 함.
		m_fnSendTrseCrtJobRes(_rclsXml, E_TRSS_SESSION_CLOSE);//실패로 응답 해야 함.
		m_fnCreateSessionEndEv();
		return;
	}
	else
	{
		IFLOG(E_LOG_DEBUG, "[Session] Trss Active Exist");
		//Trss Node의 분배와 해당 노드의 정보를 가져옴(addr,to,from,sessionID)
		KString clsTrsgName, clsTrssName, clsSessionID;
		Net5Tuple_t stTrssAddr;
		if(pclsTrssNodeMgr->m_fnDecisionNode(stTrssAddr, clsTrsgName, clsTrssName, clsSessionID)==0)//DecisionNode fail.
		{
			IFLOG(E_LOG_ERR, "[Session] DecisionNode fail");
			m_fnSendTrseCrtJobRes(_rclsXml, E_TRSS_SESSION_CLOSE);//실패로 응답 해야 함.
			m_fnCreateSessionEndEv();
			return;
		}
		else
		{
			KString clsAddrString;
			IFLOG(E_LOG_INFO, "[Session] DecisionNode ok[%s]", stTrssAddr.getStr((KSTR)clsAddrString,128,false));
			KString clsTrssIP = stTrssAddr.m_szRemoteIp;
			if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnSetTrssIP(clsTrssIP);
		}

		TrssTransaction * pclsTrssTr = pclsWorker->m_clsTrssTrMgr.m_fnFind(m_unTid, m_clsJobID, clsSessionID);
		if(pclsTrssTr)
		{
			IFLOG(E_LOG_ERR, "[Session] Duplicated Session Key : %s-%d-%s", (KCSTR)clsSessionID, m_unTid, (KCSTR)m_clsJobID);
			m_fnSendTrseCrtJobRes(_rclsXml, E_TRSS_SESSION_CLOSE);//실패로 응답 해야 함.
			m_fnCreateSessionEndEv();
			return;
		}
		else
		{
			pclsTrssTr = pclsWorker->m_clsTrssTrMgr.m_fnSet(m_unTid, m_clsJobID, clsSessionID, clsTrsgName, clsTrssName);//Create TrssTr
			pclsTrssTr->m_pvSession = (void*)this;//TrssTr -> Ses link
			pclsTrssTr->m_stTrssAddr = stTrssAddr;
			m_pclsTrssTr = pclsTrssTr;//Sess -> TrssTr link
		}

		//To TRSS 연결 되어 있으면 메시지 전달하자. 단, Job ID를 생성해서 붙히고 전달.
		m_fnSendTrssCrtJobReq(_rclsXml, m_clsInternalNasFileName, m_clsInternalNasImgFileName); //trss로 전달해야 함. 단, Job ID를 생성해서 붙히고 전달.
	}
}
void Session::m_fnSendTrssCrtJobReq(KString & _rclsXml, KString & _rclsInternalNasFile, KString & _rclsInternalNasImgFile)
{
	if(m_eSt != E_SESS_ST_TRSE_CREATE_JOB_REQ_RCV) return;
	m_eSt = E_SESS_ST_TRSS_CREATE_JOB_REQ_SND;

	if(m_pclsTrssTr->m_fnSendReq(_rclsXml, m_clsJobID, _rclsInternalNasFile, _rclsInternalNasImgFile))
	{
		//To TRSE
		m_fnSendTrseCrtJobRes(_rclsXml, E_JOB_RES_OK);//성공으로 응답해야 함.
	}
	else
	{
		m_fnSendTrseCrtJobRes(_rclsXml, E_TRSS_TCP_REQUEST_FAILED);//TRSS_TCP_REQUEST_FAILED
		m_fnCreateSessionEndEv();
	}
}
void Session::m_fnSendTrseCrtJobRes(KString & _rclsXml, ETrsgCodeSet_t _eSt)
{
	//create Job 을 trss로 전달하지 못하는 상황이 있을 경우 바로 trse에 응답해야 할 수 있음.
	if(m_eSt != E_SESS_ST_TRSS_CREATE_JOB_REQ_SND
			&& m_eSt != E_SESS_ST_TRSE_CREATE_JOB_REQ_RCV) return;
	m_eSt = E_SESS_ST_TRSE_CREATE_JOB_RES_SND;

	m_pclsTrseTr->m_fnSendRes(_eSt);
}
void Session::m_fnRecvTrssJobChgNotiCreated(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_TRSE_CREATE_JOB_RES_SND) return;
	m_eSt = E_SESS_ST_TRSS_JOB_CHG_NOTI_CREATED_RCV;

	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnRecvTrssJobChgNotiCreated(_rclsXml);
	m_pclsTrssTr->m_fnRecvNotiCreated(_rclsXml);
	m_fnSendTrseJobChgNotiCreated(_rclsXml);
}
void Session::m_fnSendTrseJobChgNotiCreated(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_TRSS_JOB_CHG_NOTI_CREATED_RCV) return;
	m_eSt = E_SESS_ST_TRSE_JOB_CHG_NOTI_CREATED_SND;

	m_pclsTrseTr->m_fnSendNotiCreated(_rclsXml);
}
void Session::m_fnRecvTrssJobChgNotiWaiting(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_TRSE_JOB_CHG_NOTI_CREATED_SND) return;
	m_eSt = E_SESS_ST_TRSS_JOB_CHG_NOTI_WAITING_RCV;

	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnRecvTrssJobChgNotiWaiting(_rclsXml);
	m_pclsTrssTr->m_fnRecvNotiWaiting(_rclsXml);
	m_fnSendTrseJobChgNotiWaiting(_rclsXml);
}
void Session::m_fnSendTrseJobChgNotiWaiting(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_TRSS_JOB_CHG_NOTI_WAITING_RCV) return;
	m_eSt = E_SESS_ST_TRSE_JOB_CHG_NOTI_WAITING_SND;

	m_pclsTrseTr->m_fnSendNotiWaiting(_rclsXml);
}
void Session::m_fnRecvTrssJobChgNotiJobStarted(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_TRSE_JOB_CHG_NOTI_WAITING_SND) return;
	m_eSt = E_SESS_ST_TRSS_JOB_CHG_NOTI_JOB_STARTED_RCV;

	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnRecvTrssJobChgNotiJobStarted(_rclsXml);
	m_pclsTrssTr->m_fnRecvNotiJobStarted(_rclsXml);
	m_fnSendTrseJobChgNotiJobStarted(_rclsXml);
}
void Session::m_fnSendTrseJobChgNotiJobStarted(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_TRSS_JOB_CHG_NOTI_JOB_STARTED_RCV) return;
	m_eSt = E_SESS_ST_TRSE_JOB_CHG_NOTI_JOB_STARTED_SND;

	m_pclsTrseTr->m_fnSendNotiJobStarted(_rclsXml);
}
void Session::m_fnRecvTrssJobChgNotiTcStarted(KString & _rclsXml)
{
	//TC Start/Stop은 쌍을 이뤄 반복적으로 들어올 수 있음
	if(m_eSt != E_SESS_ST_TRSE_JOB_CHG_NOTI_JOB_STARTED_SND
			&& m_eSt != E_SESS_ST_TRSE_JOB_CHG_NOTI_TC_STOPPED_SND) return;
	m_eSt = E_SESS_ST_TRSS_JOB_CHG_NOTI_TC_STARTED_RCV;

	m_clsTranscodesCnt.nCurrent++;//T/C Start 수신시마다 Current 증가.
	KString clsTransSessionID;
	AppXmlParser::m_fnGetTransSessionID(_rclsXml, clsTransSessionID);
	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnRecvTrssJobChgNotiTcStarted(_rclsXml, clsTransSessionID);
	m_pclsTrssTr->m_fnRecvNotiTCStarted(_rclsXml);
	m_fnSendTrseJobChgNotiTcStarted(_rclsXml);
}
void Session::m_fnSendTrseJobChgNotiTcStarted(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_TRSS_JOB_CHG_NOTI_TC_STARTED_RCV) return;
	m_eSt = E_SESS_ST_TRSE_JOB_CHG_NOTI_TC_STARTED_SND;

	m_pclsTrseTr->m_fnSendNotiTCStarted(_rclsXml);
}
void Session::m_fnRecvTrssJobChgNotiTcStopped(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_TRSE_JOB_CHG_NOTI_TC_STARTED_SND) return;
	m_eSt = E_SESS_ST_TRSS_JOB_CHG_NOTI_TC_STOPPED_RCV;

	KString clsResultCode;
	AppXmlParser::m_fnGetJobStateResultCode(_rclsXml, clsResultCode);
	//T/C 결과 성공/실패에 따른 Cnt 증가.
	if(g_fnGetEnumIdx((KCSTR)clsResultCode) == E_JOB_RES_OK) m_clsTranscodesCnt.nSuccess++;
	else m_clsTranscodesCnt.nFail++;

	//AppXmlParser::m_fnGetTransCnt(_rclsXml, NULL, NULL, NULL, &m_clsTranscodesCnt.nSuccess, NULL);

	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnRecvTrssJobChgNotiTcStopped(_rclsXml, clsResultCode);
	m_pclsTrssTr->m_fnRecvNotiTCSStopped(_rclsXml);
	m_fnSendTrseJobChgNotiTcStopped(_rclsXml, clsResultCode);
}
void Session::m_fnSendTrseJobChgNotiTcStopped(KString & _rclsXml, KString & _rclsResultCode)
{
	if(m_eSt != E_SESS_ST_TRSS_JOB_CHG_NOTI_TC_STOPPED_RCV) return;
	m_eSt = E_SESS_ST_TRSE_JOB_CHG_NOTI_TC_STOPPED_SND;

	m_pclsTrseTr->m_fnSendNotiTCSStopped(_rclsXml, _rclsResultCode);
}
void Session::m_fnRecvTrssJobChgNotiJobStopped(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_TRSE_JOB_CHG_NOTI_TC_STOPPED_SND) return;
	m_eSt = E_SESS_ST_TRSS_JOB_CHG_NOTI_JOB_STOPPED_RCV;

	m_clsTranscodesCnt.nCurrent = 0;// T/C Start 수신시마다 Current 증가.(JobStop 부터 다시  0).

	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnRecvTrssJobChgNotiJobStopped(_rclsXml);
	m_pclsTrssTr->m_fnRecvNotiJobStopped(_rclsXml);
	m_fnSendTrseJobChgNotiJobStopped(_rclsXml);
}
void Session::m_fnSendTrseJobChgNotiJobStopped(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_TRSS_JOB_CHG_NOTI_JOB_STOPPED_RCV) return;
	m_eSt = E_SESS_ST_TRSE_JOB_CHG_NOTI_JOB_STOPPED_SND;

	m_pclsTrseTr->m_fnSendNotiJobStopped(_rclsXml);
}
void Session::m_fnRecvTrssJobChgNotiJobDestroyed(KString & _rclsXml)
{
	if(m_eSt < E_SESS_ST_TRSS_CREATE_JOB_REQ_SND) return; //CrtJob을 보내지도 않았으면 Destory가 올 수 없음.
	if(m_eSt > E_SESS_ST_TRSS_JOB_CHG_NOTI_JOB_DESTROYED_RCV) return; //Destory를 이미 받았으면 메시지 버림.

	m_eSt = E_SESS_ST_TRSS_JOB_CHG_NOTI_JOB_DESTROYED_RCV;
	//Destroy가 성공으로 받았는지 봐야함. Trss에서 Ses Full일때 줄 수도 있어서 코드 확인 후  CDR 기입해야함.
	m_clsTranscodesCnt.nCurrent = 0;// T/C Start 수신시마다 Current 증가.(JobStop 부터 다시  0 Destroyed도 마찬가지).
	m_clsTranscodesCnt.nFail = m_clsTranscodesCnt.nTotal - m_clsTranscodesCnt.nSuccess;//성공 외 모두 실패 처리

	KString clsResultCode;
	AppXmlParser::m_fnGetJobStateResultCode(_rclsXml, clsResultCode);

	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnRecvTrssJobChgNotiJobDestroyed(_rclsXml, clsResultCode);
	m_pclsTrssTr->m_fnRecvNotiJobDestroy(_rclsXml);
	m_fnSendTrseJobChgNotiJobDestroyed(_rclsXml);
}
void Session::m_fnSendTrseJobChgNotiJobDestroyed(KString & _rclsXml)
{
	if(m_eSt != E_SESS_ST_TRSS_JOB_CHG_NOTI_JOB_DESTROYED_RCV) return;
	m_eSt = E_SESS_ST_TRSE_JOB_CHG_NOTI_JOB_DESTROYED_SND;

	m_pclsTrseTr->m_fnSendNotiJobDestroy(_rclsXml);
}
void Session::m_fnRecvTrssTrTimeout()
{
	//TRSS 에게 JobStatusChangedNotify(Created) 수신하였으면 return (TRSE로 CREATE_JOB_RES 전송 이후 Created 받지 못하고 멈춰있을때 예외처리 로직임)
	if(m_eSt != E_SESS_ST_TRSE_CREATE_JOB_RES_SND) return;

	//Trse로 Destroy 보내고 종료하여야 함.
	m_clsTranscodesCnt.nFail = m_clsTranscodesCnt.nTotal;//모두 실패 처리
	KString clsNotify; clsNotify.m_fnReSize(10240);
	m_pclsTrseTr->m_fnSendNotiJobDestroy(clsNotify);

	//세션 종료 처리.
	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnRecvTrssJobCreatedTimeout();
	m_fnCreateSessionEndEv();
}
void Session::m_fnRecvSessionTimeOut()
{
	//TRSS 에게 Created 를 받았어도 이후 멈춰있을 수 있음에 대한 예외 처리.(Destroy까지 정상적으로 지나가지 못하고 멈춰있을 때의 예외처리.)
	if(E_SESS_ST_TRSS_JOB_CHG_NOTI_CREATED_RCV <= m_eSt && m_eSt <= E_SESS_ST_TRSE_JOB_CHG_NOTI_JOB_STOPPED_SND)
	{
		//Trse로 Destroy 보내고 종료하여야 함.
		m_clsTranscodesCnt.nFail = m_clsTranscodesCnt.nTotal - m_clsTranscodesCnt.nSuccess;//성공빼고 나머지 실패 처리.
		KString clsNotify; clsNotify.m_fnReSize(10240);
		m_pclsTrseTr->m_fnSendNotiJobDestroy(clsNotify);

		//세션 종료 처리.
		if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnRecvSessionTimeOut();
		m_fnCreateSessionEndEv();
	}
}
void Session::m_fnRecvTrseDisconSesClear()
{
	//TRSE가 접속 종료 되었을 경우에 대한 예외처리 로직
	if(m_eSt == E_SESS_ST_NONE) return;
	if(m_eSt > E_SESS_ST_TRSE_JOB_CHG_NOTI_JOB_DESTROYED_SND) return;

	//세션 종료 처리.
	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnRecvTrseDisconSesClear();
	m_fnCreateSessionEndEv();
}
void Session::m_fnGarbageClear()
{
	//Garbage 에 걸린 Session의 경우 CDR 기록 후 무조건 삭제함.
	if(m_pclsCTrsgCdrInfo) m_pclsCTrsgCdrInfo->m_fnGarbageClear();
}
void Session::m_fnCreateSessionEndEv()
{
	AppSessionEndEvent * pclsEv = new AppSessionEndEvent;
	pclsEv->m_clsSessionID = m_clsSessionID;
	pclsEv->m_unTid = m_unTid;
	pclsEv->m_eT = E_MAIN_EV_SESSION_END;
	Worker::m_fnPutSessionEndEv(pclsEv);
}
