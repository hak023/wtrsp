/**
 * @file  eSipRteMgr.h
 * the sip route manager header
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

#ifndef __ESIP_CORE_SIP_ROUTE_MANAGER__
#define __ESIP_CORE_SIP_ROUTE_MANAGER__


#include "eSipRteDBMgr.hxx"
namespace eSIP
{
class CSipRteMgr
{
public:
   CSipRteMgr();
   virtual ~CSipRteMgr();
   int    m_fnWrite();
public:
   static void m_pfnCallBackNotifyData(int _nIndex, void* _pvData, int _nCnt,bool bChanged); //lock
   static CSipRteMgr* m_fnGetInstance();
public:
   //init
   int   m_fnInit(ESipRteApiCallBack_t* _pstCB, int _nLevel, char* _pStrDir=NULL); 
   int   m_fnInit2(ESipRteApiCallBack_t* _pstCB);
   //add @vibc
   int   m_fnInit( ESipRteApiCallBack_t* _pstCB,
                   int _nLevel,
                   int _nAttachSystem,
                   int _nAttachModule,
                   int _nHAStatus,
                   char* _pStrDir=NULL);
   int m_fnSetRouteStatus(
         SipNodeLocal_t*     _pstICLocal,    //IN/OUT, ip/port/transport type
         SipNodeRemote_t*    _pstICRemote,   //IN/OUT, ip/port/transport type
         int                 _nRouteID,      //IN 
         bool                _bRouteID,
         int                 _nStatus,
         char*               _pError);
   int m_fnCheckSetRecord(int _nIndex, int _nID ,void* _pvData, char* _pError);

   int m_fnCheckChangedfile();
   char* m_fnGetFileName(int _nIndex);
   //get handler
   CSipRteData* m_fnGetDataHandler(int _nIndex);
   //upate trace
   void m_fnUpdateTrace();
   //manangement
   int   m_fnAddRecord(int _nIndex,int _nID, void* _pvData, char* _pError, bool _bFirst=false);
   int   m_fnModRecord(int _nIndex,int _nID, void* _pvData, char* _pError);
   int   m_fnDelRecord(int _nIndex,int _nID, char* _pError);
   int   m_fnFindRecord(int _nIndex,int _nID, void* _pvData);
   //add response code @R106 201603
   //int   m_fnRecvOption200OK( SipNodeLocal_t* _pLocal, SipNodeRemote_t* _pRemote, bool _bOKRsp);
   int   m_fnRecvOption200OK( SipNodeLocal_t* _pLocal, SipNodeRemote_t* _pRemote, bool _bOKRsp,int _nResponseCode);
   int   m_fnRecvOption( SipNodeLocal_t* _pLocal, SipNodeRemote_t* _pRemote);
   //manage route count
   void m_fnInCrease1RouteCount(unsigned int _uiID, bool _bIC);
   void m_fnDeCrease1RouteCount(unsigned int _uiID, bool _bIC);
   void m_fnSetZeroRouteCount(unsigned int _uiID);
   void m_fnSetZeroRouteCount_All();

   //decision route by norma route sequence
   int m_fnDecisionTargetByRSeq(
         unsigned int         _uiRSeqID,     //in
         SipNodeLocal_t*      _pstICLocal,   //in/out
         SipNodeRemote_t*     _pstICRemote,  //in/out
         SipRoute_t*          _pstICRoute,   //out
         SipNodeLocal_t*      _pstOGLocal,   //out
         SipNodeRemote_t*     _pstOGRemote,  //out
         SipRoute_t*          _pstOGRoute,   //out
         char* _pError,
         bool _bOnlyTCP=false);
   int m_fnDecisionTargetByRSeq(
         unsigned int         _uiRSeqID,     //in
         SipNodeLocal_t*      _pstOGLocal,   //out
         SipNodeRemote_t*     _pstOGRemote,  //out
         SipRoute_t*          _pstOGRoute,   //out
         char* _pError,
         bool _bOnlyTCP=false);
   //decision route by special route sequence
   int m_fnDecisionTargetBySpecialRSeq(
         unsigned int         _uiSRSeqID,    //in
         SipNodeLocal_t*      _pstICLocal,   //in/out
         SipNodeRemote_t*     _pstICRemote,  //in/out
         SipRoute_t*          _pstICRoute,   //out
         SipNodeLocal_t*      _pstOGLocal,   //out
         SipNodeRemote_t*     _pstOGRemote,  //out
         SipRoute_t*          _pstOGRoute,   //out
         char* _pError,
         bool _bOnlyTCP=false);
   int m_fnDecisionTargetBySpecialRSeq(
         unsigned int         _uiSRSeqID,    //in
         SipNodeLocal_t*      _pstOGLocal,   //out
         SipNodeRemote_t*     _pstOGRemote,  //out
         SipRoute_t*          _pstOGRoute,   //out
         char* _pError,
         bool _bOnlyTCP=false);
   int  m_fnGetLocalByTransport(
         SipNodeLocal_t*      _pstLocal,   //in/out
         char* _pError);
   //@vibc
   int   m_fnSetLocalStatusByTransport(
         SipNodeLocal_t*      _pstLocal,   //in/out
         int                 _nStatus,   //INT, AVAIL(0), UNAVAIL(1)
         char* _pError);
   int  m_fnGetRemoteByTransport(
         SipNodeRemote_t*      _pstRemote,   //in/out
         char* _pError);
   int  m_fnGetRouteByLIDAndRID(
         unsigned int _uiLID,
         unsigned int _uiRID,
         SipRoute_t*  _pstRoute,   //out
         char* _pError);
   int m_fnGetRouteByTransport(
         SipNodeLocal_t*      _pstLocal,   //in/out
         SipNodeRemote_t*     _pstRemote,  //in/out
         SipRoute_t*          _pstRoute,   //out
         bool        _bCheckRouteStatus,   //in
         char*                _pError);
   int m_fnCheckCPSByTransport(
         SipNodeLocal_t*     _pstLocal,      //IN/OUT, ip/port/transport type
         SipNodeRemote_t*    _pstRemote,     //IN/OUT, ip/port/transport type
         SipRoute_t*         _pstRoute,      //OUT, route info
         int&                _rnCurCPS,
         int&                _rnMaxCPS,
         bool       _bCheckRouteStatus,
         bool                _bIC,           //IN, IC/OG 
         bool                _bPlusCnt,      //IN, InCrease Busy Count
         char*               _pszMIN,
         bool                _bAudio,
         char*               _pError);
   int m_fnCheckCPSByRouteID(
         unsigned int        _uiRouteID,     //IN, Route ID
         int&                _rnCurCPS,
         int&                _rnMaxCPS,
         bool                _bIC,           //IN, IC/OG 
         bool                _bPlusCnt,      //IN, InCrease Busy Count
         char*               _pszMIN,
         bool                _bAudio,
         char*               _pError);
   int m_fnCheckTPSByTransport(
         SipNodeLocal_t*     _pstLocal,      //IN/OUT, ip/port/transport type
         SipNodeRemote_t*    _pstRemote,     //IN/OUT, ip/port/transport type
         SipRoute_t*         _pstRoute,      //OUT, route info
         int&                _rnCurTPS,
         int&                _rnMaxTPS,
         bool       _bCheckRouteStatus,
         char*               _pError);
   int m_fnCheckTPSByRouteID(
         unsigned int        _uiID,
         int&                _rnCurTPS,
         int&                _rnMaxTPS,
         char*               _pError);

   int m_fnGetReasonByRouteID(
               unsigned int _uiRouteID,   //IN,  Route ID
               int          _nCauseIndex, //IN,  cause index. SipReasonCode_t::ECauseIndex_t (eSipRte.hxx)
               int&         _rnCauseCode, //OUT, cause code
               int&         _rnRspCode,   //OUT, response code
               char*        _pCauseText,  //OUT, cause text
               char*        _pError);
   //trgw
   int m_fnDecisionTrGW(TrGWRoute_t* _pstTrGW,char* _pError);
   int m_fnSetTrGWStatus(
         char* szSvrIP,         //IN, TCP Server IP
         int nSvrPort,          //IN, TCP Server Port
         char* szTrGWIP,        //IN, TCP Client IP
         int nTrGWPort,         //IN, TCP Client Port
         bool bIsConnect);       //IN, true=Connect, false=disconnect 
   int m_fnInCrease1TrGW(
         char* szSvrIP,         //IN, TCP Server IP
         int nSvrPort,          //IN, TCP Server Port
         char* szTrGWIP,        //IN, TCP Client IP
         int nTrGWPort);         //IN, TCP Client Port
   int m_fnDeCrease1TrGW(
         char* szSvrIP,         //IN, TCP Server IP
         int nSvrPort,          //IN, TCP Server Port
         char* szTrGWIP,        //IN, TCP Client IP
         int nTrGWPort);         //IN, TCP Client Port
   int m_fnSetCountTrGW(
         char* szSvrIP,         //IN, TCP Server IP
         int nSvrPort,          //IN, TCP Server Port
         char* szTrGWIP,        //IN, TCP Client IP
         int nTrGWPort,         //IN, TCP Client Port
         int nCnt);

   void m_fnInCrease1SesCount();
   void m_fnDeCrease1SesCount();
   void m_fnSetZeroSesCount();
   void m_fnSetSesCount(int nSes);

   int m_fnCheckMSGSize(int _nSize);

   bool  m_fnHasbeenInit(){
      return m_bInit;
   }
   void  m_fnLock(){ m_pLock->lock(); }
   void  m_fnUnLock(){ m_pLock->unlock(); }
   int   m_fnLoad(); 

   //trace
   bool m_fnCheckTrace(
                  const char*     _pszCaller, //IN,  O, the called number
                  const char*     _pszCallee, //IN,  O, the callee number
                  const int       _nRemoteID, //IN,  O, the remote id
                  int&            _rnCnt,
                  SipCoreTrace_t* _pstTrace[SipTraceDataBase_t::E_MAXNUM]);

   int m_fnFindTCPRoute(SipNodeLocal_t* _pLocal, SipNodeRemote_t* _pRemote,
                          SipRoute_t* _pRoute);

   //add @R106 201603 ALL DEACT by GroupID
   int m_fnRoute_Control(  int      _nACTFlag, //0: ACT, 1:BYPBASS, 2: DEACT 
                           int      _nGroupID,
                           bool     _bEnableGroup,
                           bool     _bIncludeBypass, //if _nACTFlag!=Bypass, is not valid.
                           char*    _pError);
protected:
   static CSipRteMgr*         m_pIns;
   resip::RWMutex*            m_pLock;
   CSipRteData*               m_pRte[CSipRteData::E_SIPRTE_MAX];
   bool                       m_bInit;
public:
   ESipRteApiCallBack_t*      m_pCB;
   int                        m_nAttachSystem;
   int                        m_nAttachModule;
   int                        m_nHAStatus;
};


}; //end namespace

#define THESIPRTEMGR     (eSIP::CSipRteMgr::m_fnGetInstance())
#define THESIPRTECB      (THESIPRTEMGR->m_pCB)

#endif
