/**
 * @file  nwm_mgr.h
 * the network manager api header
 *
 * @attention Copyright (c) 2013 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project      : IBC 
 * @par Last Changed : 
 * @par 2013.07.30   hc_yun   1.0     first written
*/


//@par Should be  : 
// add object aloxUtil.o, ifcfg.o, aloxConf.o 
// link lib pdk3.2.2
#ifndef __PI_NWM_MGR_H__
#define __PI_NWM_MGR_H__

#include "ifcfg.h"
#include <string>
#include <map>

class CNwmMgr
   : public CUserThread
{
public:
   //constructor
   CNwmMgr();
   //destructor
   ~CNwmMgr();

   bool Initialize(int nAPPID,  bool bServer =false);
   void AppTouch();
   bool IsRunNetworkIF(const char* pIfName,const char* pIPAddr);
   void DownIPAddr(const char* pIfName, const char* pIPAddr);
   bool UpIPAddr(const char* pIfName,const char* pIPAddr, const char* pIPv4NetMask);
public:
   //network interface
   struct PINWM_INTERFACE{
      char              szIFName[16];
      char              szIPAddr[48]; 
      struct timeval    touchTime;
   };
   //config file
   struct   PINWM_CONF{
      unsigned int      uiLogLevel      ; //0, 1, 2, 3
      time_t            ulLastModifyTime;
      unsigned short    usARPCheckTime  ; //msec
      unsigned short    usARPCheckSleep  ; //msec
      unsigned short    usAliveCheckTime; //msec
      unsigned short    usAliveCheckDown; //0: nothing, 1:check down
   };
   //shared memory
   struct   PINWM_SHM{
      enum { e_IFMAX_NUM = 8,
             e_DEFAULT_CHECKTIME = 500 };
      PINWM_CONF        nwmConf;
      struct timeval    touchTime;
      PINWM_INTERFACE   nwIf[e_IFMAX_NUM]; 
   };
protected:
   void*    ThreadProc();
   bool     CheckChangedFile(); 
   bool     LoadConfigFile();
   void     AddUPIf(const char* pIfName,const char* pIPAddr, bool bUP, bool bFirstUP=false);
   void     AutoDown(struct timeval* pTime, unsigned short usDownSec);
   void     AutoARP();
   void     SendARPAndNDP(char* pIFName, char* pIPAddr);
   void     PrintSHM();
protected:
   CIfCfg*      m_pIfCfg;
   CArp*        m_pArp;
   PINWM_SHM*   m_pSHM;
   bool         m_bServer;
   char         m_szConf[256];
};

#endif
