#ifndef SAM_COM_H
#define SAM_COM_H
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>
#include <ctype.h>
#include <sys/stat.h>
#include <strings.h>
#if defined(_LINUX_)
#include <sys/ioctl.h>
#include <linux/sockios.h>
#elif defined(_SUNOS_)
#	include <sys/sockio.h>
#endif 
#include <net/if.h>
#include <semaphore.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define MAX_FLEX_OPT_NUM      65

//added by khd for ipv6
#define IS_IPV6(s)	strchr(s, ':')

#define SIPA_APPLICATION
#ifdef SIPA_APPLICATION
#define NEW_COMP_ID 4096
#define RV_RX_QUEUE    0xabcde
#define   SVC_PING_CONNECT        0xFA03
#define   SVC_PING_COLSE            0xFA04
#define   SVC_PING_COMPLETE      0xFA05
#define   SVC_PING_DISABLE         0xFA06 
//#define READ_TIME_STAMP_COUNTER      // test rdtsc timer....
#define SIPA_PRACK_SUPPORTED             //   PRACK inform AS
#endif // <<< SIPA_APPLICATION

/**
 * @attention Copyright (c) 2010 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point
 * @par Last Changed : 
 * @par 2011.01.30   chang woon kim(responsibility:SIPA)        first written
 * @par chang woon kim    modify SDP Structure...       // samkim-2011-06-08
 * @par chang woon kim    Add the TAS Command...     // samkim-2011-08-01
 * @par chang woon kim     Add String Parse Util           // samkim-2011-08-18
 * @par chang woon kim     Add Tas Info                     // samkim-2011-09-20
 * @par chang woon kim     Add Number Plan               // samkim-2012-01-17
 * @par chang woon kim     Add replication                  // samkim-2012-05-20
 * @par chang woon kim     Add Request URI Parse       // samkim-2012-09-24
 */

#define MAX_SIP_CALL_NUMBER   128
#define MAX_SIP_PACKET_SIZE    3200
#define MAX_SIP_SDP_SIZE      1500
#define MAX_TOK_PARAM     20
#define MAX_TOK_DATA_SIZE    256

#define MAX_SDP_PT_NUM      20

#define FX_NULL (0)
#define FX_IN
#define FX_OUT
#define FX_INOUT
#define FX_TRUE (1)
#define FX_FALSE (0)
typedef enum
{
	SIPA_METHOD_INVITE = 0x1000,       
	SIPA_METHOD_ACK,      
	SIPA_METHOD_BYE,         
	SIPA_METHOD_REGISTER,     
	SIPA_METHOD_REFER,        
	SIPA_METHOD_NOTIFY,        
	SIPA_METHOD_OTHER,        
	SIPA_METHOD_PRACK,
	SIPA_METHOD_CANCEL,        
	SIPA_METHOD_SUBSCRIBE,
	SIPA_METHOD_MESSAGE = 0x1100,
	SIPA_METHOD_OPTIONS,
	SIPA_METHOD_INFO,
	SIPA_METHOD_UPDATE,
	SIPA_METHOD_RSP,
	SIPA_METHOD_TERMINATION = 0x1200,
	SIPA_METHOD_INVITE_INFO,
	SIPA_METHOD_REINVITE,
	SIPA_METHOD_ERROR,
	SIPA_METHOD_REINVITE_REQ,
	SIPA_METHOD_TAS_BLOCK,
	SIPA_METHOD_TAS_UNBLOCK,
	SIPA_METHOD_TAS_RESET,    
	SIPA_METHOD_STORECALL,
	SIPA_METHOD_DELCALL,
	SIPA_METHOD_ACTIVE,
	SIPA_METHOD_STANBY,
	SIPA_PAIR_INTERNAL = 0x1300,
} SIP_Method_Type;
#define SIPA_METHOD_DTMF SIPA_METHOD_INFO
#define SIPA_PAIR_SIMOG   0x1400
#define SIPA_OG_SCEN_TIMER 0x1500
#define SIPA_IN_SIMACCEPT 0x1600
#define SIPA_IN_SIMACKTIMEOUT     0x1700
#define SIPA_OG_SIMACCEPTTIMEOUT     0x1800
#define SIPA_OG_ACCEPT_TIMEOUT 0x1601
#define SIPA_IN_ACK_TIMEOUT 0x1602
typedef enum
{
	SIPA_CALL_TRANS_MODE = 0x00,   // belong to call session
	SIPA_TRANS_ONLY_MODE = 0x01,   // belong to independant session
}e_transaction_mode;
typedef enum 
{
	CONTENT_TYPE_EMPTY= 0,
	CONTENT_TYPE_APP_SDP,
	CONTENT_TYPE_APP_DTMF_RELAY,
	CONTENT_TYPE_TEXT,
	CONTENT_TYPE_OTHER
} e_content;
typedef enum
{
	INFO_NONE,
	INFO_MESSAGE,
	INFO_DTMF,
	INFO_XML,
}e_info;
typedef enum
{
	OPT_NONE,
	OPT_SIP_CALLER,
	OPT_SIP_CALLEE,
	OPT_SIP_USER_AGENT,
	OPT_SIP_REQ_URI,
	OPT_SIP_CONTENT_TYPE,
	OPT_SIP_SESSION_EXPIRES,
	OPT_SIP_EXPIRES,
	OPT_SIP_REFRESSHER,
	OPT_SIP_SUPPORTED,
	OPT_SIP_REQUIRE,
	OPT_SIP_ALLOW,
	OPT_SIP_PRIVACY,
	OPT_SIP_REASON,
	OPT_SIP_TIMESTAMP,
	OPT_SIP_ACCEPT,
	OPT_SIP_ACCEPT_ENCODING,
	OPT_SIP_ACCEPT_LANUAGE,
	OPT_SIP_RSEQ,
	OPT_SIP_RACK,
	OPT_SIP_MINSE,
	OPT_SIP_AUTHENTICATION,
	OPT_SIP_AUTHORIZATION,
	OPT_SIP_WARNING,
	OPT_SIP_REPLY_TO,
	OPT_SIP_OTHER,
	OPT_SDP_COMMON_V = 0x40,       // v=<protocol version>
	OPT_SDP_COMMON_O,       // o=<Origin>
	OPT_SDP_COMMON_O_SESSION,
	OPT_SDP_COMMON_O_NAME,
	OPT_SDP_COMMON_S,       // o=<Session name>
	OPT_SDP_COMMON_I,        // i=<Session Information>
	OPT_SDP_COMMON_U,       // u=<URI>
	OPT_SDP_COMMON_E,       // e=<E-mail Address>
	OPT_SDP_COMMON_P,       // p=<Phone Number>
	OPT_SDP_COMMON_C,       // c=<Connection Data>
	OPT_SDP_COMMON_B,       // b=<Bandwidth>
	OPT_SDP_COMMON_T,       // t=<Timing>
	OPT_SDP_COMMON_R,       // r=<Repeat Times>
	OPT_SDP_COMMON_Z,       // t=<Tme Zones>
	OPT_SDP_COMMON_K,        // k=<Encryption Keys>
	OPT_SDP_COMMON_M,       //m=<media> <port>/<number of ports> <proto> <fmt> ...
	OPT_SDP_COMMON_A,        //a=<attri_type>:<attribute specific parameters>
	OPT_SDP_COMMON_OTHER,             //opt_string   (ex> "h=other sdp fields")
	OPT_SDP_DESCRIPTOR_OTHER = 0x80,        //m="opt_string"
	OPT_SDP_ATTRIBUTE_CAT = 0xc0,               //a=cat:<category>
	OPT_SDP_ATTRIBUTE_KEYWDS,        //a=keywds:<keywords>
	OPT_SDP_ATTRIBUTE_TOOL,             //a=tool:<name and version of tool>
	OPT_SDP_ATTRIBUTE_PTIME,            //a=ptime:<packet time>
	OPT_SDP_ATTRIBUTE_MAXPTIME,      //a=maxptime:<maximum packet time>
	OPT_SDP_ATTRIBUTE_RTPMAP,          //a=rtpmap:<payload type> <encoding name>/<clock rate> [/<encoding parameters>]
	OPT_SDP_ATTRIBUTE_ORIENT,           //a=orient:<orientation>
	OPT_SDP_ATTRIBUTE_TYPE,              //a=type:<conference type>
	OPT_SDP_ATTRIBUTE_CHARSET,        //a=charset:<character set>
	OPT_SDP_ATTRIBUTE_SDPLANG,         //a=sdplang:<language tag>
	OPT_SDP_ATTRIBUTE_LANG,               //a=lang:<language tag>
	OPT_SDP_ATTRIBUTE_FRAMERATE,      //a=framerate:<frame rate>
	OPT_SDP_ATTRIBUTE_QUALITY,          //a=quality:<quality>
	OPT_SDP_ATTRIBUTE_FMTP,               //a=fmtp:<format> <format specific parameters>
	OPT_SDP_ATTRIBUTE_FRAMESIZE,       //a=framesize:<type>
	OPT_SDP_ATTRIBUTE_CANDIDATE,         //a=candidate:<format>
	OPT_SDP_ATTRIBUTE_VISIT_REALM,
	OPT_SDP_ATTRIBUTE_VISIT_REALM_CNT,
	OPT_SDP_ATTRIBUTE_OMR_M_CK,
	OPT_SDP_ATTRIBUTE_OMR_S_CK,     
	OPT_SDP_ATTRIBUTE_OTHER,              //a="opt_string"
	OPT_OTHER,
	OPT_SDP_ERROR,
   OPT_SDP_COMMON_O_SESSION_VER = 0x90, // o field vesion
}e_opt;
typedef enum
{ 
	OPT_SDP_ERROR_SUCCESS= OPT_SDP_ERROR+1,
	OPT_SDP_ERROR_MISMATCH ,   // mismatch  content length , real length
	OPT_SDP_ERROR_O_EMPTY,
	OPT_SDP_ERROR_O_LACK,
	OPT_SDP_ERROR_O_NETTYPE,
	OPT_SDP_ERROR_O_ADDRTYPE,
	OPT_SDP_ERROR_O_IP,
	OPT_SDP_ERROR_M_LACK,
	OPT_SDP_ERROR_M_MEDIATYPE,
	OPT_SDP_ERROR_M_PORT,
	OPT_SDP_ERROR_C_LACK,
	OPT_SDP_ERROR_C_NETTYPE,
	OPT_SDP_ERROR_C_ADDRTYPE,
	OPT_SDP_ERROR_C_IP,
	OPT_SDP_ERROR_V_EMPTY,
	OPT_SDP_ERROR_S_EMPTY,
	OPT_SDP_ERROR_PARSEFAIL,                          // Parse Fail
	OPT_SDP_ERROR_PARSEOVERFLLOW,
	OPT_SDP_ERROR_OTHER,
}e_sdp_error;
typedef enum 
{
	SDPMEDIATYPE_NOTSET   = 0,
	SDPMEDIATYPE_AUDIO,               //  audio  
	SDPMEDIATYPE_NAS,                  //  nas    
	SDPMEDIATYPE_VIDEO,               //  video    
	SDPMEDIATYPE_APP,                   //  application 
	SDPMEDIATYPE_DATA,                 //  data    
	SDPMEDIATYPE_IMAGE,               //  image   
	SDPMEDIATYPE_CONTROL,           //  control 
	SDPMEDIATYPE_MESSAGE,           
	SDPMEDIATYPE_MSRP,
	SDPMEDIATYPE_MSRP_TLS,
	SDPMEDIATYPE_UNKNOWN
} e_sdp_media;
typedef enum 
{
	SDPPROTOCOL_NOTSET       = 0,
	SDPPROTOCOL_RTP,                  // "RTP/AVP"  
	SDPPROTOCOL_RTP_SAVP,         //  "RTP/SAVP"
	SDPPROTOCOL_LOCAL,              //  "LOCAL" 
	SDPPROTOCOL_ATM,                  //  "atm/avp" 
	SDPPROTOCOL_UDP,
	SDPPROTOCOL_UDP_T38,
	SDPPROTOCOL_TCP,    
	SDPPROTOCOL_AAL1ATMF,
	SDPPROTOCOL_AAL1ITU, 
	SDPPROTOCOL_AAL1CUSTOM,
	SDPPROTOCOL_AAL2ATMF,  
	SDPPROTOCOL_AAL2ITU,   
	SDPPROTOCOL_AAL2CUSTOM,
	SDPPROTOCOL_AAL5ATMF,  
	SDPPROTOCOL_AAL5ITU,   
	SDPPROTOCOL_AAL5CUSTOM,
	SDPPROTOCOL_H323C, 
	SDPPROTOCOL_MSRP,
	SDPPROTOCOL_MSRP_TLS,
	SDPPROTOCOL_UNKNOWN   
} e_sdp_proto;
typedef enum
{
	SDPCONNECTMODE_NOTSET    = 0,
	SDPCONNECTMODE_SENDONLY,       // "sendonly" 
	SDPCONNECTMODE_RECVONLY,       // "recvonly" 
	SDPCONNECTMODE_SENDRECV,       // "sendrecv" 
	SDPCONNECTMODE_INACTIVE         // "inactive" 
} e_conn_mode;
typedef enum
{
	PAYLOAD_PCMU_AUDIO = 0,     // == PT_G711U
	PAYLOAD_1016_AUDIO = 1,
	PAYLOAD_G721_AUDIO = 2,
	PAYLOAD_GSM_AUDIO = 3,
	PAYLOAD_G723_AUDIO = 4,
	PAYLOAD_DV14_AUDIO_8KHZ = 5,
	PAYLOAD_DV14_AUDIO_16KHZ = 6,
	PAYLOAD_LPC_AUDIO = 7,
	PAYLOAD_PCMA_AUDIO = 8,        // == PT_G711A
	PAYLOAD_G722_AUDIO = 9,
	PAYLOAD_L16_AUDIO_STEREO = 10,
	PAYLOAD_L16_AUDIO_MONO = 11,
	PAYLOAD_TPS0_AUDIO = 12,
	PAYLOAD_VSC_AUDIO = 13,
	PAYLOAD_MPA_AUDIO  = 14,
	PAYLOAD_G728_AUDIO = 15,
	PAYLOAD_G729_AUDIO = 18,
	PAYLOAD_RGB8_VIDEO = 23,
	PAYLOAD_HDCC_VIDEO = 24,
	PAYLOAD_CELB_VIDEO = 25,
	PAYLOAD_JPEG_VIDEO = 26,
	PAYLOAD_CUSM_VIDEO = 27,
	PAYLOAD_NV_VIDEO = 28,
	PAYLOAD_PICW_VIDEO = 29,
	PAYLOAD_CPV_VIDEO = 30,
	PAYLOAD_H261_VIDEO = 31,
	PAYLOAD_MPV_VIDEO = 32,
	PAYLOAD_MP2T_VIDEO = 33,
	PAYLOAD_H263_VIDEO = 34,
	PAYLOAD_AMR_RFC = 96,
	PAYLOAD_EVRC = 97,
	PAYLOAD_MPEG4 = 99,
	PAYLOAD_AMR_IF2 = 127,
	PAYLOAD_OTHER,
}e_payload;
typedef enum
{
	BUILD_ERR_NONE = 0,
	BUILD_ERR_INIT_FAIL,
	BUILD_ERR_LENGTH_EMPTY,
	BUILD_ERR_OVER_SIZE_TOTAL,
	BUILD_ERR_PAIR_EMPTY,
	BUILD_ERR_REQUEST_ILLEGAL,
	BUILD_ERR_OG_TRANSPORT_EMPTY,
	BUILD_ERR_OG_METHOD,
	BUILD_ERR_OG_OPT_EMPTY,
	BUILD_ERR_SESSION_HDR_EMPTY,
	BUILD_ERR_OVERLAP,
	BUILD_ERR_CONTENT_LENGTH_EMPTY,
	BUILD_ERR_OVER_SIZE_CONTENTS,
	BUILD_ERR_SDP_MEDIA_TYPE_EMPTY,
	BUILD_ERR_SDP_MODE_TYPE_EMPTY,
	BUILD_ERR_SDP_PORT_EMPTY,
	BUILD_ERR_SDP_SUBKEY_OVERLAP,
}e_build_err;
typedef enum
{
	STRING_SDP_NONE,
	STRING_SDP_COMMON,
	STRING_SDP_MEDIA,
	STRING_SDP_PAYLOAD,
}e_sdp_string;
typedef enum
{
	TOK_PARSE_NONE,
	TOK_PARSE_CHAR,
	TOK_PARSE_CHAR_DIVIDE,
	TOK_PARSE_STRING,
	TOK_PARSE_PATTERN,
}e_parse_type;

typedef enum
{
	MSRP_SETUP_NONE,
	MSRP_SETUP_ACTPASS,
	MSRP_SETUP_ACTIVE,
	MSRP_SETUP_PASSIVE,
}e_msrp_setup;

#define FX_NUMERIC(c)      ((c) >= '0' && (c) <= '9')
#define FX_MIN(a, b)       ((a) > (b) ? (b) : (a))
#define FX_MAX(a, b)       ((a) < (b) ? (b) : (a))
#define FX_ABS(x)          (((x) >= 0) ? (x) : -(x))
#define FX_TRUE_SELECT(a,b,c)    ((a) >= 1 ? (b) : (c))
#define FX_AVERAGE(a,b) ((unsigned long int)((unsigned long int)(((unsigned long int)a*100)/(unsigned long int)b)))

#define FX_GEN_INDEX(phCall, count) \
	( \
		( (((unsigned long)phCall & 0xFF000000) >> 24) ^ \
		(((unsigned long)phCall & 0x00FF0000) >> 16) ^ \
		(((unsigned long)phCall & 0x0000FF00) >> 8) ^ \
		((unsigned long)phCall & 0x000000FF) ) % count \
	)
inline long Fx_Long_Shift(FX_IN char *p, unsigned int n)
{
	if((n >= 32) && (sizeof(long) == 4)) return 0;    // detecting 32 bit cpu
	if(n==0) return ((unsigned long)((unsigned char)p[0]));
	return (((unsigned long)((unsigned char)p[0]))<<n);
}
inline void Fx_Set_Shift(FX_IN long val,FX_OUT char *p,FX_IN unsigned int n)
{
	if((n >= 32) && (sizeof(long) == 4)) return;    // detecting 32 bit cpu
	if(n==0) p[0] = (char)((val) & 0xff);
	else p[0] = (char)((val >> n) & 0xff);
}
inline long Fx_Get_Long(FX_IN char *p)
{
	if(sizeof(long) == 4)   // detecting 32 bit cpu
	{
		return Fx_Long_Shift(&p[0],24) + Fx_Long_Shift(&p[1],16)
		+ Fx_Long_Shift(&p[2],8) + Fx_Long_Shift(&p[3],0);
	}
	return Fx_Long_Shift(&p[0],56) + Fx_Long_Shift(&p[1],48)
	+ Fx_Long_Shift(&p[2],40) + Fx_Long_Shift(&p[3],32)
	+ Fx_Long_Shift(&p[4],24) + Fx_Long_Shift(&p[5],16)
	+ Fx_Long_Shift(&p[6],8) + Fx_Long_Shift(&p[7],0);
}
inline void Fx_Set_Long(FX_IN long val,FX_OUT char *p)
{
	if(sizeof(long) ==4)  // detecting 32 bit cpu
	{
		Fx_Set_Shift(val,&p[0],24);Fx_Set_Shift(val,&p[1],16);
		Fx_Set_Shift(val,&p[2],8);Fx_Set_Shift(val,&p[3],0);return;
	}
	Fx_Set_Shift(val,&p[0],56);Fx_Set_Shift(val,&p[1],48);
	Fx_Set_Shift(val,&p[2],40);Fx_Set_Shift(val,&p[3],32);
	Fx_Set_Shift(val,&p[4],24);Fx_Set_Shift(val,&p[5],16);
	Fx_Set_Shift(val,&p[6],8);Fx_Set_Shift(val,&p[7],0);
}
inline int Fx_Get_Int(FX_IN char *p)
{
	return (((unsigned int)((unsigned char)p[0]))<<24) + (((unsigned int)((unsigned char)p[1]))<<16)
		+(((unsigned int)((unsigned char)p[2]))<<8) + ((unsigned int)((unsigned char)p[3]));
}
inline void Fx_Set_Int(FX_IN int val,FX_OUT char *p)
{
	p[0] = (char)((val >> 24) & 0xff);p[1] = (char)((val >> 16) & 0xff);
	p[2] = (char)((val >> 8) & 0xff);p[3] = (char)((val) & 0xff);
	return;
}
inline int Fx_Get_Short(FX_IN char *p)
{
	return (((unsigned int)((unsigned char)p[0]))<<8) + ((unsigned int)((unsigned char)p[1]));
}
inline void Fx_Set_Short(FX_IN int val,FX_OUT char *p)
{
	p[0] = (char)((val >> 8) & 0xff);p[1] = (char)((val) & 0xff);
	return;
}
inline unsigned int Fx_Strlen(FX_IN char *src)
{
	if(src==FX_NULL) return 0;if(src[0]==0) return 0;return strlen(src);
}
inline int Fx_Atoi(FX_IN char *src)
{
	if(src == FX_NULL) return 0;if(src[0] == 0) return 0;return atoi(src);
}
inline void Fx_StrCpy(FX_OUT char *dst,FX_IN char *src)
{
	if(src == FX_NULL || dst == FX_NULL) return;
	else {while ((*dst++ = *src++) != '\0');}
}
inline void Fx_StrnCpy(FX_OUT char *dst,FX_IN char *src,FX_IN int len)
{
	if(src == FX_NULL || dst == FX_NULL) return;
	else strncpy(dst,src,len);
}
inline bool Fx_StrCmp(FX_IN char *src1,FX_IN char *src2)
{
	if(src1== FX_NULL || src2==FX_NULL) return false;
	if(src1[0]==0 || src2[0]==0) return false;
	for(int i = 0;;i++)
	{if(src1[i]==0 && src2[i]==0) return true;
	if(src1[i]==0 || src2[i]==0) return false;	
	if(src1[i] != src2[i]) return false;}
}
inline bool Fx_StrnCmp(FX_IN char *src1,FX_IN char *src2,FX_IN int len)
{
	if(src1== FX_NULL || src2==FX_NULL) return false;
	if(src1[0]==0 || src2[0]==0) return false;
	for(int i = 0;i<len;i++)
	{if(src1[i]==0 || src2[i]==0) return false;if(src1[i] != src2[i]) return false;}
	return true;
}
inline char * Fx_StrStr(FX_IN char *src,FX_IN char *key)
{
	if(src==FX_NULL || key==FX_NULL) return FX_NULL;
	if(src[0]==0 || key[0]==0) return FX_NULL;
	char *cp = (char *) src;char *s1, *s2;
	while (*cp)
	{
		s1 = cp;s2 = (char *) key;
		while (*s1 && *s2 && !(*s1 - *s2)) s1++, s2++;
		if (!*s2) return cp;
		cp++;
	}
	return FX_NULL;
}
inline int Fx_StrStr_2(FX_IN char *src,FX_IN char *key)
{
	if(src==FX_NULL || key==FX_NULL) return -1;
	if(src[0]==0 || key[0]==0) return -1;
	char *cp = (char *) src;char *s1, *s2;
	int idx = 0;
	while (*cp)
	{
		s1 = cp;s2 = (char *) key;
		while (*s1 && *s2 && !(*s1 - *s2)) s1++, s2++;
		if (!*s2) return idx;
		cp++;idx++;
	}
	return -1;
}
inline char * Fx_StrStrNext(FX_IN char *src,FX_IN char *key)
{
	if(src==FX_NULL || key==FX_NULL) return FX_NULL;
	if(src[0]==0 || key[0]==0) return FX_NULL;
	char *cp = (char *) src;char *s1, *s2;
	int idx = 0;
	while (*cp)
	{
		s1 = cp;s2 = (char *) key;
		while (*s1 && *s2 && !(*s1 - *s2)) s1++, s2++;
		if (!*s2) return &src[idx+strlen(key)];
		cp++;idx++;
	}
	return FX_NULL;
}
inline char * Fx_StrStr_Limit(FX_IN char *src,FX_IN char *key, FX_IN int limit)
{
	if(src==FX_NULL || key==FX_NULL) return FX_NULL;
	if(src[0]==0 || key[0]==0) return FX_NULL;
	char *cp = (char *) src;char *s1, *s2;int mlimit = 0;
	while (*cp)
	{
		s1 = cp;s2 = (char *) key;
		while (*s1 && *s2 && !(*s1 - *s2)) s1++, s2++;
		if (!*s2) return cp;
		cp++;mlimit++;
		if(mlimit == limit) return FX_NULL;
	}
	return FX_NULL;
}
inline void Fx_StrCat(FX_OUT char *mstr,FX_IN const char * pFormat,...)
{
	if(mstr==FX_NULL) return;
	unsigned int len = Fx_Strlen(mstr);char *temp = &mstr[len];
	va_list ap;va_start(ap, pFormat);vsprintf(temp, pFormat, ap);va_end(ap);
}
inline int Fx_StrnCat(FX_OUT char *mstr,FX_IN unsigned int max_size,FX_IN const char * pFormat,...)
{
	if(mstr==FX_NULL) return -1;unsigned int len = Fx_Strlen(mstr);
	if(len >= (max_size-2)) return -1;char *temp = &mstr[len]; len = max_size-len;
	va_list ap;va_start(ap, pFormat);int ret = vsnprintf(temp,len,pFormat, ap);va_end(ap);
	return ret;
}
inline void Fx_ByteCat(FX_OUT void *mstr,FX_INOUT unsigned int *update_len,
						FX_IN void *add_src, FX_IN unsigned int len)
{
	if(mstr == FX_NULL || update_len == FX_NULL) return;
	char *source = (char*)mstr;
	if(*update_len == 0) *update_len = Fx_Strlen(source);
	memcpy(&source[*update_len],add_src,len);
	*update_len +=len;
}
inline void Fx_StrStrCpy(FX_IN char *Dst, FX_OUT char *Src,FX_IN char *match)
{
	int cp_len = Fx_StrStr_2(Src,match);
	if(cp_len == -1) return;
	memcpy(Dst,Src,cp_len);Dst[cp_len]=0;
}
inline void Fx_StrChrCpy(FX_IN char *Dst, FX_OUT char *Src,FX_IN char match)
{
	unsigned int i=0;
	for(;Src[i]!=0x00;i++)
	{
		if(Src[i] == match) break;
		Dst[i] = Src[i];
	}
	Dst[i] = 0x00;
}
inline int Fx_Remove_Chr(FX_OUT char* dest,FX_IN char *source,FX_IN char* ptrim)
{
	int cnt = 0;unsigned int len = strlen(source);
	for(unsigned int i = 0;i< len;i++)
	{
		chr_jump:
		for(unsigned int j = 0;j<strlen(ptrim);j++)
		{
			if(*source == ptrim[j])
			{
				source++;cnt++;i++;
				goto chr_jump;
			}
		}
		*dest++ = *source++;			
	}
	return cnt;
}
inline char *Fx_Skip_Trim(FX_IN char *src, FX_IN char *ptrim,FX_OUT unsigned int *pcnt)
{
	char *Ret = src;unsigned int i=0;unsigned int j=0;unsigned int skp_cnt=0;bool btrim = false;
	while(1)
	{
		if(Ret[i]==0) 
		{
			if(pcnt)*pcnt = skp_cnt;return &src[0]; 
		}
		btrim = false;
		for(j = 0;j<strlen(ptrim);j++)
		{
			if(Ret[i] == ptrim[j])
			{
				skp_cnt++;if(pcnt)*pcnt = skp_cnt;btrim = true;break;
			}
		}
		if(!btrim) return &Ret[skp_cnt];
		i++;
	}
	return NULL;
}
inline void Fx_Skip_TrimRight(FX_IN char *src, FX_IN char *ptrim)
{
	unsigned int len = Fx_Strlen(src);unsigned int i=0;unsigned int j=0;if(len==0) return;len--;
	while(1)
	{
		for(j = 0;j<strlen(ptrim);j++)
		{
			if(src[i] == ptrim[j])
			{
				src[i]=0;break;
			}
		}
		if(i==0) break;
		i--;
	}
}
inline unsigned long Fx_HashKey(FX_IN char* strKey)
{
	if(strKey == NULL) return 0;if(strKey[0] == 0x00) return 0;
	unsigned long h_key = 5381;
	while (*strKey)
	{
		if(strKey[0] == ';') break;
		h_key = ((h_key)<<5) + (h_key) + *strKey++;
	}
	return h_key;
}
inline int Fx_Sip_Gen_IDX(char *data, unsigned int pool)
{
	if(data == NULL) return 0;
	unsigned long key = 0;unsigned long ch = 0;char *pFind = NULL;unsigned int i = 0;
	if(( pFind= Fx_StrStr(data,(char*)"Call-ID")))
	{
		key = 5381;
		for(i = 0;(pFind[i] != 0x00);i++)
		{
			if(pFind[i] == 0x0d) break;
			ch = (unsigned long)pFind[i];key = ((key<< 5) + key) + ch;
		}
	}
	return (unsigned int)FX_GEN_INDEX(key,pool);
}
inline const char *Fx_Str(char *Str)
{
	return FX_TRUE_SELECT(Fx_Strlen(Str)>0, Str, "");
}
inline const char *Fx_StrAdd(char *Str,const char *add)
{
	return FX_TRUE_SELECT(Fx_Strlen(Str)>0,add,"");
}
inline char *INL_ChrDump(FX_OUT char *Resource,FX_IN void *pRawData,FX_IN int Len)
{
	char *pData = (char*)pRawData;int j=0;int i=0;
	for(j=0,i=0;i<Len;i++)
	{
		j = i+1;
		if((j%95 == 0) && (j != 1))
		{
			if(pData[i] == 0x00)
				Fx_StrCat(Resource," \n");
			else
				Fx_StrCat(Resource,"%c\n",(unsigned char)(*(&pData[i])));
		}
		else 
		{
			if(pData[i] == 0x00)
				Fx_StrCat(Resource," ");
			else
				Fx_StrCat(Resource,"%c ",(unsigned char)(*(&pData[i])));
		}
	}
	return Resource;
}
inline char *SIP_ChrDump(FX_OUT char *Resource,FX_IN void *pRawData,FX_IN int Len)
{
	Resource[0]=0;
	Fx_StrCat(Resource,"\n===================== CHAR =====================\n");
	INL_ChrDump(Resource,pRawData,Len);
	Fx_StrCat(Resource,"\n================================================\n\n");
	return Resource;
}
inline char *INL_HexDump(FX_OUT char *Resource,FX_IN void *pRawData,FX_IN int Len)
{
	char *pData = (char*)pRawData;int j=0;int i=0;
	for(j=0,i=0;i<Len;i++)
	{
		j = i+1;
		if((j%32 == 0) && (j != 1))
		{
			Fx_StrCat(Resource,"%02x\n",(unsigned char)(*(&pData[i])));
		}
		else 
		{
			Fx_StrCat(Resource,"%02x ",(unsigned char)(*(&pData[i])));
		}
	}
	return Resource;
}
inline char *SIP_HexDump(FX_OUT char *Resource,FX_IN void *pRawData,FX_IN int Len)
{
	Resource[0]=0;
	Fx_StrCat(Resource,"\n===================== HEX  =====================\n");
	INL_HexDump(Resource,pRawData,Len);
	Fx_StrCat(Resource,"\n================================================\n\n");
	return Resource;
}
inline char *Proto_HexDump(FX_OUT char *Resource,FX_IN void *pRawData,FX_IN int Len)
{
	Resource[0]=0;char *pData = (char*)pRawData;int j=0;int i=0;
	char step[]="\r\n\r\n"; int step_cnt = 0;
	Fx_StrCat(Resource,"\n");
	for(i=0;i<Len;i++)
	{
		if(pData[i] == step[step_cnt])
		{
			step_cnt++;
		}
		else step_cnt = 0;
		Resource[i] = pData[i];
		if(step_cnt == 4) break;
	}
	i++;
	step_cnt = 0;
	for(j=0;i<Len;i++)
	{
		j = i+1;
		if((j%32 == 0) && (j != 1))
		{
			Fx_StrCat(Resource,"%02x\n",(unsigned char)(*(&pData[i])));
		}
		else 
		{
			Fx_StrCat(Resource,"%02x ",(unsigned char)(*(&pData[i])));
		}
	}
	Fx_StrCat(Resource,"\n");
	return Resource;
}
// 2015.03.23 by minimei7
inline bool Fx_IsDigit(char *str)
{
	char *ptr = str;
	while(*ptr) {
		if (!isdigit(*ptr++))
			return false;
	}
	return true;
}
inline bool Fx_IsAddr(char *str)
{
	char *ptr = str;
	while(*ptr) {
		if ( !isxdigit(*ptr) && (*ptr != '.') && (*ptr != '/') && (*ptr != ':') )
			return false;
		*ptr++;
	}
	return true;
}

typedef struct st_Circle_Pool
{
	st_Circle_Pool(){prev=NULL;next=NULL;idx=0;PoolData=NULL;reallen=0;}
	~st_Circle_Pool(){if(PoolData) delete [] PoolData;}
	struct st_Circle_Pool *prev;
	struct st_Circle_Pool *next;
	unsigned int idx;
	unsigned int poolsz;
	unsigned int reallen;
	char *PoolData;
	void Init(int pool_sz)
	{
		PoolData = new char[pool_sz];
		poolsz = pool_sz;
	}
	void PutData(void *data, int len)
	{
		memcpy(PoolData,data,len);
		reallen = (unsigned int)len;
	}
	void GetData(void *data, int *plen)
	{
		memcpy(data,PoolData,reallen); *plen=reallen;
	}
	void Refresh()
	{
		reallen=0;memset(PoolData,0x00,poolsz);
	}
}st_Circle_Pool;
inline st_Circle_Pool *INL_Create_Pool(int pool_num, int pool_sz)
{
	st_Circle_Pool *mPool = new st_Circle_Pool[pool_num];
	for(int i=0;i<pool_num;i++)
	{
		mPool[i].Init(pool_sz);
		mPool[i].idx = i;
		if(i==0)
		{
			mPool[0].prev = &mPool[pool_num-1];
			mPool[0].next = &mPool[1];
		}
		else if(i == (pool_num-1))
		{
			mPool[i].next = &mPool[0];
			mPool[i].prev = &mPool[i-1];
		}
		else
		{
			mPool[i].next = &mPool[i+1];
			mPool[i].prev = &mPool[i-1];
		}
	}
	return &mPool[0];
}
inline const char * Sipa_Method_Str(FX_IN int method)
{
	switch(method)
	{
		case SIPA_METHOD_INVITE: return "INVITE";
		case SIPA_METHOD_ACK: return "ACK";
		case SIPA_METHOD_BYE: return "BYE";
		case SIPA_METHOD_REGISTER: return "REGISTER";
		case SIPA_METHOD_REFER: return "REFER";
		case SIPA_METHOD_NOTIFY: return "NOTIFY";
		case SIPA_METHOD_OTHER: return "OTHER";
		case SIPA_METHOD_PRACK: return "PRACK";
		case SIPA_METHOD_CANCEL: return "CANCEL";
		case SIPA_METHOD_SUBSCRIBE: return "SUBSCRIBE";
		case SIPA_METHOD_MESSAGE: return "MESSAGE";
		case SIPA_METHOD_OPTIONS: return "OPTIONS";
		case SIPA_METHOD_INFO: return "INFO";
		case SIPA_METHOD_UPDATE: return "UPDATE";
		case SIPA_METHOD_RSP: return "RSP";
		case SIPA_METHOD_TERMINATION: return "TERMINATION";
		case SIPA_METHOD_INVITE_INFO: return "INFO";
		case SIPA_METHOD_REINVITE: return "REINVITE";
		case SIPA_METHOD_ERROR: return "ERROR";
		case SIPA_METHOD_REINVITE_REQ: return "REINVITE_REQ"; 
		case SIPA_METHOD_TAS_BLOCK: return "TAS_BLOCK";
		case SIPA_METHOD_TAS_UNBLOCK: return "TAS_UNBLOCK";
		case SIPA_METHOD_TAS_RESET: return "TAS_RESET";
		case SIPA_METHOD_STORECALL: return "STORECALL";
		case SIPA_METHOD_DELCALL: return "DELCALL";
		case SIPA_METHOD_ACTIVE: return "ACTIVE";
		case SIPA_METHOD_STANBY: return "STANBY";
		case SIPA_PAIR_INTERNAL: return "INTERNAL";
		case SIPA_PAIR_SIMOG: return "SIMOG";
		case SIPA_OG_SCEN_TIMER: return "TIMER";
		case SIPA_IN_SIMACCEPT: return "SIMACCEPT";
		case SIPA_IN_SIMACKTIMEOUT: return "SIMINBYE";
		case SIPA_OG_SIMACCEPTTIMEOUT: return "SIMOGBYE";
		case SIPA_IN_ACK_TIMEOUT: return "ACKTIMEOUT";
		case SIPA_OG_ACCEPT_TIMEOUT: return "ACCEPTTIMEOUT";
		default: return "UNKNOWN";
	}
	return "UNKNOWN";
}
inline const char * Trans_Mode_Str(FX_IN int mode)
{
	switch(mode)
	{
		case SIPA_CALL_TRANS_MODE: return "SIPA_CALL_TRANS_MODE";
		case SIPA_TRANS_ONLY_MODE: return "SIPA_TRANS_ONLY_MODE";
		default: return "unknown mode";
	}
	return "unknown mode";
}
inline const char * Trans_Mode_Str2(FX_IN int mode)
{
	switch(mode)
	{
		case SIPA_CALL_TRANS_MODE: return "CALL";
		case SIPA_TRANS_ONLY_MODE: return "TRAN";
		default: return "unknown mode";
	}
	return "unknown mode";
}
inline const char * Contents_Type_Str(FX_IN int ctype)
{
	switch(ctype)
	{
		case CONTENT_TYPE_EMPTY: return "CONTENT_TYPE_EMPTY";
		case CONTENT_TYPE_APP_SDP: return "CONTENT_TYPE_APP_SDP";
		case CONTENT_TYPE_APP_DTMF_RELAY: return "CONTENT_TYPE_APP_DTMF_RELAY";
		case CONTENT_TYPE_TEXT: return "CONTENT_TYPE_TEXT";
		case CONTENT_TYPE_OTHER: return "CONTENT_TYPE_OTHER";
		default: return "unknown Contents";
	}
	return "unknown Contents";
}
inline const char * Sdp_Proto_Str(FX_IN int proto)
{
	switch(proto)
	{
		case SDPPROTOCOL_NOTSET: return "SDPPROTOCOL_NOTSET";
		case SDPPROTOCOL_RTP: return "SDPPROTOCOL_RTP";
		case SDPPROTOCOL_RTP_SAVP: return "SDPPROTOCOL_RTP_SAVP";
		case SDPPROTOCOL_LOCAL: return "SDPPROTOCOL_LOCAL";
		case SDPPROTOCOL_ATM: return "SDPPROTOCOL_ATM";
		case SDPPROTOCOL_UDP: return "SDPPROTOCOL_UDP";
		case SDPPROTOCOL_UDP_T38: return "SDPPROTOCOL_UDP_T38";
		case SDPPROTOCOL_TCP: return "SDPPROTOCOL_TCP";
		case SDPPROTOCOL_AAL1ATMF: return "SDPPROTOCOL_AAL1ATMF";
		case SDPPROTOCOL_AAL1ITU: return "SDPPROTOCOL_AAL1ITU";
		case SDPPROTOCOL_AAL1CUSTOM: return "SDPPROTOCOL_AAL1CUSTOM";
		case SDPPROTOCOL_AAL5ATMF: return "SDPPROTOCOL_AAL5ATMF";
		case SDPPROTOCOL_AAL5ITU: return "SDPPROTOCOL_AAL5ITU";
		case SDPPROTOCOL_AAL5CUSTOM: return "SDPPROTOCOL_AAL5CUSTOM";
		case SDPPROTOCOL_H323C: return "SDPPROTOCOL_H323C";
		case SDPPROTOCOL_MSRP: return "SDPPROTOCOL_MSRP";
		case SDPPROTOCOL_MSRP_TLS: return "SDPPROTOCOL_MSRP_TLS";
		default: return "SDPPROTOCOL_UNKNOWN";
	}
	return "SDPPROTOCOL_UNKNOWN";
}
inline const char *opt_keyString(FX_IN int mkey)
{
	switch(mkey)
	{
		case OPT_NONE: return "OPT_NONE";
		case OPT_SIP_CALLER: return "OPT_SIP_CALLER";
		case OPT_SIP_CALLEE: return "OPT_SIP_CALLEE";
		case OPT_SIP_USER_AGENT: return "OPT_SIP_USER_AGENT";
		case OPT_SIP_REQ_URI: return "OPT_SIP_REQ_URI";
		case OPT_SIP_CONTENT_TYPE: return "OPT_SIP_CONTENT_TYPE";
		case OPT_SIP_SESSION_EXPIRES: return "OPT_SIP_SESSION_EXPIRES";
		case OPT_SIP_EXPIRES: return "OPT_SIP_EXPIRES";
		case OPT_SIP_REFRESSHER: return "OPT_SIP_REFRESSHER";
		case OPT_SIP_SUPPORTED: return "OPT_SIP_SUPPORTED";
		case OPT_SIP_REQUIRE: return "OPT_SIP_REQUIRE";
		case OPT_SIP_ALLOW: return "OPT_SIP_ALLOW";
		case OPT_SIP_PRIVACY: return "OPT_SIP_PRIVACY";
		case OPT_SIP_REASON: return "OPT_SIP_REASON";
		case OPT_SIP_TIMESTAMP: return "OPT_SIP_TIMESTAMP";
		case OPT_SIP_ACCEPT: return "OPT_SIP_ACCEPT";
		case OPT_SIP_ACCEPT_ENCODING: return "OPT_SIP_ACCEPT_ENCODING";
		case OPT_SIP_ACCEPT_LANUAGE: return "OPT_SIP_ACCEPT_LANUAGE";			
		case OPT_SIP_RSEQ: return "OPT_SIP_RSEQ";
		case OPT_SIP_RACK: return "OPT_SIP_RACK";
		case OPT_SIP_MINSE: return "OPT_SIP_MINSE";
		case OPT_SIP_AUTHENTICATION: return "OPT_SIP_AUTHENTICATION";
		case OPT_SIP_AUTHORIZATION: return "OPT_SIP_AUTHORIZATION";
		case OPT_SIP_WARNING: return "OPT_SIP_WARNING";
		case OPT_SIP_REPLY_TO: return "OPT_SIP_REPLY_TO";
		case OPT_SDP_COMMON_V: return "OPT_SDP_COMMON_V";
		case OPT_SDP_COMMON_O: return "OPT_SDP_COMMON_O";
		case OPT_SDP_COMMON_O_SESSION: return "OPT_SDP_COMMON_O_SESSION";
		case OPT_SDP_COMMON_O_NAME: return "OPT_SDP_COMMON_O_NAME";
		case OPT_SDP_COMMON_S: return "OPT_SDP_COMMON_S";
		case OPT_SDP_COMMON_I: return "OPT_SDP_COMMON_I";
		case OPT_SDP_COMMON_U: return "OPT_SDP_COMMON_U";
		case OPT_SDP_COMMON_E: return "OPT_SDP_COMMON_E";
		case OPT_SDP_COMMON_P: return "OPT_SDP_COMMON_P";
		case OPT_SDP_COMMON_C: return "OPT_SDP_COMMON_C";
		case OPT_SDP_COMMON_B: return "OPT_SDP_COMMON_B";
		case OPT_SDP_COMMON_T: return "OPT_SDP_COMMON_T";
		case OPT_SDP_COMMON_R: return "OPT_SDP_COMMON_R";
		case OPT_SDP_COMMON_Z: return "OPT_SDP_COMMON_Z";
		case OPT_SDP_COMMON_K: return "OPT_SDP_COMMON_K";
		case OPT_SDP_COMMON_M: return "OPT_SDP_COMMON_M";
		case OPT_SDP_COMMON_A: return "OPT_SDP_COMMON_A";
		case OPT_SDP_COMMON_OTHER: return "OPT_SDP_COMMON_OTHER";
		case OPT_SDP_DESCRIPTOR_OTHER: return "OPT_SDP_DESCRIPTOR_OTHER";
		case OPT_SDP_ATTRIBUTE_CAT: return "OPT_SDP_ATTRIBUTE_CAT";
		case OPT_SDP_ATTRIBUTE_KEYWDS: return "OPT_SDP_ATTRIBUTE_KEYWDS";
		case OPT_SDP_ATTRIBUTE_TOOL: return "OPT_SDP_ATTRIBUTE_TOOL";
		case OPT_SDP_ATTRIBUTE_PTIME: return "OPT_SDP_ATTRIBUTE_PTIME";
		case OPT_SDP_ATTRIBUTE_MAXPTIME: return "OPT_SDP_ATTRIBUTE_MAXPTIME";
		case OPT_SDP_ATTRIBUTE_RTPMAP: return "OPT_SDP_ATTRIBUTE_RTPMAP";
		case OPT_SDP_ATTRIBUTE_ORIENT: return "OPT_SDP_ATTRIBUTE_ORIENT";
		case OPT_SDP_ATTRIBUTE_TYPE: return "OPT_SDP_ATTRIBUTE_TYPE";
		case OPT_SDP_ATTRIBUTE_CHARSET: return "OPT_SDP_ATTRIBUTE_CHARSET";
		case OPT_SDP_ATTRIBUTE_SDPLANG: return "OPT_SDP_ATTRIBUTE_SDPLANG";
		case OPT_SDP_ATTRIBUTE_LANG: return "OPT_SDP_ATTRIBUTE_LANG";
		case OPT_SDP_ATTRIBUTE_FRAMERATE: return "OPT_SDP_ATTRIBUTE_FRAMERATE";
		case OPT_SDP_ATTRIBUTE_QUALITY: return "OPT_SDP_ATTRIBUTE_QUALITY";
		case OPT_SDP_ATTRIBUTE_FMTP: return "OPT_SDP_ATTRIBUTE_FMTP";
		case OPT_SDP_ATTRIBUTE_FRAMESIZE: return "OPT_SDP_ATTRIBUTE_FRAMESIZE";
		case OPT_SDP_ATTRIBUTE_CANDIDATE: return "OPT_SDP_ATTRIBUTE_CANDIDATE";
		case OPT_SDP_ATTRIBUTE_VISIT_REALM: return "OPT_SDP_ATTRIBUTE_VISIT_REALM";
		case OPT_SDP_ATTRIBUTE_VISIT_REALM_CNT: return "OPT_SDP_ATTRIBUTE_VISIT_REALM_CNT";
		case OPT_SDP_ATTRIBUTE_OMR_M_CK: return "OPT_SDP_ATTRIBUTE_OMR_M_CK";
		case OPT_SDP_ATTRIBUTE_OMR_S_CK: return "OPT_SDP_ATTRIBUTE_OMR_S_CK";
		case OPT_SDP_ATTRIBUTE_OTHER: return "OPT_SDP_ATTRIBUTE_OTHER";
		case OPT_SDP_ERROR: return "OPT_SDP_ERROR";
      case OPT_SDP_COMMON_O_SESSION_VER: return "OPT_SDP_COMMON_O_SESSION_VER";
		case OPT_SDP_ERROR_SUCCESS: return "OPT_SDP_ERROR_SUCCESS";
		case OPT_SDP_ERROR_MISMATCH: return "CONTENT LENGTH ERROR";
		case OPT_SDP_ERROR_O_EMPTY: return "[SDP ERROR] O FIELD IS EMPTY";
		case OPT_SDP_ERROR_O_LACK: return "[SDP ERROR] O FIELD PARAMTER COUNT ERROR";
		case OPT_SDP_ERROR_O_NETTYPE: return "[SDP ERROR] O FIELD NETWORK ERROR";
		case OPT_SDP_ERROR_O_ADDRTYPE: return "[SDP ERROR] O FIELD ADDRESS TYPE ERROR";
		case OPT_SDP_ERROR_O_IP: return "[SDP ERROR] O FIELD INVALID ADDRESS";
		case OPT_SDP_ERROR_M_LACK: return "[SDP ERROR] M FIELD PARAMTER COUNT ERROR";
		case OPT_SDP_ERROR_M_MEDIATYPE: return "[SDP ERROR] M FIELD MIDIEA TYPE ERROR";
		case OPT_SDP_ERROR_M_PORT: return "[SDP ERROR] M FIELD MIDIEA PORT ERROR";
		case OPT_SDP_ERROR_C_LACK: return "[SDP ERROR] C FIELD PARAMTER COUNT ERROR";
		case OPT_SDP_ERROR_C_NETTYPE: return "[SDP ERROR] C FIELD NETWORK ERROR";
		case OPT_SDP_ERROR_C_ADDRTYPE: return "[SDP ERROR] C FIELD ADDRESS TYPE ERROR";
		case OPT_SDP_ERROR_C_IP: return "[SDP ERROR] C FIELD INVALID ADDRESS";
		case OPT_SDP_ERROR_V_EMPTY: return "[SDP ERROR] V FIELD IS EMPTY";
		case OPT_SDP_ERROR_S_EMPTY: return "[SDP ERROR] S FIELD IS EMPTY";
		case OPT_SDP_ERROR_PARSEOVERFLLOW: return "[SDP ERROR] PARSE OVERFLLOW";
		case OPT_SDP_ERROR_PARSEFAIL: return "[SDP ERROR] PARSE FAIL";
		case OPT_SDP_ERROR_OTHER: return "[SDP ERROR] OTHER";
		default: return "OTHER";
	}
	return "OTHER";
}
inline const char *sdp_media_str(FX_IN int m)
{
	switch(m)
	{
		case SDPMEDIATYPE_NOTSET: return "SDPMEDIATYPE_NOTSET";
		case SDPMEDIATYPE_AUDIO: return "SDPMEDIATYPE_AUDIO";
		case SDPMEDIATYPE_NAS: return "SDPMEDIATYPE_NAS";
		case SDPMEDIATYPE_VIDEO: return "SDPMEDIATYPE_VIDEO";
		case SDPMEDIATYPE_APP: return "SDPMEDIATYPE_APP";
		case SDPMEDIATYPE_DATA: return "SDPMEDIATYPE_DATA";
		case SDPMEDIATYPE_IMAGE: return "SDPMEDIATYPE_IMAGE";
		case SDPMEDIATYPE_CONTROL: return "SDPMEDIATYPE_CONTROL";
		case SDPMEDIATYPE_MESSAGE: return "SDPMEDIATYPE_MESSAGE";
		case SDPMEDIATYPE_MSRP: return "SDPMEDIATYPE_MSRP";
		case SDPMEDIATYPE_MSRP_TLS: return "SDPMEDIATYPE_MSRP_TLS";
		case SDPMEDIATYPE_UNKNOWN: return "SDPMEDIATYPE_UNKNOWN";
		default: return "SDPMEDIATYPE_UNKNOWN";
	}
	return "SDPMEDIATYPE_UNKNOWN";
}
inline const char *con_mod_str(FX_IN int mode)
{
	switch(mode)
	{
		case SDPCONNECTMODE_NOTSET: return "SDPCONNECTMODE_NOTSET";
		case SDPCONNECTMODE_SENDONLY: return "SDPCONNECTMODE_SENDONLY";
		case SDPCONNECTMODE_RECVONLY: return "SDPCONNECTMODE_RECVONLY";
		case SDPCONNECTMODE_SENDRECV: return "SDPCONNECTMODE_SENDRECV";
		case SDPCONNECTMODE_INACTIVE: return "SDPCONNECTMODE_INACTIVE";
		default : return "SDPCONNECTMODE_UNKNOWN";
	}
	return "SDPCONNECTMODE_UNKNOWN";
}
inline const char * payload_str(FX_IN int mpt)
{
	switch(mpt)
	{
		case PAYLOAD_PCMU_AUDIO: return "PAYLOAD_PCMU_AUDIO";
		case PAYLOAD_1016_AUDIO: return "PAYLOAD_1016_AUDIO";
		case PAYLOAD_G721_AUDIO: return "PAYLOAD_G721_AUDIO";
		case PAYLOAD_GSM_AUDIO: return "PAYLOAD_GSM_AUDIO";
		case PAYLOAD_G723_AUDIO: return "PAYLOAD_G723_AUDIO";
		case PAYLOAD_DV14_AUDIO_8KHZ: return "PAYLOAD_DV14_AUDIO_8KHZ";
		case PAYLOAD_DV14_AUDIO_16KHZ: return "PAYLOAD_DV14_AUDIO_16KHZ";
		case PAYLOAD_LPC_AUDIO: return "PAYLOAD_LPC_AUDIO";
		case PAYLOAD_PCMA_AUDIO: return "PAYLOAD_PCMA_AUDIO";
		case PAYLOAD_G722_AUDIO: return "PAYLOAD_G722_AUDIO";
		case PAYLOAD_L16_AUDIO_STEREO: return "PAYLOAD_L16_AUDIO_STEREO";
		case PAYLOAD_L16_AUDIO_MONO: return "PAYLOAD_L16_AUDIO_MONO";
		case PAYLOAD_TPS0_AUDIO: return "PAYLOAD_TPS0_AUDIO";
		case PAYLOAD_VSC_AUDIO: return "PAYLOAD_VSC_AUDIO";
		case PAYLOAD_MPA_AUDIO: return "PAYLOAD_MPA_AUDIO";
		case PAYLOAD_G728_AUDIO: return "PAYLOAD_G728_AUDIO";
		case PAYLOAD_G729_AUDIO: return "PAYLOAD_G729_AUDIO";
		case PAYLOAD_RGB8_VIDEO: return "PAYLOAD_RGB8_VIDEO";
		case PAYLOAD_HDCC_VIDEO: return "PAYLOAD_HDCC_VIDEO";
		case PAYLOAD_CELB_VIDEO: return "PAYLOAD_CELB_VIDEO";
		case PAYLOAD_JPEG_VIDEO: return "PAYLOAD_JPEG_VIDEO";
		case PAYLOAD_CUSM_VIDEO: return "PAYLOAD_CUSM_VIDEO";
		case PAYLOAD_NV_VIDEO: return "PAYLOAD_NV_VIDEO";
		case PAYLOAD_PICW_VIDEO: return "PAYLOAD_PICW_VIDEO";
		case PAYLOAD_CPV_VIDEO: return "PAYLOAD_CPV_VIDEO";
		case PAYLOAD_H261_VIDEO: return "PAYLOAD_H261_VIDEO";
		case PAYLOAD_MPV_VIDEO: return "PAYLOAD_MPV_VIDEO";
		case PAYLOAD_MP2T_VIDEO: return "PAYLOAD_MP2T_VIDEO";
		case PAYLOAD_H263_VIDEO: return "PAYLOAD_H263_VIDEO";
		case PAYLOAD_AMR_RFC: return "PAYLOAD_AMR_RFC";
		case PAYLOAD_EVRC: return "PAYLOAD_EVRC";
		case PAYLOAD_MPEG4: return "PAYLOAD_MPEG4";
		case PAYLOAD_AMR_IF2: return "PAYLOAD_AMR_IF2";
		case PAYLOAD_OTHER: return "PAYLOAD_OTHER";
		default: return "PAYLOAD_OTHER";
	}
	return "PAYLOAD_OTHER";
}
inline const char * Build_Err_Str(FX_IN int mtype)
{
	switch(mtype)
	{
		case BUILD_ERR_NONE:	 return "BUILD_ERR_NONE";
		case BUILD_ERR_INIT_FAIL:	return "BUILD_ERR_INIT_FAIL";
		case BUILD_ERR_LENGTH_EMPTY: return "BUILD_ERR_LENGTH_EMPTY";
		case BUILD_ERR_OVER_SIZE_TOTAL: return "BUILD_ERR_OVER_SIZE_TOTAL";
		case BUILD_ERR_PAIR_EMPTY: return "BUILD_ERR_PAIR_EMPTY";
		case BUILD_ERR_REQUEST_ILLEGAL: return "BUILD_ERR_REQUEST_ILLEGAL";	
		case BUILD_ERR_OG_TRANSPORT_EMPTY: return "BUILD_ERR_OG_TRANSPORT_EMPTY";	
		case BUILD_ERR_OG_METHOD: return "BUILD_ERR_OG_METHOD";	
		case BUILD_ERR_OG_OPT_EMPTY: return "BUILD_ERR_OG_OPT_EMPTY";	
		case BUILD_ERR_SESSION_HDR_EMPTY: return "BUILD_ERR_SESSION_HDR_EMPTY";	
		case BUILD_ERR_OVERLAP: return "BUILD_ERR_OVERLAP";
		case BUILD_ERR_CONTENT_LENGTH_EMPTY: return "BUILD_ERR_CONTENT_LENGTH_EMPTY";
		case BUILD_ERR_OVER_SIZE_CONTENTS: return "BUILD_ERR_OVER_SIZE_CONTENTS";
		case BUILD_ERR_SDP_MEDIA_TYPE_EMPTY: return "BUILD_ERR_SDP_MEDIA_TYPE_EMPTY";
		case BUILD_ERR_SDP_MODE_TYPE_EMPTY: return "BUILD_ERR_SDP_MODE_TYPE_EMPTY";
		case BUILD_ERR_SDP_PORT_EMPTY: return "BUILD_ERR_SDP_PORT_EMPTY";
		case BUILD_ERR_SDP_SUBKEY_OVERLAP: return "BUILD_ERR_SDP_SUBKEY_OVERLAP";
		default:return "BUILD_ERR_NONE";
	}
	return "BUILD_ERR_NONE";
}
inline const char * Sdp_String_type(FX_IN int mtype)
{
	switch(mtype)
	{
		case STRING_SDP_NONE: return "STRING_SDP_NONE";
		case STRING_SDP_COMMON: return "STRING_SDP_COMMON";
		case STRING_SDP_MEDIA: return "STRING_SDP_MEDIA";
		case STRING_SDP_PAYLOAD: return "STRING_SDP_PAYLOAD";
		default:return "STRING_SDP_NONE";
	}
	return "STRING_SDP_NONE";
}
inline const char *String_ContentType(unsigned int m)
{
	switch(m)
	{
		case CONTENT_TYPE_EMPTY: return "text/plain";
		case CONTENT_TYPE_APP_SDP: return "application/sdp";
		case CONTENT_TYPE_APP_DTMF_RELAY: return "application/dtmf-relay";
		case CONTENT_TYPE_TEXT: return "text/plain";
		case CONTENT_TYPE_OTHER: return "text/plain";
		default: return "text/plain";
	};
	return "text/plain";
}
inline const char *String_Msrp_Setup(FX_IN int mMode)
{
	switch(mMode)
	{
		case MSRP_SETUP_NONE: return "MSRP_SETUP_NONE";
		case MSRP_SETUP_ACTPASS: return "MSRP_SETUP_ACTPASS";
		case MSRP_SETUP_ACTIVE: return "MSRP_SETUP_ACTIVE";
		case MSRP_SETUP_PASSIVE: return "MSRP_SETUP_PASSIVE";
		default: return "MSRP_SETUP_NONE";
	}
	return "MSRP_SETUP_NONE";
}

/* IP Address Conversion
IP ???? ??????
N2A == inet_ntoa ?????? ???? ?????? -> ??????
A2N  == inet_addr ?????? ????  ??????  -> ??????
*/
typedef struct
{
	char data[64];
	char * N2A(unsigned int aa)   // inet_ntoa   
	{
		unsigned int a,b,c,d;
		a = (aa&0x000000ff);b = (((unsigned int)(aa&0x0000ff00))>>8);
		c= (((unsigned int)(aa&0x00ff0000))>>16);d = (((unsigned int)(aa&0xff000000))>> 24);
		data[0]=0;Fx_StrCat(data,"%d.%d.%d.%d",a,b,c,d);
		return data;
	}
	unsigned int A2N(char *strip)    //inet_addr
	{
		if(strip==FX_NULL) return 0;
		if(strip[0]==0x00) return 0;
		char *a;char *b;char *c;char*d;
		unsigned int i;	unsigned int ret = 0;unsigned int pot_cnt = 0;
		memset(data,0x00,64);memcpy(data,strip,Fx_Strlen(strip));
		a= &data[0];
		for(i = 0;i<Fx_Strlen(strip);i++)if(strip[i] == '.') pot_cnt++; 
		if(pot_cnt <  3) return 0;
		for(i = 0;i<Fx_Strlen(strip);i++){if('.' == data[i]){data[i] = 0x00;i++;b= &data[i];break;}}
		for(;i<Fx_Strlen(strip);i++){if('.' == data[i]){data[i] = 0x00;i++;c= &strip[i];break;}}
		for(;i<Fx_Strlen(strip);i++){if('.' == data[i]){data[i] = 0x00;i++;d= &data[i];break;}}
		ret = ((unsigned int)Fx_Atoi(a)) | (((unsigned int)Fx_Atoi(b))<<8)
		| (((unsigned int)Fx_Atoi(c))<<16) | (((unsigned int)Fx_Atoi(d))<<24);
		return ret;
	}
}st_addressing;
typedef struct
{
	e_parse_type mType;
	unsigned int max_param;
	unsigned int max_buff;
	unsigned int Orig_Len;
	unsigned int Trim_Len;
	unsigned int state;      // 0 = empty, 1 = full
	unsigned int pos_cnt;
	char *data;                // store buff
	char *fdata()
	{
		if(mType != TOK_PARSE_CHAR_DIVIDE)return (char*)&(data[max_param*2]);
		else return (char*)&(data[max_param*4]);
	}
	int GetPos(int idx){return Fx_Get_Short(&(data[idx*2]));}
	void SetPos(int idx,int val){Fx_Set_Short(val,&(data[idx*2]));}
	int GetPosLen(int idx){return Fx_Get_Short(&(data[max_param*2+idx*2]));}
	void SetPosLen(int idx,int val){Fx_Set_Short(val,&(data[max_param*2+idx*2]));}
	void Init(int MaxParam,int MaxData,char *pstore)
	{
		max_param = MaxParam;max_buff = MaxData;Orig_Len = 0;state = 0;pos_cnt = 0;
		mType = TOK_PARSE_NONE;Trim_Len=0;data = pstore;memset(data,0x00,MaxData);
	}
	int Flex_Hdr_Size()
	{
		if(mType != TOK_PARSE_CHAR_DIVIDE) return max_param*2;
		else return max_param*4;
	}
	int Flex_Size(){return Flex_Hdr_Size() + Orig_Len;}
	int Src_Copy(e_parse_type mParse,char *pOrig,int len)
	{
		if(max_buff <= (unsigned int)len) return -1;
		memset(data,0x00,max_buff);Orig_Len = len;mType = mParse;memcpy(fdata(),pOrig,Orig_Len);
		return Flex_Size();
	}
	int Src_Cat(e_parse_type mParse,const char * pFormat,...)
	{
		memset(data,0x00,max_buff);mType = mParse;
		va_list ap;va_start(ap, pFormat);vsprintf(fdata(), pFormat, ap);va_end(ap);
		int len = Fx_Strlen(fdata());if(max_buff <= (unsigned int)len) return -1;
		Orig_Len = len;
		return Flex_Size();
	}
	int Src_CatAdd(const char * pFormat,...)
	{
		char *temp = &fdata()[Fx_Strlen(fdata())];
		va_list ap;va_start(ap, pFormat);vsprintf(temp, pFormat, ap);va_end(ap);
		int len = Fx_Strlen(fdata());if(max_buff <= (unsigned int)len) return -1;
		Orig_Len = len;
		return Flex_Size();
	}
	bool Parse(e_parse_type mParse,char *pTrim, char *pOrig)
	{
		mType = mParse;state = 0;pos_cnt = 0;	
		if(pTrim == FX_NULL){return false;}
		else if(pTrim[0]==0){return false;}
		else if(pOrig == FX_NULL && Fx_Strlen(fdata())==0){return false;}
		else if(pOrig == FX_NULL &&  Fx_Strlen(fdata()) > 0)
		{
			// Already Src_Copy
		}
		else
		{
			if(pOrig[0] == 0) return false;
			if(max_buff <= Orig_Len) return false;
			Orig_Len = 0;	memset(data,0x00,max_buff);
			Orig_Len = Fx_Strlen(pOrig);memcpy(fdata(),pOrig,Orig_Len);
		}
		Trim_Len = Fx_Strlen(pTrim);		
		if(mParse==TOK_PARSE_CHAR) return Parse_Char(pTrim);
		else if(mParse==TOK_PARSE_CHAR_DIVIDE) return Parse_Char_Divide(pTrim);
		else if(mParse==TOK_PARSE_STRING) return Parse_String(pTrim);
		else if(mParse==TOK_PARSE_PATTERN) return Parse_Pattern(pTrim);
		return true;
	}
	bool Parse_Char(char *pTrim)
	{
		for(unsigned int i = 0;i<Orig_Len;i++)
		{
			if(Compare_Tok_Char(pTrim,fdata()[i])){fdata()[i]= 0;state = 0;}  
			else	{if(state == 0){if(pos_cnt == max_param) return false;SetPos(pos_cnt,i);state = 1;pos_cnt++;}}
		}
		return true;
	}
	bool Parse_Char_Divide(char *pTrim)
	{
		unsigned int i = 0;
		for(i = 0;i<Orig_Len;i++)
		{
			if(Compare_Tok_Char(pTrim,fdata()[i]))
			{
				if(state == 1) SetPosLen(pos_cnt-1,i-GetPos(pos_cnt-1));
				state = 0;
			}  
			else	
			{
				if(state == 0)
				{
					if(pos_cnt == max_param)
					{
						if(GetPosLen(pos_cnt-1) == 0){SetPosLen(pos_cnt-1,i-GetPos(pos_cnt-1));}
						return false;
					}
					SetPos(pos_cnt,i);state = 1;pos_cnt++;
				}
			}
		}
		if(GetPosLen(pos_cnt-1) == 0){SetPosLen(pos_cnt-1,i-GetPos(pos_cnt-1));}
		return true;
	}
	bool Parse_String(char *pTrim)
	{
		return Compare_Tok_String(fdata(),pTrim);
	}
	bool Parse_Pattern(char *pTrim)
	{
		unsigned int pattern_step = 0;
		unsigned int i = 0;
		for(;i<Orig_Len;i++)
		{
			if(state ==0)
			{
				if(fdata()[i] == pTrim[pattern_step])
				{
					state =1;	pattern_step++;fdata()[i] =0x00;
				}
				else if(pos_cnt == 0)
				{
					for(;i<Orig_Len;i++)
					{
						if(fdata()[i] != ' ') break;
						else fdata()[i]=0x00;
					}
					if(i == Orig_Len) return false;
					if(fdata()[i] == pTrim[pattern_step])
					{
						state =1;	pattern_step++;fdata()[i] =0x00;continue;
					}
					else
					{
						if(pos_cnt == max_param) return false;SetPos(pos_cnt,i);state = 0;pos_cnt++;
					}
				}
			}
			else if(state == 1)
			{
				if(fdata()[i] != pTrim[pattern_step])
				{
					if(fdata()[i] == pTrim[pattern_step-1]) fdata()[i] = 0x00;
					else
					{
						for(;i<Orig_Len;i++)
						{
							if(fdata()[i] != ' ') break;
							else fdata()[i]=0x00;
						}
						if(i == Orig_Len) return false;
						if(fdata()[i] == pTrim[pattern_step])
						{
							state =1;	pattern_step++;fdata()[i] =0x00;continue;
						}
						else
						{
							if(pos_cnt == max_param) return false;SetPos(pos_cnt,i);state = 0;pos_cnt++;
						}
					}
				}
				else if(fdata()[i] == pTrim[pattern_step])
				{
					if(fdata()[i] != pTrim[pattern_step-1]) pattern_step++;
					fdata()[i] = 0x00;
				}
			}
		}
		if(Fx_Strlen(pTrim) != pattern_step) return false;
		return true;
	}
	bool Compare_Tok_Char(char *pTrim, char input)     // condition.. trim chariters
	{
		if(pTrim == FX_NULL) return false;
		for(unsigned int i = 0;i<Fx_Strlen(pTrim);i++){if(input == pTrim[i]) return true;}
		return false;
	}
	bool Compare_Tok_String(char *dst,char *ptrim)
	{
		if(strncmp(dst,ptrim,Trim_Len) != 0)	{SetPos(pos_cnt,0);pos_cnt++;}
		for(unsigned int i = 0;i<Orig_Len;i++)
		{
			tok_str_jump:
			if(i >= Orig_Len) break;
			for(unsigned int j=0;j<Trim_Len;j++,i++)
			{if(ptrim[j] != dst[i]){i++;goto tok_str_jump;}}
			for(unsigned int k = 0;k<Trim_Len;k++) fdata()[i-k-1]  = 0;
			if(i < Orig_Len-1){if(pos_cnt == max_param)return false;SetPos(pos_cnt,i);pos_cnt++;}
		}
		return true;
	}
	char * GetParam(int idx)
	{
		if((unsigned int)idx >= pos_cnt) return FX_NULL;
		else if(data) return &(fdata()[GetPos(idx)]);
		else return FX_NULL;
	}
	unsigned int GetParamLen(int idx)
	{
		if((unsigned int)idx >= max_param) return 0;
		return GetPosLen(idx);
	}
	unsigned int CopyParam(int idx,char *InOut,unsigned int input_len)
	{
		char *StrParam = GetParam(idx); if(StrParam == FX_NULL) return 0;
		if(input_len <= GetParamLen(idx)) return 0;
		memset(InOut,0x00,input_len);
		memcpy(InOut,StrParam,GetParamLen(idx));
	}
	char *Debug_Print(char *Resource)
	{
		Resource[0]=0;
		for(unsigned int i=0;i<pos_cnt;i++)
		{
			if(mType != TOK_PARSE_CHAR_DIVIDE) Fx_StrCat(Resource,"[%d] %s\r\n",i,GetParam(i));
			else Fx_StrCat(Resource,"[%d][len=%d] %s\n",i,GetParamLen(i),GetParam(i));
		}
		return Resource;
	}
}st_tok_param;
/*
???????? ???? ???? ?????? ???? ???? ??????
*/
typedef struct
{
	unsigned int pos_start[MAX_TOK_PARAM];
	unsigned int pos_len[MAX_TOK_PARAM];
	unsigned int Orig_Len;
	unsigned int state;      // 0 = empty, 1 = full
	unsigned int pos_cnt;
	char *data;
	void Init( char *pTrim, char *pOrig)
	{
		unsigned int i = 0;
		for(;i<MAX_TOK_PARAM;i++){pos_start[i] = 0;pos_len[i] = 0;}
		for(;i<MAX_TOK_PARAM;i++){pos_start[i] = 0;pos_len[i] = 0;}
		Orig_Len = 0;state = 0;	pos_cnt = 0;data = NULL;
		if(pTrim== FX_NULL || pOrig == FX_NULL) return;
		if(pTrim[0]==0 || pOrig[0]==0) return;
		data = pOrig;
		Orig_Len = Fx_Strlen(data);
		for(i = 0;i<Orig_Len;i++)
		{
			if(Compare_Tok(pTrim,data[i])){if(state == 1){pos_len[pos_cnt-1] = i - pos_start[pos_cnt-1];}state = 0;}  
			else	{if(state == 0){pos_start[pos_cnt] = i;state = 1;pos_cnt++;}}
		}
		if(pos_len[pos_cnt-1] == 0){pos_len[pos_cnt-1] = i - pos_start[pos_cnt-1];}
	}
	bool Compare_Tok(char *pTrim, char input)     // condition.. trim chariters
	{
		for(unsigned int i = 0;i<Fx_Strlen(pTrim);i++){if(input == pTrim[i]) return true;}
		return false;
	}
	char * GetParam(int idx)
	{
		if(data) return &data[pos_start[idx]];
		else if((unsigned int)idx >= pos_cnt) return FX_NULL;
		else return FX_NULL;
	}
	unsigned int GetParamLen(int idx)
	{
		if(idx >= MAX_TOK_PARAM) return 0;
		return pos_len[idx];
	}
	unsigned int CopyParam(int idx,char *InOut,unsigned int input_len)
	{
		char *StrParam = GetParam(idx); if(StrParam == FX_NULL) return 0;
		if(input_len <= GetParamLen(idx)) return 0;
		memset(InOut,0x00,input_len);
		memcpy(InOut,StrParam,GetParamLen(idx));
	}
}st_tok_param_divide;
/*    ???? ???? ????   ???? ?????? ????*/
inline bool INL_String_Parse(char *Orig,char *pTrim_Chr,int max_param,
								char *pResult[], int *Result_Num)
{
	if(Orig == FX_NULL) return false;if(Result_Num == FX_NULL) return false;
	int Orig_Len = Fx_Strlen(Orig);int pTrim_Len = Fx_Strlen(pTrim_Chr);
	int i=0;int j=0;int prev_st = 0;int next_st = 0;*Result_Num = 0;bool bmatch = false;
	for(i = 0; i<max_param;i++) pResult[i] = FX_NULL;
	for(i = 0;i<Orig_Len;i++)
	{
		for(j = 0;j<pTrim_Len;j++)
		{
			if(Orig[i] == pTrim_Chr[j]){Orig[i] = 0;prev_st = 0;break;}
			else prev_st = 1;
		}
		if(prev_st != next_st)
		{
			if(prev_st == 1)   // Detecting String
			{
				if(max_param <= (*Result_Num)) {return false;}
				if(i == 0) bmatch = false;
				else bmatch = true;
				pResult[*Result_Num] = &Orig[i];(*Result_Num)++;
			}
		}
		next_st = prev_st;
	}
	if(bmatch) return true;
	else return false;
}
/*    ?????? ???? ????   ???? ?????? ????*/
inline bool INL_String_Parse_2(char *Orig,char *pTrim_Str,int max_param,
								char *pResult[],int *Result_Num)
{
	int i=0;int match_idx = 0;int match_temp = -1;
	for(i = 0; i<max_param;i++) pResult[i] = FX_NULL;
	if(Orig == FX_NULL) return false; if(Result_Num == FX_NULL) return false;
	int Orig_Len = Fx_Strlen(Orig);int pTrim_Len = Fx_Strlen(pTrim_Str);
	if(Orig_Len <= pTrim_Len) return false;*Result_Num = 0;
	while(1)
	{
		match_temp = Fx_StrStr_2(&Orig[match_idx],pTrim_Str);
		if(match_temp  == (-1))
		{
			if((*Result_Num) > 0) return true;
			else return false;
		}
		match_idx += match_temp;
		memset(&Orig[match_idx],0x00,pTrim_Len);
		match_idx+=pTrim_Len;		
		if(match_idx >= Orig_Len) return true;
		if(match_temp == 0)
		{
			if((*Result_Num) > 0) (*Result_Num)--;
		}
		else if(match_temp > 0)
		{
			if((*Result_Num) == 0)
			{
				pResult[0] = &Orig[0];
				(*Result_Num)++;
			}
		}
		pResult[(*Result_Num)] = &Orig[match_idx];
		(*Result_Num)++;
	}
	return true;
}
/*    ???? ???? ????   ?????????? ???? ???????? ???? ????*/
inline bool INL_String_Parse_3(char *Orig,char *pTrim_Chr,int max_param,
								char *pResult[],int Result_Len[],int *Result_Num)
{
	if(Orig == FX_NULL) return false; if(Result_Num == FX_NULL) return false;
	int Orig_Len = Fx_Strlen(Orig);int pTrim_Len = Fx_Strlen(pTrim_Chr);
	int temp_len = 0;int i=0;int j=0;bool bmatch = false;
	int prev_st = 0;int next_st = 0; *Result_Num = 0;
	for(i = 0; i<max_param;i++) pResult[i] = FX_NULL;
	for(i = 0; i<max_param;i++) Result_Len[i] = 0;
	for(i = 0;i<Orig_Len;i++)
	{
		for(j = 0;j<pTrim_Len;j++)
		{
			if(Orig[i] == pTrim_Chr[j]) {/*Orig[i] = 0*/;prev_st = 0;break;}
			else prev_st = 1;
		}
		if(prev_st != next_st)
		{
			if(prev_st == 0)   // Finnished String
			{
				if((*Result_Num) > 0) Result_Len[(*Result_Num)-1] = temp_len;
				else Result_Len[0] = temp_len;
				temp_len = 0;
			}
			else if(prev_st == 1)   // Detecting String
			{
				if(max_param <= (*Result_Num)) {return true;}
				if(i == 0) bmatch = false;
				else bmatch = true;
				pResult[(*Result_Num)] = &Orig[i];(*Result_Num)++;
				temp_len++;
			}
		}
		else
		{
			if(prev_st == 1) temp_len++;
		}
		next_st = prev_st;
	}
	if(bmatch)
	{
		if(prev_st == 1)
		{
			if((*Result_Num) > 0) Result_Len[(*Result_Num)-1] = temp_len;
			else Result_Len[0] = temp_len;
		}
		return true;
	}
	else return false;
}
/*     ???? ?????? ????     */
/*      pattern <:@>  Input <sip:1234@sam.net>                   */
inline bool INL_Pattern_Parse(char *Orig, char *Pattern,int Max, char *Result[],int *R_Num)
{
	int len = Fx_Strlen(Orig);int plen = Fx_Strlen(Pattern);
	for(int j=0;j<Max;j++) Result[j]=NULL;int p_cnt = 0;int R_cnt = 0;
	int bPrev_P = 3;int bNext_P = 3;char *pTemp = NULL;
	for(int i=0;i<len;i++)
	{
		if(bPrev_P == 1)
		{
			if(Orig[i] != Pattern[p_cnt]) bPrev_P = 2;
			else  return false;
		}
		else 
		{
			if(Orig[i] == Pattern[p_cnt]){bPrev_P = 1;p_cnt++;Orig[i]=0;}
			else bPrev_P = 2;
		}
		if(bPrev_P != bNext_P)
		{
			if(bPrev_P == 2)
			{
				if(R_cnt == (Max-1)) return false;
				pTemp = &Orig[i];
				for(int k=0;((pTemp[k]!=Pattern[p_cnt])||(pTemp[k]==0));k++)
				{
					if(pTemp[k] != ' ')
					{
						if(Fx_Strlen(&(pTemp[k]))>0) Result[R_cnt++] = &(pTemp[k]);
						break;
					}
				}
			}
		}
		bNext_P = bPrev_P;
	}
	if(plen != p_cnt) return false;
	if(R_Num) *R_Num = R_cnt;
	return true;
}

typedef struct
{
	unsigned int conv_type;
	bool bIsDisplay;
	char conv_num[MAX_TOK_DATA_SIZE];
	st_tok_param tok;
	char strtok[MAX_TOK_DATA_SIZE];
	void Init()
	{
		conv_type = 0;
		memset(conv_num,0x00,MAX_TOK_DATA_SIZE);
		bIsDisplay = false;
		tok.Init(MAX_TOK_PARAM,MAX_TOK_DATA_SIZE,strtok);
	}
	char *MatchPrefix(char *pre_fix[],char *call_num)
	{
		if(pre_fix[0] == NULL) return call_num;
		char *temp_num = call_num;
		int match_idx[20];
		Match_Retry:
		for(int i=0;i<20;i++) match_idx[i] = 0;
		int match_cnt = 0;
		int result_idx = 0;
		for(int i=0;pre_fix[i]!=NULL;i++)
		{
			if(Fx_StrnCmp(temp_num,pre_fix[i],Fx_Strlen(pre_fix[i])))
			{
				match_idx[match_cnt] = i;
				if(match_cnt > 0)
				{
					if(Fx_Strlen(pre_fix[match_idx[match_cnt-1]]) > Fx_Strlen(pre_fix[match_idx[match_cnt]]))
					{
						result_idx = match_idx[match_cnt-1];
					}
					else
					{
						result_idx = match_idx[match_cnt];
					}
				}
				else result_idx = i;
				match_cnt++;
			}
		}
		if(match_cnt==0)
		{
			return temp_num;
		}
		else
		{
			temp_num = &temp_num[Fx_Strlen(pre_fix[result_idx])];
			goto Match_Retry;
		}
	}
	char * conversion_Uri(FX_IN char *prefix[],FX_IN char * In_Uri)
	{
		conv_type = 1;
		st_tok_param_divide tok_disp;
		tok_disp.Init((char*)" <",In_Uri);
		if(tok_disp.pos_cnt == 2) bIsDisplay = true;
		tok.Parse(TOK_PARSE_CHAR,(char*)" <:@>;",In_Uri);
		char *full_Number = GetFullNumber();
		full_Number = MatchPrefix(prefix,full_Number);
		if(full_Number[0] == '+')  full_Number+=3;	
		else if(full_Number[0] == '8' && full_Number[1] == '2') full_Number +=2;
		return collect_number(full_Number);
	}
	char * conversion_Number(FX_IN char *prefix[],FX_IN char * In_FullNum)
	{
		conv_type = 2;
		char *temp = FX_NULL;
		char *full_Number = In_FullNum;
		unsigned int i = 0;
		if(In_FullNum == FX_NULL) return FX_NULL;
		memset(conv_num,0x00,MAX_TOK_DATA_SIZE);
		full_Number = MatchPrefix(prefix,full_Number);
		if(full_Number[0] == '+') full_Number +=3;
		else if(full_Number[0] == '8' && full_Number[1] == '2') full_Number +=2;
		tok.Parse(TOK_PARSE_CHAR,(char*)"+()-.,/",full_Number);
		for(i = 0;i<tok.pos_cnt;i++)
		{
			temp = tok.GetParam(i);
			if((i == 0) && ((temp[0] > '0') && (temp[0] <= '9'))) sprintf(conv_num,"0%s",temp);
			else Fx_StrCat(conv_num,temp);
		}
		return conv_num;
	}
	char * collect_number(FX_IN char *full_Number)
	{
		st_tok_param tok_num;char strtemp[MAX_TOK_DATA_SIZE];
		tok_num.Init(MAX_TOK_PARAM,MAX_TOK_DATA_SIZE,strtemp);
		char *temp = FX_NULL;
		unsigned int i = 0;
		for(i=0;i<Fx_Strlen(full_Number);i++)
		{if(full_Number[i] == ';') full_Number[i] = 0;}
		tok_num.Parse(TOK_PARSE_CHAR,(char*)"+()-.,/",full_Number);
		memset(conv_num,0x00,MAX_TOK_DATA_SIZE);
		for(i=0;i<tok_num.pos_cnt;i++)
		{
			temp = tok_num.GetParam(i);
			if((i == 0) && ((temp[0] > '0') && (temp[0] <= '9'))) sprintf(conv_num,"0%s",temp);
			else Fx_StrCat(conv_num,temp);
		}
		return conv_num;
	}
	char * GetDisplay(){if(bIsDisplay) return tok.GetParam(0);else return FX_NULL;}
	char * GetNumType()
	{if(conv_type != 1) return FX_NULL; if(bIsDisplay) return tok.GetParam(1);else return tok.GetParam(0);}
	char * GetFullNumber()
	{if(conv_type != 1) return FX_NULL; if(bIsDisplay) return tok.GetParam(2);else return tok.GetParam(1);}
	char * GetConvNumber(){return conv_num;}
}st_call_number_plan;

/*
	sip:1234@pointi.com SIP/2.0
	tel:1234;phone-context=pointi.com SIP/2.0
*/
typedef struct
{
	st_tok_param mtok;char strtok[MAX_TOK_DATA_SIZE];char conv_num[256];
	char * Init(FX_IN char *pOrig,FX_IN char *prefix[])
	{
		unsigned int i = 0;mtok.Init(MAX_TOK_PARAM,MAX_TOK_DATA_SIZE,strtok);
		st_tok_param ltok;char lstr[MAX_TOK_DATA_SIZE];	ltok.Init(MAX_TOK_PARAM,MAX_TOK_DATA_SIZE,lstr);
		char *temp = FX_NULL;mtok.Parse(TOK_PARSE_CHAR,(char*)" :@;=",pOrig);
		char *full_number = mtok.GetParam(1);
		while(1)
		{
			if(prefix[i] == FX_NULL) break;
			else
			{
				if(Fx_StrnCmp(full_number,prefix[i],Fx_Strlen(prefix[i])))
				{full_number+=Fx_Strlen(prefix[i]);break;}
			}
			i++;
		}
		if(full_number[0] == '+') full_number+=3;
		ltok.Parse(TOK_PARSE_CHAR,(char*)"+()-.,/",full_number);memset(conv_num,0x00,256);
		for(i=0;i<ltok.pos_cnt;i++)
		{
			temp = ltok.GetParam(i);
			if((i == 0) && ((temp[0] > '0') && (temp[0] <= '9'))) sprintf(conv_num,"0%s",temp);
			else Fx_StrCat(conv_num,temp);
		}
		return conv_num;
	}
}st_request_uri_plan;

/***************** Flexible Data Struct **********************/
//---------> Flex Control Header
typedef struct
{
	char k_cnt;char d_cnt;char len[2];char key[MAX_FLEX_OPT_NUM*2];     
	char kpos[MAX_FLEX_OPT_NUM*2];char dpos[MAX_FLEX_OPT_NUM*2];       
	int GetCtlSize(){return (MAX_FLEX_OPT_NUM*6 + 4);};
}st_fixed_ctl;
typedef struct
{
	char hdr_num;char k_cnt;char d_cnt;char len[2];          
	char *data(){return (char*)(this+1);}
	void Init(int mhdr){hdr_num = (unsigned char)mhdr;Set_Ctl_Len(hdr_num*8);}
	int Get_key_arr(FX_IN int idx){return Fx_Get_Short(&(data()[idx*2]));}
	int Get_kpos_arr(FX_IN int idx){return Fx_Get_Short(&(data()[hdr_num*2+idx*2]));}
	int Get_dpos_arr(FX_IN int idx){return Fx_Get_Short(&(data()[hdr_num*4+idx*2]));}
	int Get_d_len(FX_IN int idx){return Fx_Get_Short(&(data()[hdr_num*6+idx*2]));}
	void Set_key_arr(FX_IN int mpos){Fx_Set_Short(mpos,&(data()[k_cnt*2]));}
	void Set_kpos_arr(FX_IN int mpos){Fx_Set_Short(mpos,&(data()[hdr_num*2 + k_cnt*2]));}
	void Set_dpos_arr(FX_IN int mpos){Fx_Set_Short(mpos,&(data()[hdr_num*4 + d_cnt*2]));}
	void Set_d_len(FX_IN int mlen){Fx_Set_Short(mlen,&(data()[hdr_num*6 + d_cnt*2]));}
	void Set_Ctl_Len(FX_IN int mlen){Fx_Set_Short(mlen, &(len[0]));}
	int Get_Ctl_Len(){return Fx_Get_Short(&(len[0]));}
	int GetCtlSize(){return 5;}
}st_variable_ctl;
/* Type 4:  [key enum & key string & data String Array] Max Header => MAX_FLEX_OPT_NUM*/
typedef struct
{
	st_fixed_ctl ctl;
	char *data(){return (char *)(this + 1);}
	int GetKey(FX_IN int i){return Fx_Get_Short(&(ctl.key[i*2]));}
	void SetKey(FX_IN int mpos){Fx_Set_Short(mpos, &(ctl.key[ctl.k_cnt*2]));return;}
	int GetKPos(FX_IN int i){return Fx_Get_Short(&(ctl.kpos[i*2]));}
	void SetKPos(FX_IN int mpos){Fx_Set_Short(mpos, &(ctl.kpos[ctl.k_cnt*2]));return;}
	int GetDPos(FX_IN int i){return Fx_Get_Short(&(ctl.dpos[i*2]));}
	void SetDPos(FX_IN int mpos){Fx_Set_Short(mpos, &(ctl.dpos[ctl.d_cnt*2]));return;}
	int GetOptLen(){return Fx_Get_Short(&(ctl.len[0]));}
	void SetOptLen(FX_IN int mlen){Fx_Set_Short(mlen, &(ctl.len[0]));return;}
	int GetKOptCnt(){return (ctl.k_cnt);}
	int GetDOptCnt(){return (ctl.d_cnt);}
	char * GetKOption(FX_IN int i){if(GetKOptCnt() <= i) return FX_NULL;return (char *)&(data()[GetKPos(i)]);}
	char * GetDOption(FX_IN int i){if(GetDOptCnt() <= i) return FX_NULL;return (char *)&(data()[GetDPos(i)]);}
	void SetOption(FX_IN int mkey,FX_IN char * key_str,FX_IN char * op)
	{
		if(GetDOptCnt() == MAX_FLEX_OPT_NUM) return;
		if (ctl.d_cnt == 0x00)
		{
			SetKey(mkey);SetKPos(0);
			if(key_str != FX_NULL)
			{memcpy(&(data()[0]),key_str,Fx_Strlen(key_str));SetOptLen((1 + Fx_Strlen(key_str)));}
			else SetOptLen(1);
			ctl.k_cnt = 1;	SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,Fx_Strlen(op));SetOptLen((GetOptLen() + Fx_Strlen(op) + 1));}
			else SetOptLen(GetOptLen()+1);
			ctl.d_cnt = 1;
		}
		else
		{
			SetKey(mkey);SetKPos(GetOptLen());
			if(key_str != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.k_cnt) ++;SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,Fx_Strlen(op));SetOptLen((GetOptLen() + Fx_Strlen(op) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.d_cnt) ++;		
		}
		return;
	}
	void SetOption_2(FX_IN int mkey,FX_IN char * key_str,FX_IN char * op,FX_IN int cp_len)
	{
		if(GetDOptCnt() == MAX_FLEX_OPT_NUM) return;
		if (ctl.d_cnt == 0x00)
		{
			SetKey(mkey);SetKPos(0);
			if(key_str != FX_NULL)
			{memcpy(&(data()[0]),key_str,Fx_Strlen(key_str));SetOptLen((1 + Fx_Strlen(key_str)));}
			else SetOptLen(1);
			ctl.k_cnt = 1;	SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,cp_len);SetOptLen((GetOptLen() + cp_len + 1));}
			else SetOptLen(GetOptLen()+1);
			ctl.d_cnt = 1;
		}
		else
		{
			SetKey(mkey);SetKPos(GetOptLen());
			if(key_str != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.k_cnt) ++;SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,cp_len);SetOptLen((GetOptLen() + cp_len + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.d_cnt) ++;		
		}
		return;
	}
	void Fx_Flex_Add(FX_IN int mkey,FX_IN char * key_str,FX_IN const char * op,...)
	{
		char *Cur_Data = FX_NULL;
		if(GetDOptCnt() == MAX_FLEX_OPT_NUM) return;
		if (ctl.d_cnt == 0x00)
		{
			SetKey(mkey);SetKPos(0);
			if(key_str != FX_NULL)
			{memcpy(&(data()[0]),key_str,Fx_Strlen(key_str));SetOptLen((1 + Fx_Strlen(key_str)));}
			else SetOptLen(1);
			ctl.k_cnt = 1;	SetDPos(GetOptLen());	Cur_Data = &(data()[GetOptLen()]);
			va_list ap;va_start(ap, op);vsprintf(Cur_Data, op, ap);va_end(ap);
			SetOptLen(GetOptLen()+Fx_Strlen(Cur_Data) + 1);	ctl.d_cnt = 1;
		}
		else
		{
			SetKey(mkey);SetKPos(GetOptLen());
			if(key_str != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.k_cnt) ++;SetDPos(GetOptLen());Cur_Data = &(data()[GetOptLen()]);
			va_list ap;va_start(ap, op);	vsprintf(Cur_Data, op, ap);va_end(ap);
			SetOptLen(GetOptLen()+Fx_Strlen(Cur_Data) + 1);	(ctl.d_cnt) ++;		
		}
		return;
	}
	char * Find_1(FX_IN int mType)
	{
		for(int i = 0;i<GetKOptCnt();i++)	{if(GetKey(i) == mType) return GetDOption(i);}
		return FX_NULL;
	}
	char * Find_2(FX_IN char *pkey)
	{
		if(pkey == FX_NULL) return FX_NULL;
		for(int i=0;i<GetKOptCnt();i++)
		{
			if(GetKOption(i) != FX_NULL)
			{if(Fx_StrCmp(GetKOption(i),pkey)) return GetDOption(i);}
		}
		return FX_NULL;
	}
	int GetFlexSize()
	{
		if(ctl.d_cnt == 0x00) return ctl.GetCtlSize();
		else return ctl.GetCtlSize() + GetOptLen();
	}
}st_flex_4;
inline char * FindFlex_Add_1(FX_IN char *ptkey,FX_IN int mpt,FX_IN int mType, FX_IN st_flex_4 *flex)
{
	for(int i=0;i<flex->GetDOptCnt(); i++)
	{if(ptkey[i] == mpt){if(flex->GetKey(i) == mType) return flex->GetDOption(i);}}
	return FX_NULL;
}
inline char * FindFlex_Add_2(FX_IN char *ptkey,FX_IN int mpt,FX_IN char *pkey,FX_IN st_flex_4 *flex)
{
	if(pkey == FX_NULL) return FX_NULL;
	for(int i=0;i<flex->GetDOptCnt(); i++)
	{if(ptkey[i] == mpt){if(Fx_StrCmp(flex->GetKOption(i),pkey)) return flex->GetDOption(i);}}
	return FX_NULL;
}
/* Type 5:  [key enum & key string & data String Array] Max Header => User Input Value */
typedef struct 
{
	st_variable_ctl ctl;
	char *data(){return (char*)(this+1);}    // flexible data
	void Init(int hdr_max){memset(&ctl,0x00,sizeof(st_variable_ctl));ctl.Init(hdr_max);}
	int GetMaxHdr(){return ctl.hdr_num;}
	int GetKey(FX_IN int i){return ctl.Get_key_arr(i);}
	void SetKey(FX_IN int mpos){ctl.Set_key_arr(mpos);}
	int GetKPos(FX_IN int i){return ctl.Get_kpos_arr(i);}
	void SetKPos(FX_IN int mpos){ctl.Set_kpos_arr(mpos);}
	int GetDPos(FX_IN int i){return ctl.Get_dpos_arr(i);}
	void SetDPos(FX_IN int mpos){ctl.Set_dpos_arr(mpos);}
	int GetDLen(FX_IN int i){return ctl.Get_d_len(i);}
	void SetDLen(FX_IN int mlen){ctl.Set_d_len(mlen);}
	int GetOptLen(){return ctl.Get_Ctl_Len();}
	void SetOptLen(FX_IN int mlen){ctl.Set_Ctl_Len(mlen);}
	int GetKOptCnt(){	return (ctl.k_cnt);	}
	int GetDOptCnt(){	return (ctl.d_cnt);}
	char * GetKOption(FX_IN int i){if(GetKOptCnt() <= i) return FX_NULL;return (char *)&(data()[GetKPos(i)]);}
	char * GetDOption(FX_IN int i){if(GetDOptCnt() <= i) return FX_NULL;return (char *)&(data()[GetDPos(i)]);}
	void SetOption(FX_IN int mkey,FX_IN char * key_str,FX_IN char * op)
	{
		if(GetDOptCnt() == ctl.hdr_num) return;
		SetKey(mkey);SetKPos(GetOptLen());
		if(key_str != FX_NULL)
		{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
		else SetOptLen((GetOptLen() + 1));
		(ctl.k_cnt) ++;SetDPos(GetOptLen());
		if(op != FX_NULL)
		{memcpy(&(data()[GetOptLen()]),op,Fx_Strlen(op));SetOptLen((GetOptLen() + Fx_Strlen(op) + 1));}
		else SetOptLen((GetOptLen() + 1));
		SetDLen(Fx_Strlen(op));(ctl.d_cnt) ++;
		return;
	}
	void SetOption_2(FX_IN int mkey,FX_IN char * key_str,FX_IN void * op,FX_IN int cp_len)
	{
		if(GetDOptCnt() == MAX_FLEX_OPT_NUM) return;
		if(cp_len <=0) return;
		if (ctl.d_cnt == 0x00)
		{
			SetKey(mkey);SetKPos(0);
			if(key_str != FX_NULL)
			{memcpy(&(data()[0]),key_str,Fx_Strlen(key_str));SetOptLen((1 + Fx_Strlen(key_str)));}
			else SetOptLen(1);
			ctl.k_cnt = 1;	SetDPos(GetOptLen());
			{memcpy(&(data()[GetOptLen()]),op,cp_len);SetOptLen((GetOptLen() + cp_len + 1));}
		}
		else
		{
			SetKey(mkey);SetKPos(GetOptLen());
			if(key_str != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
			else SetOptLen((GetOptLen() + 1));
			(ctl.k_cnt) ++;SetDPos(GetOptLen());
			if(op != FX_NULL)
			{memcpy(&(data()[GetOptLen()]),op,cp_len);SetOptLen((GetOptLen() + cp_len + 1));}
			else SetOptLen((GetOptLen() + 1));
			SetDLen(cp_len);	(ctl.d_cnt) ++;		
		}
		return;
	}
	void Fx_Flex_Add(FX_IN int mkey,FX_IN char * key_str,FX_IN const char * op,...)
	{
		char *Cur_Data = FX_NULL;
		if(GetDOptCnt() == ctl.hdr_num) return;
		SetKey(mkey);SetKPos(GetOptLen());
		if(key_str != FX_NULL)
		{memcpy(&(data()[GetOptLen()]),key_str,Fx_Strlen(key_str));SetOptLen((GetOptLen() + Fx_Strlen(key_str) + 1));}
		else SetOptLen((GetOptLen() + 1));
		(ctl.k_cnt) ++;SetDPos(GetOptLen());Cur_Data = &(data()[GetOptLen()]);
		va_list ap;va_start(ap, op);	vsprintf(Cur_Data, op, ap);va_end(ap);
		SetOptLen(GetOptLen()+Fx_Strlen(Cur_Data) + 1);SetDLen(Fx_Strlen(Cur_Data));(ctl.d_cnt) ++;		
		return;
	}
	char * Find_1(FX_IN int mType)
	{
		for(int i=0;i<GetKOptCnt();i++){if(GetKey(i) == mType) return GetDOption(i);}
		return FX_NULL;
	}
	char * Find_2(FX_IN char *pkey)
	{
		if(pkey ==FX_NULL) return FX_NULL;
		for(int i=0;i<GetKOptCnt();i++)
		{if(GetKOption(i) != FX_NULL){if(Fx_StrCmp(GetKOption(i),pkey)) return GetDOption(i);}}
		return FX_NULL;
	}
	void * Find_ByteData(FX_IN int mType, FX_IN char *pkey, FX_OUT unsigned int *plen)
	{
		int i = 0;
		if(mType != -1)
		{
			for(i=0;i<GetKOptCnt();i++)
			{
				if(GetKey(i) == mType)
				{
					*plen = GetDLen(i); return (void*)GetDOption(i);
				}
			}
			return NULL;
		}
		else if(pkey != NULL)
		{
			for(i=0;i<GetKOptCnt();i++)
			{
				if(GetKOption(i) != FX_NULL)
				{
					if(Fx_StrCmp(GetKOption(i),pkey))
					{
						*plen = GetDLen(i);return (void*)GetDOption(i);
					}
				}
			}
			return NULL;
		}
		return NULL;
	}
	int GetFlexSize()
	{if(ctl.d_cnt == 0x00) return ctl.GetCtlSize() + ctl.hdr_num*8;else return ctl.GetCtlSize() + GetOptLen();}
}st_flex_5;
/***************** Flexible Data Struct End**********************/

/***************** Contents Structures **********************/
// >>> 1. dtmf
typedef struct
{
    unsigned char   cnt;
    unsigned char   digit[32];
} st_dtmf;
// >>> 2. sdp
typedef struct
{
	char payload;        //ex> PAYLOAD_G723_AUDIO
	char samplerate[4];
	char bitrate[4];
	char parameter; // by hak. R102 PKG. 150513.
	int Getsize(){return 10;} // modify by hak. R102 PKG. 150513. orig value = 9.
	void Set_Payload(FX_IN int val){payload = (char)val;return;}    
	int Get_Payload(){return (unsigned char)payload;}  
	void Set_SampleRate(FX_IN int val){Fx_Set_Int(val, &samplerate[0]);return;}
	int Get_SampleRate(){return Fx_Get_Int(&samplerate[0]);}
	void Set_BitRate(FX_IN int val){Fx_Set_Int(val, &bitrate[0]);return;}
	int Get_BitRate(){return Fx_Get_Int(&bitrate[0]);}
	void Set_Parameter(FX_IN int val){parameter = (char)val;return;}
	int Get_Parameter(){return (unsigned char)parameter;}
}st_attribute;
typedef struct
{
	char media_t;    // SDPMEDIATYPE_AUDIO
	char mode;       //SDPCONNECTMODE_SENDRECV
	char proto;       //   SDPPROTOCOL_RTP                    
	char a_cnt;        // atrribute count  
	char ip[4];                                                      
	char port[4];   
	char ptime[4];         //60 ms
	char framerate[4];
	st_attribute attri[MAX_SDP_PT_NUM];
	int Getsize(){return 20 + 10*MAX_SDP_PT_NUM;}    //92                                                
	int Get_IP(){return Fx_Get_Int(&ip[0]);}                                
	void Set_IP(FX_IN int mIp){Fx_Set_Int(mIp,&ip[0]);return;}             
	int Get_Proto(){return (unsigned char)proto;}                        
	void Set_Proto(FX_IN int mproto){proto = (char)mproto;return;}   
	int Get_Media(){return (unsigned char)media_t;}   
	void Set_Media(int mm){media_t = (char)mm;return;}  
	int Get_Mode(){return (unsigned char)mode;}          
	void Set_Mode(FX_IN int val){mode = (char)val;return;}   
	int Get_Attr_Count(){return (unsigned char)a_cnt;}     
	void Set_Attr_Count(FX_IN int cnt){a_cnt = (char)cnt;return;}    
	void Set_Port(FX_IN int val){Fx_Set_Int(val, &port[0]);return;}
	int Get_Port(){return Fx_Get_Int(&port[0]);}
	void Set_PTime(FX_IN int val){Fx_Set_Int(val, &ptime[0]);return;}
	int Get_PTime(){return Fx_Get_Int(&ptime[0]);}
	void Set_FrameRate(FX_IN int val){Fx_Set_Int(val, &framerate[0]);return;}
	int Get_FrameRate(){return Fx_Get_Int(&framerate[0]);}
}st_media_descriptor;

//imageattr:<payload type> send [x=320,y=240] recv [x=320,y=240]
typedef struct
{
	char m_cnt;
	char sdp_ip[4];
	char m_num[MAX_FLEX_OPT_NUM];                     // Additional Flexible Control Data 
	char pt[MAX_FLEX_OPT_NUM];                           // Additional Flexible Control Data
	st_media_descriptor desc[4];
	st_flex_4 flex;                                                 // ex> Flexible Opt String Process   
	/**************** Flexible Common Proc ****************/
	int Getkey(FX_IN int i){return flex.GetKey(i);}
	int GetPTkey(FX_IN int i){return (unsigned char)(pt[i]);}     
	int GetMediakey(FX_IN int i){return m_num[i];}
	int GetKPos(FX_IN int i){return flex.GetKPos(i);}
	int GetDPos(FX_IN int i){return flex.GetDPos(i);}
	int GetOptLen(){return flex.GetOptLen();}
	int GetKOptCnt(){return flex.GetKOptCnt();}
	int GetDOptCnt(){return flex.GetDOptCnt();}
	char * GetKOption(FX_IN int i){return flex.GetKOption(i);}
	char * GetDOption(FX_IN int i){return flex.GetDOption(i);}
	int GetStringType(FX_IN int i)
	{
		if(((unsigned char)(pt[i]) == 0xff) &&((unsigned char)(m_num[i]) == 0xff)) return STRING_SDP_COMMON;
		else if(((unsigned char)(pt[i]) == 0xff) &&((unsigned char)(m_num[i]) != 0xff)) return STRING_SDP_MEDIA;
		else if(((unsigned char)(pt[i]) != 0xff)) return STRING_SDP_PAYLOAD;
		return STRING_SDP_NONE;
	}
	int Getsize(){return (5 + desc[0].Getsize()*4+ MAX_FLEX_OPT_NUM*2  +  flex.GetFlexSize());}   //373
	void SetPTkey(FX_IN int mpt){pt[flex.GetKOptCnt()] = (char)(mpt);return;}     
	void SetMediakey(FX_IN int mpt){m_num[flex.GetKOptCnt()] = (char)(mpt);return;}   
	void Flex_Set(FX_IN int m_arr,FX_IN int mpt,FX_IN int mkey,FX_IN char *str_key,FX_IN char * op)
	{
		if(IsOver()) return;
		SetMediakey(m_arr);SetPTkey(mpt);flex.SetOption(mkey,str_key,op);
	}
	void Flex_Set_2(FX_IN int m_arr,FX_IN int mpt,FX_IN int mkey,FX_IN char *str_key,FX_IN char * op,FX_IN unsigned int cp_len)
	{
		if(IsOver()) return;
		SetMediakey(m_arr);SetPTkey(mpt);flex.SetOption_2(OPT_OTHER,str_key,op,cp_len);
	}
	bool IsOver(){if(flex.GetDOptCnt() == MAX_FLEX_OPT_NUM) return true;return false;}
	/**************** Media Address Proc ****************/
	int Get_IP(){return Fx_Get_Int(&sdp_ip[0]);}
	char *Get_OIP(){return Find_Common_2((char*)"o_ip");}
	char *Get_CIP(){return Find_Common_2((char*)"c_ip");}
	char *Get_MIP(FX_IN int m_arr){return Find_Media_2(m_arr,(char*)"m_ip");}
	char *Get_StrOIP(){return Find_Common_2((char*)"o_v6Ip");}
	char *Get_StrMIP(FX_IN int m_arr){return Find_Media_2(m_arr,(char*)"m_v6Ip");}
	void Set_IP(FX_IN int val){Fx_Set_Int(val, &sdp_ip[0]);return;}
	void Set_OIP(char * strIP){Set_Common_2((char*)"o_ip",strIP);}
	void Set_CIP(char * strIP){Set_Common_2((char*)"c_ip",strIP);}
	void Set_MIP(FX_IN int m_arr,FX_IN char *strIP){Set_Media_2(m_arr,(char*)"m_ip",strIP);}
	void Set_StrOIP(FX_IN char *strIP){Set_Common_2((char*)"o_v6Ip",strIP);}
	void Set_StrMIP(FX_IN int m_arr,FX_IN char *strIP){Set_Media_2(m_arr,(char*)"m_v6Ip",strIP);}
	/**************** Common SDP Proc ****************/
	void Set_Common_1(FX_IN int mkey,FX_IN char * op){Flex_Set(0xff,0xff,mkey,FX_NULL,op);}
	void Set_Common_2(FX_IN char * key_str,FX_IN char * op){Flex_Set(0xff,0xff,OPT_OTHER,key_str,op);}
	void Set_Common_3(FX_IN char * key_str,FX_IN char * op,FX_IN unsigned int cp_len){Flex_Set_2(0xff,0xff,OPT_OTHER,key_str,op,cp_len);}
	void Set_Common_UserName(FX_IN char * op){Flex_Set(0xff,0xff,OPT_SDP_COMMON_O_NAME,FX_NULL,op);}
	void Set_Common_SessionID(FX_IN char * op){Flex_Set(0xff,0xff,OPT_SDP_COMMON_O_SESSION,FX_NULL,op);}
   void Set_Common_SessionVer(FX_IN char * op){Flex_Set(0xff,0xff,OPT_SDP_COMMON_O_SESSION_VER,FX_NULL,op);}
	char * Find_Common_1(FX_IN int mType){return flex.Find_1(mType);}
	char * Find_Common_2(FX_IN char *pkey){return flex.Find_2(pkey);}
	/**************** Media Parameter Proc ****************/
	int Get_Media_Count(){return m_cnt;}
	void Set_Media_Count(FX_IN int val){m_cnt = (unsigned char)val;return;}
	void Set_Media_1(FX_IN int m_arr,FX_IN int mkey,FX_IN char * op){	Flex_Set(m_arr,0xff,mkey,FX_NULL,op);}
	void Set_Media_2(FX_IN int m_arr,FX_IN char * key_str,FX_IN char * op){Flex_Set(m_arr,0xff,OPT_OTHER,key_str,op);}
	char * Find_Media_1(FX_IN int m_arr,FX_IN int mType){return FindFlex_Add_1(&m_num[0], m_arr,mType, &flex);}
	char * Find_Media_2(FX_IN int m_arr,FX_IN char *pkey){return FindFlex_Add_2(&m_num[0],m_arr, pkey, &flex);}
	/**************** Media Payload Proc ****************/
	void Set_Media_PT(FX_IN int m_arr,FX_IN int mpt,FX_IN int mkey,FX_IN char * op){Flex_Set(m_arr,mpt,mkey,FX_NULL,op);}
	void Set_Media_PT_2(FX_IN int m_arr,FX_IN int mpt,FX_IN char *pkey,FX_IN char * op){Flex_Set(m_arr,mpt,OPT_OTHER,pkey,op);}
	void Set_PT_Framesize(FX_IN int m_arr,FX_IN int mpt,FX_IN char * op){Flex_Set(m_arr,mpt,OPT_SDP_ATTRIBUTE_FRAMESIZE,FX_NULL,op);	}
	void Set_PT_Framerate(FX_IN int m_arr,FX_IN int mpt,FX_IN char * op){Flex_Set(m_arr,mpt,OPT_SDP_ATTRIBUTE_FRAMERATE,FX_NULL,op);} 
	void Set_PT_Codec(FX_IN int m_arr,FX_IN int mpt,FX_IN char *op){	Flex_Set(m_arr,mpt,OPT_SDP_ATTRIBUTE_RTPMAP,FX_NULL,op);}
	void Set_PT_Fmtp(FX_IN int m_arr,FX_IN int mpt,FX_IN char *op){Flex_Set(m_arr,mpt,OPT_SDP_ATTRIBUTE_FMTP,FX_NULL,op);}
	char * Find_Media_PT(FX_IN int m_arr,FX_IN int mpt,FX_IN int mType)
	{
		for(int i=0;i<flex.GetDOptCnt(); i++)
		{
			if(pt[i] == mpt && m_num[i] == m_arr && flex.GetKey(i) == mType) return flex.GetDOption(i);
		}
		return FX_NULL;
	}
	char * Find_Media_PT_2(FX_IN int m_arr,FX_IN int mpt,FX_IN char *pkey)
	{
		for(int i=0;i<flex.GetDOptCnt(); i++)
		{
			if(pt[i] == mpt && m_num[i] == m_arr && (Fx_StrCmp(flex.GetKOption(i),pkey))) return flex.GetDOption(i);
		}
		return FX_NULL;
	}
	char * Find_PT_Framesize(FX_IN int m_arr,FX_IN int mpt){return Find_Media_PT(m_arr,mpt,OPT_SDP_ATTRIBUTE_FRAMESIZE);}
	char * Find_PT_Framerate(FX_IN int m_arr,FX_IN int mpt){return Find_Media_PT(m_arr,mpt,OPT_SDP_ATTRIBUTE_FRAMERATE);}
	char * Find_PT_Codec(FX_IN int m_arr,FX_IN int mpt){return Find_Media_PT(m_arr,mpt,OPT_SDP_ATTRIBUTE_RTPMAP);}
	char * Find_PT_Fmtp(FX_IN int m_arr,FX_IN int mpt){return Find_Media_PT(m_arr,mpt,OPT_SDP_ATTRIBUTE_FMTP);}
	char * Find_Common_UserName(){return flex.Find_1(OPT_SDP_COMMON_O_NAME);}
	char * Find_Common_SessionID(){return flex.Find_1(OPT_SDP_COMMON_O_SESSION);}
   char * Find_Common_SessionVer(){return flex.Find_1(OPT_SDP_COMMON_O_SESSION_VER);}
	bool Find_PT_Fmtp_AMR_Sub(FX_INOUT st_tok_param *m_param,FX_IN int mpt,FX_OUT char *modeset, FX_OUT char *octetalign)
	{
		char * pFmtp = Find_PT_Fmtp(0,mpt);
		if(pFmtp != FX_NULL)
		{
			m_param->Parse(TOK_PARSE_CHAR,(char*)" =;",pFmtp);
			for(unsigned int i = 0;i<m_param->pos_cnt;i++)
			{
				if(Fx_StrCmp((char*)"mode-set",m_param->GetParam(i))) modeset = m_param->GetParam(i+1);
				else if(Fx_StrCmp((char*)"octet-align",m_param->GetParam(i))) octetalign = m_param->GetParam(i+1);
			}			
			return true;
		}
		else return false;
	}
   bool Find_PT_Fmtp_AMR_Sub2(FX_IN char *pszCodecName, FX_IN char *pFmtp, FX_OUT unsigned int *modeset, FX_OUT unsigned int *octetalign)
   {
      if(pFmtp != FX_NULL)
      {
         st_tok_param ltok;
         bool bFindOctetAlign = false;
         bool bFindModeSet = false;
         char lstr[MAX_TOK_DATA_SIZE];
         ltok.Init(MAX_TOK_PARAM,MAX_TOK_DATA_SIZE,lstr);
         ltok.Parse(TOK_PARSE_CHAR,(char*)" =;",pFmtp);

         for(unsigned int i = 0;i<ltok.pos_cnt;i++)
         {
            if(Fx_StrCmp((char*)"mode-set",ltok.GetParam(i)))
          {
             *modeset = Fx_Atoi(ltok.GetParam(i+1));
             bFindModeSet = true;
          }
            else if(Fx_StrCmp((char*)"octet-align",ltok.GetParam(i)))
          {
             *octetalign = Fx_Atoi(ltok.GetParam(i+1));
             bFindOctetAlign = true;
          }
         }
         if(bFindOctetAlign == false)
        {
           if(Fx_StrCmp((char*)"AMR", pszCodecName) || Fx_StrCmp((char*)"AMR-WB", pszCodecName))
           {
              *octetalign = 1;
           }
        }
         if(bFindModeSet == false)
         {
           if(Fx_StrCmp((char*)"AMR", pszCodecName))
           {
              *modeset = 7;
           }
           else if(Fx_StrCmp((char*)"AMR-WB", pszCodecName))
           {
              *modeset = 8;
           }
         }
         return true;
      }
      else return false;
   }
	bool IsMultiFrameRate()
	{
		unsigned int ret = 0;
		for(int i = 0;i<flex.GetDOptCnt();i++)
		{if(flex.GetKey(i) == OPT_SDP_ATTRIBUTE_FRAMERATE) ret++;}
		if(ret >= 2) return true;
		else return false;
	}
	/**************** MSRP Parameter Proc ****************/
	char * Find_Msrp_Attribute(FX_IN int m_arr,FX_IN char *pkey){return FindFlex_Add_2(&m_num[0],m_arr, pkey, &flex);}
	void Set_Msrp_Attribute(FX_IN int m_arr,FX_IN char * key_str, FX_IN char * op){Flex_Set(m_arr,0xff,OPT_SDP_ATTRIBUTE_OTHER,key_str,op);	}
	int Get_Msrp_Type(FX_IN int m_arr){return desc[m_arr].Get_Media();}
	int Get_Msrp_SetupMode()
	{
		char *pSetup = Find_Msrp_Attribute(0,(char*)"setup");
		if(Fx_StrStr(pSetup,(char*)"active")) return MSRP_SETUP_ACTIVE;
		else if(Fx_StrStr(pSetup,(char*)"actpass")) return MSRP_SETUP_ACTPASS;
		else if(Fx_StrStr(pSetup,(char*)"passive")) return MSRP_SETUP_PASSIVE;
		return MSRP_SETUP_NONE;	
	}
	char *Get_Msrp_FileSelector(){return Find_Msrp_Attribute(0,(char*)"file-selector");}
	char *Get_Msrp_Path(){return Find_Msrp_Attribute(0,(char*)"path");}
	char *Get_Msrp_FileTransferID(){return Find_Msrp_Attribute(0,(char*)"file-transfer-id");}
	char *Get_Msrp_ContentsType(){return Find_Msrp_Attribute(0,(char*)"accept-types");}
	char *Get_Msrp_WrappedType(){return Find_Msrp_Attribute(0,(char*)"accept-wrapped-types");}
	/************* Visited Realm Parameter Proc **************/
	int Get_Visit_Cnt(FX_IN int desc_idx){return Fx_Atoi(Find_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_VISIT_REALM_CNT));}
	bool Get_Visited(FX_INOUT char *Resource,FX_IN int src_len, FX_IN int visit_idx,FX_IN int desc_idx,
						FX_OUT char **nod, FX_OUT char **domain,
						FX_OUT bool *bV6, FX_OUT char **IP, FX_OUT int *Port,FX_OUT char **net_type)
	{
		char *pVisit = Find_Media_PT(desc_idx,visit_idx,OPT_SDP_ATTRIBUTE_VISIT_REALM);
		if(pVisit == NULL) return false;
		// a=visited-realm:1 abcX.sktims.net IN IP4 192.0.2.1 1000 net-type nat
		st_tok_param mtok;mtok.Init(10,src_len,Resource);
		if(!mtok.Parse(TOK_PARSE_PATTERN,(char*)" .    ",pVisit)) return false;
		if(mtok.pos_cnt < 7) return false;
		*nod = mtok.GetParam(1);*domain = mtok.GetParam(2);
		if(Fx_StrCmp(mtok.GetParam(4),(char*)"IP6")) *bV6 = true;
		else *bV6 = false;
		*IP = mtok.GetParam(5);*Port = Fx_Atoi(mtok.GetParam(6));
		char *nettype = Fx_StrStrNext(mtok.GetParam(6),(char *)"net-type");
		if(nettype)
		{
			unsigned int pcnt = 0;
			*net_type = Fx_Skip_Trim(nettype,(char*)" ",&pcnt);
		}
		return true;
	}
	void Set_Visited(FX_IN int visit_idx, FX_IN int desc_idx, FX_IN char *node,
					FX_IN char *domain, FX_IN bool bV6, FX_IN char *IP, int Port, FX_IN char *net_type)
	{
		if(flex.GetDOptCnt() == MAX_FLEX_OPT_NUM) return;
		SetPTkey(visit_idx);SetMediakey(desc_idx);
		if(Fx_Strlen(net_type)>0)
		{
			flex.Fx_Flex_Add(OPT_SDP_ATTRIBUTE_VISIT_REALM,FX_NULL,
			"%d %s.%s IN %s %s %d net-type %s",visit_idx+1,node,domain,
			FX_TRUE_SELECT(bV6==true,"IP6","IP4"),IP,Port,net_type);
		}
		else
		{
			flex.Fx_Flex_Add(OPT_SDP_ATTRIBUTE_VISIT_REALM,FX_NULL,
			"%d %s.%s IN %s %s %d",visit_idx+1,node,domain,
			FX_TRUE_SELECT(bV6==true,"IP6","IP4"),IP,Port);
		}
		Incre_Visit_Counting(desc_idx);
	}
	void Incre_Visit_Counting(FX_IN int desc_idx)
	{
		int visit_idx = 0;
		if(!Find_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_VISIT_REALM_CNT))
		{
			char visit_cnt[6];visit_cnt[0]=0;Fx_StrCpy(visit_cnt,(char*)"1    ");
			Set_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_VISIT_REALM_CNT,visit_cnt);
		}
		else
		{
			char *visit_cnt = Find_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_VISIT_REALM_CNT);
			visit_idx = Fx_Atoi(visit_cnt);visit_idx++;
			visit_cnt[0]=0; Fx_StrCat(visit_cnt,"%d",visit_idx);
		}
	}
	/**************** OMR Parameter Proc ****************/
	void Set_OMR_M_CkSum(FX_IN int desc_idx,FX_IN char * pcksum){Set_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_OMR_M_CK,pcksum);}
	void Set_OMR_S_CkSum(FX_IN int desc_idx,FX_IN char * pcksum){Set_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_OMR_S_CK,pcksum);}
	char *Find_OMR_M_CkSum(FX_IN int desc_idx){return Find_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_OMR_M_CK);}
	char *Find_OMR_S_CkSum(FX_IN int desc_idx){return Find_Media_1(desc_idx,OPT_SDP_ATTRIBUTE_OMR_S_CK);}
	/********** ImageAttr Payload Parameter Proc ***********/

//input ex)a=imageattr:102 >> signal 11, Segmentation fault

/*
	bool Get_Image_Attr(FX_IN int m_arr,FX_IN int mpt,
							FX_OUT int *s_x,FX_OUT int *s_y,FX_OUT int *r_x,FX_OUT int *r_y )
	{
		 //a=imageattr:<payload type> send [x=320,y=240] recv [x=320,y=240]
		 
		char *strattr = Find_Media_PT_2(m_arr,mpt,(char*)"imageattr");if(strattr==FX_NULL) return false;
		char *send = Fx_StrStrNext(strattr,(char*)"send");char *recv = Fx_StrStrNext(strattr,(char*)"recv");
		char *px = Fx_StrStrNext(send,(char*)"x");char *py = Fx_StrStrNext(send,(char*)"y");
		if(s_x) *s_x = Fx_Atoi(Fx_Skip_Trim(px,(char*)" =", NULL));
		if(s_y) *s_y = Fx_Atoi(Fx_Skip_Trim(py,(char*)" =", NULL));
		px = Fx_StrStrNext(recv,(char*)"x");py = Fx_StrStrNext(recv,(char*)"y");
		if(r_x) *r_x = Fx_Atoi(Fx_Skip_Trim(px,(char*)" =", NULL));
		if(r_y) *r_y = Fx_Atoi(Fx_Skip_Trim(py,(char*)" =", NULL));
		return true;
	}
*/
	int Get_Image_Attr(FX_IN int m_arr,FX_IN int mpt,
							FX_OUT int *s_x,FX_OUT int *s_y,FX_OUT int *r_x,FX_OUT int *r_y )
	{
		 //a=imageattr:<payload type> send [x=320,y=240] recv [x=320,y=240]
		 
		char *strattr = Find_Media_PT_2(m_arr,mpt,(char*)"imageattr"); if(strattr==FX_NULL) return 2;
		char *send = Fx_StrStrNext(strattr,(char*)"send");
      if(send != FX_NULL)
      {
		   char *px = Fx_StrStrNext(send,(char*)"x"); if(px == FX_NULL) return 3;
         char *py = Fx_StrStrNext(send,(char*)"y"); if(py == FX_NULL) return 3;
		   if(s_x) *s_x = Fx_Atoi(Fx_Skip_Trim(px,(char*)" =", NULL));
		   if(s_y) *s_y = Fx_Atoi(Fx_Skip_Trim(py,(char*)" =", NULL));
      }
      else
      {
         return 3;
      }

      char *recv = Fx_StrStrNext(strattr,(char*)"recv");
      if(recv != FX_NULL)
      {
		   char *px = Fx_StrStrNext(recv,(char*)"x"); if(px == FX_NULL) return 3;
         char *py = Fx_StrStrNext(recv,(char*)"y"); if(py == FX_NULL) return 3;
		   if(r_x) *r_x = Fx_Atoi(Fx_Skip_Trim(px,(char*)" =", NULL));
		   if(r_y) *r_y = Fx_Atoi(Fx_Skip_Trim(py,(char*)" =", NULL));
      }
      else
      {
         return 3;
      }
		return 1;
	}
	void Set_Image_Attr(FX_IN int m_arr, FX_IN int mpt,FX_IN int s_x,FX_IN int s_y,FX_IN int r_x,FX_IN int r_y)
	{
		SetMediakey(m_arr);SetPTkey(mpt);
		flex.Fx_Flex_Add(OPT_OTHER,(char*)"imageattr","%u send [x=%u,y=%u] recv [x=%u,y=%u]",mpt,s_x,s_y,r_x,r_y);
	}
	/**************** Debug Functions ****************/
	int valid()
	{
		int i = 0;
		for(i =0;i<m_cnt; i++)
		{
			if(desc[i].media_t == SDPMEDIATYPE_NOTSET) return BUILD_ERR_SDP_MEDIA_TYPE_EMPTY;
			if(desc[i].mode == SDPCONNECTMODE_NOTSET) return BUILD_ERR_SDP_MODE_TYPE_EMPTY;
			if(desc[i].Get_Port() == 0x00) return BUILD_ERR_SDP_PORT_EMPTY;
		}
		for(i=0;i<flex.GetDOptCnt();i++)
		{
			if(((unsigned char)(pt[i]) != 0xff) &&((unsigned char)(m_num[i]) != 0xff))
				return BUILD_ERR_SDP_SUBKEY_OVERLAP;
		}
		return BUILD_ERR_NONE;
	}
	void Set_SDP_Error(FX_IN int m_sdp_error,FX_IN char *strOtherError)
	{
		if(flex.GetDOptCnt() == MAX_FLEX_OPT_NUM) return;
		char Temp[24];Temp[0]=0;Fx_StrCat(Temp,"%u",m_sdp_error);
		if(flex.Find_1(OPT_SDP_ERROR) == FX_NULL)
		{
			SetPTkey(0xff);SetMediakey(0xff);
			if(strOtherError)
			{
				flex.SetOption(OPT_SDP_ERROR,Temp,strOtherError);
			}
			else
			{
				flex.SetOption(OPT_SDP_ERROR,Temp,(char*)opt_keyString(m_sdp_error));
			}	
		}
	}
	char *IsSDP_Error(){return flex.Find_1(OPT_SDP_ERROR);}
	int GetSDP_Error_Code()
	{
		if(IsSDP_Error() == FX_NULL) return 0;
		int i = 0;
		for(i = 0;i<flex.GetDOptCnt();i++)
		{
			if(flex.GetKey(i) == OPT_SDP_ERROR)  break;
		}
		unsigned int Temp_Idx = Fx_Atoi(flex.GetKOption(i));
		if(Temp_Idx <= OPT_SDP_ERROR) return 0;
		return Temp_Idx;
	}
	char *Debug(FX_IN char *strDebug)
	{
		int i = 0;int j = 0;st_addressing m_addr;
		Fx_StrCat(strDebug,"\tRTP> Contents Len %d\n",Getsize());
		Fx_StrCat(strDebug,"\tRTP> session Id : %s, user name : %s\n",Find_Common_SessionID(),
																Find_Common_UserName());
		//added by khd for ipv6
		if( Get_IP() )
		{
			Fx_StrCat(strDebug,"\tRTP> O IP : %s, Media Count : %d\n",
															m_addr.N2A(Get_IP()),Get_Media_Count());
		}
		else
			Fx_StrCat(strDebug,"\tRTP> O IP : %s, Media Count : %d\n",Get_StrOIP(),Get_Media_Count());
		Fx_StrCat(strDebug,"\tRTP> C IP : %s\n",Get_CIP());
		for(i=0;i<Get_Media_Count();i++)
		{
			if(Get_MIP(i)) Fx_StrCat(strDebug,"\tRTP> Media C IP : %s\n",Get_MIP(i));
			//added by khd for ipv6
			if( Get_StrMIP(i) )
			{
				Fx_StrCat(strDebug,"\tRTP> Media IP : %s:%d, Mode : %s\n",Get_StrMIP(i),desc[i].Get_Port(),
																con_mod_str(desc[i].Get_Mode()));
			}
			else
			{
				Fx_StrCat(strDebug,"\tRTP> Media IP : %s:%d, Mode : %s\n",m_addr.N2A(desc[i].Get_IP()),
												desc[i].Get_Port(),con_mod_str(desc[i].Get_Mode()));
			}
			Fx_StrCat(strDebug,"\tRTP> Media type : %s, Proto Type : %s\n",
				sdp_media_str(desc[i].Get_Media()),Sdp_Proto_Str(desc[i].Get_Proto()));
			Fx_StrCat(strDebug,"\tRTP> ptime : %d, FrameRate : %d\n",desc[i].Get_PTime(),
																		desc[i].Get_FrameRate());
			Fx_StrCat(strDebug,"\tRTP> atr cnt : %d\n",desc[i].Get_Attr_Count());
			for(j= 0;j<desc[i].a_cnt; j++)
			{
				Fx_StrCat(strDebug,"\tRTP> payload : %d, codec : %s, samplerate : %d, bitrate : %d\n",
					desc[i].attri[j].Get_Payload(),Find_PT_Codec(i,desc[i].attri[j].Get_Payload()),
					desc[i].attri[j].Get_SampleRate(),desc[i].attri[j].Get_BitRate());
			}
		}
		Fx_StrCat(strDebug,"\tRTP> RTP Pos Len %d, Count %d\n",GetOptLen(),GetDOptCnt());
		for(j =0;j<GetDOptCnt();j++)
		{
			if(GetStringType(j) == STRING_SDP_COMMON)
			{
				Fx_StrCat(strDebug,"\tRTP_Opt> %d,Common K[%s][%s], D[%s]\n"
					,j,opt_keyString(Getkey(j)),GetKOption(j),GetDOption(j));
			}
			else if(GetStringType(j) == STRING_SDP_MEDIA)
			{
				Fx_StrCat(strDebug,"\tRTP_Opt> %d,Media, M[%d], K[%s][%s], D[%s]\n"
					,j,GetMediakey(j),opt_keyString(Getkey(j)),GetKOption(j),GetDOption(j));
			}
			else if(GetStringType(j) == STRING_SDP_PAYLOAD)
			{
				Fx_StrCat(strDebug,"\tRTP_Opt> %d,Payload P[%d], K[%s][%s], D[%s]\n"
					,j,GetPTkey(j),opt_keyString(Getkey(j)),GetKOption(j),GetDOption(j));
			}
		}
		return strDebug;
	}
}st_sdp_com;
/***************** Contents Structures End **********************/

#define BOX_RAW_NUM    98
#define BOX_HEX_NUM     33
#define BOX_RAW "|--------------------------------------------------------------------------------------------------|\r\n"
#define BOX_EMT "|                                                                                                  |\r\n"
inline char *INL_BOXLine(FX_OUT char *Resource,FX_IN bool bStart)
{
	if(bStart) Fx_StrCat(Resource,"\r\n%s",(char*)BOX_RAW);
	else Fx_StrCat(Resource,"%s\r\n",(char*)BOX_RAW);
	return Resource;
}
inline char *INL_BOXDump_Chr(FX_OUT char *Resource,FX_IN void *pRawData,FX_IN int Len)
{
	char *pData = (char*)pRawData;int j=0;int i=0;int k=0;
	Fx_StrCat(Resource,"|");
	for(j=0,i=0;i<Len;i++)
	{
		j = i+1;
		k = j%BOX_RAW_NUM;
		if((k == 0) && (j != 1))
		{
			if(i == Len-1) Fx_StrCat(Resource,"%c|\r\n",(unsigned char)(*(&pData[i])));
			else Fx_StrCat(Resource,"%c|\r\n|",(unsigned char)(*(&pData[i])));
		}
		else 
		{
			Fx_StrCat(Resource,"%c",(unsigned char)(*(&pData[i])));
		}
	}
	if(k!= 0)
	{
		while(1)
		{
			k = j%BOX_RAW_NUM;
			if(k==0)
			{
				Fx_StrCat(Resource,"|\r\n");break;
			}
			else Fx_StrCat(Resource," ");
			j++;	
		}
	}
	return Resource;
}
inline char *INL_BOXDump_Hex(FX_OUT char *Resource,FX_IN void *pRawData,FX_IN int Len)
{
	char *pData = (char*)pRawData;int j=0;int i=0;int k=0;
	Fx_StrCat(Resource,"|");
	for(j=0,i=0;i<Len;i++)
	{
		j = i+1;
		k = j%BOX_HEX_NUM;
		if((k == 0) && (j != 1))
		{
			if(i == Len-1) Fx_StrCat(Resource,"%02x|\r\n",(unsigned char)(*(&pData[i])));
			else Fx_StrCat(Resource,"%02x|\r\n|",(unsigned char)(*(&pData[i])));
		}
		else 
		{
			Fx_StrCat(Resource,"%02x ",(unsigned char)(*(&pData[i])));
		}
	}
	j = k;
	if(k!= 0)
	{
		while(1)
		{
			j++;	
			k = j%BOX_HEX_NUM;
			if(k==0)
			{
				Fx_StrCat(Resource,"  |\r\n");break;
			}
			else Fx_StrCat(Resource,"   ");
			
		}
	}
	return Resource;
}

#endif 

