
#ifndef COM_AS_ERR_HXX
#define COM_AS_ERR_HXX

#include "eRte/IBCErrorDef.hxx"

namespace ComAs
{

typedef enum
{
	E_COM_AS_ERR_none = 0,
	E_COM_AS_ERR_rule_fail_decide_routeOp,
	E_COM_AS_ERR_extAs_route_unAvailable,
	E_COM_AS_ERR_extAs_fail_decide_route,
	E_COM_AS_ERR_extAs_fail_find_route,
	E_COM_AS_ERR_extAs_fail_send,
	E_COM_AS_ERR_extAs_fail_encoding,
	E_COM_AS_ERR_extAs_fail_decoding,
	E_COM_AS_ERR_internal_err,

	E_COM_AS_ERR_NUM,
}EComAsErrType;

const char * g_fnGetComAsErrStr(int _n);
int g_fnConvertErrReason4SessCdr(EComAsErrType _e);
int g_fnConvertExtAsErrReason4SessCdr(unsigned int _un);
int g_fnConvertErrReason4FileCdrFromSessCdr(int _n);
int g_fnConvertErrReason4SipHdr(EComAsErrType _e);
bool g_fnIsComAsErr(int _nSessReason);

}

#endif

