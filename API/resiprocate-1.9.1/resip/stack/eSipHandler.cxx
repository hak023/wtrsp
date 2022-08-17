
#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#ifdef ESIP_IN_RESIP

#include "resip/stack/eSipHandler.hxx"
#include "resip/stack/SipMessage.hxx"
#include "resip/stack/StatisticsManager.hxx"
#include "resip/stack/HeaderTypes.hxx"
#include "resip/stack/SipStack.hxx"
#include "resip/stack/InternalTransport.hxx"
#include "resip/stack/TransactionController.hxx"
#include "resip/stack/ExtensionParameter.hxx"

#include "eSipUtil/timeUtil.hxx"

namespace resip
{

#define SIMPLE_PARSING_STR(parsingData) \
			<<" : method["<<getMethodName(parsingData.m_eMethodType)<<"], rspcode["<<parsingData.m_nRspCode<<"], CallId["<<parsingData.m_szCallId<<"]" \
			<<", Initial["<<parsingData.m_bInitial<<"], From["<<parsingData.m_szFromUsername<<"], To["<<parsingData.m_szToUsername<<"]"

typedef struct
{
	EMsgDropType_t m_eMsgDropType;
	const char * m_pszDesc;
}MsgDropTypeInfo_t;

MsgDropTypeInfo_t s_arrMsgDropTypeInfo[E_DROP_TYPE_NUM] =
{
	{E_DROP_TYPE_acl, 							"ErrByAcl"},
	{E_DROP_TYPE_cps, 						"ErrByCps"},
	{E_DROP_TYPE_tps, 						"ErrByTps"},
	{E_DROP_TYPE_cpuOver, 					"ErrByCpuOver"},
	{E_DROP_TYPE_memOver, 				"ErrByMemOver"},
	{E_DROP_TYPE_msgSizeErr, 			"ErrByMsgSizeOver"},
	{E_DROP_TYPE_msgSizeDrop, 			"DropByMsgSizeOver"},
	{E_DROP_TYPE_sipCallIdError, 			"DropBySipCallid"},
	{E_DROP_TYPE_sipViaError, 				"DropBySipVia"},
	{E_DROP_TYPE_sipFromError, 			"DropBySipFrom"},	
	{E_DROP_TYPE_sipToError, 				"DropBySipTo"},	
	{E_DROP_TYPE_sipCseqError, 			"DropBySipCseq"},	
	{E_DROP_TYPE_sipContactError, 		"DropBySipContact"},	
	{E_DROP_TYPE_sipContentLenError, "DropBySipContentLen"},		
	{E_DROP_TYPE_sipReqLineError, 		"DropBySipReqLine"},	
	{E_DROP_TYPE_sipStatusLineError, 	"DropBySipStatusLine"},
	{E_DROP_TYPE_sipOtherError, 			"DropBySipOther"},
	{E_DROP_TYPE_systemShutdown, 	"DropBySystemShutdown"},
};

const char * getMsgDropDesc(EMsgDropType_t _eMsgDropType)
{
	if( _eMsgDropType >= 0 && _eMsgDropType < E_DROP_TYPE_NUM )
		return s_arrMsgDropTypeInfo[_eMsgDropType].m_pszDesc;
	else
		return "";
}

EMsgDropType_t getMsgDropType2HeaderType(Headers::Type _eHeader)
{
	switch(_eHeader)
	{
		case Headers::CallID :
		{
			return E_DROP_TYPE_sipCallIdError;
		}
		break;
		case Headers::Via :
		{
			return E_DROP_TYPE_sipViaError;
		}
		break;
		case Headers::From :
		{
			return E_DROP_TYPE_sipFromError;
		}
		break;
		case Headers::To :
		{
			return E_DROP_TYPE_sipToError;
		}
		break;
		case Headers::CSeq :
		{
			return E_DROP_TYPE_sipCseqError;
		}
		break;
		case Headers::Contact :
		{
			return E_DROP_TYPE_sipContactError;
		}
		break;
		case Headers::ContentLength :
		{
			return E_DROP_TYPE_sipContentLenError;
		}
		break;
		default :	break;
	}

	return E_DROP_TYPE_sipOtherError;
}

Headers::Type getHeaderType2MsgDropType(EMsgDropType_t _eMsgDropType)
{
	switch(_eMsgDropType)
	{
		case E_DROP_TYPE_sipCallIdError :
		{
			return Headers::CallID;
		}
		break;
		case E_DROP_TYPE_sipViaError :
		{
			return Headers::Via;
		}
		break;
		case E_DROP_TYPE_sipFromError :
		{
			return Headers::From;
		}
		break;
		case E_DROP_TYPE_sipToError :
		{
			return Headers::To;
		}
		break;
		case E_DROP_TYPE_sipCseqError :
		{
			return Headers::CSeq;
		}
		break;
		case E_DROP_TYPE_sipContactError :
		{
			return Headers::Contact;
		}
		break;
		case E_DROP_TYPE_sipContentLenError :
		{
			return Headers::ContentLength;
		}
		break;
		default :	break;
	}

	return Headers::UNKNOWN;
}

typedef struct
{
	EDelayTestType_t m_eDelayTestType;
	unsigned int m_unResetCnt;
	unsigned int m_unLimitDalyMs;
}DelayTestTypeInfo_t;

DelayTestTypeInfo_t s_arrDelayTestTypeInfo[E_DELAY_TEST_TYPE_NUM] = 
{
	{E_DELAY_TEST_TYPE_TRSP, 100000, 10},
	{E_DELAY_TEST_TYPE_MIDDLE_TRSC, 100000, 10},
	{E_DELAY_TEST_TYPE_MONITOR, 100000, 10},
};

unsigned int getResetCnt4DelayTest(EDelayTestType_t _eDelayTestType)
{
	if( _eDelayTestType >= 0 && _eDelayTestType < E_DELAY_TEST_TYPE_NUM )
		return s_arrDelayTestTypeInfo[_eDelayTestType].m_unResetCnt;
	else
		return 0;
}

unsigned int getLimitMs4DelayTest(EDelayTestType_t _eDelayTestType)
{
	if( _eDelayTestType >= 0 && _eDelayTestType < E_DELAY_TEST_TYPE_NUM )
		return s_arrDelayTestTypeInfo[_eDelayTestType].m_unLimitDalyMs;
	else
		return 0;
}

void SipParsingInfo::apply(const SipMessage & _rclsSipMsg)
{
	reset();

	m_eMethodType = _rclsSipMsg.method();
	
	if( _rclsSipMsg.isRequest() )
	{
		m_nRspCode = 0;
		if( !_rclsSipMsg.empty(h_To) && !_rclsSipMsg.header(h_To).exists(p_tag) )
			m_bInitial = true;
		else
			m_bInitial = false;
	}
	else
	{
		m_nRspCode = _rclsSipMsg.header(h_StatusLine).responseCode();
		m_bInitial = false;
	}
	
	if( !_rclsSipMsg.empty(h_AcceptContacts) )
	{
		if( _rclsSipMsg.header(h_AcceptContacts).front().exists(p_video) )
			m_bVideo = true;
		else
			m_bVideo = false;
	}
	else
		m_bVideo = false;

	Data clsParamName("phone-context");
	ExtensionParameter clsPhoneCtxParam(clsParamName);
	if( !_rclsSipMsg.empty(h_From) )
	{
		m_pszFromUsername = _rclsSipMsg.header(h_From).uri().user().c_str();
		if( _rclsSipMsg.header(h_From).uri().exists(clsPhoneCtxParam) )
			m_pszFromPhoneCtx = _rclsSipMsg.header(h_From).uri().param(clsPhoneCtxParam).c_str();
		if( _rclsSipMsg.header(h_From).exists(p_tag) )
			m_pszFromTag = _rclsSipMsg.header(h_From).param(p_tag).c_str();
	}
	if( !_rclsSipMsg.empty(h_To) )
	{
		m_pszToUsername = _rclsSipMsg.header(h_To).uri().user().c_str();
		if( _rclsSipMsg.header(h_To).uri().exists(clsPhoneCtxParam) )
			m_pszToPhoneCtx = _rclsSipMsg.header(h_To).uri().param(clsPhoneCtxParam).c_str();
		if( _rclsSipMsg.header(h_To).exists(p_tag) )
			m_pszToTag = _rclsSipMsg.header(h_To).param(p_tag).c_str();
	}

	if( !_rclsSipMsg.empty(h_CallId) )
		m_pszCallId =  _rclsSipMsg.header(h_CallId).value().c_str();

	DebugLog(<<"SipParsingInfo : apply() : MethodType["<<resip::getMethodName(m_eMethodType)<<"], RspCode["<<m_nRspCode<<"], Initial["<<m_bInitial<<"], Video["<<m_bVideo<<"]"
					<<", FromUser["<<m_pszFromUsername<<"], FromPhoneCtx["<<m_pszFromPhoneCtx<<"], FromTag["<<m_pszFromTag<<"]"
					<<", ToUser["<<m_pszToUsername<<"], ToPhoneCtx["<<m_pszToPhoneCtx<<"], ToTag["<<m_pszToTag<<"], CallID["<<m_pszCallId<<"]");
}

void SipParsingInfo::reset()
{
	static const char * pszBlank = "";

	m_pszFromUsername = pszBlank;
	m_pszFromPhoneCtx = pszBlank;
	m_pszFromTag = pszBlank;
	m_pszToUsername = pszBlank;
	m_pszToPhoneCtx = pszBlank;
	m_pszToTag = pszBlank;
	m_pszCallId = pszBlank;
}

TransportType convertNetTrspType2ResipType(eSipUtil::ETransportType_t _eNetTrspType)
{
	if( _eNetTrspType == eSipUtil::E_TRANSPORT_TYPE_udp )
	{
		return UDP;
	}
	else if( _eNetTrspType == eSipUtil::E_TRANSPORT_TYPE_tcp )
	{
		return TCP;
	}
	else if( _eNetTrspType == eSipUtil::E_TRANSPORT_TYPE_tls )
	{
		return TLS;
	}
	else
	{
		return UNKNOWN_TRANSPORT;
	}

}

bool convertNet5TupleToResipTuple(const eSipUtil::Net5Tuple & _rstNet5Tuple, Tuple & _rclsLocal, Tuple & _rclsRemote)
{
	GenericIPAddress stGenericLocalAddr, stGenericRemoteAddr;

	if( eSipUtil::isIpv6(_rstNet5Tuple.m_szLocalIp) )
	{
		struct sockaddr_in6 sockAddr;
		memset(&sockAddr, 0x00, sizeof(sockAddr));
		sockAddr.sin6_family = AF_INET6;
		if( inet_pton(AF_INET6, _rstNet5Tuple.m_szLocalIp, &(sockAddr.sin6_addr)) != 1 )
		{
			ErrLog(<<"convertNet5TupleToResipTuple : fail : can't convert LocalIp v6");
			return false;
		}
		sockAddr.sin6_port = htons(_rstNet5Tuple.m_nLocalPort);
		stGenericLocalAddr.v6Address = sockAddr;
	}
	else
	{
		struct sockaddr_in sockAddr;
		memset(&sockAddr, 0x00, sizeof(sockAddr));
		sockAddr.sin_family = AF_INET;
		if( inet_pton(AF_INET, _rstNet5Tuple.m_szLocalIp, &(sockAddr.sin_addr)) != 1 )
		{
			ErrLog(<<"convertNet5TupleToResipTuple : fail : can't convert LocalIp v4");
			return false;
		}
		sockAddr.sin_port = htons(_rstNet5Tuple.m_nLocalPort);
		stGenericLocalAddr.v4Address = sockAddr;
	}

	if( eSipUtil::isIpv6(_rstNet5Tuple.m_szRemoteIp) )
	{
		struct sockaddr_in6 sockAddr;
		memset(&sockAddr, 0x00, sizeof(sockAddr));
		sockAddr.sin6_family = AF_INET6;
		if( inet_pton(AF_INET6, _rstNet5Tuple.m_szRemoteIp, &(sockAddr.sin6_addr)) != 1 )
		{
			ErrLog(<<"convertNet5TupleToResipTuple : fail : can't convert RemoteIp v6");
			return false;
		}
		sockAddr.sin6_port = htons(_rstNet5Tuple.m_nRemotePort);
		stGenericRemoteAddr.v6Address = sockAddr;
	}
	else
	{
		struct sockaddr_in sockAddr;
		memset(&sockAddr, 0x00, sizeof(sockAddr));
		sockAddr.sin_family = AF_INET;
		if( inet_pton(AF_INET, _rstNet5Tuple.m_szRemoteIp, &(sockAddr.sin_addr)) != 1 )
		{
			ErrLog(<<"convertNet5TupleToResipTuple : fail : can't convert RemoteIp v4");
			return false;
		}
		sockAddr.sin_port = htons(_rstNet5Tuple.m_nRemotePort);
		stGenericRemoteAddr.v4Address = sockAddr;
	}

	TransportType eResipTrspType = convertNetTrspType2ResipType(_rstNet5Tuple.m_eTransportType);
	_rclsLocal.setType(eResipTrspType);
	_rclsRemote.setType(eResipTrspType);
	
	_rclsLocal.setSockaddr(stGenericLocalAddr);		
	_rclsLocal.setPort(_rstNet5Tuple.m_nLocalPort);
	_rclsRemote.setSockaddr(stGenericRemoteAddr);		
	_rclsRemote.setPort(_rstNet5Tuple.m_nRemotePort);

	return true;
}

SipMessage * convertFromSlbMsgToResipMsg(const SlbInternalMsgHdr & _rstInternalMsg)
{
	SipMessage * pclsSipMsg = NULL;
	Tuple clsSource, clsRecvLocal;
	char * pszNewRawSip = NULL;
	const SlbRcvSipBody * pBody = (const SlbRcvSipBody *)(_rstInternalMsg.m_fnGetBody());

	/*==== create raw sip ====*/
	pszNewRawSip = (char *)malloc(pBody->m_unSipLen+1);
	if( !pszNewRawSip )
	{
		ErrLog(<<"convertFromSlbMsgToResipMsg : fail : can't create SipBuffer : CallId["<<pBody->m_szCallId<<"]");
		goto ERR_PROC;
	}
	memcpy(pszNewRawSip, pBody->m_fnGetSipMsg(), pBody->m_unSipLen);
	pszNewRawSip[pBody->m_unSipLen] = 0x00;

	/*==== create SipMessage ====*/
	pclsSipMsg = new SipMessage();
	if( !pclsSipMsg )
	{
		ErrLog(<<"convertFromSlbMsgToResipMsg : fail : can't create SipMessage : CallId["<<pBody->m_szCallId<<"]");
		goto ERR_PROC;
	}
	pclsSipMsg->setFromExternal();
	if( !convertNet5TupleToResipTuple(pBody->m_stRcvTuple, clsRecvLocal, clsSource) )
	{
		ErrLog(<<"convertFromSlbMsgToResipMsg : fail : can't convert Tuple : CallId["<<pBody->m_szCallId<<"]");
		goto ERR_PROC;
	}
	pclsSipMsg->setRecvTuple(pBody->m_stRcvTuple);
	pclsSipMsg->setReceivedLocalTuple(clsRecvLocal);
	pclsSipMsg->setSource(clsSource);			
	pclsSipMsg->addBuffer(pszNewRawSip);
	pclsSipMsg->setRelayTcp(pBody->m_stInternalSipInfo.m_bTcpRelay);
	pszNewRawSip = NULL;
	
	return 	pclsSipMsg;

ERR_PROC :
	free(pszNewRawSip);
	delete pclsSipMsg;
	
	return NULL;
}

eSipHandler::eSipHandler()
	: m_pfnRcvTraceCb(NULL), m_pfnSndTraceCb(NULL), m_pfnGenerateToTag4Rsp(NULL),
	m_pfnCheckOverload(NULL), m_pfnCheckOverload4Slb(NULL), m_pfnCheckSip(NULL), m_pfnCheckSip4Slb(NULL), m_pfnFaultMsgCb(NULL),
	m_bDumpSip(false), m_bDumpSip4Cps(true),
	m_pclsStatMgr(NULL), m_pclsSipStack(NULL)
{
}

eSipHandler::~eSipHandler()
{
}

bool eSipHandler::doRcvTransportJob(SipMessage & _rclsSipMsg, const char * _pszSip)
{
	if( !_pszSip || strlen(_pszSip) == 0 || !_rclsSipMsg.getReceivedTransport() )
	{
		ErrLog(<<"doRcvTransportJob : fail : invalid argument");
		return false;
	}

	eSipUtil::DelayTest stDelayTest;
	SimpleParsingData stSimpleParsingData;
	unsigned int unSipLen = strlen(_pszSip);
	const Tuple & rclsLocalTuple = _rclsSipMsg.getReceivedTransport()->getTuple();
	const Tuple & rclsRemoteTuple = _rclsSipMsg.getSource();
	SipNodeLocal_t stNodeLocal;
	SipNodeRemote_t stNodeRemote;
	SipRoute_t stRoute;
	TraceInfo_t stTrace;

	/*==== dump SIP ====*/
	stDelayTest.setStartDelayTest("RcvTrspJob:dump");
	dumpSip(true, _pszSip, unSipLen, 
					Tuple::inet_ntop(rclsLocalTuple).c_str(), rclsLocalTuple.getPort(), 
					Tuple::inet_ntop(rclsRemoteTuple).c_str(), rclsRemoteTuple.getPort(), rclsLocalTuple.getType());
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));

	/*==== SimpleParsing ====*/
	stDelayTest.setStartDelayTest("RcvTrspJob:SimpleParsing");
	SimpleParsing::parsing(_pszSip, unSipLen, stSimpleParsingData, rclsLocalTuple.getType());
	stSimpleParsingData.log();
	if( strlen(stSimpleParsingData.m_szCallId) > 0 )
	{
		_rclsSipMsg.setSimpleCallId(stSimpleParsingData.m_szCallId, strlen(stSimpleParsingData.m_szCallId));
	}
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));	

	/*==== set Rte/Trace/Statistics ====*/
	stDelayTest.setStartDelayTest("RcvTrspJob:Rte/Trace/Stat");
	setRte(stSimpleParsingData, rclsLocalTuple, rclsRemoteTuple, stNodeLocal, stNodeRemote, stRoute);
	DebugLog(<<"RcvTrspJob : LocalId["<<stNodeLocal.m_uiID<<"], RemoteId["<<stNodeRemote.m_uiID<<"], RouteId["<<stRoute.m_uiID<<"]");
	onRecv(_rclsSipMsg, _pszSip, unSipLen, stNodeLocal, stNodeRemote, stRoute, stSimpleParsingData, stTrace);
	_rclsSipMsg.setLocalId(stNodeLocal.m_uiID);
	_rclsSipMsg.setRemoteId(stNodeRemote.m_uiID);
	_rclsSipMsg.setRouteId(stRoute.m_uiID);
	_rclsSipMsg.setTraceInfo(stTrace);
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));

	/*==== check ACL and Overload ====*/
	stDelayTest.setStartDelayTest("RcvTrspJob:Overload");
	if( !checkOverload((Transport *)(_rclsSipMsg.getReceivedTransport()), 
									_rclsSipMsg, _pszSip, unSipLen,
									rclsLocalTuple, rclsRemoteTuple, stNodeLocal, stNodeRemote, stRoute, 
									stSimpleParsingData, stTrace) )
	{
		return false;
	}
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));	

	/*==== check congestion of queue ====*/
#if 0
	stDelayTest.setStartDelayTest("RcvTrspJob:Congest");
	{
		resip::Data clsCallId(stSimpleParsingData.m_szCallId);
		Data clsCongestionDesc;
		if( m_pclsSipStack && m_pclsSipStack->checkCongestion(clsCallId, clsCongestionDesc) )
		{
			ErrLog(<<"RcvTrspJob : fail : congestion : "<<clsCongestionDesc
						<<" : method["<<getMethodName(stSimpleParsingData.m_eMethodType)<<"], rspcode["<<stSimpleParsingData.m_nRspCode<<"], CallId["<<stSimpleParsingData.m_szCallId<<"]"
						<<", Initial["<<stSimpleParsingData.m_bInitial<<"], From["<<stSimpleParsingData.m_szFromUsername<<"], To["<<stSimpleParsingData.m_szToUsername<<"]");
			return false;
		}
	}
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));	
#endif

	/*==== push Perform Thread ====*/
	stDelayTest.setStartDelayTest("RcvTrspJob:add");
	if( !MiddleTranscFifoThread4Perform::add(m_pclsSipStack, &_rclsSipMsg) )
	{
		ErrLog(<<"RcvTrspJob : fail : can't add"
					<<" : method["<<getMethodName(stSimpleParsingData.m_eMethodType)<<"], rspcode["<<stSimpleParsingData.m_nRspCode<<"], CallId["<<stSimpleParsingData.m_szCallId<<"]"
					<<", Initial["<<stSimpleParsingData.m_bInitial<<"], From["<<stSimpleParsingData.m_szFromUsername<<"], To["<<stSimpleParsingData.m_szToUsername<<"]");
		return false;
	}
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));

	return true;
}

bool eSipHandler::doRcvTransportJob4WithSlb(SipMessage & _rclsSipMsg, const char * _pszSip, const SlbInternalSipInfo & _rstInternalSipInfo)
{
	if( !_pszSip || strlen(_pszSip) == 0 )
	{
		ErrLog(<<"doRcvTransportJob4WithSlb : fail : invalid argument");
		return false;
	}

	eSipUtil::DelayTest stDelayTest;
	SimpleParsingData stSimpleParsingData;
	unsigned int unSipLen = strlen(_pszSip);
	const Tuple & rclsLocalTuple = _rclsSipMsg.getReceivedLocalTuple();
	const Tuple & rclsRemoteTuple = _rclsSipMsg.getSource();
	SipNodeLocal_t stNodeLocal;
	SipNodeRemote_t stNodeRemote;
	SipRoute_t stRoute;
	TraceInfo_t stTrace;

	/*==== dump SIP ====*/
	stDelayTest.setStartDelayTest("RcvTrspJob:dump");
	dumpSip(true, _pszSip, unSipLen, 
					Tuple::inet_ntop(rclsLocalTuple).c_str(), rclsLocalTuple.getPort(), 
					Tuple::inet_ntop(rclsRemoteTuple).c_str(), rclsRemoteTuple.getPort(), rclsLocalTuple.getType());
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));

	/*==== SimpleParsing ====*/
	stDelayTest.setStartDelayTest("RcvTrspJob:SimpleParsing");
	stSimpleParsingData.m_fnApply(_pszSip, unSipLen, _rstInternalSipInfo);
	stSimpleParsingData.log();
	if( strlen(stSimpleParsingData.m_szCallId) > 0 )
	{
		_rclsSipMsg.setSimpleCallId(stSimpleParsingData.m_szCallId, strlen(stSimpleParsingData.m_szCallId));
	}
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));	

	/*==== set Rte/Trace/Statistics ====*/
	stDelayTest.setStartDelayTest("RcvTrspJob:Rte/Trace/Stat");
	setRte(stSimpleParsingData, rclsLocalTuple, rclsRemoteTuple, stNodeLocal, stNodeRemote, stRoute);
	DebugLog(<<"RcvTrspJob : LocalId["<<stNodeLocal.m_uiID<<"], RemoteId["<<stNodeRemote.m_uiID<<"], RouteId["<<stRoute.m_uiID<<"]");
	onRecv(_rclsSipMsg, _pszSip, unSipLen, stNodeLocal, stNodeRemote, stRoute, stSimpleParsingData, stTrace);
	_rclsSipMsg.setLocalId(stNodeLocal.m_uiID);
	_rclsSipMsg.setRemoteId(stNodeRemote.m_uiID);
	_rclsSipMsg.setRouteId(stRoute.m_uiID);
	_rclsSipMsg.setTraceInfo(stTrace);
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));	

	/*==== check ACL and Overload ====*/
	stDelayTest.setStartDelayTest("RcvTrspJob:Overload");
	if( !checkOverload4Slb(*(m_pclsSipStack->getTransportMgr4Slb()),
										_rclsSipMsg, _pszSip, unSipLen, 
										rclsLocalTuple, rclsRemoteTuple, stNodeLocal, stNodeRemote, stRoute, 
										stSimpleParsingData, stTrace) )
	{
		return false;
	}
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));	

	/*==== check congestion of queue ====*/
#if 0
	stDelayTest.setStartDelayTest("RcvTrspJob:Congest");
	{
		resip::Data clsCallId(stSimpleParsingData.m_szCallId);
		Data clsCongestionDesc;
		if( m_pclsSipStack && m_pclsSipStack->checkCongestion(clsCallId, clsCongestionDesc) )
		{
			ErrLog(<<"doRcvTransportJob4WithSlb : fail : congestion : "<<clsCongestionDesc
						<<" : method["<<getMethodName(stSimpleParsingData.m_eMethodType)<<"], rspcode["<<stSimpleParsingData.m_nRspCode<<"], CallId["<<stSimpleParsingData.m_szCallId<<"]"
						<<", Initial["<<stSimpleParsingData.m_bInitial<<"], From["<<stSimpleParsingData.m_szFromUsername<<"], To["<<stSimpleParsingData.m_szToUsername<<"]");
			return false;
		}
	}
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));	
#endif

	/*==== push Perform Thread ====*/
	stDelayTest.setStartDelayTest("RcvTrspJob:add");
	if( !MiddleTranscFifoThread4Perform::add(m_pclsSipStack, &_rclsSipMsg) )
	{
		ErrLog(<<"doRcvTransportJob4WithSlb : fail : can't add"
					<<" : method["<<getMethodName(stSimpleParsingData.m_eMethodType)<<"], rspcode["<<stSimpleParsingData.m_nRspCode<<"], CallId["<<stSimpleParsingData.m_szCallId<<"]"
					<<", Initial["<<stSimpleParsingData.m_bInitial<<"], From["<<stSimpleParsingData.m_szFromUsername<<"], To["<<stSimpleParsingData.m_szToUsername<<"]");
		return false;
	}
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_TRSP, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_TRSP), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_TRSP));

	return true;
}

void eSipHandler::setDumpSip4Cps(bool _bDumpFlag4Cps)
{
	if( m_bDumpSip4Cps != _bDumpFlag4Cps )
		ErrLog(<<"setDumpSip4Cps : "<<m_bDumpSip4Cps<<" ===> "<<_bDumpFlag4Cps);

	m_bDumpSip4Cps = _bDumpFlag4Cps;
}

void eSipHandler::dumpSip(bool _bRcv, const char * _pszSipString, unsigned int _unLen,
												const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType)
{
	if( !_pszSipString || _unLen == 0 || !_pszLocalIp || !_pszRemoteIp )
		return;

	if( m_bDumpSip && m_bDumpSip4Cps )
	{
		if( _bRcv )
		{
			DumpLog(<<"DumpSip Rcv [Remote("<<_pszRemoteIp<<":"<<_nRemotePort<<") --->Local("<<_pszLocalIp<<":"<<_nLocalPort<<") "<<toData(_eTransportType)<<"], Len["<<_unLen<<"]\n"
							<<_pszSipString);
		}
		else
		{
			DumpLog(<<"DumpSip Snd [Local("<<_pszLocalIp<<":"<<_nLocalPort<<") --->Remote("<<_pszRemoteIp<<":"<<_nRemotePort<<") "<<toData(_eTransportType)<<"], Len["<<_unLen<<"]\n"
							<<_pszSipString);
		}
	}
}

void eSipHandler::onRecv(const SipMessage & _rclsSipMsg, const char * _pszSipString, unsigned int _unLen,
											const SipNodeLocal_t & _rstNodeLocal, const SipNodeRemote_t & _rstNodeRemote, const SipRoute_t & _rstRoute,
											const SimpleParsingData & _rstSimpleParsing, TraceInfo_t & _rstTraceInfo)
{
	//trace
	if( m_pfnRcvTraceCb )
		m_pfnRcvTraceCb(_pszSipString, _unLen, _rstNodeLocal, _rstNodeRemote, _rstRoute, _rstSimpleParsing, _rstTraceInfo);

	//statistics
	if( m_pclsStatMgr )
		m_pclsStatMgr->received(_rstSimpleParsing.m_eMethodType, _rstSimpleParsing.m_nRspCode, _rstNodeLocal.m_szIP, _rstNodeLocal.m_usPort, _rstNodeRemote.m_szIP, _rstNodeRemote.m_usPort, (TransportType)(_rstNodeLocal.m_ucProto));
}

void eSipHandler::onRecvDrop(EMsgDropType_t eMsgDropType, MethodTypes _eMsgMethodType, int _nRspCode,
														const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType)
{
	//statistics
	if( m_pclsStatMgr )
		m_pclsStatMgr->receivedDrop(eMsgDropType, _eMsgMethodType, _nRspCode, _pszLocalIp, _nLocalPort, _pszRemoteIp, _nRemotePort, _eTransportType);
}

void eSipHandler::onRecvDropByTransc(EMsgDropType_t eMsgDropType, MethodTypes _eMethodType, int _nRspCode, 
																				const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType)
{
	//statistics
	if( m_pclsStatMgr )
		m_pclsStatMgr->receivedDropByTransc(eMsgDropType, _eMethodType, _nRspCode, _pszLocalIp, _nLocalPort, _pszRemoteIp, _nRemotePort, _eTransportType);
}

void eSipHandler::onRecvRetrans(const SipMessage & _rclsSipMsg)
{
	//statistics
	if( m_pclsStatMgr )
		m_pclsStatMgr->receivedRetrans(_rclsSipMsg);
}

void eSipHandler::onSend(const Data & _rclsSipMsg, unsigned int _unLen, MethodTypes _eMsgMethodType, int _nRspCode,
												const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType,
												unsigned int _unLocalNodeId, unsigned int _unRemoteNodeId, unsigned int _unRouteId,
												const TraceInfo_t * _pstTraceInfo, bool _bRetrans)
{
	onSend(_rclsSipMsg.c_str(), _unLen, _eMsgMethodType, _nRspCode,
				_pszLocalIp, _nLocalPort, _pszRemoteIp, _nRemotePort, _eTransportType,
				_unLocalNodeId, _unRemoteNodeId, _unRouteId,
				_pstTraceInfo, _bRetrans);
}

void eSipHandler::onSend(const char * _pszSipMsg, unsigned int _unLen, MethodTypes _eMsgMethodType, int _nRspCode,
												const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType,
												unsigned int _unLocalNodeId, unsigned int _unRemoteNodeId, unsigned int _unRouteId,
												const TraceInfo_t * _pstTraceInfo, bool _bRetrans)
{
	//sip dump
	dumpSip(false, _pszSipMsg, _unLen, _pszLocalIp, _nLocalPort, _pszRemoteIp, _nRemotePort, _eTransportType);

	//trace
	if( m_pfnSndTraceCb && _pstTraceInfo )
		m_pfnSndTraceCb(_pszSipMsg, _unLen, _pszLocalIp, _nLocalPort, _pszRemoteIp, _nRemotePort, _eTransportType, _unLocalNodeId, _unRemoteNodeId, _unRouteId, *_pstTraceInfo);

	//statistics
	if( m_pclsStatMgr )
	{
		if( _bRetrans )
			m_pclsStatMgr->retransmitted(_eMsgMethodType, _nRspCode, _pszLocalIp, _nLocalPort, _pszRemoteIp, _nRemotePort, _eTransportType);
		else
			m_pclsStatMgr->sent(_eMsgMethodType, _nRspCode, _pszLocalIp, _nLocalPort, _pszRemoteIp, _nRemotePort, _eTransportType);
	}
}

void eSipHandler::setSipStack(SipStack * _pclsSipStack)
{
	m_pclsSipStack = _pclsSipStack;
}

void eSipHandler::setRte(const SimpleParsingData & _rstParsingData,
											const Tuple & _rclsLocal, const Tuple & _rclsRemote, 
											SipNodeLocal_t & _rstNodeLocal, SipNodeRemote_t & _rstNodeRemote, SipRoute_t & _rstRoute)
{
	memset(&_rstNodeLocal, 0x00, sizeof(SipNodeLocal_t));
	memset(&_rstNodeRemote, 0x00, sizeof(SipNodeRemote_t));
	memset(&_rstRoute, 0x00, sizeof(SipRoute_t));	
		
	_rstNodeLocal.m_ucProto = (unsigned char)_rclsLocal.getType();
	_rstNodeRemote.m_ucProto = (unsigned char)_rclsRemote.getType();

	if( _rstNodeRemote.m_ucProto == TCP || _rstNodeRemote.m_ucProto == TLS )
	{
		if( _rstParsingData.m_nRspCode == 0 )
		{
			strncpy(_rstNodeLocal.m_szIP, resip::Tuple::inet_ntop(_rclsLocal).c_str(), DEF_RTE_MAXLEN_IP-1);
			_rstNodeLocal.m_szIP[DEF_RTE_MAXLEN_IP-1] = 0x00;
			_rstNodeLocal.m_usPort = (unsigned short)(_rclsLocal.getPort());
			if( strlen(_rstParsingData.m_szViaIp) > 0 )
			{
				strncpy(_rstNodeRemote.m_szIP, _rstParsingData.m_szViaIp, DEF_RTE_MAXLEN_IP-1);
				_rstNodeRemote.m_szIP[DEF_RTE_MAXLEN_IP-1] = 0x00;
				_rstNodeRemote.m_usPort = (unsigned short)(_rstParsingData.m_nViaPort);			
			}
			
			IfESipRteApi_GetRouteByTransport(&_rstNodeLocal, &_rstNodeRemote, &_rstRoute, false);
			
			return;
		}
		else if( _rstParsingData.m_nRspCode > 0 )
		{
			if( strlen(_rstParsingData.m_szViaIp) > 0 )
			{
				strncpy(_rstNodeLocal.m_szIP, _rstParsingData.m_szViaIp, DEF_RTE_MAXLEN_IP-1);
				_rstNodeLocal.m_szIP[DEF_RTE_MAXLEN_IP-1] = 0x00;
				_rstNodeLocal.m_usPort = (unsigned short)(_rstParsingData.m_nViaPort);			
			}
			strncpy(_rstNodeRemote.m_szIP, resip::Tuple::inet_ntop(_rclsRemote).c_str(), DEF_RTE_MAXLEN_IP-1);
			_rstNodeRemote.m_szIP[DEF_RTE_MAXLEN_IP-1] = 0x00;
			_rstNodeRemote.m_usPort = (unsigned short)(_rclsRemote.getPort());
			
			IfESipRteApi_GetRouteByTransport(&_rstNodeLocal, &_rstNodeRemote, &_rstRoute, false);
			
			return;
		}
	}

	strncpy(_rstNodeLocal.m_szIP, resip::Tuple::inet_ntop(_rclsLocal).c_str(), DEF_RTE_MAXLEN_IP-1);
	_rstNodeLocal.m_szIP[DEF_RTE_MAXLEN_IP-1] = 0x00;
	_rstNodeLocal.m_usPort = (unsigned short)(_rclsLocal.getPort());
	strncpy(_rstNodeRemote.m_szIP, resip::Tuple::inet_ntop(_rclsRemote).c_str(), DEF_RTE_MAXLEN_IP-1);
	_rstNodeRemote.m_szIP[DEF_RTE_MAXLEN_IP-1] = 0x00;
	_rstNodeRemote.m_usPort = (unsigned short)(_rclsRemote.getPort());

	IfESipRteApi_GetRouteByTransport(&_rstNodeLocal, &_rstNodeRemote, &_rstRoute, false);
}

bool eSipHandler::checkOverload(Transport * _pclsTransport, 
																SipMessage & _rclsSipMsg, const char * _pBuffer, unsigned int _unLen,
																const Tuple & _rclsLocal, const Tuple & _rclsRemote, 
																const SipNodeLocal_t & _rstNodeLocal, const SipNodeRemote_t & _rstNodeRemote, const SipRoute_t & _rstRoute,
																const SimpleParsingData & _rstSimpleParsing, const TraceInfo_t & _rclsTrace)
{
	if( !_pclsTransport || !_pBuffer || _unLen == 0 )
	{
		ErrLog(<<"checkOverload : fail : invalid param");
		return false;
	}

	/*==== check overload ====*/
	if( m_pfnCheckOverload )
	{
		bool bRelayTcp = false, bNoDropCnt = false;
		EMsgDropType_t eMsgDropType = m_pfnCheckOverload(_pclsTransport, _pBuffer, _unLen,
																								_rclsLocal, _rclsRemote, 
																								_rstNodeLocal, _rstNodeRemote, _rstRoute,
																								_rstSimpleParsing, _rclsTrace, bRelayTcp, bNoDropCnt);
		
		
		if( eMsgDropType == E_DROP_TYPE_NUM )
		{
			_rclsSipMsg.setRelayTcp(bRelayTcp);
		}
		else
		{
			if( !bNoDropCnt )
			{
				if( m_bDumpSip4Cps )
				{
					ErrLog(<<"Drop["<<getMsgDropDesc(eMsgDropType)<<"] : fail"
								<<" : method["<<getMethodName(_rstSimpleParsing.m_eMethodType)<<"], rspcode["<<_rstSimpleParsing.m_nRspCode<<"], CallId["<<_rstSimpleParsing.m_szCallId<<"]"
								<<", Initial["<<_rstSimpleParsing.m_bInitial<<"], From["<<_rstSimpleParsing.m_szFromUsername<<"], To["<<_rstSimpleParsing.m_szToUsername<<"]"
								<<" : remoteIp["<<_rstNodeRemote.m_szIP<<"], remotePort["<<_rstNodeRemote.m_usPort<<"], localIp["<<_rstNodeLocal.m_szIP<<"], localPort["<<_rstNodeLocal.m_usPort<<"], type["<<toData((TransportType)(_rstNodeRemote.m_ucProto))<<"]");
				}

				onRecvDrop(eMsgDropType, _rstSimpleParsing.m_eMethodType, _rstSimpleParsing.m_nRspCode, 
									_rstNodeLocal.m_szIP, _rstNodeLocal.m_usPort, _rstNodeRemote.m_szIP, _rstNodeRemote.m_usPort, (TransportType)(_rstNodeRemote.m_ucProto));
			}

			return false;
		}
	}

	return true;
}

bool eSipHandler::checkOverload4Slb(TransportMgrWithSlb & _rclsTrspMgr, 
																		SipMessage & _rclsSipMsg, const char * _pBuffer, unsigned int _unLen,
																		const Tuple & _rclsLocal, const Tuple & _rclsRemote, 
																		const SipNodeLocal_t & _rstNodeLocal, const SipNodeRemote_t & _rstNodeRemote, const SipRoute_t & _rstRoute,
																		const SimpleParsingData & _rstSimpleParsing, const TraceInfo_t & _rclsTrace)
{
	if( !_pBuffer || _unLen == 0 )
	{
		ErrLog(<<"checkOverload4Slb : fail : invalid param");
		return false;
	}

	/*==== check overload ====*/
	if( m_pfnCheckOverload4Slb )
	{
		bool bRelayTcp = false, bNoDropCnt = false;
		EMsgDropType_t eMsgDropType = m_pfnCheckOverload4Slb(_rclsTrspMgr, _pBuffer, _unLen,
																										_rclsSipMsg.getRecvTuple(),
																										_rclsLocal, _rclsRemote, 
																										_rstNodeLocal, _rstNodeRemote, _rstRoute,
																										_rstSimpleParsing, _rclsTrace, bRelayTcp, bNoDropCnt);
		
		
		if( eMsgDropType == E_DROP_TYPE_NUM )
		{
			if( bRelayTcp )	//slb or ccm
				_rclsSipMsg.setRelayTcp(bRelayTcp);
		}
		else
		{
			if( !bNoDropCnt )
			{
				ErrLog(<<"Drop["<<getMsgDropDesc(eMsgDropType)<<"] : fail"
							<<" : method["<<getMethodName(_rstSimpleParsing.m_eMethodType)<<"], rspcode["<<_rstSimpleParsing.m_nRspCode<<"], CallId["<<_rstSimpleParsing.m_szCallId<<"]"
							<<", Initial["<<_rstSimpleParsing.m_bInitial<<"], From["<<_rstSimpleParsing.m_szFromUsername<<"], To["<<_rstSimpleParsing.m_szToUsername<<"]"
							<<" : remoteIp["<<_rstNodeRemote.m_szIP<<"], remotePort["<<_rstNodeRemote.m_usPort<<"], localIp["<<_rstNodeLocal.m_szIP<<"], localPort["<<_rstNodeLocal.m_usPort<<"], type["<<toData((TransportType)(_rstNodeRemote.m_ucProto))<<"]");

				onRecvDrop(eMsgDropType, _rstSimpleParsing.m_eMethodType, _rstSimpleParsing.m_nRspCode, 
									_rstNodeLocal.m_szIP, _rstNodeLocal.m_usPort, _rstNodeRemote.m_szIP, _rstNodeRemote.m_usPort, (TransportType)(_rstNodeRemote.m_ucProto));
			}

			return false;
		}
	}

	return true;
}


bool eSipHandler::checkSip(Transport * _pclsTransport, 
													SipMessage & _rclsSipMsg, const char * _pBuffer, unsigned int _unLen, 
													const Tuple & _rclsRemote, int _nRouteId, const TraceInfo_t & _rstTrace)
{
	if( !_pclsTransport || !_pBuffer || _unLen == 0 )
	{
		ErrLog(<<"checkSip : fail : invalid param");
		return false;
	}
	
	if( m_pfnCheckSip )
	{
		EMsgDropType_t eMsgDropType = m_pfnCheckSip(_pclsTransport, _rclsSipMsg, _pBuffer, _unLen, _rclsRemote, _nRouteId, _rstTrace);
		if( eMsgDropType != E_DROP_TYPE_NUM )
		{
			int nRspCode = 0;
			if( _rclsSipMsg.isRequest() )
				nRspCode = 0;
			else
				nRspCode = _rclsSipMsg.header(h_StatusLine).responseCode();

			Data clsCallId;
			if( !_rclsSipMsg.empty(h_CallId) )
				clsCallId = _rclsSipMsg.header(h_CallId).value();
			
			Data clsLocalIp, clsRemoteIp;
			int nLocalPort = -1;
			int nRemotePort = -1;
			if( _rclsSipMsg.getReceivedTransport() )
			{
				clsLocalIp = Tuple::inet_ntop(_rclsSipMsg.getReceivedTransport()->getTuple());
				nLocalPort = _rclsSipMsg.getReceivedTransport()->getTuple().getPort();
			}
			clsRemoteIp = Tuple::inet_ntop(_rclsSipMsg.getSource());
			nRemotePort = _rclsSipMsg.getSource().getPort();
				
			ErrLog(<<"checkSip : fail for eSIP : Drop["<<getMsgDropDesc(eMsgDropType)<<"]"
						<<" : method["<<getMethodName(_rclsSipMsg.method())<<"], rspcode["<<nRspCode<<"], CallId["<<clsCallId<<"]"
						<<" : remoteIp["<<clsRemoteIp<<"], remotePort["<<nRemotePort<<"], localIp["<<clsLocalIp<<"], localPort["<<nLocalPort<<"], type["<<toData(_rclsSipMsg.getSource().getType())<<"]");
			DebugLog(<<"\n"<<Data(Data::Borrow, _pBuffer, _unLen));

			onRecvDrop(eMsgDropType, _rclsSipMsg.method(), nRspCode, clsLocalIp.c_str(), nLocalPort, clsRemoteIp.c_str(), nRemotePort, _rclsSipMsg.getSource().getType());

			return false;
		}
	}

	return true;
}

bool eSipHandler::checkSip4Slb(TransportMgrWithSlb & _rclsTrspMgr, 
															SipMessage & _rclsSipMsg, const char * _pBuffer, unsigned int _unLen, 
															const Tuple & _rclsRemote, int _nRouteId, const TraceInfo_t & _rstTrace)
{
	if( !_pBuffer || _unLen == 0 )
	{
		ErrLog(<<"checkSip4Slb : fail : invalid param");
		return false;
	}
	
	if( m_pfnCheckSip4Slb )
	{
		EMsgDropType_t eMsgDropType = m_pfnCheckSip4Slb(_rclsTrspMgr, _rclsSipMsg, _pBuffer, _unLen, _rclsRemote, _nRouteId, _rstTrace);
		if( eMsgDropType != E_DROP_TYPE_NUM )
		{
			int nRspCode = 0;
			if( _rclsSipMsg.isRequest() )
				nRspCode = 0;
			else
				nRspCode = _rclsSipMsg.header(h_StatusLine).responseCode();

			Data clsCallId;
			if( !_rclsSipMsg.empty(h_CallId) )
				clsCallId = _rclsSipMsg.header(h_CallId).value();
			
			Data clsLocalIp, clsRemoteIp;
			int nLocalPort = -1;
			int nRemotePort = -1;
			clsLocalIp = Tuple::inet_ntop(_rclsSipMsg.getReceivedLocalTuple());
			nLocalPort = _rclsSipMsg.getReceivedLocalTuple().getPort();
			clsRemoteIp = Tuple::inet_ntop(_rclsSipMsg.getSource());
			nRemotePort = _rclsSipMsg.getSource().getPort();
				
			ErrLog(<<"checkSip4Slb : fail for eSIP : Drop["<<getMsgDropDesc(eMsgDropType)<<"]"
						<<" : method["<<getMethodName(_rclsSipMsg.method())<<"], rspcode["<<nRspCode<<"], CallId["<<clsCallId<<"]"
						<<" : remoteIp["<<clsRemoteIp<<"], remotePort["<<nRemotePort<<"], localIp["<<clsLocalIp<<"], localPort["<<nLocalPort<<"], type["<<toData(_rclsSipMsg.getSource().getType())<<"]");
			DebugLog(<<"\n"<<Data(Data::Borrow, _pBuffer, _unLen));

			onRecvDrop(eMsgDropType, _rclsSipMsg.method(), nRspCode, clsLocalIp.c_str(), nLocalPort, clsRemoteIp.c_str(), nRemotePort, _rclsSipMsg.getSource().getType());

			return false;
		}
	}

	return true;
}

void eSipHandler::setTimeTestFlag(bool _bTimeTest)
{
	eSipUtil::DelayTest::m_bUse = _bTimeTest;
}

TransportMgrWithSlb::TransportMgrWithSlb(SipStack & _rclsSipStack, eSipHandler & _rclsEsipHandler, PfuncQueueFull _pfnQueueFullCb, unsigned int _unMaxSipLen4Udp)
	: m_rclsSipStack(_rclsSipStack), m_rclsEsipHandler(_rclsEsipHandler), m_pclsInternalTrspMgr(NULL),
		m_pfnDelTransportCb(NULL), m_pfnConnected4NonblockCb(NULL),
		m_unSocketRcvBufferSize(0), m_unSocketSndBufferSize(0),
		m_bBlock(false)
{
	m_unRcvBufferSize = SlbInternalMsgHdr::m_fnGetHeaderLen()+sizeof(SlbRcvSipBody)+_unMaxSipLen4Udp;
	m_unSndBufferSize = m_unRcvBufferSize;

	eSipUtil::TransportMgrOption stTrspMgrOption;
	stTrspMgrOption.m_pszMgrName = "TrspWithSlb";
	stTrspMgrOption.m_unMaxTransportSize = 1;
	stTrspMgrOption.m_unMaxEpollSize = 1000;
	stTrspMgrOption.m_unMaxThreadQueueSize = m_rclsSipStack.getTransportMaxFifoSize();
	stTrspMgrOption.m_nThreadQueueWaitMs = -1;
	stTrspMgrOption.m_bUseSendThread = false;
	stTrspMgrOption.m_pfnQueueFullCb = _pfnQueueFullCb;
	m_pclsInternalTrspMgr = new eSipUtil::TransportMgr(stTrspMgrOption);
	assert(m_pclsInternalTrspMgr);
	m_pclsInternalTrspMgr->setAppData(this);
}

TransportMgrWithSlb::~TransportMgrWithSlb()
{
	delete m_pclsInternalTrspMgr;
}

bool TransportMgrWithSlb::m_fnAddTransport(const eSipUtil::Net5Tuple_t & _rstNet5Tuple, unsigned int _unSocketRcvBufferSize, unsigned int _unSocketSndBufferSize)
{
	char szNet5Tuple[eSipUtil::Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};

	if( m_stNet5Tuple.m_eTransportType != eSipUtil::E_TRANSPORT_TYPE_NUM )
	{
		ErrLog(<<"TransportMgrWithSlb : AddTransport : fail : already Transport :"<<_rstNet5Tuple.getStr(szNet5Tuple, eSipUtil::Net5Tuple::E_CONST_MAX_STR_LEN));
		return false;
	}

	eSipUtil::SocketCfg_t stSocketCfg;
	eSipUtil::TransportConfig_t stTransportCfg;

	stSocketCfg.m_nBlock = 0;
	stSocketCfg.m_nUseLinger = 1;
	stSocketCfg.m_nLingerWaitTime = 0;
	stSocketCfg.m_nReuse = 1;
	if( _unSocketRcvBufferSize > 0 )
		m_unSocketRcvBufferSize = _unSocketRcvBufferSize;
	if( _unSocketSndBufferSize > 0 )
		m_unSocketSndBufferSize = _unSocketSndBufferSize;
	stSocketCfg.m_nRcvSocketBufferSize = m_unSocketRcvBufferSize;
	stSocketCfg.m_nSndSocketBufferSize = m_unSocketSndBufferSize;
	stSocketCfg.m_nDscp = 0;

	stTransportCfg.m_stTcpConfig.m_nTcpRole = 1;
	stTransportCfg.m_stTcpConfig.m_unSndBufferSize = m_unSndBufferSize;
	stTransportCfg.m_stTcpConfig.m_unBacklogSize = 0;
	stTransportCfg.m_stTcpConfig.m_unMinRcvNotiSize = sizeof(SlbInternalMsgHdr);
	stTransportCfg.m_stTcpConfig.m_pfnTcpRcvMatchCb = m_fnCbCheckCompleteMsg4Rcv;
	stTransportCfg.m_stTcpConfig.m_pfnConnected4NonblockCb = m_fnCbConnected4NonBlock;	
	stTransportCfg.m_unRcvBufferSize = m_unRcvBufferSize;
	stTransportCfg.m_pfnRcvProcCb = m_fnCbRecv;
	stTransportCfg.m_pfnSndAfterProcCb = m_fnCbSndSucc;
	stTransportCfg.m_pfnDelTransportCb = m_fnCbDelTransport;

	CritLog(<<"TransportMgrWithSlb : AddTransport : "<<_rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));

	m_stNet5Tuple = _rstNet5Tuple;
	m_pclsInternalTrspMgr->addTransport(_rstNet5Tuple, stSocketCfg, stTransportCfg);
		
	return true;
}

void TransportMgrWithSlb::m_fnDelTransport(const eSipUtil::Net5Tuple_t & _rstNet5Tuple)
{
	char szNet5Tuple[eSipUtil::Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};

	CritLog(<<"TransportMgrWithSlb : DelTransport : "<<_rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));

	m_pclsInternalTrspMgr->delTransport(_rstNet5Tuple);
}

bool TransportMgrWithSlb::m_fnSendSip(const eSipUtil::Net5Tuple_t & _rstSlbTuple, const char * _pszSipMsg, unsigned int _unSipLen, 
																		MethodTypes _eMethodType, int _nRspCode, const char * _pszCallId,
																		unsigned int _unLocalNodeId, unsigned int _unRemoteNodeId, unsigned int _unRouteId,
																		const TraceInfo_t & _rstTraceInfo, bool _bRetrans)
{
	if( !_pszSipMsg || _unSipLen == 0 )
	{
		ErrLog(<<"TransportMgrWithSlb : SendSip : fail : invalid argument");
		return false;
	}

	if( m_stNet5Tuple.m_eTransportType == eSipUtil::E_TRANSPORT_TYPE_NUM )
	{
		ErrLog(<<"TransportMgrWithSlb : SendSip : fail : not found Transport");
		return false;
	}	

	if( strlen(_pszSipMsg) != _unSipLen )
	{
		ErrLog(<<"TransportMgrWithSlb : SendSip : fail : unMatch SipLen : Len1["<<strlen(_pszSipMsg)<<"] != ["<<_unSipLen<<"]");
		return false;
	}

	SlbInternalMsgHdr * pstInternalMsgHdr = (SlbInternalMsgHdr *)malloc(SlbInternalMsgHdr::m_fnGetHeaderLen()+sizeof(SlbSndSipBody)+_unSipLen+1);
	if( pstInternalMsgHdr )
	{
		char szPrintHdr[SlbInternalMsgHdr::E_CONST_MAX_STR_LEN] = {0,};

		pstInternalMsgHdr->m_fnReset();
		pstInternalMsgHdr->m_eMsgType = E_INTERNAL_MSG_TYPE_SND_SIP;		
		SlbSndSipBody * pstBody = (SlbSndSipBody *)(pstInternalMsgHdr->m_fnGetBody());
		pstBody->m_fnReset();
		pstBody->m_stSndTuple = _rstSlbTuple;
		pstBody->m_eMethodType = (ESlbInternalSipMethodType_t)_eMethodType;
		pstBody->m_nRspCode =_nRspCode;
		pstBody->m_fnSetCallId(_pszCallId);
		pstBody->m_unSipLen= _unSipLen;
		char * pszSipMsg = pstBody->m_fnGetSipMsg();
		memcpy(pszSipMsg, _pszSipMsg, _unSipLen);
		pszSipMsg[_unSipLen] = 0x00;
		pstInternalMsgHdr->m_unBodyLen = pstBody->m_fnGetBodyLen();

		DebugLog(<<"TransportMgrWithSlb : SendSip : SndLen["<<pstInternalMsgHdr->m_fnGetTotalLen()<<"]"
							<<" : "<<pstBody->m_fnGetStr(szPrintHdr, SlbInternalMsgHdr::E_CONST_MAX_STR_LEN, true));

		SndSipInfo * pstSndSipInfo = (SndSipInfo *)malloc(sizeof(struct SndSipInfo));
		if( pstSndSipInfo )
		{
			pstSndSipInfo->reset();
			pstSndSipInfo->m_eMsgMethodType = _eMethodType;
			pstSndSipInfo->m_nRspCode = _nRspCode;
			pstSndSipInfo->m_stSlbSndTuple = _rstSlbTuple;
			pstSndSipInfo->m_unLocalNodeId = _unLocalNodeId;
			pstSndSipInfo->m_unRemoteNodeId = _unRemoteNodeId;
			pstSndSipInfo->m_unRouteId = _unRouteId;
			if( _rstTraceInfo.m_bTraceFlag )
				pstSndSipInfo->m_stTraceInfo = _rstTraceInfo;
			pstSndSipInfo->m_bRetrans = _bRetrans;
		}
		else
		{
			ErrLog(<<"TransportMgrWithSlb : SendSip : can't create SndSipInfo : SndLen["<<pstInternalMsgHdr->m_fnGetTotalLen()<<"]"
						<<" : "<<pstBody->m_fnGetStr(szPrintHdr, SlbInternalMsgHdr::E_CONST_MAX_STR_LEN, true));
		}

		int nRet;
		eSipUtil::ETransportErr_t eTrspErr;
		if( (nRet = m_pclsInternalTrspMgr->sendData(m_stNet5Tuple, (char *)pstInternalMsgHdr, pstInternalMsgHdr->m_fnGetTotalLen(), -1, pstSndSipInfo, &eTrspErr)) < 0 )
		{
			ErrLog(<<"TransportMgrWithSlb : SendSip : fail : SndLen["<<pstInternalMsgHdr->m_fnGetTotalLen()<<"]"
						<<" : "<<eSipUtil::g_fnGetTransportErrStr(eTrspErr)
						<<" : "<<pstBody->m_fnGetStr(szPrintHdr, SlbInternalMsgHdr::E_CONST_MAX_STR_LEN, true));
			free(pstInternalMsgHdr);
			free(pstSndSipInfo);
			return false;
		}

		return true;
	}
	else
	{
		ErrLog(<<"TransportMgrWithSlb : SendSip : fail : can't create InternalMsgHdr");
		return false;
	}
}

bool TransportMgrWithSlb::m_fnSendInit(const char * _pszVcmId, const char * _pszTagPrefix, const char * _pszStackTagPrefix, unsigned int _unMaxSessNum)
{
	if( !_pszVcmId || strlen(_pszVcmId) == 0 )
	{
		ErrLog(<<"TransportMgrWithSlb : SendInit : fail : invalid VcmId");
		return false;
	}

	if( !_pszTagPrefix || strlen(_pszTagPrefix) == 0 )
	{
		ErrLog(<<"TransportMgrWithSlb : SendInit : fail : invalid TagPrefix");
		return false;
	}

	if( !_pszStackTagPrefix || strlen(_pszStackTagPrefix) == 0 )
	{
		ErrLog(<<"TransportMgrWithSlb : SendInit : fail : invalid StackTagPrefix");
		return false;
	}

	if( m_stNet5Tuple.m_eTransportType == eSipUtil::E_TRANSPORT_TYPE_NUM )
	{
		ErrLog(<<"TransportMgrWithSlb : SendInit : fail : not found Transport");
		return false;
	}

	SlbInternalMsgHdr * pstInternalMsgHdr = (SlbInternalMsgHdr *)malloc(SlbInternalMsgHdr::m_fnGetHeaderLen() + sizeof(SlbInitReqBody));
	if( pstInternalMsgHdr )
	{
		char szPrintHdr[SlbInternalMsgHdr::E_CONST_MAX_STR_LEN] = {0,};
	
		pstInternalMsgHdr->m_fnReset();
		pstInternalMsgHdr->m_eMsgType = E_INTERNAL_MSG_TYPE_INIT_REQ;
		SlbInitReqBody * pstBody = (SlbInitReqBody *)(pstInternalMsgHdr->m_fnGetBody());
		pstBody->m_fnReset();
		pstBody->m_unMaxSessionNum = _unMaxSessNum;
		pstBody->m_fnSetCcmId(_pszVcmId);
		pstBody->m_fnSetTagPrefix(_pszTagPrefix);
		pstBody->m_fnSetStackTagPrefix(_pszStackTagPrefix);
		pstInternalMsgHdr->m_unBodyLen = pstBody->m_fnGetBodyLen();

		DebugLog(<<"TransportMgrWithSlb : SendInit : SndLen["<<pstInternalMsgHdr->m_fnGetTotalLen()<<"]"
							<<" : "<<pstBody->m_fnGetStr(szPrintHdr, SlbInternalMsgHdr::E_CONST_MAX_STR_LEN, true));

		int nRet;
		eSipUtil::ETransportErr_t eTrspErr;
		if( (nRet = m_pclsInternalTrspMgr->sendData(m_stNet5Tuple, (char *)pstInternalMsgHdr, pstInternalMsgHdr->m_fnGetTotalLen(), -1, NULL, &eTrspErr)) < 0 )
		{
			ErrLog(<<"TransportMgrWithSlb : SendInit : fail : SndLen["<<pstInternalMsgHdr->m_fnGetTotalLen()<<"]"
						<<" : "<<eSipUtil::g_fnGetTransportErrStr(eTrspErr)
						<<" : "<<pstBody->m_fnGetStr(szPrintHdr, SlbInternalMsgHdr::E_CONST_MAX_STR_LEN, true));
			free(pstInternalMsgHdr);			
			return false;
		}

		return true;
	}
	else
	{
		ErrLog(<<"TransportMgrWithSlb : SendInit : fail : can't create InternalMsgHdr");
		return false;
	}
}

bool TransportMgrWithSlb::m_fnSendPing(const char * _pszVcmId, bool _bActive, unsigned int _unActiveSessNum)
{
	if( m_stNet5Tuple.m_eTransportType == eSipUtil::E_TRANSPORT_TYPE_NUM )
	{
		ErrLog(<<"TransportMgrWithSlb : SendPing : fail : not found Transport");
		return false;
	}

	SlbInternalMsgHdr * pstInternalMsgHdr = (SlbInternalMsgHdr *)malloc(SlbInternalMsgHdr::m_fnGetHeaderLen() + sizeof(SlbPingReqBody));
	if( pstInternalMsgHdr )
	{
		char szPrintHdr[SlbInternalMsgHdr::E_CONST_MAX_STR_LEN] = {0,};
	
		pstInternalMsgHdr->m_fnReset();
		pstInternalMsgHdr->m_eMsgType = E_INTERNAL_MSG_TYPE_PING_REQ;
		SlbPingReqBody * pstBody = (SlbPingReqBody *)(pstInternalMsgHdr->m_fnGetBody());
		pstBody->m_fnReset();
		pstBody->m_fnSetCcmId(_pszVcmId);
		pstBody->m_bActive = _bActive;
		pstBody->m_unActiveSessionNum = _unActiveSessNum;
		pstInternalMsgHdr->m_unBodyLen = pstBody->m_fnGetBodyLen();

		StackLog(<<"TransportMgrWithSlb : SendPing : SndLen["<<pstInternalMsgHdr->m_fnGetTotalLen()<<"]"
							<<" : "<<pstBody->m_fnGetStr(szPrintHdr, SlbInternalMsgHdr::E_CONST_MAX_STR_LEN, true));

		int nRet;
		eSipUtil::ETransportErr_t eTrspErr;
		if( (nRet = m_pclsInternalTrspMgr->sendData(m_stNet5Tuple, (char *)pstInternalMsgHdr, pstInternalMsgHdr->m_fnGetTotalLen(), -1, NULL, &eTrspErr)) < 0 )
		{
			ErrLog(<<"TransportMgrWithSlb : SendPing : fail : SndLen["<<pstInternalMsgHdr->m_fnGetTotalLen()<<"]"
						<<" : "<<eSipUtil::g_fnGetTransportErrStr(eTrspErr)
						<<" : "<<pstBody->m_fnGetStr(szPrintHdr, SlbInternalMsgHdr::E_CONST_MAX_STR_LEN, true));
			free(pstInternalMsgHdr);			
			return false;
		}

		return true;
	}
	else
	{
		ErrLog(<<"TransportMgrWithSlb : SendInit : fail : can't create InternalMsgHdr");
		return false;
	}
}

int TransportMgrWithSlb::m_fnCbCheckCompleteMsg4Rcv(eSipUtil::TransportMgr & _rclsTransportMgr, const char * _pRcvData, unsigned int _unRcvDataLen, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, void * _pAppBuffer)
{
	if( !_pRcvData || _unRcvDataLen == 0 )
		return -1;

	if( _unRcvDataLen < SlbInternalMsgHdr::m_fnGetHeaderLen() )
		return 0;

	SlbInternalMsgHdr * pHeader = (SlbInternalMsgHdr *)_pRcvData;
	
	return pHeader->m_fnGetTotalLen();
}

void TransportMgrWithSlb::m_fnCbRecv(eSipUtil::TransportMgr & _rclsTransportMgr, char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstRcvTuple)
{
	SipMessage * pclsSipMsg = NULL;
	SlbInternalMsgHdr * pHeader = NULL;
	const SlbRcvSipBody * pBody = NULL;
	EInternalMsgMatchResult_t eInternalMsgResult = E_INTERNAL_MSG_MATCH_RESULT_NUM;
	char szInternalMsg[SlbInternalMsgHdr::E_CONST_MAX_STR_LEN] = {0,};
	TransportMgrWithSlb * pclsTrspMgrWithSlb = (TransportMgrWithSlb *)(_rclsTransportMgr.getAppData());

	if( !_pData || _unDataLen == 0 || !pclsTrspMgrWithSlb )
	{
		ErrLog(<<"TransportMgrWithSlb : cbRecv : fail : invalid argument : RcvLen["<<_unDataLen<<"]");
		goto ERR_PROC;
	}

	/*==== validate InternalMsg ====*/
	pHeader = (SlbInternalMsgHdr *)_pData;
	eInternalMsgResult = pHeader->m_fnIsMatch(_pData, _unDataLen);
	if( eInternalMsgResult != E_INTERNAL_MSG_MATCH_RESULT_SUCC )
	{
		ErrLog(<<"TransportMgrWithSlb : cbRecv : fail : IsMatch : "<<eInternalMsgResult<<" : RcvLen["<<_unDataLen<<"]");
		goto ERR_PROC;
	}

	if( pHeader->m_eMsgType == E_INTERNAL_MSG_TYPE_PING_RSP )
		StackLog(<<"TransportMgrWithSlb : cbRecv : RcvLen["<<_unDataLen<<"] : "<<pHeader->m_fnGetStr(szInternalMsg, sizeof(szInternalMsg), true));
	else
		InfoLog(<<"TransportMgrWithSlb : cbRecv : RcvLen["<<_unDataLen<<"] : "<<pHeader->m_fnGetStr(szInternalMsg, sizeof(szInternalMsg), true));	

	switch(pHeader->m_eMsgType)
	{
		case E_INTERNAL_MSG_TYPE_INIT_RSP :
		case E_INTERNAL_MSG_TYPE_PING_RSP :
		case E_INTERNAL_MSG_TYPE_NOTI_RTE_STATUS :
		{
			if( pclsTrspMgrWithSlb->m_pfnRcvLbManageMsgCb )
				pclsTrspMgrWithSlb->m_pfnRcvLbManageMsgCb(*pclsTrspMgrWithSlb, _rstRcvTuple, pHeader);
			else
				goto ERR_PROC;
		}
		break;
		case E_INTERNAL_MSG_TYPE_RCV_SIP :
		{
			if( pclsTrspMgrWithSlb->m_bBlock )
			{
				DebugLog(<<"TransportMgrWithSlb : cbRecv : blocking");
				goto ERR_PROC;
			}
			
			pclsSipMsg = convertFromSlbMsgToResipMsg(*pHeader);
			if( pclsSipMsg )
			{
				pBody = (SlbRcvSipBody *)(pHeader->m_fnGetBody());
				if( !pclsTrspMgrWithSlb->m_rclsEsipHandler.doRcvTransportJob4WithSlb(*pclsSipMsg, pclsSipMsg->getBuffer(), pBody->m_stInternalSipInfo) )
				{
					delete pclsSipMsg;
				}
				free(_pData);
				_pData = NULL;
			}
			else
				goto ERR_PROC;
		}
		break;
		default :
		{
			ErrLog(<<"TransportMgrWithSlb : cbRecv : fail : invalid Type["<<pHeader->m_eMsgType<<"] : RcvLen["<<_unDataLen<<"] : "<<pHeader->m_fnGetStr(szInternalMsg, sizeof(szInternalMsg), true));		
			goto ERR_PROC;
		}
	}

	return;
	
ERR_PROC :
	free(_pData);
	
}

void TransportMgrWithSlb::m_fnCbSndSucc(eSipUtil::TransportMgr & _rclsTransportMgr, const char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstSndTuple, void * _pSndAppData)
{
	TransportMgrWithSlb * pclsTrspMgrWithSlb = (TransportMgrWithSlb *)(_rclsTransportMgr.getAppData());

	if( !_pData || _unDataLen == 0 || !pclsTrspMgrWithSlb )
	{
		ErrLog(<<"TransportMgrWithSlb : CbSndSucc : fail : invalid argument : RcvLen["<<_unDataLen<<"]");
		return;
	}

	if( _pData && strncmp(_pData, MAGIC_COOKIE, MSG_MAGIC_COOKIE_LEN) == 0 )
	{
		char szInternalMsg[SlbInternalMsgHdr::E_CONST_MAX_STR_LEN] = {0,};
		const SlbInternalMsgHdr * pstSndHeader = (const SlbInternalMsgHdr *)_pData;

		if( pstSndHeader->m_eMsgType == E_INTERNAL_MSG_TYPE_PING_REQ )
			StackLog(<<"TransportMgrWithSlb : CbSndSucc : SndLen["<<_unDataLen<<"] : "<<pstSndHeader->m_fnGetStr(szInternalMsg, sizeof(szInternalMsg), true));
		else
			InfoLog(<<"TransportMgrWithSlb : CbSndSucc : SndLen["<<_unDataLen<<"] : "<<pstSndHeader->m_fnGetStr(szInternalMsg, sizeof(szInternalMsg), true));

		if( pstSndHeader->m_eMsgType == E_INTERNAL_MSG_TYPE_SND_SIP && _pSndAppData )
		{
			const SlbSndSipBody * pstSndSipBody = (const SlbSndSipBody *)(pstSndHeader->m_fnGetBody());
			const char * pszSipMsg = pstSndSipBody->m_fnGetSipMsg();
			const SndSipInfo * pstSndSipInfo = (const SndSipInfo *)_pSndAppData;
			const TraceInfo_t * pstTraceInfo = NULL;
			if( pstSndSipInfo->m_stTraceInfo.m_bTraceFlag )
				pstTraceInfo = &(pstSndSipInfo->m_stTraceInfo);
			
			pclsTrspMgrWithSlb->m_rclsEsipHandler.onSend(pszSipMsg, pstSndSipBody->m_unSipLen, pstSndSipInfo->m_eMsgMethodType, pstSndSipInfo->m_nRspCode, 
																						pstSndSipInfo->m_stSlbSndTuple.m_szLocalIp, pstSndSipInfo->m_stSlbSndTuple.m_nLocalPort, 
																						pstSndSipInfo->m_stSlbSndTuple.m_szRemoteIp, pstSndSipInfo->m_stSlbSndTuple.m_nRemotePort, 
																						convertNetTrspType2ResipType(pstSndSipInfo->m_stSlbSndTuple.m_eTransportType),
																						pstSndSipInfo->m_unLocalNodeId, pstSndSipInfo->m_unRemoteNodeId, pstSndSipInfo->m_unRouteId, 
																						pstTraceInfo, pstSndSipInfo->m_bRetrans);
		}
	}
}

void TransportMgrWithSlb::m_fnCbDelTransport(eSipUtil::TransportMgr & _rclsTransportMgr, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, eSipUtil::ETransportErr_t _eTrspErr)
{
	TransportMgrWithSlb * pclsTrspMgrWithSlb = (TransportMgrWithSlb *)(_rclsTransportMgr.getAppData());

	if( !pclsTrspMgrWithSlb )
	{
		ErrLog(<<"TransportMgrWithSlb : CbDelTransport : fail : invalid argument");
		return;
	}
	
	pclsTrspMgrWithSlb->m_stNet5Tuple.reset();
	if( pclsTrspMgrWithSlb->m_pfnDelTransportCb )
	{
		DebugLog(<<"TransportMgrWithSlb : CbDelTransport : execute CbDelTransport");
		pclsTrspMgrWithSlb->m_pfnDelTransportCb(*pclsTrspMgrWithSlb, _rstNet5Tuple, _eTrspErr);
	}
}

void TransportMgrWithSlb::m_fnCbConnected4NonBlock(eSipUtil::TransportMgr & _rclsTransportMgr, const eSipUtil::Net5Tuple_t & _rstNet5Tuple)
{
	TransportMgrWithSlb * pclsTrspMgrWithSlb = (TransportMgrWithSlb *)(_rclsTransportMgr.getAppData());

	if( !pclsTrspMgrWithSlb )
	{
		ErrLog(<<"TransportMgrWithSlb : CbConnected4NonBlock : fail : invalid argument");
		return;
	}

	if( pclsTrspMgrWithSlb->m_pfnConnected4NonblockCb )
	{
		DebugLog(<<"TransportMgrWithSlb : CbConnected4NonBlock : execute CbConnected4NonBlock");
		pclsTrspMgrWithSlb->m_pfnConnected4NonblockCb(*pclsTrspMgrWithSlb, _rstNet5Tuple);
	}
}


}

#endif

