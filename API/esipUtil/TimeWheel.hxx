
#ifndef DEF_UTIL_TIME_WHEEL
#define DEF_UTIL_TIME_WHEEL

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <iterator>
#include <list>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

namespace eSipUtil
{
#define DEF_ESIP_TIMESLOT    1000   // 1000 ms 
class UtilTimeBase;
typedef void (*PfuncUtilTimeOut)(void *,UtilTimeBase *);    // user object, time out object
typedef std::list<unsigned long> UtilTimeList_t;
typedef std::map<unsigned long, unsigned long> UtilTimeMap_t;
/************************* Time Wheel TimeOut Object Class ***********************************/
class UtilTimeBase
{
	public:
		UtilTimeBase();
		virtual ~UtilTimeBase();
};
/************************* Time Wheel TimeOut List Class *************************************/
class UtilTimeList
{
	public:
		UtilTimeList();
		~UtilTimeList();
		bool fnPushBack(UtilTimeBase * _pclsTime);
		unsigned int fnClear(void * _pvUser,PfuncUtilTimeOut _pfn);
		UtilTimeBase * fnBegin();
		UtilTimeBase * fnNext();
		unsigned int fnTimeOut(void * _pvUser,PfuncUtilTimeOut _pfn);
		UtilTimeList_t m_listTr;
		unsigned int m_unCount;
	private:
		UtilTimeList_t::iterator m_listItor;
};
/************************* Time Wheel Slot Class *******************************************/
class UtilTimeSlot    // per ms
{
	public:
		UtilTimeSlot();
		~UtilTimeSlot();
		UtilTimeList * fnFindMap(unsigned long _nKey);
		UtilTimeList * fnSetMap(unsigned long _nKey);
		unsigned int fnDelMap(void * _pvUser,unsigned long _nKey,PfuncUtilTimeOut _pfn);
		unsigned int  fnClear(void * _pvUser,PfuncUtilTimeOut _pfn);
		UtilTimeList * fnBegin();
		UtilTimeList * fnNext();
		unsigned int fnTimeOut(void * _pvUser,unsigned long _ulKey,PfuncUtilTimeOut _pfn);
		UtilTimeMap_t m_mapR;
	private:
		UtilTimeMap_t::iterator m_mapItorInt;
};
/************************* Time Wheel Class **********************************************/
class UtilTimeWheel
{
	public:
		UtilTimeWheel();
		~UtilTimeWheel();
		inline static unsigned long fnGetCurrentMs()
		{
			#if 0   // change get time func
			struct timeval curtv;gettimeofday(&curtv,NULL);
			unsigned long ulSec = (curtv.tv_sec*1000); 
			unsigned long ulMsec = (curtv.tv_usec/1000);
			return (ulSec + ulMsec);
			#else
			struct timespec stCurrentTs;
			clock_gettime(CLOCK_REALTIME, &stCurrentTs);
			unsigned long ulSec = (unsigned long)(stCurrentTs.tv_sec*1000);
			unsigned long ulMsec = (unsigned long)(stCurrentTs.tv_nsec/1000000);
			return (ulSec + ulMsec);
			#endif
		}
		void fnAddTime(UtilTimeBase * _pclsObj, unsigned long msOffset);
		unsigned int fnProcessTimeOut(void * _pvUser,PfuncUtilTimeOut _pfn);
		unsigned int m_unTotalMapCount;
		unsigned int m_unTotalTrCount;
	private:
		inline UtilTimeSlot * fnAssignSlot(unsigned long _ulFutureMs)
		{return &m_clsSlot[_ulFutureMs%DEF_ESIP_TIMESLOT];}
		unsigned long m_ulNextMs;
		UtilTimeSlot m_clsSlot[DEF_ESIP_TIMESLOT];
		void * m_pvUser;
};

}

#endif

