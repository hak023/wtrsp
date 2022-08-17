#ifndef RESIP_CUNKNOWNPARSE_HXX
#define RESIP_CUNKNOWNPARSE_HXX

#include "CRuleUtiles.h"
#include "CRuSipParse.h"

/********************** Parse Unknown Header Parameter ****************************/
class AppUnknownParam : public RUObject
{
	public:
		enum
		{
			E_APP_PARSE_NONE=0,
			E_APP_PARSE_KEY_STATE=1,
			E_APP_PARSE_KEY_SP=2,
			E_APP_PARSE_VALUE_STATE=3
		};
		typedef bool (*PFuncParseState)(AppUnknownParam *_pclsParam, unsigned int _unStep);
		AppUnknownParam();
		~AppUnknownParam();
		void fnParse(char * _pszParam, bool _bQuestion=false);
		void fnModParam(const char * _pszKey, const char * _pszVal, bool _bQuestion=false);
		void fnDebug();
		void fnBuild(unsigned int _unSize,char * _pszBuild);
		char * m_pszParamK;
		char * m_pszParamV;
		char * m_pszBuff;
		bool m_bQuestion;
		bool m_bQuote;
		unsigned int m_uiState;
	private:
		static bool fnParseStateNone(AppUnknownParam *_pclsParam, unsigned int _unStep);
		static bool fnParseStateKey(AppUnknownParam *_pclsParam, unsigned int _unStep);
		static bool fnParseStateKeySp(AppUnknownParam *_pclsParam, unsigned int _unStep);
		static bool fnParseStateValue(AppUnknownParam *_pclsParam, unsigned int _unStep);
		static PFuncParseState m_pfnParse[E_APP_PARSE_VALUE_STATE+1];
};
/************************* Parse Unknown Header Parameters **********************************/
class AppUnknownParams
{
	public:
		enum
		{
			E_APP_PARSE_PARAMS_NONE=0,
			E_APP_PARSE_PARAMS_SP=1,
			E_APP_PARSE_PARAMS_QSP=2,
			E_APP_PARSE_PARAMS_VAL=3,
			E_APP_PARSE_PARAMS_QVAL=4,
		};
		typedef bool (*PFuncParseState)(AppUnknownParams *_pclsParams, unsigned int _unStep);
		AppUnknownParams();
		~AppUnknownParams();
		void fnParse(char * _pszParam,bool _bQuestion=false);
		void fnDebug();
		void fnBuild(unsigned int _unSize,char * _pszBuild);
		bool fnRemoveParam(const char * _pszParam);
		bool fnAddParam(const char * _pszKey, const char * _pszVal);
		bool fnAddParamF(const char * _pszKey, const char * _pszVal);
		bool fnAddParamQuestion(const char * _pszKey, const char * _pszVal);
		AppUnknownParam * fnFindParam(const char * _pszP);
		AppUnknownParam * fnBegin(){return (AppUnknownParam *)m_listParam.fnBegin();}
		AppUnknownParam * fnNext(){return (AppUnknownParam *)m_listParam.fnNext();}
		RUList m_listParam;
		char * m_pszBuff;
		unsigned int m_uiState;
		bool m_bQuestion;
		char * m_pszCurrentP;
	private:
		void fnParseParam(bool _bQuestion, char * _pszStart);
		static bool fnParseStateNone(AppUnknownParams *_pclsParam, unsigned int _unStep);
		static bool fnParseStateSp(AppUnknownParams *_pclsParam, unsigned int _unStep);
		static bool fnParseStateQSp(AppUnknownParams *_pclsParam, unsigned int _unStep);
		static bool fnParseStateValue(AppUnknownParams *_pclsParam, unsigned int _unStep);
		static bool fnParseStateQValue(AppUnknownParams *_pclsParam, unsigned int _unStep);
		static PFuncParseState m_pfnParse[E_APP_PARSE_PARAMS_QVAL+1];
};
// "display"    <type:user@host;param1=aaaa;param2;l___   _r> 
// "display"    <type;host;param1=aaaa;param2;l___   _r> 
// "display"    <type;user;param1=aaaa;param2;l___   _r> 
class AppUnknownUri
{
	public:
		enum
		{
			E_APP_PARSE_URL_NONE=0,
			E_APP_PARSE_URL_DISPLAY=1,
			E_APP_PARSE_URL_DISPLAY_SP=2,
			E_APP_PARSE_URL_TYPE=3,
			E_APP_PARSE_URL_TYPE_SP=4,
			E_APP_PARSE_URL_USER=5,
			E_APP_PARSE_URL_USER_SP=6,
			E_APP_PARSE_URL_HOST=7,
			E_APP_PARSE_URL_HOST_SP=8,
			E_APP_PARSE_URL_PORT=9,
			E_APP_PARSE_URL_PORT_SP=10,
			E_APP_PARSE_URL_PARAMS=11,
			E_APP_PARSE_URL_QPARAMS=12,
		};
		typedef bool (*PFuncParseState)(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		AppUnknownUri();
		~AppUnknownUri();
		bool fnParse(char * _pszUrl);
		void fnDebug();
		void fnBuild(unsigned int _unSize,char * _pszBuild);
		void fnAddDisplayName(const char * _pszDisplay);
		void fnAddScheme(const char * _pszV);
		void fnAddUser(const char * _pszV);
		void fnAddHost(const char * _pszV);
		void fnAddPort(const char * _pszV);
		bool fnRemoveParam(const char * _pszParam);
		bool fnAddParam(const char * _pszKey, const char * _pszVal);
		bool fnAddParamF(const char * _pszKey, const char * _pszVal);
		bool fnAddParamQuestion(const char * _pszKey, const char * _pszVal);
		AppUnknownParam * fnFindParam(const char * _pszP){return m_clsParams.fnFindParam(_pszP);}
		AppUnknownParam * fnBegin(){return m_clsParams.fnBegin();}
		AppUnknownParam * fnNext(){return m_clsParams.fnNext();}
		char * m_pszDisplay;
		char * m_pszType;
		char * m_pszUser;
		char * m_pszHost;
		char * m_pszPort;
		char * m_pszBuff;
		char * m_pszBackup;
		bool m_bV6;
		unsigned int m_uiSt;
		AppUnknownParams  m_clsParams;
	private:
		static bool fnParseStateNone(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStateDisplay(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStateDisplaySp(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStateType(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStateTypeSp(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStateUser(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStateUserSp(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStateHost(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStateHostSp(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStatePort(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStatePortSp(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStateParams(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static bool fnParseStateQParams(AppUnknownUri *_pclsUrl, unsigned int _unStep);
		static PFuncParseState m_pfnParse[E_APP_PARSE_URL_QPARAMS+1];
		RUBuffers m_clsBuffers;
};
// "display"<type:user@host;param1;param2=b;lr>;tag=1234;lr;video;branch=1234;reason="";
class AppUnknownHParser
{
	public:
		AppUnknownHParser();
		~AppUnknownHParser();
		bool fnParse(const char * _pszHeaderVal,bool _bHexConv = false);
		void fnConvNameAddrSubHeaders(char * _pszSrc);
		void fnDebug();
		void fnBuild(unsigned int _unSize, char * _pszBuild);
		bool fnRemoveParam(const char * _pszParam);
		bool fnRemoveUriParam(const char * _pszParam);
		bool fnAddParam(const char * _pszKey, const char * _pszVal);
		bool fnAddUriParam(const char * _pszKey, const char * _pszVal);
		bool fnAddUriParamF(const char * _pszKey, const char * _pszVal);
		bool fnAddUriParamQuestion(const char * _pszKey, const char * _pszVal);
		void fnAddDisplayName(const char * _pszDisplay){m_clsUri.fnAddDisplayName(_pszDisplay);}
		void fnAddScheme(const char * _pszV){m_clsUri.fnAddScheme(_pszV);}
		void fnAddUser(const char * _pszV){m_clsUri.fnAddUser(_pszV);}
		void fnAddHost(const char * _pszV){m_clsUri.fnAddHost(_pszV);}
		void fnAddPort(const char * _pszV){m_clsUri.fnAddPort(_pszV);}
		AppUnknownParam * fnBeginUriP(){return m_clsUri.fnBegin();}
		AppUnknownParam * fnNextUriP(){return m_clsUri.fnNext();}
		AppUnknownParam * fnBeginHeaderP(){return m_clsParams.fnBegin();}
		AppUnknownParam * fnNextHeaderP(){return m_clsParams.fnNext();}
		AppUnknownParam * fnFindHeaderParam(const char * _pszParamK);
		AppUnknownParam * fnFindUriParam(const char * _pszParamK);
		AppUnknownUri m_clsUri;
		char * m_pszVal;
		char * m_pszBackup;
		AppUnknownParams  m_clsParams;
		RUBuffers m_clsBuff;
		bool m_bIsWellFormed;
};
#endif

