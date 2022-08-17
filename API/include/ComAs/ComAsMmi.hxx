
#ifndef COM_MMI_HXX
#define COM_MMI_HXX

#include "eSipUtil/transportMgr.hxx"

namespace ComAs
{

class ComAsMmiMgr
{
	public :
		static ComAsMmiMgr & getInstance();
		~ComAsMmiMgr();

		bool m_fnInit();
		void m_fnAddTransport();
		void m_fnDelTransport();

	private :
		/*==== callback of UtilTrspMgr ====*/
		static void m_fnCbRecv(eSipUtil::TransportMgr & _rclsUtilTrspMgr, char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstRcvTuple);
		static void m_fnCbSndSucc(eSipUtil::TransportMgr & _rclsUtilTrspMgr, const char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstSndTuple, void * _pSndAppData);
		static void m_fnCbDelTransport(eSipUtil::TransportMgr & _rclsUtilTrspMgr, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, eSipUtil::ETransportErr_t _eTrspErr);
		
		ComAsMmiMgr();

		void m_fnProcRuleDisReq(const void * _pBody);
		void m_fnProcRuleAddReq(const void * _pBody);
		void m_fnProcRuleDelReq(const void * _pBody);
		void m_fnProcRuleChgReq(const void * _pBody);

		void m_fnProcScrActDisReq(const void * _pBody);
		void m_fnProcScrActAddReq(const void * _pBody);
		void m_fnProcScrActDelReq(const void * _pBody);
		void m_fnProcScrActChgReq(const void * _pBody);

		void m_fnProcScrOpDisReq(const void * _pBody);
		void m_fnProcScrOpAddReq(const void * _pBody);
		void m_fnProcScrOpDelReq(const void * _pBody);
		void m_fnProcScrOpChgReq(const void * _pBody);

		void m_fnProcRouteOpDisReq(const void * _pBody);
		void m_fnProcRouteOpAddReq(const void * _pBody);
		void m_fnProcRouteOpDelReq(const void * _pBody);
		void m_fnProcRouteOpChgReq(const void * _pBody);

		bool m_bInit;
		eSipUtil::Net5Tuple m_stNet5Tuple;
		eSipUtil::SocketCfg_t m_stSocketCfg;
		eSipUtil::TransportConfig_t m_stTrspCfg;
		eSipUtil::TransportMgr * m_pclsUtilTrspMgr;
};

}

#endif
