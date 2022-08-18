#ifndef TRAFFIC_COUNTER_HXX
#define TRAFFIC_COUNTER_HXX
#include "TimeWheel.hxx"
#include "lock.hxx"
#include "string.hxx"

namespace eSipUtil
{
/*********************************** Traffic Slice ****************************************/
class TrafficSlice
{
	public:
		TrafficSlice();
		~TrafficSlice();
		void m_fnIncre(unsigned long _ulCnt);
		void m_fnDebug(KString & _rclsDebug);
		unsigned int m_unIdx;
		unsigned long m_ulCnt;
		TrafficSlice * m_pclsPrev;
		TrafficSlice * m_pclsNext;
};
/*********************************** Traffic Counter **************************************/
class TrafficCounter
{
	public:
		TrafficCounter();
		~TrafficCounter();
		//==========> Initalize , (10 Sec Buffer, 100 ms Slice, 1 Sec Duration)
		bool m_fnInit(unsigned int _unMaxCircleMs=10000,             // max circle buff time ms , ex> 10,000 ms
					unsigned int _unSliceMs=100,                     // slice time ms , ex> 10 ms
					unsigned int _unTrafficDurationMs=1000);    // counting durations ms , ex> 1000 
		//==========> Change Duration Ms
		bool m_fnChangeDuration(unsigned int _unTrafficDurationMs);
		//==========> Increment
		void m_fnIncre(unsigned long _ulCnt);
		//==========> Get Sum
		unsigned long m_fnGetSumPerDuration();
		void m_fnClearNextSlice();
		//==========> Debug Screen (View Traffic Slices)
		void m_fnDebug(KString &_rclsDebug);
		unsigned int m_unMaxCircleMs;
		unsigned int m_unSliceMs;
		unsigned int m_unTrafficDurationMs;
		unsigned int m_unMaxCircleNums;
		unsigned int m_unTrafficCalNums;
	private:
		TrafficSlice * m_fnCreateSlice(unsigned int _unNums);
		TrafficSlice * m_fnGetCurSlice();
		RwMutex m_clsLock;
		bool m_bInit;
		TrafficSlice * m_pclsSlice;
};
}
#endif
