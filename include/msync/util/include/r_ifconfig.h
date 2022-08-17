
#ifndef __RPDK_IFCONFIG_H__
#define __RPDK_IFCONFIG_H__

#include <string.h>
#include <errno.h>
#include <sys/socket.h>

#if defined(__LINUX__)
#	include <sys/ioctl.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <net/if.h>
#	include <netinet/ip.h>
//#	include <netinet/ip6.h>
#	include <linux/sockios.h>
#	include "ipv6.h"
#	include "net-support.h"
#	include <ifaddrs.h>
#elif defined(__SUNOS__)
#	include <sys/sockio.h>
#endif //

#include <net/if.h>
#include "r_base.h"
#include <string>
#include <list>

// add by paul 
#include <sys/socket.h>
#include <netdb.h>

#if 0//ndef _LINUX_IN6_H
/*
 *    This is in linux/include/net/ipv6.h.
 */

struct in6_ifreq {
    struct in6_addr ifr6_addr;
    __u32 ifr6_prefixlen;
    unsigned int ifr6_ifindex;
};

#endif

namespace RED{
class DLLPORT CIfConfig
{
public:
	CIfConfig();
	~CIfConfig();

	std::list<std::string> *GetInterface();
	short GetFlags(const char* pszifname);
	std::string GetAddr(const char* pszifname);
	std::string GetMask(const char* pszifname);
	std::string GetBroadCast(const char* pszifname);
	std::string GetHwAddr(const char* pszifname);
	bool IsUp(const char* pszifname);
   int IsUpAddr(const char* addr);
	std::string GetIfName(const char* addr);
	std::string GetIfName6(const char* addr);
	bool SetAddr(int ver,char* pszifname,char* ip,char* netmask,int mode);//mode 1 up 0 down
protected:
	void GetInterfaceList();
	std::string _getaddr(const char* pszifname, int request);
	int set_flag(char *ifname, short flag);
	int clr_flag(char *ifname, short flag);
	int set_netmask(struct ifreq *ifr, struct sockaddr *sa);
	int set_ip_using(const char *name, int c, unsigned long ip);

	int			m_nSock;
	int			m_nSock6;
	struct aftype m_inet6_aftype;
	struct sockaddr_in6 m_sa6;
	struct in6_ifreq m_ifr6;
   std::list<std::string> m_strList;
};

inline CIfConfig::CIfConfig() : m_nSock(-1)
{
	m_nSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_nSock == -1)
		throw CXException("socket create error[%d:%s]", errno, strerror(errno));
	m_nSock6 = socket(AF_INET6, SOCK_DGRAM, 0);
	if (m_nSock == -1)
		throw CXException("socket create error[%d:%s]", errno, strerror(errno));
	GetInterfaceList();
}

inline CIfConfig::~CIfConfig()
{
	if (m_nSock != -1)
		close(m_nSock);
	if (m_nSock6 != -1)
		close(m_nSock6);
}

inline bool CIfConfig::SetAddr(int ver,char* pszifname,char* ip,char* netmask,int mode)
{
   struct ifreq ifr;
	struct in6_addr addr6;
	if(ver==4)
	{
		if(ip && set_ip_using(pszifname,SIOCSIFADDR,inet_addr(ip))==-1)
			return 0;
		if(netmask && set_ip_using(pszifname,SIOCSIFNETMASK,inet_addr(netmask))==-1)
			return 0;
		if(mode==1)
		{
			if(set_flag(pszifname, IFF_UP | IFF_RUNNING)==-1) return 0;
		}
		else
		{
			if(clr_flag(pszifname, IFF_UP)==-1) return 0;
		}
	}
	else
	{
#if 0
		if(m_inet6_aftype.input(1,ip,(struct sockaddr*)&m_sa6)<0)
		{
			return 0;
		}
#endif
		strncpy(ifr.ifr_name,pszifname,IFNAMSIZ);
		inet_pton(AF_INET6,ip,&addr6);
		memcpy((char*)&m_ifr6.ifr6_addr,(char*)&addr6,sizeof(struct in6_addr));
		if(ioctl(m_nSock6,SIOGIFINDEX,&ifr) < 0)
		{
			return 0;
		}
		m_ifr6.ifr6_ifindex = ifr.ifr_ifindex;
		m_ifr6.ifr6_prefixlen = atoi(netmask);
		if(mode == 1)
		{
			if(ioctl(m_nSock6,SIOCSIFADDR,&m_ifr6) < 0)
			{
				return 0;
			}
		}
		else
		{
			if(ioctl(m_nSock6,SIOCDIFADDR,&m_ifr6) < 0)
			{
				return 0;
			}
		}
	}
	return 1;
}

inline int CIfConfig::set_flag(char *ifname, short flag)
{
    struct ifreq ifr;

    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(m_nSock, SIOCGIFFLAGS, &ifr) < 0) {
   fprintf(stderr, "%s: unknown interface: %s\n",
      ifname,  strerror(errno));
   return (-1);
    }
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    ifr.ifr_flags |= flag;
    if (ioctl(m_nSock, SIOCSIFFLAGS, &ifr) < 0) {
   	perror("SIOCSIFFLAGS");
   return -1;
    }
    return (0);
}

inline int CIfConfig::clr_flag(char *ifname, short flag)
{
    struct ifreq ifr;

    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(m_nSock, SIOCGIFFLAGS, &ifr) < 0) {
   fprintf(stderr, "%s: unknown interface: %s\n",
      ifname, strerror(errno));
   return -1;
    }
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
    ifr.ifr_flags &= ~flag;
    if (ioctl(m_nSock, SIOCSIFFLAGS, &ifr) < 0) {
   perror("SIOCSIFFLAGS");
   return -1;
    }
    return (0);
}
inline int CIfConfig::set_netmask(struct ifreq *ifr, struct sockaddr *sa)
{
    int err = 0;

    memcpy((char *) &ifr->ifr_netmask, (char *) sa,
      sizeof(struct sockaddr));
    if (ioctl(m_nSock, SIOCSIFNETMASK, ifr) < 0) {
   fprintf(stderr, "SIOCSIFNETMASK: %s\n",
      strerror(errno));
   err = 1;
    }
    return 0;
}

inline int CIfConfig::set_ip_using(const char *name, int c, unsigned long ip)
{
    struct ifreq ifr;
    struct sockaddr_in sin;

    strncpy(ifr.ifr_name, name, IFNAMSIZ);
    memset(&sin, 0, sizeof(struct sockaddr));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ip;
    memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
    if (ioctl(m_nSock, c, &ifr) < 0)
   return -1;
    return 0;
}

inline void CIfConfig::GetInterfaceList()
{
	struct ifconf ifc;
	char          buf[4096];

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;

	if (ioctl(m_nSock, SIOCGIFCONF, (char *)&ifc) < 0)
		throw CXException("ioctl SIOCGIFCONF [%d:%s]", errno, strerror(errno));

   m_strList.clear();
	struct ifreq *ifrp = ifc.ifc_req;
	for (int n = ifc.ifc_len / sizeof(struct ifreq); n > 0; n--, ifrp++)
      m_strList.push_back(std::string(ifrp->ifr_name));
}

inline std::string CIfConfig::GetHwAddr(const char* pszifname)
{
   struct ifreq ifr;
   strcpy(ifr.ifr_name, pszifname);

   if (ioctl(m_nSock, SIOCGIFHWADDR, &ifr) < 0)
#if 0
      throw CXException("ioctl [%d:%s]", errno, strerror(errno));
#else
      return std::string("None");
#endif
	char hwaddr[128];
	sprintf(hwaddr,"%02x:%02x:%02x:%02x:%02x:%02x",ifr.ifr_hwaddr.sa_data[0]&0xff,
		ifr.ifr_hwaddr.sa_data[1]&0xff,ifr.ifr_hwaddr.sa_data[2]&0xff,ifr.ifr_hwaddr.sa_data[3]&0xff,ifr.ifr_hwaddr.sa_data[4]&0xff,
		ifr.ifr_hwaddr.sa_data[5]&0xff);
	return std::string(hwaddr);
}

inline std::string CIfConfig::_getaddr(const char* pszifname, int request)
{
	struct ifreq ifr;
	strcpy(ifr.ifr_name, pszifname);
   std::string strResult("None");

	if (ioctl(m_nSock, request, (caddr_t)&ifr) < 0)
#if 0
		throw CXException("ioctl [%d:%s]", errno, strerror(errno));
#else
		return strResult;
#endif

	struct sockaddr_in *paddr;
	paddr = (struct sockaddr_in *)&(ifr.ifr_addr);
	strResult = inet_ntoa(paddr->sin_addr);
	return strResult;
}

inline std::list<std::string> *CIfConfig::GetInterface()
{
	return &m_strList;
}

inline short CIfConfig::GetFlags(const char* pszifname)
{
	struct ifreq ifr;
	strcpy(ifr.ifr_name, pszifname);

	if (ioctl(m_nSock, SIOCGIFFLAGS, (caddr_t)&ifr) < 0)
		throw CXException("ioctl SIOCGIFFLAGS [%d:%s]", errno, strerror(errno));
	return ifr.ifr_flags;
}

inline std::string CIfConfig::GetAddr(const char* pszifname)
{
	return _getaddr(pszifname, SIOCGIFADDR);
}

inline std::string CIfConfig::GetMask(const char* pszifname)
{
	return _getaddr(pszifname, SIOCGIFNETMASK);
}

inline std::string CIfConfig::GetBroadCast(const char* pszifname)
{
	return _getaddr(pszifname, SIOCGIFBRDADDR);
}

inline bool CIfConfig::IsUp(const char* pszifname)
{
	return (GetFlags(pszifname) & IFF_UP) != 0;
}

inline int CIfConfig::IsUpAddr(const char* addr)
{
   GetInterfaceList();
   std::list<std::string>::iterator itr;
   std::string strIfName;
   std::string strAddr;
   std::string strAddr2 = addr;
   for(itr = m_strList.begin(); itr != m_strList.end(); itr++)
   {
      strIfName = *itr;
      strAddr = GetAddr(strIfName.c_str());
      if( strAddr == "None" ){
         return -1;
      }
      if(strAddr == strAddr2)
         return (IsUp(strIfName.c_str())? 1 : 0); 
   }
   return 0;
}

inline std::string CIfConfig::GetIfName(const char* addr)
{
   GetInterfaceList();
   std::list<std::string>::iterator itr;
   std::string strIfName;
   std::string strAddr;
   std::string strAddr2 = addr;
   for(itr = m_strList.begin(); itr != m_strList.end(); itr++)
   {
      strIfName = *itr;
      strAddr = GetAddr(strIfName.c_str());
      if(strAddr == strAddr2)
		return strIfName;
   }
   return std::string("");
}

inline std::string CIfConfig::GetIfName6(const char* addr)
{
   std::string strIfName;
   struct ifaddrs *if_addr, *ifap;
   int family, res;
   char host_id[NI_MAXHOST], *host_addr;
   int ifcount;
   int ifindex_matched = -1;

   if (getifaddrs(&if_addr) == -1)
   {
      return std::string("");
   }

   ifcount = 0;
   for (ifap = if_addr; ifap != NULL; ifap = ifap->ifa_next)
   {
      if (ifap->ifa_addr == NULL)
      {
         ifcount++;
         continue;
      }

      family = ifap->ifa_addr->sa_family;
      if (family == AF_INET6)
      {
         res = getnameinfo(ifap->ifa_addr, sizeof(struct sockaddr_in6),
         host_id, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
         if (res != 0)
         { /* failure to get IPv6 address */
            continue;
         }
         host_addr = strtok(host_id, "%");
         if ((strncmp(host_id, addr, INET6_ADDRSTRLEN)) == 0)
         {
            strIfName = ifap->ifa_name;
            ifindex_matched = if_nametoindex(ifap->ifa_name);
            return strIfName;;
         }
         ifcount++;
      }
   }
   return std::string("");
}
};//namespace RED

#endif // __IFCONFIG_H__
