#include "TrssTransport.h"
#include "Worker.h"
#include "TrssNodeMgr.h"
#include "MainConfig.h"
#include "CZip.h"
#include "AppXmlParser.h"
#include "main.h"

//#define SPLIT_SEND_LEN 16000 //16kb
//#define SPLIT_SEND_LEN 32000 //32kb
#define SPLIT_SEND_LEN 10000000 //10mbyte

using namespace eSipUtil;

TrssTransport * g_fnCreateTrssTransport()
{
	TrssTransport * pclsObj = TrssTransport::m_fnGetInstance();
	return pclsObj;
}

TrssTransport * TrssTransport::m_pclsMy = NULL;
TrssTransport::TrssTransport():TrspSocketCom("TrssTransport")
{
	m_fnAddConnection();
}
TrssTransport::~TrssTransport(){}
TrssTransport * TrssTransport::m_fnGetInstance()
{
	if(m_pclsMy==NULL)
	{
		m_pclsMy = new TrssTransport;
	}
	return m_pclsMy;
}
void TrssTransport::m_fnAddConnection()
{
	TrssNodeMgr * pclsNodeMgr = TrssNodeMgr::m_fnGetInstance();
	ListItr_t stItor;
	NodeLoc * pclsLoc = pclsNodeMgr->m_clsNode.m_fnBeginLoc(stItor);
	while(pclsLoc)
	{
		if(pclsLoc->m_eMod == E_TCP_ROLE_SERVER)
		{
			Net5Tuple_t stAddr;
			stAddr.m_fnSet("tcp", (KCSTR) pclsLoc->m_clsIP, pclsLoc->m_unPort, "0.0.0.0", 0);
			KString clsAddrString;
			IFLOG(E_LOG_INFO,"TRSS Local Open[%s]", stAddr.getStr((KSTR )clsAddrString, 128, false));
			m_fnAdd(stAddr, E_TCP_ROLE_SERVER);
		}
		pclsLoc = pclsNodeMgr->m_clsNode.m_fnNextLoc(stItor);
	}
}
void TrssTransport::m_fnDelConnection(const Net5Tuple_t & _rstNet5Tuple)
{
	m_fnDel(_rstNet5Tuple);
}
void TrssTransport::m_fnCbkNotify(ETcpNotiEv_t _eT, const eSipUtil::Net5Tuple_t & _rstNet5Tuple,
										eSipUtil::ETransportErr_t _eDisconErr)
{
	if(_eT == E_TCP_NOTI_ACCEPT_CON)
	{
		TrssConEv * pclsNew = new TrssConEv;
		pclsNew->m_eT = E_TRSS_IF_CONNECTED;
		pclsNew->m_stAddr = _rstNet5Tuple;
		eSipUtil::KString clsAddrString;
		IFLOG(E_LOG_INFO,"TRSS Connected [%s]", pclsNew->m_stAddr.getStr((KSTR)clsAddrString,128,false));
		TrssNodeMgr::m_fnPutEv(pclsNew);
	}
	else if(_eT == E_TCP_NOTI_DISCON)
	{
		TrssConEv * pclsNew = new TrssConEv;
		pclsNew->m_eT = E_TRSS_IF_DISCONNECTED;
		pclsNew->m_stAddr = _rstNet5Tuple;
		eSipUtil::KString clsAddrString;
		IFLOG(E_LOG_ERR,"TRSS Disconnected [%s]", pclsNew->m_stAddr.getStr((KSTR)clsAddrString,128,false));
		TrssNodeMgr::m_fnPutEv(pclsNew);
	}
}
//==========> Packet Size Decision CallBack
int TrssTransport::m_fnCbkAnalizePacket(const char * _pRcvData,
										unsigned int _unRcvDataLen, 
										const eSipUtil::Net5Tuple_t & _rstNet5Tuple)
{
	if(_unRcvDataLen < TRSS_HEADER_SIZE) return 0;

	char szHeader[TRSS_HEADER_SIZE+1]; memset(szHeader,0x00,sizeof(szHeader));
	KString::m_fnStrnCatCpy(szHeader, _pRcvData,sizeof(szHeader));

	return KString::m_fnAtoi(szHeader+TRSS_HEADER_KSIP)+TRSS_HEADER_SIZE;
}

void TrssTransport::m_fnCbkTracePacket(bool _bSend, const char * _pData,
										unsigned int _unDataLen, 
										const eSipUtil::Net5Tuple_t & _rstNet5Tuple,
										void * _pSndAppData)
{
	if(_bSend==false)
	{
		// Rcv Pk Log Add
		if (KString::m_fnStrnCmp(_pData, "WTRSTP/1.0", 10) != 0)
		{
			KString clsAddrString;
			IFLOG(E_LOG_ERR, "[Trss RECV ERR][%s][len:%d]",
					_rstNet5Tuple.getStr((KSTR )clsAddrString, 128, false),
					_unDataLen);
			if (_pData)
				delete _pData;
			return;
		}

		KString clsAddrString;
		char szHeader[TRSS_HEADER_SIZE+1]; memset(szHeader,0x00,sizeof(szHeader));
		KString::m_fnStrnCatCpy(szHeader, _pData, sizeof(szHeader));
		IFLOG(E_LOG_INFO, "[Trss RECV][%s][len:%d][%s]",
				_rstNet5Tuple.getStr((KSTR )clsAddrString, 128, false),
				_unDataLen, szHeader);

		KString clsUncompressData;
		ezbuffer bufSrc( KString::m_fnAtoi(szHeader+TRSS_HEADER_KSIP) );
		memcpy(bufSrc.pBuf,(unsigned char*)_pData+TRSS_HEADER_SIZE, KString::m_fnAtoi(szHeader+TRSS_HEADER_KSIP));
		ezbuffer bufDest( KString::m_fnAtoi(szHeader+TRSS_HEADER_KSIP) * 2 );

		if(CZip::m_fnEZUncompress(bufDest, bufSrc) == false)
		{
			//1005 �Ǵ� 1017 ������� �ϴµ� �԰��� �̻��ؼ� ������.
			//������������ ���и� � �޽����� ������������ �˼� ���� ������ res �� ���� �� ����.
			if(_pData) delete _pData;
			IFLOG(E_LOG_DEBUG,"[TrssTransport] Uncompress Fail");
			return;
		}
		else
		{
			IFLOG(E_LOG_DEBUG, "[TrssTransport] Uncompress Len:%ld",
					bufDest.nLen);
			clsUncompressData.m_fnByteCat((KCSTR)bufDest.pBuf, bufDest.nLen);
			if (_pData) delete _pData;
		}

		//1015 �Ǵ� 2007 ������� �ϴµ� �԰��� �̻��ؼ� ������.
		//�Ľ̺��� ���и� � �޽����� ������������ �˼� ���� ������ res �� ���� �� ����.
		if(AppXmlParser::m_fnValid(clsUncompressData) == false) return;

		unsigned int unTid = AppXmlParser::m_fnGetTransactionID(clsUncompressData);
		KString clsBodyType;
		AppXmlParser::m_fnGetBodyType(clsUncompressData, clsBodyType);
		if(clsBodyType == DEF_BODY_ESTABLISH_SESSION_REQ)
		{
			TrssConEv * pclsNew = new TrssConEv;
			pclsNew->m_eT = E_TRSS_IF_ESTABLISH_SESSION_REQ;
			pclsNew->m_stAddr = _rstNet5Tuple;
			pclsNew->m_clsXml = clsUncompressData;
			TrssNodeMgr::m_fnPutEv(pclsNew);
		}
		else if(clsBodyType == DEF_BODY_LINK_TEST_REQ)
		{
			TrssConEv * pclsNew = new TrssConEv;
			pclsNew->m_eT = E_TRSS_IF_LINK_TEST_REQ;
			pclsNew->m_stAddr = _rstNet5Tuple;
			pclsNew->m_clsXml = clsUncompressData;
			TrssNodeMgr::m_fnPutEv(pclsNew);
		}
		else if(clsBodyType == DEF_JOB_STATUS_CHANGED_NOTIFY_CREATED)
		{
			AppTrssEvent * pclsEv = new AppTrssEvent;
			pclsEv->m_eT = E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_CREATED;
			pclsEv->m_stAddr = _rstNet5Tuple;
			pclsEv->m_unTid = unTid;
			AppXmlParser::m_fnGetSessionID(clsUncompressData, pclsEv->m_clsSessionID);
			AppXmlParser::m_fnGetJobID(clsUncompressData, pclsEv->m_clsJobID);
			pclsEv->m_clsXml = clsUncompressData;
			Worker::m_fnPutTrssEv(pclsEv);
		}
		else if(clsBodyType == DEF_JOB_STATUS_CHANGED_NOTIFY_WAITING)
		{
			AppTrssEvent * pclsEv = new AppTrssEvent;
			pclsEv->m_eT = E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_WAITING;
			pclsEv->m_stAddr = _rstNet5Tuple;
			pclsEv->m_unTid = unTid;
			AppXmlParser::m_fnGetSessionID(clsUncompressData, pclsEv->m_clsSessionID);
			AppXmlParser::m_fnGetJobID(clsUncompressData, pclsEv->m_clsJobID);
			pclsEv->m_clsXml = clsUncompressData;
			Worker::m_fnPutTrssEv(pclsEv);
		}
		else if(clsBodyType == DEF_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED)
		{
			AppTrssEvent * pclsEv = new AppTrssEvent;
			pclsEv->m_eT = E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED;
			pclsEv->m_stAddr = _rstNet5Tuple;
			pclsEv->m_unTid = unTid;
			AppXmlParser::m_fnGetSessionID(clsUncompressData, pclsEv->m_clsSessionID);
			AppXmlParser::m_fnGetJobID(clsUncompressData, pclsEv->m_clsJobID);
			pclsEv->m_clsXml = clsUncompressData;
			Worker::m_fnPutTrssEv(pclsEv);
		}
		else if(clsBodyType == DEF_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED)
		{
			AppTrssEvent * pclsEv = new AppTrssEvent;
			pclsEv->m_eT = E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED;
			pclsEv->m_stAddr = _rstNet5Tuple;
			pclsEv->m_unTid = unTid;
			AppXmlParser::m_fnGetSessionID(clsUncompressData, pclsEv->m_clsSessionID);
			AppXmlParser::m_fnGetJobID(clsUncompressData, pclsEv->m_clsJobID);
			pclsEv->m_clsXml = clsUncompressData;
			Worker::m_fnPutTrssEv(pclsEv);
		}
		else if(clsBodyType == DEF_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED)
		{
			AppTrssEvent * pclsEv = new AppTrssEvent;
			pclsEv->m_eT = E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED;
			pclsEv->m_stAddr = _rstNet5Tuple;
			pclsEv->m_unTid = unTid;
			AppXmlParser::m_fnGetSessionID(clsUncompressData, pclsEv->m_clsSessionID);
			AppXmlParser::m_fnGetJobID(clsUncompressData, pclsEv->m_clsJobID);
			pclsEv->m_clsXml = clsUncompressData;
			Worker::m_fnPutTrssEv(pclsEv);
		}
		else if(clsBodyType == DEF_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED)
		{
			AppTrssEvent * pclsEv = new AppTrssEvent;
			pclsEv->m_eT = E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED;
			pclsEv->m_stAddr = _rstNet5Tuple;
			pclsEv->m_unTid = unTid;
			AppXmlParser::m_fnGetSessionID(clsUncompressData, pclsEv->m_clsSessionID);
			AppXmlParser::m_fnGetJobID(clsUncompressData, pclsEv->m_clsJobID);
			pclsEv->m_clsXml = clsUncompressData;
			Worker::m_fnPutTrssEv(pclsEv);
		}
		else if(clsBodyType == DEF_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED)
		{
			AppTrssEvent * pclsEv = new AppTrssEvent;
			pclsEv->m_eT = E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED;
			pclsEv->m_stAddr = _rstNet5Tuple;
			pclsEv->m_unTid = unTid;
			AppXmlParser::m_fnGetSessionID(clsUncompressData, pclsEv->m_clsSessionID);
			AppXmlParser::m_fnGetJobID(clsUncompressData, pclsEv->m_clsJobID);
			pclsEv->m_clsXml = clsUncompressData;
			Worker::m_fnPutTrssEv(pclsEv);
		}
		else
		{
			IFLOG(E_LOG_ERR,"[TrssTransport] [clsBodyType]Error [%s]", (KCSTR)clsBodyType);
		}
	}
	else
	{
		// Snd Pk Log Add
	}
}
bool TrssTransport::m_fnXmlSend(const Net5Tuple_t & _rstNet5Tuple, KString  _clsXml)
{
	ezbuffer bufSrc( _clsXml.m_unLen); memcpy(bufSrc.pBuf, (KCSTR)_clsXml, _clsXml.m_unLen);
	ezbuffer bufDest( EZ_COMPRESSMAXDESTLENGTH(_clsXml.m_unLen) );

	if(CZip::m_fnEZCompress(bufDest, bufSrc) == true)
	{
		//Make Header
		KString clsHeader; clsHeader.m_fnReSize(50);
		KString::m_fnStrnCat((KSTR)clsHeader,clsHeader.m_unLen, "WTRSTP/1.0%020d", bufDest.nLen);

		//Header + comp data
		int nSendLen = TRSS_HEADER_SIZE+bufDest.nLen;
		if(nSendLen > SPLIT_SEND_LEN)//�ش� + ���൥���� ũ�� 10M �ʰ� �޽��� ����.
		{
			IFLOG(E_LOG_ERR,"[TrssTransport] Send MaxLen Overflow SendLen[%d] MaxLan[%d]", nSendLen, SPLIT_SEND_LEN);
			return false;
		}
		Bytef* pSendByte = new Bytef[nSendLen]; std::fill_n(pSendByte,nSendLen,0x00);

		memcpy(pSendByte, (KCSTR)clsHeader, TRSS_HEADER_SIZE);
		memcpy(pSendByte+TRSS_HEADER_SIZE, bufDest.pBuf, bufDest.nLen);

		IFLOG(E_LOG_DEBUG,"[TrssTransport] Make Header[%s] SendLen[%d]", (KCSTR)clsHeader, nSendLen);
		//SPLIT_SEND_LEN ���� �۰ų� ������ �� ���� ����
		if(nSendLen <= SPLIT_SEND_LEN)
		{
			bool bSend = m_fnSend(_rstNet5Tuple, pSendByte, nSendLen);
			if(!bSend)
			{
				IFLOG(E_LOG_ERR,"[TrssTransport] Send Fail[%d]", nSendLen);
				return false;
			}
		}
		//���� ���� �� �ٸ� �޽����� ����� ������ �־� ������� ����.
		/*
		else//���� ����
		{
			unsigned int unTotalLen = nSendLen;
			unsigned int unSendingLen = 0;
			while(unTotalLen > unSendingLen)
			{
				if((unTotalLen - unSendingLen) <= SPLIT_SEND_LEN)//���� ũ�Ⱑ SPLIT_SEND_LEN ���� �۰ų� ������
				{
					Bytef* pSplitByte = new Bytef[unTotalLen - unSendingLen]; std::fill_n(pSplitByte,unTotalLen - unSendingLen,0x00);
					memcpy(pSplitByte, pSendByte+unSendingLen, unTotalLen - unSendingLen);
					bool bSend = m_fnSend(_rstNet5Tuple, pSplitByte, unTotalLen - unSendingLen);
					if(!bSend)
					{
						IFLOG(E_LOG_ERR,"[TrssTransport] Split Sending Len[%d/%d] Last Fail", unSendingLen, unTotalLen);
						if(pSendByte) delete pSendByte;
						return false;
					}
					unSendingLen += (unTotalLen - unSendingLen);
					IFLOG(E_LOG_DEBUG,"[TrssTransport] Split Sending Len[%d/%d] Last", unSendingLen, unTotalLen);
				}
				else
				{
					Bytef* pSplitByte = new Bytef[SPLIT_SEND_LEN]; std::fill_n(pSplitByte,SPLIT_SEND_LEN,0x00);
					memcpy(pSplitByte, pSendByte+unSendingLen, SPLIT_SEND_LEN);
					bool bSend = m_fnSend(_rstNet5Tuple, pSplitByte, SPLIT_SEND_LEN);
					if(!bSend)
					{
						IFLOG(E_LOG_DEBUG,"[TrssTransport] Split Sending Len[%d/%d] Ing Fail", unSendingLen, unTotalLen);
						if(pSendByte) delete pSendByte;
						return false;
					}
					unSendingLen += SPLIT_SEND_LEN;
					IFLOG(E_LOG_DEBUG,"[TrssTransport] Split Sending Len[%d/%d] Ing", unSendingLen, unTotalLen);
				}

			}
			if(pSendByte) delete pSendByte;
		}
		*/
	}
	else
	{
		IFLOG(E_LOG_ERR,"TrssTransport XmlSend data comp fail[%s]", (KCSTR)_clsXml);
		return false;
	}
	return true;
}
