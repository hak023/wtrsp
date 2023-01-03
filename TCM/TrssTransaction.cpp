#include "TrssTransaction.h"
#include "TrssTransport.h"
#include "TrssNodeMgr.h"
#include "Worker.h"
#include "MainConfig.h"
#include "AppXmlParser.h"
#include "main.h"

using namespace eSipUtil;

TrssTransaction::TrssTransaction()
{
	m_eSt = E_TRSS_TR_ST_NONE;
	m_stTrssAddr.reset();
	m_pclsOwner = NULL;
	m_pvSession = NULL;
	m_clsReq = KNULL;
	m_clsJobID = KNULL;
	m_clsRes = KNULL;
	m_clsSessionID = KNULL;//trss node SessionID
	m_clsTrsgName = KNULL;
	m_clsTrssName = KNULL;
}
TrssTransaction::~TrssTransaction()
{

}
void * TrssTransaction::m_fnGetWorker()
{
	TrssTransactionMgr * pclsTrMgr = (TrssTransactionMgr*)m_pclsOwner;
	return pclsTrMgr->m_pclsOwner;
}
bool TrssTransaction::m_fnSendReq(KString & _rclsReq, KString & _rclsJobID, KString & _rclsInternalNasFileName, KString & _rclsInternalNasImgFileName)
{
	if(m_eSt != E_TRSS_TR_ST_NONE) return false;

	m_eSt = E_TRSS_TR_ST_REQ_SND;
	m_clsJobID = _rclsJobID;

	TrssTransport * pclsTrans = TrssTransport::m_fnGetInstance();

	KString clsCreateJobReq; clsCreateJobReq.m_fnReSize(10240);
	AppXmlParser::m_fnMakeCreateJobReq(_rclsReq, m_clsTrssName/*To*/, m_clsTrsgName/*From*/, m_clsSessionID/*trss*/, m_clsJobID, _rclsInternalNasFileName, _rclsInternalNasImgFileName, clsCreateJobReq);
	m_clsReq = clsCreateJobReq;
	m_fnCallLog(true,clsCreateJobReq);

	Session * pclsSess = (Session *)m_pvSession;
	if(pclsTrans->m_fnXmlSend(m_stTrssAddr, m_clsReq) == true)
	{
		if(pclsSess->m_pclsCTrsgCdrInfo) pclsSess->m_pclsCTrsgCdrInfo->m_fnSendTrssCrtJobReq(_rclsReq);
		Worker * pclsWorker = (Worker*)m_fnGetWorker();
		unsigned int unTid = AppXmlParser::m_fnGetTransactionID(_rclsReq);
		pclsWorker->m_clsTrssTrTimer.m_fnSetTimer((KCSTR)m_clsSessionID, (KCSTR)m_clsJobID, unTid,  MainConfig::m_fnGetInstance()->m_unTrssJobCreatedTimeout);
	}
	else//CrtJobReq 전송 실패.
	{
		if(pclsSess->m_pclsCTrsgCdrInfo) pclsSess->m_pclsCTrsgCdrInfo->m_fnSendTrssCrtJobReqFail(_rclsReq);
		return false;
	}
	return true;
}
void TrssTransaction::m_fnRecvNotiCreated(KString & _rclsNotify)
{
	if(m_eSt != E_TRSS_TR_ST_REQ_SND) return;

	m_eSt = E_TRSS_TR_ST_NOTI_CREATED_RCV;
	m_fnCallLog(false,_rclsNotify);
}
void TrssTransaction::m_fnRecvNotiWaiting(KString & _rclsNotify)
{
	if(m_eSt != E_TRSS_TR_ST_NOTI_CREATED_RCV) return;

	m_eSt = E_TRSS_TR_ST_NOTI_WAITING_RCV;
	m_fnCallLog(false,_rclsNotify);
}
void TrssTransaction::m_fnRecvNotiJobStarted(KString & _rclsNotify)
{
	if(m_eSt != E_TRSS_TR_ST_NOTI_WAITING_RCV) return;

	m_eSt = E_TRSS_TR_ST_NOTI_JOB_STARTED_RCV;
	m_fnCallLog(false,_rclsNotify);
}
void TrssTransaction::m_fnRecvNotiTCStarted(KString & _rclsNotify)
{
	if(m_eSt != E_TRSS_TR_ST_NOTI_JOB_STARTED_RCV
			&& m_eSt != E_TRSS_TR_ST_NOTI_TC_STOPPED_RCV) return;

	m_eSt = E_TRSS_TR_ST_NOTI_TC_STARTED_RCV;
	m_fnCallLog(false,_rclsNotify);
}
void TrssTransaction::m_fnRecvNotiTCSStopped(KString & _rclsNotify)
{
	if(m_eSt != E_TRSS_TR_ST_NOTI_TC_STARTED_RCV) return;

	m_eSt = E_TRSS_TR_ST_NOTI_TC_STOPPED_RCV;
	m_fnCallLog(false,_rclsNotify);
}
void TrssTransaction::m_fnRecvNotiJobStopped(KString & _rclsNotify)
{
	if(m_eSt != E_TRSS_TR_ST_NOTI_TC_STOPPED_RCV) return;

	m_eSt = E_TRSS_TR_ST_NOTI_JOB_STOPPED_RCV;

	m_fnCallLog(false,_rclsNotify);
}
void TrssTransaction::m_fnRecvNotiJobDestroy(KString & _rclsNotify)
{
	if(m_eSt < E_TRSS_TR_ST_REQ_SND) return;
	if(m_eSt == E_TRSS_TR_ST_NOTI_JOB_DESTROYED_RCV) return;//이미 받은적이 있으면 버림.

	m_eSt = E_TRSS_TR_ST_NOTI_JOB_DESTROYED_RCV;
	m_fnCallLog(false,_rclsNotify);
}
void TrssTransaction::m_fnCallLog(bool bSend, KString _clsXml)
{
	if(g_fnCheckLog(E_LOG_INFO) == false ) return;

	ELogLevel_t eLv = E_LOG_INFO;

	KString clsLog; 
   if (MainConfig::m_fnGetInstance()->m_bXmlFormatter) clsLog.m_fnReSize(4096 + _clsXml.m_unRealLen);
   else clsLog.m_fnReSize(1024 + _clsXml.m_unRealLen);
	char * pszTemp = (KSTR)clsLog;

	KString clsSessionID = m_clsSessionID;
	unsigned int unTid = AppXmlParser::m_fnGetTransactionID(_clsXml);

	KString clsMine;
	KString clsOther;
	KString clsBodyType;
	AppXmlParser::m_fnGetBodyType(_clsXml, clsBodyType);

	KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"TrssTr [S:%s][T:%010u] ",(KCSTR)clsSessionID,unTid);

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
		if(MainConfig::m_fnGetInstance()->m_bSkipBinary && bSend)//ss로 부터 받는 메시지는 바이너리로 전달해 주지 않는다. 따라서 로그를 줄일 필요 없음.
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
