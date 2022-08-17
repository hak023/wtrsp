
#ifndef ESIP_TRGWTCPSERVER_HXX
#define ESIP_TRGWTCPSERVER_HXX

#include "rutil/ThreadIf.hxx"
#include "rutil/ConfigParse.hxx"
#include "rutil/Data.hxx"

#include "eRule/CRuleUtiles.h"
#include "eRule/CRUSocket.h"
#include "eRule/CRUTcpServer.h"
#include "TrgwMsg.h"
#include "IBCDefine.h"
#ifdef DEF_COM_CONFIG
#include "ColumFile.hxx"
#endif
typedef enum ETgASState_t
{
	E_TGAS_STATE_OUTOFSRV = 0x00,
	E_TGAS_STATE_PANDING1,
	E_TGAS_STATE_PANDING2,
	E_TGAS_STATE_ACTIVE_SECONDARY,
	E_TGAS_STATE_ACTIVE_PRIMARY,
}ETgASState_t;
inline const char * g_fnStringTgASState(ETgASState_t _eT)
{
	switch(_eT)
	{
		case E_TGAS_STATE_OUTOFSRV: return "E_TGAS_STATE_OUTOFSRV";
		case E_TGAS_STATE_PANDING1: return "E_TGAS_STATE_PANDING1";
		case E_TGAS_STATE_PANDING2: return "E_TGAS_STATE_PANDING2";
		case E_TGAS_STATE_ACTIVE_SECONDARY: return "E_TGAS_STATE_ACTIVE_SECONDARY";
		case E_TGAS_STATE_ACTIVE_PRIMARY: return "E_TGAS_STATE_ACTIVE_PRIMARY";
		default: return "Unknown";
	};
	return "Unknown";
}
typedef enum ETgASReason_t
{
	E_TGAS_REASON_UNDEF = 0x00,
	E_TGAS_REASON_BLOCK = 0x20,
	E_TGAS_REASON_CONNECT_FAIL = 0x40,
	E_TGAS_REASON_HW_FAIL = 0x10,
}ETgASReason_t;
inline const char * g_fnStringTgASReason(ETgASReason_t _eT)
{
	switch(_eT)
	{
		case E_TGAS_REASON_UNDEF: return "E_TGAS_REASON_UNDEF";
		case E_TGAS_REASON_BLOCK: return "E_TGAS_REASON_BLOCK";
		case E_TGAS_REASON_CONNECT_FAIL: return "E_TGAS_REASON_CONNECT_FAIL";
		case E_TGAS_REASON_HW_FAIL: return "E_TGAS_REASON_HW_FAIL";
		default: return "Unknown";
	};
	return "Unknown";
}
typedef enum ETgASBox_t
{
	E_TGAS_BOX_A = 1,
	E_TGAS_BOX_B,
}ETgASBox_t;
inline const char * g_fnStringTgASBox(ETgASBox_t _eT)
{
	switch(_eT)
	{
		case E_TGAS_BOX_A: return "E_TGAS_BOX_A";
		case E_TGAS_BOX_B: return "E_TGAS_BOX_B";
		default: return "Unknown";
	};
	return "Unknown";
}
class TgASStatus
{
	public:
		TgASStatus(){fnClear();}
		~TgASStatus(){fnClear();}
		void fnClear()
		{
			m_eRemoteState=E_TGAS_STATE_OUTOFSRV;m_eRemoteReason=E_TGAS_REASON_UNDEF;
			m_unHBCnt=1;m_eRemoteBox = E_TGAS_BOX_A;m_eLocalBox = E_TGAS_BOX_A;
		}
		void fnRcvBindReq(NS_CS::TrgwBindReqMsg_t * _pstBindReq)
		{
			m_eRemoteBox = (ETgASBox_t)_pstBindReq->m_stBind.box;
			m_eRemoteState = (ETgASState_t)_pstBindReq->m_stBind.self_state;
		}
		void fnRcvHBReq(NS_CS::TrgwHBReqMsg_t * _pstHBReq)
		{
			m_eRemoteBox = (ETgASBox_t)_pstHBReq->m_stHB.box;
			m_eRemoteState = (ETgASState_t)_pstHBReq->m_stHB.self_state;
			m_eRemoteReason = (ETgASReason_t)_pstHBReq->m_stHB.oos_reason;
		}
		ETgASState_t m_eRemoteState;
		ETgASReason_t m_eRemoteReason;
		ETgASBox_t m_eRemoteBox;
		ETgASBox_t m_eLocalBox;
		unsigned int m_unHBCnt;
		
};
typedef enum
{
	E_TRGW_STATE_NONE = 0,
	E_TRGW_STATE_CONNECTED,
	E_TRGW_STATE_PING_ST,
	E_TRGW_STATE_DISCONNECTED
}ETrGWState_t;
inline const char * g_fnStringETrGWState(unsigned int _eT)
{
	switch(_eT)
	{
		case E_TRGW_STATE_NONE: return "E_TRGW_STATE_NONE";
		case E_TRGW_STATE_CONNECTED: return "E_TRGW_STATE_CONNECTED";
		case E_TRGW_STATE_PING_ST: return "E_TRGW_STATE_PING_ST";
		case E_TRGW_STATE_DISCONNECTED: return "E_TRGW_STATE_DISCONNECTED";
		default: return "E_TRGW_STATE_NONE";
	}
	return "E_TRGW_STATE_NONE";
}
/****************************** Tcp Server **********************************************/
#ifdef DEF_TRGW_TRANSPORT_CHANGED
#include "eSipUtil/transportMgr.hxx"
#include "eSipUtil/queueThread.hxx"
#include "eSipUtil/NodeMgr.hxx"
#include "eSipUtil/string.hxx"
#include "eSipUtil/TrafficCounter.hxx"
void g_fnTrGWAddServerTransport();
class TrGWNodeInfo : public eSipUtil::StlObject
{
	public:
		TrGWNodeInfo();
		~TrGWNodeInfo();
		void fnDebug(char * _pszDebug, unsigned int _unMax);
		void fnChangeConnectivity(bool _bConnected);
		void fnIncre();
		unsigned int fnGetCps();
		unsigned int m_unHBCnt;
		unsigned int m_unPingTryCnt;
		unsigned int m_unBindTryCnt;
		ETrGWState_t m_eState;
		unsigned int m_eOOS;
		bool m_bConnected;
		bool m_bOnline;
		bool m_bSessionFull;
		unsigned int m_unCur;
		unsigned int m_unMax;
		eSipUtil::TrafficCounter  m_clsCounter;
		eSipUtil::NodeConnection * m_pclsCon;
};
class TrgwTcpServer
{
	public:
		TrgwTcpServer();
		~TrgwTcpServer();
		static TrgwTcpServer * fnGetInstance();
		static TrgwTcpServer * fnLoadingFromConf();
		static void fnFinalize();
		static bool fnAvailableTrGW();
		static void fnWaitForAvailableTrGW(bool _bAll=false);
		static void fnUpLinkServer();
		static void fnDownStanbyClient();
		static void fnSend2TgAS(const eSipUtil::Net5Tuple_t & _rstNet5Tuple, void * _pvData, unsigned int _unLen);
		static void fnSendBindReq(const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		static void fnSendBindRsp(const eSipUtil::Net5Tuple_t & _rstNet5Tuple, NS_CS::TrgwBindReqMsg_t * _pstReq);
		static void fnRecvBindReq(const eSipUtil::Net5Tuple_t & _rstNet5Tuple,NS_CS::TrgwBindReqMsg_t * _pstReq);
		static void fnRecvBindRsp(const eSipUtil::Net5Tuple_t & _rstNet5Tuple, NS_CS::TrgwBindRspMsg_t * _pstReq);
		static void fnSendHBReq(const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		static void fnSendHBRsp(const eSipUtil::Net5Tuple_t & _rstNet5Tuple, NS_CS::TrgwHBReqMsg_t * _pstReq);
		static void fnRecvHBReq(const eSipUtil::Net5Tuple_t & _rstNet5Tuple, NS_CS::TrgwHBReqMsg_t * _pstReq);
		static void fnRecvHBRsp(const eSipUtil::Net5Tuple_t & _rstNet5Tuple, NS_CS::TrgwHBRspMsg_t * _pstRsp);
		static unsigned int  fnAssignTgAs(eSipUtil::Net5Tuple & _rstResult);
		static void fnPollingConnectAlarm();
		static void fnStanbyClearAlarm();
		static bool fnIsStackBlock(bool _bLock=true);
		static bool fnIsAllMBlock(bool _bLock=true);
		static bool fnIsAllFBlock(bool _bLock=true);
		static bool fnIsAllFullEmp(bool _bLock=true);
		static bool fnIsAllDisconnected(bool _bLock=true);
		static bool fnIsAllDisConOrMBlk(bool _bLock=true);
		static bool fnIsAllFullOrMBlk(bool _bLock=true);
		static bool fnIsAllEmpHWFail();
		static bool fnIsFullSession(TrGWNodeInfo * _pclsAppInfo, NS_CS::TrgwHBReqMsg_t * _pstReq);
		#if defined(DEF_R120) && defined(DEF_FAULT_MSG)
		static bool fnIsAllEmpExtFail();
		#endif
		static bool fnIsDisConOrMBlk(eSipUtil::NodeRte * _pclsRte);
		static bool fnIsFullOrMBlk(eSipUtil::NodeRte * _pclsRte);
		static void fnSendPing();
		static TrGWNodeInfo * fnFindAppNode(const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		static void fnDebug(char * _pszDebug, unsigned int _unMax);
		#ifdef DEF_COM_CONFIG
		static void fnCbkComConfigChanged(eSipUtil::ColumFile * _pclsObj);
		void fnBuildComConfig();
		void fnDelTransportVM();
		bool fnFindVmIp(const char * _pszIP);
		void fnReLoadFromComConf();
		void fnLoadFromComConf();
		#endif
		unsigned int m_unIndex;
		static TrgwTcpServer * m_pclsSrv;
		static bool m_bOdering;
		static unsigned int m_unEquipment;
		eSipUtil::NodeMgr m_clsNodeMgr;
		eSipUtil::RwMutex m_clsLock;
		#ifdef DEF_COM_CONFIG
		eSipUtil::ColumFile m_clsComConfig;
		#endif
};
#else  //--------------- Old TrGW Tcp Server(poll)---------------------->

#endif
#endif

