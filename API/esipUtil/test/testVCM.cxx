
#include "eSipUtil/bufferControl.hxx"

#include "testVCM.hxx"
#include "testTransport.hxx"
#include "log.hxx"

using namespace eSipUtil;

extern int g_nLogInstanceId;
extern char g_szProcName[100];
extern bool sendData(const Net5Tuple_t & _rstNet5Tuple, char * _pszSipMsg, unsigned int _unMsgLen);
extern TransportEntityInfo g_clsTransportEntityInfo;

#define LogTest(_level, _format, args...) Log(g_nLogInstanceId, 0, _level, _format, ##args)

static const unsigned char s_arrRandomPermutation[256] = 
{
   44, 9, 46, 184, 21, 30, 92, 231, 79, 7, 166, 237, 173, 72, 91, 123, 
   212, 183, 16, 99, 85, 45, 190, 130, 118, 107, 169, 119, 100, 179, 251, 177,
   23, 125, 12, 101, 121, 246, 61, 38, 156, 114, 159, 57, 181, 145, 198, 182,
   58, 215, 174, 225, 82, 178, 150, 161, 63, 103, 32, 203, 68, 151, 139, 55, 
   143, 2, 36, 110, 209, 154, 204, 89, 62, 17, 187, 226, 31, 105, 195, 208,
   49, 56, 238, 172, 37, 3, 234, 206, 134, 233, 19, 148, 64, 4, 10, 224,
   144, 88, 93, 191, 20, 131, 138, 199, 243, 244, 39, 50, 214, 87, 6, 84,
   185, 112, 171, 75, 192, 193, 239, 69, 106, 43, 194, 1, 78, 67, 116, 200,
   83, 70, 213, 25, 59, 137, 52, 13, 153, 42, 232, 0, 133, 210, 76, 33,
   255, 236, 124, 104, 65, 201, 53, 155, 140, 254, 54, 196, 120, 146, 216, 29,
   28, 86, 245, 90, 98, 26, 81, 115, 180, 66, 102, 136, 167, 51, 109, 132,
   77, 175, 14, 202, 222, 48, 223, 188, 40, 242, 157, 5, 128, 229, 71, 127,
   164, 207, 247, 8, 80, 149, 94, 160, 47, 117, 135, 176, 129, 142, 189, 97,
   11, 250, 221, 218, 96, 220, 35, 197, 152, 126, 219, 74, 170, 252, 163, 41,
   95, 27, 34, 22, 205, 230, 241, 186, 168, 228, 253, 249, 113, 108, 111, 211,
   235, 217, 165, 122, 15, 141, 158, 147, 240, 24, 162, 18, 60, 73, 227, 248
};

static unsigned int g_fnRawHash(const unsigned char * _pBuffer, unsigned int _unLen)
{
	union 
	{
		unsigned int unHash;
		unsigned char arrHash[4];
	};
	
	unHash = 0;
	arrHash[0] = s_arrRandomPermutation[10];
	arrHash[1] = s_arrRandomPermutation[11];
	arrHash[2] = s_arrRandomPermutation[12];
	arrHash[3] = s_arrRandomPermutation[13];

	const unsigned char * pEnd = _pBuffer + _unLen;
	for( ; _pBuffer != pEnd; ++_pBuffer )
	{
		arrHash[0] = s_arrRandomPermutation[(*_pBuffer) ^ arrHash[0]];
		arrHash[1] = s_arrRandomPermutation[(*_pBuffer) ^ arrHash[1]];
		arrHash[2] = s_arrRandomPermutation[(*_pBuffer) ^ arrHash[2]];
		arrHash[3] = s_arrRandomPermutation[(*_pBuffer) ^ arrHash[3]];
	}

	return unHash;
}

eSipUtil::QueueThread< RcvInfo > SimulVCM::m_arrRcvThread[SimulVCM::E_CONST_WORKER_NUM];
eSipUtil::TimerTable< TimerInfo > SimulVCM::m_clsTimerTable(m_fnCbProcTimer, "SimVcmTimer");
char SimulVCM::m_szTagPrefix[128];
char SimulVCM::m_szStackTagPrefix[128];

void SimulVCM::m_fnInit()
{
	for( int i = 0; i < E_CONST_WORKER_NUM; ++i )
	{
		m_arrRcvThread[i].setMaxQueueSize(1000);
		m_arrRcvThread[i].setProcessCb(m_fnRecvProc);
		m_arrRcvThread[i].setQueueWaitMs(-1);
		m_arrRcvThread[i].run();
	}

	snprintf(m_szTagPrefix, sizeof(m_szTagPrefix)-1, "%s_simul_", g_szProcName);
	snprintf(m_szStackTagPrefix, sizeof(m_szStackTagPrefix)-1, "%s_simulStack_", g_szProcName);
}

void SimulVCM::m_fnSendTo(SndInfo * _pstSndData)
{

}

int SimulVCM::cbCheckCompleteSlbMsg4Rcv(TransportMgr & _rclsTransportMgr, const char * _pRcvData, unsigned int _unRcvDataLen, const Net5Tuple_t & _rstNet5Tuple, void * _pAppBuffer)
{
	if( !_pRcvData || _unRcvDataLen == 0 )
		return -1;

	if( _unRcvDataLen < SlbInternalMsgHdr::m_fnGetHeaderLen() )
		return 0;

	SlbInternalMsgHdr * pHeader = (SlbInternalMsgHdr *)_pRcvData;
	
	return pHeader->m_fnGetTotalLen();
}

void SimulVCM::cbRecv(TransportMgr & _rclsTransportMgr, char * _pData, unsigned int _unDataLen, const Net5Tuple_t & _rstRcvTuple)
{
	char szNet5Tuple[Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};
	RcvInfo * pstRcvInfo = NULL;
	SlbInternalMsgHdr * pHeader = NULL;
	EInternalMsgMatchResult_t eInternalMsgResult = E_INTERNAL_MSG_MATCH_RESULT_NUM;
	unsigned int unWorkerIdx = 0;
	char szInternalMsg[SlbInternalMsgHdr::E_CONST_MAX_STR_LEN] = {0,};

	if( !_pData || _unDataLen == 0 )
	{
		LogTest(E_LOG_ERR, "SimulVCM : cbRecv : fail : invalid argument : RcvData[%p], RcvLen[%u] : %s", _pData, _unDataLen, _rstRcvTuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
		goto ERR_PROC;
	}

	/*==== validate InternalMsg ====*/
	pHeader = (SlbInternalMsgHdr *)_pData;
	eInternalMsgResult = pHeader->m_fnIsMatch(_pData, _unDataLen);
	if( eInternalMsgResult != E_INTERNAL_MSG_MATCH_RESULT_SUCC )
	{
		LogTest(E_LOG_ERR, "SimulVCM : cbRecv : fail : IsMatch : %d : RcvLen[%u] : %s", eInternalMsgResult, _unDataLen, _rstRcvTuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
		goto ERR_PROC;
	}
	
	LogTest(E_LOG_INFO, "SimulVCM : cbRecv : RcvLen[%u] : %s : %s", 
										_unDataLen, _rstRcvTuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)), pHeader->m_fnGetStr(szInternalMsg, sizeof(szInternalMsg), true));

	/*==== create RcvInfo ====*/
	pstRcvInfo = new RcvInfo;
	if( !pstRcvInfo )
	{
		LogTest(E_LOG_ERR, "SimulVCM : cbRecv : fail : can't create RcvInfo : RcvLen[%u] : %s", _unDataLen, _rstRcvTuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
		goto ERR_PROC;
	}
	pstRcvInfo->m_stRcvNet5Tuple = _rstRcvTuple;
	pstRcvInfo->m_pRcvInternalMsgHdr = pHeader;
	_pData = NULL;

	/*==== determine index of WorkerTh ====*/
	if( pHeader->m_eMsgType == E_INTERNAL_MSG_TYPE_RCV_SIP )
	{
		SlbRcvSipBody * pstBody = (SlbRcvSipBody *)(pHeader->m_fnGetBody());
		if( !pstBody->m_stInternalSipInfo.m_fnGetHeaderStr(pstBody->m_fnGetSipMsg(), E_INTERNAL_SIP_STR_TYPE_CALLID, pstRcvInfo->m_fnGetCallId(), RcvInfo::E_CONST_MAX_CALLID_LEN) )
		{
			LogTest(E_LOG_ERR, "SimulVCM : cbRecv : fail : can't find CallID : RcvLen[%u] : %s", _unDataLen, _rstRcvTuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
			goto ERR_PROC;
		}
		unWorkerIdx = g_fnRawHash((const unsigned char *)(pstRcvInfo->m_fnGetCallId()), strlen(pstRcvInfo->m_fnGetCallId())) % E_CONST_WORKER_NUM;
	}
	else
		unWorkerIdx = 0;

	if( m_arrRcvThread[unWorkerIdx].put(pstRcvInfo) )
	{
		LogTest(E_LOG_DEBUG, "SimulVCM : cbRecv : send to queue fo recv thread[%u] : RcvLen[%u] : %s", 
												unWorkerIdx, _unDataLen, _rstRcvTuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
	}
	else
	{
		LogTest(E_LOG_ERR, "SimulVCM : cbRecv : fail : queue of recv thread is full[%u] : RcvLen[%u] : %s", 
											m_arrRcvThread[unWorkerIdx].getCurrentQueueSize(), _unDataLen, _rstRcvTuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
		goto ERR_PROC;
	}
	
	return;

ERR_PROC :
	free(_pData);
	delete pstRcvInfo;
	
}

void SimulVCM::cbSnd(eSipUtil::TransportMgr & _rclsTransportMgr, const char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstSndTuple, void * _pAppBuffer)
{
	if( _pData && strncmp(_pData, MAGIC_COOKIE, MSG_MAGIC_COOKIE_LEN) == 0 )
	{
		char szNet5Tuple[eSipUtil::Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};
		char szInternalMsg[SlbInternalMsgHdr::E_CONST_MAX_STR_LEN] = {0,};
		const SlbInternalMsgHdr * pstMsgHeader = (const SlbInternalMsgHdr *)_pData;

		LogTest(E_LOG_INFO, "SimulVCM : cbSnd : SndLen[%u] : %s : %s", 
										_unDataLen, _rstSndTuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)), pstMsgHeader->m_fnGetStr(szInternalMsg, sizeof(szInternalMsg), true));
	}
}

void SimulVCM::cbConnected4NonBlock(eSipUtil::TransportMgr & _rclsTransportMgr, const eSipUtil::Net5Tuple_t & _rstNet5Tuple)
{
	SlbInternalMsgHdr * pstSndHeader = NULL;
	SlbInitReqBody * pstInitReqBody = NULL;
	unsigned int unMallocHeaderLen = sizeof(SlbInternalMsgHdr) + sizeof(SlbInitReqBody);
	TransportEntity stTransportEntity;
	char szNet5Tuple[eSipUtil::Net5Tuple::E_CONST_MAX_STR_LEN] = {0,};

	if( g_clsTransportEntityInfo.getTransportEntity(_rstNet5Tuple, stTransportEntity) )
	{
		LogTest(E_LOG_DEBUG, "SimulVCM : Connected4NonBlock : %s", _rstNet5Tuple.getStr(szNet5Tuple, eSipUtil::Net5Tuple::E_CONST_MAX_STR_LEN));

		pstSndHeader = (SlbInternalMsgHdr *)malloc(unMallocHeaderLen);
		if( !pstSndHeader )
		{
			LogTest(E_LOG_DEBUG, "SimulVCM : Connected4NonBlock : fail : can't create InternalMsgHdr : %s", _rstNet5Tuple.getStr(szNet5Tuple, eSipUtil::Net5Tuple::E_CONST_MAX_STR_LEN));
			return;
		}

		pstInitReqBody = (SlbInitReqBody *)(pstSndHeader->m_fnGetBody());
		pstInitReqBody->m_fnReset();
		pstInitReqBody->m_fnSetCcmId(g_szProcName);
		pstInitReqBody->m_fnSetTagPrefix(m_szTagPrefix);
		pstInitReqBody->m_fnSetStackTagPrefix(m_szStackTagPrefix);
	
		pstSndHeader->m_fnReset();
		pstSndHeader->m_eMsgType = E_INTERNAL_MSG_TYPE_INIT_REQ;
		pstSndHeader->m_unBodyLen = pstInitReqBody->m_fnGetBodyLen();

		LogTest(E_LOG_DEBUG, "SimulVCM : Connected4NonBlock : %s:%d <== %s:%d : SndLen[%u] : Init request : Id[%s]", 
												_rstNet5Tuple.m_szRemoteIp, _rstNet5Tuple.m_nRemotePort, _rstNet5Tuple.m_szLocalIp, _rstNet5Tuple.m_nLocalPort,
												unMallocHeaderLen, g_szProcName);

		sendData(_rstNet5Tuple, (char *)pstSndHeader, unMallocHeaderLen);		
	}
	else
	{
		LogTest(E_LOG_ERR, "SimulVCM : Connected4NonBlock : fail : can't find TransportEntity : %s", _rstNet5Tuple.getStr(szNet5Tuple, eSipUtil::Net5Tuple::E_CONST_MAX_STR_LEN));
	}
}

void SimulVCM::m_fnCbProcTimer(TimerInfo * _pstTimerInfo)
{
	if( _pstTimerInfo )
	{
		switch(_pstTimerInfo->m_eTimerType)
		{
			case E_TIMER_TYPE_ping :
			{
				SlbInternalMsgHdr * pstSndHeader = NULL;
				SlbPingReqBody * pstPingReqBody = NULL;
				TransportEntity stTransportEntity;
				unsigned int unMallocHeaderLen = sizeof(SlbInternalMsgHdr) + sizeof(SlbPingReqBody);

				if( g_clsTransportEntityInfo.getTransportEntity(_pstTimerInfo->m_stNet5Tuple, stTransportEntity) )
				{
					pstSndHeader = (SlbInternalMsgHdr *)malloc(unMallocHeaderLen);
					pstPingReqBody = (SlbPingReqBody *)(pstSndHeader->m_fnGetBody());
					pstPingReqBody->m_fnReset();
					pstPingReqBody->m_fnSetCcmId(g_szProcName);
					pstPingReqBody->m_bActive = true;
					pstPingReqBody->m_unActiveSessionNum = 0;
					pstSndHeader->m_fnReset();
					pstSndHeader->m_eMsgType = E_INTERNAL_MSG_TYPE_PING_REQ;
					pstSndHeader->m_unBodyLen = pstPingReqBody->m_fnGetBodyLen();

					LogTest(E_LOG_DEBUG, "SimulVCM : CbProcTimer : send PingReq : %s:%d <== %s:%d", 
															_pstTimerInfo->m_stNet5Tuple.m_szRemoteIp, _pstTimerInfo->m_stNet5Tuple.m_nRemotePort, 
															_pstTimerInfo->m_stNet5Tuple.m_szLocalIp, _pstTimerInfo->m_stNet5Tuple.m_nLocalPort);
					
					sendData(_pstTimerInfo->m_stNet5Tuple, (char *)pstSndHeader, unMallocHeaderLen);
				}
				else
				{
					LogTest(E_LOG_DEBUG, "SimulVCM : CbProcTimer : fail : send PingReq : %s:%d <== %s:%d : can't get TransportEntity", 
															_pstTimerInfo->m_stNet5Tuple.m_szRemoteIp, _pstTimerInfo->m_stNet5Tuple.m_nRemotePort, 
															_pstTimerInfo->m_stNet5Tuple.m_szLocalIp, _pstTimerInfo->m_stNet5Tuple.m_nLocalPort);
				}
				
				m_clsTimerTable.setTimer(_pstTimerInfo, _pstTimerInfo->m_unTimerMillicSec);
				return;
			}
			break;
			default : break;
		}

		delete _pstTimerInfo;
	}
}

bool SimulVCM::m_fnChangeTag(bool _bAdd, const char * _pHdrStartPos, const char * _pHdrEndPos, const char ** _pTagStartPos, const char ** _pTagEndPos, std::string & _rclsNewTag)
{	
	eSipUtil::BufferControl clsTagBufferControl(_pHdrStartPos, _pHdrEndPos-_pHdrStartPos);
	_rclsNewTag = "";

	const char * pTagStartPos = NULL, * pTagEndPos = NULL;
	pTagStartPos = clsTagBufferControl.findStr("tag");
	if( pTagStartPos )
	{
		clsTagBufferControl.setStartPos(pTagStartPos+strlen("tag"));			
		clsTagBufferControl.skipWhitespace();
		if( !clsTagBufferControl.isChar('=') )
			return false;
		clsTagBufferControl.move(1);
		clsTagBufferControl.skipWhitespace();
		pTagStartPos = clsTagBufferControl.getStartPos();
		pTagEndPos = clsTagBufferControl.findChar(';');
		if( !pTagEndPos )
			pTagEndPos = clsTagBufferControl.getEndPos();

		*_pTagStartPos = pTagStartPos;
		*_pTagEndPos = pTagEndPos;

		bool bExistPrefix = false;
		if( strncmp(pTagStartPos, m_szTagPrefix, strlen(m_szTagPrefix)) == 0 )
			bExistPrefix = true;

		if( _bAdd && bExistPrefix )
			return false;
		else if( !_bAdd && !bExistPrefix )
			return false;

		if( _bAdd )
		{
			_rclsNewTag.append(m_szTagPrefix);
			_rclsNewTag.append(pTagStartPos, pTagEndPos-pTagStartPos);
		}
		else
		{
			pTagStartPos += strlen(m_szTagPrefix);
			_rclsNewTag.append(pTagStartPos, pTagEndPos-pTagStartPos);
		}

		return true;
	}
	else
		return false;
}

bool SimulVCM::m_fnChangeSip4Snd(const char * _pszSrcSip, char * _pszDstSip)
{
	if( !_pszSrcSip || strlen(_pszSrcSip) == 0 || !_pszDstSip )
		return false;

	_pszDstSip[0] = 0x00;

	const char * pFromHdrStartPos = NULL, * pFromHdrEndPos = NULL;
	const char * pToHdrStartPos = NULL, * pToHdrEndPos = NULL;
	
	pFromHdrStartPos = strcasestr(_pszSrcSip, "\r\nFrom");
	if( pFromHdrStartPos )
	{
		pFromHdrStartPos += strlen("\r\nFrom");
		pFromHdrEndPos = strstr(pFromHdrStartPos, "\r\n");
	}

	pToHdrStartPos = strcasestr(_pszSrcSip, "\r\nTo");
	if( pToHdrStartPos )
	{
		pToHdrStartPos += strlen("\r\nTo");
		pToHdrEndPos = strstr(pToHdrStartPos, "\r\n");
	}

	if( !pFromHdrStartPos || !pFromHdrEndPos || !pToHdrStartPos || !pToHdrEndPos )
		return false;

	const char * pAddHdrStartPos = NULL, * pAddHdrEndPos = NULL;
	const char * pDeleteHdrStartPos = NULL, * pDeleteHdrEndPos = NULL;
	if( strncmp(_pszSrcSip, "SIP/2.0", strlen("SIP/2.0")) == 0 )
	{
		pAddHdrStartPos = pToHdrStartPos;
		pAddHdrEndPos = pToHdrEndPos;
		pDeleteHdrStartPos = pFromHdrStartPos;
		pDeleteHdrEndPos = pFromHdrEndPos;
	}
	else
	{
		pAddHdrStartPos = pFromHdrStartPos;
		pAddHdrEndPos = pFromHdrEndPos;
		pDeleteHdrStartPos = pToHdrStartPos;
		pDeleteHdrEndPos = pToHdrEndPos;
	}

	bool bFirstAdd = false;
	const char * pFormerHdrStartPos = NULL, * pFormerHdrEndPos = NULL;
	const char * pLatterHdrStartPos = NULL, * pLatterHdrEndPos = NULL;
	if( pAddHdrStartPos < pDeleteHdrStartPos )
	{
		pFormerHdrStartPos = pAddHdrStartPos;
		pFormerHdrEndPos = pAddHdrEndPos;
		pLatterHdrStartPos = pDeleteHdrStartPos;
		pLatterHdrEndPos = pDeleteHdrEndPos;
		bFirstAdd = true;
	}
	else
	{
		pFormerHdrStartPos = pDeleteHdrStartPos;
		pFormerHdrEndPos = pDeleteHdrEndPos;
		pLatterHdrStartPos = pAddHdrStartPos;
		pLatterHdrEndPos = pAddHdrEndPos;
		bFirstAdd = false;
	}

	const char * pFormerTagStartPos = NULL, * pFormerTagEndPos = NULL;
	const char * pLatterTagStartPos = NULL, * pLatterTagEndPos = NULL;
	std::string clsChangeTag;
	bool bChange = false;
	int nCpLen = 0;
	const char * pNextPos = NULL;
	if( m_fnChangeTag(bFirstAdd, pFormerHdrStartPos, pFormerHdrEndPos, &pFormerTagStartPos, &pFormerTagEndPos, clsChangeTag) )
	{
		bChange = true;
		memcpy(_pszDstSip + nCpLen, _pszSrcSip, pFormerTagStartPos - _pszSrcSip);
		nCpLen += pFormerTagStartPos - _pszSrcSip;
		memcpy(_pszDstSip + nCpLen, clsChangeTag.c_str(), clsChangeTag.size());
		nCpLen += clsChangeTag.size();
		pNextPos = pFormerTagEndPos;
	}
	else
	{
		pNextPos = _pszSrcSip;
	}
	
	if( m_fnChangeTag(!bFirstAdd, pLatterHdrStartPos, pLatterHdrEndPos, &pLatterTagStartPos, &pLatterTagEndPos, clsChangeTag) )
	{
		bChange = true;
		memcpy(_pszDstSip + nCpLen, pNextPos, pLatterTagStartPos - pNextPos);
		nCpLen += pLatterTagStartPos - pNextPos;
		memcpy(_pszDstSip + nCpLen, clsChangeTag.c_str(), clsChangeTag.size());
		nCpLen += clsChangeTag.size();
		strcpy(_pszDstSip + nCpLen, pLatterTagEndPos);
	}
	else if( bChange )
	{
		strcpy(_pszDstSip + nCpLen, pNextPos);
	}
	
	if( !bChange )
		strcpy(_pszDstSip, _pszSrcSip);

	return true;
}

void SimulVCM::m_fnRecvProc(RcvInfo * _pstRcvData, void * _pObject)
{
	if( !_pstRcvData || !(_pstRcvData->m_pRcvInternalMsgHdr) )
	{
		LogTest(E_LOG_DEBUG, "SimulVCM : RecvProc : fail : invalid argument");
		return;
	}
	
	switch(_pstRcvData->m_pRcvInternalMsgHdr->m_eMsgType)
	{
		case E_INTERNAL_MSG_TYPE_INIT_RSP :
		{
			SlbInitRspBody * pstRcvBody = (SlbInitRspBody *)(_pstRcvData->m_pRcvInternalMsgHdr->m_fnGetBody());
			LogTest(E_LOG_INFO, "SimulVCM : RecvProc : Init Rsp [%d]", pstRcvBody->m_eInitRspType);
			if( pstRcvBody->m_eInitRspType == SlbInitRspBody::E_INIT_RSP_TYPE_SUCC )
			{
				TimerInfo * pstTimerInfo = new TimerInfo();
				pstTimerInfo->m_eTimerType = E_TIMER_TYPE_ping;
				pstTimerInfo->m_unTimerMillicSec = 1000;
				pstTimerInfo->m_stNet5Tuple = _pstRcvData->m_stRcvNet5Tuple;
				m_clsTimerTable.setTimer(pstTimerInfo, pstTimerInfo->m_unTimerMillicSec);
			}
		}
		break;
		case E_INTERNAL_MSG_TYPE_PING_RSP :
		{			
			LogTest(E_LOG_DEBUG, "SimulVCM : RecvProc : Ping Rsp");
		}
		break;
		case E_INTERNAL_MSG_TYPE_RCV_SIP :
		{
			m_fnRecvProc4Sip(*(_pstRcvData->m_fnGetHdrOwn()), _pstRcvData->m_stRcvNet5Tuple, _pstRcvData->m_fnGetCallId());
		}
		break;
		case E_INTERNAL_MSG_TYPE_NOTI_RTE_STATUS :
		{
			;
		}
		break;
		default :
		{
			LogTest(E_LOG_ERR, "SimulVCM : RecvProc : fail : invalid type[%d] : %s:%d ==> %s:%d", 
												_pstRcvData->m_pRcvInternalMsgHdr->m_eMsgType, _pstRcvData->m_stRcvNet5Tuple.m_szRemoteIp, _pstRcvData->m_stRcvNet5Tuple.m_nRemotePort, _pstRcvData->m_stRcvNet5Tuple.m_szLocalIp, _pstRcvData->m_stRcvNet5Tuple.m_nLocalPort);
		}			
		break;
	}

	delete _pstRcvData;
}

void SimulVCM::m_fnRecvProc4Sip(SlbInternalMsgHdr & _rstRcvMsgHdr, const eSipUtil::Net5Tuple & _rstInternalRcvTuple, const char * _pszCallId)
{
	SlbRcvSipBody * pstRcvBody = (SlbRcvSipBody *)(_rstRcvMsgHdr.m_fnGetBody());
	SlbInternalMsgHdr * pstSndHeader = NULL;
	SlbSndSipBody * pstSndBody = NULL;
	char * pszSndSip = NULL;
	TransportEntity stTransportEntity;
	char szInternalSipInfo[SlbInternalSipInfo::E_CONST_MAX_PRINT_LEN] = {0,};

	if( g_clsTransportEntityInfo.getTransportEntity(_rstInternalRcvTuple, stTransportEntity) )
	{
		pstSndHeader = (SlbInternalMsgHdr *)malloc(sizeof(SlbInternalMsgHdr) + sizeof(SlbSndSipBody) + strlen(pstRcvBody->m_fnGetSipMsg())+strlen(m_szTagPrefix) + 1);
		if( !pstSndHeader )
		{
			LogTest(E_LOG_ERR, "SimulVCM : RecvProc4Sip : fail : can't create SndHeader : RcvSipLen[%u] : %s", 
												pstRcvBody->m_unSipLen, pstRcvBody->m_stInternalSipInfo.m_fnGetStr(szInternalSipInfo, sizeof(szInternalSipInfo), true));
			goto ERR_PROC;
		}

		pstSndBody = (SlbSndSipBody *)(pstSndHeader->m_fnGetBody());
		pstSndBody->m_fnReset();		
		if( !m_fnChangeSip4Snd(pstRcvBody->m_fnGetSipMsg(), pstSndBody->m_fnGetSipMsg()) )
		{
			LogTest(E_LOG_ERR, "SimulVCM : RecvProc : fail : can't create SndSip : RcvSipLen[%u] : %s", 
												pstRcvBody->m_unSipLen, pstRcvBody->m_stInternalSipInfo.m_fnGetStr(szInternalSipInfo, sizeof(szInternalSipInfo), true));
			goto ERR_PROC;
		}
		
		m_fnDetermineTargetTuple4SndSip(stTransportEntity, pstRcvBody->m_stInternalSipInfo, pstRcvBody->m_fnGetSipMsg(), pstSndBody->m_stSndTuple);
		pstSndBody->m_fnSetCallId(_pszCallId);		
		pstSndBody->m_unSipLen = strlen(pstSndBody->m_fnGetSipMsg());
		pstSndHeader->m_fnReset();
		pstSndHeader->m_eMsgType = E_INTERNAL_MSG_TYPE_SND_SIP;
		pstSndHeader->m_unBodyLen = pstSndBody->m_fnGetBodyLen();

		LogTest(E_LOG_INFO, "SimulVCM : RecvProc4Sip : %s:%d <== %s:%d : SndSipLen[%u] : %s", 
												_rstInternalRcvTuple.m_szRemoteIp, _rstInternalRcvTuple.m_nRemotePort, _rstInternalRcvTuple.m_szLocalIp, _rstInternalRcvTuple.m_nLocalPort,
												pstSndBody->m_unSipLen, pszSndSip);
		
		sendData(_rstInternalRcvTuple, (char *)pstSndHeader, pstSndHeader->m_fnGetTotalLen());
	}
	else
	{
		LogTest(E_LOG_ERR, "SimulVCM : RecvProc4Sip : fail : SipLen[%u] : can't find TransportEntity : %s", 
											pstRcvBody->m_unSipLen, pstRcvBody->m_stInternalSipInfo.m_fnGetStr(szInternalSipInfo, sizeof(szInternalSipInfo), true));
		goto ERR_PROC;
	}

	delete &_rstRcvMsgHdr;
	return;

ERR_PROC :
	free(pstSndHeader);
	free(pszSndSip);
	delete &_rstRcvMsgHdr;
}

void SimulVCM::m_fnDetermineTargetTuple4SndSip(const TransportEntity & _rstTrspEntity, const SlbInternalSipInfo & _rstInternalSipInfo, const char * _pszRcvSip, eSipUtil::Net5Tuple & _rstTagetTuple)
{
	if( _rstInternalSipInfo.m_nRspCode == 0 )
		_rstTagetTuple = _rstTrspEntity.m_stTermSipNet5Tuple;
	else
	{
		_rstTagetTuple.m_fnSetLocalIp(_rstTrspEntity.m_szOrigSipLocalIp);
		_rstTagetTuple.m_fnSetLocalPort(_rstTrspEntity.m_nOrigSipLocalPort);
		_rstInternalSipInfo.m_fnGetHeaderStr(_pszRcvSip, E_INTERNAL_SIP_STR_TYPE_VIAIP, _rstTagetTuple.m_szRemoteIp, sizeof(_rstTagetTuple.m_szRemoteIp));
		_rstTagetTuple.m_fnSetRemotePort(_rstInternalSipInfo.m_nViaPort);
		_rstTagetTuple.m_fnSetTransportType(_rstTrspEntity.m_eOrigSipTransportType);
	}
}

