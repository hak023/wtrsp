#ifndef __CALL_TRACE_H__
#define __CALL_TRACE_H__

#include "eventdefs_vic.h"

//////////////////////////////////////////////////////////////////////////////
#define    TRACE_MAX_MIN        10   //Min 개수
#define    TRACE_MAX_MIN_LEN    16  //Min 길이

#define    TRACE_MIN_CALLER      1
#define    TRACE_MIN_CALLED      2

#define    TRACE_MIN             1
#define    TRACE_CHANNEL         2
#define    TRACE_CHANNEL_REL     3

#define MSG_TYPE_TRACE                    				0x0800
#define MSG_SUBTYPE_TRACE_NOTIFY           0x0851

/////////////TRACE_NEW///////////////////////////////////////////////////
typedef struct {
   unsigned int     nStatus; //value : 0(OFF), 1(ON)
   unsigned int     nWho;        //(TRACE_MIN_CALLER:1, TRACE_MIN_CALLED:2)
   char             cMinNo[TRACE_MAX_MIN_LEN];
   unsigned int     nMinNo;
   unsigned int     nCount;
   unsigned int     lStartTime;  //long int : trace start time
   unsigned int     lEndTime;    //           trace end time
   unsigned int     nLevel;
   unsigned int     nUserLevel;
   char             cUserID[32];
} TRACE_MIN_INFO, *PTRACE_MIN_INFO; 

typedef struct {
   unsigned int    nSet;
   unsigned int    reserved;
   TRACE_MIN_INFO  stMInfo[TRACE_MAX_MIN];
} TRACE_INFO, *PTRACE_INFO; //size 
//////////////////////////////////////////////////////////////////////////////
#define  TRACE_INFO_MAX_LEN   2048

typedef struct {
  int       nTraceRel; //1:Release, else
  int       nTraceId;
  char		stSDNCaller[16];		
  char		stSDNCalled[16];		
  char		cTraceLog[TRACE_INFO_MAX_LEN];

	void hton()
	{
		nTraceRel = htonl(nTraceRel);
		nTraceId = htonl(nTraceId);
	}
} TRACE_LOG, *PTRACE_LOG;

typedef struct {
  char            szPhoneid[16];
  char            szPinNo[8];
  unsigned short   usResult;
  unsigned short   usReason;
} TRACE_MSGBASIC, *PTRACE_MSGBASIC;

typedef struct 
{
	XTMMSGHDR h;
	ARSLINE line;

#if 1
	TRACE_LOG traceLog;   //Trace log
#else
	union u {
		TRACE_INFO    traceInfo;  //Trace 설정 정보
		TRACE_LOG     traceLog;   //Trace log
	} pParm;
#endif

	void hton()
	{
		h.hton();
		line.hton();
		traceLog.hton();
	}
} CALL_TRACE, *PCALL_TRACE;

#endif //__CALL_TRACE_H__

