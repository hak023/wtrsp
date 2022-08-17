/**
 * @file   eErr2RteErr.h
 * the convert cm error to rtelib errror
 *
 * @attention Copyright (c) 2014 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2014.11.12   hc_yun   1.0     first written
 */

#ifndef __E_CMERROR2_RTELIBE_ERR_M__
#define __E_CMERROR2_RTELIBE_ERR_M__

#include "eRte/eSipRte.hxx"
#include "eRte/IBCErrorDef.hxx"
#include "eRte/eSipRteErr.hxx"
/*
struct SipReasonCode_t
      E_SYSTEM_OVERLOAD_CAUSE         = 0,
      E_CPS_OVERLOAD_CAUSE            = 1,
      E_CPU_OVERLOAD_CAUSE            = 2,
      E_MEM_OVERLOAD_CAUSE            = 3,
      E_UNKNOWN_ROUTE_CAUSE           = 4,
      E_ROUTING_FAIL_CAUSE            = 5,
      E_INTERNAL_ERR_CAUSE            = 6,
      E_INCORRECT_MSG_CAUSE           = 7,
      E_FILTER_DENY_CAUSE             = 8,
      E_SESSION_NOT_EXIST_CAUSE       = 9,
      E_TIMEOUT_ERR_CAUSE             =10,
      E_MP_NOTCONT_CAUSE              =11,
      E_TRGW_NEGO_FAIL_CAUSE          =12,
      E_TRGW_NO_RTP_FAIL_CAUSE        =13,
      E_TRGW_INSUFFICIENT_RSC_CAUSE   =14,
      E_TRGW_SDP_INCORRECT_CAUSE      =15,
      E_TRGW_INCORRECT_CAUSE          =16,
      E_ROUTE_STS_DOWN_CAUSE          =17,
      E_CAUSE_MAX                     =18,
*/

inline const int fnRteError2CMError(int _nCode)
{
int nLevel=(_nCode)&0xffffff00;
//IfESipRteApi_GetRouteByTransport
//IfESipRteApi_CheckTPSByRouteID
//IfESipRteApi_IsNormalSysLoad
//IfESipRteApi_DecisionTarget???
if( nLevel== E_RTE_FailGetICRoute_NotFoundLocalNode){
   switch(_nCode)
   {
   case E_RTE_FailGetICRoute_UnavalibleRoute:
   case E_RTE_FailGetOGRoute_UnavalibleRoute:
   return E_IBC_ERROR_CM_STATUS_DOWN; 
   case E_RTE_FailGetICRoute_OverLoadGroupCPS:
   case E_RTE_FailGetICRoute_OverLoadGlobalCPS:
   case E_RTE_FailGetICRoute_OverLoadGroupTPS:
   case E_RTE_FailGetICRoute_OverLoadGlobalTPS:
   case E_RTE_FailGetICRoute_OverLoadEmergency:
   case E_RTE_FailGetICRoute_OverLoadAudio:
   case E_RTE_FailGetICRoute_OverLoadVideo:
   return E_IBC_ERROR_CM_OVER_CPS;

   case E_RTE_FailGetICRoute_OverLoadCPU:
   return E_IBC_ERROR_CM_OVER_CPU;

   case E_RTE_FailGetICRoute_OverLoadMEM:
   return E_IBC_ERROR_CM_OVER_MEM;
   }
   return E_IBC_ERROR_CM_ROUTING_FAIL;
//IfESipRteApi_FindRemote
}else if ( nLevel== E_RTE_CheckRemote_InvalidID ){
   return E_IBC_ERROR_CM_TARGET_FAIL;
//IfESipRteApi_FindRoute
}else if ( nLevel== E_RTE_CheckRoute_InvalidID ){
   return E_IBC_ERROR_CM_UNKNOWN_ROUTE;
//IfESipRteApi_DecisionTrGW
}else if ( nLevel== E_RTE_CheckTrGW_InvalidID ){
   switch(_nCode){
   case E_RTE_FailGetTrGW_Unavailable_Status:
   return E_IBC_ERROR_CM_NOT_CONNECTED_EMP;
   }
}

return E_IBC_ERROR_CM_INTERNAL_ERROR;
}

inline const int fnCMError2RteError(int _nCode)    
{                                                                 
int nETCError=0;
switch(_nCode)                                                       
{
case E_IBC_ERROR_CM_NONE: return 0; //normal
case E_IBC_ERROR_CM_OVER_SYSTEM: return SipReasonCode_t::E_SYSTEM_OVERLOAD_CAUSE; // 시스템 과부하
case E_IBC_ERROR_CM_TARGET_FAIL: return SipReasonCode_t::E_ROUTING_FAIL_CAUSE; // 수신된 Target이 옳바르지 않음
case E_IBC_ERROR_CM_INVALID_TARGET: return SipReasonCode_t::E_UNKNOWN_ROUTE_CAUSE;// 송신될 Target이 옳바르지 않음
case E_IBC_ERROR_CM_ROUTING_FAIL: return SipReasonCode_t::E_ROUTING_FAIL_CAUSE;// Routing 실패
case E_IBC_ERROR_CM_TIMEOUT_SESSION: return SipReasonCode_t::E_TIMEOUT_ERR_CAUSE; //Session timeout
case E_IBC_ERROR_CM_NOT_CONNECTED_EMP: return SipReasonCode_t::E_MP_NOTCONT_CAUSE; //eMP과 연동되지 않음.
case E_IBC_ERROR_CM_SEND_FAIL_EMP: return SipReasonCode_t::E_MP_NOTCONT_CAUSE; // eMP로 전송 실패
case E_IBC_ERROR_CM_DECISION_EMP_FAIL: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE; //  eMP간 Trasaction 실패
case E_IBC_ERROR_CM_TIMEOUTE_EMP: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE; //  eMP에 대한 응답 실패
case E_IBC_ERROR_CM_INTERNAL_ERROR: return SipReasonCode_t::E_INTERNAL_ERR_CAUSE; //  내부 에러발생시
case E_IBC_ERROR_CM_MSG_INVALID: return SipReasonCode_t::E_INCORRECT_MSG_CAUSE; //  잘못된 메시지
case E_IBC_ERROR_CM_STATE_ERROR: return SipReasonCode_t::E_INCORRECT_MSG_CAUSE;//   메시지 State 처리시 에러
case E_IBC_ERROR_CM_OVER_SYSTEM_: return SipReasonCode_t::E_SYSTEM_OVERLOAD_CAUSE; //  시스템 과부하, SYSTEM_OVERLOAD
case E_IBC_ERROR_CM_OVER_CPS: return SipReasonCode_t::E_CPS_OVERLOAD_CAUSE; //   CPS 과부하 제어
case E_IBC_ERROR_CM_OVER_CPU: return SipReasonCode_t::E_CPU_OVERLOAD_CAUSE; //  CPU 과부하 제어
case E_IBC_ERROR_CM_OVER_MEM: return SipReasonCode_t::E_MEM_OVERLOAD_CAUSE; // MEM 과부하 제어
case E_IBC_ERROR_CM_UNKNOWN_ROUTE: return SipReasonCode_t::E_UNKNOWN_ROUTE_CAUSE;//  알 수 없는 Route
case E_IBC_ERROR_CM_ROUTING_FAIL_: return SipReasonCode_t::E_ROUTING_FAIL_CAUSE; //  Routing 실패
case E_IBC_ERROR_CM_INTERNAL_ERROR_: return SipReasonCode_t::E_INTERNAL_ERR_CAUSE; //  내부 에러발생시
case E_IBC_ERROR_CM_MSG_INVALID_: return SipReasonCode_t::E_INCORRECT_MSG_CAUSE; // 잘못된 메시지
case E_IBC_ERROR_CM_FILTER: return SipReasonCode_t::E_FILTER_DENY_CAUSE; //   필터 거부
case E_IBC_ERROR_CM_NOT_FOUND_SESSION: return SipReasonCode_t::E_SESSION_NOT_EXIST_CAUSE; //  세션을 찾을수 없음
case E_IBC_ERROR_CM_NOT_CONNECTED_EMP_: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE; //  eMP와 연동 장애
case E_IBC_ERROR_CM_OVER_EMP: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//  eMP의 CPU/MEM 장애
case E_IBC_ERROR_CM_EMP_INTERNAL_ERROR: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE; // eMP 내부 에러 발생시
case E_IBC_ERROR_CM_STATUS_DOWN: return SipReasonCode_t::E_ROUTE_STS_DOWN_CAUSE; //  송신될 Target이 다운됨
/**EMP**/
case E_IBC_ERROR_EMP_MAKE_SDP_FAIL: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;// SDP make fail
case E_IBC_ERROR_EMP_NOT_FOUND_SESSION: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE; // Session 을 찾지 못함
case E_IBC_ERROR_EMP_NOT_USE: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE; // Session 을 찾지 못함
case E_IBC_ERROR_EMP_FULL_SESSION: return SipReasonCode_t::E_TRGW_INSUFFICIENT_RSC_CAUSE; // 내부 세션을 모두 사용
case E_IBC_ERROR_EMP_INTERNAL_ERROR: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//  Internal Error
case E_IBC_ERROR_EMP_NEGO_FAIL: return SipReasonCode_t::E_TRGW_NEGO_FAIL_CAUSE; //  네고 실패
case E_IBC_ERROR_EMP_NOT_FOUND_AUDIO: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//  audio 정보 찾지 못함
case E_IBC_ERROR_EMP_NOT_FOUND_VIDEO: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;// video 정보 찾지 못함
case E_IBC_ERROR_EMP_NOT_FOUND_CODEC: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//  코덱 정보 찾지 못함
case E_IBC_ERROR_EMP_GARBAGE_SESSION: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//  가비지 콜렉터에 의한 세션 정리
case E_IBC_ERROR_EMP_NOT_USE_: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE; //  사용중인 세션
case E_IBC_ERROR_EMP_USING_SESSION: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//  SDP 파싱 에러
case E_IBC_ERROR_EMP_PARSE_ERROR: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//  SDP 파싱 에러
case E_IBC_ERROR_EMP_NOT_FOUND_MSRP: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//   MSRP 찾지 못함
case E_IBC_ERROR_EMP_NET_FAIL_KILL: return SipReasonCode_t::E_TRGW_NO_RTP_FAIL_CAUSE; // no rtp
case E_IBC_ERROR_EMP_RETRANS_ERROR: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//  Retrans 로직 에러
case E_IBC_ERROR_EMP_CONTENTS_ERROR: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//   Content Type이 SDP 가 아닌 경우
case E_IBC_ERROR_EMP_TGA_CONNECTION_FAIL: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE; //   TGA와 커넥션 Fail 
case E_IBC_ERROR_EMP_MSRP_CONNECTION_FAIL: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE; //  MSRPA와 커넥션 Fail
case E_IBC_ERROR_EMP_SDP_SYNTEX_ERROR: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE; //   SDP Syntax Error
case E_IBC_ERROR_EMP_NOT_SUPPORT_EARLY: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE; //   early session not support
case E_IBC_ERROR_EMP_NOT_USE__: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//   ADD REQ timeout 시
case E_IBC_ERROR_EMP_ADD_TIMEOUT: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//   ADD REQ timeout 시
case E_IBC_ERROR_EMP_MOD_TIMEOUT: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//   MOD REQ timeout 시
case E_IBC_ERROR_EMP_DEL_TIMEOUT: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//   DEL REQ timeout 시
case E_IBC_ERROR_EMP_NOT_USE___: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//   ADD RSP 이후 후속 메시지 대기 timeout 시
case E_IBC_ERROR_EMP_ADD_RSP_TIMEOUT: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//   Offer 이후 Answer 메시지 대기 timeout 시
case E_IBC_ERROR_EMP_ANSWER_TIMEOUT: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//   ADD RSP 이후 후속 메시지 대기 timeout 시
case E_IBC_ERROR_EMP_NOT_FOUND_PT_GROUP: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//  PT Group 못찾음
case E_IBC_ERROR_EMP_NOT_FOUND_CODEC_LIST: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//   Codec List 못찾음
case E_IBC_ERROR_EMP_USING_PT_LIST: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//   PT List가 이미 존재함
case E_IBC_ERROR_EMP_NOT_FOUND_PT_LIST: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//   PT List를 찾을 수 없음
case E_IBC_ERROR_EMP_USING_PT_GROUP: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//   PT Group 이 이미 존재함
case E_IBC_ERROR_EMP_USING_CODEC_LIST: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//   Codec List가 이미 존재함
case E_IBC_ERROR_EMP_USING_CODEC_NAME: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE; //   Codec Name이 이미 존재함
case E_IBC_ERROR_EMP_USING_RULE_LIST: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//   Rule List가 이미 존재함
case E_IBC_ERROR_EMP_NOT_FOUND_RULE_LIST: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//   Rule List를 찾을 수 없음
case E_IBC_ERROR_EMP_USING_CODEC_ID: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//   Codec ID 가 이미 존재함
case E_IBC_ERROR_EMP_USING_RULE_ID: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//   Rule ID 가 이미 존재함
case E_IBC_ERROR_EMP_EXCEED_RULE: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//   Rule 의 숫자가 Max값을 넘음
case E_IBC_ERROR_EMP_INVALID_CODEC: return SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE;//   잘못된 Codec Type
case E_IBC_ERROR_EMP_NOT_USE____: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//   ADD RSP 이후 후속 메시지 대기 timeout 시
case E_IBC_ERROR_EMP_NOT_USE_____: return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;//   ADD RSP 이후 후속 메시지 대기 timeout 시
case E_IBC_ERROR_EMP_RESOURCE_FAIL: return SipReasonCode_t::E_TRGW_INSUFFICIENT_RSC_CAUSE; //   eMP 자원 부족
#ifdef DEF_RTE_R131
case E_IBC_ERROR_CM_STATUS_NPDB_DOWN: return SipReasonCode_t::E_NPDB_STS_DOWN_CAUSE;
case E_IBC_ERROR_CM_STATUS_HLR_DOWN: return SipReasonCode_t::E_HLR_STS_DOWN_CAUSE;
#endif
default:{
   nETCError=_nCode&0xF0000000;
   if( nETCError == 0x10000000){
      return SipReasonCode_t::E_INTERNAL_ERR_CAUSE;
   }
   return SipReasonCode_t::E_TRGW_INCORRECT_CAUSE;
}
}///

}
#endif
