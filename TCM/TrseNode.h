#ifndef TRSE_NODE_H
#define TRSE_NODE_H
#include "StlMap.hxx"
#include "transportMgr.hxx"
#include "NodeMgr.hxx"
#include "DailyTime.h"
#include "AclSystemTable.h"

using namespace eSipUtil;

typedef enum
{
	E_TRSE_NODE_ST_NONE = 0,
	E_TRSE_NODE_ST_CONNECTED,
	E_TRSE_NODE_ST_DISCONNECTED,
	E_TRSE_NODE_ST_ESTABLISH_SESSION_REQ,
	E_TRSE_NODE_ST_ESTABLISH_SESSION_RES,
	E_TRSE_NODE_ST_LINK_TEST_REQ,
	E_TRSE_NODE_ST_LINK_TEST_RES,
	E_TRSE_NODE_ST_CLOSE_SESSION_REQ,
	E_TRSE_NODE_ST_CLOSE_SESSION_RES,
	E_TRSE_NODE_ST_MAX
}ETrseNodeSt_t;
class TrseNode : public eSipUtil::StlObject
{
	public:
		TrseNode();
		~TrseNode();
		const char * m_fnGetAddrString();
		void m_fnConnected();
		void m_fnDisConnected();
		void m_fnRecvEstablishSesReq(KString & _rclsXml);
		void m_fnSendEstablishSesRes(KString & _rclsXml, ETrsgCodeSet_t _eSt);
		void m_fnRecvLinkTestReq(KString & _rclsXml);
		void m_fnSendLinkTestRes(KString & _rclsXml);
		void m_fnRecvCloseSesReq(KString & _rclsXml);
		void m_fnSendCloseSesRes(KString & _rclsXml);
		void m_fnRecvEstablishSesTimeOut();
		unsigned int m_fnGenTid();
		void m_fnCallLog(bool bSend, KString & _rclsXml);
		eSipUtil::NodeConnection * m_pclsOwner;//m_pclsOwner->m_stAddr
		eSipUtil::Net5Tuple m_clsAddr;
		eSipUtil::KString m_clsAddrString;
		eSipUtil::KString m_clsSessionID;
		ETrseNodeSt_t m_eSt;
		DailyTime m_clsTime;
		eSipUtil::RwMutex m_clsTrLock;
		unsigned int m_unTrGenId;
		bool m_bConnect;
};
#endif
