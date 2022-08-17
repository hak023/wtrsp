#ifndef RUTCPCLIENT_H
#define RUTCPCLIENT_H

#include "rutil/ThreadIf.hxx"
#include "rutil/ConfigParse.hxx"
#include "rutil/Data.hxx"

#include "CRuleUtiles.h"
#include "CRUSocket.h"

/******************************* Tcp Client **********************************************/
typedef void (*PfuncClientConnect_t)(const char *, int);
typedef void (*PfuncClientDisconnect_t)(const char *, int);
class RUTcpClient : public resip::ThreadIf 
{
	public:
		RUTcpClient();
		~RUTcpClient();
		void fnStart(const char  * _pszOwnerName,unsigned int _unHdrSize,
															PfuncDecideBodyLen_t _pfnLen,
															PfuncPacketRcv_t _pfnRcv,
															PfuncClientConnect_t _pfnCon,
															PfuncClientDisconnect_t _pfnDisCon,
															bool _bNoneBlock = false);
		void fnAddConnection(int _nSocBuff, const char * _pszLocalIP, int _nLocalPort,
													const char * _pszRemoteIP, int _nRemotePort);
		void fnDeleteConnection();
		int fnSend(void *Buffer, int nLen);
		void fnLock(){m_clsLock.Lock();}
		void fnUnlock(){m_clsLock.Unlock();}
		bool m_bNoneBlock;
		char m_szOwnerName[128];
		int m_nSoc;
		char m_szLocalIP[64];
		int m_nLocalPort;
		char m_szRemoteIP[64];
		int m_nRemotePort;
		int m_nSocBuff;
		PfuncClientConnect_t m_pfnCon;
		PfuncClientDisconnect_t m_pfnDisCon;
		bool m_bFinished;
	private:
		void fnCloseClient();
		void fnSetAddress(const char * _pszLocalIP, int _nLocalPort, 
												const char * _pszRemoteIP, int _nRemotePort);
		bool fnCreateSocket();
		void fnAddEvent(TcpEv *_pclsEv);
		bool fnGetEvent(TcpEv & _rclsEv);
		void fnStop();
		virtual void thread();
		RULock m_clsLock;
		RUTcpBuffer m_clsBuffer;
		RUList m_listCmd;
};
#endif
