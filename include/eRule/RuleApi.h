#ifndef RULEAPI_H
#define RULEAPI_H
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
   #include <WINSOCK2.H>
#else 
   #include <netinet/in.h>
#endif

#include "RuleFeature.h"
/******************************* Enum *************************************************/
#define DEF_LM_RULE_STRING_SZ       128
#define DEF_LM_PARAM_MAX_SZ       100 //40   --> 100
#define DEF_LM_EMPTY_STRING     "-1"
typedef enum
{
	E_MSG_POS_NONE = 0,
	E_MSG_POS_ALL,
	E_MSG_POS_EXCEPT_FIRST,
	E_MSG_POS_EXCEPT_LAST,
	E_MSG_POS_ONLY_FIRST,
	E_MSG_POS_ONLY_LAST,
	E_MSG_POS_DEPEND_RULE,
	E_MSG_POS_SHIFT_UP,
	E_MSG_POS_SHIFT_DOWN,
	E_MSG_POS_ORDER,
}EMsgPos_t;
inline const char * fnRuleApiStringMsgPos(EMsgPos_t _eT)
{
	switch(_eT)
	{
		case E_MSG_POS_NONE: return              "E_MSG_POS_NONE";         
		case E_MSG_POS_ALL: return               "E_MSG_POS_ALL";          
		case E_MSG_POS_EXCEPT_FIRST: return      "E_MSG_POS_EXCEPT_FIRST"; 
		case E_MSG_POS_EXCEPT_LAST: return       "E_MSG_POS_EXCEPT_LAST";  
		case E_MSG_POS_ONLY_FIRST: return        "E_MSG_POS_ONLY_FIRST";   
		case E_MSG_POS_ONLY_LAST: return         "E_MSG_POS_ONLY_LAST";    
		case E_MSG_POS_DEPEND_RULE: return       "E_MSG_POS_DEPEND_RULE";  
		case E_MSG_POS_SHIFT_UP: return          "E_MSG_POS_SHIFT_UP";     
		case E_MSG_POS_SHIFT_DOWN: return        "E_MSG_POS_SHIFT_DOWN";   
		case E_MSG_POS_ORDER: return             "E_MSG_POS_ORDER"; 
		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_RULE_NONE = 0,
	E_RULE_RULE,
	E_RULE_RULESET
}ERuleType_t;
inline const char * fnRuleApiStringRule(ERuleType_t _eT)
{
	switch(_eT)
	{
		case E_RULE_NONE: return              "E_RULE_NONE";         
		case E_RULE_RULE: return               "E_RULE_RULE";          
		case E_RULE_RULESET: return      "E_RULE_RULESET"; 
		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_RULE_MATCH_NONE = 0,
	E_RULE_MATCH_FULL,
	E_RULE_MATCH_PART,
	E_RULE_MATCH_NUM,
	E_RULE_MATCH_SEQ
}ERuleMatch_t;
inline const char * fnRuleApiStringRuleMatch(ERuleMatch_t _eT)
{
	switch(_eT)
	{
		case E_RULE_MATCH_NONE: return              "E_RULE_MATCH_NONE";         
		case E_RULE_MATCH_FULL: return               "E_RULE_MATCH_FULL";          
		case E_RULE_MATCH_PART: return      "E_RULE_MATCH_PART"; 
		case E_RULE_MATCH_NUM: return       "E_RULE_MATCH_NUM";  
		case E_RULE_MATCH_SEQ: return       "E_RULE_MATCH_SEQ";  
		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_RULE_ATT_NONE = 0,
	E_RULE_ATT_CALLRING_DOMAIN,
	E_RULE_ATT_CALLER_DOMAIN,
	E_RULE_ATT_CALLED_DOMAIN,
	E_RULE_ATT_CALLER_PREFIX,
	E_RULE_ATT_CALLED_PREFIX,
	E_RULE_ATT_CALLER_REALM,
	E_RULE_ATT_CALLED_REALM,
	E_RULE_ATT_HEADER_NAME,
	E_RULE_ATT_HEADER_VALUE,
	E_RULE_ATT_RECV_LOC,
	E_RULE_ATT_REQUEST,
	E_RULE_ATT_RESPONSE_MC,
	E_RULE_ATT_HEADER_GENVALUE,
	E_RULE_ATT_DISPLAY_NAME,
	E_RULE_ATT_URI_GEN_VALUE,
	E_RULE_ATT_URI_GEN_HEADER,
	E_RULE_ATT_URI_USERNAME,
	E_RULE_ATT_HOST,
	E_RULE_ATT_URI_REALM,
	E_RULE_ATT_URI_PORT,
	E_RULE_ATT_URI_SCHEME,
	E_RULE_ATT_URI_STRING,
	E_RULE_ATT_EXT_HDR_GEN,
	E_RULE_ATT_URI_SUB_VALUE,
	E_RULE_ATT_RULE_GROUP,
	E_RULE_ATT_MATCH_LOC_ID,
	E_RULE_ATT_MATCH_RMT_ID,
	E_RULE_ATT_MATCH_RTE_ID,
	E_RULE_ATT_DIRECTION,
	#ifdef DEF_RULE_R131
	E_RULE_ATT_NPDB_RESULT,
	E_RULE_ATT_HLR_RESULT,
	E_RULE_ATT_POINT_CODE,
	#endif
	E_RULE_ATT_MAX
}ERuleAttribute_t;
inline const char * fnRuleApiStringRuleAttribute(ERuleAttribute_t _eT)
{
	switch(_eT)
	{
		case E_RULE_ATT_NONE:              return "E_RULE_ATT_NONE";               
		case E_RULE_ATT_CALLRING_DOMAIN:   return "E_RULE_ATT_CALLRING_DOMAIN";
		case E_RULE_ATT_CALLER_DOMAIN:     return "E_RULE_ATT_CALLER_DOMAIN";  
		case E_RULE_ATT_CALLED_DOMAIN:     return "E_RULE_ATT_CALLED_DOMAIN";  
		case E_RULE_ATT_CALLER_PREFIX:     return "E_RULE_ATT_CALLER_PREFIX";  
		case E_RULE_ATT_CALLED_PREFIX:     return "E_RULE_ATT_CALLED_PREFIX";  
		case E_RULE_ATT_CALLER_REALM:      return "E_RULE_ATT_CALLER_REALM";   
		case E_RULE_ATT_CALLED_REALM:      return "E_RULE_ATT_CALLED_REALM";   
		case E_RULE_ATT_HEADER_NAME:       return "E_RULE_ATT_HEADER_NAME";    
		case E_RULE_ATT_HEADER_VALUE:      return "E_RULE_ATT_HEADER_VALUE";   
		case E_RULE_ATT_RECV_LOC:          return "E_RULE_ATT_RECV_LOC";       
		case E_RULE_ATT_REQUEST:           return "E_RULE_ATT_REQUEST";        
		case E_RULE_ATT_RESPONSE_MC:       return "E_RULE_ATT_RESPONSE_MC";    
		case E_RULE_ATT_HEADER_GENVALUE:   return "E_RULE_ATT_HEADER_GENVALUE";
		case E_RULE_ATT_DISPLAY_NAME:      return "E_RULE_ATT_DISPLAY_NAME";   
		case E_RULE_ATT_URI_GEN_VALUE:     return "E_RULE_ATT_URI_GEN_VALUE";  
		case E_RULE_ATT_URI_GEN_HEADER:    return "E_RULE_ATT_URI_GEN_HEADER"; 
		case E_RULE_ATT_URI_USERNAME:      return "E_RULE_ATT_URI_USERNAME";   
		case E_RULE_ATT_HOST:              return "E_RULE_ATT_HOST";           
		case E_RULE_ATT_URI_REALM:         return "E_RULE_ATT_URI_REALM";      
		case E_RULE_ATT_URI_PORT:          return "E_RULE_ATT_URI_PORT";       
		case E_RULE_ATT_URI_SCHEME:        return "E_RULE_ATT_URI_SCHEME";     
		case E_RULE_ATT_URI_STRING:        return "E_RULE_ATT_URI_STRING";     
		case E_RULE_ATT_EXT_HDR_GEN:       return "E_RULE_ATT_EXT_HDR_GEN";    
		case E_RULE_ATT_URI_SUB_VALUE:     return "E_RULE_ATT_URI_SUB_VALUE";  
		case E_RULE_ATT_RULE_GROUP:         return "E_RULE_ATT_RULE_GROUP";    
		case E_RULE_ATT_MATCH_LOC_ID:         return "E_RULE_ATT_MATCH_LOC_ID";    
		case E_RULE_ATT_MATCH_RMT_ID:         return "E_RULE_ATT_MATCH_RMT_ID";    
		case E_RULE_ATT_MATCH_RTE_ID:         return "E_RULE_ATT_MATCH_RTE_ID";    
		case E_RULE_ATT_DIRECTION:         return "E_RULE_ATT_DIRECTION";    
		#ifdef DEF_RULE_R131
		case E_RULE_ATT_NPDB_RESULT:         return "E_RULE_ATT_NPDB_RESULT";
		case E_RULE_ATT_HLR_RESULT:         return "E_RULE_ATT_HLR_RESULT";
		case E_RULE_ATT_POINT_CODE:        return "E_RULE_ATT_POINT_CODE";
		#endif
		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_RULE_COND_NONE = 0,
	E_RULE_COND_ON,
	E_RULE_COND_OFF,
	E_RULE_COND_AND,
	E_RULE_COND_OR,
}ERuleCond_t;
inline const char * fnRuleApiStringRuleCond(ERuleCond_t _eT)
{
	switch(_eT)
	{
		case E_RULE_COND_NONE:              return "E_RULE_COND_NONE";               
		case E_RULE_COND_ON:   return "E_RULE_COND_ON";
		case E_RULE_COND_OFF:     return "E_RULE_COND_OFF";  
		case E_RULE_COND_AND:     return "E_RULE_COND_AND";  
		case E_RULE_COND_OR:     return "E_RULE_COND_OR";  
     		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_RULE_PARAM_NONE = 0,
	E_RULE_PARAM_NAME,
	E_RULE_PARAM_ATTRIBUTE,
	E_RULE_PARAM_POS,
	E_RULE_PARAM_MATCH,
	E_RULE_PARAM_EXIST,
	E_RULE_PARAM_SAVE,
	E_RULE_PARAM_VALUE1,
	E_RULE_PARAM_VALUE2,
	E_RULE_PARAM_VALUE3	
}ERuleParam_t;
inline const char * fnRuleApiStringRuleParam(ERuleParam_t _eT)
{
	switch(_eT)
	{
		case E_RULE_PARAM_NONE:        return "E_RULE_PARAM_NONE";         
		case E_RULE_PARAM_NAME:        return "E_RULE_PARAM_NAME";     
		case E_RULE_PARAM_ATTRIBUTE:   return "E_RULE_PARAM_ATTRIBUTE";
		case E_RULE_PARAM_POS:         return "E_RULE_PARAM_POS";      
		case E_RULE_PARAM_MATCH:       return "E_RULE_PARAM_MATCH";    
		case E_RULE_PARAM_EXIST:       return "E_RULE_PARAM_EXIST";    
		case E_RULE_PARAM_SAVE:        return "E_RULE_PARAM_SAVE";     
		case E_RULE_PARAM_VALUE1:      return "E_RULE_PARAM_VALUE1";   
		case E_RULE_PARAM_VALUE2:      return "E_RULE_PARAM_VALUE2";   
		case E_RULE_PARAM_VALUE3:       return "E_RULE_PARAM_VALUE3";	
     		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_RULE_SAVE_NONE= 0,
	E_RULE_SAVE_NOT,
	E_RULE_SAVE_VALUE1,
	E_RULE_SAVE_VALUE2,
	E_RULE_SAVE_VALUE3,
	E_RULE_SAVE_POS,
	E_RULE_SAVE_VALUE_INST		
}ERuleSave_t;
inline const char * fnRuleApiStringRuleSave(ERuleSave_t _eT)
{
	switch(_eT)
	{
		case E_RULE_SAVE_NONE:       	return "E_RULE_SAVE_NONE";       
		case E_RULE_SAVE_NOT:        	return "E_RULE_SAVE_NOT";   
		case E_RULE_SAVE_VALUE1:     	return "E_RULE_SAVE_VALUE1";
		case E_RULE_SAVE_VALUE2:     	return "E_RULE_SAVE_VALUE2";
		case E_RULE_SAVE_VALUE3:     	return "E_RULE_SAVE_VALUE3";
		case E_RULE_SAVE_POS:        	return "E_RULE_SAVE_POS";   
		case E_RULE_SAVE_VALUE_INST:  	return "E_RULE_SAVE_VALUE_INST";	
     		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_SCREEN_ACTION_NONE=0,
	E_SCREEN_ACTION_ADD_HDR_VALUE,
	E_SCREEN_ACTION_ADD_HDR_PARAM,
	E_SCREEN_ACTION_DEL_HDR_NAME,
	E_SCREEN_ACTION_DEL_HDR_PARAM,
	E_SCREEN_ACTION_CVT_REQ_URI,
	E_SCREEN_ACTION_ADD_URI_GEN_PARAM,
	E_SCREEN_ACTION_ADD_URI_HDR_PARAM,
	E_SCREEN_ACTION_ADD_URI_SCHEME,
	E_SCREEN_ACTION_ADD_URI_USERNAME,
	E_SCREEN_ACTION_ADD_URI_HOST,
	E_SCREEN_ACTION_ADD_URI_PORT,
	E_SCREEN_ACTION_ADD_URI_DISPLAYNAME,
	E_SCREEN_ACTION_ADD_URI_STRING,
	E_SCREEN_ACTION_DEL_URI_DISPLAYNAME,
	E_SCREEN_ACTION_DEL_URI_GEN_PARAM,
	E_SCREEN_ACTION_DEL_URI_HDR_PARAM,
	E_SCREEN_ACTION_DEL_URI_SCHEME,
	E_SCREEN_ACTION_DEL_URI_USERNAME,
	E_SCREEN_ACTION_DEL_URI_HOST,
	E_SCREEN_ACTION_DEL_URI_PORT,
	E_SCREEN_ACTION_DEL_URI_STRING,
	E_SCREEN_ACTION_MOV_PARAM,
	E_SCREEN_ACTION_MOV_URI_PARAM,
	E_SCREEN_ACTION_MOV_URI_HDR,
	E_SCREEN_ACTION_CVT_RSP,
	E_SCREEN_ACTION_CVT_URI_SCHEME,
}EScreenAct_t;
inline const char * fnRuleApiStringScreenAct(EScreenAct_t _eT)
{
	switch(_eT)
	{
		case E_SCREEN_ACTION_NONE:                 return "E_SCREEN_ACTION_NONE=0";                     
		case E_SCREEN_ACTION_ADD_HDR_VALUE:        return "E_SCREEN_ACTION_ADD_HDR_VALUE";      
		case E_SCREEN_ACTION_ADD_HDR_PARAM:        return "E_SCREEN_ACTION_ADD_HDR_PARAM";      
		case E_SCREEN_ACTION_DEL_HDR_NAME:         return "E_SCREEN_ACTION_DEL_HDR_NAME";       
		case E_SCREEN_ACTION_DEL_HDR_PARAM:        return "E_SCREEN_ACTION_DEL_HDR_PARAM";      
		case E_SCREEN_ACTION_CVT_REQ_URI:          return "E_SCREEN_ACTION_CVT_REQ_URI";        
		case E_SCREEN_ACTION_ADD_URI_GEN_PARAM:    return "E_SCREEN_ACTION_ADD_URI_GEN_PARAM";  
		case E_SCREEN_ACTION_ADD_URI_HDR_PARAM:    return "E_SCREEN_ACTION_ADD_URI_HDR_PARAM";  
		case E_SCREEN_ACTION_ADD_URI_SCHEME:       return "E_SCREEN_ACTION_ADD_URI_SCHEME";     
		case E_SCREEN_ACTION_ADD_URI_USERNAME:     return "E_SCREEN_ACTION_ADD_URI_USERNAME";   
		case E_SCREEN_ACTION_ADD_URI_HOST:         return "E_SCREEN_ACTION_ADD_URI_HOST";       
		case E_SCREEN_ACTION_ADD_URI_PORT:         return "E_SCREEN_ACTION_ADD_URI_PORT";       
		case E_SCREEN_ACTION_ADD_URI_DISPLAYNAME:  return "E_SCREEN_ACTION_ADD_URI_DISPLAYNAME";
		case E_SCREEN_ACTION_ADD_URI_STRING:       return "E_SCREEN_ACTION_ADD_URI_STRING";     
		case E_SCREEN_ACTION_DEL_URI_DISPLAYNAME:  return "E_SCREEN_ACTION_DEL_URI_DISPLAYNAME";
		case E_SCREEN_ACTION_DEL_URI_GEN_PARAM:    return "E_SCREEN_ACTION_DEL_URI_GEN_PARAM";  
		case E_SCREEN_ACTION_DEL_URI_HDR_PARAM:    return "E_SCREEN_ACTION_DEL_URI_HDR_PARAM";  
		case E_SCREEN_ACTION_DEL_URI_SCHEME:       return "E_SCREEN_ACTION_DEL_URI_SCHEME";     
		case E_SCREEN_ACTION_DEL_URI_USERNAME:     return "E_SCREEN_ACTION_DEL_URI_USERNAME";   
		case E_SCREEN_ACTION_DEL_URI_HOST:         return "E_SCREEN_ACTION_DEL_URI_HOST";       
		case E_SCREEN_ACTION_DEL_URI_PORT:         return "E_SCREEN_ACTION_DEL_URI_PORT";       
		case E_SCREEN_ACTION_DEL_URI_STRING:       return "E_SCREEN_ACTION_DEL_URI_STRING";   
		case E_SCREEN_ACTION_MOV_PARAM:        return "E_SCREEN_ACTION_MOV_PARAM";      
		case E_SCREEN_ACTION_MOV_URI_PARAM:        return "E_SCREEN_ACTION_MOV_URI_PARAM";      
		case E_SCREEN_ACTION_MOV_URI_HDR:          return "E_SCREEN_ACTION_MOV_URI_HDR";  
		case E_SCREEN_ACTION_CVT_RSP: return "E_SCREEN_ACTION_CVT_RSP";
		case E_SCREEN_ACTION_CVT_URI_SCHEME: return "E_SCREEN_ACTION_CVT_URI_SCHEME";
     		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_ROUTE_OP_NONE = 0,
	E_ROUTE_OP_RSEQ,
	E_ROUTE_OP_SSEQ
}ERouteOp_t;
inline const char * fnRuleApiStringRouteOp(ERouteOp_t _eT)
{
	switch(_eT)
	{
		case E_ROUTE_OP_NONE:                 return "E_ROUTE_OP_NONE=0";                     
		case E_ROUTE_OP_RSEQ:        return "E_ROUTE_OP_RSEQ";      
		case E_ROUTE_OP_SSEQ:        return "E_ROUTE_OP_SSEQ";      
     		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_SERVICE_OPS_NONE = 0,
	E_SERVICE_OPS_SRC_SCREEN,
	E_SERVICE_OPS_DST_SCREEN,
	E_SERVICE_OPS_FILTER,
	E_SERVICE_OPS_MAX
}EServiceOPS_t;
inline const char * fnRuleApiStringServiceOPS(EServiceOPS_t _eT)
{
	switch(_eT)
	{
		case E_SERVICE_OPS_NONE:                 return "E_SERVICE_OPS_NONE";                     
		case E_SERVICE_OPS_SRC_SCREEN:        return "E_SERVICE_OPS_SRC_SCREEN";  
		case E_SERVICE_OPS_DST_SCREEN:        return "E_SERVICE_OPS_DST_SCREEN";  
		case E_SERVICE_OPS_FILTER:        return "E_SERVICE_OPS_FILTER";  
		case E_SERVICE_OPS_MAX:        return "E_SERVICE_OPS_MAX";      
     		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_RULE_CATEGORY_NONE,
	E_RULE_CATEGORY_RULE,
	E_RULE_CATEGORY_SCR_OP,
	E_RULE_CATEGORY_SCR_OPS,
	E_RULE_CATEGORY_SCR_ACT,
	E_RULE_CATEGORY_RTE_OP,
	E_RULE_CATEGORY_FILTER_OP,
	E_RULE_CATEGORY_FILTER_OPS,
	E_RULE_CATEGORY_FILTER_ACT,
	E_RULE_CATEGORY_MAX
}ERuleApiCategory_t;
inline const char * fnRuleApiStringCategory(ERuleApiCategory_t _eT)
{
	switch(_eT)
	{
		case E_RULE_CATEGORY_NONE:         return "E_RULE_CATEGORY_NONE";                   
		case E_RULE_CATEGORY_RULE:         return "E_RULE_CATEGORY_RULE";      
		case E_RULE_CATEGORY_SCR_OP:       return "E_RULE_CATEGORY_SCR_OP";    
		case E_RULE_CATEGORY_SCR_OPS:      return "E_RULE_CATEGORY_SCR_OPS";   
		case E_RULE_CATEGORY_SCR_ACT:      return "E_RULE_CATEGORY_SCR_ACT";   
		case E_RULE_CATEGORY_RTE_OP:       return "E_RULE_CATEGORY_RTE_OP";    
		case E_RULE_CATEGORY_FILTER_OP:    return "E_RULE_CATEGORY_FILTER_OP"; 
		case E_RULE_CATEGORY_FILTER_OPS:    return "E_RULE_CATEGORY_FILTER_OPS"; 
		case E_RULE_CATEGORY_FILTER_ACT:   return "E_RULE_CATEGORY_FILTER_ACT";
		case E_RULE_CATEGORY_MAX:          return "E_RULE_CATEGORY_MAX";
		default: return "NONE";   
	};
	return "NONE";
}
typedef enum
{
	E_FILTER_ACT_NONE = 0,
	E_FILTER_ACT_DENY,
	E_FILTER_ACT_DISCARD
}EFilterAttribute_t;
inline const char * fnRuleApiStringFilterAttribute(EFilterAttribute_t _eT)
{
	switch(_eT)
	{
		case E_FILTER_ACT_NONE:         return "E_FILTER_ACT_NONE";                   
		case E_FILTER_ACT_DENY:         return "E_FILTER_ACT_DENY";      
		case E_FILTER_ACT_DISCARD:       return "E_FILTER_ACT_DISCARD"; 
	};
	return "NONE";
}
inline void fnRuleApiStrnCpy(char * _pszDst, const char * _pszSrc)
{
	if(_pszDst == NULL || _pszSrc == NULL) return;
	strncpy(_pszDst,_pszSrc,DEF_LM_RULE_STRING_SZ);
}
inline int fnRuleApiStrnCat(char *_szSrc,unsigned int _unMaxSz,const char * _szVar,...)
{
	if(_szSrc==NULL) return -1;
	unsigned int unLen = strlen(_szSrc);
	if(unLen >= _unMaxSz)
	{
		_szSrc[_unMaxSz-3]='\r'; _szSrc[_unMaxSz-2]='\n'; _szSrc[_unMaxSz-1]=0;return _unMaxSz;
	}
	char *pszTemp = &_szSrc[unLen];
	va_list ap;va_start(ap, _szVar);vsnprintf(pszTemp, _unMaxSz-unLen-1,_szVar, ap);va_end(ap);
	return 0;
}
/**************************** Rule (RULE) ***********************************************/
typedef struct LmRule_t
{
	LmRule_t(){m_fnClear();}
	void m_fnClear()
	{
		m_nID = -1;
		m_eAtt = E_RULE_ATT_NONE;
		m_ePos = E_MSG_POS_NONE;
		m_eMatch = E_RULE_MATCH_NONE;
		m_eSave = E_RULE_SAVE_NONE;
		m_bExist = -1;
		memset(m_szName,0x00,DEF_LM_RULE_STRING_SZ);
		memset(m_szVal1,0x00,DEF_LM_RULE_STRING_SZ);
		memset(m_szVal2,0x00,DEF_LM_RULE_STRING_SZ);
		memset(m_szVal3,0x00,DEF_LM_RULE_STRING_SZ);
		memcpy(m_szName,DEF_LM_EMPTY_STRING,2);
		memcpy(m_szVal1,DEF_LM_EMPTY_STRING,2);
		memcpy(m_szVal2,DEF_LM_EMPTY_STRING,2);
		memcpy(m_szVal3,DEF_LM_EMPTY_STRING,2);
	}
	void m_fnHtoN()
	{
		m_nID=htonl(m_nID);
		m_eAtt=htonl(m_eAtt);
		m_ePos=htonl(m_ePos);
		m_eMatch=htonl(m_eMatch);
		m_eSave=htonl(m_eSave);
		m_bExist=htonl(m_bExist);
	}
	void m_fnNtoH()
	{
		m_nID=ntohl(m_nID);
		m_eAtt=ntohl(m_eAtt);
		m_ePos=ntohl(m_ePos);
		m_eMatch=ntohl(m_eMatch);
		m_eSave=ntohl(m_eSave);
		m_bExist=ntohl(m_bExist);
	}
	void m_fnSetID(int _nID){ m_nID = _nID;}
	void m_fnSetAttribute(ERuleAttribute_t _eAtt){m_eAtt = _eAtt;}
	void m_fnSetPos(EMsgPos_t _ePos){m_ePos = _ePos;}
	void m_fnSetMatch(ERuleMatch_t _eMatch){m_eMatch = _eMatch;}
	void m_fnSetSave(ERuleSave_t _eSave){m_eSave = _eSave;}
	void m_fnSetExist(bool _bExist){if(_bExist) m_bExist=1;else m_bExist = 0;}
	void m_fnSetName(const char * _pszVal){fnRuleApiStrnCpy(m_szName,_pszVal);}
	void m_fnSetVal1(const char * _pszVal){fnRuleApiStrnCpy(m_szVal1,_pszVal);}
	void m_fnSetVal2(const char * _pszVal){fnRuleApiStrnCpy(m_szVal2,_pszVal);}
	void m_fnSetVal3(const char * _pszVal){fnRuleApiStrnCpy(m_szVal3,_pszVal);}
	int m_nID;
	int m_eAtt;       //ERuleAttribute_t
	int m_ePos;      // EMsgPos_t
	int m_eMatch;   // ERuleMatch_t
	int m_eSave;     //ERuleSave_t
	int m_bExist;
	char m_szName[DEF_LM_RULE_STRING_SZ];
	char m_szVal1[DEF_LM_RULE_STRING_SZ];
	char m_szVal2[DEF_LM_RULE_STRING_SZ];
	char m_szVal3[DEF_LM_RULE_STRING_SZ];
	bool m_fnIsAddCheck()
	{
		if(m_eAtt <= 0) return false;
		if(m_ePos <= 0) return false;
		if(m_eMatch <= 0) return false;
		//if(m_eSave == E_RULE_SAVE_NONE) return false;
		return true;
	}
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		fnRuleApiStrnCat(_pszBuff,_unSize,
"<LmRule_t: ID=%d, Name=%s, Attribute=%s, Pos=%s, Match=%s, Save=%s, bExit=%d, Val1=%s, Val2=%s, Val3=%s>\r\n", 
				m_nID, m_szName, 
				fnRuleApiStringRuleAttribute((ERuleAttribute_t)m_eAtt),
				fnRuleApiStringMsgPos((EMsgPos_t)m_ePos),
				fnRuleApiStringRuleMatch((ERuleMatch_t)m_eMatch),
				fnRuleApiStringRuleSave((ERuleSave_t)m_eSave),
				m_bExist,m_szVal1, m_szVal2,m_szVal3);
		return _pszBuff;
	}
}LmRule_t;

/************************ Rule Set Param *************************************************/
typedef struct LmRuleSetParam_t
{
	LmRuleSetParam_t(){m_fnClear();}
	void m_fnClear()
	{
		m_eCond = E_RULE_COND_NONE;
		m_nRuleID = -1;
	}
	void m_fnHtoN()
	{
		m_eCond=htonl(m_eCond);
		m_nRuleID=htonl(m_nRuleID);
	}
	void m_fnNtoH()
	{
		m_eCond=ntohl(m_eCond);
		m_nRuleID=ntohl(m_nRuleID);
	}
	void m_fnSetCond(ERuleCond_t _eCond){m_eCond = _eCond;}
	void m_fnSetRuleID(int _nRuleID){m_nRuleID = _nRuleID;}
	int m_eCond;   //ERuleCond_t
	int m_nRuleID;
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		fnRuleApiStrnCat(_pszBuff,_unSize,
							"%s %d ", fnRuleApiStringRuleCond((ERuleCond_t)m_eCond), m_nRuleID);
		return _pszBuff;
	}
}LmRuleSetParam_t;
/************************* Rule Set (RULESET) ********************************************/
typedef struct LmRuleSet_t
{
	LmRuleSet_t(){m_fnClear();}
	void m_fnHtoN()
	{
		for(unsigned int i =0;i<m_unCnt;i++) m_arrRuleSet[i].m_fnHtoN();
		m_unCnt=htonl(m_unCnt);
	}
	void m_fnNtoH()
	{
		m_unCnt=ntohl(m_unCnt);
		for(unsigned int i =0;i<m_unCnt;i++) m_arrRuleSet[i].m_fnNtoH();
	}
	void m_fnClear()
	{
		m_unCnt=0;
		for(int i=0;i<DEF_LM_PARAM_MAX_SZ;i++) m_arrRuleSet[i].m_fnClear();
	}
	void m_fnSetRuleSet(ERuleCond_t _eCond, int _iID)
	{
		if(m_unCnt>= DEF_LM_PARAM_MAX_SZ) return;
		m_arrRuleSet[m_unCnt].m_fnSetCond( _eCond);
		m_arrRuleSet[m_unCnt].m_fnSetRuleID(_iID);
		m_unCnt++;
	}
	unsigned int m_unCnt;
	LmRuleSetParam_t m_arrRuleSet[DEF_LM_PARAM_MAX_SZ];
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		fnRuleApiStrnCat(_pszBuff,_unSize,"(Count %d), ",m_unCnt);
		for(unsigned int i=0;i<m_unCnt;i++) m_arrRuleSet[i].m_fnStr(_pszBuff,_unSize);
		return _pszBuff;
	}
}LmRuleSet_t;
/*************************** Service Op List (OP_LIST) ***************************************/
typedef struct LmServiceOPList_t
{
	LmServiceOPList_t(){m_fnClear();}
	void m_fnClear()
	{
		m_unCnt = 0;
		for(int i=0;i<DEF_LM_PARAM_MAX_SZ;i++) m_arrServiceOp[i] = -1;
	}
	void m_fnHtoN()
	{
		for(unsigned int i =0;i<m_unCnt;i++) m_arrServiceOp[i] = htonl(m_arrServiceOp[i]);
		m_unCnt=htonl(m_unCnt);
	}
	void m_fnNtoH()
	{
		m_unCnt=ntohl(m_unCnt);
		for(unsigned int i =0;i<m_unCnt;i++) m_arrServiceOp[i] = ntohl(m_arrServiceOp[i]);
	}
	void m_fnSetServiceOP(int _nOP)
	{
		if(_nOP == -1 ) return;
		if(m_unCnt >= DEF_LM_PARAM_MAX_SZ) return;
		m_arrServiceOp[m_unCnt++] = _nOP;
	}
	unsigned int m_unCnt;
	int m_arrServiceOp[DEF_LM_PARAM_MAX_SZ];
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		fnRuleApiStrnCat(_pszBuff,_unSize,"Count=%d, ",m_unCnt);
		for(unsigned int i=0;i<m_unCnt;i++) fnRuleApiStrnCat(_pszBuff,_unSize,"/%d",m_arrServiceOp[i]);
		return _pszBuff;
	}
}LmServiceOPList_t;

/********************** Screen Operation (SCR_OP) *****************************************/
typedef struct LmScreenOp_t
{
	LmScreenOp_t(){m_fnClear();}
	void m_fnClear()
	{
		m_nID = -1;
		m_iScreenActID = -1;
		memset(m_szName,0x00,DEF_LM_RULE_STRING_SZ);
		memcpy(m_szName,DEF_LM_EMPTY_STRING,2);
		m_stRuleSet.m_fnClear();
	}
	void m_fnHtoN()
	{
		m_nID=htonl(m_nID);
		m_iScreenActID=htonl(m_iScreenActID);
		m_stRuleSet.m_fnHtoN();
	}
	void m_fnNtoH()
	{
		m_nID=ntohl(m_nID);
		m_iScreenActID=ntohl(m_iScreenActID);
		m_stRuleSet.m_fnNtoH();
	}
	void m_fnSetID(int _iID){m_nID = _iID;}
	void m_fnSetScreenActID(int _iID){m_iScreenActID = _iID;}
	void m_fnSetName(const char * _pszVal){fnRuleApiStrnCpy(m_szName,_pszVal);}
	void m_fnSetRuleSet(ERuleCond_t _eCond, int _iID)
	{
		m_stRuleSet.m_fnSetRuleSet(_eCond,_iID);
	}
	int m_nID;
	int m_iScreenActID;
	char m_szName[DEF_LM_RULE_STRING_SZ];
	LmRuleSet_t m_stRuleSet;
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		fnRuleApiStrnCat(_pszBuff,_unSize,"<LmScreenOp_t: ID=%d, Name=%s, ScrAct=%d, RuleSet= ",
											m_nID, m_szName,m_iScreenActID);
		m_stRuleSet.m_fnStr(_pszBuff, _unSize);
		fnRuleApiStrnCat(_pszBuff,_unSize,">\r\n");
		return _pszBuff;
	}
}LmScreenOp_t;

/******************* Screen Operation Set (SCR_OPS) ****************************************/
typedef struct LmScreenOpSet_t
{	
	LmScreenOpSet_t(){m_fnClear();}
	void m_fnClear()
	{
		m_nID = -1;
		memset(m_szName,0x00,DEF_LM_RULE_STRING_SZ);
		memcpy(m_szName,DEF_LM_EMPTY_STRING,2);
		m_stOPList.m_fnClear();
	}
	void m_fnHtoN()
	{
		m_nID=htonl(m_nID);
		m_stOPList.m_fnHtoN();
	}
	void m_fnNtoH()
	{
		m_nID=ntohl(m_nID);
		m_stOPList.m_fnNtoH();
	}
	void m_fnSetID(int _iID){m_nID = _iID;}
	void m_fnSetName(const char * _pszVal){fnRuleApiStrnCpy(m_szName,_pszVal);}
	void m_fnSetServiceOP(int _nOP)
	{
		m_stOPList.m_fnSetServiceOP(_nOP);
	}
	int m_nID;
	char m_szName[DEF_LM_RULE_STRING_SZ];
	LmServiceOPList_t m_stOPList;	
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		fnRuleApiStrnCat(_pszBuff,_unSize,"<LmScreenOpSet_t: ID=%d, Name=%s, ",	m_nID, m_szName);
		m_stOPList.m_fnStr(_pszBuff,_unSize);
		fnRuleApiStrnCat(_pszBuff,_unSize,">\r\n");
		return _pszBuff;
	}
}LmScreenOpSet_t;

/***************************** Screen Action (SCR_ACT) *************************************/
typedef struct LmScreenAct_t
{
	LmScreenAct_t(){m_fnClear();}
	void m_fnClear()
	{
		m_nID = -1;
		m_eAct = E_SCREEN_ACTION_NONE;
		m_ePos = E_MSG_POS_NONE;
		memset(m_szName,0x00, DEF_LM_RULE_STRING_SZ);
		memset(m_szVal1,0x00,DEF_LM_RULE_STRING_SZ);
		memset(m_szVal2,0x00,DEF_LM_RULE_STRING_SZ);
		memset(m_szVal3,0x00,DEF_LM_RULE_STRING_SZ);
		memcpy(m_szName,DEF_LM_EMPTY_STRING,2);
		memcpy(m_szVal1,DEF_LM_EMPTY_STRING,2);
		memcpy(m_szVal2,DEF_LM_EMPTY_STRING,2);
		memcpy(m_szVal3,DEF_LM_EMPTY_STRING,2);
	}
	void m_fnHtoN()
	{
		m_nID=htonl(m_nID);
		m_eAct=htonl(m_eAct);
		m_ePos=htonl(m_ePos);
	}
	void m_fnNtoH()
	{
		m_nID=ntohl(m_nID);
		m_eAct=ntohl(m_eAct);
		m_ePos=ntohl(m_ePos);
	}
	void m_fnSetID(int _nID){ m_nID = _nID;}
	void m_fnSetAct(EScreenAct_t _eAct){m_eAct = _eAct;}
	void m_fnSetPos(EMsgPos_t _ePos){m_ePos = _ePos;}
	void m_fnSetName(const char * _pszVal){fnRuleApiStrnCpy(m_szName,_pszVal);}
	void m_fnSetVal1(const char * _pszVal){fnRuleApiStrnCpy(m_szVal1,_pszVal);}
	void m_fnSetVal2(const char * _pszVal){fnRuleApiStrnCpy(m_szVal2,_pszVal);}
	void m_fnSetVal3(const char * _pszVal){fnRuleApiStrnCpy(m_szVal3,_pszVal);}
	int m_nID;
	int m_eAct;      // EScreenAct_t
	int m_ePos;     // EMsgPos_t
	char m_szName[DEF_LM_RULE_STRING_SZ];
	char m_szVal1[DEF_LM_RULE_STRING_SZ];
	char m_szVal2[DEF_LM_RULE_STRING_SZ];
	char m_szVal3[DEF_LM_RULE_STRING_SZ];
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		fnRuleApiStrnCat(_pszBuff,_unSize,
					"<LmScreenAct_t: ID=%d, Name=%s, Act=%s, Pos=%s, Val1=%s, Val2=%s, Val3=%s>\r\n",
					m_nID, m_szName,
					fnRuleApiStringScreenAct((EScreenAct_t) m_eAct),
					fnRuleApiStringMsgPos((EMsgPos_t)m_ePos),
					m_szVal1,m_szVal2,m_szVal3);
		return _pszBuff;
	}
}LmScreenAct_t;
/******************************* Filter Operation (FILTER_OP) ********************************/
typedef struct LmFilterOp_t
{
	LmFilterOp_t(){m_fnClear();}
	void m_fnClear()
	{
		m_nID = -1;
		m_nActID = -1;
		memset(m_szName,0x00, DEF_LM_RULE_STRING_SZ);
		memcpy(m_szName,DEF_LM_EMPTY_STRING,2);
		m_stRuleSet.m_fnClear();
	}
	void m_fnHtoN()
	{
		m_nID=htonl(m_nID);
		m_nActID=htonl(m_nActID);
		m_stRuleSet.m_fnHtoN();
	}
	void m_fnNtoH()
	{
		m_nID=ntohl(m_nID);
		m_nActID=ntohl(m_nActID);
		m_stRuleSet.m_fnNtoH();
	}
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		fnRuleApiStrnCat(_pszBuff,_unSize,"<LmFilterOp_t: ID=%d,Name=%s, Attribute=%s, RuleSet= ",
											m_nID,m_szName,fnRuleApiStringFilterAttribute((EFilterAttribute_t)m_nActID));
		m_stRuleSet.m_fnStr(_pszBuff,_unSize);
		fnRuleApiStrnCat(_pszBuff,_unSize,">\r\n");
		return _pszBuff;
	}
	int m_nID;
	int m_nActID;        // EFilterAttribute_t
	char m_szName[DEF_LM_RULE_STRING_SZ];
	LmRuleSet_t m_stRuleSet;
}LmFilterOp_t;
/******************* Screen Operation Set (FILTER_OPS) ****************************************/
typedef struct LmFilterOpSet_t
{	
	LmFilterOpSet_t(){m_fnClear();}
	void m_fnClear()
	{
		m_nID = -1;
		memset(m_szName,0x00,DEF_LM_RULE_STRING_SZ);
		memcpy(m_szName,DEF_LM_EMPTY_STRING,2);
		m_stOPList.m_fnClear();
	}
	void m_fnHtoN()
	{
		m_nID=htonl(m_nID);
		m_stOPList.m_fnHtoN();
	}
	void m_fnNtoH()
	{
		m_nID=ntohl(m_nID);
		m_stOPList.m_fnNtoH();
	}
	void m_fnSetID(int _iID){m_nID = _iID;}
	void m_fnSetName(const char * _pszVal){fnRuleApiStrnCpy(m_szName,_pszVal);}
	void m_fnSetServiceOP(int _nOP)
	{
		m_stOPList.m_fnSetServiceOP(_nOP);
	}
	int m_nID;
	char m_szName[DEF_LM_RULE_STRING_SZ];
	LmServiceOPList_t m_stOPList;	
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		fnRuleApiStrnCat(_pszBuff,_unSize,"<LmFilterOpSet_t: ID=%d, Name=%s, ",m_nID, m_szName);
		m_stOPList.m_fnStr(_pszBuff,_unSize);
		fnRuleApiStrnCat(_pszBuff,_unSize,">\r\n");
		return _pszBuff;
	}
}LmFilterOpSet_t;

/********************************* Filter Action (FILTER_ACT) ********************************/
typedef struct LmFilterAct_t
{
	LmFilterAct_t(){m_fnClear();}
	void m_fnClear()
	{
		m_nID = -1;
		m_nResponseCode = -1;
		m_nReasonCode = -1;
		memset(m_szName,0x00, DEF_LM_RULE_STRING_SZ);
		memset(m_szResponseDesc,0x00, DEF_LM_RULE_STRING_SZ);
		memset(m_szReasonText,0x00, DEF_LM_RULE_STRING_SZ);
		memcpy(m_szName,DEF_LM_EMPTY_STRING,2);
		memcpy(m_szResponseDesc,DEF_LM_EMPTY_STRING,2);
		memcpy(m_szReasonText,DEF_LM_EMPTY_STRING,2);
	}
	void m_fnHtoN()
	{
		m_nID=htonl(m_nID);
		m_nResponseCode=htonl(m_nResponseCode);
		m_nReasonCode=htonl(m_nReasonCode);
	}
	void m_fnNtoH()
	{
		m_nID=ntohl(m_nID);
		m_nResponseCode=ntohl(m_nResponseCode);
		m_nReasonCode=ntohl(m_nReasonCode);
	}
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		fnRuleApiStrnCat(_pszBuff,_unSize,"<LmFilterAct_t: ID=%d, Name=%s, RspCode=%d, RspDesc=%s,",
										m_nID, m_szName,m_nResponseCode,m_szResponseDesc);
		fnRuleApiStrnCat(_pszBuff,_unSize,"ReasonCode=%d, ReasonText=%s>\r\n",
										m_nReasonCode, m_szReasonText);
		return _pszBuff;
	}
	int m_nID;
	int m_nResponseCode;
	int m_nReasonCode;
	char m_szName[DEF_LM_RULE_STRING_SZ];
	char m_szResponseDesc[DEF_LM_RULE_STRING_SZ];
	char m_szReasonText[DEF_LM_RULE_STRING_SZ];
}LmFilterAct_t;
/****************************** Route Operation (RTE_OP) ************************************/
typedef struct LmRouteOp_t
{
	LmRouteOp_t(){m_fnClear();}
	void m_fnClear()
	{
		m_nID = -1;
		m_eRoute = E_ROUTE_OP_NONE;
		m_nPriority = -1;
		m_nSeq = -1;
		memset(m_szName,0x00,DEF_LM_RULE_STRING_SZ);
		memcpy(m_szName,DEF_LM_EMPTY_STRING,2);
		m_stRuleSet.m_fnClear();
		for(int j=0;j<DEF_LM_PARAM_MAX_SZ;j++) m_nServiceOPS[j] = -1;
	}
	void m_fnHtoN()
	{
		m_nID=htonl(m_nID);
		m_eRoute=htonl(m_eRoute);
		m_nPriority=htonl(m_nPriority);
		m_nSeq=htonl(m_nSeq);
		for(unsigned int i=0;i<E_SERVICE_OPS_MAX;i++) m_nServiceOPS[i] = htonl(m_nServiceOPS[i]);
	}
	void m_fnNtoH()
	{
		m_nID=ntohl(m_nID);
		m_eRoute=ntohl(m_eRoute);
		m_nPriority=ntohl(m_nPriority);
		m_nSeq=ntohl(m_nSeq);
		for(unsigned int i=0;i<E_SERVICE_OPS_MAX;i++) m_nServiceOPS[i] = ntohl(m_nServiceOPS[i]);
	}
	void m_fnSetID(int _nID){m_nID = _nID;}
	void m_fnSetRouteType(ERouteOp_t _eRoute){m_eRoute = _eRoute;}
	void m_fnSetPriority(int _nPriority){m_nPriority = _nPriority;}
	void m_fnSetSeq(int _nSeq){m_nSeq = _nSeq;}
	void m_fnSetName(const char * _pszVal){fnRuleApiStrnCpy(m_szName,_pszVal);}
	void m_fnSetRuleSet(ERuleCond_t _eCond, int _nRuleID)
	{
		m_stRuleSet.m_fnSetRuleSet(_eCond,_nRuleID);
	}
	void m_fnSetServiceOPS(EServiceOPS_t _eService, int _nOPS)
	{
		if(_eService <= E_SERVICE_OPS_NONE && _eService >= E_SERVICE_OPS_MAX) return;
		m_nServiceOPS[_eService] = _nOPS;
	}
	int m_nID;
	int m_eRoute;   // ERouteOp_t 
	int m_nPriority;
	int m_nSeq;
	char m_szName[DEF_LM_RULE_STRING_SZ];
	LmRuleSet_t m_stRuleSet;
	int m_nServiceOPS[DEF_LM_PARAM_MAX_SZ];
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		fnRuleApiStrnCat(_pszBuff,_unSize,
					"<LmRouteOp_t: ID=%d, Name=%s, Type=%s, Priority=%d, Seq=%d, Src_Screen=%d,Dst_Screen=%d,Filter=%d, RuleSet= ",
													m_nID, m_szName,
													fnRuleApiStringRouteOp((ERouteOp_t)m_eRoute),
													m_nPriority, m_nSeq, 
													m_nServiceOPS[E_SERVICE_OPS_SRC_SCREEN],
													m_nServiceOPS[E_SERVICE_OPS_DST_SCREEN],
													m_nServiceOPS[E_SERVICE_OPS_FILTER]);
		m_stRuleSet.m_fnStr(_pszBuff,_unSize);
		fnRuleApiStrnCat(_pszBuff,_unSize,">\r\n");
		return _pszBuff;
	}
}LmRouteOp_t;
#endif
