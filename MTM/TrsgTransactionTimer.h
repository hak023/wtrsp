#ifndef TRSG_TRANSACTION_TIMER_H
#define TRSG_TRANSACTION_TIMER_H
#include "timerTable.hxx"
#include "string.hxx"

class TrsgTrTimerMgr;
class TrsgTrTimerObj
{
	public:
		TrsgTrTimerObj(){m_pclsOwner=NULL;m_clsSessionID=KNULL;m_unTid=0;m_clsJobID=KNULL;}
		~TrsgTrTimerObj(){}
		TrsgTrTimerMgr * m_pclsOwner;
		eSipUtil::KString m_clsSessionID;
		unsigned int m_unTid;
		eSipUtil::KString m_clsJobID;
};
class TrsgTrTimerMgr
{
	public:
		TrsgTrTimerMgr();
		~TrsgTrTimerMgr();
		void m_fnSetTimer(const char * _pszSessionID, unsigned int _unTid, const char * _pszJobID, unsigned long _ulMs);
		static void fnCbkTimerOut(TrsgTrTimerObj * _pclsData);
		eSipUtil::TimerTable<TrsgTrTimerObj> m_clsTimer;
		void * m_pclsOwner; // Worker
};

#endif
