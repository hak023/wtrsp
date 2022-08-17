#ifndef DAILY_TIME_H
#define DAILY_TIME_H
#include "timeUtil.hxx"
#include "string.hxx"

class DailyTime
{
	public:
		enum
		{
			E_SEC_SEC = 1,
			E_SEC_MIN = 60,
			E_SEC_HOUR = 3600,
			E_SEC_DAY = 86400,
			E_SEC_WEEK = 604800,
		};
		typedef enum
		{
			E_TIME_CMP_NONE = 0,
			E_TIME_CMP_YEAR,
			E_TIME_CMP_MON,
			E_TIME_CMP_WEEK,
			E_TIME_CMP_DAY,
			E_TIME_CMP_HOUR,
			E_TIME_CMP_MIN,
			E_TIME_CMP_SEC,
			E_TIME_CMP_MSEC
		}ETimeComp_t;
		DailyTime();
		~DailyTime();
		void m_fnDiffSec(int _nSec, bool _bMinor);
		void m_fnDiffMin(int _nMin,bool _bMinor);
		void m_fnDiffHour(int _nHour,bool _bMinor);
		void m_fnDiffDay(int _nDay,bool _bMinor);
		void m_fnDiffWeek(int _nWeek,bool _bMinor);
		void m_fnUpdateTime();
		void m_fnGetTime();
		void m_fnGetTimetToStr(unsigned int _unTime, eSipUtil::KString & _rclsStrTime);
		void m_fnSetTime(int _nYr, int _nMon, int _nDay, int _nHr, int _nMin,int _nSec);
		void m_fnSetTimeFromTime_t(unsigned int _unTime);
		bool m_fnIsCompareTime(ETimeComp_t _eT, DailyTime & _rclsSrc);
		int m_nYr;
		int m_nMon;
		int m_nWk;
		int m_nDay;
		int m_nHr;
		int m_nMin;
		int m_nSec;
		time_t m_stTime;
		struct tm m_stTm;	
};

#endif
