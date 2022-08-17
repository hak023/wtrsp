#ifndef OMC_IF_H
#define OMC_IF_H
#include "eSipUtil/timerTable.hxx"
#include "eRule/CRuleUtiles.h"
#include "eRule/RuleLibCom.h"
#include "CAppTimer.h"
#include "omc.h"
#include "alarm.h"
#include "calltrace.h"
#include "CAppSession.h"
#include "eSipTrace.hxx"
#include "eRule/CRUInetUdp.h"
#include "IBCDefine.h"
#if defined(DEF_R120) && defined(DEF_FAULT_MSG) 
#include "eRule/CRuleUtiles.h"
#include "eRule/CRUFile.h"
#include "eRule/CRUTime.h"
#include "rutil/TimeLimitFifo.hxx"
#endif
#include "eSipUtil/string.hxx"
#include "eSipUtil/NodeMgr.hxx"

/*
		e_node_Internal = 0,      // 내부망
		e_node_LB,          
		e_node_CM,        
		e_node_EMP,        
		e_node_External,          // 외부망
		e_node_max,
*/
typedef enum {
        E_TRACE_NODE_INTERNAL=0,   //e_node_internal = 0,     // internal
        #ifdef DEF_VIBC_ENABLE
        E_TRACE_NODE_LB,
	  #endif
	  E_TRACE_NODE_CM,               //e_node_cm,             // ibcf
        E_TRACE_NODE_EMP,             //e_node_emp,             // trgw
        E_TRACE_NODE_EXTERNAL,     //e_node_external,         // external
        E_TRACE_NODE_PCDS,
        E_TRACE_NODE_MAX               //e_node_max,     
}ETraceNode_t;
extern const char * g_arrTraceNodeString[E_TRACE_NODE_MAX+1];
class AlarmParam
{
	public:
		AlarmParam(){}
		~AlarmParam(){}
		unsigned int m_eType;
		unsigned int m_eLevel;
		unsigned int m_eOwner;
		bool m_bOccured;
		RUString m_clsText;
		RUString m_clsCause;
	
};
#if defined(DEF_R120) && defined(DEF_FAULT_MSG)
#define IBC_MSG_FUNC_ALARM                    0x0500
#define IBC_MSG_SUBF_ALARM_NOTIFY             0x0502    // OMCMSGALARM
#define IBC_MSG_SUBF_ALARM_CONSOLE            0x050A    // OMCMSGCONSOLE
/// CONSOLE
#define  IBC_MAX_CONSOLE_CODE 64
#define  IBC_MAX_CONSOLE_SRC 64
#define  IBC_MAX_CONSOLE_LOG 1024
#define  IBC_MAX_CONSOLE_SOLUTION 1024
// nLevel
#define IBC_CON_LOG_FAULT  0
#define IBC_CON_LOG_WARN   1
#define IBC_CON_LOG_INFO   2
typedef enum
{
	E_IBC_FAULT_CM_NONE = 4300,
	E_IBC_FAULT_SYNTAX_ERROR,
	E_IBC_FAULT_SESSION_FULL,
	E_IBC_FAULT_SESSION_NOT_FOUND,
	E_IBC_FAULT_EARLY_DIALOG_FULL,
	E_IBC_FAULT_DIALOG_FULL,
	E_IBC_FAULT_DIALOG_NOT_FOUND,
	E_IBC_FAULT_INVALID_CALL_STATE,
	E_IBC_FAULT_REMOTE_STATUS_DOWN,
	E_IBC_FAULT_TIMEOUT_C_TIMER,
	E_IBC_FAULT_TIMEOUT_ALIVE_TIMER,
	E_IBC_FAULT_ROUTING_FAIL,
	E_IBC_FAULT_TARGET_STATUS_DOWN,
	E_IBC_FAULT_TGAS_ALL_DOWN,
	E_IBC_FAULT_TGAS_TIMEOUT,
	E_IBC_FAULT_TGAS_NONE = 4700,
	E_IBC_FAULT_MAX = 4799
}EIbcFault_t;
typedef struct IBCFaultInfo_t
{
	unsigned int    m_unTime;                                      // nTime 이 0 이면 OMP 에서 메시지를 받은 시간을 적재
	int             m_nLevel;                                          // 0 : FAULT, 1 : WARN, 2 : INFO
	char            m_szCode[IBC_MAX_CONSOLE_CODE];   // CCM 4300 ~ 4399   // TGAS 4700 ~ 4799
	char            m_szSrc[IBC_MAX_CONSOLE_SRC];        // Server name/Process name/xxx/xxx, CM01/OMA/CPU
	char            m_szLog[IBC_MAX_CONSOLE_LOG]; 
	char            m_szSolution[IBC_MAX_CONSOLE_SOLUTION]; // Optional
} IBCFaultInfo_t;
typedef struct IbcFaultMsg_t
{
	XTMMSGHDR       m_stHdr;
	VMSIVRLINE        m_stLine;
	IBCFaultInfo_t    m_stInfo;
	IbcFaultMsg_t()
	{
		memset(this,0x00,sizeof(IbcFaultMsg_t));
		m_stHdr.usType = IBC_MSG_FUNC_ALARM;
		m_stHdr.usSubType = IBC_MSG_SUBF_ALARM_CONSOLE;
		m_stHdr.nMsgLen = sizeof(IbcFaultMsg_t);
	}
	void m_fnSetLog(const char * _pszServerName, const char * _pszProcName, 
						const char * _pszModuleName, EIBCNewErr_t _eT, const char * _pszLog,
						const char * _pszCopingWays)
	{
		fnRUStrnCat(m_stInfo.m_szCode,IBC_MAX_CONSOLE_CODE,"F%X",_eT);
		fnRUStrnCat(m_stInfo.m_szSrc,IBC_MAX_CONSOLE_SRC,"%s/%s/%s",_pszServerName,
																	_pszProcName,
																	_pszModuleName);
		if(_pszLog) strncpy(m_stInfo.m_szLog,_pszLog,IBC_MAX_CONSOLE_LOG-1);
		if(_pszCopingWays) strncpy(m_stInfo.m_szSolution,_pszCopingWays,IBC_MAX_CONSOLE_SOLUTION-1);
	}
} IbcFaultMsg_t;
typedef struct OmcQ_t
{
	OmcQ_t()
	{
		m_bIsShutDown = false;m_pstFault = NULL;m_pstTrace = NULL;m_pstAlarm = NULL;
		#if defined(DEF_R125)
		m_pstControl = NULL; m_pstRestart = NULL;
		#endif
	}
	~OmcQ_t()
	{
		if(m_pstFault) delete m_pstFault;
		if(m_pstTrace) delete m_pstTrace;
		if(m_pstAlarm) delete m_pstAlarm;
		#if defined(DEF_R125)
		if(m_pstControl) delete m_pstControl;
		if(m_pstRestart) delete m_pstRestart;
		#endif
	}
	bool m_bIsShutDown;
	IbcFaultMsg_t * m_pstFault;
	MSGTRACELOG * m_pstTrace;
	OMCMSGALARM * m_pstAlarm;
	#if defined(DEF_R125)
	OMCMSGPROCCONTROL * m_pstControl;
	OMCMSGREQUEST * m_pstRestart;
	#endif
}OmcQ_t;
class OmcIf;
class OmcQ : public resip::ThreadIf
{
	public:
		OmcQ();
		~OmcQ();
		bool fnPut(OmcQ_t *_pstNew);
		void fnStart(OmcIf * _pclsOwner);
		void fnStop();
		void fnSetMaxFifo(unsigned int _unMaxQNum){mFifo.setMaxFifoSize(_unMaxQNum);}
		unsigned int fnGetMaxFifo(){return mFifo.getMaxFifoSize();}
		unsigned int fnGetCurrentFifo(){return mFifo.getCurrentFifoNum();}
		bool fnIsCongestion(){return mFifo.checkCongestion(NULL,0);}
	private:
		void thread();
		bool m_bFinished;
		resip::TimeLimitFifo<OmcQ_t> mFifo;
		OmcIf *m_pclsOwner;
};
#endif

class OmcIf
{
	public:
		OmcIf();
		~OmcIf();
		static OmcIf * fnGetInstance();
		static void fnFinalize();
		bool fnStart(const char* _pszLocalPath,const char *_pszRemotePath,int _nSocBuff,
										const char * _pszTraceLocalIp, int _nTraceLocalPort,
										const char * _pszTraceRemoteIp, int _nTraceRemotePort);
		void fnStop();
		void fnSendDebug(eDum::Session *_pclsSession,const char *_szLog, ...);
		//==> Only SipMessage (Stack Trace CallBack)
		void fnSendSipTraceMsg(const eSIP::TraceResult & _rstTraceResult,const char * _pszTitle);
		//==> Only Routing Info
		void fnSendTraceRoutingMsg(AppSession *_pclsApp,const char * _pszTitle,
																		const char * _pszBody);
		//==> Etc Service Trace (Filter,Screen,CDR,...)
		void fnSendTraceServiceMsg(eDum::Session * _pclsSession,
																const char * _pszTitle,
																const char * _pszTraceBody,
																ETraceNode_t _eFrom,
																ETraceNode_t _eTo);
		void fnSendMBlkAlarm(bool _bBlk,const char * _pszName);
		void fnSendPDEAlarm(bool _bConnect);
		void fnSendTrGWAlarm(const eSipUtil::Net5Tuple_t & _rstNet5Tuple,bool _bConnected);
		void fnSendTrGWAlarmMy(const eSipUtil::Net5Tuple_t & _rstNet5Tuple,bool _bConnected);
		//==> Send Alarm
		void fnSendAlarm(ALARM_LEVEL _eLevel, ALARM_TYPE _eType, const char* sztext, 
										const char* szcause, const char* szsrc_other);
		#if defined(DEF_R120) && defined(DEF_FAULT_MSG)
		//==> Send Fault Msg
		void fnSendFault(const char * _pszServerName, const char * _pszProcessName,
							const char * _pszModuleName,const char * _pszLog,
							EIBCNewErr_t _eT,const char * _pszCopingWays);
		#endif
		#if defined(DEF_R125)
		void fnSendRestart();
		void fnSendHADeAct();
		#endif
		#ifdef DEF_R131
		void fnSendAlarmEq(ALARM_LEVEL _eLevel, ALARM_TYPE _eType, const char* sztext, 
							const char* szcause, const char * _pszEqName,const char* szsrc_other);
		#endif
		static OmcIf * m_pclsMy;
		AlarmParam m_clsPDE;	
		RULock m_clsLock;
		CRUUnixUdp m_clsAlarmUdp;
		CRUInetUdp m_clsTraceUdp;
	private:
		//==> Send Trace Msg
		void fnSendTrace(const resip::TraceInfo_t & _rstTraceResult,MSGTRACELOG * _pstTrace);
		void fnSendTrace(const eSIP::TraceResult & _rstTraceResult,MSGTRACELOG * _pstTrace);
		#if defined(DEF_R120) && defined(DEF_FAULT_MSG)
		OmcQ m_clsQ;
		#endif
};
#endif

