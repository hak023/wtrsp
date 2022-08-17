/*
   Point-I Copy Right

   File : alarm.h

   1. Declare Alarm 

   Create Date : 2013. Yun Chi Ho (Cho)
*/

#ifndef __INET_UDP_ALARM_H__
#define __INET_UDP_ALARM_H__

//#include "sock.h"
//#include "jthread.h"


#define  ALARM_UUDP_PATH   "../ipc/udp/comm2oma"
#define  ALARM_IUDP_PATH   "127.0.0.1"

//////////////////////////////////////////////////////////////////////////////

#define  CONSOLE_INFO_MAX_LEN 256

#define CON_LOG_ERROR  0
#define CON_LOG_WARN   1
#define CON_LOG_INFO   2

#define OMC_MAGIC_HDR                     0x564D531F

#define MSG_FUNC_ALARM_OMC                0x0500    
#define MSG_FUNC_TRACE_OMC	               0x0800

#define MSG_SUBF_ALARM_NOTIFY_OMC         0x0502
#define MSG_SUBF_TRACE_NOTIFY_OMC         0x0801
#define MSG_SUBF_CONSOLE_OMC              0x080A

namespace OMC
{

typedef struct {
	 unsigned short usZoneId;
	 unsigned short usSysId;
	 unsigned short usParentId;
	 unsigned short usChildId;
	 unsigned int  ulTmpPid;

	 void netorder() {
		  usZoneId = htons(usZoneId);
		  usSysId = htons(usSysId);
		  usParentId = htons(usParentId);
		  usChildId = htons(usChildId);
		  ulTmpPid = htonl(ulTmpPid);
	 }
} OMCADDR, *OMCADDRPTR;

typedef struct {
	 unsigned int   uMagicCookie;
	 int            nMsgLen;
	 OMCADDR        stReceiver;
	 OMCADDR        stSender;
	 short          snHopCnt;
	 unsigned short usTransactionId;
	 short          snSerNo;
	 unsigned short _reserved;
	 unsigned short usType;
	 unsigned short usSubType;

	 void netorder() {
		  uMagicCookie = htonl(uMagicCookie);
		  nMsgLen = htonl(nMsgLen);
		  stReceiver.netorder();
		  stSender.netorder();
		  snHopCnt = htons(snHopCnt);
		  usTransactionId = htons(usTransactionId);
		  snSerNo = htons(snSerNo);
		  _reserved = htons(_reserved);
		  usType = htons(usType);
		  usSubType = htons(usSubType);
	 }
} OMCMSGHDR, *OMCMSGHDRPTR;

typedef struct {
	 unsigned short usAreaId;
	 unsigned short usSystemId;
	 unsigned short usBoardId;
	 unsigned short usTrunkId;
	 unsigned short usChannelId;
	 unsigned short _reserved[3];

	 void netorder() {
		  usAreaId = htons(usAreaId);
		  usSystemId = htons(usSystemId);
		  usBoardId = htons(usBoardId);
		  usTrunkId = htons(usTrunkId);
		  usChannelId = htons(usChannelId);
	 }
} OMCIVRLINE, *OMCIVRLINEPTR;

typedef struct {
	 int             nLevel;	// Event Level
	 char            szCaller[16];	// Don't care
	 char            szCalled[16];	// Don't care
	 char            szLog[CONSOLE_INFO_MAX_LEN]; // Event Message
	 void netorder()
	 {
	 	nLevel = htonl(nLevel);
	 }
} OMCCONSOLELOG, *POMCCONSOLELOG;

typedef struct {
	 OMCMSGHDR       h;
	 OMCIVRLINE     	li;
	 OMCCONSOLELOG      Info;
	 void netorder()
	 {
	 	h.netorder();
	 	li.netorder();
	 	Info.netorder();
	 }
} OMCMSGCONSOLELOG, *POMCMSGCONSOLELOG;


typedef enum {
	 ALARM_CLEARED,
	 ALARM_MINOR,
	 ALARM_MAJOR,
	 ALARM_CRITICAL
} ALARM_LEVEL, *PALARM_LEVEL;

typedef enum {
	 communicationFail,
	 unixSocketError,
	 inetSocketError,
	 databaseError,
	 ipcShmError,
	 ipcMsgqError,
	 ipcSemError,
	 tcpCLosedError,
	 processError,
	 configError,
	 signalError,
	 processKilled,
	 cpuOverflow,
	 diskOverflow,
	 memoryOverflow,
	 swapOverflow,
	 e1boardError,
	 e1portError,
	 ss7SystemError,
	 ss7BoardError,
	 ss7LinkError,
	 nowHacking,
	 DTMFNotify, 
	 statusChange, 
	 cpuUtilChange, 
	 memUtilChange, 
	 diskUtilChange,
	 procShutdown, 
	 versionNotify, 
	 threadKilled, 
	 queueFull, 
	 fileError, 
	 DiskFail,
	 RomsConnectionError,
	 NetworkFail,
	 ChannelStatusError,
	 PowerStatusError,
	 HoldingTimeError,
	 IsupfailOver,
	 SuccessFailError,
	 ChannelOverload = 40,
	 NTPDelayError,
	 NTPOffsetError,
	 NTPStatusError,
	 NAScommunicationFail,
	 Alive,
	 DBResponseDelay = 50,
	 SPSActiveChange = 51,
	 SMSCDSError = 52,
	 boardAPIError = 53,
	 e1portErrorBlock = 54,
	 OverloadDrop = 57, // Add by Cho for 'TMemo 과부하 제어'
	 SipSyntaxError = 58

} ALARM_TYPE, *PALARM_TYPE;

//////////////////////////////////////////////////////////////////////////////
#define MAX_ALARM_SRC   64
#define MAX_ALARM_TEXT  512
#define MAX_ALARM_CAUSE 128

typedef struct {
	 unsigned int    nTime;
	 ALARM_LEVEL     nLevel;
	 ALARM_TYPE      nType;
	 char            szSrc[MAX_ALARM_SRC];
	 char            szText[MAX_ALARM_TEXT];
	 char            szCause[MAX_ALARM_CAUSE];
	 void netorder()
	 {
	 	nTime = htonl(nTime);
		if(sizeof(ALARM_LEVEL) == sizeof(short))
		{
			nLevel = (ALARM_LEVEL)htons((short)nLevel);
		} else {
			nLevel = (ALARM_LEVEL)htonl((int)nLevel);
		}
		if(sizeof(ALARM_TYPE) == sizeof(short))
		{
			nType = (ALARM_TYPE)htons((short)nType);
		} else {
			nType = (ALARM_TYPE)htonl((int)nType);
		}
	 }
} OMCALARMMSG, *POMCALARMMSG;

typedef struct {
	 OMCMSGHDR		 h;	
	 OMCIVRLINE		 li;	
	 OMCALARMMSG     alarm;
	 void netorder()
	 {
	 	h.netorder();
	 	li.netorder();
	 	alarm.netorder();
	 }
} OMCOMCCOMMALARM, *POMCOMCCOMMALARM;

#define OMC_TRACE_INFO_MAX_LEN 2048

typedef struct {
	 char	szCaller[32];
	 char	szCalled[32];
	 char	szTraceLog[OMC_TRACE_INFO_MAX_LEN];
	 void netorder()
	 {
	 	return;
	 }
} TRACE_LOG, *PTRACE_LOG;

typedef struct {
	 OMCMSGHDR		 h;	
	 OMCIVRLINE		 li;	
	 TRACE_LOG		stTraceLog;
	 void netorder()
	 {
	 	h.netorder();
	 	li.netorder();
	 	stTraceLog.netorder();
	 }
} OMCTRACEINFO, *POMCTRACEINFO;

#if 0
//////////////////////////////////////////////////////////////////////////////
class CAlarmUdp_UUDP : public CUnixUdp
{
public:
   CAlarmUdp_UUDP();
   ~CAlarmUdp_UUDP();
   
   bool Init(char *pszSrc, 
             char *pszDest);

	void SendToUUDP(char* pMsg, int nSize);

private:      
   char *m_pszAlarmPath;
}; 

//////////////////////////////////////////////////////////////////////////////
class CAlarmUdp_IUDP : public CInetUdpSocket
{
public:
	CAlarmUdp_IUDP();
	~CAlarmUdp_IUDP();

   bool Init( int nDestPort, char *pszDestIP
				 ,int nLocalPort, char *pszLocalIP);

	void SendToIUDP(char* pMsg, int nSize); 

private:      
   char m_pszDestIP[24]; 
	int m_nDestPort;
};

//////////////////////////////////////////////////////////////////////////////
class CAlarmUdp
{
public:
   CAlarmUdp();
   ~CAlarmUdp();

   bool Init(char *pszAlarmSrc,
             char *pszSrc,
             char *pszDest = ALARM_UUDP_PATH);
   bool Init(char *pszAlarmSrc
				 ,int nDestPort, char *pszDestIP
				 ,int nLocalPort, char *pszLocalIP="0.0.0.0");
   void Trace(const char *pszCaller, const char *pszCalled, const char *pszText);
   void Alarm(ALARM_LEVEL nLevel, ALARM_TYPE nType,
              char *pszText, char *pszCause,
              char *pszSrc=NULL);
   void AlarmSub(ALARM_LEVEL nLevel, ALARM_TYPE nType,
              char *pszText, char *pszCause,
              char *pszSrc);

   void ConsoleMsg(const int nLevel, const char *pszText);

	void Close();

private:
	CAlarmUdp_UUDP* m_pUUDP;
	CAlarmUdp_IUDP* m_pIUDP;
   OMCOMCCOMMALARM *m_pMsg;
   char *m_pszAlarmSrc;

	bool  m_bIsUUDP;

   static CCritSec m_AlarmLock;
};

//////////////////////////////////////////////////////////////////////////////
void   SetAlarmSocket(CAlarmUdp* pAlarmSock);

void   SendTraceToOMC(const char *pszCaller, const char *pszCalled, const char *pszText);
void   SendAlarmToOMC(ALARM_LEVEL nLevel, ALARM_TYPE nType,
                      char *pszText, char *pszCause,
                      char *pszSrc=NULL);
void   SendAlarmSubToOMC(ALARM_LEVEL nLevel, ALARM_TYPE nType,
                      char *pszText, char *pszCause,
                      char *pszSrc);

void	SendConsoleMsgToOMC(const int nLevel, const char * pszText);
#endif

}
#endif //__ALARM_H__
