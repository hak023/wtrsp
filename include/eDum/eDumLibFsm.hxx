
#ifndef DUMLIB_FSM_HXX
#define DUMLIB_FSM_HXX

#include "resip/stack/SdpContents.hxx"
#include "resip/stack/MethodTypes.hxx"

#include "eDumLibDef.hxx"
#include "eDumLibDialogTimer.hxx"

namespace eDum
{

class Dialog;

typedef enum
{
	E_SIP_STATE_IDLE = 0,
		
	/*==== INVITE ====*/
	E_SIP_STATE_INV_UAS_INVITING,
	E_SIP_STATE_INV_UAC_INVITING,
	E_SIP_STATE_INV_UAS_PROCEEDING,
	E_SIP_STATE_INV_UAC_PROCEEDING,
	E_SIP_STATE_INV_UAS_PROCEEDING_REL,
	E_SIP_STATE_INV_UAC_PROCEEDING_REL,
	E_SIP_STATE_INV_UAS_PRACK,
	E_SIP_STATE_INV_UAC_PRACK,
	E_SIP_STATE_INV_UAS_ACCEPT,
	E_SIP_STATE_INV_UAC_ACCEPT,
	E_SIP_STATE_INV_ESTABLISH,
	E_SIP_STATE_INV_UAS_RE_INVITING,
	E_SIP_STATE_INV_UAC_RE_INVITING,
	E_SIP_STATE_INV_UAS_RE_PROCEEDING,
	E_SIP_STATE_INV_UAC_RE_PROCEEDING,
	E_SIP_STATE_INV_UAS_RE_ACCEPT,
	E_SIP_STATE_INV_UAC_RE_ACCEPT,
	E_SIP_STATE_INV_TERMINATED,
	E_SIP_STATE_INV_TERMINATING,
	E_SIP_STATE_INV_TERMINATE,

	/*==== StandAlone ====*/
	E_SIP_STATE_STANDALONE_UAS_REQ,
	E_SIP_STATE_STANDALONE_UAC_REQ,
	E_SIP_STATE_STANDALONE_UAS_PROCEEDING,
	E_SIP_STATE_STANDALONE_UAC_PROCEEDING,
	E_SIP_STATE_STANDALONE_TERMINATE,

	/*==== SUBSCRIBE ====*/
	E_SIP_STATE_SUBSCRIBE_UAS_REQ,
	E_SIP_STATE_SUBSCRIBE_UAC_REQ,
	E_SIP_STATE_SUBSCRIBE_UAS_PROCEEDING,
	E_SIP_STATE_SUBSCRIBE_UAC_PROCEEDING,
	E_SIP_STATE_SUBSCRIBE_UAS_ACCEPT,
	E_SIP_STATE_SUBSCRIBE_UAC_ACCEPT,
	E_SIP_STATE_SUBSCRIBE_TERMINATE,

	E_SIP_STATE_NUM,
}ESipState_t;

typedef enum
{
	E_SDP_STATE_IDLE = 0,
		
	/*==== regular ====*/
	E_SDP_STATE_OFFER,
	E_SDP_STATE_FAKE_ANSWER,
	E_SDP_STATE_ANSWER,

	/*==== early-session ====*/
	E_SDP_STATE_EARLY_OFFER,
	E_SDP_STATE_EARLY_ANSWER,

	E_SDP_STATE_NUM,
}ESdpState_t;

typedef enum
{		
	/*==== SIP Event ====*/
	E_FSM_EVENT_SIP_RCV = 0,
	E_FSM_EVENT_SIP_SND,

	/*==== Etc Event ====*/
	E_FSM_EVENT_TIMEOUT,
	E_FSM_EVENT_TERM,

	E_FSM_EVENT_NUM,
}EFsmEventType_t;

/*
typedef enum
{
	E_FSM_REQ_TYPE_OUT_DIALOG = 0,
	E_FSM_REQ_TYPE_IN_DIALOG,

	E_FSM_REQ_TYPE_NUM,
}EFsmRequestType_t;
*/

/*==== SIP state ====*/
bool isValidSipState(ESipState_t _eSipState);
const char * getSipStateString(ESipState_t _eSipState);
/*==== SDP state ====*/
bool isValidSdpState(ESdpState_t _eSdpState);
const char * getSdpStateString(ESdpState_t _eSdpState);
/*==== FSM Event Type ====*/
bool isValidFsmEventType(EFsmEventType_t _eFsmEventType);
const char * getFsmEventTypeString(EFsmEventType_t _eFsmEventType);

class Fsm
{
	public :
		static bool convertTerm2SipEvent(ESipState_t _eCurrentSipState,
																			EFsmEventType_t & _reSipFsmEventType,
																			resip::MethodTypes & _reSipMethodType,
																			int & _rnSipRspCode,
																			bool _bSetBye);
		static bool convertTimeout2SipEvent(ESipState_t _eCurrentSipState,
																					EDialogTimerType_t _eDialogTimerType,
																					EFsmEventType_t & _reSipFsmEventType,
																					resip::MethodTypes & _reSipMethodType,
																					int & _rnSipRspCode);
		static bool getNextSipState(const Dialog & _rclsDialog,
															EDialogMethodType_t _eDialogMethodType,
															ESipState_t _eCurrentSipState,
															EFsmEventType_t _eFsmEventType,
															resip::MethodTypes _eMethodType,
															int _nRspCode,
															bool _b100Rel,
															unsigned int _unRseq,
															ESipState_t & _reNextSipState,
															bool & _bReInviteFailOver);

		static bool getNextSdpState(const Dialog & _rclsDialog,
																ESipState_t _eSipStateForSdp,
																ESdpState_t _eCurrentSdpState,
																resip::MethodTypes _eMethodType,
																int _nRspCode,
																bool _bIsEarlySession,
																bool _bIsReliable,
																ESdpState_t & _reNextSdpState,
																bool & _rbInitOffer);

		static bool checkRetransMsg(resip::SipMessage & _rclsSipMsg, const Dialog & _rclsDialog, bool _bRcv, resip::SipMessage ** _ppSndRequest=NULL);

	private :		
		static bool getNextInviteSipState(const Dialog & _rclsDialog,
																		ESipState_t _eCurrentSipState,
																		EFsmEventType_t _eFsmEventType,
																		resip::MethodTypes _eMethodType,
																		int _nRspCode,
																		bool _b100Rel,
																		unsigned int _unRseq,
																		ESipState_t & _reNextSipState);
		static bool getNextReInviteSipState(const Dialog & _rclsDialog,
																				ESipState_t _eCurrentSipState,
																				EFsmEventType_t _eFsmEventType,
																				resip::MethodTypes _eMethodType,
																				int _nRspCode,
																				bool _b100Rel,
																				ESipState_t & _reNextSipState,
																				bool & _bReInviteFailOver);
		static bool getNextStandAloneSipState(const Dialog & _rclsDialog,
																						ESipState_t _eCurrentSipState,
																						EFsmEventType_t _eFsmEventType,
																						resip::MethodTypes _eMethodType,
																						int _nRspCode,
																						ESipState_t & _reNextSipState);
		static bool getNextSubscribeSipState(const Dialog & _rclsDialog,
																					ESipState_t _eCurrentSipState,
																					EFsmEventType_t _eFsmEventType,
																					resip::MethodTypes _eMethodType,
																					int _nRspCode,
																					ESipState_t & _reNextSipState);
		static bool getNextInviteSdpState(const Dialog & _rclsDialog,
																			ESipState_t _eSipStateForSdp,
																			ESdpState_t _eCurrentSdpState,
																			resip::MethodTypes _eMethodType,
																			int _nRspCode,
																			bool _bIsEarlySession,
																			bool _bIsReliable,
																			ESdpState_t & _reNextSdpState,
																			bool & _rbInitOffer);
		static bool getNextReInviteSdpState(const Dialog & _rclsDialog,
																				ESipState_t _eSipStateForSdp,
																				ESdpState_t _eCurrentSdpState,
																				resip::MethodTypes _eMethodType,
																				int _nRspCode,
																				bool _bIsEarlySession,
																				bool _bIsReliable,
																				ESdpState_t & _reNextSdpState);
			
};

}

#endif
