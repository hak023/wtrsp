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
//@par add object aloxUtil.o, ifcfg.o, aloxConf.o nwm_mgr.o
//@par link lib pdk3.2.2
#ifndef __PI_NWM_API_H__
#define __PI_NWM_API_H__

#include "nwm_mgr.h"


//func   Initialize. 1 run only once
//param  int nAPPID    IN TGA = 0 , MSRPA = 1
//       bool bServer  IN TGA, MSRPA set false 
//return true/false
bool  PINWM_API_Initialize(int nAPPID, bool bServer =false);

//func   touch app time
//       check whether app is down or not.
//param  none
//return true/false
void  PINWM_API_AppTouch();


//func   check whether ip is down or not.
//param  const char* pIfName    IN   network interface name ex) bond0, bond1:1
//       const char* pIPAddr    IN   ip ver 4 or 6
//return if true. ip alive or not
bool  PINWM_API_IsNETIFAlive(const char* pIfName,const char* pIPAddr);

//func   ip up
//param  const char* pIfName       IN   network interface name ex) bond0, bond1:1
//       const char* pIPAddr       IN   ip ver 4 or 6 . if ipver 6, set prefix ex)fec0::230:64ff:fe0f:f044/64
//       const char* pIPv4NetMask  IN   if only ip ver 4
//return if true. ip up or not
bool  PINWM_API_UpIPAddr(const char* pIfName,const char* pIPAddr, const char* pIPv4NetMask);


//func   ip down
//param  const char* pIfName    IN   network interface name ex) bond0, bond1:1
//       const char* pIPAddr    IN   ip ver 4 or 6
//return none
void  PINWM_API_DownIPAddr(const char* pIfName, const char* pIPAddr);


#endif
