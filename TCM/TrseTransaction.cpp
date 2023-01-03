#include "TrseTransaction.h"
#include "TrseTransport.h"
#include "TrseNodeMgr.h"
#include "Worker.h"
#include "MainConfig.h"
#include "AppXmlParser.h"
#include "main.h"

using namespace eSipUtil;

TrseTransaction::TrseTransaction()
{
	m_eSt = E_TRSE_TR_ST_NONE;
	m_stTrseAddr.reset();
	m_pclsOwner = NULL;
	m_pvSession = NULL;
	m_clsReq = KNULL;
	m_clsTrseName = KNULL;
	m_clsTrsgName = KNULL;
	m_clsJobID = KNULL;
	m_clsRes = KNULL;
}
TrseTransaction::~TrseTransaction()
{

}
void * TrseTransaction::m_fnGetWorker()
{
	TrseTransactionMgr * pclsTrMgr = (TrseTransactionMgr*)m_pclsOwner;
	return pclsTrMgr->m_pclsOwner;
}
void TrseTransaction::m_fnRecvReq(KString & _rclsReq, KString & _rclsJobID)
{
	if(m_eSt != E_TRSE_TR_ST_NONE) return;
	m_eSt = E_TRSE_TR_ST_REQ_RCV;

	m_clsReq = _rclsReq;
	m_clsJobID = _rclsJobID;
	AppXmlParser::m_fnGetToFrom(m_clsReq, m_clsTrsgName, m_clsTrseName);
	m_fnCallLog(false,_rclsReq);
}
void TrseTransaction::m_fnSendRes(ETrsgCodeSet_t _eSt)
{
	if(m_eSt != E_TRSE_TR_ST_REQ_RCV) return;

	m_eSt = E_TRSE_TR_ST_RSP_SND;
	KString clsCreateJobRes; clsCreateJobRes.m_fnReSize(10240);
	AppXmlParser::m_fnMakeCreateJobRes(m_clsReq, m_clsTrseName/*To*/, m_clsTrsgName/*From*/, m_clsJobID, _eSt, clsCreateJobRes);
	m_clsRes = clsCreateJobRes;
	m_fnCallLog(true,clsCreateJobRes);

	TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
	if(pclsTrans->m_fnXmlSend(m_stTrseAddr, m_clsRes))
	{
		Session * pclsSess = (Session *)m_pvSession;
		if(pclsSess->m_pclsCTrsgCdrInfo) pclsSess->m_pclsCTrsgCdrInfo->m_fnSendTrseCrtJobRes(m_clsRes, _eSt);
	}
	else
	{
		Session * pclsSess = (Session *)m_pvSession;
		if(pclsSess->m_pclsCTrsgCdrInfo) pclsSess->m_pclsCTrsgCdrInfo->m_fnSendTrseCrtJobResFail(m_clsRes, E_TRSE_TCP_RESPONSE_FAILED);
	}
}
void TrseTransaction::m_fnSendNotiCreated(KString & _rclsNotify)
{
	if(m_eSt != E_TRSE_TR_ST_RSP_SND) return;
	m_eSt = E_TRSE_TR_ST_NOTI_CREATED_SND;

	TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
	Session * pclsSess = (Session *)m_pvSession;
	KString clsJobStatusChgNotify; clsJobStatusChgNotify.m_fnReSize(10240);
	AppXmlParser::m_fnMakeJobStatusChangedNotify_Created(_rclsNotify, m_clsTrseName/*To*/, m_clsTrsgName/*From*/, pclsSess->m_clsSessionID, clsJobStatusChgNotify);
	m_fnCallLog(true,clsJobStatusChgNotify);
	if(pclsTrans->m_fnXmlSend(m_stTrseAddr, clsJobStatusChgNotify))
	{
		if(pclsSess->m_pclsCTrsgCdrInfo) pclsSess->m_pclsCTrsgCdrInfo->m_fnSendTrseJobChgNotiCreated(clsJobStatusChgNotify);
	}
}
void TrseTransaction::m_fnSendNotiWaiting(KString & _rclsNotify)
{
	if(m_eSt != E_TRSE_TR_ST_NOTI_CREATED_SND) return;

	m_eSt = E_TRSE_TR_ST_NOTI_WAITING_SND;

	TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
	Session * pclsSess = (Session *)m_pvSession;
	KString clsJobStatusChgNotify; clsJobStatusChgNotify.m_fnReSize(10240);
	AppXmlParser::m_fnMakeJobStatusChangedNotify_Waiting(_rclsNotify, m_clsTrseName/*To*/, m_clsTrsgName/*From*/, pclsSess->m_clsSessionID, clsJobStatusChgNotify);
	m_fnCallLog(true,clsJobStatusChgNotify);
	if(pclsTrans->m_fnXmlSend(m_stTrseAddr, clsJobStatusChgNotify))
	{
		if(pclsSess->m_pclsCTrsgCdrInfo) pclsSess->m_pclsCTrsgCdrInfo->m_fnSendTrseJobChgNotiWaiting(clsJobStatusChgNotify);
	}
}
void TrseTransaction::m_fnSendNotiJobStarted(KString & _rclsNotify)
{
	if(m_eSt != E_TRSE_TR_ST_NOTI_WAITING_SND) return;

	m_eSt = E_TRSE_TR_ST_NOTI_JOB_STARTED_SND;

	TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
	Session * pclsSess = (Session *)m_pvSession;
	KString clsJobStatusChgNotify; clsJobStatusChgNotify.m_fnReSize(10240);
	AppXmlParser::m_fnMakeJobStatusChangedNotify_JobStarted(_rclsNotify, m_clsTrseName/*To*/, m_clsTrsgName/*From*/, pclsSess->m_clsSessionID, pclsSess->m_clsTranscodesCnt, clsJobStatusChgNotify);
	m_fnCallLog(true,clsJobStatusChgNotify);
	if(pclsTrans->m_fnXmlSend(m_stTrseAddr, clsJobStatusChgNotify))
	{
		if(pclsSess->m_pclsCTrsgCdrInfo) pclsSess->m_pclsCTrsgCdrInfo->m_fnSendTrseJobChgNotiJobStarted(clsJobStatusChgNotify);
	}
}
void TrseTransaction::m_fnSendNotiTCStarted(KString & _rclsNotify)
{
	if(m_eSt != E_TRSE_TR_ST_NOTI_JOB_STARTED_SND
			&& m_eSt != E_TRSE_TR_ST_NOTI_TC_STOPPED_SND) return;

	m_eSt = E_TRSE_TR_ST_NOTI_TC_STARTED_SND;

	TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
	Session * pclsSess = (Session *)m_pvSession;
	KString clsJobStatusChgNotify; clsJobStatusChgNotify.m_fnReSize(10240);
	AppXmlParser::m_fnMakeJobStatusChangedNotify_TC_Started(_rclsNotify, m_clsTrseName/*To*/, m_clsTrsgName/*From*/, pclsSess->m_clsSessionID, pclsSess->m_clsTranscodesCnt, clsJobStatusChgNotify);
	m_fnCallLog(true,clsJobStatusChgNotify);
	if(pclsTrans->m_fnXmlSend(m_stTrseAddr, clsJobStatusChgNotify))
	{
		if(pclsSess->m_pclsCTrsgCdrInfo) pclsSess->m_pclsCTrsgCdrInfo->m_fnSendTrseJobChgNotiTcStarted(clsJobStatusChgNotify);
	}
}
void TrseTransaction::m_fnSendNotiTCSStopped(KString & _rclsNotify, KString & _rclsResultCode)
{
	if(m_eSt != E_TRSE_TR_ST_NOTI_TC_STARTED_SND) return;

	m_eSt = E_TRSE_TR_ST_NOTI_TC_STOPPED_SND;

	TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
	Session * pclsSess = (Session *)m_pvSession;
	KString clsJobStatusChgNotify; clsJobStatusChgNotify.m_fnReSize(1024+_rclsNotify.m_unRealLen);
	bool bRtn = AppXmlParser::m_fnMakeJobStatusChangedNotify_TC_Stopped(_rclsNotify, m_clsTrseName/*To*/, m_clsTrsgName/*From*/, pclsSess->m_clsSessionID, pclsSess->m_clsTranscodesCnt, _rclsResultCode, clsJobStatusChgNotify);

	if(bRtn)
	{/*T/C Stop 수신이 ResultCode 에 따라 이미 반영되어 있음.*/}
	else//Binary to Base64 실패 시 카운트 조절이 필요함.
	{/*m_fnMakeJobStatusChangedNotify_TC_Stopped 내부에서 반영함.*/}

	m_fnCallLog(true,clsJobStatusChgNotify);
	if(pclsTrans->m_fnXmlSend(m_stTrseAddr, clsJobStatusChgNotify))
	{
		if(pclsSess->m_pclsCTrsgCdrInfo)
		{
			pclsSess->m_pclsCTrsgCdrInfo->m_fnSendTrseJobChgNotiTcStopped(clsJobStatusChgNotify, bRtn);
		}
	}
}
void TrseTransaction::m_fnSendNotiJobStopped(KString & _rclsNotify)
{
	if(m_eSt != E_TRSE_TR_ST_NOTI_TC_STOPPED_SND) return;

	m_eSt = E_TRSE_TR_ST_NOTI_JOB_STOPPED_SND;

	TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
	Session * pclsSess = (Session *)m_pvSession;
	KString clsJobStatusChgNotify; clsJobStatusChgNotify.m_fnReSize(10240);
	AppXmlParser::m_fnMakeJobStatusChangedNotify_JobStopped(_rclsNotify, m_clsTrseName/*To*/, m_clsTrsgName/*From*/, pclsSess->m_clsSessionID, pclsSess->m_clsTranscodesCnt, clsJobStatusChgNotify);
	m_fnCallLog(true,clsJobStatusChgNotify);
	if(pclsTrans->m_fnXmlSend(m_stTrseAddr, clsJobStatusChgNotify))
	{
		if(pclsSess->m_pclsCTrsgCdrInfo) pclsSess->m_pclsCTrsgCdrInfo->m_fnSendTrseJobChgNotiJobStopped(clsJobStatusChgNotify);
	}
}
void TrseTransaction::m_fnSendNotiJobDestroy(KString & _rclsNotify)
{
	if(m_eSt < E_TRSE_TR_ST_RSP_SND) return;//TRSE 에게 CreateJobResponse 를 보낸적이 없으면 return
	if(m_eSt >= E_TRSE_TR_ST_NOTI_JOB_DESTROYED_SND) return;// Destroyed TRSE 에게 Destroyed 를 이미 보냈으면  return

	ETrsgCodeSet_t eSt;
	if (m_eSt == E_TRSE_TR_ST_RSP_SND) eSt = E_JOB_CREATED_TIMEOUT; //Created 못보냈으면 CREATED_TIMEOUT 처리.
	else eSt = E_JOB_TRANSCODE_TIMEOUT;//보냈으면 TRANSCODE_TIMEOUT 처리.

	m_eSt = E_TRSE_TR_ST_NOTI_JOB_DESTROYED_SND;

	TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
	Session * pclsSess = (Session *)m_pvSession;
	KString clsJobStatusChgNotify; clsJobStatusChgNotify.m_fnReSize(10240);

	if(_rclsNotify.m_unRealLen == 0)
	{
		_rclsNotify = m_clsRes;//자체적으로 Destroy를 생성해서 보내야 할때. TRSE로 부터 받은 CreateJobResponse 메시지를 기반으로 Destroy 메시지를 만든다.
		AppXmlParser::m_fnMakeJobStatusChangedNotify_Destroyed(_rclsNotify, clsJobStatusChgNotify, pclsSess->m_clsTranscodesCnt, eSt);
	}
	else
	{
		AppXmlParser::m_fnMakeJobStatusChangedNotify_Destroyed(_rclsNotify, m_clsTrseName/*To*/, m_clsTrsgName/*From*/, pclsSess->m_clsSessionID, pclsSess->m_clsTranscodesCnt, clsJobStatusChgNotify);
	}
	m_fnCallLog(true,clsJobStatusChgNotify);
	if(pclsTrans->m_fnXmlSend(m_stTrseAddr, clsJobStatusChgNotify))
	{
		if(pclsSess->m_pclsCTrsgCdrInfo) pclsSess->m_pclsCTrsgCdrInfo->m_fnSendTrseJobChgNotiJobDestroyed(clsJobStatusChgNotify);
	}
}
void TrseTransaction::m_fnCallLog(bool bSend, KString _clsXml)
{
	if(g_fnCheckLog(E_LOG_INFO) == false ) return;

	ELogLevel_t eLv = E_LOG_INFO;

	KString clsLog;
	if (MainConfig::m_fnGetInstance()->m_bXmlFormatter) clsLog.m_fnReSize(4096 + 128 + _clsXml.m_unRealLen);
	else clsLog.m_fnReSize(1024 + 128 + _clsXml.m_unRealLen);
	char * pszTemp = (KSTR)clsLog;

	Session * pclsSess = (Session *)m_pvSession;
	KString clsSessionID = pclsSess->m_clsSessionID;
	unsigned int unTid = AppXmlParser::m_fnGetTransactionID(_clsXml);

	KString clsMine;
	KString clsOther;
	KString clsBodyType;
	KString clsAddrString;
	AppXmlParser::m_fnGetBodyType(_clsXml, clsBodyType);

	KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"TrseTr %s [S:%s][T:%010u] ",m_stTrseAddr.getStr((KSTR )clsAddrString, 128, false), (KCSTR)clsSessionID, unTid);

	if(bSend)
	{
		AppXmlParser::m_fnGetToFrom(_clsXml, clsOther, clsMine);
		KString::m_fnStrnCat(pszTemp, clsLog.m_unLen,"%s --> %s : %s",  (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR)clsOther, (KCSTR)clsBodyType);
	}
	else
	{
		AppXmlParser::m_fnGetToFrom(_clsXml, clsMine, clsOther);
		KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"%s <-- %s : %s",  (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR)clsOther, (KCSTR)clsBodyType);
	}
	if(g_fnCheckLog(E_LOG_DEBUG))
	{
		eLv = E_LOG_DEBUG;
		if(MainConfig::m_fnGetInstance()->m_bSkipBinary)
		{
			KString clsSimpleXml;
			AppXmlParser::m_fnSetLongLogSkip(_clsXml, clsSimpleXml);
			if(MainConfig::m_fnGetInstance()->m_bXmlFormatter)
			{
				KString clsXmlFormat;
				AppXmlParser::m_fnXmlFormatter(clsSimpleXml, clsXmlFormat);
				KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"\n%s", (KCSTR)clsXmlFormat);
			}
			else
				KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"\n%s", (KCSTR)clsSimpleXml);
		}
		else
		{
			if(MainConfig::m_fnGetInstance()->m_bXmlFormatter)
			{
				KString clsXmlFormat;
				AppXmlParser::m_fnXmlFormatter(_clsXml, clsXmlFormat);
				KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"\n%s", (KCSTR)clsXmlFormat);
			}
			else
				KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"\n%s", (KCSTR)_clsXml);
		}
	}
	IFLOG(eLv, pszTemp);
}
