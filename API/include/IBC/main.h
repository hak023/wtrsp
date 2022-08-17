#ifndef IBC_MAIN_HXX
#define IBC_MAIN_HXX
#include <pthread.h>

#include "resip/stack/SipMessage.hxx"
#include "rutil/Logger.hxx"
#include "rutil/TransportType.hxx"
#include "rutil/TimeLimitFifo.hxx"

#include "eDum/eDumLibDef.hxx"
#include "eDum/eDumLibStack.hxx"
#include "eDum/eDumLibSession.hxx"

#include "eSipType.hxx"
#include "eSipCallback.hxx"
#include "eSipTu.hxx"
#include "eSipAppMessage.hxx"
#include "resip/stack/ExtensionParameter.hxx"

#include "eRule/RuleInterface.h"
#include "eRte/eSipRteApi.hxx"
#include "eSipCommand.hxx"
#include "eSipTrace.hxx"
#include "eSipAlarm.hxx"
#include "IBCDefine.h"

#if defined (DEF_R120) && defined(DEF_FAULT_MSG)
#include "eSipFault.hxx"
#endif

extern const char * s_pszModName;
typedef enum
{
	E_IBC_HA_ST_NONE = 0,
	E_IBC_HA_ST_ACTIVE,
	E_IBC_HA_ST_STANBY	
}EIBCHA_t;
inline const char * g_fnStringIBCStatus(unsigned int _eT)
{
	switch(_eT)
	{
		case E_IBC_HA_ST_NONE: return "E_IBC_HA_ST_NONE";
		case E_IBC_HA_ST_ACTIVE: return "E_IBC_HA_ST_ACTIVE";
		case E_IBC_HA_ST_STANBY: return "E_IBC_HA_ST_STANBY";
	}
	return "E_IBC_HA_ST_NONE";
}
typedef struct
{
	eDum::ESipState_t m_eSipState;
	eSIP::EStateCbLayer_t m_eCbLayer;
	eSIP::PfuncStateCallback_t m_pfnStateCb;
	const char * m_szDesc;
}IBCStateCallBackInfo_t;
/******************************* HA Status Call Back Function ********************************/
void g_fnCbkHAStatus(int _nCurStatus,int _nBeforeStatus);// 1 active -2 stanby
void g_fnCbkHASessionLoad(eDum::Session & _rclsSession);
/******************************* OverLoad CDR Call Back Function *****************************/
void g_fnRcvOverloadCdr(const eSIP::OverloadCdr_t & _rstOverloadCdr);
/******************************* Trace Call Back Function ***********************************/
void g_fnRcvTraceMsg(const eSIP::TraceResult & _rstTraceResult);
/******************************* Core Alarm Call Back Function *******************************/
void g_fnCbkCoreAlarm(const eSIP::AlarmSendData_t & _rstAlarmSndData);
/******************************* Hang Detected Call back Function *****************************/
#if defined(DEF_R125)
void g_fnCbkHangDetected(int _nEnableHA, int _nEnableReset);
#endif
/***************************** TGAS Rte SetConfig Call back Function ***************************/
void g_fnCbkChangedTrgwTimer(int _nTrgwTimer);
/*************************** TGAS Recv Call Back Function ***********************************/
#define DEF_RESUME_ARGUMENT eSIP::ProcSipParam_t & _rstCbParam,\
								void * _pResumeData, int _nResumeDataLen,\
								eSIP::SuspendResult_t & _rstSuspendResult,\
								eSIP::EventResult & _rclsEventResult
#define DEF_RESUME_ARGUMENT_CONV _rstCbParam,_pResumeData,_nResumeDataLen,\
														_rstSuspendResult,_rclsEventResult
typedef struct StackResumCbInfo_t
{
	StackResumCbInfo_t(DEF_RESUME_ARGUMENT)
	{
		m_pstCbParam = &_rstCbParam;
		m_pResumeData = _pResumeData;
		m_nResumeDataLen = _nResumeDataLen;
		m_pstSuspendResult=&_rstSuspendResult;
		m_pclsEventResult = &_rclsEventResult;
		if(g_fnCheckLogLevel(resip::Log::Debug))
		{
			char szLog[DEF_RU_SZ_LOG]; memset(szLog,0x00,DEF_RU_SZ_LOG);
			fnRUStrnCat(szLog,DEF_RU_SZ_LOG,
							"<Resume CallBack Parameter> StateInfo=%p, ResumeData=%p, Len=%d"\
							"SuspendResult=%p,EventResult=%p",
							m_pstCbParam,
							m_pResumeData,
							m_nResumeDataLen,
							m_pstSuspendResult,
							m_pclsEventResult);
			RLOGb(szLog);
		}
	}
	StackResumCbInfo_t & operator=(StackResumCbInfo_t & _rclsSrc)
	{
		m_pstCbParam     = _rclsSrc.m_pstCbParam    ;    
		m_pResumeData     = _rclsSrc.m_pResumeData    ; 
		m_nResumeDataLen     = _rclsSrc.m_nResumeDataLen    ; 
		m_pstSuspendResult = _rclsSrc.m_pstSuspendResult;
		m_pclsEventResult = _rclsSrc.m_pclsEventResult;
		return * this;
	}
	eSIP::ProcSipParam_t * m_pstCbParam;
	eSIP::SuspendResult_t  * m_pstSuspendResult;
	eSIP::EventResult * m_pclsEventResult;
	void * m_pResumeData;
	int m_nResumeDataLen;
}StackResumCbInfo_t;
#define DEF_RESUME_ARG_CREATE StackResumCbInfo_t l_stResumeInfo(DEF_RESUME_ARGUMENT_CONV)
#define DEF_RESUME_ARG_INSTANCE    l_stResumeInfo
#define DEF_RESUME_PARG_INSTANCE  &l_stResumeInfo
eSIP::EStateCbResultType_t g_fnCbkRcvTRGW(DEF_RESUME_ARGUMENT);
#ifdef DEF_R131
#define ERRLOG(A) do{RLoger(eSipUtil::E_LOG_ERR,"%s %s %d %s",__FILE__,__func__,__LINE__,A);}while(false)	
void g_fnGetEnumInfo(eDum::Dialog * _pclsDialog, bool & _rbNpdb, bool & _rbHlr);
eSIP::EStateCbResultType_t g_fnCbkRcvEnumIf(DEF_RESUME_ARGUMENT);
eSIP::EStateCbResultType_t g_fnCbkRcvEnumTimeOut(DEF_RESUME_ARGUMENT);
eSIP::EStateCbResultType_t g_fnInitEnumProc(StackResumCbInfo_t & _rclsInfo);
#endif
/***************************** TrGW Status Call Back Function *********************************/
int g_fnCbkTrGWAlarm( int _nAvail, TrGWRoute_t * _pstTrGW, char * _pszFailReason);  // max 128
#if defined(DEF_R120) && defined(DEF_FAULT_MSG)
/***************************** Stack Fault Call Back Function **********************************/
void g_fnCbkFaultMsg(eSIP::ECoreError_t _eCoreError, const char * _pszCaller, const char * _pszCallee, 
														const char * _pszCallId, char * _pszFaultDesc);
#endif
/**************************** Session Event Call Back ***************************************/
#define DEF_ASYNC_ARGUEMT eDum::Session * _pclsSession,\
														eDum::Dialog * _pclsDialog, \
														int _nType, \
														void * _pvData,\
														eSIP::EventResult & _rclsEventResult
#define DEF_ASYNC_ARGUEMT_CONV _pclsSession,\
														_pclsDialog, \
														_nType, \
														_pvData,\
														_rclsEventResult	
typedef struct StackAsyncCbInfo_t
{
	StackAsyncCbInfo_t(DEF_ASYNC_ARGUEMT)
	{
		m_pclsSession = _pclsSession;
		m_pclsDialog = _pclsDialog;
		m_nType = _nType;
		m_pvData = _pvData;
		m_pclsEventResult = &_rclsEventResult;
		if(g_fnCheckLogLevel(resip::Log::Debug))
		{
			char szLog[DEF_RU_SZ_LOG]; memset(szLog,0x00,DEF_RU_SZ_LOG);
			fnRUStrnCat(szLog,DEF_RU_SZ_LOG,
							"<ASync CallBack Parameter>\r\nSession=%p, Dialog=%p,"\
							" Type=%d, pvData=%p, ResultCb=%p",
							m_pclsSession,m_pclsDialog,m_nType,m_pvData,m_pclsEventResult);
			RLOGb(szLog);
		}
	}
	StackAsyncCbInfo_t & operator=(StackAsyncCbInfo_t & _rclsSrc)
	{
		m_pclsSession = _rclsSrc.m_pclsSession;
		m_pclsDialog = _rclsSrc.m_pclsDialog;
		m_nType = _rclsSrc.m_nType;
		m_pvData = _rclsSrc.m_pvData;
		m_pclsEventResult = _rclsSrc.m_pclsEventResult;
		return * this;
	}
	eDum::Session * m_pclsSession;
	eDum::Dialog * m_pclsDialog;
	int m_nType;
	void * m_pvData;
	eSIP::EventResult * m_pclsEventResult;
}StackAsyncCbInfo_t;
#define DEF_ASYNC_ARG_CREATE StackAsyncCbInfo_t l_stASyncInfo(DEF_ASYNC_ARGUEMT_CONV)
#define DEF_ASYNC_ARG_INSTANCE    l_stASyncInfo
#define DEF_ASYNC_PARG_INSTANCE  &l_stASyncInfo
eSIP::EAsyncCbResultType_t g_fnCbkRcvSessionEvent(DEF_ASYNC_ARGUEMT);
/**************************** Dialog Gard Time Out Call Back **********************************/
void IBCGardTimeOutDialogCb(eDum::Session  *_pclsSession,eDum::Dialog & _rclsDialog,
											eSIP	::Tu & _rclsTu,eSIP::EventResult & _rclsEventResul);
/**************************** Session Del Call Back *****************************************/
void IBCSessionDelCb(eDum::Session & _rclsSession,eSIP::Tu & _rclsTu);
/**************************** Internal Term Event Call Back ***********************************/
void IBCInternalDialogEventCb(eDum::Session & _rclsSession,eDum::Dialog & _rclsDialog,
											eSIP	::Tu & _rclsTu,eSIP::EventResult & _rclsEventResul);
/**************************** Routing Call Back Function *************************************/
void IBCRoutingCb(eSIP::ProcSipParam_t & _rstCbParam, eSIP::EventResult & _rclsEventResult);
/**************************** State Call Back Functions **************************************/
eSIP::EStateCbResultType_t IBCInvUasInviteCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUasProceedingCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUasProceedingRelCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUasPrackCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUasAcceptCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUasReInviteCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUasReProceedingCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUasReAcceptCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUacInviteCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUacProceedingCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUacProceedingRelCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUacPrackCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUacAcceptCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUacReInviteCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUacReProceedingCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvUacReAcceptCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvEstablishCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvTerminatedCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvTerminatingCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCInvTerminateCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCStandAloneUasReqCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCStandAloneUasProceedingCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCStandAloneUacReqCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCStandAloneUacProceedingCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCStandAloneTerminateCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCSubUasReqCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCSubUacReqCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCSubUasProceedingCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCSubUacProceedingCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCSubUasAcceptCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCSubUacAcceptCb(DEF_ST_ARGUMENT);
eSIP::EStateCbResultType_t IBCSubTerminateCb(DEF_ST_ARGUMENT);
#endif

