#ifndef VWTRSG_WORKER_H
#define VWTRSG_WORKER_H
#include "queueThread.hxx"
#include "AppIf.h"
#include "ApiEvent.h"
/*TRSS*/
#include "TrssTransport.h"
#include "TrssTransactionMgr.h"
#include "TrssTransactionTimer.h"
#include "TrssNodeMgr.h"
/*TRSE*/
#include "TrseTransport.h"
#include "TrseTransactionMgr.h"
//#include "TrseTransactionTimer.h"

#include "SessionMgr.h"
#include "SessionTimer.h"
#include "GarbageTimer.h"

class Worker
{
	public:
		Worker();
		~Worker();
		static Worker * m_fnGetInstance(unsigned int _unNum);
		static void m_fnPutTrseEv(AppTrseEvent * _pclsEv);
		static void m_fnPutTrssEv(AppTrssEvent * _pclsEv);
		static void m_fnPutTrssTimerEv(AppTrssTimerEvent * _pclsEv);
		static void m_fnPutSessionTimerEv(AppSessionTimerEvent * _pclsEv);
		static void m_fnPutGarbageClear(AppGarbageTimerEvent * _pclsEv);
		static void m_fnPutTrseDisconSesClear(AppTrseDisconEvent * _pclsEv);
		static void m_fnPutSessionEndEv(AppSessionEndEvent * _pclsEv);
		static Worker * m_pclsMy;
		static unsigned int m_unMax;
		unsigned int m_unIdx;
		TrseTransactionMgr m_clsTrseTrMgr;
		TrssTransactionMgr m_clsTrssTrMgr;

		SessionMgr m_clsSesMgr;
		TrssTrTimerMgr m_clsTrssTrTimer;

		SessionTimerMgr m_clsSessionTimer;
		GarbageTimerMgr m_clsGarbageTimer;
	private:
		void m_fnStart(unsigned int _unIdx);
		void m_fnProcTrseCrtJobReq(AppTrseEvent * _pclsEv);
		void m_fnProcTrssStatusChangedNotifyCreated(AppTrssEvent * _pclsEv);
		void m_fnProcTrssStatusChangedNotifyWaiting(AppTrssEvent * _pclsEv);
		void m_fnProcTrssStatusChangedNotifyJobStarted(AppTrssEvent * _pclsEv);
		void m_fnProcTrssStatusChangedNotifyTranscodingStarted(AppTrssEvent * _pclsEv);
		void m_fnProcTrssStatusChangedNotifyTranscodingStopped(AppTrssEvent * _pclsEv);
		void m_fnProcTrssStatusChangedNotifyJobStopped(AppTrssEvent * _pclsEv);
		void m_fnProcTrssStatusChangedNotifyDestroyed(AppTrssEvent * _pclsEv);
		//void m_fnProcTrseTrTimeOut(AppTrseTimerEvent * _pclsEv);//use??
		void m_fnProcTrssTrTimeOut(AppTrssTimerEvent * _pclsEv);
		void m_fnProcSessionTimeOut(AppSessionTimerEvent * _pclsEv);
		void m_fnProcGarbageClear(AppGarbageTimerEvent * _pclsEv);
		void m_fnProcTrseDisconSesClear(AppTrseDisconEvent * _pclsEv);
		void m_fnProcSessionEnd(AppSessionEndEvent * _pclsEv);
		void m_fnProcAllSesClear();
		void m_fnCallLog(bool bSend, AppBaseEvent * _pclsEv);
		static void m_fnCbkProcess(AppBaseEvent * _pclsEv, void * _pvOwner);
		eSipUtil::QueueThread<AppBaseEvent> * m_pclsQ;
};
Worker * g_fnCreateWorker();
#endif
