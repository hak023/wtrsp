/*
 *
 */

#ifndef _LM_ERROR_H_
#define _LM_ERROR_H_

#define LM_BASE_RESCODE 0x00040000
enum LMReasonCode {
	lmRcNoErr = 0x0000,
	lmRcInternalError = LM_BASE_RESCODE | 0x0001,//= 0x0001,
	lmRcInvalidMsg ,//= 0x0002,
	lmRcTimeoutCS ,//= 0x0003,
	lmRcTimeoutPDE
};

static const char* fnLmApiStringError(int rc)
{
	int nCode = rc&0x000fffff;
	switch(nCode)
	{
	case lmRcNoErr: return "NoErr";
	case lmRcInternalError: return "InternalError";
	case lmRcInvalidMsg: return "InvalidMsg";
	case lmRcTimeoutCS: return "Timeout CS";
	case lmRcTimeoutPDE: return "Timeout PDE";
	default: return "Undefine";
	}
}

#endif
