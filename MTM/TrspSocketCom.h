#ifndef TRSP_SOCKETCOM_H
#define TRSP_SOCKETCOM_H

#include "transportMgr.hxx"

typedef enum
{
	E_TCP_ROLE_SERVER = 0,
	E_TCP_ROLE_CLIENT = 1,
	E_TCP_ROLE_ACCEPTED = 2
}ETcpRole_t;
typedef enum
{
	E_TCP_NOTI_NONE = 0,
	E_TCP_NOTI_ACCEPT_CON,
	E_TCP_NOTI_CLIENT_CON,
	E_TCP_NOTI_DISCON
}ETcpNotiEv_t;
using namespace eSipUtil;
class TrspSocketCom
{
	public:
		TrspSocketCom(const char * _pszName);
		~TrspSocketCom();
		//==========> Server or Client Transport Add
		void m_fnAdd(const Net5Tuple_t & _rstNet5Tuple, ETcpRole_t _eRole=E_TCP_ROLE_SERVER);
		//==========> Server or Client Transport Delete
		void m_fnDel(const Net5Tuple_t & _rstNet5Tuple);
		//==========> Send Data
		bool m_fnSend(const Net5Tuple_t & _rstNet5Tuple,void * _pvData,unsigned int _unSndLen);
		static const char * m_fnGetEvString(ETcpNotiEv_t _eT);
	protected:
		//==========> Tcp Event Notify CallBack
		virtual void m_fnCbkNotify(ETcpNotiEv_t _eT, const Net5Tuple_t & _rstNet5Tuple,
												ETransportErr_t _eDisconErr)=0;
		//==========> Packet Size Decision CallBack
		virtual int m_fnCbkAnalizePacket(const char * _pRcvData, 
												unsigned int _unRcvDataLen, 
												const Net5Tuple_t & _rstNet5Tuple) = 0;
		//==========> Send/Recv Trace CallBack
		virtual void m_fnCbkTracePacket(bool _bSend, const char * _pData, 
												unsigned int _unDataLen, 
												const Net5Tuple_t & _rstNet5Tuple,
												void * _pSndAppData) = 0;
		KString m_szName;
	private:
		static void s_fnCbkAcceptConnected(TransportMgr & _rclsTransportMgr, 
												Net5Tuple_t & _rstNet5Tuple);
		static void s_fnCbkClientConnected(TransportMgr & _rclsTransportMgr,
												const Net5Tuple_t & _rstNet5Tuple);
		static void s_fnCbkDisconnected(TransportMgr & _rclsTransportMgr, 
												const Net5Tuple_t & _rstNet5Tuple,
												ETransportErr_t _eTrspErr);
		static int s_fnCbkAnalizePacket(TransportMgr & _rclsTransportMgr,
												const char * _pRcvData, 
												unsigned int _unRcvDataLen, 
												const Net5Tuple_t & _rstNet5Tuple, 
												void * _pvUser);
		static void s_fnCbkSendPacket( TransportMgr & _rclsTransportMgr,
												const char * _pData, 
												unsigned int _unDataLen, 
												const Net5Tuple_t & _rstNet5Tuple,
												void * _pSndAppData);
		static void s_fnCbkRecvPacket(TransportMgr & _rclsTransportMgr,
												char * _pclsData, 
												unsigned int _unDataLen,  
												const Net5Tuple_t & _rstNet5Tuple);
		TransportMgr * m_pclsTrans;
		TransportMgrOption m_stOption;
		SocketCfg m_stSocCfg;
		TransportConfig   m_stTransCfg;
};
#endif
