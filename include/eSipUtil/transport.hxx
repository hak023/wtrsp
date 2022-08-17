
#ifndef ESIPUTIL_TRANSPORT_H
#define ESIPUTIL_TRANSPORT_H

#include <string.h>

#include "socket.hxx"
#include "ePoll.hxx"
#include "lock.hxx"
#include "transportDef.hxx"

namespace eSipUtil
{

class Transport
{
	public :
		enum
		{
			E_CONST_MAX_STR_LEN = 256,
		};
		
		Transport(TransportMgr & _rclsTransportMgr, Epoll & _rclsEpoll, const Net5Tuple_t & _rstNet5Tuple, const SocketCfg & _rstSocketCfg, const TransportConfig & _rstTransportCfg)
			: m_rclsTransportMgr(_rclsTransportMgr), m_rclsEpoll(_rclsEpoll), m_stNet5Tuple(_rstNet5Tuple), m_stSocketCfg(_rstSocketCfg), m_stTransportCfg(_rstTransportCfg), m_nFd(-1)
		{
			if( strchr(m_stNet5Tuple.m_szLocalIp, ':') )
				m_nIpVersion = 6;
			else
				m_nIpVersion = 4;			
		}
		virtual ~Transport()
		{
			freeRsc();
		}
		
		virtual ETransportErr_t init(int _nFd) = 0;
		virtual ETransportErr_t start() = 0;
				
		/*=========================================================
		Process recv event of epoll
		=========================================================*/
		virtual ETransportErr_t recvEventProcess() = 0;
		/*=========================================================
		Process send event of epoll
		=========================================================*/
		virtual ETransportErr_t sndEventProcess() = 0;
		/*=========================================================
		Process send data. _pSndData must free by this function
		1. return : -1(fail), 0<=(send data length or WouldBlock(tcp))
		=========================================================*/
		virtual int sndData(char * _pSndData, unsigned int _unSndLen, const char * _pszRemoteIp, int _nRemotePort, void * _pSndAppData, ETransportErr_t * _peTransportErr) = 0;

		inline const char * getStr() const{ return m_szDesc; }
		inline void freeRsc()
		{
			m_rclsEpoll.delFD(m_nFd);
			Socket::closeSocket(m_nFd);
			m_nFd = -1;
			m_szDesc[0] = 0x00;
		}
		inline Net5Tuple_t & getNet5Tuple(){ return m_stNet5Tuple; }
		inline const Net5Tuple_t & getNet5Tuple() const{ return m_stNet5Tuple; }
		inline const SocketCfg & getSocketCfg() const{ return m_stSocketCfg; }
		inline const TransportConfig_t & getTransportCfg() const{ return m_stTransportCfg; }
		inline int getFd() const{ return m_nFd; }
		inline void setSocketCfg(const SocketCfg & _rstSocketCfg)
		{
			Socket::setSocketCfg(m_nFd, _rstSocketCfg);
		}
		inline void setDscp(int _nDscp)
		{
			Socket::setDscp(m_nFd, _nDscp);
		}
		
	protected :
		TransportMgr & m_rclsTransportMgr;
		Epoll & m_rclsEpoll;
		Net5Tuple_t m_stNet5Tuple;
		const SocketCfg m_stSocketCfg;
		const TransportConfig m_stTransportCfg;
		int m_nIpVersion;
		int m_nFd;
		Mutex m_clsSndMutex;		//It is only used when SendThread is disable

		char m_szDesc[E_CONST_MAX_STR_LEN];
		
};

}

#endif

