#ifndef VWTRSS_WORKER_H
#define VWTRSS_WORKER_H
#include "queueThread.hxx"
#include "AppIf.h"
#include "ApiEvent.h"
/*TRSG*/
#include "TrsgTransport.h"
#include "TrsgTransactionMgr.h"
#include "TrsgTransactionTimer.h"
#include "TrsgNodeMgr.h"

#include "SessionMgr.h"
#include "SessionTimer.h"
#include "GarbageTimer.h"
#include "idManager.hxx"

class Worker
{
public:
	Worker();
	~Worker();
	static Worker* m_fnGetInstance(unsigned int _unNum);
	static void m_fnPutTrsgEv(AppTrsgEvent *_pclsEv);
	static void m_fnPutTrsgTcStartEv(AppTrsgTcStartEvent *_pclsEv);
	static void m_fnPutTrsgTcStopEv(AppTrsgTcStopEvent *_pclsEv);
	static void m_fnPutTrsgTimerEv(AppTrsgTimerEvent *_pclsEv);
	static void m_fnPutSessionTimerEv(AppSessionTimerEvent *_pclsEv);
	static void m_fnPutGarbageClear(AppGarbageTimerEvent *_pclsEv);
	static void m_fnPutSessionEndEv(AppSessionEndEvent *_pclsEv);

	static Worker *m_pclsMy;
	static unsigned int m_unMax;
	static eSipUtil::IdMgr *m_pWorkerIdMgr;
	unsigned int m_unIdx;
	TrsgTransactionMgr m_clsTrsgTrMgr;
	SessionMgr m_clsSesMgr;
	TrsgTrTimerMgr m_clsTrsgTrTimer;
	SessionTimerMgr m_clsSessionTimer;
	GarbageTimerMgr m_clsGarbageTimer;

private:
	void m_fnStart(unsigned int _unIdx);
	void m_fnProcTrsgCrtJobReq(AppTrsgEvent *_pclsEv); /*pclsSess->m_fnRecvTrseCrtJobReq(_pclsEv->m_clsXml);*/
	void m_fnProcTrsgStatusChangedNotifyCreated(AppTrsgEvent *_pclsEv);
	void m_fnProcTrsgStatusChangedNotifyWaiting(AppTrsgEvent *_pclsEv);
	void m_fnProcTrsgStatusChangedNotifyJobStarted(AppTrsgEvent *_pclsEv);
	void m_fnProcTrsgStatusChangedNotifyTranscodingStarted(AppTrsgTcStartEvent *_pclsEv);
	void m_fnProcTrsgStatusChangedNotifyTranscodingStopped(AppTrsgTcStopEvent *_pclsEv);
	void m_fnProcTrsgStatusChangedNotifyJobStopped(AppTrsgEvent *_pclsEv);
	void m_fnProcTrsgStatusChangedNotifyDestroyed(AppTrsgEvent *_pclsEv);
	void m_fnProcTrsgTrTimeOut(AppTrsgTimerEvent *_pclsEv);
	void m_fnProcSessionTimeOut(AppSessionTimerEvent *_pclsEv);
	void m_fnProcGarbageClear(AppGarbageTimerEvent *_pclsEv);
	void m_fnProcSessionEnd(AppSessionEndEvent *_pclsEv);
	void m_fnProcAllSesClear();
	static void m_fnCallLog(bool bSend, AppBaseEvent *_pclsEv);
	static void m_fnCbkProcess(AppBaseEvent *_pclsEv, void *_pvOwner);
	eSipUtil::QueueThread<AppBaseEvent> *m_pclsQ;
};
Worker* g_fnCreateWorker();
#endif
