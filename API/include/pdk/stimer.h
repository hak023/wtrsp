#ifndef __STIMER_H__
#define __STIMER_H__

#include <stdlib.h>
#include <map>
#include <list>

#include "qthread.h"
#include "jthread.h"
#include "eventdefs.h"

#define MAX_STIMER_NUM 2048

typedef struct{
	int nType;
	struct timeval end_tv;
	CEvtQueue *pQueue;
}TIMER_SET,*PTIMER_SET;

typedef std::list<TIMER_SET> listTimerAssoc,*plistTimerAssoc;
typedef listTimerAssoc::iterator intr_listTimerAssoc,*pintr_listTimerAssoc;
typedef std::map<int,intr_listTimerAssoc> mapTimerAssoc,*pmapTimerAssoc;
typedef mapTimerAssoc::iterator intr_mapTimerAssoc,*pintr_mapTimerAssoc;
typedef std::map<CEvtQueue*,pmapTimerAssoc> mapTimerSet,*pmapTimerSet;
typedef mapTimerSet::iterator intr_mapTimerSet,*pintr_mapTimerSet;


class CSimpleTimer : CThread
{
public:
	CSimpleTimer(int nSubType = 0);
	~CSimpleTimer();
	bool Start();
	void Stop();
	bool StartTimer(CEvtQueue *pQueue,int nType,int msec);
	bool StopTimer(CEvtQueue *pQueue,int nType);
	void *ThreadProc();
protected:
   int m_nSubType;
	listTimerAssoc m_listTimerAssoc;
	mapTimerAssoc m_mapTimerAssoc[MAX_STIMER_NUM];
	mapTimerSet m_mapTimerSet;
	int m_nTimerAssocAssignNum;
	CCritSec m_TimerLock;
};

#endif //__STIMER_H__
