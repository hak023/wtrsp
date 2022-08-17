#ifndef CROUTEPROC_H
#define CROUTEPROC_H
#include "CRuleUtiles.h"
#include "CRuleFile.h"
#include "CRule.h"
#include "CRuleProc.h"
#include "CRoute.h"

class RouteEvent
{
	public:
		RouteEvent();
		~RouteEvent();
		RouteOP * fnRouting(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
												resip::SipMessage * _pclsSndSipMsg);		
};
#endif
