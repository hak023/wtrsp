
#ifndef COM_AS_SIP_HELPER_HXX
#define COM_AS_SIP_HELPER_HXX

#include <list>
#include <string>

#include "resip/stack/SipMessage.hxx"

namespace ComAs
{

struct ValuePerHdr
{
	unsigned int m_unHdrIdx;	//_unHdrIdx : 0(top) ~
	std::string m_clsValue;
};
typedef std::list< ValuePerHdr > list_ValuePerHdr;

typedef enum
{
	E_SIP_URI_reqUri = 0,
	E_SIP_URI_from,
	E_SIP_URI_to,
	E_SIP_URI_contact,

	E_SIP_URI_NUM,
}ESipUriHdr;

bool g_fnTranslateUser(const char * _pszSrcUser, const char * _pszPhoneCtx, std::string & _rclsDstUser);
void g_fnConvTelUri(ESipUriHdr eUriHdr, resip::SipMessage & _rclsMsg);

unsigned int g_fnGetSipHdrNum(const resip::SipMessage & _rclsSipMsg, const char * _pszHdrName);
/*==== Get for Req-URI ====*/
bool g_fnGetReqUri(const resip::SipMessage & _rclsSipMsg, std::string & _rclsReqUriValue);
bool g_fnGetReqUriUser(const resip::SipMessage & _rclsSipMsg, std::string & _rclsUsername);
bool g_fnGetReqUriHost(const resip::SipMessage & _rclsSipMsg, std::string & _rclsHost);
bool g_fnGetReqUriParamValue(const resip::SipMessage & _rclsSipMsg, const char * _pszParamName, std::string & _rclsParamValue);
/*==== Get for Header ====*/
bool g_fnGetAllSipHdrValue(const resip::SipMessage & _rclsSipMsg, const char * _pszHdrName, list_ValuePerHdr & _rlistValuePerHdr);
bool g_fnGetSipHdrValue(const resip::SipMessage & _rclsSipMsg, const char * _pszHdrName, unsigned int _unHdrIdx, std::string & _rclsHdrValue);
bool g_fnGetAllSipUser(const resip::SipMessage & _rclsSipMsg, const char * _pszHdrName, list_ValuePerHdr & _rlistValuePerHdr);
bool g_fnGetSipUser(const resip::SipMessage & _rclsSipMsg, const char * _pszHdrName, unsigned int _unHdrIdx, std::string & _rclsUsername);
bool g_fnGetAllSipHost(const resip::SipMessage & _rclsSipMsg, const char * _pszHdrName, list_ValuePerHdr & _rlistValuePerHdr);
bool g_fnGetSipHost(const resip::SipMessage & _rclsSipMsg, const char * _pszHdrName, unsigned int _unHdrIdx, std::string & _rclsHost);
//only used for known header
bool g_fnGetAllSipParamValue(const resip::SipMessage & _rclsSipMsg, const char * _pszHdrName, const char * _pszParamName, list_ValuePerHdr & _rlistValuePerHdr);
bool g_fnGetSipParamValue(const resip::SipMessage & _rclsSipMsg, const char * _pszHdrName, unsigned int _unHdrIdx, const char * _pszParamName, std::string & _rclsParamValue);

/*==== Set for Req-URI ====*/
bool g_fnSetReqUri(const char * _pszReqUriValue, resip::SipMessage & _rclsSipMsg);
bool g_fnSetReqUriUser(const char * _pszUsername, resip::SipMessage & _rclsSipMsg);
bool g_fnSetReqUriHost(const char * _pszHost, resip::SipMessage & _rclsSipMsg);
bool g_fnAddReqUriParam(const char * _pszParamName, const char * _pszParamValue, resip::SipMessage & _rclsSipMsg);
bool g_fnSetReqUriParamValue(const char * _pszParamName, const char * _pszParamValue, resip::SipMessage & _rclsSipMsg);
/*==== Set for Header ====*/
//_nHdrIdx : 0(front), -1(back)
bool g_fnAddSipHdr(const char * _pszHdrName, int _nHdrIdx, const char * _pszHdrValue, resip::SipMessage & _rclsSipMsg);
bool g_fnSetSipHdrValue(const char * _pszHdrName, unsigned int _unHdrIdx, const char * _pszHdrValue, resip::SipMessage & _rclsSipMsg);
bool g_fnSetSipUser(const char * _pszHdrName, unsigned int _unHdrIdx, const char * _pszUsername, resip::SipMessage & _rclsSipMsg);
bool g_fnSetSipHost(const char * _pszHdrName, unsigned int _unHdrIdx, const char * _pszHost, resip::SipMessage & _rclsSipMsg);
//only used for known header
bool g_fnAddSipParam(const char * _pszHdrName, unsigned int _unHdrIdx, const char * _pszParamName, const char * _pszParamValue, resip::SipMessage & _rclsSipMsg);
bool g_fnSetSipParamValue(const char * _pszHdrName, unsigned int _unHdrIdx, const char * _pszParamName, const char * _pszParamValue, resip::SipMessage & _rclsSipMsg);

/*==== Del for Req-URI ====*/
void g_fnDelReqUriUser(resip::SipMessage & _rclsSipMsg);
void g_fnDelReqUriHost(resip::SipMessage & _rclsSipMsg);
void g_fnDelReqUriParam(const char * _pszParamName, resip::SipMessage & _rclsSipMsg);
/*==== Del for Header ====*/
void g_fnDelAllSipHdr(const char * _pszHdrName, resip::SipMessage & _rclsSipMsg);
void g_fnDelSipHdr(const char * _pszHdrName, unsigned int _unHdrIdx, resip::SipMessage & _rclsSipMsg);
void g_fnDelAllSipUser(const char * _pszHdrName, resip::SipMessage & _rclsSipMsg);
void g_fnDelSipUser(const char * _pszHdrName, unsigned int _unHdrIdx, resip::SipMessage & _rclsSipMsg);
void g_fnDelAllSipHost(const char * _pszHdrName, resip::SipMessage & _rclsSipMsg);
void g_fnDelSipHost(const char * _pszHdrName, unsigned int _unHdrIdx, resip::SipMessage & _rclsSipMsg);
//only used for known header
void g_fnDelAllSipParam(const char * _pszHdrName, const char * _pszParamName, resip::SipMessage & _rclsSipMsg);
void g_fnDelSipParam(const char * _pszHdrName, unsigned int _unHdrIdx, const char * _pszParamName, resip::SipMessage & _rclsSipMsg);


}

#endif

