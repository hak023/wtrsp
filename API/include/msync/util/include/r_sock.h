#ifndef _RPDK_SOCK_H_
#define _RPDK_SOCK_H_

#include "r_base.h"
#include "r_util.h"
#include <errno.h>
#include <sys/types.h>
#ifndef WIN32
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <netdb.h>
#	include <sys/un.h>
#	include <unistd.h>
#	include <fcntl.h>
#endif // WIN32
#ifdef USING_POLL
#include <sys/poll.h>
#endif
namespace RED
{

//////////////////////////////////////////////////////////////////////////////
#ifndef INVALID_SOCKET
#	define INVALID_SOCKET           (-1)
#endif // INVALID_SOCKET
#define SOCKET_ERROR             (-1)
#ifndef INADDR_NONE
#	define INADDR_NONE              0xffffffff
#endif // INADDR_NONE

#if defined(__HPUX__) || defined(__LINUX__) || defined(WIN32)
#  define SOCKLEN_T int
#else // __HPUX__
#  define SOCKLEN_T socklen_t
#endif // __HPUX__

//#define DEBUG_SELECT

typedef struct sockaddr_in       SOCKADDR_IN; 
typedef struct sockaddr          SOCKADDR;
typedef struct hostent *         LPHOSTENT;
typedef struct in_addr *         LPIN_ADDR;
typedef int                      PDKSOCKET;
typedef unsigned int             UINT;


enum SOCKMODE  { SOCK_NONE = -1, SOCK_TCP, SOCK_UDP, SOCK_UUDP };
enum SOCKRMODE { NONE, SERVER, CLIENT };
// 2011.01.19 by minimei7
#if 0
enum CONNECTSTATUS { OK, FAIL };
#endif

//////////////////////////////////////////////////////////////////////////////
// CSocket
class DLLPORT CSocket
{
public:
   CSocket();
//@@ Add 0724
   virtual ~CSocket();

#ifndef WIN32
   virtual bool Open(char* pszPath);
#endif // !WIN32
   virtual bool Open(UINT nPort, char* pszAddr = NULL);

   virtual void Close();

   virtual int Select(int timeout); // msec
   virtual int Select();
#ifdef USING_POLL
	virtual int Poll(int timeout);//msec
	virtual int Poll();
#endif
	virtual int Recv(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int RecvFrom(void* lpBuf, int nBufLen,
					         char* rSocketAddress, UINT& rSocketPort, 
                        int nFlags = 0);

	virtual int Send  (const void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int SendTo(const void* lpBuf, int nBufLen,
			             UINT nHostPort, LPSTR lpszHostAddress = NULL, 
                      int nFlags = 0, bool bDebug = true);

   bool  GetSockName(char* rSocketAddress, UINT& rSocketPort);
   bool  GetPeerName(char* rPeerAddress, UINT& rPeerPort);

	int   GetAddrType()	{ return m_nAddrType; }
   int   GetSocket()    { return m_hSocket; }   
   char* GetPath()      { return m_pszPath; }
   void SetNonBlock();

   operator PDKSOCKET() const
   { return m_hSocket; }

	bool AssignSocket(int hSocket, int nAddrType);
#ifdef DEBUG_SELECT
	void Interrupt();
	bool CreateIntrPipe();
#endif // DEBUG_SELECT

protected:
   bool CreateSocket(int nAddrFormat, int nSocketType, int nProtocolType = 0);
#ifndef WIN32
   bool Bind(char* pszPath);
#endif // !WIN32
   bool Bind(UINT nPort, char* pszAddr = NULL);
#if defined(__LINUX__)
	inline int getsockname (int s, sockaddr *name, int *namelen)
		{ return ::getsockname(s, name, (socklen_t *)namelen); }
	inline ssize_t recvfrom(int s, void *buf, size_t len, int flags,
		struct sockaddr *from, int *fromlen)
		{ return ::recvfrom(s, buf, len, flags, from, (socklen_t *)fromlen); }
	inline int getpeername(int s, struct sockaddr *name, int *namelen)
		{ return ::getpeername(s, name, (socklen_t *)namelen); }
	inline int getsockopt(int s, int level, int optname,
		void *optval, int *optlen)
		{ return ::getsockopt(s, level, optname, optval, (socklen_t *)optlen); }
	inline int accept(int s, struct sockaddr *addr, int *addrlen)
		{ return ::accept(s, addr, (socklen_t *)addrlen); }
#endif

protected:
#ifdef WIN32
	static bool InitSock();
	static bool	m_bInitialized;
#endif // WIN32
	PDKSOCKET   m_hSocket;
   char*       m_pszPath;
	int	      m_nAddrType;
   int         m_fErrOccurred;
#ifdef DEBUG_SELECT
	int			m_hIntrPipe[2];
#endif // DEBUG_SELECT
public:
//   friend class CInetUdpSocket;
//   friend class CUnixUdp;
//   friend class CTcpSocket;
};

//////////////////////////////////////////////////////////////////////////////
// CTcpSocket
// : 주의 - Server는 연결 요청시, accept된 소켓으로 listen 소켓을 치환해버린다
class DLLPORT CTcpSocket : public CSocket
{
// Construction
public:
	CTcpSocket();
	virtual ~CTcpSocket();

#ifndef WIN32
   bool Open(char* pszPath);
#endif // !WIN32
	bool Open(UINT nPort, char* pszAddr = NULL);
	void Close();

   bool PrepareToClient(UINT nPort, char* pszAddr,
					 bool bBlock = false, //F:block mode, T:non-block mode,
					 int  nSec = 1); //non-block mode timeout (sec)
   bool PrepareToServer(UINT nPort, char* pszAddr,
                        int nConnectionBacklog = 1);

public:
	bool Listen(int nConnectionBacklog=1);

	bool Connect(UINT nPort, const char* lpszAddr,
					 bool bBlock = false, //F:block mode, T:non-block mode,
					 int  nSec = 5); //non-block mode timeout (sec)
	bool Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);
	bool Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen,
					 int nSec);

	bool ReConnect(); 
	char* GetConnectAddr() { return m_pszAddr; }

   void SetEnvParameters(int nInterval, int nRetryCount) {
				  m_nReconnectInterval = nInterval;
				  m_nReconnectRetryCount = nRetryCount;
	}

	bool Accept(SOCKADDR* lpSockAddr, int* lpSockAddrLen);
//	bool Accept(CTcpSocket& rConnectedSocket,
//		         SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

//@@ Add 0724
	CSocket *Accept(SOCKADDR_IN *pSockAddr = NULL);
protected:
	UINT  m_nPort;
	char  m_pszAddr[16];

   //ReConnect...
	int   m_nReconnectInterval;
	int   m_nReconnectRetryCount;
   bool  m_bExit;
};


#ifndef WIN32
/////////////////////////////////////////////////////////////////////////////
// UnixUdp
class DLLPORT CUnixUdp : public CSocket
{
public:
   CUnixUdp();   
   virtual ~CUnixUdp();
   
   bool Open(char* pszPath);	
   bool Open(UINT nPort, char* pszAddr = NULL);

   int  SendTo(const char *lpBuf, int nBuflen, char *pszTo, bool bDebug=true);
   int  SendTo(sockaddr_un *pTo, char *lpBuf, int nBuflen, bool bDebug=true);   
   int  RecvFrom(char *pBuf, int buflen, sockaddr_un *pFrom = NULL);

   //virtual functions
   int Recv(void* lpBuf, int nBufLen, int nFlags = 0) 
//   { return 0; }
   { return RecvFrom((char*)lpBuf, nBufLen); }
   int RecvFrom(void* lpBuf, int nBufLen,
                char* rSocketAddress, UINT& rSocketPort,                         
                int nFlags = 0) 
   { return 0; }

   int Send  (const void* lpBuf, int nBufLen, int nFlags = 0)
   { return 0; }
   int SendTo(const void* lpBuf, int nBufLen,			             
              UINT nHostPort, LPSTR lpszHostAddress = NULL,                       
              int nFlags = 0)
   { return 0; }
};
#endif // WIN32

/////////////////////////////////////////////////////////////////////////////
class DLLPORT CInetUdpSocket : public CSocket
{
public:
	CInetUdpSocket();
	virtual ~CInetUdpSocket();

   bool Open(char* pszPath);
	bool Open(UINT nPort, char* pszAddr = NULL);
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
}; //namespace RED
#endif //_SOCK_H_
