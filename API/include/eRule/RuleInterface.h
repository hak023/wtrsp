#ifndef RULEINTERFACE_H
#define RULEINTERFACE_H
#include <pthread.h>
//======> stack lib
#include "resip/stack/SipMessage.hxx"
#include "rutil/Logger.hxx"
#include "rutil/TransportType.hxx"
#include "rutil/TimeLimitFifo.hxx"
#include "eSipAppMessage.hxx"
#include "resip/stack/ExtensionParameter.hxx"
//======> dum lib
#include "eDum/eDumLibDef.hxx"
#include "eDum/eDumLibStack.hxx"
#include "eDum/eDumLibSession.hxx"
//======> session lib
#include "eSipType.hxx"
#include "eSipCallback.hxx"
#include "eSipTu.hxx"
#include "eSipDef.hxx"
//======> rule lib
#include "RuleLibCom.h"
#include "CRuleProc.h"
#include "CRouteProc.h"
#include "CScreenProc.h"
#include "CFilterProc.h"
#include "RuleError.h"
#include "CRUFile.h"

typedef enum
{
	E_RULELIB_CFG_RULE,
	E_RULELIB_CFG_ROUTEOP,
	E_RULELIB_CFG_SCREEN,
	E_RULELIB_CFG_FILTER,
}ERuleLibFile_t;
class RuleLib
{
	public:
		enum{E_RULE_SZ_PATH = 256};
		RuleLib();
		~RuleLib();
		static RuleLib * fnGetInstance();
		static void fnFinalize();
		static const char * fnGetBuildInfo();
		//=============> Read/Write Config
		bool fnLoadLibFile(ERuleLibFile_t _eFile, const char * _szPath);
		bool fnWriteLibFile(ERuleLibFile_t _eFile, const char * _szPath = NULL);
		bool fnLoadLibMem(ERuleLibFile_t _eFile, const char * _pszSrc);
		bool fnWriteLibMem(ERuleLibFile_t _eFile, char * _pszDst, unsigned int _unSize);
		void fnDebugLoad();
		void fnReLoadLibFile(ERuleLibFile_t _eFile);
		void fnReLoadLibMem(ERuleLibFile_t _eFile);
		void fnReLoadLibAuto(ERuleLibFile_t _eFile);
		/**************** Rule Api ****************************/
		bool fnFindRule(unsigned int _uiKey,Rule & _clsRule);
		bool fnGetRule(unsigned int _uiKey,LmRule_t * _pstLmRule);
		bool fnAddRule(LmRule_t * _pstLmRule,LmRuleResult_t &_rstResult);
		bool fnModiRule(LmRule_t * _pstLmRule,LmRuleResult_t &_rstResult);
		int fnDisRule(LmRule_t * _pstLmRule,unsigned int _unMax);
		int fnDisAttRule(ERuleAttribute_t _eAtt,LmRule_t * _pstLmRule,unsigned int _unMax);
		bool fnDelRule(LmRule_t * _pstLmRule,LmRuleResult_t &_rstResult);
		/**************** Screen Api **************************/
		bool fnFindScreenOP(unsigned int _uiKey, ScreenOP & _clsOP);
		bool fnGetScreenOP(unsigned int _uiKey,LmScreenOp_t * _pstOp);
		bool fnAddScreenOP(LmScreenOp_t * _pstOp,LmRuleResult_t & _rstResult);
		bool fnModiScreenOP(LmScreenOp_t * _pstOp,LmRuleResult_t & _rstResult);
		int fnDisScreenOP(LmScreenOp_t * _pstOp,unsigned int _unMax);
		bool fnDelScreenOP(LmScreenOp_t * _pstOp,LmRuleResult_t & _rstResult);
		bool fnFindScreenOPS(unsigned int _uiKey, ScreenOPS & _clsOPS);
		bool fnGetScreenOPS(unsigned int _uiKey, LmScreenOpSet_t * _pstOpSet);
		bool fnAddScreenOPS(LmScreenOpSet_t * _pstOpSet,LmRuleResult_t & _rstResult);
		bool fnModiScreenOPS(LmScreenOpSet_t * _pstOpSet,LmRuleResult_t & _rstResult);
		int fnDisScreenOPS(LmScreenOpSet_t * _pstOpSet, unsigned int _unMax);
		bool fnDelScreenOPS(LmScreenOpSet_t * _pstOpSet,LmRuleResult_t & _rstResult);
		bool fnFindScreenAct(unsigned int _uiKey, ScreenAct & _clsAct);
		bool fnGetScreenAct(unsigned int _uiKey, LmScreenAct_t * _pstAct);
		bool fnAddScreenAct(LmScreenAct_t * _pstAct,LmRuleResult_t & _rstResult);
		bool fnModiScreenAct(LmScreenAct_t * _pstAct,LmRuleResult_t & _rstResult);
		int fnDisScreenAct(LmScreenAct_t * _pstAct, unsigned int _unMax);
		bool fnDelScreenAct(LmScreenAct_t * _pstAct,LmRuleResult_t & _rstResult);
		bool fnScreening(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
												resip::SipMessage * _pclsOrigMsg,
												unsigned int _unSOPS,
												bool _bTrace,
												char * _pszTrace,
												unsigned int _unTraceSize);
		/***************** RouteOP Api *************************/
		bool fnFindRouteOP(unsigned int _uiKey,RouteOP & _plsRouteOP);
		bool fnGetRouteOP(unsigned int _uiKey,LmRouteOp_t * _pstRouteOp);
		bool fnAddRouteOP(LmRouteOp_t * _pstRouteOp,LmRuleResult_t & _rstResult);
		bool fnModiRouteOP(LmRouteOp_t * _pstRouteOp,LmRuleResult_t & _rstResult);
		int fnDisRouteOP(LmRouteOp_t * _pstRouteOp, unsigned int _unMax);
		bool fnDelRouteOP(LmRouteOp_t * _pstRouteOp,LmRuleResult_t & _rstResult);
		RouteOP * fnRouting(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
														resip::SipMessage * _pclsSndSipMsg);
		/***************** Filter Api *****************************/
		bool fnFindFilterOP(unsigned int _uiKey, FilterOP & _clsOP);
		bool fnGetFilterOP(unsigned int _uiKey, LmFilterOp_t * _pstOp);
		bool fnAddFilterOP(LmFilterOp_t * _pstOp,LmRuleResult_t & _rstResult);
		bool fnModiFilterOP(LmFilterOp_t * _pstOp,LmRuleResult_t & _rstResult);
		int fnDisFilterOP(LmFilterOp_t * _pstOp,unsigned int _unMax);
		bool fnDelFilterOP(LmFilterOp_t * _pstOp,LmRuleResult_t & _rstResult);
		bool fnFindFilterOPS(unsigned int _uiKey, FilterOPS & _clsOPS);
		bool fnGetFilterOPS(unsigned int _uiKey, LmFilterOpSet_t * _pstOpSet);
		bool fnAddFilterOPS(LmFilterOpSet_t * _pstOpSet,LmRuleResult_t & _rstResult);
		bool fnModiFilterOPS(LmFilterOpSet_t * _pstOpSet,LmRuleResult_t & _rstResult);
		int fnDisFilterOPS(LmFilterOpSet_t * _pstOpSet, unsigned int _unMax);
		bool fnDelFilterOPS(LmFilterOpSet_t * _pstOpSet,LmRuleResult_t & _rstResult);
		bool fnIsValidLoad();   // validatioln check
		/*
		bool fnFindFilterAct(unsigned int _uiKey, FilterAct & _clsAct);
		bool fnGetFilterAct(unsigned int _uiKey, LmFilterAct_t * _pstAct);
		bool fnAddFilterAct(LmFilterAct_t * _pstAct,LmRuleResult_t & _rstResult);
		bool fnModiFilterAct(LmFilterAct_t * _pstAct,LmRuleResult_t & _rstResult);
		int fnDisFilterAct(LmFilterAct_t * _pstAct, unsigned int _unMax);
		bool fnDelFilterAct(LmFilterAct_t * _pstAct,LmRuleResult_t & _rstResult);
		*/
		EFilterAttribute_t fnFiltering(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
												resip::SipMessage * _pclsSndSipMsg,
												unsigned int _unFOPS,
												bool _bTrace,
												char * _pszTrace,
												unsigned int _unTraceSize);
		/***************** Refresh Config Function ************************/
		void fnCheckChangedFile();    // must call function periodically.   ex)1 per sec
		resip::RWMutex m_Mutex;
		static RuleLib *g_pclsRuleLib;
		RuleMgr * m_pclsRuleMgr;
		ScreenMgr * m_pclsScreenMgr;
		RouteOPMgr * m_pclsRouteMgr;
		FilterMgr * m_pclsFilterMgr;
		unsigned int m_unPid;
		static char m_szBuildInfo[256];
	private:
		// resiprocate readwrite lock
		void fnWLock(){m_Mutex.writelock();}
		void fnRLock(){m_Mutex.readlock();}
		void fnUnlock(){m_Mutex.unlock();}
		//==============> Read/Write Config
		bool fnLoadRuleFile(const char * _szPath);
		bool fnLoadScreenFile(const char * _szPath);
		bool fnLoadRouteFile(const char * _szPath);
		bool fnLoadFilterFile(const char * _szPath);
		bool fnWriteRuleFile(const char * _szPath);
		bool fnWriteScreenFile(const char * _szPath);
		bool fnWriteRouteFile(const char * _szPath);
		bool fnWriteFilterFile(const char * _szPath);
		bool fnLoadRuleMem(const char * _pszSrc);
		bool fnLoadScreenMem(const char * _pszSrc);
		bool fnLoadRouteMem(const char * _pszSrc);
		bool fnLoadFilterMem(const char * _pszSrc);
		bool fnWriteRuleMem(char * _pszDst, unsigned int _unSize);
		bool fnWriteScreenMem(char * _pszDst, unsigned int _unSize);
		bool fnWriteRouteMem(char * _pszDst, unsigned int _unSize);
		bool fnWriteFilterMem(char * _pszDst, unsigned int _unSize);
		static void fnCbkChangedRuleFile(void * _pvFile);
		static void fnCbkChangedScreenFile(void * _pvFile);
		static void fnCbkChangedFilterFile(void * _pvFile);
		static void fnCbkChangedRouteFile(void * _pvFile);
		RUString m_clsRulePath;
		RUString m_clsScreenPath;
		RUString m_clsFilterPath;
		RUString m_clsRoutePath;
		char * m_pszRuleMem;
		char * m_pszScreenMem;
		char * m_pszFilterMem;
		char * m_pszRouteMem;
		FileChangedInfo m_stChkRuleInfo;
		FileChangedInfo m_stChkScreenInfo;
		FileChangedInfo m_stChkFilterInfo;
		FileChangedInfo m_stChkRouteInfo;
};
#endif

