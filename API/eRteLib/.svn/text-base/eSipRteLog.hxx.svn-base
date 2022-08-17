/**
 * @file  eSipRteLog.h
 * the sip log header
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

#ifndef __ESIP_CORE_SIP_LOG__
#define __ESIP_CORE_SIP_LOG__


#include "eSipRteUtil.hxx"
#include "eSipRteApi.hxx"

namespace eSIP
{

class CSipRteLog
{
public:
   CSipRteLog();
   virtual ~CSipRteLog();
   static CSipRteLog* GetInstance();
   void m_fnSetLogLevel(int _nLevel);
   int m_fnLog(int _nLevel, const char *_pFormat, ...);
protected:
   static CSipRteLog*                  m_pIns;
   int                                 m_nLevel;
};

}; //end namespace

#define THESIPLOGGER     (eSIP::CSipRteLog::GetInstance())
#define SLOG(v...)       (THESIPLOGGER)->m_fnLog(v)




#endif
