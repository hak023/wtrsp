
#ifndef DUMLIB_INV_GENERAL_HELPER_HXX
#define DUMLIB_INV_GENERAL_HELPER_HXX

#include "resip/stack/SipMessage.hxx"
#include "resip/stack/eSipHandler.hxx"

namespace eDum
{

class Dialog;
class Session;

class SipHelper
{
	public :
		static bool isDialogCreateMsg(const resip::SipMessage & _rclsSipMsg);
		static bool isDialogCreateMethod(resip::MethodTypes _eResipMethodType);
		static bool isRoutingSipMsg(const resip::SipMessage & _rclsSipMsg, const eDum::Dialog & _rclsDialog);
		static bool isInvDialogTermMsg(const resip::SipMessage & _rclsSipMsg, const eDum::Dialog & _rclsDialog);
		static bool isInvDialogTermMsg(resip::MethodTypes _eMsgResipMethodType, int _nRspCode, const eDum::Dialog & _rclsDialog);
		static bool isInvDialogTermMsg(resip::MethodTypes _eMsgResipMethodType, int _nRspCode, bool _bInitial)	;
		static bool isGlareCondition(const eDum::Session & _rclsSession, const eDum::Dialog & _rclsDialog, const resip::SipMessage & _rclsSipMsg);
		static bool determineRefresher(bool _bRcv, const resip::SipMessage & _rclsSipMsg);

		//resip
		static bool checkValidUri(const resip::Uri & _rclsUri);
		static unsigned int getStartLineLength(const resip::SipMessage & _rclsSipMsg);
		static unsigned int getHeaderLength(const resip::SipMessage & _rclsSipMsg, resip::Headers::Type _eHeaderType, unsigned int & _rnMaxSingleLen);
		static bool isEmptyValueOfSingleHeader(const resip::SipMessage & _rclsSipMsg, resip::Headers::Type _eHeaderType);
		static unsigned int getHeaderNumber(const resip::SipMessage & _rclsSipMsg, resip::Headers::Type _eHeaderType);
		static unsigned int getUnknownHeaderLength(const resip::SipMessage & _rclsSipMsg, const resip::Data & _rclsUnknownHeaderName, unsigned int & _rnMaxSingleLen);
		static bool isEmptyValueOfUnknownSingleHeader(const resip::SipMessage & _rclsSipMsg, const resip::Data & _rclsUnknownHeaderName);
		static unsigned int getUnknownHeaderNumber(const resip::SipMessage & _rclsSipMsg, const resip::Data & _rclsUnknownHeaderName);
};

}

#endif
