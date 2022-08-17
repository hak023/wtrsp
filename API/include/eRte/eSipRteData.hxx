/**
 * @file  eSipRteData.h
 * the sip route message header
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

#ifndef __ESIP_CORE_SIP_ROUTE_DATA__
#define __ESIP_CORE_SIP_ROUTE_DATA__


#include "eSipRteUtil.hxx"
#include "rutil/RWMutex.hxx"
#include "eSipRte.hxx"
#include "eSipRteLog.hxx"
#include "eSipRteErr.hxx"

namespace eSIP
{

typedef void (*PfuncNotifyData_t)(int _nIndex, void* _pvData, int _nCnt,bool bChanged);
class CSipRteData
{
public:
   enum ESipRteDataMaxDefine_t{ 
      //@R106 201603
      //E_MAXCNT_RECORD=100,
      E_MAXCNT_RECORD=256,
      E_MAXLEN_ERR =255,
      E_MAXLEN_FILE=256
   };
   enum ESipRteElement_t{
      E_SIPRTE_LN   = 0,
      E_SIPRTE_RMT  = 1,
      E_SIPRTE_GRTE = 2,
      E_SIPRTE_RTE  = 3,
      E_SIPRTE_RSEQ = 4,
      E_SIPRTE_SSEQ = 5,
      E_SIPRTE_TRGW = 6,
      E_SIPRTE_RSN  = 7,
      E_SIPRTE_ENV  = 8,
      E_SIPRTE_ENV_LB  = 9,
      E_SIPRTE_TIME = 10,
      E_SIPRTE_TRC  = 11,
      E_SIPRTE_EMER = 12,
      E_SIPRTE_MAX  = 13 
   };
public:
   CSipRteData();
   virtual ~CSipRteData();
   int  m_fnInit(int _nIdx, char* _pName, PfuncNotifyData_t _pFunc);
   int  m_fnRead(bool bChanged=false);

   //@vibc
   bool m_fnCheckChangedfile();

   int  m_fnOpenWriteNeedClose();
   int  m_fnCloseWrite();
   int  m_fnWrite(char* buf);
   void m_fnBakup2ActiveDB();

   //file
   char* m_fnGetFileName()
   {
      return m_szFile;
   }

   //locking
   void m_fnReadLock();
   void m_fnLock();
   void m_fnUnLock();

   int m_fnString2Data(
         void* _pvOutData,
         void* _pstData,
         int _nCnt, 
         char* _pError);
   int m_fnCheckData(void* _pvData, char* _pError, bool _bAdd=true);
   int m_fnMakeModify(void* _pvNewData,void* _pvCurData,char* _pError);
   //file write
   int m_fnMakeStringWrite(void* _pvData,char* _pszOut);

protected:
   //struct to string
   int  m_fnStLocalNode2String(SipNodeLocal_t* _pstValue, char* _pszOut);
   int  m_fnStRemoteNode2String(SipNodeRemote_t* _pstValue, char* _pszOut);
   int  m_fnStRouteGroup2String(SipRouteGroup_t* _pstValue, char* _pszOut);
   int  m_fnStRoute2String(SipRoute_t* _pstValue, char* _pszOut);
   int  m_fnStRouteSeq2String(SipRouteSeq_t* _pstValue, char* _pszOut);
   int  m_fnStSpecialRouteSeq2String(SipSpecialRouteSeq_t* _pstValue, char* _pszOut);
   int  m_fnStTrGW2String(TrGWRoute_t* _pstValue, char* _pszOut);
   int  m_fnStReason2String(SipReasonCode_t* _pstValue, char* _pszOut);
   int  m_fnStEnv2String(GlobalOverLoadEnv_t* _pstValue, char* _pszOut);
   int  m_fnStTimer2String(SipCoreTimer_t* _pstValue, char* _pszOut);
   int  m_fnStEmer2String(EmergencyCall_t* _pstValue, char* _pszOut);

   //modify
   int m_fnMakeModifyLocalNode(SipNodeLocal_t* _pstNew, SipNodeLocal_t* _pstCur,char* _pError);
   int m_fnMakeModifyRemoteNode(SipNodeRemote_t* _pstNew, SipNodeRemote_t* _pstCur, char* _pError);
   int m_fnMakeModifyRouteGroup(SipRouteGroup_t* _pstNew, SipRouteGroup_t* _pstCur,char* _pError);
   int m_fnMakeModifyRoute(SipRoute_t* _pstNew, SipRoute_t* _pstCur,char* _pError);
   int m_fnMakeModifyRouteSeq(SipRouteSeq_t* _pstNew, SipRouteSeq_t* _pstCur, char* _pError);
   int m_fnMakeModifySpecialRouteSeq(SipSpecialRouteSeq_t* _pstNew, 
                  SipSpecialRouteSeq_t* _pstCur,char* _pError);
   int m_fnMakeModifyTrGW(TrGWRoute_t* _pstNew, TrGWRoute_t* _pstCur,char* _pError);
   int m_fnMakeModifyReason(SipReasonCode_t* _pstNew, SipReasonCode_t* _pstCur,char* _pError);
   int m_fnMakeModifyEnv(GlobalOverLoadEnv_t* _pstNew, GlobalOverLoadEnv_t* _pstCur,char* _pError);
   int m_fnMakeModifyTimer(SipCoreTimer_t* _pstNew, SipCoreTimer_t* _pstCur,char* _pError);
   int m_fnMakeModifyEmer(EmergencyCall_t* _pstNew, EmergencyCall_t* _pstCur,char* _pError);
  
   //local node
   int  m_fnCheckLocalNode(
         SipNodeLocal_t* _pstValue,
         char* _pError,
         bool _bAdd=true);
   int  m_fnString2StLocalNode(
         CSipRteUtil::CSipRteUtilValue_t* _pstData,
         int _nCnt,
         SipNodeLocal_t* _pstValue,
         char* _pError);
   //remote node
   int  m_fnCheckRemoteNode(
         SipNodeRemote_t* _pstValue,
         char* _pError,
         bool _bAdd=true);
   int  m_fnString2StRemoteNode(
         CSipRteUtil::CSipRteUtilValue_t* _pstData,
         int _nCnt,
         SipNodeRemote_t* _pstValue,
         char* _pError);
   //route group
   int  m_fnCheckSipRouteGroup(
         SipRouteGroup_t* _pstValue,
         char* _pError,
         bool _bAdd=true);
   int  m_fnString2StRouteGroup(
         CSipRteUtil::CSipRteUtilValue_t* _pstData,
         int _nCnt,
         SipRouteGroup_t* _pstValue,
         char* _pError);
   //route
   int  m_fnCheckSipRoute(
         SipRoute_t* _pstValue,
         char* _pError,
         bool _bAdd=true);
   int  m_fnString2StRoute(
         CSipRteUtil::CSipRteUtilValue_t* _pstData, 
         int _nCnt,
         SipRoute_t* _pstValue, 
         char* _pError);
   //route seq
   int  m_fnCheckSipRouteSeq(
         SipRouteSeq_t* _pstValue,
         char* _pError,
         bool _bAdd=true);
   int  m_fnString2StRouteSeq(
         CSipRteUtil::CSipRteUtilValue_t* _pstData,
         int _nCnt,
         SipRouteSeq_t* _pstValue,
         char* _pError);

   //special route seq
   int  m_fnCheckSipSpecialRouteSeq(
         SipSpecialRouteSeq_t* _pstValue,
         char* _pError,
         bool _bAdd=true);
   int  m_fnString2StSpecialRouteSeq(
         CSipRteUtil::CSipRteUtilValue_t* _pstData,
         int _nCnt,
         SipSpecialRouteSeq_t* _pstValue,
         char* _pError);
   //trgw
   int  m_fnCheckTrGW(
         TrGWRoute_t* _pstValue,
         char* _pError,
         bool _bAdd=true);
   int  m_fnString2StTrGW(
         CSipRteUtil::CSipRteUtilValue_t* _pstData,
         int _nCnt,
         TrGWRoute_t* _pstValue,
         char* _pError);
   //reason
   int  m_fnCheckReason(
         SipReasonCode_t* _pstValue,
         char* _pError,
         bool _bAdd=true);
   int  m_fnString2StReason(
         CSipRteUtil::CSipRteUtilValue_t* _pstData,
         int _nCnt,
         SipReasonCode_t* _pstValue,
         char* _pError);
   //env
   int  m_fnCheckEnv(
         GlobalOverLoadEnv_t* _pstValue,
         char* _pError,
         bool _bAdd=true);
   int  m_fnString2StEnv(
         CSipRteUtil::CSipRteUtilValue_t* _pstData,
         int _nCnt,
         GlobalOverLoadEnv_t* _pstValue,
         char* _pError);
   //timer
   int  m_fnCheckTimer(
         SipCoreTimer_t* _pstValue,
         char* _pError,
         bool _bAdd=true);
   int  m_fnString2StTimer(
         CSipRteUtil::CSipRteUtilValue_t* _pstData,
         int _nCnt,
         SipCoreTimer_t* _pstValue,
         char* _pError);
   //Emer
   int  m_fnCheckEmer(
         EmergencyCall_t* _pstValue,
         char* _pError,
         bool _bAdd=true);
   int  m_fnString2StEmer(
         CSipRteUtil::CSipRteUtilValue_t* _pstData,
         int _nCnt,
         EmergencyCall_t* _pstValue,
         char* _pError);
   //trace
   int  m_fnString2StTrace(
         CSipRteUtil::CSipRteUtilValue_t* _pstData,
         int _nCnt,
         SipCoreTrace_t* _pstValue,
         char* _pError);
protected:
   FILE*                            m_pReadFP;
   FILE*                            m_pWriteFP;
   resip::RWMutex*                  m_pLock;
   CSipRteUtil::CSipRteUtilValue_t* m_pRecord;
   int                              m_nIndex;
   time_t                           m_last_mtime;
   unsigned int                     m_uiLastSize;
   PfuncNotifyData_t                m_pFunc;
   char                             m_szFile[E_MAXLEN_FILE];
   char                             m_szFileBak[E_MAXLEN_FILE];
};

}; //end namespace




#endif
