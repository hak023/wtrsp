/**
 * @file   eSipRteErr.hxx
 * the sip sharedmemory 
 *
 * @attention Copyright (c) 2014 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2014.11.12   hc_yun   1.0     first written
 * @par 2015.02.10   hc_yun   1.2     add modify error 
 * @par 2015.04.15   hc_yun   1.3     add timer, trace error
 * @par 2015.04.30   hc_yun   1.4     add route param error (MaxCnt, DeactRsp)
 *                                    add timer parame error(garbage) 
 *                                    add route error(CheckRoute_MismatchTransport)
 * @par 2015.11.02   hc_yun   1.5     add emergency
 * @par 2015.11.07   hc_yun   1.6     add timer
 * @par 2016.03.15   hc_yun   1.7     add check domain @R106
 * @par 2016.07.22   hc_yun   1.7     add check local, route_group @R120
 * @par 2017.03.15   hc_yun   1.8     add check local, nat @local_nat
 * @par 2017.04.24   hc_yun   1.9     del check loc @vibc
 */

#ifndef __ESIP_CORE_SIP_RTE_ERROR_H__
#define __ESIP_CORE_SIP_RTE_ERROR_H__

#include <stdio.h>

enum ESipRteError_t
{
E_RTE_COMMON_BASE                            = 0x00011000,
E_RTE_IllegalParam                           = 0x00011001,
E_RTE_InternalMemError                       = 0x00011002,
E_RTE_HasNotbeenInit                         = 0x00011003,
E_RTE_FailCreate_SharedMemory                = 0x00011004,
E_RTE_FailLoadLocalNode_NotFoundFile         = 0x00011005,
E_RTE_FailLoadRemoteNode_NotFoundFile        = 0x00011006,
E_RTE_FailLoadRouteGroup_NotFoundFile        = 0x00011007,
E_RTE_FailLoadRoute_NotFoundFile             = 0x00011008,
E_RTE_FailLoadRouteSeq_NotFoundFile          = 0x00011009,
E_RTE_FailLoadSepcialRouteSeq_NotFoundFile   = 0x0001100A,
E_RTE_FailLoadTrGW_NotFoundFile              = 0x0001100B,
E_RTE_FailLoadReason_NotFoundFile            = 0x0001100C,
E_RTE_FailLoadEnv_NotFoundFile               = 0x0001100D,
E_RTE_FailLoadTimer_NotFoundFile             = 0x0001100E,
E_RTE_FailLoadEmer_NotFoundFile              = 0x0001100F,

E_RTE_CheckLocal_InvalidID                   = 0x00011100,
E_RTE_CheckLocal_InvalidIPver                = 0x00011101,
E_RTE_CheckLocal_InvalidPort                 = 0x00011102,
E_RTE_CheckLocal_InvalidIP4                  = 0x00011103,
E_RTE_CheckLocal_InvalidIP6                  = 0x00011104,
E_RTE_CheckLocal_InvalidProtocolType         = 0x00011105,
E_RTE_CheckLocal_DuplicateID                 = 0x00011106,
E_RTE_CheckLocal_DuplicateIP_Port_Type       = 0x00011107,
E_RTE_CheckLocal_ExceedMaxRecord             = 0x00011108,
E_RTE_CheckLocal_NotFoundID                  = 0x00011109,
E_RTE_FailLocal_ExistRoute                   = 0x0001110A,
E_RTE_FailLocal_AppInitialize                = 0x0001110B,
E_RTE_CheckLocal_NoModIP_Port_Type           = 0x0001110C,
E_RTE_CheckLocal_SameAsExistingValue         = 0x0001110D,
E_RTE_CheckLocal_InvalidDesc                 = 0x0001110E,
//@R106 201603
E_RTE_CheckLocal_InvalidDomain               = 0x0001110F,
//@R120 201609
E_RTE_CheckLocal_InvalidRCSOn                = 0x00011110,
//@local_nat 2017/03/17
E_RTE_CheckLocal_InvalidNATOn                = 0x00011111,
//@vibc 20170424
E_RTE_FailDelLocal_AvailableNode             = 0x00011112,

E_RTE_CheckRemote_InvalidID                  = 0x00011200,
E_RTE_CheckRemote_InvalidIPver               = 0x00011201,
E_RTE_CheckRemote_InvalidPort                = 0x00011202,
E_RTE_CheckRemote_InvalidIP4                 = 0x00011203,
E_RTE_CheckRemote_InvalidIP6                 = 0x00011204,
E_RTE_CheckRemote_InvalidProtocolType        = 0x00011205,
E_RTE_CheckRemote_DuplicateID                = 0x00011206,
E_RTE_CheckRemote_DuplicateIP_Port_Type      = 0x00011207,
E_RTE_CheckRemote_ExceedMaxRecord            = 0x00011208,
E_RTE_CheckRemote_NotFoundID                 = 0x00011209,
E_RTE_FailRemote_ExistRoute                  = 0x0001120A,
E_RTE_CheckRemote_NoModIP_Port_Type          = 0x0001120B,
E_RTE_CheckRemote_SameAsExistingValue        = 0x0001120C,
E_RTE_CheckRemote_InvalidDesc                = 0x0001120D,
E_RTE_CheckRemote_InvalidNATOn               = 0x0001120E,
E_RTE_CheckRemote_InvalidDSCP                = 0x0001120F,
//@R106 201603
E_RTE_CheckRemote_InvalidDomain              = 0x00011210,

E_RTE_CheckRoute_InvalidID                   = 0x00011300,
E_RTE_CheckRoute_InvalidLocalID              = 0x00011301,
E_RTE_CheckRoute_InvalidRemoteID             = 0x00011302,
E_RTE_CheckRoute_InvalidTrGWID               = 0x00011303,
E_RTE_CheckRoute_InvalidRouteType            = 0x00011304,
E_RTE_CheckRoute_InvalidMediaRouteFlag       = 0x00011306,
E_RTE_CheckRoute_InvalidAction               = 0x00011307,
E_RTE_CheckRoute_DuplicateID                 = 0x00011308,
E_RTE_CheckRoute_IllegalParam_Action_Time    = 0x00011309,
E_RTE_CheckRoute_NotFoundLocal               = 0x0001130A,
E_RTE_CheckRoute_NotFoundRemote              = 0x0001130B,
E_RTE_CheckRoute_ExceedMaxRecord             = 0x0001130C,
E_RTE_CheckRoute_DuplicateLocalID_RemoteID   = 0x0001130D,
E_RTE_CheckRoute_NotFoundID                  = 0x0001130E,
E_RTE_FailRoute_ExistDRTE_InRouteSeq         = 0x0001130F,
E_RTE_FailRoute_ExistARTE_InRouteSeq         = 0x00011310,
E_RTE_CheckRoute_InvalidOptTime              = 0x00011311,
E_RTE_CheckRoute_InvalidOptRetry             = 0x00011312,
E_RTE_CheckRoute_InvalidSesTime              = 0x00011313,
E_RTE_CheckRoute_NoModLID_RID                = 0x00011314,
E_RTE_CheckRoute_SameAsExistingValue         = 0x00011315,
E_RTE_CheckRoute_InvalidRouteGroup           = 0x00011316,
E_RTE_CheckRoute_InvalidDesc                 = 0x00011317,
E_RTE_CheckRoute_InvalidMaxCnt               = 0x00011318,
E_RTE_CheckRoute_InvalidDeactRsp             = 0x00011319,
E_RTE_CheckRoute_MismatchTransport           = 0x0001131A,
//@R120 201609
E_RTE_CheckRoute_NotFoundGroupID             = 0x0001131B,

E_RTE_CheckRouteSeq_InvalidID                = 0x00011400,
E_RTE_CheckRouteSeq_InvalidDRTE              = 0x00011401,
E_RTE_CheckRouteSeq_DuplicateID              = 0x00011402,
E_RTE_CheckRouteSeq_NotFoundDRTE             = 0x00011403,
E_RTE_CheckRouteSeq_InvalidARTE              = 0x00011404,
E_RTE_CheckRouteSeq_SameDRTE_ARTE            = 0x00011405,
E_RTE_CheckRouteSeq_NotFoundARTE             = 0x00011406,
E_RTE_CheckRouteSeq_ExceedMaxRecord          = 0x00011407,
E_RTE_CheckRouteSeq_NotFoundID               = 0x00011408,
E_RTE_CheckRouteSeq_DuplicateARTE            = 0x00011409,
E_RTE_FailRouteSeq_ExistSRouteSeq            = 0x0001140A,
E_RTE_CheckRouteSeq_SameAsExistingValue      = 0x0001140B,
E_RTE_CheckRouteSeq_InvalidDesc              = 0x0001140C,
E_RTE_CheckRouteSeq_ExistRouteOp             = 0x0001140D,

E_RTE_CheckSRouteSeq_InvalidID               = 0x00011500,
E_RTE_CheckSRouteSeq_DuplicateID             = 0x00011501,
E_RTE_CheckSRouteSeq_ExceedMaxRecord         = 0x00011502,
E_RTE_CheckSRouteSeq_NotFoundID              = 0x00011503,
E_RTE_CheckSRouteSeq_InvalidRate             = 0x00011504,
E_RTE_CheckSRouteSeq_FirstRSeqZero           = 0x00011505,
E_RTE_CheckSRouteSeq_InvalidRSeq             = 0x00011506,
E_RTE_CheckSRouteSeq_NotFoundRSeq            = 0x00011507,
E_RTE_CheckSRouteSeq_OutofValidRange_RSeq    = 0x00011508,
E_RTE_CheckSRouteSeq_OutofValidRange_Rate    = 0x00011509,
E_RTE_CheckSRouteSeq_SameAsExistingValue     = 0x0001150A,
E_RTE_CheckSRouteSeq_DuplicateRSeq           = 0x0001150B,
E_RTE_CheckSRouteSeq_InvalidDesc             = 0x0001150C,
E_RTE_CheckSRouteSeq_ExistRouteOp            = 0x0001150D,

E_RTE_CheckRouteGroup_InvalidID              = 0x00011600,
E_RTE_CheckRouteGroup_InvalidMaxCPS          = 0x00011601,
E_RTE_CheckRouteGroup_InvalidCPSFlag         = 0x00011602,
E_RTE_CheckRouteGroup_InvalidRSPID           = 0x00011603,
E_RTE_CheckRouteGroup_SameAsExistingValue    = 0x00011604,
E_RTE_CheckRouteGroup_IllegalParam_CPS_Flag  = 0x00011605,
E_RTE_CheckRouteGroup_DuplicateID            = 0x00011606,
E_RTE_CheckRouteGroup_ExceedMaxRecord        = 0x00011607,
E_RTE_CheckRouteGroup_NotFoundID             = 0x00011608,
E_RTE_CheckRouteGroup_InvalidDSCP            = 0x00011609,
E_RTE_CheckRouteGroup_InvalidNATFlag         = 0x0001160A,
E_RTE_CheckRouteGroup_InvalidDesc            = 0x0001160B,
E_RTE_CheckRouteGroup_InvalidMaxTPS          = 0x0001160C,
E_RTE_CheckRouteGroup_InvalidTPSFlag         = 0x0001160D,
//@R120 201609
E_RTE_CheckRouteGroup_InvalidChargingON      = 0x0001160E,
E_RTE_CheckRouteGroup_InvalidChargingCode    = 0x0001160F,
E_RTE_CheckRouteGroup_InvalidChargingIDC     = 0x00011610,
E_RTE_CheckRouteGroup_InvalidRoamingIDC      = 0x00011611,
E_RTE_FailRouteGroup_ExistRoute              = 0x00011612,
//@vibc
E_RTE_CheckRouteGroup_InvalidLBMaxCPS        = 0x00011613,
E_RTE_CheckRouteGroup_InvalidLBCPSFlag       = 0x00011614,
E_RTE_CheckRouteGroup_InvalidLBMaxTPS        = 0x00011615,
E_RTE_CheckRouteGroup_InvalidLBTPSFlag       = 0x00011616,
E_RTE_CheckRouteGroup_IllegalParam_LBCPS_Flag = 0x00011617,

E_RTE_CheckTrGW_InvalidID                    = 0x00011700,
E_RTE_CheckTrGW_InvalidSvrIP4                = 0x00011701,
E_RTE_CheckTrGW_InvalidTrGWIP4               = 0x00011702,
E_RTE_CheckTrGW_InvalidSvrPort               = 0x00011703,
E_RTE_CheckTrGW_InvalidTrGWPort              = 0x00011704,
E_RTE_CheckTrGW_InvalidCallRate              = 0x00011705,
E_RTE_CheckTrGW_InvalidMaxCnt                = 0x00011706,
E_RTE_CheckTrGW_InvalidStatus                = 0x00011707,
E_RTE_CheckTrGW_NoModSvrIP                   = 0x00011708,
E_RTE_CheckTrGW_NoModTrGWIP                  = 0x00011709,
E_RTE_CheckTrGW_NoModSvrPort                 = 0x0001170A,
E_RTE_CheckTrGW_NoModTrGWPort                = 0x0001170B,
E_RTE_CheckTrGW_SameAsExistingValue          = 0x0001170C,
E_RTE_CheckTrGW_DuplicateID                  = 0x0001170D,
E_RTE_CheckTrGW_ExceedMaxRecord              = 0x0001170E,
E_RTE_CheckTrGW_DuplicateSvrIP_Port          = 0x0001170F,
E_RTE_CheckTrGW_DuplicateTrGWIP_Port         = 0x00011710,
E_RTE_CheckTrGW_NotFoundID                   = 0x00011711,
E_RTE_CheckTrGW_UsingSession                 = 0x00011712,
E_RTE_CheckTrGW_NoModStatus                  = 0x00011713,
E_RTE_FailGetTrGW_Unavailable_Status         = 0x00011714,
E_RTE_CheckTrGW_InvalidDesc                  = 0x00011715,

E_RTE_CheckReason_InvalidID                  = 0x00011800,
E_RTE_CheckReason_CauseCode                  = 0x00011801,
E_RTE_CheckReason_RspCode                    = 0x00011802,
E_RTE_CheckReason_SameAsExistingValue        = 0x00011803,
E_RTE_CheckReason_DuplicateID                = 0x00011804,
E_RTE_CheckReason_ExceedMaxRecord            = 0x00011805,
E_RTE_CheckReason_NotFoundID                 = 0x00011806,
E_RTE_CheckReason_InvalidDesc                = 0x00011807,

E_RTE_CheckEnv_InvalidMaxSES                 = 0x00011900,
E_RTE_CheckEnv_InvalidMaxMSG                 = 0x00011901,
E_RTE_CheckEnv_InvalidMaxCPS                 = 0x00011902,
E_RTE_CheckEnv_InvalidMaxCPU                 = 0x00011903,
E_RTE_CheckEnv_InvalidMaxMEM                 = 0x00011904,
E_RTE_CheckEnv_InvalidMaxSES_Flag            = 0x00011905,
E_RTE_CheckEnv_InvalidMaxCPS_Flag            = 0x00011906,
E_RTE_CheckEnv_InvalidMaxCPU_Flag            = 0x00011907,
E_RTE_CheckEnv_InvalidMaxMEM_Flag            = 0x00011908,
E_RTE_CheckEnv_SameAsExistingValue           = 0x00011909,
E_RTE_CheckEnv_InvalidDSCP                   = 0x0001190A,
E_RTE_CheckEnv_InvalidMaxMSG_Flag            = 0x0001190B,
E_RTE_CheckEnv_InvalidMaxTPS                 = 0x0001190C,
E_RTE_CheckEnv_InvalidMaxTPS_Flag            = 0x0001190D,
//add 1.5
E_RTE_CheckEnv_InvalidMinCPS_Emer            = 0x0001190E,
E_RTE_CheckEnv_InvalidMaxCPS_Audio           = 0x0001190F,
E_RTE_CheckEnv_InvalidMaxCPS_Video           = 0x00011910,
E_RTE_CheckEnv_InvalidCPSClass_Flag          = 0x00011911,
//@vibc_svt
E_RTE_CheckEnv_InvalidSumOfLevelCPS          = 0x00011912,

E_RTE_FailGetICRoute_NotFoundLocalNode       = 0x00011A00,
E_RTE_FailGetICRoute_NotFoundRemoteNode      = 0x00011A01,
E_RTE_FailGetICRoute_NotFoundRoute           = 0x00011A02,
E_RTE_FailGetICRoute_UnavalibleRoute         = 0x00011A03,
E_RTE_FailGetOGRoute_NotFoundRSeq            = 0x00011A04,
E_RTE_FailGetOGRoute_NotFoundDRTE            = 0x00011A05,
E_RTE_FailGetOGRoute_NotFoundARTE            = 0x00011A06,
E_RTE_FailGetOGRoute_UnavalibleRoute         = 0x00011A07,
E_RTE_FailGetOGRoute_NotFoundLocalNode       = 0x00011A08,
E_RTE_FailGetOGRoute_NotFoundRemoteNode      = 0x00011A09,
E_RTE_FailGetOGRoute_NotFoundSpecialRSeq     = 0x00011A0A,
E_RTE_FailGetOGRoute_InvaildSpecialRSeq      = 0x00011A0B,
E_RTE_FailGetICRoute_OverLoadGroupCPS        = 0x00011A0C,
E_RTE_FailGetICRoute_OverLoadGlobalCPS       = 0x00011A0D,
E_RTE_FailGetICRoute_OverLoadCPU             = 0x00011A0E,
E_RTE_FailGetICRoute_OverLoadMEM             = 0x00011A0F,
E_RTE_FailGetICRoute_OverLoadGroupTPS        = 0x00011A10,
E_RTE_FailGetICRoute_OverLoadGlobalTPS       = 0x00011A11,
//add 1.5
E_RTE_FailGetICRoute_OverLoadEmergency       = 0x00011A12,
E_RTE_FailGetICRoute_OverLoadAudio           = 0x00011A13,
E_RTE_FailGetICRoute_OverLoadVideo           = 0x00011A14,

E_RTE_CheckTimer_SameAsExistingValue         = 0x00011B00,
E_RTE_CheckTimer_InvalidT1                   = 0x00011B01,
E_RTE_CheckTimer_InvalidT2                   = 0x00011B02,
E_RTE_CheckTimer_InvalidT4                   = 0x00011B03,
E_RTE_CheckTimer_InvalidTA                   = 0x00011B04,
E_RTE_CheckTimer_InvalidTB                   = 0x00011B05,
E_RTE_CheckTimer_InvalidTC                   = 0x00011B06,
E_RTE_CheckTimer_InvalidTD                   = 0x00011B07,
E_RTE_CheckTimer_InvalidTE                   = 0x00011B08,
E_RTE_CheckTimer_InvalidTF                   = 0x00011B09,
E_RTE_CheckTimer_InvalidTG                   = 0x00011B0A,
E_RTE_CheckTimer_InvalidTH                   = 0x00011B0B,
E_RTE_CheckTimer_InvalidTI                   = 0x00011B0C,
E_RTE_CheckTimer_InvalidTJ                   = 0x00011B0D,
E_RTE_CheckTimer_InvalidTK                   = 0x00011B0E,
E_RTE_CheckTimer_InvalidTL                   = 0x00011B0F,
E_RTE_CheckTimer_InvalidTM                   = 0x00011B10,
E_RTE_CheckTimer_InvalidDAlive               = 0x00011B11,
E_RTE_CheckTimer_InvalidDTerm                = 0x00011B12,
E_RTE_CheckTimer_Invalid1xxToRecvPRACK       = 0x00011B13,
E_RTE_CheckTimer_InvalidWaitForPRACK         = 0x00011B14,
E_RTE_CheckTimer_InvalidGarbage              = 0x00011B15,
E_RTE_CheckTimer_InvalidTrGW                 = 0x00011B16,

E_RTE_CheckTrace_DuplicateID                 = 0x00011C00,
E_RTE_CheckTrace_ExceedMaxRecord             = 0x00011C01,
E_RTE_CheckTrace_NotFoundID                  = 0x00011C02,


E_RTE_CheckEmer_DuplicateID                  = 0x00011D00,
E_RTE_CheckEmer_SameAsExistingValue          = 0x00011D01,
E_RTE_CheckEmer_InvalidID                    = 0x00011D02,
E_RTE_CheckEmer_InvalidDesc                  = 0x00011D03,
E_RTE_CheckEmer_InvalidMIN                   = 0x00011D04,
E_RTE_CheckEmer_DuplicateMIN                 = 0x00011D05,
E_RTE_CheckEmer_ExceedMaxRecord              = 0x00011D06,
E_RTE_CheckEmer_NotFoundID                   = 0x00011D07,
E_RTE_CheckEmer_NoModMIN                     = 0x00011D08,

};

inline const char * fnSipRteApiStringError(int _nCode)    
{                                                                 
int nLevel=(_nCode)&0xffffff00;                                 
switch(_nCode)                                                       
{
if( nLevel == E_RTE_COMMON_BASE){
//common error                                                             
case E_RTE_COMMON_BASE:                          return "COMMON_BASE";
case E_RTE_IllegalParam:                         return "IllegalParam";
case E_RTE_InternalMemError:                     return "InternalMemError";
case E_RTE_HasNotbeenInit:                       return "HasNotbeenInit";
case E_RTE_FailCreate_SharedMemory:              return "FailCreate_SharedMemory";
case E_RTE_FailLoadLocalNode_NotFoundFile:       return "FailLoadLocalNode_NotFoundFile";
case E_RTE_FailLoadRemoteNode_NotFoundFile:      return "FailLoadRemoteNode_NotFoundFile";
case E_RTE_FailLoadRouteGroup_NotFoundFile:      return "FailLoadRouteGroup_NotFoundFile";
case E_RTE_FailLoadRoute_NotFoundFile:           return "FailLoadRoute_NotFoundFile";
case E_RTE_FailLoadRouteSeq_NotFoundFile:        return "FailLoadRouteSeq_NotFoundFile";
case E_RTE_FailLoadSepcialRouteSeq_NotFoundFile: return "FailLoadSepcialRouteSeq_NotFoundFile";
case E_RTE_FailLoadTrGW_NotFoundFile:            return "FailLoadTrGW_NotFoundFile";
case E_RTE_FailLoadReason_NotFoundFile:          return "FailLoadReason_NotFoundFile";
case E_RTE_FailLoadEnv_NotFoundFile:             return "FailLoadEnv_NotFoundFile";
case E_RTE_FailLoadTimer_NotFoundFile:           return "FailLoadTimer_NotFoundFile";
case E_RTE_FailLoadEmer_NotFoundFile:            return "FailLoadEmer_NotFoundFile";

//check local
}else if( nLevel == E_RTE_CheckLocal_InvalidID){
case E_RTE_CheckLocal_InvalidID:                 return "CheckLocal_InvalidID";
case E_RTE_CheckLocal_InvalidIPver:              return "CheckLocal_InvalidIPver";
case E_RTE_CheckLocal_InvalidPort:               return "CheckLocal_InvalidPort";
case E_RTE_CheckLocal_InvalidIP4:                return "CheckLocal_InvalidIP4";
case E_RTE_CheckLocal_InvalidIP6:                return "CheckLocal_InvalidIP6";
case E_RTE_CheckLocal_InvalidProtocolType:       return "CheckLocal_InvalidProtocolType";
case E_RTE_CheckLocal_DuplicateID:               return "CheckLocal_DuplicateID";
case E_RTE_CheckLocal_DuplicateIP_Port_Type:     return "CheckLocal_DuplicateIP_Port_Type";
case E_RTE_CheckLocal_ExceedMaxRecord:           return "CheckLocal_ExceedMaxRecord";
case E_RTE_CheckLocal_NotFoundID:                return "CheckLocal_NotFoundID";
case E_RTE_FailLocal_ExistRoute:                 return "FailLocal_ExistRoute";
case E_RTE_FailLocal_AppInitialize:              return "FailLocal_AppInitialize";
case E_RTE_CheckLocal_NoModIP_Port_Type:         return "CheckLocal_NoModIP_Port_Type";
case E_RTE_CheckLocal_SameAsExistingValue:       return "CheckLocal_SameAsExistingValue";
case E_RTE_CheckLocal_InvalidDesc:               return "CheckLocal_InvalidDesc";
case E_RTE_CheckLocal_InvalidDomain:             return "CheckLocal_InvalidDomain";
//@R120 201609
case E_RTE_CheckLocal_InvalidRCSOn:              return "CheckLocal_InvalidRCSOn";
//@local_nat 2017/03/17
case E_RTE_CheckLocal_InvalidNATOn:              return "CheckLocal_InvalidNATOn";
//@vibc 20170424
case E_RTE_FailDelLocal_AvailableNode:           return "FailDelLocal_AvailableNode";

//check remote
}else if( nLevel == E_RTE_CheckRemote_InvalidID){
case E_RTE_CheckRemote_InvalidID:                return "CheckRemote_InvalidID";
case E_RTE_CheckRemote_InvalidIPver:             return "CheckRemote_InvalidIPver";
case E_RTE_CheckRemote_InvalidPort:              return "CheckRemote_InvalidPort";
case E_RTE_CheckRemote_InvalidIP4:               return "CheckRemote_InvalidIP4";
case E_RTE_CheckRemote_InvalidIP6:               return "CheckRemote_InvalidIP6";
case E_RTE_CheckRemote_InvalidProtocolType:      return "CheckRemote_InvalidProtocolType";
case E_RTE_CheckRemote_DuplicateID:              return "CheckRemote_DuplicateID";
case E_RTE_CheckRemote_DuplicateIP_Port_Type:    return "CheckRemote_DuplicateIP_Port_Type";
case E_RTE_CheckRemote_ExceedMaxRecord:          return "CheckRemote_ExceedMaxRecord";
case E_RTE_CheckRemote_NotFoundID:               return "CheckRemote_NotFoundID";
case E_RTE_FailRemote_ExistRoute:                return "FailRemote_ExistRoute";
case E_RTE_CheckRemote_NoModIP_Port_Type:        return "CheckRemote_NoModIP_Port_Type";
case E_RTE_CheckRemote_SameAsExistingValue:      return "CheckRemote_SameAsExistingValue";
case E_RTE_CheckRemote_InvalidDesc:              return "CheckRemote_InvalidDesc";
case E_RTE_CheckRemote_InvalidNATOn:             return "CheckRemote_InvalidNATOn";
case E_RTE_CheckRemote_InvalidDSCP:              return "CheckRemote_InvalidDSCP";
case E_RTE_CheckRemote_InvalidDomain:            return "CheckRemote_InvalidDomain";

//check route
}else if( nLevel == E_RTE_CheckRoute_InvalidID){
case E_RTE_CheckRoute_InvalidID:                 return "CheckRoute_InvalidID";
case E_RTE_CheckRoute_InvalidLocalID:            return "CheckRoute_InvalidLocalID";
case E_RTE_CheckRoute_InvalidRemoteID:           return "CheckRoute_InvalidRemoteID";
case E_RTE_CheckRoute_InvalidTrGWID:             return "CheckRoute_InvalidTrGWID";
case E_RTE_CheckRoute_InvalidRouteType:          return "CheckRoute_InvalidRouteType";
case E_RTE_CheckRoute_InvalidMediaRouteFlag:     return "CheckRoute_InvalidMediaRouteFlag";
case E_RTE_CheckRoute_InvalidAction:             return "CheckRoute_InvalidAction";
case E_RTE_CheckRoute_DuplicateID:               return "CheckRoute_DuplicateID";
case E_RTE_CheckRoute_IllegalParam_Action_Time:  return "CheckRoute_IllegalParam_Action_Time";
case E_RTE_CheckRoute_NotFoundLocal:             return "CheckRoute_NotFoundLocal";
case E_RTE_CheckRoute_NotFoundRemote:            return "CheckRoute_NotFoundRemote";
case E_RTE_CheckRoute_ExceedMaxRecord:           return "CheckRoute_ExceedMaxRecord";
case E_RTE_CheckRoute_DuplicateLocalID_RemoteID: return "CheckRoute_DuplicateLocalID_RemoteID";
case E_RTE_CheckRoute_NotFoundID:                return "CheckRoute_NotFoundID";
case E_RTE_FailRoute_ExistDRTE_InRouteSeq:       return "FailRoute_ExistDRTE_InRouteSeq";
case E_RTE_FailRoute_ExistARTE_InRouteSeq:       return "FailRoute_ExistARTE_InRouteSeq";
case E_RTE_CheckRoute_InvalidOptTime:            return "CheckRoute_InvalidOptTime";
case E_RTE_CheckRoute_InvalidOptRetry:           return "CheckRoute_InvalidOptRetry";
case E_RTE_CheckRoute_InvalidSesTime:            return "CheckRoute_InvalidSesTime";
case E_RTE_CheckRoute_NoModLID_RID:              return "CheckRoute_NoModLID_RID";
case E_RTE_CheckRoute_SameAsExistingValue:       return "CheckRoute_SameAsExistingValue";
case E_RTE_CheckRoute_InvalidRouteGroup:         return "CheckRoute_InvalidRouteGroup";
case E_RTE_CheckRoute_InvalidDesc:               return "CheckRoute_InvalidDesc";
case E_RTE_CheckRoute_InvalidMaxCnt:             return "CheckRoute_InvalidMaxCnt";
case E_RTE_CheckRoute_InvalidDeactRsp:           return "CheckRoute_InvalidDeactRsp";
case E_RTE_CheckRoute_MismatchTransport:         return "CheckRoute_MismatchTransport";
//@R120 201609
case E_RTE_CheckRoute_NotFoundGroupID:           return "CheckRoute_NotFoundGroupID";

//check route seq
}else if( nLevel == E_RTE_CheckRouteSeq_InvalidID){
case E_RTE_CheckRouteSeq_InvalidID:              return "CheckRouteSeq_InvalidID";
case E_RTE_CheckRouteSeq_InvalidDRTE:            return "CheckRouteSeq_InvalidDRTE";
case E_RTE_CheckRouteSeq_DuplicateID:            return "CheckRouteSeq_DuplicateID";
case E_RTE_CheckRouteSeq_NotFoundDRTE:           return "CheckRouteSeq_NotFoundDRTE";
case E_RTE_CheckRouteSeq_InvalidARTE:            return "CheckRouteSeq_InvalidARTE";
case E_RTE_CheckRouteSeq_SameDRTE_ARTE:          return "CheckRouteSeq_SameDRTE_ARTE";
case E_RTE_CheckRouteSeq_NotFoundARTE:           return "CheckRouteSeq_NotFoundARTE";
case E_RTE_CheckRouteSeq_ExceedMaxRecord:        return "CheckRouteSeq_ExceedMaxRecord";
case E_RTE_CheckRouteSeq_NotFoundID:             return "CheckRouteSeq_NotFoundID";
case E_RTE_CheckRouteSeq_DuplicateARTE:          return "CheckRouteSeq_DuplicateARTE";
case E_RTE_FailRouteSeq_ExistSRouteSeq:          return "FailRouteSeq_ExistSRouteSeq";
case E_RTE_CheckRouteSeq_SameAsExistingValue:    return "CheckRouteSeq_SameAsExistingValue";
case E_RTE_CheckRouteSeq_InvalidDesc:            return "CheckRouteSeq_InvalidDesc";
case E_RTE_CheckRouteSeq_ExistRouteOp:           return "CheckRouteSeq_ExistRouteOp";

//check special route seq
}else if( nLevel == E_RTE_CheckSRouteSeq_InvalidID){
case E_RTE_CheckSRouteSeq_InvalidID:             return "CheckSRouteSeq_InvalidID";
case E_RTE_CheckSRouteSeq_DuplicateID:           return "CheckSRouteSeq_DuplicateID";
case E_RTE_CheckSRouteSeq_ExceedMaxRecord:       return "CheckSRouteSeq_ExceedMaxRecord";
case E_RTE_CheckSRouteSeq_NotFoundID:            return "CheckSRouteSeq_NotFoundID";
case E_RTE_CheckSRouteSeq_InvalidRate:           return "CheckSRouteSeq_InvalidRate";
case E_RTE_CheckSRouteSeq_FirstRSeqZero:         return "CheckSRouteSeq_FirstRSeqZero";
case E_RTE_CheckSRouteSeq_InvalidRSeq:           return "CheckSRouteSeq_InvalidRSeq";
case E_RTE_CheckSRouteSeq_NotFoundRSeq:          return "CheckSRouteSeq_NotFoundRSeq";
case E_RTE_CheckSRouteSeq_OutofValidRange_RSeq:  return "CheckSRouteSeq_OutofValidRange_RSeq";
case E_RTE_CheckSRouteSeq_OutofValidRange_Rate:  return "CheckSRouteSeq_OutofValidRange_Rate";
case E_RTE_CheckSRouteSeq_SameAsExistingValue:   return "CheckSRouteSeq_SameAsExistingValue";
case E_RTE_CheckSRouteSeq_DuplicateRSeq:         return "CheckSRouteSeq_DuplicateRSeq";
case E_RTE_CheckSRouteSeq_InvalidDesc:           return "CheckSRouteSeq_InvalidDesc";
case E_RTE_CheckSRouteSeq_ExistRouteOp:          return "CheckSRouteSeq_ExistRouteOp";

//check route group
}else if( nLevel == E_RTE_CheckRouteGroup_InvalidID){
case E_RTE_CheckRouteGroup_InvalidID:            return "CheckRouteGroup_InvalidID";
case E_RTE_CheckRouteGroup_InvalidMaxCPS:        return "CheckRouteGroup_InvalidMaxCPS";
case E_RTE_CheckRouteGroup_InvalidCPSFlag:       return "CheckRouteGroup_InvalidCPSFlag";
case E_RTE_CheckRouteGroup_InvalidRSPID:         return "CheckRouteGroup_InvalidRSPID";
case E_RTE_CheckRouteGroup_SameAsExistingValue:  return "CheckRouteGroup_SameAsExistingValue";
case E_RTE_CheckRouteGroup_IllegalParam_CPS_Flag:return "CheckRouteGroup_IllegalParam_CPS_Flag";
case E_RTE_CheckRouteGroup_DuplicateID:          return "CheckRouteGroup_DuplicateID";
case E_RTE_CheckRouteGroup_ExceedMaxRecord:      return "CheckRouteGroup_ExceedMaxRecord";
case E_RTE_CheckRouteGroup_NotFoundID:           return "CheckRouteGroup_NotFoundID";
case E_RTE_CheckRouteGroup_InvalidDSCP:          return "CheckRouteGroup_InvalidDSCP";
case E_RTE_CheckRouteGroup_InvalidNATFlag:       return "CheckRouteGroup_InvalidNATFlag";
case E_RTE_CheckRouteGroup_InvalidDesc:          return "CheckRouteGroup_InvalidDesc";
case E_RTE_CheckRouteGroup_InvalidMaxTPS:        return "CheckRouteGroup_InvalidMaxTPS";
case E_RTE_CheckRouteGroup_InvalidTPSFlag:       return "CheckRouteGroup_InvalidTPSFlag";
//@R120 201609
case E_RTE_CheckRouteGroup_InvalidChargingON:    return "CheckRouteGroup_InvalidChargingON";
case E_RTE_CheckRouteGroup_InvalidChargingCode:  return "CheckRouteGroup_InvalidChargingCode";
case E_RTE_CheckRouteGroup_InvalidChargingIDC:   return "CheckRouteGroup_InvalidChargingIDC";
case E_RTE_CheckRouteGroup_InvalidRoamingIDC:    return "CheckRouteGroup_InvalidRoamingIDC";
case E_RTE_FailRouteGroup_ExistRoute:            return "FailRouteGroup_ExistRoute";
//@vibc
case E_RTE_CheckRouteGroup_InvalidLBMaxCPS:       return "CheckRouteGroup_InvalidLBMaxCPS";
case E_RTE_CheckRouteGroup_InvalidLBCPSFlag:      return "CheckRouteGroup_InvalidLBCPSFlag";
case E_RTE_CheckRouteGroup_InvalidLBMaxTPS:       return "CheckRouteGroup_InvalidLBMaxTPS";
case E_RTE_CheckRouteGroup_InvalidLBTPSFlag:      return "CheckRouteGroup_InvalidLBTPSFlag";
case E_RTE_CheckRouteGroup_IllegalParam_LBCPS_Flag: return "CheckRouteGroup_IllegalParam_LBCPS_Flag";

//check trgw
}else if( nLevel == E_RTE_CheckTrGW_InvalidID){
case E_RTE_CheckTrGW_InvalidID:                  return "CheckTrGW_InvalidID";
case E_RTE_CheckTrGW_InvalidSvrIP4:              return "CheckTrGW_InvalidSvrIP4";
case E_RTE_CheckTrGW_InvalidTrGWIP4:             return "CheckTrGW_InvalidTrGWIP4";
case E_RTE_CheckTrGW_InvalidSvrPort:             return "CheckTrGW_InvalidSvrPort";
case E_RTE_CheckTrGW_InvalidTrGWPort:            return "CheckTrGW_InvalidTrGWPort";
case E_RTE_CheckTrGW_InvalidCallRate:            return "CheckTrGW_InvalidCallRate";
case E_RTE_CheckTrGW_InvalidMaxCnt:              return "CheckTrGW_InvalidMaxCnt";
case E_RTE_CheckTrGW_InvalidStatus:              return "CheckTrGW_InvalidStatus";
case E_RTE_CheckTrGW_NoModSvrIP:                 return "CheckTrGW_NoModSvrIP";
case E_RTE_CheckTrGW_NoModTrGWIP:                return "CheckTrGW_NoModTrGWIP";
case E_RTE_CheckTrGW_NoModSvrPort:               return "CheckTrGW_NoModSvrPort";
case E_RTE_CheckTrGW_NoModTrGWPort:              return "CheckTrGW_NoModTrGWPort";
case E_RTE_CheckTrGW_SameAsExistingValue:        return "CheckTrGW_SameAsExistingValue";
case E_RTE_CheckTrGW_DuplicateID:                return "CheckTrGW_DuplicateID";
case E_RTE_CheckTrGW_ExceedMaxRecord:            return "CheckTrGW_ExceedMaxRecord";
case E_RTE_CheckTrGW_DuplicateSvrIP_Port:        return "CheckTrGW_DuplicateSvrIP_Port";
case E_RTE_CheckTrGW_DuplicateTrGWIP_Port:       return "CheckTrGW_DuplicateTrGWIP_Port";
case E_RTE_CheckTrGW_NotFoundID:                 return "CheckTrGW_NotFoundID";
case E_RTE_CheckTrGW_UsingSession:               return "CheckTrGW_UsingSession";
case E_RTE_CheckTrGW_NoModStatus:                return "CheckTrGW_NoModStatus";
case E_RTE_FailGetTrGW_Unavailable_Status:       return "FailGetTrGW_Unavailable_Status";
case E_RTE_CheckTrGW_InvalidDesc:                return "CheckTrGW_InvalidDesc";

//check reason
}else if( nLevel == E_RTE_CheckReason_InvalidID){
case E_RTE_CheckReason_InvalidID:                return "CheckReason_InvalidID";
case E_RTE_CheckReason_CauseCode:                return "CheckReason_CauseCode";
case E_RTE_CheckReason_RspCode:                  return "CheckReason_RspCode";
case E_RTE_CheckReason_SameAsExistingValue:      return "CheckReason_SameAsExistingValue";
case E_RTE_CheckReason_DuplicateID:              return "CheckReason_DuplicateID";
case E_RTE_CheckReason_ExceedMaxRecord:          return "CheckReason_ExceedMaxRecord";
case E_RTE_CheckReason_NotFoundID:               return "CheckReason_NotFoundID";
case E_RTE_CheckReason_InvalidDesc:              return "CheckReason_InvalidDesc";

//check env
}else if( nLevel == E_RTE_CheckEnv_InvalidMaxSES){
case E_RTE_CheckEnv_InvalidMaxSES:               return "CheckEnv_InvalidMaxSES";
case E_RTE_CheckEnv_InvalidMaxMSG:               return "CheckEnv_InvalidMaxMSG";
case E_RTE_CheckEnv_InvalidMaxCPS:               return "CheckEnv_InvalidMaxCPS";
case E_RTE_CheckEnv_InvalidMaxCPU:               return "CheckEnv_InvalidMaxCPU";
case E_RTE_CheckEnv_InvalidMaxMEM:               return "CheckEnv_InvalidMaxMEM";
case E_RTE_CheckEnv_InvalidMaxSES_Flag:          return "CheckEnv_InvalidMaxSES_Flag";
case E_RTE_CheckEnv_InvalidMaxCPS_Flag:          return "CheckEnv_InvalidMaxCPS_Flag";
case E_RTE_CheckEnv_InvalidMaxCPU_Flag:          return "CheckEnv_InvalidMaxCPU_Flag";
case E_RTE_CheckEnv_InvalidMaxMEM_Flag:          return "CheckEnv_InvalidMaxMEM_Flag";
case E_RTE_CheckEnv_SameAsExistingValue:         return "CheckEnv_SameAsExistingValue";
case E_RTE_CheckEnv_InvalidDSCP:                 return "CheckEnv_InvalidDSCP";
case E_RTE_CheckEnv_InvalidMaxMSG_Flag:          return "CheckEnv_InvalidMaxMSG_Flag";
case E_RTE_CheckEnv_InvalidMaxTPS:               return "CheckEnv_InvalidMaxTPS";
case E_RTE_CheckEnv_InvalidMaxTPS_Flag:          return "CheckEnv_InvalidMaxTPS_Flag";
case E_RTE_CheckEnv_InvalidMinCPS_Emer:          return "CheckEnv_InvalidMinCPS_Emer";
case E_RTE_CheckEnv_InvalidMaxCPS_Audio:         return "CheckEnv_InvalidMaxCPS_Audio";
case E_RTE_CheckEnv_InvalidMaxCPS_Video:         return "CheckEnv_InvalidMaxCPS_Video";
case E_RTE_CheckEnv_InvalidCPSClass_Flag:        return "CheckEnv_InvalidCPSClass_Flag";
case E_RTE_CheckEnv_InvalidSumOfLevelCPS:        return "CheckEnv_InvalidSumOfLevelCPS";

//check decision route
}else if( nLevel == E_RTE_FailGetICRoute_NotFoundLocalNode){
case E_RTE_FailGetICRoute_NotFoundLocalNode:     return "FailGetICRoute_NotFoundLocalNode";
case E_RTE_FailGetICRoute_NotFoundRemoteNode:    return "FailGetICRoute_NotFoundRemoteNode";
case E_RTE_FailGetICRoute_NotFoundRoute:         return "FailGetICRoute_NotFoundRoute";
case E_RTE_FailGetICRoute_UnavalibleRoute:       return "FailGetICRoute_UnavalibleRoute";
case E_RTE_FailGetOGRoute_NotFoundRSeq:          return "FailGetOGRoute_NotFoundRSeq";
case E_RTE_FailGetOGRoute_NotFoundDRTE:          return "FailGetOGRoute_NotFoundDRTE";
case E_RTE_FailGetOGRoute_NotFoundARTE:          return "FailGetOGRoute_NotFoundARTE";
case E_RTE_FailGetOGRoute_UnavalibleRoute:       return "FailGetOGRoute_UnavalibleRoute";
case E_RTE_FailGetOGRoute_NotFoundLocalNode:     return "FailGetOGRoute_NotFoundLocalNode";
case E_RTE_FailGetOGRoute_NotFoundRemoteNode:    return "FailGetOGRoute_NotFoundRemoteNode";
case E_RTE_FailGetOGRoute_NotFoundSpecialRSeq:   return "FailGetOGRoute_NotFoundSpecialRSeq";
case E_RTE_FailGetOGRoute_InvaildSpecialRSeq:    return "FailGetOGRoute_InvaildSpecialRSeq";
case E_RTE_FailGetICRoute_OverLoadGroupCPS:      return "FailGetICRoute_OverLoadGroupCPS";
case E_RTE_FailGetICRoute_OverLoadGlobalCPS:     return "FailGetICRoute_OverLoadGlobalCPS";
case E_RTE_FailGetICRoute_OverLoadCPU:           return "FailGetICRoute_OverLoadCPU";
case E_RTE_FailGetICRoute_OverLoadMEM:           return "FailGetICRoute_OverLoadMEM";
case E_RTE_FailGetICRoute_OverLoadGroupTPS:      return "FailGetICRoute_OverLoadGroupTPS";
case E_RTE_FailGetICRoute_OverLoadGlobalTPS:     return "FailGetICRoute_OverLoadGlobalTPS";
case E_RTE_FailGetICRoute_OverLoadEmergency:     return "FailGetICRoute_OverLoadEmergency";
case E_RTE_FailGetICRoute_OverLoadAudio:         return "FailGetICRoute_OverLoadAudio";
case E_RTE_FailGetICRoute_OverLoadVideo:         return "FailGetICRoute_OverLoadVideo";
//check timer
}else if( nLevel == E_RTE_CheckTimer_SameAsExistingValue){
case E_RTE_CheckTimer_SameAsExistingValue:       return "CheckTimer_SameAsExistingValue";  
case E_RTE_CheckTimer_InvalidT1:                 return "CheckTimer_InvalidT1"; 
case E_RTE_CheckTimer_InvalidT2:                 return "CheckTimer_InvalidT2";
case E_RTE_CheckTimer_InvalidT4:                 return "CheckTimer_InvalidT4";
case E_RTE_CheckTimer_InvalidTA:                 return "CheckTimer_InvalidTA";
case E_RTE_CheckTimer_InvalidTB:                 return "CheckTimer_InvalidTB";
case E_RTE_CheckTimer_InvalidTC:                 return "CheckTimer_InvalidTC";
case E_RTE_CheckTimer_InvalidTD:                 return "CheckTimer_InvalidTD";
case E_RTE_CheckTimer_InvalidTE:                 return "CheckTimer_InvalidTE";
case E_RTE_CheckTimer_InvalidTF:                 return "CheckTimer_InvalidTF";               
case E_RTE_CheckTimer_InvalidTG:                 return "CheckTimer_InvalidTG";
case E_RTE_CheckTimer_InvalidTH:                 return "CheckTimer_InvalidTH";
case E_RTE_CheckTimer_InvalidTI:                 return "CheckTimer_InvalidTI";
case E_RTE_CheckTimer_InvalidTJ:                 return "CheckTimer_InvalidTJ";
case E_RTE_CheckTimer_InvalidTK:                 return "CheckTimer_InvalidTK";
case E_RTE_CheckTimer_InvalidTL:                 return "CheckTimer_InvalidTL";
case E_RTE_CheckTimer_InvalidTM:                 return "CheckTimer_InvalidTM";
case E_RTE_CheckTimer_InvalidDAlive:             return "CheckTimer_InvalidDAlive";
case E_RTE_CheckTimer_InvalidDTerm:              return "CheckTimer_InvalidDTerm"; 
case E_RTE_CheckTimer_Invalid1xxToRecvPRACK:     return "CheckTimer_Invalid1xxToRecvPRACK";
case E_RTE_CheckTimer_InvalidWaitForPRACK:       return "CheckTimer_InvalidWaitForPRACK";
case E_RTE_CheckTimer_InvalidGarbage:            return "CheckTimer_InvalidGarbage";
case E_RTE_CheckTimer_InvalidTrGW:            return "CheckTimer_InvalidTrGW";
//check trace
}else if( nLevel == E_RTE_CheckTrace_DuplicateID){
case E_RTE_CheckTrace_DuplicateID:               return "CheckTrace_DuplicateID";
case E_RTE_CheckTrace_ExceedMaxRecord:           return "CheckTrace_ExceedMaxRecord";
case E_RTE_CheckTrace_NotFoundID:                return "CheckTrace_NotFoundID";

//check env
}else if( nLevel == E_RTE_CheckEmer_DuplicateID){
case E_RTE_CheckEmer_DuplicateID:               return "CheckEmer_DuplicateID";
case E_RTE_CheckEmer_SameAsExistingValue:       return "CheckEmer_SameAsExistingValue";
case E_RTE_CheckEmer_InvalidID:                 return "CheckEmer_InvalidID";
case E_RTE_CheckEmer_InvalidDesc:               return "CheckEmer_InvalidDesc";
case E_RTE_CheckEmer_InvalidMIN:                return "CheckEmer_InvalidMIN";
case E_RTE_CheckEmer_DuplicateMIN:              return "CheckEmer_DuplicateMIN";
case E_RTE_CheckEmer_ExceedMaxRecord:           return "CheckEmer_ExceedMaxRecord";
case E_RTE_CheckEmer_NotFoundID:                return "CheckEmer_NotFoundID";
case E_RTE_CheckEmer_NoModMIN:                  return "CheckEmer_NoModMIN";
}else{
   return "Unknown Route Error";
}//end if
default: return "Unknown Route Error";
}
return "Unknown Route Error";

}

inline char* GetStringSipRteErr(int _nCode,char* _pErrStr,int _nLen)
{
char szBuf[256]; szBuf[0]=0x00;
int nLevel=_nCode&0xffffff00;
switch(_nCode){               
if( nLevel == E_RTE_COMMON_BASE){                
//common error
case E_RTE_IllegalParam: snprintf(szBuf,sizeof(szBuf)-1,"IllegalParam"); break;
case E_RTE_InternalMemError: snprintf(szBuf,sizeof(szBuf)-1,"InternalMemError"); break;
case E_RTE_HasNotbeenInit: snprintf(szBuf,sizeof(szBuf)-1,"HasNotbeenInit"); break;
//load file
case E_RTE_FailCreate_SharedMemory: snprintf(szBuf,sizeof(szBuf)-1,"FailCreate_SharedMemory"); break;
case E_RTE_FailLoadLocalNode_NotFoundFile: snprintf(szBuf,sizeof(szBuf)-1,"FailLoadLocalNode_NotFoundFile"); break;
case E_RTE_FailLoadRemoteNode_NotFoundFile: snprintf(szBuf,sizeof(szBuf)-1,"FailLoadRemoteNode_NotFoundFile"); break;
case E_RTE_FailLoadRouteGroup_NotFoundFile:snprintf(szBuf,sizeof(szBuf)-1,"FailLoadRouteGroup_NotFoundFile");break;
case E_RTE_FailLoadRouteSeq_NotFoundFile:snprintf(szBuf,sizeof(szBuf)-1,"FailLoadRouteSeq_NotFoundFile"); break;
case E_RTE_FailLoadSepcialRouteSeq_NotFoundFile: snprintf(szBuf,sizeof(szBuf)-1,"FailLoadSepcialRouteSeq_NotFoundFile"); break;
case E_RTE_FailLoadTrGW_NotFoundFile: snprintf(szBuf,sizeof(szBuf)-1,"FailLoadTrGW_NotFoundFile"); break;
case E_RTE_FailLoadReason_NotFoundFile: snprintf(szBuf,sizeof(szBuf)-1,"FailLoadReason_NotFoundFile"); break;
case E_RTE_FailLoadEnv_NotFoundFile: snprintf(szBuf,sizeof(szBuf)-1,"FailLoadEnv_NotFoundFile"); break;
case E_RTE_FailLoadTimer_NotFoundFile: snprintf(szBuf,sizeof(szBuf)-1,"FailLoadTimer_NotFoundFile"); break;
case E_RTE_FailLoadEmer_NotFoundFile: snprintf(szBuf,sizeof(szBuf)-1,"FailLoadEmer_NotFoundFile"); break;

//local
}else if( nLevel == E_RTE_CheckLocal_InvalidID ){
case E_RTE_CheckLocal_InvalidID : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_InvalidID "); break;
case E_RTE_CheckLocal_InvalidIPver : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_InvalidIPver"); break;
case E_RTE_CheckLocal_InvalidPort : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_InvalidPort"); break;
case E_RTE_CheckLocal_InvalidIP4 : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_InvalidIP4"); break;
case E_RTE_CheckLocal_InvalidIP6 : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_InvalidIP6"); break;
case E_RTE_CheckLocal_InvalidProtocolType : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_InvalidProtocolType"); break;
case E_RTE_CheckLocal_DuplicateID : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_DuplicateID"); break;
case E_RTE_CheckLocal_DuplicateIP_Port_Type : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_DuplicateIP_Port_Type"); break;
case E_RTE_CheckLocal_ExceedMaxRecord : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_ExceedMaxRecord"); break;
case E_RTE_CheckLocal_NotFoundID : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_NotFoundID"); break;
case E_RTE_FailLocal_ExistRoute : snprintf(szBuf,sizeof(szBuf)-1,"FailLocal_ExistRoute"); break;
case E_RTE_FailLocal_AppInitialize : snprintf(szBuf,sizeof(szBuf)-1,"FailLocal_AppInitialize"); break;
case E_RTE_CheckLocal_NoModIP_Port_Type : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_NoModIP_Port_Type"); break;
case E_RTE_CheckLocal_SameAsExistingValue : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_SameAsExistingValue"); break;
case E_RTE_CheckLocal_InvalidDesc : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_InvalidDesc"); break;
case E_RTE_CheckLocal_InvalidDomain : snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_InvalidDomain"); break;
//@R120 201609
case E_RTE_CheckLocal_InvalidRCSOn:  snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_InvalidRCSOn"); break;
//@local_nat 2017/03/17
case E_RTE_CheckLocal_InvalidNATOn:  snprintf(szBuf,sizeof(szBuf)-1,"CheckLocal_InvalidNATOn"); break;
//@vibc 20170424
case E_RTE_FailDelLocal_AvailableNode:  snprintf(szBuf,sizeof(szBuf)-1,"FailDelLocal_AvailableNode"); break;
//remote
}else if( nLevel == E_RTE_CheckRemote_InvalidID ){
case E_RTE_CheckRemote_InvalidID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_InvalidID"); break;
case E_RTE_CheckRemote_InvalidIPver : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_InvalidIPver"); break;
case E_RTE_CheckRemote_InvalidPort : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_InvalidPort"); break;
case E_RTE_CheckRemote_InvalidIP4 : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_InvalidIP4"); break;
case E_RTE_CheckRemote_InvalidIP6 : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_InvalidIP6"); break;
case E_RTE_CheckRemote_InvalidProtocolType : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_InvalidProtocolType"); break;
case E_RTE_CheckRemote_DuplicateID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_DuplicateID"); break;
case E_RTE_CheckRemote_DuplicateIP_Port_Type : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_DuplicateIP_Port_Type"); break;
case E_RTE_CheckRemote_ExceedMaxRecord : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_ExceedMaxRecord"); break;
case E_RTE_CheckRemote_NotFoundID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_NotFoundID"); break;
case E_RTE_FailRemote_ExistRoute : snprintf(szBuf,sizeof(szBuf)-1,"FailRemote_ExistRoute"); break;
case E_RTE_CheckRemote_NoModIP_Port_Type : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_NoModIP_Port_Type"); break;
case E_RTE_CheckRemote_SameAsExistingValue : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_SameAsExistingValue"); break;
case E_RTE_CheckRemote_InvalidDesc : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_InvalidDesc"); break;
case E_RTE_CheckRemote_InvalidNATOn : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_InvalidNATOn"); break;
case E_RTE_CheckRemote_InvalidDSCP : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_InvalidDSCP"); break;
case E_RTE_CheckRemote_InvalidDomain : snprintf(szBuf,sizeof(szBuf)-1,"CheckRemote_InvalidDomain"); break;
//route
}else if( nLevel == E_RTE_CheckRoute_InvalidID ){
case E_RTE_CheckRoute_InvalidID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidID"); break;
case E_RTE_CheckRoute_InvalidLocalID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidLocalID"); break;
case E_RTE_CheckRoute_InvalidRemoteID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidRemoteID"); break;
case E_RTE_CheckRoute_InvalidTrGWID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidTrGWID"); break;
case E_RTE_CheckRoute_InvalidRouteType : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidRouteType"); break;
case E_RTE_CheckRoute_InvalidMediaRouteFlag : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidMediaRouteFlag"); break;
case E_RTE_CheckRoute_InvalidAction : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidAction"); break;
case E_RTE_CheckRoute_DuplicateID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_DuplicateID"); break;
case E_RTE_CheckRoute_IllegalParam_Action_Time : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_IllegalParam_Action_Time"); break;
case E_RTE_CheckRoute_NotFoundLocal : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_NotFoundLocal"); break;
case E_RTE_CheckRoute_NotFoundRemote : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_NotFoundRemote"); break;
case E_RTE_CheckRoute_ExceedMaxRecord : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_ExceedMaxRecord"); break;
case E_RTE_CheckRoute_DuplicateLocalID_RemoteID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_DuplicateLocalID_RemoteID"); break;
case E_RTE_CheckRoute_NotFoundID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_NotFoundID"); break;
case E_RTE_FailRoute_ExistDRTE_InRouteSeq : snprintf(szBuf,sizeof(szBuf)-1,"FailRoute_ExistDRTE_InRouteSeq"); break;
case E_RTE_FailRoute_ExistARTE_InRouteSeq : snprintf(szBuf,sizeof(szBuf)-1,"FailRoute_ExistARTE_InRouteSeq"); break;
case E_RTE_CheckRoute_InvalidOptTime : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidOptTime"); break;
case E_RTE_CheckRoute_InvalidOptRetry : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidOptRetry"); break;
case E_RTE_CheckRoute_InvalidSesTime : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidSesTime"); break;
case E_RTE_CheckRoute_NoModLID_RID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_NoModLID_RID"); break;
case E_RTE_CheckRoute_SameAsExistingValue : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_SameAsExistingValue"); break;
case E_RTE_CheckRoute_InvalidRouteGroup : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidRouteGroup"); break;
case E_RTE_CheckRoute_InvalidDesc : snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidDesc"); break;
case E_RTE_CheckRoute_InvalidMaxCnt: snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidMaxCnt"); break;
case E_RTE_CheckRoute_InvalidDeactRsp: snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_InvalidDeactRsp"); break;
case E_RTE_CheckRoute_MismatchTransport: snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_MismatchTransport"); break;
case E_RTE_CheckRoute_NotFoundGroupID: snprintf(szBuf,sizeof(szBuf)-1,"CheckRoute_NotFoundGroupID"); break;
//route group
}else if( nLevel == E_RTE_CheckRouteGroup_InvalidID ){
case E_RTE_CheckRouteGroup_InvalidID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidID"); break;
case E_RTE_CheckRouteGroup_InvalidMaxCPS: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidMaxCPS"); break;
case E_RTE_CheckRouteGroup_InvalidCPSFlag: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidCPSFlag"); break;
case E_RTE_CheckRouteGroup_InvalidRSPID: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidRSPID"); break;
case E_RTE_CheckRouteGroup_SameAsExistingValue: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_SameAsExistingValue"); break;
case E_RTE_CheckRouteGroup_IllegalParam_CPS_Flag: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_IllegalParam_CPS_Flag"); break;
case E_RTE_CheckRouteGroup_DuplicateID: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_DuplicateID"); break;
case E_RTE_CheckRouteGroup_ExceedMaxRecord: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_ExceedMaxRecord"); break;
case E_RTE_CheckRouteGroup_NotFoundID: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_NotFoundID"); break;
case E_RTE_CheckRouteGroup_InvalidDSCP: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidDSCP"); break;
case E_RTE_CheckRouteGroup_InvalidNATFlag: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidNATFlag"); break;
case E_RTE_CheckRouteGroup_InvalidDesc : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidDesc"); break;
case E_RTE_CheckRouteGroup_InvalidMaxTPS : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidMaxTPS"); break;
case E_RTE_CheckRouteGroup_InvalidTPSFlag : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidTPSFlag"); break;
//@R120 201609
case E_RTE_CheckRouteGroup_InvalidChargingON: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidChargingON"); break;
case E_RTE_CheckRouteGroup_InvalidChargingCode: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidChargingCode"); break;
case E_RTE_CheckRouteGroup_InvalidChargingIDC: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidChargingIDC"); break;
case E_RTE_CheckRouteGroup_InvalidRoamingIDC: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidRoamingIDC"); break;
case E_RTE_FailRouteGroup_ExistRoute: snprintf(szBuf,sizeof(szBuf)-1,"FailRouteGroup_ExistRoute"); break;
//@vibc
case E_RTE_CheckRouteGroup_InvalidLBMaxCPS: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidLBMaxCPS"); break;
case E_RTE_CheckRouteGroup_InvalidLBCPSFlag: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidLBCPSFlag"); break;
case E_RTE_CheckRouteGroup_InvalidLBMaxTPS: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidLBMaxTPS"); break;
case E_RTE_CheckRouteGroup_InvalidLBTPSFlag: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_InvalidLBTPSFlag"); break;
case E_RTE_CheckRouteGroup_IllegalParam_LBCPS_Flag: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteGroup_IllegalParam_LBCPS_Flag"); break;
//route seq
}else if( nLevel == E_RTE_CheckRouteSeq_InvalidID ){
case E_RTE_CheckRouteSeq_InvalidID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_InvalidID"); break;
case E_RTE_CheckRouteSeq_InvalidDRTE : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_InvalidDRTE"); break;
case E_RTE_CheckRouteSeq_DuplicateID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_DuplicateID"); break;
case E_RTE_CheckRouteSeq_NotFoundDRTE : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_NotFoundDRTE"); break;
case E_RTE_CheckRouteSeq_InvalidARTE : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_InvalidARTE"); break;
case E_RTE_CheckRouteSeq_SameDRTE_ARTE : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_SameDRTE_ARTE"); break;
case E_RTE_CheckRouteSeq_NotFoundARTE : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_NotFoundARTE"); break;
case E_RTE_CheckRouteSeq_ExceedMaxRecord : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_ExceedMaxRecord"); break;
case E_RTE_CheckRouteSeq_NotFoundID : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_NotFoundID"); break;
case E_RTE_CheckRouteSeq_DuplicateARTE : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_DuplicateARTE"); break;
case E_RTE_FailRouteSeq_ExistSRouteSeq : snprintf(szBuf,sizeof(szBuf)-1,"FailRouteSeq_ExistSRouteSeq"); break;
case E_RTE_CheckRouteSeq_SameAsExistingValue : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_SameAsExistingValue"); break;
case E_RTE_CheckRouteSeq_InvalidDesc : snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_InvalidDesc"); break;
case E_RTE_CheckRouteSeq_ExistRouteOp: snprintf(szBuf,sizeof(szBuf)-1,"CheckRouteSeq_ExistRouteOp"); break;
//special route seq  
}else if( nLevel == E_RTE_CheckSRouteSeq_InvalidID ){
case E_RTE_CheckSRouteSeq_InvalidID : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_InvalidID"); break;
case E_RTE_CheckSRouteSeq_DuplicateID : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_DuplicateID"); break;
case E_RTE_CheckSRouteSeq_ExceedMaxRecord : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_ExceedMaxRecord"); break;
case E_RTE_CheckSRouteSeq_NotFoundID : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_NotFoundID"); break;
case E_RTE_CheckSRouteSeq_InvalidRate : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_InvalidRate"); break;
case E_RTE_CheckSRouteSeq_FirstRSeqZero : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_FirstRSeqZero"); break;
case E_RTE_CheckSRouteSeq_InvalidRSeq : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_InvalidRSeq"); break;
case E_RTE_CheckSRouteSeq_NotFoundRSeq : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_NotFoundRSeq"); break;
case E_RTE_CheckSRouteSeq_OutofValidRange_RSeq : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_OutofValidRange_RSeq"); break;
case E_RTE_CheckSRouteSeq_OutofValidRange_Rate : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_OutofValidRange_Rate"); break;
case E_RTE_CheckSRouteSeq_SameAsExistingValue : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_SameAsExistingValue"); break;
case E_RTE_CheckSRouteSeq_DuplicateRSeq : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_DuplicateRSeq"); break;
case E_RTE_CheckSRouteSeq_InvalidDesc : snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_InvalidDesc"); break;
case E_RTE_CheckSRouteSeq_ExistRouteOp: snprintf(szBuf,sizeof(szBuf)-1,"CheckSRouteSeq_ExistRouteOp"); break;
//special route seq  
//trgw
}else if( nLevel == E_RTE_CheckTrGW_InvalidID ){
case E_RTE_CheckTrGW_InvalidID: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_InvalidID"); break;
case E_RTE_CheckTrGW_InvalidSvrIP4: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_InvalidSvrIP4"); break;
case E_RTE_CheckTrGW_InvalidTrGWIP4: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_InvalidTrGWIP4"); break;
case E_RTE_CheckTrGW_InvalidSvrPort: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_InvalidSvrPort"); break;
case E_RTE_CheckTrGW_InvalidTrGWPort: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_InvalidTrGWPort"); break;
case E_RTE_CheckTrGW_InvalidCallRate: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_InvalidCallRate"); break;
case E_RTE_CheckTrGW_InvalidMaxCnt: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_InvalidMaxCnt"); break;
case E_RTE_CheckTrGW_InvalidStatus: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_InvalidStatus"); break;
case E_RTE_CheckTrGW_NoModSvrIP: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_NoModSvrIP"); break;
case E_RTE_CheckTrGW_NoModTrGWIP: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_NoModTrGWIP"); break;
case E_RTE_CheckTrGW_NoModSvrPort: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_NoModSvrPort"); break;
case E_RTE_CheckTrGW_NoModTrGWPort: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_NoModTrGWPort"); break;
case E_RTE_CheckTrGW_SameAsExistingValue: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_SameAsExistingValue"); break;
case E_RTE_CheckTrGW_DuplicateID: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_DuplicateID"); break;
case E_RTE_CheckTrGW_ExceedMaxRecord: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_ExceedMaxRecord"); break;
case E_RTE_CheckTrGW_DuplicateSvrIP_Port: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_DuplicateSvrIP_Port"); break;
case E_RTE_CheckTrGW_DuplicateTrGWIP_Port: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_DuplicateTrGWIP_Port"); break;
case E_RTE_CheckTrGW_NotFoundID: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_NotFoundID"); break;
case E_RTE_CheckTrGW_UsingSession: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_UsingSession"); break;
case E_RTE_CheckTrGW_NoModStatus: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_NoModStatus"); break;
case E_RTE_FailGetTrGW_Unavailable_Status: snprintf(szBuf,sizeof(szBuf)-1,"FailGetTrGW_Unavailable_Status"); break;
case E_RTE_CheckTrGW_InvalidDesc: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrGW_InvalidDesc"); break;
//check reason code 
}else if( nLevel == E_RTE_CheckReason_InvalidID ){
case E_RTE_CheckReason_InvalidID: snprintf(szBuf,sizeof(szBuf)-1,"CheckReason_InvalidID"); break;
case E_RTE_CheckReason_CauseCode: snprintf(szBuf,sizeof(szBuf)-1,"CheckReason_CauseCode"); break;
case E_RTE_CheckReason_RspCode: snprintf(szBuf,sizeof(szBuf)-1,"CheckReason_RspCode"); break;
case E_RTE_CheckReason_SameAsExistingValue: snprintf(szBuf,sizeof(szBuf)-1,"CheckReason_SameAsExistingValue"); break;
case E_RTE_CheckReason_DuplicateID: snprintf(szBuf,sizeof(szBuf)-1,"CheckReason_DuplicateID"); break;
case E_RTE_CheckReason_ExceedMaxRecord: snprintf(szBuf,sizeof(szBuf)-1,"CheckReason_ExceedMaxRecord"); break;
case E_RTE_CheckReason_NotFoundID: snprintf(szBuf,sizeof(szBuf)-1,"CheckReason_NotFoundID"); break;
case E_RTE_CheckReason_InvalidDesc: snprintf(szBuf,sizeof(szBuf)-1,"CheckReason_InvalidDesc"); break;
//check env
}else if( nLevel == E_RTE_CheckEnv_InvalidMaxSES ){
case E_RTE_CheckEnv_InvalidMaxSES: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxSES"); break;
case E_RTE_CheckEnv_InvalidMaxMSG: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxMSG"); break;
case E_RTE_CheckEnv_InvalidMaxCPS: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxCPS"); break;
case E_RTE_CheckEnv_InvalidMaxCPU: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxCPU"); break;
case E_RTE_CheckEnv_InvalidMaxMEM: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxMEM"); break;
case E_RTE_CheckEnv_InvalidMaxSES_Flag: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxSES_Flag"); break;
case E_RTE_CheckEnv_InvalidMaxCPS_Flag: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxCPS_Flag"); break;
case E_RTE_CheckEnv_InvalidMaxCPU_Flag: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxCPU_Flag"); break;
case E_RTE_CheckEnv_InvalidMaxMEM_Flag: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxMEM_Flag"); break;
case E_RTE_CheckEnv_SameAsExistingValue: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_SameAsExistingValue"); break;
case E_RTE_CheckEnv_InvalidDSCP: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidDSCP"); break;
case E_RTE_CheckEnv_InvalidMaxMSG_Flag: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxMSG_Flag"); break;
case E_RTE_CheckEnv_InvalidMaxTPS: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxTPS"); break;
case E_RTE_CheckEnv_InvalidMaxTPS_Flag: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxTPS_Flag"); break;
case E_RTE_CheckEnv_InvalidMinCPS_Emer: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMinCPS_Emer"); break;
case E_RTE_CheckEnv_InvalidMaxCPS_Audio: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxCPS_Audio"); break;
case E_RTE_CheckEnv_InvalidMaxCPS_Video: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidMaxCPS_Video"); break;
case E_RTE_CheckEnv_InvalidCPSClass_Flag: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidCPSClass_Flag"); break;
case E_RTE_CheckEnv_InvalidSumOfLevelCPS: snprintf(szBuf,sizeof(szBuf)-1,"CheckEnv_InvalidSumOfLevelCPS"); break;
//decision 
}else if( nLevel == E_RTE_FailGetICRoute_NotFoundLocalNode ){
case E_RTE_FailGetICRoute_NotFoundLocalNode: snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_NotFoundLocalNode"); break;
case E_RTE_FailGetICRoute_NotFoundRemoteNode: snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_NotFoundRemoteNode"); break;
case E_RTE_FailGetICRoute_NotFoundRoute: snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_NotFoundRoute"); break;
case E_RTE_FailGetICRoute_UnavalibleRoute: snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_UnavalibleRoute"); break;
case E_RTE_FailGetOGRoute_NotFoundRSeq: snprintf(szBuf,sizeof(szBuf)-1,"FailGetOGRoute_NotFoundRSeq"); break;
case E_RTE_FailGetOGRoute_NotFoundDRTE: snprintf(szBuf,sizeof(szBuf)-1,"FailGetOGRoute_NotFoundDRTE"); break;
case E_RTE_FailGetOGRoute_NotFoundARTE: snprintf(szBuf,sizeof(szBuf)-1,"FailGetOGRoute_NotFoundARTE"); break;
case E_RTE_FailGetOGRoute_UnavalibleRoute: snprintf(szBuf,sizeof(szBuf)-1,"FailGetOGRoute_UnavalibleRoute"); break;
case E_RTE_FailGetOGRoute_NotFoundLocalNode: snprintf(szBuf,sizeof(szBuf)-1,"FailGetOGRoute_NotFoundLocalNode"); break;
case E_RTE_FailGetOGRoute_NotFoundRemoteNode: snprintf(szBuf,sizeof(szBuf)-1,"FailGetOGRoute_NotFoundRemoteNode"); break;
case E_RTE_FailGetOGRoute_NotFoundSpecialRSeq: snprintf(szBuf,sizeof(szBuf)-1,"FailGetOGRoute_NotFoundSpecialRSeq"); break;
case E_RTE_FailGetOGRoute_InvaildSpecialRSeq: snprintf(szBuf,sizeof(szBuf)-1,"FailGetOGRoute_InvaildSpecialRSeq"); break;
case E_RTE_FailGetICRoute_OverLoadGroupCPS: snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_OverLoadGroupCPS"); break;
case E_RTE_FailGetICRoute_OverLoadGlobalCPS: snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_OverLoadGlobalCPS"); break;
case E_RTE_FailGetICRoute_OverLoadCPU: snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_OverLoadCPU"); break;
case E_RTE_FailGetICRoute_OverLoadMEM: snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_OverLoadMEM"); break;
case E_RTE_FailGetICRoute_OverLoadGroupTPS: snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_OverLoadGroupTPS"); break;
case E_RTE_FailGetICRoute_OverLoadGlobalTPS: snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_OverLoadGlobalTPS"); break;
case E_RTE_FailGetICRoute_OverLoadEmergency: snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_OverLoadEmergency"); break;
case E_RTE_FailGetICRoute_OverLoadAudio:     snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_OverLoadAudio"); break;
case E_RTE_FailGetICRoute_OverLoadVideo:     snprintf(szBuf,sizeof(szBuf)-1,"FailGetICRoute_OverLoadVideo"); break;
//check timer
}else if( nLevel == E_RTE_CheckTimer_SameAsExistingValue){
case E_RTE_CheckTimer_SameAsExistingValue: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_SameAsExistingValue"); break;
case E_RTE_CheckTimer_InvalidT1: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidT1"); break;
case E_RTE_CheckTimer_InvalidT2: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidT2"); break;
case E_RTE_CheckTimer_InvalidT4: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidT4"); break;
case E_RTE_CheckTimer_InvalidTA: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTA"); break;
case E_RTE_CheckTimer_InvalidTB: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTB"); break;
case E_RTE_CheckTimer_InvalidTC: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTC"); break;
case E_RTE_CheckTimer_InvalidTD: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTD"); break;
case E_RTE_CheckTimer_InvalidTE: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTE"); break;
case E_RTE_CheckTimer_InvalidTF: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTF"); break;
case E_RTE_CheckTimer_InvalidTG: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTG"); break;
case E_RTE_CheckTimer_InvalidTH: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTH"); break;
case E_RTE_CheckTimer_InvalidTI: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTI"); break;
case E_RTE_CheckTimer_InvalidTJ: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTJ"); break;
case E_RTE_CheckTimer_InvalidTK: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTK"); break;
case E_RTE_CheckTimer_InvalidTL: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTL"); break;
case E_RTE_CheckTimer_InvalidTM: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTM"); break;
case E_RTE_CheckTimer_InvalidDAlive: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidDAlive"); break;
case E_RTE_CheckTimer_InvalidDTerm: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidDTerm"); break;
case E_RTE_CheckTimer_Invalid1xxToRecvPRACK: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_Invalid1xxToRecvPRACK"); break;
case E_RTE_CheckTimer_InvalidWaitForPRACK: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidWaitForPRACK"); break;
case E_RTE_CheckTimer_InvalidTrGW: snprintf(szBuf,sizeof(szBuf)-1,"CheckTimer_InvalidTrGW"); break;
//check trace
}else if( nLevel == E_RTE_CheckTrace_DuplicateID){
case E_RTE_CheckTrace_DuplicateID: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrace_DuplicateID"); break;
case E_RTE_CheckTrace_ExceedMaxRecord: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrace_ExceedMaxRecord"); break;  
case E_RTE_CheckTrace_NotFoundID: snprintf(szBuf,sizeof(szBuf)-1,"CheckTrace_NotFoundID"); break;

//check emer
}else if( nLevel == E_RTE_CheckEmer_DuplicateID){
case E_RTE_CheckEmer_DuplicateID:          snprintf(szBuf,sizeof(szBuf)-1,"CheckEmer_DuplicateID"); break;
case E_RTE_CheckEmer_SameAsExistingValue:  snprintf(szBuf,sizeof(szBuf)-1,"CheckEmer_SameAsExistingValue"); break;
case E_RTE_CheckEmer_InvalidID:            snprintf(szBuf,sizeof(szBuf)-1,"CheckEmer_InvalidID"); break;
case E_RTE_CheckEmer_InvalidDesc:          snprintf(szBuf,sizeof(szBuf)-1,"CheckEmer_InvalidDesc"); break;
case E_RTE_CheckEmer_InvalidMIN:           snprintf(szBuf,sizeof(szBuf)-1,"CheckEmer_InvalidMIN"); break;
case E_RTE_CheckEmer_DuplicateMIN:         snprintf(szBuf,sizeof(szBuf)-1,"CheckEmer_DuplicateMIN"); break;
case E_RTE_CheckEmer_ExceedMaxRecord:      snprintf(szBuf,sizeof(szBuf)-1,"CheckEmer_ExceedMaxRecord"); break;
case E_RTE_CheckEmer_NotFoundID:           snprintf(szBuf,sizeof(szBuf)-1,"CheckEmer_NotFoundID"); break;
case E_RTE_CheckEmer_NoModMIN:             snprintf(szBuf,sizeof(szBuf)-1,"CheckEmer_NoModMIN"); break;
}else{
   snprintf(szBuf,sizeof(szBuf)-1,"Unknown Route Error"); break;
}
default:
   snprintf(szBuf,sizeof(szBuf)-1,"Unknown Route Error"); break;
}
snprintf(_pErrStr,_nLen-1,"%s",szBuf);

return _pErrStr;
}

#endif
