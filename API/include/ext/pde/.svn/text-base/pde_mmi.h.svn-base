/*
 * Point-I Diameter Engine MMI Message header
 */
#ifndef _PDE_MMI_H_
#define _PDE_MMI_H_

#include "common_msg.h"

static const char* GetPM_RcStr(int rc);

#define MAX_PM_BODY_SIZE 8192
#define MAX_PDE_RMT_NUM 32
#define MAX_PDE_RTE_NUM 64


typedef struct {
	unsigned int uiID;
	void hton()
	{
		uiID = htonl(uiID);
	}
	void ntoh()
	{
		uiID = ntohl(uiID);
	}
	const char* str(char* szBuff,int nLen)
	{
		snprintf(szBuff,nLen,"id:%d",uiID);
		return szBuff;
	}
}stPM_ComReq;

typedef struct {
	unsigned int uiID;
	int nResult;
   int nReason;
   char szReasonDesc[128];

	void hton()
	{
		uiID = htonl(uiID);
		nResult = htonl(nResult);
		nReason = htonl(nReason);
	}
	void ntoh()
	{
		uiID = ntohl(uiID);
		nResult = ntohl(nResult);
		nReason = ntohl(nReason);
	}
	const char* str(char* szBuff,int nLen)
	{
		snprintf(szBuff,nLen-1,"id:%d result:%d reason:%#x(%s)",uiID, nResult, nReason, szReasonDesc);
		return szBuff;
	}
}stPM_ComRes;

typedef struct {
	enum {
		PROTO_TCP,
		PROTO_SCTP
	}_proto;
	unsigned int uiID;
	char szDesc[40];
	char szIP[40];
	short sIPVer;//0:ver4 , 1:ver6
	short sReserved;
	int nPort;
	char szHost[128];
	char szRealm[128];
	short sProtocol; //_proto
	short sUseTLS;//0: not use, 1: use
	void hton()
	{
		uiID = htonl(uiID);
		sIPVer = htons(sIPVer);
		nPort = htonl(nPort);
		sProtocol = htons(sProtocol);
		sUseTLS = htons(sUseTLS);
	}
	void ntoh()
	{
		uiID = ntohl(uiID);
		sIPVer = ntohs(sIPVer);
		nPort = ntohl(nPort);
		sProtocol = ntohs(sProtocol);
		sUseTLS = ntohs(sUseTLS);
	}
	const char* str(char* szBuff,int nLen)
	{
		snprintf(szBuff,nLen-1,"id:%d desc:%s ip:%s ipver:%d port:%d host:%s realm:%s proto:%d tls:%d",
			uiID,szDesc,szIP,sIPVer,nPort,szHost,szRealm,sProtocol,sUseTLS);
		return szBuff;
	}
}stHostInf;

typedef struct {
	int nResult;
   int nReason;
   char szReasonDesc[128];
	stHostInf stLN;
	void hton()
	{
		nResult = htonl(nResult);
		nReason = htonl(nReason);
		stLN.hton();	
	}
	void ntoh()
	{
		nResult = ntohl(nResult);
		nReason = ntohl(nReason);
		stLN.ntoh();
	}
	const char* str(char* szBuff,int nLen)
	{
		char szLN[10240]; szLN[0] = '\0';
		stLN.str(szLN, sizeof(szLN));
		snprintf(szBuff, nLen, "Result:%d Reason:%#x(%s) Local Node(%s)", 
			nResult, nReason, szReasonDesc, szLN);
		return szBuff;	
	}
}stPM_DisLNRes;

typedef struct {
	int nResult;
   int nReason;
   char szReasonDesc[128];

	unsigned int uiNum;//Remote Info Number
	stHostInf stRMT[MAX_PDE_RMT_NUM];
	void hton()
	{
		nResult = htonl(nResult);
		nReason = htonl(nReason);
		for(unsigned int i=0;i<uiNum;i++)
		{
			stRMT[i].hton();
		}
		uiNum = htonl(uiNum);
	}
	void ntoh()
	{
		nResult = ntohl(nResult);
		nReason = ntohl(nReason);
		uiNum = ntohl(uiNum);
		for(unsigned int i=0;i<uiNum;i++)
		{
			stRMT[i].ntoh();
		}
	}
	const char* str(char* szBuff,int nLen)
	{
		char szTmp[128];
		char szTmp2[128];
		char szRMT[10240]; szRMT[0] = '\0';
		for(unsigned int i=0;i<uiNum;i++)
		{
			szTmp[0] = '\0';
			szTmp2[0] = '\0';
			stRMT[i].str(szTmp2, sizeof(szTmp2));
			snprintf(szTmp, sizeof(szTmp), "[%d]%s", i, szTmp2);
			strcat(szRMT, szTmp);
		}
		snprintf(szBuff, nLen, "result:%d reason:%#x(%s) num:%d RMT(%s)", 
			nResult, nReason, szReasonDesc, uiNum, szRMT);
		return szBuff;
	}
}stPM_DisRMTRes;

typedef struct {
	stHostInf stRMT;
	void hton()
	{
		stRMT.hton();	
	}
	void ntoh()
	{
		stRMT.ntoh();
	}
	const char* str(char* szBuff,int nLen)
	{
		return stRMT.str(szBuff,nLen);	
	}
}stPM_AddRMTReq;

typedef struct {
	int nResult;
   int nReason;
   char szReasonDesc[128];
	stHostInf stRMT;
	void hton()
	{
		nResult = htonl(nResult);
		nReason = htonl(nReason);
		stRMT.hton();	
	}
	void ntoh()
	{
		nResult = ntohl(nResult);
		nReason = ntohl(nReason);
		stRMT.ntoh();
	}
	const char* str(char* szBuff,int nLen)
	{
		char szRMT[10240]; szRMT[0] = '\0';
		stRMT.str(szRMT, sizeof(szRMT));
		snprintf(szBuff, nLen, "Result:%d Reason:%#x(%s) Remote(%s)", 
			nResult, nReason, szReasonDesc, szRMT);
		return szBuff;	
	}
}stPM_AddRMTRes;

typedef stPM_ComReq stPM_DelRMTReq;

typedef stPM_ComRes stPM_DelRMTRes;

typedef stPM_AddRMTReq stPM_ChgRMTReq;

typedef stPM_AddRMTRes stPM_ChgRMTRes;

typedef struct {
	enum{
		STS_DISCONNECT,
		STS_CONNECT
	}_status;
	unsigned int uiID;
	char szDesc[40];
	int nLNID;
	int nRMTID;
	int nCallCnt;
	int nStatus;//_status
	void hton()
	{
		uiID = htonl(uiID);
		nLNID = htonl(nLNID);
		nRMTID = htonl(nRMTID);
		nCallCnt = htonl(nCallCnt);
		nStatus = htonl(nStatus);
	}
	void ntoh()
	{
		uiID = ntohl(uiID);
		nLNID = ntohl(nLNID);
		nRMTID = ntohl(nRMTID);
		nCallCnt = ntohl(nCallCnt);
		nStatus = ntohl(nStatus);
	}
	const char* str(char * szBuff,int nLen)
	{
		snprintf(szBuff,nLen-1,"id:%d desc:%s ln:%d rmt:%d call:%d sts:%d",
			uiID,szDesc,nLNID,nRMTID,nCallCnt,nStatus);
		return szBuff;
	}
}stRTEInf;

typedef struct {
	int nResult;
   int nReason;
   char szReasonDesc[128];

	unsigned int uiNum;//Route Info Number
	stRTEInf stRTE[MAX_PDE_RTE_NUM];
	void hton()
	{
		nResult = htonl(nResult);
		nReason = htonl(nReason);
		for(unsigned int i=0;i<uiNum;i++)
		{
			stRTE[i].hton();
		}
		uiNum = htonl(uiNum);
	}
	void ntoh()
	{
		nResult = ntohl(nResult);
		nReason = ntohl(nReason);
		uiNum = ntohl(uiNum);
		for(unsigned int i=0;i<uiNum;i++)
		{
			stRTE[i].ntoh();
		}
	}
	const char* str(char* szBuff,int nLen)
	{
		char szTmp[128];
		char szTmp2[128];
		char szRTE[10240]; szRTE[0] = '\0';
		for(unsigned int i=0;i<uiNum;i++)
		{
			szTmp[0] = '\0';
			szTmp2[0] = '\0';
			stRTE[i].str(szTmp2, sizeof(szTmp2));
			snprintf(szTmp, sizeof(szTmp), "[%d]%s", i, szTmp2);
			strcat(szRTE, szTmp);
		}
		snprintf(szBuff, nLen, "result:%d reason:%#x(%s) num:%d RTE(%s)", 
			nResult, nReason, szReasonDesc, uiNum, szRTE);
		return szBuff;
	}
}stPM_DisRTERes;

typedef struct {
	stRTEInf stRTE;
	void hton()
	{
		stRTE.hton();
	}
	void ntoh()
	{
		stRTE.ntoh();
	}
	const char* str(char* szBuff,int nLen)
	{
		return stRTE.str(szBuff,nLen);
	}
}stPM_AddRTEReq;

typedef struct {
	int nResult;
   int nReason;
   char szReasonDesc[128];

	stRTEInf stRTE;
	void hton()
	{
		nResult = htonl(nResult);
		nReason = htonl(nReason);
		stRTE.hton();
	}
	void ntoh()
	{
		nResult = ntohl(nResult);
		nReason = ntohl(nReason);
		stRTE.ntoh();
	}
	const char* str(char* szBuff,int nLen)
	{
		char szRTE[10240]; szRTE[0] = '\0';
		stRTE.str(szRTE, sizeof(szRTE));
		snprintf(szBuff, nLen, "Result:%d Reason:%#x(%s) Route(%s)", 
			nResult, nReason, szReasonDesc, szRTE);
		return szBuff;
	}
}stPM_AddRTERes;

typedef stPM_ComReq stPM_DelRTEReq;
typedef stPM_ComRes stPM_DelRTERes;

typedef stPM_AddRTEReq stPM_ChgRTEReq;
typedef stPM_AddRTERes stPM_ChgRTERes;

typedef stPM_DisRTERes stPM_DisSesRes;

typedef stPM_ComReq stPM_ConSesReq;
typedef stPM_ComRes stPM_ConSesRes;

typedef stPM_ComReq stPM_AbortSesReq;
typedef stPM_ComRes stPM_AbortSesRes;

typedef stPM_ComReq stPM_ShutdownSesReq;
typedef stPM_ComRes stPM_ShutdownSesRes;


typedef struct{
	unsigned int uiCERTimeout;
	unsigned int uiDWRTimeout;
	unsigned int uiDWRRetry;
	unsigned int uiMsgTimeout;
	unsigned int uiMsgRetry;
	void hton()
	{
		uiCERTimeout = htonl(uiCERTimeout);
		uiDWRTimeout = htonl(uiDWRTimeout);
		uiDWRRetry = htonl(uiDWRRetry);
		uiMsgTimeout = htonl(uiMsgTimeout);
		uiMsgRetry = htonl(uiMsgRetry);
	}
	void ntoh()
	{
		uiCERTimeout = ntohl(uiCERTimeout);
		uiDWRTimeout = ntohl(uiDWRTimeout);
		uiDWRRetry = ntohl(uiDWRRetry);
		uiMsgTimeout = ntohl(uiMsgTimeout);
		uiMsgRetry = ntohl(uiMsgRetry);
	}
	const char* str(char* szBuff,int nLen)
	{
		snprintf(szBuff, nLen, "cer_timeout:%d dwr_timeout:%d dwr_cnt:%d msg_timeout:%d msg_cnt:%d", 
			uiCERTimeout, uiDWRTimeout, uiDWRRetry, uiMsgTimeout, uiMsgRetry);
		return szBuff;
	}
}stPM_TimerInf;

typedef struct{
	int nResult;
   int nReason;
   char szReasonDesc[128];
	stPM_TimerInf stTimer;
	void hton()
	{
		nResult = htonl(nResult);
		nReason = htonl(nReason);
		stTimer.hton();
	}
	void ntoh()
	{
		nResult = ntohl(nResult);
		nReason = ntohl(nReason);
		stTimer.ntoh();
	}
	const char* str(char* szBuff,int nLen)
	{
		char szTimer[10240]; szTimer[0] = '\0';
		stTimer.str(szTimer, sizeof(szTimer));
		snprintf(szBuff, nLen, "Result:%d Reason:%#x(%s) Timer(%s)", 
			nResult, nReason, szReasonDesc, szTimer);
		return szBuff;
	}
}stPM_DisTimerRes;


typedef struct{
	stPM_TimerInf stTimer;
	void hton()
	{
		stTimer.hton();
	}
	void ntoh()
	{
		stTimer.ntoh();
	}
	const char* str(char* szBuff,int nLen)
	{
		char szTimer[10240]; szTimer[0] = '\0';
		stTimer.str(szTimer, sizeof(szTimer));
		snprintf(szBuff, nLen, "Timer(%s)", 
			szTimer);
		return szBuff;
	}
}stPM_ChgTimerReq;

typedef stPM_DisTimerRes stPM_ChgTimerRes;

enum PMReasonCode {
	pmRcNoErr = 0x0000,
	pmRcInvalidParm = 0x0001,
	pmRcAlreadyExistId = 0x0002,
	pmRcNotExistId = 0x0003
};

inline const char* GetPM_RcStr(int rc)
{
	switch(rc)
	{
	case pmRcNoErr: return "NoErr";
	case pmRcInvalidParm: return "InvalidParm";
	case pmRcAlreadyExistId: return "AlreadyExistId";
	case pmRcNotExistId: return "NotExistId";
	default: return "undefine";
	}
}

#endif
/*
 * EOF
 */
	
