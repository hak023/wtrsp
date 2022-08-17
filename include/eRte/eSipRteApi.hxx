/**
 * @file  eSipRteApi.h
 * the sip route api header
 *
 * @attention Copyright (c) 2015 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2015.01.30   hc_yun   1.0     first written
 * @par 2015.03.27   hc_yun   1.1     add IfESipRteApi_ChangeHAStandby2Active
 * @par 2015.04.07   hc_yun   1.2     add IfESipRteApi_GetLocalByTransport
 *                                    add IfESipRteApi_GetRemoteByTransport
 *                                    add IfESipRteApi_GetRouteByLIDAndRID
 * @par 2015.04.09   hc_yun   1.3     add IfESipRteApi_RecvInitMsgByRouteID
 *                                    add IfESipRteApi_GetReasonByRouteID
 * @par 2015.04.14   hc_yun   1.4     add param check status in IfESipRteApi_RecvInitMsgByTransport
 *                                    add param check status in IfESipRteApi_CheckTPSByTransport
 *                                    add param check status in IfESipRteApi_GetRouteByTransport
 *                                    add CallBack PfuncESipRteApiChangeTimer_t
 *                                    add Modify/Find/GetAll SipCoreTimer_t
 *                                    add Check Trace, IfESipRteApi_CheckTrace
 * @par 2015.04.15   hc_yun   1.5     add param cur/max cps...in CPS/TPS/Load func
 * @par 2015.04.30   hc_yun   1.6     mod return IfESipRteApi_RecvOption (ret=0, 2, else..)
 *                                    add check msg size
 *                                    add param(_bOnlyTCP) in the IfESipRteApi_DecisionTarget x 4
 * @par 2015.05.07   hc_yun   1.7     add rout&ses count zero i/f
 *                                        - IfESipRteApi_SetZeroRouteCount_All
 *                                        - IfESipRteApi_SetZeroSesCount
 * @par 2015.05.19   hc_yun   1.8     mod RecvOption/RecvOption200OK param. IN -> IN/OUT
 * @par 2015.05.26   hc_yun   1.9     mod IfESipRteApi_CheckTrace
 *                                    add IfESipRteApi_FindTCPRoute
 * @par 2015.06.03   hc_yun   2.0     add a param(_bOKRsp) to the RecvOption200OK 
 * @par 2015.11.02   hc_yun   2.1     add Emergency, 
 *                                    mod param _pszMIN, _bAudio
 *                                        IfESipRteApi_RecvInitMsgByTransport,IfESipRteApi_RecvInitMsgByRouteID
 * @par 2015.11.07   hc_yun   2.2     modify  timer callback
 * @par 2016.02.22   hc_yun   2.3     @R106 20160222  
 *                                    add reason to PfuncESipRteApiNotify_t
 *                                    add reason to PfuncESipRteApiNotifyChangeRouteStatus_t
 *                                    add nResponseCode to IfESipRteApi_RecvOption200OK
 *                                    add ALL DEACT to MMI I/F
 * @par 2016.11.30   hc_yun   2.4     add func. for vibc @vibc
 * @par 2017.04.17   hc_yun   2.5     add func. IfESipRteApi_SetLocalStatusByTransport
 */

#ifndef __ESIP_CORE_SIP_ROUTE_API__
#define __ESIP_CORE_SIP_ROUTE_API__


#include "eSipRte.hxx"

///defines the api log level
enum ESipRteApiLogLevel_t{
   E_ESipRteLogLevel_CRI=0,//critical
   E_ESipRteLogLevel_ERR=1,//error
   E_ESipRteLogLevel_WAR=2,//warning
   E_ESipRteLogLevel_INF=3,//info
   E_ESipRteLogLevel_DEB=4,//debug
   E_ESipRteLogLevel_MAX=5
};

//add @vibc
enum ESipRteApiAttachSystem_t{
   E_ESipRteAttachSystem_LB=0,
   E_ESipRteAttachSystem_CM=1
};

enum ESipRteApiAttachModule_t{
   E_ESipRteAttachModule_OAM=0,
   E_ESipRteAttachModule_SVC=1
};

enum ESipRteApiSystemHAStatus_t{
   E_ESipRteSystemHAStatus_ACTIVE=0,
   E_ESipRteSystemHAStatus_STANDBY=1
};

///when occured option timeout, will invoke this func.
//return    0 : sucess
//       != 0 : fail
typedef int (*PfuncESipRteApiSendOption_t)(
   SipRoute_t*       _pstRte,
   SipNodeLocal_t*   _pstLocal,
   SipNodeRemote_t*  _pstRemote);

///when occured update timeout, will invoke this func.
//return  not used
typedef int (*PfuncESipRteApiSendUpdate_t)(
   SipRoute_t*       _pstRte,
   SipNodeLocal_t*   _pstLocal,
   SipNodeRemote_t*  _pstRemote);

///when a route status changed, will invoke this func.
//return not used
typedef int (*PfuncESipRteApiNotifyChangeRouteStatus_t)(
   int               _nAvail, //1:AVAILABLE, 0: UNVAILABLE
   SipRoute_t*       _pstRte,
   SipNodeLocal_t*   _pstLocal,
   SipNodeRemote_t*  _pstRemote,
//add 2.3 @R106 20160222
   char*             _pFailReason); //MAX SIZE:128

///when a trgw status changed, will invoke this func.
//return not used
typedef int (*PfuncESipRteApiNotifyTrGWStatus_t)(
   int               _nAvail, //1:AVAILABLE, 0: UNVAILABLE
   TrGWRoute_t*      _pstTrGW,
//add 2.3 @R106 20160222
   char*             _pFailReason); //MAX SIZE:128

///when add/del local node, will invoke this func.
//return    0 : sucess
//       != 0 : fail. will add fail.
typedef int (*PfuncESipRteApiChangeLocalNode_t)(
   int               _nIsDelFlag, //0: add, 1: delete
   SipNodeLocal_t*   _pstLocal);

///the api log print
typedef void (*PfuncESipRteApiPrintLog_t)(
   int      _nLevel,
   char*    _pMsg);

///mod 2.2
typedef void (*PfuncESipRteApiChangeTimer_t)(
   SipCoreTimer_t*   _pstTimer,
   bool              _bChangeTrGWTimer);

//add 2.3 @R106 20160222
typedef int (*PfuncESipRteApiNotifySystemResource_t)(
   int               _nAlarmFlag,    //1:occured alarm, 0:alarm cleared
   int               _nResourceFlag, //0:CPU, 1:MEM
   int               _nCurUsage,     //Current Usage(%)
   int               _nLimitUsage,   //Limit Usage(%). set by mmi
   char*             _pFailReason);  //MAX SIZE: 128

///defines the api callback
struct ESipRteApiCallBack_t{
   PfuncESipRteApiSendOption_t                  m_pfuncESipRteApiSendOption;
   PfuncESipRteApiSendUpdate_t                  m_pfuncESipRteApiSendUpdate;
   PfuncESipRteApiNotifyChangeRouteStatus_t     m_pfuncESipRteApiNotifyChangeRouteStatus;
   PfuncESipRteApiNotifyTrGWStatus_t            m_pfuncESipRteApiNotifyTrGWStatus;
   PfuncESipRteApiChangeLocalNode_t             m_pfuncESipRteApiChangeLocalNode;
   PfuncESipRteApiPrintLog_t                    m_pfuncESipRteApiPrintLog;
   PfuncESipRteApiChangeTimer_t                 m_pfuncESipRteApiChangeTimer;
   //add 2.3 @R106 20160222
   PfuncESipRteApiNotifySystemResource_t        m_pfuncESipRteApiNotifySystemResource_t;
}; 
//the retrun value is applied in common.
//return    0: sucess 
//       !=0 : fail. see errorcode in eSipRteErr.hxx

///loglevel Change
int IfESipRteApi_ChangeLogLevelResip2Api(int nLevel);
int IfESipRteApi_ChangeLogLevelAip2Resip(int nLevel);

///initialize
int IfESipRteApi_Init(ESipRteApiCallBack_t* _pstCB, int _nLevel);
int IfESipRteApi_InitForOnlyDisplay(ESipRteApiCallBack_t* _pstCB=NULL);
//add @vibc
int IfESipRteApi_Init(ESipRteApiCallBack_t* _pstCB,
                      int _nLevel,
                      int _nAttachSystem, //0:LB, 1:CM
                      int _nAttachModule, //0:VLM, 1:CM,LB
                      int _nHAStatus,     //0:ACTIVE, 1:STANDBY
                      char* _pStrDir=NULL);
//add @vibc
void IfESipRteApi_SetHAStatus(int _nHAStatus); //0:ACTIVE, 1:STANDBY
int  IfESipRteApi_SetRouteStatusByRouteID(
         int                 _nRouteID,      //IN: Route ID
         int                 _nStatus,       //IN: RouteStatus. 0:AVAIL, 1:UNAVAIL
         char*               _pError=NULL);  //OUT, error string
int IfESipRteApi_SetRouteStatusByTransport(
         SipNodeLocal_t*     _pstLocal,      //IN/OUT, ip/port/transport type
         SipNodeRemote_t*    _pstRemote,     //IN/OUT, ip/port/transport type
         int                 _nStatus,       //IN: RouteStatus 0:AVAIL, 1:UNAVAIL
         char*               _pError=NULL);  //OUT, error string

//add callback
int IfESipRteApi_SetUpSendOption(PfuncESipRteApiSendOption_t _pFunc);
int IfESipRteApi_SetUpSendUpdate(PfuncESipRteApiSendUpdate_t _pFunc);
int IfESipRteApi_SetUpNotifyChangeRouteStatus(PfuncESipRteApiNotifyChangeRouteStatus_t _pFunc);
int IfESipRteApi_NotifyTrGWStatus(PfuncESipRteApiNotifyTrGWStatus_t _pFunc);
int IfESipRteApi_ChangeLocalNode(PfuncESipRteApiChangeLocalNode_t _pFunc);
int IfESipRteApi_PrintLog(PfuncESipRteApiPrintLog_t _pFunc);

int IfESipRteApi_SetLogLevel(int _nLevel);
//////////////Using MMI
///add
int IfESipRteApi_AddLocal( SipNodeLocal_t* _pstValue,SipNodeLocal_t* _pstResult,
                           char* _pError=NULL);
int IfESipRteApi_AddRemote(SipNodeRemote_t* _pstValue,SipNodeRemote_t* _pstResult,
                           char* _pError=NULL);
int IfESipRteApi_AddRouteGroup(SipRouteGroup_t* _pstValue,SipRouteGroup_t* _pstResult,
                               char* _pError=NULL);
int IfESipRteApi_AddRoute( SipRoute_t* _pstValue,SipRoute_t* _pstResult,
                           char* _pError=NULL);
int IfESipRteApi_AddRouteSeq( SipRouteSeq_t* _pstValue,SipRouteSeq_t* _pstResult,
                              char* _pError=NULL);
int IfESipRteApi_AddSpecialRouteSeq(SipSpecialRouteSeq_t* _pstValue,
                              SipSpecialRouteSeq_t* _pstResult,char* _pError=NULL);
//add 2.1
int IfESipRteApi_AddEmer(EmergencyCall_t* _pstValue,
                         EmergencyCall_t* _pstResult,char* _pError=NULL);
//not using mmi. only dis, mod
int IfESipRteApi_AddTrGW(TrGWRoute_t* _pstValue,TrGWRoute_t* _pstResult,char* _pError=NULL);
int IfESipRteApi_AddReason(SipReasonCode_t* _pstValue,SipReasonCode_t* _pstResult,char* _pError=NULL);

//modify
int IfESipRteApi_ModLocal(SipNodeLocal_t* _pstValue,SipNodeLocal_t* _pstResult,
                          char* _pError=NULL);
int IfESipRteApi_ModRemote(SipNodeRemote_t* _pstValue,SipNodeRemote_t* _pstResult,
                          char* _pError=NULL);
int IfESipRteApi_ModRouteGroup(SipRouteGroup_t* _pstValue,SipRouteGroup_t* _pstResult,
                          char* _pError=NULL);
int IfESipRteApi_ModRoute( SipRoute_t* _pstValue,SipRoute_t* _pstResult,
                          char* _pError=NULL);
//add 2.3 @R106 20160222 ALL DEACT by GroupID
int IfESipRteApi_Control_AllRoute(  int _nActFlag,          /* IN    ACT:1, DEACT:0 */
                                    int _nGroupID,          /* IN    Route Group ID */
                                    bool _bEnableGroup,     /* IN    whether control by the group, or all */
                                    bool _bIncludeBypass,   /* IN    whether include BYPASS MODE, or not */
                                    char* _pError=NULL);    /* OUT   error string */
int IfESipRteApi_ModRouteSeq(SipRouteSeq_t* _pstValue,SipRouteSeq_t* _pstResult,
                          char* _pError=NULL);
int IfESipRteApi_ModSpecialRouteSeq(SipSpecialRouteSeq_t* _pstValue,
                          SipSpecialRouteSeq_t* _pstResult, char* _pError=NULL);
int IfESipRteApi_ModTrGW(TrGWRoute_t* _pstValue,TrGWRoute_t* _pstResult,
                           char* _pError=NULL);
int IfESipRteApi_ModReason(SipReasonCode_t* _pstValue,SipReasonCode_t* _pstResult,
                           char* _pError=NULL);
int IfESipRteApi_ModEnv(GlobalOverLoadEnv_t* _pstValue,GlobalOverLoadEnv_t* _pstResult,
                           char* _pError=NULL);
//@vibc
int IfESipRteApi_ModEnv_LB(GlobalOverLoadEnv_t* _pstValue,GlobalOverLoadEnv_t* _pstResult,
                           char* _pError=NULL);
int IfESipRteApi_ModTimer(SipCoreTimer_t* _pstValue,SipCoreTimer_t* _pstResult,
                           char* _pError=NULL);
//add 2.1
int IfESipRteApi_ModEmer(EmergencyCall_t* _pstValue,EmergencyCall_t* _pstResult,
                           char* _pError=NULL);
//find
int IfESipRteApi_FindLocal(unsigned int _uiID, SipNodeLocal_t* _pstValue);
int IfESipRteApi_FindRemote(unsigned int _uiID, SipNodeRemote_t* _pstValue);
int IfESipRteApi_FindRoute(unsigned int _uiID, SipRoute_t* _pstValue);
int IfESipRteApi_FindRouteGroup(unsigned int _uiID, SipRouteGroup_t* _pstValue);
int IfESipRteApi_FindRouteSeq(unsigned int _uiID, SipRouteSeq_t* _pstValue);
int IfESipRteApi_FindSpecialRouteSeq(unsigned int _uiID, SipSpecialRouteSeq_t* _pstValue);
int IfESipRteApi_FindTrGW(unsigned int _uiID, TrGWRoute_t* _pstValue);
int IfESipRteApi_FindReason(unsigned int _uiID, SipReasonCode_t* _pstValue);
int IfESipRteApi_FindEnv(GlobalOverLoadEnv_t* _pstValue);
//@vibc
int IfESipRteApi_FindEnv_LB(GlobalOverLoadEnv_t* _pstValue);
int IfESipRteApi_FindTimer(SipCoreTimer_t* _pstValue);
//add 2.1
int IfESipRteApi_FindEmer(unsigned int _uiID,EmergencyCall_t* _pstValue);
//del
int IfESipRteApi_DelLocal(unsigned int _uiID, char* _pError=NULL);
int IfESipRteApi_DelRemote(unsigned int _uiID, char* _pError=NULL);
int IfESipRteApi_DelRoute(unsigned int _uiID, char* _pError=NULL);
int IfESipRteApi_DelRouteGroup(unsigned int _uiID, char* _pError=NULL);
int IfESipRteApi_DelRouteSeq(unsigned int _uiID, char* _pError=NULL);
int IfESipRteApi_DelSpecialRouteSeq(unsigned int _uiID, char* _pError=NULL);
int IfESipRteApi_DelTrGW(unsigned int _uiID, char* _pError=NULL);
int IfESipRteApi_DelReason(unsigned int _uiID, char* _pError=NULL);
//add 2.1
int IfESipRteApi_DelEmer(unsigned int _uiID, char* _pError=NULL);
//dis
int IfESipRteApi_GetAllLocal(SipLocalNodeDataBase_t* _pstData);
int IfESipRteApi_GetAllRemote(SipRemoteNodeDataBase_t* _pstData);
int IfESipRteApi_GetAllRoute(SipRouteDataBase_t* _pstData);
int IfESipRteApi_GetAllRouteGroup(SipRouteGroupDataBase_t* _pstData);
int IfESipRteApi_GetAllRouteSeq(SipRouteSeqDataBase_t* _pstData);
int IfESipRteApi_GetAllSpecialRouteSeq(SipSpecialRouteSeqDataBase_t* _pstData);
int IfESipRteApi_GetAllTrGW(TrGWDataBase_t* _pstData);
int IfESipRteApi_GetAllReason(SipReasonCodeDataBase_t* _pstData);
int IfESipRteApi_GetAllEnv(GlobalOverLoadEnv_t* _pstData);
//@vibc
int IfESipRteApi_GetAllEnv_LB(GlobalOverLoadEnv_t* _pstData);
int IfESipRteApi_GetAllTimer(SipCoreTimer_t* _pstData);
int IfESipRteApi_GetAllTrace(SipTraceDataBase_t* _pstData);
//add 2.1
int IfESipRteApi_GetAllEmer(EmergencyCallDataBase_t* _pstData);

/////////////////////////////////using application
//trgw
int IfESipRteApi_DecisionTrGW(
         TrGWRoute_t*         _pstTrGW,           //OUT,
         char*                _pError=NULL);      //OUT
int IfESipRteApi_SetTrGWStatus(
         char*          _szSvrIP,        //IN, TCP Server IP,   0
         int            _nSvrPort,       //IN, TCP Server Port, M
         char*          _szTrGWIP,       //IN, TCP Client IP,   O
         int            _nTrGWPort,      //IN, TCP Client Port. O
         bool           _bIsConnect);    //IN, true=Connect, false=disconnect
void IfESipRteApi_InCrease1TrGW(
         char*          _szSvrIP,        //IN, TCP Server IP,  
         int            _nSvrPort,       //IN, TCP Server Port,
         char*          _szTrGWIP,       //IN, TCP Client IP
         int            _nTrGWPort);     //IN, TCP Client Port
void IfESipRteApi_DecreaseCrease1TrGW(
         char*          _szSvrIP,        //IN, TCP Server IP
         int            _nSvrPort,       //IN, TCP Server Port
         char*          _szTrGWIP,       //IN, TCP Client IP
         int            _nTrGWPort);     //IN, TCP Client Port
void IfESipRteApi_SetCountTrGW(
         char*          _szSvrIP,       //IN, TCP Server IP
         int            _nSvrPort,      //IN, TCP Server Port
         char*          _szTrGWIP,      //IN, TCP Client IP
         int            _nTrGWPort,     //IN, TCP Client IP
         int            _nCnt);         //IN, TCP Count

/////////////////////////////////using sip core
//if core recv opt 200OK,this func. invoke
void IfESipRteApi_RecvOption200OK(
         //@ver1.8 local,Remote -> IN/OUT
         SipNodeLocal_t* _pLocal,   //IN/OUT ip/port/transport type
         SipNodeRemote_t* _pRemote, //IN/OUT ip/port/transport type
         //ver2.0  whether the response of option is ok or not
         bool _bOKRsp,              //IN 200 OK or not
         //add param @R106 201603
         int  _nResponseCode);      //Error Code
         

//mo
//if core recv opt,this func. invoke
// return 0 : send 200 OK
// return 2 : don't send 200 OK -> will drop
// return != 0 or 2, should be send 500 error.
int IfESipRteApi_RecvOption(
         //@ver1.8 local,Remote -> IN/OUT
         SipNodeLocal_t* _pLocal,      //IN/OUT, ip/port/transport type
         SipNodeRemote_t* _pRemote);   //IN/OUT, ip/port/transport type
//without ic info
int IfESipRteApi_DecisionTargetByRouteSeq(
         unsigned int         _uiRSeqID,          //IN - route squence id
         SipNodeLocal_t*      _pstOGLocal,        //OUT, target local
         SipNodeRemote_t*     _pstOGRemote,       //OUT, target remote
         SipRoute_t*          _pstOGRoute=NULL,   //OUT, og route
         char*                _pError=NULL,       //OUT, error string
         bool                 _bOnlyTCP=false);   //IN,  only transport protocol tcp
//with ic info
int IfESipRteApi_DecisionTargetByRouteSeq(
         unsigned int         _uiRSeqID,          //IN - route squence id
         SipNodeLocal_t*      _pstICLocal,        //IN/OUT, ip/port/transport type
         SipNodeRemote_t*     _pstICRemote,       //IN/OUT, ip/port/transport type
         SipNodeLocal_t*      _pstOGLocal,        //OUT, target local
         SipNodeRemote_t*     _pstOGRemote,       //OUT, target remote
         SipRoute_t*          _pstICRoute=NULL,   //OUT, ic route
         SipRoute_t*          _pstOGRoute=NULL,   //OUT, og route
         char*                _pError=NULL,       //OUT, error string
         bool                 _bOnlyTCP=false);   //IN,  only transport protocol tcp
//without ic info
int IfESipRteApi_DecisionTargetBySpecialRouteSeq(
         unsigned int         _uiSRSeqID,         //IN - special route squence id
         SipNodeLocal_t*      _pstOGLocal,        //OUT, target local
         SipNodeRemote_t*     _pstOGRemote,       //OUT, target remote
         SipRoute_t*          _pstOGRoute=NULL,   //OUT, og route
         char*                _pError=NULL,       //OUT, error string
         bool                 _bOnlyTCP=false);   //IN,  only transport protocol tcp
//with ic info
int IfESipRteApi_DecisionTargetBySpecialRouteSeq(
         unsigned int         _uiSRSeqID,         //IN - special route squence id
         SipNodeLocal_t*      _pstICLocal,        //IN/OUT, ip/port/transport type
         SipNodeRemote_t*     _pstICRemote,       //IN/OUT, ip/port/transport type
         SipNodeLocal_t*      _pstOGLocal,        //OUT, target local
         SipNodeRemote_t*     _pstOGRemote,       //OUT, target remote
         SipRoute_t*          _pstICRoute=NULL,   //OUT, ic route
         SipRoute_t*          _pstOGRoute=NULL,   //OUT, og route
         char*                _pError=NULL,       //OUT, error string
         bool                 _bOnlyTCP=false);   //IN,  only transport protocol tcp
//mod 1.4
int IfESipRteApi_GetRouteByTransport(
         SipNodeLocal_t*     _pstLocal,                //IN/OUT, ip/port/transport type
         SipNodeRemote_t*    _pstRemote,               //IN/OUT, ip/port/transport type
         SipRoute_t*         _pstRoute,                //OUT, route info
         bool                _bCheckRouteStatus=true,  //IN, check route status
         char*               _pError=NULL);            //OUT, error string
//add 1.2
int IfESipRteApi_GetLocalByTransport(
         SipNodeLocal_t*     _pstLocal,           //IN/OUT, ip/port/transport type
         char*               _pError=NULL);       //OUT, error string
//add 2.5 @vibc
int IfESipRteApi_SetLocalStatusByTransport(
         SipNodeLocal_t*     _pstLocal,        //IN/OUT, ip/port/transport type
         int                 _nStatus,         //INT, AVAIL(0), UNAVAIL(1)
         char*               _pError=NULL);    //OUT, error string
//add 1.2
int IfESipRteApi_GetRemoteByTransport(
         SipNodeRemote_t*     _pstRemote,         //IN/OUT, ip/port/transport type
         char*               _pError=NULL);       //OUT, error string
//add 1.2
int IfESipRteApi_GetRouteByLIDAndRID(
         unsigned  int       _uiLID,              //IN , Local ID
         unsigned  int       _uiRID,              //IN , Remote ID
         SipRoute_t*         _pstRoute,           //OUT, route info
         char*               _pError=NULL);       //OUT, error string


//check cps
//add 2.1
//cps err: E_RTE_FailGetICRoute_OverLoadGroupCPS or E_RTE_FailGetICRoute_OverLoadGlobalCPS
//         E_RTE_FailGetICRoute_OverLoadEmergency or E_RTE_FailGetICRoute_OverLoadAudio
//         E_RTE_FailGetICRoute_OverLoadVideo
//acl err: other error
//mod 1.4
int IfESipRteApi_RecvInitMsgByTransport(
         SipNodeLocal_t*     _pstLocal,                //IN/OUT, ip/port/transport type
         SipNodeRemote_t*    _pstRemote,               //IN/OUT, ip/port/transport type
         SipRoute_t*         _pstRoute,                //OUT, route info
         int&                _rnCurCPS,                //OUT 
         int&                _rnMaxCPS,                //OUT
         bool                _bCheckRouteStatus=true,  //IN, check route status
         bool                _bPlusCnt=true,           //IN, InCrease Busy Count
         //add 2.2
         char*               _pszMIN=NULL,             //IN, Called min
         bool                _bAudio=true,             //IN, Audio
         char*               _pError=NULL);            //OUT, error string
int IfESipRteApi_RecvInitMsgByRouteID(
         unsigned int        _uiRouteID,          //IN, Route ID
         int&                _rnCurCPS,           //OUT
         int&                _rnMaxCPS,           //OUT
         bool                _bPlusCnt,           //IN, InCrease Busy Count
         //add 2.2
         char*               _pszMIN=NULL,        //IN, Called min
         bool                _bAudio=true,        //IN, Audio
         char*               _pError=NULL);       //OUT, error string
//check tps
//tps err: E_RTE_FailGetICRoute_OverLoadGroupTPS or E_RTE_FailGetICRoute_OverLoadGlobalTPS
//acl err: other error
int IfESipRteApi_CheckTPSByRouteID(
         unsigned int        _uiID,               //IN:RouteID
         int&                _rnCurTPS,           //OUT
         int&                _rnMaxTPS,           //OUT
         char*               _pError=NULL);
//mod 1.4
int IfESipRteApi_CheckTPSByTransport(
         SipNodeLocal_t*     _pstLocal,                //IN/OUT, ip/port/transport type
         SipNodeRemote_t*    _pstRemote,               //IN/OUT, ip/port/transport type
         SipRoute_t*         _pstRoute,                //OUT, route info
         int&                _rnCurTPS,                //OUT
         int&                _rnMaxTPS,                //OUT
         bool                _bCheckRouteStatus=true,  //IN, check route status
         char*               _pError=NULL);

//check system load
//cpu error: E_RTE_FailGetICRoute_OverLoadCPU
//mem error: E_RTE_FailGetICRoute_OverLoadMEM
int IfESipRteApi_IsNormalSysLoad(
            int& _rnCurCPU, int& _rnMaxCPU,
            int& _rnCurMEM, int& _rnMaxMEM);

//get reason
int IfESipRteApi_GetReasonByRouteID(
               unsigned int _uiRouteID,   //IN,  Route ID
               int          _nCauseIndex, //IN,  cause index. SipReasonCode_t::ECauseIndex_t (eSipRte.hxx)
               int&         _rnCauseCode, //OUT, cause code
               int&         _rnRspCode,   //OUT, response code
               char*        _pCauseText,  //OUT, cause text. min buffer size 56
               char*        _pError=NULL);

//ha standby -> active
void IfESipRteApi_ChangeHAStandby2Active();

//add 1.5. the phone number or the remote id
// caller or callee, remote id
bool IfESipRteApi_CheckTrace(
                  const char*     _pszCaller,       //IN,  O, the called number
                  const char*     _pszCallee,       //IN,  O, the callee number
                  const int       _nRemoteID,       //IN,  O, the remote id
                  //mod 1.9
                  int&            _rnCount,         //OUT, O, the number of trace
                  SipCoreTrace_t* _pstTrace[SipTraceDataBase_t::E_MAXNUM]);  //OUT, trace info 

///add 1.6. check sip message size 
//return 0: do nothing
//       1: drop
//       2: response error
//       3: if recv by udp, relay tcp
int IfESipRteApi_CheckMSGSize(int _nSize);

//add 1.9
int IfESipRteApi_FindTCPRoute( SipNodeLocal_t* _pLocal,   //IN/OUT, M, local node 
                               SipNodeRemote_t* _pRemote, //IN/OUT, M, remote node 
                               SipRoute_t* _pRoute=NULL); //OUT, O, route

//mananement route count
void IfESipRteApi_InCrease1RouteCount(unsigned int _uiID, bool _bIC);
void IfESipRteApi_DeCrease1RouteCount(unsigned int _uiID, bool _bIC);
void IfESipRteApi_SetZeroRouteCount(unsigned int _uiID);
void IfESipRteApi_SetZeroRouteCount_All();

void IfESipRteApi_InCrease1SesCount();
void IfESipRteApi_DeCrease1SesCount();
void IfESipRteApi_SetZeroSesCount();
void IfESipRteApi_SetSesCount(int nSes);


#endif
