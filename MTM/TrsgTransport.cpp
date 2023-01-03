#include "TrsgTransport.h"
#include "Worker.h"
#include "TrsgNodeMgr.h"
#include "MainConfig.h"
#include "CZip.h"
#include "AppXmlParser.h"
#include "main.h"

//#define SPLIT_SEND_LEN 32000 //32kb
#define SPLIT_SEND_LEN 10000000 //10mbyte
using namespace eSipUtil;

TrsgTransport* g_fnCreateTrsgTransport()
{
	TrsgTransport *pclsObj = TrsgTransport::m_fnGetInstance();
	return pclsObj;
}
TrsgTransport *TrsgTransport::m_pclsMy = NULL;
TrsgTransport::TrsgTransport():TrspSocketCom("TrsgTransport")
{
	m_fnAddConnection();
}
TrsgTransport::~TrsgTransport(){}
TrsgTransport * TrsgTransport::m_fnGetInstance()
{
	if (m_pclsMy == NULL)
	{
		m_pclsMy = new TrsgTransport;
	}
	return m_pclsMy;
}
void TrsgTransport::m_fnAddConnection()
{
	TrsgNodeMgr *pclsNodeMgr = TrsgNodeMgr::m_fnGetInstance();
	/*
	 ListItr_t stItorLoc;
	 ListItr_t stItorRmt;
	 NodeLoc * pclsLoc = pclsNodeMgr->m_clsNode.m_fnBeginLoc(stItorLoc);
	 NodeRmt * pclsRmt = pclsNodeMgr->m_clsNode.m_fnBeginRmt(stItorRmt);
	 while(pclsLoc)
	 {
	 Net5Tuple_t stAddr;
	 stAddr.m_fnSet("tcp", (KCSTR) pclsLoc->m_clsIP, pclsLoc->m_unPort, (KCSTR)pclsRmt->m_clsIP, pclsRmt->m_unPort);
	 KString clsAddrString;
	 IFLOG(E_LOG_INFO,"TRSG Local Open[%s]", stAddr.getStr((KSTR )clsAddrString, 128, false));
	 m_fnAdd(stAddr, E_TCP_ROLE_CLIENT);
	 pclsLoc = pclsNodeMgr->m_clsNode.m_fnNextLoc(stItorLoc);
	 }
	 */
	ListItr_t stItor;
	NodeRte *pclsRte = pclsNodeMgr->m_clsNode.m_fnBeginRte(stItor);
	while (pclsRte)
	{
		/*
		 KString clsAddrString;
		 clsAddrString.m_fnReSize(1024);
		 IFLOG(E_LOG_DEBUG, "TRSG Local Open[%s]", pclsRte->m_stAddr.getStr((eSipUtil::KSTR)clsAddrString,1024,false));
		 */
		m_fnAdd(pclsRte->m_stAddr, E_TCP_ROLE_CLIENT);
		pclsRte = pclsNodeMgr->m_clsNode.m_fnNextRte(stItor);
	}
}
/*
void TrsgTransport::m_fnDelConnection()
{
	TrsgNodeMgr *pclsNodeMgr = TrsgNodeMgr::m_fnGetInstance();
	ListItr_t stItor;
	NodeRte *pclsRte = pclsNodeMgr->m_clsNode.m_fnBeginRte(stItor);
	while (pclsRte)
	{
		IFLOG(E_LOG_INFO, "Del TRSG Connect!![%s]", m_fnGetAddrString(pclsRte->m_stAddr));
		m_fnDel(pclsRte->m_stAddr);
		pclsRte = pclsNodeMgr->m_clsNode.m_fnNextRte(stItor);
	}
}
*/
void TrsgTransport::m_fnDelConnection(const Net5Tuple_t & _rstNet5Tuple)
{
   m_fnDel(_rstNet5Tuple);
}
const char* TrsgTransport::m_fnGetAddrString(const eSipUtil::Net5Tuple_t &_rstNet5Tupl)
{
	if (m_pclsMy == NULL)
		return NULL;
	if (m_pclsMy->m_clsAddrString.m_unRealLen == 0)
	{
		m_pclsMy->m_clsAddrString.m_fnReSize(1024);
		_rstNet5Tupl.getStr((eSipUtil::KSTR) m_clsAddrString, 1024, false);
	}
	return (eSipUtil::KCSTR) m_pclsMy->m_clsAddrString;
}
//==========> Tcp Event Notify CallBack
void TrsgTransport::m_fnCbkNotify(ETcpNotiEv_t _eT, const Net5Tuple_t &_rstNet5Tuple, ETransportErr_t _eDisconErr)
{
	if (_eT == E_TCP_NOTI_CLIENT_CON)
	{
		TrsgConEv *pclsNew = new TrsgConEv;
		pclsNew->m_eT = E_TRSG_IF_CONNECTED;
		pclsNew->m_stAddr = _rstNet5Tuple;
		KString clsAddrString;
		IFLOG(E_LOG_INFO, "TRSG Connected [%s]", pclsNew->m_stAddr.getStr((KSTR) clsAddrString, 128, false));
		TrsgNodeMgr::m_fnPutEv(pclsNew);
	}
	else if (_eT == E_TCP_NOTI_DISCON)
	{
		TrsgConEv *pclsNew = new TrsgConEv;
		pclsNew->m_eT = E_TRSG_IF_DISCONNECTED;
		pclsNew->m_stAddr = _rstNet5Tuple;
		KString clsAddrString;
		IFLOG(E_LOG_ERR, "TRSG Disconnected [%s]", pclsNew->m_stAddr.getStr((KSTR) clsAddrString, 128, false));
		TrsgNodeMgr::m_fnPutEv(pclsNew);
	}
}
//==========> Packet Size Decision CallBack
int TrsgTransport::m_fnCbkAnalizePacket(const char *_pRcvData, unsigned int _unRcvDataLen, const Net5Tuple_t &_rstNet5Tuple)
{
	if (_unRcvDataLen < TRSG_HEADER_SIZE)
		return 0;

	char szHeader[TRSG_HEADER_SIZE + 1];
	memset(szHeader, 0x00, sizeof(szHeader));
	KString::m_fnStrnCatCpy(szHeader, _pRcvData, sizeof(szHeader));

	return KString::m_fnAtoi(szHeader + TRSG_HEADER_KSIP) + TRSG_HEADER_SIZE;
}
//==========> Send/Recv Trace CallBack
void TrsgTransport::m_fnCbkTracePacket(bool _bSend, const char *_pData, unsigned int _unDataLen, const Net5Tuple_t &_rstNet5Tuple, void *_pSndAppData)
{
	if (_bSend == false)
	{
		// Rcv Pk Log Add
		KString clsAddrString;
		char szHeader[TRSG_HEADER_SIZE + 1];
		memset(szHeader, 0x00, sizeof(szHeader));
		KString::m_fnStrnCatCpy(szHeader, _pData, sizeof(szHeader));
		IFLOG(E_LOG_DEBUG, "[Trsg RECV][%s][len:%d][%s]", _rstNet5Tuple.getStr((KSTR )clsAddrString, 128, false), _unDataLen, szHeader);

		KString clsUncompressData;
		ezbuffer bufSrc(KString::m_fnAtoi(szHeader + TRSG_HEADER_KSIP));
		memcpy(bufSrc.pBuf, (unsigned char*) _pData + TRSG_HEADER_SIZE, KString::m_fnAtoi(szHeader + TRSG_HEADER_KSIP));
		ezbuffer bufDest(KString::m_fnAtoi(szHeader + TRSG_HEADER_KSIP) * 2);

		if (CZip::m_fnEZUncompress(bufDest, bufSrc) == false)
		{
			//1005 또는 1017 리턴줘야 하는데 규격이 이상해서 보류함.
			//압축해제부터 실패면 어떤 메시지가 오류난것인지 알수 없기 때문에 res 를 만들 수 없음.
			if (_pData)
				delete _pData;
			IFLOG(E_LOG_DEBUG, "[TrsgTransport] Uncompress Fail");
			return;
		}
		else
		{
			//clsUncompressData = (const char *)bufDest.pBuf;
			clsUncompressData.m_fnByteCat((KCSTR) bufDest.pBuf, bufDest.nLen);
			if (_pData)
				delete _pData;
			IFLOG(E_LOG_DEBUG, "[TrsgTransport] Uncompress Len:%ld", bufDest.nLen);
		}

		if (AppXmlParser::m_fnValid(clsUncompressData) == false)
			return;

		unsigned int unTid = AppXmlParser::m_fnGetTransactionID(clsUncompressData);
		KString clsBodyType;
		AppXmlParser::m_fnGetBodyType(clsUncompressData, clsBodyType);
		if (clsBodyType == DEF_BODY_ESTABLISH_SESSION_RES)
		{
			TrsgConEv *pclsNew = new TrsgConEv;
			pclsNew->m_eT = E_TRSG_IF_ESTABLISH_SESSION_RES;
			pclsNew->m_stAddr = _rstNet5Tuple;
			pclsNew->m_clsXml = clsUncompressData;
			TrsgNodeMgr::m_fnPutEv(pclsNew);
		}
		else if (clsBodyType == DEF_BODY_LINK_TEST_RES)
		{
			TrsgConEv *pclsNew = new TrsgConEv;
			pclsNew->m_eT = E_TRSG_IF_LINK_TEST_RES;
			pclsNew->m_stAddr = _rstNet5Tuple;
			pclsNew->m_clsXml = clsUncompressData;
			TrsgNodeMgr::m_fnPutEv(pclsNew);
		}
		else if (clsBodyType == DEF_BODY_CREATE_JOB_REQ)
		{
			AppTrsgEvent *pclsEv = new AppTrsgEvent;
			pclsEv->m_eT = E_MAIN_EV_TRSG_CREATE_JOB_REQ;
			pclsEv->m_stAddr = _rstNet5Tuple;
			pclsEv->m_unTid = unTid;
			AppXmlParser::m_fnGetSessionID(clsUncompressData, pclsEv->m_clsSessionID);
			AppXmlParser::m_fnGetJobID(clsUncompressData, pclsEv->m_clsJobID);
			pclsEv->m_clsXml = clsUncompressData;
			Worker::m_fnPutTrsgEv(pclsEv);
		}
		else
		{
			IFLOG(E_LOG_ERR, "[TrsgTransport] [clsBodyType]Error [%s]", (KCSTR ) clsBodyType);
			return;
		}
	}
	else
	{
		// Snd Pk Log Add
	}
}
bool TrsgTransport::m_fnXmlSend(const Net5Tuple_t &_rstNet5Tuple, KString _clsXml)
{
   ezbuffer bufSrc( _clsXml.m_unLen); memcpy(bufSrc.pBuf, (KCSTR)_clsXml, _clsXml.m_unLen);
   ezbuffer bufDest( EZ_COMPRESSMAXDESTLENGTH(_clsXml.m_unLen) );

   if(CZip::m_fnEZCompress(bufDest, bufSrc) == true)
   {
      //Make Header
      KString clsHeader; clsHeader.m_fnReSize(50);
      KString::m_fnStrnCat((KSTR)clsHeader,clsHeader.m_unLen, "WTRSTP/1.0%020d", bufDest.nLen);

      //Header + comp data
      int nSendLen = TRSG_HEADER_SIZE+bufDest.nLen;
      if(nSendLen > SPLIT_SEND_LEN)//해더 + 압축데이터 크기 10M 초과 메시지 버림.
      {
         IFLOG(E_LOG_ERR,"[TrsgTransport] Send MaxLen Overflow SendLen[%d] MaxLan[%d]", nSendLen, SPLIT_SEND_LEN);
         return false;
      }
      Bytef* pSendByte = new Bytef[nSendLen]; std::fill_n(pSendByte,nSendLen,0x00);

      memcpy(pSendByte, (KCSTR)clsHeader, TRSG_HEADER_SIZE);
      memcpy(pSendByte+TRSG_HEADER_SIZE, bufDest.pBuf, bufDest.nLen);

      IFLOG(E_LOG_DEBUG,"[TrsgTransport] Make Header[%s] SendLen[%d]", (KCSTR)clsHeader, nSendLen);
      //SPLIT_SEND_LEN 보다 작거나 같으면 한 번에 전송
      if(nSendLen <= SPLIT_SEND_LEN)
      {
         bool bSend = m_fnSend(_rstNet5Tuple, pSendByte, nSendLen);
         if(!bSend)
         {
            IFLOG(E_LOG_ERR,"[TrsgTransport] Send Fail[%d]", nSendLen);
            return false;
         }
      }
   }

/*
   if(CZip::m_fnEZCompress(bufDest, bufSrc) == true)
   {
      //Make Header
      KString clsHeader; clsHeader.m_fnReSize(1024);
      KString::m_fnStrnCat((KSTR)clsHeader,clsHeader.m_unLen, "WTRSTP/1.%021d", bufDest.nLen);

      //Header + comp data
      int nSendLen = TRSG_HEADER_SIZE+bufDest.nLen;
      Bytef* pSendByte = new Bytef[nSendLen]; std::fill_n(pSendByte,nSendLen,0x00);

      memcpy(pSendByte, (KCSTR)clsHeader, TRSG_HEADER_SIZE);
      memcpy(pSendByte+TRSG_HEADER_SIZE, bufDest.pBuf, bufDest.nLen);

      IFLOG(E_LOG_DEBUG,"[TrsgTransport] Make Header[%s] SendLen[%d]", (KCSTR)clsHeader, nSendLen);
      //SPLIT_SEND_LEN 보다 작거나 같으면 한 번에 전송
      if(nSendLen <= SPLIT_SEND_LEN)
      {
         bool bSend = m_fnSend(_rstNet5Tuple, pSendByte, nSendLen);
         if(!bSend)
         {
            IFLOG(E_LOG_ERR, "[TrsgTransport] Split Send Fail[%d]", nSendLen);
            return false;
         }
      }
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
                  IFLOG(E_LOG_ERR,"[TrsgTransport] Split Sending Len[%d/%d] Last Fail", unSendingLen, unTotalLen);
                  if(pSendByte) delete pSendByte;
                  return false;
               }
               unSendingLen += (unTotalLen - unSendingLen);
               IFLOG(E_LOG_DEBUG,"[TrsgTransport] Split Sending Len[%d/%d] Last", unSendingLen, unTotalLen);
            }
            else
            {
               Bytef* pSplitByte = new Bytef[SPLIT_SEND_LEN]; std::fill_n(pSplitByte,SPLIT_SEND_LEN,0x00);
               memcpy(pSplitByte, pSendByte+unSendingLen, SPLIT_SEND_LEN);
               bool bSend = m_fnSend(_rstNet5Tuple, pSplitByte, SPLIT_SEND_LEN);
               if(!bSend)           
               {
                  IFLOG(E_LOG_DEBUG,"[TrsgTransport] Split Sending Len[%d/%d] Ing Fail", unSendingLen, unTotalLen);
                  if(pSendByte) delete pSendByte;
                  return false;     
               }
               unSendingLen += SPLIT_SEND_LEN;
               IFLOG(E_LOG_DEBUG,"[TrsgTransport] Split Sending Len[%d/%d] Ing", unSendingLen, unTotalLen);
            }

         }
         if(pSendByte) delete pSendByte;
      }
   }
*/
   else
   {
      IFLOG(E_LOG_ERR, "TrsgTransport XmlSend data comp fail[%s]", (KCSTR)_clsXml);
      return false;
   }
   return true;
}

