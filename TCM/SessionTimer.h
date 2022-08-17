#ifndef SES_SESSION_TIMER_H
#define SES_SESSION_TIMER_H
#include "timerTable.hxx"
#include "string.hxx"

class SessionTimerMgr;
class SessionTimerObj
{
	public:
		SessionTimerObj(){m_pclsOwner=NULL;m_clsSessionID=KNULL;m_unTid=0;}
		~SessionTimerObj(){}
		SessionTimerMgr * m_pclsOwner;
		eSipUtil::KString m_clsSessionID;
		unsigned int m_unTid;
};
class SessionTimerMgr
{
	public:
		SessionTimerMgr();
		~SessionTimerMgr();
		void m_fnSetTimer(const char * _pszSessionID, unsigned int _unTid, unsigned long _ulMs);
		static void fnCbkTimerOut(SessionTimerObj * _pclsData);
		eSipUtil::TimerTable<SessionTimerObj> m_clsTimer;
		void * m_pclsOwner; // Worker
};

#endif
