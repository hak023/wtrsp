#ifndef _SOCK_H_
#define _SOCK_H_

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>

#include "logger.h"

//////////////////////////////////////////////////////////////////////////////
#define INVALID_SOCKET           (-1)
#define SOCKET_ERROR             (-1)
#ifndef INADDR_NONE
#	define INADDR_NONE              0xffffffff
#endif // INADDR_NONE

#ifndef SOCKLEN_T
#if defined(_HPUX_) || defined(_LINUX_)
#  define SOCKLEN_T int
#else // _HPUX_
#  define SOCKLEN_T socklen_t
#endif // _HPUX_
#endif // SOCKLEN_T

//#define DEBUG_SELECT

typedef struct sockaddr_in       SOCKADDR_IN; 
typedef struct sockaddr          SOCKADDR;
typedef struct hostent *         LPHOSTENT;
typedef struct in_addr *         LPIN_ADDR;
typedef int                      PDKSOCKET;
typedef unsigned int             UINT;


enum SOCKMODE  { SOCK_NONE = -1, SOCK_TCP, SOCK_UDP, SOCK_UUDP };
enum SOCKRMODE { NONE, SERVER, CLIENT };
//delete yun 2010-10-28. compile error
//enum CONNECTSTATUS { OK, FAIL };

//////////////////////////////////////////////////////////////////////////////
// CSocket
class CSocket : public CObject
{
	DECLARE_DYNAMIC(CSocket);
public:
   CSocket();
//@@ Add 0724
   virtual ~CSocket();

   virtual bool Open(char* pszPath);
   virtual bool Open(UINT nPort, char* pszAddr = NULL);

   virtual void Close();

   virtual int Select(int timeout); // msec
   virtual int Select();

   //add yun 
   virtual int Poll(int timeout);

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

   operator PDKSOCKET() const
   { return m_hSocket; }

	bool AssignSocket(int hSocket, int nAddrType);
#ifdef DEBUG_SELECT
	void Interrupt();
	bool CreateIntrPipe();
#endif // DEBUG_SELECT

protected:
   bool CreateSocket(int nAddrFormat, int nSocketType, int nProtocolType = 0);
   bool Bind(char* pszPath);
   bool Bind(UINT nPort, char* pszAddr = NULL);
#if defined(_LINUX_)
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
	PDKSOCKET   m_hSocket;
   CLogger*    m_pLogger;
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
class CTcpSocket : public CSocket
{
	DECLARE_DYNAMIC(CTcpSocket);
// Construction
public:
	CTcpSocket();
	virtual ~CTcpSocket();

   bool Open(char* pszPath);
	bool Open(UINT nPort, char* pszAddr = NULL);
	void Close();

   bool PrepareToClient(UINT nPort, char* pszAddr,
					 bool bBlock = false, //F:block mode, T:non-block mode,
					 int  nSec = 5); //non-block mode timeout (sec)
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
	int ReceiveComplete(void* lpBuf, int nBufLen, int nFlags = 0);
protected:
	UINT  m_nPort;
	char  m_pszAddr[16];

   //ReConnect...
	int   m_nReconnectInterval;
	int   m_nReconnectRetryCount;
   bool  m_bExit;
};


/////////////////////////////////////////////////////////////////////////////
// UnixUdp
class CUnixUdp : public CSocket
{
	DECLARE_DYNAMIC(CUnixUdp);

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

/////////////////////////////////////////////////////////////////////////////
class CInetUdpSocket : public CSocket
{
	DECLARE_DYNAMIC(CInetUdpSocket);

public:
	CInetUdpSocket();
	virtual ~CInetUdpSocket();

   bool Open(char* pszPath);
	bool Open(UINT nPort, char* pszAddr = NULL);
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#endif //_SOCK_H_
