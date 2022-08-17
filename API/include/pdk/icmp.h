


#ifndef __SIMPLE_ICMP_H__
#define __SIMPLE_ICMP_H__

#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "sock.h"
#include "qthread.h"
#include "util.h"

//#define  MAXPACKET   (65536 - 60 - 8) /* max packet size     */
#define  MAXPACKET   (1024 * 10 - 60 - 8) /* max packet size     */
#define  DEFDATALEN  (64 - 8)  /* default data length */

#ifndef ENTRY_LENGTH
#	define ENTRY_LENGTH       64
#endif

#if 0
	#define PACKET_DEBUG
	#define LOG_STDERR
#endif

typedef enum {
	PROTO_RAW,
	PROTO_ICMP
} IPPROTO_TYPE;

class CRawSocket : public CSocket
{
public:
	bool Open(IPPROTO_TYPE nType = PROTO_ICMP) {
		m_bAvailable = false;
		m_nProtoType = nType;
		if (nType == PROTO_RAW) {
			if (CreateSocket(AF_INET, SOCK_RAW, IPPROTO_RAW) == false)
				throw CException("socket creation error:%d:%s",
										errno, strerror(errno));
#	ifdef IP_HDRINCL
			int one = 1;
			setsockopt(GetSocket(), IPPROTO_IP,
				IP_HDRINCL, (void *) &one, sizeof(one));
#	endif // IP_HDRINCL
		} else {
			struct protoent *proto;
			if ((proto = getprotobyname("icmp")) == NULL)
				throw CException("Unknown protocol: icmp. Exiting.");
			if (CreateSocket(AF_INET, SOCK_RAW, proto->p_proto) == false)
				throw CException("socket creation error:%d:%s",
										errno, strerror(errno));
		}
		m_bAvailable = true;
		return true;
	}
	bool Bind(LPCSTR pszAddr) {
		return CSocket::Bind(0, (LPSTR)pszAddr);
	}
	bool GetAvailable() { return m_bAvailable; }
	IPPROTO_TYPE GetProtoType() { return m_nProtoType; }
protected:
	bool		m_bAvailable;
	IPPROTO_TYPE m_nProtoType;
};

class CICMPManager : public CThread
{
public:
	CICMPManager(IPPROTO_TYPE nType = PROTO_RAW);
	~CICMPManager();
	CQThread *GetOwner() { return m_pOwner; }
	int SendICMP();
	bool Start(CQThread *pOwner, LPCSTR pszAddr, PDKINT priority, LPCSTR pszBind, PCallBack aNotify = NULL);
	bool IsActive() { return m_bActive; }
	void SetDelay(PDKINT msec) { m_nDelay = msec; }
	PDKINT GetDelay() { return m_nDelay; }
	PDKINT GetPriority() { return m_nPriority; }
	LPCSTR GetAddr() { return (LPCSTR)m_strAddr; }
	LPCSTR GetBind() { return (LPCSTR)m_strBind; }
	void ResetLink();
	IPPROTO_TYPE GetProtoType() { return m_nProtoType; }
	PCallBack		 OnSendNotify; // params : this, *m_pICMPs

protected:
	int CheckSum(PDK16U *addr, int nLen);
	void *ThreadProc();

	struct timeval m_TimeVal;
	CRawSocket	*m_pSock;
	CRawSocket	*m_pRSock;
	CQThread		*m_pOwner;
	bool			 m_bActive;
	IPPROTO_TYPE m_nProtoType;
	PDKINT		 m_nDelay;
	PDKINT		 m_nPriority;
	CString		 m_strAddr;
	CString		 m_strBind;
	CLogger		*m_pLogger;
};

inline CICMPManager::CICMPManager(IPPROTO_TYPE nType)
	: m_pSock(new CRawSocket),
	  m_pRSock(new CRawSocket),
	  m_pOwner(NULL),
	  m_bActive(false),
	  m_nProtoType(nType),
	  m_nDelay(1000),
	  m_pLogger(CLogger::GetInstance())
{
	OnSendNotify = NULL;
}

inline bool CICMPManager::Start(CQThread *pOwner, LPCSTR pszAddr, PDKINT priority, LPCSTR pszBind, PCallBack aNotify)
{
	m_pOwner = pOwner;
	m_strAddr= pszAddr;
	m_nPriority = priority;
	OnSendNotify = aNotify;

	try {
		m_pSock->Open(m_nProtoType);
		m_pRSock->Open();
		if (pszBind) {
			m_strBind = pszBind;
			if (m_pRSock->Bind(pszBind) == false) {
				m_pLogger->Log(__FILE__, __LINE__, CLogger::Error,
									"bind error(Addr:%s,Err:%d-%s)",
									pszBind, errno, strerror(errno));
				delete m_pSock;
				m_pRSock = NULL;
			}
		} else {
			/* this is BUG : 051019 modify. KMJ. */
			/* m_strBind = pszAddr; */
			GetHostIPAddr(m_strBind);
		}
	} catch (CException e) {
		m_pLogger->Log(CLogger::Critical,
							"Row[ICMP] Socket : %s",
							(LPCSTR)e.Cause());
		delete m_pSock;
		m_pSock = NULL;
		delete m_pRSock;
		m_pRSock = NULL;
	}
	if (!m_pSock) return false;
	bool bReturn = Create();
	if (bReturn) SendICMP();
	return bReturn;
}

inline CICMPManager::~CICMPManager()
{
#ifdef DEBUG_SELECT
	m_pRSock->Interrupt();
#endif // DEBUG_SELECT

	Close();

	if (m_pSock)
		delete m_pSock;
	if (m_pRSock)
		delete m_pRSock;
}

inline void CICMPManager::ResetLink()
{
	if (m_pRSock) {
		delete m_pRSock;
		m_pRSock = NULL;
	}
	m_pSock = new CRawSocket;
	m_pSock->Open(PROTO_RAW);
}

inline int CICMPManager::SendICMP()
{
	if (!m_pSock) return -1;

	if (OnSendNotify) {
		if (!OnSendNotify(this))
			return 1;
		else
			m_pLogger->Log(CLogger::Info,
						"[Priority %d] SendICMP",
							m_nPriority );
	}

	PDK8U Buffer[MAXPACKET];
	register int nLen = DEFDATALEN + 8;
	memset(Buffer, 0, MAXPACKET);

	struct ip *ip = NULL;
	struct icmp *pPacket = NULL;
	if (m_pSock->GetProtoType() == PROTO_RAW) {
		ip = (struct ip *)Buffer;
		pPacket = (struct icmp *)(ip + 1);
		ip->ip_v   = 4;
		ip->ip_hl  = 5;
		ip->ip_len = htons(sizeof(struct ip) + nLen);
#ifndef _LINUX_
		ip->ip_id  = htons(getpid());
#else // _LINUX_
		ip->ip_id  = htons(getppid());
#endif // _LINUX_
		ip->ip_ttl = 255;
		ip->ip_p   = IPPROTO_ICMP;
		ip->ip_src.s_addr = inet_addr((LPCSTR)m_strBind);
		ip->ip_dst.s_addr = inet_addr((LPCSTR)m_strAddr);
		ip->ip_sum =
			CheckSum((PDK16U *)ip, sizeof(struct ip));
	} else
		pPacket = (struct icmp *)Buffer;

	pPacket->icmp_type  = ICMP_ECHO;
	pPacket->icmp_code  = 0;
	pPacket->icmp_cksum = 0;
	pPacket->icmp_seq   = m_pSock->GetSocket();
#ifndef _LINUX_
	pPacket->icmp_id    = getpid();
#else // _LINUX_
	pPacket->icmp_id    = getppid();
#endif // _LINUX_
	gettimeofday((struct timeval *)pPacket->icmp_data, NULL);
	memcpy(&m_TimeVal, pPacket->icmp_data, sizeof(struct timeval));
	pPacket->icmp_cksum = CheckSum((PDK16U *)pPacket, nLen);

	int nSent;
	if (m_pSock->GetProtoType() == PROTO_RAW) {
	  	struct sockaddr_in sock;
		sock.sin_family = AF_INET;
		sock.sin_addr = ip->ip_dst;

		nLen = ntohs(ip->ip_len);
		int sd = m_pSock->GetSocket();
		nSent = sendto(sd, Buffer, nLen, 0,
							(struct sockaddr *)&sock, sizeof sock);
	} else
		nSent = m_pSock->SendTo((void *)Buffer, nLen,
											0, (LPSTR)m_strAddr);

#ifdef PACKET_DEBUG
	fprintf(stderr, "\ndest[%s], src[%s]\n",
		(LPCSTR)m_strAddr, (LPCSTR)m_strBind);
	for(int i = 0; i < nLen; i++)
		fprintf(stderr, "%02X%c", Buffer[i], ((i+1)%16) ? ' ': '\n');
#endif
	if (nSent != nLen)
		m_pLogger->Log(CLogger::Error,
							"Cannot send ICMP packet[%s] Sent=%d, nLen=%d",
							(LPCSTR)m_strAddr, nSent, nLen);
#if 0
	else
		m_pLogger->Log(CLogger::Event,
							"Send ICMP packet[%s] Sent=%d, nLen=%d",
							(LPCSTR)m_strAddr, nSent, nLen);
#endif

	return nSent;
}

inline int CICMPManager::CheckSum(PDK16U *addr, int nLen)
{
	register int nLeft = nLen;
	register PDK16U *w = addr;
	register int nSum  = 0;
	PDK16U answer      = 0;

	for (; nLeft > 1; nLeft -= 2)
		nSum += *w++;
	if (nLeft == 1) {
		*(PDK16U *)(&answer) = *(PDK8U *)w;
		nSum += answer;
	}

	nSum = (nSum >> 16) + (nSum & 0xffff);
	nSum += (nSum >> 16);
	answer = ~nSum;
	return answer;
}

inline void *CICMPManager::ThreadProc()
{
	int nResult, nLen;
	char Packet[MAXPACKET];
	struct timeval TimeVal;

	while (!DoExit()) {
		if ((nResult = m_pRSock->Select(m_nDelay)) == -1) {
			m_bActive = false;
			m_pLogger->Log(CLogger::Error,
								"ICMP Generator #%d, %s", errno, strerror(errno));
			break;
		}
		if (!nResult) {
			m_bActive = false;
			m_pLogger->Log(CLogger::Event,
								"[%s:%s] Timeout[%d]",
								(LPCSTR)m_strAddr, (LPCSTR)m_strBind, m_nDelay);
			continue;
		}
		if ((nLen = m_pRSock->Recv((char *)Packet, MAXPACKET)) < 1) {
			if (errno != EINTR)
				m_pLogger->Log(CLogger::Error,
									"Error reading ICMP data from %s",
									(LPCSTR)m_strAddr);
			break;
		}
		int nHeaderLen = ((struct ip *)Packet)->ip_hl << 2;
		struct icmp *icmp = (struct icmp *)(Packet + nHeaderLen);
		if (nLen < nHeaderLen + ICMP_MINLEN) {
			m_pLogger->Log(CLogger::Error,
								"Received short packet from %s", (LPCSTR)m_strAddr);
			break;
		}

		if (icmp->icmp_type == ICMP_ECHOREPLY &&
#ifndef _LINUX_
				icmp->icmp_id == getpid() &&
#else // _LINUX_
				icmp->icmp_id == getppid() &&
#endif // _LINUX_
				icmp->icmp_seq == m_pSock->GetSocket()) {
			gettimeofday(&TimeVal, NULL);
			m_bActive = true;

#ifdef PACKET_DEBUG
#	ifdef LOG_STDERR
			CTime_ tCur;
			fprintf(stderr,"[t%05ld] %s "
								"Received packet from %s[%d:%d:%d]\n",
								pthread_self(), (LPCSTR)tCur.Format("%T"),

#	else // LOG_STDERR
			m_pLogger->Log(CLogger::Event,
								"Received packet from %s[%d:%d:%d]",
#	endif // LOG_STDERR
								(LPCSTR)m_strAddr, icmp->icmp_type,
								icmp->icmp_id, icmp->icmp_seq);
#endif // PACKET_DEBUG
#ifdef PACKET_DEBUG
		} else {
#	ifdef LOG_STDERR
			CTime_ tCur;
			fprintf(stderr,"[t%05ld] %s [%d:%d:%d]"
								"UnInteresting Packet from %s[%d:%d:%d]\n",
								pthread_self(), (LPCSTR)tCur.Format("%T"),
								ICMP_ECHOREPLY, getppid(), m_pRSock->GetSocket(),
#	else // LOG_STDERR
			m_pLogger->Log(CLogger::Info,
								"UnInteresting Packet from %s[%d:%d:%d]",
#	endif // LOG_STDERR
								(LPCSTR)m_strAddr, icmp->icmp_type,
								icmp->icmp_id, icmp->icmp_seq);
#endif // PACKET_DEBUG
		}
		if (DIFFTIME(m_TimeVal, TimeVal) > m_nDelay)
			m_bActive = false;
	}
	return NULL;
}

class CICMPMonitor : public CQThread
{
public:
	CICMPMonitor();
	~CICMPMonitor();
	bool Start(LPCSTR pszAddrs, IPPROTO_TYPE nType = PROTO_RAW, PCallBack aNotify = NULL);
	bool AddManager(LPCSTR pszAddr, IPPROTO_TYPE nType = PROTO_RAW, PCallBack aNotify = NULL);
	bool RemoveManager(LPCSTR pszAddr);
	void ResetManager();
	void SetInterval(PDKINT msec) { m_nInterval = msec; }
	PDKINT GetInterval() { return m_nInterval; }
	void SetDelay(PDKINT msec);
	PDKINT GetDelay();
	CMapStringToPtr *GetICMPs() { return m_pICMPs; }
	bool StartTimer(int msec, PDK8U which = 0);
	bool StopTimer(PDK8U which = 0);
	static bool SendNotify(void *self);
	PCallBack2		 OnNotify; // params : this, *m_pICMPs
	PCallBack		 OnAliveNotify;

protected:
	void *ThreadProc();

	CMapStringToPtr *m_pICMPs;
	PDKINT			 m_nInterval;
	PDKINT			 m_nDelay;
	CLogger			 *m_pLogger;
};

inline CICMPMonitor::CICMPMonitor()
	: m_pICMPs(new CMapStringToPtr(8 * 2 - 1)),
	  m_nInterval(500),
	  m_nDelay(1000),
	  m_pLogger(CLogger::GetInstance())
{
	OnNotify = NULL;
	OnAliveNotify = NULL;
}

inline CICMPMonitor::~CICMPMonitor()
{
	OnNotify = NULL;
	OnAliveNotify = NULL;

	Stop();

	CString strKey;
	CICMPManager *pICMPManager;

	POSITION pos = m_pICMPs->GetStartPosition();
	while (pos) {
		m_pICMPs->GetNextAssoc(pos, strKey, (void *&)pICMPManager);
		delete pICMPManager;
	}
	delete m_pICMPs;
}

inline bool CICMPMonitor::AddManager(LPCSTR pszAddr, IPPROTO_TYPE nType, PCallBack aNotify)
{
	CArray <CString, CString> strIcmpInfo;
	Tokenize((LPSTR)pszAddr, strIcmpInfo, ":");
	PDKINT priority = strIcmpInfo.GetSize() == 1 ? 1 : atoi(strIcmpInfo[1]);
	char *pszBindAddr = NULL;
	if (strIcmpInfo.GetSize() == 3)
		pszBindAddr = (LPSTR)strIcmpInfo[2];

	bool bResult = false;
	CICMPManager *pICMPManager = new CICMPManager(nType);
	if (pICMPManager == NULL) {
		m_pLogger->Log(CLogger::Critical, "Memory Allocation Error");
		return bResult;
	}
	bResult = pICMPManager->Start(this, (LPCSTR)strIcmpInfo[0], priority, pszBindAddr, aNotify);
	m_pICMPs->SetAt(pszAddr, (void *&)pICMPManager);
	return bResult;
}

inline bool CICMPMonitor::RemoveManager(LPCSTR pszAddr)
{
	CICMPManager *pICMPManager = NULL;
	if (m_pICMPs->Lookup(pszAddr, (void *&)pICMPManager)) {
		m_pICMPs->RemoveKey(pszAddr);
		delete pICMPManager;
		m_pLogger->Log(CLogger::Info,
							"ICMPManager[%s] Removed", pszAddr);
		return true;
	} else {
		m_pLogger->Log(CLogger::Info,
							"ICMPManager[%s] Not Found", pszAddr);
		return false;
	}
}

inline void CICMPMonitor::ResetManager()
{
	CString strKey;
	CICMPManager *pICMPManager;
	POSITION pos = m_pICMPs->GetStartPosition();
	while (pos) {
		m_pICMPs->GetNextAssoc(pos, strKey, (void *&)pICMPManager);
		pICMPManager->ResetLink();
	}
}

inline bool CICMPMonitor::Start(LPCSTR pszAddrs, IPPROTO_TYPE nType, PCallBack aNotify)
{
#if 0 // setuid => 0
	if (getuid()) {
		m_pLogger->Log(CLogger::Critical, "Can only be used by \"root\"");
		return false;
	}
#endif
	CArray<CString,CString> strAddrArray;
	Tokenize((LPSTR)pszAddrs, strAddrArray, " ");
	for (int i = 0; i < strAddrArray.GetSize(); i++) {
#if 0
		CArray <CString, CString> strIcmpInfo;
		Tokenize((LPSTR)strAddrArray[i], strIcmpInfo, ":");
		PDKINT priority = strIcmpInfo.GetSize() == 1 ? 1 : atoi(strIcmpInfo[1]);
		char *pszBindAddr = NULL;
		if (strIcmpInfo.GetSize() == 3)
			pszBindAddr = (LPSTR)strIcmpInfo[2];

		CICMPManager *pICMPManager = new CICMPManager(nType);
		pICMPManager->Start(this, (LPCSTR)strIcmpInfo[0], priority, pszBindAddr, aNotify);
		m_pICMPs->SetAt((LPCSTR)strAddrArray[i], (void *&)pICMPManager);
#else
		AddManager((LPCSTR)strAddrArray[i], nType, aNotify);
#endif
#if 1
		m_pLogger->Log(CLogger::Info,
							"Host Addr[%s]", (LPCSTR)strAddrArray[i]);
#endif
	}
	return Create();
}

inline bool CICMPMonitor::StartTimer(int msec, PDK8U which)
{
	return ::StartTimer(this, msec, which);
}

inline bool CICMPMonitor::StopTimer(PDK8U which)
{
	return ::StopTimer(this, which);
}

inline void CICMPMonitor::SetDelay(PDKINT msec)
{
	m_nDelay = msec;
	CString strKey;
	CICMPManager *pICMPManager;

	POSITION pos = m_pICMPs->GetStartPosition();
	while (pos) {
		m_pICMPs->GetNextAssoc(pos, strKey, (void *&)pICMPManager);
		pICMPManager->SetDelay(msec);
	}
}

inline PDKINT CICMPMonitor::GetDelay()
{
	return m_nDelay;
}

inline void *CICMPMonitor::ThreadProc()
{
	CString strKey;
	CICMPManager *pICMPManager;

	StartTimer(m_nInterval);
	while (!DoExit()) {
		PEVENTINFO pEvent = Remove();
		assert(pEvent != NULL);

		if (EVENTINFO_TYPES(pEvent) == CI_TIMER_DONE) {
			if (OnNotify)
				OnNotify((void *)this, (void *)m_pICMPs);

//			m_pLogger->Log(CLogger::Info, "ICMP OnAliveNotify");
			if (OnAliveNotify)
				OnAliveNotify((void *)this);

			POSITION pos = m_pICMPs->GetStartPosition();
			while (pos) {
				m_pICMPs->GetNextAssoc(pos, strKey, (void *&)pICMPManager);
				pICMPManager->SendICMP();
			}

			StartTimer(m_nInterval);
		}
	}
	return NULL;
}

#endif // __SIMPLE_ICMP_H__
