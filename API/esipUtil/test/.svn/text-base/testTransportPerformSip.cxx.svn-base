
#include "testTransportPerformSip.hxx"
#include "testTransport.hxx"
#include "bufferControl.hxx"
#include "log.hxx"

using namespace eSipUtil;

extern int g_nLogInstanceId;

#define LogTest(_level, _format, args...) Log(g_nLogInstanceId, 0, _level, _format, ##args)

static const unsigned int s_unMaxCallIdLen = 256;
static const unsigned int s_unMaxSipLen = 5000;
static const char * s_pszEndLine = "\r\n";
static const char * s_pszEndHeader = "\r\n\r\n";
static const char * s_pszCallIdHeaderName = "\r\nCall-ID";
static const char * s_pszContentLengthName = "\r\nContent-Length";
static const char * g_pszLineDelimiter = 
"============================================================================\n\
============================================================================";

static const unsigned int s_unMaxPerformSipUasNum = 5;
static PerformSipUas s_clsPerformSipUas[s_unMaxPerformSipUasNum];

extern bool sendData(const Net5Tuple_t & _rstNet5Tuple, char * _pszSipMsg, unsigned int _unMsgLen);
extern TransportEntityInfo g_clsTransportEntityInfo;

static const char * s_pszInv100 = 
"SIP/2.0 100 Trying\r\n\
Via: SIP/2.0/UDP 223.33.173.35:5066;received=223.33.173.35;rport=5066;branch=z9hG4bK-524287-1---c996ec63c3dda70f\r\n\
From: <sip:01088033061@sktims.net:5060;user=phone>;tag=skt19815c6\r\n\
To: \"+82-1032214843\"<tel:+82-1032214843>\r\n\
Call-ID%s\r\n\
CSeq: 1 INVITE\r\n\
Content-Length: 0\r\n\r\n";

static const char * s_pszInv180 = 
"SIP/2.0 180 Ringing\r\n\
Via: SIP/2.0/UDP 223.33.173.35:5066;received=223.33.173.35;rport=5066;branch=z9hG4bK-524287-1---c996ec63c3dda70f\r\n\
From: <sip:01088033061@sktims.net:5060;user=phone>;tag=skt19815c6\r\n\
To: \"+82-1032214843\"<tel:+82-1032214843>\r\n\
Call-ID%s\r\n\
CSeq: 1 INVITE\r\n\
Content-Length: 0\r\n\r\n";

static const char * s_pszInv200 = 
"SIP/2.0 200 OK\r\n\
Via: SIP/2.0/UDP 211.188.227.78:5062;branch=z9hG4bK7f71ccb07e0032f7cd40_d296e\r\n\
Record-Route: <sip:server@125.144.154.130:5060;lr>;ck\r\n\
To: <tel:+82-1029213332>;tag=27a26a80-2f90907d-13c4-1808288-26c01762-1808288\r\n\
From: <sip:01020965222@sktims.net>;tag=236d6d8eb6d28\r\n\
Call-ID%s\r\n\
CSeq: 1 INVITE\r\n\
Contact: <sip:server@125.144.154.130:5060>\r\n\
Require: 100rel,early-session\r\n\
RSeq: 1\r\n\
Accept:  application/sdp\r\n\
P-Called-Party-ID: <tel:+821029213332>\r\n\
P-Charging-Vector: icid-value=5694BEE8A381B4A0.gr-ibc01\r\n\
Content-Type: application/sdp\r\n\
Content-Disposition: early-session\r\n\
Content-Length:  273\r\n\
\r\n\
v=0\r\n\
o=KT-IBCF 1452588776 1452588776 IN IP4 125.144.154.15\r\n\
s=CNSR-XIP\r\n\
c=IN IP4 125.144.154.15\r\n\
t=0 0\r\n\
m=audio 43094 RTP/AVP 100 101\r\n\
b=AS:41\r\n\
a=rtpmap:100 AMR-WB/16000\r\n\
a=fmtp:100 octet-align=1; mode-set=8\r\n\
a=rtpmap:101 telephone-event/8000\r\n\
a=fmtp:101 0-15\r\n\
a=sendrecv\r\n";

static const char * s_pszBye200 = 
"SIP/2.0 200 OK\r\n\
Via: SIP/2.0/TCP 223.33.173.35:5066;received=223.33.173.35;rport=5066;branch=z9hG4bK-524287-1---c996ec63c3dda70f\r\n\
From: <sip:01088033061@sktims.net:5060;user=phone>;tag=skt19815c6\r\n\
To: \"+82-1032214843\"<tel:+82-1032214843>;tag=27a26a80-2f90907d\r\n\
Call-ID%s\r\n\
CSeq: 2 BYE\r\n\
Content-Length: 0\r\n\r\n";

typedef enum
{
	SIP_METHOD_invite = 0,
	SIP_METHOD_bye,
	SIP_METHOD_ack,

	SIP_METHOD_NUM,
}E_SIP_METHOD_t;

struct SipMethodInfo
{
	E_SIP_METHOD_t m_eSipMethod;
	const char * m_pszMethod;
};

static const SipMethodInfo s_arrMethodInfo[SIP_METHOD_NUM] =
{
	{SIP_METHOD_invite, 		"INVITE"},
	{SIP_METHOD_bye, 			"BYE"},
	{SIP_METHOD_ack, 			"ACK"},
};

int cbCheckCompletePerformSip4Rcv(TransportMgr & _rclsTransportMgr, const char * _pRcvData, unsigned int _unRcvDataLen, const Net5Tuple_t & _rstNet5Tuple, void * _pAppBuffer)
{
	if( !_pRcvData || _unRcvDataLen == 0 )
		return -1;

	const char * pStartContentLengthPos = NULL, * pEndContentLengthPos = NULL, * pEndTotalHeaderPos = NULL;
	unsigned int unContentLength = 0;
	if( (pStartContentLengthPos = strstr(_pRcvData, s_pszContentLengthName)) )
	{
		pStartContentLengthPos += strlen(s_pszContentLengthName);
		if( (pEndContentLengthPos = strstr(pStartContentLengthPos, s_pszEndLine)) && (pEndContentLengthPos > pStartContentLengthPos) )
		{
			BufferControl clsBufferControl(pStartContentLengthPos, pEndContentLengthPos-pStartContentLengthPos);
			if( (pStartContentLengthPos = clsBufferControl.findChar(':')) )
			{
				clsBufferControl.move(1);
				clsBufferControl.skipWhitespace();
				pStartContentLengthPos = clsBufferControl.getStartPos();
				if( pEndContentLengthPos > pStartContentLengthPos )
				{
					char szContentLength[pEndContentLengthPos-pStartContentLengthPos+1];
					memcpy(szContentLength, pStartContentLengthPos, pEndContentLengthPos-pStartContentLengthPos);
					szContentLength[pEndContentLengthPos-pStartContentLengthPos] = 0x00;
					unContentLength = atoi(szContentLength);
				}
			}
		}
	}

	if( (pEndTotalHeaderPos = strstr(_pRcvData, s_pszEndHeader)) )
	{
		int nTotalLen = pEndTotalHeaderPos-_pRcvData+strlen(s_pszEndHeader)+unContentLength;
		LogTest(E_LOG_DEBUG, "cbCheckCompletePerformSip4Rcv callback : Complete SIP Length[%d], HeaderLen[%li] Content-Length[%u]", 
									nTotalLen, pEndTotalHeaderPos-_pRcvData, unContentLength);
		return nTotalLen;
	}
	else
	{
		LogTest(E_LOG_DEBUG, "cbCheckCompletePerformSip4Rcv callback : unknown length. BufferCurrentLen[%u]", _unRcvDataLen); 
		return 0;
	}
}

void cbRecvPerformSip(TransportMgr & _rclsTransportMgr, char * _pData, unsigned int _unDataLen, const Net5Tuple_t & _rstNet5Tuple)
{
	char szNet5Tuple[Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};

	if( !_pData || _unDataLen == 0 )
	{
		LogTest(E_LOG_ERR, "TestRecvPerformSip callback : fail : invalid argument : Data[%p], DataLen[%u] : %s", _pData, _unDataLen, _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
		free(_pData);
		return;
	}

	TransportEntity stTransportEntity;
	RcvSipData_t * pstRcvSipData = new RcvSipData_t();
	if( !pstRcvSipData )
	{
		LogTest(E_LOG_ERR, "TestRecvPerformSip callback : fail : can't create RcvSipData : %s", _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
		free(_pData);
		return;
	}

	LogTest(E_LOG_DEBUG, "TestRecvPerformSip callback : %s, Data[%s], DataLen[%u]", _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)), _pData, _unDataLen);

	pstRcvSipData->m_stNet5Tuple = _rstNet5Tuple;
	pstRcvSipData->m_pszSip = _pData;
	pstRcvSipData->m_unSipLen = _unDataLen;
	if( g_clsTransportEntityInfo.getTransportEntity(_rstNet5Tuple, stTransportEntity) )
	{
		if( stTransportEntity.m_nPerformSipUasIndex >= 0 && stTransportEntity.m_nPerformSipUasIndex < (int)s_unMaxPerformSipUasNum )
		{
			LogTest(E_LOG_DEBUG, "TestRecvPerformSip callback : sendTo PerformSipUas[%d] : %s", stTransportEntity.m_nPerformSipUasIndex, _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
			pstRcvSipData->m_nUasIndex = stTransportEntity.m_nPerformSipUasIndex;
			s_clsPerformSipUas[stTransportEntity.m_nPerformSipUasIndex].sendTo(pstRcvSipData);
			return;
		}
		else
		{
			LogTest(E_LOG_ERR, "TestRecvPerformSip callback : fail : invalid PerformSipUasIndex : %s", _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
			delete pstRcvSipData;
			return;
		}
	}
	else
	{
		LogTest(E_LOG_ERR, "TestRecvPerformSip callback : fail : not found TransportEntity : %s", _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
		delete pstRcvSipData;
		return;
	}	
}

void cbSndPerformSip(TransportMgr & _rclsTransportMgr, const char * _pData, unsigned int _unDataLen, const Net5Tuple_t & _rstNet5Tuple, void * _pSndAppData)
{
	char szNet5Tuple[Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};

	if( !_pData || _unDataLen == 0 )
	{
		LogTest(E_LOG_ERR, "TestSndPerformSip callback : fail : invalid argument. Data[%p], DataLen[%u] : %s",
									_pData, _unDataLen, _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
		return;
	}

	LogTest(E_LOG_DEBUG, "\n%s\n[SND : %s:%d ==> %s:%d, %s]\n%s", g_pszLineDelimiter,
									_rstNet5Tuple.m_szLocalIp, _rstNet5Tuple.m_nLocalPort, _rstNet5Tuple.m_szRemoteIp, _rstNet5Tuple.m_nRemotePort, getTransportTypeStr(_rstNet5Tuple.m_eTransportType),
									_pData);
}

int getUasIndex4NewTransport()
{
	static unsigned int i = 0;

	return i++%s_unMaxPerformSipUasNum;
}

void recvProc(RcvSipData_t * _pstRcvData, void * _pObject)
{
	if( !_pstRcvData || _pstRcvData->m_unSipLen <= 10 || !(_pstRcvData->m_pszSip) )
	{
		LogTest(E_LOG_ERR, "PerformSipUas : recvProc : fail : invalid argument");
		delete _pstRcvData;
		return;
	}

	E_SIP_METHOD_t eSipMethod = SIP_METHOD_NUM;
	char szNet5Tuple[Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};
	char szCallId[s_unMaxCallIdLen] = {0,};

	for( int i = 0; i < SIP_METHOD_NUM; ++i )
	{
		if( strncmp(_pstRcvData->m_pszSip, s_arrMethodInfo[i].m_pszMethod, strlen(s_arrMethodInfo[i].m_pszMethod)) == 0 )
		{
			eSipMethod = s_arrMethodInfo[i].m_eSipMethod;
			break;
		}
	}

	LogTest(E_LOG_DEBUG, "\n%s\n[RCV : %s:%d ==> %s:%d, %s]\n%s", g_pszLineDelimiter,
									_pstRcvData->m_stNet5Tuple.m_szRemoteIp, _pstRcvData->m_stNet5Tuple.m_nRemotePort, _pstRcvData->m_stNet5Tuple.m_szLocalIp, _pstRcvData->m_stNet5Tuple.m_nLocalPort, getTransportTypeStr(_pstRcvData->m_stNet5Tuple.m_eTransportType),
									_pstRcvData->m_pszSip);

	//parsing call-id
	char * pStartCallIdPos = NULL, * pEndCallIdPos = NULL;
	if( (pStartCallIdPos = strstr(_pstRcvData->m_pszSip, s_pszCallIdHeaderName)) )
	{
		pStartCallIdPos += strlen(s_pszCallIdHeaderName);
		if( (pEndCallIdPos = strstr(pStartCallIdPos, s_pszEndLine)) && (pStartCallIdPos < pEndCallIdPos) )
		{
			unsigned int unCallIdLen = ((pEndCallIdPos-pStartCallIdPos)>s_unMaxCallIdLen-1) ? s_unMaxCallIdLen-1:(pEndCallIdPos-pStartCallIdPos);
			memcpy(szCallId, pStartCallIdPos, unCallIdLen);
			szCallId[unCallIdLen] = 0x00;
		}
		else
		{
			LogTest(E_LOG_ERR, "PerformSipUas[%d] : recvProc : fail : not found end of Call-Id : %s\n%s", _pstRcvData->m_nUasIndex,
										_pstRcvData->m_stNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)), _pstRcvData->m_pszSip);
			delete _pstRcvData;
			return;
		}
	}
	else
	{
		LogTest(E_LOG_ERR, "PerformSipUas[%d] : recvProc : fail : not found start of Call-Id : %s\n%s", _pstRcvData->m_nUasIndex,
									_pstRcvData->m_stNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)), _pstRcvData->m_pszSip);
		delete _pstRcvData;
		return;
	}

	if( eSipMethod == SIP_METHOD_invite )
	{
		char * pszSipMsg100 = (char *)malloc(s_unMaxSipLen);
		char * pszSipMsg180 = (char *)malloc(s_unMaxSipLen);
		char * pszSipMsg200 = (char *)malloc(s_unMaxSipLen);
		if( pszSipMsg100 && pszSipMsg180 && pszSipMsg200 )
		{
			snprintf(pszSipMsg100, s_unMaxSipLen-1, s_pszInv100, szCallId);
			sendData(_pstRcvData->m_stNet5Tuple, pszSipMsg100, strlen(pszSipMsg100));
			snprintf(pszSipMsg180, s_unMaxSipLen-1, s_pszInv180, szCallId);
			sendData(_pstRcvData->m_stNet5Tuple, pszSipMsg180, strlen(pszSipMsg180));
			snprintf(pszSipMsg200, s_unMaxSipLen-1, s_pszInv200, szCallId);
			sendData(_pstRcvData->m_stNet5Tuple, pszSipMsg200, strlen(pszSipMsg200));
		}
		else
		{
			LogTest(E_LOG_ERR, "PerformSipUas[%d] : recvProc : fail : can't create response of invite : %s\n%s", _pstRcvData->m_nUasIndex,
										_pstRcvData->m_stNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)), _pstRcvData->m_pszSip);
			free(pszSipMsg100);
			free(pszSipMsg180);
			free(pszSipMsg200);
		}
	}
	else if( eSipMethod == SIP_METHOD_bye )
	{
		char * pszSipMsg200 = (char *)malloc(s_unMaxSipLen);
		if( pszSipMsg200 )
		{
			snprintf(pszSipMsg200, s_unMaxSipLen-1, s_pszBye200, szCallId);
			sendData(_pstRcvData->m_stNet5Tuple, pszSipMsg200, strlen(pszSipMsg200));
		}
		else
		{
			LogTest(E_LOG_ERR, "PerformSipUas[%d] : recvProc : fail : can't create response of bye : %s\n%s", _pstRcvData->m_nUasIndex,
										_pstRcvData->m_stNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)), _pstRcvData->m_pszSip);
		}
	}
	else if( eSipMethod == SIP_METHOD_ack )
	{
		;
	}
	else
	{
		LogTest(E_LOG_ERR, "PerformSipUas[%d] : recvProc : fail : unknown SIP Method", _pstRcvData->m_nUasIndex);
	}

	delete _pstRcvData;
	
}

PerformSipUas::PerformSipUas()
	: m_clsRcvThread(1000, recvProc, -1, "PerformSipUas")
{
	m_clsRcvThread.run();
}

PerformSipUas::~PerformSipUas()
{
	m_clsRcvThread.shutdown();
	m_clsRcvThread.join();
}

void PerformSipUas::sendTo(RcvSipData_t * _pstData)
{
	m_clsRcvThread.put(_pstData);
}

