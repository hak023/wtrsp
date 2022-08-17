#include "TrsgNode.h"
#include "TrsgNodeMgr.h"
#include "MainConfig.h"
#include "TrsgTransport.h"
#include "AppXmlParser.h"
#include "NasSystemTable.h"
#include "main.h"

using namespace eSipUtil;
TrsgNode::TrsgNode()
{
	m_pclsOwner = NULL;
	m_unEstablishSesRetryCnt = 0;
	m_unLinkTestRetryCnt = 0;
	m_bConnect = false;
	m_eSt = E_TRSG_NODE_ST_DISCONNECTED;
	m_unTrGenId = 1;
	m_clsSessionID = (KCSTR)g_fnGetUUID();	// 발급해주는게 맞는지
}
TrsgNode::~TrsgNode()
{
}
unsigned int TrsgNode::m_fnGenTid()
{
	// 1~정수 범위
	unsigned int unId = 0;
	DailyTime curTime;
	m_clsTrLock.m_fnLock();
	if (m_unTrGenId == 2147483647)
	{
		m_unTrGenId = 1;
		unId = 1;
	}
	unId = m_unTrGenId;
	++(m_unTrGenId);
	m_clsTrLock.m_fnUnlock();
	return unId;
}
const char* TrsgNode::m_fnGetAddrString()
{
	if (m_pclsOwner == NULL)
		return NULL;
	if (m_clsAddrString.m_unRealLen == 0)
	{
		m_clsAddrString.m_fnReSize(1024);
		m_pclsOwner->m_stAddr.getStr((eSipUtil::KSTR) m_clsAddrString, 1024, false);
	}
	return (eSipUtil::KCSTR) m_clsAddrString;
}
void TrsgNode::m_fnConnected()
{
	m_eSt = E_TRSG_NODE_ST_CONNECTED;
	m_bConnect = true;
	m_fnSendEstablishSesReq();
}
void TrsgNode::m_fnDisConnected()
{
	SGLOG(E_LOG_ERR, "[S:%s] Trsg Node DisConnected", (KCSTR)m_clsSessionID);
	unsigned int unTrsgReconnTmr = 100;
	TrsgNodeMgr *pclsMgr = TrsgNodeMgr::m_fnGetInstance();
	MainConfig *pclsConf = MainConfig::m_fnGetInstance();

	m_eSt = E_TRSG_NODE_ST_DISCONNECTED;
	m_bConnect = false;

	unTrsgReconnTmr = pclsConf->m_unReconnTimeout;
	m_unEstablishSesRetryCnt = 0;
	m_unLinkTestRetryCnt = 0;

	TrsgConEv *pclsNew = new TrsgConEv;
	pclsNew->m_eT = E_TRSG_IF_RECONNECTING;
	pclsNew->m_pclsTrsg = this;
	pclsMgr->m_clsTimer.setTimer(pclsNew, unTrsgReconnTmr);
}
void TrsgNode::m_fnSendEstablishSesReq()
{
	SGLOG(E_LOG_INFO, "Trsg Node Send EstablishSessionRequest");
	if (m_eSt != E_TRSG_NODE_ST_CONNECTED)
		return;
	m_eSt = E_TRSG_NODE_ST_ESTABLISH_SESSION_REQ;

	TrsgNodeMgr *pclsMgr = TrsgNodeMgr::m_fnGetInstance();
	TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();
	MainConfig *pclsConf = MainConfig::m_fnGetInstance();

	TrsgConEv *pclsNew = new TrsgConEv;
	pclsNew->m_eT = E_TRSG_IF_ESTABLISH_SESSION_RES_TMR;
	pclsNew->m_pclsTrsg = this;
	pclsMgr->m_clsTimer.setTimer(pclsNew, pclsConf->m_unEstablishSesResTimeout);

	++m_unEstablishSesRetryCnt;

	int clsTranId = m_fnGenTid();
	KString clsEstablishSessionReq;
	clsEstablishSessionReq.m_fnReSize(10240);
	AppXmlParser::m_fnMakeEstablishSessionReq(pclsConf->m_clsLocName, pclsConf->m_clsDstName, clsTranId, clsEstablishSessionReq);

	m_fnCallLog(true, clsEstablishSessionReq);
	pclsTrans->m_fnXmlSend(m_pclsOwner->m_stAddr, clsEstablishSessionReq);
}
void TrsgNode::m_fnRecvEstablishSesRes(KString &_rclsXml)
{
	if (m_eSt != E_TRSG_NODE_ST_ESTABLISH_SESSION_REQ)
		return;

   SGLOG(E_LOG_INFO, "[S:%s] Trsg Node Connected!", (KCSTR)m_clsSessionID);
	m_eSt = E_TRSG_NODE_ST_ESTABLISH_SESSION_RES;
	TrsgNodeMgr *pclsMgr = TrsgNodeMgr::m_fnGetInstance();
	pclsMgr->m_clsNode.m_fnChangeConnectivity(m_pclsOwner->m_stAddr, true);

	AppXmlParser::m_fnGetSessionID(_rclsXml, m_clsSessionID);
	m_fnCallLog(false, _rclsXml);
	m_fnSendLinkTestReq();
}
void TrsgNode::m_fnReSendEstablishSesReq()
{
	if (m_eSt >= E_TRSG_NODE_ST_ESTABLISH_SESSION_RES)
		return;
	if (m_eSt == E_TRSG_NODE_ST_DISCONNECTED)
		return;

	MainConfig *pclsConfig = MainConfig::m_fnGetInstance();
	if (pclsConfig->m_unEstablishSesRetryCnt > m_unEstablishSesRetryCnt)
	{
		m_fnSendEstablishSesReq();
	}
	else
	{
      TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();
	   pclsTrans->m_fnDelConnection(m_pclsOwner->m_stAddr);
	}
}
void TrsgNode::m_fnSendLinkTestReq()
{
	TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();
	MainConfig *pclsConf = MainConfig::m_fnGetInstance();

	if(m_eSt < E_TRSG_NODE_ST_CONNECTED)
		return;
	if(m_eSt >= E_TRSG_NODE_ST_ESTABLISH_SESSION_RES)
	{
		if(pclsConf->m_unLinkTestRetryCnt <= m_unLinkTestRetryCnt)
		{
         SGLOG(E_LOG_ERR, "Not Receive LinkTestResponse, Trsg Node Disconnection");
	      pclsTrans->m_fnDelConnection(m_pclsOwner->m_stAddr);
			return;
		}
		m_eSt = E_TRSG_NODE_ST_LINK_TEST_REQ;

		TrsgNodeMgr *pclsMgr = TrsgNodeMgr::m_fnGetInstance();
		TrsgConEv *pclsNew = new TrsgConEv;
		pclsNew->m_eT = E_TRSG_IF_LINK_TEST_RES_TMR;
		pclsNew->m_pclsTrsg = this;
		pclsMgr->m_clsTimer.setTimer(pclsNew, pclsConf->m_unLinkTestResTimeout);

		++m_unLinkTestRetryCnt;
		int clsTranId = m_fnGenTid();

		SGLOG(E_LOG_INFO, "Trsg Node Send LinkTestRequest[%d]", clsTranId);
		KString clsLinkTestReq;
		clsLinkTestReq.m_fnReSize(10240);
		AppXmlParser::m_fnMakeLinkTestReq(pclsConf->m_clsLocName, pclsConf->m_clsDstName, m_clsSessionID, clsTranId, clsLinkTestReq);
		m_fnCallLog(true, clsLinkTestReq);
		pclsTrans->m_fnXmlSend(m_pclsOwner->m_stAddr, clsLinkTestReq);
	}
}
void TrsgNode::m_fnRecvLinkTestRes(KString &_rclsXml)
{
	m_unLinkTestRetryCnt = 0;
	m_eSt = E_TRSG_NODE_ST_LINK_TEST_RES;
	m_fnCallLog(false, _rclsXml);
}
void TrsgNode::m_fnSendEstablishSesResTimeOut()
{
	SGLOG(E_LOG_ERR, "[S:%s] Trsg Node EstablishSesResTimeOut", (KCSTR)m_clsSessionID);
	if (m_eSt == E_TRSG_NODE_ST_ESTABLISH_SESSION_REQ)
	{
		TrsgNodeMgr *pclsMgr = TrsgNodeMgr::m_fnGetInstance();
		TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();
		if (pclsMgr->m_fnConExists(m_pclsOwner->m_stAddr))
		{
			pclsTrans->m_fnDelConnection(m_pclsOwner->m_stAddr);
		}
	}
}
void TrsgNode::m_fnSendLinkTestResTimeOut()
{
	SGLOG(E_LOG_ERR, "[S:%s] Trsg Node LinkTestResTimeOut", (KCSTR)m_clsSessionID);
	if (m_eSt == E_TRSG_NODE_ST_LINK_TEST_REQ)
	{
		SGLOG(E_LOG_ERR, "[S:%s] Trsg m_fnSendLinkTestResTimeOut", (KCSTR) m_clsSessionID);
		TrsgNodeMgr *pclsMgr = TrsgNodeMgr::m_fnGetInstance();
		TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();
		if(pclsMgr->m_fnConExists(m_pclsOwner->m_stAddr))
		{
			pclsTrans->m_fnDelConnection(m_pclsOwner->m_stAddr);
		}
	}
}
void TrsgNode::m_fnReconnecting()
{
	TrsgTransport *pclsTrans = TrsgTransport::m_fnGetInstance();
	TrsgNodeMgr *pclsMgr = TrsgNodeMgr::m_fnGetInstance();
	if (pclsMgr->m_fnConExists(m_pclsOwner->m_stAddr) == false) //Recon Tmr 동안 Node 설정이 변경되었수도 있음으로 중복 확인 함.
	{
		SGLOG(E_LOG_INFO, "Trsg Node trying to Reconnecting [%s]...", pclsTrans->m_fnGetAddrString(m_pclsOwner->m_stAddr));
		pclsTrans->m_fnAdd(m_pclsOwner->m_stAddr, E_TCP_ROLE_CLIENT);
	}
}
void TrsgNode::m_fnCallLog(bool bSend, KString &_rclsXml)
{
	if (g_fnCheckTrsgLog(E_LOG_INFO) == false)
		return;

	ELogLevel_t eLv = E_LOG_INFO;
	KString clsLog;
	clsLog.m_fnReSize(20480);
	char *pszTemp = (KSTR) clsLog;

	KString clsSessionID = m_clsSessionID;
	unsigned int unTid = AppXmlParser::m_fnGetTransactionID(_rclsXml);

	KString clsMine;
	KString clsOther;
	KString clsBodyType;
	AppXmlParser::m_fnGetBodyType(_rclsXml, clsBodyType);

	KString::m_fnStrnCat(pszTemp, 20480, "[S:%s][T:%010u]", (KCSTR)clsSessionID, unTid);

	if (bSend)
	{
		AppXmlParser::m_fnGetToFrom(_rclsXml, clsOther, clsMine);
		KString::m_fnStrnCat(pszTemp, 20480, "%s --> %s : %s", (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR) clsOther, (KCSTR) clsBodyType);
	}
	else
	{
		AppXmlParser::m_fnGetToFrom(_rclsXml, clsMine, clsOther);
		KString::m_fnStrnCat(pszTemp, 20480, "%s <-- %s : %s", (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR) clsOther, (KCSTR) clsBodyType);
	}
	if (g_fnCheckTrsgLog (E_LOG_DEBUG))
	{
		eLv = E_LOG_DEBUG;
		KString::m_fnStrnCat(pszTemp, 20480, "\n%s", (KCSTR) _rclsXml);
	}
	SGLOG(eLv, pszTemp);
}

