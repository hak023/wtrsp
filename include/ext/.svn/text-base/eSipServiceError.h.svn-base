/**
 * @file  eSipServiceError.h
 * defines the esip service error
 *
 * @attention Copyright (c) 2015 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2015.03.27   hc_yun   1.0     first written
 */


#ifndef _ESIP_SERVICE_ERROR_H__
#define _ESIP_SERVICE_ERROR_H__

//the esip ha error    - base: 0x00000000
#include "msync_err.h"
//the esip route error - base: 0x00010000
#include "eSipRteErr.hxx"
//the esip rule error  - base: 0x00030000
#include "RuleError.h"
//lm error             - base: 0x00040000
#include "lm_err.h"
//pde error            - base: 0x00050000
#include "pde_err.h"

inline const char * fneSipServiceStringError(int _nCode)
{
int nLevel=_nCode&0xffff0000;
switch(nLevel)
{
//ha error
case 0x00000000:
return fnMSyncApiStringError(_nCode);
//route error
case 0x00010000:
return fnSipRteApiStringError(_nCode);
//rule error
case 0x00030000:
return fnRuleApiStringRuleError((ERuleError_t)_nCode);
//lm error
case LM_BASE_RESCODE: //0x0040000
return fnLmApiStringError(_nCode);
//pde error
case 0x00050000:
return fnPdeApiStringError(_nCode);

default: 
return "Unknown Service Error";
};
return "Unknown Serivce Error";
}

#endif
