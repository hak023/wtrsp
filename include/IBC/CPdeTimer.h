#ifndef CPDETIMER_H
#define CPDETIMER_H
#include "eSipUtil/timerTable.hxx"
#include "eRule/CRuleUtiles.h"
#include "CPdeTcpServer.h"
#include "IBCDefine.h"
#include "CFailCharging.h"
#include "eSipUtil/string.hxx"
typedef enum
{
	E_PDETIMER_NONE = 0,
	E_PDETIMER_START,
	E_PDETIMER_STOP,
	E_PDETIMER_INTERIM,
	E_PDETIMER_REFRESH,
	#ifdef DEF_PDE_RETRANS
	E_PDETIMER_START_RE,
	E_PDETIMER_STOP_RE,
	E_PDETIMER_INTERIM_RE,
	E_PDETIMER_REFRESH_RE,
	#endif
	E_PDETIMER_MAX
}EPdeTimer_t;
extern const char * g_arrPdeTimer[E_PDETIMER_MAX];
typedef struct PdeTimerData_t
{
	PdeTimerData_t()
	{
		m_eT = E_PDETIMER_NONE;m_pclsChannel = NULL;m_uiSessionID=0;m_uiTransID=0;
		#ifdef DEF_PDE_RETRANS
		memset(&m_stBody,0x00,sizeof(PDEMsg_t));m_unRetransCnt=0;
		memset(&m_szCaller[0],0x00,64);memset(&m_szCalled[0],0x00,64);
		#endif
	}
	~PdeTimerData_t()
	{
	}
	void m_fnSetType(EPdeTimer_t _eT)
	{
		m_eT = _eT;
	}
	#ifdef DEF_PDE_RETRANS
	void m_fnSetBody(void  *_pstData)
	{
		memcpy(&m_stBody,_pstData,sizeof(PDEMsg_t));
	}
	void m_fnSetCaller(const char * _pszCaller)
	{
		if(m_szCaller[0]==0)
		eSipUtil::KString::m_fnStrnCat(&m_szCaller[0],64,_pszCaller);
	}
	void m_fnSetCalled(const char * _pszCalled)
	{
		if(m_szCalled[0]==0)
		eSipUtil::KString::m_fnStrnCat(&m_szCalled[0],64,_pszCalled);
	}
	unsigned int m_unRetransCnt;
	PDEMsg_t m_stBody;
	#endif
	EPdeTimer_t m_eT;
	RUTcpChannel * m_pclsChannel;
	unsigned int m_uiSessionID;
	unsigned int m_uiTransID;
	char m_szCaller[64];
	char m_szCalled[64];
}PdeTimerData_t;
class PdeTimer
{
	public :
		static void fnStop();
		//static void fnClear();
		static void fnReset();
		static void fnSetTransaction(EPdeTimer_t _eT, RUTcpChannel * _pclsCh, 
										unsigned int _uiSessionID, unsigned int _uiTransID);
		static void fnSetReTransaction(EPdeTimer_t _eT, RUTcpChannel * _pclsCh, 
													unsigned int _uiSessionID, unsigned int _uiTransID,
													struct timespec * _pstTime);
		#ifdef DEF_PDE_RETRANS
		static void fnSetTransactionRe(EPdeTimer_t _eT, RUTcpChannel * _pclsCh, 
													unsigned int _uiSessionID, unsigned int _uiTransID,
													void * _pvData);
		static void fnSetRetransTimer(PdeTimerData_t * _pclsData);
		static unsigned int fnGetMaxTimerCnt();
		#endif
		static void fnSetTimer(PdeTimerData_t * _pclsData,unsigned long _ulAfterMilliSec);
		static void fnSetTimer(PdeTimerData_t * _pclsData, struct timespec * _pstTime);
		static void fnCbkAppTimerOut(PdeTimerData_t * _pstDialogTimerData);

	private :
		static eSipUtil::TimerTable<PdeTimerData_t> m_clsPdeTimerTable;

};

#endif
