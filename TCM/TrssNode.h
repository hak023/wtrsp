#ifndef TRSS_NODE_H
#define TRSS_NODE_H
#include "StlMap.hxx"
#include "transportMgr.hxx"
#include "NodeMgr.hxx"
#include "DailyTime.h"

using namespace eSipUtil;

typedef enum
{
	E_TRSS_NODE_ST_NONE = 0,
	E_TRSS_NODE_ST_CONNECTED,
	E_TRSS_NODE_ST_DISCONNECTED,
	E_TRSS_NODE_ST_ESTABLISH_SESSION_REQ,
	E_TRSS_NODE_ST_ESTABLISH_SESSION_RES,
	E_TRSS_NODE_ST_LINK_TEST_REQ,
	E_TRSS_NODE_ST_LINK_TEST_RES,
	E_TRSS_NODE_ST_MAX
} ETrssNodeSt_t;
class TrssNode : public eSipUtil::StlObject
{
	public:
		TrssNode();
		~TrssNode();
		const char * m_fnGetAddrString();
		void m_fnConnected();
		void m_fnDisConnected();
		void m_fnRecvEstablishSesReq(KString & _rclsXml);
		void m_fnSendEstablishSesRes(KString & _rclsXml);
		void m_fnRecvLinkTestReq(KString & _rclsXml);
		void m_fnSendLinkTestRes(KString & _rclsXml);
		void m_fnRecvEstablishSesTimeOut();
		void m_fnCallLog(bool bSend, KString & _rclsXml);
		eSipUtil::NodeConnection * m_pclsOwner;//m_pclsOwner->m_stAddr
		eSipUtil::KString m_clsAddrString;
		eSipUtil::KString m_clsTrsgName;
		eSipUtil::KString m_clsTrssName;
		eSipUtil::KString m_clsSessionID;
		ETrssNodeSt_t m_eSt;
		DailyTime m_clsTime;
		bool m_bConnect;
};
#endif
