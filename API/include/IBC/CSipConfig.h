#ifndef CSIPCONFIG_H
#define  CSIPCONFIG_H
#include "eRule/RuleInterface.h"
#include "eSipCommand.hxx"
#include "eRte/eSipRteApi.hxx"
#include "IBCDefine.h"
typedef enum
{
	E_SIP_APP_TIMER_LOG_LEVEL = 0,
	E_SIP_APP_TIMER_T1,
	E_SIP_APP_TIMER_T2,
	E_SIP_APP_TIMER_T4,
	E_SIP_APP_TIMER_TC,
	E_SIP_APP_TIMER_TD,
	E_SIP_APP_TIMER_TG,
	E_SIP_APP_TIMER_TH,
	E_SIP_APP_TIMER_DIALOG_ALIVE,
	E_SIP_APP_TIMER_DIALOG_TERM,
	E_SIP_APP_TIMER_REL,
	E_SIP_APP_TIMER_REL_PRACK,
	E_SIP_APP_TIMER_STAT_INTERVAL,
}ESipAppTimer_t;
extern const char * g_arrSipTimer[E_SIP_APP_TIMER_STAT_INTERVAL+1];
typedef struct IbcStack_t
{
	IbcStack_t(){m_fnClear();}
	void m_fnClear()
	{
		m_nLogLevel=-1;m_nT1=-1;m_nT2=-1;m_nT4=-1;m_nTC=-1;m_nTD=-1;
		m_nTG=-1;m_nTH=-1;m_nDialogAlive=-1;m_nDialogTerm=-1;m_nRel=-1;
		m_nRelPrack=-1;
	}
	void m_fnDebug(RUString & _rclsDebug)
	{
		_rclsDebug<<"<<<STACK: LogLevel="<<RUIntString(m_nLogLevel)<<
					", T1="<<RUIntString(m_nT1)<<
					", T2="<<RUIntString(m_nT2)<<
					", T4="<<RUIntString(m_nT4)<<
					", TC="<<RUIntString(m_nTC)<<
					", TD="<<RUIntString(m_nTD)<<
					", TG="<<RUIntString(m_nTG)<<
					", TH="<<RUIntString(m_nTH)<<
					", DialogAlive="<<RUIntString(m_nDialogAlive)<<
					", Rel="<<RUIntString(m_nRel)<<
					", RelPrack="<<RUIntString(m_nRelPrack)<<
					"\r\n";
	}
	IbcStack_t & operator=(IbcStack_t & _rstSrc)
	{
		if(_rstSrc.m_nLogLevel > -1)     m_nLogLevel = _rstSrc.m_nLogLevel          ;
		if(_rstSrc.m_nT1 > -1)           m_nT1 = _rstSrc.m_nT1                      ;
		if(_rstSrc.m_nT2 > -1)           m_nT2 = _rstSrc.m_nT2                      ;
		if(_rstSrc.m_nT4 > -1)           m_nT4 = _rstSrc.m_nT4                      ;
		if(_rstSrc.m_nTC > -1)           m_nTC = _rstSrc.m_nTC                      ;
		if(_rstSrc.m_nTD > -1)           m_nTD = _rstSrc.m_nTD                      ;
		if(_rstSrc.m_nTG > -1)           m_nTG = _rstSrc.m_nTG                      ;
		if(_rstSrc.m_nTH > -1)           m_nTH = _rstSrc.m_nTH                      ;
		if(_rstSrc.m_nDialogAlive > -1)  m_nDialogAlive = _rstSrc.m_nDialogAlive    ;
		if(_rstSrc.m_nDialogTerm > -1)   m_nDialogTerm = _rstSrc.m_nDialogTerm      ;
		if(_rstSrc.m_nRel > -1)          m_nRel = _rstSrc.m_nRel                    ;
		if(_rstSrc.m_nRelPrack > -1)     m_nRelPrack = _rstSrc.m_nRelPrack          ;
		return *this;
	}
	int m_nLogLevel;
	int m_nT1;
	int m_nT2;
	int m_nT4;
	int m_nTC;
	int m_nTD;
	int m_nTG;
	int m_nTH;
	int m_nDialogAlive;
	int m_nDialogTerm;
	int m_nRel;
	int m_nRelPrack;
}IbcStack_t;
class RuSipConfig : public RuleFile
{
	public:
		RuSipConfig();
		~RuSipConfig();
		static RuSipConfig * fnGetInstance();
		static void fnFinalize();
		void fnLoadSipFile(const char * _szPath);
		void fnLoadSipMem(const char * _Src);
		void fnWriteSipFile(const char * _szPath);
		void fnWriteSipMem(char * _Src, unsigned int _unMax);
		void fnModiStackTimer(IbcStack_t * _pstModi);
		void fnGetStackTimer(IbcStack_t * _pstReturn);
		void fnDebug();
		IbcStack_t m_stStack;
		static RuSipConfig * g_pclsSipConf;
		RUString m_clsPath;
	private:
		void fnLoadCfg();
		void fnRegiFunction();
		void fnArrangeLine();
		void fnWLock(){m_Mutex.writelock();}
		void fnRLock(){m_Mutex.readlock();}
		void fnUnlock(){m_Mutex.unlock();}
		resip::RWMutex m_Mutex;
};
#endif

