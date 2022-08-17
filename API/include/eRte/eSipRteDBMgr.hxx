/**
 * @file  eSipRteDBMgr.hxx
 * the sip route db header
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

#ifndef __ESIP_CORE_SIP_ROUTE_DB_MANAGER__
#define __ESIP_CORE_SIP_ROUTE_DB_MANAGER__



#include "eSipRteApi.hxx"
#include "eSipRteData.hxx"
#include "eSipRteUsrMap.hxx"
#include "eSipRteLoad.hxx"
#include <string>
namespace eSIP
{

class CSipRteDBMgr
{
//add @R106 201603 check alarm system overload
protected:
   struct SystemResource_t{
      int         m_nNotifyFlag;      //1: notify, 0: not notify
      int         m_nCurUsage;
      int         m_nLimitUsage;
      CTimeDiff   m_stNotifyTime;
   };
   struct InternalNotify_t{
      SystemResource_t  m_stCPU;
      SystemResource_t  m_stMEM;
      char              m_szReserved[16];
   };
public:
   typedef CUsrMap<int,void*,MapKeyBypass_t> CSipData_IntMap;
   typedef CUsrMap<std::string,void*> CSipData_StrMap;
public:
   CSipRteDBMgr();
   virtual ~CSipRteDBMgr();
   static CSipRteDBMgr* m_fnGetInstance();

   //add @vibc
   int   m_fnInit2(bool _bUseLocal, bool bClear=true);
   void  m_fnClearInternal(int _nIndex);
   void  m_fnSetCompareModRecord(int _nIndex, void* _pvCur,  void* _pvNew);
   void  m_fnClearInternalMap(int _nIndex);

   int   m_fnInit(bool bClear=true);
   void  m_fnClear(int _nIndex);
   void m_fnBuffer2WriteDB(int _nIndex);
   //print
   void  m_fnPrint(int _nIndex);
   void* m_fnGetRecord(int _nIndex);
   int   m_fnFindRecord(int _nIndex, int _nID,void*& _pvOut);
   int   m_fnAddRecordByIndex(int _nIndex, int _nNum, void* _pvData);
   void  m_fnSetRecordByIndex(int _nIndex, int _nNum, void* _pvData);
   int   m_fnDelRecordByIndex(int _nIndex, int _nNum);
   int   m_fnFindEmptyRecord(int _nIndex);

   //@vibc
   int  m_fnFindRecord_SHM(int _nIndex, int _nID, void*& _pvOut);

   //detail check
   int m_fnLocalNodeValidCheck(void* _pvData, char* _pError);
   int m_fnRemoteNodeValidCheck(void* _pvData, char* _pError);
   int m_fnRouteGroupValidCheck(void* _pvData, char* _pError);
   int m_fnRouteValidCheck(void* _pvData, char* _pError, bool _bMod=false);
   int m_fnRouteSeqValidCheck(void* _pvData, char* _pError);
   int m_fnSpecialRouteSeqValidCheck(void* _pvData, char* _pError);
   int m_fnTrGWValidCheck(void* _pvData, char* _pError);
   int m_fnReasonValidCheck(void* _pvData, char* _pError);
   int m_fnEnvValidCheck(void* _pvData, char* _pError);
   int m_fnTimerValidCheck(void* _pvData, char* _pError);
   int m_fnTraceValidCheck(void* _pvData, char* _pError);
   int m_fnEmerValidCheck(void* _pvData, char* _pError);
   //delete check
   int m_fnLocalNodeDelCheck(unsigned int uiID, char* _pError);
   int m_fnRemoteNodeDelCheck(unsigned int uiID, char* _pError);
   int m_fnRouteGroupDelCheck(unsigned int uiID, char* _pError);
   int m_fnRouteDelCheck(unsigned int uiID, char* _pError);
   int m_fnRouteSeqDelCheck(unsigned int uiID, char* _pError);
   int m_fnTrGWDelCheck(unsigned int uiID, char* _pError);
   int m_fnReasonDelCheck(unsigned int uiID, char* _pError);
   int m_fnEnvDelCheck(unsigned int uiID, char* _pError);
   int m_fnTimerDelCheck(unsigned int uiID, char* _pError);
   int m_fnTraceDelCheck(unsigned int uiID, char* _pError);
   int m_fnEmerDelCheck(unsigned int uiID, char* _pError);

   //get
   void m_fnGetAllRecord(int _nIndex,void* _pvData);
   void m_fnSortByID(int _nIndex,void* _pvData);
   //modify
   int m_fnSetModRecord_LocalNode(void* _pvCur,  void* _pvNew);
   int m_fnSetModRecord_RemoteNode(void* _pvCur,  void* _pvNew);
   int m_fnSetModRecord_Route(void* _pvCur,  void* _pvNew);
   int m_fnSetModRecord_RouteGroup(void* _pvCur,  void* _pvNew);
   int m_fnSetModRecord_RouteSeq(void* _pvCur,  void* _pvNew);
   int m_fnSetModRecord_SRouteSeq(void* _pvCur,  void* _pvNew);
   int m_fnSetModRecord_TrGW(void* _pvCur,  void* _pvNew);
   int m_fnSetModRecord_Reason(void* _pvCur,  void* _pvNew);
   //@vibc
   int m_fnSetModRecord_Env(void* _pvCur,  void* _pvNew, int _nIndex);
   int m_fnSetModRecord_Timer(void* _pvCur,  void* _pvNew);
   int m_fnSetModRecord_Trace(void* _pvCur,  void* _pvNew);
   int m_fnSetModRecord_Emer(void* _pvCur,  void* _pvNew);
   //@vibc
   int m_fnSetCompareModRecord_LocalNode(void* _pvCur,  void* _pvNew);
   int m_fnSetCompareModRecord_RemoteNode(void* _pvCur,  void* _pvNew);
   int m_fnSetCompareModRecord_Route(void* _pvCur,  void* _pvNew);
   int m_fnSetCompareModRecord_RouteGroup(void* _pvCur,  void* _pvNew);
   int m_fnSetCompareModRecord_RouteSeq(void* _pvCur,  void* _pvNew);
   int m_fnSetCompareModRecord_SRouteSeq(void* _pvCur,  void* _pvNew);
   int m_fnSetCompareModRecord_TrGW(void* _pvCur,  void* _pvNew);
   int m_fnSetCompareModRecord_Reason(void* _pvCur,  void* _pvNew);
   //@vibc
   int m_fnSetCompareModRecord_Env(void* _pvCur,  void* _pvNew, int _nIndex);
   int m_fnSetCompareModRecord_Timer(void* _pvCur,  void* _pvNew);
   int m_fnSetCompareModRecord_Trace(void* _pvCur,  void* _pvNew);
   int m_fnSetCompareModRecord_Emer(void* _pvCur,  void* _pvNew);

   void m_fnSentOption(SipRoute_t* _pRte);
   //mod @R106 201603 hcyun. 
   //void m_fnRecvOption200OK( SipNodeLocal_t* _pLocal, SipNodeRemote_t* _pRemote, bool _bOKRsp);
   void m_fnRecvOption200OK( SipNodeLocal_t* _pLocal, SipNodeRemote_t* _pRemote, bool _bOKRsp, int nResponseCode);
   int m_fnRecvOption(SipNodeLocal_t* _pLocal, SipNodeRemote_t* _pRemote);

   int m_fnGetLocal(SipNodeLocal_t*  _pstICLocal,char* _pError);
   //@vibc
   int m_fnSetLocalStatus(SipNodeLocal_t*  _pstICLocal,int _nStatus,char* _pError);
   int m_fnGetRemote(SipNodeRemote_t* _pstICRemote, char* _pError);
   int m_fnGetRouteByLIDAndRID(
         unsigned int _uiLID,
         unsigned int _uiRID,
         SipRoute_t*  _pstICRoute,   //out
         char* _pError);

   int m_fnGetICRoute(
         SipNodeLocal_t*      _pstICLocal,   //in/out
         SipNodeRemote_t*     _pstICRemote,  //in/out
         SipRoute_t*          _pstICRoute,   //out
         SipRoute_t*&         _rpstICRoute,   //out
         bool                _bCheckStaus,
         char* _pError);
   int m_fnGetOGRouteByRSeq(
         unsigned int         _uiRSeqID,     //in
         SipNodeLocal_t*      _pstOGLocal,   //out
         SipNodeRemote_t*     _pstOGRemote,  //out
         SipRoute_t*          _pstOGRoute,   //out
         char* _pError,
         bool _bOnlyTCP=false);

   //decision route
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
   //decision route
   int m_fnDecisionRSeqBySpecialRSeq(
         unsigned int         _uiSRSeqID,    //in
         unsigned int&        _ruiRSeqID,     //out
         int&                 _rValidCnt,
         char*                _pError) ;
   //R120 20161101
   int m_fnDecisionRouteBySpecialRSeq(
         unsigned int         _uiSRSeqID,     //in
         SipNodeLocal_t*      _pstOGLocal,   //out
         SipNodeRemote_t*     _pstOGRemote,  //out
         SipRoute_t*          _pstOGRoute,   //out
         char* _pError,
         bool _bOnlyTCP);
   //decision trgw
   int m_fnDecisionTrGW(TrGWRoute_t* _pstOut,bool& _rbFind,char* _pError);
   bool m_fnFindTrGW(
                  char* szSvrIP,         //IN, TCP Server IP
                  int nSvrPort,          //IN, TCP Server Port
                  char* szTrGWIP,        //IN, TCP Client IP
                  int nTrGWPort,
                  TrGWRoute_t*& _rpOut);
   void m_fnChangeTrGWStatus(TrGWRoute_t* _pstTrGW, bool bConnect);
   //add @R106 201603 
   void m_fnNotifyTrGWStatus(TrGWRoute_t* _pstTrGW);

   //ctrol route count
   void m_fnCtrolRouteCount(
                     unsigned int _uiID,
                     bool         _bIC,
                     bool         _bPlusOne,
                     bool         _bForceZero);
   void m_fnCtrolRouteCount_SetZeroAll( );
   //current cps
   void m_fnCalculateCurrentCPS();
   int m_fnCheckCPSByRouteID(
         unsigned int        _uiRouteID,
         int&                _rnCurCPS,
         int&                _rnMaxCPS,
         bool                _bIC,           //IN, IC/OG 
         bool                _bPlusCnt,      //IN, InCrease Busy Count
         char*               _pszMIN,
         bool                _bAudio,
         char*               _pError);
   int m_fnCheckCPSByTransport(
         SipNodeLocal_t*     _pstLocal,      //IN/OUT, ip/port/transport type
         SipNodeRemote_t*    _pstRemote,     //IN/OUT, ip/port/transport type
         SipRoute_t*         _pstRoute,      //OUT, route info
         int&                _rnCurCPS,
         int&                _rnMaxCPS,
         bool        _bCheckRouteStatus,
         bool                _bIC,           //IN, IC/OG 
         bool                _bPlusCnt,      //IN, InCrease Busy Count
         char*               _pszMIN,
         bool                _bAudio,
         char*               _pError);
   int m_fnCheckTPSByTransport(
         SipNodeLocal_t*     _pstLocal,      //IN/OUT, ip/port/transport type
         SipNodeRemote_t*    _pstRemote,     //IN/OUT, ip/port/transport type
         SipRoute_t*         _pstRoute,
         int&                _rnCurTPS,
         int&                _rnMaxTPS,
         bool        _bCheckRouteStatus,
         char*  _pError);
   
   void m_fnSetSystemLoad(int _nCPU, int _nMEM);
   //add @R106 201603 system alarm
   void m_fnNotifyAlarmSystemLoad(int _nCurUsage, int _nLimitUsage, bool _bCPU);
   int  m_fnIsNormalSysLoad(
            int& _rnCurCPU, int& _rnMaxCPU,
            int& _rnCurMEM, int& _rnMaxMEM);
   //sescount
   void m_fnInCrease1SesCount();
   void m_fnDeCrease1SesCount();
   void m_fnSetZeroSesCount();
   void m_fnSetSesCount(int nSes);
   int m_fnCheckCPSByTransport(int nGroupID,int& nCurCPS, int& nMaxCPS, 
                               bool bEmer, bool bGlobalCheck, char* _pError);
   int m_fnCheckTPSByTransport(int nGroupID,int& nCurTPS, int& nMaxTPS, char* _pError);
   //trace
   bool m_fnCheckTrace(
                  const char*     _pszCaller, //IN,  O, the called number
                  const char*     _pszCallee, //IN,  O, the callee number
                  const int       _nRemoteID, //IN,  O, the remote id
                  int&            _rnCnt,
                  SipCoreTrace_t* _pstTrace[SipTraceDataBase_t::E_MAXNUM]);

   //check tcp
   int m_fnFindTCPRoute(SipNodeLocal_t* _pLocal, SipNodeRemote_t* _pRemote,
                          SipRoute_t* _pRoute);
   //@R106 20160308
   SipRouteManagerDataBase_t*  m_fnGetDB(){
      return m_pDB;
   }
   void m_fnChangeRouteStatus(bool _bAdd, SipRoute_t* _pstRoute,
                              bool _bMod, int _nNewMode,
                              bool _bOpt, int _nNewStatus,
                              //add param @R106 201603 
                              int nResponseCode); //0: timeout other response code);
   //@vibc
   void m_fnPrintMap();
   void m_fnUpdate_RouteStatus();
   int  m_fnSetRouteStatus(
         SipNodeLocal_t*     _pstICLocal,    //IN/OUT, ip/port/transport type
         SipNodeRemote_t*    _pstICRemote,   //IN/OUT, ip/port/transport type
         int                 _nRouteID,      //IN 
         bool                _bRouteID,
         int                 _nStatus,
         char*               _pError);
protected:
   //add @vibc
   int m_fnFindRecordChange_SHM(int _nIndex, int _nID, void* _pvOut);
   void m_fnSetNATInfo(SipNodeRemote_t* _pRemote, SipNodeRemote_t* _pDBRemote );
   int m_fnCheckCPSClassByTransport(int nGroupID,int& nCurCPS, int& nMaxCPS,
                                    char* szMIN, bool bAudio, char* _pError);
   void m_fnTouchCPSGroup(int nGroupID, int nLoad=0);
protected:
   static CSipRteDBMgr*         m_pIns;
   SipRouteManagerDataBase_t*   m_pDB;
   //add @vibc
   SipRouteManagerDataBase_t*   m_pDB_SHM;
   int                          m_nUseLocal;
   CLoadCalculator<>*           m_pGroupLoad;
   CLoadCalculator<>*           m_pGroupLoadTPS;
   CLoadCalculator<>*           m_pGlobalLoad;
   CLoadCalculator<>*           m_pGlobalLoadTPS;
   //@vibc level_cps 20180219
   CLoadCalculator<>*           m_pAudioLoad;
   CLoadCalculator<>*           m_pVideoLoad;

   CSipData_IntMap*             m_pIntMap[CSipRteData::E_SIPRTE_MAX];
   CSipData_StrMap*             m_pLocalStrMap;
   CSipData_StrMap*             m_pRmtStrMap;
   CSipData_StrMap*             m_pRteStrMap;
   CSipData_StrMap*             m_pRteEmerStrMap;
   //add internal @R106 201603
   InternalNotify_t             m_InterNotify;
public:
   //@vibc svt. route/session count
   int                          m_nMaintain;
};


}; //end namespace

#define THESIPDBMGR     (eSIP::CSipRteDBMgr::m_fnGetInstance())

#endif
