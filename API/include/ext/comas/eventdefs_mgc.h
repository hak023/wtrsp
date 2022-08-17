
#ifndef _EVENT_DEFINITION_H_
#define _EVENT_DEFINITION_H_

#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
//#include "jthread.h"

#define MAX_EVENTINFO_SIZE (8 * 1024)
#define EVENTINFO_HEADER_LENGTH (sizeof(XTMMSGHDR) + sizeof(ARSLINE))
#define MAX_VALUE_SIZE (MAX_EVENTINFO_SIZE - EVENTINFO_HEADER_LENGTH)

#define EVENTQUEUE   CEventQueue
#define PEVENTQUEUE  CEventQueue *


#if defined(_LINUX_) && !defined(uint16_t)
#	include <stdint.h>
#endif

#ifndef XTMADDR_DEF
#	define XTMADDR_DEF
typedef struct {
   unsigned short usZoneId;
   unsigned short usSysId;
   unsigned short usParentId;
   unsigned short usChildId;
   unsigned int   ulTmpPid;
#if 1
   void ntoh() {
   	usZoneId	  =	ntohs(usZoneId	 )	;
	   usSysId	  =	ntohs(usSysId	 )	;
	   usParentId =	ntohs(usParentId) ;
	   usChildId  =	ntohs(usChildId ) ;
	   ulTmpPid	  =	ntohl(ulTmpPid	 )	;}
   void hton() {
   	usZoneId	  =	htons(usZoneId	 )	;
	   usSysId	  =	htons(usSysId	 )	;
	   usParentId =	htons(usParentId) ;
	   usChildId  =	htons(usChildId ) ;
	   ulTmpPid	  =	htonl(ulTmpPid	 )	;}
#endif
} XTMADDR, *PXTMADDR, VMSADDR, *VMSADDRPTR;;

#define XtmAddr(st, Zone, Sys, Parent, Child) \
   ((st).unZoneId = (Zone), (st).usSysId = (Sys), \
    (st).usParentId = (Parent), (st).usChildId = (Child), (st))

typedef struct {
   unsigned int 	nMagicCookie;
   int 				nMsgLen;
   XTMADDR 			stReceiver;
   XTMADDR 			stSender;
   short 			snHopCnt;
   unsigned short usTransactionId;
   short 			snSerNo;
//#ifdef NEW_VERSION
#if 1
   unsigned short	_reserved;
   unsigned short	usType;
   unsigned short	usSubType;
#else
   unsigned short	usType;
   unsigned short usSubType;
   unsigned short _reserved;
#endif

#if 1
	void ntoh() {
   	nMagicCookie	  	=	ntohl(nMagicCookie		)	;
	   nMsgLen	  			=	ntohl(nMsgLen	  			)	;
	   stReceiver.ntoh();
	   stSender.ntoh();
	   snHopCnt	  			=	ntohs(snHopCnt	  			)	;
	   usTransactionId	=	ntohs(usTransactionId	)	;
	   snSerNo	  			=	ntohs(snSerNo	  			)	;
	   _reserved	  		=	ntohs(_reserved	  			)	;
	   usType	  			=	ntohs(usType	  			)	;
	   usSubType	  		=	ntohs(usSubType	  			)	;
	}
   void hton() {
   	nMagicCookie	  	=	htonl(nMagicCookie		)	;
	   nMsgLen	  			=	htonl(nMsgLen	  			)	;
	   stReceiver.hton();
	   stSender.hton();
	   snHopCnt	  			=	htons(snHopCnt	  			)	;
	   usTransactionId	=	htons(usTransactionId	)	;
	   snSerNo	  			=	htons(snSerNo	  			)	;
	   _reserved	  		=	htons(_reserved	  			)	;
	   usType	  			=	htons(usType	  			)	;
	   usSubType	  		=	htons(usSubType	  			)	;
	}
#endif
} XTMMSGHDR, *PXTMMSGHDR, VMSMSGHDR, *VMSMSGHDRPTR;

#endif // XTMADDR_DEF

typedef struct {
   unsigned short usAreaId			;
   unsigned short usSystemId		;
   unsigned short usBoardId		;
   unsigned short usTrunkId		;
   unsigned short usChannelId		;
   unsigned short usReserved[3]	;
#if 1
   void ntoh() {
   	usAreaId		=	ntohs(usAreaId				)	;
	   usSystemId	=	ntohs(usSystemId			)	;
	   usBoardId	=	ntohs(usBoardId			)	;
	   usTrunkId	=	ntohs(usTrunkId			)	;
	   usChannelId	=	ntohs(usChannelId 		)	;
	}
   void hton() {
   	usAreaId		=	htons(usAreaId				)	;
	   usSystemId	=	htons(usSystemId			)	;
	   usBoardId	=	htons(usBoardId			)	;
	   usTrunkId	=	htons(usTrunkId			)	;
	   usChannelId	=	htons(usChannelId 		)	;
	}
#endif
} ARSLINE, *PARSLINE, VMSIVRLINE, *VMSIVRLINEPTR;;

typedef struct {
   XTMMSGHDR h;
   ARSLINE line;
   char Value[MAX_VALUE_SIZE];
} EVENTINFO, *PEVENTINFO;

#define EVENTINFO_LENGTH(e)        ((e)->h.nMsgLen)
#define EVENTINFO_VALUE_LENGTH(e)  ((e)->h.nMsgLen - EVENTINFO_HEADER_LENGTH)
#define EVENTINFO_MAGIC(e)         ((e)->h.nMagicCookie)
#define EVENTINFO_RECEIVER(e)      ((e)->h.stReceiver.usSysId)
#define EVENTINFO_SENDER(e)        ((e)->h.stSender.usSysId)
#define EVENTINFO_RECV_PARENTID(e) ((e)->h.stReceiver.usParentId)
#define EVENTINFO_TYPE(e)          ((e)->h.usType)
#define EVENTINFO_SUBTYPE(e)       ((e)->h.usSubType)
#define EVENTINFO_MODULEID(e)      ((e)->h.usTransactionId)
#define EVENTINFO_TYPES(e)         ((uint32_t)((e)->h.usType))
#define EVENTINFO_AREAID(e)        ((e)->line.usAreaId)
#define EVENTINFO_SYSTEMID(e)      ((e)->line.usSystemId)
#define EVENTINFO_CLIENTID(e)      ((e)->line.usBoardId)
#define EVENTINFO_BOARD(e)         ((e)->line.usBoardId)
#define EVENTINFO_TRUNK(e)         ((e)->line.usTrunkId)
#define EVENTINFO_CHANNEL(e)       ((e)->line.usChannelId)

#define DEF_EVENT_QUEUE_LENGTH    32
#define BOARD_EVENT_QUEUE_LENGTH  DEF_EVENT_QUEUE_LENGTH * 4
#define APP_EVENT_QUEUE_LENGTH  	 BOARD_EVENT_QUEUE_LENGTH //* 4
#define EVENT_RETRY_COUNT  		 1
#define EVENT_RETRY_DELAY 		    100

///////////////////////////////////////////////////////////
//Fish Add -2008.08.02
typedef enum {
	//msMMC Event Type
   CI_GTS_ARS_MSG            =0x50E1,
	CI_CALL_ADMISSION_REQ      =0x50E2,
	CI_CALL_ADMISSION_RSP      =0x50E3,
	CI_CALL_CONNECT_REQ        =0x50E4,
   CI_CALL_CONNECT_RSP        =0x50E5,
   CI_CALL_RELEASE_REQ        =0x50E6,
   CI_CALL_RELEASE_RSP        =0x50E7,
   CI_CALL_DISCONNECT_REQ     =0x50E8,
   CI_CALL_DISCONNECT_RSP     =0x50E9,
   CI_MENT_PLAY_REQ           =0x50EA,
   CI_MENT_PLAY_RSP           =0x50EB,
   CI_MENT_PLAY_DONE          =0x50EC,
   CI_DTMF_NOTIFY             =0x50ED,
   CI_CALL_STAT_DATA          =0x50EE,
   CI_DTMF_SINGLE             =0x000d,
   CI_CALL_CLEAR_REQ          =0x0010,
   CI_CALL_CLEAR_RSP          =0x0011,
   CI_VXML_REQUEST            =0x1000,
   CI_VXML_APP_RESPONSE,
   CI_JUMP_DOC,
   CI_VXML_DONE,
   CI_TIMER_DONE,
   CI_DTMF_MULTI,

	CI_NOMATCH,
	CI_NOINPUT,
	CI_ALARM,
   CI_QUIT_THREAD,
   CI_IDLE_TIME_OUT,
   CI_HOLDING_TIME_OUT,
/*
   USER_CERT_REQ,
	USER_CERT_ACK,
	PIN_UPDATE_REQ,
	PIN_UPDATE_ACK,
	GREET_CHG_REQ,
	GREET_CHG_ACK,
	NAME_CHG_REQ,
	NAME_CHG_ACK,
	MAIL_SAVE_REQ,
	MAIL_SAVE_ACK,
	MAIL_INFO_REQ,
	MAIL_INFO_ACK,
*/
   CI_TIMER,
   CI_QUIT = 0x7fff,
   CI_NONE = 0x8000,
   CI_ACK = 0x8001,
   CI_LOGIN = 0x8002,
   CI_LOGOUT = 0x8003,
   CI_TIMEOUT = 0x8004,

} EVENTTYPE;

///////////////////////////////////////////////////////////
//typedef CQueue<PEVENTINFO, EVENTINFO> CEventQueue;

#endif
