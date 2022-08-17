#ifndef _PROCPDE_H
#define _PROCPDE_H
#include "CAppSession.h"
#include "main.h"
#include "IBCDefine.h"

typedef enum
{
	E_IBC_CHARGE_METHOD_INVITE = 0,
	E_IBC_CHARGE_METHOD_200OK,
	E_IBC_CHARGE_METHOD_RE_INVITE,
	E_IBC_CHARGE_METHOD_RE_200OK,
	E_IBC_CHARGE_METHOD_RE_XXX,
}EChargingMethod_t;
bool g_fnDecisionChargingServiceType(StackStateCbInfo_t & _rclsInfo,EChargingMethod_t _eT);
void g_fnStoreChargingCallNumber(StackStateCbInfo_t & _rclsInfo);
unsigned int fnE_PDE_EV_NONE();
unsigned int fnSndE_PDE_EV_START_REQ(StackStateCbInfo_t & _rclsInfo);
unsigned int fnSndE_PDE_EV_STOP_REQ_FC(StackStateCbInfo_t & _rclsInfo);
eSIP::EAsyncCbResultType_t fnRcvE_PDE_EV_START_RSP(StackAsyncCbInfo_t & _rclsInfo);
eSIP::EAsyncCbResultType_t fnRcvE_PDE_EV_START_TO(StackAsyncCbInfo_t & _rclsInfo);
unsigned int fnSndE_PDE_EV_INTERIM_REQ(StackStateCbInfo_t & _rclsInfo);
eSIP::EAsyncCbResultType_t fnRcvE_PDE_EV_INTERIM_RSP(StackAsyncCbInfo_t & _rclsInfo);
eSIP::EAsyncCbResultType_t fnRcvE_PDE_EV_INTERIM_TO(StackAsyncCbInfo_t & _rclsInfo);
unsigned int fnSndE_PDE_EV_STOP_REQ(StackStateCbInfo_t & _rclsInfo);
eSIP::EAsyncCbResultType_t fnRcvE_PDE_EV_STOP_RSP(StackAsyncCbInfo_t & _rclsInfo);
eSIP::EAsyncCbResultType_t fnRcvE_PDE_EV_STOP_TO(StackAsyncCbInfo_t & _rclsInfo);
eSIP::EAsyncCbResultType_t fnRcvE_PDE_EV_REFRESH_TO(StackAsyncCbInfo_t & _rclsInfo);
eSIP::EAsyncCbResultType_t fnSndE_PDE_EV_REFRESH_REQ(eDum::Session * _pclsSession);	
#ifdef DEF_PDE_RETRANS
eSIP::EAsyncCbResultType_t fnRcvE_PDE_EV_START_RE(StackAsyncCbInfo_t & _rclsInfo);
eSIP::EAsyncCbResultType_t fnRcvE_PDE_EV_INTERIM_RE(StackAsyncCbInfo_t & _rclsInfo);
eSIP::EAsyncCbResultType_t fnRcvE_PDE_EV_REFRESH_RE(StackAsyncCbInfo_t & _rclsInfo);
eSIP::EAsyncCbResultType_t fnRcvE_PDE_EV_STOP_RE(StackAsyncCbInfo_t & _rclsInfo);
#endif
#endif

