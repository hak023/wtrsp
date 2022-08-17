/**
 * @file  com_msgtype.hpp
 * the common message type
 *
 * @attention Copyright (c) 2010 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2011.01.07   hc_yun   1.0     first written
 * @par 2011.08.17   hc_yun   1.2     uiCompSesId => union tdm info  @union_compses
 * @par 2011.09.29   hc_yun   1.5     add Reserved[20]               @add_reserved
 * @par 2012.04.23   hc_yun   2.0     union sp                       @sp
 * @par 2012.09.12   hc_yun   2.1     add trace                      @trace
 */

#ifndef __SVC_COMMON_MESSAGE_TYPE__
#define __SVC_COMMON_MESSAGE_TYPE__

#include <stdio.h>
#include <string>
#include <string.h> //add by paul 2013.01.18

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

//@union_compses
#ifndef _LINUX_
#define V2TDM_BIG_ENDIAN 
#endif

///defines the common service message
#define SVC_MSG_MAGIC_COOKIE  0x9D1FC7B8
struct SVCComMsgHdr
{
   enum { SVC_MAX_DESCNAME = 32 };
   ///defines the first element
   unsigned int uiMagicCookie              ; //the message integrity check(0x9D1FC7B8)
   unsigned int uiMsgLen                   ; //including header, the message length

   ///defines type
   unsigned int uiType                     ; //the message type
   unsigned int uiSubType                  ; //the message subtype

   ///defines 
   //uiCompId + uiCompSesId : set in Component, elsewhere should maintain
   //uiAsId   + uiAsSesId   : set in AS, elsewhere should maintain
   unsigned int uiCompId                   ; //the component(resource app) id
   //@union_compses
   unsigned int uiCompSesId                ; //the component session id
#if 0
   union {
      unsigned int   uiCompSesId;
      struct {
#ifdef V2TDM_BIG_ENDIAN
         unsigned char   ucShelf    :8;
         unsigned char   ucBoard    :8;
         unsigned char   ucTrunk    :8;
         unsigned char   ucChannel  :8;
#else
         unsigned char   ucChannel  :8;
         unsigned char   ucTrunk    :8;
         unsigned char   ucBoard    :8;
         unsigned char   ucShelf    :8;
#endif
      }tdm_;
   };
#endif
   unsigned int uiAsId                     ; //the AS id
   unsigned int uiAsSesId                  ; //the AS session id
   //if the szSesDesc is set, set in componet or as.
   char         szSesDesc[SVC_MAX_DESCNAME]; //sesssion description

   //the reason code
   unsigned int uiReasonCode               ;
   //@add_reserved
   //char         Reserved[24]             ;
   //@sp
   union{
      char         Reserved[24]            ;
      struct {
         unsigned int uiOPC                ; //opc
         unsigned int uiDPC                ; //dpc
      }sp_; 
      struct {
#ifdef V2TDM_BIG_ENDIAN
         unsigned char   ucShelf    :8;
         unsigned char   ucBoard    :8;
         unsigned char   ucTrunk    :8;
         unsigned char   ucChannel  :8;
#else
         unsigned char   ucChannel  :8;
         unsigned char   ucTrunk    :8;
         unsigned char   ucBoard    :8;
         unsigned char   ucShelf    :8;
#endif
      }tdm_;
   };
   char* data(){
      return (char*)(this+1);
   }
   
   //converts from host byte order to network byte order
   void hton(){
      uiMagicCookie=htonl(uiMagicCookie);
      uiMsgLen=htonl(uiMsgLen);
      uiType=htonl(uiType);
      uiSubType=htonl(uiSubType);
      uiCompId=htonl(uiCompId);
      uiCompSesId=htonl(uiCompSesId);
      uiAsId=htonl(uiAsId);
      uiAsSesId=htonl(uiAsSesId);
      uiReasonCode=htonl(uiReasonCode);
      //@sp
      sp_.uiOPC=htonl(sp_.uiOPC);
      sp_.uiDPC=htonl(sp_.uiDPC);
   }

   //converts from network byte order to host byte order
   void ntoh(){
      uiMagicCookie=ntohl(uiMagicCookie);
      uiMsgLen=ntohl(uiMsgLen);
      uiType=ntohl(uiType);
      uiSubType=ntohl(uiSubType);
      uiCompId=ntohl(uiCompId);
      uiCompSesId=ntohl(uiCompSesId);
      uiAsId=ntohl(uiAsId);
      uiAsSesId=ntohl(uiAsSesId);
      uiReasonCode=ntohl(uiReasonCode);
      //@sp
      sp_.uiOPC=ntohl(sp_.uiOPC);
      sp_.uiDPC=ntohl(sp_.uiDPC);
   }
   //set tdm info
   void set_tdm(const int nbid, const int ntid, const int ncid){
      tdm_.ucBoard=(unsigned char)nbid;
      tdm_.ucTrunk=(unsigned char)ntid;
      tdm_.ucChannel=(unsigned char)ncid;
   }
   //get tdm info
   void get_tdm(int& nbid, int& ntid, int& ncid){
      nbid=(int)tdm_.ucBoard;
      ntid=(int)tdm_.ucTrunk;
      ncid=(int)tdm_.ucChannel;
   }
   //get tdm str
   char* get_tdmstr(char* pbuf, int nlen){
      snprintf(pbuf,nlen,"B%02dT%02dC%02d",tdm_.ucBoard,tdm_.ucTrunk,tdm_.ucChannel);
      return pbuf;
   }
   //get compses id
   unsigned int get_sesidx( ){
      return (((tdm_.ucTrunk)*32)+tdm_.ucChannel);
   }
   static unsigned int get_sesidx(const int ntid, const int ncid ){
      return ((ntid*32)+ncid);
   }
   static unsigned int get_sesidx( const unsigned int nsesid ){
      return ((0x000000FF&nsesid) + (((0x0000FF00&nsesid)>>8)*32));
   }
};
///defines common body
enum SVC_COMMON_TYPE
{
   SVC_SETUP_APPS = 0xF001,   //SVC_SETUP_APPS
   SVC_ALIVE      = 0xF002,   //no body

   SVC_OOS_CHECK  = 0xF003,   //no body, MGI<->MPRM, H324RM
};

struct SVCSetupApp
{
   //the application or component id
   unsigned int uiAppId;
   //the max session number
   //if an app is tdm, set the number of e1.
   unsigned int uiSesnSnt;

   void hton(){
      uiAppId=htonl(uiAppId);
      uiSesnSnt=htonl(uiSesnSnt);
   }

   void ntoh(){
      uiAppId=ntohl(uiAppId);
      uiSesnSnt=ntohl(uiSesnSnt);
   }
};

//////////////// DTMF INFO
///defines the dtmf from rfc2833
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

///get the digit dtmf mask
inline unsigned char GetDTMFInt2Char(unsigned char& ucdtmf,unsigned short usdtmf)
{
   ucdtmf = 0x00 ;
   if ( DTMF_0 == usdtmf ){
      ucdtmf ='0';
   }else if ( DTMF_1 <= usdtmf  && usdtmf <= DTMF_9 ){
      ucdtmf = ((int)'9' - ( DTMF_9 - usdtmf));
   }else if ( usdtmf == DTMF_STAR ){
      ucdtmf = '*';
   }else if ( usdtmf == DTMF_PND ){
      ucdtmf = '#';
   }else if ( usdtmf == DTMF_A ){
      ucdtmf = 'A';
   }else if ( usdtmf == DTMF_B ){
      ucdtmf = 'B';
   }else if ( usdtmf == DTMF_C ){
      ucdtmf = 'C';
   }else if ( usdtmf == DTMF_D ){
      ucdtmf = 'D';
   }
   return ucdtmf; 
}

inline unsigned short GetDTMFChar2Int(unsigned char ucdtmf,unsigned short& usdtmf)
{
   usdtmf=DTMF_NONE;
   if ( '0' <= ucdtmf && ucdtmf <= '9' ){
      usdtmf = (unsigned short)DTMF_9 - ((unsigned short)((int)'9' - (int)ucdtmf));
   }else if ( ucdtmf == '*' ){
      usdtmf = (unsigned short)DTMF_STAR;
   }else if ( ucdtmf == '#' ){
      usdtmf = (unsigned short)DTMF_PND;
   }else if ( ucdtmf == 'A' ){
      usdtmf = (unsigned short)DTMF_A;
   }else if ( ucdtmf == 'B' ){
      usdtmf = (unsigned short)DTMF_B;
   }else if ( ucdtmf == 'C' ){
      usdtmf = (unsigned short)DTMF_C;
   }else if ( ucdtmf == 'D'){ 
      usdtmf = (unsigned short)DTMF_D;
   }
   return usdtmf;
}
//@trace

struct SVCTraceUserInfo_t{
   unsigned char ucTraceFlag; // enum REQ_MUTIBITMASK, set by TGAS
   unsigned char ucTID;
   unsigned char ucLevel;
	unsigned char ucReserved[5];

	unsigned int  uiDstIP;
	unsigned int  uiDstPort;
};

struct SVCTrace_t{
   enum { MAX_TRACE_NUM = 100 };
   ///defines trace type. set by trace soure(TGAS,TGA,MSRPA)
	enum TRACETYPE{ 
      ALL     = 0,
      CALL    = 1,
      MEDIA   = 2,
      DUMP    = 3
   };

   //set by TGAS
	enum REQ_MUTIBITMASK{ 
      SET_OFF      = 0x00, //OFF.
      SET_IPC      = 0x01, //TGAS 
		SET_MSRP     = 0x02, //MSRPA
		SET_RTP      = 0x04, //TGA
		SET_SIP      = 0x08, //SIP
		SET_RTCP     = 0x10, //TGA
		SET_DUMP     = 0x20, //TGA
		SET_DIAM     = 0x40, //SIP
		SET_ALL      = 0xFF 
   };

   unsigned int  uiDstUserInfoCount;
	char          cReserved[4];
   SVCTraceUserInfo_t DstUserInfoArray[MAX_TRACE_NUM];

	char * str(char * buf,int len)
	{
      char sztmp[128];sztmp[0]=0x00;
      char sztmp2[128];sztmp2[0]=0x00;
	   snprintf(buf,len-1, "UIcnt=%d", uiDstUserInfoCount);

      int i,count ;
      count = (uiDstUserInfoCount>MAX_TRACE_NUM)?MAX_TRACE_NUM:uiDstUserInfoCount ;
      for(i = 0 ; i < count ; i++){
      	str_flag(i,sztmp,sizeof(sztmp));
         snprintf(sztmp2,sizeof(sztmp2),
				" UI%d(trcflag:%04x(%s) ID=%d,Level=%d,IP=%d.%d.%d.%d,Port=%d)",
				i,DstUserInfoArray[i].ucTraceFlag,sztmp,
            DstUserInfoArray[i].ucTID, DstUserInfoArray[i].ucLevel,
				((DstUserInfoArray[i].uiDstIP&0xFF000000)>>24), 
				((DstUserInfoArray[i].uiDstIP&0x00FF0000)>>16), 
				((DstUserInfoArray[i].uiDstIP&0x0000FF00)>>8), 
				((DstUserInfoArray[i].uiDstIP&0x000000FF)),
				DstUserInfoArray[i].uiDstPort);
          strcat(buf,sztmp2);
      }
		return buf;
	}

	char* str_flag(int idx,char * buf,int len)
	{
		buf[0]='\0';
      if( DstUserInfoArray[idx].ucTraceFlag == SET_OFF )
      	snprintf(buf+strlen(buf),len-1,"OFF");
      if ( DstUserInfoArray[idx].ucTraceFlag & SET_IPC  ){
         snprintf(buf+strlen(buf),len-1,"|IPC");
      }
      if ( DstUserInfoArray[idx].ucTraceFlag & SET_MSRP ){
         snprintf(buf+strlen(buf),len-1,"|MSRP");
      }
      if ( DstUserInfoArray[idx].ucTraceFlag & SET_RTP ){
         snprintf(buf+strlen(buf),len-1,"|RTP");
      }
      if ( DstUserInfoArray[idx].ucTraceFlag & SET_SIP ){
         snprintf(buf+strlen(buf),len-1,"|SIP");
      }
      if ( DstUserInfoArray[idx].ucTraceFlag & SET_RTCP ){
         snprintf(buf+strlen(buf),len-1,"|RTCP");
      }
      if ( DstUserInfoArray[idx].ucTraceFlag & SET_DUMP ){
         snprintf(buf+strlen(buf),len-1,"|DUMP");
      }
		return buf;
	}
   void hton(){
		unsigned int i;
      for(i = 0 ; i < uiDstUserInfoCount ; i++)
		{
			DstUserInfoArray[i].uiDstIP = htonl(DstUserInfoArray[i].uiDstIP);
			DstUserInfoArray[i].uiDstPort = htonl(DstUserInfoArray[i].uiDstPort);
		}
      uiDstUserInfoCount = htonl(uiDstUserInfoCount);
   }
   void ntoh(){

      uiDstUserInfoCount = ntohl(uiDstUserInfoCount);
      for(unsigned int i = 0 ; i < uiDstUserInfoCount ; i++)
		{
			DstUserInfoArray[i].uiDstIP = ntohl(DstUserInfoArray[i].uiDstIP);
			DstUserInfoArray[i].uiDstPort = ntohl(DstUserInfoArray[i].uiDstPort);
		}
   }
};

// added by MIND
#define MOBILE_NUMBER_MAX (31)

#endif
