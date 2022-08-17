#include "TrseNodeMgr.h"
#include "main.h"
#include "TrseNode.h"
#include "MainConfig.h"
#include "TrseTransport.h"

using namespace eSipUtil;
TrseNodeMgr * g_fnCreateTrseNodeMgr()
{
	return TrseNodeMgr::m_fnGetInstance();
}
TrseNodeMgr * TrseNodeMgr::m_pclsMy = NULL;
TrseNodeMgr::TrseNodeMgr() :m_clsTimer(TrseNodeMgr::fnCbkTimerOut,"TrseTimer")
{
	m_unTrGenId = 0;
	m_mapTrseCon.m_fnClear();
	m_pclsQ = new QueueThread<TrseConEv>;
	m_pclsQ->setName("TrseNodeMgr");
	m_pclsQ->setMaxQueueSize(1000);
	m_pclsQ->setObject(this);
	m_pclsQ->setProcessCb(m_fnCbkProc);
	m_pclsQ->run();
}
TrseNodeMgr::~TrseNodeMgr(){}
TrseNodeMgr * TrseNodeMgr::m_fnGetInstance()
{
	if(m_pclsMy==NULL) m_pclsMy = new TrseNodeMgr;
	return m_pclsMy;
}
void TrseNodeMgr::m_fnPutEv(TrseConEv * _pclsEv)
{
	TrseNodeMgr * pclsMy = TrseNodeMgr::m_fnGetInstance();
	if(pclsMy->m_pclsQ->put(_pclsEv) == false)
	{
		IFLOG(E_LOG_ERR,"TrseNodeMgr Queue Full");
		delete _pclsEv;
	}
}
void TrseNodeMgr::m_fnCbkProc(TrseConEv * _pclsEv, void * _pvOwner)
{
	TrseNodeMgr * pclsMy = TrseNodeMgr::m_fnGetInstance();
	if(_pclsEv->m_eT == E_TRSE_IF_CONNECTED)
	{
		pclsMy->m_fnProcConnected(_pclsEv);
	}
	else if(_pclsEv->m_eT == E_TRSE_IF_DISCONNECTED)
	{
		pclsMy->m_fnProcDisConnected(_pclsEv);
	}
	else if(_pclsEv->m_eT == E_TRSE_IF_ESTABLISH_SESSION_REQ)
	{
		pclsMy->m_fnProcRecvEstablishSesReq(_pclsEv);
	}
	else if(_pclsEv->m_eT == E_TRSE_IF_LINK_TEST_REQ)
	{
		pclsMy->m_fnProcRecvLinkTestReq(_pclsEv);
	}
	else if(_pclsEv->m_eT == E_TRSE_IF_CLOSE_SESSION_REQ)
	{
		pclsMy->m_fnProcRecvCloseSesReq(_pclsEv);
	}
	else if(_pclsEv->m_eT == E_TRSE_IF_ESTABLISH_SESSION_TMR)
	{
		pclsMy->m_fnProcRecvEstablishSesTimeOut(_pclsEv);
	}
	else
	{
		IFLOG(E_LOG_ERR,"[TrseNodeMgr]Unknown Event Type(%d)", _pclsEv->m_eT);
	}
	delete _pclsEv;
}
void TrseNodeMgr::m_fnProcConnected(TrseConEv *_pclsEv)
{
   TrseNode * pclsNode = m_fnGetTrseNode(_pclsEv);
   if(pclsNode == NULL) return;
   pclsNode->m_fnConnected();
}
void TrseNodeMgr::m_fnProcDisConnected(TrseConEv *_pclsEv)
{
	TrseNode * pclsNode = m_fnGetTrseNode(_pclsEv);
	if (pclsNode == NULL) return;
	pclsNode->m_fnDisConnected();
	m_fnDelTrseNode(_pclsEv);
}
void TrseNodeMgr::m_fnProcRecvEstablishSesReq(TrseConEv *_pclsEv)
{
	TrseNode * pclsNode = m_fnGetTrseNode(_pclsEv);
	if (pclsNode == NULL) return;
	pclsNode->m_fnRecvEstablishSesReq(_pclsEv->m_clsXml);
}
void TrseNodeMgr::m_fnProcRecvLinkTestReq(TrseConEv *_pclsEv)
{
	TrseNode * pclsNode = m_fnGetTrseNode(_pclsEv);
	if (pclsNode == NULL) return;
	pclsNode->m_fnRecvLinkTestReq(_pclsEv->m_clsXml);
}
void TrseNodeMgr::m_fnProcRecvCloseSesReq(TrseConEv *_pclsEv)
{
	TrseNode * pclsNode = m_fnGetTrseNode(_pclsEv);
	if (pclsNode == NULL) return;
	pclsNode->m_fnRecvCloseSesReq(_pclsEv->m_clsXml);
}
void TrseNodeMgr::m_fnProcRecvEstablishSesTimeOut(TrseConEv *_pclsEv)
{
	TrseNode * pclsNode = m_fnGetTrseNode(_pclsEv);
	if (pclsNode == NULL) return;
	pclsNode->m_fnRecvEstablishSesTimeOut();
}
TrseNode * TrseNodeMgr::m_fnGetTrseNode(TrseConEv *_pclsEv)
{
	//Map key : Tuple(Local:121.134.202.25/15334, Remote:121.134.202.25/47708, TCP)
	m_clsTrLock.m_fnLock();
	KString clsAddrString;
	_pclsEv->m_stAddr.getStr((KSTR )clsAddrString, 128, false);
	TrseNode * pclsNode = (TrseNode*)m_mapTrseCon.m_fnFindMap((KCSTR) clsAddrString);

	if (pclsNode == NULL)
	{
		pclsNode = new TrseNode;
		pclsNode->m_clsAddr = _pclsEv->m_stAddr;
		if(m_mapTrseCon.m_fnSetMap((KCSTR)clsAddrString, pclsNode) == false)
		{
			m_clsTrLock.m_fnUnlock();
			IFLOG(E_LOG_ERR,"Get Trse Node Error");
			return NULL;
		}
	}
	m_clsTrLock.m_fnUnlock();
	return pclsNode;
}
void TrseNodeMgr::m_fnDelTrseNode(TrseConEv *_pclsEv)
{
	//Map key : Tuple(Local:121.134.202.25/15334, Remote:121.134.202.25/47708, TCP)
	//Disconnect 이벤트 발생 시, Connection 맵에서 지움.
	m_clsTrLock.m_fnLock();
	KString clsAddrString;
	_pclsEv->m_stAddr.getStr((KSTR )clsAddrString, 128, false);
	TrseNode * pclsNode = (TrseNode*)m_mapTrseCon.m_fnFindMap((KCSTR) clsAddrString);

	if (pclsNode == NULL)
	{
		//none
	}
	else
	{
		m_mapTrseCon.m_fnDelMap((KCSTR)clsAddrString);
	}
	m_clsTrLock.m_fnUnlock();
}
void TrseNodeMgr::fnCbkTimerOut(TrseConEv * _pclsData)
{
	m_fnPutEv(_pclsData);
}
