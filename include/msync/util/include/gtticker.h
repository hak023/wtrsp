#ifndef _GTTICKER_
#define _GTTICKER_

#include "r_destroyer.h"
#include "r_jthread.h"
#include <sched.h>
#if ( VERINFO_KERNEL_MAJOR > 22 )
	#include <sys/timerfd.h>
#endif

#define GTICK (CGTTicker::GetInstance())
#define DIFF_TIME(tvp,uvp) \
   ((tvp.tv_sec - uvp.tv_sec)*1000 + \
	(tvp.tv_usec - uvp.tv_usec)/1000)

typedef struct 
{
   int timer_fd;
   unsigned long long wakeups_missed;
}periodic_info;

class CGTTicker : public RED::CThread
{
public:
	CGTTicker();
	~CGTTicker();
	inline void GetTick(struct timeval *ptv)
	{
		memcpy(ptv,m_ptv,sizeof(struct timeval));
		if(ptv->tv_usec >= 1000000L)
			ptv->tv_usec = 999999;
	}
	unsigned int GetTickMS()
	{
		return m_nTickMS;
	}
	static CGTTicker* GetInstance();
#if ( VERINFO_KERNEL_MAJOR > 22 )
	int Make_periodic (unsigned int period,periodic_info *info);
	void Wait_period (periodic_info *info);
#endif
protected:
	static CGTTicker* m_pInstance;
	periodic_info* m_pPeriodInfo;
	struct timeval * m_ptv;
	unsigned int m_nTickMS;
	void* ThreadProc();

};

#endif //_GTTICKER_H_
