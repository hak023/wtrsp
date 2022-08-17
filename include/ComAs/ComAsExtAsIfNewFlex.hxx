
#ifndef COM_AS_EXT_IF_NEW_FLEX_HXX
#define COM_AS_EXT_IF_NEW_FLEX_HXX

#include "eSipUtil/flexIf.hxx"

#include "ComAsExtAsIf.hxx"
#include "ComAsSipHelper.hxx"

namespace ComAs
{

class ComAsAsyncEvent;

class IfNewFlex
{
	public :
		static const ExtAsIfCallback & m_fnGetExtAsIfCallback(){ return m_stNewFxIfCallback; }
		static void m_fnSetExtAsIfMgr(const ExtAsIfMgr * _pclsExtAsIfMgr){ m_pclsExtAsIfMgr = _pclsExtAsIfMgr; }
		
		static unsigned int m_fnGetMinMsgLen4Tcp();
		static int m_fnGetTotalMsgLen(const void * _pExtAsIfPacket, unsigned int _unPacketLen);
		//custom
		static bool m_fnEncodeNewFx4CustomSipEvent(void * _pExtAsIfPacket, const char * _pszKey, const char * _pszValue);
		static const char * m_fnDecodeNewFx4CustomSipEvent(const void * _pExtAsIfPacket, const char * _pszKey, bool & _bResult);
		//encode
		static void * m_fnEncodeNewFx4InitReq(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		static void * m_fnEncodeNewFx4InitRsp(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		static void * m_fnEncodeNewFx4PingReq(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		static void * m_fnEncodeNewFx4PingRsp(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		static void * m_fnEncodeNewFx4SipEvent(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		static void * m_fnEncodeNewFx4InternalTerm(const ExtAsIfComInfo & _rstExtAsIfComInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		//decode
		static EExtAsIfMsgType m_fnDecodeNewFx4ComHdr(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfComInfo & _rstExtAsIfComInfo);
		static bool m_fnDecodeNewFx4InitReq(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo, char ** _ppPacketStr);
		static bool m_fnDecodeNewFx4InitRsp(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo, char ** _ppPacketStr);
		static bool m_fnDecodeNewFx4PingReq(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo, char ** _ppPacketStr);
		static bool m_fnDecodeNewFx4PingRsp(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo, char ** _ppPacketStr);
		static bool m_fnGetPreInfoNewFx4SipEvent(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgSipPreInfo & _rstExtAsIfSipPreInfo, char ** _ppPacketStr);
		static bool m_fnDecodeNewFx4SipEventInDialog(const ComAsAsyncEvent & _rstAsyncEvent, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo);
		static bool m_fnDecodeNewFx4SipEventOutDialog(const void * _pExtAsIfPacket, unsigned int _unPacketLen, 
																													ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, eSIP::SetSessionInfo & _rstSetSessionInfo);

	private :
		static eDum::EDialogMethodType_t m_fnConvertDialogMethodType(const char * _pszFxStr);
		static const char * m_fnConvertFxSessType(int _nDialogMethodType, resip::MethodTypes _eSipMethod);
		static resip::TransportType m_fnConvertResipTrspType(const char * _pszFxStr);
		static const char * m_fnConvertFxTrspType(int _nResipTrspType);
		static EExtAsIfMsgType m_fnConvertExtAsMsgType(const char * _pszFxStr);
		static const char * m_fnConvertFxMsgType(int _nExtAsMsgType);
		static EExtAsIfRspResult_t m_fnConvertExtAsRspResult(const char * _pszFxStr);
		static const char * m_fnConvertFxRspResult(int _nExtAsIfRspResult);
		static resip::MethodTypes m_fnConvertResipSipMethod(const char * _pszFxStr);
		static const char * m_fnConvertFxSipMethod(resip::MethodTypes _eResipMethodType);
		
		//encode
		static bool m_fnEncodeComHdr(const ExtAsIfComInfo & _rstExtAsIfComInfo, eSipUtil::FlexIf & _rclsFlexIf);
		static bool m_fnEncodeBodyInitReq(const ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo, eSipUtil::FlexIf & _rclsFlexIf);
		static bool m_fnEncodeBodyInitRsp(const ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo, eSipUtil::FlexIf & _rclsFlexIf);
		static bool m_fnEncodeBodyPingReq(const ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo, eSipUtil::FlexIf & _rclsFlexIf);
		static bool m_fnEncodeBodyPingRsp(const ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo, eSipUtil::FlexIf & _rclsFlexIf);
		static bool m_fnEncodeBodySipEvent4SipHdr(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, eSipUtil::FlexIf & _rclsFlexIf);
		static bool m_fnEncodeBodySipEvent4SipContent(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, eSipUtil::FlexIf & _rclsFlexIf);
		static bool m_fnEncodeBodySipEvent4Custom(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, eSipUtil::FlexIf & _rclsFlexIf);
		//decode
		static bool m_fnDecodeComHdr(const eSipUtil::FlexIf & _rclsFlexIf, ExtAsIfComInfo & _rstExtAsIfComInfo);
		static bool m_fnDecodeBodyInitReq(const eSipUtil::FlexIf & _rclsFlexIf, ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo);
		static bool m_fnDecodeBodyInitRsp(const eSipUtil::FlexIf & _rclsFlexIf, ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo);
		static bool m_fnDecodeBodyPingReq(const eSipUtil::FlexIf & _rclsFlexIf, ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo);
		static bool m_fnDecodeBodyPingRsp(const eSipUtil::FlexIf & _rclsFlexIf, ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo);
		static bool m_fnDecodeBodySipEvent4SipHdr(bool _bInDlg, const eSipUtil::FlexIf & _rclsFlexIf, resip::SipMessage & _rclsSipMsg);
		static bool m_fnDecodeBodySipEvent4SipContent(const eSipUtil::FlexIf & _rclsFlexIf, resip::SipMessage & _rclsSipMsg);
		static bool m_fnDecodeBodySipEvent4Custom(const eSipUtil::FlexIf & _rclsFlexIf, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo);
		static bool m_fnDecodeBodyInitSipEvent4Custom(const eSipUtil::FlexIf & _rclsFlexIf, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, eSIP::SetSessionInfo & _rstSetSessionInfo);

		static const ExtAsIfCallback m_stNewFxIfCallback;
		static const ExtAsIfMgr * m_pclsExtAsIfMgr;
		
};

}

#endif

