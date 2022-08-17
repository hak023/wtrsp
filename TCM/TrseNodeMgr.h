#ifndef TRSE_NODE_MGR_H
#define TRSE_NODE_MGR_H
#include "NodeMgr.hxx"
#include "queueThread.hxx"
#include "timerTable.hxx"
#include "NodeMgr.hxx"
#include "transportMgr.hxx"
#include "DailyTime.h"
#include "TrseNode.h"

typedef enum
{
	E_TRSE_IF_NONE = 0,
	E_TRSE_IF_CONNECTED,
	E_TRSE_IF_DISCONNECTED,
	E_TRSE_IF_ESTABLISH_SESSION_REQ,
	E_TRSE_IF_ESTABLISH_SESSION_RES,
	E_TRSE_IF_ESTABLISH_SESSION_TMR,
	E_TRSE_IF_LINK_TEST_REQ,
	E_TRSE_IF_LINK_TEST_RES,
	E_TRSE_IF_LINK_TEST_TMR,
	E_TRSE_IF_CLOSE_SESSION_REQ,
	E_TRSE_IF_CLOSE_SESSION_RES,
	E_TRSE_IF_CLOSE_SESSION_TMR,
	E_TRSE_CON_MAX,
} ETrseConEv_t;
class TrseConEv
{
public:
	TrseConEv()
	{
		m_eT = E_TRSE_IF_NONE;
		m_pclsTrse = NULL;
		m_clsXml = KNULL;
	}
	~TrseConEv(){}
	ETrseConEv_t m_eT;
	TrseNode * m_pclsTrse;
	eSipUtil::Net5Tuple_t m_stAddr;
	eSipUtil::KString m_clsXml;   // TRSE Xml Data
};
class TrseNodeMgr
{
public:
	TrseNodeMgr();
	~TrseNodeMgr();
	static TrseNodeMgr * m_fnGetInstance();
	static unsigned int m_fnGenTid();
	static void m_fnPutEv(TrseConEv * _pclsEv);
	eSipUtil::TimerTable<TrseConEv> m_clsTimer;
private:
	static void m_fnCbkProc(TrseConEv * _pclsEv, void * _pvOwner);
	static void fnCbkTimerOut(TrseConEv * _pclsData);

	void m_fnProcConnected(TrseConEv *_pclsEv);
	void m_fnProcDisConnected(TrseConEv *_pclsEv);
	void m_fnProcRecvEstablishSesReq(TrseConEv *_pclsEv);
	void m_fnProcRecvLinkTestReq(TrseConEv *_pclsEv);
	void m_fnProcRecvCloseSesReq(TrseConEv *_pclsEv);
	void m_fnProcRecvEstablishSesTimeOut(TrseConEv *_pclsEv);

	TrseNode * m_fnGetTrseNode(TrseConEv *_pclsEv);
	void m_fnDelTrseNode(TrseConEv *_pclsEv);
	eSipUtil::QueueThread<TrseConEv> * m_pclsQ;
	static TrseNodeMgr * m_pclsMy;
	DailyTime m_clsTime;
	eSipUtil::RwMutex m_clsTrLock;
	unsigned int m_unTrGenId;
	eSipUtil::StlMap m_mapTrseCon;
};
TrseNodeMgr * g_fnCreateTrseNodeMgr();
#endif

