#include "TrseTransport.h"
#include "Worker.h"
#include "TrseNodeMgr.h"
#include "MainConfig.h"
#include "CZip.h"
#include "AppXmlParser.h"
#include "main.h"

//#define SPLIT_SEND_LEN 16000 //16kb
//#define SPLIT_SEND_LEN 32000 //32kb
#define SPLIT_SEND_LEN 10000000 //10mbyte

using namespace eSipUtil;

TrseTransport * g_fnCreateTrseTransport()
{
	TrseTransport * pclsObj = TrseTransport::m_fnGetInstance();
	pclsObj->m_fnAddConnection();
	return pclsObj;
}
TrseTransport * TrseTransport::m_pclsMy = NULL;
TrseTransport::TrseTransport():TrspSocketCom("TrseTransport"){}
TrseTransport::~TrseTransport(){}
TrseTransport * TrseTransport::m_fnGetInstance()
{
	if(m_pclsMy==NULL)
	{
		m_pclsMy = new TrseTransport;
	}
	return m_pclsMy;
}
void TrseTransport::m_fnAddConnection()
{
	IFLOG(E_LOG_INFO, "TRSE Add Connection");
	MainConfig* pclsMainConfig = MainConfig::m_fnGetInstance();

	Net5Tuple_t stAddr;
	stAddr.m_fnSet("tcp", (KCSTR) pclsMainConfig->m_clsTrseLocalIP, pclsMainConfig->m_unTrseLocalPort, "0.0.0.0", 0);
	m_fnAdd(stAddr, E_TCP_ROLE_SERVER);
}
void TrseTransport::m_fnDelConnection(const Net5Tuple_t & _rstNet5Tuple)
{
	m_fnDel(_rstNet5Tuple);
}
//==========> Tcp Event Notify CallBack
void TrseTransport::m_fnCbkNotify(ETcpNotiEv_t _eT, const Net5Tuple_t & _rstNet5Tuple,
										ETransportErr_t _eDisconErr)
{
	//L4 health check 무시.
	if(KString::m_fnStrCmp(_rstNet5Tuple.m_szRemoteIp, (KCSTR)MainConfig::m_fnGetInstance()->m_clsTrseLoadBalanceIP) == 0) return;

	if (_eT == E_TCP_NOTI_ACCEPT_CON)
	{
		TrseConEv * pclsNew = new TrseConEv;
		pclsNew->m_eT = E_TRSE_IF_CONNECTED;
		pclsNew->m_stAddr = _rstNet5Tuple;
		KString clsAddrString;
		IFLOG(E_LOG_INFO, "TRSE Connected [%s]",
				_rstNet5Tuple.getStr((KSTR )clsAddrString, 128, false));
		TrseNodeMgr::m_fnPutEv(pclsNew);
	}
	else if (_eT == E_TCP_NOTI_DISCON)
	{
		TrseConEv * pclsNew = new TrseConEv;
		pclsNew->m_eT = E_TRSE_IF_DISCONNECTED;
		pclsNew->m_stAddr = _rstNet5Tuple;
		KString clsAddrString;
		IFLOG(E_LOG_ERR, "TRSE Disconnected [%s]",
				_rstNet5Tuple.getStr((KSTR )clsAddrString, 128, false));
		TrseNodeMgr::m_fnPutEv(pclsNew);
	}
}
//==========> Packet Size Decision CallBack
int TrseTransport::m_fnCbkAnalizePacket(const char * _pRcvData,
										unsigned int _unRcvDataLen, 
										const Net5Tuple_t & _rstNet5Tuple)
{
	if(_unRcvDataLen < TRSE_HEADER_SIZE) return 0;

	char szHeader[TRSE_HEADER_SIZE+1]; memset(szHeader,0x00,sizeof(szHeader));
	KString::m_fnStrnCatCpy(szHeader, _pRcvData,sizeof(szHeader));

	return KString::m_fnAtoi(szHeader+TRSE_HEADER_KSIP)+TRSE_HEADER_SIZE;
}
//==========> Send/Recv Trace CallBack
void TrseTransport::m_fnCbkTracePacket(bool _bSend, const char * _pData,
										unsigned int _unDataLen, 
										const Net5Tuple_t & _rstNet5Tuple,
										void * _pSndAppData)
{
	if(_bSend==false)
	{
		// Rcv Pk Log Add
		if (KString::m_fnStrnCmp(_pData, "WTRSTP/1.0", 10) != 0)
		{
			KString clsAddrString;
			IFLOG(E_LOG_ERR, "[Trse RECV ERR][%s][len:%d]",
					_rstNet5Tuple.getStr((KSTR )clsAddrString, 128, false),
					_unDataLen);
			if (_pData)
				delete _pData;
			return;
		}

		KString clsAddrString;
		char szHeader[TRSE_HEADER_SIZE+1]; memset(szHeader,0x00,sizeof(szHeader));
		KString::m_fnStrnCatCpy(szHeader, _pData, sizeof(szHeader));
		IFLOG(E_LOG_INFO, "[Trse RECV][%s][len:%d][%s]",
				_rstNet5Tuple.getStr((KSTR )clsAddrString, 128, false),
				_unDataLen, szHeader);

		KString clsUncompressData;
		ezbuffer bufSrc( KString::m_fnAtoi(szHeader+TRSE_HEADER_KSIP) );
		memcpy(bufSrc.pBuf,(unsigned char*)_pData+TRSE_HEADER_SIZE, KString::m_fnAtoi(szHeader+TRSE_HEADER_KSIP));
		ezbuffer bufDest( KString::m_fnAtoi(szHeader+TRSE_HEADER_KSIP) * 2 );

		if(CZip::m_fnEZUncompress(bufDest, bufSrc) == false)
		{
			//1005 또는 1017 리턴줘야 하는데 규격이 이상해서 보류함.
			//압축해제부터 실패면 어떤 메시지가 오류난것인지 알수 없기 때문에 res 를 만들 수 없음.
			if(_pData) delete _pData;
			IFLOG(E_LOG_DEBUG,"[TrseTransport] Uncompress Fail");
			return;
		}
		else
		{
			IFLOG(E_LOG_DEBUG, "[TrseTransport] Uncompress Len:%ld",
					bufDest.nLen);
			clsUncompressData.m_fnByteCat((KCSTR)bufDest.pBuf, bufDest.nLen);
			if (_pData) delete _pData;
		}

		//1015 또는 2007 리턴줘야 하는데 규격이 이상해서 보류함.
		//파싱부터 실패면 어떤 메시지가 오류난것인지 알수 없기 때문에 res 를 만들 수 없음.
		if(AppXmlParser::m_fnValid(clsUncompressData) == false) return;

		unsigned int unTid = AppXmlParser::m_fnGetTransactionID(clsUncompressData);
		KString clsBodyType;
		AppXmlParser::m_fnGetBodyType(clsUncompressData, clsBodyType);
		if(clsBodyType == DEF_BODY_ESTABLISH_SESSION_REQ)
		{
			TrseConEv * pclsNew = new TrseConEv;
			pclsNew->m_eT = E_TRSE_IF_ESTABLISH_SESSION_REQ;
			pclsNew->m_stAddr = _rstNet5Tuple;
			pclsNew->m_clsXml = clsUncompressData;
			TrseNodeMgr::m_fnPutEv(pclsNew);
		}
		else if(clsBodyType == DEF_BODY_LINK_TEST_REQ)
		{
			TrseConEv * pclsNew = new TrseConEv;
			pclsNew->m_eT = E_TRSE_IF_LINK_TEST_REQ;
			pclsNew->m_stAddr = _rstNet5Tuple;
			pclsNew->m_clsXml = clsUncompressData;
			TrseNodeMgr::m_fnPutEv(pclsNew);
		}
		else if(clsBodyType == DEF_BODY_CLOSE_SESSION_REQ)
		{
			TrseConEv * pclsNew = new TrseConEv;
			pclsNew->m_eT = E_TRSE_IF_CLOSE_SESSION_REQ;
			pclsNew->m_stAddr = _rstNet5Tuple;
			pclsNew->m_clsXml = clsUncompressData;
			TrseNodeMgr::m_fnPutEv(pclsNew);
		}
		else if(clsBodyType == DEF_BODY_CREATE_JOB_REQ)
		{
			AppTrseEvent * pclsEv = new AppTrseEvent;
			pclsEv->m_eT = E_MAIN_EV_TRSE_CREATE_JOB_REQ;
			pclsEv->m_unTid = unTid;
			AppXmlParser::m_fnGetSessionID(clsUncompressData, pclsEv->m_clsSessionID);
			pclsEv->m_stAddr = _rstNet5Tuple;
			pclsEv->m_clsXml = clsUncompressData;
			Worker::m_fnPutTrseEv(pclsEv);
		}
		else
		{
			IFLOG(E_LOG_ERR,"[TrseTransport] [clsBodyType]Error [%s]", (KCSTR)clsBodyType);
		}
	}
	else
	{
		// Snd Pk Log Add
	}
}
bool TrseTransport::m_fnXmlSend(const Net5Tuple_t & _rstNet5Tuple, KString  _clsXml)
{
	ezbuffer bufSrc( _clsXml.m_unLen); memcpy(bufSrc.pBuf, (KCSTR)_clsXml, _clsXml.m_unLen);
	ezbuffer bufDest( EZ_COMPRESSMAXDESTLENGTH(_clsXml.m_unLen) );

	if(CZip::m_fnEZCompress(bufDest, bufSrc) == true)
	{
		//Make Header
		KString clsHeader; clsHeader.m_fnReSize(50);
		KString::m_fnStrnCat((KSTR)clsHeader,clsHeader.m_unLen, "WTRSTP/1.0%020d", bufDest.nLen);

		//Header + comp data
		int nSendLen = TRSE_HEADER_SIZE+bufDest.nLen;
		if(nSendLen > SPLIT_SEND_LEN)//해더 + 압축데이터 크기 10M 초과 메시지 버림.
		{
			IFLOG(E_LOG_ERR,"[TrseTransport] Send MaxLen Overflow SendLen[%d] MaxLan[%d]", nSendLen, SPLIT_SEND_LEN);
			return false;
		}
		Bytef* pSendByte = new Bytef[nSendLen]; std::fill_n(pSendByte,nSendLen,0x00);

		memcpy(pSendByte, (KCSTR)clsHeader, TRSE_HEADER_SIZE);
		memcpy(pSendByte+TRSE_HEADER_SIZE, bufDest.pBuf, bufDest.nLen);

		IFLOG(E_LOG_DEBUG,"[TrseTransport] Make Header[%s] SendLen[%d]", (KCSTR)clsHeader, nSendLen);
		//SPLIT_SEND_LEN 보다 작거나 같으면 한 번에 전송
		if(nSendLen <= SPLIT_SEND_LEN)
		{
			bool bSend = m_fnSend(_rstNet5Tuple, pSendByte, nSendLen);
			if(!bSend)
			{
				IFLOG(E_LOG_ERR,"[TrseTransport] Send Fail[%d]", nSendLen);
				return false;
			}
		}
		//분할 전송 중 다른 메시지가 끼어들 여지가 있어 사용하지 않음.
		/*
		else//분할 전송
		{
			unsigned int unTotalLen = nSendLen;
			unsigned int unSendingLen = 0;
			while(unTotalLen > unSendingLen)
			{
				if((unTotalLen - unSendingLen) <= SPLIT_SEND_LEN)//남은 크기가 SPLIT_SEND_LEN 보다 작거나 같을때
				{
					Bytef* pSplitByte = new Bytef[unTotalLen - unSendingLen]; std::fill_n(pSplitByte,unTotalLen - unSendingLen,0x00);
					memcpy(pSplitByte, pSendByte+unSendingLen, unTotalLen - unSendingLen);
					bool bSend = m_fnSend(_rstNet5Tuple, pSplitByte, unTotalLen - unSendingLen);
					if(!bSend)
					{
						IFLOG(E_LOG_ERR,"[TrseTransport] Split Sending Len[%d/%d] Last Fail", unSendingLen, unTotalLen);
						if(pSendByte) delete pSendByte;
						return false;
					}
					unSendingLen += (unTotalLen - unSendingLen);
					IFLOG(E_LOG_DEBUG,"[TrseTransport] Split Sending Len[%d/%d] Last", unSendingLen, unTotalLen);
				}
				else
				{
					Bytef* pSplitByte = new Bytef[SPLIT_SEND_LEN]; std::fill_n(pSplitByte,SPLIT_SEND_LEN,0x00);
					memcpy(pSplitByte, pSendByte+unSendingLen, SPLIT_SEND_LEN);
					bool bSend = m_fnSend(_rstNet5Tuple, pSplitByte, SPLIT_SEND_LEN);
					if(!bSend)
					{
						IFLOG(E_LOG_DEBUG,"[TrseTransport] Split Sending Len[%d/%d] Ing Fail", unSendingLen, unTotalLen);
						if(pSendByte) delete pSendByte;
						return false;
					}
					unSendingLen += SPLIT_SEND_LEN;
					IFLOG(E_LOG_DEBUG,"[TrseTransport] Split Sending Len[%d/%d] Ing", unSendingLen, unTotalLen);
				}

			}
			if(pSendByte) delete pSendByte;
		}
		*/
	}
	else
	{
		IFLOG(E_LOG_ERR,"TrseTransport XmlSend data comp fail[%s]", (KCSTR)_clsXml);
		return false;
	}
	return true;
}
