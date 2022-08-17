
#ifndef ESIPUTIL_TRANSPORT_MGR_H
#define ESIPUTIL_TRANSPORT_MGR_H

#include <pthread.h>
#include <tr1/unordered_map>
#include <string>
#include <stdlib.h>

#include "transport.hxx"
#include "queueThread.hxx"
#include "lock.hxx"

namespace eSipUtil
{

struct TransportInternalMsg;
struct TransportData;
struct InternalSndInfo;

typedef void (* PfuncQueueFull)(const char * _pszQueueName, int _nCurrentRate, int _nLimitRate);

struct TransportMgrOption
{
	const char * m_pszMgrName;
	unsigned int m_unMaxTransportSize;
	unsigned int m_unMaxEpollSize;
	unsigned int m_unMaxThreadQueueSize;
	int m_nThreadQueueWaitMs;
	bool m_bUseSendThread;
	PfuncQueueFull m_pfnQueueFullCb;

	TransportMgrOption()
	{
		m_pszMgrName = "";
		m_unMaxTransportSize = 10;
		m_unMaxEpollSize = 1000;
		m_unMaxThreadQueueSize = 1000;
		m_nThreadQueueWaitMs = -1;
		m_bUseSendThread = false;
		m_pfnQueueFullCb = NULL;
	};
};

/*==========================================
NOTE : This manager is basically worked as socket asynchronous. It works wrong as synchronous.
When tcp with random local port(local port is 0), one transport(port 0) can be added only.
==========================================*/
class TransportMgr
{
	public :
		TransportMgr(const TransportMgrOption & _rstTrspMgrOpt, int _nCoreID=-1);
		~TransportMgr();
		
		inline const char * getMgrName() const{ return m_szName; }
		void resetData();
		void addTransport(const Net5Tuple_t & _rstNet5Tuple, const SocketCfg & _rstSocketCfg, const TransportConfig & _rstTransportCfg);
		void delTransportAll(ETransportErr_t _eTrspErr = E_TRSP_ERR_NONE);
		void delTransport(const Net5Tuple_t & _rstNet5Tuple, ETransportErr_t _eTrspErr = E_TRSP_ERR_NONE);
		//Caller must be malloc _pSndData and _pSndAppData. If result is false, caller invoking this function must free _pSndData and _pSndAppData
		//return : -1(fail), 0<=(send data length)
		int sendData(const Net5Tuple_t & _rstNet5Tuple, char * _pSndData, unsigned int _unSndLen, int _nDscp = -1, void * _pSndAppData = NULL, ETransportErr_t * _peTransportErr = NULL);
		int sendDataAsync(const Net5Tuple_t & _rstNet5Tuple, char * _pSndData, unsigned int _unSndLen, int _nDscp = -1, void * _pSndAppData = NULL, ETransportErr_t * _peTransportErr = NULL);
		bool isUseSndThread() const{ return m_bUseSendThread;}

		bool modifySocketCfg(const Net5Tuple_t & _rstNet5Tuple, const SocketCfg_t & _rstSocketCfg);
		bool modifySocketCfgBlock(const Net5Tuple_t & _rstNet5Tuple, bool _bBlock);
		bool modifySocketCfgReuse(const Net5Tuple_t & _rstNet5Tuple, bool _bReuse);
		bool modifySocketCfgRcvBufferSize(const Net5Tuple_t & _rstNet5Tuple, unsigned int _unBufferSize);
		bool modifySocketCfgSndBufferSize(const Net5Tuple_t & _rstNet5Tuple, unsigned int _unBufferSize);
		bool modifySocketCfgDscp(const Net5Tuple_t & _rstNet5Tuple, int _nDscp);

		void getQueueStatusStr(char * _pszOutStr, unsigned int _unMaxLen) const;

		inline void setAppData(void * _pData){ m_pAppData = _pData; }
		inline void * getAppData(){ return m_pAppData; }

	private :
		static bool checkValidTransportInternalMsg(const Transport & _rclsTransport, const TransportInternalMsg & _rclsTransportInternalMsg);

		bool put(bool _bRcvTh, TransportInternalMsg * _pstTrspInternalMsg);
		Transport * addTransportInternal(const Net5Tuple_t & _rstNet5Tuple, const SocketCfg & _rstSocketCfg, const TransportConfig & _rstTransportCfg, bool _bDelCb, int _nFd);
		void delTransportInternalAll(ETransportErr_t _eTransportErr);
		void delTransportInternal(const Net5Tuple_t & _rstNet5Tuple, int _nFd, ETransportErr_t _eTransportErr);
		void delTransportInternal(int _nTransportDataIdx, const Net5Tuple_t & _rstNet5Tuple, ETransportErr_t _eTransportErr);
		void acceptTransportInternal(int _nAcceptFd, PfuncAcceptConnection _pfnAcceptCb, const Net5Tuple_t & _rstNet5Tuple, const SocketCfg & _rstSocketCfg, const TransportConfig & _rstTransportCfg);
		void procRcvEpollEvent(int _nFd);
		void procSndEpollEvent(int _nFd);
		bool procAsyncSndData(const Net5Tuple_t & _rstNet5Tuple, const InternalSndInfo & _rstInternalSndInfo);
		
		/*==== Map ====*/
		bool addToMap(const char * _pszNet5TupleStr, int _nTransportDataIndex, int _nFd);
		void delFromMap(const char * _pszNet5TupleStr, int _nFd);
		int getTransportDataIndex(const char * _pszNet5TupleStr) const;
		int getTransportDataIndex(int _nFd) const;

		/*==== TransportData (Array) ====*/
		int getAvailableTransportDataIndex();
		bool addTransportData(Transport * _pclsTransport, int _nTransportDataIdx);
		void delTransportData(int _nTransportDataIdx);
		TransportData * getTransportData(int _nTransportDataIdx);
		const TransportData * getTransportData(int _nTransportDataIdx) const;

		/*==== callback of queueThread ====*/
		static void callbackEpollProcess(TransportMgr * _pclsTransportMgr);
		static void callbackRcvThreadProcess(TransportInternalMsg * _pstTransportInternalMsg, void * _pObject);
		static void callbackSndThreadProcess(TransportInternalMsg * _pstTransportInternalMsg, void * _pObject);
		
		typedef std::tr1::unordered_map< std::string, int > Map_Net5TupleInfo; 			//key(string of Net5Tuple), value(TransportData index)
		typedef std::tr1::unordered_map< int, int > Map_FdInfo; 									//key(Fd), value(TransportData index)

		char m_szName[20];
		bool m_bActive;
		bool m_bUseSendThread;
		unsigned int m_unMaxTransportSize;

		//map info		
		mutable RwMutex m_MapMutex;
		Map_Net5TupleInfo m_mapNet5TupleInfo;
		Map_FdInfo m_mapFdInfo;

		//array data
		TransportData * m_arrTransportData;
		
		Epoll m_clsEpoll;

		NormalThread< TransportMgr > * m_pclsEpollThread;
		QueueThread< TransportInternalMsg > * m_pclsSndThread;
		QueueThread< TransportInternalMsg > * m_pclsRcvThread;

		PfuncQueueFull m_pfnQueueFullCb;

		void * m_pAppData;

		friend class TcpTransport;
};

}

#endif

