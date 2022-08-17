
#ifndef  __AM_SOCK_H__
#define  __AM_SOCK_H__

#include <errno.h>
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>
#endif // WIN32
#ifdef USING_POLL
#include <sys/poll.h>
#endif

#ifndef WIN32
#define INVALID_SOCKET           (-1)
#endif //WIN32


class CAMSock
{
public:
   static bool m_bInitialized;

   static bool InitSock();
   static int  Socket(); // server only.
   static bool SetNoDelay(int sockfd, bool bTrue);
   static bool TryAssignConn(int* pSockfd, const char * szAddr, int nPort, int  nSec=-1); // client only
   static bool Close(int* pSockfd);
   static bool Bind(int sockfd, const char * szAddr, int nPort);
   static bool Listen(int sockfd, int nBacklog);
   static int  Accept(int sockfd);
   static int  Send(int sockfd, const char* pBuf, int n, int msTimeout=-1);
   static int  ReadyToSend(int sockfd, int msTimeout);
   static int  ReadyToRecv(int sockfd, int msTimeout);
   static int  RecvUntil(int sockfd, char* pBuf, int n, int msTimeout=-1);
   static bool GetPeerName(int sockfd, char* szPeerAddr, int& nPeerPort);
};

#endif  //__AM_SOCK_H__
