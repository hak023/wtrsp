
#include <string.h>

#include "ConfigFile.hxx"
#include "TrsgCdrMgr.h"
#include "main.h"
#include "Directory.hxx"

TrsgCdrMgr::TrsgCdrMgr()
{
	m_pclsCdrThread = NULL;
	m_pclsStm = NULL;
	m_unSysIndex = 0;
	m_nYear = 0;
	m_nMonth = 0;
	m_nDay = 0;
	m_nHour = 0;
	m_nMinute = 0;
	m_nPeriod = 1;
	memset(m_nFileMin,0x00,sizeof(m_nFileMin));

	m_bInit = false;
}
TrsgCdrMgr::~TrsgCdrMgr()
{
	if(m_pclsStm) delete m_pclsStm;
	if(m_bInit)
	{
		m_pclsCdrThread->shutdown();
		m_pclsCdrThread->join();
		delete m_pclsCdrThread;
	}
}
void TrsgCdrMgr::m_fnInit(const char * _pszName, const char * _pszPath, const unsigned int _unSysIndex, const int _nPeriod)
{
	memset(m_nFileMin,0x00,sizeof(m_nFileMin));
	m_clsName = _pszName; m_clsPath = _pszPath; m_unSysIndex = _unSysIndex; m_nPeriod = _nPeriod;

	int nfileMin = 0;
	for(int i = 0; i<60; i++)//current min
	{
		m_nFileMin[i] = nfileMin;
		nfileMin += m_nPeriod;//Period
		if(nfileMin >= 60) break;
	}

	Directorys::m_fnCatPath(m_clsPath);
	m_pclsCdrThread = new QueueThread<TrsgCdrData>;
	m_pclsCdrThread->setObject(this);
	m_pclsCdrThread->setMaxQueueSize(1000);
	m_pclsCdrThread->setProcessCb(m_fnCbProcessCdr);
	m_pclsCdrThread->setQueueWaitMs(1000);//1s wait
	m_pclsCdrThread->setName(_pszName);
	m_pclsCdrThread->run();
	m_bInit = true;
	IFLOG(E_LOG_INFO, "%s:%s: Initialize Cdr",(KCSTR)m_clsName,__func__);
}
bool TrsgCdrMgr::m_fnAddCdr(TrsgCdrData * _pclsData)
{
	if(m_bInit == false) return false;
	if( !_pclsData )
	{
		IFLOG(E_LOG_ERR, "%s:%s: _pclsData is Null",(KCSTR)m_clsName,__func__);
		return false;
	}
	if( m_pclsCdrThread->put(_pclsData) )
	{
		return true;
	}
	else
	{
		IFLOG(E_LOG_ERR, "%s:%s: Queue Put Fail",(KCSTR)m_clsName,__func__);
		delete _pclsData;
		return false;
	}
}
void TrsgCdrMgr::m_fnCbProcessCdr(TrsgCdrData * _pstTrsgData, void * _pObject)
{
	if( !_pObject )
	{
		IFLOG(E_LOG_ERR, "%s:%s: _pObject is NULL","NULL",__func__);
		if(_pstTrsgData) delete _pstTrsgData;
		return;
	}
	TrsgCdrMgr & rclsCdrMgr = *((TrsgCdrMgr *)_pObject);
	struct timespec stCurrentTs;clock_gettime(CLOCK_REALTIME, &stCurrentTs);struct tm tmTime;
	localtime_r(&(stCurrentTs.tv_sec), &tmTime);
	if( !rclsCdrMgr.m_pclsStm 
		|| rclsCdrMgr.m_nYear != tmTime.tm_year  || rclsCdrMgr.m_nMonth != tmTime.tm_mon 
		|| rclsCdrMgr.m_nDay != tmTime.tm_mday || rclsCdrMgr.m_nHour != tmTime.tm_hour 
		|| rclsCdrMgr.m_nMinute != rclsCdrMgr.m_nFileMin[tmTime.tm_min/rclsCdrMgr.m_nPeriod] )//로그 파일 생성 주기별로 계산
	{
		if(rclsCdrMgr.m_pclsStm) delete rclsCdrMgr.m_pclsStm;
		char szDir[256];memset(szDir,0x00,256);
		snprintf(szDir, 255, "%s%04d%02d%02d",(KCSTR)rclsCdrMgr.m_clsPath, 
											tmTime.tm_year + 1900, 
											tmTime.tm_mon+1, 
											tmTime.tm_mday);
		CfgFile::m_fnCreateDir(szDir);
		char szFullPath[256];memset(szFullPath,0x00,256);
		snprintf(szFullPath, 255, "%s/%s.%03d.%04d%02d%02d%02d%02d.log", szDir, (KCSTR)rclsCdrMgr.m_clsName, rclsCdrMgr.m_unSysIndex,
				tmTime.tm_year + 1900, tmTime.tm_mon+1, tmTime.tm_mday, tmTime.tm_hour, rclsCdrMgr.m_nFileMin[tmTime.tm_min/rclsCdrMgr.m_nPeriod]);
		rclsCdrMgr.m_pclsStm = new std::ofstream(szFullPath, std::ios_base::out | std::ios_base::app);
		if( !rclsCdrMgr.m_pclsStm )
		{
			IFLOG(E_LOG_ERR, "%s:%s: can't create ofstream : %02d%02d",
															(KCSTR)rclsCdrMgr.m_clsName,__func__,
															tmTime.tm_hour, tmTime.tm_min);
			if(_pstTrsgData) delete _pstTrsgData;
			return;
		}
		rclsCdrMgr.m_nYear = tmTime.tm_year;
		rclsCdrMgr.m_nMonth = tmTime.tm_mon;
		rclsCdrMgr.m_nDay = tmTime.tm_mday;
		rclsCdrMgr.m_nHour = tmTime.tm_hour;
		//rclsCdrMgr.m_nMinute = tmTime.tm_min;
		rclsCdrMgr.m_nMinute = rclsCdrMgr.m_nFileMin[tmTime.tm_min/rclsCdrMgr.m_nPeriod];

	}
	if(_pstTrsgData)
	{
		rclsCdrMgr.m_fnWriteCdr(*(rclsCdrMgr.m_pclsStm), *_pstTrsgData);
		delete _pstTrsgData;
	}
}
void TrsgCdrMgr::m_fnWriteCdr(std::ofstream & m_rclsStm, TrsgCdrData & _rstData)
{
	m_rclsStm<<(KCSTR)_rstData.m_clsBuff;
	m_rclsStm<<std::endl;
}

