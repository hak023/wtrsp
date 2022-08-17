#ifndef __IBCF_TRACE_H__
#define __IBCF_TRACE_H__

#include "eventdefs.h"

#define MSG_FUNC_TRACE             0x800
#define MSG_SUBF_TRACE_NOTIFY      0x801
#define MSG_SUBF_TRACE_STAT_NOTIFY      0x810

#define TRACE_MAX_MIN        20   //Min 개수
#define TRACE_MAX_MIN_LEN    16  //Min 길이
#define TRACE_DATA_MAX_LEN   7200 // 호추적 로그 길이
#define TRACE_TITLE_MAX_LEN   128// 호추적 요약 정보 길이
#define TRACE_MIN_CALLER      1
#define TRACE_MIN_CALLED      2
#define TRACE_CODEC_NAME_MAX_LEN    16  //호추적 코덱 이름 길이

enum e_trace_node_id {
	e_node_cscf_cr = 0,      // caller side cscf
	e_node_ibcf,             // ibcf
	e_node_trgw,             // trgw
	e_node_cscf_cd,          // called side cscf
	e_node_max,     
};

typedef struct {
	int nTraceId;
	char szCaller[TRACE_MAX_MIN_LEN];
	char szCalled[TRACE_MAX_MIN_LEN];
	int nTypeId;
	int nFromNodeId;
	int nToNodeId;
	int nLevel;
	int nSTimeSec;
	int nSTimeUSec;
	int nLocation;
	int nResult;
	int nReserve;
	int nEnd;
	int nDataLen;
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
	}

} TRACE_STAT, *PTRACE_STAT;


#define TRACE_LOG_MAX_LEN  8192

typedef struct {
   XTMMSGHDR     h;
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

#endif
