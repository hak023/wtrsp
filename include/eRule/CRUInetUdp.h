#ifndef _RU_INET_UDP_H_
#define _RU_INET_UDP_H_

#include <netinet/in.h>

#include "CRUSocket.h"

typedef struct sockaddr          SOCKADDR;
#ifndef MIN
#  define MIN(a, b)       ((a) > (b) ? (b) : (a))
#endif // MIN

#define DEF_MAX_UDP_PACKET 1024*60

class CRUInetUdp : public resip::ThreadIf
{

	public:
		CRUInetUdp();
		~CRUInetUdp();
		int   fnStart(const char * _pszName,const char *_pszMyIp, int _nMyPort, 
													const char *_pszRemoteIp, int _nRemotePort,
													unsigned int _unSocBuff);
		void  fnStop();
		int   fnSend(void * _pvMsg, int _nLen);
		RULock m_Lock;
	protected:
		virtual void fnRecv(char * _pszRcvData,int _nRcvLen,char * _pszRecvIp, int _nRecvPort);
		char m_szMyIp[32];
		char m_szRemoteIp[32];
		char m_szDesc[32];
	private:
		void thread();
		void  fnSetServerAddr(const char *pszIp, int nPort, sockaddr_in *pAddr);
		int          m_Sock;
		int          m_nMyPort;
		int          m_nRemotePort;
		sockaddr_in  m_MyAddr;
		sockaddr_in  m_RemoteAddr;
		unsigned int m_unSocBuff;
};
class CRUUnixUdp : public resip::ThreadIf
{
	public:
		CRUUnixUdp();
		~CRUUnixUdp();
		bool fnStart(const char * _pszDesc,const char* _pszLocalPath,
														const char * _pszRemotePath,int _nSocBuff);
		void fnStop();
		int fnSend(const void* _pvBuf, int _nBufLen);
		RULock m_clsLock;
		RUString m_clsName;
	protected:
		virtual void fnRecv(char * _pszRecvData,int _nRecvLen);
	private:
		bool fnOpen(const char* _pszLocalPath,const char * _pszRemotePath,int _nSocBuff);
		void thread();
		bool fnCreateSocket(int _nAddrFormat, int _nSocketType, int _nProtocolType);
		bool fnBind(const char* _pszLocalPath);
		void fnClose();
		int fnRecvFrom(char *pBuf, int buflen, sockaddr_un *pFrom);
		int fnSelect(int timeout);
		int m_nSoc;
		RUString m_clsLocalPath;
		RUString m_clsRemotePath;
};


#endif
