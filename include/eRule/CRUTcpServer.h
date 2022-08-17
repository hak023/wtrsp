
#ifndef ESIP_RUTCPSERVER_HXX
#define ESIP_RUTCPSERVER_HXX

#include "rutil/ThreadIf.hxx"
#include "rutil/ConfigParse.hxx"
#include "rutil/Data.hxx"

#include "CRuleUtiles.h"
#include "CRUSocket.h"

/****************************** Tcp Server **********************************************/
class RUTcpChannel;
typedef void (*PfuncChannelConnect_t)(RUTcpChannel *,const char *, int);
typedef void (*PfuncChannelDisconnect_t)(RUTcpChannel *,const char *, int);
class RUTcpServer : public resip::ThreadIf 
{
	friend class RUTcpChannel;
	public:
		RUTcpServer();
		virtual ~RUTcpServer();
		void fnCreate(const char * _pszSrvName,const char* _pszAddr, int _nPort, int _nMaxClient,
												unsigned int _unHdrSize,
												PfuncDecideBodyLen_t _pfnLen,
												PfuncPacketRcv_t _pfnRcv,
												PfuncChannelConnect_t _pfnCon,
												PfuncChannelDisconnect_t _pfnDisCon,
												bool _bNoneBlock = true);
		
		void fnStop();
		bool fnAvailableClient();
		void fnUpLinkServer(const char * _pszIP, int _nPort);
		void fnDownLinkServer();
		void fnLock(){m_clsLock.Lock();}
		void fnUnlock(){m_clsLock.Unlock();}
		char m_szName[128];
		int m_nSoc;
		char m_szIP[64];
		int m_nPort;
		RULock m_clsLock;
		int m_nMaxClient;
		int m_nSocBuffer;
		bool m_bNoneBlock;
		unsigned int m_unHdrSize;
		unsigned int m_unCurrent;
		RUTcpChannel * m_pclsCh;
		PfuncDecideBodyLen_t m_pfnLen;
		PfuncPacketRcv_t m_pfnRcv;
		PfuncChannelConnect_t m_pfnCon;
		PfuncChannelDisconnect_t m_pfnDisCon;
		bool m_bFinished;
		bool m_bLink;
	private:
		void fnInsertCh(int client,SOCKADDR_IN * pAddr);
		void fnStart(const char* _pszAddr, int _nPort,int _nMaxClient);
		bool fnCreateServerSocket(int nPort, char* pszAddr);
		void fnDecreaseCh(bool _bOwnerLock);
		void fnClearAllCh();
		void fnAddEvent(TcpEv *_pclsEv);
		bool fnGetEvent(TcpEv & _rclsEv);
		virtual void thread();
		RUList m_listCmd;
};
/***************************** Tcp Channel **********************************************/
class RUTcpChannel : public resip::ThreadIf 
{
	public:
		RUTcpChannel();
		~RUTcpChannel();
		bool fnIsValid();
		void fnStart(RUTcpServer * _pclsServer,unsigned int _unCnt,
											PfuncDecideBodyLen_t _pfnLen,PfuncPacketRcv_t _pfnRcv,
											bool _bNoneBlock = false);
		bool fnUpLinkChannel(int _nSoc,SOCKADDR_IN * _pstAddr);
		void fnDownLinkChannel();
		int fnSend(void *Buffer, int nLen);
		void fnLock(){m_clsLock.Lock();}
		void fnUnlock(){m_clsLock.Unlock();}
		void fnStop();
		RUTcpServer * fnGetOwner(){return m_pclsOwner;}
		char * m_pszOwnerName;
		int m_nSoc;
		bool m_bNoneBlock;
		unsigned int m_unIndex;
		char m_szName[128];
		char m_szRemoteIP[64];
		int m_nRemotePort;
		RUTcpServer * m_pclsOwner;
		bool m_bFinished;
		bool m_bConnected;
	private:
		bool fnDeleteChannel();
		void fnCloseChannel(bool _bOwnerLock);
		void fnAddEvent(TcpEv *_pclsEv);
		bool fnGetEvent(TcpEv & _rclsEv);
		void fnSetAddress(int _nSoc, const char * _pszIP,int _nPort);
		virtual void thread();
		RUTcpBuffer m_clsBuffer;
		RULock m_clsLock;
		RUList m_listCmd;
};
#endif


