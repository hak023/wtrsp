
#ifndef COM_MMI_DATA_HXX
#define COM_MMI_DATA_HXX

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define D_MMI_REASON_MAX_LEN						128

#define D_MMI_TYPE_REQ	 									0x01000000
#define D_MMI_TYPE_RSP	 									0x02000000

#define D_MMI_SUBTYPE_RULE_DIS_REQ					0x21001181
#define D_MMI_SUBTYPE_RULE_DIS_RSP					0x22001181
#define D_MMI_SUBTYPE_RULE_ADD_REQ				0x21001182
#define D_MMI_SUBTYPE_RULE_ADD_RSP				0x22001182
#define D_MMI_SUBTYPE_RULE_DEL_REQ					0x21001183
#define D_MMI_SUBTYPE_RULE_DEL_RSP					0x22001183
#define D_MMI_SUBTYPE_RULE_CHG_REQ				0x21001184
#define D_MMI_SUBTYPE_RULE_CHG_RSP				0x22001184

#define D_MMI_SUBTYPE_SCRACT_DIS_REQ			0x21001191
#define D_MMI_SUBTYPE_SCRACT_DIS_RSP				0x22001191
#define D_MMI_SUBTYPE_SCRACT_ADD_REQ			0x21001192
#define D_MMI_SUBTYPE_SCRACT_ADD_RSP			0x22001192
#define D_MMI_SUBTYPE_SCRACT_DEL_REQ			0x21001193
#define D_MMI_SUBTYPE_SCRACT_DEL_RSP			0x22001193
#define D_MMI_SUBTYPE_SCRACT_CHG_REQ			0x21001194
#define D_MMI_SUBTYPE_SCRACT_CHG_RSP			0x22001194

#define D_MMI_SUBTYPE_SCROP_DIS_REQ				0x21001201
#define D_MMI_SUBTYPE_SCROP_DIS_RSP				0x22001201
#define D_MMI_SUBTYPE_SCROP_ADD_REQ				0x21001202
#define D_MMI_SUBTYPE_SCROP_ADD_RSP				0x22001202
#define D_MMI_SUBTYPE_SCROP_DEL_REQ				0x21001203
#define D_MMI_SUBTYPE_SCROP_DEL_RSP				0x22001203
#define D_MMI_SUBTYPE_SCROP_CHG_REQ				0x21001204
#define D_MMI_SUBTYPE_SCROP_CHG_RSP				0x22001204

#define D_MMI_SUBTYPE_ROUTEOP_DIS_REQ			0x21001211
#define D_MMI_SUBTYPE_ROUTEOP_DIS_RSP			0x22001211
#define D_MMI_SUBTYPE_ROUTEOP_ADD_REQ		0x21001212
#define D_MMI_SUBTYPE_ROUTEOP_ADD_RSP		0x22001212
#define D_MMI_SUBTYPE_ROUTEOP_DEL_REQ		0x21001213
#define D_MMI_SUBTYPE_ROUTEOP_DEL_RSP			0x22001213
#define D_MMI_SUBTYPE_ROUTEOP_CHG_REQ		0x21001214
#define D_MMI_SUBTYPE_ROUTEOP_CHG_RSP		0x22001214

namespace mmi
{

//Result of MMI request
typedef enum
{
	E_MMI_RESULT_nok = -1,
	E_MMI_RESULT_ok = 0,
}E_MMI_RESULT;

inline const char * g_mmi_strncat(char * _pszSrc, unsigned int _unMaxSrcLen, const char * _pszFormat, ...)
{
	if( !_pszSrc || _unMaxSrcLen == 0 )
		return _pszSrc;
	
	unsigned int unCurrentLen = strlen(_pszSrc);
	if( unCurrentLen >= _unMaxSrcLen -1 )
	{
		_pszSrc[_unMaxSrcLen-1] = 0x00;
	}
	else
	{
		char * pszTemp = &(_pszSrc[unCurrentLen]);
		va_list ap;
		va_start(ap, _pszFormat);
		vsnprintf(pszTemp, _unMaxSrcLen-unCurrentLen-1,_pszFormat, ap);
		va_end(ap);
	}
	
	return _pszSrc;
}

/*==================== RULE ====================*/
struct RuleInfo
{
	unsigned int m_unId; 					//(1~)
	char m_szName[32];
	int m_nMain;									//EMain
	char m_szMainValue[64];
	int m_nMainExist;							//EExist	
	int m_nSubPart;							//ESubPart
	int m_nSubRange;						//ESubRange
	char m_szSubValue1[128];
	int m_nSubExist;							//EExist
	char m_szSubValue2[128];

	void m_fnInit()
	{
		m_unId = 0;
		m_szName[0] = 0x00;
		m_nMain = -1;
		m_szMainValue[0] = 0x00;
		m_nMainExist = -1;
		m_nSubPart = -1;
		m_nSubRange = -1;
		m_szSubValue1[0] = 0x00;
		m_nSubExist = -1;
		m_szSubValue2[0] = 0x00;
	}

	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;
	
		return g_mmi_strncat(_pszStr, _unMaxLen, "(Id=%u,Name=%s,Main=%d,MainValue=%s,MainExist=%d,SubPart=%d,SubRange=%d,SubValue1=%s,SubExist=%d,SubValue2=%s)", 
																						m_unId, 
																						m_szName, 
																						m_szMainValue,
																						m_nMainExist,
																						m_nSubPart,
																						m_nSubRange,
																						m_szSubValue1,
																						m_nSubExist,
																						m_szSubValue2);
	}
};

struct DisRuleReq
{
	unsigned int m_unID;

	void m_fnInit(){	m_unID = 0; }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		return g_mmi_strncat(_pszStr, _unMaxLen, "DisRuleReq{Id=%u}", m_unID);
	}
};

struct DisRuleRsp
{
	enum
	{
		E_CONST_MAX_RULE_NUM = 100,
	};

	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

	unsigned int m_unDataNum;
	RuleInfo m_arrRuleInfo[E_CONST_MAX_RULE_NUM];
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_unDataNum = 0;
		for( unsigned int i = 0; i < E_CONST_MAX_RULE_NUM; ++i )
		{
			m_arrRuleInfo[i].m_fnInit();
		}
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "DisRuleRsp{Result=%d,Reason=%d,ReasonTxt=%s,Num=%u,", 
																			m_nResult, m_nReason, m_szReason, m_unDataNum);

		for( unsigned int i = 0; i < m_unDataNum; ++i )
		{
			if( i >= E_CONST_MAX_RULE_NUM )
				break;

			g_mmi_strncat(_pszStr, _unMaxLen, "\n");
			m_arrRuleInfo[i].m_fnStr(_pszStr, _unMaxLen, false);
			g_mmi_strncat(_pszStr, _unMaxLen, ",");
		}

		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct AddRuleReq
{
	RuleInfo m_stRuleInfo;

	void m_fnInit(){	m_stRuleInfo.m_fnInit(); }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "AddRuleReq{");
		m_stRuleInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct AddRuleRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	RuleInfo m_stRuleInfo ;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stRuleInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "AddRuleRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stRuleInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct DelRuleReq
{
	unsigned int m_unID;

	void m_fnInit(){	m_unID = 0; }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		return g_mmi_strncat(_pszStr, _unMaxLen, "DelRuleReq{Id=%u}", m_unID);
	}
};

struct DelRuleRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	RuleInfo m_stRuleInfo ;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stRuleInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "DelRuleRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stRuleInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct ChgRuleReq
{
	RuleInfo m_stRuleInfo;

	void m_fnInit(){	m_stRuleInfo.m_fnInit(); }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "ChgRuleReq{");
		m_stRuleInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct ChgRuleRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	RuleInfo m_stRuleInfo ;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stRuleInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "ChgRuleRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stRuleInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};


/*==================== Screen Action ====================*/
struct ScreenActionInfo
{
	unsigned int m_unId;				//(1~)
	int m_nSipScrOp;						//ESipScrOp	
	int m_nSipScrMain;					//ESipScrMain
	char m_szSipScrMainValue[64];	
	int m_nSipScrSubPart;				//ESipScrSubPart
	int m_nSipScrSubRange;			//ESipScrSubRange	
	char m_szSipScrSubValue1[128];
	char m_szSipScrSubValue2[128];
	int m_nSipScrSubIdx;

	void m_fnInit()
	{
		m_unId = 0;
		m_nSipScrOp = -1;
		m_nSipScrMain = -1;
		m_szSipScrMainValue[0] = 0x00;
		m_nSipScrSubPart = -1;
		m_nSipScrSubRange = -1;
		m_szSipScrSubValue1[0] = 0x00;
		m_szSipScrSubValue2[0] = 0x00;
		m_nSipScrSubIdx = -1;
	}

	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;
	
		return g_mmi_strncat(_pszStr, _unMaxLen, "(Id=%u,Op=%d,Main=%d,MainValue=%s,SubPart=%d,SubRange=%d,SubValue1=%s,SubValue2=%s,SubIdx=%d)", 
																						m_unId, 
																						m_nSipScrOp, 
																						m_nSipScrMain,
																						m_szSipScrMainValue,
																						m_nSipScrSubPart,
																						m_nSipScrSubRange,
																						m_szSipScrSubValue1,
																						m_szSipScrSubValue2,
																						m_nSipScrSubIdx);
	}
};

struct DisScrActReq
{
	unsigned int m_unID;

	void m_fnInit(){	m_unID = 0; }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		return g_mmi_strncat(_pszStr, _unMaxLen, "DisScrActReq{Id=%u}", m_unID);
	}
};

struct DisScrActRsp
{
	enum
	{
		E_CONST_MAX_SCR_ACT_NUM = 100,
	};

	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

	unsigned int m_unDataNum;
	ScreenActionInfo m_arrScrActInfo[E_CONST_MAX_SCR_ACT_NUM];
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_unDataNum = 0;
		for( unsigned int i = 0; i < E_CONST_MAX_SCR_ACT_NUM; ++i )
		{
			m_arrScrActInfo[i].m_fnInit();
		}
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "DisScrActRsp{Result=%d,Reason=%d,ReasonTxt=%s,Num=%u,", 
																			m_nResult, m_nReason, m_szReason, m_unDataNum);

		for( unsigned int i = 0; i < m_unDataNum; ++i )
		{
			if( i >= E_CONST_MAX_SCR_ACT_NUM )
				break;

			g_mmi_strncat(_pszStr, _unMaxLen, "\n");
			m_arrScrActInfo[i].m_fnStr(_pszStr, _unMaxLen, false);
			g_mmi_strncat(_pszStr, _unMaxLen, ",");
		}

		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct AddScrActReq
{
	ScreenActionInfo m_stScrActInfo;

	void m_fnInit(){	m_stScrActInfo.m_fnInit(); }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "AddScrActReq{");
		m_stScrActInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct AddScrActRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	ScreenActionInfo m_stScrActInfo;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stScrActInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "AddScrActRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stScrActInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct DelScrActReq
{
	unsigned int m_unID;

	void m_fnInit(){	m_unID = 0; }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		return g_mmi_strncat(_pszStr, _unMaxLen, "DelScrActReq{Id=%u}", m_unID);
	}
};

struct DelScrActRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	ScreenActionInfo m_stScrActInfo;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stScrActInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "DelScrActRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stScrActInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct ChgScrActReq
{
	ScreenActionInfo m_stScrActInfo;

	void m_fnInit(){	m_stScrActInfo.m_fnInit(); }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "ChgScrActReq{");
		m_stScrActInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct ChgScrActRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	ScreenActionInfo m_stScrActInfo;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stScrActInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "ChgScrActRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stScrActInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};


/*==================== Screen Operation ====================*/
struct ScrOpInfo
{
	unsigned int m_unId;		//(1~)
	char m_szName[32];
	char m_szRuleMix[64];
	char m_szScr[128];

	void m_fnInit()
	{
		m_unId = 0;
		m_szName[0] = 0x00;
		m_szRuleMix[0] = 0x00;
		m_szScr[0] = 0x00;
	}

	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;
	
		return g_mmi_strncat(_pszStr, _unMaxLen, "(Id=%u,Name=%s,RuleMix=%s,Scr=%s)", 
																						m_unId, m_szName, m_szRuleMix, m_szScr);
	}
};

struct DisScrOpReq
{
	unsigned int m_unID;

	void m_fnInit(){	m_unID = 0; }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		return g_mmi_strncat(_pszStr, _unMaxLen, "DisScrOpReq{Id=%u}", m_unID);
	}
};

struct DisScrOpRsp
{
	enum
	{
		E_CONST_MAX_SCR_OP_NUM = 100,
	};

	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

	unsigned int m_unDataNum;
	ScrOpInfo m_arrScrOpInfo[E_CONST_MAX_SCR_OP_NUM];
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_unDataNum = 0;
		for( unsigned int i = 0; i < E_CONST_MAX_SCR_OP_NUM; ++i )
		{
			m_arrScrOpInfo[i].m_fnInit();
		}
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "DisScrOpRsp{Result=%d,Reason=%d,ReasonTxt=%s,Num=%u,", 
																			m_nResult, m_nReason, m_szReason, m_unDataNum);

		for( unsigned int i = 0; i < m_unDataNum; ++i )
		{
			if( i >= E_CONST_MAX_SCR_OP_NUM )
				break;

			g_mmi_strncat(_pszStr, _unMaxLen, "\n");
			m_arrScrOpInfo[i].m_fnStr(_pszStr, _unMaxLen, false);
			g_mmi_strncat(_pszStr, _unMaxLen, ",");
		}

		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct AddScrOpReq
{
	ScrOpInfo m_stScrOpInfo;

	void m_fnInit(){	m_stScrOpInfo.m_fnInit(); }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "AddScrOpReq{");
		m_stScrOpInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct AddScrOpRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	ScrOpInfo m_stScrOpInfo;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stScrOpInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "AddScrOpRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stScrOpInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct DelScrOpReq
{
	unsigned int m_unID;

	void m_fnInit(){	m_unID = 0; }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		return g_mmi_strncat(_pszStr, _unMaxLen, "DelScrOpReq{Id=%u}", m_unID);
	}
};

struct DelScrOpRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	ScrOpInfo m_stScrOpInfo;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stScrOpInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "DelScrOpRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stScrOpInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct ChgScrOpReq
{
	ScrOpInfo m_stScrOpInfo;

	void m_fnInit(){	m_stScrOpInfo.m_fnInit(); }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "ChgScrOpReq{");
		m_stScrOpInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct ChgScrOpRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	ScrOpInfo m_stScrOpInfo;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stScrOpInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "ChgScrOpRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stScrOpInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};


/*==================== Route Operation ====================*/
struct RouteOpInfo
{
	unsigned int m_unId;					//(1~)
	char m_szName[32];
	unsigned int m_unPriority;			//(1~)
	char m_szRuleMix[64];
	unsigned int m_unRouteSeqId;	//(1~)
	char m_szScrOp[64];

	void m_fnInit()
	{
		m_unId = 0;
		m_szName[0] = 0x00;
		m_unPriority = 0;
		m_szRuleMix[0] = 0x00;
		m_unRouteSeqId = 0;
		m_szScrOp[0] = 0x00;
	}

	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;
	
		return g_mmi_strncat(_pszStr, _unMaxLen, "(Id=%u,Name=%s,Priority=%u,RuleMix=%s,RouteSeqId=%u,ScrOp=%s)", 
																						m_unId, m_szName, m_unPriority, m_szRuleMix, m_unRouteSeqId, m_szScrOp);
	}
};

struct DisRouteOpReq
{
	unsigned int m_unID;

	void m_fnInit(){	m_unID = 0; }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		return g_mmi_strncat(_pszStr, _unMaxLen, "DisRouteOpReq{Id=%u}", m_unID);
	}
};

struct DisRouteOpRsp
{
	enum
	{
		E_CONST_MAX_ROUTE_OP_NUM = 100,
	};

	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

	unsigned int m_unDataNum;
	RouteOpInfo m_arrRouteOpInfo[E_CONST_MAX_ROUTE_OP_NUM];
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_unDataNum = 0;
		for( unsigned int i = 0; i < E_CONST_MAX_ROUTE_OP_NUM; ++i )
		{
			m_arrRouteOpInfo[i].m_fnInit();
		}
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "DisRouteOpRsp{Result=%d,Reason=%d,ReasonTxt=%s,Num=%u,", 
																			m_nResult, m_nReason, m_szReason, m_unDataNum);

		for( unsigned int i = 0; i < m_unDataNum; ++i )
		{
			if( i >= E_CONST_MAX_ROUTE_OP_NUM )
				break;

			g_mmi_strncat(_pszStr, _unMaxLen, "\n");
			m_arrRouteOpInfo[i].m_fnStr(_pszStr, _unMaxLen, false);
			g_mmi_strncat(_pszStr, _unMaxLen, ",");
		}

		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct AddRouteOpReq
{
	RouteOpInfo m_stRouteOpInfo;

	void m_fnInit(){	m_stRouteOpInfo.m_fnInit(); }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "AddRouteOpReq{");
		m_stRouteOpInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct AddRouteOpRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	RouteOpInfo m_stRouteOpInfo;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stRouteOpInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "AddRouteOpRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stRouteOpInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct DelRouteOpReq
{
	unsigned int m_unID;

	void m_fnInit(){	m_unID = 0; }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		return g_mmi_strncat(_pszStr, _unMaxLen, "DelRouteOpReq{Id=%u}", m_unID);
	}
};

struct DelRouteOpRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	RouteOpInfo m_stRouteOpInfo;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stRouteOpInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "DelRouteOpRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stRouteOpInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct ChgRouteOpReq
{
	RouteOpInfo m_stRouteOpInfo;

	void m_fnInit(){	m_stRouteOpInfo.m_fnInit(); }
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "ChgRouteOpReq{");
		m_stRouteOpInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

struct ChgRouteOpRsp
{
	int m_nResult;		//E_MMI_RESULT
	int m_nReason;
	char m_szReason[D_MMI_REASON_MAX_LEN];

 	RouteOpInfo m_stRouteOpInfo;
	
	void m_fnInit()
	{
		m_nResult = E_MMI_RESULT_nok;
		m_nReason = 0;
		m_szReason[0] = 0x00;
		
		m_stRouteOpInfo.m_fnInit();
	}
	
	const char * m_fnStr(char * _pszStr, unsigned int _unMaxLen, bool _bInit=true) const
	{
		if( !_pszStr || _unMaxLen == 0 )
			return _pszStr;

		if( _bInit )
			_pszStr[0] = 0x00;

		g_mmi_strncat(_pszStr, _unMaxLen, "ChgRouteOpRsp{Result=%d,Reason=%d,ReasonTxt=%s,", 
																			m_nResult, m_nReason, m_szReason);
		m_stRouteOpInfo.m_fnStr(_pszStr, _unMaxLen, false);
		g_mmi_strncat(_pszStr, _unMaxLen, "}");

		return _pszStr;
	}
};

inline unsigned int g_fnGetBodyLen(unsigned int _unSubType)
{
	switch(_unSubType)
	{
		//rule
		case D_MMI_SUBTYPE_RULE_DIS_REQ :
		{
			return sizeof(DisRuleReq);
		}
		break;
		case D_MMI_SUBTYPE_RULE_DIS_RSP :
		{
			return sizeof(DisRuleRsp);
		}
		break;
		case D_MMI_SUBTYPE_RULE_ADD_REQ :
		{
			return sizeof(AddRuleReq);
		}
		break;
		case D_MMI_SUBTYPE_RULE_ADD_RSP :
		{
			return sizeof(AddRuleRsp);
		}
		break;
		case D_MMI_SUBTYPE_RULE_DEL_REQ :
		{
			return sizeof(DelRuleReq);
		}
		break;
		case D_MMI_SUBTYPE_RULE_DEL_RSP :
		{
			return sizeof(DelRuleRsp);
		}
		break;
		case D_MMI_SUBTYPE_RULE_CHG_REQ :
		{
			return sizeof(ChgRuleReq);
		}
		break;
		case D_MMI_SUBTYPE_RULE_CHG_RSP :
		{
			return sizeof(ChgRuleRsp);
		}
		break;
		//screen action
		case D_MMI_SUBTYPE_SCRACT_DIS_REQ :
		{
			return sizeof(DisScrActReq);
		}
		break;
		case D_MMI_SUBTYPE_SCRACT_DIS_RSP :
		{
			return sizeof(DisScrActRsp);
		}
		break;
		case D_MMI_SUBTYPE_SCRACT_ADD_REQ :
		{
			return sizeof(AddScrActReq);
		}
		break;
		case D_MMI_SUBTYPE_SCRACT_ADD_RSP :
		{
			return sizeof(AddScrActRsp);
		}
		break;
		case D_MMI_SUBTYPE_SCRACT_DEL_REQ :
		{
			return sizeof(DelScrActReq);
		}
		break;
		case D_MMI_SUBTYPE_SCRACT_DEL_RSP :
		{
			return sizeof(DelScrActRsp);
		}
		break;
		case D_MMI_SUBTYPE_SCRACT_CHG_REQ :
		{
			return sizeof(ChgScrActReq);
		}
		break;
		case D_MMI_SUBTYPE_SCRACT_CHG_RSP :
		{
			return sizeof(ChgScrActRsp);
		}
		break;
		//screen Op
		case D_MMI_SUBTYPE_SCROP_DIS_REQ :
		{
			return sizeof(DisScrOpReq);
		}
		break;
		case D_MMI_SUBTYPE_SCROP_DIS_RSP :
		{
			return sizeof(DisScrOpRsp);
		}
		break;
		case D_MMI_SUBTYPE_SCROP_ADD_REQ :
		{
			return sizeof(AddScrOpReq);
		}
		break;
		case D_MMI_SUBTYPE_SCROP_ADD_RSP :
		{
			return sizeof(AddScrOpRsp);
		}
		break;
		case D_MMI_SUBTYPE_SCROP_DEL_REQ :
		{
			return sizeof(DelScrOpReq);
		}
		break;
		case D_MMI_SUBTYPE_SCROP_DEL_RSP :
		{
			return sizeof(DelScrOpRsp);
		}
		break;
		case D_MMI_SUBTYPE_SCROP_CHG_REQ :
		{
			return sizeof(ChgScrOpReq);
		}
		break;
		case D_MMI_SUBTYPE_SCROP_CHG_RSP :
		{
			return sizeof(ChgScrOpRsp);
		}
		break;
		//route Op
		case D_MMI_SUBTYPE_ROUTEOP_DIS_REQ :
		{
			return sizeof(DisRouteOpReq);
		}
		break;
		case D_MMI_SUBTYPE_ROUTEOP_DIS_RSP :
		{
			return sizeof(DisRouteOpRsp);
		}
		break;
		case D_MMI_SUBTYPE_ROUTEOP_ADD_REQ :
		{
			return sizeof(AddRouteOpReq);
		}
		break;
		case D_MMI_SUBTYPE_ROUTEOP_ADD_RSP :
		{
			return sizeof(AddRouteOpRsp);
		}
		break;
		case D_MMI_SUBTYPE_ROUTEOP_DEL_REQ :
		{
			return sizeof(DelRouteOpReq);
		}
		break;
		case D_MMI_SUBTYPE_ROUTEOP_DEL_RSP :
		{
			return sizeof(DelRouteOpRsp);
		}
		break;
		case D_MMI_SUBTYPE_ROUTEOP_CHG_REQ :
		{
			return sizeof(ChgRouteOpReq);
		}
		break;
		case D_MMI_SUBTYPE_ROUTEOP_CHG_RSP :
		{
			return sizeof(ChgRouteOpRsp);
		}
		break;
	}

	return 0;
}

}

#endif

