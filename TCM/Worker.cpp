#include "Worker.h"
#include "main.h"
#include "MainConfig.h"
#include "AppXmlParser.h"

using namespace eSipUtil;

Worker * g_fnCreateWorker()
{
	MainConfig * pclsConf = MainConfig::m_fnGetInstance();
	return Worker::m_fnGetInstance(pclsConf->m_unWorkerNum);
}
Worker * Worker::m_pclsMy = NULL;
unsigned int Worker::m_unMax = 0;
//(Worker Num ) * (IdMgrNum) = (Total Session)
eSipUtil::IdMgr *Worker::m_pWorkerIdMgr = new IdMgr( MainConfig::m_fnGetInstance()->m_unWorkerNum * (MainConfig::m_fnGetInstance()->m_unIdMgrNum));
Worker::Worker()
{
	m_pclsQ = NULL;m_unIdx = 0; 
	m_clsTrseTrMgr.m_pclsOwner = (void*)this;
	m_clsTrssTrMgr.m_pclsOwner = (void*)this;
	m_clsSesMgr.m_pclsOwner = (void*)this;
	m_clsTrssTrTimer.m_pclsOwner = (void*)this;
	m_clsSessionTimer.m_pclsOwner = (void*)this;
	m_clsGarbageTimer.m_pclsOwner = (void*)this;
}
Worker::~Worker(){}
Worker * Worker::m_fnGetInstance(unsigned int _unNum)
{
	m_unMax = _unNum;
	m_pclsMy = new Worker[_unNum];
	for(unsigned int i=0;i<_unNum;i++)
	{
		m_pclsMy[i].m_fnStart(i);
		m_pclsMy[i].m_clsGarbageTimer.m_fnSetTimer(i, MainConfig::m_fnGetInstance()->m_unGarbageInvervalTmr);
	}
	return m_pclsMy;
}
void Worker::m_fnStart(unsigned int _unIdx)
{
	m_unIdx = _unIdx;
	KString clsName;
	clsName << "Worker_" << m_unIdx;
	m_pclsQ = new eSipUtil::QueueThread<AppBaseEvent>;
	m_pclsQ->setName((KCSTR)clsName);
	m_pclsQ->setMaxQueueSize(MainConfig::m_fnGetInstance()->m_unWorkerQueueCnt);
	m_pclsQ->setObject(this);
	m_pclsQ->setProcessCb(m_fnCbkProcess);
	m_pclsQ->run();

}
void Worker::m_fnPutTrseEv(AppTrseEvent * _pclsEv)
{
	unsigned int unTid = (KUINT)_pclsEv->m_unTid;
	if(unTid == 0 )
		delete _pclsEv;
	else
	{
		unsigned int nDecision = unTid%m_unMax;//동일한 TID는 동일한 Worker에 할당 되도록 함.
		if(m_pclsMy[nDecision].m_pclsQ->put(_pclsEv)==false)
		{
			delete _pclsEv;
		}
	}
}
void Worker::m_fnPutTrssEv(AppTrssEvent * _pclsEv)
{
	unsigned int unTid = (KUINT)_pclsEv->m_unTid;
	if(unTid == 0 )
		delete _pclsEv;
	else
	{
		unsigned int nDecision = unTid%m_unMax;//동일한 TID는 동일한 Worker에 할당 되도록 함.
		if(m_pclsMy[nDecision].m_pclsQ->put(_pclsEv)==false)
		{
			delete _pclsEv;
		}
	}
}
void Worker::m_fnPutTrssTimerEv(AppTrssTimerEvent * _pclsEv)
{
	unsigned int unTid = (KUINT)_pclsEv->m_unTid;
	unsigned int unDecision = unTid%m_unMax;//동일한 TID는 동일한 Worker에 할당 되도록 함.
	if(m_pclsMy[unDecision].m_pclsQ->put(_pclsEv)==false)
	{
		delete _pclsEv;
	}
}
void Worker::m_fnPutSessionTimerEv(AppSessionTimerEvent * _pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	unsigned int unDecision = unTid%m_unMax;//동일한 TID는 동일한 Worker에 할당 되도록 함.
	if(m_pclsMy[unDecision].m_pclsQ->put(_pclsEv)==false)
	{
		delete _pclsEv;
	}
}
void Worker::m_fnPutGarbageClear(AppGarbageTimerEvent * _pclsEv)//Garbage Check timer
{
	unsigned int unDecision = _pclsEv->m_unWorkerIdx;
	if(m_pclsMy[unDecision].m_pclsQ->put(_pclsEv)==false)
	{
		delete _pclsEv;
	}
}
void Worker::m_fnPutTrseDisconSesClear(AppTrseDisconEvent * _pclsEv)
{
	unsigned int unDecision = _pclsEv->m_unWorkerIdx;
	if(m_pclsMy[unDecision].m_pclsQ->put(_pclsEv)==false)
	{
		delete _pclsEv;
	}
}
void Worker::m_fnPutSessionEndEv(AppSessionEndEvent * _pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	unsigned int unDecision = unTid%m_unMax;//동일한 TID는 동일한 Worker에 할당 되도록 함.
	if(m_pclsMy[unDecision].m_pclsQ->put(_pclsEv)==false)
	{
		delete _pclsEv;
	}
}
void Worker::m_fnCbkProcess(AppBaseEvent * _pclsEv, void * _pvOwner)
{
	Worker * pclsMy = (Worker*)_pvOwner;
	if(_pclsEv->m_eT == E_MAIN_EV_TRSE_CREATE_JOB_REQ)
	{
		pclsMy->m_fnProcTrseCrtJobReq((AppTrseEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_CREATED)
	{
		pclsMy->m_fnProcTrssStatusChangedNotifyCreated((AppTrssEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_WAITING)
	{
		pclsMy->m_fnProcTrssStatusChangedNotifyWaiting((AppTrssEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED)
	{
		pclsMy->m_fnProcTrssStatusChangedNotifyJobStarted((AppTrssEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED)
	{
		pclsMy->m_fnProcTrssStatusChangedNotifyTranscodingStarted((AppTrssEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED)
	{
		pclsMy->m_fnProcTrssStatusChangedNotifyTranscodingStopped((AppTrssEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED)
	{
		pclsMy->m_fnProcTrssStatusChangedNotifyJobStopped((AppTrssEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED)
	{
		pclsMy->m_fnProcTrssStatusChangedNotifyDestroyed((AppTrssEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_TRSS_TR_TIMER)//Trse로 부터 CrtJobReq 전송 시 Timer 시작.(Created 수신 만료 시간)
	{
		pclsMy->m_fnProcTrssTrTimeOut((AppTrssTimerEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_SESSION_TIMEOUT)//Trse로 부터 CrtJobReq 수신 시 Timer 시작.
	{
		pclsMy->m_fnProcSessionTimeOut((AppSessionTimerEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_GARAGE_TIMER)
	{
		pclsMy->m_fnProcGarbageClear((AppGarbageTimerEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_TRSE_DISCON)
	{
		pclsMy->m_fnProcTrseDisconSesClear((AppTrseDisconEvent*)_pclsEv);
	}
	else if(_pclsEv->m_eT == E_MAIN_EV_SESSION_END)
	{
		pclsMy->m_fnProcSessionEnd((AppSessionEndEvent*)_pclsEv);
	}
	delete _pclsEv;
}
void Worker::m_fnProcTrseCrtJobReq(AppTrseEvent * _pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;

   if(clsSessionID.m_unRealLen == 0)
   {
      IFLOG(E_LOG_ERR,"SessionID Len is zero. [Key:%s-%d]", (KSTR)clsSessionID, unTid);
      return;
   }
	if(m_clsSesMgr.m_fnIsOverload())//해당 Woker에 세션이 Full 일 경우
	{
		_pclsEv->m_clsJobID = (KCSTR)g_fnGetUUID();
		m_fnCallLog(false, _pclsEv);
		IFLOG(E_LOG_ERR,"Session IsOverload(E_JOB_IS_LIMITED) [Key:%s-%d]", (KSTR)clsSessionID, unTid);
		CTrsgCdrInfo * pclsCTrsgCdrInfo = new CTrsgCdrInfo;//CDR 기록을 위해 임시 생성.
		if(pclsCTrsgCdrInfo)
		{
			KString clsTrsgIP = _pclsEv->m_stAddr.m_szLocalIp;
			KString clsClientIP = _pclsEv->m_stAddr.m_szRemoteIp;
			KString clsTrsgName, clsTrseName;
			AppXmlParser::m_fnGetToFrom(_pclsEv->m_clsXml, clsTrsgName, clsTrseName);
			unsigned int unTranscodingList = AppXmlParser::m_fnGetTranscodingList(_pclsEv->m_clsXml);
			//수신 시간 설정.
			pclsCTrsgCdrInfo->m_fnRecvTrseCrtJobReq(_pclsEv->m_clsXml, clsSessionID, clsTrsgIP, clsClientIP, unTranscodingList);

			KString clsCreateJobRes; clsCreateJobRes.m_fnReSize(10240);
			KString clsJobID = (KCSTR)_pclsEv->m_clsJobID;
			//에러 응답 메시지 생성.
			AppXmlParser::m_fnMakeCreateJobRes(_pclsEv->m_clsXml, clsTrseName/*To*/, clsTrsgName/*From*/, clsJobID, E_JOB_IS_LIMITED, clsCreateJobRes);

			//에러 응답 전송
			TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
			pclsTrans->m_fnXmlSend(_pclsEv->m_stAddr, clsCreateJobRes);
			IFLOG(E_LOG_ERR, "IsOverLoad Response[%s]", (KCSTR)clsCreateJobRes);

			pclsCTrsgCdrInfo->m_fnSendTrseCrtJobRes(clsCreateJobRes, E_JOB_IS_LIMITED);//Trsg code 설정.
			pclsCTrsgCdrInfo->m_fnMakeCdrList(_pclsEv->m_clsJobID);//write
			delete pclsCTrsgCdrInfo;
		}
		return;
	}

	TrseTransaction * pclsTrseTr = m_clsTrseTrMgr.m_fnFind(unTid, clsSessionID);
	if(pclsTrseTr)
	{
		IFLOG(E_LOG_ERR,"TrseTransaction Error[Exist TrseTransaction Key:%s-%d]", (KSTR)clsSessionID, unTid);
		return;
	}
	pclsTrseTr = m_clsTrseTrMgr.m_fnSet(unTid, clsSessionID);//new trse transaction
	if(pclsTrseTr == NULL)
	{
		IFLOG(E_LOG_ERR,"TrseTransaction Create Fail[Key:%s-%d]", (KSTR)clsSessionID, unTid);
		return;
	}

	pclsTrseTr->m_stTrseAddr = _pclsEv->m_stAddr;//Trse Addr set

	Session * pclsSess = m_clsSesMgr.m_fnSessionFind(unTid, clsSessionID);
	if (pclsSess)
	{
		//이미 존재하면 Old Session 및  Old Trse Tr 삭제 및 신규 생성
		IFLOG(E_LOG_ERR, "Duplicated Session Key : %s-%d", (KCSTR)clsSessionID, unTid);
		m_clsSesMgr.m_fnTrseDel(unTid, clsSessionID);
	}
	//신규 Session 생성 및 TrseTr과 맵핑
	pclsSess = m_clsSesMgr.m_fnSessionSet(unTid, clsSessionID, (void*) pclsTrseTr);
	if (pclsSess == NULL)
	{
		IFLOG(E_LOG_ERR,"Session Create Fail[Key:%s-%d]", (KSTR)clsSessionID, unTid);
		m_clsTrseTrMgr.m_fnDel(unTid, clsSessionID);
		return;
	}

	pclsTrseTr->m_pvSession = (void*)pclsSess;
	_pclsEv->m_clsJobID = pclsSess->m_clsJobID;
	//TrssTr 은 Session 에서 Node 상태 확인 후 생성
	m_fnCallLog(false, _pclsEv);
	pclsSess->m_fnRecvTrseCrtJobReq(_pclsEv->m_clsXml);
}
void Worker::m_fnProcTrssStatusChangedNotifyCreated(AppTrssEvent * _pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

	TrssTransaction * pclsTr = m_clsTrssTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trss Error(Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
	m_fnCallLog(false, _pclsEv);
	Session * pclsSess = (Session*)pclsTr->m_pvSession;
	if(pclsSess)
	{
		pclsSess->m_fnRecvTrssJobChgNotiCreated(_pclsEv->m_clsXml);
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Trss<->Sess (Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrssStatusChangedNotifyWaiting(AppTrssEvent * _pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

	TrssTransaction * pclsTr = m_clsTrssTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trss Error(Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
	m_fnCallLog(false, _pclsEv);
	Session * pclsSess = (Session*)pclsTr->m_pvSession;
	if(pclsSess)
	{
		pclsSess->m_fnRecvTrssJobChgNotiWaiting(_pclsEv->m_clsXml);
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Trss<->Sess (Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrssStatusChangedNotifyJobStarted(AppTrssEvent * _pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

	TrssTransaction * pclsTr = m_clsTrssTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trss Error(Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
	m_fnCallLog(false, _pclsEv);
	Session * pclsSess = (Session*)pclsTr->m_pvSession;
	if(pclsSess)
	{
		pclsSess->m_fnRecvTrssJobChgNotiJobStarted(_pclsEv->m_clsXml);
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Trss<->Sess (Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrssStatusChangedNotifyTranscodingStarted(AppTrssEvent * _pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

	TrssTransaction * pclsTr = m_clsTrssTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trss Error(Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
	m_fnCallLog(false, _pclsEv);
	Session * pclsSess = (Session*)pclsTr->m_pvSession;
	if(pclsSess)
	{
		pclsSess->m_fnRecvTrssJobChgNotiTcStarted(_pclsEv->m_clsXml);
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Trss<->Sess (Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrssStatusChangedNotifyTranscodingStopped(AppTrssEvent * _pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

	TrssTransaction * pclsTr = m_clsTrssTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trss Error(Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
	m_fnCallLog(false, _pclsEv);
	Session * pclsSess = (Session*)pclsTr->m_pvSession;
	if(pclsSess)
	{
		pclsSess->m_fnRecvTrssJobChgNotiTcStopped(_pclsEv->m_clsXml);
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Trss<->Sess (Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrssStatusChangedNotifyJobStopped(AppTrssEvent * _pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

	TrssTransaction * pclsTr = m_clsTrssTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trss Error(Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
	m_fnCallLog(false, _pclsEv);
	Session * pclsSess = (Session*)pclsTr->m_pvSession;
	if(pclsSess)
	{
		pclsSess->m_fnRecvTrssJobChgNotiJobStopped(_pclsEv->m_clsXml);
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Trss<->Sess (Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrssStatusChangedNotifyDestroyed(AppTrssEvent * _pclsEv)
{
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

	TrssTransaction * pclsTr = m_clsTrssTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTr == NULL)
	{
		IFLOG(E_LOG_ERR, "Trss Error(Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
	m_fnCallLog(false, _pclsEv);
	Session * pclsSess = (Session*)pclsTr->m_pvSession;
	if(pclsSess)
	{
		pclsSess->m_fnRecvTrssJobChgNotiJobDestroyed(_pclsEv->m_clsXml);

		//_pclsEv는 Trss의 Session ID값이므로, Trse의 Session ID 값을 찾아서 Del 해야 한다.
		if (m_clsSesMgr.m_fnTrseDel(unTid, pclsSess->m_clsSessionID) == false)
		{
			IFLOG(E_LOG_ERR, "Session Delete Fail(Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)pclsSess->m_clsJobID, (KCSTR)pclsSess->m_clsSessionID);
		}
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session link Error Trss<->Sess (Tid:%d, JobID:%s, SessionID:%s)", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcTrssTrTimeOut(AppTrssTimerEvent * _pclsEv)
{
	//TRSS 에게 JobStatusChangedNotify(Created) 수신하지 못함.(Trss Tr Timeout)
	IFLOG(E_LOG_INFO, "<---- %s(not recv Created msg) ----- APP", g_fnGetAppString(_pclsEv->m_eT));
	unsigned int unTid = _pclsEv->m_unTid;
	KString clsSessionID = _pclsEv->m_clsSessionID;
	KString clsJobID = _pclsEv->m_clsJobID;

	TrssTransaction * pclsTr = m_clsTrssTrMgr.m_fnFind(unTid, clsJobID, clsSessionID);
	if (pclsTr == NULL)
	{
		IFLOG(E_LOG_INFO, "Trss(Tid:%d, JobID:%s, SessionID:%s) TrssTrTimeOut (Not Exist Trss Tr) ", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}

	Session * pclsSess = (Session*)pclsTr->m_pvSession;
	if(pclsSess)
	{
		pclsSess->m_fnRecvTrssTrTimeout();
	}
	else
	{
		IFLOG(E_LOG_ERR, "Session(Tid:%d, JobID:%s, SessionID:%s) link Error Trss<->Sess ", unTid, (KCSTR)clsJobID, (KCSTR)clsSessionID);
		return;
	}
}
void Worker::m_fnProcSessionTimeOut(AppSessionTimerEvent * _pclsEv)
{
	IFLOG(E_LOG_INFO, "<---- %s ----- APP", g_fnGetAppString(_pclsEv->m_eT));
	Session * pclsSes = m_clsSesMgr.m_fnSessionFind( _pclsEv->m_unTid, _pclsEv->m_clsSessionID);
	if(pclsSes == NULL)
	{
		IFLOG(E_LOG_INFO, "Session(Tid:%d, SessionID:%s)Not Exist Session (SessionTimeOut)", _pclsEv->m_unTid, (KCSTR)_pclsEv->m_clsSessionID);
		return;
	}
	else
	{
		pclsSes->m_fnRecvSessionTimeOut();
	}

	if(m_clsSesMgr.m_fnTrseDel(_pclsEv->m_unTid, _pclsEv->m_clsSessionID)==false)
	{
		IFLOG(E_LOG_ERR, "Session(Tid:%d, SessionID:%s)SesMgrMap Fail (SessionTimeOut)", _pclsEv->m_unTid, (KCSTR)_pclsEv->m_clsSessionID);
	}
}
void Worker::m_fnProcGarbageClear(AppGarbageTimerEvent * _pclsEv)
{
	m_clsSesMgr.m_fnGarbageClear();
	m_clsGarbageTimer.m_fnSetTimer(_pclsEv->m_unWorkerIdx, MainConfig::m_fnGetInstance()->m_unGarbageInvervalTmr);
}
void Worker::m_fnProcTrseDisconSesClear(AppTrseDisconEvent * _pclsEv)
{
	m_clsSesMgr.m_fnTrseDisconSesClear(_pclsEv->m_clsSessionID);
}
void Worker::m_fnProcSessionEnd(AppSessionEndEvent * _pclsEv)
{
	IFLOG(E_LOG_INFO, "<---- %s ----- APP", g_fnGetAppString(_pclsEv->m_eT));
	Session * pclsSes = m_clsSesMgr.m_fnSessionFind( _pclsEv->m_unTid, _pclsEv->m_clsSessionID);
	if(pclsSes == NULL)
	{
		IFLOG(E_LOG_ERR, "Session(Tid:%d, SessionID:%s)Not Exist Session (SessionEnd)", _pclsEv->m_unTid, (KCSTR)_pclsEv->m_clsSessionID);
		return;
	}

	if(m_clsSesMgr.m_fnTrseDel(_pclsEv->m_unTid, _pclsEv->m_clsSessionID)==false)
	{
		IFLOG(E_LOG_ERR, "Session(Tid:%d, SessionID:%s)SesMgrMap Fail (SessionEnd)", _pclsEv->m_unTid, (KCSTR)_pclsEv->m_clsSessionID);
	}
}
void Worker::m_fnCallLog(bool bSend, AppBaseEvent * _pclsEv)
{
	if(g_fnCheckLog(E_LOG_INFO) == false ) return;

	ELogLevel_t eLv = E_LOG_INFO;
	KString clsLog;
	char * pszTemp = (KSTR)clsLog;

	KString clsSessionID;
	unsigned int unTid;
	KString clsJobID;
	KString clsXml;

	switch(_pclsEv->m_eT)
	{
		case E_MAIN_EV_TRSE_CREATE_JOB_REQ:
		{
			AppTrseEvent * pclsEv = (AppTrseEvent*)_pclsEv;
			clsXml = pclsEv->m_clsXml;
			clsSessionID = pclsEv->m_clsSessionID;
			unTid = pclsEv->m_unTid;
			clsJobID = pclsEv->m_clsJobID;
			clsXml = pclsEv->m_clsXml;
			break;
		}
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_CREATED:
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_WAITING:
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED:
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED:
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED:
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED:
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED:
		{
			AppTrssEvent * pclsEv = (AppTrssEvent*)_pclsEv;
			clsXml = pclsEv->m_clsXml;
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
	KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"Worker [S:%s][T:%010u][J:%s] ",(KCSTR)clsSessionID,unTid,(KCSTR)clsJobID);

	if(bSend)
	{
		AppXmlParser::m_fnGetToFrom(clsXml, clsOther, clsMine);
		KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"%s --> %s : %s", (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR)clsOther, g_fnGetAppString(_pclsEv->m_eT));
	}
	else
	{
		AppXmlParser::m_fnGetToFrom(clsXml, clsMine, clsOther);
		KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"%s <-- %s : %s", (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR)clsOther, g_fnGetAppString(_pclsEv->m_eT));
	}
	IFLOG(eLv, pszTemp);
}
