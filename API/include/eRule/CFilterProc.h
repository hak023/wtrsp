#ifndef CFILTERPROC_H
#define CFILTERPROC_H
#include "CRuleProc.h"
#include "CFilter.h"
#include "RuleLibCom.h"

class FilterEvent
{
	public:
		FilterEvent();
		~FilterEvent();
		EFilterAttribute_t fnFiltering(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
												resip::SipMessage * _pclsSndSipMsg,
												unsigned int _unFOPS,
												bool _bTrace,
												char * _pszTrace,
												unsigned int _unTraceSize);		
};
#endif