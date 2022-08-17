#ifndef _PROCSESSIONEVENT_H
#define _PROCSESSIONEVENT_H
#include "main.h"
#include "IBCDefine.h"
namespace name_sessionevent
{
	typedef eSIP::EAsyncCbResultType_t (*PfuncTable_t)(StackAsyncCbInfo_t & _rclsInfo);
	eSIP::EAsyncCbResultType_t fnExe(StackAsyncCbInfo_t & _rclsInfo);
};

#endif
