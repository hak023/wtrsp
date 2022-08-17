/*

   HURIM Copy Right

	1. DB Server <=> App  Common Header File 

	File : apptypes.h
	Create Date : 2001. by Jeong
	Updated : 2002.1 by Windjun
             
*/

#ifndef __APPTYPES_H__
#define __APPTYPES_H__

//#include "vmscomm.h"
#include "eventdefs.h"

//////////////////////////////////////////////////////////////////////////////
/* MSG_FUNC's and SUBF's */
#define MSG_FUNC_OMC_CONTROL			   0x0301
#define MSG_FUNC_OMC_RCV				   0x0302
#define MSG_FUNC_DPRINTF_CTRL			   0x0303
#define MSG_FUNC_IVR_SVC				   0x0304
#define MSG_FUNC_ALARM					   0x0500
//#define MSG_FUNC_TRACE                 0x030c //totoy temp

/* For IVR_SVC */
#define MSG_SUBF_USER_CERT 			   0x0351 // 가입자 인증 요청
#define MSG_SUBF_PIN_UPDATE 			   0x0352 // VMS 비밀번호 변경
#define MSG_SUBF_GREET_CHG 			   0x0353 // 인사말 변경 요청
#define MSG_SUBF_NAME_CHG 				   0x0354 // 이름 변경 요청 
#define MSG_SUBF_MAIL_SAVE 			   0x0355 // 호출 정보 저장 요청 
#define MSG_SUBF_MAIL_INFO				   0x0356 // 호출 정보 조회 요청 
#define MSG_SUBF_DTMF_SAVE 			   0x0357 // 사용자가 사용한 메뉴 저장
#define MSG_SUBF_MAIL_DEL   			   0x0358 // 호출 정보 삭제 요청
#define MSG_SUBF_SMS_SEND  			   0x0359 // SMS호출 요구
#define MSG_SUBF_BOARDID_SEND          0x0360 // App P.의 Board ID 전송
#define MSG_SUBF_MAIL_DEL_ALL          0x0361 // 전체메일 삭제 요청
#define MSG_SUBF_DTMF_WATCH				0x0362 // DTMF 모니터링
#define MSG_SUBF_OGREQ_FOR_NONSMS      0x0363 // OutGoing Call Req
#define MSG_SUBF_MAIL_UPDATE   			0x0364 // 호출 정보 변경 요청
#define MSG_SUBF_HACK_SAVE             0x0365 // Hacking

#define MSG_SUBF_YOU_ARE_ACTIVE        0x0366 // Master APP
#define MSG_SUBF_BOARD_ALARM           0x0367 // Board Alarm
#define MSG_SUBF_LINE_CHECK            0x0368 // Line Check

#define MSG_SUBF_TRACE_ON              0x0c01 // Call Trace ON
#define MSG_SUBF_TRACE_OFF             0x0c02 // Call Trace OF
#define MSG_SUBF_TRACE_LOG             0x0c11 // Call Trace Log

/* For Message */
#define MSG_TYPE_REQ			            0
#define MSG_TYPE_RSP			            1
#define MSG_TYPE_FAIL                  3 

#define VMS_SYSID_DBSVR 		         0x0101
#define VMS_SYSID_IVR 			         0x0102

#define VMS_SYSID_PARENTID_DBSVRPM 		0x0131
#define VMS_SYSID_PARENTID_IVRIF 		0x0132
#define VMS_SYSID_PARENTID_CDSIF 		0x0133
#define VMS_SYSID_PARENTID_SMSIF 		0x0134

#define VMS_SYSID_PARENTID_OMCIF 		0x0171

#define VMS_SYSID_PARENTID_IVRPC 		0x0181

#define MAIL_BOX_INFO_VOICE            1
#define MAIL_BOX_INFO_TEXT             2

#define IVR_SVC_FAIL_DB_ACCESS_FAIL    0x0001
#define IVR_SVC_FAIL_NOT_FOUND         0x0002

//add 2002.4.9
#define IVR_SVC_FAIL_OG_FULL           0x0003
#define IVR_SVC_FAIL_NOT_OG_BOARD      0x0004
#define IVR_SVC_FAIL_NOT_READY_SS7     0x0005
#define IVR_SVC_FAIL_INVALID_MSG       0x0006
#define IVR_SVC_FAIL_SS7_REJECT        0x0007
#define IVR_SVC_FAIL_NOT_READY_BOARD   0x0008

//////////////////////////////////////////////////////////////////////////////
typedef ARSLINE VMSIVRLINE;
typedef PARSLINE PVMSIVRLINE;

typedef struct {
	char	        szPhoneid[16];
	char	        szPinNo[8];
	unsigned short	usResult;
	unsigned short	usReason;
} VMSMSGBASIC, *VMSMSGBASICPTR;

// IVR에서 가입자 정보를 요구할 때 사용
typedef struct {
	unsigned short	usUserClass;
	unsigned short	usUserStatus;
	unsigned short	usSMSStatus;
	unsigned short	usNameFlag;
	char	szName[128];
	unsigned short	usGreetFlag;
	char			   szGreet[128];
	unsigned short	usNumVNewMail;
	unsigned short	usNumTNewMail;
	unsigned short	usNumVTotMail;
	unsigned short	usNumTTotMail;
	unsigned short	usTraceFlag;
	char			   szCreated[16];
	unsigned short	usCheckNonHLR;
	unsigned short	_reserved[1];
} VMSUSERINFO, *VMSUSERINFOPTR;

// IVR에서 각각의  가입자의 저장된 
// 메일들에 대한 정보 요청시 사용
typedef struct {
	unsigned short	usMsgType;
	unsigned short	usNumTotMail;
	unsigned short	usMsgIndex;
	char			   szMsgLoc[128];
	unsigned short	usPlayed;
	unsigned short	usReqCallFlag;
	char			   szReqCallId[16];
	char			   szCreated[16];
	char			   szCrUSec[8];
	unsigned short	_reserved[3];
} VMSMAILINFO, *VMSMAILINFOPTR;

// IVR에서 각각의 사용자가 사용한
// 메뉴 History 저장 요청시 사용
typedef struct {
	unsigned short usInOut;        //In/Out Going
	unsigned int  ulStartTime;    //IVR Service Start Time
	unsigned int  ulEndTime;      //IVR Service Stop Time;
   char           szMenuRatio[64];//Menu Ratio
	char			   szDTMFHistory[128];
	char           szCallerId[32]; //Caller ID : S7_MAX_ADDRESS - 32
} VMSCDRINFO, *VMSCDRINFOPTR;

//SMS Info
typedef struct {
   unsigned short usMsgType;
	char           szCallbackId[20]; //호출자가 남겨놓은 번호
	char           szMessage[200];   //호출자가 남겨놓은 메세지
	unsigned short usUrgency;        //1:지금바로호출, 0:예약호출
	char           szMessageId[20];  //?
	unsigned int  ulTransReq;       //예약호출시간
	unsigned short _reserved[1];
} VMSSMSINFO, *VMSSMSINFOPTR;

// ONM에서의 Process Restart 등의 메시지를
// 전달할 때 사용
typedef struct {
	union u {
		char	      szParams[128];
		int         nParams[20];
	} uParams;
} VMSMSGSIMPLE, *VMSMSGSIMPLEPTR;

//////////////////////////////////////////////////////////////////////////////
typedef struct {
	XTMMSGHDR	  h;
	ARSLINE		  li;
	VMSMSGBASIC       mb;
   union u {
	   VMSUSERINFO    stUInfo;
	   VMSMAILINFO    stMInfo;
		VMSCDRINFO     stDTMF;
		VMSSMSINFO     stSInfo;      
		VMSMSGSIMPLE   stSMS;
   } uParams;
} IVRAPPMSG, *PIVRAPPMSG;

//////////////////////////////////////////////////////////////////////////////
typedef enum {
	START_TIME,
	STOP_TIME
} TIMERACTION;

#endif // __APPTYPES_H__
//////////////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////////////
