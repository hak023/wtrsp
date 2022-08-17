#ifndef _ESIP_CFILTER_HXX
#define _ESIP_CFILTER_HXX
#include "CRuleProc.h"
#include "RuleLibCom.h"

class FilterOP;
class FilterMgr;

typedef enum
{
	E_FILTER_CREATOR_NONE = 0,
	E_FILTER_CREATOR_OP,
	E_FILTER_CREATOR_OPS,
}EFCreator_t;
inline const char *fnStringFilterCreator(EFCreator_t _eType)
{
	switch(_eType)
	{
		case E_FILTER_CREATOR_OP: return "FOP";
		case E_FILTER_CREATOR_OPS: return "FOPS";
		default: return "";
	};
	return "";
}
inline EFCreator_t fnEnumFilterCreator(const char * _pszParam)
{
	if(fnRUStrCmp(_pszParam,"filter-op")==0) return E_FILTER_CREATOR_OP;
	if(fnRUStrCmp(_pszParam,"filter-ops")==0) return E_FILTER_CREATOR_OPS;
	return E_FILTER_CREATOR_NONE;	
}

inline const char * fnStringFilterAttr(EFilterAttribute_t _eT)
{
	if(_eT == E_FILTER_ACT_DENY) return "DENY";
	else if(_eT == E_FILTER_ACT_DISCARD) return "DISCARD";
	return "Unknown";
}
inline EFilterAttribute_t fnEnumFilterAttr(const char * _pszAttr)
{
	if(fnRUStrCaseCmp(_pszAttr,"DENY")==0) return E_FILTER_ACT_DENY;
	if(fnRUStrCmp(_pszAttr,"DISCARD")==0) return E_FILTER_ACT_DISCARD;
	return E_FILTER_ACT_NONE;
}
#define DEF_FILTEROP_COMMENT "// ||filter-op||ID||NAME||FILTER_Attribute||RULE_IDs...||"
class FilterOP : public RUObject
{
	public : 
		FilterOP();
		~FilterOP();
		FilterOP & operator=( FilterOP &_clsOP);
		void fnLoadCfg(RuleFileParam *pclsParam);
		void fnDebug();
		void fnDisplay(char * _pszLog, unsigned int _unSize);
		void fnBuildConfig(char * pszBuild);
		void fnSetID(int _nID);
		void fnSetName(const char * _pszVal);
		void fnSetAttribute(int _nAttr);
		void fnSetRuleSet(LmRuleSet_t * pstRuleSet);
		bool fnMatchFilter(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
												resip::SipMessage * _pclsSndSipMsg);
		char * fnTrace(char * _pszTrace,unsigned int _unLen);
		bool fnIsValid();
		unsigned int m_Id;
		char *m_pszName;
		unsigned int m_unPriority;
		EFilterAttribute_t m_eAttribute;
		RuleSet m_clsRuleSet;
		RuleFileParam * m_pclsRawData;
};

class FilterOPS : public RUObject
{
	public:
		FilterOPS();
		~FilterOPS();
		FilterOPS & operator=(FilterOPS &_clsOPS);
		void fnLoadCfg(RuleFileParam *pclsParam);
		void fnLoadOPList(const char * _pszString);
		void fnDebug();
		void fnDisplay(char * _pszLog,unsigned int _unSize);
		char * fnBuildConfg(char *_pszDisplay);
		void fnAction(PfuncActionList_t _pfnAction, void * _pvUser);
		OPListObj * fnBegin(){return (OPListObj *)m_listOPS.fnBegin();}
		OPListObj * fnNext(){return (OPListObj *)m_listOPS.fnNext();}
		OPListObj * fnBegin(RuListItem_t &_rclsItem){return (OPListObj *)m_listOPS.fnBegin(_rclsItem);}
		OPListObj * fnNext(RuListItem_t &_rclsItem){return (OPListObj *)m_listOPS.fnNext(_rclsItem);}
		FilterOP * fnBeginOP();
		FilterOP * fnNextOP();
		FilterOP * fnBeginOP(RuListItem_t &_rclsItem);
		FilterOP * fnNextOP(RuListItem_t &_rclsItem);
		bool fnLoadSet(int _nID);
		void fnSetID(int _nID);
		void fnSetName(const char * _pszVal);
		void fnSetOPList(LmServiceOPList_t *_pstOpList);
		bool fnIsValid();
		RUList m_listOPS;
		unsigned int m_Id;
		char *m_pszName;
		RuleFileParam * m_pclsRawData;
	private:
		static void fnCbkOpListBuild(unsigned long _ulData, void * _pvUser);
		static void fnCbkOPListCopy(unsigned long _ulData, void * _pvKey);
};
class FilterMgr : public RuleFile
{
	public:
		FilterMgr();
		~FilterMgr();
		static FilterMgr * fnGetInstance();
		static void fnFinalize();
		void fnMgrAction();
		bool fnLoadFilterFile(const char * _pszPath);
		bool fnWriteFilterFile(const char * _pszPath);
		bool fnLoadFilterMem(const char * _pszSrc);
		bool fnWriteFilterMem(char * _pszDst, unsigned int _unSize);
		// ||filter-op||ID||NAME||FilterAttribute||RULE_01 OR RULE_02 AND RULE_04||
		FilterOP * fnFindFilterOP(unsigned int _uiKey);
		bool fnAddFilterOP(unsigned int _uiKey,const char * _pszName,const char * _pszString);
		bool fnAddFilterOP(LmFilterOp_t *_pstOp,LmRuleResult_t & _rstResult);
		bool fnGetFilterOP(unsigned int _uiKey, LmFilterOp_t * _pstOp);
		bool fnModiFilterOP(LmFilterOp_t *_pstOp,LmRuleResult_t & _rstResult);
		bool fnDelFilterOP(unsigned int _uiKey);
		bool fnDelFilterOP(LmFilterOp_t *_pstOp,LmRuleResult_t & _rstResult);
		bool fnIsValidOPS(LmFilterOpSet_t * _pstOpSet);
		// ||filter-ops||ID||NAME||OP1/OP2/...
		FilterOPS * fnFindFilterOPS(unsigned int _uiKey);
		bool fnAddFilterOPS(unsigned int _uiKey,const char * _pszName,const char * _pszString);
		bool fnAddFilterOPS(LmFilterOpSet_t *_pstOp,LmRuleResult_t & _rstResult);
		bool fnGetFilterOPS(unsigned int _uiKey, LmFilterOpSet_t * _pstOpSet);
		bool fnModiFilterOPS(LmFilterOpSet_t *_pstOp,LmRuleResult_t & _rstResult);
		bool fnDelFilterOPS(unsigned int _uiKey);
		bool fnDelFilterOPS(LmFilterOpSet_t *_pstOp,LmRuleResult_t & _rstResult);
		bool fnUsingReferenceID(ESrvCfgCategory_t _eT, unsigned int _unID);
		FilterOP * fnBeginOP(){return(FilterOP *)m_mapFOP.fnBeginI();}
		FilterOP * fnNextOP(){return (FilterOP *)m_mapFOP.fnNextI();}
		FilterOPS * fnBeginOPS(){return(FilterOPS *)m_mapFOPS.fnBeginI();}
		FilterOPS * fnNextOPS(){return (FilterOPS *)m_mapFOPS.fnNextI();}
		FilterOP * fnBeginOP(RuMapIntItem_t & _rclsItem){return(FilterOP *)m_mapFOP.fnBeginI(_rclsItem);}
		FilterOP * fnNextOP(RuMapIntItem_t & _rclsItem){return (FilterOP *)m_mapFOP.fnNextI(_rclsItem);}
		FilterOPS * fnBeginOPS(RuMapIntItem_t & _rclsItem){return(FilterOPS *)m_mapFOPS.fnBeginI(_rclsItem);}
		FilterOPS * fnNextOPS(RuMapIntItem_t & _rclsItem){return (FilterOPS *)m_mapFOPS.fnNextI(_rclsItem);}
		void fnDebugFilter();
		bool fnIsValid();
		RUMap m_mapFOP;
		RUMap m_mapFOPS;
		RUMap m_mapFA;
		static FilterMgr * g_pclsFilterMgr;
	private:
		void fnInitMgr();
		void fnArrangeLine();
		void fnSortFilter();
		static void * fnCbkCreator(RuleFileParam * _pclsParam, unsigned int * _puiRet);
		static void fnCbkTerminator(void * _pvUser, unsigned int _uiType);
		static void fnCbkAction(void * _pvUser,unsigned int _uiType);
		static bool fnCbkSortOP(unsigned long _pv1, unsigned long _pv2);
		static bool fnCbkSortOPS(unsigned long _pv1, unsigned long _pv2);
};
#endif

