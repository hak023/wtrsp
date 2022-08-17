#ifndef CCM_FAIL_CHARGE_STAT_H
#define CCM_FAIL_CHARGE_STAT_H
#include <time.h>
#include <iostream>
#include <fstream>
#include "eSipUtil/thread.hxx"
#include "eSipUtil/lock.hxx"
#include "eSipUtil/define.hxx"
#include "eSipUtil/string.hxx"
extern const char * s_szCcmShmFCCookieStr;
typedef enum
{
	E_CCM_CHARGING_START_REQ = 0,
	E_CCM_CHARGING_START_RSP,
	E_CCM_CHARGING_INTERIM_REQ,
	E_CCM_CHARGING_INTERIM_RSP,
	E_CCM_CHARGING_STOP_REQ,
	E_CCM_CHARGING_STOP_RSP,
	E_CCM_CHARGING_START_TO,
	E_CCM_CHARGING_INTERIM_TO,
	E_CCM_CHARGING_STOP_TO,
	E_CCM_CHARGING_START_REQ_RE,
	E_CCM_CHARGING_INTERIM_REQ_RE,
	E_CCM_CHARGING_STOP_REQ_RE,
	E_CCM_CHARGING_MAX,
}ECCMChargingStat_t;
class CcmFcStatConfig
{
	public:
		enum
		{
			E_CONST_MAX_STAT_PATH_LEN = 100,
			E_CONST_MAX_STAT_NAME_LEN = 50,
		};
		CcmFcStatConfig()
		{
			m_nStatEnable = 1;
			m_nStatShmKey = 0;
			m_unStatIntervalSec = 30;
		}
		~CcmFcStatConfig(){}
		int m_nStatEnable;				      //0:disable, 1:enable
		eSipUtil::KString m_clsStatPath;            // ../stat/sip/
		eSipUtil::KString m_clsStatName;          //   slb_stat
		eSipUtil::KString m_clsStatShmPath;     // ../ipc/shm/
		int m_nStatShmKey;                          //   
		unsigned int m_unStatIntervalSec;	     //  60
};
typedef struct CcmFcStatRspData
{
	CcmFcStatRspData()
	{
		m_fnReset();
	}
	void m_fnReset()
	{
		m_nRspCode = -1;
		m_unCnt = 0;
	}
	int m_nRspCode;
	unsigned int m_unCnt;
}CcmFcStatRspData;
typedef struct CcmFcStatData
{
	unsigned int m_arrCnt[E_CCM_CHARGING_MAX];
	CcmFcStatData()
	{
		m_fnReset();
	}
	void m_fnCounting(ECCMChargingStat_t _eT)
	{
		++(m_arrCnt[_eT]);
	}
	void m_fnReset()
	{
		memset(this,0x00,sizeof(CcmFcStatData));
	}
	char * m_fnStr(char * _pszDebug,unsigned int _unMax)
	{
		bool bExist = false;
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","START_REQ",
																m_arrCnt[E_CCM_CHARGING_START_REQ]);
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","START_RSP",
																m_arrCnt[E_CCM_CHARGING_START_RSP]);
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","INTERIM_REQ",
																m_arrCnt[E_CCM_CHARGING_INTERIM_REQ]);
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","INTERIM_RSP",
																m_arrCnt[E_CCM_CHARGING_INTERIM_RSP]);
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","STOP_REQ",
																m_arrCnt[E_CCM_CHARGING_STOP_REQ]);
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","STOP_RSP",
																m_arrCnt[E_CCM_CHARGING_STOP_RSP]);
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","START_TO",
																m_arrCnt[E_CCM_CHARGING_START_TO]);
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","INTERIM_TO",
																m_arrCnt[E_CCM_CHARGING_INTERIM_TO]);
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","STOP_TO",
																m_arrCnt[E_CCM_CHARGING_STOP_TO]);
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","START_RE",
																m_arrCnt[E_CCM_CHARGING_START_REQ_RE]);
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","INTERIM_RE",
																m_arrCnt[E_CCM_CHARGING_INTERIM_REQ_RE]);
		eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"%-16s: %-10u\n","STOP_RE",
																m_arrCnt[E_CCM_CHARGING_STOP_REQ_RE]);
		for(unsigned int i=0;i<E_CCM_CHARGING_MAX;i++)
		{
			if(m_arrCnt[i] > 0) {bExist = true;break;}
		}
		if(bExist)
		{
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
			eSipUtil::KString::m_fnStrnCat(_pszDebug,_unMax,"                                                       \n");
		}
		return _pszDebug;
	}
}CcmFcStatData;
typedef struct CcmFcStatShm
{	
	void m_fnReset()
	{
		m_cWriteFlag = 0;
		m_stNextWritTime = 0;
		m_arrStatA.m_fnReset();
		m_arrStatB.m_fnReset();
		strncpy(m_szMagicCookie, s_szCcmShmFCCookieStr, sizeof(m_szMagicCookie)-1);
		m_szMagicCookie[sizeof(m_szMagicCookie)-1] = 0x00;
	}
	int m_cWriteFlag;			//0: A, 1:B
	time_t m_stNextWritTime;
	CcmFcStatData m_arrStatA;
	CcmFcStatData m_arrStatB;
	char m_szMagicCookie[50];
}CcmFcStatShm;
class CcmFailStatMgr
{
	public :	
		CcmFailStatMgr(CcmFcStatConfig & _rstStatConfig);
		~CcmFailStatMgr();
		void m_fnCounting(ECCMChargingStat_t _eT);
	private :
		static void m_fnCbProcessStat(CcmFailStatMgr * _pObject);
		void m_fnPollStat();
		void m_fnWriteStat(CcmFcStatData * _arrStatByTuple, time_t _stTime);		
		CcmFcStatConfig  m_rstStatConfig;
		CcmFcStatShm * m_pstStatShm;
		eSipUtil::Mutex m_clsStatLock;
		time_t m_stNextTimeTick;
		eSipUtil::NormalThread< CcmFailStatMgr > m_clsRefreshThread;
};
void g_fnCreateCcmFailChargeStat(const char * _pszProcName);
#endif

