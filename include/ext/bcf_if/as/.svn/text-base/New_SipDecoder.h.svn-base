
#ifndef __NEWSIPDECODER__
#define __NEWSIPDECODER__

#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#ifdef _LINUX_
#include <sys/time.h>
#endif
#include "Sam_Com.h"

#ifndef NEWSIP_LOG
#include "CSam_Log.h"
#define NEWSIP_LOG(v...)         CSam_Log::pSingleton->Log(v)
#endif

#define NEW_MAX_PARES_BUFFER         10240
#define NEW_MAX_PARSE_PACKET         5120

typedef bool (*pMainDecHandler)(void  *k_data,char input_chr);
typedef bool (*pSubDecHandler)(void *k_data,void *pLine, char input_chr);

typedef enum
{
	PARSE_GROUP_NONE,
	PARSE_GROUP_SIP,
	PARSE_GROUP_SDPCOMMON,
	PARSE_GROUP_SDPDESC_1,
	PARSE_GROUP_SDPDESC_2,
	PARSE_GROUP_SDPDESC_3,
	PARSE_GROUP_SDPDESC_4,
	PARSE_GROUP_SDPDESC_5,
	PARSE_GROUP_SDPDESC_6,
	PARSE_GROUP_SDPDESC_7,
	PARSE_GROUP_SDPDESC_8,
	PARSE_GROUP_SDPDESC_OTHER,
	PARSE_GROUP_OTHER,
}e_parse_group;
typedef enum
{
	SIP_HDR_NONE,
	SIP_HDR_METHOD,
	SIP_HDR_ACCEPT,
	SIP_HDR_ACCEPTENCODING,
	SIP_HDR_ACCEPTLANGUAGE	,
	SIP_HDR_ALERTINFO,
	SIP_HDR_ALLOW	,
	SIP_HDR_ALLOW_EVENTS,
	SIP_HDR_AUTHORIZATION,
	SIP_HDR_CALLID,
	SIP_HDR_CONTACT,    // 10
	SIP_HDR_CONTENTLENGTH,
	SIP_HDR_CONTENTTYPE,
	SIP_HDR_CSEQ,
	SIP_HDR_DATE,
	SIP_HDR_EVENT,
	SIP_HDR_EXPIRES,
	SIP_HDR_FROM,
	SIP_HDR_MAXFORWARDS,
	SIP_HDR_PNUATYPE,
	SIP_HDR_PROXYAUTHENTICATE,      // 20
	SIP_HDR_PROXYAUTHORIZATION,
	SIP_HDR_PROXYREQUIRE,
	SIP_HDR_QUEST	,
	SIP_HDR_RECORDROUTE,
	SIP_HDR_RETRYAFTER,
	SIP_HDR_REQUIRE	,
	SIP_HDR_ROUTE	,
	SIP_HDR_SERVER,
	SIP_HDR_SUPPORTED,
	SIP_HDR_UNSUPPORTED,     // 30
	SIP_HDR_TIMESTAMP,
	SIP_HDR_TO,
	SIP_HDR_USERAGENT,
	SIP_HDR_WARNING,
	SIP_HDR_WWWAUTHENTICATE,
	SIP_HDR_RACK,
	SIP_HDR_RSEQ,
	SIP_HDR_ALSO,
	SIP_HDR_REQUESTEDBY,
	SIP_HDR_REFERTO,    // 40
	SIP_HDR_SREFERTO,
	SIP_HDR_REFERREDBY,
	SIP_HDR_SESSIONEXPIRES,
	SIP_HDR_MINSE,
	SIP_HDR_REMOTEPARTYID,
	SIP_HDR_RPIDPRIVACY,
	SIP_HDR_DIVERSION,
	SIP_HDR_PASSERTEDIDENTITY	,
	SIP_HDR_PPREFERREDIDENTITY,
	SIP_HDR_PCALLEDPARTID,     // 50
	SIP_HDR_SMS_INDICATOR,
	SIP_HDR_SMS_REPLY,
	SIP_HDR_VIA,
	SIP_HDR_BODY,   
	SIP_HDR_OMC_CMD,
	SIP_HDR_OMC_TIME,
	SIP_HDR_OMC_TRANS,
	SIP_HDR_OTHER,
	SIP_HDR_MAX,         // 60
	
}e_sip_hdr;
typedef enum
{
	SDP_V = 0x400,       // v=<protocol version>
	SDP_O,       // o=<Origin>
	SDP_S,       // s=<Session name>
	SDP_I,        // i=<Session Information>
	SDP_U,       // u=<URI>
	SDP_E,       // e=<E-mail Address>
	SDP_P,       // p=<Phone Number>
	SDP_C,       // c=<Connection Data>
	SDP_B,       // b=<Bandwidth>
	SDP_T,       // t=<Timing>
	SDP_R,       // r=<Repeat Times>
	SDP_Z,       // z=<Tme Zones>
	SDP_K,        // k=<Encryption Keys>
	SDP_M,       //m=<media> <port>/<number of ports> <proto> <fmt> ...
	SDP_A,        //a=<attri_type>:<attribute specific parameters>
	SDP_OTHER,
}e_sdp_hdr;
typedef enum
{
	SDP_ATTRIBUTE_CAT = 0x800,               //a=cat:<category>
	SDP_ATTRIBUTE_KEYWDS,        //a=keywds:<keywords>
	SDP_ATTRIBUTE_TOOL,             //a=tool:<name and version of tool>
	SDP_ATTRIBUTE_PTIME,            //a=ptime:<packet time>
	SDP_ATTRIBUTE_MAXPTIME,      //a=maxptime:<maximum packet time>
	SDP_ATTRIBUTE_RTPMAP,          //a=rtpmap:<payload type> <encoding name>/<clock rate> [/<encoding parameters>]
	SDP_ATTRIBUTE_ORIENT,           //a=orient:<orientation>
	SDP_ATTRIBUTE_TYPE,              //a=type:<conference type>
	SDP_ATTRIBUTE_CHARSET,        //a=charset:<character set>
	SDP_ATTRIBUTE_SDPLANG,         //a=sdplang:<language tag>
	SDP_ATTRIBUTE_LANG,               //a=lang:<language tag>
	SDP_ATTRIBUTE_FRAMERATE,      //a=framerate:<frame rate>
	SDP_ATTRIBUTE_QUALITY,          //a=quality:<quality>
	SDP_ATTRIBUTE_FMTP,               //a=fmtp:<format> <format specific parameters>
	SDP_ATTRIBUTE_FRAMESIZE,       //a=framesize:<type>
	SDP_ATTRIBUTE_CANDIDATE,      // a=candidate:<format>
	SDP_ATTRIBUTE_VISIT_REALM,
	SDP_ATTRIBUTE_OMR_M_CK,
	SDP_ATTRIBUTE_OMR_S_CK,
	SDP_ATTRIBUTE_OTHER,             //a="opt_string"
}e_sdp_attribute_hdr;
typedef enum
{
	SIP_SUBSCRIBE,
	SIP_REGISTER,
	SIP_OPTIONS,
	SIP_MESSAGE,
	SIP_INVITE,
	SIP_CANCEL,
	SIP_UPDATE,
	SIP_NOTIFY,
	SIP_REFER,
	SIP_QUEST,
	SIP_PRACK,
	SIP_INFO,
	SIP_PING,
	SIP_ACK,
	SIP_BYE,
	SIP_RSP,
	SIP_UNKNOWN,
}e_request_dec;
typedef enum
{
	PARSE_PROTO_NONE,
	PARSE_PROTO_SIP,
	PARSE_PROTO_SDP,
}e_parse_proto;
typedef enum
{
	SIP_PARSE_IDLE,
	SIP_PARSE_METHOD,
	SIP_PARSE_METHOD_DATA,
	SIP_PARSE_DETECT,
	SIP_PARSE_DATA,
	SIP_PARSE_BODY,
	SIP_PARSE_COM_DATA,
	SIP_PARSE_ATT_HDR,
	SIP_PARSE_ATT_DATA,
}e_func_type;
typedef enum
{
	SIP_SUBPARSE_IDLE,
	SIP_SUBPARSE_MATCH_TRIM,
	SIP_SUBPARSE_MATCH_PATTERN,
}e_subfunc_type;
typedef enum 
{
	SDP_MTYPE_NOTSET   = 0,
	SDP_MTYPE_AUDIO,               //  audio  
	SDP_MTYPE_NAS,                  //  nas    
	SDP_MTYPE_VIDEO,               //  video    
	SDP_MTYPE_APP,                   //  application 
	SDP_MTYPE_DATA,                 //  data    
	SDP_MTYPE_IMAGE,               //  image   
	SDP_MTYPE_CONTROL,           //  control 
	SDP_MTYPE_UNKNOWN
} e_sdp_hdr_m;
typedef enum 
{
	PARSE_CONTENT_EMPTY= 0,
	PARSE_CONTENT_APP_SDP,
	PARSE_CONTENT_APP_DTMF_RELAY,
	PARSE_CONTENT_TEXT,
	PARSE_CONTENT_OTHER
} e_parse_content;
inline const char *eString_Proto(int m)
{
	switch(m)
	{
		case PARSE_PROTO_NONE:	return "PROTO_NONE";
		case PARSE_PROTO_SIP:	return "PROTO_SIP";
		case PARSE_PROTO_SDP:	return "PROTO_SDP";
		default:	return "PARSE_PROTO_NONE";
	}
	return FX_NULL;
}
inline const char *eString_ParseState(int m)
{
	switch(m)
	{
		case SIP_PARSE_IDLE:	return "SIP_PARSE_IDLE";
		case SIP_PARSE_METHOD:	return "SIP_PARSE_METHOD";
		case SIP_PARSE_METHOD_DATA:	return "SIP_PARSE_METHOD_DATA";
		case SIP_PARSE_DETECT:	return "SIP_PARSE_DETECT";
		case SIP_PARSE_DATA:	return "SIP_PARSE_DATA";
		case SIP_PARSE_BODY:	return "SIP_PARSE_BODY";
		case SIP_PARSE_COM_DATA:	return "SIP_PARSE_COM_DATA";
		case SIP_PARSE_ATT_HDR:	return "SIP_PARSE_ATT_HDR";
		case SIP_PARSE_ATT_DATA:	return "SIP_PARSE_ATT_DATA";
		default:	return "SIP_PARSE_IDLE";
	}
	return FX_NULL;
}
inline const char *eString_SubParseState(int m)
{
	switch(m)
	{
		case SIP_SUBPARSE_IDLE:	return "SIP_SUBPARSE_IDLE";
		case SIP_SUBPARSE_MATCH_TRIM:return "SIP_SUBPARSE_MATCH_TRIM";
		case SIP_SUBPARSE_MATCH_PATTERN:return "SIP_SUBPARSE_MATCH_PATTERN";
		default:	return "SIP_SUBPARSE_IDLE";
	}
	return FX_NULL;
}
inline const char * eString_SipRequest(int m)
{
	switch(m)
	{
		case SIP_SUBSCRIBE:	return "SUBSCRIBE";
		case SIP_REGISTER:	return "REGISTER";
		case SIP_OPTIONS:	return "OPTIONS";
		case SIP_MESSAGE:	return "MESSAGE";
		case SIP_INVITE:		return "INVITE";
		case SIP_CANCEL:		return "CANCEL";
		case SIP_UPDATE:		return "UPDATE";
		case SIP_NOTIFY:		return "NOTIFY";
		case SIP_REFER:		return "REFER";
		case SIP_QUEST:		return "QUEST";
		case SIP_PRACK:		return "PRACK";
		case SIP_INFO:		return "INFO";
		case SIP_PING:		return "PING";
		case SIP_ACK:		return "ACK";
		case SIP_BYE:		return "BYE";
		case SIP_RSP:		return "RSP";
		case SIP_UNKNOWN:	return "UNKNOWN";
	}
	return FX_NULL;
}
inline const char * eString_SipHdr(int m)
{
	switch(m)
	{
		case SIP_HDR_NONE: return "MSGHDR_NONE";
		case SIP_HDR_METHOD: return "MSGHDR_METHOD";
		case SIP_HDR_ACCEPT: return "MSGHDR_ACCEPT";
		case SIP_HDR_ACCEPTENCODING: return "MSGHDR_ACCEPTENCODING";
		case SIP_HDR_ACCEPTLANGUAGE: return "MSGHDR_ACCEPTLANGUAGE";
		case SIP_HDR_ALERTINFO: return "MSGHDR_ALERTINFO";
		case SIP_HDR_ALLOW: return "MSGHDR_ALLOW";
		case SIP_HDR_ALLOW_EVENTS: return "MSGHDR_ALLOW_EVENTS";
		case SIP_HDR_AUTHORIZATION: return "MSGHDR_AUTHORIZATION";
		case SIP_HDR_CALLID: return "MSGHDR_CALLID";
		case SIP_HDR_CONTACT: return "MSGHDR_CONTACT";
		case SIP_HDR_CONTENTLENGTH: return "MSGHDR_CONTENTLENGTH";
		case SIP_HDR_CONTENTTYPE: return "MSGHDR_CONTENTTYPE";
		case SIP_HDR_CSEQ: return "MSGHDR_CSEQ";
		case SIP_HDR_DATE: return "MSGHDR_DATE";
		case SIP_HDR_EVENT: return "MSGHDR_EVENT";
		case SIP_HDR_EXPIRES: return "MSGHDR_EXPIRES";
		case SIP_HDR_FROM: return "MSGHDR_FROM";
		case SIP_HDR_MAXFORWARDS: return "MSGHDR_MAXFORWARDS";
		case SIP_HDR_PNUATYPE: return "MSGHDR_PNUATYPE";
		case SIP_HDR_PROXYAUTHENTICATE: return "MSGHDR_PROXYAUTHENTICATE";
		case SIP_HDR_PROXYAUTHORIZATION: return "MSGHDR_PROXYAUTHORIZATION";
		case SIP_HDR_PROXYREQUIRE: return "MSGHDR_PROXYREQUIRE";
		case SIP_HDR_QUEST: return "MSGHDR_QUEST";
		case SIP_HDR_RECORDROUTE: return "MSGHDR_RECORDROUTE";
		case SIP_HDR_RETRYAFTER: return "MSGHDR_RETRYAFTER";
		case SIP_HDR_REQUIRE: return "MSGHDR_REQUIRE";
		case SIP_HDR_ROUTE: return "MSGHDR_ROUTE";
		case SIP_HDR_SERVER: return "MSGHDR_SERVER";
		case SIP_HDR_SUPPORTED: return "MSGHDR_SUPPORTED";
		case SIP_HDR_UNSUPPORTED: return "MSGHDR_UNSUPPORTED";
		case SIP_HDR_TIMESTAMP: return "MSGHDR_TIMESTAMP";
		case SIP_HDR_TO: return "MSGHDR_TO";
		case SIP_HDR_USERAGENT: return "MSGHDR_USERAGENT";
		case SIP_HDR_WARNING: return "MSGHDR_WARNING";
		case SIP_HDR_WWWAUTHENTICATE: return "MSGHDR_WWWAUTHENTICATE";
		case SIP_HDR_RACK: return "MSGHDR_RACK";
		case SIP_HDR_RSEQ: return "MSGHDR_RSEQ";
		case SIP_HDR_ALSO: return "MSGHDR_ALSO";
		case SIP_HDR_REQUESTEDBY: return "MSGHDR_REQUESTEDBY";
		case SIP_HDR_REFERTO: return "MSGHDR_REFERTO";
		case SIP_HDR_SREFERTO: return "MSGHDR_SREFERTO";
		case SIP_HDR_REFERREDBY: return "MSGHDR_REFERREDBY";
		case SIP_HDR_SESSIONEXPIRES: return "MSGHDR_SESSIONEXPIRES";
		case SIP_HDR_MINSE: return "MSGHDR_MINSE";
		case SIP_HDR_REMOTEPARTYID: return "MSGHDR_REMOTEPARTYID";
		case SIP_HDR_RPIDPRIVACY: return "MSGHDR_RPIDPRIVACY";
		case SIP_HDR_DIVERSION: return "MSGHDR_DIVERSION";
		case SIP_HDR_PASSERTEDIDENTITY: return "MSGHDR_PASSERTEDIDENTITY";
		case SIP_HDR_PPREFERREDIDENTITY: return "MSGHDR_PPREFERREDIDENTITY";
		case SIP_HDR_PCALLEDPARTID: return "MSGHDR_PCALLEDPARTID";
		case SIP_HDR_SMS_INDICATOR: return "MSGHDR_SMS_INDICATOR";
		case SIP_HDR_SMS_REPLY: return "MSGHDR_SMS_REPLY";
		case SIP_HDR_VIA: return "MSGHDR_VIA";
		case SIP_HDR_BODY: return "MSGHDR_BODY";
		case SIP_HDR_OMC_CMD: return "MSGHDR_OMC_CMD";
		case SIP_HDR_OMC_TIME: return "MSGHDR_OMC_TIME";
		case SIP_HDR_OMC_TRANS: return "MSGHDR_OMC_TRANS";
		case SIP_HDR_OTHER: return "MSGHDR_OTHER";
		case SIP_HDR_MAX: return "MSGHDR_MAX";
		default: return "SIP-MSGHDR_OTHER";
	}
	return FX_NULL;
}
inline const char * eString_SdpHdr(int m)
{
	switch(m)
	{
		case SDP_V: return "SDP_V";
		case SDP_O: return "SDP_O";
		case SDP_S: return "SDP_S";
		case SDP_I: return "SDP_I";
		case SDP_U: return "SDP_U";
		case SDP_E: return "SDP_E";
		case SDP_P: return "SDP_P";
		case SDP_C: return "SDP_C";
		case SDP_B: return "SDP_B";
		case SDP_T: return "SDP_T";
		case SDP_R: return "SDP_R";
		case SDP_Z: return "SDP_Z";
		case SDP_K: return "SDP_K";
		case SDP_M: return "SDP_M";
		case SDP_A: return "SDP_A";
		case SDP_OTHER: return "SDP_OTHER";
		default: return "SDP_UNKNOWN";
	}
	return FX_NULL;
}
inline const char * eString_SdpAttHdr(int m)
{
	switch(m)
	{
		case SDP_ATTRIBUTE_CAT: return "ATTRIBUTE_CAT";
		case SDP_ATTRIBUTE_KEYWDS: return "ATTRIBUTE_KEYWDS";
		case SDP_ATTRIBUTE_TOOL: return "ATTRIBUTE_TOOL";
		case SDP_ATTRIBUTE_PTIME: return "ATTRIBUTE_PTIME";
		case SDP_ATTRIBUTE_MAXPTIME: return "ATTRIBUTE_MAXPTIME";
		case SDP_ATTRIBUTE_RTPMAP: return "ATTRIBUTE_RTPMAP";
		case SDP_ATTRIBUTE_ORIENT: return "ATTRIBUTE_ORIENT";
		case SDP_ATTRIBUTE_TYPE: return "ATTRIBUTE_TYPE";
		case SDP_ATTRIBUTE_CHARSET: return "ATTRIBUTE_CHARSET";
		case SDP_ATTRIBUTE_SDPLANG: return "ATTRIBUTE_SDPLANG";
		case SDP_ATTRIBUTE_LANG: return "ATTRIBUTE_LANG";
		case SDP_ATTRIBUTE_FRAMERATE: return "ATTRIBUTE_FRAMERATE";
		case SDP_ATTRIBUTE_QUALITY: return "ATTRIBUTE_QUALITY";
		case SDP_ATTRIBUTE_FMTP: return "ATTRIBUTE_FMTP";
		case SDP_ATTRIBUTE_FRAMESIZE: return "ATTRIBUTE_FRAMESIZE";
		case SDP_ATTRIBUTE_CANDIDATE: return "SDP_ATTRIBUTE_CANDIDATE";
		case SDP_ATTRIBUTE_OTHER: return "ATTRIBUTE_OTHER";
		default: return "ATTRIBUTE_UNKNOWN";
	}
	return FX_NULL;
}
inline const char *eString_HeaderT(int m)
{
	if(m >= SIP_HDR_NONE && m <= SIP_HDR_MAX)
		return eString_SipHdr(m);
	else if(m >= SDP_V && m <= SDP_OTHER)
		return eString_SdpHdr(m);
	else if(m >= SDP_ATTRIBUTE_CAT && m <= SDP_ATTRIBUTE_OTHER)
		return eString_SdpAttHdr(m);
	return "SIP_UNKNOWN";
}
inline const char * eString_ParseGroup(int m)
{
	switch(m)
	{
		case PARSE_GROUP_NONE: return "GROUP_NONE";
		case PARSE_GROUP_SIP: return "GROUP_SIP";
		case PARSE_GROUP_SDPCOMMON: return "GROUP_SDPCOMMON";
		case PARSE_GROUP_SDPDESC_1: return "GROUP_SDPDESC_1";
		case PARSE_GROUP_SDPDESC_2: return "GROUP_SDPDESC_2";
		case PARSE_GROUP_SDPDESC_3: return "GROUP_SDPDESC_3";
		case PARSE_GROUP_SDPDESC_4: return "GROUP_SDPDESC_4";
		case PARSE_GROUP_SDPDESC_5: return "GROUP_SDPDESC_5";
		case PARSE_GROUP_SDPDESC_6: return "GROUP_SDPDESC_6";
		case PARSE_GROUP_SDPDESC_7: return "GROUP_SDPDESC_7";
		case PARSE_GROUP_SDPDESC_8: return "GROUP_SDPDESC_8";
		case PARSE_GROUP_SDPDESC_OTHER: return "GROUP_SDPDESC_OTHER";
		case PARSE_GROUP_OTHER: return "GROUP_OTHER";
		default: return "GROUP_UNKNOWN";
	}
	return FX_NULL;
}
inline const char * eString_SdpM(int m)
{
	switch(m)
	{
		case SDP_MTYPE_NOTSET: return "SDP_MTYPE_NOTSET";
		case SDP_MTYPE_AUDIO: return "SDP_MTYPE_AUDIO";
		case SDP_MTYPE_NAS: return "SDP_MTYPE_NAS";
		case SDP_MTYPE_VIDEO: return "SDP_MTYPE_VIDEO";
		case SDP_MTYPE_APP: return "SDP_MTYPE_APP";
		case SDP_MTYPE_DATA:return "SDP_MTYPE_DATA";
		case SDP_MTYPE_IMAGE: return "SDP_MTYPE_IMAGE";
		case SDP_MTYPE_CONTROL: return "SDP_MTYPE_CONTROL";
		case SDP_MTYPE_UNKNOWN: return "SDP_MTYPE_UNKNOWN";
		default: return "GROUP_UNKNOWN";
	}
	return FX_NULL;
}
inline const char * eString_ParseContent(int m)
{
	switch(m)
	{
		case PARSE_CONTENT_EMPTY: return "CONTENT_EMPTY";
		case PARSE_CONTENT_APP_SDP: return "CONTENT_APP_SDP";
		case PARSE_CONTENT_APP_DTMF_RELAY: return "CONTENT_APP_DTMF_RELAY";
		case PARSE_CONTENT_TEXT: return "CONTENT_TEXT";
		case PARSE_CONTENT_OTHER: return "CONTENT_OTHER";
		default: return "CONTENT_UNKNOWN";
	}
	return FX_NULL;
}
inline bool INL_Get_String_Address(char *Src,char **ppIP,unsigned int *pPort) 
{
	char *strIP = NULL;
	strIP = strchr(Src,'[');
	if(strIP)    //IPv6
	{
		strIP++;
		if(strIP == NULL) return false;
		*ppIP = strIP;
		for(int i = 0;;i++)
		{
			if(strIP[i]==']')
			{
				strIP[i] = 0x00;
				if(strIP[i+1] == 0x00) return false;
				i++;
				for(;;i++)
				{
					if(strIP[i] == 0x00) return false;
					if(strIP[i] == ':')
					{
						if(strIP[i+1] == 0x00) return false;
						*pPort = (unsigned int)Fx_Atoi(&strIP[i+1]);
						return true;
					}
				}
				break;
			}
			if(strIP[i] == 0x00) return false;		
		}
	}
	else                              //IPv4
	{
		strIP = Src;
		*ppIP = strIP;
		for(int j = 0;;j++)
		{
			if(strIP[j]==':')
			{
				strIP[j] = 0x00;
				if(strIP[j+1] == 0x00) return false;
				*pPort = (unsigned int)Fx_Atoi(&strIP[j+1]);
				break;
			}
			if(strIP[j] == 0x00) return false;		
		}
	}
	return true;
}

typedef struct{int Number;const char *pchString;}st_sip_rsp_tbl;
typedef struct{e_sip_hdr k_h_num;const char *k_h_str;}st_sip_hdr_tbl;
typedef struct{e_request_dec req_num;const char *req_string;int size;}st_sip_req_tbl;
typedef struct{e_sdp_hdr sdp_num;const char *sdp_string;int size;}st_sip_sdp_tbl;
typedef struct{e_sdp_attribute_hdr att_num;const char *att_string;int size;}st_sip_att_tbl;
typedef struct
{
	int parse_hdr;int parse_type;int parse_max;bool btrim_clear;const char *pPattern;
	bool Is_Trim(char input_chr)
	{
		if(parse_type != TOK_PARSE_CHAR_DIVIDE) return false;
		for(unsigned int i=0;i<Fx_Strlen((char*)pPattern);i++)
		{
			if(input_chr == pPattern[i]) return true;
		}
		return false;
	}
	bool Is_Pattern(char input_chr, int pattern_idx)
	{
		if(parse_type != TOK_PARSE_PATTERN) return false;
		if(input_chr == pPattern[pattern_idx]) return true;
		return false;
	}
}st_line_sub_tbl;

extern const st_sip_hdr_tbl g_sip_hdr_tbl[];
extern const st_sip_rsp_tbl g_sip_rsp_tbl[];
extern const st_sip_req_tbl g_sip_req_tbl[];
extern char newtest_sip[];
extern char newtest_sdp[];
extern st_line_sub_tbl g_line_sub_tbl[];

typedef struct
{
	/*      Message Parse Token Param        */
	unsigned int act_parse_len;
	unsigned int total_token;
	unsigned int sub_token;
	unsigned int line_cnt;
	unsigned int pack_sz;
	unsigned int decode_state;
	unsigned int subdecode_state;
	bool ln_ch_s;
	bool newsdp_parse_enable;

	/*      Message Header Token Param      */
	unsigned int method;
	unsigned int sip_res;
	unsigned int cseq_num;
	unsigned int cseq_method;
	unsigned int content_type;
	unsigned int content_length;
	unsigned int body_index;
	unsigned int sdp_m_cnt;
	
	/*           Routing Token Param          */
	unsigned int via_cnt;
	unsigned int recordroute_cnt;
	unsigned int route_cnt;
	unsigned int strvia[10];
	unsigned int strrecordroute[10];
	unsigned int strroute[10];
	unsigned int bvia[10];
	unsigned int via_ip[10];
	unsigned int via_port[10];
	unsigned int recordroute_ip[10];
	unsigned int recordroute_port[10];
	unsigned int route_ip[10];
	unsigned int route_port[10];
	
	/*      Message Decoding Handler         */
	pMainDecHandler pAbnf;
	pSubDecHandler pSubAbnf;
	void Increase_Line(){(line_cnt)++;return;}
	void Increase_Char(){(sub_token)++;return;}
	void Clear_Line(){ln_ch_s = false;sub_token = 0;}
	void Clear_SubParam(){sub_token = 0;}
	bool Line_Change_Prepare(char input)
	{if(input ==0x0d){ln_ch_s = true;return true;}else return false;}
	bool Is_EndData(char input){if(input == 0x0a) return true;else return false;}
	bool Is_BeginBody(char input){	if(input == 0x0a) return true;else return false;}
	bool Is_EndHeader(char input){	if(input == ':') return true;else return false;}
	bool Is_EndHeader_sdp(char input){	if(input == '=') return true;else return false;}
	bool Is_SkipSpace(char input){if(!((sub_token == 0) && (input == ' '))) return true;else return false;}
	bool Is_Rsp(char *pMethod)
	{if(Fx_StrCmp(pMethod,(char*)g_sip_req_tbl[SIP_RSP].req_string)) return true;else return false;}
}new_token;
typedef struct
{
	char prev_len[2];
	char param_max;
	char param_cnt;
	char tbl_idx;
	char pattern_step;
	char hdr_kpos[2];
	char hdr_kpos2[2];
	char hdr_dpos[2];
	char hdr_proto[2];
	char hdr_group[2];
	char hdr_type[2];
	char hdr_type2[2];
}st_parse_ctl;
typedef struct st_sip_line_ctl
{
	st_parse_ctl ctl;
	char *ndata(){return (char*)(this+1);}
	void Init(int max_param){ctl.param_max = (char)max_param;}
	void ResetParamMax(){ctl.param_max = (char)(g_line_sub_tbl[Get_TblIdx()].parse_max);}
	st_line_sub_tbl * Get_Tbl(){return &g_line_sub_tbl[Get_TblIdx()];}
	void Set_TblIdx(int val){ctl.tbl_idx = (char)val;}
	int Get_TblIdx(){return ctl.tbl_idx;}
	bool Is_Trim(){return Get_Tbl()->btrim_clear;}
	void Incre_PatternStep(){(ctl.pattern_step)++;}
	int Get_PatternStep(){return ctl.pattern_step;}
	void Set_HdrPos(int val){Fx_Set_Short(val,ctl.hdr_kpos);}
	int Get_HdrPos(){return Fx_Get_Short(ctl.hdr_kpos);}
	void Set_HdrPos2(int val){Fx_Set_Short(val,ctl.hdr_kpos2);}
	int Get_HdrPos2(){return Fx_Get_Short(ctl.hdr_kpos2);}
	void Set_DataPos(int val){Fx_Set_Short(val,ctl.hdr_dpos);}
	int Get_DataPos(){return Fx_Get_Short(ctl.hdr_dpos);}
	void Set_HdrProto(int val){Fx_Set_Short(val,ctl.hdr_proto);}
	int Get_HdrProto(){return Fx_Get_Short(ctl.hdr_proto);}
	void Set_HdrGroup(int val){Fx_Set_Short(val,ctl.hdr_group);}
	int Get_HdrGroup(){return Fx_Get_Short(ctl.hdr_group);}
	void Set_HdrType(int val){Fx_Set_Short(val,ctl.hdr_type);}
	int Get_HdrType(){return Fx_Get_Short(ctl.hdr_type);}
	void Set_HdrType2(int val){Fx_Set_Short(val,ctl.hdr_type2);}
	int Get_HdrType2(){return Fx_Get_Short(ctl.hdr_type2);}
	void Set_ParamPos(int val){Fx_Set_Short(val,&ndata()[ctl.param_cnt*2]);(ctl.param_cnt)++;}
	int Get_ParamPos(int idx){return Fx_Get_Short(&ndata()[idx*2]);}
	st_sip_line_ctl * Add_Line_Construct(int max_param)
	{
		// Next line construct prev_len set...
		Fx_Set_Short(GetSize(),&(ndata()[GetFlexSize()]));	
		char *pNext = &(ndata()[GetFlexSize()]);
		pNext[2] = (char)max_param;
		return (st_sip_line_ctl*)pNext;
	}
	st_sip_line_ctl * Prev_Line_Construct()
	{
		if(Fx_Get_Short(ctl.prev_len) == 0) return NULL;
		return (st_sip_line_ctl *)(&(ctl.prev_len[0])-Fx_Get_Short(ctl.prev_len));
	}
	st_sip_line_ctl * Next_Line_Construct()
	{
		if(Fx_Get_Short(&ndata()[GetFlexSize()]) == 0) return NULL;
		return (st_sip_line_ctl *)&(ndata()[GetFlexSize()]);
	}
	char *String_Key(char *pk){return &pk[Get_HdrPos()];}
	char *String_Key2(char *pk){return &pk[Get_HdrPos2()];}
	char *String_Data(char *pk){return &pk[Get_DataPos()];}
	char *String_SubData(char *pk, int idx)
	{
		if(idx >=ctl.param_cnt) return NULL;
		return &pk[Get_ParamPos(idx)];
	};
	char *Debug_String(char *Resource, char *pk)
	{
		Resource[0]=0;
		Fx_StrCat(Resource,"[%s] [%s] [%s] [%s] [%s]\n",
				eString_ParseGroup(Get_HdrGroup()),eString_HeaderT(Get_HdrType()),
				eString_HeaderT(Get_HdrType2()),String_Key(pk),String_Data(pk));
		if(ctl.param_cnt ==1) return Resource;
		for(unsigned int j = 0;j<(unsigned int)(ctl.param_cnt);j++)
		{
			Fx_StrCat(Resource,"[Param %02d] %s\n",j,String_SubData(pk,j));
		}		
		return Resource;
	}
	int GetFlexSize(){return ctl.param_max*2;};
	int GetSize(){return sizeof(ctl)+ ctl.param_max*2;}
}st_sip_line_ctl;
typedef struct
{
	new_token tok;
	char data[NEW_MAX_PARES_BUFFER];
	/********************* Internal Functions *********************************/
	bool Parse_Init(char *pPacket,pMainDecHandler pHandle,bool bSdp)
	{
		memset(&tok,0x00,sizeof(new_token));memset(data,0x00, NEW_MAX_PARES_BUFFER);
		tok.pack_sz = strlen(pPacket);tok.pAbnf = pHandle;
		tok.newsdp_parse_enable = bSdp;
		tok.act_parse_len = NEW_MAX_PARSE_PACKET;
		st_sip_line_ctl *pFirst = First_Hdr();
		pFirst->Init(10);
		return true;
	}
	bool Encode_Init(char *pPacket,pMainDecHandler pHandle,bool bSdp)
	{
		memset(&tok,0x00,sizeof(new_token));memset(data,0x00, NEW_MAX_PARES_BUFFER);
		tok.pack_sz = NEW_MAX_PARSE_PACKET;tok.pAbnf = pHandle;
		tok.newsdp_parse_enable = bSdp;
		tok.act_parse_len = NEW_MAX_PARSE_PACKET;
		st_sip_line_ctl *pFirst = First_Hdr();
		pFirst->Init(10);
		return true;
	}
	bool IsOverFllow()
	{
		if(tok.total_token >= (NEW_MAX_PARSE_PACKET-1)) return true;
		return false;
	}
	st_sip_line_ctl *First_Hdr()
	{
		return (st_sip_line_ctl*)&data[NEW_MAX_PARSE_PACKET];
	}
	bool Updata_ActSize(st_sip_line_ctl *pLine)
	{
		tok.act_parse_len+=pLine->GetSize();
		if(tok.act_parse_len >= (NEW_MAX_PARES_BUFFER-1)) return false;
		return true;
	}
	st_sip_line_ctl *Current_Hdr()
	{
		return (st_sip_line_ctl *)&data[tok.act_parse_len];
	}
	st_sip_line_ctl *Prev_Hdr()
	{
		st_sip_line_ctl *pCur = Current_Hdr();
		if(pCur== NULL) return NULL;
		return pCur->Prev_Line_Construct();
	}
	void Store_Char(int input_chr){data[tok.total_token] = input_chr;}
	bool Store_String(const char * pFormat,...)
	{
		char *temp = &data[tok.total_token];
		va_list ap;va_start(ap, pFormat);vsprintf(temp, pFormat, ap);va_end(ap);
		(tok.total_token)+= Fx_Strlen(temp);
		if(tok.total_token >= NEW_MAX_PARSE_PACKET) return false;
		return true;
	}
	bool Store_Byte(void * mByte, unsigned int len)
	{
		if((tok.total_token+len) >= NEW_MAX_PARSE_PACKET) return false;
		memcpy(&data[tok.total_token],mByte,len);
		(tok.total_token)+= len; return true;
	}
	char * Current_String(){return &data[tok.total_token];}
	void * Current_Byte(){return (void*) &data[tok.total_token];}
	/********************* Basic Functions ***********************************/
	st_sip_line_ctl *Find_Type(int key)
	{
		st_sip_line_ctl *pCur = First_Hdr();
		st_sip_line_ctl *pPrev = pCur;
		st_sip_line_ctl *pNext = pCur;
		if(pCur->Get_HdrType() == key) return pCur;
		for(unsigned int i=0;i<tok.line_cnt+1;i++)
		{
			pNext = pPrev->Next_Line_Construct();
			if(pNext == NULL) return NULL;
			if(pNext->Get_HdrType() == key) return pNext;
			pPrev = pNext;
		}
		return NULL;
	}
	st_sip_line_ctl *Find_KeyGroup(int key, int groubkey)
	{
		st_sip_line_ctl *pCur = First_Hdr();
		st_sip_line_ctl *pPrev = pCur;
		st_sip_line_ctl *pNext = pCur;
		if(pCur->Get_HdrType() == key && pCur->Get_HdrGroup() == groubkey) return pCur;
		for(unsigned int i=0;i<tok.line_cnt+1;i++)
		{
			pNext = pPrev->Next_Line_Construct();
			if(pNext == NULL) return NULL;
			if(pNext->Get_HdrType() == key && pNext->Get_HdrGroup() == groubkey) return pNext;
			pPrev = pNext;
		}
		return NULL;
	}
	st_sip_line_ctl *Find_Other(char *strkey)
	{
		st_sip_line_ctl *pCur = First_Hdr();
		st_sip_line_ctl *pPrev = pCur;
		st_sip_line_ctl *pNext = pCur;
		if(Fx_StrnCmp(&data[pCur->Get_HdrPos()],strkey,Fx_Strlen(strkey))) return pCur;
		for(unsigned int i=0;i<tok.line_cnt+1;i++)
		{
			pNext = pPrev->Next_Line_Construct();
			if(pNext == NULL) return NULL;
			if(Fx_StrnCmp(&data[pNext->Get_HdrPos()],strkey,Fx_Strlen(strkey))) return pNext;
			pPrev = pNext;
		}
		return NULL;
	}
	st_sip_line_ctl *Get_Line_IDX(unsigned int idx)
	{
		if(tok.line_cnt < idx) return NULL;
		if(idx==0) return First_Hdr();
		st_sip_line_ctl *pCur = First_Hdr();
		st_sip_line_ctl *pPrev = pCur;
		st_sip_line_ctl *pNext = pCur;
		for(unsigned int i=0;i<idx;i++)
		{
			pNext = pPrev->Next_Line_Construct();
			pPrev = pNext;
		}
		return pNext;
	}
	int Get_Method(){return tok.method;}
	const char *Get_String_Method()
	{
		return eString_SipRequest(tok.method);
	}
	int Get_Rsp(){return tok.sip_res;}
	char *Get_String_Tag(int hdr)
	{
		st_sip_line_ctl *pLine = Find_Type(hdr);
		char *strtag = NULL;
		strtag = pLine->String_SubData(data,3);
		if(strtag == NULL) return NULL;
		char *tag = Fx_StrStr(strtag,(char*)"tag=");
		if(tag == NULL) return NULL;
		if(Fx_Strlen(tag) <= 4) return NULL;
		return &tag[4];
	}
	char *Get_String_CallID()
	{
		st_sip_line_ctl *pLine = Find_Type(SIP_HDR_CALLID);
		if(pLine == NULL) return NULL;
		return pLine->String_Data(data);
	}
	char *Get_String_CSeq()
	{
		char Temp[64];memset(Temp,0x00,64);
		Fx_StrCat(Temp,"%u %s",tok.cseq_num,eString_SipRequest(tok.cseq_method));
		memcpy(&data[10000],Temp,Fx_Strlen(Temp));
		data[10000+Fx_Strlen(Temp)] = 0x00;
		return &data[10000];
	}
	char *Get_String_Body()
	{
		st_sip_line_ctl *pLine = Find_Type(SIP_HDR_BODY);
		if(pLine == NULL) return NULL;
		return pLine->String_Data(data);
	}
	bool Is_URI_Line(int mHdr)
	{
		if((mHdr == SIP_HDR_TO) || (mHdr == SIP_HDR_FROM) || (mHdr == SIP_HDR_CONTACT) 
			|| (mHdr == SIP_HDR_ROUTE) || (mHdr == SIP_HDR_RECORDROUTE))
			return true;
		return false;
	}
	bool Is_Com_Hdr(int mHdr)
	{
		if((mHdr >  SIP_HDR_METHOD) && (mHdr <  SIP_HDR_VIA))
		{
			return true;
		}
		return false;
	}
	/********************* Routing Functions *********************************/
	st_sip_line_ctl *Get_Line_IDX_Via(unsigned int idx)
	{
		if(tok.via_cnt< (idx+1)) return NULL;
		return Get_Line_IDX(tok.strvia[idx]);
	}
	st_sip_line_ctl *Get_Line_IDX_Route(unsigned int idx)
	{
		if(tok.route_cnt < (idx+1)) return NULL;
		return Get_Line_IDX(tok.strroute[idx]);
	}
	bool Get_String_Route_IDX_Address(unsigned int idx,char *Resource,
												char **ppIP,unsigned int *pPort)   
	{
		// displayname <sip:uri>;etc
		//v6 = []:5060 v4 = x.x.x.x:5060
		st_sip_line_ctl *pLine = Get_Line_IDX(tok.strroute[idx]);
		if(pLine == NULL) return false;
		char *strdomain = pLine->String_SubData(data,2);
		if(strdomain == NULL) return false;
		Resource[0]=0;Fx_StrCpy(Resource,strdomain);
		return INL_Get_String_Address(Resource,ppIP,pPort);
	}
	st_sip_line_ctl *Get_Line_IDX_RecordRoute(unsigned int idx)
	{
		if(tok.recordroute_cnt < (idx+1)) return NULL;
		return Get_Line_IDX(tok.strrecordroute[idx]);
	}
	bool Get_String_RecoredRoute_IDX_Address(unsigned int idx,char *Resource,
												char **ppIP,unsigned int *pPort)   
	{
		// displayname <sip:uri>;etc
		//v6 = []:5060 v4 = x.x.x.x:5060
		st_sip_line_ctl *pLine = Get_Line_IDX(tok.strrecordroute[idx]);
		if(pLine == NULL) return false;
		char *strdomain = pLine->String_SubData(data,2);
		if(strdomain == NULL) return false;
		Resource[0]=0;Fx_StrCpy(Resource,strdomain);
		return INL_Get_String_Address(Resource,ppIP,pPort);
	}
	bool Get_String_Via_IDX_Address(unsigned int idx,char *Resource,
												char **ppIP,unsigned int *pPort)   
	{
		//v6 = []:5060 v4 = x.x.x.x:5060
		st_sip_line_ctl *pLine = Get_Line_IDX(tok.strvia[idx]);
		if(pLine == NULL) return false;
		char *strdomain = pLine->String_SubData(data,1);
		if(strdomain == NULL) return false;
		Resource[0]=0;Fx_StrCpy(Resource,strdomain);
		return INL_Get_String_Address(Resource,ppIP,pPort);
	}
	char *Get_String_Via_IDX_Branch(unsigned int idx)
	{
		st_sip_line_ctl *pLine = Get_Line_IDX(tok.strvia[idx]);
		if(pLine == NULL) return NULL;
		char *strbranch = pLine->String_SubData(data,2);
		char *branch = Fx_StrStr(strbranch,(char*)"branch=");
		if(Fx_Strlen(branch) <= 7) return NULL;
		return &branch[7];		
	}
	/********************* URI Functions ************************************/
	bool Other_Parse_URI_D(char *strhdr,char *Resource,int MaxData,char **display,
													char **strtype,char **strurl, char **etc)
	{
		// display <:> etc;
		if(strhdr == NULL) return false;
		st_sip_line_ctl *pLine = Find_Other(strhdr);char *Orig = pLine->String_Data(data);
		if(pLine == NULL) return false;if(Fx_Strlen(Orig) ==0) return false;
		st_tok_param mtok;mtok.Init(5,MaxData,Resource);
		if(mtok.Parse(TOK_PARSE_PATTERN,(char*)"<:>",Orig))
		{
			if(Orig[0]!='<')
			{
				if(display)*display = mtok.GetParam(0);
				if(strtype)*strtype = mtok.GetParam(1);
				if(strurl)*strurl = mtok.GetParam(2);
				if(etc)*etc = mtok.GetParam(3);
			}
			else
			{
				if(strtype)*strtype = mtok.GetParam(0);
				if(strurl)*strurl = mtok.GetParam(1);
				if(etc)*etc = mtok.GetParam(2);
			}
		}
		else
		{
			mtok.Init(5,MaxData,Resource);
			mtok.Parse(TOK_PARSE_CHAR,(char*)":",Orig);
			if(strtype)*strtype = mtok.GetParam(0);
			if(strurl)*strurl = mtok.GetParam(1);
		}
		return true;
	}
	bool Other_Parse_URI_A(char *strhdr,char *Resource,int MaxData,char **display,
									char **strtype,char **struser,char **strurl, char **etc)
	{
		// display <:> etc;
		if(strhdr == NULL) return false;
		st_sip_line_ctl *pLine = Find_Other(strhdr);char *Orig = pLine->String_Data(data);
		if(pLine == NULL) return false;if(Fx_Strlen(Orig) ==0) return false;
		st_tok_param mtok;mtok.Init(5,MaxData,Resource);
		if(mtok.Parse(TOK_PARSE_PATTERN,(char*)"<:@>",Orig))
		{
			if(Orig[0]!='<')
			{
				if(display)*display = mtok.GetParam(0);
				if(strtype)*strtype = mtok.GetParam(1);
				if(strtype)*struser = mtok.GetParam(2);
				if(strtype)*strurl = mtok.GetParam(3);
				if(strtype)*etc = mtok.GetParam(4);
			}
			else
			{
				if(strtype)*strtype = mtok.GetParam(0);
				if(struser)*struser = mtok.GetParam(1);
				if(strurl)*strurl = mtok.GetParam(2);
				if(etc)*etc = mtok.GetParam(3);
			}
		}
		else if(mtok.Parse(TOK_PARSE_PATTERN,(char*)"<:>",Orig))
		{
			if(Orig[0]!='<')
			{
				if(display)*display = mtok.GetParam(0);
				if(strtype)*strtype = mtok.GetParam(1);
				if(struser)*struser = mtok.GetParam(2);
				if(etc)*etc = mtok.GetParam(3);
			}
			else
			{
				if(strtype)*strtype = mtok.GetParam(0);
				if(struser)*struser = mtok.GetParam(1);
				if(etc)*etc = mtok.GetParam(2);
			}
		}
		else
		{
			mtok.Init(5,MaxData,Resource);
			mtok.Parse(TOK_PARSE_CHAR,(char*)":",Orig);
			if(strtype)*strtype = mtok.GetParam(0);
			if(struser)*struser = mtok.GetParam(1);
		}
		return true;
	}
	/******************** reqline, from, to, contact,route,record-route **************/
	char *Get_String_URI(int hdr)    
	{
		st_sip_line_ctl *pLine = Find_Type(hdr);
		if(pLine == NULL) return NULL;
		if(hdr == SIP_HDR_METHOD) return pLine->String_SubData(data,1);
		else return pLine->String_SubData(data,2);
	}
	char *Get_String_URI_UserNumber(int hdr,char *Resource)  
	{
		// To: <tel:07099999999;phone-context=lte-lguplus.co.kr>
		st_sip_line_ctl *pLine = Find_Type(hdr);
		if(pLine == NULL) return NULL;
		char *uri = NULL;
		if(pLine == NULL) return NULL;
		if(hdr == SIP_HDR_METHOD) uri =  pLine->String_SubData(data,1);
		else uri = pLine->String_SubData(data,2);
		Resource[0]=0;Fx_StrCpy(Resource,uri);
		for(unsigned int i=0;i<Fx_Strlen(Resource);i++)
		{
			if((Resource[i] == '@') || (Resource[i] == ';'))
			{
				Resource[i] = 0x00;break;
			}
		}
		if(Resource[0] == '+')  // +821012345678
		{
			char TempNum[128];memset(TempNum,0x00,128);
			Fx_Remove_Chr(TempNum,Resource,(char*)" +-()/.");
			Resource[0]=0;Fx_StrCat(Resource,"0%s",&TempNum[2]);
		}
		return Resource;
	}
	char *Get_String_URI_Domain(int hdr)  
	{
		// To: <tel:07099999999;phone-context=lte-lguplus.co.kr>
		st_sip_line_ctl *pLine = Find_Type(hdr);
		if(pLine == NULL) return NULL;
		char *uri = NULL; char *strtype = NULL;bool bTel = false;
		if(pLine == NULL) return NULL;
		if(hdr == SIP_HDR_METHOD)
		{
			strtype = pLine->String_SubData(data,0);
			
			uri =  pLine->String_SubData(data,1);
		}
		else
		{
			strtype = pLine->String_SubData(data,1);
			uri = pLine->String_SubData(data,2);
		}
		if(Fx_StrStr(strtype,(char*)"tel")) bTel = true;
		for(unsigned int i=0;i<Fx_Strlen(uri);i++)
		{
			if(uri[i] == '@')
			{
				if(uri[i+1] == 0x00) return NULL;
				return &uri[i+1];
			}
			else if(bTel && uri[i]=='=')
			{
				if(uri[i+1] == 0x00) return NULL;
				return &uri[i+1];
			}
		}
		return NULL;
	}
	bool Get_String_URI_Address(int hdr,char *Resource,char **ppIP, unsigned int *pPort)
	{
		st_sip_line_ctl *pLine = Find_Type(hdr);
		if(pLine == NULL) return false;
		char *uri = NULL;
		if(pLine == NULL) return false;
		if(hdr == SIP_HDR_METHOD) uri =  pLine->String_SubData(data,1);
		else uri = pLine->String_SubData(data,2);
		for(unsigned int i=0;i<Fx_Strlen(uri);i++)
		{
			if(uri[i] == '@')
			{
				if(uri[i+1] == 0x00) return false;
				Resource[0]=0;Fx_StrCpy(Resource,&uri[i+1]);
				return INL_Get_String_Address(Resource,ppIP,pPort);
			}
		}
		return false;
	}
	/********************* Encode Functions *********************************/
	bool Encode_Line_Ctl(st_sip_line_ctl *pLine,unsigned int mHdr,char *strkey)
	{
		bool ret = false;
		pLine->Set_HdrType(mHdr);
		pLine->Set_HdrPos(tok.total_token);
		if(mHdr == SIP_HDR_METHOD)
		{
			pLine->Set_HdrGroup(PARSE_GROUP_SIP);
			ret = Store_String("%s ",strkey);
		}
		if(mHdr > SIP_HDR_METHOD && mHdr <= SIP_HDR_OTHER)
		{
			pLine->Set_HdrGroup(PARSE_GROUP_SIP);
			ret = Store_String("%s: ",strkey);
		}
		else if(mHdr >= SDP_V && mHdr <= SDP_OTHER)
		{
			if(mHdr == SDP_M) (tok.sdp_m_cnt)++;
			if(tok.sdp_m_cnt ==0)
			pLine->Set_HdrGroup(PARSE_GROUP_SDPCOMMON);
			else pLine->Set_HdrGroup(PARSE_GROUP_SDPCOMMON+tok.sdp_m_cnt);
			ret = Store_String("%s=",strkey);
		}
		else if(mHdr >= SDP_ATTRIBUTE_CAT && mHdr <= SDP_ATTRIBUTE_OTHER)
		{
			if(tok.sdp_m_cnt ==0)
			pLine->Set_HdrGroup(PARSE_GROUP_SDPCOMMON);
			else pLine->Set_HdrGroup(PARSE_GROUP_SDPCOMMON+tok.sdp_m_cnt);
			ret = Store_String("a=%s:",strkey);
		}
		pLine->Set_DataPos(tok.total_token);
		return ret;
	}
	bool Encode_Line(unsigned int mHdr,char *strkey,const char * pFormat,...)
	{
		st_sip_line_ctl *pCur = NULL;st_sip_line_ctl *pNext = NULL;char *temp  =NULL;
		if(tok.line_cnt ==0)
		{
			pCur = First_Hdr(); 
			if(!Updata_ActSize(pCur)) return false;
			if(!Encode_Line_Ctl(pCur,mHdr,strkey)) return false;
			temp =  Current_String();
			va_list ap;va_start(ap, pFormat);vsprintf(temp, pFormat, ap);va_end(ap);
			(tok.total_token)+= Fx_Strlen(temp);
			
		}
		else
		{
			pCur = Current_Hdr();
			pNext = pCur->Add_Line_Construct(1);
			if(!Updata_ActSize(pNext)) return false;
			if(!Encode_Line_Ctl(pNext,mHdr,strkey)) return false;
			temp = Current_String();
			va_list ap;va_start(ap, pFormat);vsprintf(temp, pFormat, ap);va_end(ap);
			(tok.total_token)+= Fx_Strlen(temp);
			
		}
		if(tok.total_token >= NEW_MAX_PARSE_PACKET) return false;
		return true;
	}
	/********************* Packetize Functions *******************************/
	void Packetize_Rsp(char *rsp_buff,int rsp)
	{
		for(int i = 0;;i++)
		{
			if(g_sip_rsp_tbl[i].Number == rsp)
			{
				Fx_StrCat(rsp_buff,"SIP/2.0 %d %s\r\n",rsp, g_sip_rsp_tbl[i].pchString);
			}	
			if(g_sip_rsp_tbl[i].Number == 606)  return;
		}	
	}
	void Packetize_Com_Line(char *rsp_buff,st_sip_line_ctl *pLine)
	{
		Fx_StrCat(rsp_buff,"%s: %s\r\n",pLine->String_Key(data),pLine->String_Data(data));
	}
	void Packetize_URI_Line(char *rsp_buff,st_sip_line_ctl *pLine)
	{
		if(pLine->Get_HdrType() == SIP_HDR_METHOD)
		{
			Fx_StrCat(rsp_buff,"%s %s:%s %s\r\n",pLine->String_Key(data),
											pLine->String_SubData(data,0),
											pLine->String_SubData(data,1),
											pLine->String_SubData(data,2));
		}
		else
		{
			char *pdisplay = pLine->String_SubData(data,0);
			char *etc = pLine->String_SubData(data,3);
			Fx_StrCat(rsp_buff,"%s: %s<%s:%s>%s\r\n",pLine->String_Key(data),
										Fx_Str(pdisplay),
										pLine->String_SubData(data,1),
										pLine->String_SubData(data,2),
										Fx_Str(etc));
		}
	}
	void Packetize_Via_Line(char *rsp_buff,st_sip_line_ctl *pLine)
	{
		Fx_StrCat(rsp_buff,"Via: %s %s;%s\r\n",pLine->String_SubData(data,0),
										pLine->String_SubData(data,1),
										pLine->String_SubData(data,2));
	}
	void Packetize_CSeq_Line(char *rsp_buff,st_sip_line_ctl *pLine)
	{
		Fx_StrCat(rsp_buff,"CSeq: %s %s\r\n",pLine->String_SubData(data,0),
										pLine->String_SubData(data,1));
	}
	void Packetize_ContentLength_Line(char *rsp_buff,st_sip_line_ctl *pLine)
	{
		if(Find_Type(SIP_HDR_CONTENTTYPE))
			Fx_StrCat(rsp_buff,"Content-Length: %d\r\n",Fx_Strlen(Get_String_Body()));
		else
			Fx_StrCat(rsp_buff,"Content-Length: 0\r\n");
	}
	void Packetize_Body_Line(char *rsp_buff)
	{
		if(Get_String_Body()) Fx_StrCat(rsp_buff,"\r\n%s",Get_String_Body());
	}
	/********************* Debug Functions **********************************/
	char *Debug_String(char *Resource)
	{
		Resource[0]=0;
		st_sip_line_ctl *pCur = First_Hdr();
		st_sip_line_ctl *pPrev = pCur;
		st_sip_line_ctl *pNext = pCur;
		Fx_StrCat(Resource,"\nLine Obj : %d\n",tok.line_cnt);
		Fx_StrCat(Resource,"[--] [%s] [%s] [%s] [%s] [%s]\n",
		eString_ParseGroup(pCur->Get_HdrGroup()),eString_HeaderT(pCur->Get_HdrType()),
		eString_HeaderT(pCur->Get_HdrType2()),pCur->String_Key(data),pCur->String_Data(data));
		if(pCur->ctl.param_cnt > 1)
		{
			for(unsigned int k = 0;k<(unsigned int)(pCur->ctl.param_cnt);k++)
			{
				Fx_StrCat(Resource,"[Param %02d] %s\n",k,pCur->String_SubData(data,k));
			}
		}
		for(unsigned int i=0;i<tok.line_cnt;i++)
		{
			pNext = pPrev->Next_Line_Construct();
			if(pNext == NULL)
			{
				Fx_StrCat(Resource,"[%02d] Line NULL",i);
				break;
			}
			Fx_StrCat(Resource,"[%02d] [%s] [%s] [%s] [%s] [%s]\n",i,
				eString_ParseGroup(pCur->Get_HdrGroup()),eString_HeaderT(pNext->Get_HdrType()),
				eString_HeaderT(pNext->Get_HdrType2()),pNext->String_Key(data),pNext->String_Data(data));
			if(pNext->ctl.param_cnt > 1)
			{
				for(unsigned int j = 0;j<(unsigned int)(pNext->ctl.param_cnt);j++)
				{
					Fx_StrCat(Resource,"[Param %02d] %s\n",j,pNext->String_SubData(data,j));
				}
			}
			pPrev = pNext;
		}
		return Resource;
	}
}st_new_dec;

/**************** Using Functions ******************/
/*     Decode Util Functions    Internal Functions (Not Use) */
/*     Decoding Handler   Internal Functions (Not Use) */

/*     Decode Functions    */
extern void new_sip_test(void);
extern void new_sdp_rtp_test(void);
extern bool newsip_parse(st_new_dec *k_data,char* pPacket,bool bSdp);
extern bool newsip_parse_2(st_new_dec *k_data,char *pPacket,bool bSdp,unsigned int len);
extern bool newsdp_parse(st_new_dec *k_data,char* pPacket);
extern bool newsdp_parse_2(st_new_dec *k_data,char *pPacket,unsigned int len);
extern void newsip_req_encode(st_new_dec *pDec,char *OutPk);
extern void newsdp_addr_convert_construct(char *Dst, st_new_dec *pSrcDec,
											char *sdp_ip,unsigned int audio_port,unsigned int video_port);
extern char *newsdp_addr_change(char *Dst, char *Src,char *sdp_ip,unsigned int audio_port,
								unsigned int video_port);
extern e_sdp_hdr newsip_sdp_hdr_find(void *k_data,char input_chr);
extern e_sdp_attribute_hdr newsip_att_hdr_find(void *k_data,char * idx);
/***********************************************/

#endif

