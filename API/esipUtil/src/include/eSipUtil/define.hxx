
#ifndef ESIPUTIL_DEFINE_H
#define ESIPUTIL_DEFINE_H

#include <string.h>

#include "string.hxx"

namespace eSipUtil
{

const char * getBlankStr();
bool isIpv6(const char * _pszIpAddr);

typedef enum
{
	E_TRANSPORT_TYPE_udp = 0,
	E_TRANSPORT_TYPE_tcp,
	E_TRANSPORT_TYPE_tls,

	E_TRANSPORT_TYPE_NUM,
}ETransportType_t;
const char * getTransportTypeStr(ETransportType_t _eTransportType);

struct Net5Tuple
{
	enum
	{
		E_CONST_MAX_IPADDR_LEN = 50,
	};
	
	char m_szLocalIp[E_CONST_MAX_IPADDR_LEN];
	int m_nLocalPort;
	int m_nClientRealLocalPort;
	char m_szRemoteIp[E_CONST_MAX_IPADDR_LEN];
	int m_nRemotePort;
	ETransportType_t m_eTransportType;

	int m_nAppKey;	//default : -1

	Net5Tuple()
	{
		reset();
	}

	enum
	{
		E_CONST_MAX_STR_LEN =  128,
	};
	void m_fnSet(const char * _pszType, const char * _pszLIP, int _nLPort, const char * _pszRIP,int _nRPort)
	{
		if(_pszType)
		{
			if(strcasecmp(_pszType,"tcp")==0) m_eTransportType = E_TRANSPORT_TYPE_tcp;
			else if(strcasecmp(_pszType,"udp")==0) m_eTransportType = E_TRANSPORT_TYPE_udp;
			else if(strcasecmp(_pszType,"tls")==0) m_eTransportType = E_TRANSPORT_TYPE_tls;
		}
		m_fnSetLocalIp(_pszLIP);m_fnSetLocalPort(_nLPort);
		m_fnSetRemoteIp(_pszRIP);m_fnSetRemotePort(_nRPort);
	}
	inline void m_fnSetLocalIp(const char * _pszIpAddr)
	{
		if( _pszIpAddr )
		{
			strncpy(m_szLocalIp, _pszIpAddr, E_CONST_MAX_IPADDR_LEN-1);
			m_szLocalIp[E_CONST_MAX_IPADDR_LEN-1] = 0x00;
		}
	}
	inline void m_fnSetLocalPort(int _nPort){ m_nLocalPort = _nPort; }
	inline void m_fnSetClientRealLocalPort(int _nPort){ m_nClientRealLocalPort = _nPort; }
	inline void m_fnSetRemoteIp(const char * _pszIpAddr)
	{
		if( _pszIpAddr )
		{
			strncpy(m_szRemoteIp, _pszIpAddr, E_CONST_MAX_IPADDR_LEN-1);
			m_szRemoteIp[E_CONST_MAX_IPADDR_LEN-1] = 0x00;
		}
	}
	inline void m_fnSetRemotePort(int _nPort){ m_nRemotePort = _nPort; }
	inline void m_fnSetTransportType(ETransportType_t _eTransportType){ m_eTransportType = _eTransportType; }
	inline void m_fnSetAppKey(int _nAppKey){ m_nAppKey = _nAppKey; }
	
	void reset();
	bool operator==(const Net5Tuple & _rstNet5Tuple) const;
	bool checkSame4Transport(const Net5Tuple & _rstNet5Tuple) const;
	const char * getKeyStr(char * _pszOutStr, unsigned int _unMaxStrLen, bool _bInitial = true) const;
	const char * getStr(char * _pszOutStr, unsigned int _unMaxStrLen, bool _bInitial = true) const;
	const char * getStrDir(char * _pszOutStr, unsigned int _unMaxStrLen, bool _bRcv, bool _bInitial = true) const;
};
typedef struct Net5Tuple Net5Tuple_t;

}

#endif

