#ifndef TRSG_NODE_MGR_H
#define TRSG_NODE_MGR_H
#include "NodeMgr.hxx"
#include "queueThread.hxx"
#include "timerTable.hxx"
#include "NodeMgr.hxx"
#include "transportMgr.hxx"
#include "DailyTime.h"
#include "TrsgNode.h"

typedef enum
{
	E_TRSG_IF_NONE = 0,
	E_TRSG_IF_CONNECTED,
	E_TRSG_IF_DISCONNECTED,
	E_TRSG_IF_ESTABLISH_SESSION_REQ,
	E_TRSG_IF_ESTABLISH_SESSION_RES,
	E_TRSG_IF_ESTABLISH_SESSION_RES_TMR,
	E_TRSG_IF_LINK_TEST_REQ,
	E_TRSG_IF_LINK_TEST_RES,
	E_TRSG_IF_LINK_TEST_RES_TMR,
	E_TRSG_IF_LINK_TEST_INTERVAL,
	E_TRSG_IF_RECONNECTING,
	E_TRSG_IF_ADD_CONNECTION,
	E_TRSG_IF_DEL_CONNECTION,
	E_TRSG_CON_MAX,
} ETrsgConEv_t;
class TrsgConEv
{
public:
	TrsgConEv()
	{
		m_eT = E_TRSG_IF_NONE;
		m_pclsTrsg = NULL;
		m_clsXml = KNULL;
	}
	~TrsgConEv()
	{
	}
	ETrsgConEv_t m_eT;
	TrsgNode *m_pclsTrsg;
	eSipUtil::Net5Tuple_t m_stAddr;
	eSipUtil::KString m_clsXml;   // TRSG Xml Data
};
class TrsgNodeMgr
{
public:
	TrsgNodeMgr();
	~TrsgNodeMgr();
	static TrsgNodeMgr* m_fnGetInstance();
	static void m_fnPutEv(TrsgConEv *_pclsEv);
	unsigned int m_fnDecisionNode(eSipUtil::Net5Tuple &_rstResult);
	bool m_fnIsAllDeactive();
	bool m_fnConExists(const eSipUtil::Net5Tuple &_rstAddr);
	void m_fnGetNodeRteInfo(const eSipUtil::Net5Tuple &_rstAddr, unsigned int *_punRteId, eSipUtil::KString &_rclsRteName, unsigned int *_punEnable);
	const char* m_fnGetNodeRteName(const eSipUtil::Net5Tuple &_rstAddr);
	eSipUtil::TimerTable<TrsgConEv> m_clsTimer;
	eSipUtil::NodeMgr m_clsNode;
private:
	static void m_fnCbkProc(TrsgConEv *_pclsEv, void *_pvOwner);
	static void fnCbkTimerOut(TrsgConEv *_pclsData);
	static void m_fnCbkNodeChanged(eSipUtil::ColumFile *_pclsObj);

	void m_fnProcConnected(TrsgConEv *_pclsEv);
	void m_fnProcDisConnected(TrsgConEv *_pclsEv);
	//void m_fnProcSendEstablishSesReq(TrsgConEv *_pclsEv);
	void m_fnProcRecvEstablishSesRes(TrsgConEv *_pclsEv);
	void m_fnProcEstablishSesResTimeOut(TrsgConEv *_pclsEv);
	void m_fnProcSendLinkTestReq(TrsgConEv *_pclsEv);
	void m_fnProcRecvLinkTestRes(TrsgConEv *_pclsEv);
	void m_fnProcReconnecting(TrsgConEv *_pclsEv);

	void m_fnProcAddConnection();   //trsg_node.cfg modify
	void m_fnProcDelConnection();   //trsg_node.cfg modify
	TrsgNode* m_fnGetTrsgNode(TrsgConEv *_pclsEv);
	bool m_fnDelTrsgNode(TrsgConEv *_pclsEv);
	eSipUtil::QueueThread<TrsgConEv> *m_pclsQ;
	static TrsgNodeMgr *m_pclsMy;
	DailyTime m_clsTime;
	eSipUtil::RwMutex m_clsTrLock;
	unsigned int m_unTrGenId;
};
TrsgNodeMgr* g_fnCreateTrsgNodeMgr();
#endif

