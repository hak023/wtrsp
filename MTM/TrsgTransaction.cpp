#include "TrsgTransaction.h"
#include "TrsgTransport.h"
#include "TrsgNodeMgr.h"
#include "Worker.h"
#include "MainConfig.h"
#include "AppXmlParser.h"
#include "main.h"

using namespace eSipUtil;

TrsgTransaction::TrsgTransaction()
{
	m_eSt = E_TRSG_TR_ST_NONE;
	m_stTrsgAddr.reset();
	m_pclsOwner = NULL;
	m_clsSessionID = KNULL;
}
TrsgTransaction::~TrsgTransaction()
{

}
void* TrsgTransaction::m_fnGetWorker()
{
	TrsgTransactionMgr *pclsTrMgr = (TrsgTransactionMgr*) m_pclsOwner;
	return pclsTrMgr->m_pclsOwner;
}
void TrsgTransaction::m_fnRecvCrtJobReq(eSipUtil::KString &_rclsXml)
{
	if(m_eSt != E_TRSG_TR_ST_NONE) return;

	m_eSt = E_TRSG_TR_ST_REQ_RCV;

	AppXmlParser::m_fnGetServiceName(_rclsXml, m_clsServiceName);
	m_fnCallLog(false, _rclsXml);
}
void TrsgTransaction::m_fnSendNotiCreated(KString &_rclsXml, KString &_rclsNotify)
{
	m_eSt = E_TRSG_TR_ST_NOTI_CREATED_SND;

	TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();

	AppXmlParser::m_fnGetSessionID(_rclsXml, m_clsSessionID);
	AppXmlParser::m_fnMakeJobStatusChangedNotify_Created(_rclsXml, m_clsServiceName, _rclsNotify);
	m_fnCallLog(true, _rclsNotify);
	pclsTrans->m_fnXmlSend(m_stTrsgAddr, _rclsNotify);

}
void TrsgTransaction::m_fnSendNotiWaiting(KString &_rclsXml, KString &_rclsNotify)
{
	if (m_eSt != E_TRSG_TR_ST_NOTI_CREATED_SND)
		return;

	m_eSt = E_TRSG_TR_ST_NOTI_WAITING_SND;

	TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();

	AppXmlParser::m_fnMakeJobStatusChangedNotify_Waiting(_rclsXml, _rclsNotify);
	m_fnCallLog(true, _rclsNotify);
	pclsTrans->m_fnXmlSend(m_stTrsgAddr, _rclsNotify);
}
void TrsgTransaction::m_fnSendNotiJobStarted(KString &_rclsXml, unsigned int _unTranscodingList, KString &_rclsNotify)
{
	if (m_eSt != E_TRSG_TR_ST_NOTI_WAITING_SND)
		return;

	m_eSt = E_TRSG_TR_ST_NOTI_JOB_STARTED_SND;

	TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();

	AppXmlParser::m_fnMakeJobStatusChangedNotify_JobStarted(_rclsXml, m_clsServiceName, _unTranscodingList, _rclsNotify);
	m_fnCallLog(true, _rclsNotify);
	pclsTrans->m_fnXmlSend(m_stTrsgAddr, _rclsNotify);
}
void TrsgTransaction::m_fnSendNotiTcStarted(KString &_rclsXml, SourceContent * _pclsSource, TargetContent * _pclsTarget,
		unsigned int _unFail, unsigned int _unSuccess, unsigned int _unTargetIdx, KString &_rclsNotify)
{
	if (m_eSt < E_TRSG_TR_ST_NOTI_JOB_STARTED_SND)
		return;

	m_eSt = E_TRSG_TR_ST_NOTI_TC_STARTED_SND;

	TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();

	AppXmlParser::m_fnMakeJobStatusChangedNotify_TC_Started(_rclsXml, m_clsServiceName, _pclsSource, _pclsTarget, _unFail, _unSuccess, _unTargetIdx, _rclsNotify);
	m_fnCallLog(true, _rclsNotify);
	pclsTrans->m_fnXmlSend(m_stTrsgAddr, _rclsNotify);
}

void TrsgTransaction::m_fnSendNotiTcStopped(KString &_rclsXml, unsigned int _unFail, unsigned int _unSuccess, ETrssCodeSet_t _eSt, KString &_rclsResultDescription, KString &_rclsNotify)
{
	if (m_eSt != E_TRSG_TR_ST_NOTI_TC_STARTED_SND)
		return;

	m_eSt = E_TRSG_TR_ST_NOTI_TC_STOPPED_SND;

	TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();

	AppXmlParser::m_fnMakeJobStatusChangedNotify_TC_Stopped(_rclsXml, _eSt, _rclsResultDescription, _unFail, _unSuccess, _rclsNotify);
	m_fnCallLog(true, _rclsNotify);
	pclsTrans->m_fnXmlSend(m_stTrsgAddr, _rclsNotify);
}
void TrsgTransaction::m_fnSendNotiJobStopped(KString &_rclsXml, KString &_rclsNotify)
{
	if (m_eSt != E_TRSG_TR_ST_NOTI_TC_STOPPED_SND)
		return;

	m_eSt = E_TRSG_TR_ST_NOTI_JOB_STOPPED_SND;

	TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();

	AppXmlParser::m_fnMakeJobStatusChangedNotify_JobStopped(_rclsXml, m_clsServiceName, _rclsNotify);
	m_fnCallLog(true, _rclsNotify);
	pclsTrans->m_fnXmlSend(m_stTrsgAddr, _rclsNotify);
}
void TrsgTransaction::m_fnSendNotiJobDestroy(KString &_rclsXml, unsigned int _unTranscodingList, ETrssCodeSet_t _eSt, KString &_rclsNotify)
{
	if (m_eSt < E_TRSG_TR_ST_REQ_RCV)	return;
	if (m_eSt >= E_TRSG_TR_ST_NOTI_JOB_DESTROYED_SND)	return;

	m_eSt = E_TRSG_TR_ST_NOTI_JOB_DESTROYED_SND;

	TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();

	AppXmlParser::m_fnMakeJobStatusChangedNotify_Destroyed(_rclsXml, m_clsServiceName, _unTranscodingList, _eSt, _rclsNotify);
	m_fnCallLog(true, _rclsNotify);
	pclsTrans->m_fnXmlSend(m_stTrsgAddr, _rclsNotify);
}

void TrsgTransaction::m_fnCallLog(bool bSend, KString _clsXml)
{
	if(g_fnCheckLog(E_LOG_INFO) == false)
		return;

   ELogLevel_t eLv = E_LOG_INFO;

	KString clsLog;
   if (MainConfig::m_fnGetInstance()->m_bXmlFormatter) clsLog.m_fnReSize(4096 + _clsXml.m_unRealLen);
   else clsLog.m_fnReSize(1024 + _clsXml.m_unRealLen);

	char *pszTemp = (KSTR) clsLog;

	KString clsSessionID = m_clsSessionID;
	unsigned int unTid = AppXmlParser::m_fnGetTransactionID(_clsXml);
	KString clsJobID;
	AppXmlParser::m_fnGetJobID(_clsXml, clsJobID);

	KString clsMine;
	KString clsOther;
	KString clsBodyType;
	AppXmlParser::m_fnGetBodyType(_clsXml, clsBodyType);

	KString::m_fnStrnCat(pszTemp, clsLog.m_unLen, "TrsgTr [S:%s][T:%010u][J:%s]", (KCSTR)clsSessionID, unTid, (KCSTR)clsJobID);

	if(bSend)
	{
		AppXmlParser::m_fnGetToFrom(_clsXml, clsOther, clsMine);
		KString::m_fnStrnCat(pszTemp, clsLog.m_unLen, "%s --> %s : %s", (KCSTR) MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR) clsOther, (KCSTR) clsBodyType);
	}
	else
	{
		AppXmlParser::m_fnGetToFrom(_clsXml, clsMine, clsOther);
		KString::m_fnStrnCat(pszTemp, clsLog.m_unLen, "%s <-- %s : %s", (KCSTR) MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR) clsOther, (KCSTR) clsBodyType);
	}
	if(g_fnCheckLog (E_LOG_DEBUG))
	{
		eLv = E_LOG_DEBUG;
		if(MainConfig::m_fnGetInstance()->m_bXmlFormatter)
		{
			KString clsXmlFormat;
			AppXmlParser::m_fnXmlFormatter(_clsXml, clsXmlFormat);
			KString::m_fnStrnCat(pszTemp, clsLog.m_unLen, "\n%s", (KCSTR) clsXmlFormat);
		}
		else
			KString::m_fnStrnCat(pszTemp, clsLog.m_unLen, "\n%s", (KCSTR) _clsXml);
	}
	IFLOG(eLv, pszTemp);
}
