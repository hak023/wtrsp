
#ifndef COM_AS_EXT_AS_NODE_HXX
#define COM_AS_EXT_AS_NODE_HXX

#include "eSipUtil/NodeMgr.hxx"
#include "eSipUtil/lock.hxx"
#include "eSipUtil/queueThread.hxx"
#include "eSipUtil/string.hxx"
#include "eSipUtil/timerTable.hxx"

#include "eSipCallback.hxx"

#include "ComAsExtAsTrsp.hxx"
#include "ComAsErr.hxx"

namespace ComAs
{

class ExtAsManageEvent;
class ExtAsSipEvent;

typedef enum
{
	E_EXT_AS_NODE_STATUS_CONNECT = 0,		//tcp connect
	E_EXT_AS_NODE_STATUS_DISCONNECT,		//tcp close
	E_EXT_AS_NODE_STATUS_OK,							//success to init, Ping for enable
	E_EXT_AS_NODE_STATUS_FAULT,					//Ping for disable

	E_EXT_AS_NODE_STATUS_NUM,
}EExtAsNodeStatus_t;

typedef enum
{
	E_EXT_AS_TCP_TUPLE_none = 0,
	E_EXT_AS_TCP_TUPLE_server,
	E_EXT_AS_TCP_TUPLE_client,
}EExtAsTcpTupleType;

struct ExtAsNodeTimerData
{
	typedef enum
	{
		E_TIMER_tryCon = 0,
		E_TIMER_sndInit,
		E_TIMER_sndPing,

		E_TIMER_NUM,
	}ETimerType;

	ETimerType m_eTimerType;
	unsigned int m_unAsId;
	time_t m_ulConnectTime;
	eSipUtil::Net5Tuple m_stNet5Tuple;
	bool m_bClient;
	
	ExtAsNodeTimerData()
	{
		m_eTimerType = E_TIMER_NUM;
		m_unAsId = 0;
		m_ulConnectTime = 0;
		m_bClient = true;
	}
};

class ExtAsNodeInfo : public eSipUtil::StlObject
{
	public :
		ExtAsNodeInfo()
		{
			m_fnReset();
		}
		
		void m_fnReset()
		{
			m_eNodeStatus = E_EXT_AS_NODE_STATUS_DISCONNECT;
			m_unAsId = 0;
			m_unMaxSessNum = 0;
			m_unCurrentSessNum = 0;
			m_ulConnectTime = 0;
			m_ulLastTouchTime = 0;
		}

		void m_fnTouch()
		{
			m_ulLastTouchTime = time(NULL);
		}

	private :
		EExtAsNodeStatus_t m_eNodeStatus;
		unsigned int m_unAsId;
		unsigned int m_unMaxSessNum;
		unsigned int m_unCurrentSessNum;
		time_t m_ulConnectTime;
		time_t m_ulLastTouchTime;		

		friend class ExtAsNodeMgr;
};

class ExtAsNodeMgr
{
	public :
		static ExtAsNodeMgr & getInstance();
		~ExtAsNodeMgr();

		static unsigned int m_fnConvertReason();
		
		bool m_fnInit();		
		void m_fnResetNodeStatus();
		bool m_fnIsAvailableCon();
		void m_fnAddTransport();
		void m_fnDelTransportAll();	
		bool m_fnCheckAcceptTuple(const eSipUtil::Net5Tuple & _rclsAcceptTuple);
		EExtAsTcpTupleType m_fnGetTcpTupleType(const eSipUtil::Net5Tuple & _rclsNet5Tuple, bool _bLock=true);
		
		bool m_fnPutMgrEvent(ExtAsManageEvent * _pstExtAsManageEvent);
		bool m_fnPutSipEvent(ExtAsSipEvent * _pstExtAsSipEvent);
		
		unsigned int m_fnGetCurrentQueueSize() const;
		const char * m_fnGetNodeStatusStr(char * _pszPrint, unsigned int _unMaxLen);

		EComAsErrType m_fnProcSipEvent4InitFromTu(unsigned int _unRouteSeqId, ComAsSipEvent & _rstComAsSipEvent);
		EComAsErrType m_fnProcSipEvent4OtherFromTu(ComAsSipEvent & _rstComAsSipEvent);
		EComAsErrType m_fnProcInternalTermFromTu(ComAsSipEvent & _rstComAsSipEvent);
		bool m_fnProcSipEventFromExtAs(ExtAsSipEvent & _rstExtAsSipEvent);

		bool m_fnIsRteSeq(unsigned int _unId);

	private :
		static void m_fnCbNodeCfgRefresh(eSipUtil::ColumFile * _pclsObj);
		static void m_fnCbCheckGarbageNode(ExtAsNodeMgr * _pclsExtAsNodeMgr);
		static void m_fnCbProcManageEvent(ExtAsManageEvent * _pstExtAsManageEvent, void * _pObject);
		static void m_fnCbProcSipEvent(ExtAsSipEvent * _pstExtAsSipEvent, void * _pObject);
		static eSIP::EAsyncCbResultType_t m_fnCbAsyncEvent(eDum::Session * _pclsSession, eDum::Dialog * _pclsDialog, int _nAsyncEventType, void * _pData, eSIP::EventResult & _rclsEventResult);
		static void m_fnCbNodeTimer(ExtAsNodeTimerData * _pstTimerData);

		ExtAsNodeMgr();
		void m_fnDestroy();

		void m_fnAddTransport(const eSipUtil::Net5Tuple & _rclsConTuple, bool _bClient, bool _bLock=true);

		void m_fnReloadNodeConfig();
		void m_fnUpdateNodeConfig();

		void m_fnProcCloseEvent(const eSipUtil::Net5Tuple & _rstNet5Tuple);
		void m_fnProcConnectEvent(const eSipUtil::Net5Tuple & _rstNet5Tuple);
		
		void m_fnProcInitReq(const eSipUtil::Net5Tuple & _rstNet5Tuple, ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo);
		void m_fnProcInitRsp(const eSipUtil::Net5Tuple & _rstNet5Tuple, const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo);
		void m_fnProcPingReq(const eSipUtil::Net5Tuple & _rstNet5Tuple, const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo);
		void m_fnProcPingRsp(const eSipUtil::Net5Tuple & _rstNet5Tuple, const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo);

		void m_fnChangeStatusConnect(eSipUtil::NodeConnection & _rclsNodeCon);
		void m_fnChangeStatusDisconnect(eSipUtil::NodeConnection & _rclsNodeCon);
		void m_fnChangeStatusInit(eSipUtil::NodeConnection & _rclsNodeCon, const ExtAsIfComInfo & _rstExtAsIfComInfo, unsigned int _unMaxAsSessNum);
		void m_fnChangeStatusOk(eSipUtil::NodeConnection & _rclsNodeCon, const ExtAsIfComInfo & _rstExtAsIfComInfo, unsigned int _unCurAsSessNum);
		void m_fnChangeStatusFault(eSipUtil::NodeConnection & _rclsNodeCon, const ExtAsIfComInfo & _rstExtAsIfComInfo, unsigned int _unCurAsSessNum);		
		
		ExtAsNodeInfo * m_fnFindNodeInfoFromTuple(const eSipUtil::Net5Tuple & _rstNet5Tuple);
		ExtAsNodeInfo * m_fnFindNodeInfoFromAsId(unsigned int _unAsId, eSipUtil::Net5Tuple * _pstNet5Tuple);

		mutable eSipUtil::RwMutex m_clsNodeRwMutex;
		eSipUtil::NodeMgr m_clsUtilNodeMgr;
		unsigned int m_unRteNum;
		ExtAsTrsp m_clsExtAsTrsp;
		eSipUtil::QueueThread< ExtAsManageEvent > m_clsManageEventThread;
		eSipUtil::QueueThread< ExtAsSipEvent > ** m_parrSipEventThread;		
		eSipUtil::NormalThread< ExtAsNodeMgr > m_clsCheckGarbageNodeThread;
		eSipUtil::TimerTable< ExtAsNodeTimerData > m_clsNodeTimerTable;

};

}

#endif

