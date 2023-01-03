#include "TrseNode.h"
#include "TrseNodeMgr.h"
#include "MainConfig.h"
#include "TrseTransport.h"
#include "AppXmlParser.h"
#include "main.h"

using namespace eSipUtil;
TrseNode::TrseNode()
{
	m_pclsOwner=NULL;
	m_clsAddr.reset();
	m_bConnect=false;
	m_eSt = E_TRSE_NODE_ST_DISCONNECTED;
	m_unTrGenId = 50001;
	m_clsSessionID = (KCSTR)g_fnGetUUID();
}
TrseNode::~TrseNode()
{
}
unsigned int TrseNode::m_fnGenTid()
{
	//50001 ~ 60000
	unsigned int unId = 50001; DailyTime curTime;
	m_clsTrLock.m_fnLock();
	if(m_clsTime.m_fnIsCompareTime(DailyTime::E_TIME_CMP_DAY,curTime)== false)
	{
		m_clsTime.m_fnUpdateTime();
		m_unTrGenId = 50001;
		unId = 50001;
	}
	else if(m_unTrGenId > 60000)
	{
		m_unTrGenId = 50001;
		unId = 50001;
	}
	else
	{
		unId = m_unTrGenId;
		++(m_unTrGenId);
	}
	m_clsTrLock.m_fnUnlock();
	return unId;
}
const char * TrseNode::m_fnGetAddrString()
{
	if(m_pclsOwner==NULL) return NULL;
	if(m_clsAddrString.m_unRealLen==0)
	{
		m_clsAddrString.m_fnReSize(1024);
		m_pclsOwner->m_stAddr.getStr((eSipUtil::KSTR)m_clsAddrString,1024,false);
	}
	return (eSipUtil::KCSTR)m_clsAddrString;
}
void TrseNode::m_fnConnected()
{
	SELOG(E_LOG_INFO,"[S:%s] Trse Node Connected", (KCSTR)m_clsSessionID);
	TrseNodeMgr * pclsMgr = TrseNodeMgr::m_fnGetInstance();
	m_eSt = E_TRSE_NODE_ST_CONNECTED;
	m_bConnect = true;

	//ESTABLISH_SESSION_REQ 메시지가 설정된 시간 내로 수신되어야 한다.
	//수신되지 않을 시 Disconnect 처리.
	TrseConEv * pclsNew = new TrseConEv;
	pclsNew->m_eT = E_TRSE_IF_ESTABLISH_SESSION_TMR;
	pclsNew->m_stAddr = m_clsAddr;
	pclsNew->m_pclsTrse = this;
	pclsMgr->m_clsTimer.setTimer(pclsNew, MainConfig::m_fnGetInstance()->m_unTrseEstablishTimeout);
}
void TrseNode::m_fnDisConnected()
{
	SELOG(E_LOG_ERR,"[S:%s] Trse Node Disconnected", (KCSTR)m_clsSessionID);
	m_eSt = E_TRSE_NODE_ST_DISCONNECTED;
	m_bConnect = false;
	for(unsigned int i=0;i< MainConfig::m_fnGetInstance()->m_unWorkerNum;i++)
	{
		AppTrseDisconEvent* pclsEv = new AppTrseDisconEvent;
		pclsEv->m_unWorkerIdx = i;
		pclsEv->m_clsSessionID = (KCSTR)m_clsSessionID;
		Worker::m_fnPutTrseDisconSesClear(pclsEv);
	}
}
void TrseNode::m_fnRecvEstablishSesReq(KString & _rclsXml)
{
	if(m_eSt != E_TRSE_NODE_ST_CONNECTED) return;//only Connect Status.
	m_eSt = E_TRSE_NODE_ST_ESTABLISH_SESSION_REQ;

	m_fnCallLog(false, _rclsXml);
	//<EstablishSessionRequest>가 실패할 경우 W-TRSG는 강제로 Disconnect 한다.
	AclSystemTable* pclsAclSystemTable = AclSystemTable::m_fnGetInstance();
	KString clsID, clsPWD, clsIP;
	AppXmlParser::m_fnGetAuthInfo(_rclsXml, clsID, clsPWD, clsIP);

	//	E_JOB_RES_OK          "0000" "OK"
	//	E_IP_ACCESS_FAILED    "1006" "IP_ACCESS_FAILED"
	//	E_ID_PASSWORD_FAILED  "1007" "ID_PASSWORD_FAILED"
	ETrsgCodeSet_t eSt = pclsAclSystemTable->m_fnCheckLogin(clsID, clsPWD, clsIP);

	m_fnSendEstablishSesRes(_rclsXml, eSt);
}
void TrseNode::m_fnSendEstablishSesRes(KString & _rclsXml, ETrsgCodeSet_t _eSt)
{
	if(m_eSt != E_TRSE_NODE_ST_ESTABLISH_SESSION_REQ) return;
	TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
	m_eSt = E_TRSE_NODE_ST_ESTABLISH_SESSION_RES;

	KString clsEstablishSessionRes; clsEstablishSessionRes.m_fnReSize(10240);
	AppXmlParser::m_fnMakeEstablishSessionRes(_rclsXml, m_clsSessionID, _eSt, clsEstablishSessionRes);

	m_fnCallLog(true, clsEstablishSessionRes);
	pclsTrans->m_fnXmlSend(m_clsAddr, clsEstablishSessionRes);

	if(_eSt != E_JOB_RES_OK ) pclsTrans->m_fnDelConnection(m_clsAddr);//인증 실패 시 Disconnect 처리.
}
void TrseNode::m_fnRecvLinkTestReq(KString & _rclsXml)
{
	if(m_eSt >= E_TRSE_NODE_ST_ESTABLISH_SESSION_RES && m_eSt < E_TRSE_NODE_ST_CLOSE_SESSION_REQ)
	{
		m_eSt = E_TRSE_NODE_ST_LINK_TEST_REQ;
		m_fnCallLog(false, _rclsXml);
		m_fnSendLinkTestRes(_rclsXml);
	}
}
void TrseNode::m_fnSendLinkTestRes(KString & _rclsXml)
{
	if(m_eSt != E_TRSE_NODE_ST_LINK_TEST_REQ) return;
	TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
	m_eSt = E_TRSE_NODE_ST_LINK_TEST_RES;

	KString clsLinkTestRes; clsLinkTestRes.m_fnReSize(10240);
	AppXmlParser::m_fnMakeLinkTestRes(_rclsXml, clsLinkTestRes);
	m_fnCallLog(true, clsLinkTestRes);
	pclsTrans->m_fnXmlSend(m_clsAddr, clsLinkTestRes);
}
void TrseNode::m_fnRecvCloseSesReq(KString & _rclsXml)
{
	m_eSt = E_TRSE_NODE_ST_CLOSE_SESSION_REQ;
	m_fnCallLog(false, _rclsXml);
	m_fnSendCloseSesRes(_rclsXml);
}
void TrseNode::m_fnSendCloseSesRes(KString & _rclsXml)
{
	TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
	if(m_eSt != E_TRSE_NODE_ST_CLOSE_SESSION_REQ) return;
	m_eSt = E_TRSE_NODE_ST_CLOSE_SESSION_RES;

	KString clsCloseSessionRes; clsCloseSessionRes.m_fnReSize(10240);
	AppXmlParser::m_fnMakeCloseSessionRes(_rclsXml, clsCloseSessionRes);
	m_fnCallLog(true, clsCloseSessionRes);
	pclsTrans->m_fnXmlSend(m_clsAddr, clsCloseSessionRes);
	pclsTrans->m_fnDelConnection(m_clsAddr);
}
void TrseNode::m_fnRecvEstablishSesTimeOut()
{
	if(m_eSt == E_TRSE_NODE_ST_CONNECTED)
	{
		SELOG(E_LOG_ERR,"[S:%s]Trse EstablishSesTimeOut", (KCSTR)m_clsSessionID);
		TrseTransport * pclsTrans = TrseTransport::m_fnGetInstance();
		pclsTrans->m_fnDelConnection(m_clsAddr);
	}
}
void TrseNode::m_fnCallLog(bool bSend, KString & _rclsXml)
{
	if(g_fnCheckTrseLog(E_LOG_INFO) == false ) return;

	ELogLevel_t eLv = E_LOG_INFO;
	KString clsLog; clsLog.m_fnReSize(1024 + _rclsXml.m_unRealLen);
	char * pszTemp = (KSTR)clsLog;

	KString clsSessionID = m_clsSessionID;
	unsigned int unTid = AppXmlParser::m_fnGetTransactionID(_rclsXml);

	KString clsMine;
	KString clsOther;
	KString clsBodyType;
	KString clsAddrString;
	AppXmlParser::m_fnGetBodyType(_rclsXml, clsBodyType);

	KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"%s [S:%s][T:%010u] ",m_clsAddr.getStr((KSTR )clsAddrString, 128, false), (KCSTR)clsSessionID,unTid);

	if(bSend)
	{
		AppXmlParser::m_fnGetToFrom(_rclsXml, clsOther, clsMine);
		KString::m_fnStrnCat(pszTemp, clsLog.m_unLen,"%s --> %s : %s",  (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR)clsOther, (KCSTR)clsBodyType);
	}
	else
	{
		AppXmlParser::m_fnGetToFrom(_rclsXml, clsMine, clsOther);
		KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"%s <-- %s : %s",  (KCSTR)MainConfig::m_fnGetInstance()->m_clsSysName, (KCSTR)clsOther, (KCSTR)clsBodyType);
	}
	if(g_fnCheckTrseLog(E_LOG_DEBUG))
	{
		eLv = E_LOG_DEBUG;
		if(MainConfig::m_fnGetInstance()->m_bXmlFormatter)
		{
			KString clsXmlFormat;
			AppXmlParser::m_fnXmlFormatter(_rclsXml, clsXmlFormat);
			KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"\n%s", (KCSTR)clsXmlFormat);
		}
		else
			KString::m_fnStrnCat(pszTemp,clsLog.m_unLen,"\n%s", (KCSTR)_rclsXml);
	}
	SELOG(eLv, pszTemp);
}

