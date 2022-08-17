#ifndef TRGWMSG_H
#define TRGWMSG_H
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
   #include <WINSOCK2.H>
#else 
   #include <netinet/in.h>
#endif
#include "cs_msg.h"
#include "eRule/CRuleUtiles.h"
#include "IBCDefine.h"

namespace NS_CS
{
inline const char* g_fnTrGWState(int nstate)
{
	switch(nstate)
	{
		case STACK_STATE_OOS: return "OOS";
		case STACK_STATE_PENDING1: return "PENDING1";
		case STACK_STATE_PENDING2: return "PENDING2";
		case STACK_STATE_ACTIVE_SECOND: return "ACT-SECOND";
		case STACK_STATE_ACTIVE_PRI: return  "ACT-PRI";
		default: return "NOT_DEF(0x%04X)";
	}
	return "NOT_DEF(0x%04X)";
}
typedef struct TrgwComMsg_t
{
	TCP_ApiMsgHdr_t m_stCom;
	void m_fnHtoN()
	{
		m_stCom.MMode = htonl(m_stCom.MMode);
		m_stCom.MType = htonl(m_stCom.MType);
		m_stCom.SType = htonl(m_stCom.SType);
		m_stCom.src_proc = htons(m_stCom.src_proc);
		m_stCom.dst_proc = htons(m_stCom.dst_proc);
		m_stCom.src_tag = htonl(m_stCom.src_tag);
		m_stCom.dst_tag = htonl(m_stCom.dst_tag);
		m_stCom.length = htonl(m_stCom.length);
	}
	void m_fnNtoH()
	{
		m_stCom.MMode = ntohl(m_stCom.MMode);
		m_stCom.MType = ntohl(m_stCom.MType);
		m_stCom.SType = ntohl(m_stCom.SType);
		m_stCom.src_proc = ntohs(m_stCom.src_proc);
		m_stCom.dst_proc = ntohs(m_stCom.dst_proc);
		m_stCom.src_tag = ntohl(m_stCom.src_tag);
		m_stCom.dst_tag = ntohl(m_stCom.dst_tag);
		m_stCom.length = ntohl(m_stCom.length);
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		if(_bOdering)
		{
			unsigned int unLen = m_stCom.length; unLen = ntohl(unLen); return unLen;
		}
		else return m_stCom.length;
	}
}TrgwComMsg_t;
inline const char * g_fnStringTgASMode(unsigned int _eT)
{
	switch(_eT)
	{
		case STACK_CMSAPI_MMODE_NONE: return "STACK_CMSAPI_MMODE_NONE";
		case STACK_CMSAPI_MMODE_KSIP: return "STACK_CMSAPI_MMODE_KSIP";
		case STACK_CMSAPI_MMODE_KCS: return "STACK_CMSAPI_MMODE_KCS";
		case STACK_CMSAPI_MMODE_KLM: return "STACK_CMSAPI_MMODE_KLM";
		case STACK_CMSAPI_MMODE_KMML: return "STACK_CMSAPI_MMODE_KMML";
		case STACK_CMSAPI_MMODE_TRGW: return "STACK_CMSAPI_MMODE_TRGW";
		case STACK_CMSAPI_MMODE_OMP: return "STACK_CMSAPI_MMODE_OMP";
		case STACK_CMSAPI_MMODE_MAX_NUMS: return "STACK_CMSAPI_MMODE_MAX_NUMS";
		default: return "STACK_CMSAPI_MMODE_NONE";
	}
	return "STACK_CMSAPI_MMODE_NONE";
}
inline const char * g_fnStringTgASType(unsigned int _eT)
{
	switch(_eT)
	{
		case STACK_CMSAPI_MTYPE_NONE: return "STACK_CMSAPI_MTYPE_NONE";
		case STACK_CMSAPI_MTYPE_CONTROL: return "STACK_CMSAPI_MTYPE_CONTROL";
		case STACK_CMSAPI_MTYPE_PROC_MSG: return "STACK_CMSAPI_MTYPE_PROC_MSG";
		case STACK_CMSAPI_MTYPE_MGT_MSG: return "STACK_CMSAPI_MTYPE_MGT_MSG";
		case STACK_CMSAPI_MTYPE_ALARM_MSG: return "STACK_CMSAPI_MTYPE_ALARM_MSG";
		case STACK_CMSAPI_MTYPE_MAX_NUMS: return "STACK_CMSAPI_MTYPE_MAX_NUMS";
		default: return "STACK_CMSAPI_MTYPE_NONE";
	}
	return "STACK_CMSAPI_MTYPE_NONE";
}  
inline const char * g_fnStringTgASSType(unsigned int _eT)
{
	switch(_eT)
	{
		case STACK_CMSAPI_CTYPE_HB_REQ: return "STACK_CMSAPI_CTYPE_HB_REQ";
		case STACK_CMSAPI_CTYPE_HB_RSP: return "STACK_CMSAPI_CTYPE_HB_RSP";
		case STACK_CMSAPI_CTYPE_BIND_REQ: return "STACK_CMSAPI_CTYPE_BIND_REQ";
		case STACK_CMSAPI_CTYPE_BIND_RSP: return "STACK_CMSAPI_CTYPE_BIND_RSP";
		case STACK_CMSAPI_CTYPE_HA_IND: return "STACK_CMSAPI_CTYPE_HA_IND";
		case STACK_CMSAPI_CTYPE_LM: return "STACK_CMSAPI_CTYPE_LM";
		case STACK_CMSAPI_CTYPE_LM_INITDUMP_REQ: return "STACK_CMSAPI_CTYPE_LM_INITDUMP_REQ";
		case STACK_CMSAPI_CTYPE_LM_RULE: return "STACK_CMSAPI_CTYPE_LM_RULE";
		case STACK_CMSAPI_CTYPE_LM_RRS: return "STACK_CMSAPI_CTYPE_LM_RRS";
		case STACK_CMSAPI_CTYPE_LM_RTS: return "STACK_CMSAPI_CTYPE_LM_RTS";
		case STACK_CMSAPI_CTYPE_LM_TN: return "STACK_CMSAPI_CTYPE_LM_TN";
		case STACK_CMSAPI_CTYPE_LM_TRS: return "STACK_CMSAPI_CTYPE_LM_TRS";
		case STACK_CMSAPI_CTYPE_LM_TT: return "STACK_CMSAPI_CTYPE_LM_TT";
		case STACK_CMSAPI_CTYPE_LM_SRS: return "STACK_CMSAPI_CTYPE_LM_SRS";
		case STACK_CMSAPI_CTYPE_LM_ST: return "STACK_CMSAPI_CTYPE_LM_ST";
		case STACK_CMSAPI_CTYPE_LM_AT: return "STACK_CMSAPI_CTYPE_LM_AT";
		case STACK_CMSAPI_CTYPE_LM_LN: return "STACK_CMSAPI_CTYPE_LM_LN";
		case STACK_CMSAPI_CTYPE_TRGW: return "STACK_CMSAPI_CTYPE_TRGW";
		default: return "STACK_CMSAPI_CTYPE_NONE";
	}
	return "STACK_CMSAPI_CTYPE_NONE";
} 
inline char * g_fnStringTgASHdr(char * _pszLog,unsigned int _unSize,TCP_ApiMsgHdr_t * _pstHdr)
{
	fnRUStrnCat(_pszLog,_unSize,"  <TCP_ApiMsgHdr_t>\r\n");
	fnRUStrnCat(_pszLog,_unSize,"    MMode  = %d (%s)\r\n",_pstHdr->MMode,g_fnStringTgASMode(_pstHdr->MMode));
	fnRUStrnCat(_pszLog,_unSize,"    MType  = %d (%s)\r\n",_pstHdr->MType,g_fnStringTgASType(_pstHdr->MType));
	fnRUStrnCat(_pszLog,_unSize,"    SType  = %d (%s)\r\n",_pstHdr->SType,g_fnStringTgASSType(_pstHdr->SType));
	fnRUStrnCat(_pszLog,_unSize,"    src_proc  = %d\r\n",_pstHdr->src_proc);
	fnRUStrnCat(_pszLog,_unSize,"    dst_proc  = %d\r\n",_pstHdr->dst_proc);
	fnRUStrnCat(_pszLog,_unSize,"    src_tag  = %d\r\n",_pstHdr->src_tag);
	fnRUStrnCat(_pszLog,_unSize,"    dst_tag  = %d\r\n",_pstHdr->dst_tag);
	fnRUStrnCat(_pszLog,_unSize,"    length  = %d\r\n\r\n",_pstHdr->length);
	return _pszLog;
}
inline char * g_fnStringTgASCom(char * _pszLog,unsigned int _unSize,TrgwComMsg_t * _pstHdr)
{
	return g_fnStringTgASHdr(_pszLog,_unSize,&_pstHdr->m_stCom);
}

inline char * g_fnStringTgASBind(char * _pszLog,unsigned int _unSize,TCP_BIND_t * _pstPk)
{
	fnRUStrnCat(_pszLog,_unSize,"  <TCP_BIND_t>\r\n");
	fnRUStrnCat(_pszLog,_unSize,"    box = %d\r\n",_pstPk->box);
	fnRUStrnCat(_pszLog,_unSize,"    self_state = %d (%s)\r\n",_pstPk->self_state,g_fnTrGWState(_pstPk->self_state));
	fnRUStrnCat(_pszLog,_unSize,"    mate_state = %d (%s)\r\n\r\n",_pstPk->mate_state,g_fnTrGWState(_pstPk->mate_state));
	return _pszLog;
}
typedef struct TrgwBindReqMsg_t
{
	TrgwComMsg_t m_stCom;
	TCP_BIND_t m_stBind;
	TrgwBindReqMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stCom,0x00,sizeof(TrgwComMsg_t));
		memset(&m_stBind,0x00,sizeof(TCP_BIND_t));
		m_stCom.m_stCom.length = sizeof(TCP_BIND_t);
		m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_KCS;
		m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_CONTROL;
		m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_BIND_REQ;
	}
	void m_fnHtoN()
	{
		m_stCom.m_fnHtoN();
		m_stBind.box = htonl(m_stBind.box);
		m_stBind.self_state = htonl(m_stBind.self_state);
		m_stBind.mate_state = htonl(m_stBind.mate_state);
	}
	void m_fnNtoH()
	{
		m_stCom.m_fnNtoH();
		m_stBind.box = ntohl(m_stBind.box);
		m_stBind.self_state = ntohl(m_stBind.self_state);
		m_stBind.mate_state = ntohl(m_stBind.mate_state);
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		return m_stCom.m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	char* m_fnStr(char * _pszLog,unsigned int _unSize)
	{
		fnRUStrnCat(_pszLog,_unSize,"<TrgwBindReqMsg_t>\r\n");
		fnRUStrnCat(_pszLog,_unSize,"  Body : box=%u, self_state=%s, mate_state=%s\r\n",m_stBind.box,
															g_fnTrGWState(m_stBind.self_state),
															g_fnTrGWState(m_stBind.mate_state));
		
		return _pszLog;
	}
}TrgwBindReqMsg_t;
inline char * g_fnStringTgASBindReq(char * _pszLog,unsigned int _unSize,TrgwBindReqMsg_t * _pstPk)
{
	fnRUStrnCat(_pszLog,_unSize,"<TrgwBindReqMsg_t>\r\n");
	g_fnStringTgASCom(_pszLog,_unSize,&_pstPk->m_stCom);
	g_fnStringTgASBind(_pszLog,_unSize,&_pstPk->m_stBind);
	return _pszLog;
}
typedef struct TrgwBindRspMsg_t
{
	TrgwComMsg_t m_stCom;
	TCP_BIND_t m_stBind;
	TrgwBindRspMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stCom,0x00,sizeof(TrgwComMsg_t));
		memset(&m_stBind,0x00,sizeof(TCP_BIND_t));
		m_stCom.m_stCom.length = sizeof(TCP_BIND_t);
		m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_KCS;
		m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_CONTROL;
		m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_BIND_RSP;
	}
	void m_fnHtoN()
	{
		m_stCom.m_fnHtoN();
		m_stBind.box = htonl(m_stBind.box);
		m_stBind.self_state = htonl(m_stBind.self_state);
		m_stBind.mate_state = htonl(m_stBind.mate_state);
	}
	void m_fnNtoH()
	{
		m_stCom.m_fnNtoH();
		m_stBind.box = ntohl(m_stBind.box);
		m_stBind.self_state = ntohl(m_stBind.self_state);
		m_stBind.mate_state = ntohl(m_stBind.mate_state);
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		return m_stCom.m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	char* m_fnStr(char * _pszLog,unsigned int _unSize)
	{
		fnRUStrnCat(_pszLog,_unSize,"<TrgwBindRspMsg_t>\r\n");
		fnRUStrnCat(_pszLog,_unSize,"  Body : box=%u, self_state=%s, mate_state=%s\r\n",m_stBind.box,
															g_fnTrGWState(m_stBind.self_state),
															g_fnTrGWState(m_stBind.mate_state));
		
		return _pszLog;
	}
}TrgwBindRspMsg_t;
inline char * g_fnStringTgASBindRsp(char * _pszLog,unsigned int _unSize,TrgwBindRspMsg_t * _pstPk)
{
	fnRUStrnCat(_pszLog,_unSize,"<TrgwBindRspMsg_t>\r\n");
	g_fnStringTgASCom(_pszLog,_unSize,&_pstPk->m_stCom);
	g_fnStringTgASBind(_pszLog,_unSize,&_pstPk->m_stBind);
	return _pszLog;
}
typedef struct TrgwHBReqMsg_t
{
	TrgwComMsg_t m_stCom;
	TCP_HC_t m_stHB;
	TrgwHBReqMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stCom,0x00,sizeof(TrgwComMsg_t));
		memset(&m_stHB,0x00,sizeof(TCP_HC_t));
		m_stCom.m_stCom.length = sizeof(TCP_HC_t);
		m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_KCS;
		m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_CONTROL;
		m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_HB_REQ;
	}
	void m_fnHtoN()
	{
		m_stCom.m_fnHtoN();
		m_stHB.box = htonl(m_stHB.box);
		m_stHB.self_state = htonl(m_stHB.self_state);
		m_stHB.mate_state = htonl(m_stHB.mate_state);
		m_stHB.oos_reason = htonl(m_stHB.oos_reason);
	}
	void m_fnNtoH()
	{
		m_stCom.m_fnNtoH();
		m_stHB.box = ntohl(m_stHB.box);
		m_stHB.self_state = ntohl(m_stHB.self_state);
		m_stHB.mate_state = ntohl(m_stHB.mate_state);
		m_stHB.oos_reason = ntohl(m_stHB.oos_reason);
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		return m_stCom.m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	const char * m_fnGetSysName()
	{
		return m_stHB.sys_name;
	}
	void m_fnSetSysName(const char * _pszVmID)
	{
		if(_pszVmID) strncpy(m_stHB.sys_name,_pszVmID,31);
	}
	char * m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		fnRUStrnCat(_pszLog,_unSize,"<TrgwHBReqMsg_t>\r\n");
		#if 1
		fnRUStrnCat(_pszLog,_unSize,"  Body : sys_name=%s, box=%u, self_state=%s, mate_state=%s, "\
									"Cur=%d, Max=%d\r\n",
															m_stHB.sys_name,
															m_stHB.box,
															g_fnTrGWState(m_stHB.self_state),
															g_fnTrGWState(m_stHB.mate_state),
															m_stHB.current_session,
															m_stHB.max_session);
		#else
		fnRUStrnCat(_pszLog,_unSize,"  Body : sys_name=%s, box=%u, self_state=%s, mate_state=%s",
															m_stHB.sys_name,
															m_stHB.box,
															g_fnTrGWState(m_stHB.self_state),
															g_fnTrGWState(m_stHB.mate_state));
		#endif
		return _pszLog;
	}
}TrgwHBReqMsg_t;
typedef struct TrgwHBRspMsg_t
{
	TrgwComMsg_t m_stCom;
	TCP_HC_t m_stHB;
	TrgwHBRspMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stCom,0x00,sizeof(TrgwComMsg_t));
		memset(&m_stHB,0x00,sizeof(TCP_HC_t));
		m_stCom.m_stCom.length = sizeof(TCP_HC_t);
		m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_KCS;
		m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_CONTROL;
		m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_HB_RSP;
	}
	void m_fnHtoN()
	{
		m_stCom.m_fnHtoN();
		m_stHB.box = htonl(m_stHB.box);
		m_stHB.self_state = htonl(m_stHB.self_state);
		m_stHB.mate_state = htonl(m_stHB.mate_state);
		m_stHB.oos_reason = htonl(m_stHB.oos_reason);
	}
	void m_fnNtoH()
	{
		m_stCom.m_fnNtoH();
		m_stHB.box = ntohl(m_stHB.box);
		m_stHB.self_state = ntohl(m_stHB.self_state);
		m_stHB.mate_state = ntohl(m_stHB.mate_state);
		m_stHB.oos_reason = ntohl(m_stHB.oos_reason);
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		return m_stCom.m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	char * m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		fnRUStrnCat(_pszLog,_unSize,"<TrgwHBRspMsg_t>\r\n");
		fnRUStrnCat(_pszLog,_unSize,"  Body : box=%u, self_state=%s, mate_state=%s\r\n",m_stHB.box,
															g_fnTrGWState(m_stHB.self_state),
															g_fnTrGWState(m_stHB.mate_state));
		
		return _pszLog;
	}
}TrgwHBRspMsg_t;
inline char * g_fnStringTgASTrGW(char * _pszLog,unsigned int _unSize,TrGW_ApiMsgHdr_t * _pstPk)
{
	fnRUStrnCat(_pszLog,_unSize,"  <TrGW_ApiMsgHdr_t>\r\n");
	fnRUStrnCat(_pszLog,_unSize,"    primid = %d\r\n",_pstPk->primid);
	fnRUStrnCat(_pszLog,_unSize,"    Trans_Id = %u\r\n\r\n",_pstPk->Trans_Id);
	return _pszLog;
}
typedef struct TrgwMediaMsg_t
{
	TrgwComMsg_t m_stCom;
	TrGW_ApiMsgHdr_t m_stTrgw;
	void m_fnHtoN()
	{
		m_stCom.m_fnHtoN();
		m_stTrgw.primid = htonl(m_stTrgw.primid);
		m_stTrgw.Trans_Id = htonl(m_stTrgw.Trans_Id);
	}
	void m_fnNtoH()
	{
		m_stCom.m_fnNtoH();
		m_stTrgw.primid = ntohl(m_stTrgw.primid);
		m_stTrgw.Trans_Id = ntohl(m_stTrgw.Trans_Id);
	}
	char * m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		fnRUStrnCat(_pszLog,_unSize,"<<TrgwMediaMsg_t>>\r\n");
		fnRUStrnCat(_pszLog,_unSize,
				"<Com HDR> MMode=%d,MType=%d,SType=%d,src_proc=%d,dst_proc=%d,length=%d",
								m_stCom.m_stCom.MMode,m_stCom.m_stCom.MType,
								m_stCom.m_stCom.SType,m_stCom.m_stCom.src_proc,
								m_stCom.m_stCom.dst_proc,m_stCom.m_stCom.length);
		fnRUStrnCat(_pszLog,_unSize,"<Trgw HDR> primid=%d,Trans_Id=%d\r\n",
															m_stTrgw.primid,m_stTrgw.Trans_Id);
		return _pszLog;
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		return m_stCom.m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
}TrgwMediaMsg_t;
inline char * g_fnStringTgASMedia(char * _pszLog,unsigned int _unSize,TrgwMediaMsg_t * _pstPk)
{
	fnRUStrnCat(_pszLog,_unSize,"  <TrgwMediaMsg_t>\r\n");
	g_fnStringTgASCom(_pszLog,_unSize,&_pstPk->m_stCom);
	g_fnStringTgASTrGW(_pszLog,_unSize,&_pstPk->m_stTrgw);
	return _pszLog;
}
typedef struct TrgwAddReqMsg_t
{
	TrgwMediaMsg_t m_stHdr;
	struct
	{	
		int calltype; // 0:regular 1:early
		int sdp_oa;   // 0:offer 1:answer
		int caller_tid;
		int caller_trte;
		#ifdef DEF_R126
		int caller_gid; // R126
		#endif
		int nreserved;
		int called_tid; 
		int called_trte;
		#ifdef DEF_R126
		int called_gid; // R126
		#endif
		int nreserved2;
		int req_sdptype;
		int rsp_sdptype;
		int retrans_flag;
		char call_id[128];
		char to_uri[UIR_LEN];
		char from_uri[UIR_LEN];
		char content_type[CONTENT_TYPE_LEN];
		char content_enc[CONTENT_ENC_LEN];
		char user_agent[UA_LEN];
	}m_stBody;
	TrgwAddReqMsg_t(){m_fnClear();}  
	void m_fnClear()
	{
		memset(&m_stHdr,0x00,sizeof(TCP_ApiMsgHdr_t));
		m_stHdr.m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_TRGW;
		m_stHdr.m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_PROC_MSG;
		m_stHdr.m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_TRGW;
		m_stHdr.m_stCom.m_stCom.src_proc = 0;     // src process id
		m_stHdr.m_stCom.m_stCom.dst_proc = 0;     // dst process id
		m_stHdr.m_stCom.m_stCom.src_tag = 0;       // src session id
		m_stHdr.m_stCom.m_stCom.dst_tag = 0;       // dst session id
		m_stHdr.m_stCom.m_stCom.length = sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody);
		m_stHdr.m_stTrgw.primid = PRI_TYPE_ADD_REQ;
		m_stHdr.m_stTrgw.Trans_Id = 0;
		memset(&m_stBody,0x00,sizeof(m_stBody));
	}
	void m_fnHtoN()
	{
		m_stHdr.m_fnHtoN();
		m_stBody.calltype      = htonl(m_stBody.calltype    );   
		m_stBody.sdp_oa        = htonl(m_stBody.sdp_oa      );  
		m_stBody.caller_tid    = htonl(m_stBody.caller_tid  );  
		m_stBody.caller_trte   = htonl(m_stBody.caller_trte );  
		m_stBody.nreserved     = htonl(m_stBody.nreserved   );  
		m_stBody.called_tid    = htonl(m_stBody.called_tid  );  
		m_stBody.called_trte   = htonl(m_stBody.called_trte );  
		m_stBody.nreserved2    = htonl(m_stBody.nreserved2  );  
		m_stBody.req_sdptype   = htonl(m_stBody.req_sdptype );  
		m_stBody.rsp_sdptype   = htonl(m_stBody.rsp_sdptype );  
		m_stBody.retrans_flag  = htonl(m_stBody.retrans_flag);  
	}
	void m_fnNtoH()
	{
		m_stHdr.m_fnNtoH();
		m_stBody.calltype      = ntohl(m_stBody.calltype    );   
		m_stBody.sdp_oa        = ntohl(m_stBody.sdp_oa      );  
		m_stBody.caller_tid    = ntohl(m_stBody.caller_tid  );  
		m_stBody.caller_trte   = ntohl(m_stBody.caller_trte );  
		m_stBody.nreserved     = ntohl(m_stBody.nreserved   );  
		m_stBody.called_tid    = ntohl(m_stBody.called_tid  );  
		m_stBody.called_trte   = ntohl(m_stBody.called_trte );  
		m_stBody.nreserved2    = ntohl(m_stBody.nreserved2  );  
		m_stBody.req_sdptype   = ntohl(m_stBody.req_sdptype );  
		m_stBody.rsp_sdptype   = ntohl(m_stBody.rsp_sdptype );  
		m_stBody.retrans_flag  = ntohl(m_stBody.retrans_flag);  
	}
	char * m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		unsigned int unAdditionalLen = m_fnGetAdditionalDataLen(false);
		fnRUStrnCat(_pszLog,_unSize,"<<<TrgwAddReqMsg_t>>>\r\n");
		m_stHdr.m_fnStr(_pszLog, _unSize);
		#ifdef DEF_R126
		fnRUStrnCat(_pszLog,_unSize,"<body param> calltype=%s,sdp_oa=%s,caller_tid=%d,caller_trte=%d,caller_gid=%d"\
									"called_tip=%d,called_trte=%d,called_gid=%d"\
									"req_sdptype=%s,rsp_sdptype=%s,retrans_flag=%d,call_id=%s,"\
									"to_uri=%s,from_uri=%s,"\
									"content_type=%s,content_enc=%s,ua=%s,sdpLen=%u\r\n",
									TRUE_SELECT(m_stBody.calltype==0,"reqular","early"),
									TRUE_SELECT(m_stBody.sdp_oa==0,"offer","answer"),
									m_stBody.caller_tid,m_stBody.caller_trte,m_stBody.caller_gid,
									m_stBody.called_tid,m_stBody.called_trte,m_stBody.called_gid,
									TRUE_SELECT(m_stBody.req_sdptype==1,"caller","called"),
									TRUE_SELECT(m_stBody.rsp_sdptype==1,"caller","called"),
									m_stBody.retrans_flag,
									m_stBody.call_id,m_stBody.to_uri,
									m_stBody.from_uri,m_stBody.content_type,
									m_stBody.content_enc,m_stBody.user_agent,unAdditionalLen);
		#else
		fnRUStrnCat(_pszLog,_unSize,"<body param> calltype=%s,sdp_oa=%s,caller_tid=%d,caller_trte=%d,"\
									"called_tip=%d,called_trte=%d,"\
									"req_sdptype=%s,rsp_sdptype=%s,retrans_flag=%d,call_id=%s,"\
									"to_uri=%s,from_uri=%s,"\
									"content_type=%s,content_enc=%s,ua=%s,sdpLen=%u\r\n",
									TRUE_SELECT(m_stBody.calltype==0,"reqular","early"),
									TRUE_SELECT(m_stBody.sdp_oa==0,"offer","answer"),
									m_stBody.caller_tid,m_stBody.caller_trte,
									m_stBody.called_tid,m_stBody.called_trte,
									TRUE_SELECT(m_stBody.req_sdptype==1,"caller","called"),
									TRUE_SELECT(m_stBody.rsp_sdptype==1,"caller","called"),
									m_stBody.retrans_flag,
									m_stBody.call_id,m_stBody.to_uri,
									m_stBody.from_uri,m_stBody.content_type,
									m_stBody.content_enc,m_stBody.user_agent,unAdditionalLen);
		#endif
		if(unAdditionalLen > 0)
		{
			if(m_stBody.content_enc[0]==0)
			{
				fnRUStrnCat(_pszLog,_unSize,"<SDP>\r\n%s",(char*)m_fnGetSdp(false));
			}
			else
			{
				fnRUStrnCat(_pszLog,_unSize,"<Encoded SDP>\r\n");
				fnRUHexnCat(m_fnGetSdp(false),unAdditionalLen, _pszLog,_unSize);
			}
		}
		return _pszLog;
	}
	void * m_fnGetSdp(bool _bOdering)
	{	
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	void m_fnSetSdp(const void * _pvSdp, unsigned int _unLen)
	{
		memcpy(this+1,_pvSdp,_unLen);
		m_stHdr.m_stCom.m_stCom.length = sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody) + _unLen;
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		return m_stHdr.m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetAdditionalDataLen(bool _bOdering)
	{
		unsigned int unAdditionalLen = m_fnGetBodyLength(_bOdering);
		unAdditionalLen = unAdditionalLen - sizeof(m_stBody) - sizeof(TrGW_ApiMsgHdr_t);
		return unAdditionalLen;
	}
}TrgwAddReqMsg_t;
inline char * g_fnStringTgASAddReq(char * _pszLog,unsigned int _unSize,TrgwAddReqMsg_t * _pstPk)
{
	unsigned int unAdditionalLen = _pstPk->m_fnGetAdditionalDataLen(false);
	fnRUStrnCat(_pszLog,_unSize,"<TrgwAddReqMsg_t>\r\n");
	g_fnStringTgASMedia(_pszLog,_unSize,&_pstPk->m_stHdr);
	fnRUStrnCat(_pszLog,_unSize,"  calltype = %d (%s)\r\n",_pstPk->m_stBody.calltype,
										TRUE_SELECT(_pstPk->m_stBody.calltype==0,"reqular","early"));
	fnRUStrnCat(_pszLog,_unSize,"  sdp_oa = %d (%s)\r\n",_pstPk->m_stBody.sdp_oa,
										TRUE_SELECT(_pstPk->m_stBody.sdp_oa==0,"offer","answer"));
	fnRUStrnCat(_pszLog,_unSize,"  caller_tid = %d\r\n",_pstPk->m_stBody.caller_tid);
	fnRUStrnCat(_pszLog,_unSize,"  caller_trte = %d\r\n",_pstPk->m_stBody.caller_trte);
	#ifdef DEF_R126
	fnRUStrnCat(_pszLog,_unSize,"  caller_gid = %d\r\n",_pstPk->m_stBody.caller_gid);  // R126
	#endif
	fnRUStrnCat(_pszLog,_unSize,"  nreserved = %d\r\n",_pstPk->m_stBody.nreserved);
	fnRUStrnCat(_pszLog,_unSize,"  called_tid = %d\r\n",_pstPk->m_stBody.called_tid);
	fnRUStrnCat(_pszLog,_unSize,"  called_trte = %d\r\n",_pstPk->m_stBody.called_trte);
	#ifdef DEF_R126
	fnRUStrnCat(_pszLog,_unSize,"  called_gid = %d\r\n",_pstPk->m_stBody.called_gid);  // R126
	#endif
	fnRUStrnCat(_pszLog,_unSize,"  nreserved2 = %d\r\n",_pstPk->m_stBody.nreserved2);
	fnRUStrnCat(_pszLog,_unSize,"  req_sdptype = %d (%s)\r\n",_pstPk->m_stBody.req_sdptype,
										TRUE_SELECT(_pstPk->m_stBody.req_sdptype==1,"caller","called"));
	fnRUStrnCat(_pszLog,_unSize,"  rsp_sdptype = %d (%s)\r\n",_pstPk->m_stBody.rsp_sdptype,
										TRUE_SELECT(_pstPk->m_stBody.rsp_sdptype==1,"caller","called"));
	fnRUStrnCat(_pszLog,_unSize,"  retrans_flag = %d\r\n",_pstPk->m_stBody.retrans_flag);
	fnRUStrnCat(_pszLog,_unSize,"  call_id = %s\r\n",_pstPk->m_stBody.call_id);
	fnRUStrnCat(_pszLog,_unSize,"  to_uri = %s\r\n",_pstPk->m_stBody.to_uri);
	fnRUStrnCat(_pszLog,_unSize,"  from_uri = %s\r\n",_pstPk->m_stBody.from_uri);
	fnRUStrnCat(_pszLog,_unSize,"  content_type = %s\r\n",_pstPk->m_stBody.content_type);
	fnRUStrnCat(_pszLog,_unSize,"  content_enc = %s\r\n",_pstPk->m_stBody.content_enc);
	fnRUStrnCat(_pszLog,_unSize,"  user_agent = %s\r\n\r\n",_pstPk->m_stBody.user_agent);
	if(unAdditionalLen > 0)
	{
		if(_pstPk->m_stBody.content_enc[0]==0)
		{
			fnRUStrnCat(_pszLog,_unSize,"<SDP>\r\n%s",(char*)_pstPk->m_fnGetSdp(false));
		}
		else
		{
			fnRUStrnCat(_pszLog,_unSize,"<Encoded SDP>\r\n");
			fnRUHexnCat(_pstPk->m_fnGetSdp(false),unAdditionalLen, _pszLog,_unSize);
		}
	}
	return _pszLog;
}
inline TrgwAddReqMsg_t * g_fnCreateAddReq(unsigned int _unSdpSize)
{
	char * pszNew = new char[sizeof(TrgwAddReqMsg_t) + _unSdpSize]; 
	memset(pszNew,0x00,sizeof(TrgwAddReqMsg_t) + _unSdpSize);
	TrgwAddReqMsg_t * pclsNew = (TrgwAddReqMsg_t *)pszNew;
	pclsNew->m_fnClear();return pclsNew;
}
typedef struct TrgwAddRspMsg_t
{
	TrgwMediaMsg_t m_stHdr;
	struct
	{
		int result;
		int reason;
		char content_type[CONTENT_TYPE_LEN];
		char content_enc[CONTENT_ENC_LEN];
	}m_stBody;
	TrgwAddRspMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stHdr,0x00,sizeof(TrgwMediaMsg_t));
		m_stHdr.m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_TRGW;
		m_stHdr.m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_PROC_MSG;
		m_stHdr.m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_TRGW;
		m_stHdr.m_stCom.m_stCom.src_proc = 0;     // src process id
		m_stHdr.m_stCom.m_stCom.dst_proc = 0;     // dst process id
		m_stHdr.m_stCom.m_stCom.src_tag = 0;       // src session id
		m_stHdr.m_stCom.m_stCom.dst_tag = 0;       // dst session id
		m_stHdr.m_stCom.m_stCom.length = sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody);
		m_stHdr.m_stTrgw.primid = PRI_TYPE_ADD_RSP;
		m_stHdr.m_stTrgw.Trans_Id = 0;
		memset(&m_stBody,0x00,sizeof(m_stBody));
	}
	void m_fnHtoN()
	{
		m_stHdr.m_fnHtoN();
		m_stBody.result = htonl(m_stBody.result);
		m_stBody.reason = htonl(m_stBody.reason);
	}
	void m_fnNtoH()
	{
		m_stHdr.m_fnNtoH();
		m_stBody.result = ntohl(m_stBody.result);
		m_stBody.reason = ntohl(m_stBody.reason);
	}
	char * m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		unsigned int unAdditionalLen = m_fnGetAdditionalDataLen(false);
		fnRUStrnCat(_pszLog,_unSize,"<<<TrgwAddRspMsg_t>>>\r\n");
		m_stHdr.m_fnStr(_pszLog, _unSize);
		fnRUStrnCat(_pszLog,_unSize,
					"<body param> result=%d,reason=%d,content_type=%s,content_enc=%s,sdpLen=%u\r\n",
					m_stBody.result,m_stBody.reason,m_stBody.content_type,m_stBody.content_enc,unAdditionalLen);
		if(m_stBody.result == -1)
		{
			fnRUStrnCat(_pszLog,_unSize,"<Error String>\r\n%s",m_fnGetReason(false));
		}
		else if(unAdditionalLen > 0)
		{
			if(m_stBody.content_enc[0]==0)
			{
				fnRUStrnCat(_pszLog,_unSize,"<SDP>\r\n%s",(char*)m_fnGetSdp(false));
			}
			else
			{
				fnRUStrnCat(_pszLog,_unSize,"<Encoded SDP>\r\n");
				fnRUHexnCat(m_fnGetSdp(false),unAdditionalLen, _pszLog,_unSize);
			}
		}
		return _pszLog;
	}
	void * m_fnGetSdp(bool _bOdering)
	{	
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	char * m_fnGetReason(bool _bOdering)
	{	
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	void m_fnSetSdp(const void * _pvSdp, unsigned int _unLen)
	{
		memcpy(this+1,_pvSdp,_unLen);
		m_stHdr.m_stCom.m_stCom.length = sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody) + _unLen;
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		return m_stHdr.m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetAdditionalDataLen(bool _bOdering)
	{
		unsigned int unAdditionalLen = m_fnGetBodyLength(_bOdering);
		unAdditionalLen = unAdditionalLen - sizeof(m_stBody) - sizeof(TrGW_ApiMsgHdr_t);
		return unAdditionalLen;
	}
}TrgwAddRspMsg_t;
inline char * g_fnStringTgASAddRsp(char * _pszLog,unsigned int _unSize,TrgwAddRspMsg_t * _pstPk)
{
	unsigned int unAdditionalLen = _pstPk->m_fnGetAdditionalDataLen(false);
	fnRUStrnCat(_pszLog,_unSize,"<TrgwAddRspMsg_t>\r\n");
	g_fnStringTgASMedia(_pszLog,_unSize,&_pstPk->m_stHdr);
	fnRUStrnCat(_pszLog,_unSize,"  result = %d\r\n",_pstPk->m_stBody.result);
	fnRUStrnCat(_pszLog,_unSize,"  reason = 0x%x\r\n",_pstPk->m_stBody.reason);
	fnRUStrnCat(_pszLog,_unSize,"  content_type = %s\r\n",_pstPk->m_stBody.content_type);
	fnRUStrnCat(_pszLog,_unSize,"  content_enc = %s\r\n",_pstPk->m_stBody.content_enc);	
	if(_pstPk->m_stBody.result == -1)
	{
		fnRUStrnCat(_pszLog,_unSize,"<Error String>\r\n%s",_pstPk->m_fnGetReason(false));
	}
	else if(unAdditionalLen > 0)
	{
		if(_pstPk->m_stBody.content_enc[0]==0)
		{
			fnRUStrnCat(_pszLog,_unSize,"<SDP>\r\n%s",(char*)_pstPk->m_fnGetSdp(false));
		}
		else
		{
			fnRUStrnCat(_pszLog,_unSize,"<Encoded SDP>\r\n");
			fnRUHexnCat(_pstPk->m_fnGetSdp(false),unAdditionalLen, _pszLog,_unSize);
		}
	}
	return _pszLog;
}
inline TrgwAddRspMsg_t * g_fnCreateAddRsp(unsigned int _unSdpSize)
{
	char * pszNew = new char[sizeof(TrgwAddRspMsg_t) + _unSdpSize];
	memset(pszNew,0x00,sizeof(TrgwAddRspMsg_t) + _unSdpSize);
	TrgwAddRspMsg_t * pclsNew = (TrgwAddRspMsg_t *)pszNew;
	pclsNew->m_fnClear();return pclsNew;
}
typedef struct TrgwModReqMsg_t
{
	TrgwMediaMsg_t m_stHdr;
	struct
	{
		int calltype;
		int sdp_oa;
		int caller_tid;
		int caller_trte;
		#ifdef DEF_R126
		int caller_gid;  // R126
		#endif
		int reinvite_rsp;
		int called_tid;
		int called_trte;
		#ifdef DEF_R126
		int called_gid; // R126
		#endif
		int nreserved2;
		int req_sdptype;
		int rsp_sdptype;
		int retrans_flag;
		char to_uri[UIR_LEN];
		char from_uri[UIR_LEN];
		char content_type[CONTENT_TYPE_LEN];
		char content_enc[CONTENT_ENC_LEN];
		char user_agent[UA_LEN];
	}m_stBody;
	TrgwModReqMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stHdr,0x00,sizeof(TrgwMediaMsg_t));
		m_stHdr.m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_TRGW;
		m_stHdr.m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_PROC_MSG;
		m_stHdr.m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_TRGW;
		m_stHdr.m_stCom.m_stCom.src_proc = 0;     // src process id
		m_stHdr.m_stCom.m_stCom.dst_proc = 0;     // dst process id
		m_stHdr.m_stCom.m_stCom.src_tag = 0;       // src session id
		m_stHdr.m_stCom.m_stCom.dst_tag = 0;       // dst session id
		m_stHdr.m_stCom.m_stCom.length = sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody);
		m_stHdr.m_stTrgw.primid = PRI_TYPE_MOD_REQ;
		m_stHdr.m_stTrgw.Trans_Id = 0;
		memset(&m_stBody,0x00,sizeof(m_stBody));
	}
	void m_fnHtoN()
	{
		m_stHdr.m_fnHtoN();
		m_stBody.calltype     = htonl(m_stBody.calltype     );  
		m_stBody.sdp_oa       = htonl(m_stBody.sdp_oa       );
		m_stBody.caller_tid   = htonl(m_stBody.caller_tid   );
		m_stBody.caller_trte  = htonl(m_stBody.caller_trte  );
		m_stBody.reinvite_rsp = htonl(m_stBody.reinvite_rsp );
		m_stBody.called_tid   = htonl(m_stBody.called_tid   );
		m_stBody.called_trte  = htonl(m_stBody.called_trte  );
		m_stBody.nreserved2   = htonl(m_stBody.nreserved2   );
		m_stBody.req_sdptype  = htonl(m_stBody.req_sdptype  );
		m_stBody.rsp_sdptype  = htonl(m_stBody.rsp_sdptype  );
		m_stBody.retrans_flag = htonl(m_stBody.retrans_flag );
	}
	void m_fnNtoH()
	{
		m_stHdr.m_fnNtoH();
		m_stBody.calltype     = ntohl(m_stBody.calltype     );  
		m_stBody.sdp_oa       = ntohl(m_stBody.sdp_oa       );
		m_stBody.caller_tid   = ntohl(m_stBody.caller_tid   );
		m_stBody.caller_trte  = ntohl(m_stBody.caller_trte  );
		m_stBody.reinvite_rsp = ntohl(m_stBody.reinvite_rsp );
		m_stBody.called_tid   = ntohl(m_stBody.called_tid   );
		m_stBody.called_trte  = ntohl(m_stBody.called_trte  );
		m_stBody.nreserved2   = ntohl(m_stBody.nreserved2   );
		m_stBody.req_sdptype  = ntohl(m_stBody.req_sdptype  );
		m_stBody.rsp_sdptype  = ntohl(m_stBody.rsp_sdptype  );
		m_stBody.retrans_flag = ntohl(m_stBody.retrans_flag );
	}
	char * m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		unsigned int unAdditionalLen = m_fnGetAdditionalDataLen(false);
		fnRUStrnCat(_pszLog,_unSize,"<<<TrgwModReqMsg_t>>>\r\n");
		m_stHdr.m_fnStr(_pszLog, _unSize);
		#ifdef DEF_R126
		fnRUStrnCat(_pszLog,_unSize,"<body param> calltype=%s,sdp_oa=%s,caller_tid=%d,caller_trte=%d,caller_gid=%d"\
									"reinvite_rsp=%d,called_tid=%d,called_trte=%d,called_gid=%d,nreserved2=%d,"\
									"req_sdptype=%s,rsp_sdptype=%s,"\
									"retrans_flag=%d,to_uri=%sfrom_uri=%scontent_type=%s,"\
									"content_enc=%s,ua=%s,sdpLen=%u\r\n",
									TRUE_SELECT(m_stBody.calltype==0,"reqular","early"),
									TRUE_SELECT(m_stBody.sdp_oa==0,"offer","answer"),
									m_stBody.caller_tid,m_stBody.caller_trte,m_stBody.caller_gid,
									m_stBody.reinvite_rsp,m_stBody.called_tid,m_stBody.called_gid,
									m_stBody.called_trte,m_stBody.nreserved2,
									TRUE_SELECT(m_stBody.req_sdptype==1,"caller","called"),
									TRUE_SELECT(m_stBody.rsp_sdptype==1,"caller","called"),
									m_stBody.retrans_flag,m_stBody.to_uri,m_stBody.from_uri,
									m_stBody.content_type,m_stBody.content_enc,m_stBody.user_agent,
									unAdditionalLen);
		#else
		fnRUStrnCat(_pszLog,_unSize,"<body param> calltype=%s,sdp_oa=%s,caller_tid=%d,caller_trte=%d,"\
									"reinvite_rsp=%d,called_tid=%d,called_trte=%d,nreserved2=%d,"\
									"req_sdptype=%s,rsp_sdptype=%s,"\
									"retrans_flag=%d,to_uri=%sfrom_uri=%scontent_type=%s,"\
									"content_enc=%s,ua=%s,sdpLen=%u\r\n",
									TRUE_SELECT(m_stBody.calltype==0,"reqular","early"),
									TRUE_SELECT(m_stBody.sdp_oa==0,"offer","answer"),
									m_stBody.caller_tid,m_stBody.caller_trte,
									m_stBody.reinvite_rsp,m_stBody.called_tid,
									m_stBody.called_trte,m_stBody.nreserved2,
									TRUE_SELECT(m_stBody.req_sdptype==1,"caller","called"),
									TRUE_SELECT(m_stBody.rsp_sdptype==1,"caller","called"),
									m_stBody.retrans_flag,m_stBody.to_uri,m_stBody.from_uri,
									m_stBody.content_type,m_stBody.content_enc,m_stBody.user_agent,
									unAdditionalLen);
		#endif
		if(unAdditionalLen > 0)
		{
			if(m_stBody.content_enc[0]==0)
			{
				fnRUStrnCat(_pszLog,_unSize,"<SDP>\r\n%s",(char*)m_fnGetSdp(false));
			}
			else
			{
				fnRUStrnCat(_pszLog,_unSize,"<Encoded SDP>\r\n");
				fnRUHexnCat(m_fnGetSdp(false),unAdditionalLen, _pszLog,_unSize);
			}
		}
		return _pszLog;
	}
	void * m_fnGetSdp(bool _bOdering)
	{	
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	void m_fnSetSdp(const void * _pvSdp, unsigned int _unLen)
	{
		memcpy(this+1,_pvSdp,_unLen);
		m_stHdr.m_stCom.m_stCom.length = sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody) + _unLen;
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		return m_stHdr.m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetAdditionalDataLen(bool _bOdering)
	{
		unsigned int unAdditionalLen = m_fnGetBodyLength(_bOdering);
		unAdditionalLen = unAdditionalLen - sizeof(m_stBody) - sizeof(TrGW_ApiMsgHdr_t);
		return unAdditionalLen;
	}
}TrgwModReqMsg_t;
inline char * g_fnStringTgASModReq(char * _pszLog,unsigned int _unSize,TrgwModReqMsg_t * _pstPk)
{
	unsigned int unAdditionalLen = _pstPk->m_fnGetAdditionalDataLen(false);
	fnRUStrnCat(_pszLog,_unSize,"<TrgwModReqMsg_t>\r\n");
	g_fnStringTgASMedia(_pszLog,_unSize,&_pstPk->m_stHdr);
	fnRUStrnCat(_pszLog,_unSize,"  calltype = %d (%s)\r\n",_pstPk->m_stBody.calltype,
											TRUE_SELECT(_pstPk->m_stBody.calltype==0,"reqular","early"));
	fnRUStrnCat(_pszLog,_unSize,"  sdp_oa = %d (%s)\r\n",_pstPk->m_stBody.sdp_oa,
											TRUE_SELECT(_pstPk->m_stBody.sdp_oa==0,"offer","answer"));
	fnRUStrnCat(_pszLog,_unSize,"  caller_tid = %d\r\n",_pstPk->m_stBody.caller_tid);
	fnRUStrnCat(_pszLog,_unSize,"  caller_trte = %d\r\n",_pstPk->m_stBody.caller_trte);
	#ifdef DEF_R126
	fnRUStrnCat(_pszLog,_unSize,"  caller_gid = %d\r\n",_pstPk->m_stBody.caller_gid);  // R126
	#endif
	fnRUStrnCat(_pszLog,_unSize,"  reinvite_rsp = %d\r\n",_pstPk->m_stBody.reinvite_rsp);
	fnRUStrnCat(_pszLog,_unSize,"  called_tid = %d\r\n",_pstPk->m_stBody.called_tid);
	fnRUStrnCat(_pszLog,_unSize,"  called_trte = %d\r\n",_pstPk->m_stBody.called_trte);
	#ifdef DEF_R126
	fnRUStrnCat(_pszLog,_unSize,"  called_gid = %d\r\n",_pstPk->m_stBody.called_gid);  // R126
	#endif
	fnRUStrnCat(_pszLog,_unSize,"  nreserved2 = %d\r\n",_pstPk->m_stBody.nreserved2);
	fnRUStrnCat(_pszLog,_unSize,"  req_sdptype = %d (%s)\r\n",_pstPk->m_stBody.req_sdptype,
										TRUE_SELECT(_pstPk->m_stBody.req_sdptype==1,"caller","called"));
	fnRUStrnCat(_pszLog,_unSize,"  rsp_sdptype = %d (%s)\r\n",_pstPk->m_stBody.rsp_sdptype,
										TRUE_SELECT(_pstPk->m_stBody.rsp_sdptype==1,"caller","called"));
	fnRUStrnCat(_pszLog,_unSize,"  retrans_flag = %d\r\n",_pstPk->m_stBody.retrans_flag);
	fnRUStrnCat(_pszLog,_unSize,"  to_uri = %s\r\n",_pstPk->m_stBody.to_uri);
	fnRUStrnCat(_pszLog,_unSize,"  from_uri = %s\r\n",_pstPk->m_stBody.from_uri);
	fnRUStrnCat(_pszLog,_unSize,"  content_type = %s\r\n",_pstPk->m_stBody.content_type);
	fnRUStrnCat(_pszLog,_unSize,"  content_enc = %s\r\n",_pstPk->m_stBody.content_enc);
	fnRUStrnCat(_pszLog,_unSize,"  user_agent = %s\r\n\r\n",_pstPk->m_stBody.user_agent);
	if(unAdditionalLen > 0)
	{
		if(_pstPk->m_stBody.content_enc[0]==0)
		{
			fnRUStrnCat(_pszLog,_unSize,"<SDP>\r\n%s",(char*)_pstPk->m_fnGetSdp(false));
		}
		else
		{
			fnRUStrnCat(_pszLog,_unSize,"<Encoded SDP>\r\n");
			fnRUHexnCat(_pstPk->m_fnGetSdp(false),unAdditionalLen, _pszLog,_unSize);
		}
	}
	return _pszLog;
}
inline TrgwModReqMsg_t * g_fnCreateModReq(unsigned int _unSdpSize)
{
	char * pszNew = new char[sizeof(TrgwModReqMsg_t) + _unSdpSize];
	memset(pszNew,0x00,sizeof(TrgwModReqMsg_t) + _unSdpSize);
	TrgwModReqMsg_t * pclsNew = (TrgwModReqMsg_t *)pszNew;
	pclsNew->m_fnClear();return pclsNew;
}
typedef struct TrgwModRspMsg_t
{
	TrgwMediaMsg_t m_stHdr;
	struct
	{
		int   result;
		int   reason;
		char content_type[CONTENT_TYPE_LEN];
		char content_enc[CONTENT_ENC_LEN];
		char video_use; // 0:audio 1:video
		char reserved[7];
	}m_stBody;
	TrgwModRspMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stHdr,0x00,sizeof(TrgwMediaMsg_t));
		m_stHdr.m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_TRGW;
		m_stHdr.m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_PROC_MSG;
		m_stHdr.m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_TRGW;
		m_stHdr.m_stCom.m_stCom.src_proc = 0;     // src process id
		m_stHdr.m_stCom.m_stCom.dst_proc = 0;     // dst process id
		m_stHdr.m_stCom.m_stCom.src_tag = 0;       // src session id
		m_stHdr.m_stCom.m_stCom.dst_tag = 0;       // dst session id
		m_stHdr.m_stCom.m_stCom.length = sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody);
		m_stHdr.m_stTrgw.primid = PRI_TYPE_MOD_RSP;
		m_stHdr.m_stTrgw.Trans_Id = 0;
		memset(&m_stBody,0x00,sizeof(m_stBody));
	}
	void m_fnHtoN()
	{
		m_stHdr.m_fnHtoN();
		m_stBody.result = htonl(m_stBody.result);
		m_stBody.reason = htonl(m_stBody.reason);
	}
	void m_fnNtoH()
	{
		m_stHdr.m_fnNtoH();
		m_stBody.result = ntohl(m_stBody.result);
		m_stBody.reason = ntohl(m_stBody.reason);
	}
	void m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		unsigned int unAdditionalLen = m_fnGetAdditionalDataLen(false);
		fnRUStrnCat(_pszLog,_unSize,"<<<TrgwModRspMsg_t>>>\r\n");
		m_stHdr.m_fnStr(_pszLog,_unSize);
		fnRUStrnCat(_pszLog,_unSize,
			"<body param> result=%d,reason=%d,content_type=%s,content_enc=%s,video_use=%s,sdpLen=%u\r\n",
			m_stBody.result,m_stBody.reason,m_stBody.content_type,m_stBody.content_enc,
			TRUE_SELECT(m_stBody.video_use==0,"audio","video"),unAdditionalLen);
		if(m_stBody.result == -1)
		{
			fnRUStrnCat(_pszLog,_unSize,"<Error String>\r\n%s",m_fnGetReason(false));
		}
		else if(unAdditionalLen > 0)
		{
			if(m_stBody.content_enc[0]==0)
			{
				fnRUStrnCat(_pszLog,_unSize,"<SDP>\r\n%s",(char*)m_fnGetSdp(false));
			}
			else
			{
				fnRUStrnCat(_pszLog,_unSize,"<Encoded SDP>\r\n");
				fnRUHexnCat(m_fnGetSdp(false),unAdditionalLen, _pszLog,_unSize);
			}
		}
	}
	void * m_fnGetSdp(bool _bOdering)
	{	
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	char * m_fnGetReason(bool _bOdering)
	{	
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	void m_fnSetSdp(const void * _pvSdp, unsigned int _unLen)
	{
		memcpy(this+1,_pvSdp,_unLen);
		m_stHdr.m_stCom.m_stCom.length = sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody) + _unLen;
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		return m_stHdr.m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetAdditionalDataLen(bool _bOdering)
	{
		unsigned int unAdditionalLen = m_fnGetBodyLength(_bOdering);
		unAdditionalLen = unAdditionalLen - sizeof(m_stBody) - sizeof(TrGW_ApiMsgHdr_t);
		return unAdditionalLen;
	}
}TrgwModRspMsg_t;
inline char * g_fnStringTgASModRsp(char * _pszLog,unsigned int _unSize,TrgwModRspMsg_t * _pstPk)
{
	unsigned int unAdditionalLen = _pstPk->m_fnGetAdditionalDataLen(false);
	fnRUStrnCat(_pszLog,_unSize,"<TrgwModRspMsg_t>\r\n");
	g_fnStringTgASMedia(_pszLog,_unSize,&_pstPk->m_stHdr);
	fnRUStrnCat(_pszLog,_unSize,"  result = %d\r\n",_pstPk->m_stBody.result);
	fnRUStrnCat(_pszLog,_unSize,"  reason = 0x%x\r\n",_pstPk->m_stBody.reason);
	fnRUStrnCat(_pszLog,_unSize,"  content_type = %s\r\n",_pstPk->m_stBody.content_type);
	fnRUStrnCat(_pszLog,_unSize,"  content_enc = %s\r\n",_pstPk->m_stBody.content_enc);	
	fnRUStrnCat(_pszLog,_unSize,"  video_use = %d\r\n",_pstPk->m_stBody.video_use);	
	fnRUStrnCat(_pszLog,_unSize,"  reserved = %s\r\n\r\n",_pstPk->m_stBody.reserved);	
	if(_pstPk->m_stBody.result == -1)
	{
		fnRUStrnCat(_pszLog,_unSize,"<Error String>\r\n%s",_pstPk->m_fnGetReason(false));
	}
	else if(unAdditionalLen > 0)
	{
		if(_pstPk->m_stBody.content_enc[0]==0)
		{
			fnRUStrnCat(_pszLog,_unSize,"<SDP>\r\n%s",(char*)_pstPk->m_fnGetSdp(false));
		}
		else
		{
			fnRUStrnCat(_pszLog,_unSize,"<Encoded SDP>\r\n");
			fnRUHexnCat(_pstPk->m_fnGetSdp(false),unAdditionalLen, _pszLog,_unSize);
		}
	}
	return _pszLog;
}
inline TrgwModRspMsg_t * g_fnCreateModRsp(unsigned int _unSdpSize)
{
	char * pszNew = new char[sizeof(TrgwModRspMsg_t) + _unSdpSize];
	memset(pszNew,0x00,sizeof(TrgwModRspMsg_t) + _unSdpSize);
	TrgwModRspMsg_t * pclsNew = (TrgwModRspMsg_t *)pszNew;
	pclsNew->m_fnClear();return pclsNew;
}
typedef struct TrgwDelReqMsg_t
{
	TrgwMediaMsg_t m_stHdr;
	TrgwDelReqMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stHdr,0x00,sizeof(TrgwMediaMsg_t));
		m_stHdr.m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_TRGW;
		m_stHdr.m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_PROC_MSG;
		m_stHdr.m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_TRGW;
		m_stHdr.m_stCom.m_stCom.src_proc = 0;     // src process id
		m_stHdr.m_stCom.m_stCom.dst_proc = 0;     // dst process id
		m_stHdr.m_stCom.m_stCom.src_tag = 0;       // src session id
		m_stHdr.m_stCom.m_stCom.dst_tag = 0;       // dst session id
		m_stHdr.m_stCom.m_stCom.length = sizeof(_TrGW_ApiMsgHdr_t);
		m_stHdr.m_stTrgw.primid = PRI_TYPE_DEL_REQ;
		m_stHdr.m_stTrgw.Trans_Id = 0;
	}
	void m_fnHtoN(){m_stHdr.m_fnHtoN();}
	void m_fnNtoH(){m_stHdr.m_fnNtoH();}
	void m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		fnRUStrnCat(_pszLog,_unSize,"<<<TrgwDelReqMsg_t>>>\r\n");
		m_stHdr.m_fnStr(_pszLog,_unSize);
	}
	char * m_fnGetReason(bool _bOdering)
	{	
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	unsigned int m_fnGetBodyLength(bool _bOdering){return m_stHdr.m_fnGetBodyLength(_bOdering);}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetAdditionalDataLen(bool _bOdering)
	{
		unsigned int unAdditionalLen = m_fnGetBodyLength(_bOdering);
		unAdditionalLen = unAdditionalLen - sizeof(TrGW_ApiMsgHdr_t);
		return unAdditionalLen;
	}
}TrgwDelReqMsg_t;
inline char * g_fnStringTgASDelReq(char * _pszLog,unsigned int _unSize,TrgwDelReqMsg_t * _pstPk)
{
	fnRUStrnCat(_pszLog,_unSize,"<TrgwDelReqMsg_t>\r\n");
	g_fnStringTgASMedia(_pszLog,_unSize,&_pstPk->m_stHdr);
	return _pszLog;
}
inline TrgwDelReqMsg_t * g_fnCreateDelReq(unsigned int _unSdpSize)
{
	char * pszNew = new char[sizeof(TrgwDelReqMsg_t) + _unSdpSize];
	memset(pszNew,0x00,sizeof(TrgwDelReqMsg_t) + _unSdpSize);
	TrgwDelReqMsg_t * pclsNew = (TrgwDelReqMsg_t *)pszNew;
	pclsNew->m_fnClear();return pclsNew;
}
typedef struct TrgwDelRspMsg_t
{
	TrgwMediaMsg_t m_stHdr;
	struct
	{
		int   result;
		int   reason;
	}m_stBody;
	TrgwDelRspMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stHdr,0x00,sizeof(TrgwMediaMsg_t));
		m_stHdr.m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_TRGW;
		m_stHdr.m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_PROC_MSG;
		m_stHdr.m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_TRGW;
		m_stHdr.m_stCom.m_stCom.src_proc = 0;     // src process id
		m_stHdr.m_stCom.m_stCom.dst_proc = 0;     // dst process id
		m_stHdr.m_stCom.m_stCom.src_tag = 0;       // src session id
		m_stHdr.m_stCom.m_stCom.dst_tag = 0;       // dst session id
		m_stHdr.m_stCom.m_stCom.length =  sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody);
		m_stHdr.m_stTrgw.primid = PRI_TYPE_DEL_RSP;
		m_stHdr.m_stTrgw.Trans_Id = 0;
		memset(&m_stBody,0x00,sizeof(m_stBody));
	}
	void m_fnHtoN()
	{
		m_stHdr.m_fnHtoN();
		m_stBody.result = htonl(m_stBody.result);
		m_stBody.reason = htonl(m_stBody.reason);
	}
	void m_fnNtoH()
	{
		m_stHdr.m_fnNtoH();
		m_stBody.result = ntohl(m_stBody.result);
		m_stBody.reason = ntohl(m_stBody.reason);
	}
	void m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		fnRUStrnCat(_pszLog,_unSize,"<<<TrgwDelRspMsg_t>>>\r\n");
		m_stHdr.m_fnStr(_pszLog,_unSize);
		fnRUStrnCat(_pszLog,_unSize,	"<body param> result=%d,reason=%d\r\n<Error String>\r\n%s\r\n",
												m_stBody.result,m_stBody.reason,m_fnGetReason(false));
	}
	char * m_fnGetReason(bool _bOdering)
	{	
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	unsigned int m_fnGetBodyLength(bool _bOdering){return m_stHdr.m_fnGetBodyLength(_bOdering);}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetAdditionalDataLen(bool _bOdering)
	{
		unsigned int unAdditionalLen = m_fnGetBodyLength(_bOdering);
		unAdditionalLen = unAdditionalLen - sizeof(m_stBody) - sizeof(TrGW_ApiMsgHdr_t);
		return unAdditionalLen;
	}
}TrgwDelRspMsg_t;
inline char * g_fnStringTgASDelRsp(char * _pszLog,unsigned int _unSize,TrgwDelRspMsg_t * _pstPk)
{
	fnRUStrnCat(_pszLog,_unSize,"<TrgwDelRspMsg_t>\r\n");
	g_fnStringTgASMedia(_pszLog,_unSize,&_pstPk->m_stHdr);
	fnRUStrnCat(_pszLog,_unSize,"  result = %d\r\n",_pstPk->m_stBody.result);
	fnRUStrnCat(_pszLog,_unSize,"  reason = 0x%x\r\n",_pstPk->m_stBody.reason);
	return _pszLog;
}
typedef struct TrgwChgReqMsg_t
{
	TrgwMediaMsg_t m_stHdr;
	struct
	{
		int caller_tid;
		int caller_trte;
		int called_tid;
		int called_trte;
		int reserved1;
		int rsp_sdptype;
		char to_uri[UIR_LEN];
		char from_uri[UIR_LEN];
	}m_stBody;
	TrgwChgReqMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stHdr,0x00,sizeof(TrgwMediaMsg_t));
		m_stHdr.m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_TRGW;
		m_stHdr.m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_PROC_MSG;
		m_stHdr.m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_TRGW;
		m_stHdr.m_stCom.m_stCom.src_proc = 0;     // src process id
		m_stHdr.m_stCom.m_stCom.dst_proc = 0;     // dst process id
		m_stHdr.m_stCom.m_stCom.src_tag = 0;       // src session id
		m_stHdr.m_stCom.m_stCom.dst_tag = 0;       // dst session id
		m_stHdr.m_stCom.m_stCom.length =  sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody);
		m_stHdr.m_stTrgw.primid = PRI_TYPE_SDP_REQ;
		m_stHdr.m_stTrgw.Trans_Id = 0;
		memset(&m_stBody,0x00,sizeof(m_stBody));
	}
	void m_fnHtoN()
	{
		m_stHdr.m_fnHtoN();
		m_stBody.caller_tid = htonl(m_stBody.caller_tid);
		m_stBody.caller_trte = htonl(m_stBody.caller_trte);
		m_stBody.called_tid = htonl(m_stBody.called_tid);
		m_stBody.called_trte = htonl(m_stBody.called_trte);
		m_stBody.rsp_sdptype = htonl(m_stBody.rsp_sdptype);
	}
	void m_fnNtoH()
	{
		m_stHdr.m_fnNtoH();
		m_stBody.caller_tid = ntohl(m_stBody.caller_tid);
		m_stBody.caller_trte = ntohl(m_stBody.caller_trte);
		m_stBody.called_tid = ntohl(m_stBody.called_tid);
		m_stBody.called_trte = ntohl(m_stBody.called_trte);
		m_stBody.rsp_sdptype = ntohl(m_stBody.rsp_sdptype);
	}
	/* attach sdp message */
	char * m_fnGetReason(bool _bOdering)
	{	
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		return m_stHdr.m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetAdditionalDataLen(bool _bOdering)
	{
		unsigned int unAdditionalLen = m_fnGetBodyLength(_bOdering);
		unAdditionalLen = unAdditionalLen - sizeof(m_stBody) - sizeof(TrGW_ApiMsgHdr_t);
		return unAdditionalLen;
	}
	void m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		fnRUStrnCat(_pszLog,_unSize,"<<<TrgwChgReqMsg_t>>>\r\n");
		m_stHdr.m_fnStr(_pszLog,_unSize);
		fnRUStrnCat(_pszLog,_unSize,
							"<body param> caller_tid=%d, caller_trte=%d, called_tid=%d, called_trte=%d"\
							"rsp_sdptype=%d,to_uri=%s,from_uri=%s\r\n",
							m_stBody.caller_tid,m_stBody.caller_trte,
							m_stBody.called_tid,m_stBody.called_trte,m_stBody.rsp_sdptype,
							fnRUStr(m_stBody.to_uri),fnRUStr(m_stBody.from_uri));
	}
}TrgwChgReqMsg_t;
inline char * g_fnStringTgASChgReq(char * _pszLog,unsigned int _unSize,TrgwChgReqMsg_t * _pstPk)
{
	fnRUStrnCat(_pszLog,_unSize,"<TrgwChgReqMsg_t>\r\n");
	g_fnStringTgASMedia(_pszLog,_unSize,&_pstPk->m_stHdr);
	fnRUStrnCat(_pszLog,_unSize,"  caller_tid = %d\r\n",_pstPk->m_stBody.caller_tid);
	fnRUStrnCat(_pszLog,_unSize,"  caller_trte = %d\r\n",_pstPk->m_stBody.caller_trte);
	fnRUStrnCat(_pszLog,_unSize,"  called_tid = %d\r\n",_pstPk->m_stBody.called_tid);
	fnRUStrnCat(_pszLog,_unSize,"  called_trte = %d\r\n",_pstPk->m_stBody.called_trte);
	fnRUStrnCat(_pszLog,_unSize,"  reserved1 = %d\r\n",_pstPk->m_stBody.reserved1);
	fnRUStrnCat(_pszLog,_unSize,"  rsp_sdptype = %d (%s)\r\n",_pstPk->m_stBody.rsp_sdptype,
									TRUE_SELECT(_pstPk->m_stBody.rsp_sdptype==1,"caller","called"));
	fnRUStrnCat(_pszLog,_unSize,"  to_uri = %s\r\n",_pstPk->m_stBody.to_uri);
	fnRUStrnCat(_pszLog,_unSize,"  from_uri = %s\r\n\r\n",_pstPk->m_stBody.from_uri);
	return _pszLog;
}
inline TrgwChgReqMsg_t * g_fnCreateChgReq(unsigned int _unSdpSize)
{
	char * pszNew = new char[sizeof(TrgwChgReqMsg_t) + _unSdpSize];
	memset(pszNew,0x00,sizeof(TrgwChgReqMsg_t) + _unSdpSize);
	TrgwChgReqMsg_t * pclsNew = (TrgwChgReqMsg_t *)pszNew;
	pclsNew->m_fnClear();return pclsNew;
}
typedef struct TrgwChgRspMsg_t
{
	TrgwMediaMsg_t m_stHdr;
	struct
	{
		int   result;
		int   reason;
		char content_type[CONTENT_TYPE_LEN];
		char content_enc[CONTENT_ENC_LEN];
	}m_stBody;
	TrgwChgRspMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stHdr,0x00,sizeof(TrgwMediaMsg_t));
		m_stHdr.m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_TRGW;
		m_stHdr.m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_PROC_MSG;
		m_stHdr.m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_TRGW;
		m_stHdr.m_stCom.m_stCom.src_proc = 0;     // src process id
		m_stHdr.m_stCom.m_stCom.dst_proc = 0;     // dst process id
		m_stHdr.m_stCom.m_stCom.src_tag = 0;       // src session id
		m_stHdr.m_stCom.m_stCom.dst_tag = 0;       // dst session id
		m_stHdr.m_stCom.m_stCom.length =  sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody);
		m_stHdr.m_stTrgw.primid = PRI_TYPE_SDP_RSP;
		m_stHdr.m_stTrgw.Trans_Id = 0;
		memset(&m_stBody,0x00,sizeof(m_stBody));
	}
	void m_fnHtoN()
	{
		m_stHdr.m_fnHtoN();
		m_stBody.result = htonl(m_stBody.result);
		m_stBody.reason = htonl(m_stBody.reason);
	}
	void m_fnNtoH()
	{
		m_stHdr.m_fnNtoH();
		m_stBody.result = ntohl(m_stBody.result);
		m_stBody.reason = ntohl(m_stBody.reason);
	}
	/* attach sdp message */
	void * m_fnGetSdp(bool _bOdering)
	{
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	char * m_fnGetReason(bool _bOdering)
	{	
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	void m_fnSetSdp(const void * _pvSdp, unsigned int _unLen)
	{
		memcpy(this+1,_pvSdp,_unLen);
		m_stHdr.m_stCom.m_stCom.length = sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody) + _unLen;
	}
	unsigned int m_fnGetBodyLength(bool _bOdering)
	{
		return m_stHdr.m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetAdditionalDataLen(bool _bOdering)
	{
		unsigned int unAdditionalLen = m_fnGetBodyLength(_bOdering);
		unAdditionalLen = unAdditionalLen - sizeof(m_stBody) - sizeof(TrGW_ApiMsgHdr_t);
		return unAdditionalLen;
	}
	void m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		fnRUStrnCat(_pszLog,_unSize,"<<<TrgwChgRspMsg_t>>>\r\n");
		m_stHdr.m_fnStr(_pszLog,_unSize);
		fnRUStrnCat(_pszLog,_unSize,
							"<body param> result=%d, reason=%d, content_type=%s, content_enc=%s\r\n",
							m_stBody.result,m_stBody.reason,m_stBody.content_type,m_stBody.content_enc);
	}
}TrgwChgRspMsg_t;
inline char * g_fnStringTgASChgRsp(char * _pszLog,unsigned int _unSize,TrgwChgRspMsg_t * _pstPk)
{
	unsigned int unAdditionalLen = _pstPk->m_fnGetAdditionalDataLen(false);
	fnRUStrnCat(_pszLog,_unSize,"<TrgwChgRspMsg_t>\r\n");
	g_fnStringTgASMedia(_pszLog,_unSize,&_pstPk->m_stHdr);
	fnRUStrnCat(_pszLog,_unSize,"  result = %d\r\n",_pstPk->m_stBody.result);
	fnRUStrnCat(_pszLog,_unSize,"  reason = 0x%x\r\n",_pstPk->m_stBody.reason);
	fnRUStrnCat(_pszLog,_unSize,"  content_type = %d\r\n",_pstPk->m_stBody.content_type);
	fnRUStrnCat(_pszLog,_unSize,"  content_enc = %d\r\n\r\n",_pstPk->m_stBody.content_enc);
	if(_pstPk->m_stBody.result == -1)
	{
		fnRUStrnCat(_pszLog,_unSize,"<Error String>\r\n%s",_pstPk->m_fnGetReason(false));
	}
	else if(unAdditionalLen > 0)
	{
		if(_pstPk->m_stBody.content_enc[0]==0)
		{
			fnRUStrnCat(_pszLog,_unSize,"<SDP>\r\n%s",(char*)_pstPk->m_fnGetSdp(false));
		}
		else
		{
			fnRUStrnCat(_pszLog,_unSize,"<Encoded SDP>\r\n");
			fnRUHexnCat(_pstPk->m_fnGetSdp(false),unAdditionalLen, _pszLog,_unSize);
		}
	}
	return _pszLog;
}
typedef struct TrgwArmIndMsg_t 
{
	TrgwMediaMsg_t m_stHdr;
	struct
	{
		int   result;
		int   reason;
		int   status;
	}m_stBody;
	TrgwArmIndMsg_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(&m_stHdr,0x00,sizeof(TrgwMediaMsg_t));
		m_stHdr.m_stCom.m_stCom.MMode = STACK_CMSAPI_MMODE_TRGW;
		m_stHdr.m_stCom.m_stCom.MType = STACK_CMSAPI_MTYPE_PROC_MSG;
		m_stHdr.m_stCom.m_stCom.SType = STACK_CMSAPI_CTYPE_TRGW;
		m_stHdr.m_stCom.m_stCom.src_proc = 0;     // src process id
		m_stHdr.m_stCom.m_stCom.dst_proc = 0;     // dst process id
		m_stHdr.m_stCom.m_stCom.src_tag = 0;       // src session id
		m_stHdr.m_stCom.m_stCom.dst_tag = 0;       // dst session id
		m_stHdr.m_stCom.m_stCom.length =  sizeof(_TrGW_ApiMsgHdr_t) + sizeof(m_stBody);
		m_stHdr.m_stTrgw.primid = PRI_TYPE_ARM_IND;
		m_stHdr.m_stTrgw.Trans_Id = 0;
		memset(&m_stBody,0x00,sizeof(m_stBody));
	}
	void m_fnHtoN()
	{
		m_stHdr.m_fnHtoN();
		m_stBody.result = htonl(m_stBody.result);
		m_stBody.reason = htonl(m_stBody.reason);
		m_stBody.status = htonl(m_stBody.status);
	}
	void m_fnNtoH()
	{
		m_stHdr.m_fnNtoH();
		m_stBody.result = ntohl(m_stBody.result);
		m_stBody.reason = ntohl(m_stBody.reason);
		m_stBody.status = ntohl(m_stBody.status);
	}
	void m_fnStr(char * _pszLog, unsigned int _unSize)
	{
		fnRUStrnCat(_pszLog,_unSize,"<<<TrgwArmInd_t>>>\r\n");
		m_stHdr.m_fnStr(_pszLog,_unSize);
		fnRUStrnCat(_pszLog,_unSize,
									"<body param> result=%d,reason=%d\r\n<Error String>\r\n%s",
													m_stBody.result,m_stBody.reason,m_fnGetReason(false));
	}
	char * m_fnGetReason(bool _bOdering)
	{	
		if(m_fnGetAdditionalDataLen(_bOdering) > 0) return (char*)(this+1);	
		else return NULL;
	}
	unsigned int m_fnGetBodyLength(bool _bOdering){return m_stHdr.m_fnGetBodyLength(_bOdering);}
	unsigned int m_fnGetTotalLength(bool _bOdering)
	{
		return sizeof(TrgwComMsg_t) + m_fnGetBodyLength(_bOdering);
	}
	unsigned int m_fnGetAdditionalDataLen(bool _bOdering)
	{
		unsigned int unAdditionalLen = m_fnGetBodyLength(_bOdering);
		unAdditionalLen = unAdditionalLen - sizeof(m_stBody) - sizeof(TrGW_ApiMsgHdr_t);
		return unAdditionalLen;
	}
} TrgwArmIndMsg_t;
inline void g_fnTrgwPkLog(bool _bSnd,void * _pvTrgw)
{
	TrgwMediaMsg_t * pclsHdr = (TrgwMediaMsg_t*)_pvTrgw;
	char szDebug[DEF_RU_SZ_LOG]; memset(szDebug,0x00,DEF_RU_SZ_LOG);
	if(1)//pclsHdr->m_stCom.m_stCom.MMode == STACK_CMSAPI_MMODE_TRGW)
	{
		if(pclsHdr->m_stTrgw.primid == (int)(PRI_TYPE_ADD_REQ))
		{
			TrgwAddReqMsg_t * pAddReq = (TrgwAddReqMsg_t*)_pvTrgw;
			pAddReq->m_fnStr(szDebug,DEF_RU_SZ_LOG);
		}
		else if(pclsHdr->m_stTrgw.primid == (int)(PRI_TYPE_MOD_REQ))
		{
			TrgwModReqMsg_t * pModReq = (TrgwModReqMsg_t*)_pvTrgw;
			pModReq->m_fnStr(szDebug,DEF_RU_SZ_LOG);
		}
		else if(pclsHdr->m_stTrgw.primid == (int)(PRI_TYPE_DEL_REQ))
		{
			TrgwDelReqMsg_t * pDelReq = (TrgwDelReqMsg_t*)_pvTrgw;
			pDelReq->m_fnStr(szDebug,DEF_RU_SZ_LOG);
		}
		else if(pclsHdr->m_stTrgw.primid == (int)(PRI_TYPE_ADD_RSP))
		{
			TrgwAddRspMsg_t * pAddRsp = (TrgwAddRspMsg_t*)_pvTrgw;
			pAddRsp->m_fnStr(szDebug,DEF_RU_SZ_LOG);
		}
		else if(pclsHdr->m_stTrgw.primid == (int)(PRI_TYPE_MOD_RSP))
		{
			TrgwModRspMsg_t * pModRsp = (TrgwModRspMsg_t*)_pvTrgw;
			pModRsp->m_fnStr(szDebug,DEF_RU_SZ_LOG);
		}
		else if(pclsHdr->m_stTrgw.primid == (int)(PRI_TYPE_DEL_RSP))
		{
			TrgwDelRspMsg_t * pDelRsp = (TrgwDelRspMsg_t*)_pvTrgw;
			pDelRsp->m_fnStr(szDebug,DEF_RU_SZ_LOG);
		}
		else if(pclsHdr->m_stTrgw.primid == (int)(PRI_TYPE_SDP_REQ))
		{
			TrgwChgReqMsg_t * pChgReq = (TrgwChgReqMsg_t*)_pvTrgw;
			pChgReq->m_fnStr(szDebug,DEF_RU_SZ_LOG);
		}
		else if(pclsHdr->m_stTrgw.primid == (int)(PRI_TYPE_SDP_RSP))
		{
			TrgwChgRspMsg_t * pChgRsp = (TrgwChgRspMsg_t*)_pvTrgw;
			pChgRsp->m_fnStr(szDebug,DEF_RU_SZ_LOG);
		}
		else if(pclsHdr->m_stTrgw.primid == (int)(PRI_TYPE_ARM_IND))
		{
			TrgwArmIndMsg_t * pChgRsp = (TrgwArmIndMsg_t*)_pvTrgw;
			pChgRsp->m_fnStr(szDebug,DEF_RU_SZ_LOG);
		}
		RLOG(((RUSTR)BoxString(szDebug)));
	}
}
}
#endif

