#ifndef ESIP_CROUTE_HXX
#define ESIP_CROUTE_HXX
#include "CRuleUtiles.h"
#include "CRuleFile.h"
#include "CRule.h"
#include "CRuleProc.h"
#include "RuleLibCom.h"

inline const char * fnStringRouteOP(ERouteOp_t _eRoute)
{
	if(_eRoute == E_ROUTE_OP_RSEQ) return "RSEQ";
	else if(_eRoute == E_ROUTE_OP_SSEQ) return "SSEQ";
	else return "NONE";
}
inline ERouteOp_t fnEnumRouteOP(const char * _pszVal)
{
	if(fnRUStrCmp(_pszVal,"RSEQ")==0) return E_ROUTE_OP_RSEQ;
	else if(fnRUStrCmp(_pszVal,"SSEQ")==0) return E_ROUTE_OP_SSEQ; 
	else return E_ROUTE_OP_NONE;
}

class RouteOP;
typedef enum
{
	E_ROUTE_CREATOR_NONE = 8000,
	E_ROUTE_CREATOR_OP,
}EROCreator_t;
#define DEF_ROUTE_COMMENT "// ||route-op||ID||NAME||RTE_TYPE||PRIORITY||SEQ||SRC_SOPS||DST_SOPS||FILTER_OPS||TEMP_IP||TEMP_PORT||RULEs...||"
class RouteOP : public RUObject
{
	public:
		RouteOP();
		~RouteOP();
		RouteOP &operator=(RouteOP & _clsRule);
		void fnLoadCfg(RuleFileParam *_pclsParam);
		void fnDebug();
		void fnResult();
		void fnBuildConfig(char * _pszBuild);
		bool fnMatchRoute(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
												resip::SipMessage * _pclsSndSipMsg);
		// ||route-op||ID||NAME||RTE_TYPE||PRIORITY||SEQ||SRC_SOPS||DST_SOPS||FILTER_OPS||TEMP_IP||TEMP_PORT||RULEs...||
		void fnSetID(int _nID);
		void fnSetName(const char * _pszVal);
		void fnSetRouteType(ERouteOp_t _eRoute);
		void fnSetPriority(int _nPriority);
		void fnSetSeq(int _nSeq);
		void fnSetSrcScreenSet(int _nID);
		void fnSetDstScreenSet(int _nID);
		void fnSetFilterSet(int _nID);
		void fnSetTempIp(const char * _pszVal);
		void fnSetTempPort(int _nPort);
		void fnSetRuleSet(LmRuleSet_t *_pstRuleSet);
		bool fnIsValid();
		unsigned int m_uiID;
		char *m_szName;
		ERouteOp_t m_eRoute;
		unsigned int m_uiPriority;
		unsigned int m_uiSeqID;
		unsigned int m_uiSrcSOPS;
		unsigned int m_uiDstSOPS;
		unsigned int m_uiFOPS;
		char *m_pszIP;
		unsigned int m_uiPort;
		RuleSet m_clsRuleSet;
		RuleFileParam *m_pclsRawData;
	private:
		unsigned int m_unCnt;
};

class RouteOPMgr : public RuleFile
{
	public:
		RouteOPMgr();
		~RouteOPMgr();
		static RouteOPMgr * fnGetInstance();
		static void fnFinalize();
		void fnMgrAction();
		bool fnLoadOPFile(const char * _pszPath);
		bool fnWriteOPFile(const char * _pszPath);
		bool fnLoadOPMem(const char * _pszSrc);
		bool fnWriteOPMem(char * _pszDst, unsigned int _unSize);
		RouteOP * fnFindRouteOP(unsigned int _uiKey);
		// ||route-op||ID||NAME||RTE_TYPE||PRIORITY||SEQ||SRC_SOPS||DST_SOPS||FILTER_OPS||TEMP_IP||TEMP_PORT||RULEs...||
		bool fnAddRouteOP(unsigned int _uiKey, const char * _pszName,const char * _pszString);
		bool fnAddRouteOP(LmRouteOp_t * _pstRouteOp,LmRuleResult_t & _rstResult);
		bool fnGetRouteOP(unsigned int _uiKey,LmRouteOp_t * _pstRouteOp);
		bool fnModiRouteOP(LmRouteOp_t * _pstRouteOp,LmRuleResult_t & _rstResult);
		bool fnDelRouteOP(unsigned int _uiKey);
		bool fnDelRouteOP(LmRouteOp_t * _pstRouteOp,LmRuleResult_t & _rstResult);
		RouteOP * fnBegin(){return (RouteOP *)m_listRoute.fnBegin();}
		RouteOP * fnNext(){return (RouteOP *)m_listRoute.fnNext();}
		RouteOP * fnBegin(RuListItem_t &_rclsItem){return (RouteOP *)m_listRoute.fnBegin(_rclsItem);}
		RouteOP * fnNext(RuListItem_t &_rclsItem){return (RouteOP *)m_listRoute.fnNext(_rclsItem);}
		bool fnUsingReferenceID(ESrvCfgCategory_t _eT, unsigned int _unID);
		bool fnExistPriority(unsigned int _unPriority);
		void fnDebugRO();
		bool fnIsValid();
		static RouteOPMgr * g_pclsROMgr;
		RUList m_listRoute;
	private:
		void fnInitMgr();
		void fnArrangeLine();
		void fnSortRoute();
		static void * fnCbkCreator(RuleFileParam * _pclsParam, unsigned int * _puiRet);
		static void fnCbkTerminator(void * _pvUser, unsigned int _uiType);
		static void fnCbkAction(void * _pvUser,unsigned int _uiType);
		static bool fnCbkPrioritySort(unsigned long _pv1, unsigned long _pv2);
		static bool fnCbkSortOP(unsigned long _pv1, unsigned long _pv2);
};
#endif

