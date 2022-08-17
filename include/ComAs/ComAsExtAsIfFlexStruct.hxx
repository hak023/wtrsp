
#ifndef COM_AS_EXT_IF_FLEX_STRUCT_HXX
#define COM_AS_EXT_IF_FLEX_STRUCT_HXX

#include "sipif_fx_msg.h"

#include "ComAsExtAsIf.hxx"
#include "ComAsSipHelper.hxx"

namespace ComAs
{

class ComAsAsyncEvent;

class IfFlexStruct
{
	public :
		static const ExtAsIfCallback & m_fnGetExtAsIfCallback(){ return m_stFxStructCallback; }
		static void m_fnSetExtAsIfMgr(const ExtAsIfMgr * _pclsExtAsIfMgr){ m_pclsExtAsIfMgr = _pclsExtAsIfMgr; }
		
		static unsigned int m_fnGetMinMsgLen4Tcp();
		static int m_fnGetTotalMsgLen(const void * _pExtAsIfPacket, unsigned int _unPacketLen);

		//custom
		static bool m_fnEncodeFxPacket4CustomSipEvent(void * _pExtAsIfPacket, const char * _pszKey, const char * _pszValue);
		static const char * m_fnDecodeFxPacket4CustomSipEvent(const void * _pExtAsIfPacket, const char * _pszKey, bool & _bResult);
		//encode
		static void * m_fnEncodeFxPacket4InitReq(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		static void * m_fnEncodeFxPacket4InitRsp(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		static void * m_fnEncodeFxPacket4PingReq(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		static void * m_fnEncodeFxPacket4PingRsp(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		static void * m_fnEncodeFxPacket4SipEvent(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		static void * m_fnEncodeFxPacket4InternalTerm(const ExtAsIfComInfo & _rstExtAsIfComInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
		//decode
		static EExtAsIfMsgType m_fnDecodeFxPacket4ComHdr(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfComInfo & _rstExtAsIfComInfo);
		static bool m_fnDecodeFxPacket4InitReq(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo, char ** _ppPacketStr);
		static bool m_fnDecodeFxPacket4InitRsp(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo, char ** _ppPacketStr);
		static bool m_fnDecodeFxPacket4PingReq(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo, char ** _ppPacketStr);
		static bool m_fnDecodeFxPacket4PingRsp(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo, char ** _ppPacketStr);
		static bool m_fnGetPreInfoFxPacket4SipEvent(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgSipPreInfo & _rstExtAsIfSipPreInfo, char ** _ppPacketStr);
		static bool m_fnDecodeFxPacket4SipEventInDialog(const ComAsAsyncEvent & _rstAsyncEvent, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo);
		static bool m_fnDecodeFxPacket4SipEventOutDialog(const void * _pExtAsIfPacket, unsigned int _unPacketLen, 
																														ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, 	eSIP::SetSessionInfo & _rstSetSessionInfo);

	private :
		static eDum::EDialogMethodType_t m_fnConvertDialogMethodType(int _nFxSessType);
		static ESipIfFxSipSessType_t m_fnConvertFxSessType(int _nDialogMethodType, resip::MethodTypes _eSipMethod);
		static resip::TransportType m_fnConvertResipTrspType(int _nFxTrspType);
		static ESipIfFxTransportType_t m_fnConvertFxTrspType(int _nResipTrspType);
		static EExtAsIfMsgType m_fnConvertExtAsMsgType(int _nFxMsgType);
		static ESipIfFxMsgType_t m_fnConvertFxMsgType(int _nExtAsMsgType);
		static EExtAsIfRspResult_t m_fnConvertExtAsRspResult(int _nFxRspResult);
		static ESipIfFxRspResult_t m_fnConvertFxRspResult(int _nExtAsIfRspResult);
		static resip::MethodTypes m_fnConvertResipSipMethod(int _nFxMethodType);
		static ESipIfFxSipMethod_t m_fnConvertFxSipMethod(resip::MethodTypes _eResipMethodType);
		static const char * m_fnConvertContentTypeStr(int _nFxContentType);
		static ESipIfFxSipContentType_t m_fnConvertFxSipContentType(const char * _pszContentType);
		
		//encode
		static bool m_fnEncodeComHdr(const ExtAsIfComInfo & _rstExtAsIfComInfo, SVCComMsgHdr & _rstComHdr);
		static bool m_fnEncodeBodyInitReq(const ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo, SVC_Fx_InitReq_t & _rstFxInitReqMsg);
		static bool m_fnEncodeBodyInitRsp(const ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo, SVC_Fx_Rsp_t & _rstFxInitRspMsg);
		static bool m_fnEncodeBodyPingReq(const ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo, SVC_Fx_PingReq_t & _rstFxPingReqMsg);
		static bool m_fnEncodeBodyPingRsp(const ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo, SVC_Fx_Rsp_t & _rstFxPingRspMsg);
		static bool m_fnEncodeBodySipEvent4SipHdr(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, SVC_Fx_Sip_t & _rstFxSipMsg);
		static bool m_fnEncodeBodySipEvent4SipContent(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, SVC_Fx_Sip_t & _rstFxSipMsg);
		static bool m_fnEncodeBodySipEvent4Custom(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, SVC_Fx_Sip_t & _rstFxSipMsg);
		//decode
		static bool m_fnDecodeComHdr(const SVCComMsgHdr & _rstComHdr, ExtAsIfComInfo & _rstExtAsIfComInfo);
		static bool m_fnDecodeBodyInitReq(const SVC_Fx_InitReq_t & _rstFxInitReqMsg, ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo);
		static bool m_fnDecodeBodyInitRsp(const SVC_Fx_Rsp_t & _rstFxInitRspMsg, ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo);
		static bool m_fnDecodeBodyPingReq(const SVC_Fx_PingReq_t & _rstFxPingReqMsg, ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo);
		static bool m_fnDecodeBodyPingRsp(const SVC_Fx_Rsp_t & _rstFxPingRspMsg, ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo);
		static bool m_fnDecodeBodySipEvent4SipHdr(bool _bInDlg, const SVC_Fx_Sip_t & _rstFxSipInfo, resip::SipMessage & _rclsSipMsg);
		static bool m_fnDecodeBodySipEvent4SipContent(const SVC_Fx_Sip_t & _rstFxSipInfo, resip::SipMessage & _rclsSipMsg);
		static bool m_fnDecodeBodySipEvent4Custom(const SVC_Fx_Sip_t & _rstFxSipInfo, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo);
		static bool m_fnDecodeBodyInitSipEvent4Custom(const SVC_Fx_Sip_t & _rstFxSipInfo, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, eSIP::SetSessionInfo & _rstSetSessionInfo);

		static const ExtAsIfCallback m_stFxStructCallback;
		static const ExtAsIfMgr * m_pclsExtAsIfMgr;
		
};

}

#endif

