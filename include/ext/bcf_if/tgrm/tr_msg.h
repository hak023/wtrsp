/**
 * @file  tr_msg.h
 * the tgrm message type
 *
 * @attention Copyright (c) 2010 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point
 * @par Last Changed : 
 * @par        2013.01.21   paul           first written
**/

#ifndef __SVC_TR_MESSAGE_TYPE__
#define __SVC_TR_MESSAGE_TYPE__

#include "common_msg.h"
#include "tr_err.h"
#include "if_com.h"
//#include <linux/in6.h>
#include <arpa/inet.h>

namespace TGRM
{

// kernel data structure 
#define MAX_TR_PORT  0xFFFF
#define MAX_TR_LOC_ADDR 10
#define MAX_RECOVER_SES 1
#define TR_PORT_PER_RELAYSES  4

////////////////////////////////////////////////////////////////////////////////

// type
enum TR_MSG_TYPE
{
	TR_MSG_NONE      = 0x0000,     // none
	TR_INTERNAL      = 0x0001,     // reserved for Internal message(don't use)

	TR_ADD_REQ       = 0x0002,     // add request   
	TR_ADD_RES       = 0x0003,     // add response
	TR_MOD_REQ       = 0x0004,     // mod request
	TR_MOD_RES       = 0x0005,     // mod response
	TR_DEL_REQ       = 0x0006,     // del request
	TR_DEL_RES       = 0x0007,     // del response
	TR_CLOSED_SES    = 0x0008,     // closed session
	TR_TC_MOD_REQ    = 0x0009,     // mod tc request
	TR_TC_MOD_RES    = 0x000A,     // mod tc response

	TR_MM_BLOCK      = 0x0030,     // Request & Response..not implement
	TR_MM_UNBLOCK    = 0x0031,     // Request & Response..not implement
	TR_MM_RESET      = 0x0032,     // Request & Response..
	TR_MM_CLEAR      = 0x0034,     // Request & Response..clear media flush

	TR_NETFAIL       = 0x0040,     // Info only..
	TR_ALIVE_CHECK   = 0x0FFF,     // Session Alive
};

// sub-type
enum TR_MSG_STYPE
{
	TR_INFO = 0x00,
   TR_REQ  = 0x01,
	TR_RES  = 0x02,
};

////////////////////////////////////////////////////////////////////////////////

struct tr_enc_inf
{
	unsigned short auth_alg; // disable:0 sha1:1
	unsigned short enc_alg; // disable:0 aes:1

	char enc_key[128];

	//ordering
	void hton()
	{
		auth_alg = htons(auth_alg);
		enc_alg  = htons(enc_alg);
	}   
	void ntoh()
	{
		auth_alg = ntohs(auth_alg);
		enc_alg  = ntohs(enc_alg);
	}

	char* str(char* pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "auth[alg:%d] enc[alg:%d key:%s]", 
		   auth_alg, enc_alg, enc_key);
		return pbuf;
	}
};

//ip address 
struct tr_addr
{
	unsigned int  ver;
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
		snprintf(pbuf, nlen, "ver:%d ip:%s", ver, szAddr);
		return pbuf;
	}

	char* conf_str(char* pbuf, int nlen)
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
		snprintf(pbuf, nlen, "%s", szAddr);
		return pbuf;
	}
};

struct tr_init_addr
{
#if 0
	char szNIC[64];
	unsigned int  ver;
	union
	{
	   struct     in_addr  v4;
      struct     in6_addr v6;
   };
#endif
	st_init_addr addr;

	struct tr_enc_inf enc_info;

	//ordering
	void hton()
	{
		addr.hton();
		enc_info.hton();
	}   
	void ntoh()
	{
		addr.ntoh();
		enc_info.ntoh();
	}

	char* str(char* pbuf, int nlen)
	{
		char szAddr[64]; szAddr[0] = '\0';
		char szEnc[1024]; szEnc[0] = '\0';

		addr.str(szAddr, sizeof(szAddr));
		enc_info.str(szEnc, sizeof(szEnc));

		snprintf(pbuf, nlen, "%s enc:%s", szAddr, szEnc);
		return pbuf;
	}
};

// rtp information
#if 0
struct tr_rtp_inf
{
   unsigned char   pkt_type; // 0x10:rtp, 0x11:rtp_audio, 0x12:rtp_video, 0x20:rtcp, 0x21:rtcp audio, 0x22:rtcp video.
   unsigned char   pt; 
	unsigned char   pt_dtmf;
	unsigned char   ucReserved;

	unsigned char   chk_pt;    // checking in-pt
	unsigned char   chk_ssrc;  // checking in-ssrc
	unsigned char   chg_pt;    // convet  pt  in -> out
	unsigned char   chg_ssrc;  // convet  ssrc  in -> out   

   unsigned int    ssrc;
   
	char            use_dscp;
	char            dscp_value; // value

	//ordering
	void hton()
	{
		ssrc     = htonl(ssrc);
	}   
	void ntoh()
	{
		ssrc     = ntohl(ssrc);
	}

	char* str(char *pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "chk(pt:%d ssrc:%d) chg(pt:%d ssrc:%d) type:0x%x pt:%d dtmf:%d ssrc:%d use_dscp:%d dscp_value:%d",
			chk_pt, chk_ssrc, chg_pt, chg_ssrc,	pkt_type, pt, pt_dtmf, ssrc, use_dscp, dscp_value);
		return pbuf;
	}
};
#endif
#if 1

#define MAX_TR_PT        8
#define MAX_TR_PT_DTMF   4

//R132 REDROCK
struct tr_audio_inf
{
	enum eCodecID
	{
		E_NOTDEF=0,
		E_AMR_NB,
		E_AMR_WB,
		E_PCMA,
		E_PCMU,
		E_TELEPHONE_EVENT
	};
	unsigned int uiPTime;		//PlayTime per frame, PCM:10ms, AMR:20ms
	unsigned int uiCodecID;    // eCodecID enum number.
	unsigned int uiSampleRate;	// AMR-WB is 16000, else codec is 8000.
	unsigned int uiModeSet;		// AMR(7), AMR-WB(8).
	unsigned int uiPacketMode;	// 1: octet-align or notset, 0: bandwidth mode.
	int getMediaCodec(char* szCodecName)
	{
		if(szCodecName == NULL)	return E_NOTDEF;
		if(strlen(szCodecName) == 0) return E_NOTDEF;

		if(strcasecmp(szCodecName, "AMR") == 0) return E_AMR_NB;
		else if(strcasecmp(szCodecName, "AMR-WB") == 0) return E_AMR_WB;
		else if(strcasecmp(szCodecName, "PCMU") == 0) return E_PCMU;
		else if(strcasecmp(szCodecName, "PCMA") == 0) return E_PCMA;
		else if(strcasecmp(szCodecName, "telephone-event") == 0) return E_TELEPHONE_EVENT;
		else return E_NOTDEF;
	}
	char * getMediaCodec(int uCodecID)
	{
		if(uCodecID == E_AMR_NB) return (char*)"AMR-NB";
		else if(uCodecID == E_AMR_WB) return  (char*)"AMR-WB";
		else if(uCodecID == E_PCMU) return  (char*)"PCMU";
		else if(uCodecID == E_PCMA) return  (char*)"PCMA";
		else if(uCodecID == E_TELEPHONE_EVENT) return  (char*)"TELEPHONE_EVENT";
		else return  (char*)"UNKNOWN";
	}
	void hton()
	{
		uiPTime = htonl(uiPTime);
		uiCodecID = htonl(uiCodecID);
		uiSampleRate = htonl(uiSampleRate);
		uiModeSet = htonl(uiModeSet);
		uiPacketMode = htonl(uiPacketMode);
	}
	void ntoh()
	{
		uiPTime = ntohl(uiPTime);
		uiCodecID = ntohl(uiCodecID);
		uiSampleRate = ntohl(uiSampleRate);
		uiModeSet = ntohl(uiModeSet);
		uiPacketMode = ntohl(uiPacketMode);
	}
	char * str(char *pbuf,int nlen)
	{
		snprintf(pbuf, nlen, "ptime(%d) codecid(%s) samplerate(%d) modeset(%d) packetmode(%d)",
				uiPTime, getMediaCodec(uiCodecID), uiSampleRate, uiModeSet, uiPacketMode);
		return pbuf;
	}
};

struct tr_media_ses1
{
	unsigned int enable;
	unsigned int uiTCSessionID;//transcode session id base:1~ reserved by TGRM
	tr_audio_inf amedia;

	char * str(char *pbuf,int nlen)
	{
		char szAmedia[1024]; szAmedia[0] = '\0';
		if(enable)
		{
			amedia.str(szAmedia, sizeof(szAmedia));
			snprintf(pbuf, nlen, "enable(%d) tcid(%d) amedia(%s)", enable, uiTCSessionID, szAmedia);
		}
		else
		{
			snprintf(pbuf, nlen, "enable(%d)", enable);
		}
		return pbuf;
	}
};
//R132 END

struct tr_rtp_inf
{
	enum {
		CHK_PT = 0x1,
      CHK_SSRC = 0x2,
		CHK_SEQ = 0x4,
		CHK_TS = 0x8
	};
	enum {
		CHG_PT = 0x1,
		CHG_SSRC = 0x2
	};
   unsigned char   pkt_type; // 0x10:rtp, 0x11:rtp_audio, 0x12:rtp_video, 0x20:rtcp, 0x21:rtcp audio, 0x22:rtcp video.
	unsigned char   pt_cnt;
	unsigned char   pt_dtmf_cnt;
   unsigned char   pt[MAX_TR_PT]; 
	unsigned char   pt_dtmf[MAX_TR_PT_DTMF];
	unsigned char   ucReserved;

	unsigned int    chk_flag;
   unsigned int    chg_flag;
   unsigned int    ssrc;
	char            use_dscp;
	char            dscp_value; // value
	char            cReserved[2];

	//ordering
	void hton()
	{
		ssrc     = htonl(ssrc);
	}   
	void ntoh()
	{
		ssrc     = ntohl(ssrc);
	}

	char* str(char *pbuf, int nlen)
	{
		char szchk[128];szchk[0]='\0';
		char szchg[128];szchg[0]='\0';
		char szpt[128];szpt[0]='\0';
		char szpt_dtmf[128];szpt_dtmf[0]='\0';

		if((chk_flag & CHK_PT) == CHK_PT)
			strcat(szchk,"pt ");
		if((chk_flag & CHK_SSRC) == CHK_SSRC)
			strcat(szchk,"ssrc ");
		if((chk_flag & CHK_SEQ) == CHK_SEQ)
			strcat(szchk,"seq ");
		if((chk_flag & CHK_TS) == CHK_TS)
			strcat(szchk,"ts ");
		
		if((chg_flag & CHG_PT) == CHG_PT)
			strcat(szchg,"pt ");
		if((chg_flag & CHG_SSRC) == CHG_SSRC)
			strcat(szchg,"ssrc ");

		for(int i=0;i<pt_cnt;i++)
		{
			if (i > 0) snprintf(szpt+strlen(szpt), sizeof(szpt), " ");
			snprintf(szpt+strlen(szpt), sizeof(szpt), "%d", pt[i]);
		}
		for(int i=0;i<pt_dtmf_cnt;i++)
		{
			if (i > 0) snprintf(szpt+strlen(szpt_dtmf), sizeof(szpt_dtmf), " ");
			snprintf(szpt_dtmf+strlen(szpt_dtmf), sizeof(szpt_dtmf), "%d", pt_dtmf[i]);
		}

		snprintf(pbuf, nlen, "chk(%s) chg(%s) type:0x%x pt(%s) dtmf(%s) ssrc:%d dscp(use:%d val:%d)",
			szchk,szchg,pkt_type, szpt, szpt_dtmf, ssrc, use_dscp, dscp_value);
		return pbuf;
	}
};
#endif
// session qos information
struct tr_qos_inf 
{
	unsigned int        max_bps;   // bit per sec  
	unsigned int        max_pps;   // packet per sec

	//ordering
	void hton()
	{
		max_bps =  htonl(max_bps);
		max_pps =  htonl(max_pps);
	}   
	void ntoh()
	{
		max_bps = ntohl(max_bps);
		max_pps = ntohl(max_pps);
	}
	char* str(char* pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "[max(bps:%d pps:%d)]",
		   max_bps, max_pps);
		return pbuf;
	}
};

// session status
struct tr_ses_stat
{
   unsigned int stime;              // set relay on time
   unsigned int mtime;              // set update(modify) time
   unsigned int ltime;              // last received pkt 

   unsigned int pkts_in;            // recv packet count
   unsigned int bytes_in;           // rtp payload len 
   unsigned int pkts_out;           // send packet count 
   unsigned int bytes_out;          // rtp payload len 

   unsigned int load_in;            // cur load in
   unsigned int load_out;           // cur load out 
   unsigned int peakload_in;        // peak cur load in
   unsigned int peakload_out;       // peak cur load route

   unsigned int drops;   // drop pkt count
	// ordering
	void hton()
	{
		stime        =  htonl(stime);
		mtime        =  htonl(mtime);
		ltime        =  htonl(ltime);
		pkts_in      =  htonl(pkts_in);
		bytes_in     =  htonl(bytes_in);
		load_in      =  htonl(load_in);
		peakload_in  =  htonl(peakload_in);
		pkts_out     =  htonl(pkts_out);
		bytes_out    =  htonl(bytes_out);
		load_out     =  htonl(load_out);
		peakload_out =  htonl(peakload_out);
		drops        =  htonl(drops);
	}   
	void ntoh()
	{
		stime        =  ntohl(stime);
		mtime        =  ntohl(mtime);
		ltime        =  ntohl(ltime);
		pkts_in      =  ntohl(pkts_in);
		bytes_in     =  ntohl(bytes_in);
		load_in      =  ntohl(load_in);
		peakload_in  =  ntohl(peakload_in);
		pkts_out     =  ntohl(pkts_out);
		bytes_out    =  ntohl(bytes_out);
		load_out     =  ntohl(load_out);
		peakload_out =  ntohl(peakload_out);
		drops        =  ntohl(drops);
	}
	char* str(char* pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "time(s:%d m:%d l:%d) in(pkts:%d bytes:%d) out(pkts:%d bytes:%d) drops:%d)",
			stime, mtime, ltime, pkts_in, bytes_in, pkts_out, bytes_out, drops);
		return pbuf;
	}
};

//struct tr_ses_inf
struct tr_rtp_ses1
{
	unsigned char        enable;
	unsigned char        reserved[5];
	unsigned short       port; // add res

	struct tr_addr       ip;   // add res
	struct tr_rtp_inf    rtp;  // add req // in : check / drop , out : replace 

	//ordering
	void hton()
	{
		port = htons(port);
		ip.hton();
		rtp.hton();
	}   
	void ntoh()
	{
		port = ntohs(port);
		ip.ntoh();
		rtp.ntoh();
	}

	char* str(char* pbuf, int nlen)
	{
		char szAddr[64]; szAddr[0] = '\0';
		char szRtpInf[128]; szRtpInf[0] = '\0';

		if (enable == 0)
		{
			snprintf(pbuf, nlen, "disable");
		}
		else
		{
			ip.str(szAddr, sizeof(szAddr));
			rtp.str(szRtpInf, sizeof(szRtpInf));

			snprintf(pbuf, nlen, "en:%d port:%d addr(%s) inf(%s)",
				enable, port, szAddr, szRtpInf);
		}

		return pbuf;
	}

};

struct tr_cfg_inf
{
   unsigned char  ucChkIPPort;
	unsigned char  ucUseRemoteNAT;
	unsigned char  ucUseSRTP;
	unsigned char  ucReserved;  
	struct tr_enc_inf		enc;

	void hton()
	{
		enc.hton();
	}   
	void ntoh()
	{
		enc.ntoh();
   }

	char* str(char* pbuf, int nlen)
	{
		char szEnc[1024];szEnc[0]='\0';
		enc.str(szEnc,1024);
		snprintf(pbuf, nlen, "NAT:%d Chk IP/PORT:%d SRTP:%d ENC:%s", 
		   ucUseRemoteNAT, ucChkIPPort, ucUseSRTP,szEnc);
		return pbuf;
	}
};

struct tr_rtp_ses4  // 4rtp session
{
   struct tr_rtp_ses1 artp;
   struct tr_rtp_ses1 artcp;
   struct tr_rtp_ses1 vrtp;
   struct tr_rtp_ses1 vrtcp;

	void hton()
	{
		artp.hton();
		artcp.hton();
		vrtp.hton();
		vrtcp.hton();
	}   
	void ntoh()
	{
		artp.ntoh();
		artcp.ntoh();
		vrtp.ntoh();
		vrtcp.ntoh();
   }

	char* str(char* pbuf, int nlen)
	{
		char szARtp[1024]; szARtp[0]='\0';
		char szARtcp[1024]; szARtcp[0]='\0';
		char szVRtp[1024]; szVRtp[0]='\0';
		char szVRtcp[1024]; szVRtcp[0]='\0';

		artp.str(szARtp, sizeof(szARtp));
		artcp.str(szARtcp, sizeof(szARtcp));
		vrtp.str(szVRtp, sizeof(szVRtp));
		vrtcp.str(szVRtcp, sizeof(szVRtcp));

		snprintf(pbuf, nlen, "artp[%s] artcp[%s] vrtp[%s] vrtcp[%s]", 
			szARtp, szARtcp, szVRtp, szVRtcp);
		return pbuf;
	}
};

// 1 rtp relay session
struct tr_rel_ses1
{
	unsigned short        loc_sport;  // add res
	unsigned short        loc_dport;  // add res

	struct tr_cfg_inf     caller_cfg;
	struct tr_cfg_inf     called_cfg;

   struct tr_rtp_ses1    ssdp;       // add req, caller sdp
   struct tr_rtp_ses1    dsdp;       // mod req, called sdp
	struct tr_qos_inf     qos;
	//ordering
	void hton()
	{
		loc_sport = htons(loc_sport);
		loc_dport = htons(loc_dport);
		caller_cfg.hton();
		called_cfg.hton();
		ssdp.hton();
		dsdp.hton();
		qos.hton();
	}   
	void ntoh()
	{
		loc_sport = ntohs(loc_sport);
		loc_dport = ntohs(loc_dport);
		caller_cfg.ntoh();
		called_cfg.ntoh();
		ssdp.ntoh();
		dsdp.ntoh();
		qos.ntoh();
	}
	char* str(char* pbuf, int nlen)
	{
		char szSsdp[1024]; szSsdp[0]='\0';
		char szDsdp[1024]; szDsdp[0]='\0';
		char szQos[256]; szQos[0]='\0';
		char szCallerCfg[128]; szCallerCfg[0]='\0';
		char szCalledCfg[128]; szCalledCfg[0]='\0';
		ssdp.str(szSsdp, sizeof(szSsdp));
		dsdp.str(szDsdp, sizeof(szDsdp));
		qos.str(szQos, sizeof(szQos));
		caller_cfg.str(szCallerCfg, sizeof(szCallerCfg));
		called_cfg.str(szCalledCfg, sizeof(szCalledCfg));

		snprintf(pbuf, nlen, "loc_sport:%d loc_dport:%d ssdp[%s] dsdp[%s] qos[%s] cr_cfg[%s], cd_cfg[%s]", 
			loc_sport, loc_dport, szSsdp, szDsdp, szQos, szCallerCfg, szCalledCfg);
		return pbuf;
	}
};

// RM bound
// 4 rtp relay session info (a/v rtp & rtcp)
struct tr_rel_ses4
{
	//int    caller_trte;  // caller area id
	//int    called_trte;  // called area id
	int    sid;  // caller pool index
	int    did;  // caller pool index

   struct tr_rel_ses1   artp;
   struct tr_rel_ses1   artcp;
   struct tr_rel_ses1   vrtp;
   struct tr_rel_ses1   vrtcp;
// R132PKG REDROCK
	struct tr_media_ses1 smedia;
	struct tr_media_ses1 dmedia;
// R132PKG END

	void hton()
	{
		sid = htonl(sid);
		did = htonl(did);

		artp.hton();
		artcp.hton();
		vrtp.hton();
		vrtcp.hton();
	}   
	void ntoh()
	{
		sid = ntohl(sid);
		did = ntohl(did);

		artp.ntoh();
		artcp.ntoh();
		vrtp.ntoh();
		vrtcp.ntoh();
	}   
	char* str(char* pbuf, int nlen, bool bPrintRtcp = true)
	{
		char szARtp[1024]; szARtp[0]='\0';
		char szARtcp[1024]; szARtcp[0]='\0';
		char szVRtp[1024]; szVRtp[0]='\0';
		char szVRtcp[1024]; szVRtcp[0]='\0';
		char szSmedia[1024]; szSmedia[0]='\0';
		char szDmedia[1024]; szDmedia[0]='\0';

		artp.str(szARtp, sizeof(szARtp));
		artcp.str(szARtcp, sizeof(szARtcp));
		vrtp.str(szVRtp, sizeof(szVRtp));
		vrtcp.str(szVRtcp, sizeof(szVRtcp));
		smedia.str(szSmedia, sizeof(szSmedia));
		dmedia.str(szDmedia, sizeof(szDmedia));

		if (bPrintRtcp)
		{
			snprintf(pbuf, nlen, "sid:%d did:%d artp[%s] artcp[%s] vrtp[%s] vrtcp[%s] smedia[%s] dmedia[%s]",
				sid, did, szARtp, szARtcp, szVRtp, szVRtcp, szSmedia, szDmedia);
		}
		else
		{
			snprintf(pbuf, nlen, "sid:%d did:%d artp[%s] vrtp[%s] smedia[%s] dmedia[%s]",
				sid, did, szARtp, szVRtp, szSmedia, szDmedia);
		}
		return pbuf;
	}
};

struct tr_ses_stat4
{
	unsigned char   ucArtpEnable;
	unsigned char   ucArtcpEnable;
	unsigned char   ucVrtpEnable;
	unsigned char   ucVrtcpEnable;
	unsigned char   ucReserved[4];
	struct tr_ses_stat   artp_stat;
	struct tr_ses_stat   artcp_stat;
	struct tr_ses_stat   vrtp_stat;
	struct tr_ses_stat   vrtcp_stat;
	//ordering
	void hton()
	{
		artp_stat.hton();
		artcp_stat.hton();
		vrtp_stat.hton();
		vrtcp_stat.hton();
	}   
	void ntoh()
	{
		artp_stat.ntoh();
		artcp_stat.ntoh();
		vrtp_stat.ntoh();
		vrtcp_stat.ntoh();
	}
	char* str(char* pbuf, int nlen)
	{
	   char szArtp[256]; szArtp[0]='\0';
	   char szArtcp[256]; szArtcp[0]='\0';
	   char szVrtp[256]; szVrtp[0]='\0';
	   char szVrtcp[256]; szVrtcp[0]='\0';

		if (ucArtpEnable)
         artp_stat.str(szArtp, sizeof(szArtp));
		else
			snprintf(szArtp, sizeof(szArtp), "disable");
		
		if (ucArtcpEnable)
         artcp_stat.str(szArtcp, sizeof(szArtcp));
		else
			snprintf(szArtcp, sizeof(szArtcp), "disable");

		if (ucVrtpEnable)
         vrtp_stat.str(szVrtp, sizeof(szVrtp));
		else
			snprintf(szVrtp, sizeof(szVrtp), "disable");

      if (ucVrtcpEnable)
         vrtcp_stat.str(szVrtcp, sizeof(szVrtcp));
		else
			snprintf(szVrtcp, sizeof(szVrtcp), "disable");

		snprintf(pbuf, nlen, "[artp:%s] [artcp:%s] [vrtp:%s] [vrtcp:%s]", 
			szArtp, szArtcp, szVrtp, szVrtcp);
		return pbuf;
	}
};

typedef struct tr_relay_stat
{
	int board; 
	int caller_trte; 
	int called_trte; 
	int reserved; 

	struct tr_ses_stat4  src_stat;
	struct tr_ses_stat4  dst_stat;

	//ordering
	void hton()
	{
		board = htonl(board);
		caller_trte = htonl(caller_trte);
		called_trte = htonl(called_trte);
		src_stat.hton();
		dst_stat.hton();
	}   
	void ntoh()
	{
		board = ntohl(board);
		caller_trte = ntohl(caller_trte);
		called_trte = ntohl(called_trte);
		src_stat.ntoh();
		dst_stat.ntoh();
	}
	char* str(char* pbuf, int nlen)
	{
		char szSstat[2048]; szSstat[0]='\0';
		char szDstat[2048]; szDstat[0]='\0';
		src_stat.str(szSstat, sizeof(szSstat));
		dst_stat.str(szDstat, sizeof(szDstat));

		snprintf(pbuf, nlen, "board:%d, caller_trte:%d, called_trte:%d src_stat[%s] dst_stat[%s]", 
		   board, caller_trte, called_trte, szSstat, szDstat);
		return pbuf;
	}
}tr_del_res, tr_alive_check_res;

struct tr_session 
{
	unsigned int         uiboard;
	unsigned int         uitrsesid;
	
   struct tr_rel_ses4   rel_ses;
	struct tr_rel_ses4   tc_ses;
	struct SVCTrace_t    stTrace;
	struct tr_ses_stat4  sstat;
	struct tr_ses_stat4  dstat;

	//ordering
	void hton()
	{
		uiboard   = htonl(uiboard);
		uitrsesid = htonl(uitrsesid);
		rel_ses.hton();
		tc_ses.hton();
		stTrace.hton();
		sstat.hton();
		dstat.hton();
	}   
	void ntoh()
	{
		uiboard   = ntohl(uiboard);
		uitrsesid = ntohl(uitrsesid);
		rel_ses.ntoh();
		tc_ses.ntoh();
		stTrace.ntoh();
		sstat.ntoh();
		dstat.ntoh();
	}
	char* str(char* pbuf, int nlen)
	{
		char szTrSes[2048]; szTrSes[0]='\0';
		char szTcSes[2048]; szTcSes[0]='\0';
		char szTrace[1024]; szTrace[0]='\0';
		char szSstat[2048]; szSstat[0]='\0';
		char szDstat[2048]; szDstat[0]='\0';

		rel_ses.str(szTrSes, sizeof(szTrSes));
		tc_ses.str(szTcSes, sizeof(szTcSes));
		stTrace.str(szTrace, sizeof(szTrace));
		sstat.str(szSstat, sizeof(szSstat));
		dstat.str(szDstat, sizeof(szDstat));

		snprintf(pbuf, nlen, "B:%d S:%d ses[%s] tc[%s] sstat[%s] dstat[%s]", 
			uiboard, uitrsesid, szTrSes, szTcSes, szSstat, szDstat);
		return pbuf;
	}
};

// add req & mod req
struct tr_add_req
{
   enum { e_max_min = 40 };
	enum { e_dir_src = 1, e_dir_dst = 2 };

	char                 szcaller[e_max_min];	 // add req
	char                 szcalled[e_max_min];  // add req

	int                  caller_trte;          // caller area id
	int                  called_trte;          // callee area id

	unsigned int         tgt_dir;              // add & mod req // require id = 1:src, 2:dst
	unsigned char        ucIPVer;
	unsigned char        ucEnableAudio;
	unsigned char        ucEnableVideo;
	unsigned char        ucReserved;

	SVCTrace_t           Trace;

	//ordering
	void hton()
	{
		caller_trte = htonl(caller_trte);
		called_trte = htonl(called_trte);
		tgt_dir  = htonl(tgt_dir);
		Trace.hton();
	}   
	void ntoh()
	{
		caller_trte = ntohl(caller_trte);
		called_trte = ntohl(called_trte);
		tgt_dir = ntohl(tgt_dir);
		Trace.ntoh();
	}

	char* str(char *pbuf, int nlen)
	{
		char szRtp[2048]; szRtp[0] = '\0';
		char szTgtDir[16]; szTgtDir[0] = '\0';
		char szTrace[1024]; szTrace[1024] = '\0';

		Trace.str(szTrace, sizeof(szTrace));

		switch(tgt_dir)
		{
			case e_dir_src: snprintf(szTgtDir, sizeof(szTgtDir), "src"); break;
			case e_dir_dst: snprintf(szTgtDir, sizeof(szTgtDir), "dst"); break;
			default: snprintf(szTgtDir, sizeof(szTgtDir), "unknown"); break;
		}

		snprintf(pbuf, nlen, 
			"cr:%s cd:%s trte(cr:%d cd:%d) enable(a:%d v:%d) tgt_dir:%s ipver:%d szTrace{%s}",
			szcaller, szcalled, caller_trte, called_trte, ucEnableAudio, ucEnableVideo, szTgtDir, ucIPVer, szTrace);

		return pbuf;
	}

};

struct tr_mod_req
{
   enum { e_max_min = 40 };
	enum { e_dir_src = 1, e_dir_dst = 2 };

	char                 szcaller[e_max_min];	 // add req
	char                 szcalled[e_max_min];  // add req

	int                  caller_trte;          // caller area id
	int                  called_trte;          // callee area id

	unsigned int         tgt_dir;              // add & mod req // require id = 1:src, 2:dst
	unsigned char        ucIPVer;
	unsigned char        ucReserved;
	unsigned char        ucEnableAudio;
	unsigned char        ucEnableVideo;

	struct tr_rtp_ses4   caller_ses;           // target node rtp sdp(4 session) 
	struct tr_rtp_ses4   called_ses;           // target node rtp sdp(4 session) 

   struct tr_cfg_inf    caller_cfg;
   struct tr_cfg_inf    called_cfg;

	struct tr_qos_inf    artpqos;
	struct tr_qos_inf    artcpqos;
	struct tr_qos_inf    vrtpqos;
	struct tr_qos_inf    vrtcpqos;
	
	//ordering
	void hton()
	{
		caller_trte = htonl(caller_trte);
		called_trte = htonl(called_trte);
		tgt_dir  = htonl(tgt_dir);
		caller_ses.hton();
		called_ses.hton();
		caller_cfg.hton();
		called_cfg.hton();
		artpqos.hton();
		artcpqos.hton();
		vrtpqos.hton();
		vrtcpqos.hton();
	}   
	void ntoh()
	{
		caller_trte = ntohl(caller_trte);
		called_trte = ntohl(called_trte);
		tgt_dir = ntohl(tgt_dir);
		caller_ses.ntoh();
		called_ses.ntoh();
		caller_cfg.ntoh();
		called_cfg.ntoh();
		artpqos.ntoh();
		artcpqos.ntoh();
		vrtpqos.ntoh();
		vrtcpqos.ntoh();
	}

	char* str(char *pbuf, int nlen)
	{
		char szCallerRtp[1024]; szCallerRtp[0] = '\0';
		char szCalledRtp[1024]; szCallerRtp[0] = '\0';
		char szTgtDir[16]; szTgtDir[0] = '\0';
		char szArtpQos[32]; szArtpQos[0] = '\0';
		char szArtcpQos[32]; szArtcpQos[0] = '\0';
		char szVrtpQos[32]; szVrtpQos[0] = '\0';
		char szVrtcpQos[32]; szVrtcpQos[0] = '\0';
		char szCallerCfg[32]; szCallerCfg[0] = '\0';
		char szCalledCfg[32]; szCalledCfg[0] = '\0';

		caller_ses.str(szCallerRtp, sizeof(szCallerRtp));
		called_ses.str(szCalledRtp, sizeof(szCalledRtp));
		artpqos.str(szArtpQos, sizeof(szArtpQos));
		artcpqos.str(szArtcpQos, sizeof(szArtcpQos));
		vrtpqos.str(szVrtpQos, sizeof(szVrtpQos));
		vrtcpqos.str(szVrtcpQos, sizeof(szVrtcpQos));
		caller_cfg.str(szCallerCfg, sizeof(szCallerCfg));
		called_cfg.str(szCalledCfg, sizeof(szCalledCfg));

		switch(tgt_dir)
		{
			case e_dir_src: snprintf(szTgtDir, sizeof(szTgtDir), "src"); break;
			case e_dir_dst: snprintf(szTgtDir, sizeof(szTgtDir), "dst"); break;
			default: snprintf(szTgtDir, sizeof(szTgtDir), "unknown"); break;
		}

		snprintf(pbuf, nlen, 
			"cr:%s cd:%s trte(cr:%d cd:%d) enable(a:%d v:%d) tgt_dir:%s ipver:%d cr_ses{%s} cd_ses{%s} Qos[Artp:%s, Artcp:%s, Vrtp:%s, Vrtcp:%s], cr_cfg[%s], cd_cfg[%s]",
			szcaller, szcalled, caller_trte, called_trte, ucEnableAudio, ucEnableVideo, 
			   szTgtDir, ucIPVer, szCallerRtp, szCalledRtp, szArtpQos, szArtcpQos, szVrtpQos, szVrtcpQos,
			      szCallerCfg, szCalledCfg);

		return pbuf;
	}

};

struct tr_tc_mod_req
{
   struct tr_mod_req  ses;

   unsigned char ucAudioTC;
   unsigned char ucVideoTC;
   unsigned char ucReserved[8];

	struct tr_addr     tc_cr_a_ip;
	struct tr_addr     tc_cr_v_ip;
	struct tr_addr     tc_cd_a_ip;
	struct tr_addr     tc_cd_v_ip;

	unsigned int       tc_cr_a_port;
	unsigned int       tc_cr_v_port;
	unsigned int       tc_cd_a_port;
	unsigned int       tc_cd_v_port;

	//ordering
	void hton()
	{
		ses.hton();
      tc_cr_a_ip.hton();
      tc_cr_v_ip.hton();
      tc_cd_a_ip.hton();
      tc_cd_v_ip.hton();
		tc_cr_a_port = htonl(tc_cr_a_port);
		tc_cr_v_port = htonl(tc_cr_v_port);
		tc_cd_a_port = htonl(tc_cd_a_port);
		tc_cd_v_port = htonl(tc_cd_v_port);
	}   
	void ntoh()
	{
		ses.hton();
      tc_cr_a_ip.hton();
      tc_cr_v_ip.hton();
      tc_cd_a_ip.hton();
      tc_cd_v_ip.hton();
		tc_cr_a_port = htonl(tc_cr_a_port);
		tc_cr_v_port = htonl(tc_cr_v_port);
		tc_cd_a_port = htonl(tc_cd_a_port);
		tc_cd_v_port = htonl(tc_cd_v_port);
	}

	char* str(char *pbuf, int nlen)
	{
		char szSes[3072]; szSes[0] = '\0';
		char szCrA[128]; szCrA[0] = '\0';
		char szCrV[128]; szCrV[0] = '\0';
		char szCdA[128]; szCdA[0] = '\0';
		char szCdV[128]; szCdV[0] = '\0';

		ses.str(szSes, sizeof(szSes));
		tc_cr_a_ip.str(szCrA, sizeof(szCrA));
		tc_cr_v_ip.str(szCrV, sizeof(szCrV));
		tc_cd_a_ip.str(szCdA, sizeof(szCdA));
		tc_cd_v_ip.str(szCdV, sizeof(szCdV));

		snprintf(pbuf, nlen, 
			"[ses:%s] [tc_cr a:%s:%u v:%s:%u] [tc_cd a:%s:%u v:%s:%u]", 
			   szSes, szCrA, tc_cr_a_port, szCrV, tc_cr_v_port, 
            szCdA, tc_cd_a_port, szCdV, tc_cd_v_port);

		return pbuf;
	}
};

struct tr_res
{
   enum { e_max_min = 40 };

	char             szcaller[e_max_min];	 // add req
	char             szcalled[e_max_min];   // add req

	int                  caller_trte;
	struct tr_rtp_ses4   src_ses;

	int                  called_trte;
	struct tr_rtp_ses4   dst_ses;

	//ordering
	void hton()
	{
		caller_trte = htonl(caller_trte);
		src_ses.hton();

		called_trte = htonl(called_trte);
		dst_ses.hton();
	}   
	void ntoh()
	{
		caller_trte = ntohl(caller_trte);
		src_ses.ntoh();

		called_trte = ntohl(called_trte);
		dst_ses.ntoh();
	}

	char* str(char *pbuf, int nlen)
	{
		char szSrc[1024]; szSrc[0] = '\0';
		char szDst[1024]; szDst[0] = '\0';

		src_ses.str(szSrc, sizeof(szSrc));
		dst_ses.str(szDst, sizeof(szDst));

		snprintf(pbuf, nlen, "cr:%s cd:%s src[id:%d %s] dst[id:%d %s]",
			szcaller, szcalled, caller_trte, szSrc, called_trte, szDst);

		return pbuf;
	}
};

// add res & mod res
struct tr_tc_res
{
   struct tr_res        ses;

	int                  tc_cr_trte;
	struct tr_rtp_ses4   tc_cr_ses;

	int                  tc_cd_trte;
	struct tr_rtp_ses4   tc_cd_ses;

	//ordering
	void hton()
	{
		ses.hton();
		tc_cr_trte = htonl(tc_cr_trte);
		tc_cr_ses.hton();
		tc_cd_trte = htonl(tc_cd_trte);
		tc_cd_ses.hton();
	}   
	void ntoh()
	{
		ses.ntoh();
		tc_cr_trte = ntohl(tc_cr_trte);
		tc_cr_ses.ntoh();
		tc_cd_trte = ntohl(tc_cd_trte);
		tc_cd_ses.ntoh();
	}

	char* str(char *pbuf, int nlen)
	{
		char szSes[2048]; szSes[0] = '\0';
		char szIn[1024]; szIn[0] = '\0';
		char szOut[1024]; szOut[0] = '\0';

		ses.str(szSes, sizeof(szSes));
		tc_cr_ses.str(szIn, sizeof(szIn));
		tc_cd_ses.str(szOut, sizeof(szOut));

		snprintf(pbuf, nlen, "ses[%s] tc_cr[rte:%d, %s] tc_cd[rte:%d, %s]",
			szSes, tc_cr_trte, szIn, tc_cd_trte, szOut);

		return pbuf;
	}
};

////////////////////////////////////////////////////////////////////////////////

struct TRMaintReq
{
   enum{ TGRM=0x1, TGA=0x2, SESSION=0x3};
   unsigned int cRange;
   char cReserved[4];
   void hton() { }
   void ntoh() { }
   TRMaintReq() { memset(this,0x00,sizeof(TRMaintReq)); }
   char* str(char* strBuff,int nBufLen)
   {
      switch(cRange)
      {
      case TGRM:
         snprintf(strBuff, sizeof(strBuff), "[range: TGRM]");
      break;
      case TGA:
         snprintf(strBuff, sizeof(strBuff), "[range: TGA]");
      break;
      case SESSION:
         snprintf(strBuff, sizeof(strBuff), "[range: SESSION]");
      break;
      default:
         snprintf(strBuff, sizeof(strBuff), "[range: undefine]");
      break;
      }
      return strBuff;
   }
};

////////////////////////////////////////////////////////////////////////////////
// common header + body
////////////////////////////////////////////////////////////////////////////////

typedef struct st_tr_msg // SVC_TrMsg
{
	enum { e_maxbody=(1024*10) }; 
	SVCComMsgHdr hdr;   //common header
	char         szbody[e_maxbody-sizeof(SVCComMsgHdr)];

	void hton()
	{
		ordering(true, true);
	}
	void ntoh()
	{
		ordering(false, true );
	}
	void hton_body()
	{
		ordering(true, false);
	}
	void ntoh_body()
	{
		ordering(false, false );
	}

	char * str(char * strBuff, int nBufLen) 
	{
		char szType[16];
		char szSubType[8];
		char strHdr[1024]; strHdr[0] = '\0';
		char strBody[2048]; strBody[0] = '\0';
      // type
		switch(hdr.uiType) 
		{
			case  SVC_SETUP_APPS:    
				sprintf(szType, "SETUP_APPS");   
				break;
			case  SVC_ALIVE:    
				sprintf(szType, "ALIVE");   
				break;
			case  SVC_OOS_CHECK:    
				sprintf(szType, "OOS_CHECK");   
				break;
			case TR_MSG_NONE:    
				sprintf(szType, "TR_MSG_NONE");   
				break;
			case TR_INTERNAL:    
				sprintf(szType, "TR_INTERNAL");   
				break;
			case TR_ADD_REQ:    
				sprintf(szType, "TR_ADD_REQ");   
				((tr_add_req*)szbody)->str(strBody, sizeof(strBody)); 
				break;
			case TR_ADD_RES:    
				sprintf(szType, "TR_ADD_RES");   
				((tr_res*)szbody)->str(strBody, sizeof(strBody)); 
				break;
			case TR_MOD_REQ:    
				sprintf(szType, "TR_MOD_REQ");   
				((tr_mod_req*)szbody)->str(strBody, sizeof(strBody)); 
				break;
			case TR_MOD_RES:    
				sprintf(szType, "TR_MOD_RES");   
				((tr_res*)szbody)->str(strBody, sizeof(strBody)); 
				break;
			case TR_DEL_REQ:    
				sprintf(szType, "TR_DEL_REQ");   
				break;
			case TR_DEL_RES:    
				sprintf(szType, "TR_DEL_RES");   
				((tr_del_res*)szbody)->str(strBody, sizeof(strBody)); 
				break;
			case TR_CLOSED_SES: 
		 	   sprintf(szType, "TR_CLOSED_SES"); 
				break;
	      case TR_TC_MOD_REQ:
				sprintf(szType, "TR_TC_MOD_REQ");   
				((tr_tc_mod_req*)szbody)->str(strBody, sizeof(strBody)); 
				break;
	      case TR_TC_MOD_RES:
				sprintf(szType, "TR_TC_MOD_RES");   
				((tr_tc_res*)szbody)->str(strBody, sizeof(strBody)); 
				break;
			case TR_MM_BLOCK:    
				sprintf(szType, "TR_MM_BLOCK");   
				break;
			case TR_MM_UNBLOCK:    
				sprintf(szType, "TR_MM_UNBLOCK");   
				break;
			case TR_MM_RESET:    
				sprintf(szType, "TR_MM_RESET");   
				break;
			case TR_MM_CLEAR:    
				sprintf(szType, "TR_MM_CLEAR");   
				break;
			case TR_NETFAIL:    
				sprintf(szType, "TR_NETFAIL");   
				break;
			case TR_ALIVE_CHECK:    
				sprintf(szType, "TR_ALIVE_CHECK");   
				//if (hdr.uiSubType == TR_RES)
					((tr_alive_check_res*)szbody)->str(strBody, sizeof(strBody)); 
				break;
			default:
				sprintf(szType, "Unknown(%u)", hdr.uiType);
				break;
		}
      // subtype
		switch(hdr.uiSubType) 
		{
			case TR_INFO:
				sprintf(szSubType, "INF");
				break;
			case TR_REQ: 
				sprintf(szSubType, "REQ");
				break;
			case TR_RES:
				sprintf(szSubType, "RES");
				break;
			default:
				sprintf(szSubType, "Unknown(%u)", hdr.uiSubType);
				break;
		}

		snprintf(strHdr, sizeof(strHdr), "[LEN:%u][DESC:%s][MSG:%s-%s][A:%d_S:%d C:%d_S:%u][res:%#x(%s)]", 
		                  hdr.uiMsgLen, hdr.szSesDesc, szType, szSubType, 
                        hdr.uiAsId, hdr.uiAsSesId, hdr.uiCompId, hdr.uiCompSesId , 
                        hdr.uiReasonCode, TGReason::geterr_str(hdr.uiReasonCode));
		snprintf(strBuff, nBufLen, "HDR(%d):%s BODY(%d):%s",
			(int)sizeof(SVCComMsgHdr), strHdr, (int)(hdr.uiMsgLen - sizeof(SVCComMsgHdr)), strBody);
		return strBuff;
	}

protected:
      // ordering
		void ordering(bool bhton=true, bool bincluding_header=true)
		{

      //if network to host
      if ( !bhton && bincluding_header ) hdr.ntoh();
      unsigned int uitype=hdr.uiType;
      unsigned int uistype=hdr.uiSubType;
      //if host to network
		if( bhton && bincluding_header ) hdr.hton();

		//body ordering
		switch(uitype)
		{
			//common message type
			case SVC_SETUP_APPS:
				{
					SVCSetupApp* ptmp=(SVCSetupApp*)szbody;  
					if ( uistype == TR_REQ )
					{
						if( bhton ) ptmp->hton();
						else ptmp->ntoh();
					}
				} 
				break;

			case TR_ADD_REQ:
				{
					tr_add_req* ptmp=(tr_add_req*)szbody;
					if( bhton ) ptmp->hton();
					else ptmp->ntoh();
				}
				break;
			case TR_MOD_REQ:
				{
					tr_mod_req* ptmp=(tr_mod_req*)szbody;
					if( bhton ) ptmp->hton();
					else ptmp->ntoh();
				}
				break;
			case TR_ADD_RES:
				{
					tr_res* ptmp=(tr_res*)szbody;
					if( bhton ) ptmp->hton();
					else ptmp->ntoh();
				}
				break;
			case TR_MOD_RES:
				{
					tr_res* ptmp=(tr_res*)szbody;
					if( bhton ) ptmp->hton();
					else ptmp->ntoh();
				}
				break;
			case TR_DEL_RES:
				{
					tr_del_res* ptmp=(tr_del_res*)szbody;
					if( bhton ) ptmp->hton();
					else ptmp->ntoh();
				}
         break;
			case TR_ALIVE_CHECK:
				{
					tr_alive_check_res* ptmp=(tr_alive_check_res*)szbody;
					if ( uistype == TR_RES )
					{
						if( bhton ) ptmp->hton();
						else ptmp->ntoh();
					}
				}
         break;
	      case TR_TC_MOD_REQ:
			   {
					tr_tc_mod_req* ptmp=(tr_tc_mod_req*)szbody;
					if( bhton ) ptmp->hton();
					else ptmp->ntoh();
				}
			break;
	      case TR_TC_MOD_RES:
			{
					tr_tc_res* ptmp=(tr_tc_res*)szbody;
					if( bhton ) ptmp->hton();
					else ptmp->ntoh();
		   }
			break;
			default:
			break;
      }
   }
} SVC_TrMsg, st_trmsg;

////////////////////////////////////////////////////////////////////////////////
// get str
////////////////////////////////////////////////////////////////////////////////

// util function
inline char* getstr_method(int ntype,char* str)
{
   sprintf(str,"NOT_DEF(0x%04X)",ntype);
   switch(ntype)
	{
		// default
	   case SVC_SETUP_APPS:     strcpy(str,"SETUP_APPS");      break;
		case SVC_ALIVE:          strcpy(str,"ALIVE");           break;
		case SVC_OOS_CHECK:      strcpy(str,"OOS_CHECK");       break;

	   case TR_MSG_NONE:        strcpy(str,"TR_MSG_NON");      break;
	   case TR_INTERNAL:        strcpy(str,"TR_INTRENAL");     break;

	   case TR_ADD_REQ:         strcpy(str,"TR_ADD_REQ");      break;
	   case TR_ADD_RES:         strcpy(str,"TR_ADD_RES");      break;
	   case TR_MOD_REQ:         strcpy(str,"TR_MOD_REQ");      break;
	   case TR_MOD_RES:         strcpy(str,"TR_MOD_RES");      break;
		case TR_DEL_REQ:         strcpy(str,"TR_DEL_REQ");      break;
		case TR_DEL_RES:         strcpy(str,"TR_DEL_RES");      break;
		case TR_CLOSED_SES:      strcpy(str,"TR_CLOSED_SES");   break;
		case TR_TC_MOD_REQ:      strcpy(str,"TR_TC_MOD_REQ");   break;
		case TR_TC_MOD_RES:      strcpy(str,"TR_TC_MOD_RES");   break;

	   case TR_NETFAIL:         strcpy(str,"TR_NETFAIL");      break;
	   case TR_ALIVE_CHECK:     strcpy(str,"TR_ALIVE_CHECK");  break;
	   
		case TR_MM_BLOCK:        strcpy(str,"TR_BLOCK");        break;
	   case TR_MM_UNBLOCK:      strcpy(str,"TR_UNBLOCK");      break;
	   case TR_MM_RESET:        strcpy(str,"TR_RESET");        break;
	   case TR_MM_CLEAR:        strcpy(str,"TR_CLEAR");        break;
   }
   return str;
}

// get header information
inline char* getstr_hdr(st_tr_msg* pmsg, char* szbuf, int nlen)
{
	char szMethod[32];
	char szSubType[32];
	SVCComMsgHdr *phdr = (SVCComMsgHdr*)&(pmsg->hdr);

	szMethod[0] = '\0';
	szSubType[0] = '\0';

	getstr_method(phdr->uiType, szMethod);

	switch(phdr->uiSubType)
	{
		case TR_INFO: snprintf(szSubType, sizeof(szSubType), "INFO"); break;
		case TR_REQ:  snprintf(szSubType, sizeof(szSubType), "REQ"); break;
		case TR_RES:  snprintf(szSubType, sizeof(szSubType), "RES"); break;
		default: snprintf(szSubType, sizeof(szSubType), "undefine:0x%x", phdr->uiSubType); break;
	}

	snprintf(szbuf, nlen, "[LEN:%u][DESC:%s][MSG:%s-%s][A:%d_S:%d C:%d_S:%u][res:%#x(%s)]",
		                    phdr->uiMsgLen, phdr->szSesDesc, szMethod, szSubType, 
                          phdr->uiAsId, phdr->uiAsSesId, phdr->uiCompId, phdr->uiCompSesId, 
                          phdr->uiReasonCode, TGReason::geterr_str(phdr->uiReasonCode)); 
		
	return szbuf;
}

inline char* getstr_body(st_tr_msg* pmsg, char* szbuf, int nlen)
{
   szbuf[0]=0x00;
   if( nlen < 20 ) return szbuf;

   unsigned int uitype=pmsg->hdr.uiType;
   unsigned int uistype=pmsg->hdr.uiSubType;

   char szsub[4096];
   char szsub2[4096];
   int  nsublen=sizeof(szsub);
   int  nsublen2=sizeof(szsub2);
   memset(szsub,0x00,sizeof(nsublen));
   memset(szsub2,0x00,sizeof(nsublen2));
   switch(uitype){
   case TR_ADD_REQ:{
      tr_add_req* pbody=(tr_add_req*)(pmsg->szbody);
      snprintf(szbuf,nlen-1,
         "cr:%s cd:%s, trte(cr:%d cd:%d), tgt_dir:%d)",
         pbody->szcaller, pbody->szcalled, pbody->caller_trte, pbody->called_trte, 
			pbody->tgt_dir);
   }
   break;
   case TR_MOD_REQ:{
      tr_mod_req* pbody=(tr_mod_req*)(pmsg->szbody);
      pbody->caller_ses.str(szsub, nsublen);
      pbody->called_ses.str(szsub2, nsublen2);
      snprintf(szbuf,nlen-1,
         "cr:%s cd:%s, trte(cr:%d cd:%d), tgt_dir:%d, cr_ses[%s] cd_ses[%s])",
         pbody->szcaller, pbody->szcalled, pbody->caller_trte, pbody->called_trte, 
			pbody->tgt_dir, szsub, szsub2);
   }
   break;
	case TR_ADD_RES:
   case TR_MOD_RES:{
      tr_res* pbody=(tr_res*)(pmsg->szbody);
      pbody->src_ses.str(szsub, nsublen);
      pbody->dst_ses.str(szsub2, nsublen2);

      snprintf(szbuf,nlen-1,
			"cr:%s, cd:%s, caller_trte:%d, called_trte:%d, src[%s], dst[%s]",
         pbody->szcaller, pbody->szcalled, pbody->caller_trte, pbody->called_trte, szsub, szsub2);
   }
   break;
   case TR_DEL_RES:{
      tr_del_res* pbody=(tr_del_res*)(pmsg->szbody);
      pbody->src_stat.str(szsub, nsublen);
      pbody->dst_stat.str(szsub2, nsublen2);

      snprintf(szbuf,nlen-1,
			"board:%d, caller_trte:%d, called_trte:%d, src[%s], dst[%s]", 
			   pbody->board, pbody->caller_trte, pbody->called_trte, szsub, szsub2);
   }
	break;
   case TR_ALIVE_CHECK:
	{
      tr_alive_check_res* pbody=(tr_alive_check_res*)(pmsg->szbody);
		if (uistype == TR_RES)
		{
      	pbody->src_stat.str(szsub, nsublen);
      	pbody->dst_stat.str(szsub2, nsublen2);
      	snprintf(szbuf,nlen-1,
				"board:%d, caller_trte:%d, called_trte:%d, src[%s], dst[%s]", 
			   pbody->board, pbody->caller_trte, pbody->called_trte, szsub, szsub2);
		}
	}
   break;
   case TR_TC_MOD_REQ:{
      tr_tc_mod_req* pbody=(tr_tc_mod_req*)(pmsg->szbody);
      pbody->str(szsub, nsublen);
      snprintf(szbuf,nlen-1, "TC_MOD_REQ[%s])",szsub);
   }
   break;
   case TR_TC_MOD_RES:{
      tr_tc_res* pbody=(tr_tc_res*)(pmsg->szbody);
      pbody->str(szsub, nsublen);
      snprintf(szbuf,nlen-1, "TC_MOD_RES[%s])",szsub);
   }
   break;
   //nobody
   case TR_DEL_REQ:
   case TR_MSG_NONE:
   case TR_INTERNAL:
   case TR_CLOSED_SES:
   case TR_MM_BLOCK:
   case TR_MM_UNBLOCK:
   case TR_MM_RESET:
   case TR_MM_CLEAR:
   case TR_NETFAIL:
   break;
   }
	
   return szbuf;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// TGAS <=> TGA Interface
//////////////////////////////////////////////////////////////////////////////////////////////////

// type
enum TGA_MSG_TYPE
{
	TGA_MSG_NONE       = 0x0200,     // none
	TGA_INTERNAL       = 0x0201,     // reserved for Internal message(don't use)
	TGA_HW_INFO        = 0x0202,     // cpu/mem/nic information
	//TGA_NIC_INFO       = 0x0203,     // nic down information

	TGA_INIT           = 0x0210,     // check local address 

	TGA_SET_LOCADDR    = 0x0211,     // set local address 
	TGA_SET_TMPADDR    = 0x0212,     // set temp address

	TGA_SET_SES        = 0x0213,     // set rtp proxy session (4udp relay : a/v rtp, rtcp) 
	TGA_RECOVER_SES    = 0x0214,     // set recover rtp proxy session infomation

	TGA_CLR_LOCADDR    = 0x0215,     // clear local address 
	TGA_CLR_TMPADDR    = 0x0216,     // set rtp proxy session (4udp relay : a/v rtp, rtcp) 

	TGA_CLR_SES        = 0x0217,     // clear rtp proxy session(4udp relay : a/v rtp, rtcp)   
   TGA_CLR_SESALL     = 0x0218,     // clear all rtp proxy session  

	TGA_SET_TC_SES     = 0x0219,     // set tc session
	TGA_CLR_TC_SES     = 0x021A,     // set tc session

   TGA_ENC_SET        = 0x021B,     // set encryption information
	TGA_RESET          = 0x021F,     // all clear (local/temp address & all session)

	TGA_CLOSED_SES     = 0x0220,     // session closed

	TGA_NETFAIL        = 0x0240,     // Info only..
	TGA_ALIVE_CHECK    = 0x0FFF,     // Session Alive
};

// sub-type
enum TGA_MSG_STYPE
{
	TGA_INFO = 0x00,
   TGA_REQ  = 0x01,
	TGA_RES  = 0x02,
};

#if 0
enum e_bd_mode
{
	e_bd_mode_active     = 0x01,
	e_bd_mode_redundancy = 0x02,
	e_bd_mode_fail       = 0x04,
};
#endif

typedef struct tr_rel_ses4 tga_session;

#if 0
struct tga_if_info
{
	enum { e_maxnum_if=32 };
	char szNetName[e_maxnum_if];
   struct tr_addr    ip;

   void hton()
	{
		ip.hton();
	}
   void ntoh()
	{
		ip.ntoh();
	}
	char* str(char* pbuf, int nlen)
	{
      char szIP[128]; szIP[0]='\0';
		ip.str(szIP, sizeof(szIP));
	   snprintf(pbuf, nlen, "IF NAME:%s IP[%s]", szNetName, szIP);
		return pbuf;
	}
};

struct tga_hw_info
{
   int cpu;
   int mem;

   void hton()
	{
		cpu = htonl(cpu);
		mem = htonl(mem);
	}   
	void ntoh()
	{
		cpu = ntohl(cpu);
		mem = ntohl(mem);
	}
   char* str(char* pbuf, int nlen)
	{
      snprintf(pbuf, nlen, "cpu:%d, mem:%d", cpu, mem);
      return pbuf;
	}
};
#endif

struct tga_loc_ses 
{
   unsigned short sport; 
   unsigned short dport;

   void hton()
	{
		sport = htons(sport);
		dport = htons(dport);
	}   
	void ntoh()
	{
		sport = ntohs(sport);
		dport = ntohs(dport);
	}
   char* str(char* pbuf, int nlen)
	{
      snprintf(pbuf, nlen, "sport:%d, dport:%d", sport, dport);
      return pbuf;
	}
};

#if 0
struct tga_loc_ses4 
{
   unsigned int         sid;    
   unsigned int         did;
   struct tga_loc_ses   artp;
   struct tga_loc_ses   artcp;
   struct tga_loc_ses   vrtp;
   struct tga_loc_ses   vrtcp;

   struct tr_ses_stat   artp_sstat;
   struct tr_ses_stat   artcp_sstat;
   struct tr_ses_stat   vrtp_sstat;
   struct tr_ses_stat   vrtcp_sstat;

   struct tr_ses_stat   artp_dstat;
   struct tr_ses_stat   artcp_dstat;
   struct tr_ses_stat   vrtp_dstat;
   struct tr_ses_stat   vrtcp_dstat;
	void hton()
	{
		sid = htonl(sid);
		did = htonl(did);

		artp.hton();
		artcp.hton();
		vrtp.hton();
		vrtcp.hton();

      artp_sstat.hton();
      artcp_sstat.hton();
      vrtp_sstat.hton();
      vrtcp_sstat.hton();

      artp_dstat.hton();
      artcp_dstat.hton();
      vrtp_dstat.hton();
      vrtcp_dstat.hton();
	}   
	void ntoh()
	{
		sid = ntohl(sid);
		did = ntohl(did);

		artp.ntoh();
		artcp.ntoh();
		vrtp.ntoh();
		vrtcp.ntoh();

      artp_sstat.ntoh();
      artcp_sstat.ntoh();
      vrtp_sstat.ntoh();
      vrtcp_sstat.ntoh();

      artp_dstat.ntoh();
      artcp_dstat.ntoh();
      vrtp_dstat.ntoh();
      vrtcp_dstat.ntoh();
	}
	char* str(char* pbuf, int nlen)
	{
      char szARtp[48]; szARtp[0]='\0';      artp.str(szARtp, sizeof(szARtp));
      char szARtcp[48]; szARtcp[0]='\0';    artcp.str(szARtcp, sizeof(szARtcp));
      char szVRtp[48]; szVRtp[0]='\0';      vrtp.str(szVRtp, sizeof(szVRtp));
      char szVRtcp[48]; szVRtcp[0]='\0';    vrtcp.str(szVRtcp, sizeof(szVRtcp));

      char szARtps[256]; szARtps[0]='\0';   artp_sstat.str(szARtps, sizeof(szARtps));
      char szARtcps[256]; szARtcps[0]='\0'; artcp_sstat.str(szARtcps, sizeof(szARtcps));
      char szVRtps[256]; szVRtps[0]='\0';   vrtp_sstat.str(szVRtps, sizeof(szVRtps));
      char szVRtcps[256]; szVRtcps[0]='\0'; vrtcp_sstat.str(szVRtcps, sizeof(szVRtcps));

      char szARtpd[256]; szARtpd[0]='\0';   artp_dstat.str(szARtps, sizeof(szARtpd));
      char szARtcpd[256]; szARtcpd[0]='\0'; artcp_dstat.str(szARtcps, sizeof(szARtcpd));
      char szVRtpd[256]; szVRtpd[0]='\0';   vrtp_dstat.str(szVRtps, sizeof(szVRtpd));
      char szVRtcpd[256]; szVRtcpd[0]='\0'; vrtcp_dstat.str(szVRtcps, sizeof(szVRtcpd));

	   snprintf(pbuf, nlen, "sid:%d, did:%d, artp[%s#%s#%s] artcp[%s#%s#%s], vrtp[%s#%s#%s] vrtcp[%s#%s#%s]",
		   sid, did, szARtp, szARtps, szARtpd, szARtcp, szARtcps, szARtcpd, szVRtp, szVRtps, szVRtpd, szVRtcp, szVRtcps, szVRtcpd);
      return pbuf; 
	}
};
#endif

struct tga_loc_ses4 
{
   unsigned int         sid;    
   unsigned int         did;
   struct tga_loc_ses   artp;
   struct tga_loc_ses   artcp;
   struct tga_loc_ses   vrtp;
   struct tga_loc_ses   vrtcp;

   struct tr_ses_stat4  sstat;
   struct tr_ses_stat4  dstat;
	void hton()
	{
		sid = htonl(sid);
		did = htonl(did);

		artp.hton();
		artcp.hton();
		vrtp.hton();
		vrtcp.hton();

      sstat.hton();
      dstat.hton();
	}   
	void ntoh()
	{
		sid = ntohl(sid);
		did = ntohl(did);

		artp.ntoh();
		artcp.ntoh();
		vrtp.ntoh();
		vrtcp.ntoh();

      sstat.ntoh();
      dstat.ntoh();
	}
	char* str(char* pbuf, int nlen)
	{
      char szARtp[48]; szARtp[0]='\0';      artp.str(szARtp, sizeof(szARtp));
      char szARtcp[48]; szARtcp[0]='\0';    artcp.str(szARtcp, sizeof(szARtcp));
      char szVRtp[48]; szVRtp[0]='\0';      vrtp.str(szVRtp, sizeof(szVRtp));
      char szVRtcp[48]; szVRtcp[0]='\0';    vrtcp.str(szVRtcp, sizeof(szVRtcp));

      char szSStat[1024]; szSStat[0]='\0';   sstat.str(szSStat, sizeof(szSStat));

      char szDStat[1024]; szDStat[0]='\0';   dstat.str(szDStat, sizeof(szDStat));

	   snprintf(pbuf, nlen, "sid:%d, did:%d, sstat[%s] dstat[%s]",
		   sid, did, szSStat, szDStat);
      return pbuf; 
	}
};

#if 1
typedef struct tga_init{
   unsigned char ucType; // 1:alloc 2:remove
   unsigned char ucIPCnt;
   unsigned char ucBdMode; // active or redundancy
   unsigned char ucRecoverBoard; // 절체시에 원래 보드 Index
   unsigned char ucMMI; // 응답시에 그대로 리턴
   unsigned char ucReserved[3];

   struct tr_init_addr ip[MAX_TG_LOC_ADDR];

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
} st_tg_init_req, *pst_tg_init_req; 
#endif

typedef st_init_res st_tg_init_res;

struct tga_set_locaddr 
{
   unsigned int      id;
   struct tr_addr    ip;

	void hton()
	{
		id = htonl(id);
		ip.hton();
	}   
	void ntoh()
	{
		id = ntohl(id);
		ip.ntoh();
	}
   char* str(char* pbuf, int nlen)
	{
		char szIP[128]; szIP[0]='\0'; ip.str(szIP, sizeof(szIP));
      snprintf(pbuf, nlen, "id:%d ip:%s", id, szIP);
      return pbuf;
	}
};
// TGA_SET_LOCADDR RES : no body

struct tga_clear_locaddr
{
   unsigned int      id;
	void hton()
	{
		id = htonl(id);
	}   
	void ntoh()
	{
		id = ntohl(id);
	}
   char* str(char* pbuf, int nlen)
	{
      snprintf(pbuf, nlen, "id:%d", id);
      return pbuf;
	}
};
// TGA_CLR_LOCADDR RES : no body

struct tga_set_tmpaddr 
{
   struct tr_addr    ip4;
   struct tr_addr    ip6;

	void hton()
	{
		ip4.hton();
		ip6.hton();
	}   
	void ntoh()
	{
		ip4.ntoh();
		ip6.ntoh();
	}
   char* str(char* pbuf, int nlen)
	{
		char szIP4[128]; szIP4[0]='\0'; ip4.str(szIP4, sizeof(szIP4));
		char szIP6[128]; szIP6[0]='\0'; ip6.str(szIP6, sizeof(szIP6));
      snprintf(pbuf, nlen, "ip4:%s ip6:%s", szIP4, szIP6);
      return pbuf;
	}
};
// TGA_SET_TMPADDR RES : no body
// TGA_CLR_TMPADDR REQ & RES : no body

struct tga_trace
{
   enum { e_max_min = 32 };
	char   cEnable;
   char   cCrOwnRoute; // cr side 0:타사 1:자사 
   char   cCdOwnRoute; // cd side 0:타사 1:자사
   char   cReserved[4];
   char   cResetMediaTrace; // 20150825 by hak for R103 PKG. 값이 1일 경우 미디어 트레이스 초기화함.
	char   szcallid[128]; // 2014.07.15 8->128
	char   szcaller[e_max_min];
	char   szcalled[e_max_min];
	struct SVCTrace_t    Trace;

	void hton()
	{
      Trace.hton();
	}
	void ntoh()
	{
		Trace.ntoh();
	}
	char* str(char* pbuf, int nlen)
	{
		char szTrace[1024]; szTrace[0]='\0'; Trace.str(szTrace, sizeof(szTrace));
      snprintf(pbuf, nlen, "rstmedia:%d callid:%s own(cr:%d cd:%d) cr:%s cd:%s trc[%s]", 
			cResetMediaTrace,szcallid, cCrOwnRoute, cCdOwnRoute, szcaller, szcalled, szTrace);
      return pbuf;
	}

};

struct tga_set_tc_ses_req
{
   tga_session ses;
   tga_session tc_ses;
	struct tga_trace stTrace;

	void hton()
	{
		ses.hton();
		tc_ses.hton();
		stTrace.hton();
	}   
	void ntoh()
	{
		ses.ntoh();
		tc_ses.ntoh();
		stTrace.ntoh();
   }
   char* str(char* pbuf, int nlen)
	{
		char szSes[1024]; szSes[0]='\0'; 
		char szTcSes[1024]; szTcSes[0]='\0'; 
		char szTrace[1024]; szTrace[0]='\0'; 
		ses.str(szSes, sizeof(szSes));
		tc_ses.str(szTcSes, sizeof(szTcSes));
		stTrace.str(szTrace, sizeof(szTrace));
      snprintf(pbuf, nlen, "Ses[%s] TC[%s] Trace[%s]", szSes, szTcSes, szTrace);
      return pbuf;
	}
};

struct tga_set_ses_req
{
   tga_session ses;
	struct tga_trace stTrace;

	void hton()
	{
		ses.hton();
		stTrace.hton();
	}   
	void ntoh()
	{
		ses.ntoh();
		stTrace.ntoh();
   }
   char* str(char* pbuf, int nlen, bool bPrintRtcp = true)
	{
		char szSes[2048]; szSes[0]='\0'; 
		char szTrace[1024]; szTrace[0]='\0'; 
		ses.str(szSes, sizeof(szSes), bPrintRtcp);
		stTrace.str(szTrace, sizeof(szTrace));
      snprintf(pbuf, nlen, "Ses[%s] Trace[%s]", szSes, szTrace);
      return pbuf;
	}
};
// TGA_SET RES : no body

struct tga_recover_ses_req
{
	int nRecoverCnt;
	unsigned char ucEndReq;
	unsigned char ucReserved[3];

   struct tga_set_ses_req ses[MAX_RECOVER_SES];

	void hton()
	{
		nRecoverCnt = htonl(nRecoverCnt);
      for (int id =0; id<MAX_RECOVER_SES; id++) 
	      ses[id].hton();
	}
	void ntoh()
	{
		nRecoverCnt = ntohl(nRecoverCnt);
		for (int id = 0; id<MAX_RECOVER_SES; id++)
			ses[id].ntoh();
	}
	char* str(char*pbuf, int nlen)
	{
		snprintf(pbuf, nlen, "RecoverCnt[%d] EndReq[%d]", nRecoverCnt, ucEndReq);
		return pbuf;
	}
};
// TGA_RECOVER RES : no body

struct tga_clear_tc_ses
{
   struct tga_loc_ses4 ses; 
   struct tga_loc_ses4 tc_ses; 
	void hton()
	{
      ses.hton();
      tc_ses.hton();
	}   
	void ntoh()
	{
      ses.ntoh();
      tc_ses.ntoh();
   }
   char* str(char* pbuf, int nlen)
	{
		char szSes[2048]; szSes[0]='\0'; 
		char szTcSes[2048]; szTcSes[0]='\0'; 
		ses.str(szSes, sizeof(szSes));
		tc_ses.str(szTcSes, sizeof(szTcSes));
      snprintf(pbuf, nlen, "Ses[%s] TC[%s]", szSes, szTcSes);
      return pbuf;
	}
};

#if 0
typedef struct tga_clear_ses
{
   struct tga_loc_ses4 ses; 
	void hton()
	{
      ses.hton();
	}   
	void ntoh()
	{
      ses.ntoh();
   }
   char* str(char* pbuf, int nlen)
	{
		char szSes[2048]; szSes[0]='\0'; 
		ses.str(szSes, sizeof(szSes));
      snprintf(pbuf, nlen, "Ses[%s]", szSes);
      return pbuf;
	}
} tga_alive_check;
#endif

typedef tga_loc_ses4 tga_clear_ses;
typedef tga_clear_ses tga_alive_check;
typedef tga_clear_ses tga_closed_ses;

// struct tga_clear_ses req & response

////////////////////////////////////////////////////////////////////////////////
// common header + body
////////////////////////////////////////////////////////////////////////////////

typedef struct st_tga_msg // SVC_TgaMsg
{
	enum { e_maxbody=(1024*10) }; 
	SVCComMsgHdr hdr;   //common header
	char         szbody[e_maxbody-sizeof(SVCComMsgHdr)];

	void hton()
	{
		ordering(true, true);
	}
	void ntoh()
	{
		ordering(false, true );
	}
	void hton_body()
	{
		ordering(true, false);
	}
	void ntoh_body()
	{
		ordering(false, false );
	}

	char * str(char * strBuff, int nBufLen, bool bPrintRtcp = true) 
	{
		char szType[32];
		char szSubType[8];
		char strHdr[1024]; strHdr[0] = '\0';
		char strBody[4096]; strBody[0] = '\0';
      // type
		switch(hdr.uiType) 
		{
			case  SVC_SETUP_APPS:    
				sprintf(szType, "SETUP");   
				break;
			case  SVC_ALIVE:    
				sprintf(szType, "ALIVE");   
				break;
			case  SVC_OOS_CHECK:    
				sprintf(szType, "OOS_CHECK");   
				break;
	      case TGA_INTERNAL:                        // reserved for Internal message(don't use)
				sprintf(szType, "INTERNAL");   
				break;
		   case TGA_HW_INFO:
				sprintf(szType, "TGA_HW_INFO");   
				if (hdr.uiSubType == TGA_INFO)
				   ((st_hw_info*)szbody)->str(strBody, sizeof(strBody)); 
				break;
#if 0
			case TGA_NIC_INFO:
			   sprintf(szType, "TGA_NIC_INFO");
				if (hdr.uiSubType == TGA_INFO)
					((tga_if_info*)szbody)->str(strBody, sizeof(strBody));
				break;
#endif
	      case TGA_INIT:  								   // set local address 
				sprintf(szType, "TGA_INIT");   
				if (hdr.uiSubType == TGA_REQ)
				   ((st_tg_init_req*)szbody)->str(strBody, sizeof(strBody)); 
				else if (hdr.uiSubType == TGA_RES)
				   ((st_tg_init_res*)szbody)->str(strBody, sizeof(strBody)); 

				break;
	      case TGA_SET_LOCADDR:                     // set local address 
				sprintf(szType, "SET_LOCADDR");   
				break;
	      case TGA_SET_TMPADDR:                     // set temp address
				sprintf(szType, "SET_TMPADDR");   
				break;
	      case TGA_SET_SES:                         // set rtp proxy session (4udp relay : a/v rtp, rtcp) 
				sprintf(szType, "SET_SESSION");   
		      if (hdr.uiSubType == TGA_REQ)
				   ((tga_set_ses_req*)szbody)->str(strBody, sizeof(strBody), bPrintRtcp); 
				break;
			case TGA_RECOVER_SES:
			   sprintf(szType, "RECOVER_SESSION");
		      if (hdr.uiSubType == TGA_REQ)
				   ((tga_recover_ses_req*)szbody)->str(strBody, sizeof(strBody)); 
				break;
	      case TGA_CLR_LOCADDR:                     // clear local address 
				sprintf(szType, "CLR_LOCADDR");   
				break;
	      case TGA_CLR_TMPADDR:                     // set rtp proxy session (4udp relay : a/v rtp, rtcp) 
				sprintf(szType, "CLR_TMPADDR");   
				break;
	      case TGA_CLR_SES:                         // clear rtp proxy session(4udp relay : a/v rtp, rtcp)   
				sprintf(szType, "CLR_SESSION");   
				((tga_clear_ses*)szbody)->str(strBody, sizeof(strBody)); 
				break;
         case TGA_CLR_SESALL:                      // clear all rtp proxy session  
				sprintf(szType, "CLR_SESSIONALL");   
				break;
	      case TGA_CLOSED_SES:  // closed rtp proxy session(4udp relay : a/v rtp, rtcp)   
				sprintf(szType, "CLR_SESSION");   
				((tga_closed_ses*)szbody)->str(strBody, sizeof(strBody)); 
				break;
	      case TGA_RESET:                           // all clear (local/temp address & all session)
				sprintf(szType, "RESET");  
				break;
	      case TGA_NETFAIL:                         // Info only..
				sprintf(szType, "NETFAIL");   
				break;
	      case TGA_ALIVE_CHECK:                     // Session Alive
				sprintf(szType, "ALIVE");   
		      if (hdr.uiSubType == TGA_RES)
				   ((tga_alive_check*)szbody)->str(strBody, sizeof(strBody)); 
				break;
			case TGA_SET_TC_SES:
				sprintf(szType, "SET_TC_SESSION");   
		      if (hdr.uiSubType == TGA_REQ)
				   ((tga_set_tc_ses_req*)szbody)->str(strBody, sizeof(strBody)); 
				break;
			case TGA_CLR_TC_SES:
				sprintf(szType, "CLR_TC_SESSION");   
				((tga_clear_tc_ses*)szbody)->str(strBody, sizeof(strBody)); 
				break;
			default:
				sprintf(szType, "Unknown(%u)", hdr.uiType);
				break;
		}

      // subtype
		switch(hdr.uiSubType) 
		{
			case TGA_INFO:
				sprintf(szSubType, "INF");
				break;
			case TGA_REQ:
				sprintf(szSubType, "REQ");
				break;
			case TGA_RES:
				sprintf(szSubType, "RES");
				break;

			default:
				sprintf(szSubType, "Unknown(%u)", hdr.uiSubType);
				break;
		}

		snprintf(strHdr, sizeof(strHdr), "[LEN:%u][DSC:%s][MSG:%s-%s][A:%d_S:%d C:%d_S:%u][res:%#x(%s)]", 
		                  hdr.uiMsgLen, hdr.szSesDesc, szType, szSubType, 
                        hdr.uiAsId, hdr.uiAsSesId,
                        hdr.uiCompId, hdr.uiCompSesId , 
                        hdr.uiReasonCode, TGReason::geterr_str(hdr.uiReasonCode));
		snprintf(strBuff, nBufLen, "HDR(%d):%s BODY(%d):%s",
			(int)sizeof(SVCComMsgHdr), strHdr, (int)(hdr.uiMsgLen - sizeof(SVCComMsgHdr)), strBody);
		return strBuff;
	}

protected:
      // ordering
   void ordering(bool bhton=true, bool bincluding_header=true)
   {
      //if network to host
      if ( !bhton && bincluding_header ) hdr.ntoh();
      unsigned int uitype=hdr.uiType;
      unsigned int uistype=hdr.uiSubType;
      //if host to network
      if( bhton && bincluding_header ) hdr.hton();

      //body ordering
      switch(uitype)
		{
         //common message type
      case SVC_SETUP_APPS:
         if ( uistype == TGA_REQ )
         {
            SVCSetupApp* ptmp=(SVCSetupApp*)szbody;  
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         }
         break;
      case TGA_HW_INFO:
		   if ( uistype == TGA_INFO )
			{
            st_hw_info * ptmp=(st_hw_info *)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
			}
         break;
#if 0
		case TGA_NIC_INFO:
		   if ( uistype == TGA_INFO )
		   {
            struct tga_if_info * ptmp=(struct tga_if_info *)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
			break;
#endif
      case TGA_INIT:
         if ( uistype == TGA_REQ )
         {  
            //struct tga_init * ptmp=(struct tga_init *)szbody;
            st_tg_init_req * ptmp=(st_tg_init_req *)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         }  
			else
			{
            st_tg_init_res * ptmp=(st_tg_init_res *)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
			}
         break;
      case TGA_SET_LOCADDR:
         if ( uistype == TGA_REQ )
         {  
            struct tga_set_locaddr * ptmp=(struct tga_set_locaddr *)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         }  
         break;
      case TGA_SET_TMPADDR: 
         if ( uistype == TGA_REQ )
         {  
            struct tga_set_tmpaddr * ptmp=(struct tga_set_tmpaddr *)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         }  
         break;
      case TGA_SET_SES:
         if ( uistype == TGA_REQ ) {
            struct  tga_set_ses_req* ptmp=(struct tga_set_ses_req*)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         } 
         break;
      case TGA_RECOVER_SES:
         if ( uistype == TGA_REQ ) {
            struct  tga_recover_ses_req* ptmp=(struct tga_recover_ses_req*)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         } 
         break;
      case TGA_CLR_LOCADDR:
         if ( uistype == TGA_REQ ) {
            struct tga_clear_locaddr * ptmp=(struct tga_clear_locaddr *)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         }  
         break;
      case TGA_CLR_TMPADDR:
         break;
      case TGA_CLR_SES:
         if ( uistype == TGA_REQ ) {
            tga_clear_ses* ptmp=(tga_clear_ses*)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         } else if (uistype == TGA_RES) {
            tga_clear_ses* ptmp=(tga_clear_ses*)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         }  
         break;
      case TGA_CLOSED_SES:
		{
         tga_closed_ses* ptmp=(tga_closed_ses*)szbody;
         if( bhton ) ptmp->hton();
         else ptmp->ntoh();
		}
      break;
      case TGA_ALIVE_CHECK:
		   if ( uistype == TGA_RES ) {
				tga_alive_check* ptmp=(tga_alive_check*)szbody;
				if( bhton ) ptmp->hton();
				else ptmp->ntoh();
			}
		   break;
      case TGA_SET_TC_SES:
         if ( uistype == TGA_REQ ) {
            struct  tga_set_tc_ses_req* ptmp=(struct tga_set_tc_ses_req*)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         } 
		   break;
      case TGA_CLR_TC_SES:
         if ( uistype == TGA_REQ ) {
            struct tga_clear_tc_ses* ptmp=(struct tga_clear_tc_ses*)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         } else if (uistype == TGA_RES) {
            struct tga_clear_tc_ses* ptmp=(struct tga_clear_tc_ses*)szbody;
            if( bhton ) ptmp->hton();
            else ptmp->ntoh();
         }  
		   break;
      case TGA_CLR_SESALL:
         break;
      case TGA_RESET:
         break;
      default:
         break;
    }

   }
} SVC_TgaMsg, st_tgamsg;

////////////////////////////////////////////////////////////////////////////////
// get str
////////////////////////////////////////////////////////////////////////////////

// util function
inline char* get_str_method(int ntype,char* str, int nlen)
{
   switch(ntype)
	{
		// default
	   case SVC_SETUP_APPS:     snprintf(str,nlen,"SETUP_APPS");      break;
		case SVC_ALIVE:          snprintf(str,nlen,"ALIVE");           break;
		case SVC_OOS_CHECK:      snprintf(str,nlen,"OOS_CHECK");       break;

	   case TGA_MSG_NONE:       snprintf(str,nlen,"TGA_MSG_NON");     break;
	   case TGA_INTERNAL:       snprintf(str,nlen,"TGA_INTRENAL");    break;
      case TGA_HW_INFO:        snprintf(str,nlen,"TGA_HW_INFO");     break;

	   case TGA_INIT:           snprintf(str,nlen,"TGA_INIT");        break;
	   case TGA_SET_LOCADDR:    snprintf(str,nlen,"TGA_SET_LOCADDR"); break;
	   case TGA_SET_TMPADDR:    snprintf(str,nlen,"TGA_SET_TMPADDR"); break;
	   case TGA_SET_SES:        snprintf(str,nlen,"TGA_SET_SES");     break;
	   case TGA_RECOVER_SES:    snprintf(str,nlen,"TGA_RECOVER_SES"); break;
	   case TGA_CLR_LOCADDR:    snprintf(str,nlen,"TGA_CLR_LOCADDR"); break;
	   case TGA_CLR_TMPADDR:    snprintf(str,nlen,"TGA_CLR_TMPADDR"); break;
	   case TGA_CLR_SES:        snprintf(str,nlen,"TGA_CLR_SES");     break;
	   case TGA_CLR_SESALL:     snprintf(str,nlen,"TGA_CLR_SESALL");  break;
	   case TGA_RESET:          snprintf(str,nlen,"TGA_RESET");       break;
	   case TGA_NETFAIL:        snprintf(str,nlen,"TGA_NETFAIL");     break;
	   case TGA_ALIVE_CHECK:    snprintf(str,nlen,"TGA_ALIVE_CHECK"); break;
		case TGA_SET_TC_SES:     snprintf(str,nlen,"TGA_SET_TC_SES");  break;
		case TGA_CLR_TC_SES:     snprintf(str,nlen,"TGA_CLR_TC_SES");  break;
		case TGA_CLOSED_SES:     snprintf(str,nlen,"TGA_CLOSED_SES");  break;
		default:                 snprintf(str,nlen,"Unknown(0x%x",ntype);         break;
   }
   return str;
}

// get header information
inline char* getstr_hdr(st_tga_msg* pmsg, char* szbuf, int nlen)
{
	char szMethod[32];
	char szSubType[32];
	SVCComMsgHdr *phdr = (SVCComMsgHdr*)&(pmsg->hdr);

	szMethod[0] = '\0';
	szSubType[0] = '\0';

	get_str_method(phdr->uiType, szMethod, sizeof(szMethod));

	switch(phdr->uiSubType)
	{
		case TGA_INFO: snprintf(szSubType, sizeof(szSubType), "INFO"); break;
		case TGA_REQ:  snprintf(szSubType, sizeof(szSubType), "REQ"); break;
		case TGA_RES:  snprintf(szSubType, sizeof(szSubType), "RES"); break;
		default: snprintf(szSubType, sizeof(szSubType), "undefine:0x%x", phdr->uiSubType); break;
	}

	snprintf(szbuf, nlen, "[LEN:%u][DESC:%s][MSG:%s-%s][A:%d_S:%d C:%d_S:%u][res:%#x(%s)]",
		                    phdr->uiMsgLen, phdr->szSesDesc, szMethod, szSubType, 
                          phdr->uiAsId, phdr->uiAsSesId, phdr->uiCompId, phdr->uiCompSesId, 
                          phdr->uiReasonCode, TGReason::geterr_str(phdr->uiReasonCode)); 
		
	return szbuf;
}

inline char* getstr_body(st_tga_msg* pmsg, char* szbuf, int nlen)
{
   szbuf[0]=0x00;
   if( nlen < 20 ) return szbuf;

   unsigned int uitype=pmsg->hdr.uiType;
   unsigned int uistype=pmsg->hdr.uiSubType;

   char szsub[2048];
   int  nsublen=sizeof(szsub);
   memset(szsub,0x00,sizeof(nsublen));
   switch(uitype)
	{
      case TGA_HW_INFO:
		{
		   if (uistype == TGA_INFO)
			{
				st_hw_info* pbody=(st_hw_info*)(pmsg->szbody);
				pbody->str(szsub, nsublen);
				snprintf(szbuf,nlen-1, "hw_info[%s]", szsub);
			}
		}
		break;
#if 0
		case TGA_NIC_INFO:
		{
         if (uistype == TGA_INFO)
			{
            tga_if_info* pbody=(tga_if_info*)(pmsg->szbody);
				pbody->str(szsub, nsublen);
				snprintf(szbuf,nlen-1, "nic_info[%s]", szsub);
			}
		}
		break;
#endif
      case TGA_INIT:
		{
		   if (uistype == TGA_REQ)
			{
				st_tg_init_req* pbody=(st_tg_init_req*)(pmsg->szbody);
				pbody->str(szsub, nsublen);
				snprintf(szbuf,nlen-1, "init_req[%s]", szsub);
			}
			else if (uistype == TGA_RES)
			{
				st_tg_init_res* pbody=(st_tg_init_res*)(pmsg->szbody);
				pbody->str(szsub, nsublen);
				snprintf(szbuf,nlen-1, "init_res[%s]", szsub);
			}
		}
		break;
      case TGA_SET_SES:
		{
		   if (uistype == TGA_REQ)
		   {
				tga_set_ses_req* pbody=(tga_set_ses_req*)(pmsg->szbody);
				pbody->str(szsub, nsublen);
				snprintf(szbuf,nlen-1, "ses[%s])", szsub);
			}
      }
      break;
      case TGA_RECOVER_SES:
		{
		   if (uistype == TGA_REQ)
		   {
				tga_recover_ses_req* pbody=(tga_recover_ses_req*)(pmsg->szbody);
				pbody->str(szsub, nsublen);
				snprintf(szbuf,nlen-1, "recover ses[%s])", szsub);
			}
      }
      break;
      case TGA_CLR_SES:
		{
         tga_clear_ses* pbody=(tga_clear_ses*)(pmsg->szbody);
         pbody->str(szsub, nsublen);
         snprintf(szbuf,nlen-1, "ses[%s]", szsub);
      }
      break;
      case TGA_CLOSED_SES:
		{
         tga_closed_ses* pbody=(tga_closed_ses*)(pmsg->szbody);
         pbody->str(szsub, nsublen);
         snprintf(szbuf,nlen-1, "ses[%s]", szsub);
      }
      break;
      case TGA_ALIVE_CHECK:
		{
			tga_alive_check* pbody=(tga_alive_check*)(pmsg->szbody);
			pbody->str(szsub, nsublen);
			snprintf(szbuf,nlen-1, "ses[%s]", szsub);
	   }
      break;
		case TGA_SET_TC_SES:
		{
		   if (uistype == TGA_REQ)
		   {
				tga_set_tc_ses_req* pbody=(tga_set_tc_ses_req*)(pmsg->szbody);
				pbody->str(szsub, nsublen);
				snprintf(szbuf,nlen-1, "ses[%s])", szsub);
			}
		}
		break;
		case TGA_CLR_TC_SES:
		{
         tga_clear_tc_ses* pbody=(tga_clear_tc_ses*)(pmsg->szbody);
         pbody->str(szsub, nsublen);
         snprintf(szbuf,nlen-1, "ses[%s]", szsub);
		}
		break;
      //nobody
      case TGA_MSG_NONE:
      case TGA_INTERNAL:
      case TGA_SET_LOCADDR:
      case TGA_SET_TMPADDR:
      case TGA_CLR_LOCADDR:
      case TGA_CLR_TMPADDR:
      case TGA_CLR_SESALL:
      case TGA_RESET:
      case TGA_NETFAIL:
      break;
   }
   return szbuf;
}


}//name space TGRM
#endif

