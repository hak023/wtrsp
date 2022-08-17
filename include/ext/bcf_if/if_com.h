#ifndef __IBC_IF_COMMON__
#define __IBC_IF_COMMON__

enum e_bd_mode
{
   e_bd_mode_none = 0x00, // 최초 기동 상태.. 먼저 붙는 쪽이 active로 전이
   e_bd_mode_active = 0x01,
   e_bd_mode_standby = 0x02,
   e_bd_mode_standalone = 0x04,
};

enum e_init_type
{
   e_init_type_alloc = 1,
   e_init_type_remove = 2,
};

enum e_bd_ip_mode
{
   e_bd_ip_alloc = 0x01,
   e_bd_ip_fail = 0x02,
   e_bd_ip_not_alloc = 0x04, // 최초 기동시
};

#define MAX_LEN_TG_NIC  32

typedef struct st_init_addr
{
   char szNIC[MAX_LEN_TG_NIC];
   unsigned int  ver;
   unsigned char ucPoolIndex;
   unsigned char ucReserved[3];
   union
   {
      struct     in_addr  v4;
      struct     in6_addr v6;
   };

   //ordering
   void hton()
   {
      ver = htonl(ver);
   }
   void ntoh()
   {
      ver = ntohl(ver);
   }
   char* str(char* pbuf, int nlen)
   {
      char szAddr[64]; szAddr[0] = '\0';

      if (ver == 4)
      {
         inet_ntop(AF_INET, &v4, szAddr, sizeof(szAddr));
      }
      else if (ver == 6)
      {
         inet_ntop(AF_INET6, &v6, szAddr, sizeof(szAddr));
      }

      snprintf(pbuf, nlen, "nic=%s ver=%d pidx=%d ip=%s", szNIC, ver, ucPoolIndex, szAddr);
      return pbuf;
   }
} st_init_addr;

#define MAX_TG_LOC_ADDR 10

// tg_init_req와 msrp_init_req 는 구조체 틀림.
#if 0
typedef struct st_init_req
{
   unsigned char ucType; // 1:alloc 2:remove
   unsigned char ucIPCnt;
   unsigned char ucBdMode; // active or redundancy
   unsigned char ucRecoverBoard; // 절체시에 원래 보드 Index
   unsigned char ucMMI; // 응답시에 그대로 리턴
   unsigned char ucReserved[3];
   struct st_init_addr ip[MAX_TG_LOC_ADDR];

   void hton()
   {
      for(int id=0; id<ucIPCnt; id++)
         ip[id].hton();
   }

   void ntoh()
   {
      for(int id=0; id<ucIPCnt; id++)
         ip[id].ntoh();
   }

   char* str(char* pbuf, int nlen)
   {
      char szIP[128]; szIP[0]='\0';
      char szIPAll[1024]; szIPAll[0]='\0';
      for(int id=0; id<ucIPCnt; id++)
      {
         memset(szIP, 0, sizeof(szIP));
         ip[id].str(szIP, sizeof(szIP));
         snprintf(szIPAll+strlen(szIPAll), sizeof(szIPAll)-strlen(szIPAll), "(%d):%s ",
            id+1, szIP);
      }
      snprintf(pbuf, nlen, "type:%d mode:%d rbd:%d ipcnt:%d mmi:%d vip[%s] ",
         ucType, ucBdMode, ucRecoverBoard, ucIPCnt, ucMMI, szIPAll);
      return pbuf;
   }
} st_init_req;
#endif

typedef struct st_init_res
{
   unsigned char ucType; // 1:alloc 2:remove
   unsigned char ucRecoverBoard;
   unsigned char ucResult;
   unsigned char ucMMI;
   unsigned char ucReserved[4];

   void hton()
   {
   }
   void ntoh()
   {
   }

   char* str(char* pbuf, int nlen)
   {
      snprintf(pbuf+strlen(pbuf), nlen, "type:%d rbd:%d res:%d mmi:%d", 
         ucType, ucRecoverBoard, ucResult, ucMMI);
      return pbuf;
   }

} st_init_res;

typedef struct st_nic_info
{
   unsigned char ucPoolIndex;
   unsigned char ucResult;
   unsigned char ucReserved[6];
   char szNIC[MAX_LEN_TG_NIC];

   void hton()
   {
   }
   void ntoh()
   {
   }
   char* str(char* pbuf, int nlen)
   {
      snprintf(pbuf, nlen, "nic=%s pidx=%d res=%d", szNIC, ucPoolIndex, ucResult);
      return pbuf;
   }

} st_nic_info;

typedef struct st_hw_info
{
   unsigned char ucCpuUsage;
   unsigned char ucMemUsage;
   unsigned char ucNICCnt;
   unsigned char ucReserved[5];
   st_nic_info stNIC[MAX_TG_LOC_ADDR];

   void hton()
   {
      for(int i=0;i<ucNICCnt;i++)
         stNIC[i].hton();
   }
   void ntoh()
   {
      for(int i=0;i<ucNICCnt;i++)
         stNIC[i].ntoh();
   }

   char* str(char* pbuf, int nlen)
   {
      std::string strNIC="";
      char szTmp[128];

      for(int i=0; i<ucNICCnt;i++)
      {
         stNIC[i].str(szTmp, sizeof(szTmp));
         strNIC += szTmp;
         strNIC += " ";
      }
      snprintf(pbuf, nlen, "CPU:%d MEM:%d CNT:%d NIC(%s)",
         ucCpuUsage, ucMemUsage, ucNICCnt, strNIC.c_str());
      return pbuf;
   }

} st_hw_info;

#endif
