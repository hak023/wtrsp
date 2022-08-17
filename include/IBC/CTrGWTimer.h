#ifndef CTRGWTIMER_H
#define CTRGWTIMER_H
#include "eSipUtil/timerTable.hxx"
#include "eRule/CRuleUtiles.h"
#include "CTrgwTcpServer.h"
#include "IBCDefine.h"
typedef enum
{
	E_TRGWTIMER_NONE = 0,
	E_TRGWTIMER_ADD,
	E_TRGWTIMER_MOD,
	E_TRGWTIMER_DEL,
	E_TRGWTIMER_CHG,
	#ifdef DEF_TRGW_RETRANS
	E_TRGWTIMER_ADD_RETRANS,
	E_TRGWTIMER_MOD_RETRANS,
	E_TRGWTIMER_DEL_RETRANS,
	E_TRGWTIMER_MAX
	#endif
}ETrGWTimer_t;
#ifdef DEF_TRGW_RETRANS
#define DEF_TRGW_RETRANS_SIZE     10240
extern const char * g_arrTrGWTimer[E_TRGWTIMER_MAX];
#else
extern const char * g_arrTrGWTimer[E_TRGWTIMER_CHG+1];
#endif
typedef struct TrGWTimerData_t
{
	TrGWTimerData_t()
	{
		m_eT = E_TRGWTIMER_NONE;m_pclsChannel = NULL;m_uiSessionID=0;m_uiTransID=0;
		#ifdef DEF_TRGW_RETRANS
		memset(&m_pvData[0],0x00,DEF_TRGW_RETRANS_SIZE);m_unLen=0;m_unRetransCnt=0;
		#endif
	}
	~TrGWTimerData_t()
	{
	}
	void m_fnSetType(ETrGWTimer_t _eT)
	{
		m_eT = _eT;
	}
	#ifdef DEF_TRGW_RETRANS
	void m_fnSetBody(void * _pvData,unsigned int _unLen)
	{
		m_unLen = _unLen;
		if(_unLen < DEF_TRGW_RETRANS_SIZE)
			memcpy(&m_pvData[0],_pvData,_unLen);
		else
			memcpy(&m_pvData[0],_pvData,DEF_TRGW_RETRANS_SIZE);
	}
	void * m_fnAllocCopy()
	{
		void * pvData = (void*)new char [m_unLen+1];
		memcpy(pvData,m_pvData,m_unLen); ((char*)pvData)[m_unLen] = 0x00;
		return pvData;
	}
	#endif
	ETrGWTimer_t m_eT;
	RUTcpChannel * m_pclsChannel;
	unsigned int m_uiSessionID;
	unsigned int m_uiTransID;
	#ifdef DEF_TRGW_RETRANS
	unsigned int m_unLen;
	unsigned int m_unRetransCnt;
	char  m_pvData[DEF_TRGW_RETRANS_SIZE];
	#endif
}TrGWTimerData_t;
class TrGWTimer
{
	public :
		static void fnStop();
		//static void fnClear();
		static void fnReset();
		static void fnSetTransaction(ETrGWTimer_t _eT, RUTcpChannel * _pclsCh, 
										unsigned int _uiSessionID, unsigned int _uiTransID);
		#ifdef DEF_TRGW_RETRANS
		static void fnSetTransactionRe(ETrGWTimer_t _eT, RUTcpChannel * _pclsCh, 
													unsigned int _uiSessionID, unsigned int _uiTransID,
													void * _pvData,unsigned int _unLen);
		#endif
		static void fnSetReTransaction(ETrGWTimer_t _eT, RUTcpChannel * _pclsCh, 
													unsigned int _uiSessionID, unsigned int _uiTransID,
													struct timespec * _pstTime);
		static void fnSetTimer(TrGWTimerData_t * _pclsData,unsigned long _ulAfterMilliSec);
		static void fnSetTimer(TrGWTimerData_t * _pclsData, struct timespec * _pstTime);
		static void fnCbkAppTimerOut(TrGWTimerData_t * _pstDialogTimerData);

	private :
		static eSipUtil::TimerTable<TrGWTimerData_t> m_clsTrGWTimerTable;

};

#endif
