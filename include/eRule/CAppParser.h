#ifndef ESIP_CAPPPARSER_HXX
#define ESIP_CAPPPARSER_HXX


#include "resip/stack/SipMessage.hxx"
#include "rutil/Logger.hxx"
#include "rutil/TransportType.hxx"
#include "rutil/TimeLimitFifo.hxx"

#include "eDum/eDumLibDef.hxx"
#include "eDum/eDumLibStack.hxx"
#include "eDum/eDumLibSession.hxx"
#include "resip/stack/UnknownParameter.hxx"
#include "resip/stack/QValueParameter.hxx"
#include "resip/stack/QValue.hxx"
#include "resip/stack/ParserCategory.hxx"
#include "resip/stack/ParserContainer.hxx"

#include "eSipType.hxx"
#include "eSipCallback.hxx"
#include "eSipTu.hxx"
#include "eSipAppMessage.hxx"
#include "CRuleUtiles.h"
#include "CUnknownParse.h"
#include "CRule.h"

/**************************** Header Category Enum **************************************/
typedef enum
{
	E_APP_H_CATE_NONE = 0,
	E_APP_H_CATE_VIA,
	E_APP_H_CATE_INT32,
	E_APP_H_CATE_NAMEADDR,
	E_APP_H_CATE_TOKEN,
	E_APP_H_CATE_AUTH,
	E_APP_H_CATE_STRING,
	E_APP_H_CATE_GENURI,
	E_APP_H_CATE_CALLID,
	E_APP_H_CATE_CSEQ,
	E_APP_H_CATE_EXPIRES,
	E_APP_H_CATE_MIME,
	E_APP_H_CATE_DATE,
	E_APP_H_CATE_WARNING,
	E_APP_H_CATE_RACK,
	E_APP_H_CATE_PRIVACY,	
	E_APP_H_CATE_URI,	
}EAppHCategory_t;

/**************************** Parameter Category Enum ***********************************/
typedef enum
{
	E_APP_P_CATE_NONE = 0,
	E_APP_P_CATE_EXIST,     //   E_APP_P_CATE_EXIST
	E_APP_P_CATE_QUOTED,    // E_APP_P_CATE_QUOTED
	E_APP_P_CATE_EXISTOR,    // E_APP_P_CATE_EXISTOR
	E_APP_P_CATE_INT32,       // E_APP_P_CATE_INT32
	E_APP_P_CATE_DATA,        //E_APP_P_CATE_DATA
	E_APP_P_CATE_BRANCH,     // BranchParameter
	E_APP_P_CATE_QVALUE,     // E_APP_P_CATE_QVALUE
	E_APP_P_CATE_RPORT,         //   RportParameter
}EAppPCategory_t;

/************************** Child resip Parameter Class ********************************/
/*
class AppParam : public resip::Parameter
{
	public:
		AppParam();
		~AppParam();
		resip::ParameterTypes::Type fnGetType();
		void fnGetValue(char ** _ppszK, char ** _ppszV, unsigned long * _pulV);
};
class AppUParam :public resip::UnknownParameter
{
	public:
		AppUParam();
		~AppUParam();
		void fnGetValue(char ** _ppszK, char ** _ppszV, unsigned long * _pulV);
};
*/
class AppCapParam    // resip param capsule
{
	public:
		AppCapParam();
		~AppCapParam();
		void fnCapParam(resip::Parameter * _pclsP);
		void fnGetValue(char ** _ppszK, char ** _ppszV, unsigned long * _pulV);
		resip::Parameter * m_pclsP;
		resip::UnknownParameter* m_pclsUP;
};


/************************** Child resip Uri Class *************************************/
class AppUri : public resip::Uri
{
	public:
		AppUri();
		~AppUri();
		void fnDebug();
		void fnGetParam(EAppHCategory_t _eHCate,
									resip::ParameterTypes::Type _eParam,
									const char * _pszParam,
									char ** _ppszK, 
									char ** _ppszV,
									unsigned long * _ulV,
									char** _ppszUnknownResult);
		void fnAddParam(EAppHCategory_t _eHCate,
									resip::Headers::Type _eHeader,
									const char * _pszKey, 
									const char * _pszVal);
		void fnRemoveParam(EAppHCategory_t _eHCate,
									resip::ParameterTypes::Type _eParam,
									const char * _pszParam);
		char * fnScheme(){return (char*)scheme().c_str();}
		char * fnUser(){return (char*)user().c_str();}
		char *fnHost(){return (char*)host().c_str();}
		int fnPort(){return port();}
};
class AppCapUri      // resip uri capsule
{
	public:
		AppCapUri();
		~AppCapUri();
		void fnCapUri(resip::Uri * _pclsUri);
		void fnDebug();
		void fnGetParam(EAppHCategory_t _eHCate,
									resip::ParameterTypes::Type _eParam,
									const char * _pszParam,
									char ** _ppszK, 
									char ** _ppszV,
									unsigned long * _ulV);
		void fnAddParam(EAppHCategory_t _eHCate,
									resip::Headers::Type _eHeader,
									const char * _pszKey, 
									const char * _pszVal);
		void fnRemoveParam(EAppHCategory_t _eHCate,
									resip::ParameterTypes::Type _eParam,
									const char * _pszParam);
		char * fnScheme(){return m_pclsUri->fnScheme();}
		char * fnUser(){return m_pclsUri->fnUser();}
		char *fnHost(){return m_pclsUri->fnHost();}
		int fnPort(){return m_pclsUri->fnPort();}
		AppUri * m_pclsUri;
		AppUnknownHParser m_clsParser;
};

/***************************** Child resip Header Class ************************************/
extern resip::ParameterTypes::Factory g_ParameterFactories[resip::ParameterTypes::MAX_PARAMETER];
class AppHeader : public resip::ParserCategory
{
	public:
		AppHeader();
		~AppHeader();
		resip::Parameter* createParam(resip::ParameterTypes::Type type, 
											resip::ParseBuffer& pb, 
											const std::bitset<256>& terminators, 
											resip::PoolBase* pool);
		void fnAddParam(EAppHCategory_t _eHCate,
									resip::Headers::Type _eHeader,
									const char * _pszKey, 
									const char * _pszVal);
		resip::Parameter * fnGetGenParam(EAppHCategory_t _eHCategory,
												resip::ParameterTypes::Type _eParam, 
												const char * _pszParam,
												resip::Data ** _ppclsUnknownResult);
		void fnGetParam(EAppHCategory_t _eHCate,
									resip::ParameterTypes::Type _eParam,
									const char * _pszParam,
									char ** _ppszK, 
									char ** _ppszV,
									unsigned long * _ulV);
		void fnRemoveParam(EAppHCategory_t _eHCate,
									resip::ParameterTypes::Type _eParam,
									const char * _pszParam);
		resip::Uri * fnGetUri(EAppHCategory_t _eHCategory);
		void fnDebug(EAppHCategory_t _eHCategory);
		
		#define defineParam(_enum, _name, _type, _RFC_ref_ignored)                      \
		const _enum##_Param::DType& param(const _enum##_Param& paramType) const;  \
		_enum##_Param::DType& param(const _enum##_Param& paramType); \
		friend class _enum##_Param
		defineParam(resip::data, "data", ExistsParameter, "RFC 3840");
		defineParam(resip::control, "control", ExistsParameter, "RFC 3840");
		defineParam(resip::mobility, "mobility", QuotedDataParameter, "RFC 3840"); // mobile|fixed
		defineParam(resip::description, "description", QuotedDataParameter, "RFC 3840"); // <> quoted
		defineParam(resip::events, "events", QuotedDataParameter, "RFC 3840"); // list
		defineParam(resip::priority, "priority", QuotedDataParameter, "RFC 3840"); // non-urgent|normal|urgent|emergency
		defineParam(resip::methods, "methods", QuotedDataParameter, "RFC 3840"); // list
		defineParam(resip::schemes, "schemes", QuotedDataParameter, "RFC 3840"); // list
		defineParam(resip::application, "application", ExistsParameter, "RFC 3840");
		defineParam(resip::video, "video", ExistsParameter, "RFC 3840");
		defineParam(resip::language, "language", QuotedDataParameter, "RFC 3840"); // list
		defineParam(resip::type, "type", QuotedDataParameter, "RFC 3840"); // list
		defineParam(resip::isFocus, "isfocus", ExistsParameter, "RFC 3840");
		defineParam(resip::actor, "actor", QuotedDataParameter, "RFC 3840"); // principal|msg-taker|attendant|information
		defineParam(resip::text, "text", ExistsOrDataParameter,  "RFC 3326/3840");
		defineParam(resip::cause, "cause", UInt32Parameter, "RFC3326");
		defineParam(resip::extensions, "extensions", QuotedDataParameter, "RFC 3840"); //list
		defineParam(resip::Instance, "+sip.instance", QuotedDataParameter, "RFC 5626");  // <> quoted
		defineParam(resip::regid, "reg-id", UInt32Parameter, "RFC 5626");
		defineParam(resip::ob,"ob",ExistsParameter, "RFC 5626");
		defineParam(resip::pubGruu, "pub-gruu", QuotedDataParameter, "RFC 5627");
		defineParam(resip::tempGruu, "temp-gruu", QuotedDataParameter, "RFC 5627");
		defineParam(resip::gr, "gr", ExistsOrDataParameter, "RFC 5627");
		defineParam(resip::accessType, "access-type", DataParameter, "RFC 2046");
		defineParam(resip::algorithm, "algorithm", DataParameter, "RFC 2617");
		defineParam(resip::boundary, "boundary", DataParameter, "RFC 2046");
		defineParam(resip::branch, "branch", BranchParameter, "RFC 3261");
		defineParam(resip::charset, "charset", DataParameter, "RFC 2045");
		defineParam(resip::cnonce, "cnonce", QuotedDataParameter, "RFC 2617");
		defineParam(resip::comp, "comp", DataParameter,  "RFC 3486");
		defineParam(resip::dAlg, "d-alg", DataParameter, "RFC 3329");
		defineParam(resip::dQop, "d-qop", DataParameter, "RFC 3329");
		defineParam(resip::dVer, "d-ver", QuotedDataParameter, "RFC 3329");
		defineParam(resip::directory, "directory", DataParameter, "RFC 2046");
		defineParam(resip::domain, "domain", QuotedDataParameter, "RFC 3261");
		defineParam(resip::duration, "duration", UInt32Parameter, "RFC 4240");
		defineParam(resip::expiration, "expiration", QuotedDataParameter, "RFC 2046");
		defineParam(resip::expires, "expires", UInt32Parameter, "RFC 3261");
		defineParam(resip::filename, "filename", DataParameter, "RFC 2183");
		defineParam(resip::fromTag, "from-tag", DataParameter, "RFC 4235");
		defineParam(resip::handling, "handling", DataParameter, "RFC 3261");
		defineParam(resip::id, "id", DataParameter, "RFC 3265");
		defineParam(resip::lr, "lr", ExistsParameter, "RFC 3261");
		defineParam(resip::maddr, "maddr", DataParameter, "RFC 3261");
		defineParam(resip::method, "method", DataParameter, "RFC 3261");
		defineParam(resip::micalg, "micalg", DataParameter, "RFC 1847");
		defineParam(resip::mode, "mode", DataParameter, "RFC 2046");
		defineParam(resip::name, "name", DataParameter, "RFC 2046");
		defineParam(resip::nc, "nc", DataParameter, "RFC 2617");
		defineParam(resip::nonce, "nonce", QuotedDataParameter, "RFC 2617");
		defineParam(resip::opaque, "opaque", QuotedDataParameter, "RFC 2617");
		defineParam(resip::permission, "permission", DataParameter, "RFC 2046");
		defineParam(resip::protocol, "protocol", QuotedDataParameter, "RFC 1847");
		defineParam(resip::purpose, "purpose", DataParameter, "RFC 3261");
		defineParam(resip::q, "q", QValueParameter, "RFC 3261");
		defineParam(resip::realm, "realm", QuotedDataParameter, "RFC 2617");
		defineParam(resip::reason, "reason", DataParameter, "RFC 3265");
		defineParam(resip::received, "received", DataParameter, "RFC 3261");
		defineParam(resip::require, "require", DataParameter, "RFC 5373");
		defineParam(resip::response, "response", QuotedDataParameter, "RFC 3261");
		defineParam(resip::retryAfter, "retry-after", UInt32Parameter, "RFC 3265");
		defineParam(resip::rinstance, "rinstance", DataParameter, "proprietary (resip)");
		defineParam(resip::rport, "rport", RportParameter, "RFC 3581");
		defineParam(resip::server, "server", DataParameter, "RFC 2046");
		defineParam(resip::site, "site", DataParameter, "RFC 2046");
		defineParam(resip::size, "size", DataParameter, "RFC 2046");
		defineParam(resip::smimeType, "smime-type", DataParameter, "RFC 2633");
		defineParam(resip::stale, "stale", DataParameter, "RFC 2617");
		defineParam(resip::tag, "tag", DataParameter, "RFC 3261");
		defineParam(resip::toTag, "to-tag", DataParameter, "RFC 4235");
		defineParam(resip::transport, "transport", DataParameter, "RFC 3261");
		defineParam(resip::ttl, "ttl", UInt32Parameter, "RFC 3261");
		defineParam(resip::uri, "uri", QuotedDataParameter, "RFC 3261");
		defineParam(resip::user, "user", DataParameter, "RFC 3261, 4967");
		defineParam(resip::extension, "ext", DataParameter, "RFC 3966"); // Token is used when ext is a user-parameter
		defineParam(resip::username, "username", QuotedDataParameter, "RFC 3261");
		defineParam(resip::earlyOnly, "early-only", ExistsParameter, "RFC 3891");
		defineParam(resip::refresher, "refresher", DataParameter, "RFC 4028");
		defineParam(resip::profileType, "profile-type", DataParameter, "RFC 6080");
		defineParam(resip::vendor, "vendor", QuotedDataParameter, "RFC 6080");
		defineParam(resip::model, "model", QuotedDataParameter, "RFC 6080");
		defineParam(resip::version, "version", QuotedDataParameter, "RFC 6080");
		defineParam(resip::effectiveBy, "effective-by", UInt32Parameter, "RFC 6080");
		defineParam(resip::document, "document", DataParameter, "draft-ietf-sipping-config-framework-07 (removed in 08)");
		defineParam(resip::appId, "app-id", DataParameter, "draft-ietf-sipping-config-framework-05 (renamed to auid in 06, which was then removed in 08)");
		defineParam(resip::networkUser, "network-user", DataParameter, "draft-ietf-sipping-config-framework-11 (removed in 12)");
		defineParam(resip::url, "url", QuotedDataParameter, "RFC 4483");
		defineParam(resip::sigcompId, "sigcomp-id", QuotedDataParameter, "RFC 5049");
		defineParam(resip::qop,"qop",DataParameter, "RFC 3261");
		defineParam(resip::index, "index", DataParameter, "RFC 4244");
		defineParam(resip::rc, "rc", DataParameter, "RFC 4244-bis");
		defineParam(resip::mp, "mp", DataParameter, "RFC 4244-bis");
		defineParam(resip::np, "np", DataParameter, "RFC 4244-bis");
		// Internal use only
		defineParam(resip::qopOptions,"qop",DataParameter, "RFC 3261");
		defineParam(resip::addTransport, "addTransport", ExistsParameter, "RESIP INTERNAL");
		#undef defineParam
};

/**************************** App Get Header ******************************************/
class AppGetHeader : public RUObject
{
	public:
		AppGetHeader();
		~AppGetHeader();
		void fnSetHeader(resip::ParserCategory * _pclsHeader);
		void fnSetHeader(const char * _pszUnknownHeader);
		AppCapUri * fnGetUri(EAppHCategory_t _eHCategory);
		void fnSetParam(EAppHCategory_t _eHCategory,
									EAppPCategory_t _ePCategory,
									resip::ParameterTypes::Type _eParam, 
									const char *_pszParam);
		void fnSetUriParam(EAppHCategory_t _eHCategory,
									EAppPCategory_t _ePCategory,
									resip::ParameterTypes::Type _eParam, 
									const char *_pszParam);
		void fnRemoveParam(EAppHCategory_t _eHCate, 
									resip::ParameterTypes::Type _eParam, 
									const char * _pszParam);
		void fnRemoveUriParam(EAppHCategory_t _eHCate, 
									resip::ParameterTypes::Type _eParam, 
									const char * _pszParam);
		void fnGetUriDisplayName(EAppHCategory_t _eHCategory,RUArrayString * _pclsResult);
		void fnAddUriDisplayName(EAppHCategory_t _eHCategory,const char * _pszV);
		void fnRemoveUriDisplayName(EAppHCategory_t _eHCategory);
		void fnGetUriUserName(EAppHCategory_t _eHCategory,RUArrayString * _pclsResult);
		void fnAddUriUserName(EAppHCategory_t _eHCategory,const char * _pszV);
		void fnRemoveUriUserName(EAppHCategory_t _eHCategory);
		void fnGetUriHost(EAppHCategory_t _eHCategory,RUArrayString * _pclsResult);
		void fnAddUriHost(EAppHCategory_t _eHCategory,const char * _pszV);
		void fnRemoveUriHost(EAppHCategory_t _eHCategory);
		void fnGetUriPort(EAppHCategory_t _eHCategory,RUArrayString * _pclsResult);
		void fnAddUriPort(EAppHCategory_t _eHCategory, const char * _pszV);
		void fnRemoveUriPort(EAppHCategory_t _eHCategory);
		void fnGetUriScheme(EAppHCategory_t _eHCategory,RUArrayString * _pclsResult);
		void fnAddUriScheme(EAppHCategory_t _eHCategory, const char * _pszV);
		void fnRemoveUriScheme(EAppHCategory_t _eHCategory);
		void fnAddParam(EAppHCategory_t _eHCate,
									resip::Headers::Type _eHeader,
									const char * _pszKey, 
									const char * _pszVal);
		void fnAddUriParam(EAppHCategory_t _eHCate,
									resip::Headers::Type _eHeader,
									const char * _pszKey, 
									const char * _pszVal);
		void fnGetViaTransport(RUArrayString * _pclsResult);
		void fnAddViaTransport(const char * _pszV);
		void fnGetViaHost(RUArrayString * _pclsResult);
		void fnAddViaHost(const char * _pszV);
		void fnGetViaPort(RUArrayString * _pclsResult);
		void fnAddViaPort(const char * _pszV);
		
		void fnDisplay(char * _pszLog, unsigned int _unSize)
		{
			fnRUStrnCat(_pszLog,_unSize,"<Param : %s, %s> ",m_pszParamK,m_pszParamV);
		}
		void fnDebug(EAppHCategory_t _eHCategory)
		{
			if(m_pszUnknownHeader) RLOGvn("Unknown Header : %s",m_pszUnknownHeader);
			else
			{
				RLOGvn("\r\n>>>>>>>>>>>> Header Param Debug");
				m_pclsHeader->fnDebug(_eHCategory);
				RLOGvn("<<<<<<<<<<<< Header Param Debug\r\n");
			}
		}
		/************* Header Typ Conversion ******************************/
		resip::Via * fnConvHeaderVia(){return (resip::Via*)m_pclsHeader; } 
		resip::UInt32Category * fnConvHeaderInt(){return (resip::UInt32Category*)m_pclsHeader; } 
		resip::NameAddr * fnConvHeaderNameAddr(){return (resip::NameAddr*)m_pclsHeader; } 
		resip::Token * fnConvHeaderToken(){return (resip::Token*)m_pclsHeader;} 
		resip::Auth  * fnConvHeaderAuth(){return (resip::Auth*)m_pclsHeader;} 
		resip::StringCategory * fnConvHeaderString(){return (resip::StringCategory*)m_pclsHeader; } 
		resip::GenericUri * fnConvHeaderGenUri(){return (resip::GenericUri*)m_pclsHeader; } 
		resip::CallId * fnConvHeaderCallId(){return (resip::CallId*)m_pclsHeader; } 
		resip::CSeqCategory * fnConvHeaderCSeq(){return (resip::CSeqCategory*)m_pclsHeader;} 
		resip::ExpiresCategory * fnConvHeaderExpires(){return (resip::ExpiresCategory*)m_pclsHeader; } 
		resip::Mime  * fnConvHeaderMime(){return (resip::Mime*)m_pclsHeader; } 
		resip::DateCategory  * fnConvHeaderDate()	{return (resip::DateCategory*)m_pclsHeader; } 
		resip::WarningCategory * fnConvHeaderWarning(){return (resip::WarningCategory*)m_pclsHeader; } 
		resip::RAckCategory * fnConvHeaderRAck()	{return (resip::RAckCategory*)m_pclsHeader; } 
		resip::PrivacyCategory  * fnConvHeaderPrivacy(){return (resip::PrivacyCategory*)m_pclsHeader; } 
		AppHeader *m_pclsHeader;         // get header temp address
		resip::Parameter *m_pclsParam;   // get param temp address
		char * m_pszUnknownHeader;    // get unknown header temp address
		resip::Data * m_pclsUnHeader;     // get unknown header Data Pointer
		AppUnknownHParser m_clsParser;  // unknown header parser
		AppCapUri  m_clsUri;                   // get uri Capsule temp address
		char * m_pszParamK;                 // get param key temp address
		char * m_pszParamV;	                 // get param value temp address
		unsigned long m_ulParamV;          // get param int value temp address
		RUBuffers m_clsBuffers;               // Tempory Buffer String
		bool m_bIsWellFormed;
		static char  Yes[3];
		static  char  No[3];
};

/*************************** App Get Headers ********************************************/
class AppGetHeaders
{
	public:
		AppGetHeaders();
		~AppGetHeaders();
		AppGetHeader * fnAdd(resip::ParserCategory * _pclsHeader);   // Known Header
		AppGetHeader * fnAdd(const char * _pszUnknownHeader);     // Unknown Header
		static void fnGetHeaderInfo(resip::SipMessage *_pclsMsg, const char * _pszName,
															char * _pszBuild,unsigned int _unSize);
		// Header 
		void fnGetHeaders(resip::SipMessage * _pclsMsg,const char * _pszH);
		void fnAddHeaders(resip::SipMessage *_pclsMsg, const char * _pszH,const char * _pszV,
																				EMsgPos_t _ePos);
		void fnRemoveHeaders(resip::SipMessage *_pclsMsg, const char * _pszH, EMsgPos_t _ePos);
		
		// Param
		void fnGetParams(resip::SipMessage * _pclsMsg,const char * _pszH, const char * _pszP);
		void fnAddParams(resip::SipMessage * _pclsMsg,const char * _pszH, const char * _pszP, 
																const char * _pszV,EMsgPos_t _ePos);
		void fnRemoveParams(resip::SipMessage *_pclsMsg, const char * _pszH, 
															const char * _pszP, EMsgPos_t _ePos);
		void fnShiftParams(resip::SipMessage * _pclsMsg,const char * _pszH, const char * _pszP, 
																				EMsgPos_t _ePos);
		
		// Uri Param
		void fnGetUriParams(resip::SipMessage * _pclsMsg,const char * _pszH, const char * _pszP);
		void fnAddUriParams(resip::SipMessage * _pclsMsg,const char * _pszH, const char * _pszP, 
																const char * _pszV,EMsgPos_t _ePos);
		void fnRemoveUriParams(resip::SipMessage *_pclsMsg, const char * _pszH, 
															const char * _pszP, EMsgPos_t _ePos);
		void fnShiftUriParams(resip::SipMessage * _pclsMsg,const char * _pszH, const char * _pszP,
																				EMsgPos_t _ePos);
		// Uri Display Name
		bool fnGetUriDisplayNames(resip::SipMessage * _pclsMsg,const char * _pszHeader, 
																		RUArrayString *_pclsResult);
		void fnAddUriDisplayNames(resip::SipMessage * _pclsMsg,const char * _pszHeader, 
														const char * _pszV,EMsgPos_t _ePos);
		// Uri User Name
		bool fnGetUriUserNames(resip::SipMessage * _pclsMsg,const char * _pszHeader, 
																		RUArrayString *_pclsResult);
		void fnAddUriUserNames(resip::SipMessage * _pclsMsg,const char * _pszHeader, 
														const char * _pszV, EMsgPos_t _ePos);
		// Uri Host
		bool fnGetUriHosts(resip::SipMessage * _pclsMsg,const char * _pszHeader, 
																		RUArrayString *_pclsResult);
		void fnAddUriHosts(resip::SipMessage * _pclsMsg,const char * _pszHeader, 
														const char * _pszV, EMsgPos_t _ePos);
		// Uri Port
		bool fnGetUriPorts(resip::SipMessage * _pclsMsg,const char * _pszHeader, 
																		RUArrayString *_pclsResult);
		void fnAddUriPorts(resip::SipMessage * _pclsMsg,const char * _pszHeader, 
														const char * _pszV, EMsgPos_t _ePos);
		// Uri Scheme
		bool fnGetUriSchemes(resip::SipMessage * _pclsMsg,const char * _pszHeader, 
																		RUArrayString *_pclsResult);
		void fnAddUriSchemes(resip::SipMessage * _pclsMsg,const char * _pszHeader, 
														const char * _pszV, EMsgPos_t _ePos);
		// Via Version    - No need
		// Via Transport
		bool fnGetViaTransports(resip::SipMessage * _pclsMsg,RUArrayString *_pclsResult);
		void fnAddViaTransports(resip::SipMessage * _pclsMsg, const char * _pszV, EMsgPos_t _ePos);
		// Via Host
		bool fnGetViaHosts(resip::SipMessage * _pclsMsg,RUArrayString *_pclsResult);
		void fnAddViaHosts(resip::SipMessage * _pclsMsg, const char * _pszV, EMsgPos_t _ePos);
		// Via Port
		bool fnGetViaPorts(resip::SipMessage * _pclsMsg,RUArrayString *_pclsResult);
		void fnAddViaPorts(resip::SipMessage * _pclsMsg, const char * _pszV, EMsgPos_t _ePos);
		
		void fnDisplay(char * _pszBuild, unsigned int _unSize);
		AppGetHeader * fnBegin(){return (AppGetHeader *)m_listHeader.fnBegin();}
		AppGetHeader * fnNext(){return (AppGetHeader *)m_listHeader.fnNext();}
		unsigned int fnGetNum(){return m_listHeader.fnGetSize();}
		char * m_pszH;
		char * m_pszP;
		bool m_bMulti;
		bool m_bUseHParam;    
		bool m_bUsePParam;
		resip::Headers::Type m_eHeader;
		resip::ParameterTypes::Type m_eParam;
		EAppHCategory_t m_eHCate;
		EAppPCategory_t m_ePCate;
		void * m_pvContainer;
		RUList m_listHeader;
		RUArrayBools m_arrBools;
		static void fnAddHeader(resip::SipMessage *_pclsMsg,const char* _pszH,const char * _pszV);
		static void fnRemoveAllHeader(resip::SipMessage *_pclsMsg,const char * _pszH);
		static void fnGetHeaderLine(resip::SipMessage *_pclsMsg, const char * _pszName,
																	RUArrayString *_pclsResultString);
		
};

/*************************** Multi Lines ********************************************/
class AppLine : public RUObject
{
	public:
		AppLine(){m_uiPos=0;m_pszValue=NULL;}
		~AppLine(){if(m_pszValue) delete [] m_pszValue;}
		unsigned int m_uiPos;
		char * m_pszValue;
		void fnSetHeader(const char * _pszName){fnSetKeyS(_pszName);}
		void fnSetValue(const char * _pszValue)
		{
			if(m_pszValue) delete [] m_pszValue; m_pszValue = fnRUCreateCopy(_pszValue);
		}
		char * fnGetHeader(){return m_pszKey;}
		char * fnGetValue(){return m_pszValue;}
		void fnDisplay(char * _pszLog, unsigned int _unSize)
		{
			fnRUStrnCat(_pszLog,_unSize,"[Pos(%d):Val(%s)] ",m_uiPos,m_pszValue);
		}
};
class AppMultiLine : public RUObject
{
	public:
		AppMultiLine(){}
		~AppMultiLine(){}
		void fnCreateMultiHeader(const char * _pszName){fnSetKeyS(_pszName);}
		void fnAddLine(const char * _pszValue, unsigned int _uiOrigPos)
		{
			AppLine * pclsNew = new AppLine;pclsNew->fnSetValue(_pszValue);
			pclsNew->m_uiPos = _uiOrigPos;m_listH.fnPushBack(pclsNew);
		}
		void fnSort(){m_listH.fnSort(fnCbk_Sort);}	
		void fnAddMsg(resip::SipMessage *_pclsMsg)
		{
			fnSort();
			AppLine * pclsLine = (AppLine *)m_listH.fnBegin();
			while(pclsLine)
			{
				fnAddHeader(_pclsMsg,m_pszKey,pclsLine->fnGetValue());
				pclsLine = (AppLine * )m_listH.fnNext();
			}
		}
		void fnRemoveMsg(resip::SipMessage *_pclsMsg)
		{
			fnSort(); RUArrayBools clsBools;
			AppLine * pclsLine = (AppLine *)m_listH.fnBegin();
			while(pclsLine)
			{
				fnAddPosition(&clsBools,pclsLine->m_uiPos);
				pclsLine = (AppLine * )m_listH.fnNext();
			}
			AppGetHeaders clsGet; clsGet.m_arrBools = clsBools;
			clsGet.fnRemoveHeaders( _pclsMsg,m_pszKey,E_MSG_POS_DEPEND_RULE);
		}
		void fnDisplay(char * _pszLog, unsigned int _unSize)
		{
			fnRUStrnCat(_pszLog,_unSize,"<<<HDR(%s)>>> ",m_pszKey);
			AppLine * pclsLine = (AppLine *)m_listH.fnBegin();
			while(pclsLine)
			{
				pclsLine->fnDisplay(_pszLog,_unSize);
				pclsLine = (AppLine * )m_listH.fnNext();
			}
			fnRUStrnCat(_pszLog,_unSize,"\r\n");
		}
		RUList m_listH;
	private:
		inline static void fnAddPosition(RUArrayBools* _pclsPos, unsigned int _uiPos)
		{
			unsigned int unMy = _pclsPos->fnGetSize(); unsigned int unOpp = _uiPos+1;
			if(unMy < unOpp)
			{
				for(unsigned int i =0;i<unOpp-unMy;i++) _pclsPos->fnAddBool(NULL,false);
			}
			(*_pclsPos)[_uiPos] = true;
		}
		inline static void fnAddHeader(resip::SipMessage *_pclsMsg,const char* _pszH,const char * _pszV)
		{
			resip::Headers::Type eHeader = resip::Headers::getType(_pszH, fnRUStrLen(_pszH));
			char * pszAlloc = fnRUCreateCopy(_pszV);_pclsMsg->addBuffer(pszAlloc);
			_pclsMsg->addHeader(eHeader,_pszH,fnRUStrLen(_pszH),pszAlloc,fnRUStrLen(pszAlloc));
		}
		inline static bool fnCbk_Sort(unsigned long _ulOb1, unsigned long _ulOb2)
		{
			AppLine * pclsH1 = (AppLine*)_ulOb1;AppLine * pclsH2 = (AppLine*)_ulOb2;
			if(pclsH1->m_uiPos < pclsH2->m_uiPos) return true;
			return false;			
		}
};
class AppLines
{
	public:
		AppLines(){m_bAdd = false;}
		~AppLines(){}
		AppMultiLine * fnCreateLine(const char * _pszName)
		{
			AppMultiLine * pclsNew = new AppMultiLine; pclsNew->fnCreateMultiHeader(_pszName);
			m_listMH.fnPushBack(pclsNew); return pclsNew;
		}
		void fnAddLine(const char * _pszKey, const char * _pszValue, unsigned int _uiOrigPos)
		{
			AppMultiLine * pclsFind = fnFindLine(_pszKey);
			if(pclsFind == NULL) pclsFind = fnCreateLine(_pszKey);
			pclsFind->fnAddLine(_pszValue,_uiOrigPos);
		}
		void fnAddMsg(resip::SipMessage *_pclsMsg)
		{
			m_bAdd = true;
			AppMultiLine * pclsLine = (AppMultiLine*)m_listMH.fnBegin();
			while(pclsLine)
			{
				pclsLine->fnAddMsg(_pclsMsg);
				pclsLine = (AppMultiLine *)m_listMH.fnNext();
			}
		}
		void fnRemoveMsg(resip::SipMessage *_pclsMsg)
		{
			AppMultiLine * pclsLine = (AppMultiLine*)m_listMH.fnBegin();
			while(pclsLine)
			{
				pclsLine->fnRemoveMsg(_pclsMsg);
				pclsLine = (AppMultiLine *)m_listMH.fnNext();
			}
		}
		void fnDisplay(char * _pszLog, unsigned int _unSize)
		{
			fnRUStrnCat(_pszLog,_unSize,"<<<%s Screen Multi Lines>>>\r\n",
												TRUE_SELECT(m_bAdd==true,"Added","Removed"));
			AppMultiLine * pclsLine = (AppMultiLine*)m_listMH.fnBegin();
			while(pclsLine)
			{
				pclsLine->fnDisplay(_pszLog,_unSize);
				pclsLine = (AppMultiLine *)m_listMH.fnNext();
			}
		}
		bool m_bAdd;
		RUList m_listMH;		
	private:
		AppMultiLine * fnFindLine(const char * _pszKey)
		{
			RuListItem_t clsIter;
			AppMultiLine * pclsLine = (AppMultiLine*)m_listMH.fnBegin(clsIter);
			while(pclsLine)
			{
				if(fnRUStrCaseCmp(pclsLine->m_pszKey,_pszKey)==0)
				{
					return pclsLine;
				}
				pclsLine = (AppMultiLine *)m_listMH.fnNext(clsIter);
			}
			return NULL;
		}
};
#endif

