
#ifndef __IFCONFIG_H__
#define __IFCONFIG_H__

#include <string.h>
#include <errno.h>
#include <sys/socket.h>

#if defined(_LINUX_)
#	include <sys/ioctl.h>
#	include <linux/sockios.h>
#elif defined(_SUNOS_)
#	include <sys/sockio.h>
#endif //

#include <net/if.h>
#include "base.h"

class CIfConfig
{
public:
	CIfConfig();
	~CIfConfig();

	CStringList *GetInterface();
	short GetFlags(LPCSTR pszifname);
	LPCSTR GetAddr(LPCSTR pszifname);
	LPCSTR GetMask(LPCSTR pszifname);
	LPCSTR GetBroadCast(LPCSTR pszifname);
	bool IsUp(LPCSTR pszifname);

protected:
	void GetInterfaceList();
	LPCSTR _getaddr(LPCSTR pszifname, int request);

	int			m_nSock;
	CStringList	m_strList;
};

inline CIfConfig::CIfConfig() : m_nSock(-1)
{
	m_nSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_nSock == -1)
		throw CException("socket create error[%d:%s]", errno, strerror(errno));
	GetInterfaceList();
}

inline CIfConfig::~CIfConfig()
{
	if (m_nSock != -1)
		close(m_nSock);
}

inline void CIfConfig::GetInterfaceList()
{
	struct ifconf ifc;
	char          buf[4096];

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;

	if (ioctl(m_nSock, SIOCGIFCONF, (char *)&ifc) < 0)
		throw CException("ioctl SIOCGIFCONF [%d:%s]", errno, strerror(errno));

	m_strList.RemoveAll();
	struct ifreq *ifrp = ifc.ifc_req;
	for (int n = ifc.ifc_len / sizeof(struct ifreq); n > 0; n--, ifrp++)
		m_strList.AddTail(ifrp->ifr_name);
}

inline LPCSTR CIfConfig::_getaddr(LPCSTR pszifname, int request)
{
	struct ifreq ifr;
	strcpy(ifr.ifr_name, pszifname);
	CString strResult("None");

	if (ioctl(m_nSock, request, (caddr_t)&ifr) < 0)
#if 1
		throw CException("ioctl [%d:%s]", errno, strerror(errno));
#else
		return (LPCSTR)strResult;
#endif

	struct sockaddr_in *paddr;
	paddr = (struct sockaddr_in *)&(ifr.ifr_addr);
	strResult = inet_ntoa(paddr->sin_addr);
	return (LPCSTR)strResult;
}

inline CStringList *CIfConfig::GetInterface()
{
	return &m_strList;
}

inline short CIfConfig::GetFlags(LPCSTR pszifname)
{
	struct ifreq ifr;
	strcpy(ifr.ifr_name, pszifname);

	if (ioctl(m_nSock, SIOCGIFFLAGS, (caddr_t)&ifr) < 0)
		throw CException("ioctl SIOCGIFFLAGS [%d:%s]", errno, strerror(errno));
	return ifr.ifr_flags;
}

inline LPCSTR CIfConfig::GetAddr(LPCSTR pszifname)
{
	return _getaddr(pszifname, SIOCGIFADDR);
}

inline LPCSTR CIfConfig::GetMask(LPCSTR pszifname)
{
	return _getaddr(pszifname, SIOCGIFNETMASK);
}

inline LPCSTR CIfConfig::GetBroadCast(LPCSTR pszifname)
{
	return _getaddr(pszifname, SIOCGIFBRDADDR);
}

inline bool CIfConfig::IsUp(LPCSTR pszifname)
{
	return (GetFlags(pszifname) & IFF_UP) != 0;
}


#endif // __IFCONFIG_H__
