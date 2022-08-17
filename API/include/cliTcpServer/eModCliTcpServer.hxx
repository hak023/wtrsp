
#ifndef ESIP_MOD_CLI_TCP_SERVER_HXX
#define ESIP_MOD_CLI_TCP_SERVER_HXX

#include "rutil/ConfigParse.hxx"
#include "rutil/Data.hxx"

#include "eSipUtil/transportMgr.hxx"

#include "eModCliMsg.h"

namespace eSIP
{

class ModCliTcpServerCfgParse : public resip::ConfigParse
{
	public :
		enum
		{
			E_CONST_PRINT_LEN = 1000,
		
			E_CONST_DFT_TCP_SESSION_NUMBER = 1000,
			E_CONST_DFT_LISTEN_BACKLOG_SIZE = 1024,
			E_CONST_DFT_EPOLL_SIZE = 1024,
			E_CONST_DFT_INTERNAL_BUFFER_SIZE = 5000,
		};
		
		ModCliTcpServerCfgParse();
		~ModCliTcpServerCfgParse();

		virtual void printHelpText(int argc, char **argv){}
		const char * str(char * _pszPrint, unsigned int _unLen) const;
		bool load();

		inline int getTcpSessionNumber() const{ return m_nTcpSessionNumber; }
		inline int getListenBackLogSize() const{ return m_nListenBackLogSize; }
		inline int getEpollSize() const{ return m_nEpollSize; }
		inline const resip::Data & getLocalIp() const{ return m_clsLocalIp; }
		inline int getLocalPort() const{ return m_nLocalPort; }
		inline int getInternalBufferSize() const{ return m_nInternalBufferSize; }
		
	private :
		int m_nTcpSessionNumber;
		int m_nListenBackLogSize;
		int m_nEpollSize;
		resip::Data m_clsLocalIp;
		int m_nLocalPort;
		int m_nInternalBufferSize; //message * 1.5 size

};

class CliTcpServer
{
	public :
		CliTcpServer();
		~CliTcpServer();

		bool init();

		inline eSipUtil::TransportMgr * getTransportMgr(){ return m_pclsTransportMgr; }

	private :
		static void delTransportCb(eSipUtil::TransportMgr & _rclsTransportMgr, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, eSipUtil::ETransportErr_t _eTrspErr);
		static void acceptConnectionCb(eSipUtil::TransportMgr & _rclsTransportMgr, eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		static int processRcvMatchCb(eSipUtil::TransportMgr & _rclsTransportMgr, const char * _pRcvData, unsigned int _unRcvDataLen, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, void * _pAppBuffer);
		static void processRcvMsgCb(eSipUtil::TransportMgr & _rclsTransportMgr, char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstNet5Tuple);

		ModCliTcpServerCfgParse m_clsCfgParse;
		eSipUtil::TransportMgr * m_pclsTransportMgr;

};

}

#endif

