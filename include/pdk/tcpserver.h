#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "list.h"
#include "sock.h"
#include "qthread.h"
#include "util.h"
#include "inifile.h"

#define MSG_FUNC_KILL_SERVICE 0xff

typedef enum {
	TCP_MASK = 0xefef,
	TCP_CONN = 0xe1e1,
	TCP_PING = 0xe3e3,
	TCP_PONG = 0xe7e7,
	TCP_DISC = 0x1010
} TCP_STATUS;

LPCSTR GetHostIPAddr();

typedef enum { CHANNEL_STREAM, CHANNEL_PACKET }  CHANNEL_MODE;

class CTcpServer;
class CTcpChannel : public CThread
{
	DECLARE_DYNAMIC(CTcpChannel);
public:
	CTcpChannel();
	CTcpChannel(CTcpServer *server, CSocket *sock);
	~CTcpChannel();
	CSocket *GetSocket() { return m_pSock; }
	TCP_STATUS GetStatus() { return m_Status; }
	void SetStatus(TCP_STATUS Status) { m_Status = Status; }
	PDKINT GetType() { return m_chType; }
	void SetType(PDKINT nType) { m_chType = nType; }
	virtual int Send(void *Buffer, int nLen);
	CTcpServer *GetOwner() { return m_pServer; }
	LPSTR GetRemoteAddr() { return m_szRemoteAddr; }
	PDKINT GetRemotePort() { return m_nRPort; }
	void				*pOther;

protected:
	virtual void *ThreadProc();

	CTcpServer  	*m_pServer;
	CSocket			*m_pSock;
	TCP_STATUS		 m_Status;
	bool				 m_bFirst;
	PDKINT	 		 m_chType;
	char            m_szRemoteAddr[16];  
	UINT            m_nRPort;
	CLogger	 	   *m_pLogger;
};

typedef CTcpChannel *(*PConnecting)(CTcpServer *, CSocket *);
class CTcpServer : public CThread
{
	DECLARE_DYNAMIC(CTcpServer);
public:
	CTcpServer();
	~CTcpServer(); 
	void BroadCast(void *Buffer, int nLen);
	void SendAll(void *Buffer, int nLen);
	void CloseChannel(CTcpChannel *channel);
	void Start(LPCSTR pszAddr, PDKINT nPort);
	void SetCount(int nCount) { m_nLimit = nCount; }
	int GetCount() { return m_nLimit; }
	void SetUseEvent(bool bUseEvent) { m_bUseEvent = bUseEvent; }
	bool GetUseEvent() { return m_bUseEvent; }
	void SetOwner(void *pOwner) { m_pOwner = pOwner; }
	void *GetOwner() { return m_pOwner; }
	CObList *GetChannels() { return &m_pChannels; }

	PCallBack3		OnAfterReceive;
	PCallBack3		OnBeforeSend;
	PConnecting 	OnConnecting;
	PCallBack 		OnConnected;
	PCallBack 		OnClosed;
protected:
	void CheckChannels();
	virtual void *ThreadProc();

	CObList        m_pChannels;
	CTcpSocket	  *m_pSocket;
	CString			m_strAddr;
	int				m_nPort;
	int				m_nLimit;
	CCritSec 		m_Lock;
	bool				m_bUseEvent;
	void			  *m_pOwner;
	CLogger		  *m_pLogger;
};

#include <netinet/tcp.h>
inline void TcpNoDelay(int hSocket, bool bDelay = false)
{
	PDKINT nValue = bDelay;
	if (setsockopt(hSocket, IPPROTO_TCP,
			TCP_NODELAY, &nValue, sizeof(PDKINT)) != 0) {
		throw CException("TcpNoDelay Error[%d::%s]",
			errno, strerror(errno));
	}
}

inline void NonBlocking(int hSocket)
{
	int nFlags = fcntl(hSocket, F_GETFL, 0);
	nFlags |= O_NONBLOCK;
	if (fcntl(hSocket, F_SETFL, nFlags) < 0)
		perror("fcntl error : socket nonblocking io");
}
#endif /* __TCPSERVER_H__ */
