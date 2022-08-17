#ifndef TRSG_NODE_H
#define TRSG_NODE_H
#include "StlMap.hxx"
#include "transportMgr.hxx"
#include "NodeMgr.hxx"
#include "DailyTime.h"

using namespace eSipUtil;

typedef enum
{
	E_TRSG_NODE_ST_NONE = 0,
	E_TRSG_NODE_ST_CONNECTED,
	E_TRSG_NODE_ST_DISCONNECTED,
	E_TRSG_NODE_ST_ESTABLISH_SESSION_REQ,
	E_TRSG_NODE_ST_ESTABLISH_SESSION_RES,
	E_TRSG_NODE_ST_LINK_TEST_REQ,
	E_TRSG_NODE_ST_LINK_TEST_RES,
	E_TRSG_NODE_ST_MAX
} ETrsgNodeSt_t;
class TrsgNode: public eSipUtil::StlObject
{
public:
	TrsgNode();
	~TrsgNode();
	const char* m_fnGetAddrString();
	void m_fnConnected();
	void m_fnDisConnected();
	void m_fnSendEstablishSesReq();
	void m_fnRecvEstablishSesRes(KString &_rclsXml);
	void m_fnReSendEstablishSesReq();
	void m_fnSendLinkTestReq();
	void m_fnRecvLinkTestRes(KString &_rclsXml);
	void m_fnSendEstablishSesResTimeOut();
	void m_fnSendLinkTestResTimeOut();
	void m_fnReconnecting();
	void m_fnCallLog(bool bSend, KString &_rclsXml);
	unsigned int m_fnGenTid();
	eSipUtil::NodeConnection *m_pclsOwner;
	eSipUtil::KString m_clsAddrString;
	ETrsgNodeSt_t m_eSt;
	unsigned int m_unEstablishSesRetryCnt;
	unsigned int m_unLinkTestRetryCnt;
	eSipUtil::RwMutex m_clsTrLock;
	unsigned int m_unTrGenId;
	eSipUtil::KString m_clsSessionID;
	bool m_bConnect;
};
#endif
