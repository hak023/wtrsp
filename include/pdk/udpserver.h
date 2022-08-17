#ifndef __UDPSERVER_H__
#define __UDPSERVER_H__

#include "qthread.h"
#include "logger.h"
#include "sock.h"
#include "inifile.h"

///////////////////////////////////////////////////////////////////////////////
class CUUdpSender : public CUnixUdp
{
	DECLARE_DYNAMIC(CUUdpSender);
public:
	CUUdpSender();
	~CUUdpSender();
	bool Open();
	int SendTo(void *pInfo, int nLen, LPCSTR pszDest);
	int SendTo(PEVENTINFO pInfo, LPCSTR pszDest);

protected:
	CLogger    		*m_pLogger;
};

///////////////////////////////////////////////////////////////////////////////
class CUUdpReceiver : public CThread
{
	DECLARE_DYNAMIC(CUUdpReceiver);
public:
	CUUdpReceiver();
	virtual ~CUUdpReceiver();
	virtual void Start(LPCSTR pszPath);
	void SetUseEvent(bool bUseEvent) { m_bUseEvent = bUseEvent; }
	bool GetUseEvent() { return m_bUseEvent; }
	void SetOwner(void *pOwner) { m_pOwner = pOwner; }
	void *GetOwner() { return m_pOwner; }
	PCallBack3			OnAfterReceive;

protected:
	void *ThreadProc();

	CUnixUdp			*m_pUdpRecvSock;
	bool           m_bUseEvent;
	void				*m_pOwner;
	CLogger    		*m_pLogger;
};

//#define DISTRIBUTE_SOCK
///////////////////////////////////////////////////////////////////////////////
class CUUdpServer : public CUUdpReceiver
{
	DECLARE_DYNAMIC(CUUdpServer);
public:
	CUUdpServer();
	~CUUdpServer();
	void Start(LPCSTR pszPath);
	int SendTo(PEVENTINFO pInfo, LPCSTR pszDest);
	int SendTo(void *pInfo, int nLen, LPCSTR pszDest);
	PCallBack3			OnBeforeSend;
protected:
#ifdef DISTRIBUTE_SOCK
	CUUdpSender		*m_pUdpSendSock;
#endif // DISTRIBUTE_SOCK
};

///////////////////////////////////////////////////////////////////////////////
class CIUdpSender : public CInetUdpSocket
{
	DECLARE_DYNAMIC(CIUdpSender);
public:
	CIUdpSender();
	~CIUdpSender();
	bool Open();
	int SendTo(void *pInfo, int nLen, LPCSTR pszAddr, int nPort);
	int SendTo(PEVENTINFO pInfo, LPCSTR pszAddr, int nPort);

protected:
	CLogger    		*m_pLogger;
};

class CIUdpReceiver : public CThread
{
	DECLARE_DYNAMIC(CIUdpReceiver);
public:
	CIUdpReceiver();
	virtual ~CIUdpReceiver();
	virtual bool Start(LPCSTR pszAddr, PDKINT nPort);
	LPCSTR GetAddr() { return (LPCSTR)m_strAddr; }
	LPCSTR GetRemoteAddr() { return (LPCSTR)m_szRemoteAddr; }
	int GetPort() { return m_nPort; }
	int GetRPort() { return m_nRPort; }
	void SetUseEvent(bool bUseEvent) { m_bUseEvent = bUseEvent; }
	bool GetUseEvent() { return m_bUseEvent; }
	void SetOwner(void *pOwner) { m_pOwner = pOwner; }
	void *GetOwner() { return m_pOwner; }
	PCallBack3			OnAfterReceive;

protected:
	void *ThreadProc();

	CInetUdpSocket	*m_pUdpRecvSock;
	CString			 m_strAddr;
	int				 m_nPort;
	char            m_szRemoteAddr[16];
	UINT				 m_nRPort;
	bool           m_bUseEvent;
	void				*m_pOwner;
	CLogger    		*m_pLogger;
};

class CIUdpServer : public CIUdpReceiver
{
	DECLARE_DYNAMIC(CIUdpServer);
public:
	CIUdpServer();
	~CIUdpServer();
	bool Start(LPCSTR pszAddr, PDKINT nPort);
	int SendTo(void *pBuffer, int nLen, LPCSTR pszAddr, int nPort);
	int SendTo(PEVENTINFO pInfo, LPCSTR pszAddr, PDKINT nPort);

	PCallBack3		OnBeforeSend;
protected:
#ifdef DISTRIBUTE_SOCK
	CIUdpSender		*m_pUdpSendSock;
#endif // DISTRIBUTE_SOCK
};

///////////////////////////////////////////////////////////////////////////////
#define CONF_RELOAD_MOUNT 0x01
#define CONF_DEBUG_LEVEL  0x02

class CUUDPIFThread : public CQThread
{
	DECLARE_DYNAMIC(CUUDPIFThread);
public:
	CUUDPIFThread();
	CUUDPIFThread(int n);
	virtual ~CUUDPIFThread();
	virtual void Start(LPCSTR pszPath);
	PDKINT SendTo(LPCSTR pszPath, void *Buffer, PDKINT &nLen);
	LPCSTR GetPath() { return (LPCSTR)m_strPath; }

protected:
	static int AfterReceive(void *ptr, void *pBuffer, void *pLen)
	{
		CUUdpReceiver *pUdpReceiver = (CUUdpReceiver *)ptr;
		if (pUdpReceiver->IsKindOf(RUNTIME_CLASS(CUUdpReceiver)) == false)
			return -1;
		CUUDPIFThread *self = (CUUDPIFThread *)pUdpReceiver->GetOwner();
		if (self == NULL)
			return -1;
		return self->RecvHandle(pBuffer, *(PDKINT *)pLen);
	}
	static int BeforeSend(void *ptr, void *pBuffer, void *pLen)
	{
		CUUdpReceiver *pUdpReceiver = (CUUdpReceiver *)ptr;
		if (pUdpReceiver->IsKindOf(RUNTIME_CLASS(CUUdpReceiver)) == false)
			return -1;
		CUUDPIFThread *self = (CUUDPIFThread *)pUdpReceiver->GetOwner();
		if (self == NULL)
			return -1;
		return self->SendHandle(pBuffer, *(PDKINT *)pLen);
	}
	virtual int RecvHandle(void *Buffer, PDKINT nLen) { return 0; }
	virtual int SendHandle(void *Buffer, PDKINT nLen) { return 0; }

	void *ThreadProc();

	CUUdpServer		*m_pUUdpChannel;
	CString			 m_strPath;
	CLogger   		*m_pLogger;
};

#endif /* __UDPSERVER_H__ */
