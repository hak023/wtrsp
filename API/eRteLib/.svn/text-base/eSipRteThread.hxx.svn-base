/**
 * @file  eSipRteThread.h
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

#ifndef __ESIP_CORE_SIP_ROUTE_THREAD__
#define __ESIP_CORE_SIP_ROUTE_THREAD__


#include "eSipRteDBMgr.hxx"
#include "rutil/ThreadIf.hxx"
namespace eSIP
{

class CSipRteThread : public resip::ThreadIf
{
public:
   struct SipOptMgr_t{
      CTimeDiff m_stOptTDiff;
      CTimeDiff m_stUptTDiff;
   };
public:
   CSipRteThread();
   virtual ~CSipRteThread();
   int m_fnInit();
   static CSipRteThread* m_fnGetInstance();
   void m_fnStop();
protected:
   virtual void thread();
   void SendOption();
   void SendUpdate();
protected:
   static CSipRteThread*               m_pIns;
   SipOptMgr_t*                        m_pOpt;
   bool                                m_bRun;
};


}; //end namespace

#define THESIPRTETHREAD     (eSIP::CSipRteThread::m_fnGetInstance())

#endif
