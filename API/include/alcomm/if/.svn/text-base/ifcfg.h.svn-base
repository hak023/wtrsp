#ifndef __PI_NWM_IFCONFIG_H__
#define __PI_NWM_IFCONFIG_H__

#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <arpa/inet.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if_ether.h>

#include "aloxUtil.h"

/////////////
class CIfCfg
    : private CUncopyable
{
private:
   //define internal buffer
   enum { e_MAXNUM_DEV = 20 };
   struct in6_ifreq {
       struct in6_addr ifr6_addr;
       __u32 ifr6_prefixlen;
       unsigned int ifr6_ifindex;
   };
public: //for lib interface
   CIfCfg();
   ~CIfCfg();
   bool Init();

   bool IsRunNetworkIF(const char* pIfName,const char* pAddr, bool bIPv4);
   bool UpIPAddr(const char* pIfName,const char* pAddr, const char* pIPv4Mask, bool bIPv4);
   bool DownIPAddr(const char* pIfName,const char* pAddr,bool bIPv4);

   bool CheckIFRunning(const char* pIfName, bool bCheckRun=true, const char* pIPv4Addr=NULL);
   bool CheckIFRunningIPv6(const char* pIfName, const char* pAddr);

   bool UpIPv6(const char* pIfName, const char* pIPAddr);
   bool DownIPv6(const char* pIfName, const char* pIPAddr);
   bool UpIPv4(const char* pIfName, const char* pIPv4Addr, const char* pIPv4NetMask);
   bool DownIPv4(const char* pIfName);

   bool IsEqualIPv6(const char* pAddrA, const char* pAddrB);

public: //for common interface
   bool GetHwAddr(const char* pIfName, char* szHwAddr, int nLen);
   bool GetHwAddr2(const char* pIfName, char* szHwAddr, int nLen);

   bool GetIPv4Addr(const char* pIfName, char* szOut, int nLen);   
   bool GetMask(const char* pIfName, char* szOut, int nLen);
   bool GetBroadCast(const char* pIfName, char* szOut, int nLen);

   bool GetIPv4Int(const char* pIfName, unsigned long& ulOut);
   bool GetMaskInt(const char* pIfName, unsigned long& ulOut);
   bool GetBroadCastInt(const char* pIfName, unsigned long& ulOut);

   bool IsUp(const char* pIfName);
   bool IsRunning(const char* pIfName);
   bool IsMaster(const char* pIfName);
   bool IsSlave(const char* pIfName);
//static function
public:
   static int INET_input(int type, char *bufp, struct sockaddr *sap);
   static int INET_getsock(char *bufp, struct sockaddr *sap);
   static int INET_resolve(char *name, struct sockaddr_in *sin, int hostfirst);
   static int INET6_input(int type, char *bufp, struct sockaddr *sap);
   static int INET6_getsock(char *bufp, struct sockaddr *sap);
   static int INET6_resolve(char *name, struct sockaddr_in6 *sin6);
protected:
   bool GetComIPv4Addr(const char* pIfName, int nReq, char* szOut, int nLen);
   bool GetComIPv4Int(  const char* pIfName, int nReq, unsigned long& ulOut);
   bool SetupIPv4(const char* pIfName,int nReq, const char* pIPv4Addr);
   int  GetFlags(const char* pIfName);
   bool DeleteFlag(const char* pIfName,short sFlag);
   bool AddFlag(const char* pIfName,short sFlag);
   bool CheckNetFile(const char* pIfName);
protected:
   int			   m_nSock;
   int			   m_nSock6;
   int            m_nMaxBufSize;
   char*          m_pBuf;
   char           m_szLog[16];
};

/////////////
class CArp
   : private CUncopyable
{
private:
   enum {
      MAC_ADDR_LEN    = 6,
      IP_ADDR_LEN     = 4,
      ARP_FRAME_TYPE  = 0x0806,
      IP_PROTO_TYPE   = 0x0800,
      ARP_ADD_PADING  = 18
   };
   struct arp_packet {
      unsigned char  targ_hw_addr[MAC_ADDR_LEN]; //ip layer recv mac
      unsigned char  src_hw_addr[MAC_ADDR_LEN]; //ip layer sender mac
      unsigned short frame_type;
      unsigned short hw_type;
      unsigned short prot_type;
      unsigned char  hw_addr_size;
      unsigned char  prot_addr_size;
      unsigned short op;
      unsigned char  sndr_hw_addr[MAC_ADDR_LEN]; //sender mac
      unsigned char  sndr_ip_addr[IP_ADDR_LEN];  //sender ip
      unsigned char  rcpt_hw_addr[MAC_ADDR_LEN]; //recv mac
      unsigned char  rcpt_ip_addr[IP_ADDR_LEN];  //recv ip
      unsigned char  padding[ARP_ADD_PADING]; //padding 18byte. arping none.
   };
   struct in6_pktinfo {
        struct in6_addr ipi6_addr;
        int             ipi6_ifindex;
   };
public:
   CArp();
   ~CArp();
   bool Init();
   bool SendARP( const char* pSrcIPAddr, const char* pSrcHWAddr,
                    const char* pDstIPAddr, const char* pIfName);
   bool SendNDP(const char* pIPAddr, const char* pIfName);
protected:
   uint16_t Checksum (uint16_t *addr, int len);
protected:
   int			   m_nSock;
   int			   m_nSock6;
   uint8_t*       m_pOutpack;
   uint8_t*       m_pSdhdr;
   uint8_t*       m_pCmsgCtrl;
   int            m_nCmsglen;
   CCritSec*      m_pLock;
   char           m_szLog[16];
};

#endif
