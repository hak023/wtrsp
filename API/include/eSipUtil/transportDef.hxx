
#ifndef ESIPUTIL_TRANSPORT_DEF_H
#define ESIPUTIL_TRANSPORT_DEF_H

#include "define.hxx"
#include "string.hxx"
#include "transportErr.hxx"

namespace eSipUtil
{

class TransportMgr;

/*=========================CallBack===================================*/

//_pData must be free() by callback. It is invoked after receiving complete message.
typedef void (* PfuncRecvProc)(TransportMgr & _rclsTransportMgr, char * _pData, unsigned int _unDataLen, const Net5Tuple_t & _rstNet5Tuple);

//_pData/_pAppData must not be free() by callback. It is invoked before send complete message
typedef void (* PfuncSndPrevProc)(TransportMgr & _rclsTransportMgr, const char * _pData, unsigned int _unDataLen, const Net5Tuple_t & _rstNet5Tuple, void * _pSndAppData);
typedef void (* PfuncSndAfterProc)(TransportMgr & _rclsTransportMgr, const char * _pData, unsigned int _unDataLen, const Net5Tuple_t & _rstNet5Tuple, void * _pSndAppData);

//return : length of complete message. The value of return must be determined until recvBuffer becomes full.
//-1(error), 0(unknown length), else(length of complete message)
typedef int (* PfuncTcpRecvMatch)(TransportMgr & _rclsTransportMgr, const char * _pRcvData, unsigned int _unRcvDataLen, const Net5Tuple_t & _rstNet5Tuple, void * _pAppBuffer);

//_pAppBuffer is set as buffer of application when execute callback of PfuncTcpRecvMatch.
//This function is excuted to notify resetting tcp buffer
typedef void (* PfuncResetTcpRcvBuffer)(TransportMgr & _rclsTransportMgr, void * _pAppBuffer, const Net5Tuple_t & _rstNet5Tuple);

typedef void (* PfuncDelTransport)(TransportMgr & _rclsTransportMgr, const Net5Tuple_t & _rstNet5Tuple, ETransportErr_t _eTrspErr);
typedef void (* PfuncAcceptConnection)(TransportMgr & _rclsTransportMgr, Net5Tuple_t & _rstNet5Tuple);
typedef void (* PfuncConnected4NonBlock)(TransportMgr & _rclsTransportMgr, const Net5Tuple_t & _rstNet5Tuple);

/*=================================================================*/


/*=========================Config===================================*/

struct TcpConfig
{
	int m_nTcpRole;														//0(server), 1(client), 2(accepted)
	unsigned int m_unSndBufferSize;
	unsigned int m_unBacklogSize;
	unsigned int m_unAppBufferSize;							//It is used to set application information of connection
	unsigned int m_unMinRcvNotiSize;							//It is minimum length of received data to be used to notify to application

	PfuncTcpRecvMatch m_pfnTcpRcvMatchCb;
	PfuncResetTcpRcvBuffer m_pfnResetTcpRcvBuffer;
	PfuncAcceptConnection m_pfnAcceptConnectionCb;
	PfuncConnected4NonBlock m_pfnConnected4NonblockCb;		//This function is invoked within the lock of transportData

	enum
	{
		E_CONST_MAX_STR_LEN = 128,
		E_CONST_DFT_SND_BUFFER_SIZE = 10000,
		E_CONST_DFT_BACKLOG_SIZE = 1000,
		E_CONST_DFT_AUTO_CONNECT_CNT = 5,
		E_CONST_DFT_MINIMUM_RCV_NOTI_SIZE = 50,
	};
	
	TcpConfig()
	{
		m_nTcpRole = 0;
		m_unSndBufferSize = E_CONST_DFT_SND_BUFFER_SIZE;
		m_unBacklogSize = E_CONST_DFT_BACKLOG_SIZE;
		m_unAppBufferSize = 0;
		m_unMinRcvNotiSize = E_CONST_DFT_MINIMUM_RCV_NOTI_SIZE;
		
		m_pfnTcpRcvMatchCb = NULL;
		m_pfnResetTcpRcvBuffer = NULL;		
		m_pfnAcceptConnectionCb = NULL;
		m_pfnConnected4NonblockCb = NULL;
	}

	const char * getStr(char * _pszOutStr, unsigned int _unMaxStrLen, bool _bInitial = true) const
	{
		if( !_pszOutStr || _unMaxStrLen == 0 )
			return getBlankStr();

		if( _bInitial )
			_pszOutStr[0] = 0x00;

		strncat(_pszOutStr, _unMaxStrLen-1, "TcpRole[%d], SndBufferSize[%u], BacklogSize[%u], AppBufferSize[%u], MinRcvNotiSize[%u], TcpRcvMatchCb[%p], ResetTcpBufferCb[%p], AcceptConCb[%p], Connected4NonblockCb[%p]",
																		m_nTcpRole, m_unSndBufferSize, m_unBacklogSize, m_unAppBufferSize, m_unMinRcvNotiSize, m_pfnTcpRcvMatchCb, m_pfnResetTcpRcvBuffer, m_pfnAcceptConnectionCb, m_pfnConnected4NonblockCb);

		return _pszOutStr;
	}
};
typedef struct TcpConfig TcpConfig_t;

struct TransportConfig
{
	TcpConfig m_stTcpConfig;			

	unsigned int m_unRcvBufferSize;
	PfuncRecvProc m_pfnRcvProcCb;
	PfuncSndPrevProc m_pfnSndPrevProcCb;		//This function is invoked within the lock of transportData
	PfuncSndAfterProc m_pfnSndAfterProcCb;		//This function is invoked within the lock of transportData. If sending is success, it is invoked.
	PfuncDelTransport m_pfnDelTransportCb;

	enum
	{
		E_CONST_MAX_STR_LEN = 256,
		E_CONST_DFT_RCV_BUFFER_SIZE = 10000,
	};

	TransportConfig()
	{
		m_unRcvBufferSize = E_CONST_DFT_RCV_BUFFER_SIZE;	
		m_pfnRcvProcCb = NULL;
		m_pfnSndPrevProcCb = NULL;
		m_pfnSndAfterProcCb = NULL;
		m_pfnDelTransportCb = NULL;
	}

	const char * getStr(char * _pszOutStr, unsigned int _unMaxStrLen, bool _bInitial = true) const
	{
		if( !_pszOutStr || _unMaxStrLen == 0 )
			return getBlankStr();

		if( _bInitial )
			_pszOutStr[0] = 0x00;

		m_stTcpConfig.getStr(_pszOutStr, _unMaxStrLen, _bInitial);
		strncat(_pszOutStr, _unMaxStrLen-1, ", RcvBufferSize[%u], RcvProcCb[%p], SndPrevProcCb[%p], SndAfterProcCb[%p], DelTransportCb[%p]"
																		, m_unRcvBufferSize, m_pfnRcvProcCb, m_pfnSndPrevProcCb, m_pfnSndAfterProcCb, m_pfnDelTransportCb);

		return _pszOutStr;
	}
};
typedef struct TransportConfig TransportConfig_t;

/*=================================================================*/

}

#endif
