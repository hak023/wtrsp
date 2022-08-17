
#ifndef DUMLIB_DIALOG_TIMER_HXX
#define DUMLIB_DIALOG_TIMER_HXX

#include <time.h>

#include "eDumLibDef.hxx"

namespace eDum
{

enum
{
	E_DIALOG_MAX_SET_TIMER = 4,	 //at one go, max size to set timer
};

typedef enum
{
	E_DIALOG_TIMER_SIP_C = 0,
	E_DIALOG_TIMER_SIP_18X_RETRANS,		//retransmission interval for reliable provisional response.	(18x)
	E_DIALOG_TIMER_SIP_PRACK_WAIT,		//wait time for receiving PRACK request
	E_DIALOG_TIMER_SIP_L,							//wait time for receiving ACK request for 2xx response. rfc 6026
	E_DIALOG_TIMER_SIP_G,							//retransmission interval for 200 ok. eDum does for 200 ok. SIP Stack does for 3xx-6xx response
	E_DIALOG_TIMER_SIP_SESS_REFRESH,	//sessoin refresh timer (session-expire/2)
	E_DIALOG_TIMER_ALIVE,
	E_DIALOG_TIMER_TERM,								//when dialog is terminated, it is set.
	
	E_DIALOG_TIMER_NUM,
}EDialogTimerType_t;

bool isValidDialogTimerType(EDialogTimerType_t _eDialogTimerType);
const char * getDialogTimerTypeString(EDialogTimerType_t _eDialogTimerType);

struct DialogSetTimerResult
{
	EDialogTimerType_t m_eDialogTimerType;
	struct timespec m_stExpireTs;
	unsigned long m_ulAfterMilliSec;

	DialogSetTimerResult()
	{
		m_eDialogTimerType = E_DIALOG_TIMER_NUM;
		memset(&m_stExpireTs, 0x00, sizeof(struct timespec));
		m_ulAfterMilliSec = 0;
	}
};
typedef struct DialogSetTimerResult DialogSetTimerResult_t;

class DialogTimerConfig
{
	public :
		enum
		{
			E_CONST_DFT_SIP_TIMER_T2 = 4000,							//millisec
			E_CONST_DFT_SIP_TIMER_TC = 180000,						//millisec
			E_CONST_DFT_SIP_TIMER_18X_RETRANS = 500,		//millisec
			E_CONST_DFT_SIP_TIMER_PRACK_WAIT = 32000,	//millisec
			E_CONST_DFT_SIP_TIMER_TL = 32000,							//millisec
			E_CONST_DFT_SIP_TIMER_TG = 500,								//millisec
			E_CONST_DFT_SIP_TIMER_ALIVE = 60000,					//millisec
			E_CONST_DFT_SIP_TIMER_TERM = 10000,					//millisec
			E_CONST_DFT_SIP_TIMER_GARBAGE = 300,				//sec
		};

		DialogTimerConfig();
		~DialogTimerConfig();
		
		DialogTimerConfig & operator=(const DialogTimerConfig & _rstSrcDialogTimerConfig);

		inline void setSipT2Timer(unsigned long _ulMilliSec){ m_ulT2 = _ulMilliSec; }
		inline void setSipTCTimer(unsigned long _ulMilliSec){ m_arrTimer[E_DIALOG_TIMER_SIP_C] = _ulMilliSec; }
		inline void setSipT18xRetransTimer(unsigned long _ulMilliSec){ m_arrTimer[E_DIALOG_TIMER_SIP_18X_RETRANS] = _ulMilliSec; }
		inline void setSipTPrackWaitTimer(unsigned long _ulMilliSec){ m_arrTimer[E_DIALOG_TIMER_SIP_PRACK_WAIT] = _ulMilliSec; }
		inline void setSipTLTimer(unsigned long _ulMilliSec){ m_arrTimer[E_DIALOG_TIMER_SIP_L] = _ulMilliSec; }
		inline void setSipTGTimer(unsigned long _ulMilliSec){ m_arrTimer[E_DIALOG_TIMER_SIP_G] = _ulMilliSec; }
		inline void setAliveTimer(unsigned long _ulMilliSec){ m_arrTimer[E_DIALOG_TIMER_ALIVE] = _ulMilliSec; }
		inline void setTermTimer(unsigned long _ulMilliSec){ m_arrTimer[E_DIALOG_TIMER_TERM] = _ulMilliSec; }
		inline void setGarbageTimer(unsigned int _unSec){ m_unGarbageTimer = _unSec; }
		inline void setUse18xRetrans(bool _bUse){ m_bUse18xRetrans = _bUse; }
		inline void setUse2xxRetrans(bool _bUse){ m_bUse2xxRetrans = _bUse; }

		inline unsigned long getSipT2Timer() const{ return m_ulT2; }
		inline unsigned long getDialogTimer(EDialogTimerType_t _eDialogTimerType) const
		{
			if( isValidDialogTimerType(_eDialogTimerType) )
				return m_arrTimer[_eDialogTimerType];
			else
				return -1;
		}
		inline unsigned int getGarbageTimer() const{ return m_unGarbageTimer; }
		inline bool isUse18xRetrans() const{ return m_bUse18xRetrans; }
		inline bool isUse2xxRetrans() const{ return m_bUse2xxRetrans; }

	private :
		unsigned long m_ulT2;
		unsigned long m_arrTimer[E_DIALOG_TIMER_NUM];
		unsigned int m_unGarbageTimer;
		bool m_bUse18xRetrans;
		bool m_bUse2xxRetrans;
	 
};

}

#endif
