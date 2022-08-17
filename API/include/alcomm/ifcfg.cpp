#include <netinet/icmp6.h>    // struct nd_neighbor_advert, which contains icmp6_hdr, ND_NEIGHBOR_ADVERT
#include <netinet/in.h>       // IPPROTO_IPV6, IPPROTO_ICMPV6
#include <netinet/ip.h>       // IP_MAXPACKET (65535)
#include <netdb.h>            // struct addrinfo
#include <sys/ioctl.h>        // macro ioctl is defined
#include <bits/ioctls.h>      // defines values for argument "request" of ioctl. Here, we need SIOCGIFHWADDR
#include <bits/socket.h>      // structs msghdr and cmsghdr
#include <net/if.h>           // struct ifreq

#include "ifcfg.h"



#define PI_PATH_PROCNET_DEV     "/proc/net/dev"
#define PI_PATH_PROCNET6_DEV     "/proc/net/if_inet6"



/////////////////////////
//constructor
CIfCfg::CIfCfg()
        : m_nSock(-1),
          m_nMaxBufSize(0),
          m_pBuf(NULL)

{
   m_nMaxBufSize=sizeof(struct ifreq) * e_MAXNUM_DEV;
   m_pBuf=new char[m_nMaxBufSize];
   strcpy(m_szLog,"[IFCFG]");
}

CIfCfg::~CIfCfg()
{
   if (m_nSock != -1){
      close(m_nSock);
      m_nSock=-1;
   }
   if (m_nSock6 != -1){
      close(m_nSock6);
      m_nSock6=-1;
   }
}

bool CIfCfg::Init()
{
   if(m_nSock > 0 ){
      LOGGER(PL_WAR,"%s already opened sock",m_szLog);
      return true;
   }
   //create sock
   m_nSock = socket(AF_INET, SOCK_DGRAM, 0);
   m_nSock6 = socket(AF_INET6, SOCK_DGRAM, 0);
   if ( m_nSock <= 0 || m_nSock6 <= 0  ){
      LOGGER(PL_ERR,"%s fail in open IPv4 or IPv6 sock",m_szLog);
      return false;
   }
   LOGGER(PL_INF,"%s init success",m_szLog);
   return true;
}


bool CIfCfg::IsRunNetworkIF(const char* pIfName,const char* pAddr, bool bIPv4)
{
   if( !pIfName ){
      LOGGER(PL_ERR,"%s fail in IsRunNetworkIF. param pIfName is null",m_szLog);
      return false;
   }
   //ipv6 bond0:1 -> bond0
   char szTmpBuf[128];
   char* pTmp=NULL;
   strncpy(szTmpBuf,pIfName,sizeof(szTmpBuf)-1);
   if( !bIPv4 ){
      if( !pAddr ){
         LOGGER(PL_ERR,"%s fail in IsRunNetworkIF. param pAddr is null. ipv6 is mandatory",m_szLog);
         return false;
      }
      if( (pTmp=strchr(szTmpBuf,':')) != NULL ){
         *pTmp=0x00;
      }
      return CheckIFRunningIPv6(szTmpBuf,pAddr);
   }
   return CheckIFRunning(pIfName,true);
}
bool CIfCfg::UpIPAddr(const char* pIfName,const char* pAddr, const char* pIPv4Mask, bool bIPv4)
{
   if( !pIfName || !pAddr ){
      LOGGER(PL_ERR,"%s fail in UpIPAddr. param pIfName or pAddris null",m_szLog);
      return false;
   }
   //ipv6 bond0:1 -> bond0
   char szTmpBuf[128];
   char* pTmp=NULL;
   strncpy(szTmpBuf,pIfName,sizeof(szTmpBuf)-1);
   if( !bIPv4 ){
      if( (pTmp=strchr(szTmpBuf,':')) != NULL ){
         *pTmp=0x00;
      }
      return UpIPv6(szTmpBuf,pAddr);
   }
   if ( !pIPv4Mask ){
      LOGGER(PL_ERR,"%s fail in UpIPAddr. param pIPv4Mask",m_szLog);
      return false;
   }
   return UpIPv4(pIfName,pAddr,pIPv4Mask);
}

bool CIfCfg::DownIPAddr(const char* pIfName,const char* pAddr, bool bIPv4)
{
   if( !pIfName ){
      LOGGER(PL_ERR,"%s fail in DownIPAddr. param pIfName is null",m_szLog);
      return false;
   }
   //ipv6 bond0:1 -> bond0
   char szTmpBuf[128];
   char* pTmp=NULL;
   strncpy(szTmpBuf,pIfName,sizeof(szTmpBuf)-1);
   if( !bIPv4 ){
      if( !pAddr ){
         LOGGER(PL_ERR,"%s fail in DownIPAddr. param pAddr is null. ipv6 is mandatory",m_szLog);
         return false;
      }
      if( (pTmp=strchr(szTmpBuf,':')) != NULL ){
         *pTmp=0x00;
      }
      return DownIPv6(szTmpBuf,pAddr);
   }
   return DownIPv4(pIfName);
}

bool CIfCfg::UpIPv4(const char* pIfName, const char* pIPv4Addr, const char* pIPv4NetMask)
{
   bool bRet=false;
   char tmpBuf[256]; tmpBuf[0]=0x00;
   do{
      if( !pIfName || !pIPv4Addr || !pIPv4NetMask ){
         LOGGER(PL_ERR,"%s fail in IPv4 up. param error",m_szLog);
         break;
      }
      //check already is up
      if ( CheckIFRunning(pIfName,true,pIPv4Addr) ){
         if ( GetMask(pIfName,tmpBuf,sizeof(tmpBuf))){ //check netmask
            if ( strcmp(tmpBuf,pIPv4NetMask) == 0 ){
               LOGGER(PL_INF,"%s if(%s:%s) already is running.will ",m_szLog,pIfName,pIPv4Addr);
               bRet=true;
               break;
            }
         }
      }
      //set up ipv4 address
      bRet=SetupIPv4(pIfName,SIOCSIFADDR,pIPv4Addr);
      if(!bRet) break;
      //set up ipv4 netmask
      bRet=SetupIPv4(pIfName,SIOCSIFNETMASK,pIPv4NetMask);
      if(!bRet) break;
      //add flag
      //bRet=AddFlag(pIfName,(IFF_UP | IFF_RUNNING | IFF_MASTER | IFF_MULTICAST | IFF_BROADCAST));
      bRet=AddFlag(pIfName,(IFF_UP | IFF_RUNNING | IFF_BROADCAST));
      if(!bRet) break;
   }while(0);
   return bRet;
}

bool CIfCfg::DownIPv4(const char* pIfName)
{
   bool bRet=false;
   char tmpBuf[256]; tmpBuf[0]=0x00;
   do{
      if( !pIfName ){
         LOGGER(PL_ERR,"%s fail in IPv4 Down. param error",m_szLog);
         break;
      }
      //check already is down
      if ( CheckIFRunning(pIfName,false,NULL) == false ){
         LOGGER(PL_INF,"%s if(%s:%s) already is down. ",m_szLog,pIfName);
         bRet=true;
         break;
      }
      //add flag
      bRet=DeleteFlag(pIfName,(IFF_UP));
      if(!bRet) break;
   }while(0);
   return bRet;
}

bool CIfCfg::AddFlag(const char* pIfName,short sFlag)
{
   struct ifreq ifr;
   strcpy(ifr.ifr_name,pIfName); 

   //get flag
   int nRet=0;
   nRet=ioctl(m_nSock,SIOCGIFFLAGS,&ifr);
   if( nRet < 0 ){
      LOGGER(PL_ERR,"%s fail in SetupFlag. unknown if(%s)",m_szLog,pIfName);
      return false;
   }
   //set flag
   strcpy(ifr.ifr_name,pIfName); 
   ifr.ifr_flags |= sFlag;
   nRet=ioctl(m_nSock,SIOCSIFFLAGS,&ifr);
   if( nRet < 0 ){
      LOGGER(PL_ERR,"%s fail in SetupFlag.SIOCSIFFLAGS if(%s)",m_szLog,pIfName);
      return false;
   }
   return true;
}
bool CIfCfg::DeleteFlag(const char* pIfName,short sFlag)
{
   struct ifreq ifr;
   strcpy(ifr.ifr_name,pIfName); 

   //get flag
   int nRet=0;
   nRet=ioctl(m_nSock,SIOCGIFFLAGS,&ifr);
   if( nRet < 0 ){
      LOGGER(PL_ERR,"%s fail in ClearFlag. unknown if(%s)",m_szLog,pIfName);
      return false;
   }
   //set flag
   strcpy(ifr.ifr_name,pIfName); 
   ifr.ifr_flags &= ~sFlag;
   nRet=ioctl(m_nSock,SIOCSIFFLAGS,&ifr);
   if( nRet < 0 ){
      LOGGER(PL_ERR,"%s fail in ClearFlag.SIOCSIFFLAGS if(%s)",m_szLog,pIfName);
      return false;
   }
   return true;
}

bool CIfCfg::SetupIPv4(const char* pIfName,int nReq, const char* pIPv4Addr)
{
   if ( !pIfName || !pIPv4Addr ){
      LOGGER(PL_ERR,"%s fail in SetupIPv4. param error",m_szLog);
      return false;
   }
   struct ifreq ifr;
   struct sockaddr_in sin;
   strcpy(ifr.ifr_name,pIfName); 

   memset(&sin, 0, sizeof(struct sockaddr));
   if( CIfCfg::INET_input(0,(char*)pIPv4Addr,(struct sockaddr *)&sin) < 0){
      LOGGER(PL_ERR,"%s if(%s) fail in NET_iput. convert ip string->sockaddr",m_szLog,pIfName);
   }
   memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));
   int nRet=0;

   nRet=ioctl(m_nSock,nReq,&ifr);
   if( nRet < 0 ){
      LOGGER(PL_ERR,"%s fail in SetupIPv4. if(%s)/ip(%s)",m_szLog,pIfName,pIPv4Addr);
      return false;
   }
   return true;
}
bool CIfCfg::CheckIFRunningIPv6(const char* pIfName, const char* pAddr)
{
   bool bRet=false;
   char addr6[40], devname[20];
   int plen, scope, dad_status, if_idx;
   char addr6p[8][5];
   FILE* fp=NULL;

   char tmpBuf[128];tmpBuf[0]=0x00;
   char* pTmp=NULL;
   int nPrefix=0;
   strncpy(tmpBuf,pAddr,sizeof(tmpBuf)-1); 
   if( (pTmp =strchr(tmpBuf,'/')) ){
      nPrefix=atoi(pTmp+1);
      *pTmp=0x00;
      if( (nPrefix < 0) || (nPrefix > 128)){
         LOGGER(PL_WAR,"%s fail in CheckIFRunningIPv6. prefix error(%d)",m_szLog,nPrefix);
      }
   }
   do{
      fp=fopen(PI_PATH_PROCNET6_DEV, "r");
      if(!fp){
         LOGGER(PL_ERR,"%s fail in CheckIFRunningIPv6. fopen error.(%s)",
            m_szLog,PI_PATH_PROCNET6_DEV);
      }
      while (fscanf(fp, "%4s%4s%4s%4s%4s%4s%4s%4s %02x %02x %02x %02x %20s\n",
            addr6p[0], addr6p[1], addr6p[2], addr6p[3],
            addr6p[4], addr6p[5], addr6p[6], addr6p[7],
            &if_idx, &plen, &scope, &dad_status, devname) != EOF) { 
          //check devname
         if (!strcmp(devname, pIfName)) {
              sprintf(addr6, "%s:%s:%s:%s:%s:%s:%s:%s",
              addr6p[0], addr6p[1], addr6p[2], addr6p[3],
              addr6p[4], addr6p[5], addr6p[6], addr6p[7]);
            if( IsEqualIPv6((const char*)addr6,tmpBuf) ){
               bRet=true;
               break;
            }
         }
      }
   }while(0);
   ALOX_CLOSE_FP(fp);
   return bRet;
}

bool CIfCfg::IsEqualIPv6(const char* pAddrA, const char* pAddrB)
{
   struct sockaddr_in6 sapA;
   unsigned char* pA=NULL;
   struct sockaddr_in6 sapB;
   unsigned char* pB=NULL;
   if( INET6_input(1, (char*)pAddrA, (struct sockaddr *) &sapA) < 0 ) return false;
   pA=sapA.sin6_addr.s6_addr;
   if( INET6_input(1, (char*)pAddrB, (struct sockaddr *) &sapB) < 0 ) return false;
   pB=sapB.sin6_addr.s6_addr;

   if( !pA || !pB ) return false;
   for( int i = 0 ; i < 16 ; i++){
      if ( *(pA+i) != *(pB+i) ){
         return false;
      }
   }
   return true;
}

bool CIfCfg::UpIPv6(const char* pIfName, const char* pAddr)
{
   if ( !pIfName || !pAddr){
      LOGGER(PL_ERR,"%s fail in UpIPv6. param(pIfName) is null",m_szLog);
      return false;
   }
   if( CheckIFRunningIPv6(pIfName,pAddr) == true ){
      LOGGER(PL_INF,"%s already enable. %s/%s",m_szLog,pIfName,pAddr);
      return true;
   }

   char tmpBuf[128];tmpBuf[0]=0x00;
   char* pTmp=NULL;
   int nPrefix=0;
  
   strncpy(tmpBuf,pAddr,sizeof(tmpBuf)-1); 
   if( (pTmp =strchr(tmpBuf,'/')) ){
      nPrefix=atoi(pTmp+1);
      *pTmp=0x00;
      if( (nPrefix < 0) || (nPrefix > 128)){
         LOGGER(PL_ERR,"%s fail in UpIPv6. prefix error(%d)",m_szLog,nPrefix);
         return false;
      }
   }
   struct in6_ifreq ifr6;
   struct ifreq ifr;
   struct sockaddr_in6 sa6;
   if( INET6_input(1,(char*)tmpBuf,(struct sockaddr *)(&sa6)) < 0 ){
      LOGGER(PL_ERR,"%s fail in UpIPv6. INET6_input",m_szLog);
      return false;
   }

   memcpy((char *)&ifr6.ifr6_addr,(char *)&sa6.sin6_addr,sizeof(struct in6_addr));

   strcpy(ifr.ifr_name, pIfName);
   if (ioctl(m_nSock6, SIOGIFINDEX, &ifr) < 0) {
      LOGGER(PL_ERR,"%s fail in UpIPv6. ioctl(fd, SIOGIFINDEX, &ifr)",m_szLog);
      return false;
   }

   ifr6.ifr6_ifindex = ifr.ifr_ifindex;
   ifr6.ifr6_prefixlen = nPrefix;
   if (ioctl(m_nSock6, SIOCSIFADDR, &ifr6) < 0) {
      LOGGER(PL_ERR,"%s fail in UpIPv6. ioctl(m_nSock6, SIOCSIFADDR, &ifr6)",m_szLog);
      return false;
   }
   return true;
}
bool CIfCfg::DownIPv6(const char* pIfName, const char* pAddr)
{
   if ( !pIfName || !pAddr){
      LOGGER(PL_ERR,"%s fail in DownIPv6. param(pIfName) is null",m_szLog);
      return false;
   }
   char tmpBuf[128];tmpBuf[0]=0x00;
   char* pTmp=NULL;
   int nPrefix=0;
  
   strncpy(tmpBuf,pAddr,sizeof(tmpBuf)-1); 
   if( (pTmp =strchr(tmpBuf,'/')) ){
      nPrefix=atoi(pTmp+1);
      *pTmp=0x00;
      if( (nPrefix < 0) || (nPrefix > 128)){
         LOGGER(PL_ERR,"%s fail in DownIPv6. prefix error(%d)",m_szLog,nPrefix);
         return false;
      }
   }
   struct in6_ifreq ifr6;
   struct ifreq ifr;
   struct sockaddr_in6 sa6;
   if( INET6_input(1,(char*)tmpBuf,(struct sockaddr *)(&sa6)) < 0 ){
      LOGGER(PL_ERR,"%s fail in DownIPv6. INET6_input",m_szLog);
      return false;
   }

   memcpy((char *)&ifr6.ifr6_addr,(char *)&sa6.sin6_addr,sizeof(struct in6_addr));

   strcpy(ifr.ifr_name, pIfName);
   if (ioctl(m_nSock6, SIOGIFINDEX, &ifr) < 0) {
      LOGGER(PL_ERR,"%s fail in DownIPv6. ioctl(fd, SIOGIFINDEX, &ifr)",m_szLog);
      return false;
   }

   ifr6.ifr6_ifindex = ifr.ifr_ifindex;
   ifr6.ifr6_prefixlen = nPrefix;
   if (ioctl(m_nSock6, SIOCDIFADDR, &ifr6) < 0) {
      LOGGER(PL_ERR,"%s fail in DownIPv6. ioctl(m_nSock6, SIOCDIFADDR, &ifr6)",m_szLog);
      return false;
   }
   return true;
}


bool CIfCfg::CheckIFRunning(const char* pIfName, bool bCheckRun, const char* pIPv4Addr)
{
   if ( !pIfName ){
      LOGGER(PL_ERR,"%s fail in CheckIFRunning. param(pIfName) is null",m_szLog);
      return false;
   }
   struct ifconf ifc;
   ifc.ifc_len = m_nMaxBufSize;
   ifc.ifc_buf = m_pBuf;

   struct ifreq *ifrp = ifc.ifc_req;
   struct ifreq ifr;

   char szIPAddr[256]; szIPAddr[0]=0x00;
   bool bRet=false; 
   int nRet=0;
   do{
      nRet=ioctl(m_nSock, SIOCGIFCONF, (char *)&ifc);
      if (nRet < 0){
         LOGGER(PL_ERR,"%s fail in ioctl(m_nSock, SIOCGIFCONF)",m_szLog);
         break;
      }
      for (int n = ifc.ifc_len/sizeof(struct ifreq); n > 0; n--, ifrp++){
         strcpy(ifr.ifr_name, ifrp->ifr_name);
         //check interface name
         if( strcmp(ifrp->ifr_name,pIfName) == 0 ){
            bRet=true;
            break;
         }
      }
      //file check
      if( bRet == false){
         bRet=CheckNetFile(pIfName);
         if(bRet == false) break;
         //LOGGER(PL_INF,"%s recheck found ifname(%s)",m_szLog,pIfName);
      }

      //LOGGER(PL_INF,"%s found ifname(%s)",m_szLog,pIfName);

      //check run
      nRet=GetFlags(pIfName);
      if ( bCheckRun ){
         if( !((nRet&IFF_UP) && (nRet&IFF_RUNNING)) ){
            LOGGER(PL_ERR,"%s if(%s) is not (up or running)",m_szLog,pIfName);
            break;
         }
      }else{
         if(!(nRet&IFF_UP)){
            LOGGER(PL_ERR,"%s if(%s) is not up",m_szLog,pIfName);
            break;
         }
      }
      //check ip
      if ( pIPv4Addr == NULL ){
         bRet=true;
         break;
      }  
      if( !GetIPv4Addr(pIfName,szIPAddr,sizeof(szIPAddr)) ){
         bRet=false;
         break;
      }
      if( strcmp(szIPAddr,pIPv4Addr) == 0 ){
        bRet=true;
      }
   }while(0);
   return bRet;
}

bool CIfCfg::GetComIPv4Int(  const char* pIfName, int nReq, unsigned long& ulOut)
{
   struct ifreq ifr;
   strcpy(ifr.ifr_name, pIfName);
   int nRet=0;
   nRet=ioctl(m_nSock, nReq, (caddr_t)&ifr) ;
   if (nRet<0){
      LOGGER(PL_ERR,"%s fail in ioctl(m_nSock, %d). name:%s",m_szLog,nReq,pIfName); 
      return false;
   }
   struct sockaddr_in *paddr=NULL;
   paddr = (struct sockaddr_in *)&(ifr.ifr_addr);
   if( !paddr ){
      LOGGER(PL_ERR,"%s fail in get pointer name:%s",m_szLog,pIfName); 
      return false;
   }
   memcpy(&ulOut,&(paddr->sin_addr.s_addr),sizeof(unsigned long));
   return true;
}

bool CIfCfg::GetComIPv4Addr(  const char* pIfName, int nReq, 
                              char* szOut, int nLen)
{
   struct ifreq ifr;
   strcpy(ifr.ifr_name, pIfName);
   int nRet=0;
   nRet=ioctl(m_nSock, nReq, (caddr_t)&ifr) ;
   if (nRet<0){
      LOGGER(PL_ERR,"%s fail in ioctl(m_nSock, %d). name:%s",m_szLog,nReq,pIfName); 
      return false;
   }
   struct sockaddr_in *paddr=NULL;
   paddr = (struct sockaddr_in *)&(ifr.ifr_addr);
   if( !paddr ){
      LOGGER(PL_ERR,"%s fail in get pointer name:%s",m_szLog,pIfName); 
      return false;
   }
   //get ipv4
   char* pResult=NULL;
   pResult = inet_ntoa(paddr->sin_addr);
   if( !pResult || strlen(pResult) <=0 ){
      LOGGER(PL_ERR,"%s fail in ipv4 string name:%s",m_szLog,pIfName); 
      return false;
   }
   strncpy(szOut,pResult,nLen-1);
   return true;
}
int CIfCfg::GetFlags(const char* pIfName)
{
   int nRet=0;
   struct ifreq ifr;
   strcpy(ifr.ifr_name, pIfName);
   nRet=ioctl(m_nSock, SIOCGIFFLAGS, (caddr_t)&ifr) ;
   if (nRet < 0){
      LOGGER(PL_ERR,"%s fail in ioctl(m_nSock, SIOCGIFFLAGS). name:%s",m_szLog,pIfName); 
      return 0;
   }
   return (int)ifr.ifr_flags;
}

bool CIfCfg::CheckNetFile(const char* pIfName)
{
   FILE* fp=NULL;
   bool bRet=false;
   char linebuf[1024];
   char tmpBuf[16]; 
   do{
      fp=fopen(PI_PATH_PROCNET_DEV, "r");
      if(!fp) break;
      //get line
      while(fgets(linebuf, 1024, fp) != NULL){
         memset(tmpBuf,0x00,sizeof(tmpBuf));
         memcpy(tmpBuf,linebuf,15);
         if(strstr(tmpBuf,pIfName)){
            bRet=true;
            break;
         }
      }
   }while(0);
   ALOX_CLOSE_FP(fp);
   return bRet;
}


bool CIfCfg::GetHwAddr(const char* pIfName, char* szHwAddr, int nLen)
{
   struct ifreq ifr;
   strcpy(ifr.ifr_name, pIfName);

   if (ioctl(m_nSock, SIOCGIFHWADDR, &ifr) < 0){
      LOGGER(PL_ERR,"%s fail in ioctl(m_nSock, SIOCGIFHWADDR). name:%s",m_szLog,pIfName);
      return false;
   }
   snprintf(szHwAddr,nLen-1,"%02x:%02x:%02x:%02x:%02x:%02x",ifr.ifr_hwaddr.sa_data[0]&0xff,
      ifr.ifr_hwaddr.sa_data[1]&0xff, ifr.ifr_hwaddr.sa_data[2]&0xff,
      ifr.ifr_hwaddr.sa_data[3]&0xff,ifr.ifr_hwaddr.sa_data[4]&0xff,
      ifr.ifr_hwaddr.sa_data[5]&0xff);
   return true;
}
bool CIfCfg::GetHwAddr2(const char* pIfName, char* szHwAddr, int nLen)
{
   if ( nLen < 10 ) {
      LOGGER(PL_ERR,"%s fail in GetHwAddr2 name:%s. length is short(%d/10)",m_szLog,pIfName,nLen);
      return false;
   }
   struct ifreq ifr;
   strcpy(ifr.ifr_name, pIfName);

   if (ioctl(m_nSock, SIOCGIFHWADDR, &ifr) < 0){
      LOGGER(PL_ERR,"%s fail in ioctl(m_nSock, SIOCGIFHWADDR). name:%s",m_szLog,pIfName);
      return false;
   }
   memcpy(szHwAddr,ifr.ifr_hwaddr.sa_data,8);
   szHwAddr[8]=0x00;
   return true;
}

bool CIfCfg::GetIPv4Addr(const char* pIfName, char* szOut, int nLen)
{
   return GetComIPv4Addr(pIfName,SIOCGIFADDR,szOut,nLen);
}
bool CIfCfg::GetMask(const char* pIfName, char* szOut, int nLen)
{
   return GetComIPv4Addr(pIfName,SIOCGIFNETMASK,szOut,nLen);
}
bool CIfCfg::GetBroadCast(const char* pIfName, char* szOut, int nLen)
{
   return GetComIPv4Addr(pIfName,SIOCGIFBRDADDR,szOut,nLen);
}

bool CIfCfg::GetIPv4Int(const char* pIfName, unsigned long& ulOut)
{
   return GetComIPv4Int(pIfName,SIOCGIFADDR,ulOut);
}
bool CIfCfg::GetMaskInt(const char* pIfName, unsigned long& ulOut)
{
   return GetComIPv4Int(pIfName,SIOCGIFNETMASK,ulOut);
}
bool CIfCfg::GetBroadCastInt(const char* pIfName, unsigned long& ulOut)
{
   return GetComIPv4Int(pIfName,SIOCGIFBRDADDR,ulOut);
}

bool  CIfCfg::IsUp(const char* pIfName)
{
   return (GetFlags(pIfName) & IFF_UP) != 0;
}
bool  CIfCfg::IsRunning(const char* pIfName)
{
   return (GetFlags(pIfName) & IFF_RUNNING ) != 0;
}
bool  CIfCfg::IsMaster(const char* pIfName)
{
   return (GetFlags(pIfName) & IFF_MASTER ) != 0;
}
bool  CIfCfg::IsSlave(const char* pIfName)
{
   return (GetFlags(pIfName) & IFF_SLAVE ) != 0;
}

//static function
int CIfCfg::INET_input(int type, char *bufp, struct sockaddr *sap)
{
   switch (type) {
   case 1:
   return (INET_getsock(bufp, sap));
   case 256:
   return (INET_resolve(bufp, (struct sockaddr_in *) sap, 1));
   default:
   return (INET_resolve(bufp, (struct sockaddr_in *) sap, 0));
   }
}

//char *bufp            IN/OUT   hostip
//struct sockaddr *sap  OUT/OUT  sockaddr 
int CIfCfg::INET_getsock(char *bufp, struct sockaddr *sap)
{
   char *sp = bufp, *bp;
   unsigned int i;
   unsigned val;
   struct sockaddr_in *sin;

   sin = (struct sockaddr_in *) sap;
   sin->sin_family = AF_INET;
   sin->sin_port = 0;

   val = 0;
   bp = (char *) &val;
   for (i = 0; i < sizeof(sin->sin_addr.s_addr); i++) {
      *sp = toupper(*sp);

      if ((*sp >= 'A') && (*sp <= 'F'))
         bp[i] |= (int) (*sp - 'A') + 10;
      else if ((*sp >= '0') && (*sp <= '9'))
         bp[i] |= (int) (*sp - '0');
      else
         return (-1);

      bp[i] <<= 4;
      sp++;
      *sp = toupper(*sp);

      if ((*sp >= 'A') && (*sp <= 'F'))
         bp[i] |= (int) (*sp - 'A') + 10;
      else if ((*sp >= '0') && (*sp <= '9'))
         bp[i] |= (int) (*sp - '0');
      else
         return (-1);
      sp++;
   }//end for
   sin->sin_addr.s_addr = htonl(val);

   return (sp - bufp);
}

//
//
int CIfCfg::INET_resolve(char *name, struct sockaddr_in *sin, int hostfirst)
{
   struct hostent *hp;
   struct netent *np;

   /* Grmpf. -FvK */
   sin->sin_family = AF_INET;
   sin->sin_port = 0;

   /* Default is special, meaning 0.0.0.0. */
   if (!strcmp(name, "default")) {
      sin->sin_addr.s_addr = INADDR_ANY;
      return (1);
   }
   /* Look to see if it's a dotted quad. */
   if (inet_aton(name, &sin->sin_addr)) {
      return 0;
   }
   /* If we expect this to be a hostname, try hostname database first */
   if (hostfirst && 
      (hp = gethostbyname(name)) != (struct hostent *) NULL) {
      memcpy((char *) &sin->sin_addr, (char *) hp->h_addr_list[0], 
      sizeof(struct in_addr));
      return 0;
   }
   /* Try the NETWORKS database to see if this is a known network. */
   if ((np = getnetbyname(name)) != (struct netent *) NULL) {
      sin->sin_addr.s_addr = htonl(np->n_net);
      return 1;
   }
   if (hostfirst) {
      /* Don't try again */
      errno = h_errno;
      return -1;
   }
   if ((hp = gethostbyname(name)) == (struct hostent *) NULL) {
      errno = h_errno;
      return -1;
   }
   memcpy((char *) &sin->sin_addr, (char *) hp->h_addr_list[0], 
   sizeof(struct in_addr));
   return 0;
}

int CIfCfg::INET6_input(int type, char *bufp, struct sockaddr *sap)
{
   switch (type) {
   case 1:
      return (INET6_getsock(bufp, sap));
   default:
      return (INET6_resolve(bufp, (struct sockaddr_in6 *) sap));
   }
}

int CIfCfg::INET6_getsock(char *bufp, struct sockaddr *sap)
{
   struct sockaddr_in6 *sin6;

   sin6 = (struct sockaddr_in6 *) sap;
   sin6->sin6_family = AF_INET6;
   sin6->sin6_port = 0;

   if (inet_pton(AF_INET6, bufp, sin6->sin6_addr.s6_addr) <= 0)
      return (-1);
   return 16;       /* ?;) */
}
int CIfCfg::INET6_resolve(char *name, struct sockaddr_in6 *sin6)
{
   struct addrinfo req, *ai;
   int s;

   memset (&req, '\0', sizeof req);
   req.ai_family = AF_INET6;
   if ((s = getaddrinfo(name, NULL, &req, &ai))) {
      fprintf(stderr, "getaddrinfo: %s: %d\n", name, s);
      return -1;
   }
   memcpy(sin6, ai->ai_addr, sizeof(struct sockaddr_in6));

   freeaddrinfo(ai);

   return (0);
}

///////////////////////////
CArp::CArp()
      :m_nSock(-1),
       m_nSock6(-1),
       m_pOutpack(NULL),
       m_pSdhdr(NULL),
       m_pLock(NULL)
       
       
{
   strcpy(m_szLog,"[ARP]");
}
CArp::~CArp()
{
   if (m_nSock != -1){
      close(m_nSock);
      m_nSock=-1;
   }
   if (m_nSock6 != -1){
      close(m_nSock6);
      m_nSock6=-1;
   }
}

bool CArp::Init( )
{
   if(m_nSock > 0 || m_nSock6 > 0){ 
      LOGGER(PL_WAR,"%s already opened sock",m_szLog);
      return true;
   }
   //create sock
   m_nSock = socket(AF_INET, SOCK_PACKET, htons(ETH_P_RARP));
   if ( m_nSock < 0 ){
      LOGGER(PL_ERR,"%s fail in open socket(AF_INET, SOCK_PACKET, htons(ETH_P_RAPP)",m_szLog);
      return false;
   }
  
   m_nSock6 = socket (AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
   if ( m_nSock6 < 0 ){
      LOGGER(PL_ERR,"%s fail in open m_nSock6",m_szLog);
      return false;
   }
   m_pOutpack =new uint8_t[IP_MAXPACKET];
   m_pSdhdr =new uint8_t[IP_MAXPACKET];

   m_nCmsglen=CMSG_SPACE (sizeof (int)) + CMSG_SPACE (sizeof (struct in6_pktinfo));
   m_pCmsgCtrl=new uint8_t[m_nCmsglen];
   

   m_pLock=new CCritSec;

   LOGGER(PL_INF,"%s init success",m_szLog);
   return true;
}

bool CArp::SendARP( const char* pSrcIPAddr, const char* pSrcHWAddr, 
                    const char* pDstIPAddr, const char* pIfName)
{
//send_arp sndr_ip_addr sndr_hw_addr targ_ip_addr targ_hw_addr [src_int [src_hw_addr [dest_hw_addr]]]
//send_arp 50.100.15.180 00:30:64:0F:E3:AC 50.100.15.255 ffffffffffff bond1:1
   struct in_addr src_in_addr, dst_in_addr;

   struct sockaddr_in sin;
   //change src ip 
   if( CIfCfg::INET_input(0,(char*)pSrcIPAddr,(struct sockaddr *)&sin) < 0){
      LOGGER(PL_ERR,"%s fail in change ip src addr(%s)",m_szLog,pSrcIPAddr);
      return false;
   }
   src_in_addr.s_addr=sin.sin_addr.s_addr;

   //change ip 
   memset(&sin,0x00,sizeof(sockaddr_in));
   if( CIfCfg::INET_input(0,(char*)pDstIPAddr,(struct sockaddr *)&sin) < 0){
      LOGGER(PL_ERR,"%s fail in change ip dst addr(%s)",m_szLog,pSrcIPAddr);
      return false;
   }
   dst_in_addr.s_addr=sin.sin_addr.s_addr;
   
   unsigned short   optypes[]= {2, 1, 1, 0}; // {ARP_REPLY, ARP_REQUEST, 0}; 
   struct sockaddr sa;
   char* pTmp=NULL;
   char tmpName[256];
   strncpy(tmpName,pIfName,sizeof(tmpName)-1);
   //set real ifname
   if( (pTmp = strchr(tmpName,':')) !=NULL)  *pTmp=0x00;

   arp_packet pkt;
   //0:send_arp, 1 ~ 2:arping
   for( int i=0 ; i < 3 ; i++){
      memset(&pkt,0x00,sizeof(pkt)); 
      pkt.frame_type     = htons(ARP_FRAME_TYPE); //frame type ARP
      pkt.hw_type        = htons(1); //ethernet hardware type
      pkt.prot_type      = htons(IP_PROTO_TYPE); //ip proto type
      pkt.hw_addr_size   = MAC_ADDR_LEN; //6
      pkt.prot_addr_size = IP_ADDR_LEN; //4
      pkt.op             = htons(optypes[i]);

      //set ip
      memcpy(pkt.sndr_ip_addr,&src_in_addr,IP_ADDR_LEN);
      if( i == 0 ){
         memcpy(pkt.rcpt_ip_addr,&dst_in_addr,IP_ADDR_LEN); 
      }else{
         //arp ing source ip
         memcpy(pkt.rcpt_ip_addr,&src_in_addr,IP_ADDR_LEN); 
      }

      //set mac
      memset(pkt.targ_hw_addr,0xff,MAC_ADDR_LEN);
      //memset(pkt.src_hw_addr,0xff,MAC_ADDR_LEN);
      //memset(pkt.sndr_hw_addr,0xff,MAC_ADDR_LEN);
      memset(pkt.rcpt_hw_addr,0xff,MAC_ADDR_LEN);

      memcpy(pkt.src_hw_addr,pSrcHWAddr,MAC_ADDR_LEN);
      memcpy(pkt.sndr_hw_addr,pSrcHWAddr,MAC_ADDR_LEN);

      memset(&sa,0x00,sizeof(sa));
      sa.sa_family = AF_INET ;
      strcpy(sa.sa_data,tmpName);
      if ( i == 0 ){
         if( sendto(m_nSock, (const void *)&pkt, sizeof(pkt), 0, &sa,sizeof(sa)) < 0) {
            LOGGER(PL_ERR,"%s fail in send_arp %s %s %s %s",m_szLog,pSrcIPAddr,pSrcHWAddr,pDstIPAddr,tmpName);
            return false; 
         }
      }else{
         if( sendto(m_nSock, (const void *)&pkt, (sizeof(pkt) - ARP_ADD_PADING), 0, &sa,sizeof(sa)) < 0) {
            LOGGER(PL_ERR,"%s fail in arping %s %s %s %s",m_szLog,pSrcIPAddr,pSrcHWAddr,pSrcIPAddr,tmpName);
            return false; 
         }
      }
      if( i  == 0 ){
         LOGGER(PL_INF,"%s success send_arp %s %s %s %s",m_szLog,pSrcIPAddr,pSrcHWAddr,pDstIPAddr,tmpName);
      }else{
         LOGGER(PL_INF,"%s success arping %s %s %s %s",m_szLog,pSrcIPAddr,pSrcHWAddr,pSrcIPAddr,tmpName);
      } 
   } //end for
   return true;
}

bool CArp::SendNDP( const char* pIPAddr, const char* pIfName)
{

   char tmpBuf[128];tmpBuf[0]=0x00;
   char* pTmp=NULL;
   strncpy(tmpBuf,pIPAddr,sizeof(tmpBuf)-1);
   if( (pTmp =strchr(tmpBuf,'/')) ){
      *pTmp=0x00;
   }

   struct addrinfo hints;
   struct sockaddr_in6 src, dst;
   struct msghdr msghdr;
   struct cmsghdr *cmsghdr1, *cmsghdr2;
   struct in6_pktinfo *pktinfo;
   struct iovec iov[2];

   // Fill out hints for getaddrinfo().
   memset (&hints, 0, sizeof (struct addrinfo));
   hints.ai_family = AF_INET6;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = hints.ai_flags | AI_CANONNAME;

   //source
   if( CIfCfg::INET6_input(1,tmpBuf,(struct sockaddr *)(&src)) < 0 ){
      LOGGER(PL_ERR,"%s fail in SendNDP. %s/%s, CIfCfg::INET6_input",m_szLog,pIfName,pIPAddr);
      return false;
   }
  
   //target 
   if( CIfCfg::INET6_input(1,(char*)"ff02::1",(struct sockaddr *)(&dst)) < 0 ){
      LOGGER(PL_ERR,"%s fail in SendNDP. %s/%s, CIfCfg::INET6_input target",m_szLog,pIfName,pIPAddr);
      return false;
   }

   struct ifreq ifr;
   memset (&ifr, 0, sizeof (ifr));
   snprintf (ifr.ifr_name, sizeof (ifr.ifr_name), "%s", pIfName);

   //get mac address
   if (ioctl (m_nSock6, SIOCGIFHWADDR, &ifr) < 0) {
      LOGGER(PL_ERR,"%s fail in SendNDP. %s/%s, get mac address",m_szLog,pIfName,pIPAddr);
      return false;
   }

   //set advertising MAC address into options buffer.
   int optlen = 8; // Option Type (1 byte) + Length (1 byte) + Length of MAC address (6 bytes)
   uint8_t options[8];
   options[0] = 2;
   options[1] = optlen / 8;
   for (int i=0; i<6; i++) {
      options[i+2] = (uint8_t) ifr.ifr_addr.sa_data[i];
   }

   //Find interface index from interface name.
   //This will be put in cmsghdr data in order to specify the interface we want to use.
   int ifindex=0;
   if ((ifindex = if_nametoindex (pIfName)) == 0) {
      LOGGER(PL_ERR,"%s fail in SendNDP. %s/%s, if_nametoindex error",m_szLog,pIfName,pIPAddr);
      return false;
   }

   bool bRet=false;  
   struct nd_neighbor_advert *na=NULL;
   int NA_HDRLEN = sizeof (struct nd_neighbor_advert);
   int psdhdrlen=16 + 16 + 4 + 3 + 1 + NA_HDRLEN + optlen;
   int hoplimit=0;
   m_pLock->Lock();
   do{
      memset(m_pOutpack,0x00,IP_MAXPACKET*(sizeof(uint8_t)));
      memset(m_pSdhdr,0x00,IP_MAXPACKET*(sizeof(uint8_t)));
      memset(m_pCmsgCtrl,0x00,m_nCmsglen *(sizeof(uint8_t)));


      memcpy(m_pSdhdr,src.sin6_addr.s6_addr, 16 * sizeof (uint8_t));  // Copy to checksum pseudo-header
      memcpy(m_pSdhdr + 16, dst.sin6_addr.s6_addr, 16 * sizeof (uint8_t));  // Copy to checksum pseudo-header

      na = (struct nd_neighbor_advert *) m_pOutpack;
      memset (na, 0, sizeof (struct nd_neighbor_advert));

      // Populate icmp6_hdr portion of neighbor advertisement struct.
      na->nd_na_hdr.icmp6_type = ND_NEIGHBOR_ADVERT;  // 136 (RFC 4861)
      na->nd_na_hdr.icmp6_code = 0;              // zero for neighbor advertisement (RFC 4861)
      na->nd_na_hdr.icmp6_cksum = htons(0);      // zero when calculating checksum
      // Set R/S/O flags as: R=0, S=0, O=1. Set reserved to zero (RFC 4861)
      na->nd_na_flags_reserved = htonl((1 << 29));
      //na->nd_na_flags_reserved = htonl((1 << 30) + (1 << 29));
      //set source address 
      na->nd_na_target = src.sin6_addr;          // Target address (as type in6_addr)

      // Append options to end of neighbor advertisement struct.
      memcpy (m_pOutpack + NA_HDRLEN, options, optlen * sizeof (uint8_t));
      
     
      // Prepare msghdr for sendmsg().
      memset (&msghdr, 0, sizeof (msghdr));
      msghdr.msg_name = &dst;  // Destination IPv6 address as struct sockaddr_in6
      msghdr.msg_namelen = sizeof (dst);
      memset (&iov, 0, sizeof (iov));
      iov[0].iov_base = (uint8_t *) m_pOutpack;  // Point msghdr to buffer outpack
      iov[0].iov_len = NA_HDRLEN + optlen;
      msghdr.msg_iov = iov;                 // scatter/gather array
      msghdr.msg_iovlen = 1;                // number of elements in scatter/gather array

      // Tell msghdr we're adding cmsghdr data to change hop limit and specify interface.
      // Allocate some memory for our cmsghdr data.
      msghdr.msg_control = m_pCmsgCtrl;
      msghdr.msg_controllen = m_nCmsglen;

      // Change hop limit to 255 as required for neighbor advertisement (RFC 4861).
      hoplimit = 255;
      cmsghdr1 = CMSG_FIRSTHDR (&msghdr);
      cmsghdr1->cmsg_level = IPPROTO_IPV6;
      cmsghdr1->cmsg_type = IPV6_HOPLIMIT;  // We want to change hop limit
      cmsghdr1->cmsg_len = CMSG_LEN (sizeof (int));
      *((int *) CMSG_DATA (cmsghdr1)) = hoplimit;  // Copy pointer to int hoplimit

      // Specify source interface index for this packet via cmsghdr data.
      cmsghdr2 = CMSG_NXTHDR (&msghdr, cmsghdr1);
      cmsghdr2->cmsg_level = IPPROTO_IPV6;
      cmsghdr2->cmsg_type = IPV6_PKTINFO;  // We want to specify interface here
      cmsghdr2->cmsg_len = CMSG_LEN (sizeof (struct in6_pktinfo));
      pktinfo = (struct in6_pktinfo *) CMSG_DATA (cmsghdr2);
      pktinfo->ipi6_ifindex = ifindex;

      // Compute ICMPv6 checksum (RFC 2460).
      // psdhdr[0 to 15] = source IPv6 address, set earlier.
      // psdhdr[16 to 31] = destination IPv6 address, set earlier.
      m_pSdhdr[32] = 0;  // Length should not be greater than 65535 (i.e., 2 bytes)
      m_pSdhdr[33] = 0;  // Length should not be greater than 65535 (i.e., 2 bytes)
      m_pSdhdr[34] = (NA_HDRLEN + optlen)  / 256;  // Upper layer packet length
      m_pSdhdr[35] = (NA_HDRLEN + optlen)  % 256;  // Upper layer packet length
      m_pSdhdr[36] = 0;  // Must be zero
      m_pSdhdr[37] = 0;  // Must be zero
      m_pSdhdr[38] = 0;  // Must be zero
      m_pSdhdr[39] = IPPROTO_ICMPV6;
      memcpy (m_pSdhdr + 40, m_pOutpack, (NA_HDRLEN + optlen) * sizeof (uint8_t));
      na->nd_na_hdr.icmp6_cksum = Checksum ((uint16_t *) m_pSdhdr, psdhdrlen);
 
      if (sendmsg (m_nSock6, &msghdr, 0) < 0) {
         LOGGER(PL_ERR,"%s fail in SendNDP. %s/%s, send message fail",m_szLog,pIfName,pIPAddr);
         break;
      }     
      LOGGER(PL_INF,"%s success SendNDP. %s/%s => ff02::1",m_szLog,pIfName,pIPAddr);
      bRet=true; 
   }while(0); 
   m_pLock->Unlock();
   return bRet;
}

uint16_t CArp::Checksum (uint16_t *addr, int len)
{
  int nleft = len;
  int sum = 0;
  uint16_t *w = addr;
  uint16_t answer = 0;

  while (nleft > 1) {
    sum += *w++;
    nleft -= sizeof (uint16_t);
  }

  if (nleft == 1) {
    *(uint8_t *) (&answer) = *(uint8_t *) w;
    sum += answer;
  }

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  answer = ~sum;

  return (answer);
}

