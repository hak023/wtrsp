#ifndef CRUSOCKET_H
#define CRUSOCKET_H
#include "CRuleUtiles.h"
#include <sys/poll.h>

#define RU_INVALID_SOC           (-1)
#define RU_SOC_ERROR             (-1)
#ifndef INADDR_NONE
#	define INADDR_NONE              0xffffffff
#endif // INADDR_NONE

#ifndef SOCKLEN_T
#define _LINUX_
#if defined(_HPUX_) || defined(_LINUX_)
#  define SOCKLEN_T int
#else // _HPUX_
#  define SOCKLEN_T socklen_t
#endif // _HPUX_
#endif // SOCKLEN_T

#define DEF_SOC_BUFFER_SZ   16384
#define DEF_SOC_SELECT_TIME   10
#define DEF_ENABLE_TCP_POLL

typedef struct sockaddr_in6       SOCKADDR_IN6; 
typedef struct sockaddr_in       SOCKADDR_IN; 
typedef struct sockaddr          SOCKADDR;
typedef struct hostent *         LPHOSTENT;
typedef struct in_addr *         LPIN_ADDR;
typedef int                      PDKSOCKET;
/****************************** Socket Utiles *********************************************/
class RUSocket
{
	public:
		RUSocket();
		~RUSocket();
		//AF_INET-AF_UNIX, SOCK_STREAM-SOCK_DGRAM, 0
		static bool fnCreateSocket(int *pSoc,int nAddrFormat, int nSocketType, int nProtocolType);  
		static bool fnConnect(int pSoc,unsigned int nPort, const char* lpszAddr,  bool bBlock, int  nSec);
		static bool fnNonBlocking(int pSoc);
		static bool fnConnectBlock(int pSoc,const SOCKADDR* lpSockAddr, int nSockAddrLen);
		static bool fnConnectNonBlock(int pSoc,const SOCKADDR* lpSockAddr, int nSockAddrLen, int nSec);
		static int fnAccept(int Srv_Soc,SOCKADDR_IN *pSockAddr);
		static int fnSelect(int pSoc,int timeout);
		static int fnPoll(int pSoc,int timeout,short *_rEvent);
		static int fnRecv(int pSoc,void* lpBuf, int nBufLen, int nFlags);
		static int fnRecvFrom(int pSoc,int m_nAddrType,void* lpBuf, int nBufLen, char* rSocketAddress, 
																unsigned int *rSocketPort, int nFlags);
		static bool fnListen(int pSoc,int nConnectionBacklog);
		static int fnSend(int pSoc,const void* lpBuf, int nBufLen, int nFlags);
		static int fnSendTo(int pSoc,int m_nAddrType,const void* lpBuf, int nBufLen,
											unsigned int nHostPort, char * lpszHostAddress, int nFlags);
		static int fnSendReTry(int _nSoc, void *Buffer, int nLen);
		static void fnSetSocBuff(int pSoc, int Rcv, int Snd);
		//AF_INET-AF_UNIX, SOCK_STREAM-SOCK_DGRAM
		static bool fnOpen(int *pSoc,int nAddrFormat, int nSocketType,
															unsigned int nPort, char* pszAddr); 
		static bool fnBind(int pSoc,int nAddrFormat,unsigned int nPort, char* pszAddr);    //AF_INET-AF_UNIX
		static void fnShutDown(int _nFd);
		static void fnClose(int pSoc);
		static void fnGetInfName(const char * szIpAddress, char * szInfName);
		static int fnGetMTU(const char * szIpAddress,int s_set);    // if s_set is -1, process get mtu
		static bool fnIsVirtualActive(char* pszVip);
};
/*************************** TCP Buffer Class *********************************************/
class RUTcpData
{
	public:
		enum{E_MAX_RETRY_CNT = 1000};    // max 10 seconds
		RUTcpData(unsigned int _unSize);
		~RUTcpData();
		void fnReSize(unsigned int _unNewSize);
		char * m_pszBuff;
		int m_nRcvLen;
		unsigned int m_unRemainLen;
		unsigned int m_unBodyLen;
		unsigned int m_unPos;
		unsigned int m_unMax;
		unsigned int m_unHdrRetryRcvCnt;
		unsigned int m_unBodyRetryRcvCnt;
};
typedef unsigned int (*PfuncDecideBodyLen_t)(void *);
typedef void (*PfuncPacketRcv_t)(RUTcpData *, void *);
class RUTcpBuffer
{
	public:
		enum{E_ERR_BIG_SZ = DEF_RU_1M*100};
		RUTcpBuffer();
		~RUTcpBuffer();
		void fnInit(unsigned int _unHeaderLen, void * _pvUser,
											PfuncDecideBodyLen_t _pfnLen,
											PfuncPacketRcv_t _pfnRcv,
											resip::ThreadIf * _pclsThread,
											bool _bNoneBlock = false);
		int fnRecv(unsigned int _unSoc,char * _pszIP, unsigned int * _puiPort);
		resip::ThreadIf * m_pclsOwner;
	private:
		bool m_bNoneBlock;
		unsigned int m_unHdrLen;
		PfuncDecideBodyLen_t m_pfnLen;
		PfuncPacketRcv_t m_pfnRcv;
		void * m_pvUser;
};
/******************************** TCP Event *********************************************/
typedef enum
{
	E_RU_TCP_EV_NONE,
	E_RU_TCP_EV_ADD,
	E_RU_TCP_EV_DEL,
}ETcpEv_t;
class TcpEv : public RUObject
{
	public:
		TcpEv()
		{
			m_eCmd = E_RU_TCP_EV_NONE;memset(m_szIP,0x00,64);m_nPort=0;m_nSoc=0;
			memset(m_szIP2,0x00,64);m_nPort2=0;
		}
		~TcpEv(){}
		TcpEv & operator=( TcpEv & _rclsSrc)
		{
			if(&_rclsSrc == NULL) return *this;
			m_nSoc = _rclsSrc.m_nSoc;
			m_eCmd = _rclsSrc.m_eCmd;
			memset(m_szIP,0x00,64);
			fnRUStrCpy(m_szIP,_rclsSrc.m_szIP);
			m_nPort = _rclsSrc.m_nPort;
			memset(m_szIP2,0x00,64);
			fnRUStrCpy(m_szIP2,_rclsSrc.m_szIP2);
			m_nPort2 = _rclsSrc.m_nPort2;
			return *this;
		}
		void fnSetAddr(const char * _pszLocalIP,int _nLocalPort,
														const char * _pszRemoteIP,int _nRemotePort)
		{
			memset(m_szIP,0x00,64);fnRUStrCpy(m_szIP,(char*)_pszLocalIP);m_nPort =_nLocalPort;
			memset(m_szIP2,0x00,64);fnRUStrCpy(m_szIP2,(char*)_pszRemoteIP);m_nPort2 =_nRemotePort;
		}
		void fnClear()
		{
			if(m_eCmd != E_RU_TCP_EV_NONE)
			{
				m_eCmd = E_RU_TCP_EV_NONE;memset(m_szIP,0x00,64);m_nPort=0;
			}
		}
		ETcpEv_t m_eCmd;
		int m_nSoc;
		char m_szIP[64];
		int m_nPort;
		char m_szIP2[64];
		int m_nPort2;
};
#endif

