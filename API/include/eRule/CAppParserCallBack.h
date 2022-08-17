#ifndef ESIP_CAPPPARSERCALLBACK_HXX
#define ESIP_CAPPPARSERCALLBACK_HXX


#include "CAppParser.h"

#define GETH(A)     fnCbkGetH_##A            // Get Header  Function
#define GETP(A)      fnCbkGetP_##A           // Get Param Function
#define GETPG(A)      fnCbkGetPG_##A       // Gen Param Function
#define GETHC(A)    fnCbkGetHC_##A         // Get Header Category Function
#define DELHC(A)     fnCbkDelHC_##A         // Del Header  Category Function
#define GETPC(A)    fnCbkGetPC_##A          // Get Param Category Function
#define GETPP(A)    fnCbkGetPP_##A          // Get Param Param Function

typedef void *  (*PfuncGetHeader)(resip::SipMessage*,AppGetHeaders *);
typedef void  (*PfuncRemoveHeader)(resip::SipMessage*,AppGetHeaders *,EMsgPos_t);
typedef resip::Parameter * (*PfuncGetParam)(AppHeader *,resip::ParameterTypes::Type);
typedef void (*PfuncGenParam)(AppHeader *,resip::ParameterTypes::Type,const char *);
typedef void(*PfuncGetParamParam)(resip::Parameter *, char **, char **,unsigned long *);
/********************** Header List & Param List ***********************/
typedef struct AppH_t
{
	resip::Headers::Type m_eHeader;
	const char * m_pszName;
	EAppHCategory_t m_eCate;
	const char * m_pszDesc;
	PfuncGetHeader m_pfnGetHeader;
}AppH_t;
typedef struct AppP_t
{
	resip::ParameterTypes::Type m_eParam;
	const char * m_pszName;
	EAppPCategory_t m_eParamCate;
	const char * m_pszDesc;
	PfuncGetParam m_pfnGetParam;
	PfuncGenParam m_pfnGenParam;
}AppP_t;
/********************** Header Category & Param Category **************/
typedef struct AppHCate_t
{
	EAppHCategory_t m_eCate;
	const char * m_pszName;
	PfuncRemoveHeader m_pfnRemove;
	bool m_arrUseP[resip::ParameterTypes::MAX_PARAMETER+1];
}AppHCate_t;
typedef struct AppPCate_t
{
	EAppPCategory_t m_eParamCate;
	const char * m_pszName;
	PfuncGetParamParam m_pfnParamParam;
	bool m_arrUseP[resip::ParameterTypes::MAX_PARAMETER+1];
}AppPCate_t;

extern AppH_t g_arrH[];
extern AppP_t g_arrP[];
extern AppHCate_t g_arrHCate[];
extern AppPCate_t g_arrPCate [];

/****************************** Get Header  *******************************************/
void * fnCbkGetH_Via( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_MaxForwards( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Route( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_RecordRoute( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Path( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ServiceRoute( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ProxyRequire( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ProxyAuthenticate( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Identity( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_IdentityInfo( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Require( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Contact( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_To( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_From( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_CallID( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_CSeq( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Subject( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Expires( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_SessionExpires( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_MinSE( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Accept( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_AcceptEncoding( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_AcceptLanguage( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_AlertInfo( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Allow( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_AuthenticationInfo( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_CallInfo( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ContentDisposition( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ContentEncoding( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ContentId( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ContentLanguage( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ContentTransferEncoding( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ContentType( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Date( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ErrorInfo( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_InReplyTo( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_MinExpires( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_MIMEVersion( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Organization( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Priority( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ProxyAuthorization( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ReplyTo( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_RetryAfter( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_FlowTimer( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Server( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_SIPETag( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_SIPIfMatch( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Supported( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Timestamp( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Unsupported( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_UserAgent( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Warning( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_WWWAuthenticate( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_SubscriptionState( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ReferTo( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ReferredBy( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Authorization( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Replaces( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Event( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_AllowEvents( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_SecurityClient( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_SecurityServer( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_SecurityVerify( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_RSeq( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_RAck( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Reason( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Privacy( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_RequestDisposition( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_PMediaAuthorization( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_Join( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_TargetDialog( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_PAssertedIdentity( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_PPreferredIdentity( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_AcceptContact( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_RejectContact( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_PCalledPartyId( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_PAssociatedUri( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ContentLength( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_ReferSub( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_AnswerMode( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_PrivAnswerMode( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_RemotePartyId( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_HistoryInfo( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_RESIP_DO_NOT_USE( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH);
void * fnCbkGetH_MAX_HEADERS( resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsH); 

/***************************** Del Header Category *************************************/
void fnCbkDelHC_E_APP_H_CATE_NONE(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_VIA(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_INT32(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_NAMEADDR(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_TOKEN(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_AUTH(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_STRING(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_GENURI(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_CALLID(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_CSEQ(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_EXPIRES(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_MIME(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_DATE(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_WARNING(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_RACK(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_PRIVACY(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);
void fnCbkDelHC_E_APP_H_CATE_URI(resip::SipMessage * _pclsMsg, AppGetHeaders * _pclsHs, EMsgPos_t _ePos);

/***************************** Get Param *******************************************/
resip::Parameter * fnCbkGetP_data(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_control(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_mobility(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_description(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_events(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_priority(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_methods(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_schemes(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_application(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_video(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_language(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_type(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_isFocus(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_actor(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_text(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_cause(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_extensions(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_Instance(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_regid(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_ob(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_pubGruu(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_tempGruu(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_gr(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_accessType(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_algorithm(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_boundary(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_branch(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_charset(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_cnonce(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_comp(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_dAlg(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_dQop(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_dVer(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_directory(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_domain(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_duration(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_expiration(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_expires(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_filename(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_fromTag(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam); 
resip::Parameter * fnCbkGetP_handling(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_id(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_lr(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_maddr(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_method(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_micalg(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_mode(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_name(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_nc(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_nonce(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_opaque(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_permission(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_protocol(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_purpose(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_q(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_realm(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_reason(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_received(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_require(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_response(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_retryAfter(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_rinstance(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_rport(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_server(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_site(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_size(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_smimeType(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_stale(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_tag(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_toTag(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_transport(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_ttl(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_uri(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_user(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_extension(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_username(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_earlyOnly(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_refresher(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_profileType(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_vendor(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_model(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_version(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_effectiveBy(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_document(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_appId(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_networkUser(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_url(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_sigcompId(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_qop(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_index(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_rc(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_mp(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_np(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_qopOptions(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_addTransport(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);
resip::Parameter * fnCbkGetP_MAX_PARAMETER(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam);

/***************************** Gen Param *******************************************/
void fnCbkGetPG_data(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_control(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_mobility(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_description(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_events(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_priority(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_methods(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_schemes(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_application(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_video(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_language(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_type(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_isFocus(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_actor(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_text(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_cause(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_extensions(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_Instance(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_regid(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_ob(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_pubGruu(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_tempGruu(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_gr(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_accessType(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_algorithm(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_boundary(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_branch(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_charset(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_cnonce(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_comp(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_dAlg(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_dQop(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_dVer(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_directory(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_domain(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_duration(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_expiration(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_expires(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_filename(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_fromTag(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal); 
void fnCbkGetPG_handling(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_id(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_lr(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_maddr(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_method(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_micalg(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_mode(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_name(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_nc(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_nonce(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_opaque(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_permission(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_protocol(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_purpose(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_q(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_realm(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_reason(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_received(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_require(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_response(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_retryAfter(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_rinstance(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_rport(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_server(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_site(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_size(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_smimeType(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_stale(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_tag(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_toTag(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_transport(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_ttl(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_uri(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_user(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_extension(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_username(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_earlyOnly(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_refresher(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_profileType(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_vendor(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_model(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_version(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_effectiveBy(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_document(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_appId(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_networkUser(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_url(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_sigcompId(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_qop(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_index(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_rc(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_mp(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_np(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_qopOptions(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_addTransport(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);
void fnCbkGetPG_MAX_PARAMETER(AppHeader * _pclsCate, resip::ParameterTypes::Type _eParam, const char * _pszVal);


/**************************** Get Param Param **************************************/
void fnCbkGetPP_E_APP_P_CATE_NONE(resip::Parameter * _pclsP, char **_ppszK, 
																char ** _ppszV,unsigned long * _plV);
void fnCbkGetPP_E_APP_P_CATE_EXIST(resip::Parameter * _pclsP, char **_ppszK, 
																char ** _ppszV,unsigned long * _plV);
void fnCbkGetPP_E_APP_P_CATE_QUOTED(resip::Parameter * _pclsP, char **_ppszK, 
																char ** _ppszV,unsigned long * _plV);
void fnCbkGetPP_E_APP_P_CATE_EXISTOR(resip::Parameter * _pclsP, char **_ppszK, 
																char ** _ppszV,unsigned long * _plV);
void fnCbkGetPP_E_APP_P_CATE_INT32(resip::Parameter * _pclsP, char **_ppszK, 
																char ** _ppszV,unsigned long * _plV);
void fnCbkGetPP_E_APP_P_CATE_DATA(resip::Parameter * _pclsP, char **_ppszK, 
																char ** _ppszV,unsigned long * _plV);
void fnCbkGetPP_E_APP_P_CATE_BRANCH(resip::Parameter * _pclsP, char **_ppszK, 
																char ** _ppszV,unsigned long * _plV);
void fnCbkGetPP_E_APP_P_CATE_QVALUE(resip::Parameter * _pclsP, char **_ppszK, 
																char ** _ppszV,unsigned long * _plV);
void fnCbkGetPP_E_APP_P_CATE_RPORT(resip::Parameter * _pclsP, char **_ppszK, 
																char ** _ppszV,unsigned long * _plV);


#endif

