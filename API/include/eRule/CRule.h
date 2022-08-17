
#ifndef _ESIP_RULE_HXX
#define _ESIP_RULE_HXX
#include "CRuleUtiles.h"
#include "CRuleFile.h"
#include "CAppParser.h"
#include "RuleFeature.h"

typedef enum
{
	E_RULE_CREATOR_NONE = 7000,
	E_RULE_CREATOR_RULE,
}ERCreator_t;
inline const char *fnStringOnOff(bool _bBool)
{
	if(_bBool) return "ON";
	else return "OFF";
}
inline const char *fnStringBool(bool _bBool)
{
	if(_bBool) return "TRUE";
	else return "FALSE";
}
inline const char *fnStringRuleType(ERuleType_t _eType)
{
	switch(_eType)
	{
		case E_RULE_RULE: return "RULE";
		case E_RULE_RULESET: return "RULESET";
		default: return "";
	};
	return "";
}
inline ERuleType_t fnEnumRuleType(const char * _szType)
{
	if(fnRUStrCmp(_szType,"RULE")==0) return E_RULE_RULE;
	else if(fnRUStrCmp(_szType,"RULESET")==0) return E_RULE_RULESET;
	return E_RULE_NONE;	
}
inline const char *fnStringRuleMatch(ERuleMatch_t _eType)
{
	switch(_eType)
	{
		case E_RULE_MATCH_FULL: return "FULL";
		case E_RULE_MATCH_PART: return "PART";
		case E_RULE_MATCH_NUM: return "NUM";
		case E_RULE_MATCH_SEQ: return "SEQ";
		default: return "";
	};
	return "";
}
inline ERuleMatch_t fnEnumRuleMatch(const char * _szType)
{
	if(fnRUStrCmp(_szType,"FULL")==0) return E_RULE_MATCH_FULL;
	else if(fnRUStrCmp(_szType,"PART")==0) return E_RULE_MATCH_PART;
	else if(fnRUStrCmp(_szType,"NUM")==0) return E_RULE_MATCH_NUM;
	else if(fnRUStrCmp(_szType,"SEQ")==0) return E_RULE_MATCH_SEQ;
	return E_RULE_MATCH_NONE;	
}
class TblRuleAtt
{
	public:
		inline static const char * fnGetString(ERuleAttribute_t _eAtt){ return s_arrEnum[_eAtt];}
		inline static ERuleAttribute_t fnGetEnum(const char * _szAtt)
		{
			if(fnRUStrCmp(_szAtt,"CALLRING_DOMAIN")==0) return E_RULE_ATT_CALLRING_DOMAIN;
			else if(fnRUStrCmp(_szAtt,"CALLER_DOMAIN")==0) return E_RULE_ATT_CALLER_DOMAIN;
			else if(fnRUStrCmp(_szAtt,"CALLED_DOMAIN")==0) return E_RULE_ATT_CALLED_DOMAIN;
			else if(fnRUStrCmp(_szAtt,"CALLER_PREFIX")==0) return E_RULE_ATT_CALLER_PREFIX;
			else if(fnRUStrCmp(_szAtt,"CALLED_PREFIX")==0) return E_RULE_ATT_CALLED_PREFIX;
			else if(fnRUStrCmp(_szAtt,"CALLER_REALM")==0) return E_RULE_ATT_CALLER_REALM;
			else if(fnRUStrCmp(_szAtt,"CALLED_REALM")==0) return E_RULE_ATT_CALLED_REALM;
			else if(fnRUStrCmp(_szAtt,"HEADER_NAME")==0) return E_RULE_ATT_HEADER_NAME;
			else if(fnRUStrCmp(_szAtt,"HEADER_VALUE")==0) return E_RULE_ATT_HEADER_VALUE;
			else if(fnRUStrCmp(_szAtt,"RECV_LOC")==0) return E_RULE_ATT_RECV_LOC;
			else if(fnRUStrCmp(_szAtt,"REQUEST")==0) return E_RULE_ATT_REQUEST;
			else if(fnRUStrCmp(_szAtt,"RESPONSE_MC")==0) return E_RULE_ATT_RESPONSE_MC;
			else if(fnRUStrCmp(_szAtt,"HEADER_GENVALUE")==0) return E_RULE_ATT_HEADER_GENVALUE;
			else if(fnRUStrCmp(_szAtt,"DISPLAY_NAME")==0) return E_RULE_ATT_DISPLAY_NAME;
			else if(fnRUStrCmp(_szAtt,"URI_GEN_VALUE")==0) return E_RULE_ATT_URI_GEN_VALUE;
			else if(fnRUStrCmp(_szAtt,"URI_GEN_HEADER")==0) return E_RULE_ATT_URI_GEN_HEADER;
			else if(fnRUStrCmp(_szAtt,"URI_USERNAME")==0) return E_RULE_ATT_URI_USERNAME;
			else if(fnRUStrCmp(_szAtt,"HOST")==0) return E_RULE_ATT_HOST;
			else if(fnRUStrCmp(_szAtt,"URI_REALM")==0) return E_RULE_ATT_URI_REALM;
			else if(fnRUStrCmp(_szAtt,"URI_PORT")==0) return E_RULE_ATT_URI_PORT;
			else if(fnRUStrCmp(_szAtt,"URI_SCHEME")==0) return E_RULE_ATT_URI_SCHEME;
			else if(fnRUStrCmp(_szAtt,"URI_STRING")==0) return E_RULE_ATT_URI_STRING;
			else if(fnRUStrCmp(_szAtt,"EXT_HDR_GEN")==0) return E_RULE_ATT_EXT_HDR_GEN;
			else if(fnRUStrCmp(_szAtt,"URI_SUB_VALUE")==0) return E_RULE_ATT_URI_SUB_VALUE;
			else if(fnRUStrCmp(_szAtt,"RULE_GROUP")==0) return E_RULE_ATT_RULE_GROUP;
			else if(fnRUStrCmp(_szAtt,"MATCH_LOC_ID")==0) return E_RULE_ATT_MATCH_LOC_ID;
			else if(fnRUStrCmp(_szAtt,"MATCH_RMT_ID")==0) return E_RULE_ATT_MATCH_RMT_ID;
			else if(fnRUStrCmp(_szAtt,"MATCH_RTE_ID")==0) return E_RULE_ATT_MATCH_RTE_ID;
			else if(fnRUStrCmp(_szAtt,"DIRECTION")==0) return E_RULE_ATT_DIRECTION;
			#ifdef DEF_RULE_R131
			else if(fnRUStrCmp(_szAtt,"NPDB_RESULT")==0) return E_RULE_ATT_NPDB_RESULT;
			else if(fnRUStrCmp(_szAtt,"HLR_RESULT")==0) return E_RULE_ATT_HLR_RESULT;
			#endif
			return E_RULE_ATT_NONE;		
	}
	private:
		static const char * s_arrEnum[E_RULE_ATT_MAX];
};
inline const char *fnStringRuleAttribute(ERuleAttribute_t _eAtt)
{
	return TblRuleAtt::fnGetString(_eAtt);
}
inline ERuleAttribute_t fnEnumRuleAttribute(const char * _szAtt)
{
	return TblRuleAtt::fnGetEnum(_szAtt);
}
inline const char *fnStringRuleCond(ERuleCond_t _eCond)
{
	switch(_eCond)
	{
		case E_RULE_COND_AND: return "AND";
		case E_RULE_COND_OR: return "OR";
		case E_RULE_COND_ON: return "ON";
		case E_RULE_COND_OFF: return "OFF";
		default: return "";
	};
	return "";
}
inline ERuleCond_t fnEnumRuleCond(const char * _szCond)
{
	if(fnRUStrCmp(_szCond,"AND")==0) return E_RULE_COND_AND;
	else if(fnRUStrCmp(_szCond,"OR")==0) return E_RULE_COND_OR;
	else if(fnRUStrCmp(_szCond,"ON")==0) return E_RULE_COND_ON;
	else if(fnRUStrCmp(_szCond,"OFF")==0) return E_RULE_COND_OFF;
	return E_RULE_COND_NONE;	
}
inline const char *fnStringRuleParam(ERuleParam_t _eType)
{
	switch(_eType)
	{
		case E_RULE_PARAM_NAME: return "NAME";
		case E_RULE_PARAM_ATTRIBUTE: return "ATTRIBUTE";
		case E_RULE_PARAM_POS: return "POS";
		case E_RULE_PARAM_MATCH: return "MATCH";
		case E_RULE_PARAM_EXIST: return "EXIST";
		case E_RULE_PARAM_SAVE: return "SAVE";
		case E_RULE_PARAM_VALUE1: return "VALUE1";
		case E_RULE_PARAM_VALUE2: return "VALUE2";
		case E_RULE_PARAM_VALUE3: return "VALUE3";
		default: return "";
	};
	return "";
}
inline ERuleParam_t fnEnumRuleParam(const char * _pszParam)
{
	if(fnRUStrCmp(_pszParam,"NAME")==0) return E_RULE_PARAM_NAME;
	else if(fnRUStrCmp(_pszParam,"ATTRIBUTE")==0) return E_RULE_PARAM_ATTRIBUTE;
	else if(fnRUStrCmp(_pszParam,"POS")==0) return E_RULE_PARAM_POS;
	else if(fnRUStrCmp(_pszParam,"MATCH")==0) return E_RULE_PARAM_MATCH;
	else if(fnRUStrCmp(_pszParam,"SAVE")==0) return E_RULE_PARAM_SAVE;
	else if(fnRUStrCmp(_pszParam,"VALUE1")==0) return E_RULE_PARAM_VALUE1;
	else if(fnRUStrCmp(_pszParam,"VALUE2")==0) return E_RULE_PARAM_VALUE2;
	else if(fnRUStrCmp(_pszParam,"VALUE3")==0) return E_RULE_PARAM_VALUE3;
	return E_RULE_PARAM_NONE;	
}
inline const char *fnStringRuleSave(ERuleSave_t _eType)
{
	switch(_eType)
	{
		case E_RULE_SAVE_NOT: return "NOT_SAVE";
		case E_RULE_SAVE_VALUE1: return "VALUE1";
		case E_RULE_SAVE_VALUE2: return "VALUE2";
		case E_RULE_SAVE_VALUE3: return "VALUE3";
		case E_RULE_SAVE_POS: return "POS";
		case E_RULE_SAVE_VALUE_INST: return "VALUE_INST";
		default: return "";
	};
	return "";
}
inline ERuleSave_t fnEnumRuleSave(const char * _pszParam)
{
	if(fnRUStrCmp(_pszParam,"NOT_SAVE")==0) return E_RULE_SAVE_NOT;
	else if(fnRUStrCmp(_pszParam,"VALUE1")==0) return E_RULE_SAVE_VALUE1;
	else if(fnRUStrCmp(_pszParam,"VALUE2")==0) return E_RULE_SAVE_VALUE2;
	else if(fnRUStrCmp(_pszParam,"VALUE3")==0) return E_RULE_SAVE_VALUE3;
	else if(fnRUStrCmp(_pszParam,"POS")==0) return E_RULE_SAVE_POS;
	else if(fnRUStrCmp(_pszParam,"VALUE_INST")==0) return E_RULE_SAVE_VALUE_INST;
	return E_RULE_SAVE_NONE;	
}
inline const char *fnStringRuleCreator(ERCreator_t _eType)
{
	switch(_eType)
	{
		case E_RULE_CREATOR_RULE: return "RULE";
		default: return "";
	};
	return "";
}
inline ERCreator_t fnEnumRuleCreator(const char * _pszParam)
{
	if(fnRUStrCmp(_pszParam,"rule")==0) return E_RULE_CREATOR_RULE;
	return E_RULE_CREATOR_NONE;	
}
/************************** Message Position Enum *****************************************/
inline const char *fnStringMsgPos(EMsgPos_t _ePos)
{
	switch(_ePos)
	{
		case E_MSG_POS_ALL: return "ALL";
		case E_MSG_POS_EXCEPT_FIRST: return "EXCEPT_FIRST";
		case E_MSG_POS_EXCEPT_LAST: return "EXCEPT_LAST";
		case E_MSG_POS_ONLY_FIRST: return "ONLY_FIRST";
		case E_MSG_POS_ONLY_LAST: return "ONLY_LAST";
		case E_MSG_POS_DEPEND_RULE: return "DEPEND_RULE";
		case E_MSG_POS_SHIFT_UP: return "SHIFT_UP";
		case E_MSG_POS_SHIFT_DOWN: return "SHIFT_DOWN";
		//case E_MSG_POS_ORDER: return "ORDER";
		case E_MSG_POS_ORDER: return "DEPEND_RULE";
		default: return "Unknown";
	};
	return "Unknown";
}
inline EMsgPos_t fnEnumMsgPos(const char * _szPos)
{
	if(fnRUStrCmp(_szPos,"ALL")==0) return E_MSG_POS_ALL;
	else if(fnRUStrCmp(_szPos,"EXCEPT_FIRST")==0) return E_MSG_POS_EXCEPT_FIRST;
	else if(fnRUStrCmp(_szPos,"EXCEPT_LAST")==0) return E_MSG_POS_EXCEPT_LAST;
	else if(fnRUStrCmp(_szPos,"ONLY_FIRST")==0) return E_MSG_POS_ONLY_FIRST;
	else if(fnRUStrCmp(_szPos,"ONLY_LAST")==0) return E_MSG_POS_ONLY_LAST;
	else if(fnRUStrCmp(_szPos,"DEPEND_RULE")==0) return E_MSG_POS_DEPEND_RULE;
	else if(fnRUStrCmp(_szPos,"SHIFT_UP")==0) return E_MSG_POS_SHIFT_DOWN;   // cwkim_rule_bug
	else if(fnRUStrCmp(_szPos,"SHIFT_DOWN")==0) return E_MSG_POS_SHIFT_DOWN;
	//else if(fnRUStrCmp(_szPos,"ORDER")==0) return E_MSG_POS_ORDER;
	else if(fnRUStrCmp(_szPos,"ORDER")==0) return E_MSG_POS_DEPEND_RULE;
	return E_MSG_POS_NONE;	
}
class Rule;
class RuleSet;
class RuleMgr;

class OPListObj  : public RUObject
{
	public:
		OPListObj(){m_uiOPID=0;}
		~OPListObj(){}
		OPListObj & operator=( OPListObj & _rclsOP)
		{
			m_uiOPID = _rclsOP.m_uiOPID;
			return *this;
		}
		operator RUINT () {return m_uiOPID;}
		unsigned int m_uiOPID;
};
#define DEF_RULE_COMMENT "// ||rule||ID||NAME||ATTRIBUTE||POS||MATCH||EXIST||VALUE1||VALUE2||VALUE3"
typedef enum
{
	E_RULE_USING_NONE,
	E_RULE_USING_SCR_OP,
	E_RULE_USING_RTE_OP,
	E_RULE_USING_FILTER_OP,
}EUsingRule_t;
inline const char * fnStringUsingRule(EUsingRule_t _eT)
{
	switch(_eT)
	{
		case E_RULE_USING_NONE: return "NONE";
		case E_RULE_USING_SCR_OP: return "SCR_OP";
		case E_RULE_USING_RTE_OP: return "RTE_OP";
		case E_RULE_USING_FILTER_OP: return "FILTER_OP";
		default: return "NONE";
	};
	return "NONE";
}
class UsingRule : public RUObject
{
	public:
		UsingRule(){fnClear();}
		~UsingRule(){}
		void fnClear()
		{
			m_eUsing = E_RULE_USING_NONE;
			m_unID = 0;
		}
		char * fnDebug(char * _pszLog, unsigned int _unSize)
		{
			fnRUStrnCat(_pszLog,_unSize,"[%s,%u] ",fnStringUsingRule(m_eUsing),m_unID);
			return _pszLog;
		}
		EUsingRule_t m_eUsing;
		unsigned int m_unID;
};
class Rule : public RUObject
{
	public:
		Rule();
		~Rule();
		Rule &operator=(Rule & _clsRule);
		void fnLoadCfg(RuleFileParam *pclsParam);
		void fnDebug();
		void fnDisplay(char * _pszInfo,unsigned int _unSz);
		void fnTrace(char * _pszTrace, unsigned int _unSz);
		void fnBuildConfig(char * pszBuild);
		void fnSetID(int _nID);
		void fnSetName(const char * _pszVal);
		void fnSetAttribute(ERuleAttribute_t _eAtt);
		void fnSetPos(EMsgPos_t _ePos);
		void fnSetMatch(ERuleMatch_t _eMatch);
		void fnSetExist(int _bExist);
		void fnSetVal1(const char * _pszVal);
		void fnSetVal2(const char * _pszVal);
		void fnSetVal3(const char * _pszVal);
		void fnAddUsingHistory(EUsingRule_t _eUsing, unsigned int _unID)
		{
			UsingRule * pclsNew = new UsingRule;
			RUString clsStr(1024); clsStr<<fnStringUsingRule(_eUsing)<<RUIntString(_unID);
			pclsNew->fnSetKeyS((RUSTR)clsStr); pclsNew->m_unID = _unID; pclsNew->m_eUsing = _eUsing;
			m_listUsing.fnPushBack(pclsNew);
		}
		void fnDelUsingHistory(EUsingRule_t _eUsing, unsigned int _unID)
		{
			RUString clsStr(1024); clsStr<<fnStringUsingRule(_eUsing)<<RUIntString(_unID);
			m_listUsing.fnDelNode((RUSTR)clsStr);
		}
		unsigned int fnUsingRuleNum(){return m_listUsing.fnGetSize();}
		void fnUsingInfo(char * _pszLog, unsigned int _unSize)
		{
			fnRUStrnCat(_pszLog,_unSize,"<<<Rule Related Info>>> ");
			RuListItem_t clsIter;
			UsingRule * pclsTmp = (UsingRule*)m_listUsing.fnBegin(clsIter);
			while(pclsTmp)
			{
				pclsTmp->fnDebug(_pszLog,_unSize);
				pclsTmp = (UsingRule*)m_listUsing.fnNext(clsIter);
			}
		}
		unsigned int m_Id;
		char *m_szName;
		ERuleAttribute_t m_eType;
		EMsgPos_t m_ePos;
		ERuleMatch_t m_eMatch;
		bool m_bExist;
		char *m_szVal1;
		char *m_szVal2;
		char *m_szVal3;
		RuleFileParam * m_pclsRawData;
		RUList m_listUsing;
};
class RuleSetInfo : public RUObject
{
	public:
		RuleSetInfo();
		~RuleSetInfo();
		RuleSetInfo & operator=(RuleSetInfo & _stRuleSet);
		char * fnBuild(char * _pszBuild);
		bool fnIsValid();
		void fnDebug();
		void fnDisplay(char * _pszLog,unsigned int _unSize);
		void fnTrace(char * _pszTrace, unsigned int _unSize);
		unsigned int m_RuleId;
		ERuleCond_t m_eCond;
};

#define DEF_RULESET_COMMENT "//(ON/OFF) RULE_01 OR RULE_02 AND RULE_03 AND RULE_04"

class RuleSet
{
	public:
		RuleSet();
		~RuleSet();
		RuleSet &operator=(RuleSet & _clsRuleSet);
		RuleSetInfo & operator[](unsigned int _unIndex);
		void fnLoadCfg(const char * _pszRuleSet);
		bool fnIsValid();
		char * fnBuild(char * _pszBuild);
		void fnDebug();
		void fnDisplay(char * _pszLog,unsigned int _unSize);
		void fnTrace(char * _pszTrace, unsigned int _unSize);
		void fnAction(PfuncActionList_t _pfnAction, void * _pvUser);
		RuleSetInfo * fnBegin(){return (RuleSetInfo*)m_listRuleSet.fnBegin();}
		RuleSetInfo *fnNext(){return (RuleSetInfo *)m_listRuleSet.fnNext();}
		RuleSetInfo * fnBegin(RuListItem_t &_rclsItem ){return (RuleSetInfo*)m_listRuleSet.fnBegin(_rclsItem);}
		RuleSetInfo *fnNext(RuListItem_t &_rclsItem ){return (RuleSetInfo *)m_listRuleSet.fnNext(_rclsItem);}
		unsigned int fnGetSize(){return m_listRuleSet.fnGetSize();}
		bool fnLoadSet(unsigned int _uiKey,ERuleCond_t _eCond);
		void fnAddUsingHistory(EUsingRule_t _eUsing, unsigned int _unID);
		void fnDelUsingHistory(EUsingRule_t _eUsing, unsigned int _unID);
		bool fnIsAndRuleSet();
		RUList m_listRuleSet;
		ERuleCond_t m_eCurrentCond;
		bool m_bExist;
		RuleSetInfo m_clsExcept;
	private:
		unsigned int m_unCnt;
		static void fnCbkUserCopy(unsigned long _ulData, void * _pvUser);
};
class RuleMgr : public RuleFile
{
	public:
		RuleMgr();
		~RuleMgr();
		static RuleMgr * fnGetInstance();
		static void fnFinalize();
		bool fnLoadRuleFile(const char * _pszPath);
		bool fnWriteRuleFile(const char * _pszPath);
		bool fnLoadRuleMem(const char * _pszSrc);
		bool fnWriteRuleMem(char * _pszDst, unsigned int _unSize);
		void fnMgrAction();
		bool fnIsValidRuleSet(LmRuleSet_t & _rclsRuleSet);
		Rule * fnFindRule(unsigned int _uiKey);
		// ||rule||ID||NAME||ATTRIBUTE||POS||MATCH||EXIST||VALUE1||VALUE2||VALUE3
		bool fnAddRule(unsigned int _uiKey, const char * _pszName,const char * _pszString);
		bool fnAddRule(LmRule_t * _pstLmRule,LmRuleResult_t &_rstResult);
		bool fnGetRule(unsigned int _uiKey, LmRule_t * _pstLmRule);
		bool fnModiRule(LmRule_t * _pstLmRule,LmRuleResult_t &_rstResult);
		bool fnDelRule(unsigned int _uiKey);
		bool fnDelRule(LmRule_t * _pstLmRule,LmRuleResult_t &_rstResult);
		void fnDebugRule();
		Rule * fnBegin(){return (Rule *)m_mapRule.fnBeginI();}
		Rule * fnNext(){return (Rule *)m_mapRule.fnNextI();}
		Rule * fnBegin(RuMapIntItem_t &_rclsItem){return (Rule *)m_mapRule.fnBeginI(_rclsItem);}
		Rule * fnNext(RuMapIntItem_t &_rclsItem){return (Rule *)m_mapRule.fnNextI(_rclsItem);}
		static RuleMgr * g_pclsRuleMgr;
		RUMap m_mapRule;
	private:
		void fnInitMgr();
		void fnArrangeLine();
		void fnSortRule();
		static void * fnCbkCreator(RuleFileParam * _pclsParam, unsigned int * _puiRet);
		static void fnCbkTerminator(void * _pvUser, unsigned int _uiType);
		static void fnCbkAction(void * _pvUser,unsigned int _uiType);
		static bool fnCbkSortRule(unsigned long _pvUser1, unsigned long _pvUser2);
};
#endif

