#include "Worker.h"
#include "main.h"
#include "MainConfig.h"
#include "AppXmlParser.h"

using namespace eSipUtil;

Worker* g_fnCreateWorker()
{
	MainConfig *pclsConf = MainConfig::m_fnGetInstance();
	return Worker::m_fnGetInstance(pclsConf->m_unWorkerNum);
}
Worker *Worker::m_pclsMy = NULL;
unsigned int Worker::m_unMax = 0;
eSipUtil::IdMgr *Worker::m_pWorkerIdMgr = new IdMgr(MainConfig::m_fnGetInstance()->m_unWorkerIdMgrNum);
Worker::Worker()
{
	m_pclsQ = NULL;
	m_unIdx = 0;
	m_clsTrsgTrMgr.m_pclsOwner = (void*) this;
	m_clsSesMgr.m_pclsOwner = (void*) this;
	m_clsTrsgTrTimer.m_pclsOwner = (void*) this;
	m_clsSessionTimer.m_pclsOwner = (void*) this;
	m_clsGarbageTimer.m_pclsOwner = (void*) this;
}
Worker::~Worker(){}
Worker* Worker::m_fnGetInstance(unsigned int _unNum)
{
	m_unMax = _unNum;
	m_pclsMy = new Worker[_unNum];
	for (unsigned int i = 0; i < _unNum; i++)
	{
		m_pclsMy[i].m_fnStart(i);
		m_pclsMy[i].m_clsGarbageTimer.m_fnSetTimer(i, MainConfig::m_fnGetInstance()->m_unGarbageIntervalTmr);
	}
	return m_pclsMy;
}
void Worker::m_fnStart(unsigned int _unIdx)
{
	m_unIdx = _unIdx;
	m_pclsQ = new eSipUtil::QueueThread<AppBaseEvent>;
	m_pclsQ->setName("Worker");
	m_pclsQ->setMaxQueueSize(MainConfig::m_fnGetInstance()->m_unWorkerQueueCnt);
	m_pclsQ->setObject(this);
	m_pclsQ->setProcessCb(m_fnCbkProcess);
	m_pclsQ->run();
}
void Worker::m_fnPutTrsgEv(AppTrsgEvent *_pclsEv)
{
	unsigned int unTid = (KUINT) _pclsEv->m_unTid;
	if (unTid == 0)
		delete _pclsEv;
	else
	{
		unsigned int unDecision = unTid % m_unMax; //동일한 TID는 동일한 Worker에 할당 되도록 함.
		if (m_pclsMy[unDecision].m_pclsQ->put(_pclsEv) == false)
		{
			delete _pclsEv;
		}
	}
}
void Worker::m_fnPutTrsgTcStartEv(AppTrsgTcStartEvent *_pclsEv)
{
	unsigned int unTid = (KUINT) _pclsEv->m_unTid;
	if (unTid == 0)
		delete _pclsEv;
	else
	{
		unsigned int unDecision = unTid % m_unMax; //동일한 TID는 동일한 Worker에 할당 되도록 함.
		if (m_pclsMy[unDecision].m_pclsQ->put(_pclsEv) == false)
		{
			delete _pclsEv;
		}
	}
}
void Worker::m_fnPutTrsgTcStopEv(AppTrsgTcStopEvent *_pclsEv)
{
	unsigned int unTid = (KUINT) _pclsEv->m_unTid;
	if (unTid == 0)
		delete _pclsEv;
	else
	{
		unsigned int unDecision = unTid % m_unMax; //동일한 TID는 동일한 Worker에 할당 되도록 함.
		if (m_pclsMy[unDecision].m_pclsQ->put(_pclsEv) == false)
		{
			delete _pclsEv;
		}
	}
}
void Worker::m_fnPutTrsgTimerEv(AppTrsgTimerEvent *_pclsEv)
{
	unsigned int unTid = (KUINT) _pclsEv->m_unTid;
	unsigned int unDecision = unTid % m_unMax;
	if (m_pclsMy[unDecision].m_pclsQ->put(_pclsEv) == false)
	{
		delete _pclsEv;
	}
}
void Worker::m_fnPutSessionTimerEv(AppSessionTimerEvent *_pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	unsigned int unDecision = unTid % m_unMax;
	if (m_pclsMy[unDecision].m_pclsQ->put(_pclsEv) == false)
	{
		delete _pclsEv;
	}
}
void Worker::m_fnPutGarbageClear(AppGarbageTimerEvent *_pclsEv) //Garbage Check timer
{
	unsigned int unDecision = _pclsEv->m_unWorkerIdx;
	if (m_pclsMy[unDecision].m_pclsQ->put(_pclsEv) == false)
	{
		delete _pclsEv;
	}
}
void Worker::m_fnPutSessionEndEv(AppSessionEndEvent *_pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	unsigned int unDecision = unTid % m_unMax;
	if (m_pclsMy[unDecision].m_pclsQ->put(_pclsEv) == false)
	{
		delete _pclsEv;
	}
}
void Worker::m_fnCbkProcess(AppBaseEvent *_pclsEv, void *_pvOwner)
{
	Worker *pclsMy = (Worker*) _pvOwner;
	if (_pclsEv->m_eT == E_MAIN_EV_TRSG_CREATE_JOB_REQ)
	{
		pclsMy->m_fnProcTrsgCrtJobReq((AppTrsgEvent*) _pclsEv);
	}
	else if (_pclsEv->m_eT == E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_CREATED)
	{
		pclsMy->m_fnProcTrsgStatusChangedNotifyCreated((AppTrsgEvent*) _pclsEv);
	}
	else if (_pclsEv->m_eT == E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_WAITING)
	{
		pclsMy->m_fnProcTrsgStatusChangedNotifyWaiting((AppTrsgEvent*) _pclsEv);
	}
	else if (_pclsEv->m_eT == E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED)
	{
		pclsMy->m_fnProcTrsgStatusChangedNotifyJobStarted((AppTrsgEvent*) _pclsEv);
	}
	else if (_pclsEv->m_eT == E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED)
	{
		pclsMy->m_fnProcTrsgStatusChangedNotifyTranscodingStarted((AppTrsgTcStartEvent*) _pclsEv);
	}
	else if (_pclsEv->m_eT == E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED)
	{
		pclsMy->m_fnProcTrsgStatusChangedNotifyTranscodingStopped((AppTrsgTcStopEvent*) _pclsEv);
	}
	else if (_pclsEv->m_eT == E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED)
	{
		pclsMy->m_fnProcTrsgStatusChangedNotifyJobStopped((AppTrsgEvent*) _pclsEv);
	}
	else if (_pclsEv->m_eT == E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED)
	{
		pclsMy->m_fnProcTrsgStatusChangedNotifyDestroyed((AppTrsgEvent*) _pclsEv);
	}
	else if (_pclsEv->m_eT == E_MAIN_EV_TRSG_TR_TIMER)
	{
		pclsMy->m_fnProcTrsgTrTimeOut((AppTrsgTimerEvent*) _pclsEv);
	}
	else if (_pclsEv->m_eT == E_MAIN_EV_SESSION_TIMEOUT)
	{
		pclsMy->m_fnProcSessionTimeOut((AppSessionTimerEvent*) _pclsEv);
	}
	else if (_pclsEv->m_eT == E_MAIN_EV_GARBAGE_TIMER)
	{
		pclsMy->m_fnProcGarbageClear((AppGarbageTimerEvent*) _pclsEv);
	}
	else if (_pclsEv->m_eT == E_MAIN_EV_SESSION_END)
	{
		pclsMy->m_fnProcSessionEnd((AppSessionEndEvent*) _pclsEv);
	}
	else
	{
		IFLOG(E_LOG_ERR,"UNKNOWN Event WorkerQueue(%d)", _pclsEv->m_eT);
	}

	delete _pclsEv;
}
void Worker::m_fnProcTrsgCrtJobReq(AppTrsgEvent *_pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

//	IFLOG(E_LOG_ERR, "LHJ TEST File:%s, Line:%d, Func:%s", __FILE__, __LINE__, __func__);

	// Session Full 이면 JobNotify_Destroyed 메시지 전송
	if (m_clsSesMgr.m_fnIsOverload())
	{
		m_fnCallLog(false, _pclsEv);
		IFLOG(E_LOG_ERR, "Session IsOverload(E_JOB_IS_LIMITED) [Key:%s-%d-%s]", (KSTR) clsSessionID, unTid, (KSTR) clsJobID);

		unsigned int unTranscodingList = AppXmlParser::m_fnGetTranscodingList(_pclsEv->m_clsXml);

		KString clsServiceName;
		AppXmlParser::m_fnGetServiceName(_pclsEv->m_clsXml, clsServiceName);

		KString clsJobStatusChangedNotify;
		clsJobStatusChangedNotify.m_fnReSize(10240);

		AppXmlParser::m_fnMakeJobStatusChangedNotify_Destroyed(_pclsEv->m_clsXml, clsServiceName, unTranscodingList, E_JOB_IS_LIMITED, clsJobStatusChangedNotify);

		// Destroyed 메시지 전송
		TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();
		pclsTrans->m_fnXmlSend(_pclsEv->m_stAddr, clsJobStatusChangedNotify);
		IFLOG(E_LOG_ERR, "Session IsOverLoad Response[%s]", (KCSTR) clsJobStatusChangedNotify);
		return;
	}

	TrsgTransaction *pclsTrsgTr = m_clsTrsgTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTrsgTr)
	{
		IFLOG(E_LOG_ERR, "TrsgTransaction Error[Exist TrsgTransaction Key:%s-%d-%s]", (KSTR) clsSessionID, unTid, (KSTR) clsJobID);
		return;
	}
	pclsTrsgTr = m_clsTrsgTrMgr.m_fnSet(unTid, clsJobID, clsSessionID); //new trsg transaction
	if (pclsTrsgTr == NULL)
	{
		IFLOG(E_LOG_ERR, "TrsgTransaction Create Fail[Key:%s-%d-%s]", (KSTR) clsSessionID, unTid, (KSTR) clsJobID);
		return;
	}
	pclsTrsgTr->m_stTrsgAddr = _pclsEv->m_stAddr;

	Session *pclsSess = m_clsSesMgr.m_fnSessionFind(unTid, clsJobID, clsSessionID);
	if (pclsSess)
	{
		IFLOG(E_LOG_ERR, "Duplicated Session Key : %s-%d-%s", (KCSTR) clsSessionID, unTid, (KSTR) clsJobID);
		m_clsSesMgr.m_fnTrsgDel(unTid, clsJobID, clsSessionID);

	}
	pclsSess = m_clsSesMgr.m_fnSessionSet(unTid, clsJobID, clsSessionID, (void*) pclsTrsgTr);
	if (pclsSess == NULL)
	{
		IFLOG(E_LOG_ERR, "Session Create Fail[Key:%s-%d-%s]", (KSTR) clsSessionID, unTid, (KSTR) clsJobID);
		m_clsTrsgTrMgr.m_fnDel(unTid, clsJobID, clsSessionID);
		return;
	}

   // Worker IdMgr Full
	if (pclsSess->m_nAllocWorker == -1)
	{
		m_fnCallLog(false, _pclsEv);
		IFLOG(E_LOG_ERR, "ID_MGR Full(E_JOB_IS_LIMITED) [Key:%s-%d-%s : %d]", (KSTR) clsSessionID, unTid, (KSTR) clsJobID, Worker::m_pWorkerIdMgr->getAvailableIdNum());
		// Session Delete
		m_clsSesMgr.m_fnTrsgDel(unTid, clsJobID, clsSessionID);

		KString clsServiceName;
		AppXmlParser::m_fnGetServiceName(_pclsEv->m_clsXml, clsServiceName);

		unsigned int unTranscodingList = AppXmlParser::m_fnGetTranscodingList(_pclsEv->m_clsXml);

		KString clsJobStatusChangedNotify;
		clsJobStatusChangedNotify.m_fnReSize(10240);
		AppXmlParser::m_fnMakeJobStatusChangedNotify_Destroyed(_pclsEv->m_clsXml, clsServiceName, unTranscodingList, E_JOB_IS_LIMITED, clsJobStatusChangedNotify);

		// Destroyed 메시지 전송
		TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();
		pclsTrans->m_fnXmlSend(_pclsEv->m_stAddr, clsJobStatusChangedNotify);
		IFLOG(E_LOG_ERR, "TC Request Overload Response[%s]", (KCSTR) clsJobStatusChangedNotify);
		return;
	}
	pclsSess->m_clsJobID = clsJobID;

	m_fnCallLog(false, _pclsEv);
	pclsSess->m_fnRecvTrsgCrtJobReq(_pclsEv->m_clsXml);

}
void Worker::m_fnProcTrsgStatusChangedNotifyCreated(AppTrsgEvent *_pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

//	IFLOG(E_LOG_ERR, "LHJ TEST File:%s, Line:%d, Func:%s", __FILE__, __LINE__, __func__);

	TrsgTransaction *pclsTrsgTr = m_clsTrsgTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTrsgTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trsg(Tid:%d, JobID:%s, SessionID:%s) TrsgTrTimeOut (Not Exist Trsg Tr) ", unTid, (KCSTR) clsJobID, (KCSTR) clsSessionID);
		return;
	}

	Session *pclsSess = m_clsSesMgr.m_fnSessionFind(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID);
	if(pclsSess)
	{
		pclsSess->m_fnSendTrsgJobChgNotiCreated();
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Sess<->Trsg(Tid:%d, JobID:%s, SessionID:%s", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrsgStatusChangedNotifyWaiting(AppTrsgEvent *_pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

//	IFLOG(E_LOG_ERR, "LHJ TEST File:%s, Line:%d, Func:%s", __FILE__, __LINE__, __func__);

	TrsgTransaction *pclsTrsgTr = m_clsTrsgTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTrsgTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trsg(Tid:%d, JobID:%s, SessionID:%s) TrsgTrTimeOut (Not Exist Trsg Tr) ", unTid, (KCSTR) clsJobID, (KCSTR) clsSessionID);
		return;
	}

	Session *pclsSess = m_clsSesMgr.m_fnSessionFind(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID);
	if(pclsSess)
	{
		pclsSess->m_fnSendTrsgJobChgNotiWaiting();
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Sess<->Trsg(Tid:%d, JobID:%s, SessionID:%s", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrsgStatusChangedNotifyJobStarted(AppTrsgEvent *_pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

//	IFLOG(E_LOG_ERR, "LHJ TEST File:%s, Line:%d, Func:%s", __FILE__, __LINE__, __func__);

	TrsgTransaction *pclsTrsgTr = m_clsTrsgTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTrsgTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trsg(Tid:%d, JobID:%s, SessionID:%s) TrsgTrTimeOut (Not Exist Trsg Tr) ", unTid, (KCSTR) clsJobID, (KCSTR) clsSessionID);
		return;
	}

	Session *pclsSess = m_clsSesMgr.m_fnSessionFind(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID);
	if(pclsSess)
	{
		pclsSess->m_fnSendTrsgJobChgNotiJobStarted();
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Sess<->Trsg(Tid:%d, JobID:%s, SessionID:%s", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrsgStatusChangedNotifyTranscodingStarted(AppTrsgTcStartEvent *_pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

//	IFLOG(E_LOG_ERR, "LHJ TEST File:%s, Line:%d, Func:%s", __FILE__, __LINE__, __func__);

	TrsgTransaction *pclsTrsgTr = m_clsTrsgTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTrsgTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trsg(Tid:%d, JobID:%s, SessionID:%s) TrsgTrTimeOut (Not Exist Trsg Tr) ", unTid, (KCSTR) clsJobID, (KCSTR) clsSessionID);
		return;
	}

	Session *pclsSess = m_clsSesMgr.m_fnSessionFind(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID);
	if(pclsSess)
	{
		pclsSess->m_fnSendTrsgJobChgNotiTcStarted(_pclsEv->m_pclsTargetContent, _pclsEv->m_unFail, _pclsEv->m_unSuccess, _pclsEv->m_unTargetIdx);
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Sess<->Trsg(Tid:%d, JobID:%s, SessionID:%s", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrsgStatusChangedNotifyTranscodingStopped(AppTrsgTcStopEvent *_pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

//	IFLOG(E_LOG_ERR, "LHJ TEST File:%s, Line:%d, Func:%s", __FILE__, __LINE__, __func__);

	TrsgTransaction *pclsTrsgTr = m_clsTrsgTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTrsgTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trsg(Tid:%d, JobID:%s, SessionID:%s) TrsgTrTimeOut (Not Exist Trsg Tr) ", unTid, (KCSTR) clsJobID, (KCSTR) clsSessionID);
		return;
	}

	Session *pclsSess = m_clsSesMgr.m_fnSessionFind(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID);
	if(pclsSess)
	{
		pclsSess->m_fnSendTrsgJobChgNotiTcStopped(_pclsEv->m_unFail, _pclsEv->m_unSuccess, _pclsEv->m_eTcSt, _pclsEv->m_clsResultDescription);
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Sess<->Trsg(Tid:%d, JobID:%s, SessionID:%s", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrsgStatusChangedNotifyJobStopped(AppTrsgEvent *_pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

//	IFLOG(E_LOG_ERR, "LHJ TEST File:%s, Line:%d, Func:%s", __FILE__, __LINE__, __func__);

	TrsgTransaction *pclsTrsgTr = m_clsTrsgTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTrsgTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trsg(Tid:%d, JobID:%s, SessionID:%s) TrsgTrTimeOut (Not Exist Trsg Tr) ", unTid, (KCSTR) clsJobID, (KCSTR) clsSessionID);
		return;
	}

	Session *pclsSess = m_clsSesMgr.m_fnSessionFind(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID);
	if(pclsSess)
	{
		pclsSess->m_fnSendTrsgJobChgNotiJobStopped();
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Sess<->Trsg(Tid:%d, JobID:%s, SessionID:%s", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrsgStatusChangedNotifyDestroyed(AppTrsgEvent *_pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

//	IFLOG(E_LOG_ERR, "LHJ TEST File:%s, Line:%d, Func:%s", __FILE__, __LINE__, __func__);

	TrsgTransaction *pclsTrsgTr = m_clsTrsgTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTrsgTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trsg(Tid:%d, JobID:%s, SessionID:%s) TrsgTrTimeOut (Not Exist Trsg Tr) ", unTid, (KCSTR) clsJobID, (KCSTR) clsSessionID);
		return;
	}

	Session *pclsSess = m_clsSesMgr.m_fnSessionFind(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID);
	if(pclsSess)
	{
		pclsSess->m_fnSendTrsgJobChgNotiJobDestroyed();
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Sess<->Trsg(Tid:%d, JobID:%s, SessionID:%s", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}

void Worker::m_fnProcTrsgTrTimeOut(AppTrsgTimerEvent *_pclsEv)
{
	IFLOG(E_LOG_INFO, "<---- %s ----- APP", g_fnGetAppString(_pclsEv->m_eT));
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

	TrsgTransaction *pclsTrsgTr = m_clsTrsgTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTrsgTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trsg(Tid:%d, JobID:%s, SessionID:%s) TrsgTrTimeOut (Not Exist Trsg Tr) ", unTid, (KCSTR) clsJobID, (KCSTR) clsSessionID);
		return;
	}

	Session *pclsSess = m_clsSesMgr.m_fnSessionFind(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID);
	if(pclsSess == NULL)
	{
		IFLOG(E_LOG_ERR, "Not Exist Call Trsg Tr Timer.");
		return;
	}
}
void Worker::m_fnProcSessionTimeOut(AppSessionTimerEvent *_pclsEv)
{
	IFLOG(E_LOG_INFO, "<---- %s ----- APP", g_fnGetAppString(_pclsEv->m_eT));
	Session *pclsSess = m_clsSesMgr.m_fnSessionFind(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID);
	if(pclsSess == NULL)
	{
		IFLOG(E_LOG_ERR, "Session(Tid:%d, JobID:%s, SessionID:%s)Not Exist Session (SessionTimeOut)", _pclsEv->m_unTid, (KCSTR)_pclsEv->m_clsJobID, (KCSTR)_pclsEv->m_clsSessionID);
		return;
	}
	else
	{
		pclsSess->m_fnRecvSessionTimeOut();
	}

	if(m_clsSesMgr.m_fnTrsgDel(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID) == false)
	{
		IFLOG(E_LOG_ERR, "Session(Tid:%d, JobID:%s, SessionID:%s)SesMgrMap Fail (SessionTimeOut)", _pclsEv->m_unTid, (KCSTR)_pclsEv->m_clsJobID, (KCSTR)_pclsEv->m_clsSessionID);
	}
}
void Worker::m_fnProcGarbageClear(AppGarbageTimerEvent *_pclsEv)
{
	m_clsSesMgr.m_fnGarbageClear();
	m_clsGarbageTimer.m_fnSetTimer(_pclsEv->m_unWorkerIdx, MainConfig::m_fnGetInstance()->m_unGarbageIntervalTmr);
}
void Worker::m_fnProcSessionEnd(AppSessionEndEvent *_pclsEv)
{
	IFLOG(E_LOG_INFO, "<---- %s ----- APP", g_fnGetAppString(_pclsEv->m_eT));
	Session *pclsSess = m_clsSesMgr.m_fnSessionFind(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID);
	if (pclsSess == NULL)
	{
		IFLOG(E_LOG_ERR, "Session(Tid:%d, JobID:%s, SessionID:%s)Not Exist Session (SessionEnd)", _pclsEv->m_unTid, (KCSTR)_pclsEv->m_clsJobID, (KCSTR)_pclsEv->m_clsSessionID);
		return;
	}

	if(m_clsSesMgr.m_fnTrsgDel(_pclsEv->m_unTid, _pclsEv->m_clsJobID, _pclsEv->m_clsSessionID) == false)
	{
		IFLOG(E_LOG_ERR, "Session(Tid:%d, JobID:%s, SessionID:%s)SesMgrMap Fail (SessionEnd)", _pclsEv->m_unTid, (KCSTR)_pclsEv->m_clsJobID, (KCSTR)_pclsEv->m_clsSessionID);
	}
}
void Worker::m_fnCallLog(bool bSend, AppBaseEvent *_pclsEv)
{
	if (g_fnCheckLog(E_LOG_INFO) == false)
		return;

	ELogLevel_t eLv = E_LOG_INFO;
	KString clsLog;
	char *pszTemp = (KSTR)clsLog;

	KString clsSessionID;
	unsigned int unTid;
	KString clsJobID;
	KString clsXml;

	switch(_pclsEv->m_eT)
	{
		case E_MAIN_EV_TRSG_CREATE_JOB_REQ:
		case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_CREATED:
		case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_WAITING:
		case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED:
		case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED:
		case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED:
		case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED:
		case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED:
		{
			AppTrsgEvent * pclsEv = (AppTrsgEvent*)_pclsEv;
			clsSessionID = pclsEv->m_clsSessionID;
			unTid = pclsEv->m_unTid;
			clsJobID = pclsEv->m_clsJobID;
			clsXml = pclsEv->m_clsXml;
			break;
		}
		default :
			return;
	};

	KString clsMine;
	KString clsOther;
	KString::m_fnStrnCat(pszTemp, clsLog.m_unLen, "[S:%s][T:%010u][J:%s] ", (KCSTR)clsSessionID, unTid, (KCSTR) clsJobID);

	if (bSend)
	{
		AppXmlParser::m_fnGetToFrom(clsXml, clsOther, clsMine);
		KString::m_fnStrnCat(pszTemp, clsLog.m_unLen, "%s --> %s : %s", (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR) clsOther, g_fnGetAppString(_pclsEv->m_eT));
	}
	else
	{
		AppXmlParser::m_fnGetToFrom(clsXml, clsMine, clsOther);
		KString::m_fnStrnCat(pszTemp, clsLog.m_unLen, "%s <-- %s : %s", (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR) clsOther, g_fnGetAppString(_pclsEv->m_eT));
	}
	IFLOG(eLv, pszTemp);
}
