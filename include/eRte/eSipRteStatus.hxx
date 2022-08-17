/**
 * @file  eSipRteStatus.h
 * the sip route status header
 *
 * @attention Copyright (c) 2014 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2016.03.08   hc_yun   1.0     first written
 */

#ifndef __ESIP_CORE_SIP_ROUTE_STATUS__
#define __ESIP_CORE_SIP_ROUTE_STATUS__


#include "eSipRteUtil.hxx"
#include "eSipRteUsrMap.hxx"
#include "rutil/RWMutex.hxx"

namespace eSIP
{

class CSipRteStatus
{
public:
   enum ESipRteDataMaxDefine_t{
      E_MAXCNT_RECORD =3,
      E_MAXLEN_FILE=256
   };
   typedef CUsrMap<int,int,MapKeyBypass_t> CSipData_IntMap;
public:
   CSipRteStatus();
   virtual ~CSipRteStatus();
   static CSipRteStatus* m_fnGetInstance();
   void m_fnInit();
   void m_fnLoad();
   void m_fnWriteStatus();
   int m_fnGetStatus(int _nID);
   bool m_fnCheckChangedfile();
protected:
   void m_fnWrite();
protected:
   static CSipRteStatus*   m_pIns;
   FILE*                   m_pReadFP;
   FILE*                   m_pWriteFP;
   CSipRteUtil::CSipRteUtilValue_t* m_pRecord;
   resip::RWMutex*         m_pLock;
   CSipData_IntMap*        m_pIntMap;
   time_t                  m_last_mtime;
   int                     m_uiLastSize;
   char                    m_szFile[E_MAXLEN_FILE];
   char                    m_szFileBak[E_MAXLEN_FILE];
};

}; //end namespace

#define THESIPRTESTS     (eSIP::CSipRteStatus::m_fnGetInstance())

#endif
