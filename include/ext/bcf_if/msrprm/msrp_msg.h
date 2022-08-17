/**
 * @file  msrp_msg.hpp
 * msrp message type
 *
 * @attention Copyright (c) 2013 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : IBCF
 * @par Last Changed : 
 * @par        2013.06.07   minimei7           first written
 */

#ifndef __MSRP_MESSAGE_TYPE__
#define __MSRP_MESSAGE_TYPE__

#include "common_msg.h"
#include "if_com.h"

namespace MSRP
{

#if 0
enum e_bd_mode
{
	e_bd_mode_active = 0x01,
	e_bd_mode_redundancy = 0x02,
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
#endif

enum e_pool_mode
{
	e_pool_mode_internal = 0x01,
	e_pool_mode_external = 0x02,
};

enum e_setup_mode
{
	e_setup_actpass = 1,
	e_setup_active = 2,
	e_setup_passive = 3,
};

typedef struct st_msrp_ip_info 
{
	enum {e_max_ip = 32};
	char           szip[e_max_ip];
	unsigned int   uiport;
	unsigned char  ucIPVer;
	unsigned char  ucSetup; // 0:NONE 1:ACTPASS 2:ACTIVE 3:PASSIVE
	char           cReserved[2];

	// ordering
	void hton(){
		uiport=htonl(uiport);
		return;
	}
	void ntoh(){
		uiport=ntohl(uiport);
		return;
	}

	char* str(char* pbuf, int len)
	{
		char szSetup[16];
		memset(szSetup, 0, sizeof(szSetup));

		switch(ucSetup)
		{
			case e_setup_actpass: snprintf(szSetup, sizeof(szSetup), "ACTPASS"); break;
			case e_setup_active: snprintf(szSetup, sizeof(szSetup), "ACTIVE"); break;
			case e_setup_passive: snprintf(szSetup, sizeof(szSetup), "PASSIVE"); break;
			default: snprintf(szSetup, sizeof(szSetup), "NONE(%d)", ucSetup); break;
		}

		snprintf(pbuf, len-1, "IPVer=%d Setup=%s IP=%s Port=%d", 
			ucIPVer, szSetup, szip, uiport);
		return pbuf;
	}
} st_msrp_ip_info, *pst_msrp_ip_info;

struct st_msrp_sdp_info 
{
	enum {e_max_opt=256};
	st_msrp_ip_info ip;
	char           szPath[e_max_opt];         //sdp Path

	// ordering
	void hton(){
		ip.hton();
		return;
	}
	void ntoh(){
		ip.ntoh();
		return;
	}

	char* str(char* pbuf, int len)
	{
		char szIPInfo[512];
		memset(szIPInfo, 0, sizeof(szIPInfo));

		ip.str(szIPInfo, sizeof(szIPInfo));
		snprintf(pbuf, len-1, "%s Path=%s", szIPInfo, szPath);
		return pbuf;
	}
};

//e_msrp_add_req, mod req 
typedef struct st_msrp_req
{
	enum {e_max_min = 32, e_max_ip = 32, e_max_opt=256};
	char           szcaller[e_max_min];       //caller min
	char           szcalled[e_max_min];       //called min
	int            caller_trte;
	int            called_trte;

	char           cMode; 
	char           cCrOwnRoute; 
	char           cCdOwnRoute; 
	unsigned char  ucreserved[5]; 
	st_msrp_sdp_info  stsdp_req;
	st_msrp_sdp_info  stAlloc; 

	char           szAcceptTypes[e_max_opt];  //sdp Accept-Types
	char           szFileName[e_max_opt];     //sdp file-selector filename
	char           szFileType[e_max_opt];     //sdp file-selector types
	char           szTransferID[e_max_opt];   //sdp file-transfer-id

	char           szcallid[128];   // for trace. 2014.07.15 8->128  
	SVCTrace_t           Trace;

	//ordering
	void hton(){
		caller_trte = htonl(caller_trte);
		called_trte = htonl(called_trte);
		stsdp_req.hton();
		Trace.hton();
      return;
	}
	void ntoh(){
		caller_trte = ntohl(caller_trte);
		called_trte = ntohl(called_trte);
		stsdp_req.ntoh();
		Trace.ntoh();
		return;
	}

	char* str(char* pbuf, int len)
	{
		char szSdp[128]; szSdp[0] = '\0';
		char szAlloc[128]; szAlloc[0] = '\0';
		char szMode[16]; szMode[0] = '\0';
		char szTrace[1024]; szTrace[0] = '\0';

		stsdp_req.str(szSdp, sizeof(szSdp));
		stAlloc.str(szAlloc, sizeof(szAlloc));
		Trace.str(szTrace, sizeof(szTrace));

		switch(cMode)
		{
			case 1: snprintf(szMode, sizeof(szMode), "SENDONLY"); break;
			case 2: snprintf(szMode, sizeof(szMode), "RECVONLY"); break;
			case 3: snprintf(szMode, sizeof(szMode), "SENDRECV"); break;
			default: snprintf(szMode, sizeof(szMode), "NOTSET(%d)", cMode); break;
		}

		snprintf(pbuf, len-1, 
			"Caller=%s Called=%s Trte(Cr=%d Cd=%d) Own(Cr=%d Cd=%d) Mode=%s SDP(%s) "
			"Alloc(%s) Accept-Types=%s FileName=%s FileType=%s TransferID=%s CallId=%s Trace{%s}", 
			szcaller, szcalled, caller_trte, called_trte, cCrOwnRoute, cCdOwnRoute, szMode, szSdp, 
			szAlloc, szAcceptTypes, szFileName, szFileType, szTransferID, szcallid, szTrace);
		return pbuf;
	}

} st_msrp_add_req, st_msrp_mod_req;

//e_msrp_add_res, mod_res
typedef struct st_msrp_res
{
	enum {e_max_opt=256};
	st_msrp_sdp_info  stAlloc;

	void hton(){
		stAlloc.hton();
	}
	void ntoh(){
		stAlloc.ntoh();
	}

	char* str(char* pbuf, int len)
	{
		char szAlloc[256]; szAlloc[0] = '\0';

		stAlloc.str(szAlloc, sizeof(szAlloc));
		snprintf(pbuf, len-1, "ALLOC(%s)", szAlloc);
		return pbuf;
	}
} st_msrp_add_res, st_msrp_mod_res;

//e_msrp_recover_req
typedef struct st_msrp_recover_req
{
	enum {e_max_min = 32, e_max_ip = 32, e_max_opt=256};
	char           szcaller[e_max_min];       //caller min
	char           szcalled[e_max_min];       //called min

	st_msrp_sdp_info  stsdp_cr;
	st_msrp_sdp_info  stsdp_cd;
	st_msrp_sdp_info  stalloc_cr; 
	st_msrp_sdp_info  stalloc_cd; 

	//ordering
	void hton(){
		stsdp_cr.hton();
		stsdp_cd.hton();
		stalloc_cr.hton();
		stalloc_cd.hton();
      return;
	}
	void ntoh(){
		stsdp_cr.ntoh();
		stsdp_cd.ntoh();
		stalloc_cr.ntoh();
		stalloc_cd.ntoh();
		return;
	}

	char* str(char* pbuf, int len)
	{
		char szSdp_Cr[256]; szSdp_Cr[0] = '\0';
		char szSdp_Cd[256]; szSdp_Cd[0] = '\0';
		char szAlloc_Cr[256]; szAlloc_Cr[0] = '\0';
		char szAlloc_Cd[256]; szAlloc_Cd[0] = '\0';

		stsdp_cr.str(szSdp_Cr, sizeof(szSdp_Cr));
		stsdp_cd.str(szSdp_Cd, sizeof(szSdp_Cd));
		stalloc_cr.str(szAlloc_Cr, sizeof(szAlloc_Cr));
		stalloc_cd.str(szAlloc_Cd, sizeof(szAlloc_Cd));

		snprintf(pbuf, len-1, 
			"Caller=%s Called=%s SDP_CR(%s) SDP_CD(%s) "
			"ALLOC_CR(%s) ALLOC_CD(%s)", 
			szcaller, szcalled, szSdp_Cr, szSdp_Cd,
			szAlloc_Cr, szAlloc_Cd);
		return pbuf;
	}

} st_msrp_recover_req;


typedef struct st_msrp_stat
{
	unsigned int stime;
	unsigned int etime;

	unsigned char ucCallerMsrpCnt;
	unsigned char ucCalledMsrpCnt;
	unsigned char ucBoard;
	unsigned char ucReserved[5];

	unsigned int uiCallerSendBytes;
	unsigned int uiCallerRecvBytes;
	unsigned int uiCalledSendBytes;
	unsigned int uiCalledRecvBytes;

	st_msrp_ip_info sdp_cr;
	st_msrp_ip_info sdp_cd;
	st_msrp_ip_info alloc_cr;
	st_msrp_ip_info alloc_cd;

	void hton(){
		stime = htonl(stime);
		etime = htonl(etime);
		uiCallerSendBytes = htonl(uiCallerSendBytes);
		uiCallerRecvBytes = htonl(uiCallerRecvBytes);
		uiCalledSendBytes = htonl(uiCalledSendBytes);
		uiCalledRecvBytes = htonl(uiCalledRecvBytes);
		sdp_cr.hton();
		sdp_cd.hton();
		alloc_cr.hton();
		alloc_cd.hton();
	}
	void ntoh(){
		stime = ntohl(stime);
		etime = ntohl(etime);
		uiCallerSendBytes = ntohl(uiCallerSendBytes);
		uiCallerRecvBytes = ntohl(uiCallerRecvBytes);
		uiCalledSendBytes = ntohl(uiCalledSendBytes);
		uiCalledRecvBytes = ntohl(uiCalledRecvBytes);
		sdp_cr.ntoh();
		sdp_cd.ntoh();
		alloc_cr.ntoh();
		alloc_cd.ntoh();
	}
	char* str(char* pBuf, int nLen) {
		char szSdp_Cr[256]; szSdp_Cr[0] = '\0';
		char szSdp_Cd[256]; szSdp_Cd[0] = '\0';
		char szAlloc_Cr[256]; szAlloc_Cr[0] = '\0';
		char szAlloc_Cd[256]; szAlloc_Cd[0] = '\0';

		sdp_cr.str(szSdp_Cr, sizeof(szSdp_Cr));
		sdp_cd.str(szSdp_Cd, sizeof(szSdp_Cd));
		alloc_cr.str(szAlloc_Cr, sizeof(szAlloc_Cr));
		alloc_cd.str(szAlloc_Cd, sizeof(szAlloc_Cd));

		snprintf(pBuf, nLen-1,
			"STIME=%u, ETIME=%u, BOARD=%d, Caller(SendBytes=%d,RecvBytes=%d), "
			"Called(SendBytes=%d,RecvBytes=%d) SDP_CR:%s SDP_CD:%s ALLOC_CR:%s ALLOC_CD:%s",
			stime, etime, ucBoard, uiCallerSendBytes, uiCallerRecvBytes,
			uiCalledSendBytes, uiCalledRecvBytes, szSdp_Cr, szSdp_Cd, szAlloc_Cr, szAlloc_Cd);
		return pBuf;
	}
} st_msrp_stat, *pst_msrp_stat;

typedef st_msrp_stat st_msrp_del_res;
typedef st_msrp_stat st_msrp_closed_ses;
typedef st_msrp_stat st_msrp_alive;
typedef st_msrp_stat st_msrp_netfail;

#if 0
#define MAX_LEN_MSRP_NIC  32

typedef struct st_init_addr
{
   char szNIC[MAX_LEN_MSRP_NIC];
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
#endif

#if 0
typedef struct st_msrp_nic_info
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
#endif

#if 1
typedef struct st_msrp_init_req
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
} st_msrp_init_req;

typedef st_init_res st_msrp_init_res;

#if 0
typedef struct st_msrp_init_res
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

} st_msrp_init_res;
#endif

#endif

#if 0
typedef struct st_msrp_hw_info
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

} st_msrp_hw_info;
#endif

typedef struct st_msrp_status_report
{
	unsigned char ucStatus; // e_bd_mode_active, standby
	unsigned char ucReserved[7];

	void hton()
	{
	}
	void ntoh()
	{
	}

	char* str(char* pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "status:%d", ucStatus);
		return pbuf;
	}

} st_msrp_status_report;

//defines msrp board message type
enum e_msrp_type
{
	e_msrp_add_req         =  0x0101,  //request
	e_msrp_add_res         =  0x0102,  //response
	e_msrp_mod_req         =  0x0103,  //request
	e_msrp_mod_res         =  0x0104,  //response
	e_msrp_del_req         =  0x0105,  //request
	e_msrp_del_res         =  0x0106,  //response
	e_msrp_closed_ses      =  0x0108,  //Info (rm->as, msrpa->rm) 

	e_msrp_alive           =  0x0110,  //Session Alive
	e_msrp_net_fail        =  0x0111,  //Netfail Noti
	e_msrp_hw_info         =  0x0112,  //cpu, mem, nic info
	e_msrp_status_report   =  0x0113,  //msrpa->tgas, active and standby report

	e_msrp_init_req        =  0x0121,  //request
	e_msrp_init_res        =  0x0122,  //response
	e_msrp_recover_req     =  0x0123,  //request
	e_msrp_recover_res     =  0x0124,  //request

	e_msrp_msg_max
};

//defines media board message subtype
enum e_msrp_stype 
{
	e_msrp_info = 0x00,
	e_msrp_req  = 0x01,
	e_msrp_res  = 0x02,
};

struct st_msrpmsg
{
	enum { e_maxbody=(1024*4) }; 
	SVCComMsgHdr hdr;   //commond header
	char         szbody[e_maxbody-sizeof(SVCComMsgHdr)];
   
	void hton(){
		ordering(true, true);
	}
	void ntoh(){
		ordering(false, true );
	}
	void hton_body(){
		ordering(true, false);
	}
	void ntoh_body(){
		ordering(false, false );
	}

	char* str(char* pbuf, int len)
	{
		char strHdr[256];
		char strBody[2048];

		strHdr[0] = '\0';
		strBody[0] = '\0';

		char sztype[32], szsubtype[32];
		sztype[0] = '\0';
		szsubtype[0] = '\0';

		switch(hdr.uiType)
		{
			case SVC_SETUP_APPS:
			{
				snprintf(sztype, sizeof(sztype), "SETUP_APPS");
			}
			break;
			case SVC_OOS_CHECK:
			{
				snprintf(sztype, sizeof(sztype), "OOS_CHECK");
			}
			break;
   		case e_msrp_add_req:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_ADD_REQ");
				((st_msrp_add_req*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_add_res:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_ADD_RES");
				((st_msrp_add_res*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_mod_req:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_MOD_REQ");
				((st_msrp_mod_req*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_mod_res:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_MOD_RES");
				((st_msrp_mod_res*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_del_req:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_DEL_REQ");
			}
			break;
   		case e_msrp_del_res:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_DEL_RES");
				((st_msrp_del_res*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_closed_ses:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_CLOSED_SES");
				((st_msrp_closed_ses*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_alive:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_ALIVE");
				((st_msrp_alive*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_net_fail:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_NETFAIL");
				((st_msrp_netfail*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_hw_info:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_HWINFO");
				((st_hw_info*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_status_report:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_STATUS_REPORT");
				((st_msrp_status_report*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_init_req:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_INIT_REQ");
				((st_msrp_init_req*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_init_res:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_INIT_RES");
				((st_msrp_init_res*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
   		case e_msrp_recover_req:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_RECOVER_REQ");
				((st_msrp_recover_req*)szbody)->str(strBody, sizeof(strBody));
			}
			break;
			case e_msrp_recover_res:
			{
				snprintf(sztype, sizeof(sztype), "MSRP_RECOVER_RES");
			}
			break;
			default:
				snprintf(sztype, sizeof(sztype), "undef:0x%x", hdr.uiType); break;
		}

		switch(hdr.uiSubType)
		{
			case e_msrp_info: snprintf(szsubtype, sizeof(szsubtype), "INFO"); break;
			case e_msrp_req: snprintf(szsubtype, sizeof(szsubtype), "REQ"); break;
			case e_msrp_res: snprintf(szsubtype, sizeof(szsubtype), "RES"); break;
			default: snprintf(szsubtype, sizeof(szsubtype), "undef:0x%x", hdr.uiSubType); break;
		}

		snprintf(strHdr, sizeof(strHdr), "[LEN:%u][DESC:%s][MSG:%s-%s][A:%d_S:%d C:%d_S:%u][res:%#x]",
			hdr.uiMsgLen, hdr.szSesDesc, sztype, szsubtype, hdr.uiAsId, hdr.uiAsSesId, hdr.uiCompId, hdr.uiCompSesId, hdr.uiReasonCode); 
		snprintf(pbuf, len-1, "HDR(%d):%s BODY(%d):%s",
			(int)sizeof(SVCComMsgHdr),
			strHdr,
			(int)(hdr.uiMsgLen-sizeof(SVCComMsgHdr)),
			strBody);
		return pbuf;
	}

protected:
   void ordering(bool bhton=true, bool bincluding_header=true){

		//if network to host
		if ( !bhton && bincluding_header ) hdr.ntoh();
		unsigned int uitype=hdr.uiType;
		unsigned int uistype=hdr.uiSubType;
		//if host to network
		if( bhton && bincluding_header ) hdr.hton();

		//body ordering
		switch(uitype){
			//common message type
			case SVC_SETUP_APPS:{
				SVCSetupApp* ptmp=(SVCSetupApp*)szbody;  
				if ( uistype == e_msrp_req ){
					if( bhton ) ptmp->hton();
					else ptmp->ntoh();
				}
			} 
			break;
			case e_msrp_add_req:{
				st_msrp_add_req* ptmp=(st_msrp_add_req*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
			case e_msrp_add_res:{
				st_msrp_add_res* ptmp=(st_msrp_add_res*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
			case e_msrp_mod_req:{
				st_msrp_mod_req* ptmp=(st_msrp_mod_req*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
			case e_msrp_mod_res:{
				st_msrp_mod_res* ptmp=(st_msrp_mod_res*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
			case e_msrp_del_res:{
				st_msrp_del_res* ptmp=(st_msrp_del_res*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
			case e_msrp_closed_ses:{
				st_msrp_closed_ses* ptmp=(st_msrp_closed_ses*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
			case e_msrp_alive:{
				st_msrp_alive* ptmp=(st_msrp_alive*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
			case e_msrp_net_fail:{
				st_msrp_netfail* ptmp=(st_msrp_netfail*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
			case e_msrp_hw_info:{
				st_hw_info* ptmp=(st_hw_info*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
			case e_msrp_init_req:{
				st_msrp_init_req* ptmp=(st_msrp_init_req*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
			case e_msrp_init_res:{
				st_msrp_init_res* ptmp=(st_msrp_init_res*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
			case e_msrp_recover_req:{
				st_msrp_recover_req* ptmp=(st_msrp_recover_req*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
		}
	}
};

////util function
inline char* getstr_msrpmethod(int ntype,char* str)
{
	sprintf(str,"NOT_DEF(0x%04X)",ntype);
	switch(ntype){
		case SVC_SETUP_APPS:    strcpy(str,"SETUP_APPS");         break;
		case e_msrp_add_req:    strcpy(str,"MSRP_ADD_REQ");       break;
		case e_msrp_add_res:    strcpy(str,"MSRP_ADD_RES");       break;
		case e_msrp_mod_req:    strcpy(str,"MSRP_MOD_REQ");       break;
		case e_msrp_mod_res:    strcpy(str,"MSRP_MOD_RES");       break;
		case e_msrp_del_req:    strcpy(str,"MSRP_DEL_REQ");       break;
		case e_msrp_del_res:    strcpy(str,"MSRP_DEL_RES");       break;
		case e_msrp_closed_ses: strcpy(str,"MSRP_CLOSED_SES");    break;
		case e_msrp_alive:      strcpy(str,"MSRP_ALIVE");         break;
		case e_msrp_net_fail:   strcpy(str,"MSRP_NETFAIL");       break;
		case e_msrp_init_req:   strcpy(str,"MSRP_INIT_REQ");       break;
		case e_msrp_init_res:   strcpy(str,"MSRP_INIT_RES");       break;
		case e_msrp_recover_req:    strcpy(str,"MSRP_RECOVER_REQ");       break;
		case e_msrp_recover_res:    strcpy(str,"MSRP_RECOVER_RES");       break;
	}
	return str;
}

inline char* getstr_msrphdr(st_msrpmsg* pmsg, char* szbuf, int nlen)
{
	char szMethod[32];
	char szSubType[32];
	SVCComMsgHdr *phdr = (SVCComMsgHdr*)&(pmsg->hdr);

	szMethod[0] = '\0';
	szSubType[0] = '\0';

	getstr_msrpmethod(phdr->uiType, szMethod);

	switch(phdr->uiSubType)
	{
		case e_msrp_info: snprintf(szSubType, sizeof(szSubType), "INFO"); break;
		case e_msrp_req: snprintf(szSubType, sizeof(szSubType), "REQ"); break;
		case e_msrp_res: snprintf(szSubType, sizeof(szSubType), "RES"); break;
		default: snprintf(szSubType, sizeof(szSubType), "undefine:0x%x", phdr->uiSubType); break;
	}

	snprintf(szbuf, nlen, "[LEN:%u][DESC:%s][MSG:%s-%s][A:%d_S:%d C:%d_S:%u][res:%#x]",
		phdr->uiMsgLen, phdr->szSesDesc, szMethod, szSubType, phdr->uiAsId, phdr->uiAsSesId, phdr->uiCompId, phdr->uiCompSesId, phdr->uiReasonCode); 
		
	return szbuf;
}

}//name space
#endif
