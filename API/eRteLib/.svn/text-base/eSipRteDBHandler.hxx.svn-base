/**
 * @file  eSipRteDBHandler.hxx
 * the sip route db handler
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

#ifndef __ESIP_CORE_SIP_ROUTE_DBHANDLER_H__
#define __ESIP_CORE_SIP_ROUTE_DBHANDLER_H__


#include "eSipRteDBMgr.hxx"
#include "eSipRteQueue.hxx"
#include "rutil/ThreadIf.hxx"
namespace eSIP
{

class CSipRteDBHandler : public resip::ThreadIf
{
private:
   enum E_MAXDefine_t{
      E_MAX_SYSCHECK_PERIOD=6
   };
   //db changed
   struct SipRteDBAction_t{
      int m_nChangeIndex;
      //@R106 Only Route Status 
      int m_nChangedRteStatus; //0: nothing, 1: changed
   };
   typedef CQueue<SipRteDBAction_t*,SipRteDBAction_t>  ESIP_DBHANDLER_QUEUE; 
public:
   static CSipRteDBHandler* m_fnGetInstance();
public:
   CSipRteDBHandler();
   virtual ~CSipRteDBHandler();
   int m_fnInit();
   void m_fnStop();
   int m_fnAddChangeDB(int _nIndex);
   int m_fnAddChangeDB_RteStatus();
protected:
   virtual void thread();
   void m_fnNotifyChangeDB(int _nIndex);
   void m_fnSystemUsage();
protected:
   static CSipRteDBHandler*            m_pIns;
   bool                                m_bRun;
   ESIP_DBHANDLER_QUEUE*               m_pQueue;
};


}; //end namespace

#define THESIPRTEDBHANLDER     (eSIP::CSipRteDBHandler::m_fnGetInstance())

#endif
