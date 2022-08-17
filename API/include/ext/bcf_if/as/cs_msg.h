/**
 * @file  cs_msg.h
 * the cs message header file
 *
 * @attention Copyright (c) 2013 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2013.01.18   minimei7      first written
 * @par 2013.05.08   minimei7      ver 2.0 update
 */

#ifndef _IBCF_CS_MESSAGE_H_
#define _IBCF_CS_MESSAGE_H_

#include "common_msg.h"


//namespace 
namespace NS_CS
{

#define RFAILED -1
#define ROK     0

#define ARM_IND_ALIVE 0
#define ARM_IND_KILL  1

#define   SDP_ERROR_NONE                   0
#define   SDP_ERROR_NO_RESOURCE           -1
#define   SDP_ERROR_UNKNOWN               -2
#define   SDP_ERROR_SERVER                -3

#define KCS1_PROC_ID       0x1011
#define KCS2_PROC_ID       0x1012
#define KCS3_PROC_ID       0x1013
#define KCS4_PROC_ID       0x1014
#define TRGW_PROC_ID      0x6061

typedef enum _Stack_CmsApiMMode_e {
    STACK_CMSAPI_MMODE_NONE,
    STACK_CMSAPI_MMODE_KSIP,
    STACK_CMSAPI_MMODE_KCS,   /* TrGW ? kCS */
    STACK_CMSAPI_MMODE_KLM,
    STACK_CMSAPI_MMODE_KMML,
    STACK_CMSAPI_MMODE_TRGW,  /* kCS ? TrGW */
    STACK_CMSAPI_MMODE_OMP,
    STACK_CMSAPI_MMODE_MAX_NUMS
} Stack_CmsApiMMode_e;

typedef enum _Stack_CmsApiMType_e {
    STACK_CMSAPI_MTYPE_NONE,
    STACK_CMSAPI_MTYPE_CONTROL,  /* Bind, Health check C38.=C */
    STACK_CMSAPI_MTYPE_PROC_MSG, /* 3;:N SDP(Session) C38.=C */
    STACK_CMSAPI_MTYPE_MGT_MSG,
    STACK_CMSAPI_MTYPE_ALARM_MSG,
    STACK_CMSAPI_MTYPE_MAX_NUMS
} Stack_CmsApiMType_e;

typedef enum _Stack_CmsApiSType_e {
/* MType@L STACK_CMSAPI_MTYPE_CONTROL @L8i 4Y@= ;g?k */
    STACK_CMSAPI_CTYPE_HB_REQ=100,
    STACK_CMSAPI_CTYPE_HB_RSP,
    STACK_CMSAPI_CTYPE_BIND_REQ,
    STACK_CMSAPI_CTYPE_BIND_RSP,
    STACK_CMSAPI_CTYPE_HA_IND,
/* STACK_CMSAPI_MTYPE_PROC_MSG (LM)*/
    STACK_CMSAPI_CTYPE_LM = 1000,
    STACK_CMSAPI_CTYPE_LM_INITDUMP_REQ,
    STACK_CMSAPI_CTYPE_LM_RULE, 
    STACK_CMSAPI_CTYPE_LM_RRS,
    STACK_CMSAPI_CTYPE_LM_RTS,
    STACK_CMSAPI_CTYPE_LM_TN,
    STACK_CMSAPI_CTYPE_LM_TRS,
    STACK_CMSAPI_CTYPE_LM_TT,
    STACK_CMSAPI_CTYPE_LM_SRS,
    STACK_CMSAPI_CTYPE_LM_ST,
    STACK_CMSAPI_CTYPE_LM_AT,
    STACK_CMSAPI_CTYPE_LM_LN,
/* MType@L STACK_CMSAPI_MTYPE_PROC_MSG @L8i 4Y@= ;g?k(TRGW) */
    STACK_CMSAPI_CTYPE_TRGW = 3000
} Stack_CmsApiSType_e;


typedef struct _TCP_ApiMsgHdr_t {
	unsigned int  MMode;            /* Stack_CmsApiMMode_e */
	unsigned int  MType;            /* Stack_CmsApiMtype_e */
	unsigned int  SType;            /* Stack_CmsApiStype_e */
	short  src_proc;
	short  dst_proc;
	unsigned int src_tag;
	unsigned int dst_tag;
	unsigned int  length;
	char* str(char *pBuf, int nLen)
	{
		char szstype[32];
		szstype[0]='\0';
		switch(SType)
		{
			case STACK_CMSAPI_CTYPE_HB_REQ: snprintf(szstype, sizeof(szstype),"HB_REQ"); break;
			case STACK_CMSAPI_CTYPE_HB_RSP: snprintf(szstype, sizeof(szstype),"HB_RSP"); break;
			case STACK_CMSAPI_CTYPE_BIND_REQ: snprintf(szstype, sizeof(szstype),"BIND_REQ"); break;
			case STACK_CMSAPI_CTYPE_BIND_RSP: snprintf(szstype, sizeof(szstype),"BIND_RSP"); break;
			case STACK_CMSAPI_CTYPE_HA_IND: snprintf(szstype, sizeof(szstype),"HA_IND"); break;
			case STACK_CMSAPI_CTYPE_TRGW: snprintf(szstype, sizeof(szstype),"TRGW"); break;
			default: snprintf(szstype, sizeof(szstype),"unknow stype:%d", SType); break;
		}
		snprintf(pBuf, nLen-1, "[S:%d D:%d LEN:%d %s]", src_tag, dst_tag, length, szstype);
		return pBuf;
	}
} TCP_ApiMsgHdr_t;

typedef enum _Stack_State_e {
	STACK_STATE_OOS = 0,
	STACK_STATE_PENDING1,
	STACK_STATE_PENDING2,
	STACK_STATE_ACTIVE_SECOND,
	STACK_STATE_ACTIVE_PRI,
} Stack_State;

inline char* getstr_stackstate(int nstate, char* pbuf, int nlen)
{
	switch(nstate)
	{
		case STACK_STATE_OOS: snprintf(pbuf, nlen, "OOS"); break;
		case STACK_STATE_PENDING1: snprintf(pbuf, nlen, "PENDING1"); break;
		case STACK_STATE_PENDING2: snprintf(pbuf, nlen, "PENDING2"); break;
		case STACK_STATE_ACTIVE_SECOND: snprintf(pbuf, nlen, "ACT-SECOND"); break;
		case STACK_STATE_ACTIVE_PRI: snprintf(pbuf, nlen, "ACT-PRI"); break;
		default: snprintf(pbuf, nlen, "NOT_DEF(0x%04X)", nstate); break;
	}
	return pbuf;
}

typedef enum _Oos_Reason_e {
	OOS_REASON_UNDEF         = 0x00,
	OOS_REASON_BLOCK         = 0x02,
	OOS_REASON_CONNECT_FAIL  = 0x04,
	OOS_REASON_HW_FAIL       = 0x10,
} E_OOS_REASON;

inline char* getstr_oosreason(int nreason, char* pbuf, int nlen)
{
	std::string strTemp = ""; 
	if (nreason == OOS_REASON_UNDEF)
		strTemp = "IDLE";
	else
	{
		if (nreason & OOS_REASON_BLOCK)
		{
			if (strTemp.size()) strTemp += " & ";
			strTemp += "BLK";
		}
		if (nreason & OOS_REASON_CONNECT_FAIL)
		{
			if (strTemp.size()) strTemp += " & ";
			strTemp += "CONN_FAIL";
		}
		if (nreason & OOS_REASON_HW_FAIL)
		{
			if (strTemp.size()) strTemp += " & ";
			strTemp += "HW_FAIL";
		}
	}
	snprintf(pbuf, nlen-1, "%s", strTemp.c_str());
	return pbuf;
}


typedef struct _TCP_BIND_t {
	int         box;
	int         self_state;
	int         mate_state;

	char *str(char *pbuf, int nlen)
	{
		char szSelf[32]; szSelf[0] = '\0';
		char szMate[32]; szSelf[0] = '\0';
		getstr_stackstate(self_state, szSelf, sizeof(szSelf));
		getstr_stackstate(mate_state, szMate, sizeof(szMate));
		
		snprintf(pbuf, nlen, "box:%d state(self:%s mate:%s)", box, szSelf, szMate);
		return pbuf;
	}
} TCP_BIND_t;

typedef struct _TCP_HC_t {
	int         box;
	int         self_state;
	int         mate_state;
	int         oos_reason;
	int			current_session;
	int			max_session;
	char		sys_name[32];

	char *str(char *pbuf, int nlen)
	{
		char szSelf[32]; szSelf[0] = '\0';
		char szMate[32]; szSelf[0] = '\0';
		getstr_stackstate(self_state, szSelf, sizeof(szSelf));
		getstr_stackstate(mate_state, szMate, sizeof(szMate));

		if (oos_reason != OOS_REASON_UNDEF)	
		{
			char szOos[32]; szOos[0] = '\0';
			getstr_oosreason(oos_reason, szOos, sizeof(szOos));
			snprintf(pbuf, nlen, "box:%d sys_name:%s state(self:%s mate:%s) oos_reason(%s) session(%d/%d)", box, sys_name, szSelf, szMate, szOos, current_session, max_session);
		}
		else
			snprintf(pbuf, nlen, "box:%d sys_name:%s state(self:%s mate:%s) session(%d/%d)", box, sys_name, szSelf, szMate, current_session, max_session);

		return pbuf;
	}
} TCP_HC_t;


typedef struct _TrGW_ApiMsgHdr_t {
	int    primid; 
	int    Trans_Id; 
} TrGW_ApiMsgHdr_t;

#define CSMSG_HDR_SIZE     sizeof(TCP_ApiMsgHdr_t)

#define MEDIA_STACK_BASE   0x50000000
#define MEDIA_REQ_MSG_BASE 0x01000000 /* request message 경우 */
#define MEDIA_RSP_MSG_BASE 0x02000000 /* response message 경우 */
#define MEDIA_IND_MSG_BASE 0x03000000 /* indication message 경우 */
#define KCS_ADD_MEDIA_REQ  1 /* 새로운 INVITE에 대한 Media생성 요청 메시지 */
#define KCS_ADD_MEDIA_RSP  1 /* 새로운 INVITE에 대한 Media생성 응답 메시지 */
#define KCS_MOD_MEDIA_REQ  2 /* INVITE 응답에 대한 Media 변경 요청 메시지 */
#define KCS_MOD_MEDIA_RSP  2 /* INVITE 응답에 대한 Media 변경 응답 메시지 */
#define KCS_DEL_MEDIA_REQ  3 /* BYE에 대한 Media 삭제 요청 메시지 */
#define KCS_DEL_MEDIA_RSP  3 /* BYE에 대한 Media 삭제 응답 메시지 */
#define KCS_ARM_MEDIA_IND  5 /* TrGW에 위한 Media 삭제 알림 메시지 */
#define KCS_SDP_REQ        6 /* CS Hold 에 대한 SDP Make요청 메시지 */
#define KCS_SDP_RSP        6 /* CS Hold 에 대한 SDP Make응답 메시지 */

#define PRI_TYPE_ADD_REQ  MEDIA_STACK_BASE | MEDIA_REQ_MSG_BASE | KCS_ADD_MEDIA_REQ
#define PRI_TYPE_ADD_RSP  MEDIA_STACK_BASE | MEDIA_RSP_MSG_BASE | KCS_ADD_MEDIA_RSP
#define PRI_TYPE_MOD_REQ  MEDIA_STACK_BASE | MEDIA_REQ_MSG_BASE | KCS_MOD_MEDIA_REQ
#define PRI_TYPE_MOD_RSP  MEDIA_STACK_BASE | MEDIA_RSP_MSG_BASE | KCS_MOD_MEDIA_RSP
#define PRI_TYPE_DEL_REQ  MEDIA_STACK_BASE | MEDIA_REQ_MSG_BASE | KCS_DEL_MEDIA_REQ
#define PRI_TYPE_DEL_RSP  MEDIA_STACK_BASE | MEDIA_RSP_MSG_BASE | KCS_DEL_MEDIA_RSP
#define PRI_TYPE_ARM_IND  MEDIA_STACK_BASE | MEDIA_IND_MSG_BASE | KCS_ARM_MEDIA_IND
#define PRI_TYPE_SDP_REQ  MEDIA_STACK_BASE | MEDIA_REQ_MSG_BASE | KCS_SDP_REQ
#define PRI_TYPE_SDP_RSP  MEDIA_STACK_BASE | MEDIA_RSP_MSG_BASE | KCS_SDP_RSP

#define STACK_TARGET_MAX 4
#define UIR_LEN 128
#define CONTENT_TYPE_LEN 128
#define CONTENT_ENC_LEN 128
#define CS_DIR_CALLER 1
#define CS_DIR_CALLED 2
#define UA_LEN 128 //for R104 PKG by hak. 150902. UA Agent add.

typedef enum _CallType_e {
	CALLTYPE_REGULAR        = 0,
	CALLTYPE_EARLY_SESSION  = 1,
	CALLTYPE_MAX            = 2,
} E_CALLTYPE;

typedef enum _SDPType_e {
	SDPTYPE_OFFER         = 0,
	SDPTYPE_ANSWER        = 1,
	SDPTYPE_MAX           = 2,
} E_SDPTYPE;

typedef struct _kcs_add_media_req {
	TrGW_ApiMsgHdr_t   comm;
	int calltype; // 0:regular 1:early
	int sdp_oa;   // 0:offer 1:answer
	int caller_tid;
	int caller_trte;
	int caller_gid;
	//int caller_pgid;
	int nreserved;
	int called_tid; 
	int called_trte;
	int called_gid;
	//int called_pgid;
	int nreserved2;
	int req_sdptype;
	int rsp_sdptype;
   int retrans_flag;
	char call_id[128];
	char to_uri[UIR_LEN];
	char from_uri[UIR_LEN];
   char content_type[CONTENT_TYPE_LEN];
   char content_enc[CONTENT_ENC_LEN];
   char user_agent[UA_LEN]; //for R104 PKG by hak. 150902. UA Agent add.

	/* Offer SDP message */

	char *str(char *pbuf, int nlen)
	{
		char szCalltype[32]; szCalltype[0] = '\0';
		char szSdpOa[32]; szSdpOa[0] = '\0';

		switch(calltype)
		{
			case CALLTYPE_REGULAR: snprintf(szCalltype, sizeof(szCalltype), "regular"); break;	
			case CALLTYPE_EARLY_SESSION: snprintf(szCalltype, sizeof(szCalltype), "early"); break;	
			default: snprintf(szCalltype, sizeof(szCalltype), "undef"); break;	
		}

		switch(sdp_oa)
		{
			case SDPTYPE_OFFER: snprintf(szSdpOa, sizeof(szSdpOa), "offer"); break;	
			case SDPTYPE_ANSWER: snprintf(szSdpOa, sizeof(szSdpOa), "answer"); break;	
			default: snprintf(szSdpOa, sizeof(szSdpOa), "undef"); break;	
		}

		snprintf(pbuf, nlen, 
			"trid:%d from:%s to:%s re:%d ct:%s ce:%s "
			"cr[tid:%d trte:%d gid:%d] cd[tid:%d trte:%d gid:%d] "
			"req_sdp:%d rsp_sdp:%d call:%s sdp_oa:%s ua:%s", 
			comm.Trans_Id, from_uri, to_uri, retrans_flag, content_type, content_enc, 
			caller_tid, caller_trte, caller_gid, called_tid, called_trte, called_gid,
			req_sdptype, rsp_sdptype, szCalltype, szSdpOa, user_agent);
		return pbuf;
	}
} kcs_add_media_req;

typedef struct _kcs_add_media_rsp {
	TrGW_ApiMsgHdr_t  comm;
	int result;
	int reason;
   char content_type[CONTENT_TYPE_LEN];
   char content_enc[CONTENT_ENC_LEN];
/* Offer SDP message */
	char *str(char *pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "trid:%d result:%d reason:0x%x", comm.Trans_Id, result, reason);
		return pbuf;
	}
} kcs_add_media_rsp;

typedef struct _kcs_mod_media_req {
	TrGW_ApiMsgHdr_t   comm;
	int calltype;
	int sdp_oa;
	int caller_tid;
	int caller_trte;
	int caller_gid;
	//int caller_pgid;
	int reinvite_rsp;
	int called_tid;
	int called_trte;
	int called_gid;
	//int called_pgid;
	int nreserved2;
	int req_sdptype;
	int rsp_sdptype;
   int retrans_flag;
	char to_uri[UIR_LEN];
	char from_uri[UIR_LEN];
   char content_type[CONTENT_TYPE_LEN];
   char content_enc[CONTENT_ENC_LEN];
   char user_agent[UA_LEN]; //for R104 PKG by hak. 150902. UA Agent add.

    /* Answer SDP message */

	char *str(char *pbuf, int nlen)
	{
		char szCalltype[32]; szCalltype[0] = '\0';
		char szSdpOa[32]; szSdpOa[0] = '\0';

		switch(calltype)
		{
			case CALLTYPE_REGULAR: snprintf(szCalltype, sizeof(szCalltype), "regular"); break;	
			case CALLTYPE_EARLY_SESSION: snprintf(szCalltype, sizeof(szCalltype), "early"); break;	
			default: snprintf(szCalltype, sizeof(szCalltype), "undef"); break;	
		}

		switch(sdp_oa)
		{
			case SDPTYPE_OFFER: snprintf(szSdpOa, sizeof(szSdpOa), "offer"); break;	
			case SDPTYPE_ANSWER: snprintf(szSdpOa, sizeof(szSdpOa), "answer"); break;	
			default: snprintf(szSdpOa, sizeof(szSdpOa), "undef"); break;	
		}

		snprintf(pbuf, nlen, 
			"trid:%d from:%s to:%s re:%d ct:%s ce:%s "
			"cr[tid:%d trte:%d gid:%d] cd[tid:%d trte:%d gid:%d] "
			"req_sdp:%d rsp_sdp:%d call:%s sdp_oa:%s", 
			comm.Trans_Id, from_uri, to_uri, retrans_flag, content_type, content_enc, 
			caller_tid, caller_trte, caller_gid, called_tid, called_trte, called_gid,
			req_sdptype, rsp_sdptype, szCalltype, szSdpOa);
		return pbuf;
	}
} kcs_mod_media_req;

typedef struct _kcs_mod_media_rsp {
	TrGW_ApiMsgHdr_t  comm;
	int   result;
	int   reason;
   char content_type[CONTENT_TYPE_LEN];
   char content_enc[CONTENT_ENC_LEN];
	char video_use; // 0:audio 1:video
	char reserved[7];
/* Answer SDP message */
	char *str(char *pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "trid:%d result:%d reason:0x%x v_use:%d", 
			comm.Trans_Id, result, reason, video_use);
		return pbuf;
	}
} kcs_mod_media_rsp;

typedef struct _kcs_del_media_req {
	TrGW_ApiMsgHdr_t  comm;
	char *str(char *pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "trid:%d", comm.Trans_Id);
		return pbuf;
	}
} kcs_del_media_req;

typedef struct _kcs_del_media_rsp {
	TrGW_ApiMsgHdr_t  comm;
	int   result;
	int   reason;
	char *str(char *pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "trid:%d result:%d reason:0x%x", comm.Trans_Id, result, reason);
		return pbuf;
	}
} kcs_del_media_rsp;

typedef struct _kcs_arm_media_ind {
	TrGW_ApiMsgHdr_t  comm;
	int   result;
	int   reason;
	int   status;
	char *str(char *pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "trid:%d status:%d result:%d reason:0x%x", comm.Trans_Id, status, result, reason);
		return pbuf;
	}
} kcs_arm_media_ind;

typedef struct _kcs_sdp_req {
	TrGW_ApiMsgHdr_t   comm;
	int caller_tid;
	int caller_trte;
	int called_tid;
	int called_trte;
	int reserved1;
	int rsp_sdptype;
	char to_uri[UIR_LEN];
	char from_uri[UIR_LEN];

	char *str(char *pbuf, int nlen)
	{
		snprintf(pbuf, nlen, 
			"trid:%d from:%s to:%s "
			"cr[tid:%d trte:%d] cd[tid:%d trte:%d] "
			"rsp_sdp:%d", 
			comm.Trans_Id, from_uri, to_uri,
			caller_tid, caller_trte, called_tid, called_trte,
			rsp_sdptype);
		return pbuf;
	}
} kcs_sdp_req;

typedef struct _kcs_sdp_rsp {
	TrGW_ApiMsgHdr_t  comm;
	int   result;
	int   reason;
   char content_type[CONTENT_TYPE_LEN];
   char content_enc[CONTENT_ENC_LEN];

/* Answer SDP message */
	char *str(char *pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "trid:%d result:%d reason:0x%x", 
			comm.Trans_Id, result, reason);
		return pbuf;
	}
} kcs_sdp_rsp;

struct CSMSG_T {
   enum { E_MAXBODY=(1024*4) };
   TCP_ApiMsgHdr_t hdr              ; //header
   char         szBody[E_MAXBODY]; //max body
	char* str(char* pbuf, int len)
	{
		return pbuf;
	}
		
};//end CSMSG_T

inline char* getstr_stype(int nstype, char* pbuf, int nlen)
{
	switch(nstype)
	{
		case STACK_CMSAPI_CTYPE_HB_REQ: snprintf(pbuf, nlen, "HB_REQ"); break;
		case STACK_CMSAPI_CTYPE_HB_RSP: snprintf(pbuf, nlen, "HB_RSP"); break;
		case STACK_CMSAPI_CTYPE_BIND_REQ: snprintf(pbuf, nlen, "BIND_REQ"); break;
		case STACK_CMSAPI_CTYPE_BIND_RSP: snprintf(pbuf, nlen, "BIND_RSP"); break;
		case STACK_CMSAPI_CTYPE_HA_IND: snprintf(pbuf, nlen, "HA_IND"); break;
		case STACK_CMSAPI_CTYPE_TRGW: snprintf(pbuf, nlen, "TRGW"); break;
		default: snprintf(pbuf, nlen, "NOT_DEF(0x%04X)", nstype); break;
	}
	return pbuf;
}

inline char* getstr_primid(int nprimid, char* pbuf, int nlen)
{
	switch(nprimid)
	{
		case PRI_TYPE_ADD_REQ: snprintf(pbuf, nlen, "ADD_REQ"); break;
		case PRI_TYPE_ADD_RSP: snprintf(pbuf, nlen, "ADD_RSP"); break;
		case PRI_TYPE_MOD_REQ: snprintf(pbuf, nlen, "MOD_REQ"); break;
		case PRI_TYPE_MOD_RSP: snprintf(pbuf, nlen, "MOD_RSP"); break;
		case PRI_TYPE_DEL_REQ: snprintf(pbuf, nlen, "DEL_REQ"); break;
		case PRI_TYPE_DEL_RSP: snprintf(pbuf, nlen, "DEL_RSP"); break;
		case PRI_TYPE_ARM_IND: snprintf(pbuf, nlen, "ARM_IND"); break;
		case PRI_TYPE_SDP_REQ: snprintf(pbuf, nlen, "SDP_REQ"); break;
		case PRI_TYPE_SDP_RSP: snprintf(pbuf, nlen, "SDP_RSP"); break;
		default: snprintf(pbuf, nlen, "NOT_DEF(0x%04X)", nprimid); break;
	}
	return pbuf;
}

inline char* getstr_hdr(CSMSG_T* pmsg, char* pbuf, int nlen)
{
	char szMsg[32];

	TCP_ApiMsgHdr_t *phdr = (TCP_ApiMsgHdr_t*)&(pmsg->hdr);

	szMsg[0] = '\0';

	if (phdr->SType == STACK_CMSAPI_CTYPE_TRGW)
	{
		TrGW_ApiMsgHdr_t *pcomm = (TrGW_ApiMsgHdr_t*)&(pmsg->szBody);
		getstr_primid(pcomm->primid, szMsg, sizeof(szMsg));  
	}
	else
	{
		getstr_stype(phdr->SType, szMsg, sizeof(szMsg));
	}

	snprintf(pbuf, nlen, "[LEN:%lu][MSG:%s]",
		phdr->length+sizeof(TCP_ApiMsgHdr_t), szMsg);
#if 0
	snprintf(pbuf, nlen, "[LEN:%u][MSG:%s][SP:%d_ST:%u DP:%d_DT:%u]",
		phdr->length+sizeof(TCP_ApiMsgHdr_t), szMsg, phdr->src_proc, phdr->src_tag, phdr->dst_proc, phdr->dst_tag);
#endif

	return pbuf;
}

inline char* getstr_body(CSMSG_T* pmsg, char* pbuf, int nlen)
{
	char szBody[1024] = {0,};
	char szBodyType[16] = {0,};

	TCP_ApiMsgHdr_t *phdr = (TCP_ApiMsgHdr_t*)&(pmsg->hdr);
	TrGW_ApiMsgHdr_t *pcomm = (TrGW_ApiMsgHdr_t*)&(pmsg->szBody);
	int nbodylen = phdr->length; 

	switch(phdr->MType)
	{
		case STACK_CMSAPI_MTYPE_CONTROL:
		{
			switch(phdr->SType)
			{
				case STACK_CMSAPI_CTYPE_BIND_REQ:
				case STACK_CMSAPI_CTYPE_BIND_RSP:
				{
					((TCP_BIND_t*)pmsg->szBody)->str(szBody, sizeof(szBody));
					snprintf(pbuf, nlen, "[BODY:%d][%s]", nbodylen, szBody);
				}
				break;
				case STACK_CMSAPI_CTYPE_HB_REQ:
				case STACK_CMSAPI_CTYPE_HB_RSP:
				{
					((TCP_HC_t*)pmsg->szBody)->str(szBody, sizeof(szBody));
					snprintf(pbuf, nlen, "[BODY:%d][%s]", nbodylen, szBody);
				}
				break;
				default:
					snprintf(szBody, sizeof(szBody), "unknown mtype:%#x stype:%#x", phdr->MType, phdr->SType);
				break;
			}
		}
		break;
		case STACK_CMSAPI_MTYPE_PROC_MSG:
		{
			snprintf(szBodyType, sizeof(szBodyType), "SDP");
			switch(phdr->SType)
			{
				case STACK_CMSAPI_CTYPE_TRGW:
				{
					switch(pcomm->primid)
					{
						case PRI_TYPE_ADD_REQ: 
							((kcs_add_media_req*)pmsg->szBody)->str(szBody, sizeof(szBody));
							nbodylen -= sizeof(kcs_add_media_req);
						break;
						case PRI_TYPE_ADD_RSP: 
							((kcs_add_media_rsp*)pmsg->szBody)->str(szBody, sizeof(szBody));
							nbodylen -= sizeof(kcs_add_media_rsp);
						break;
						case PRI_TYPE_MOD_REQ: 
							((kcs_mod_media_req*)pmsg->szBody)->str(szBody, sizeof(szBody));
							nbodylen -= sizeof(kcs_mod_media_req);
						break;
						case PRI_TYPE_MOD_RSP: 
							((kcs_mod_media_rsp*)pmsg->szBody)->str(szBody, sizeof(szBody));
							nbodylen -= sizeof(kcs_mod_media_rsp);
						break;
						case PRI_TYPE_DEL_REQ: 
							((kcs_del_media_req*)pmsg->szBody)->str(szBody, sizeof(szBody));
							nbodylen -= sizeof(kcs_del_media_req);
						break;
						case PRI_TYPE_DEL_RSP: 
							((kcs_del_media_rsp*)pmsg->szBody)->str(szBody, sizeof(szBody));
							nbodylen -= sizeof(kcs_del_media_rsp);
							snprintf(szBodyType, sizeof(szBodyType), "CDR");
						break;
						case PRI_TYPE_ARM_IND: 
							((kcs_arm_media_ind*)pmsg->szBody)->str(szBody, sizeof(szBody));
							nbodylen -= sizeof(kcs_arm_media_ind);
							snprintf(szBodyType, sizeof(szBodyType), "CDR");
						break;
						case PRI_TYPE_SDP_REQ: 
							((kcs_sdp_req*)pmsg->szBody)->str(szBody, sizeof(szBody));
							nbodylen -= sizeof(kcs_sdp_req);
						break;
						case PRI_TYPE_SDP_RSP: 
							((kcs_sdp_rsp*)pmsg->szBody)->str(szBody, sizeof(szBody));
							nbodylen -= sizeof(kcs_sdp_rsp);
						break;
						default:
							snprintf(szBody, sizeof(szBody), "unknown primid:%#x", pcomm->primid);
						break;
					}
				}
				break;
				default:
					snprintf(szBody, sizeof(szBody), "unknown stype:%#x", phdr->SType);
				break;
			}
			snprintf(pbuf, nlen, "[%s:%d][%s]", szBodyType, nbodylen, szBody);
		}
		break;
		default:
			snprintf(pbuf, nlen, "unknown body(mtype:%#x)", phdr->MType);
		break;
	}

	return pbuf;
}


}//name space

#endif
