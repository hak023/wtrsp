#include "DailyTime.h"

using namespace eSipUtil;

DailyTime::DailyTime(){m_fnUpdateTime();}
DailyTime::~DailyTime(){}
void DailyTime::m_fnDiffSec(int _nSec, bool _bMinor)
{
	unsigned int unTime = 0;
	if(_bMinor) unTime = ((unsigned int)m_stTime-_nSec);
	else unTime = ((unsigned int)m_stTime+_nSec);
	time_t stTime = (time_t)unTime;
	localtime_r(&stTime,&m_stTm);
	m_nYr = m_stTm.tm_year + 1900;
	m_nMon = m_stTm.tm_mon +1;
	m_nWk = m_stTm.tm_wday + 1;
	m_nDay = m_stTm.tm_mday;
	m_nHr = m_stTm.tm_hour;
	m_nMin = m_stTm.tm_min;
	m_nSec = m_stTm.tm_sec;
}
void DailyTime::m_fnDiffMin(int _nMin,bool _bMinor)
{
	m_fnDiffSec(_nMin*E_SEC_MIN,_bMinor);
}
void DailyTime::m_fnDiffHour(int _nHour,bool _bMinor)
{
	m_fnDiffSec(_nHour*E_SEC_HOUR,_bMinor);
}
void DailyTime::m_fnDiffDay(int _nDay,bool _bMinor)
{
	m_fnDiffSec(_nDay*E_SEC_DAY,_bMinor);
}
void DailyTime::m_fnDiffWeek(int _nWeek,bool _bMinor)
{
	m_fnDiffSec(_nWeek*E_SEC_WEEK,_bMinor);
}
void DailyTime::m_fnUpdateTime()
{
	m_stTime  = time(NULL);
	m_fnGetTime();
}
void DailyTime::m_fnGetTime()
{
	localtime_r(&m_stTime,&m_stTm);
	m_nYr = m_stTm.tm_year + 1900;
	m_nMon = m_stTm.tm_mon +1;
	m_nWk = m_stTm.tm_wday + 1;
	m_nDay = m_stTm.tm_mday;
	m_nHr = m_stTm.tm_hour;
	m_nMin = m_stTm.tm_min;
	m_nSec = m_stTm.tm_sec;
}
void DailyTime::m_fnGetTimetToStr(unsigned int _unTime, eSipUtil::KString & _rclsStrTime)
{   
   const time_t stTime = (time_t)_unTime;
   struct tm lt;
   localtime_r(&stTime, &lt);
    
   KString::m_fnStrnCat((KSTR)_rclsStrTime, _rclsStrTime.m_unLen, "%04d-%02d-%02d %02d:%02d:%02d"
         , lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec);
}
void DailyTime::m_fnSetTime(int _nYr, int _nMon, int _nDay, int _nHr, int _nMin,int _nSec)
{
	m_stTm.tm_year = _nYr - 1900;
	m_stTm.tm_mon = _nMon -1;
	m_stTm.tm_mday = _nDay;
	m_stTm.tm_hour = _nHr;
	m_stTm.tm_min = _nMin;
	m_stTm.tm_sec = _nSec;
	m_stTime =  mktime(&m_stTm);
	m_fnGetTime();
}
void DailyTime::m_fnSetTimeFromTime_t(unsigned int _unTime)
{
	time_t stTime = (time_t)_unTime;
	struct tm stTm;	
	localtime_r(&stTime,&stTm);
	m_fnSetTime(stTm.tm_year + 1900,stTm.tm_mon +1,stTm.tm_mday,
		        stTm.tm_hour,stTm.tm_min,stTm.tm_sec);
}
bool DailyTime::m_fnIsCompareTime(ETimeComp_t _eT, DailyTime & _rclsSrc)
{
	if(_eT >= E_TIME_CMP_YEAR)
	{
		if(m_nYr != _rclsSrc.m_nYr) return false;
	}
	if(_eT >= E_TIME_CMP_MON)
	{
		if(m_nMon != _rclsSrc.m_nMon) return false;
	}
	if(_eT >= E_TIME_CMP_WEEK)
	{
		if(m_nWk != _rclsSrc.m_nWk) return false;
	}
	if(_eT >= E_TIME_CMP_DAY)
	{
		if(m_nDay != _rclsSrc.m_nDay) return false;
	}
	if(_eT >= E_TIME_CMP_HOUR)
	{
		if(m_nHr != _rclsSrc.m_nHr) return false;
	}
	if(_eT >= E_TIME_CMP_MIN)
	{
		if(m_nMin != _rclsSrc.m_nMin) return false;
	}
	if(_eT >= E_TIME_CMP_SEC)
	{
		if(m_nSec != _rclsSrc.m_nSec) return false;
	}
	return true;
}
