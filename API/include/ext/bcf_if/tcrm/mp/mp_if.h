#ifndef __RTP_AGENT_IF_H__
#define __RTP_AGENT_IF_H__

#include <stdio.h>
#include <sstream>
#include "common_msg.h"
#include "mp_com.h"
#include "mp_err.h"
#include "mp_codectype.h"

#ifdef _MSC_VER
   #include <WINSOCK2.H>
#else
   #include <netinet/in.h>
#endif

#ifdef WIN32
   #ifndef _WINSOCKAPI_
     #define _WINSOCKAPI_
   #endif
#endif

#define ARMSG_SIZE (sizeof(ARMsg))
#define ARMSGHDR_SIZE (sizeof(ARMsgHdr))
#define ARMSGBODY_SIZE ARMSG_SIZE - ARMSGHDR_SIZE
#define MULTIPLAY_MAXFILENUM 10
#define MAX_REPLAY_COUNT 10
#define MAX_GROUP_PER_SES 2//10 //first will be coded max 2 group per ses because time limits
                             //but last end spot.. max 10 group per ses
enum AR_MSG_TYPE
{
	AMT_MSG_NONE		= 0,
   AMT_INTERNAL      = 1,      //reserved for Internal message(don't use)
   AMT_SETUP_AGNT    = 2,      //Request & Response
   AMT_SETUP_APPS    = 3,      //Request & Response (no body)
   AMT_ALIVE         = 4,      //Info only          (no body)
   AMT_START_SES     = 5,      //Request & Response
   AMT_STOP_SES      = 6,      //Request & Response
   AMT_START_SES_DUMP = 7,      //Request & Response
   AMT_WARN_SES      = 8,      //Info   only        (no body)
   AMT_CLOSED_SES    = 9,      //Info only (no body) send by rtpm only
   AMT_PLAY_READY    = 10,      //Request & Response
   AMT_PLAY          = 11,      //Request & Response
   AMT_PLAY_SIMPLE   = 12,      //Request & Response
   AMT_PLAY_MULTI    = 13,		//Request & Response
   AMT_PLAY_SIMPLE_TC = 14,     //not implement.. future use
   AMT_PLAY_MULTI_TC  = 15,     //not implement.. future use
   AMT_PLAY_STOP     = 16,      //Request & Response
   AMT_PLAY_DONE     = 17,      //Info   only
   AMT_RECORD          = 18,      //Request, Response
   AMT_RECORD_TC       = 19,      //not implement.. future use
   AMT_RECORD_STOP     = 20,      //Request & Response
   AMT_RECORD_DONE     = 21,      //Info only
   AMT_RELAY           = 22,      //Request & Response
   AMT_RELAY_TC        = 23,      //Request & Response
   AMT_RELAY_STOP      = 24,      //Request & Response
   AMT_DTMF_SET         = 25,      //Request & Response
   AMT_DTMF_GEN         = 26,      //Request & Response
   AMT_DTMF_INFO        = 27,      //Info only
   AMT_DTMF_GENINFO    = 28,       //Info only
   AMT_MM_BLOCK         = 29,      //Request & Response..not implement
   AMT_MM_UNBLOCK       = 30,      //Request & Response..not implement
   AMT_MM_RESET         = 31,      //Request & Response
   AMT_MM_BD_STS     	= 32,      //Request & Response..not implement
   AMT_MM_SES_STS   		= 33,      //Request & Response..not implement
   AMT_MM_BD_INF     	= 34,      //Info only..not implement
   AMT_MM_SES_INF   		= 35,       //Info only..not implement
   AMT_MM_RM_INF        = 36,      //Info only..not implement
	AMT_MM_CLEAR         = 37,		  //Request & Reponse ..clear media flush
	AMT_NETFAIL				= 38,			//Info only..
   AMT_MSG_MAX,
   AMT_SETUP_APPS_T		 = 0xF001,
   AMT_ALIVE_T       = 0xF002,      //Info only          (no body)
	AMT_ALIVE_CHECK	= 0xFFFF
};


enum AR_MSG_STYPE
{
   AMST_INFO      = 0x0000,
   AMST_REQ       = 0x0001,
   AMST_RES       = 0x0002,
};

enum {
   MPA_LEN_MSG       = 8192,//fixed by redrock for adjust msg with multiplay
   MPA_LEN_FILENAME  = 128
};


struct ARAudioInfo
{
   int    nCodecID;       // AMP::CODEC_TYPE
   uint   uFrameCnt;  	 // # frames in file
   uint   uPlayTime;  	 // Play Total Time ms
   uint   uSampleRate;
   uint   uBitPerSample;
   uint   uBitRate;
   uint   uTimeScale;        //
   uint   uChannelCnt;
	bool operator==(const ARAudioInfo& ainf)
	{
		if(nCodecID == ainf.nCodecID &&
			uFrameCnt == ainf.uFrameCnt &&
			uPlayTime == ainf.uPlayTime && 
			uSampleRate == ainf.uSampleRate &&
			uBitPerSample == ainf.uBitPerSample &&
			uBitRate == ainf.uBitRate &&
			uTimeScale == ainf.uTimeScale &&
			uChannelCnt == ainf.uChannelCnt)
			return true;
		return false;
	}
   void           hton() {
      nCodecID       = htonl(nCodecID       );
      uFrameCnt      = htonl(uFrameCnt      );
      uPlayTime      = htonl(uPlayTime      );
      uSampleRate    = htonl(uSampleRate    );
      uBitPerSample  = htonl(uBitPerSample  );
      uBitRate       = htonl(uBitRate       );
      uTimeScale     = htonl(uTimeScale     );
      uChannelCnt    = htonl(uChannelCnt    );
   }
   void           ntoh() {
      nCodecID       = ntohl(nCodecID       );
      uFrameCnt      = ntohl(uFrameCnt      );
      uPlayTime      = ntohl(uPlayTime      );
      uSampleRate    = ntohl(uSampleRate    );
      uBitPerSample  = ntohl(uBitPerSample  );
      uBitRate       = ntohl(uBitRate       );
      uTimeScale     = ntohl(uTimeScale     );
      uChannelCnt    = ntohl(uChannelCnt    );
   }
   // for debug
   char * str(char * strBuff, int nBufLen) {
      sprintf(strBuff, "%s %u %u %u %u %u %u %u" ,
         GetCodecTypeName((CODEC_TYPE)nCodecID),
         uFrameCnt, uPlayTime, uSampleRate, uBitPerSample,
         uBitRate, uTimeScale, uChannelCnt);
      return strBuff;
   }
};

struct ARVideoInfo
{
   int    nCodecID;       // AMP::CODEC_TYPE
   uint   uFrameCnt;  	 // # frames in file
   uint   uPlayTime;  	 // Play Totatl time ms
   uint   uFrameRate;		 // # of frames per second * 100
   uint   uBitRate;       // bps
   uint   uTimeScale;		 //
   uint   uWidth;
   uint   uHeight;
	bool operator==(const ARVideoInfo& vinf)
	{
		if(nCodecID == vinf.nCodecID &&
			uFrameCnt == vinf.uFrameCnt &&
			uPlayTime == vinf.uPlayTime &&
			uFrameRate == vinf.uFrameRate &&
			uBitRate == vinf.uBitRate &&
			uTimeScale == vinf.uTimeScale &&
			uWidth == vinf.uWidth &&
			uHeight == vinf.uHeight)
			return true;
		return false;
	}
   void hton() {
      nCodecID       = htonl(nCodecID       );
      uFrameCnt  	   = htonl(uFrameCnt  	  );
      uPlayTime  	   = htonl(uPlayTime  	  );
      uFrameRate		= htonl(uFrameRate	  );
      uBitRate       = htonl(uBitRate       );
      uTimeScale	   = htonl(uTimeScale	  );
      uWidth         = htonl(uWidth         );
      uHeight        = htonl(uHeight        );
   }
   void ntoh() {
      nCodecID       = ntohl(nCodecID       );
      uFrameCnt  	   = ntohl(uFrameCnt  	  );
      uPlayTime  	   = ntohl(uPlayTime  	  );
      uFrameRate		= ntohl(uFrameRate	  );
      uBitRate       = ntohl(uBitRate       );
      uTimeScale	   = ntohl(uTimeScale	  );
      uWidth         = ntohl(uWidth         );
      uHeight        = ntohl(uHeight        );
   }
   // for debug
   char *  str(char * strBuff, int nBufLen) {
      sprintf(strBuff, "%s %u %u %u %u %u %u %u" ,
         GetCodecTypeName((CODEC_TYPE) nCodecID),
         uFrameCnt, uPlayTime, uFrameRate, uBitRate,
         uTimeScale, uWidth, uHeight);
      return strBuff;
   }
};

struct ARMediaInfo
{
   uint   uPlayTime;  	 // Play Total time ms
   ARAudioInfo    stInfoA;
   ARVideoInfo    stInfoV;
   void           hton() {
      uPlayTime       = htonl(uPlayTime);
      stInfoA.hton();
      stInfoV.hton();
   }
   void           ntoh() {
      uPlayTime       = ntohl(uPlayTime);
      stInfoA.ntoh();
      stInfoV.ntoh();
   }
	bool operator==(const ARMediaInfo& minf)
	{
		if(stInfoA == minf.stInfoA &&
			stInfoV == minf.stInfoV)
			return true;
		return false;
	}
	
   char * str(char * strBuff, int nBufLen) {
      char szAudio[128];
      char szVideo[128];

      sprintf(strBuff, "Dur:%u A[%s] V[%s]",
               uPlayTime,
               stInfoA.str(szAudio, sizeof(szAudio)),
               stInfoV.str(szVideo, sizeof(szVideo)));
      return strBuff;
   }
};


struct ARRtpPort
{
   uint uIp;
   uint uRtpPort;   // if 0 , rtp off
   uint uRtcpPort;  // if 0, rtcp off
	// added by MIND 2012.10.05 
   unsigned char ucRtpDscp ; // rtp  dscp code value
   unsigned char ucRtcpDscp; // rtcp dscp code value
   unsigned char reserved1 ;
   unsigned char reserved2 ;
   void hton() {
      uRtpPort    = htonl(uRtpPort);
      uRtcpPort   = htonl(uRtcpPort);
   }
   void ntoh() {
      uRtpPort    = ntohl(uRtpPort);
      uRtcpPort   = ntohl(uRtcpPort);
   }

   char * str(char * strBuff, int nBufLen) {
      uchar * pIp = (uchar *) &uIp;
      sprintf(strBuff, "ip:%d.%d.%d.%d port:%u/%u dscp:%d/%d",
               pIp[0]&0xff, pIp[1]&0xff, pIp[2]&0xff, pIp[3]&0xff,
               uRtpPort,  uRtcpPort,ucRtpDscp,ucRtcpDscp);
      return strBuff;
   }
};

//#define MPA_LEN_CNAME   AMP::CRtpComm::LEN_CNAME //
#define MPA_LEN_CNAME   64

typedef enum {
   PM_DEF = 0,

   // H.263
   PM_H263 =  1, // RFC2190  (default)
   PM_H263P = 2, // RFC2429

   // H.264
   PM_H264_SINGLE = 3,   // Single NAL unit mode (default)
   PM_H264_NON_INT = 4,  // Non-interleaved mode
   PM_H264_INT = 5,      // Interleaved mode

   // AMR(NB,WB)
   PM_AMR_OCTET = 6,     // Octet Align mode
   PM_AMRNB_BAND = 7,    // Bandwidth Efficient mode
   PM_AMRWB_BAND = 8,    // Bandwidth Efficient mode

} PACKETIZATION_MODE;

struct ARRtpParam
{
	enum { E_MAXCNAME= 64 };
   enum { SENDONLY = 1, RECVONLY =2, SENDRECV = 3};
   char         szCName[E_MAXCNAME];//rtcp canonical name
   unsigned int uiPayloadType      ;//rtp payload type
   unsigned int uiPayloadType2     ;//rtp payload type for rfc2833
   unsigned int uiSSRC             ;//rtp ssrc
   unsigned int uiInitSequence     ;//rtp init sequence
   unsigned int uiModeSet          ;//determined mode PT=AMR,0-7 allowed
   unsigned int uiPacketMode       ;//packetizing mode, ref ATM_PACKET_MODE_E
   unsigned int uiStreamMode       ;//steram mode  SENDONLY RECVONLY SENDRECV

   void hton(){
      uiPayloadType   = htonl(uiPayloadType);
      uiPayloadType2  = htonl(uiPayloadType2);
      uiSSRC          = htonl(uiSSRC);
      uiModeSet       = htonl(uiModeSet);
      uiPacketMode    = htonl(uiPacketMode);
      uiStreamMode    = htonl(uiStreamMode);
   }

   void ntoh(){
      uiPayloadType   = ntohl(uiPayloadType);
      uiPayloadType2  = ntohl(uiPayloadType2);
      uiSSRC          = ntohl(uiSSRC);
      uiModeSet       = ntohl(uiModeSet);
      uiPacketMode    = ntohl(uiPacketMode);
      uiStreamMode    = ntohl(uiStreamMode);
   }
   char* str(char* pBuf,int nLen){
      snprintf(pBuf, nLen-1,
               "c:%s, pt:%d/%d, ssrc:%u, mode-set:%d pm:%d,initSeq=%u sm:%d",
               szCName,uiPayloadType,uiPayloadType2,uiSSRC,uiModeSet,uiPacketMode,uiInitSequence,uiStreamMode);
      return pBuf;
   }
};

struct ARMsgHdr
{
   uint uMagicCookie;
   uint uiMsgLen;
   uint uiType;
   uint uiSubType;        // ARES_REQ, ARES_RES, ARES_INFO

   uint uBoardId;        // (set by rtp-agent)
   uint uSesId;          // media : 1~N, else : 0  (set by mpmgr)
   uint uAppsId;         // (set by apps)
   uint uAppsSesId;		      // (set by apps) as like trid
   char szSesDesc[32];
   uint uiReasonCode;				// Request : 0, Info & Response : reason code
	uint uSvcType;        // not define specificly here..define at APPS
	uint uSvcMask;        // RTPM & RTPA don't concern about this val.. only rewrite shminf
	uint uiRerserved[4];    // Reserved for future use

   uchar * data() {
      return (uchar *)(this+1);
   }

   void         hton() {
      uMagicCookie = htonl(uMagicCookie);
      uiMsgLen     = htonl(uiMsgLen);
      uiType    = htonl(uiType);
      uiSubType = htonl(uiSubType);

      uBoardId = htonl(uBoardId);
      uSesId   = htonl(uSesId);
      uAppsId  = htonl(uAppsId);
      uAppsSesId  = htonl(uAppsSesId);
      uiReasonCode     = htonl(uiReasonCode);
		uSvcType = htonl(uSvcType);
		uSvcMask = htonl(uSvcMask);
   }

   void         ntoh() {
      uMagicCookie = ntohl(uMagicCookie);
      uiMsgLen     = ntohl(uiMsgLen);
      uiType    = ntohl(uiType);
      uiSubType = ntohl(uiSubType);

      uBoardId = ntohl(uBoardId);
      uSesId   = ntohl(uSesId);
      uAppsId  = ntohl(uAppsId);
      uAppsSesId  = ntohl(uAppsSesId);
      uiReasonCode     = ntohl(uiReasonCode);
		uSvcType = ntohl(uSvcType);
		uSvcMask = ntohl(uSvcMask);
   }

   char * str(char * strBuff, int nBufLen) {
      char szType[16];
      char szSubType[8];

      switch(uiType) {
      case AMT_INTERNAL      :
         sprintf(szType, "INTERNAL"       );
         break;

      case AMT_SETUP_AGNT         :
         sprintf(szType, "SETUP_AGENT"          );
         break;
      case AMT_SETUP_APPS         :
      case AMT_SETUP_APPS_T       :			
         sprintf(szType, "SETUP_APPS"          );
         break;
      case AMT_ALIVE      :
		case AMT_ALIVE_T      :
         sprintf(szType, "ALIVE"      );
         break;
      case AMT_WARN_SES      :
         sprintf(szType, "WARN_SES"      );
         break;
      case AMT_CLOSED_SES      :
         sprintf(szType, "CLOSED_SES"      );
         break;
      case AMT_START_SES     :
         sprintf(szType, "START_SES"      );
         break;
      case AMT_START_SES_DUMP     :
         sprintf(szType, "START_SES_DUMP" );
         break;
      case AMT_STOP_SES      :
         sprintf(szType, "STOP_SES"      );
         break;
      case AMT_PLAY_READY    :
         sprintf(szType, "PLAY_READY"    );
         break;
      case AMT_PLAY          :
         sprintf(szType, "PLAY"          );
         break;
      case AMT_PLAY_SIMPLE   :
         sprintf(szType, "PLAY_SIMPLE"   );
         break;
      case AMT_PLAY_MULTI   :
         sprintf(szType, "PLAY_MULTI"   );
         break;
      case AMT_PLAY_STOP     :
         sprintf(szType, "PLAY_STOP"     );
         break;
      case AMT_PLAY_DONE     :
         sprintf(szType, "PLAY_DONE"     );
         break;
      case AMT_RECORD        :
         sprintf(szType, "RECORD"        );
         break;
      case AMT_RECORD_STOP   :
         sprintf(szType, "RECORD_STOP"   );
         break;
      case AMT_RELAY         :
         sprintf(szType, "RELAY"         );
         break;
		case AMT_RELAY_TC         :
         sprintf(szType, "RELAY_TC"         );
         break;	
      case AMT_RELAY_STOP    :
         sprintf(szType, "RELAY_STOP"    );
         break;
        case AMT_DTMF_SET   :
            sprintf(szType,"AMT_DTMF_SET"   );
        break;
        case AMT_DTMF_GEN   :
            sprintf(szType,"AMT_DTMF_GEN");
        break;
        case AMT_DTMF_INFO  :
            sprintf(szType,"AMT_DTMF_INFO");
        break;
         case AMT_DTMF_GENINFO :
            sprintf(szType,"AMT_DTMF_GENINFO");
         break;
        case AMT_MM_BLOCK   :
            sprintf(szType,"AMT_MM_BLOCK");
        break;
        case AMT_MM_UNBLOCK :
            sprintf(szType,"AMT_MM_UNBLOCK");
        break;
        case AMT_MM_RESET   :
            sprintf(szType,"AMT_MM_RESET");
        break;
        case AMT_MM_BD_STS  :
            sprintf(szType,"AMT_MM_BD_STS");
        break;
        case AMT_MM_SES_STS :
            sprintf(szType,"AMT_MM_SES_STS");
        break;
        case AMT_MM_BD_INF  :
            sprintf(szType,"AMT_MM_BD_INF");
        break;
        case AMT_MM_SES_INF :
            sprintf(szType,"AMT_MM_SES_INF");
        break;
		  case AMT_MM_RM_INF :
            sprintf(szType,"AMT_MM_RM_INF");
        break;	
        case AMT_MM_CLEAR   :
            sprintf(szType,"AMT_MM_CLEAR");
        break;
			case AMT_NETFAIL	:
				sprintf(szType,"AMT_NETFAIL");
			break;
			case AMT_ALIVE_CHECK :
				sprintf(szType,"AMT_ALIVE_CHECK");
			break;
      default:
         sprintf(szType, "Unknown(%u)", uiType);
         break;
      }

      switch(uiSubType) {
	   case AMST_INFO:
         sprintf(szSubType, "INF");
         break;

	   case AMST_REQ:
         sprintf(szSubType, "REQ");
         break;
	   case AMST_RES:
         sprintf(szSubType, "RES");
         break;

      default:
         sprintf(szSubType, "Unknown(%u)", uiSubType);
         break;
      }

      sprintf(strBuff, "[LEN:%u][DESC:%s][MSG:%s-%s][A:%d_S:%d B:%d_S:%u][res:%#x(%s)][svc:0x%x mask:0x%x]"
         , uiMsgLen,szSesDesc, szType, szSubType, uAppsId, uAppsSesId, uBoardId, uSesId, uiReasonCode, MPReason::str(uiReasonCode),uSvcType,uSvcMask);
      return strBuff;
   }

};


struct ARSetupAgent
{
   uint uSessionCnt;
	uint uGroupCnt;
   void        hton() {
      uSessionCnt = htonl(uSessionCnt);
		uGroupCnt = htonl(uGroupCnt);
   }
   void        ntoh() {
      uSessionCnt = ntohl(uSessionCnt);
		uGroupCnt = ntohl(uGroupCnt);
   }

   char * str(char * strBuff, int nBufLen) {
      sprintf(strBuff, "[SES_CNT:%d,GRP_CNT:%d]", uSessionCnt,uGroupCnt);
      return strBuff;
   }
};


//struct ARSetupApps  (no body)

//--dtmf relate

struct AMRDtmf {
   enum DTMF_TYPE{
      DT_NONE     = 0x0000,
      DT_INBAND   = 0x0001,
      DT_RFC2833  = 0x0002,
      DT_ALL      = 0x00FF,
   };

   enum DTMF_CODE{
      DTMF_0            =  0,
      DTMF_1            =  1,
      DTMF_2            =  2,
      DTMF_3            =  3,
      DTMF_4            =  4,
      DTMF_5            =  5,
      DTMF_6            =  6,
      DTMF_7            =  7,
      DTMF_8            =  8,
      DTMF_9            =  9,
      DTMF_STAR         = 10, // *
      DTMF_PND          = 11, // #
      DTMF_A            = 12,
      DTMF_B            = 13,
      DTMF_C            = 14,
      DTMF_D            = 15,
      DTMF_C11          = 16, // +C11       700 + 1700
      DTMF_C12          = 17, // +C12       900
      DTMF_KP1          = 18, // KP1+       1100
      DTMF_KP2          = 19, // KP2+       1300
      DTMF_ST           = 20, // +ST        1500
      DTMF_24           = 21, // 2400
      DTMF_26           = 22, // 2600
      DTMF_2426         = 23, // 2400+2600
      DTMF_DT           = 24, // DialTone.  350 440
      DTMF_RING         = 25, // Ring       440 480
      DTMF_BUSY         = 26, // Busy       480 620
      DTMF_SIL          = 27, // Silence
      DTMF_NONE         = 28, // Nothing.
      TONE_DIAL         = 29, // ToneDial
      TONE_RINGBACK     = 30, // ToneRingback
      TONE_BUSY         = 31, // ToneBusy
      TONE_CONGESTION   = 32, // ToneCongestion
      TONE_WAITING      = 33, // ToneWaiting
      TONE_HOLDING      = 34, // ToneHolding
      TONE_INTERCEPT    = 35, // ToneIntercept
      TONE_SPECIALDIAL  = 36, // ToneSpecialDial
      TONE_CONFIRM      = 37, // ToneConfirm
      TONE_HOLWER       = 38, // ToneHowler
      TONE_SIT_RO1      = 39, // ToneSITReorder1
      TONE_SIT_VC       = 40, // ToneSITVacantCode
      TONE_SIT_NC1      = 41, // ToneSITNoCircuit1
      TONE_SIT_IC       = 42, // ToneSITIntercept
      TONE_SIT_RO2      = 43, // ToneSITReorder2
      TONE_SIT_NC2      = 44, // ToneSITNoCircuit2
      TONE_SIT_IO       = 45, // ToneSITIneffectiveOther
      TONE_END          = 46, // End of tone.
   };
};


struct ARDtmfSetSts
{
   ushort uEnable;
   ushort uTimeScale;
    void hton(){
      uEnable = htons(uEnable);
      uTimeScale = htons(uTimeScale);
    }
    void ntoh(){
      uEnable = ntohs(uEnable);
      uTimeScale = ntohs(uTimeScale);
    }
    char* str(char* strBuff,int nBuffLen)
    {
        sprintf(strBuff,"enable:%u ts:%u",uEnable,uTimeScale);
        return strBuff;
    }
};

enum AR_DTMFSET_BTYPE
{
    SSBT_DTMF_NONE = 0x0,
    SSBT_DTMFSET_NOTI = 0x1,
    SSBT_DTMFSET_GEN = 0x2,
    SSBT_DTMFSET_ALL = 0x3
};

inline int GetGId(uint uGroupMask)
//return index first find bit(val:1)
{
	int idx=-1;
	int nWhereBit = 0x1;
	for(uint i=0;i<MAX_GROUP_PER_SES;i++)
	{
		if((uGroupMask & (nWhereBit<<i)))
		{
			idx = i;break;
		}
	}
	return idx;
}

inline int GetGNum(uint uGroupMask)
//return nums of setted group index in gmask
{
	uint num=0;
	int nWhereBit = 0x1;
	for(uint i=0;i<MAX_GROUP_PER_SES;i++)
	{
		if((uGroupMask & (nWhereBit<<i)))
		{
			num++;
		}
	}
	return num;
}

struct ARDtmfSetReq
{
   uint uGroupMask;
	uint uiType[MAX_GROUP_PER_SES];//DTMF_TYPE AMRDtmf:: [ DT_INBAND , DT_RFC2833 ]
   //uint uBodyType;//AR_DTMFSET_BTYPE [ SSBT_DTMFSET_NOTI , SSBT_DTMFSET_GEN , SSBT_DTMFSET_ALL ]
   //ARRtpPort         rtpDstPortA[MAX_GROUP_PER_SES];       // rtp/rtcp remote audio receiver ip/port
   //ARRtpParam        rtpParamA[MAX_GROUP_PER_SES];//all group have same parm currently
   ARDtmfSetSts det[MAX_GROUP_PER_SES];
   ARDtmfSetSts gen[MAX_GROUP_PER_SES];
    void hton(){
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++,nWhereBit<<=1)
      {
         if((uGroupMask & nWhereBit)==nWhereBit)
         {
//            rtpDstPortA[i].hton();
//				rtpParamA[i].hton();
				uiType[i] = htonl(uiType[i]);
				det[i].hton();
				gen[i].hton();
        	}
      }
        
        
      uGroupMask = htonl(uGroupMask);
    }
    void ntoh(){
      uGroupMask = htonl(uGroupMask);
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if((uGroupMask & (nWhereBit<<i)))
        	{
//            rtpDstPortA[i].ntoh();
//		      rtpParamA[i].ntoh();
				uiType[i] = ntohl(uiType[i]);
				det[i].ntoh();
		      gen[i].ntoh();
         }
      }
    }
    char* str(char* strBuff,int nBuffLen)
    {
      char szBuf[128];
      char szTmpBuf[128];
      char szRtpPortA[1024];
      char szRtpParamA[1024];
		char szDtmfDet[1024];
		char szDtmfGen[1024];
		char szType[1024];
		szBuf[0] = '\0';
		szTmpBuf[0] = '\0';
		szRtpPortA[0] = '\0';
		szRtpParamA[0] = '\0';
		szDtmfDet[0] = '\0';
		szDtmfGen[0] = '\0';
		szType[0] = '\0';
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++,nWhereBit<<=1)
      {
         if((uGroupMask & nWhereBit)==nWhereBit)
         {
/*            szTmpBuf[0] = '\0';
            rtpDstPortA[i].str(szTmpBuf,128);
            strcat(szRtpPortA,szTmpBuf);
            strcat(szRtpPortA," ");
				szTmpBuf[0] = '\0';
            rtpParamA[i].str(szTmpBuf,128);
            strcat(szRtpParamA,szTmpBuf);
            strcat(szRtpParamA," ");*/
   			      
				szTmpBuf[0] = '\0';
				if(uiType[i] == AMRDtmf::DT_RFC2833)
					sprintf(szTmpBuf,"[%d]r2833 ",i);
				else if(uiType[i] == AMRDtmf::DT_INBAND)
					sprintf(szTmpBuf,"[%d]inband ",i);
				else
					sprintf(szTmpBuf,"[%d]undef %d ",i,uiType[i]);
				strcat(szType,szTmpBuf);
		  		szTmpBuf[0] = '\0';
				sprintf(szTmpBuf,"[%d]",i);
	         det[i].str(szTmpBuf+strlen(szTmpBuf),128);
	         strcat(szDtmfDet,szTmpBuf);
	         strcat(szDtmfDet," ");

				szTmpBuf[0] = '\0';
				if(uiType[i] == AMRDtmf::DT_RFC2833)
					sprintf(szTmpBuf,"[%d]r2833 ",i);
				else if(uiType[i] == AMRDtmf::DT_INBAND)
					sprintf(szTmpBuf,"[%d]inband ",i);
				else
					sprintf(szTmpBuf,"[%d]undef %d ",i,uiType[i]);
	         szTmpBuf[0] = '\0';
				sprintf(szTmpBuf,"[%d]",i);
	         gen[i].str(szTmpBuf+strlen(szTmpBuf),128);
				strcat(szDtmfGen,szTmpBuf);
	         strcat(szDtmfGen," ");
	      
         }
      }
		sprintf(strBuff,"GMASK:0x%x type:%s [det:%s] [gen:%s]",// [rtp:%s opt:%s]",
			uGroupMask,szType,szDtmfDet,szDtmfGen);//,szRtpPortA,szRtpParamA);
        return strBuff;
    }
};

//-- rtp dump relate
enum AR_DUMPSET_BTYPE
{
    SSBT_DUMP_NONE = 0x0,
    SSBT_DUMP_RELAY = 0x1,
    SSBT_DUMP_FILE = 0x2,
    SSBT_DUMP_ALL = SSBT_DUMP_RELAY | SSBT_DUMP_FILE,
	 SSBT_DUMP_MAX
};

struct ARDumpSetReq
{
	ARDumpSetReq() { memset(this,0x00,sizeof(ARDumpSetReq)); }
   uint uGroupMask;//group mask set dump..
	uint uBodyType;
	ARRtpPort rtpSrcPortA[MAX_GROUP_PER_SES];//in media
	ARRtpPort rtpSrcPortV[MAX_GROUP_PER_SES];
	ARRtpPort rtpDstPortA[MAX_GROUP_PER_SES];//out media
	ARRtpPort rtpDstPortV[MAX_GROUP_PER_SES];
	char szDumpFileA[MPA_LEN_FILENAME];//this is base name & make full name with remain info(ex. gid, media type,src ip port)
	char szDumpFileV[MPA_LEN_FILENAME];
	void hton(){
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++,nWhereBit<<=1)
      {
         if((uGroupMask & nWhereBit)==nWhereBit)
         {
            rtpDstPortA[i].hton();
            rtpDstPortV[i].hton();
         }
      }
      uGroupMask = htonl(uGroupMask);
      uBodyType = htonl(uBodyType);
	}
	void ntoh(){
      uGroupMask = ntohl(uGroupMask);
      uBodyType = ntohl(uBodyType);
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++,nWhereBit<<=1)
      {
         if((uGroupMask & nWhereBit)==nWhereBit)
         {
            rtpDstPortA[i].ntoh();
            rtpDstPortV[i].ntoh();
         }
      }

	}
    char* str(char* strBuff,int nBuffLen)
    {
      //char szBuf[128];
      char szTmpBuf[128];
      char szTmpBuf2[128];
      char szRtpDstPortA[256];
      char szRtpDstPortV[256];
      char szDumpFileGroup[256];
		szRtpDstPortA[0] = '\0';
		szRtpDstPortV[0] = '\0';
		szDumpFileGroup[0] = '\0';
      int nWhereBit = 0x1;
        switch(uBodyType&0xf)
        {
        case SSBT_DUMP_RELAY:
            for(uint i=0;i<MAX_GROUP_PER_SES;i++,nWhereBit<<=1)
            {
               if((uGroupMask &nWhereBit)==nWhereBit)
               {
                  szTmpBuf[0]='\0';
                  szTmpBuf2[0]='\0';
                  sprintf(szTmpBuf2,"[%d]%s ",i,rtpDstPortA[i].str(szTmpBuf,128));
                  strcat(szRtpDstPortA,szTmpBuf2);
                  szTmpBuf[0]='\0';
                  szTmpBuf2[0]='\0';
                  sprintf(szTmpBuf2,"[%d]%s ",i,rtpDstPortV[i].str(szTmpBuf,128));
                  strcat(szRtpDstPortV,szTmpBuf2);
               }
            }
            sprintf(strBuff,"[dump_relay] [a:%s v:%s]",
               szRtpDstPortA,szRtpDstPortV);
        break;
        case SSBT_DUMP_FILE:
            for(uint i=0;i<MAX_GROUP_PER_SES;i++,nWhereBit<<=1)
            {
               if((uGroupMask &nWhereBit)==nWhereBit)
               {

                  sprintf(szTmpBuf,"[%d]",i);
                  strcat(szDumpFileGroup,szTmpBuf);
               }
            }
				sprintf(strBuff," [dump_file g:%s a:%s v:%s]",
               szDumpFileGroup,szDumpFileA,szDumpFileV);
        break;
        case SSBT_DUMP_ALL:
            for(uint i=0;i<MAX_GROUP_PER_SES;i++,nWhereBit<<1)
            {
               if((uGroupMask &nWhereBit)==nWhereBit)
               {
                  szTmpBuf[0]='\0';
                  szTmpBuf2[0]='\0';
                  sprintf(szTmpBuf2,"[%d]%s ",i,rtpDstPortA[i].str(szTmpBuf,128));
                  strcat(szRtpDstPortA,szTmpBuf2);
                  szTmpBuf[0]='\0';
                  szTmpBuf2[0]='\0';
                  sprintf(szTmpBuf2,"[%d]%s ",i,rtpDstPortV[i].str(szTmpBuf,128));
                  strcat(szRtpDstPortV,szTmpBuf2);
               }
            }
				sprintf(strBuff," [dump_all a:%s %s v:%s %s]",
               szDumpFileA,szRtpDstPortA,szDumpFileV,szRtpDstPortV);

        break;
        default:
        break;
        }
        return strBuff;
    }
};

struct ConfParmElm_t
{
	enum{
		MAX_STR_LEN=32
	};
	char valid;
	char sec[MAX_STR_LEN];
	char name[MAX_STR_LEN];
	char val[MAX_STR_LEN];
	char type;
};

struct ConfParm_t
{
	enum{
		MAX_ELM_NUM=20,
		MAX_STR_LEN=32
	};
	enum{
		VALTYPE_NONE=0,
		VALTYPE_INT ,
		VALTYPE_FLOAT,
		VALTYPE_BOOL,
		VALTYPE_STR
	};
	uint uiNum;
	ConfParmElm_t elm[MAX_ELM_NUM];

	void setelm(int idx,char* sec,char* name,char* val,int type)
	{
		if(idx >= MAX_ELM_NUM) return;
		elm[idx].valid = 1;
		if(sec) strncpy(elm[idx].sec,sec,MAX_STR_LEN);
		if(name) strncpy(elm[idx].name,name,MAX_STR_LEN);
		if(val) strncpy(elm[idx].val,val,MAX_STR_LEN);
		elm[idx].type = type;
		int num=0;
		for(int i=0;i<MAX_ELM_NUM;i++) if(elm[i].valid) num++;
		uiNum = num;
	}
	void setelmI(int idx,char* sec,char* name,int val)
	{
		char szval[128];
		sprintf(szval,"%d",val);
		setelm(idx,sec,name,szval,VALTYPE_INT);
	}
	void setelmF(int idx,char* sec,char* name,float val)
	{
		char szval[128];
		sprintf(szval,"%f",val);
		setelm(idx,sec,name,szval,VALTYPE_FLOAT);
	}

	void hton()
	{
		uiNum = htonl(uiNum);	
	}
	void ntoh()
	{
		uiNum = ntohl(uiNum);
	}
	char* str(char* buf,int len)
	{
		char sztmp[256];
		int num=(uiNum>MAX_ELM_NUM?MAX_ELM_NUM:uiNum);
		sprintf(buf,"[CONF-num:%d-",uiNum);
		for(int i=0;i<num;i++)
		{
			sprintf(sztmp,"(%02d)%s:%s:%s:",i,elm[i].sec,elm[i].name,elm[i].val); strcat(buf,sztmp);
			switch(elm[i].type)
			{
			case VALTYPE_INT:
				strcat(buf,"int");
			break;
			case VALTYPE_FLOAT:
				strcat(buf,"float");
			break;
			case VALTYPE_BOOL:
				strcat(buf,"bool");
			break;
			case VALTYPE_STR:
				strcat(buf,"str");
			break;
			default:
				strcat(buf,"none");
			break;
			}
		}
		strcat(buf,"]");
		return buf;
	}
};

struct ARStartSessionReq
{
	enum { MAX_MIN_LEN=32};
    uint uGroupNum;               //reserve group num:(1 group per 1 direction)
                                          //reserve max group num for specific service
                                          // at here
    //ARDtmfSetReq dtmfset;			//remove dtmfset from startsession, and add at relay operation
	SVCTrace_t trace;
	char Caller[MAX_MIN_LEN];
	char Callee[MAX_MIN_LEN];
	ConfParm_t conf;
    void hton(){
      uGroupNum = htonl(uGroupNum);
		trace.hton();
		conf.hton();
      //dtmfset.hton();
	}
	   void ntoh(){
      uGroupNum = ntohl(uGroupNum);
		trace.ntoh();
		conf.ntoh();
      //dtmfset.ntoh();
      }
    char *str(char *strBuff,int nBuffLen)
    {
         //char szDtmfSet[2048];
         //szDtmfSet[0] = '\0';
         //dtmfset.str(szDtmfSet,2048);
			char sztrace[1024];sztrace[0]='\0';
			trace.str(sztrace,1024);
			char szconf[2048];szconf[0]='\0';
			conf.str(szconf,2048);
         sprintf(strBuff,"gnum:%d trc:%s caller:%s callee:%s %s",
				uGroupNum,sztrace,Caller,Callee,szconf);
        return strBuff;
    }
};

struct ARStartSessionDumpReq
{
	enum { MAX_MIN_LEN=32};
    int uGroupNum;
    //ARDtmfSetReq dtmfset;
	SVCTrace_t trace;
	 ARDumpSetReq dumpset;
	char Caller[MAX_MIN_LEN];
	char Callee[MAX_MIN_LEN];
    void hton(){
      uGroupNum = htonl(uGroupNum);
      //dtmfset.hton();
		trace.hton();
      dumpset.hton();}
    void ntoh(){
      uGroupNum = ntohl(uGroupNum);
      //dtmfset.ntoh();
		trace.ntoh();
      dumpset.ntoh();}
    char *str(char *strBuff,int nBuffLen)
    {
         //char szDtmfSet[2048];
			char szDumpSet[2048];
         //szDtmfSet[0] = '\0';
         szDumpSet[0] = '\0';
			//dtmfset.str(szDtmfSet,2048);
			char sztrace[1024];sztrace[0]='\0';
			trace.str(sztrace,1024);
			dumpset.str(szDumpSet,2048);
         sprintf(strBuff,"gnum:%d %s trc:%s caller:%s callee:%s",
				uGroupNum,szDumpSet,sztrace,Caller,Callee);
        return strBuff;
    }
};

struct ARDtmfInfo//AMT_DTMF_INFO & AMT_DTMF_GEN use this struct for body
{
    uint uGroupMask;
	 uint uiType;//DTMF_TYPE ARDtmf:: [ DT_INBAND , DT_RFC2833 ]
    ushort usBodyType;//AR_DTMFSET_BTYPE [ SSBT_DTMFSET_NOTI , SSBT_DTMFSET_GEN , SSBT_DTMFSET_ALL ]
    ushort usDigit;
    ushort usDuration;
    ushort usPower;
    void hton(){
		  uiType = htonl(uiType);
        uGroupMask = htonl(uGroupMask);
        usBodyType = htons(usBodyType);
        usDigit = htons(usDigit);
        usDuration = htons(usDuration);
        usPower = htons(usPower);
    }
    void ntoh(){
        uGroupMask = ntohl(uGroupMask);
		  uiType = ntohl(uiType);
        usBodyType = ntohs(usBodyType);
        usDigit = ntohs(usDigit);
        usDuration = ntohs(usDuration);
        usPower = ntohs(usPower);
    }
   char * str(char * strBuff, int nBufLen) {
      sprintf(strBuff, "GMASK:0x%x type:%d btype:%d digit:%d duration:%d power:%d",
        uGroupMask,uiType,usBodyType,usDigit,usDuration,usPower);
      return strBuff;
   }
};

struct ARStartSessionRS
{
   //if res ==0
   uint uGroupNum; //Reserved Group Num
   ARRtpPort rtpPortA[MAX_GROUP_PER_SES];
   ARRtpPort rtpPortV[MAX_GROUP_PER_SES];
/*
   ARRtpPort rtpSndPortA;
   ARRtpPort rtpSndPortV;

   ARRtpPort rtpRcvPortA;
   ARRtpPort rtpRcvPortV;
*/
   void hton() {
      for(uint i=0;i<uGroupNum;i++)
      {
         rtpPortA[i].hton();
         rtpPortV[i].hton();

      }
      uGroupNum = htonl(uGroupNum);
   }
   void ntoh() {
      uGroupNum = ntohl(uGroupNum);
      for(uint i=0;i<uGroupNum;i++)
      {
         rtpPortA[i].ntoh();
         rtpPortV[i].ntoh();
      }
   }

   char * str(char * strBuff, int nBufLen) {
      char szRtpPortA[2048];
      char szRtpPortV[2048];
      char szTmpBuf[128];
      char szTmpBuf2[128];
      szRtpPortA[0] = '\0';
      szRtpPortV[0] = '\0';
      for(uint i=0;i<uGroupNum;i++)
      {
         szTmpBuf[0] = '\0';
         szTmpBuf2[0] = '\0';
         rtpPortA[i].str(szTmpBuf,128);
         sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
         strcat(szRtpPortA,szTmpBuf2);

         szTmpBuf[0] = '\0';
         szTmpBuf2[0] = '\0';
         rtpPortV[i].str(szTmpBuf,128);
         sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
         strcat(szRtpPortV,szTmpBuf2);
      }
      sprintf(strBuff, "gnum:%d [A:%s V:%s]",uGroupNum,szRtpPortA,szRtpPortV);
      return strBuff;
   }
};

struct ARStat
{
	uint uiASendPackets;
	uint uiASendBytes;
	uint uiVSendPackets;
	uint uiVSendBytes;
	uint uiARecvPackets;
	uint uiARecvBytes;
	uint uiVRecvPackets;
	uint uiVRecvBytes;
	uint uiAErrPayloadPackets;
	uint uiVErrPayloadPackets;
	void hton() {
		uiASendPackets = htonl(uiASendPackets);
		uiASendBytes = htonl(uiASendBytes);
		uiVSendPackets = htonl(uiVSendPackets);
		uiVSendBytes = htonl(uiVSendBytes);
		uiARecvPackets = htonl(uiARecvPackets);
		uiARecvBytes = htonl(uiARecvBytes);
		uiVRecvPackets = htonl(uiVRecvPackets);
		uiVRecvBytes = htonl(uiVRecvBytes);
		uiAErrPayloadPackets = htonl(uiAErrPayloadPackets);
		uiVErrPayloadPackets = htonl(uiVErrPayloadPackets);
	}
	void ntoh() {
			uiASendPackets = ntohl(uiASendPackets);
			uiASendBytes = ntohl(uiASendBytes);
			uiVSendPackets = ntohl(uiVSendPackets);
			uiVSendBytes = ntohl(uiVSendBytes);
			uiARecvPackets = ntohl(uiARecvPackets);
			uiARecvBytes = ntohl(uiARecvBytes);
			uiVRecvPackets = ntohl(uiVRecvPackets);
			uiVRecvBytes = ntohl(uiVRecvBytes);
			uiAErrPayloadPackets = ntohl(uiAErrPayloadPackets);
			uiVErrPayloadPackets = ntohl(uiVErrPayloadPackets);
	}
	char* str(char* buf,int len) {
		snprintf(buf,len-1,"asnd[pkt:%d bt:%d] vsnd[pkt:%d bt:%d] arcv[pkt:%d bt:%d] vrcv[pkt:%d bt:%d]"
			" errpt[a:%d v:%d]",
			uiASendPackets,uiASendBytes,uiVSendPackets,uiVSendBytes,uiARecvPackets,uiARecvBytes,
			uiVRecvPackets,uiVRecvBytes,uiAErrPayloadPackets,uiVErrPayloadPackets);
		return buf;
	}

};

struct ARStopSessionRS
{
   uint uGroupNum; //Reserved Group Num
	ARStat stat[MAX_GROUP_PER_SES];
   void hton() {
		for(uint i=0;i<uGroupNum;i++)
			stat[i].hton();
      uGroupNum = htonl(uGroupNum);
		
   }
   void ntoh() {
      uGroupNum = ntohl(uGroupNum);
		for(uint i=0;i<uGroupNum;i++)
			stat[i].ntoh();
   }

   char * str(char * buf, int len) {
		char szbuf[1024];
		char szbuf2[128];
		sprintf(buf,"STAT[ ");
		for(uint i=0;i<uGroupNum;i++)
		{
			stat[i].str(szbuf2,1024);
			sprintf(szbuf,"G[%d] %s ",i,szbuf2);
			strcat(buf,szbuf);
		}
		strcat(buf,"]");
      return buf;
   }
};

struct ARPlayReady
{
   uint              uGroupMask;
   int               nFileFmt; // HAVC_TYPE
   char              szFileA[MPA_LEN_FILENAME];
   char              szDefFileA[MPA_LEN_FILENAME];
   char              szFileV[MPA_LEN_FILENAME];
   char              szDefFileV[MPA_LEN_FILENAME];
   void              hton() {
      uGroupMask = htonl(uGroupMask);
      nFileFmt = htonl(nFileFmt);
   }
   void              ntoh() {
      uGroupMask = htonl(uGroupMask);
      nFileFmt = ntohl(nFileFmt);
   }

   char * str(char * strBuff, int nBufLen) {
      sprintf(strBuff, "[GMASK:0x%x FMT:%d(%s) AFILE:%s ADEF:%s VFILE:%s VDEF:%s]",
         uGroupMask,nFileFmt, GetAVCTypeName(HAVC_TYPE(nFileFmt)),szFileA,
         szFileV,szDefFileA,szDefFileV);
      return strBuff;
   }
};

struct ARPlayReadyRS
{
   //if uiReasonCode == 0
   uint              uGroupMask;
   int               nFileFmt; // HAVC_TYPE
   ARMediaInfo       media;
   void              hton() {
      uGroupMask = htonl(uGroupMask);
      nFileFmt = htonl(nFileFmt);
      media.hton();
   }
   void              ntoh() {
      uGroupMask = ntohl(uGroupMask);
      nFileFmt = ntohl(nFileFmt);
      media.ntoh();
   }

   char * str(char * strBuff, int nBufLen) {
      char szMedia[256];
      sprintf(strBuff, "[GMASK:0x%x FMT:%d(%s) %s]",
         uGroupMask,nFileFmt,GetAVCTypeName(HAVC_TYPE(nFileFmt)), media.str(szMedia, sizeof(szMedia)));
      return strBuff;
   }
};

struct ARPlay
{
   uint     uGroupMask;
   uint     uReplayCount;
   uint     uSpeed;            // 50 => 0.5X , 100 => 1X, 200 => 2X(Not support)
   uint     uPosMs;            // Seek(Play position) (Not support)
   uint     uPlayTimeMs;       // Not support

   uint     uEnableA;
   ARRtpPort         rtpDstPortA[MAX_GROUP_PER_SES];       // rtp/rtcp remote audio receiver ip/port
   ARRtpParam        rtpParamA;
   uint     uEnableV;
   ARRtpPort         rtpDstPortV[MAX_GROUP_PER_SES];       //  rtp/rtcp remote Video receiver ip/port
   ARRtpParam        rtpParamV;
	//ARDumpSetReq		dumpset;
   void              hton() {
      uReplayCount   = htonl(uReplayCount);
      uSpeed         = htonl(uSpeed);
      uPosMs         = htonl(uPosMs);
      uPlayTimeMs    = htonl(uPlayTimeMs);
      uEnableA       = htonl(uEnableA);
      rtpParamA.hton();
      uEnableV = htonl(uEnableV);
      rtpParamV.hton();
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if((uGroupMask & (nWhereBit<<i)))
         {
            rtpDstPortA[i].hton();
            rtpDstPortV[i].hton();
         }
      }
      uGroupMask     = htonl(uGroupMask);
   }
   void              ntoh() {
      uGroupMask     = ntohl(uGroupMask);
      uReplayCount   = ntohl(uReplayCount);
      uSpeed         = ntohl(uSpeed);
      uPosMs         = ntohl(uPosMs);
      uPlayTimeMs    = ntohl(uPlayTimeMs);
      uEnableA       = ntohl(uEnableA);
      rtpParamA.ntoh();
      uEnableV = ntohl(uEnableV);
      rtpParamV.ntoh();
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if((uGroupMask & (nWhereBit<<i)))
         {
            rtpDstPortA[i].ntoh();
            rtpDstPortV[i].ntoh();
         }
      }
   }

   char * str(char * strBuff, int nBufLen) {
      char szRtpPortA[2048];
      char szRtpPortV[2048];
      char szRtpParamA[128];
      char szRtpParamV[128];
      char szTmpBuf[128];
      char szTmpBuf2[128];
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if(uGroupMask &(nWhereBit <<i))
         {
            szTmpBuf[0] = '\0';
            szTmpBuf2[0] = '\0';
            rtpDstPortA[i].str(szTmpBuf,128);
            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
            strcat(szRtpPortA,szTmpBuf2);

            szTmpBuf[0] = '\0';
            szTmpBuf2[0] = '\0';
            rtpDstPortV[i].str(szTmpBuf,128);
            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
            strcat(szRtpPortV,szTmpBuf2);
         }
      }
      sprintf(strBuff, "GMASK=0x%x [loop:%u speed:%u pos:%u ptime:%u A(on:%d drtp:(%s) rtp_opt:(%s)) V(on:%d drtp:(%s) rtp_opt:(%s))]",
              uGroupMask,uReplayCount, uSpeed, uPosMs, uPlayTimeMs,
              uEnableA, szRtpPortA, rtpParamA.str(szRtpParamA, 128),
              uEnableV, szRtpPortV, rtpParamV.str(szRtpParamV, 128));
      return strBuff;
   }
};

struct ARPlaySimple  // PlayReady + Play
{
   uint              uGroupMask;
   int               nFileFmt; // HAVC_TYPE
   char              szFileA[MPA_LEN_FILENAME];
   char              szDefFileA[MPA_LEN_FILENAME];
   char              szFileV[MPA_LEN_FILENAME];
   char              szDefFileV[MPA_LEN_FILENAME];

   uint      uReplayCount;
   uint      uSpeed;            // 50 => 0.5X , 100 => 1X, 200 => 2X(Not support)
   uint      uPosMs;            // Seek(Play position) (Not support)
   uint      uPlayTimeMs;       // Not support

   uint      uEnableA;
   ARRtpPort         rtpDstPortA[MAX_GROUP_PER_SES];       // rtp/rtcp remote audio receiver ip/port
   ARRtpParam        rtpParamA ;
   uint      uEnableV;
   ARRtpPort         rtpDstPortV[MAX_GROUP_PER_SES];       //  rtp/rtcp remote Video receiver ip/port
   ARRtpParam        rtpParamV;
	//ARDumpSetReq		dumpset;
   void              hton() {
      nFileFmt    = htonl(nFileFmt);

      uReplayCount = htonl(uReplayCount);

      uSpeed      = htonl(uSpeed);
      uPlayTimeMs = htonl(uPlayTimeMs);

      uEnableA = htonl(uEnableA);
      rtpParamA.hton();


      uEnableV = htonl(uEnableV);
      rtpParamV.hton();
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if((uGroupMask & nWhereBit<<i))
         {
            rtpDstPortA[i].hton();
            rtpDstPortV[i].hton();
         }
      }
      uGroupMask = htonl(uGroupMask);
   }
   void              ntoh() {
      uGroupMask = ntohl(uGroupMask);
      nFileFmt    = ntohl(nFileFmt);
      uReplayCount = ntohl(uReplayCount);
      uSpeed      = ntohl(uSpeed);
      uPlayTimeMs = ntohl(uPlayTimeMs);
      uEnableA = ntohl(uEnableA);
      rtpParamA.ntoh();
      uEnableV = ntohl(uEnableV);
      rtpParamV.ntoh();
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if(uGroupMask & (nWhereBit<<i))
         {
            rtpDstPortA[i].ntoh();
            rtpDstPortV[i].ntoh();
         }
      }
   }

   char * str(char * strBuff, int nBufLen) {
      char szRtpPortA[2048];
      char szRtpPortV[2048];
      char szRtpParamA[128];
      char szRtpParamV[128];
      char szTmpBuf[128];
      char szTmpBuf2[128];
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if(uGroupMask &(nWhereBit <<i))
         {
            szTmpBuf[0] = '\0';
            szTmpBuf2[0] = '\0';
            rtpDstPortA[i].str(szTmpBuf,128);
            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
            strcat(szRtpPortA,szTmpBuf2);

            szTmpBuf[0] = '\0';
            szTmpBuf2[0] = '\0';
            rtpDstPortV[i].str(szTmpBuf,128);
            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
            strcat(szRtpPortV,szTmpBuf2);
         }
      }
      sprintf(strBuff,
              "GMASK:0x%x [FMT:%d(%s) AFILE:%s ADEF:%s VFILE:%s VDEF:%s "
              "loop:%u speed:%u pos:%u ptime:%u "
              "A(on:%d drtp:(%s) rtp_opt:(%s)) "
              "V(on:%d drtp:(%s) rtp_opt:(%s))]",
              uGroupMask,nFileFmt, GetAVCTypeName(HAVC_TYPE(nFileFmt)),szFileA==NULL?"null":szFileA,szDefFileA==NULL?"null":szDefFileA,
              szFileV==NULL?"null":szFileV,szDefFileV==NULL?"null":szDefFileV,uReplayCount, uSpeed, uPosMs, uPlayTimeMs,
              uEnableA, szRtpPortA,rtpParamA.str(szRtpParamA,128),
              uEnableV, szRtpPortV,rtpParamV.str(szRtpParamV,128));
      return strBuff;
   }
};


struct ARPlayMulti  // PlayReady + Play
{
   uint     uGroupMask;
   int      nFileFmt; // HAVC_TYPE
   uint     uFileCount;
	uint 		uReplayCount[MULTIPLAY_MAXFILENUM];
   uint     uSpeed;            // 50 => 0.5X , 100 => 1X, 200 => 2X(Not support)
   uint     uPosMs;            // Seek(Play position) (Not support)
   uint     uPlayTimeMs;       // Not support

   uint      uEnableA;
   ARRtpPort         rtpDstPortA[MAX_GROUP_PER_SES];       // rtp/rtcp remote audio receiver ip/port
   ARRtpParam        rtpParamA ;

   uint      uEnableV;
   ARRtpPort         rtpDstPortV[MAX_GROUP_PER_SES];       //  rtp/rtcp remote Video receiver ip/port
   ARRtpParam        rtpParamV;
													// this msg followed by ARPlayMulti_FileList for filecount amount
	//ARDumpSetReq		dumpset;
   void              hton() {
      nFileFmt    = htonl(nFileFmt);

      uFileCount = htonl(uFileCount);
		for(int i=0;i<MULTIPLAY_MAXFILENUM;i++)
			uReplayCount[i] = htonl(uReplayCount[i]);
      uSpeed      = htonl(uSpeed);
      uPlayTimeMs = htonl(uPlayTimeMs);

      uEnableA = htonl(uEnableA);
      rtpParamA.hton();
      uEnableV = htonl(uEnableV);
      rtpParamV.hton();
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if(uGroupMask & (nWhereBit<<i))
         {
            rtpDstPortA[i].hton();
            rtpDstPortV[i].hton();
         }
      }
      uGroupMask = htonl(uGroupMask);
   }
   void              ntoh() {
      uGroupMask = ntohl(uGroupMask);
      nFileFmt    = ntohl(nFileFmt);
      uFileCount = ntohl(uFileCount);
		for(int i=0;i<MULTIPLAY_MAXFILENUM;i++)
			uReplayCount[i] = ntohl(uReplayCount[i]);
      uSpeed      = ntohl(uSpeed);
      uPlayTimeMs = ntohl(uPlayTimeMs);
      uEnableA = ntohl(uEnableA);
      rtpParamA.ntoh();
      uEnableV = ntohl(uEnableV);
      rtpParamV.ntoh();
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if(uGroupMask & (nWhereBit<<i))
         {
            rtpDstPortA[i].ntoh();
            rtpDstPortV[i].ntoh();
         }
      }
   }

   char * str(char * strBuff, int nBufLen) {
      char szRtpPortA[128];
      char szRtpPortV[128];
      char szRtpParamA[128];
      char szRtpParamV[128];
      char szTmpBuf[128];
      char szTmpBuf2[128];
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if(uGroupMask &(nWhereBit <<i))
         {
            szTmpBuf[0] = '\0';
            szTmpBuf2[0] = '\0';
            rtpDstPortA[i].str(szTmpBuf,128);
            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
            strcat(szRtpPortA,szTmpBuf2);

            szTmpBuf[0] = '\0';
            szTmpBuf2[0] = '\0';
            rtpDstPortV[i].str(szTmpBuf,128);
            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
            strcat(szRtpPortV,szTmpBuf2);
         }
      }
		rtpParamA.str(szRtpParamA,128);
		rtpParamV.str(szRtpParamV,128);
		sprintf(strBuff,
				  "GMASK:0x%x [FMT:%d(%s)filecnt:%u replay: %d %d %d speed:%u pos:%u ptime:%u "
				  "A(on:%d drtp:(%s) rtp_opt:(%s)) "
				  "V(on:%d drtp:(%s) rtp_opt:(%s))]",
				  uGroupMask,nFileFmt,GetAVCTypeName(HAVC_TYPE(nFileFmt)),
				  uFileCount, uReplayCount[0],uReplayCount[1],uReplayCount[2],
					uSpeed, uPosMs, uPlayTimeMs,
				  uEnableA,szRtpPortA ,szRtpParamA, uEnableV, szRtpPortV,szRtpParamV);
      return strBuff;
   }
};
/*
 * One File List atom must have below struct logic
 * char(1)   : length of filename ( strlen(filename) + 1( null separator) )
 * varstring : 1~128 length of various length filed filename
 * char(1)   : null ( separate token )
 * ------------------------------------------------
 * file list descending order was defined as like below
 *  audio - audio def - video - video def
 */
struct ARPlayMulti_FileList//multi filelist packed { aud[varlen] , auddef[varlen] , vid[varlen] , viddef[varlen] ... }
{
	uchar nFileALen[MULTIPLAY_MAXFILENUM];
	char szFileA[MULTIPLAY_MAXFILENUM][MPA_LEN_FILENAME];
	uchar nDefFileALen[MULTIPLAY_MAXFILENUM];
	char szDefFileA[MULTIPLAY_MAXFILENUM][MPA_LEN_FILENAME];
	uchar nFileVLen[MULTIPLAY_MAXFILENUM];
	char szFileV[MULTIPLAY_MAXFILENUM][MPA_LEN_FILENAME];
	uchar nDefFileVLen[MULTIPLAY_MAXFILENUM];
	char szDefFileV[MULTIPLAY_MAXFILENUM][MPA_LEN_FILENAME];
	uint uFileCount;
	uint uEnableA;
	uint uEnableV;
	ARPlayMulti_FileList()
	{
		uFileCount = 0;
		memset(&nFileALen,0x00,MULTIPLAY_MAXFILENUM);
		memset(&nDefFileALen,0x00,MULTIPLAY_MAXFILENUM);
		memset(&nFileVLen,0x00,MULTIPLAY_MAXFILENUM);
		memset(&nDefFileVLen,0x00,MULTIPLAY_MAXFILENUM);
		for(int i=0;i<MULTIPLAY_MAXFILENUM;i++)
		{

			memset(&szFileA[i],0x00,MPA_LEN_FILENAME);
			memset(&szDefFileA[i],0x00,MPA_LEN_FILENAME);
			memset(&szFileV[i],0x00,MPA_LEN_FILENAME);
			memset(&szDefFileV[i],0x00,MPA_LEN_FILENAME);
		}
	}
	ARPlayMulti_FileList(ARPlayMulti* pmsg)
	{
		uFileCount = 0;
		memset(&nFileALen,0x00,MULTIPLAY_MAXFILENUM);
		memset(&nDefFileALen,0x00,MULTIPLAY_MAXFILENUM);
		memset(&nFileVLen,0x00,MULTIPLAY_MAXFILENUM);
		memset(&nDefFileVLen,0x00,MULTIPLAY_MAXFILENUM);
      for(int i=0;i<MULTIPLAY_MAXFILENUM;i++)
      {

         memset(&szFileA[i],0x00,MPA_LEN_FILENAME);
         memset(&szDefFileA[i],0x00,MPA_LEN_FILENAME);
         memset(&szFileV[i],0x00,MPA_LEN_FILENAME);
         memset(&szDefFileV[i],0x00,MPA_LEN_FILENAME);
      }
		//alloc(pmsg);
      uFileCount = pmsg->uFileCount;
      uEnableA = pmsg->uEnableA;
      uEnableV = pmsg->uEnableV;
	}
	~ARPlayMulti_FileList()
	{
		//dealloc();
	}
	inline int getbufsize()
	{

		if(uFileCount > MULTIPLAY_MAXFILENUM)
			return 0;
#if 0
		int nTotFileCount;
		if(uEnableA && uEnableV)
		{
			nTotFileCount = 4 * uFileCount;
		}
		else if(uEnableA || uEnableV)
		{
			nTotFileCount = 2* uFileCount;
		}

		return nTotFileCount * MPA_LEN_FILENAME;
#else
		int nbufsize = 0;
		for(uint i=0;i<uFileCount;i++)
		{
			if(uEnableA)
			{
				nbufsize += nFileALen[i];
				nbufsize += nDefFileALen[i];
			}
			if(uEnableV)
			{
				nbufsize += nFileVLen[i];
				nbufsize += nDefFileVLen[i];
			}
		}
		return nbufsize;
#endif
	}
	inline int setlist(void* pbuf)
	{
		int nbufpos = 0;
		for(uint i=0;i<uFileCount;i++)
		{
			if(uEnableA && uEnableV)
			{
				//aud, , auddef, vid, viddef
				nFileALen[i] = *(((uchar*)pbuf)+nbufpos); nbufpos++;
				if(nFileALen[i] > MPA_LEN_FILENAME) return 0;
				memcpy(szFileA[i],(char*)pbuf + nbufpos,nFileALen[i]);//aud
				nbufpos += nFileALen[i];
				nDefFileALen[i] = *(((uchar*)pbuf)+nbufpos); nbufpos++;
				if(nDefFileALen[i] > MPA_LEN_FILENAME) return 0;
				memcpy(szDefFileA[i],(char*)pbuf + nbufpos,nDefFileALen[i]);//auddef
				nbufpos += nDefFileALen[i];
				nFileVLen[i] = *(((uchar*)pbuf)+nbufpos); nbufpos++;
				if(nFileVLen[i] > MPA_LEN_FILENAME) return 0;
				memcpy(szFileV[i],(char*)pbuf + nbufpos,nFileVLen[i]);//vid
				nbufpos += nFileVLen[i];
				nDefFileVLen[i] = *(((uchar*)pbuf)+nbufpos); nbufpos++;
				if(nDefFileVLen[i] > MPA_LEN_FILENAME) return 0;
				memcpy(szDefFileV[i],(char*)pbuf + nbufpos,nDefFileVLen[i]);//viddef
				nbufpos += nDefFileVLen[i];
			}
			else if(uEnableA)
			{
				nFileALen[i] = *(((uchar*)pbuf)+nbufpos); nbufpos++;
				if(nFileALen[i] > MPA_LEN_FILENAME) return 0;
				memcpy(szFileA[i],(char*)pbuf + nbufpos,nFileALen[i]);//aud
				nbufpos += nFileALen[i];
				nDefFileALen[i] = *(((uchar*)pbuf)+nbufpos); nbufpos++;
				if(nDefFileALen[i] > MPA_LEN_FILENAME) return 0;
				memcpy(szDefFileA[i],(char*)pbuf + nbufpos,nDefFileALen[i]);//auddef
				nbufpos += nDefFileALen[i];
			}
			else if(uEnableV)
			{
				nFileVLen[i] = *(((uchar*)pbuf)+nbufpos); nbufpos++;
				if(nFileVLen[i] > MPA_LEN_FILENAME) return 0;
				memcpy(szFileV[i],(char*)pbuf + nbufpos,nFileVLen[i]);//vid
				nbufpos += nFileVLen[i];
				nDefFileVLen[i] = *(((uchar*)pbuf)+nbufpos); nbufpos++;
				if(nDefFileVLen[i] > MPA_LEN_FILENAME) return 0;
				memcpy(szDefFileV[i],(char*)pbuf + nbufpos,nDefFileVLen[i]);//viddef
				nbufpos += nDefFileVLen[i];
			}
			else return 0;
		}
		return 1;
	}
	inline int setbuf(void* pbuf,int nbuflen)
	{
		if(nbuflen < getbufsize() || uFileCount > MULTIPLAY_MAXFILENUM)
			return 0;
		int nbufpos = 0;
		for(uint i=0;i<uFileCount;i++)
		{
			if(uEnableA && uEnableV)
			{
				//aud,auddef,vid,viddef
				if(nFileALen[i] > MPA_LEN_FILENAME) return 0;
				*(((uchar*)pbuf)+nbufpos) = nFileALen[i]; nbufpos++;
				memcpy((char*)pbuf + nbufpos,szFileA[i],nFileALen[i]);//aud
				nbufpos += nFileALen[i];
				if(nDefFileALen[i] > MPA_LEN_FILENAME) return 0;
				*(((uchar*)pbuf)+nbufpos) = nDefFileALen[i]; nbufpos++;
				memcpy((char*)pbuf + nbufpos,szDefFileA[i],nDefFileALen[i]);//auddef
				nbufpos += nDefFileALen[i];
				if(nFileVLen[i] > MPA_LEN_FILENAME) return 0;
				*(((uchar*)pbuf)+nbufpos) = nFileVLen[i]; nbufpos++;
				memcpy((char*)pbuf + nbufpos,szFileV[i],nFileVLen[i]);//vid
				nbufpos += nFileVLen[i];
				if(nDefFileVLen[i] > MPA_LEN_FILENAME) return 0;
				*(((uchar*)pbuf)+nbufpos) = nDefFileVLen[i]; nbufpos++;
				memcpy((char*)pbuf + nbufpos,szDefFileV[i],nDefFileVLen[i]);//viddef
				nbufpos += nDefFileVLen[i];
			}
			else if(uEnableA)
			{
				if(nFileALen[i] > MPA_LEN_FILENAME) return 0;
				*(((uchar*)pbuf)+nbufpos) = nFileALen[i]; nbufpos++;
				memcpy((char*)pbuf + nbufpos,szFileA[i],nFileALen[i]);//aud
				nbufpos += nFileALen[i];
				if(nDefFileALen[i] > MPA_LEN_FILENAME) return 0;
				*(((uchar*)pbuf)+nbufpos) = nDefFileALen[i]; nbufpos++;
				memcpy((char*)pbuf + nbufpos,szDefFileA[i],nDefFileALen[i]);//auddef
				nbufpos += nDefFileALen[i];
			}
			else if(uEnableV)
			{
				if(nFileVLen[i] > MPA_LEN_FILENAME) return 0;
				*(((uchar*)pbuf)+nbufpos) = nFileVLen[i]; nbufpos++;
				memcpy((char*)pbuf + nbufpos,szFileV[i],nFileVLen[i]);//vid
				nbufpos += nFileVLen[i];
				if(nDefFileVLen[i] > MPA_LEN_FILENAME) return 0;
				*(((uchar*)pbuf)+nbufpos) = nDefFileVLen[i]; nbufpos++;
				memcpy((char*)pbuf + nbufpos,szDefFileV[i],nDefFileVLen[i]);//viddef
				nbufpos += nDefFileVLen[i];
			}
			else return 0;
		}
		return nbufpos;
	}
#if 0
	inline int alloc(ARPlayMulti* pmsg)
	{
		if(pmsg && pmsg->uFileCount > 0)
		{
			if(pmsg->uFileCount > MULTIPLAY_MAXFILENUM)
				return 0;
			uFileCount = pmsg->uFileCount;
			uEnableA = pmsg->uEnableA;
			uEnableV = pmsg->uEnableV;
			if(uEnableA && uEnableV)
			{
				szFileA = new char*[MULTIPLAY_MAXFILENUM];
				szDefFileA = new char*[MULTIPLAY_MAXFILENUM];
				szFileV = new char*[MULTIPLAY_MAXFILENUM];
				szDefFileV = new char*[MULTIPLAY_MAXFILENUM];
				for(uint i=0;i<uFileCount;i++)
				{
					(szFileA[i]) = new char[MPA_LEN_FILENAME];
					(szDefFileA[i]) = new char[MPA_LEN_FILENAME];
					(szFileV[i]) = new char[MPA_LEN_FILENAME];
					(szDefFileV[i]) = new char[MPA_LEN_FILENAME];
				}
			}
			else if(uEnableA)
			{
				szFileA = new char*[MULTIPLAY_MAXFILENUM];
				szDefFileA = new char*[MULTIPLAY_MAXFILENUM];
				for(uint i=0;i<pmsg->uFileCount;i++)
				{
					(szFileA[i]) = new char[MPA_LEN_FILENAME];
					(szDefFileA[i]) = new char[MPA_LEN_FILENAME];
				}

			}
			else if(uEnableV)
			{
				szFileV = new char*[MULTIPLAY_MAXFILENUM];
				szDefFileV = new char*[MULTIPLAY_MAXFILENUM];
				for(uint i=0;i<pmsg->uFileCount;i++)
				{
					(szFileV[i]) = new char[MPA_LEN_FILENAME];
					(szDefFileV[i]) = new char[MPA_LEN_FILENAME];
				}

			}
			else
				return 0;
		}
		return 1;
	}
	inline void dealloc()
	{
		if(uFileCount > MULTIPLAY_MAXFILENUM) return;
		for(uint i=0;i<uFileCount;i++)
		{
			if(uEnableA && uEnableV)
			{
				if( szFileA && (szFileA[i])) delete[] (szFileA[i]);
				if( szFileV && (szFileV[i])) delete[] (szFileV[i]);
			}
			else if(uEnableA || uEnableV)
			{
				if(szFileA && (szFileA[i])) delete[] (szFileA[i]);
			}
		}
		if(szFileA) delete[] szFileA;
		if(szFileV) delete[] szFileV;
	}
#endif
	char * str(char * strBuff, int nBufLen) {
      if(!this || uFileCount < 1) return "null";
		if(uFileCount > MULTIPLAY_MAXFILENUM)
		{
			sprintf(strBuff,"filecount( %d) > MAXFILENUM(%d) error",
				uFileCount,MULTIPLAY_MAXFILENUM);
			return strBuff;
		}

      char szTmpBuf[1024];
		char szTmpFileA[4096];
		char szTmpDefFileA[4096];
		char szTmpFileV[4096];
		char szTmpDefFileV[4096];

		szTmpFileA[0] = '\0';
		szTmpDefFileA[0] = '\0';
		szTmpFileV[0] = '\0';
		szTmpDefFileV[0] ='\0';

		for(uint i=0;i<uFileCount;i++)
		{
			memset(szTmpBuf,0x00,1024);

			if(uEnableA)
			{
				if(nFileALen[i] > MPA_LEN_FILENAME) break;
				sprintf(szTmpBuf,"[%d] %s ",i,szFileA[i]==NULL?"nul":szFileA[i]);
				strncat(szTmpFileA,szTmpBuf,nFileALen[i]);
				if(nDefFileALen[i] > MPA_LEN_FILENAME) break;
				sprintf(szTmpBuf,"[%d] %s ",i,szDefFileA[i]==NULL?"nul":szDefFileA[i]);
				strncat(szTmpDefFileA,szTmpBuf,nDefFileALen[i]);
			}
			if(uEnableV)
			{
				if(nFileVLen[i] > MPA_LEN_FILENAME) break;
				sprintf(szTmpBuf,"[%d] %s ",i,szFileV[i]==NULL?"nul":szFileV[i]);
				strncat(szTmpFileV,szTmpBuf,nFileVLen[i]);
				if(nFileVLen[i] > MPA_LEN_FILENAME) break;
				sprintf(szTmpBuf,"[%d] %s ",i,szDefFileV[i]==NULL?"nul":szDefFileV[i]);
				strncat(szTmpDefFileV,szTmpBuf,nDefFileVLen[i]);
			}
		}
		sprintf(strBuff,
				  "AFILE:%s ADEF:%s VFILE:%s VDEF:%s ",
				(!szTmpFileA || szTmpFileA[0]=='\0')?"null":szTmpFileA,
				(!szTmpDefFileA || szTmpDefFileA[0]=='\0')?"null":szTmpDefFileA,
				(!szTmpFileV || szTmpFileV[0]=='\0')?"null":szTmpFileV,
				(!szTmpDefFileV || szTmpDefFileV[0]=='\0')?"null":szTmpDefFileV);
		return strBuff;
	}
};

struct ARPlayMultiRS
{
	uint uFileCount;
	uint uEnableA;
	uint uEnableV;
	int nRetFileA[MULTIPLAY_MAXFILENUM];
	int nRetDefFileA[MULTIPLAY_MAXFILENUM];
	int nRetFileV[MULTIPLAY_MAXFILENUM];
	int nRetDefFileV[MULTIPLAY_MAXFILENUM];
	ARPlayMultiRS()
	{
		uFileCount = 0;
		uEnableA = 0;
		uEnableV = 0;
		memset(&nRetFileA,0x00,sizeof(nRetFileA));
		memset(&nRetDefFileA,0x00,sizeof(nRetFileA));
		memset(&nRetFileV,0x00,sizeof(nRetFileV));
		memset(&nRetDefFileV,0x00,sizeof(nRetFileV));
	}
	void hton() {
		uFileCount = htonl(uFileCount);
		uEnableA = htonl(uEnableA);
		uEnableV = htonl(uEnableV);
		for(int i=0;i<MULTIPLAY_MAXFILENUM;i++)
		{
			nRetFileA[i] = htonl(nRetFileA[i]);
			nRetDefFileA[i] = htonl(nRetDefFileA[i]);
			nRetFileV[i] = htonl(nRetFileV[i]);
			nRetDefFileV[i] = htonl(nRetDefFileV[i]);
		}
	}
	void ntoh() {
		uFileCount = ntohl(uFileCount);
		uEnableA = ntohl(uEnableA);
		uEnableV = ntohl(uEnableV);
		for(int i=0;i<MULTIPLAY_MAXFILENUM;i++)
		{
			nRetFileA[i] = ntohl(nRetFileA[i]);
			nRetDefFileA[i] = ntohl(nRetDefFileA[i]);
			nRetFileV[i] = ntohl(nRetFileV[i]);
			nRetDefFileV[i] = ntohl(nRetDefFileV[i]);
		}
	}
	char* str(char* strBuff, int nBufLen) {
		char szTmpBuff[128];
		int nloop = (uFileCount <= MULTIPLAY_MAXFILENUM ? uFileCount : MULTIPLAY_MAXFILENUM);
		sprintf(strBuff,"RET: ");
		if(uEnableA && !uEnableV)
		{
			strcat(strBuff,"AUD:DEFA=");
			for(int i=0;i<nloop;i++)
			{
				sprintf(szTmpBuff,"[%dth]%d:%d ",i,nRetFileA[i],nRetDefFileA[i]);
				strcat(strBuff,szTmpBuff);
			}
		}
		else if(uEnableV && !uEnableA)
		{
			strcat(strBuff,"VID:DEFV=");
			for(int i=0;i<nloop;i++)
			{
				sprintf(szTmpBuff,"[%dth]%d:%d ",i,nRetFileV[i],nRetDefFileV[i]);
				strcat(strBuff,szTmpBuff);
			}
		}
		else if(uEnableA && uEnableV)
		{
			strcat(strBuff,"AUD:DEFA:VID:DEFV=");
			for(int i=0;i<nloop;i++)
			{
				sprintf(szTmpBuff,"[%dth]%d:%d:%d:%d ",i,nRetFileA[i],nRetDefFileA[i],nRetFileV[i],nRetDefFileV[i]);
				strcat(strBuff,szTmpBuff);
			}
		}
      return strBuff;
	}
};

struct ARRecord
//record not support various source media & dst media.. only one source type currently
{
   uint uGroupMask;
   int  nFileFmt; // HAVC_TYPE
   char szFileA[MPA_LEN_FILENAME];//each file name auto-fullfill with group info
   char szFileV[MPA_LEN_FILENAME];//each file name auto-fullfill with group info
   ARMediaInfo media;             // media info

   uint   uEnableA;
   //ARRtpPort      rtpSrcPortA[MAX_GROUP_PER_SES];       // rtp remote audio sender ip/port
   //removed rtpcomm didn't check recv packet ip port.. so no need addr info when record
   ARRtpParam     rtpParamA ;

   uint   uEnableV;
   //ARRtpPort      rtpSrcPortV[MAX_GROUP_PER_SES];       // rtp remote video sender ip/port
   ARRtpParam     rtpParamV;
   void  hton() {
      nFileFmt    = htonl(nFileFmt);
      media.hton();
      uEnableA = htonl(uEnableA);
      rtpParamA.hton();


      uEnableV = htonl(uEnableV);
      rtpParamV.hton();
/*
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if((uGroupMask & (nWhereBit << i)))
         {
            rtpSrcPortA[i].hton();
            rtpSrcPortV[i].hton();
         }
      }
*/
      uGroupMask = htonl(uGroupMask);
   }
   void  ntoh() {
      uGroupMask = ntohl(uGroupMask);
      nFileFmt    = ntohl(nFileFmt);
      media.ntoh();
      uEnableA = ntohl(uEnableA);
      rtpParamA.ntoh();
      uEnableV = ntohl(uEnableV);
      rtpParamV.ntoh();
   }


   char * str(char * strBuff, int nBufLen) {
      char szMedia[256];
      char szRtpParamA[128];
      char szRtpParamV[128];
      sprintf(strBuff,
         "GMASK:0x%x FMT:%d(%s) AFILE:%s VFILE:%s "
         "MEDIA(%s)"
         "A(on:%d rtp_opt:(%s))"
         "V(on:%d rtp_opt:(%s))]",
         uGroupMask,nFileFmt,GetAVCTypeName(HAVC_TYPE(nFileFmt)),szFileA, szFileV,
         media.str(szMedia,256),
         uEnableA,rtpParamA.str(szRtpParamA,128),
         uEnableV,rtpParamV.str(szRtpParamV,128));
      return strBuff;
   }
};

struct ARRelay
{
   uint   uGroupMask;
   uint   uEnableA;
   ARRtpPort      rtpDstPortA[MAX_GROUP_PER_SES];//rtp remote audio receiver ip/port
   ARRtpParam     rtpParamA[MAX_GROUP_PER_SES]; //each group has own rtp param
                                                //transcode issue
   uint   uEnableV;
   uint   uVKeepSeqEnable;
   ARRtpPort      rtpDstPortV[MAX_GROUP_PER_SES];//rtp remote video receiver ip/port
   ARRtpParam     rtpParamV[MAX_GROUP_PER_SES];
   uint   uGSwitchMask[MAX_GROUP_PER_SES];
   //each stream group has mask for dst streaming group info .. if switch mask setted,
   //  that group input source will out using setted group socket & dst addr
   //if relay did not switch and used one stream group, recv & send with only one socket..
   //  then masking switchmask gid with own gid
	//ARDumpSetReq	dumpset;
	ARDtmfSetReq dtmfset;
   void hton() {
      uEnableA = htonl(uEnableA);
      uEnableV = htonl(uEnableV);
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         uGSwitchMask[i] = htonl(uGSwitchMask[i]);
         if((uGroupMask & (nWhereBit << i)))
         {
            rtpDstPortA[i].hton();
            rtpParamA[i].hton();
            rtpDstPortV[i].hton();
            rtpParamV[i].hton();
         }
      }
      uGroupMask = htonl(uGroupMask);
		dtmfset.hton();
   }
   void ntoh() {
      uGroupMask = ntohl(uGroupMask);
      uEnableA = ntohl(uEnableA);
      uEnableV = ntohl(uEnableV);
		dtmfset.ntoh();
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         uGSwitchMask[i] = ntohl(uGSwitchMask[i]);
         if((uGroupMask & (nWhereBit << i)))
         {
            rtpDstPortA[i].ntoh();
            rtpParamA[i].ntoh();
            rtpDstPortV[i].ntoh();
            rtpParamV[i].ntoh();
         }
      }
   }

   char * str(char * strBuff, int nBufLen) {
      char szDstPortA[2048];
      char szDstPortV[2048];
      char szRtpParamA[2048];
      char szRtpParamV[2048];
      char szGSwitch[2048];
		char szDtmfSet[2048];
      char szTmpBuf[128];
      char szTmpBuf2[128];

		szDstPortA[0] = szDstPortV[0] = szRtpParamA[0] = szRtpParamV[0] = szGSwitch[0] = szDtmfSet[0] = '\0';
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if(uGroupMask & (nWhereBit <<i))
         {
         	if(uEnableA)
         	{
	            szTmpBuf[0] = '\0';
	            szTmpBuf2[0] = '\0';
	            rtpDstPortA[i].str(szTmpBuf,128);
	            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
	            strcat(szDstPortA,szTmpBuf2);
					szTmpBuf[0] = '\0';
	            szTmpBuf2[0] = '\0';
	            rtpParamA[i].str(szTmpBuf,128);
	            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
	            strcat(szRtpParamA,szTmpBuf2);
         	}

				if(uEnableV)
				{
	            szTmpBuf[0] = '\0';
	            szTmpBuf2[0] = '\0';
	            rtpDstPortV[i].str(szTmpBuf,128);
	            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
	            strcat(szDstPortV,szTmpBuf2);
	            szTmpBuf[0] = '\0';
	            szTmpBuf2[0] = '\0';
	            rtpParamV[i].str(szTmpBuf,128);
	            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
	            strcat(szRtpParamV,szTmpBuf2);
				}

            szTmpBuf2[0] = '\0';
            sprintf(szTmpBuf2,"[%d]0x%x ",i,uGSwitchMask[i]);
            strcat(szGSwitch,szTmpBuf2);
         }
      }
		dtmfset.str(szDtmfSet,2048);
      sprintf(strBuff, "GMASK:0x%x [A(on:%d drtp:%s rtp_opt:%s) "
              "V(on:%d drtp:%s rtp_opt:%s) SWI:%s DTMF:%s]",
              uGroupMask,uEnableA,szDstPortA,szRtpParamA,uEnableV,szDstPortV,szRtpParamV,
              szGSwitch,szDtmfSet);
      return strBuff;
   }

};

struct ARRelay_TC
{
   uint   uGroupMask;
   uint   uEnableA;
   ARRtpPort      rtpDstPortA[MAX_GROUP_PER_SES];//rtp remote audio receiver ip/port
   ARRtpParam     rtpParamA[MAX_GROUP_PER_SES]; //each group has own rtp param
                                                //transcode issue
   uint   uEnableV;
   uint   uVKeepSeqEnable;
   ARRtpPort      rtpDstPortV[MAX_GROUP_PER_SES];//rtp remote video receiver ip/port
   ARRtpParam     rtpParamV[MAX_GROUP_PER_SES];
   uint   uGSwitchMask[MAX_GROUP_PER_SES];
   ARMediaInfo media[MAX_GROUP_PER_SES];
   //each stream group has mask for dst streaming group info .. if switch mask setted,
   //  that group input source will out using setted group socket & dst addr
   //if relay did not switch and used one stream group, recv & send with only one socket..
   //  then masking switchmask gid with own gid
	//ARDumpSetReq	dumpset;
	ARDtmfSetReq dtmfset;
   void hton() {
      uEnableA = htonl(uEnableA);
      uEnableV = htonl(uEnableV);
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         uGSwitchMask[i] = htonl(uGSwitchMask[i]);
         if((uGroupMask & (nWhereBit << i)))
         {
            rtpDstPortA[i].hton();
            rtpParamA[i].hton();
            rtpDstPortV[i].hton();
            rtpParamV[i].hton();
            media[i].hton();
         }
      }
      uGroupMask = htonl(uGroupMask);
		dtmfset.hton();
   }

   void ntoh() {
      uGroupMask = ntohl(uGroupMask);
      uEnableA = ntohl(uEnableA);
      uEnableV = ntohl(uEnableV);
		dtmfset.ntoh();
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         uGSwitchMask[i] = ntohl(uGSwitchMask[i]);
         if((uGroupMask & (nWhereBit << i)))
         {
            rtpDstPortA[i].ntoh();
            rtpParamA[i].ntoh();
            rtpDstPortV[i].ntoh();
            rtpParamV[i].ntoh();
            media[i].ntoh();
         }
      }
   }

   char * str(char * strBuff, int nBufLen) {
      char szDstPortA[2048];
      char szDstPortV[2048];
      char szRtpParamA[2048];
      char szRtpParamV[2048];
      char szMedia[2048];
      char szGSwitch[2048];
		char szDtmfSet[2048];
      char szTmpBuf[256];
      char szTmpBuf2[256];
		
		szDstPortA[0] = szDstPortV[0] = szRtpParamA[0] = szRtpParamV[0] = szMedia[0] = szGSwitch[0] = szDtmfSet[0] = '\0';
      int nWhereBit = 0x1;
      for(uint i=0;i<MAX_GROUP_PER_SES;i++)
      {
         if(uGroupMask & (nWhereBit <<i))
         {
            szTmpBuf[0] = '\0';
            szTmpBuf2[0] = '\0';
            rtpDstPortA[i].str(szTmpBuf,256);
            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
            strcat(szDstPortA,szTmpBuf2);

            szTmpBuf[0] = '\0';
            szTmpBuf2[0] = '\0';
            rtpDstPortV[i].str(szTmpBuf,256);
            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
            strcat(szDstPortV,szTmpBuf2);

            szTmpBuf[0] = '\0';
            szTmpBuf2[0] = '\0';
            rtpParamA[i].str(szTmpBuf,256);
            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
            strcat(szRtpParamA,szTmpBuf2);

            szTmpBuf[0] = '\0';
            szTmpBuf2[0] = '\0';
            rtpParamV[i].str(szTmpBuf,256);
            sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
            strcat(szRtpParamV,szTmpBuf2);

            szTmpBuf2[0] = '\0';
            sprintf(szTmpBuf2,"[%d]0x%x ",i,uGSwitchMask[i]);
            strcat(szGSwitch,szTmpBuf2);

            szTmpBuf[0] = '\0';
			szTmpBuf2[0] = '\0';
			media[i].str(szTmpBuf,256);
			sprintf(szTmpBuf2,"[%d]%s ",i,szTmpBuf);
			strcat(szMedia,szTmpBuf2);
         }
      }
		dtmfset.str(szDtmfSet,2048);
      sprintf(strBuff, "GMASK:0x%x [A(on:%d drtp:%s rtp_opt:%s) "
              "V(on:%d drtp:%s rtp_opt:%s) media(%s) SWI:%s DTMF:%s]",
              uGroupMask,uEnableA,szDstPortA,szRtpParamA,uEnableV,szDstPortV,szRtpParamV,
              szMedia,szGSwitch,szDtmfSet);
      return strBuff;
   }

};

enum AR_MSG_INT_TYPE
{
   AMIT_TCP_CONN_APP      = 0x0001,
   AMIT_TCP_DISC_APP      = 0x0002,
	AMIT_TCP_CONN_BOARD    = 0x0011,
	AMIT_TCP_DISC_BOARD    = 0x0012,
   AMIT_TCP_CONN_BCA      = 0x0031,
   AMIT_TCP_DISC_BCA      = 0x0032,
   AMIT_RESET_APP			  = 0x0041,
   AMIT_RESET_APP_BOARD   = 0x0042,
   AMIT_RESET_BOARD       = 0x0043,
   AMIT_CLEAR_SESSION     = 0x00FF,
	AMIT_ALIVE				  = AMT_ALIVE
};

struct ARInternalMsg
{
   uint uIntMsgType;
	ARInternalMsg(){
		uIntMsgType = 0;
	}
	void              hton() {
		uIntMsgType = htonl(uIntMsgType);
	}
	void ntoh() {
		uIntMsgType = ntohl(uIntMsgType);
	}
	char* str(char* strBuff,int nBufLen) {
		switch(uIntMsgType)
		{
		case AMIT_TCP_CONN_APP:
			sprintf(strBuff,"[AMIT TCP CONN APP]");
		break;
		case AMIT_TCP_DISC_APP:
			sprintf(strBuff,"[AMIT TCP DISC APP]");
		break;
		case AMIT_TCP_CONN_BCA:
			sprintf(strBuff,"[AMIT TCP CONN BCA]");
		break;
		case AMIT_TCP_DISC_BCA:
			sprintf(strBuff,"[AMIT TCP DISC BCA]");
		break;
		case AMIT_CLEAR_SESSION:
			sprintf(strBuff,"[AMIT CLEAR SESSION]");
		break;
		default:
			sprintf(strBuff,"[AMIT undefined type(0x%x)]",uIntMsgType);
		break;
		}
      return strBuff;
	}
};

struct ARMaintReq
{
	enum{ MPRM=0x1,MPA=0x2,SESSION=0x3};
	uint cRange;
	char cReserved[3];
	void hton(){}
	void ntoh(){}
	ARMaintReq(){memset(this,0x00,sizeof(ARMaintReq));}
	char* str(char* strBuff,int nBufLen)
	{
		switch(cRange)
		{
		case MPRM:
			sprintf(strBuff,"[range: MPRM]");
		break;
		case MPA:
			sprintf(strBuff,"[range: MPA]");
		break;
		case SESSION:
			sprintf(strBuff,"[range: SESSION]");
		break;
		default:
			sprintf(strBuff,"[range: undefine]");
		break;
		}	
		return strBuff;
	}
};

//--ARMsg

struct ARMsg{
   ARMsgHdr       hdr;
   uchar body[MPA_LEN_MSG-sizeof(ARMsgHdr)];
	ARMsg(){memset(this,0x00,sizeof(ARMsg));}
   void hton_body(){
      int nMsgType    = hdr.uiType;
      int nMsgSubType = hdr.uiSubType;
        switch(nMsgType)
        {
		  case AMT_INTERNAL://internal msg include interface between RTPM & RTPA
				if(nMsgSubType == AMST_REQ || nMsgSubType == AMST_RES)
					((ARInternalMsg *)body)->hton();
			break;
        case AMT_SETUP_AGNT:
            if(nMsgSubType == AMST_REQ) ((ARSetupAgent *)body)->hton();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_SETUP_APPS://nobody
        case AMT_SETUP_APPS_T://nobody        
        break;
        case AMT_ALIVE://nobody
        case AMT_ALIVE_T:
        break;
        case AMT_START_SES:
            if(nMsgSubType == AMST_REQ) ((ARStartSessionReq*) body)->hton();
            else if(nMsgSubType == AMST_RES) ((ARStartSessionRS *) body)->hton();
        break;
		  case AMT_STOP_SES:
				if(nMsgSubType == AMST_RES) ((ARStopSessionRS *) body)->hton();
		  break;
        case AMT_START_SES_DUMP:
            if(nMsgSubType == AMST_REQ) ((ARStartSessionDumpReq*) body)->hton();
            else if(nMsgSubType == AMST_RES) ((ARStartSessionRS *) body)->hton();
        break;
        case AMT_PLAY_READY:
            if(nMsgSubType == AMST_REQ) ((ARPlayReady *) body)->hton();
            else if(nMsgSubType == AMST_RES) ((ARPlayReadyRS *) body)->hton();
        break;
        case AMT_PLAY:
            if(nMsgSubType == AMST_REQ) ((ARPlay *) body)->hton();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_PLAY_SIMPLE:
            if(nMsgSubType == AMST_REQ) ((ARPlaySimple *) body)->hton();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_PLAY_MULTI:
      	  if(nMsgSubType == AMST_REQ) ((ARPlayMulti *) body)->hton();
      	  else if(nMsgSubType == AMST_RES) ((ARPlayMultiRS *) body)->hton();
        break;
        case AMT_PLAY_SIMPLE_TC://not implement currently
        break;
        case AMT_PLAY_MULTI_TC://not implement currently
        break;
        case AMT_PLAY_STOP://nobody
        break;
        case AMT_PLAY_DONE://nobody
        break;
        case AMT_RECORD:
            if(nMsgSubType == AMST_REQ) ((ARRecord *) body)->hton();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_RECORD_TC://not implement currently
        break;
        case AMT_RECORD_STOP://nobody
        break;
        case AMT_RELAY:
            if(nMsgSubType == AMST_REQ) ((ARRelay *) body)->hton();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_RELAY_TC:
			if(nMsgSubType == AMST_REQ) ((ARRelay_TC *) body)->hton();
			//else if(nMsgSubType == AMST_RES) nobody
		break;
        case AMT_RELAY_STOP://nobody
        break;
        case AMT_DTMF_GEN:
            if(nMsgSubType == AMST_REQ) ((ARDtmfInfo *) body)->hton();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_DTMF_SET:
            if(nMsgSubType == AMST_REQ) ((ARDtmfSetReq *) body)->hton();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_DTMF_INFO:
            if(nMsgSubType == AMST_INFO) ((ARDtmfInfo *) body)->hton();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_DTMF_GENINFO:
            if(nMsgSubType == AMST_INFO) ((ARDtmfInfo *) body)->hton();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_MM_BLOCK://yet imple
        break;
        case AMT_MM_UNBLOCK://yet imple
        break;
        case AMT_MM_RESET:
		  		if(nMsgSubType == AMST_REQ) ((ARMaintReq*)body)->hton();
        break;
        case AMT_MM_BD_STS://yet imple
        break;
        case AMT_MM_SES_STS://yet imple
        break;
        case AMT_MM_BD_INF://yet imple
        break;
        case AMT_MM_SES_INF://yet imple
        break;
		  case AMT_MM_RM_INF://yet imple
        break;
        case AMT_MM_CLEAR:
		  		if(nMsgSubType == AMST_REQ) ((ARMaintReq*)body)->hton();
        break;
        }//switch

   }

   void hton() {
      hton_body();
      hdr.hton();
    }
   void ntoh_body()
   {
      int nMsgType    = hdr.uiType;
      int nMsgSubType = hdr.uiSubType;
        switch(nMsgType)
        {
		  case AMT_INTERNAL:
            if(nMsgSubType == AMST_REQ || nMsgSubType == AMST_RES) ((ARInternalMsg *)body)->ntoh();
		  break;
        case AMT_SETUP_AGNT:
            if(nMsgSubType == AMST_REQ) ((ARSetupAgent *)body)->ntoh();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_SETUP_APPS://nobody
        case AMT_SETUP_APPS_T://nobody        
        break;
        case AMT_ALIVE://nobody
        case AMT_ALIVE_T:
        break;
        case AMT_START_SES:
        case AMT_START_SES_DUMP:
            if(body && nMsgSubType == AMST_REQ) ((ARStartSessionDumpReq*)body)->ntoh();
            else if(nMsgSubType == AMST_RES) ((ARStartSessionRS *) body)->ntoh();
        break;
		  case AMT_STOP_SES:
				if(nMsgSubType == AMST_RES) ((ARStopSessionRS *) body)->ntoh();
		  break;
        case AMT_PLAY_READY:
            if(body && nMsgSubType == AMST_REQ) ((ARPlayReady *) body)->ntoh();
            else if(nMsgSubType == AMST_RES) ((ARPlayReadyRS *) body)->ntoh();
        break;
        case AMT_PLAY:
            if(body && nMsgSubType == AMST_REQ) ((ARPlay *) body)->ntoh();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_PLAY_SIMPLE:
            if( nMsgSubType == AMST_REQ) ((ARPlaySimple *) body)->ntoh();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_PLAY_MULTI:
            if( nMsgSubType == AMST_REQ) ((ARPlayMulti *) body)->ntoh();
            else if(nMsgSubType == AMST_RES) ((ARPlayMultiRS *) body)->ntoh();
        break;
        case AMT_PLAY_STOP://nobody
        break;
        case AMT_PLAY_DONE://nobody
        break;
        case AMT_RECORD:
            if(nMsgSubType == AMST_REQ) ((ARRecord *) body)->ntoh();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_RECORD_STOP://nobody
        break;
        case AMT_RELAY:
            if(nMsgSubType == AMST_REQ) ((ARRelay *) body)->ntoh();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
		  case AMT_RELAY_TC:
            if(nMsgSubType == AMST_REQ) ((ARRelay_TC *) body)->ntoh();
            //else if(nMsgSubType == AMST_RES) nobody
        break;		
        case AMT_RELAY_STOP://nobody
        break;
        case AMT_DTMF_GEN:
            if(nMsgSubType == AMST_REQ) ((ARDtmfInfo*)body)->ntoh();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_DTMF_SET:
            if(nMsgSubType == AMST_REQ) ((ARDtmfSetReq*)body)->ntoh();
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_DTMF_INFO:
            if(nMsgSubType == AMST_INFO) ((ARDtmfInfo*)body)->ntoh();
        break;
        case AMT_DTMF_GENINFO:
            if(nMsgSubType == AMST_INFO) ((ARDtmfInfo*)body)->ntoh();
        break;
        case AMT_MM_BLOCK://yet imple
        break;
        case AMT_MM_UNBLOCK://yet imple
        break;
        case AMT_MM_RESET:
        	if(nMsgSubType == AMST_REQ) ((ARMaintReq*)body)->ntoh();
        break;
        case AMT_MM_BD_STS://yet imple
        break;
        case AMT_MM_SES_STS://yet imple
        break;
        case AMT_MM_BD_INF://yet imple
        break;
        case AMT_MM_SES_INF://yet imple
        break;
		  case AMT_MM_RM_INF://yet imple
        break;
        case AMT_MM_CLEAR:
        	if(nMsgSubType == AMST_REQ) ((ARMaintReq*)body)->ntoh();
        break;
        }//switch
   }
   void              ntoh() {
      hdr.ntoh();
      ntoh_body();
   }

   char * str(char * strBuff, int nBufLen) {
      int nMsgType    = hdr.uiType;
      int nMsgSubType = hdr.uiSubType;
      char szHdr[256];
      char szBody[10240];

      szHdr[0] = '\0';
      szBody[0] = '\0';

      hdr.str(szHdr, sizeof(szHdr));

        switch(nMsgType)
        {
        case AMT_INTERNAL:
				((ARInternalMsg *)body)->str(szBody, sizeof(szBody));
			break;
        case AMT_SETUP_AGNT:
            if(nMsgSubType == AMST_REQ) ((ARSetupAgent *)body)->str(szBody, sizeof(szBody));
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_SETUP_APPS://nobody
        case AMT_SETUP_APPS_T://nobody        
        break;
        case AMT_ALIVE://nobody
        case AMT_ALIVE_T:
        break;
        case AMT_START_SES:
            if(body && nMsgSubType == AMST_REQ) ((ARStartSessionReq*)body)->str(szBody, sizeof(szBody));
            else if(nMsgSubType == AMST_RES) ((ARStartSessionRS *) body)->str(szBody, sizeof(szBody));
			break;
        case AMT_START_SES_DUMP:
            if(body && nMsgSubType == AMST_REQ) ((ARStartSessionDumpReq*)body)->str(szBody, sizeof(szBody));
            else if(nMsgSubType == AMST_RES) ((ARStartSessionRS *) body)->str(szBody, sizeof(szBody));
        break;
		  case AMT_STOP_SES:
				if(body && nMsgSubType == AMST_RES) ((ARStopSessionRS *) body)->str(szBody,sizeof(szBody));
			break;
        case AMT_PLAY_READY:
            if(body && nMsgSubType == AMST_REQ) ((ARPlayReady *) body)->str(szBody, sizeof(szBody));
            else if(nMsgSubType == AMST_RES) ((ARPlayReadyRS *) body)->str(szBody, sizeof(szBody));
        break;
        case AMT_PLAY:
            if(body && nMsgSubType == AMST_REQ) ((ARPlay *) body)->str(szBody, sizeof(szBody));
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_PLAY_SIMPLE:
            if( nMsgSubType == AMST_REQ) ((ARPlaySimple *) body)->str(szBody, sizeof(szBody));
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_PLAY_MULTI:
            if( nMsgSubType == AMST_REQ)
				{
					((ARPlayMulti *) body)->str(szBody, sizeof(szBody));
               ARPlayMulti_FileList nFileList((ARPlayMulti*)body);
					ARPlayMulti_FileList* plist = &nFileList;
					plist->setlist(body + sizeof(ARPlayMulti));
					char szTmp[10240];
					plist->str(szTmp,10240);
					strncat(szBody,szTmp,sizeof(szBody)-strlen(szBody));
					//if(plist) delete plist;
				}
            else if(nMsgSubType == AMST_RES) ((ARPlayMultiRS *) body)->str(szBody,sizeof(szBody));
        break;
        case AMT_PLAY_STOP://nobody
        break;
        case AMT_PLAY_DONE://nobody
        break;
        case AMT_RECORD:
            if(nMsgSubType == AMST_REQ) ((ARRecord *) body)->str(szBody, sizeof(szBody));
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_RECORD_STOP://nobody
        break;
        case AMT_RELAY:
            if(nMsgSubType == AMST_REQ) ((ARRelay *) body)->str(szBody, sizeof(szBody));
            //else if(nMsgSubType == AMST_RES) nobody
        break;
			case AMT_RELAY_TC:
            if(nMsgSubType == AMST_REQ) ((ARRelay_TC *) body)->str(szBody, sizeof(szBody));
            //else if(nMsgSubType == AMST_RES) nobody
        break;	
        case AMT_RELAY_STOP://nobody
        break;
        case AMT_DTMF_GEN:
            if(nMsgSubType == AMST_REQ) ((ARDtmfInfo*)body)->str(szBody, sizeof(szBody));
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_DTMF_SET:
            if(nMsgSubType == AMST_REQ) ((ARDtmfSetReq*)body)->str(szBody, sizeof(szBody));
            //else if(nMsgSubType == AMST_RES) nobody
        break;
        case AMT_DTMF_INFO:
            if(nMsgSubType == AMST_INFO) ((ARDtmfInfo*)body)->str(szBody, sizeof(szBody));
        break;
        case AMT_DTMF_GENINFO:
            if(nMsgSubType == AMST_INFO) ((ARDtmfInfo*)body)->str(szBody, sizeof(szBody));
        break;
        case AMT_MM_BLOCK://yet imple
        break;
        case AMT_MM_UNBLOCK://yet imple
        break;
        case AMT_MM_RESET://yet imple
        	if(nMsgSubType == AMST_REQ) ((ARMaintReq*)body)->str(szBody,sizeof(szBody));
        break;
        case AMT_MM_BD_STS://yet imple
        break;
        case AMT_MM_SES_STS://yet imple
        break;
        case AMT_MM_BD_INF://yet imple
        break;
        case AMT_MM_SES_INF://yet imple
        break;
		  case AMT_MM_RM_INF://yet imple
        break;
        case AMT_MM_CLEAR://yet imple
        	if(nMsgSubType == AMST_REQ) ((ARMaintReq*)body)->str(szBody,sizeof(szBody));
        break;
        }//switch

      sprintf(strBuff, "HDR(%d):%s BODY(%d):",
             sizeof(ARMsgHdr), szHdr, hdr.uiMsgLen-sizeof(ARMsgHdr));
		strncat(strBuff,szBody,nBufLen-strlen(strBuff));
      return strBuff;
   }
} ;

#endif //__RTP_AGENT_IF_H__

////@}
