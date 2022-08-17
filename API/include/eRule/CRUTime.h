#ifndef RUTIME_H
#define RUTIME_H
#include "CRuleUtiles.h"
/********************************* Time Wheel Class **************************************/
typedef enum
{
	E_TIME_WHEEL_NONE,
	E_TIME_WHEEL_MONS,
	E_TIME_WHEEL_HOURS,
	E_TIME_WHEEL_MINS,
	E_TIME_WHEEL_SEC,
	E_TIME_WHEEL_MSEC,
}ETimeWheel_t;
typedef struct TimeWheel_t
{
	TimeWheel_t(){}
	void fnClear(){memset(this,0x00,sizeof(TimeWheel_t));}
	unsigned int m_unIndex;
	TimeWheel_t * m_pstPrev;
	TimeWheel_t * m_pstNext;
}TimeWheel_t;
class TimeWheelInfo
{
	public:
		TimeWheelInfo(bool bFirstInit = false);
		~TimeWheelInfo();
		unsigned int fnPlusTime(unsigned int _unBaseHours, unsigned int _unBaseMins,
								unsigned int _unBaseSec, unsigned int _unBaseMSec,
								unsigned int _unPlusHours, unsigned int _unPlusMins, 
								unsigned int _unPlusSec, unsigned int _unPlusMSec);
		unsigned int fnMinorTime(unsigned int _unBaseHours, unsigned int _unBaseMins,
								unsigned int _unBaseSec, unsigned int _unBaseMSec,
								unsigned int _unMinorHours, unsigned int _unMinorMins, 
								unsigned int _unMinorSec, unsigned int _unMinorMSec);
	private:
		unsigned int fnMinorMSec(unsigned int _unMSec);
		unsigned int fnMinorSec(unsigned int _unSec);
		unsigned int fnMinorMin(unsigned int _unMin);
		unsigned int  fnMinorHour(unsigned int _unMin);
		unsigned int fnPlusMSec(unsigned int _unMSec);
		unsigned int fnPlusSec(unsigned int _unSec);
		unsigned int fnPlusMin(unsigned int _unMin);
		unsigned int  fnPlusHour(unsigned int _unHours);
		void fnSetBaseTime(unsigned int _unBaseHours, unsigned int _unBaseMins,
								unsigned int _unBaseSec, unsigned int _unBaseMSec);
		static TimeWheel_t m_stHours[24];
		static TimeWheel_t m_stMins[60];
		static TimeWheel_t m_stSec[60];
		static TimeWheel_t m_stMSec[1000];
		TimeWheel_t *m_pstTmpHours;
		TimeWheel_t *m_pstTmpMins;
		TimeWheel_t *m_pstTmpSec;
		TimeWheel_t *m_pstTmpMSec; 
};
/********************************* Time Info Class ****************************************/
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
typedef enum
{
	E_SEC_SEC = 1,
	E_SEC_MIN = 60,
	E_SEC_HOUR = 3600,
	E_SEC_DAY = 86400,
	E_SEC_WEEK = 604800,
}ESeconds_t;
class RUDailyTime
{
	public:
		RUDailyTime();
		~RUDailyTime();
		RUDailyTime & operator=(RUDailyTime & _rclsSrc)
		{
			m_nYr = _rclsSrc.m_nYr;
			m_nMon = _rclsSrc.m_nMon;
			m_nWk = _rclsSrc.m_nWk;
			m_nDay = _rclsSrc.m_nDay;
			m_nHr = _rclsSrc.m_nHr;
			m_nMin = _rclsSrc.m_nMin;
			m_nSec = _rclsSrc.m_nSec;
			m_lSec = _rclsSrc.m_lSec;
			m_lMSec = _rclsSrc.m_lMSec;
			m_stTime = _rclsSrc.m_stTime;
			m_stTm = _rclsSrc.m_stTm;
			return *this;
		}
		bool fnIsCompareTime(ETimeComp_t _eT, RUDailyTime & _rclsSrc);
		unsigned long fnGetMs();
		unsigned long fnDiffMs(RUDailyTime & _rclsDiff);
		unsigned long fnDiffSec(RUDailyTime & _rclsDiff);
		void fnGetAllTime();
		void fnDiffSec(int _nSec, bool _bMinor);
		void fnGetString(ETimeComp_t _eT,char * _pszResult, unsigned int _unMax);
		void fnGetTime();
		void fnSetTime(int _nYr, int _nMon, int _nDay, int _nHr, int _nMin,int _nSec);
		int m_nYr;
		int m_nMon;
		int m_nWk;
		int m_nDay;
		int m_nHr;
		int m_nMin;
		int m_nSec;
		int m_nMSec;
		unsigned long m_lSec;
		unsigned long m_lMSec;
		time_t m_stTime;
		struct tm m_stTm;	
};
#endif
