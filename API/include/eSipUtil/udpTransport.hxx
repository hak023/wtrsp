
#ifndef ESIPUTIL_TRANSPORT_UDP_H
#define ESIPUTIL_TRANSPORT_UDP_H

#include "transport.hxx"

namespace eSipUtil
{

class UdpTransport : public Transport
{
	public :
		UdpTransport(TransportMgr & _rclsTransportMgr, Epoll & _rclsEpoll, const Net5Tuple_t & _rstNet5Tuple, const SocketCfg & _rstSocketCfg, const TransportConfig & _rstTransportCfg);
		virtual ~UdpTransport();

		virtual ETransportErr_t init(int _nFd);
		virtual ETransportErr_t start();
		virtual ETransportErr_t recvEventProcess();
		virtual ETransportErr_t sndEventProcess();
		virtual int sndData(char * _pSndData, unsigned int _unSndLen, const char * _pszRemoteIp, int _nRemotePort, void * _pSndAppData, ETransportErr_t * _peTransportErr);
		
	protected :
		char * m_pRcvBuffer;
		unsigned int m_unRcvBufferMaxLen;

};

}

#endif
