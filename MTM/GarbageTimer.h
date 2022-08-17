#ifndef GARBAGE_TIMER_H
#define GARBAGE_TIMER_H
#include "timerTable.hxx"
#include "string.hxx"

class GarbageTimerMgr;
class GarbageTimerObj
{
	public:
		GarbageTimerObj(){m_unWorkerIdx=0;m_pclsOwner=NULL;}
		~GarbageTimerObj(){}
		GarbageTimerMgr * m_pclsOwner;
		unsigned int m_unWorkerIdx;
};
class GarbageTimerMgr
{
	public:
		GarbageTimerMgr();
		~GarbageTimerMgr();
		void m_fnSetTimer(const unsigned int _unWorkerIdx, unsigned long _ulMs);
		static void fnCbkTimerOut(GarbageTimerObj * _pclsData);
		eSipUtil::TimerTable<GarbageTimerObj> m_clsTimer;
		void * m_pclsOwner; // Worker
};

#endif