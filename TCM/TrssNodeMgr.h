#ifndef TRSS_NODE_MGR_H
#define TRSS_NODE_MGR_H
#include "NodeMgr.hxx"
#include "queueThread.hxx"
#include "timerTable.hxx"
#include "transportMgr.hxx"
#include "DailyTime.h"
#include "TrssNode.h"

typedef enum
{
	E_TRSS_IF_NONE = 0,
	E_TRSS_IF_CONNECTED,
	E_TRSS_IF_DISCONNECTED,
	E_TRSS_IF_ESTABLISH_SESSION_REQ,
	E_TRSS_IF_ESTABLISH_SESSION_RES,
	E_TRSS_IF_ESTABLISH_SESSION_TMR,
	E_TRSS_IF_LINK_TEST_REQ,
	E_TRSS_IF_LINK_TEST_RES,
	E_TRSS_IF_LINK_TEST_TMR,
	E_TRSS_IF_CLOSE_SESSION_TMR,
	E_TRSS_CON_MAX,
}ETrssConEv_t;
class TrssConEv
{
public:
	TrssConEv()
	{
		m_eT = E_TRSS_IF_NONE;
		m_pclsTrss = NULL;
		m_stAddr.reset();
		m_clsXml = KNULL;
	}
	~TrssConEv(){}
	ETrssConEv_t m_eT;
	TrssNode * m_pclsTrss;
	eSipUtil::Net5Tuple_t m_stAddr;
	eSipUtil::KString m_clsXml;   // TRSS Xml Data
};
class TrssNodeMgr
{
	public:
		TrssNodeMgr();
		~TrssNodeMgr();
		static TrssNodeMgr * m_fnGetInstance();
		static unsigned int m_fnGenTid();
		static void m_fnPutEv(TrssConEv * _pclsEv);
		unsigned int m_fnDecisionNode(eSipUtil::Net5Tuple & _rstResult, eSipUtil::KString & _rclsTrsgName, eSipUtil::KString & _rclsTrssName, eSipUtil::KString & _rclsSessionID);
		bool m_fnIsAllDeactive();
		bool m_fnConExists(const eSipUtil::Net5Tuple & _rstAddr);
		const char * m_fnGetNodeRteName(const eSipUtil::Net5Tuple & _rstAddr);
		eSipUtil::TimerTable<TrssConEv> m_clsTimer;
		eSipUtil::NodeMgr m_clsNode;
	private:
		static void m_fnCbkProc(TrssConEv * _pclsEv, void * _pvOwner);
		static void fnCbkTimerOut(TrssConEv * _pclsData);
		static void m_fnCbkNodeChanged(eSipUtil::ColumFile * _pclsObj);

		void m_fnProcConnected(TrssConEv *_pclsEv);
		void m_fnProcDisConnected(TrssConEv *_pclsEv);
		void m_fnProcRecvEstablishSesReq(TrssConEv *_pclsEv);
		void m_fnProcRecvLinkTestReq(TrssConEv *_pclsEv);
		void m_fnProcRecvEstablishSesTimeOut(TrssConEv *_pclsEv);

		TrssNode * m_fnGetTrssNode(TrssConEv *_pclsEv);
		bool m_fnDelTrssNode(TrssConEv *_pclsEv);
		eSipUtil::QueueThread<TrssConEv> * m_pclsQ;
		static TrssNodeMgr * m_pclsMy;
		DailyTime m_clsTime;
		eSipUtil::RwMutex m_clsTrLock;
		unsigned int m_unTrGenId;
};
TrssNodeMgr * g_fnCreateTrssNodeMgr();
#endif

