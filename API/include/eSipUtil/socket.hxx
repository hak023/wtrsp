
#ifndef ESIPUTIL_SOCKET_H
#define ESIPUTIL_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>

#include "define.hxx"
#include "string.hxx"

namespace eSipUtil
{

struct SocketCfg
{
	int m_nBlock;						//-1(unused), 0(non-block), 1(block)
	int m_nUseLinger;				//-1(unused), 0(non-linger), 1(use linger)
	int m_nLingerWaitTime;		//second. It is used only when m_bUseLinger is 1
	int m_nReuse;						//-1(unused), 0(non-reuse), 1(reuse). If m_bReuse is true, multiple processes can bind same port when UDP
	int m_nRcvSocketBufferSize;		//0(unused)
	int m_nSndSocketBufferSize;		//0(unused)
	int m_nDscp;						//-1(unused)

	enum
	{
		E_CONST_MAX_STR_LEN = 256,
	};

	void disable()
	{
		m_nBlock = -1;
		m_nUseLinger = -1;
		m_nReuse = -1;
		m_nRcvSocketBufferSize = 0;
		m_nSndSocketBufferSize = 0;
		m_nDscp = -1;
	}

	void reset()
	{
		m_nBlock = 0;
		m_nUseLinger = 0;
		m_nLingerWaitTime = 0;
		m_nReuse = 0;
		m_nRcvSocketBufferSize = 0;
		m_nSndSocketBufferSize = 0;
		m_nDscp = -1;
	}

	SocketCfg()
	{
		reset();
	}

	const char * getStr(char * _pszOutStr, unsigned int _unMaxStrLen, bool _bInitial = true) const
	{
		if( !_pszOutStr || _unMaxStrLen == 0 )
			return getBlankStr();

		if( _bInitial )
			_pszOutStr[0] = 0x00;
			
		strncat(_pszOutStr, _unMaxStrLen-1, "SocketCfg(");
		
		if( m_nBlock == 0 )
			strncat(_pszOutStr, _unMaxStrLen-1, "Block:non-block, ");
		else if( m_nBlock == 1 )
			strncat(_pszOutStr, _unMaxStrLen-1, "Block:block, ");

		strncat(_pszOutStr, _unMaxStrLen-1, "Linger:%d/%d, ", m_nUseLinger, m_nLingerWaitTime);

		if( m_nReuse == 0 )
			strncat(_pszOutStr, _unMaxStrLen-1, "ReUse:non-Reuse, ");
		else if( m_nReuse == 1 )
			strncat(_pszOutStr, _unMaxStrLen-1, "ReUse:Reuse, ");

		if( m_nRcvSocketBufferSize > 0 )
			strncat(_pszOutStr, _unMaxStrLen-1, "RcvSocketBufferSize:%d, ", m_nRcvSocketBufferSize);

		if( m_nSndSocketBufferSize > 0 )
			strncat(_pszOutStr, _unMaxStrLen-1, "SndSocketBufferSize:%d, ", m_nSndSocketBufferSize);

		if( m_nDscp >= 0 )
			strncat(_pszOutStr, _unMaxStrLen-1, "Dscp:%d, ", m_nDscp);
		
		strncat(_pszOutStr, _unMaxStrLen-1, ")");

		return _pszOutStr;
	}
};
typedef struct SocketCfg SocketCfg_t;

class Socket
{
	public:
		static int createSocket(int _nIpVersion, ETransportType_t _eTransportType, const SocketCfg & _rstSocketCfg);
		static int bindSocket(int _nFd, const char * _pszLocalIpAddr, int _nLocalPort, int _nIpVersion);
		static int connectSocket(int _nFd, const char * _pszRemoteIp, int _nRemotePort, int _nIpVersion);
		static int sendSocket(int _nFd, const void * _pBuffer, unsigned int _unSndLen, bool & _bWouldBlock);
		static int recvSocket(int _nFd, const void * _pBuffer, unsigned int _unMaxBuffLen, bool & _bWouldBlock);
		static int sendToSocket(int _nFd, const void * _pBuffer, unsigned int _unSndLen, int _nFlag, const char * _pszDstIp, int _nDstPort);
		static int recvFromSocket(int _nFd, void * _pBuffer, unsigned int _unMaxBuffLen, int _nFlag, int _nIpVersion, char * _pszSrcIp, int _nSrcIpLen, int * _pnSrcPort);
		static void getInfNameByIp(const char * _pszIpAddress, char * _pszInfName);
		static void closeSocket(int _nFd);
		static void setSocketCfg(int _nFd, const SocketCfg & _rstSocketCfg);
		static void setDscp(int _nFd, int _nDscp);
		static void setScopeIdForIpv6(const char * _pszLocalIpAddr, sockaddr_in6 & _stSockAddr6);
		static bool isIpAddress(const char * _pszIpAddress);
		static bool m_fnIsActiveVIP(const char* _pszVIp);
		static int createUnixUdpSocket(const SocketCfg & _rstSocketCfg);
		static int bindUnixUdpSocket(int _nFd, const char * _pszLocalPath);
		static int sendToUnixUdpSocket(int _nFd, const void * _pBuffer, unsigned int _unSndLen, int _nFlag, const char * _pszRemotePath);
		static int recvFromUnixUdpSocket(int _nFd, void * _pBuffer, unsigned int _unMaxBuffLen, int _nFlag, char * _pszRemotePath, unsigned int _unMaxRemotePathLen);
		static short m_fnGetFlags(int _nFd,const char * pszifname);
		static const char * m_fnGetAddr(int _nFd,const char * pszifname,char *strDst);
		static const char * m_fnGetMask(int _nFd,const char * pszifname,char *strDst);
		static const char * m_fnGetBroadCast(int _nFd,const char * pszifname,char *strDst);
		static bool m_fnIsUp(int _nFd,const char * pszifname);
		static int m_fnSetMtu(const char * szIpAddress,int s_set);

};

}

#endif

