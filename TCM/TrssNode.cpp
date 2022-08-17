#include "TrssNode.h"
#include "TrssNodeMgr.h"
#include "MainConfig.h"
#include "TrssTransport.h"
#include "AppXmlParser.h"
#include "AclSystemTable.h"
#include "main.h"

using namespace eSipUtil;
TrssNode::TrssNode()
{
	m_pclsOwner=NULL;
	m_bConnect=false;
	m_eSt = E_TRSS_NODE_ST_DISCONNECTED;
	m_clsTrsgName = KNULL;
	m_clsTrssName = KNULL;
	m_clsSessionID = (KCSTR)g_fnGetUUID();
}
TrssNode::~TrssNode()
{
}
const char * TrssNode::m_fnGetAddrString()
{
	if(m_pclsOwner==NULL) return NULL;
	if(m_clsAddrString.m_unRealLen==0)
	{
		m_clsAddrString.m_fnReSize(1024);
		m_pclsOwner->m_stAddr.getStr((eSipUtil::KSTR)m_clsAddrString,1024,false);
	}
	return (eSipUtil::KCSTR)m_clsAddrString;
}
void TrssNode::m_fnConnected()
{
	SSLOG(E_LOG_INFO,"[S:%s] Trss Node Connected", (KCSTR)m_clsSessionID);
	TrssNodeMgr * pclsMgr = TrssNodeMgr::m_fnGetInstance();
	m_eSt = E_TRSS_NODE_ST_CONNECTED;
	m_bConnect = true;

	//ESTABLISH_SESSION_REQ 메시지가 설정된 시간 내로 수신되어야 한다.
	//수신되지 않을 시 Disconnect 처리.
	TrssConEv * pclsNew = new TrssConEv;
	pclsNew->m_eT = E_TRSS_IF_ESTABLISH_SESSION_TMR;
	pclsNew->m_stAddr = m_pclsOwner->m_stAddr;
	pclsNew->m_pclsTrss = this;
	pclsMgr->m_clsTimer.setTimer(pclsNew, MainConfig::m_fnGetInstance()->m_unTrssEstablishTimeout);
}
void TrssNode::m_fnDisConnected()
{
	SSLOG(E_LOG_ERR,"[S:%s] Trss Node Disconnected", (KCSTR)m_clsSessionID);
	m_eSt = E_TRSS_NODE_ST_DISCONNECTED;
	m_bConnect = false;
}
void TrssNode::m_fnRecvEstablishSesReq(KString & _rclsXml)
{
	if(m_eSt != E_TRSS_NODE_ST_CONNECTED) return;
	m_eSt = E_TRSS_NODE_ST_ESTABLISH_SESSION_REQ;

	m_fnCallLog(false, _rclsXml);
	AppXmlParser::m_fnGetToFrom(_rclsXml, m_clsTrsgName/*To*/, m_clsTrssName/*From*/);
	m_fnSendEstablishSesRes(_rclsXml);
}
void TrssNode::m_fnSendEstablishSesRes(KString & _rclsXml)
{
	if(m_eSt != E_TRSS_NODE_ST_ESTABLISH_SESSION_REQ) return;
	TrssTransport * pclsTrans = TrssTransport::m_fnGetInstance();
	TrssNodeMgr * pclsMgr = TrssNodeMgr::m_fnGetInstance();
	pclsMgr->m_clsNode.m_fnChangeConnectivity(m_pclsOwner->m_stAddr,true);//Establish Res 전송 이후 true 설정.

	m_eSt = E_TRSS_NODE_ST_ESTABLISH_SESSION_RES;

	KString clsEstablishSessionRes; clsEstablishSessionRes.m_fnReSize(10240);
	AppXmlParser::m_fnMakeEstablishSessionRes(_rclsXml, m_clsSessionID, E_JOB_RES_OK, clsEstablishSessionRes);//인증 없음.

	m_fnCallLog(true, clsEstablishSessionRes);
	pclsTrans->m_fnXmlSend(m_pclsOwner->m_stAddr, clsEstablishSessionRes);

}
void TrssNode::m_fnRecvLinkTestReq(KString & _rclsXml)
{
	if(m_eSt >= E_TRSS_NODE_ST_ESTABLISH_SESSION_RES && m_eSt < E_TRSS_NODE_ST_MAX)
	{
		m_eSt = E_TRSS_NODE_ST_LINK_TEST_REQ;
		m_fnCallLog(false, _rclsXml);
		m_fnSendLinkTestRes(_rclsXml);
	}
}
void TrssNode::m_fnSendLinkTestRes(KString & _rclsXml)
{
	if(m_eSt != E_TRSS_NODE_ST_LINK_TEST_REQ) return;
	TrssTransport * pclsTrans = TrssTransport::m_fnGetInstance();
	m_eSt = E_TRSS_NODE_ST_LINK_TEST_RES;

	KString clsLinkTestRes; clsLinkTestRes.m_fnReSize(10240);
	AppXmlParser::m_fnMakeLinkTestRes(_rclsXml, clsLinkTestRes);
	m_fnCallLog(true, clsLinkTestRes);
	pclsTrans->m_fnXmlSend(m_pclsOwner->m_stAddr, clsLinkTestRes);
}
void TrssNode::m_fnRecvEstablishSesTimeOut()
{
	if (m_eSt == E_TRSS_NODE_ST_CONNECTED)
	{
		SSLOG(E_LOG_ERR,"[S:%s] Trss EstablishSesTimeOut", (KCSTR)m_clsSessionID);
		TrssNodeMgr * pclsMgr = TrssNodeMgr::m_fnGetInstance();
		TrssTransport * pclsTrans = TrssTransport::m_fnGetInstance();
		if (pclsMgr->m_fnConExists(m_pclsOwner->m_stAddr))//존재하는 커넥션 확인 후 Del
		{
			pclsTrans->m_fnDelConnection(m_pclsOwner->m_stAddr);
		}
	}
}
void TrssNode::m_fnCallLog(bool bSend, KString & _rclsXml)
{
	if(g_fnCheckTrssLog(E_LOG_INFO) == false ) return;

	ELogLevel_t eLv = E_LOG_INFO;
	KString clsLog; clsLog.m_fnReSize(1024 + _rclsXml.m_unRealLen);
	char * pszTemp = (KSTR)clsLog;

	KString clsSessionID = m_clsSessionID;
	unsigned int unTid = AppXmlParser::m_fnGetTransactionID(_rclsXml);

	KString clsMine;
	KString clsOther;
	KString clsBodyType;
	AppXmlParser::m_fnGetBodyType(_rclsXml, clsBodyType);

	KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"[S:%s][T:%010u] ",(KCSTR)clsSessionID,unTid);

	if(bSend)
	{
		AppXmlParser::m_fnGetToFrom(_rclsXml, clsOther, clsMine);
		KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"%s --> %s : %s", (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR)clsOther, (KCSTR)clsBodyType);
	}
	else
	{
		AppXmlParser::m_fnGetToFrom(_rclsXml, clsMine, clsOther);
		KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"%s <-- %s : %s", (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR)clsOther, (KCSTR)clsBodyType);
	}
	if(g_fnCheckTrssLog(E_LOG_DEBUG))
	{
		eLv = E_LOG_DEBUG;
		KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"\n%s", (KCSTR)_rclsXml);
	}
	SSLOG(eLv, pszTemp);
}
