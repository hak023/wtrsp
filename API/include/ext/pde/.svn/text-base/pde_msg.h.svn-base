#ifndef _PDE_MSG_H_
#define _PDE_MSG_H_

#include "common_msg.h"
#include "pde_err.h"
#include <string.h>
#define MAX_PDEMSG_BODY_SIZE 8192
#define DIA_MAX_SESSION_ID_LEN 256
#define DIA_MAX_COMM_ID_LEN 128
#define DIA_MAX_MDN_LEN 32

enum PDE_COMMON_TYPE {
	PDE_INTERNAL = 0xF104
};

enum PDE_COMMON_SUBTYPE {
	PDE_TCP_CONN,
	PDE_TCP_DISC,
};

enum ePDE_TYPE{
	PDE_CAEX		= 0x0001,			//pde inside use msg-type
	PDE_ACCOUNT = 0x0002,
	PDE_ACCOUNT_RETRANS = 0x0003,	// pde inside use msg-type
	PDE_DW		= 0x0004,			// pde inside use msg-type
	PDE_DP		= 0x0005,			// pde inside use msg-type
	PDE_ACCOUNT_FD = 0x0006			// pde inside use msg-type
};

enum ePDE_STYPE{
	PDEST_INFO	= 0x0000,
	PDEST_REQ	= 0x0001,
	PDEST_RES	= 0x0002
};

typedef struct
{
	unsigned int nEnable; 		//0:disable, 1:enable
	unsigned int nTraceId;		//Trace Set Id
	char szCallId[128];			//Call Id
	unsigned int nFromNodeId;	//From Node Id
	unsigned int nToNodeId;		//To Node Id
	unsigned int nLevel;			//0:Normal, 1:DETAIL
	unsigned int nLocation;		//Host Location(0:Internal, 1:External)
	unsigned int nReserved[2];
	void hton()
	{
		nEnable = htonl(nEnable);
		nTraceId = htonl(nTraceId);
		nFromNodeId = htonl(nFromNodeId);
		nToNodeId = htonl(nToNodeId);
		nLevel = htonl(nLevel);
		nLocation = htonl(nLocation);
	}
	void ntoh()
	{
		nEnable = ntohl(nEnable);
		nTraceId = ntohl(nTraceId);
		nFromNodeId = ntohl(nFromNodeId);
		nToNodeId = ntohl(nToNodeId);
		nLevel = ntohl(nLevel);
		nLocation = ntohl(nLocation);
	}
	char * str(char* strBuff,int nBuffLen)
	{
		sprintf(strBuff,"[TRC:en:%d id:%d from:%d to:%d lvl:%d loc:%d] ",
			nEnable,nTraceId,nFromNodeId,nToNodeId,nLevel,nLocation);
		return strBuff;
	}
}stPDETraceInfo;

typedef struct
{
	char szSessionId[DIA_MAX_SESSION_ID_LEN];
	unsigned short usRecordType;
	unsigned short usRecordNum;
	unsigned int uiAssocIdx;
	unsigned int uiHbhId;
	unsigned int uiEteId; 
	void hton(){
		usRecordType = htons(usRecordType);
		usRecordNum = htons(usRecordNum);
		uiAssocIdx = htonl(uiAssocIdx);
		uiHbhId = htonl(uiHbhId);
		uiEteId = htonl(uiEteId);
	}
	void ntoh(){
		usRecordType = htons(usRecordType);
		usRecordNum = htons(usRecordNum);
		uiAssocIdx = ntohl(uiAssocIdx);
		uiHbhId = ntohl(uiHbhId);
		uiEteId = ntohl(uiEteId);
	}
	char * str(char* strBuff,int nBuffLen)
	{
		sprintf(strBuff,"rt:%d rn:%d ac_idx:%d hbh:%d ete:%d ",usRecordType,usRecordNum,uiAssocIdx,uiHbhId,uiEteId);
		return strBuff;
	}
}stAnswer;

typedef struct {
   enum _flag{
      ORIG_IOI = 0x1,
      TERM_IOI = 0x2,
      CAUSE_CODE = 0x4,
      MAX_FLAG
   };

	/* Mandatory field */
   unsigned int flag;//enum _flag
   char szSIPMethod[32];//INVITE,BYE etc
	unsigned short usRoleOfNode;//0:ORIGINATION_ROLE, 1:TERMINATING_ROLE, 2:PROXY_ROLE, 3:PROXY B2BUA_ROLE 4:IBCF
	unsigned short usNodeFunctionality;//0:S-CSCF, 1:P-CSCF, 2:I-CSCF, 3:MRCF, 4:MGCF, 5:BGCF, 6:AS, 7:IBCF
   char szCallingPartyAddress[DIA_MAX_COMM_ID_LEN]; //tel:01021462016@node61.com
   char szCalledPartyAddress[DIA_MAX_COMM_ID_LEN];  //tel:+827087196215
   char szAlternateChargedPartyAddress[DIA_MAX_COMM_ID_LEN];//tel:01021462016@node61.com
	char szRequestPartyAddress[DIA_MAX_COMM_ID_LEN];//01022228880@node61.com
   char szIMSChargingId[DIA_MAX_COMM_ID_LEN];//ibcf-pi.sktims.net-32-2836-1389581192-6a86aca0-1
   unsigned int uiServiceType; //101:선별통화 102:Group통화 103:법인통화
    //203 : MIM 3.0 , 205 : RCS채팅, 207 : RCS통화중 공유
    //301 : Non-SIP MIM, 302 : GTS, 303 : MRC, 304 : SIP영상전화,
    //305 : SIP MIM, 306 : IBC Call, 307 : VoIP, 309 : 영상통화 EV
    //310 : VoLTE, 401 : Video Sharing, 801 : Beacon서비스, 802 : 파자마 5
   unsigned int uiServiceSubType;
   //3101 : VoLTE 음성호
   //3102 : VoLTE 영상호
   //3114 : VoLTE Audio->Video 호전환
   //3121 : 영상사서함

	/* Optional field */
   char szOrigIOI[DIA_MAX_COMM_ID_LEN];//home1.net
   char szTermIOI[DIA_MAX_COMM_ID_LEN];//ktf.com
   int iCauseCode;//SIP Reason Code ( ex> -200 )
   void hton(){
		flag = htonl(flag);
		usRoleOfNode = htons(usRoleOfNode);
		usNodeFunctionality = htons(usNodeFunctionality);
		uiServiceType = htonl(uiServiceType);
		uiServiceSubType = htonl(uiServiceSubType);
		iCauseCode = htonl(iCauseCode);
	}
	void ntoh(){
		flag = ntohl(flag);
		usRoleOfNode = ntohs(usRoleOfNode);
		usNodeFunctionality = ntohs(usNodeFunctionality);
		uiServiceType = ntohl(uiServiceType);
		uiServiceSubType = ntohl(uiServiceSubType);
		iCauseCode = ntohl(iCauseCode);
	}
	char * str(char * strBuff,int nBuffLen)
	{
		char szTmp[128];
		sprintf(strBuff,"f:0x%x sm:%s role:%d nf:%d cra:%s cda:%s acpa:%s rpa:%s icid:%s stype:%d sstype:%d ",
			flag,szSIPMethod,usRoleOfNode,usNodeFunctionality,szCallingPartyAddress,szCalledPartyAddress,szAlternateChargedPartyAddress,
			szRequestPartyAddress,szIMSChargingId,uiServiceType,uiServiceSubType);
		if((flag & ORIG_IOI)==ORIG_IOI)
		{
			sprintf(szTmp,"oi:%s ",szOrigIOI);strcat(strBuff,szTmp);
		}
		if((flag & TERM_IOI)==TERM_IOI)
		{
			sprintf(szTmp,"ti:%s ",szTermIOI);strcat(strBuff,szTmp);
		}
		if((flag & CAUSE_CODE)==CAUSE_CODE)
		{
			sprintf(szTmp,"cc:%d ",iCauseCode);strcat(strBuff,szTmp);
		}
		return strBuff;
	}
}stSrvInfo;

typedef struct {
   enum _flag{
      ROAMING_IND = 0x1,
      MAX_FLAG
   };
   unsigned int flag;

	/* optional field */
   unsigned int nRoamingInd;
	void hton(){
		flag = htonl(flag);
		nRoamingInd = htonl(nRoamingInd);
	}
	void ntoh(){
		flag = ntohl(flag);
		nRoamingInd = ntohl(nRoamingInd);
	}
	char * str(char* strBuff,int nBuffLen)
	{
		//if((flag & ROAMING_IND)==ROAMING_IND)
		{
			sprintf(strBuff,"ri:%d ",nRoamingInd);
		}
		return strBuff;
	}
}stSKTSInfo;

typedef struct {
   enum _flag{
      DIAL_NUM = 0x1,
      MAX_FLAG
   };
   unsigned int flag;

	/*optional field */
   char szDialedNum[DIA_MAX_COMM_ID_LEN];
	void hton(){
		flag = htonl(flag);
	}
	void ntoh(){
		flag = ntohl(flag);
	}
	char* str(char* strBuff,int nBuffLen)
	{
		//if((flag & DIAL_NUM)==DIAL_NUM)
		{
			sprintf(strBuff,"dn:%s ",szDialedNum);
		}
		return strBuff;
	}
}stSKBSInfo;

typedef struct {
   enum _flag{
      CALL_RESULT_TYPE = 0x1,
      DETAIL_CAUSE_CODE = 0x2,
      SKT_SINFO = 0x4,
      SKB_SINFO = 0x8,
      MAX_FLAG
   };
	enum _rec_type{
		NONE_RECORD = 0,
		EVENT_RECORD = 1,
		START_RECORD = 2,
		INTERIM_RECORD = 3,
		STOP_RECORD = 4
	};
	/* mandatory field*/
	unsigned int flag;//enum _flag
	unsigned int uiTransactionId;
	char szCaller[DIA_MAX_MDN_LEN];//0102146201661
	char szCalled[DIA_MAX_MDN_LEN];//827087196215
	char szSessionId[DIA_MAX_SESSION_ID_LEN];//skt1-6974@192.168.1.61
   unsigned short usRecordType; /* _rec_type */
	unsigned short usRecordNum;
   char szUserName[DIA_MAX_COMM_ID_LEN];//tel:+827087196215
   char szEventTimestamp[32];//3623056547
	char szSubscriptionIdData[DIA_MAX_SESSION_ID_LEN];//01022228880@node61.com
	unsigned int uiSubscriptionIdType;//0:E164, 1:IMSI, 2:SIP_URI, 3:NAI, 4:PRIVATE
   stSrvInfo srv_info;
   char szStartTime[16];//201410232135479
   char szEndTime[16];//201410232135479
   unsigned short usOriginalDomainCode;// 011: SKT, 016:KTF, 019:LGT, 901:SKB
   unsigned short usTermDomainCode;// 011: SKT, 016:KTF, 019:LGT, 901:SKB
   unsigned int uiChargingInd;// //0:비과금 대상, 1:과금 대상 
   char szAccessNetworkInfo[DIA_MAX_COMM_ID_LEN];//3GPP-E-UTRAN-FDD;utran-cell-id-3gpp=4500641fe2108207

	/*Optional field*/
	unsigned int uiRetrans;//0: no , 0 < retrans_count
	int iCallResultType;//0:success, 5:Fail
	int iDetailCauseCode; //SIP reason code외의 장비에서 자체적으로 기록하는 error code ( ex -200 )
   stSKTSInfo skt_sinfo;
   stSKBSInfo skb_sinfo;
	//stPDETraceInfo trc_info;
   void hton(){
		uiTransactionId = htonl(uiTransactionId);
		usRecordType = htons(usRecordType);
		usRecordNum = htons(usRecordNum);
		uiSubscriptionIdType = htonl(uiSubscriptionIdType);
		usOriginalDomainCode = htons(usOriginalDomainCode);
		usTermDomainCode = htons(usTermDomainCode);
		srv_info.hton();
		uiChargingInd = htonl(uiChargingInd);
		uiRetrans = htonl(uiRetrans);
		iCallResultType = htonl(iCallResultType);
		iDetailCauseCode = htonl(iDetailCauseCode);
		if((flag & SKT_SINFO)==SKT_SINFO) skt_sinfo.hton();
		if((flag & SKB_SINFO)==SKB_SINFO) skb_sinfo.hton();
		flag = htonl(flag);
		//trc_info.hton();
	}
	void ntoh(){
		flag = ntohl(flag);
		uiTransactionId = ntohl(uiTransactionId);
		usRecordType = ntohs(usRecordType);
		usRecordNum = ntohs(usRecordNum);
		uiSubscriptionIdType = ntohl(uiSubscriptionIdType);
		usOriginalDomainCode = ntohs(usOriginalDomainCode);
		usTermDomainCode = ntohs(usTermDomainCode);
		srv_info.ntoh();
		uiRetrans = ntohl(uiRetrans);
		iCallResultType = ntohl(iCallResultType);
		iDetailCauseCode = ntohl(iDetailCauseCode);
		if((flag & SKT_SINFO)==SKT_SINFO) skt_sinfo.ntoh();
		if((flag & SKB_SINFO)==SKB_SINFO) skb_sinfo.ntoh();
		//trc_info.ntoh();
	}
	char * str(char * strBuff,int nBuffLen)
	{
		char szTmp[128];
		char szSrvInfo[4096];szSrvInfo[0]='\0';
		char szSktInfo[4096];szSktInfo[0]='\0';
		char szSkbInfo[4096];szSkbInfo[0]='\0';
		//char szTrcInfo[4096];szTrcInfo[0]='\0';
		srv_info.str(szSrvInfo,4096);
		if((flag & SKT_SINFO)==SKT_SINFO) skt_sinfo.str(szSktInfo,4096);
		if((flag & SKB_SINFO)==SKB_SINFO) skb_sinfo.str(szSkbInfo,4096);
		sprintf(strBuff,"flag:0x%x tid:0x%x cr:%s cd:%s rtran:%d sid:%s rt:%d rn:%d uname:%s ets:%s srvinfo:%s stime:%s etime:%s odc:%d tdc:%d cind:%d ani:%s ",
			flag,uiTransactionId,szCaller,szCalled,uiRetrans,szSessionId,usRecordType,usRecordNum,szUserName,szEventTimestamp,szSrvInfo,szStartTime,szEndTime,usOriginalDomainCode,
			usTermDomainCode,uiChargingInd,szAccessNetworkInfo);
		if((flag & CALL_RESULT_TYPE)==CALL_RESULT_TYPE)
		{
			sprintf(szTmp,"cret:%d ",iCallResultType);strcat(strBuff,szTmp);
		}
		if((flag & DETAIL_CAUSE_CODE)==DETAIL_CAUSE_CODE)
		{
			sprintf(szTmp,"dcc:%d ",iDetailCauseCode);strcat(strBuff,szTmp);
		}
		if(szSktInfo[0]!='\0') strcat(strBuff,szSktInfo);
		if(szSkbInfo[0]!='\0') strcat(strBuff,szSkbInfo);
		//trc_info.str(szTrcInfo,4096); strcat(strBuff,szTrcInfo);
		return strBuff;
	}
}stAccReq;

typedef struct
{
	enum _rec_type{
		NONE_RECORD = 0,
		EVENT_RECORD = 1,
		START_RECORD = 2,
		INTERIM_RECORD = 3,
		STOP_RECORD = 4
	};
	unsigned int uiTransactionId;
	char szSessionId[DIA_MAX_SESSION_ID_LEN];
   unsigned short usRecordType; /*_rec_type */
   unsigned short usRecordNum;
	void hton(){
		uiTransactionId = htonl(uiTransactionId);
		usRecordType = htons(usRecordType);
		usRecordNum = htons(usRecordNum);
	}
	void ntoh(){
		uiTransactionId = ntohl(uiTransactionId);
		usRecordType = ntohs(usRecordType);
		usRecordNum = ntohs(usRecordNum);
	}
	char * str(char * strBuff,int nBuffLen)
	{
		sprintf(strBuff,"tid:0x%x sid:%s rtype:%d rnum:%d",
			uiTransactionId,szSessionId,usRecordType,usRecordNum);
		return strBuff;
	}
}stAccRes;

typedef struct
{
	stAccReq accreq;
	unsigned int uiCompSesId;
	char szFileName[256];
	void hton(){
		uiCompSesId = htonl(uiCompSesId);
		accreq.hton();
	}
	void ntoh(){
		uiCompSesId = ntohl(uiCompSesId);
		accreq.ntoh();
	}
	char * str(char *strBuff,int nBuffLen)
	{
		char szTmp[10240];
		sprintf(strBuff,"%s id:%d filename:%s",accreq.str(szTmp,10240),uiCompSesId,szFileName);
		return strBuff;
	}
}stAccReqFD;

struct stPDEMsg{
	SVCComMsgHdr hdr;
	char body[MAX_PDEMSG_BODY_SIZE];
	stPDEMsg(){memset(this,0x00,sizeof(stPDEMsg));}
	void hton_body(){
		switch(hdr.uiType)
		{
		case PDE_ACCOUNT:
			if(hdr.uiSubType == PDEST_REQ)
			{
				((stAccReq*)body)->hton();
			}
			else if(hdr.uiSubType == PDEST_RES)
			{
				((stAccRes*)body)->hton();
			}
		break;
		default:
		break;
		}
	}
	void ntoh_body(){
		switch(hdr.uiType)
		{
		case PDE_ACCOUNT:
			if(hdr.uiSubType == PDEST_REQ)
			{
				((stAccReq*)body)->ntoh();
			}
			else if(hdr.uiSubType == PDEST_RES)
			{
				((stAccRes*)body)->ntoh();
			}
		break;
		default:
		break;
		}
	}
	void hton()
	{
		hton_body();
		hdr.hton();
	}
	void ntoh()
	{
		hdr.ntoh();
		ntoh_body();
	}

	inline char* getstr_type(int nType,char* szType)
	{
		switch(nType)
		{
		case PDE_CAEX:
			sprintf(szType,"PDE_CAEX");
		break;
		case PDE_ACCOUNT:
			sprintf(szType,"PDE_ACCOUNT");
		break;
		case PDE_ACCOUNT_RETRANS:
			sprintf(szType,"PDE_ACCOUNT_RETRANS");
		break;
		case PDE_DW:
			sprintf(szType,"PDE_DW");
		break;
		case PDE_DP:
			sprintf(szType,"PDE_DP");
		break;
		case PDE_ACCOUNT_FD:
			sprintf(szType,"PDE_ACCOUNT_FD");
		break;
		case SVC_ALIVE:
			sprintf(szType,"ALIVE");
		break;
		case SVC_SETUP_APPS:
			sprintf(szType,"SETUP_APPS");
		break;
		default:
			sprintf(szType,"Undefine");
		break;
		}
		return szType;
	}
	inline char* getstr_subtype(int nSubType,char *szSubType)
	{
		switch(nSubType)
		{
		case PDEST_REQ: sprintf(szSubType,"REQ"); break;
		case PDEST_RES: sprintf(szSubType,"RES"); break;
		case PDEST_INFO: sprintf(szSubType,"INFO"); break;
		default: sprintf(szSubType,"Undefine"); break;
		}
		return szSubType;
	}
	char* str(char* strBuff,int nBufLen) {
		char szType[32];
		char szSubType[32];
		char szHdr[256];
		char szBody[10240];
		szHdr[0]='\0';
		szBody[0]='\0';
		getstr_type(hdr.uiType,szType);
		getstr_subtype(hdr.uiSubType,szSubType);
		snprintf(szHdr, 255, "[LEN:%u][DESC:%s][MSG:%s-%s][A:%d_S:%d C:%d_S:%u][res:%#x(%s)]",
      	hdr.uiMsgLen, hdr.szSesDesc, szType, szSubType,
      	hdr.uiAsId, hdr.uiAsSesId, hdr.uiCompId, hdr.uiCompSesId,
      	hdr.uiReasonCode,fnPdeApiStringError(hdr.uiReasonCode));
		switch(hdr.uiType)
		{
			case PDE_ACCOUNT:
				if(hdr.uiSubType == PDEST_REQ)
				{
					((stAccReq*)body)->str(szBody,sizeof(szBody));
				}
				else if(hdr.uiSubType == PDEST_RES)
				{
					((stAccRes*)body)->str(szBody,sizeof(szBody));
				}
			break;
			case PDE_ACCOUNT_FD:
				if(hdr.uiSubType == PDEST_REQ)
				{
					((stAccReqFD*)body)->str(szBody,sizeof(szBody));
				}
			default:
			break;
		}
		sprintf(strBuff,"HDR(%d):%s BODY(%d):",(int)sizeof(SVCComMsgHdr),szHdr,(int)(hdr.uiMsgLen-sizeof(SVCComMsgHdr)));
		strncat(strBuff,szBody,nBufLen-strlen(strBuff));
		return strBuff;
	}
};


#endif
/*
 * EOF
 */
