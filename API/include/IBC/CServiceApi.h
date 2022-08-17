#ifndef ESIP_SERVICEAPI_HXX
#define ESIP_SERVICEAPI_HXX

#include "eDum/eDumLibDef.hxx"
#include "eDum/eDumLibStack.hxx"
#include "eDum/eDumLibSession.hxx"

#include "eRule/RuleInterface.h"
#include "eRule/CStatLog.h"

#include "eRte/eSipRteApi.hxx"
#include "eRte/eSipRteErr.hxx"

#include "CIbcConfig.h"
#include "CSipConfig.h"
#include "CCmdConfig.h"
#include "CTrgwTcpServer.h"
#include "CPdeTcpServer.h"
#include "CFailCharging.h"
#include "CAppSession.h"
#include "lmproc.h"
#include "iudp.h"
#include "main.h"
#include "COmcIf.h"
#include "IBCErrorTable.h"
#include "CIbcMonitorLog.h"
#include "CIbcDelLog.h"
#include "COverLoadCdrQ.h"
#include "IBCDefine.h"

#include "eSipUtil/string.hxx"
class DebugCallStack
{
	public:
		DebugCallStack(const char * _pszFunction,unsigned int _unLine)
		{
			m_clsFunction = _pszFunction;m_unStartLine=_unLine;m_unEndLine = 0;
			memset(m_szErr,0x00,128);
			LOGGER(_ERR,"DEBUG_CALLSTACK) START %s/%u",_pszFunction,_unLine);
		}
		~DebugCallStack()
		{
			if(m_unEndLine==0)
			LOGGER(_ERR,"DEBUG_CALLSTACK) End %s/*",(eSipUtil::KCSTR)m_clsFunction);
			else
			{
				LOGGER(_ERR,"DEBUG_CALLSTACK) End %s/%u ~ %s/%u",
											(eSipUtil::KCSTR)m_clsFunction,m_unStartLine,
											m_szErr,m_unEndLine);
			}
		}
		void m_fnSetErr(unsigned int _unLine, const char * _pszFormat,...)
		{
			memset(m_szErr,0x00,128); m_unEndLine = _unLine;
			va_list ap;va_start(ap, _pszFormat);vsnprintf(m_szErr,127,_pszFormat, ap);va_end(ap);
		}
		eSipUtil::KString m_clsFunction;
		char m_szErr[128];
		unsigned int m_unStartLine;
		unsigned int m_unEndLine;
};
#define SDEBUG() DebugCallStack clsSDebug(__func__,__LINE__)
#define SERROR(v...) clsSDebug.m_fnSetErr(__LINE__,v)
/************************* IBC Service Global Functions **************************************/
//=======================> Dialog or Session Functions
eDum::Session * g_fnFindSessionFromDialog(eDum::Dialog * _pclsDialog,const char *_pszFuncName=NULL);
AppSession * g_fnFindAppSessionFromDialog(eDum::Dialog * _pclsDialog,const char *_pszFuncName=NULL);
eDum::Session * g_fnFindSessionFromID(eDum::EDialogMethodType_t _T, int _nSessionID,const char *_pszFuncName=NULL);
AppSession * g_fnFindAppSessionFromID(eDum::EDialogMethodType_t _T,int _nSessionID,const char *_pszFuncName=NULL);
eDum::Dialog * g_fnFindDialogFromID(eDum::EDialogMethodType_t _T,int _nDialogID,const char *_pszFuncName=NULL);
eDum::Dialog * g_fnFindUacDialog(eDum::EDialogMethodType_t _T,int _nSessionID,const char *_pszFuncName=NULL);
eDum::Dialog * g_fnFindUasDialog(eDum::EDialogMethodType_t _T,int _nSessionID,const char *_pszFuncName=NULL);
eDum::Dialog * g_fnGetOppDialog(eDum::Dialog * _pclsDialog,const char *_pszFuncName=NULL);
IBCAppData_t * g_fnFindAppDataFromDialog(eDum::Dialog * _pclsDialog,const char *_pszFuncName=NULL);
IBCAppData_t * g_fnFindAppDataFromID(eDum::EDialogMethodType_t _T,int _nSessionID,const char *_pszFuncName=NULL);
const char * g_fnGetCallID(eDum::Session * _pclsSession, eDum::Dialog * _pclsDialog,const char *_pszFuncName=NULL);
int g_fnGetTuID(const char * _pszCallID);
//=======================> Queueing To TU
void g_fnSendToTuMsg(eSIP::ResumeAppMessage * _pclsResumeMessage, // Resume Event Ex> TgAS Rsp
						eSIP::DialogAppMessage * _pclsDialogAppMessage,     // Term Event, Send Sip Message
						eSIP::AsyncEventAppMessage * _pclsAsyncMessage);  // Session Event, Ex> Pde, Timer
//=======================> Session Event 
void g_fnSendToTuSessionEvent(EAppSessionEvent_t _eEv,eDum::EDialogMethodType_t _eSessT,
									unsigned int _unSessionID,int _unDialogID, void * _pvData);
//=======================> Resume Event
void g_fnSendToTuResumeEvent(void * _pvData, int _nLen,
											eDum::EDialogMethodType_t _eSessT,
											unsigned int _unSessID,
											unsigned int _unDialogID,
											unsigned int _unSuspendID,
											unsigned int _unTuIndex,
											eSIP::PfuncStateResumeCallback_t _pfnResume);  // EXT --> TU
//=======================> Build Dialog Event
void g_fnBuildAddHeaderInDialogEvent(eSIP::DialogEvent *_pszDialogEvent, 
																	const char * _pszHeader,
																	const char * _pszValue, ...);
//=======================> Dialog Event
bool g_fnStateDropSession(StackStateCbInfo_t & _rclsInfo, EIBCErrors_t _eReason,
													resip::MethodTypes _eSeqMethod=resip::UNKNOWN);
bool g_fnStateFailResponse(StackStateCbInfo_t &_rclsInfo,EIBCErrors_t _eReason,
													resip::MethodTypes _eSeqMethod=resip::UNKNOWN);
bool g_fnStateOppFailResponse(StackStateCbInfo_t &_rclsInfo,EIBCErrors_t _eReason,
													resip::MethodTypes _eSeqMethod=resip::UNKNOWN);
bool g_fnStateDropResponse(StackStateCbInfo_t &_rclsInfo,EIBCErrors_t _eReason,
													resip::MethodTypes _eSeqMethod=resip::UNKNOWN);
bool g_fnAsyncDropSession(eDum::Session * _pclsSession,EIBCErrors_t _eReason,
																eSIP::EventResult * _pclsResult);
#ifdef DEF_R131
bool g_fnStateErrResponse(StackStateCbInfo_t &_rclsInfo, int  _nRsp);
#endif
#ifdef DEF_VIBC_ENABLE
void g_fnStackBlocking(bool _bNoCheck);
void g_fnStackUnblocking(bool _bNoCheck);
#endif
//=======================> RTE Functions
unsigned int g_fnGetRteID(eDum::Dialog *_pclsDialog);
int g_fnGetReasonInfo(eDum::Dialog * _pclsDialog,AppReason_t * _pstReason,ESrvReason_t _eReason);
bool g_fnIsMimeNode(StackStateCbInfo_t & _rclsInfo);
ERoutingResult_t g_fnDecideRouting(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
								resip::SipMessage  *_pclsSndSipMsg,eSIP::EventResult & _rclsEventResult);
void g_fnCopyRTE_Orig(ERouteOp_t _eOP,
											unsigned int         _uiSeqID,               //IN - route squence id
											SipNodeLocal_t*      _pstOGLocal,       //OUT, target local
											SipNodeRemote_t*     _pstOGRemote, //OUT, target remote
											SipRoute_t*          _pstOGRoute,        //OUT, og route
											char*                _pError);
int g_fnCopyRTEInfo(ERouteOp_t _eOP,
									unsigned int         _uiRSeqID,             //IN - route squence id
									SipNodeLocal_t*      _pstICLocal,        //IN/OUT, ip/port/transport type
									SipNodeRemote_t*     _pstICRemote,   //IN/OUT, ip/port/transport type
									SipNodeLocal_t*      _pstOGLocal,        //OUT, target local
									SipNodeRemote_t*     _pstOGRemote,  //OUT, target remote
									SipRoute_t*          _pstICRoute,          //OUT, ic route
									SipRoute_t*          _pstOGRoute,         //OUT, og route
									char*                _pError);
//=======================> Screening Service
bool g_fnScreenHistoryInfo(StackStateCbInfo_t & _rclsInfo);
bool g_fnServiceScreen(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
														resip::SipMessage  *_pclsOrigMsg);
//=======================> Filtering Service
bool g_fnServiceFilter(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
								resip::SipMessage  *_pclsSndSipMsg,eSIP::EventResult & _rclsEventResult,
								eSIP::EStateCbResultType_t * _pclsRet);
void g_fnConvRequestLine(StackStateCbInfo_t & _rclsInfo);
//========================> TRGW Service
void g_fnSendToTuTgASMsg(void * _pvTgAsMsg, int len);              // TU < -- TRGW
eSIP::EStateCbResultType_t g_fnSendToTgASMediaMsg(StackStateCbInfo_t & _rclsInfo);     // TU --> TRGW
//========================> PDE Service
void g_fnSendToTuPDE(void * _pvTgAsMsg, int len);                                // TU < -- PDE
int g_fnSendToPDEMsg(EPdeReq_t _eReq, StackStateCbInfo_t & _rclsInfo); // TU --> PDE

class TrGWTransID
{
	public:
		// max 4094 ,     12bit Num
		TrGWTransID(){}
		~TrGWTransID()
		{
			if(m_arrTransID) delete [] m_arrTransID;
		}
		void fnInit(unsigned int _unMaxDialog)
		{
			m_arrTransID = new unsigned int [_unMaxDialog];
			for(unsigned int i=0;i<_unMaxDialog;i++) m_arrTransID[i]=1;
		}
		unsigned int fnGetTransID(unsigned int _unDialogIdx)
		{
			unsigned int unTmp = m_arrTransID[_unDialogIdx];
			if(m_arrTransID[_unDialogIdx] >= 4093)
			{
				m_arrTransID[_unDialogIdx] = 1;
			}
			else
			{
				m_arrTransID[_unDialogIdx] = m_arrTransID[_unDialogIdx] + 1;
			}
			return unTmp;
		}
		unsigned int * m_arrTransID;
};
class ServiceApi
{
	public:
		enum{E_RULE_SZ_PATH = 256};
		ServiceApi();
		~ServiceApi();
		static ServiceApi * fnGetInstance();
		static void fnFinalize();
		static const char * fnGetBuildInfo();
		static bool fnIsHAActive();
		void fnLoadRuleLib();
		void fnPreLoadService();
		void fnPostLoadService();
		void fnLoadExternalProcess();
		void fnLoadSessionInfo();
		void fnReLoadSessionInfo();
		void fnUnLoadSessionInfo();
		void fnLoadHASession(eDum::Session &_rclsSession);
		void fnLoadLayerManager();
		void fnLoadOmcIf();
		//=================> Sessions
		bool fnIsValidSessionID(eDum::EDialogMethodType_t _T, int _unSessID);
		eDum::Session * fnGetStackSession(eDum::Dialog * _pclsDialog,unsigned int _unSessionID = 0);
		eDum::Session * fnGetStackRegSession(eDum::Dialog * _pclsDialog,
																		unsigned int _unSessionID = 0);
		eDum::Session * fnGetStackSubSession(eDum::Dialog * _pclsDialog,
																		unsigned int _unSessionID = 0);
		eDum::Session * fnGetStackStandAloneSession(eDum::Dialog * _pclsDialog,
																		unsigned int _unSessionID = 0);
		AppSession * fnGetSession(eDum::Dialog * _pclsDialog, unsigned int _uiSessionID=0);
		AppSession * fnGetRegSession(eDum::Dialog * _pclsDialog, unsigned int _uiSessionID=0);
		AppSession * fnGetSubSession(eDum::Dialog * _pclsDialog, unsigned int _uiSessionID=0);
		AppSession * fnGetStandAloneSession(eDum::Dialog * _pclsDialog, 
																		unsigned int _uiSessionID=0);
		unsigned int fnGetUsageSession(eDum::EDialogMethodType_t _T=eDum::E_DIALOG_METHOD_TYPE_INV);
		unsigned int fnGetUsageDialog(eDum::EDialogMethodType_t _T=eDum::E_DIALOG_METHOD_TYPE_INV);
		//=================> Trgw
		bool fnIsValidService();
		static unsigned int fnGenTrgwTransID(StackStateCbInfo_t & _rclsInfo);
		static void fnGetTrgwTransInfo(unsigned int _unTransID,unsigned int * _punDialog);
		//=================> Pde
		static unsigned int fnGenPdeTransID(EPdeReq_t _eCmd);
		static RUStatLog * fnLoadCdr(char * _pszStatPath, char * _pszFileName);
		static RUStatLog * fnGetCdr();
		static void fnWriteCDR(IBCAppData_t * _pstAppData);
		//=================> Rule Lib 
		RuleLib * m_pclsRuleLib;
		//=================> SCM Config
		IBCConfig * m_pclsScmConf;
		IBCCmdConfig * m_pclsCmdConf;
		//=================> TrGW Instance
		TrgwTcpServer  * m_pclsTrgw;
		TrGWTransID m_clsTrGWTransID;
		//=================> PCDS Instance
		PdeTcpServer * m_pclsPde;
		FailCharging * m_pclsFailCharging;
		unsigned int m_unPid;
		//=================> Stack Session Data Struct
		unsigned int m_unMaxSession;
		unsigned int m_unRegMaxSession;
		unsigned int m_unSubMaxSession;
		unsigned int m_unStandAloneMaxSession;
		eDum::Session * m_pclsSession;
		eDum::Session * m_pclsRegSession;
		eDum::Session * m_pclsSubSession;
		eDum::Session * m_pclsStandAloneSession;
		//=================> App Session Data Struct
		AppSession * m_pclsAppData;
		AppSession * m_pclsRegAppData;
		AppSession * m_pclsSubAppData;
		AppSession * m_pclsStandAloneAppData;
		//==================> Layer Manager Instance 
		CLMProc * m_pclsLmMgr;
		CIUdpThread * m_pclsLmUdp;
		//==================> Omc If instance
		OmcIf * m_pclsOmcIf;
		//==================> HA Status 
		static unsigned int m_unHAStatus;
		static bool m_bSessionLoading;
		//==================> Error Table
		IBCErrors * m_pclsErrorTable;
		static char m_szBuildInfo[256];
		//==================> CDR Instance
		static RUStatLog *m_pclsCdr;
		static OverLoadCdrQ * m_pclsOverCdrQ;
		//==================> Del Logger Instance
		static IbcDelLog * m_pclsDelLog;
		IbcMonitorLog * m_pclsMonitorLog;
		static bool m_bFinished;
	private:
		static ServiceApi *g_pclsService;
		RULock m_clsLock;
};
#endif

