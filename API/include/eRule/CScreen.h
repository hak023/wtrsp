#ifndef _ESIP_CSCREEN_HXX
#define _ESIP_CSCREEN_HXX

#include "CRule.h"
#include "RuleLibCom.h"

class ScreenOP;
class ScreenOPS;
class ScreenMgr;

typedef enum
{
	E_SCREEN_CREATOR_NONE = 0,
	E_SCREEN_CREATOR_OP,
	E_SCREEN_CREATOR_OPS,
	E_SCREEN_CREATOR_ACT,
}ESCreator_t;
inline const char *fnStringScreenCreator(ESCreator_t _eType)
{
	switch(_eType)
	{
		case E_SCREEN_CREATOR_OP: return "SOP";
		case E_SCREEN_CREATOR_OPS: return "SOPS";
		case E_SCREEN_CREATOR_ACT: return "SACT";
		default: return "";
	};
	return "";
}
inline ESCreator_t fnEnumScreenCreator(const char * _pszParam)
{
	if(fnRUStrCmp(_pszParam,"screen-op")==0) return E_SCREEN_CREATOR_OP;
	else if(fnRUStrCmp(_pszParam,"screen-ops")==0) return E_SCREEN_CREATOR_OPS;
	else if(fnRUStrCmp(_pszParam,"screen-act")==0) return E_SCREEN_CREATOR_ACT;
	return E_SCREEN_CREATOR_NONE;	
}
class TblScrAct
{
	public:
		inline static const char * fnGetString(EScreenAct_t _eAct){ return s_arrEnum[_eAct];}
		inline static EScreenAct_t fnGetEnum(const char * _pszParam)
		{
			if(fnRUStrCmp(_pszParam,"ADD_HDR_VALUE")==0) return E_SCREEN_ACTION_ADD_HDR_VALUE;    
			else if(fnRUStrCmp(_pszParam,"ADD_HDR_PARAM")==0) return E_SCREEN_ACTION_ADD_HDR_PARAM;    
			else if(fnRUStrCmp(_pszParam,"DEL_HDR_NAME")==0) return E_SCREEN_ACTION_DEL_HDR_NAME;  
			else if(fnRUStrCmp(_pszParam,"DEL_HDR_PARAM")==0) return E_SCREEN_ACTION_DEL_HDR_PARAM;
			else if(fnRUStrCmp(_pszParam,"CVT_REQ_URI")==0) return E_SCREEN_ACTION_CVT_REQ_URI;
			else if(fnRUStrCmp(_pszParam,"ADD_URI_GEN_PARAM")==0) return E_SCREEN_ACTION_ADD_URI_GEN_PARAM;
			else if(fnRUStrCmp(_pszParam,"ADD_URI_HDR_PARAM")==0) return E_SCREEN_ACTION_ADD_URI_HDR_PARAM;  
			else if(fnRUStrCmp(_pszParam,"ADD_URI_SCHEME")==0) return E_SCREEN_ACTION_ADD_URI_SCHEME;
			else if(fnRUStrCmp(_pszParam,"ADD_URI_USERNAME")==0) return E_SCREEN_ACTION_ADD_URI_USERNAME;
			else if(fnRUStrCmp(_pszParam,"ADD_URI_HOST")==0) return E_SCREEN_ACTION_ADD_URI_HOST;
			else if(fnRUStrCmp(_pszParam,"ADD_URI_PORT")==0) return E_SCREEN_ACTION_ADD_URI_PORT;      
			else if(fnRUStrCmp(_pszParam,"ADD_URI_DISPLAYNAME")==0) return E_SCREEN_ACTION_ADD_URI_DISPLAYNAME;      
			else if(fnRUStrCmp(_pszParam,"ADD_URI_STRING")==0) return E_SCREEN_ACTION_ADD_URI_STRING;
			else if(fnRUStrCmp(_pszParam,"DEL_URI_DISPLAYNAME")==0) return E_SCREEN_ACTION_DEL_URI_DISPLAYNAME;
			else if(fnRUStrCmp(_pszParam,"DEL_URI_GEN_PARAM")==0) return E_SCREEN_ACTION_DEL_URI_GEN_PARAM;
			else if(fnRUStrCmp(_pszParam,"DEL_URI_HDR_PARAM")==0) return E_SCREEN_ACTION_DEL_URI_HDR_PARAM;  
			else if(fnRUStrCmp(_pszParam,"DEL_URI_SCHEME")==0) return E_SCREEN_ACTION_DEL_URI_SCHEME;
			else if(fnRUStrCmp(_pszParam,"DEL_URI_USERNAME")==0) return E_SCREEN_ACTION_DEL_URI_USERNAME;
			else if(fnRUStrCmp(_pszParam,"DEL_URI_HOST")==0) return E_SCREEN_ACTION_DEL_URI_HOST;
			else if(fnRUStrCmp(_pszParam,"DEL_URI_PORT")==0) return E_SCREEN_ACTION_DEL_URI_PORT;      
			else if(fnRUStrCmp(_pszParam,"DEL_URI_STRING")==0) return E_SCREEN_ACTION_DEL_URI_STRING;
			else if(fnRUStrCmp(_pszParam,"MOV_PARAM")==0) return E_SCREEN_ACTION_MOV_PARAM;      
			else if(fnRUStrCmp(_pszParam,"MOV_URI_PARAM")==0) return E_SCREEN_ACTION_MOV_URI_PARAM;      
			else if(fnRUStrCmp(_pszParam,"MOV_URI_HDR")==0) return E_SCREEN_ACTION_MOV_URI_HDR;	
			else if(fnRUStrCmp(_pszParam,"CVT_RSP")==0) return E_SCREEN_ACTION_CVT_RSP;	
			else if(fnRUStrCmp(_pszParam,"CVT_URI_SCHEME")==0) return E_SCREEN_ACTION_CVT_URI_SCHEME;	
			return E_SCREEN_ACTION_NONE;	
	}
	private:
		static const char * s_arrEnum[E_SCREEN_ACTION_CVT_URI_SCHEME+1];
};
inline const char *fnStringScreenAct(EScreenAct_t _eType)
{
	return TblScrAct::fnGetString(_eType);
}
inline EScreenAct_t fnEnumScreenAct(const char * _pszParam)
{
	return TblScrAct::fnGetEnum(_pszParam);
}


#define DEF_SCREENOP_COMMENT "// ||screen-op||ID||NAME||SCR_ACT_ID||RULE_IDs...||"
class ScreenOP : public RUObject
{
	public : 
		ScreenOP();
		~ScreenOP();
		ScreenOP & operator=( ScreenOP &_clsOP);
		void fnLoadCfg(RuleFileParam *pclsParam);
		void fnDebug();
		void fnDisplay(char * _pszLog, unsigned int _unSize);
		void fnBuildConfig(char * pszBuild);
		void fnSetID(int _nID);
		void fnSetName(const char * _pszVal);
		void fnSetActID(int _nID);
		void fnSetRuleSet(LmRuleSet_t * pstRuleSet);
		bool fnIsValid();
		unsigned int m_Id;
		char *m_pszName;
		unsigned int m_uiActID;
		RuleSet m_clsRuleSet;
		RuleFileParam * m_pclsRawData;
};
#define DEF_SCREENOPS_COMMENT "// ||screen-ops||ID||NAME||OP_1/OP_2/OP_3...||"
class ScreenOPS : public RUObject
{
	public:
		ScreenOPS();
		~ScreenOPS();
		ScreenOPS & operator=(ScreenOPS &_clsOPS);
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


#define DEF_SCREENACT_COMMENT "// ||screen-act||ID||NAME||ACTION||POS||VALUE1||VALUE2||VALUE3||"
class ScreenAct : public RUObject
{
	public:
		ScreenAct();
		~ScreenAct();
		ScreenAct & operator=(ScreenAct & _clsAct);
		void fnLoadCfg(RuleFileParam *pclsParam);
		void fnDebug();
		void fnDisplay(char * _pszLog,unsigned int _unSize);
		void fnBuildConfig(char * pszBuild);
		void fnSetID(int _nID);
		void fnSetName(const char * _pszVal);
		void fnSetAct(EScreenAct_t _eAct);
		void fnSetPos(EMsgPos_t _ePos);
		void fnSetVal1(const char * _pszVal);
		void fnSetVal2(const char * _pszVal);
		void fnSetVal3(const char * _pszVal);
		unsigned int m_uiID;
		char * m_pszName;
		EScreenAct_t m_eAction;
		EMsgPos_t m_ePos;
		char * m_pszValue1;
		char * m_pszValue2;
		char * m_pszValue3;
		RuleFileParam * m_pclsRawData;
	private:
		unsigned int m_unCnt;
};
class ScreenMgr : public RuleFile
{
	public:
		ScreenMgr();
		~ScreenMgr();
		static ScreenMgr * fnGetInstance();
		static void fnFinalize();
		void fnMgrAction();
		bool fnLoadScreenFile(const char * _pszPath);
		bool fnWriteScreenFile(const char * _pszPath);
		bool fnLoadScreenMem(const char * _pszSrc);
		bool fnWriteScreenMem(char * _pszDst, unsigned int _unSize);
		ScreenOP * fnFindScreenOP(unsigned int _uiKey);
		ScreenOPS * fnFindScreenOPS(unsigned int _uiKey);
		ScreenAct *fnFindScreenAct(unsigned int _uiKey);
		// ||screen-op||ID||NAME||SCR_ACT_ID||RULE_01 OR RULE_02 AND RULE_04||
		bool fnAddScreenOP(unsigned int _uiKey,const char * _pszName,const char * _pszString);
		bool fnGetScreenOP(unsigned int _uiKey,LmScreenOp_t * _pstOp);
		bool fnAddScreenOP(LmScreenOp_t *_pstOp,LmRuleResult_t & _rstResult);
		bool fnModiScreenOP(LmScreenOp_t *_pstOp,LmRuleResult_t & _rstResult);
		bool fnDelScreenOP(unsigned int _uiKey);
		bool fnDelScreenOP(LmScreenOp_t *_pstOp,LmRuleResult_t & _rstResult);
		// ||screen-ops||ID||NAME||SCR_OP1/SCR_OP2/SCR_OP3/...
		bool fnIsValidScreenOPS(LmScreenOpSet_t * _pstOpSet,bool _bAdd);
		bool fnAddScreenOPS(unsigned int _uiKey,const char * _pszName,const char * _pszString);
		bool fnAddScreenOPS(LmScreenOpSet_t * _pstOpSet,LmRuleResult_t & _rstResult);
		bool fnGetScreenOPS(unsigned int _uiKey, LmScreenOpSet_t * _pstOpSet);
		bool fnModiScreenOPS(LmScreenOpSet_t * _pstOpSet,LmRuleResult_t & _rstResult);
		bool fnDelScreenOPS(unsigned int _uiKey);
		bool fnDelScreenOPS(LmScreenOpSet_t * _pstOpSet,LmRuleResult_t & _rstResult);
		// ||screen-act||ID||NAME||ACTION||POS||VALUE1||VALUE2||VALUE3
		bool fnAddScreenAct(unsigned int _uiKey,const char * _pszName,const char * _pszString);
		bool fnAddScreenAct(LmScreenAct_t *_pstAct,LmRuleResult_t & _rstResult);
		bool fnGetScreenAct(unsigned int _uiKey, LmScreenAct_t * _pstAct);
		bool fnModiScreenAct(LmScreenAct_t *_pstAct,LmRuleResult_t & _rstResult);
		bool fnDelScreenAct(unsigned int _uiKey);
		bool fnDelScreenAct(LmScreenAct_t *_pstAct,LmRuleResult_t & _rstResult);
		ScreenOP * fnBeginOP(){return(ScreenOP *)m_mapSOP.fnBeginI();}
		ScreenOP * fnNextOP(){return (ScreenOP *)m_mapSOP.fnNextI();}
		ScreenOPS * fnBeginOPS(){return (ScreenOPS *)m_mapSOPS.fnBeginI();}
		ScreenOPS * fnNextOPS(){return (ScreenOPS *)m_mapSOPS.fnNextI();}
		ScreenAct * fnBeginAct(){return (ScreenAct *)m_mapSA.fnBeginI();}
		ScreenAct * fnNextAct(){return (ScreenAct *)m_mapSA.fnNextI();}
		ScreenOP * fnBeginOP(RuMapIntItem_t &_rclsItem){return(ScreenOP *)m_mapSOP.fnBeginI(_rclsItem);}
		ScreenOP * fnNextOP(RuMapIntItem_t &_rclsItem){return (ScreenOP *)m_mapSOP.fnNextI(_rclsItem);}
		ScreenOPS * fnBeginOPS(RuMapIntItem_t &_rclsItem){return (ScreenOPS *)m_mapSOPS.fnBeginI(_rclsItem);}
		ScreenOPS * fnNextOPS(RuMapIntItem_t &_rclsItem){return (ScreenOPS *)m_mapSOPS.fnNextI(_rclsItem);}
		ScreenAct * fnBeginAct(RuMapIntItem_t &_rclsItem){return (ScreenAct *)m_mapSA.fnBeginI(_rclsItem);}
		ScreenAct * fnNextAct(RuMapIntItem_t &_rclsItem){return (ScreenAct *)m_mapSA.fnNextI(_rclsItem);}
		bool fnUsingReferenceID(ESrvCfgCategory_t _eT, unsigned int _unID);
		void fnDebugScreen();
		bool fnIsValid();
		RUMap m_mapSOP;
		RUMap m_mapSOPS;
		RUMap m_mapSA;
		static ScreenMgr * g_pclsScreenMgr;
	private:
		void fnInitMgr();
		void fnArrangeLine();
		void fnSortScreen();
		static void * fnCbkCreator(RuleFileParam * _pclsParam, unsigned int * _puiRet);
		static void fnCbkTerminator(void * _pvUser, unsigned int _uiType);
		static void fnCbkAction(void * _pvUser,unsigned int _uiType);
		static bool fnCbkSortOPS(unsigned long _pv1, unsigned long _pv2);
		static bool fnCbkSortOP(unsigned long _pv1, unsigned long _pv2);
		static bool fnCbkSortAct(unsigned long _pv1, unsigned long _pv2);
};
#endif

