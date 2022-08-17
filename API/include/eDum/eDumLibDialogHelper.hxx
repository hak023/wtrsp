
#ifndef DUMLIB_DIALOG_HELPER_HXX
#define DUMLIB_DIALOG_HELPER_HXX

#include "resip/stack/SipMessage.hxx"

#include "eDumLibFsm.hxx"

namespace eDum
{

class Session;
class Dialog;
class InviteDialog;
class StandAloneDialog;
class SubscribeDialog;

class InviteDialogHelper
{
	public :
		/*==== create SipMessage for sending ====*/
		static resip::SipMessage * createRequest(const Session & _rclsSession, const InviteDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType);
		static resip::SipMessage * createResponse(const Session & _rclsSession, const InviteDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType, int _nRspCode, const char * _szReason = NULL);
		/*==== modify SipMessage for sending, receiving ====*/
		static bool modifyRequest(const Session & _rclsSession, const InviteDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsModSipReq);
		static bool modifyResponse(const Session & _rclsSession, const InviteDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsModSipRsp);
		/*==== apply SipMsg to dialog ====*/
		static bool applyRequestToDialog(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSrcSipReq, const Session & _rclsSession, InviteDialog & _rclsDstDialog);
		static bool applyResponseToDialog(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSrcSipRsp, const Session & _rclsSession, InviteDialog & _rclsDstDialog);

		static bool validateMsg(const resip::SipMessage & _rclsSipMsg, resip::Data * _pReason = NULL, const Session * _pclsSession = NULL, const InviteDialog * _pclsDialog = NULL);

	private :
		/*==== modify header of UA attribute  ====*/
		static bool setUaAttrForRequest(const Session & _rclsSession, const InviteDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipReq);
		static bool setUaAttrForResponse(const Session & _rclsSession, const InviteDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipRsp);
		/*==== modify header Proxy attribute ====*/
		static bool setProxyAttrForRequest(const Session & _rclsSession, const InviteDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipReq);
		static bool setProxyAttrForResponse(const Session & _rclsSession, const InviteDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipRsp);
		/*==== modify specific header ====*/
		static void setCseqHeaderForRequest(const Session & _rclsSession, const InviteDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType, resip::SipMessage & _rclsDstSipReq);
		static void setReqUriHeaderForRequest(const Session & _rclsSession, const InviteDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType, resip::SipMessage & _rclsDstSipReq);
		static void setExtraHeaderForRequest(const Session & _rclsSession, const InviteDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType, resip::SipMessage & _rclsDstSipReq);
				
};

class StandAloneDialogHelper
{
	public :
		/*==== create SipMessage for sending ====*/
		static resip::SipMessage * createRequest(const Session & _rclsSession, const StandAloneDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType);
		static resip::SipMessage * createResponse(const Session & _rclsSession, const StandAloneDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType, int _nRspCode, const char * _szReason = NULL);
		/*==== modify SipMessage for sending, receiving ====*/
		static bool modifyRequest(const Session & _rclsSession, const StandAloneDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipReq);
		static bool modifyResponse(const Session & _rclsSession, const StandAloneDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipRsp);
		/*==== apply SipMsg to dialog ====*/
		static bool applyRequestToDialog(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSrcSipReq, const Session & _rclsSession, StandAloneDialog & _rclsDstDialog);
		static bool applyResponseToDialog(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSrcSipRsp, const Session & _rclsSession, StandAloneDialog & _rclsDstDialog);

		static bool validateMsg(const resip::SipMessage & _rclsSipMsg, resip::Data * _pReason = NULL, const Session * _pclsSession = NULL, const StandAloneDialog * _pclsDialog = NULL);

	private :
		/*==== modify header of UA attribute  ====*/
		static bool setUaAttrForRequest(const Session & _rclsSession, const StandAloneDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipReq);
		static bool setUaAttrForResponse(const Session & _rclsSession, const StandAloneDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipRsp);
		/*==== modify header Proxy attribute ====*/
		static bool setProxyAttrForRequest(const Session & _rclsSession, const StandAloneDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipReq);
		static bool setProxyAttrForResponse(const Session & _rclsSession, const StandAloneDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipRsp);
		/*==== modify specific header ====*/
		static void setExtraHeaderForRequest(const Session & _rclsSession, const StandAloneDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType, resip::SipMessage & _rclsDstSipReq);
				
};

class SubscribeDialogHelper
{
	public :
		/*==== create SipMessage for sending ====*/
		static resip::SipMessage * createRequest(const Session & _rclsSession, const SubscribeDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType);
		static resip::SipMessage * createResponse(const Session & _rclsSession, const SubscribeDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType, int _nRspCode, const char * _szReason = NULL);
		/*==== modify SipMessage for sending, receiving ====*/
		static bool modifyRequest(const Session & _rclsSession, const SubscribeDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipReq);
		static bool modifyResponse(const Session & _rclsSession, const SubscribeDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipRsp);
		/*==== apply SipMsg to dialog ====*/
		static bool applyRequestToDialog(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSrcSipReq, const Session & _rclsSession, SubscribeDialog & _rclsDstDialog);
		static bool applyResponseToDialog(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSrcSipRsp, const Session & _rclsSession, SubscribeDialog & _rclsDstDialog);

		static bool validateMsg(const resip::SipMessage & _rclsSipMsg, resip::Data * _pReason = NULL, const Session * _pclsSession = NULL, const SubscribeDialog * _pclsDialog = NULL);

	private :
		/*==== modify header of UA attribute  ====*/
		static bool setUaAttrForRequest(const Session & _rclsSession, const SubscribeDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipReq);
		static bool setUaAttrForResponse(const Session & _rclsSession, const SubscribeDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipRsp);
		/*==== modify header Proxy attribute ====*/
		static bool setProxyAttrForRequest(const Session & _rclsSession, const SubscribeDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipReq);
		static bool setProxyAttrForResponse(const Session & _rclsSession, const SubscribeDialog & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipRsp);
		/*==== modify specific header ====*/
		/*==== modify specific header ====*/
		static void setCseqHeaderForRequest(const Session & _rclsSession, const SubscribeDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType, resip::SipMessage & _rclsDstSipReq);
		static void setReqUriHeaderForRequest(const Session & _rclsSession, const SubscribeDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType, resip::SipMessage & _rclsDstSipReq);
		static void setExtraHeaderForRequest(const Session & _rclsSession, const SubscribeDialog & _rclsSrcDialog, resip::MethodTypes _eMethodType, resip::SipMessage & _rclsDstSipReq);
				
};


class DialogHelperMgr
{
	public :
		static resip::SipMessage * createSipMsg(const Session & _rclsSession, const Dialog & _rclsSrcDialog, resip::MethodTypes _eMsgMethodType, int _nRspCode, const char * _pszStatusLineDesc = NULL);
		static bool modifySipMsg(const Session & _rclsSession, const Dialog  & _rclsSrcDialog, EFsmEventType_t _eFsmEventType, resip::SipMessage & _rclsDstSipMsg);
		static bool applySipToDialog(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSrcSipMsg, const Session & _rclsSession, Dialog & _rclsDstDialog);
		static void applyState(Dialog  & _rclsDstDialog, ESipState_t _eSipState, ESdpState_t _eSdpState, resip::MethodTypes _eMsgMethodType, int _nRspCode, int _nEarlyDialogIdx);
		
		static void setUaFrom4SndReq(const Dialog & _rclsSrcDialog, resip::SipMessage & _rclsReq);
		static void setUaTo4SndReq(const Dialog & _rclsSrcDialog, resip::SipMessage & _rclsReq);
		static void setUaFrom4SndRsp(const Dialog & _rclsSrcDialog, resip::SipMessage & _rclsRsp);
		static void setUaTo4SndRsp(const Dialog & _rclsSrcDialog, resip::SipMessage & _rclsRsp);
		static void setProxyFrom4SndReq(const Dialog & _rclsSrcDialog, resip::SipMessage & _rclsReq);
		static void setProxyTo4SndReq(const Dialog & _rclsSrcDialog, resip::SipMessage & _rclsReq);
		static void setProxyFrom4SndRsp(const Dialog & _rclsSrcDialog, resip::SipMessage & _rclsRsp);
		static void setProxyTo4SndRsp(const Dialog & _rclsSrcDialog, resip::SipMessage & _rclsRsp);		
};

}

#endif

