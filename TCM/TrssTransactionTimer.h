#ifndef TRSS_TRANSACTION_TIMER_H
#define TRSS_TRANSACTION_TIMER_H
#include "timerTable.hxx"
#include "string.hxx"

class TrssTrTimerMgr;
class TrssTrTimerObj
{
	public:
		TrssTrTimerObj(){m_pclsOwner=NULL;m_clsSessionID=KNULL;m_clsJobID=KNULL;m_unTid=0;}
		~TrssTrTimerObj(){}
		TrssTrTimerMgr * m_pclsOwner;
		eSipUtil::KString m_clsSessionID;
		eSipUtil::KString m_clsJobID;
		unsigned int m_unTid;
};
class TrssTrTimerMgr
{
	public:
		TrssTrTimerMgr();
		~TrssTrTimerMgr();
		void m_fnSetTimer(const char * _pszSessionID, const char * _pszJobID, unsigned int _unTid, unsigned long _ulMs);
		static void fnCbkTimerOut(TrssTrTimerObj * _pclsData);
		eSipUtil::TimerTable<TrssTrTimerObj> m_clsTimer;
		void * m_pclsOwner; // Worker
};

#endif
