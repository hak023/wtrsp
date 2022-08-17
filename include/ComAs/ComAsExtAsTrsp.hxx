 
#ifndef COM_AS_EXT_AS_TRSP_HXX
#define COM_AS_EXT_AS_TRSP_HXX

#include "eSipUtil/transportMgr.hxx"

#include "ComAsExtAsIf.hxx"
#include "ComAsErr.hxx"

namespace ComAs
{

class ExtAsNodeMgr;

class ExtAsTrsp
{
	public :
		ExtAsTrsp(const char * _pszName, ExtAsNodeMgr & _rclsExtAsNodeMgr);
		~ExtAsTrsp();

		const ExtAsIfMgr & m_fnGetExtAsIfMgr() const{ return m_stExtAsIfMgr; }

		bool m_fnAddTransport(const eSipUtil::Net5Tuple_t & _rstAddTuple, bool _bClient);
		void m_fnResetTransport();
		void m_fnDelTransportAll();
		void m_fnDelTransport(const eSipUtil::Net5Tuple_t & _rstDelTuple);
		bool m_fnSendInitReq(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitReqInfo & _rstExtAsIfInitReqInfo, const eSipUtil::Net5Tuple_t & _rstExtAsTuple);
		bool m_fnSendInitRsp(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgInitRspInfo & _rstExtAsIfInitRspInfo, const eSipUtil::Net5Tuple_t & _rstExtAsTuple);
		bool m_fnSendPingReq(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingReqInfo & _rstExtAsIfPingReqInfo, const eSipUtil::Net5Tuple_t & _rstExtAsTuple);
		bool m_fnSendPingRsp(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgPingRspInfo & _rstExtAsIfPingRspInfo, const eSipUtil::Net5Tuple_t & _rstExtAsTuple);
		EComAsErrType m_fnSendSipEvent(const ExtAsIfComInfo & _rstExtAsIfComInfo, const ExtAsIfMsgSipInfo & _rstExtAsIfSipEvent, const eSipUtil::Net5Tuple_t & _rstExtAsTuple);
		bool m_fnSendInternalTerm(const ExtAsIfComInfo & _rstExtAsIfComInfo, const eSipUtil::Net5Tuple_t & _rstExtAsTuple);
		void m_fnGetQueueStatusStr(char * _pszOutStr, unsigned int _unMaxLen) const;

		//for Test
		void m_fnOgFlexStructTest();

	private :		
		/*==== callback of UtilTrspMgr ====*/
		static void m_fnCbRecv(eSipUtil::TransportMgr & _rclsUtilTrspMgr, char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstRcvTuple);
		static void m_fnCbSndSucc(eSipUtil::TransportMgr & _rclsUtilTrspMgr, const char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstSndTuple, void * _pSndAppData);
		static int m_fnCbTcpRecvMatch(eSipUtil::TransportMgr & _rclsUtilTrspMgr, const char * _pRcvData, unsigned int _unRcvDataLen, const eSipUtil::Net5Tuple_t & _rstRcvTuple, void * _pAppBuffer);
		static void m_fnCbDelTransport(eSipUtil::TransportMgr & _rclsUtilTrspMgr, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, eSipUtil::ETransportErr_t _eTrspErr);
		static void m_fnCbAcceptConnection(eSipUtil::TransportMgr & _rclsUtilTrspMgr, eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		static void m_fnCbConnected4NonBlock(eSipUtil::TransportMgr & _rclsUtilTrspMgr, const eSipUtil::Net5Tuple_t & _rstNet5Tuple);

		ExtAsNodeMgr & m_rclsExtAsNodeMgr;
		ExtAsIfMgr m_stExtAsIfMgr;
		eSipUtil::TransportMgr * m_pclsUtilTrspMgr;

};

}

#endif

