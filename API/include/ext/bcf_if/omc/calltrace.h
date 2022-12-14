#ifndef __CALL_TRACE_H__
#define __CALL_TRACE_H__

#include "eventdefs.h"

#define MSG_SUBF_TRACE_STAT_NOTIFY        0x0810

//////////////////////////////////////////////////////////////////////////////
#define    TRACE_MAX_MIN        100   //Min 개수
#define    TRACE_MAX_MIN_LEN    16  //Min 길이

#define    TRACE_MIN_CALLER      1
#define    TRACE_MIN_CALLED      2

#define    TRACE_MIN             1
#define    TRACE_CHANNEL         2
#define    TRACE_CHANNEL_REL     3
#define TRACE_DATA_MAX_LEN   7200 // 호추적 로그 길이
#define TRACE_TITLE_MAX_LEN   128// 호추적 요약 정보 길이
#define TRACE_CODEC_NAME_MAX_LEN    16  //호추적 코덱 이름 길이
#define TRACE_CALLID_MAX_LEN    128  //호추적 호 ID 길이

#define TRACE_INFO_MAX_LEN   2048

enum e_trace_node_id {
		e_node_Internal = 0,      // 내부망
		e_node_LB,          
		e_node_CM,        
		e_node_EMP,        
		e_node_External,          // 외부망
		e_node_max,
};

typedef struct {
	unsigned int nTraceId;
	char szCallId[TRACE_CALLID_MAX_LEN];
	char szCaller[TRACE_MAX_MIN_LEN];
	char szCalled[TRACE_MAX_MIN_LEN];
	unsigned int nTypeId;
	unsigned int nFromNodeId;
	unsigned int nToNodeId;
	unsigned int nLevel;
	unsigned int nSTimeSec;
	unsigned int nSTimeUSec;
	unsigned int nLocation;
	unsigned int nResult;
	unsigned int nReserve;
	unsigned int nFromTargetId;
	unsigned int nToTargetId;
	unsigned int nEnd;
	unsigned int nDataLen;
	char szFromNodeName[TRACE_TITLE_MAX_LEN];
	char szToNodeName[TRACE_TITLE_MAX_LEN];
	char szTitle[TRACE_TITLE_MAX_LEN];
	char szTraceLog[TRACE_DATA_MAX_LEN];

	void hton()
	{
		nTraceId = htonl(nTraceId);
		nTypeId = htonl(nTypeId);
		nFromNodeId = htonl(nFromNodeId);
		nToNodeId = htonl(nToNodeId);
		nLevel = htonl(nLevel);
		nSTimeSec = htonl(nSTimeSec);
		nSTimeUSec = htonl(nSTimeUSec);
		nEnd = htonl(nEnd);
		nDataLen = htonl(nDataLen);
	}

	void ntoh()
	{
		nTraceId = ntohl(nTraceId);
		nTypeId = ntohl(nTypeId);
		nFromNodeId = ntohl(nFromNodeId);
		nToNodeId = ntohl(nToNodeId);
		nLevel = ntohl(nLevel);
		nSTimeSec = ntohl(nSTimeSec);
		nSTimeUSec = ntohl(nSTimeUSec);
		nEnd = ntohl(nEnd);
		nDataLen = ntohl(nDataLen);
	}
} TRACE_LOG, *PTRACE_LOG;


typedef struct {
	int nTraceId;
	char szCallId[TRACE_CALLID_MAX_LEN];
	char szCaller[TRACE_MAX_MIN_LEN];
	char szCalled[TRACE_MAX_MIN_LEN];
	int nSTimeSec;
	int nInAudioTxBps;
	int nInAudioRxBps;
	int nInVideoTxBps;
	int nInVideoRxBps;
	int nExtAudioTxBps;
	int nExtAudioRxBps;
	int nExtVideoTxBps;
	int nExtVideoRxBps;
	int nStart;
	int nResreve;
	char szAudioCodec[TRACE_CODEC_NAME_MAX_LEN];
	char szVideoCodec[TRACE_CODEC_NAME_MAX_LEN];

        void hton()
        {
                nTraceId = htonl(nTraceId);
                nSTimeSec = htonl(nSTimeSec);
                nInAudioTxBps = htonl(nInAudioTxBps);
                nInAudioRxBps = htonl(nInAudioRxBps);
                nInVideoTxBps = htonl(nInVideoTxBps);
                nInVideoRxBps = htonl(nInVideoRxBps);
                nExtAudioTxBps = htonl(nExtAudioTxBps);
                nExtAudioRxBps = htonl(nExtAudioRxBps);
                nExtVideoTxBps = htonl(nExtVideoTxBps);
                nExtVideoRxBps = htonl(nExtVideoRxBps);
                nStart = htonl(nStart);
        }

        void ntoh()
        {
                nTraceId = ntohl(nTraceId);
                nSTimeSec = ntohl(nSTimeSec);
                nInAudioTxBps = ntohl(nInAudioTxBps);
                nInAudioRxBps = ntohl(nInAudioRxBps);
                nInVideoTxBps = ntohl(nInVideoTxBps);
                nInVideoRxBps = ntohl(nInVideoRxBps);
                nExtAudioTxBps = ntohl(nExtAudioTxBps);
                nExtAudioRxBps = ntohl(nExtAudioRxBps);
                nExtVideoTxBps = ntohl(nExtVideoTxBps);
                nExtVideoRxBps = ntohl(nExtVideoRxBps);
                nStart = ntohl(nStart);
        }

} TRACE_STAT, *PTRACE_STAT;


#define TRACE_LOG_MAX_LEN  8192

typedef struct {
   char szVnfId[64];  //system
   char szVnfcId[64]; //server
} VNID, *PVNID;

typedef struct {
   XTMMSGHDR     h;
   VNID          v;
        union {
        TRACE_LOG      tracelog;
        TRACE_STAT     tracestat;
        };
   int GetLogLen()
   {
      int len = h.nMsgLen - (sizeof(h) + sizeof(tracelog) - tracelog.nDataLen);
      return len;
   }
   
   void hton()
   {
      h.hton();
		if (h.usSubType == MSG_SUBF_TRACE_STAT_NOTIFY)
			tracestat.hton();
		else
			tracelog.hton();
   }

   void ntoh()
   {
      h.ntoh();
                if (h.usSubType == MSG_SUBF_TRACE_STAT_NOTIFY)
        tracestat.ntoh();
                else
        tracelog.ntoh();
   }

} MSGTRACELOG, *PMSGTRACELOG;

#if 0
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
  char		stSDNCaller[32];		
  char		stSDNCalled[32];		
  char		cTraceLog[TRACE_INFO_MAX_LEN];
} TRACE_LOG, *PTRACE_LOG;

typedef struct {
  char            szPhoneid[16];
  char            szPinNo[8];
  unsigned short   usResult;
  unsigned short   usReason;
} TRACE_MSGBASIC, *PTRACE_MSGBASIC;

typedef struct {
  XTMMSGHDR       h;
  ARSLINE         line;
//  TRACE_MSGBASIC  mb;
  union u {
    TRACE_INFO    traceInfo;  //Trace 설정 정보
    TRACE_LOG     traceLog;   //Trace log
  } pParm;
} CALL_TRACE, *PCALL_TRACE;
#endif

/////////////////////////////////////////////////////////////////////////////
#endif //__CALL_TRACE_H__

