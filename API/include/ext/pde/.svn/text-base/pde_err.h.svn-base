/*
 *
 */

#ifndef _PDE_ERROR_H_
#define _PDE_ERROR_H_

#define PDE_BASE_RESCODE 0x00050000
enum PDEReasonCode {
	pdeRcNoErr = PDE_BASE_RESCODE | 0x0000,
	pdeRcReservSesFail ,//= 0x0001,
	pdeRcInvalidSes ,//= 0x0002,
	pdeRcNotFoundSes ,//= 0x0003,
	pdeRcInvalidACRType ,//= 0x0004,
	pdeRcNewTranFail ,//= 0x0005,
	pdeRcInvalidState ,//= 0x0006,
	pdeRcMsgEncodeFail ,//= 0x0007,
	pdeRcSendMsgFail ,//= 0x0008,
	pdeRcMsgTimeout ,//= 0x0009
	pdeRcMultiRoundAuth , 
	pdeRcLimitedSuccess ,
	pdeRcCommandUnsupported ,
	pdeRcUnableToDeliver ,
	pdeRcRealmNotServed ,
	pdeRcTooBusy ,
	pdeRcLoopDetected ,
	pdeRcRedirectIndication ,
	pdeRcApplicationUnsupported ,
	pdeRcInvalidHdrBits ,
	pdeRcInvalidAvpBits ,
	pdeRcUnknownPeer ,
	pdeRcAuthenticationRejected ,
	pdeRcOutOfSpace,
	pdeRcElectionLost,
	pdeRcAvpUnsupported,
	pdeRcUnknownSessionId,
	pdeRcAuthorizationRejected,
	pdeRcInvalidAvpValue ,
	pdeRcMissingAvp ,
	pdeRcResourceExceeded ,
	pdeRcContradictionAvps ,
	pdeRcAvpNotAllowed ,
	pdeRcAvpOccursTooManyTimes ,
	pdeRcNoCommonApplication ,
	pdeRcUnsupportedVersion ,
	pdeRcUnableToComply ,
	pdeRcInvalidBitInHeader ,
	pdeRcInvalidAvpLength ,
	pdeRcInvalidMessageLength ,
	pdeRcInvalidAvpBitCombt ,
	pdeRcNoCommonSecurity,
	pdeRcInvalidHAStatus,
	pdeRcAlreadyExist,
	pdeRcNotExist
};

inline const char* fnPdeApiStringError(int rc)
{
	switch(rc|PDE_BASE_RESCODE)
	{
	case pdeRcNoErr: return "NoErr";
	case pdeRcReservSesFail: return "ReservSesFail";
	case pdeRcInvalidSes: return "InvalidSes";
	case pdeRcNotFoundSes: return "NotFoundSes";
	case pdeRcInvalidACRType: return "InvalidACRType";
	case pdeRcNewTranFail: return "NewTranFail";
	case pdeRcInvalidState: return "InvalidState";
	case pdeRcMsgEncodeFail: return "MsgEncodeFail";
	case pdeRcSendMsgFail: return "SendMsgFail";
	case pdeRcMsgTimeout: return "MsgTimeout";
	case pdeRcMultiRoundAuth: return "MultiRoundAuth";
	case pdeRcLimitedSuccess: return "LimitedSuccess";
	case pdeRcCommandUnsupported: return "CommandUnsupported";
	case pdeRcUnableToDeliver: return "UnableToDeliver";
	case pdeRcRealmNotServed: return "RealmNotServed";
	case pdeRcTooBusy: return "TooBusy";
	case pdeRcLoopDetected: return "LoopDetected";
	case pdeRcRedirectIndication: return "RedirectIndication";
	case pdeRcApplicationUnsupported: return "ApplicationUnsupported";
	case pdeRcInvalidHdrBits: return "InvalidHdrBits";
	case pdeRcInvalidAvpBits: return "InvalidAvpBits";
	case pdeRcUnknownPeer: return "UnknownPeer";
	case pdeRcAuthenticationRejected: return "AuthenticationRejected";
	case pdeRcOutOfSpace: return "OutOfSpace";
	case pdeRcElectionLost: return "ElectionLost";
	case pdeRcAvpUnsupported: return "AvpUnsupported";
	case pdeRcUnknownSessionId: return "UnknownSession";
	case pdeRcAuthorizationRejected: return "AuthorizationRejected";
	case pdeRcInvalidAvpValue: return "InvalidAvpValue";
	case pdeRcMissingAvp: return "MissingAvp";
	case pdeRcResourceExceeded: return "ResourceExceeded";
	case pdeRcContradictionAvps: return "ContradictionAvps";
	case pdeRcAvpNotAllowed: return "AvpNotAllowed";
	case pdeRcAvpOccursTooManyTimes: return "AvpOccursTooManyTimes";
	case pdeRcNoCommonApplication: return "NoCommonApplication";
	case pdeRcUnsupportedVersion: return "UnsupportedVersion";
	case pdeRcUnableToComply: return "UnableToComply";
	case pdeRcInvalidBitInHeader: return "InvalidBitInHeader";
	case pdeRcInvalidAvpLength: return "InvalidAvpLength";
	case pdeRcInvalidMessageLength: return "InvalidMessageLength";
	case pdeRcInvalidAvpBitCombt: return "InvalidAvpBitCombt";
	case pdeRcNoCommonSecurity: return "NoCommonSecurity";
	case pdeRcInvalidHAStatus: return "InvalidHAStatus";
	case pdeRcAlreadyExist: return "pdeRcAlreadyExist";
	case pdeRcNotExist: return "pdeRcNotExist";
	default: return "Undefine";
	}
}
#endif
