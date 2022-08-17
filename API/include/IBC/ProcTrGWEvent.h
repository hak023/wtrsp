#ifndef _PROCTRGWEVENT_H
#define _PROCTRGWEVENT_H
#include "main.h"
#include "CAppSession.h"
#include "IBCDefine.h"

eSIP::EStateCbResultType_t g_fnProcTrGWFail(StackResumCbInfo_t  * _pclsResumeInfo,
														StackAsyncCbInfo_t * _pclsAsyncInfo,
														TrGWResource_t * _pstMsg,
														const char * _pszTrGWReason,
														int _nReasonCode);     
eSIP::EStateCbResultType_t g_fnRcvTrGWResumeEvent(StackResumCbInfo_t & _rclsInfo);            // sync
eSIP::EStateCbResultType_t  g_fnSndE_MEDIA_ADD_REQ(StackStateCbInfo_t & _rclsInfo);            // state cb
eSIP::EStateCbResultType_t g_fnRcvE_MEDIA_ADD_RSP(StackResumCbInfo_t & _rclsInfo);           // sync
eSIP::EAsyncCbResultType_t g_fnRcvE_MEDIA_ADD_REQ_TO(StackAsyncCbInfo_t & _rclsInfo);   // async
eSIP::EStateCbResultType_t  g_fnSndE_MEDIA_MOD_REQ(StackStateCbInfo_t & _rclsInfo);           // state cb
eSIP::EStateCbResultType_t g_fnRcvE_MEDIA_MOD_RSP(StackResumCbInfo_t & _rclsInfo);           // sync
eSIP::EAsyncCbResultType_t g_fnRcvE_MEDIA_MOD_REQ_TO(StackAsyncCbInfo_t & _rclsInfo);   // async
eSIP::EStateCbResultType_t  g_fnSndE_MEDIA_SDP_REQ(StackStateCbInfo_t & _rclsInfo);              // state cb
eSIP::EStateCbResultType_t g_fnRcvE_MEDIA_SDP_RSP(StackResumCbInfo_t & _rclsInfo);            // sync
eSIP::EAsyncCbResultType_t g_fnRcvE_MEDIA_SDP_REQ_TO(StackAsyncCbInfo_t & _rclsInfo);    // async
int  g_fnSndE_MEDIA_DEL_REQ(StackStateCbInfo_t & _rclsInfo);              // async
eSIP::EAsyncCbResultType_t g_fnRcvE_MEDIA_DEL_REQ(StackAsyncCbInfo_t & _rclsInfo);             // async
eSIP::EAsyncCbResultType_t g_fnRcvE_MEDIA_DEL_RSP(StackAsyncCbInfo_t & _rclsInfo);              // async
eSIP::EAsyncCbResultType_t g_fnRcvE_MEDIA_DEL_REQ_TO(StackAsyncCbInfo_t & _rclsInfo);     // async
eSIP::EAsyncCbResultType_t g_fnRcvE_MEDIA_ARM_IND_REQ(StackAsyncCbInfo_t & _rclsInfo);    // async
#ifdef DEF_TRGW_RETRANS
eSIP::EAsyncCbResultType_t g_fnRcvE_MEDIA_ADD_REQ_RE(StackAsyncCbInfo_t & _rclsInfo);   // async
eSIP::EAsyncCbResultType_t g_fnRcvE_MEDIA_MOD_REQ_RE(StackAsyncCbInfo_t & _rclsInfo);   // async
eSIP::EAsyncCbResultType_t g_fnRcvE_MEDIA_DEL_REQ_RE(StackAsyncCbInfo_t & _rclsInfo);   // async
#endif

#endif
