#ifndef CAPPTIMER_H
#define CAPPTIMER_H
#include "eSipUtil/timerTable.hxx"
#include "eRule/CRuleUtiles.h"
#include "TrgwMsg.h"
#include "rutil/TimeLimitFifo.hxx"
#include "IBCDefine.h"
typedef enum
{
	E_APPTIMER_NONE = 0,
	E_APPTIMER_MONITOR,
	E_APPTIMER_TRGW_PING,
	E_APPTIMER_CDR_CHECK,
	E_APPTIMER_PDE_PING,
	#ifdef DEF_PDE_TRANSPORT_CHANGED
	E_APPTIMER_PDE_RECONNECT,
	#endif
	E_APPTIMER_CHECK_CMD_FILE,
	E_APPTIMER_CHECK_MEDIA,
	E_APPTIMER_CHECK_TRGW_HA,
	E_APPTIMER_CHECK_RULELIB,
	E_APPTIMER_CHECK_TRGW_ARM,
	E_APPTIMER_CHECK_TRGW_POLLARM,
	E_APPTIMER_MAX
}EAppTimer_t;
extern const char * g_arrAppTimerString[E_APPTIMER_MAX+1];
typedef struct AppTimerData_t
{
	AppTimerData_t(){m_eT = E_APPTIMER_NONE;m_pvUser = NULL;m_pvUser2=NULL;}
	~AppTimerData_t()
	{
	}
	void m_fnSetType(EAppTimer_t _eT)
	{
		m_eT = _eT;
	}
	EAppTimer_t m_eT;
	void * m_pvUser;
	void * m_pvUser2;
}AppTimerData_t;
class AppTimer
{
	public :
		static void fnStart();
		static void fnStop();
		//static void fnClear();
		static void fnSetTimer(AppTimerData_t * _pclsData,unsigned long _ulAfterMilliSec);
		static void fnSetTimer(AppTimerData_t * _pclsData, struct timespec * _pstTime);
		static void fnSetCdrCheck();
		static void fnSetSessionMonitor();
		static void fnSetPdePing();
		static void fnSetPdeReconnect();
		static void fnSetTrGWPing(void * _pvUser);
		static void fnSetCheckFile();
		static void fnSetCheckMedia();
		static void fnSetCheckTrGWHA();
		static void fnSetCheckRuleLib();
		static void fnSetTrGWAlarm();
		static void fnSetTrGWPollAlarm();
		static void fnCbkAppTimerOut(AppTimerData_t * _pstDialogTimerData);
		static void fnGetTimeSpec(struct timespec * _pstTime);
	private :
		static eSipUtil::TimerTable<AppTimerData_t> m_clsAppTimerTable;
};
#endif
