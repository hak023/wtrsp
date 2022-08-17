
#ifndef COM_AS_EXT_AS_IF_HXX
#define COM_AS_EXT_AS_IF_HXX

#include "resip/stack/SipMessage.hxx"

#include "eSipUtil/string.hxx"

#include "eDum/eDumLibDef.hxx"
#include "eDum/eDumLibDialog.hxx"
#include "eDum/eDumLibSession.hxx"

#include "eSipDef.hxx"
#include "eSipAppMessage.hxx"

namespace ComAs
{

class ComAsSipEvent;
class ComAsAsyncEvent;

typedef enum
{
	E_EXT_AS_IF_flexStruct = 0,
	E_EXT_AS_IF_newFlex,

	E_EXT_AS_IF_NUM,
}EExtAsIfType;

const char * g_fnGetExtAsIfTypeStr(int _nExtAsIfType);

typedef enum
{
	E_EXT_AS_IF_MSG_initReq = 0,
	E_EXT_AS_IF_MSG_initRsp,
	E_EXT_AS_IF_MSG_pingReq,
	E_EXT_AS_IF_MSG_pingRsp,
	E_EXT_AS_IF_MSG_sip,
	E_EXT_AS_IF_MSG_internalTerm,

	E_EXT_AS_IF_MSG_NUM,
}EExtAsIfMsgType;

const char * g_fnGetExtAsIfMsgTypeStr(int _nExtAsIfMsgType);

typedef enum
{
	E_EXT_AS_IF_RSP_succ = 0,
	E_EXT_AS_IF_RSP_fail_invalid_compId,
	E_EXT_AS_IF_RSP_fail_invalid_appId,
	E_EXT_AS_IF_RSP_fail_unknown_tuple,
	E_EXT_AS_IF_RSP_fail_invalid_status,
	E_EXT_AS_IF_RSP_fail_internal_err,
	
	E_EXT_AS_IF_RSP_NUM,
}EExtAsIfRspResult_t;

const char * g_fnGetExtAsIfRspResultStr(int _nExtAsIfRspResultType);

struct ExtAsIfComInfo
{
	EExtAsIfMsgType m_eExtAsIfMsgType; 
	unsigned int m_unCmId;
	unsigned int m_unCmSessId;	
	unsigned int m_unAsId;
	unsigned int m_unAsSessId;
	unsigned int m_unReasonCode;

	ExtAsIfComInfo();

	const char * m_fnGetStr(char * _pszStr, unsigned int _unMaxLen) const;
};

struct ExtAsIfMsgInitReqInfo
{
	unsigned int m_unAsId;
	unsigned int m_unMaxSessNum;

	ExtAsIfMsgInitReqInfo();

	const char * m_fnGetStr(char * _pszStr, unsigned int _unMaxLen) const;
};

struct ExtAsIfMsgInitRspInfo
{
	EExtAsIfRspResult_t m_eExtAsIfRsp;
	unsigned int m_unMaxSessNum;

	ExtAsIfMsgInitRspInfo();

	const char * m_fnGetStr(char * _pszStr, unsigned int _unMaxLen) const;
};

struct ExtAsIfMsgPingReqInfo
{
	bool m_bEnable;
	unsigned int m_unCurrentSessNum;

	ExtAsIfMsgPingReqInfo();

	const char * m_fnGetStr(char * _pszStr, unsigned int _unMaxLen) const;
};

struct ExtAsIfMsgPingRspInfo
{
	EExtAsIfRspResult_t m_eExtAsIfRsp;
	bool m_bEnable;
	unsigned int m_unCurrentSessNum;

	ExtAsIfMsgPingRspInfo();

	const char * m_fnGetStr(char * _pszStr, unsigned int _unMaxLen) const;
};

struct ExtAsIfMsgSipPreInfo
{
	int m_nTuId;
	eDum::EDialogMethodType_t m_eDialogMethodType;
	resip::MethodTypes m_eSipMethod;
	int m_nRspCode;
	bool m_bInDialog;

	ExtAsIfMsgSipPreInfo();

	const char * m_fnGetStr(char * _pszStr, unsigned int _unMaxLen) const;
};

struct ExtAsIfMsgSipInfo
{
	eDum::Session * m_pclsSession;
	eDum::Dialog * m_pclsDialog;
	int m_nTuId;
	eDum::EDialogMethodType_t m_eDialogMethodType;
	bool m_bRcv;
	eSIP::Net5Tuple_t m_stNet5Tuple;
	resip::SipMessage * m_pclsSipMsg;

	ExtAsIfMsgSipInfo();

	const char * m_fnGetStr(char * _pszStr, unsigned int _unMaxLen) const;
	void m_fnApplySipEvent(const ComAsSipEvent & _rstSipEvent);
};

/*========================== callback for ExtAsIfPacket ========================================*/
typedef unsigned int (* PFuncGetMinMsgLen4Tcp)();
typedef int (* PFuncGetTotalMsgLen)(const void * _pExtAsIfPacket, unsigned int _unPacketLen);
//callback for Custom Value of ExtAs
typedef bool (* PFuncEncodeExtAsIfPacket4CustomSipEvent)(void * _pExtAsIfPacket, const char * _pszKey, const char * _pszValue);
typedef const char * (* PFuncDecodeExtAsIfPacket4CustomSipEvent)(const void * _pExtAsIfPacket, const char * _pszKey, bool & _bResult);
//callback for encoding packet of ExtAs
typedef void * (* PFuncEncodeExtAsIfPacket4InitReq)(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
typedef void * (* PFuncEncodeExtAsIfPacket4InitRsp)(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
typedef void * (* PFuncEncodeExtAsIfPacket4PingReq)(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
typedef void * (* PFuncEncodeExtAsIfPacket4PingRsp)(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
typedef void * (* PFuncEncodeExtAsIfPacket4SipEvent)(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
typedef void * (* PFuncEncodeExtAsIfPacket4InternalTerm)(const ExtAsIfComInfo & _rstExtAsIfComInfo, unsigned int & _runPacketSize, char ** _ppPacketStr);
//callback for decoding packet of ExtAs
typedef EExtAsIfMsgType (* PFuncDecodeCommonInfo)(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfComInfo & _rstExtAsIfComInfo);
typedef bool (* PFuncDecodeExtAsIfPacket4InitReq)(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo, char ** _ppPacketStr);
typedef bool (* PFuncDecodeExtAsIfPacket4InitRsp)(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo, char ** _ppPacketStr);
typedef bool (* PFuncDecodeExtAsIfPacket4PingReq)(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo, char ** _ppPacketStr);
typedef bool (* PFuncDecodeExtAsIfPacket4PingRsp)(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo, char ** _ppPacketStr);
typedef bool (* PFuncGetPreInfoExtAsIfPacket4SipEvent)(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgSipPreInfo & _rstExtAsIfSipPreInfo, char ** _ppPacketStr);
typedef bool (* PFuncDecodeExtAsIfPacket4SipEventInDlg)(const ComAsAsyncEvent & _rstAsyncEvent, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo);
typedef bool (* PFuncDecodeExtAsIfPacket4SipEventOutDlg)(const void * _pExtAsIfPacket, unsigned int _unPacketLen, 
																														ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, eSIP::SetSessionInfo & _rstSetSessionInfo);

struct ExtAsIfCallback
{
	PFuncGetMinMsgLen4Tcp m_pfnGetMinMsgLen4Tcp;
	PFuncGetTotalMsgLen m_pfnGetTotalMsgLen;
	//custom
	PFuncEncodeExtAsIfPacket4CustomSipEvent m_pfnEncodeCustomSipEvent;
	PFuncDecodeExtAsIfPacket4CustomSipEvent m_pfnDecodeCustomSipEvent;
	//encode
	PFuncEncodeExtAsIfPacket4InitReq m_pfnEncodeInitReq;
	PFuncEncodeExtAsIfPacket4InitRsp m_pfnEncodeInitRsp;
	PFuncEncodeExtAsIfPacket4PingReq m_pfnEncodePingReq;
	PFuncEncodeExtAsIfPacket4PingRsp m_pfnEncodePingRsp;
	PFuncEncodeExtAsIfPacket4SipEvent m_pfnEncodeSipEvent;
	PFuncEncodeExtAsIfPacket4InternalTerm m_pfnEncodeInternalTerm;
	//decode
	PFuncDecodeCommonInfo m_pfnDecodeCommonInfo;
	PFuncDecodeExtAsIfPacket4InitReq m_pfnDecodeInitReq;
	PFuncDecodeExtAsIfPacket4InitRsp m_pfnDecodeInitRsp;
	PFuncDecodeExtAsIfPacket4PingReq m_pfnDecodePingReq;
	PFuncDecodeExtAsIfPacket4PingRsp m_pfnDecodePingRsp;
	PFuncGetPreInfoExtAsIfPacket4SipEvent m_pfnGetPreInfoFromSipEvent;
	PFuncDecodeExtAsIfPacket4SipEventInDlg m_pfnDecodeSipEventInDialog;
	PFuncDecodeExtAsIfPacket4SipEventOutDlg m_pfnDecodeSipEventOutDialog;

	ExtAsIfCallback()
	{
		m_pfnGetMinMsgLen4Tcp = NULL;
		m_pfnGetTotalMsgLen = NULL;

		m_pfnEncodeCustomSipEvent = NULL;
		m_pfnDecodeCustomSipEvent = NULL;

		m_pfnEncodeInitReq = NULL;
		m_pfnEncodeInitRsp = NULL;
		m_pfnEncodePingReq = NULL;
		m_pfnEncodePingRsp = NULL;
		m_pfnEncodeSipEvent = NULL;
		m_pfnEncodeInternalTerm = NULL;

		m_pfnDecodeCommonInfo = NULL;
		m_pfnDecodeInitReq = NULL;
		m_pfnDecodeInitRsp = NULL;
		m_pfnDecodePingReq = NULL;
		m_pfnDecodePingRsp = NULL;
		m_pfnGetPreInfoFromSipEvent = NULL;
		m_pfnDecodeSipEventInDialog = NULL;
		m_pfnDecodeSipEventOutDialog = NULL;
	}

	ExtAsIfCallback(PFuncGetMinMsgLen4Tcp _pfnGetMinMsgLen4Tcp,
										PFuncGetTotalMsgLen _pfnGetTotalMsgLen,
										PFuncEncodeExtAsIfPacket4CustomSipEvent _pfnEncodeCustomSipEvent,
										PFuncDecodeExtAsIfPacket4CustomSipEvent _pfnDecodeCustomSipEvent,
										PFuncEncodeExtAsIfPacket4InitReq _pfnEncodeInitReq,
										PFuncEncodeExtAsIfPacket4InitRsp _pfnEncodeInitRsp,
										PFuncEncodeExtAsIfPacket4PingReq _pfnEncodePingReq,
										PFuncEncodeExtAsIfPacket4PingRsp _pfnEncodePingRsp,
										PFuncEncodeExtAsIfPacket4SipEvent _pfnEncodeSipEvent,
										PFuncEncodeExtAsIfPacket4InternalTerm _pfnEncodeInternalTerm,
										PFuncDecodeCommonInfo  _pfnDecodeCommonInfo,
										PFuncDecodeExtAsIfPacket4InitReq _pfnDecodeInitReq,
										PFuncDecodeExtAsIfPacket4InitRsp _pfnDecodeInitRsp,
										PFuncDecodeExtAsIfPacket4PingReq _pfnDecodePingReq,
										PFuncDecodeExtAsIfPacket4PingRsp _pfnDecodePingRsp,
										PFuncGetPreInfoExtAsIfPacket4SipEvent _pfnGetPreInfo4SipEvent,
										PFuncDecodeExtAsIfPacket4SipEventInDlg _pfnDecodeSipEvent4InDialog,
										PFuncDecodeExtAsIfPacket4SipEventOutDlg _pfnDecodeSipEvent4OutDialog)
	{
		m_pfnGetMinMsgLen4Tcp = _pfnGetMinMsgLen4Tcp;
		m_pfnGetTotalMsgLen = _pfnGetTotalMsgLen;

		m_pfnEncodeCustomSipEvent = _pfnEncodeCustomSipEvent;
		m_pfnDecodeCustomSipEvent = _pfnDecodeCustomSipEvent;

		m_pfnEncodeInitReq = _pfnEncodeInitReq;
		m_pfnEncodeInitRsp = _pfnEncodeInitRsp;
		m_pfnEncodePingReq = _pfnEncodePingReq;
		m_pfnEncodePingRsp = _pfnEncodePingRsp;
		m_pfnEncodeSipEvent = _pfnEncodeSipEvent;
		m_pfnEncodeInternalTerm = _pfnEncodeInternalTerm;

		m_pfnDecodeCommonInfo = _pfnDecodeCommonInfo;
		m_pfnDecodeInitReq = _pfnDecodeInitReq;
		m_pfnDecodeInitRsp = _pfnDecodeInitRsp;
		m_pfnDecodePingReq = _pfnDecodePingReq;
		m_pfnDecodePingRsp = _pfnDecodePingRsp;
		m_pfnGetPreInfoFromSipEvent = _pfnGetPreInfo4SipEvent;
		m_pfnDecodeSipEventInDialog = _pfnDecodeSipEvent4InDialog;
		m_pfnDecodeSipEventOutDialog = _pfnDecodeSipEvent4OutDialog;
	}
};


/*========================== callback for Solution(set/get Custom of SipEvent) ========================================*/
typedef bool (* PFuncEncodeSolution4CustomSipEvent)(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, void * _pExtAsIfPacket);
typedef bool (* PFuncDecodeSolution4CustomSipEvent)(const void * _pExtAsIfPacket, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo);
typedef bool (* PFuncDecodeSolution4InitCustomSipEvent)(const void * _pExtAsIfPacket, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, eSIP::SetSessionInfo & _rstSetSessionInfo);

struct SolutionExtAsCallback
{
	PFuncEncodeSolution4CustomSipEvent m_pfnEncodeSolution4CustomSipEvent;
	PFuncDecodeSolution4CustomSipEvent m_pfnDecodeSolution4CustomSipEvent;
	PFuncDecodeSolution4InitCustomSipEvent m_pfnDecodeSolution4InitCustomSipEvent;

	SolutionExtAsCallback()
	{
		m_pfnEncodeSolution4CustomSipEvent = NULL;
		m_pfnDecodeSolution4CustomSipEvent = NULL;
		m_pfnDecodeSolution4InitCustomSipEvent = NULL;
	}

	SolutionExtAsCallback(PFuncEncodeSolution4CustomSipEvent _pfnEncodeSolution4CustomSipEvent,
														PFuncDecodeSolution4CustomSipEvent _pfnDecodeSolution4CustomSipEvent,
														PFuncDecodeSolution4InitCustomSipEvent _pfnDecodeSolution4InitCustomSipEvent)
	{
		m_pfnEncodeSolution4CustomSipEvent = _pfnEncodeSolution4CustomSipEvent;
		m_pfnDecodeSolution4CustomSipEvent = _pfnDecodeSolution4CustomSipEvent;
		m_pfnDecodeSolution4InitCustomSipEvent = _pfnDecodeSolution4InitCustomSipEvent;
	}
};


class ExtAsIfMgr
{
	public :
		ExtAsIfMgr(int _nExtAsIfType, int _nSolutionType);
		~ExtAsIfMgr();

		static bool m_fnCheckApplySipHdr(bool _bInDlg, const char * _pszHdrName);		//for decode

		unsigned int m_fnGetMinMsgLen4Tcp() const;
		int m_fnGetTotalMsgLen(const void * _pExtAsIfPacket, unsigned int _unPacketLen) const;

		/*==== custum of ExtAsIfPacket ====*/
		bool m_fnEncodeExtAsIfPacket4CustomSipEvent(void * _pExtAsIfPacket, const char * _pszKey, const char * _pszValue) const;
		const char * m_fnDecodeExtAsIfPacket4CustomSipEvent(const void * _pExtAsIfPacket, const char * _pszKey, bool & _bResult) const;
		/*==== encode of ExtAsIfPacket ====*/
		void * m_fnEncodeExtAsIfPacket4InitReq(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo, unsigned int & _runPacketSize, char ** _ppPacketStr) const;
		void * m_fnEncodeExtAsIfPacket4InitRsp(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo, unsigned int & _runPacketSize, char ** _ppPacketStr) const;
		void * m_fnEncodeExtAsIfPacket4PingReq(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo, unsigned int & _runPacketSize, char ** _ppPacketStr) const;
		void * m_fnEncodeExtAsIfPacket4PingRsp(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo, unsigned int & _runPacketSize, char ** _ppPacketStr) const;
		void * m_fnEncodeExtAsIfPacket4SipEvent(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, unsigned int & _runPacketSize, char ** _ppPacketStr) const;
		void * m_fnEncodeExtAsIfPacket4InternalTerm(const ExtAsIfComInfo & _rstExtAsIfComInfo,unsigned int & _runPacketSize, char ** _ppPacketStr) const;
		/*==== decode of ExtAsIfPacket ====*/
		EExtAsIfMsgType m_fnDecodeExtAsIfPacket4CommonInfo(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfComInfo & _rstExtAsIfComInfo) const;
		bool m_fnDecodeExtAsIfPacket4InitReq(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo, char ** _ppPacketStr) const;
		bool m_fnDecodeExtAsIfPacket4InitRsp(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo, char ** _ppPacketStr) const;
		bool m_fnDecodeExtAsIfPacket4PingReq(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo, char ** _ppPacketStr) const;
		bool m_fnDecodeExtAsIfPacket4PingRsp(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgPingRspInfo & _rstExtAsIfRspInfo, char ** _ppPacketStr) const;
		bool m_fnGetPreInfoExtAsIfPacket4SipEvent(const void * _pExtAsIfPacket, unsigned int _unPacketLen, ExtAsIfMsgSipPreInfo & _rstExtAsIfSipPreInfo, char ** _ppPacketStr) const;
		bool m_fnDecodeExtAsIfPacket4SipEventInDialog(const ComAsAsyncEvent & _rstAsyncEvent, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo) const;
		bool m_fnDecodeExtAsIfPacket4SipEventOutDialog(const void * _pExtAsIfPacket, unsigned int _unPacketLen, 
																														ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo,
																														eSIP::SetSessionInfo & _rstSetSessionInfo) const;
		/*==== SolutitionExtAs ====*/
		bool m_fnEncodeSolution4CustomSipEvent(const ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, void * _pExtAsIfPacket) const;
		bool m_fnDecodeSolution4CustomSipEvent(const void * _pExtAsIfPacket, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo) const;
		bool m_fnDecodeSolution4InitCustomSipEvent(const void * _pExtAsIfPacket, ExtAsIfMsgSipInfo & _rstExtAsIfSipInfo, eSIP::SetSessionInfo & _rstSetSessionInfo) const;
	
	private :		
		ExtAsIfCallback m_stExtAsIfCallback;
		SolutionExtAsCallback m_stSolutionExtAsCallback;

};

}

#endif

