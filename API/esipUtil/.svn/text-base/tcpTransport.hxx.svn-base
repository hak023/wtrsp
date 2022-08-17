
#ifndef ESIPUTIL_TRANSPORT_TCP_H
#define ESIPUTIL_TRANSPORT_TCP_H

#include "transport.hxx"

namespace eSipUtil
{

typedef enum
{
	E_TCP_CONNECT_STATE_idle = 0,
	E_TCP_CONNECT_STATE_try,
	E_TCP_CONNECT_STATE_establish,

	E_TCP_CONNECT_STATE_NUM,
}ETcpConnectState_t;

class TcpTransport : public Transport
{
	public :
		enum
		{
			E_CONST_DFT_RCV_BUFFER_SPARE_LEN = 50,
		};
		
		TcpTransport(TransportMgr & _rclsTransportMgr, Epoll & _rclsEpoll, const Net5Tuple_t & _rstNet5Tuple, const SocketCfg & _rstSocketCfg, const TransportConfig & _rstTransportCfg);
		virtual ~TcpTransport();

		virtual ETransportErr_t init(int _nFd);
		virtual ETransportErr_t start();
		virtual ETransportErr_t recvEventProcess();
		virtual ETransportErr_t sndEventProcess();
		virtual int sndData(char * _pSndData, unsigned int _unSndLen, const char * _pszRemoteIp, int _nRemotePort, void * _pSndAppData, ETransportErr_t * _peTransportErr);

	protected :
		ETransportErr_t start4Server();
		ETransportErr_t start4Client();
		ETransportErr_t start4Accepted();
		
		ETransportErr_t recvEvent4Server();		
		ETransportErr_t recvEvent4Connection();

		void resetRcvCurrentBuffer();
		void resetSndCurrentBuffer();
		bool resizeRcvBuffer(unsigned int _unResizeLen);
		bool allocateRcvBuffer(unsigned int _unSize = 0);

		ETcpConnectState_t m_eTcpConState;
		char * m_pRcvBuffer;											//when recv, allocate memory dynamically
		char * m_pSndBuffer;											//after send partially, this buffer is used for sending remaining data async
		unsigned int m_unRcvBufferMaxLen;					//Maximum length of m_pRcvBuffer
		unsigned int m_unRcvBufferCurrentLen;				//Current length of m_pRcvBuffer
		unsigned int m_unRcvCompleteMsgLen;				//Length of complete message
		unsigned int m_unSndBufferCurrentLen;			//Current length of m_pSndBuffer

		Mutex m_clsConnectMutex;								//when connecting is fail, for processing recv error and send error in sequence
		bool m_bConnectErr;

		char * m_pAppBuffer;

};

}

#endif

