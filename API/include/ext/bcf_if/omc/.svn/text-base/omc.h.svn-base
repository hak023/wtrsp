#ifndef __OMC_H__
#define __OMC_H__

#include "shm.h"
#include "eventdefs.h"
#include "alarm.h"
#include "ss7control.h"

typedef enum {
        SYS_TYPE_OMP=0,
        SYS_TYPE_GUI,
        SYS_TYPE_CM,
        SYS_TYPE_ATMP,
        SYS_TYPE_MP,
        SYS_TYPE_H324M,        
        SYS_TYPE_UNKNOWN
} SYS_TYPE;

typedef  enum {  
	CH_NOTUSE = 0,  
	CH_IDLE,
	CH_WAIT,  
	CH_NEGO,  
	CH_RELAY,  
	CH_PLAY,  
	CH_MA
} CHANNELSTATUS;

#define XTM_SYSID_OMP     0x0100
#define XTM_SYSID_GUI     0x0200
#define XTM_SYSID_CM      0x0300
#define XTM_SYSID_CM_A    0x0300
#define XTM_SYSID_CM_B    0x0301
#define XTM_SYSID_ATMP    0x0400
#define XTM_SYSID_MP      0x0500
#define XTM_SYSID_H324M   0x0600

// XTMADDR.usParentId, XTMMSGHDR 에는 다음 값을 세팅.
#define XTM_PARENTID_OMA       1
#define XTM_PARENTID_PNR       2
#define XTM_PARENTID_FMS       3
#define XTM_PARENTID_PMS       4
#define XTM_PARENTID_CMS       5
#define XTM_PARENTID_OMG       6
#define XTM_PARENTID_STMS      7
#define XTM_PARENTID_TMS       8
#define XTM_PARENTID_OMS       9
#define XTM_PARENTID_BMS       10
#define XTM_PARENTID_GUI       20

#define UDP_BASE               "../ipc/udp"
#define UDP_OMA_PATH           UDP_BASE "/oma"
#define UDP_FMS_PATH           UDP_BASE "/fms"
#define UDP_PMS_PATH           UDP_BASE "/pms"
#define UDP_CMS_PATH           UDP_BASE "/cms"
#define UDP_OMG_PATH           UDP_BASE "/omg"
#define UDP_STMS_PATH          UDP_BASE "/stms"
#define UDP_TMS_PATH           UDP_BASE "/tms"
#define UDP_OMS_PATH           UDP_BASE "/oms"
#define UDP_BMS_PATH           UDP_BASE "/bms"

#define ALARM_LEVEL_DEFAULT    4

#define GUI_MESSAGE_LEN        256

#define  DIRECTORY_LIST                      0
#define  FILE_CONTENT                        1

#define MAX_CONN_ERR           10 
#define MAX_CONFIG_ITEM        16
#define MAX_CP_SERVER          32
#define MAX_OP_ID              12
#define SYS_OP_ID              "system"

#define MAX_PINGCHECK 10

#define MSG_FUNC_ADMIN                    0x0100
#define MSG_FUNC_PERFORMANCE              0x0200
#define MSG_FUNC_CONFIG                   0x0300
#define MSG_FUNC_MONITOR                  0x0400
#define MSG_FUNC_STATISTICS               0x0600
#define MSG_FUNC_CTRL                     0x0700
#define MSG_FUNC_TRACE                    0x0800
#define MSG_FUNC_SERVICE                  0x0900
#define MSG_FUNC_ALCAP                    0x0A00
#define MSG_FUNC_MMI                      0x0F00

// MSG_FUNC_ADMIN
#define MSG_SUBF_LOGIN_NOTIFY             0x0101    // OMCMSGLOGINNOTIFY
#define MSG_SUBF_RELOGIN_NOTIFY           0x0102    // OMCMSGLOGINNOTIFY
#define MSG_SUBF_SEND_ADMIN_NOTE          0x0103    // OMCMSGADMINNOTE
#define MSG_SUBF_ADMIN_MESSAGE            0x0104    // OMCMSGADMINMSG
#define MSG_SUBF_KILL_GUI                 0x0105    // OMCMSGKILLGUI

// MSG_FUNC_PERFORMANCE : OMA -> OMS
#define MSG_SUBF_PERFORM_SYS_NOTIFY       0x0201    // OMCMSGSYSPF
#define MSG_SUBF_PERFORM_TRAFFIC_NOTIFY   0x0203    // OMCMSGTRAFFICPF

// MSG_FUNC_CONFIG
#define MSG_SUBF_CONFIG_GET_REQ				0x0301
#define MSG_SUBF_CONFIG_GET_RSP				0x0302
#define MSG_SUBF_CONFIG_SET_REQ				0x0303
#define MSG_SUBF_CONFIG_SET_RSP				0x0304
#define MSG_SUBF_CONFIG_LIST_REQ	    		0x0305
#define MSG_SUBF_CONFIG_LIST_RSP	   		0x0306
#define MSG_SUBF_CONFIG_UPDATE				0x0307
#define MSG_SUBF_CONFIG_FILELIST_REQ		0x0308
#define MSG_SUBF_CONFIG_FILELIST_RSP		0x0309
#define MSG_SUBF_CONFIG_NTP_GET_REQ			0x0322
#define MSG_SUBF_CONFIG_NTP_GET_RSP			0x0323
#define MSG_SUBF_CONFIG_NTP_SET_REQ			0x0324
#define MSG_SUBF_CONFIG_NTP_SET_RSP			0x0325
#define MSG_SUBF_CONFIG_BUSY_GET_REQ		0x0326
#define MSG_SUBF_CONFIG_BUSY_GET_RSP		0x0327
#define MSG_SUBF_CONFIG_BUSY_SET_REQ		0x0328
#define MSG_SUBF_CONFIG_BUSY_SET_RSP		0x0329
#define MSG_SUBF_CONFIG_MINCALL_GET_REQ	0x0341
#define MSG_SUBF_CONFIG_MINCALL_GET_RSP	0x0342
#define MSG_SUBF_CONFIG_MINCALL_SET_REQ	0x0343
#define MSG_SUBF_CONFIG_MINCALL_SET_RSP	0x0344
#define MSG_SUBF_CONFIG_PLAYSUCC_GET_REQ	0x0351
#define MSG_SUBF_CONFIG_PLAYSUCC_GET_RSP	0x0352
#define MSG_SUBF_CONFIG_PLAYSUCC_SET_REQ	0x0353
#define MSG_SUBF_CONFIG_PLAYSUCC_SET_RSP	0x0354
#define MSG_SUBF_CONFIG_THRESHOLD_GET_REQ	0x0361
#define MSG_SUBF_CONFIG_THRESHOLD_GET_RSP	0x0362
#define MSG_SUBF_CONFIG_THRESHOLD_SET_REQ	0x0363
#define MSG_SUBF_CONFIG_THRESHOLD_SET_RSP	0x0364

// MSG_FUNC_MONITOR
#define MSG_SUBF_MONITOR_CHAN_START                0x0401    // OMCMSGREQUEST
#define MSG_SUBF_MONITOR_CHAN_STOP                 0x0402    // OMCMSGREQUEST
#define MSG_SUBF_MONITOR_CHAN_INFO                 0x0403    // OMCMSGMONITORCHN
#define MSG_SUBF_MONITOR_SYS_START                 0x0404    // OMCMSGREQUEST
#define MSG_SUBF_MONITOR_SYS_STOP                  0x0405    // OMCMSGREQUEST
#define MSG_SUBF_MONITOR_SYS_INFO                  0x0406    // OMCMSGMONITORSYS
#define MSG_SUBF_MONITOR_PROCESS_LIST_START        0x0407
#define MSG_SUBF_MONITOR_PROCESS_LIST_STOP         0x0408
#define MSG_SUBF_MONITOR_PROCESS_LIST_INFO         0x0409
#define MSG_SUBF_MONITOR_CONN_START         	      0x0410
#define MSG_SUBF_MONITOR_CONN_STOP                 0x0411
#define MSG_SUBF_MONITOR_CONN_INFO                 0x0412
#define MSG_SUBF_MONITOR_NTP_STATUS_REQ            0x0413
#define MSG_SUBF_MONITOR_NTP_STATUS_RSP            0x0414
#define MSG_SUBF_MONITOR_HAPROCESS_LIST_START      0x0415
#define MSG_SUBF_MONITOR_HAPROCESS_LIST_STOP       0x0416
#define MSG_SUBF_MONITOR_HAPROCESS_LIST_INFO       0x0417
#define MSG_SUBF_MONITOR_EXT_SERVER_START          0x0424
#define MSG_SUBF_MONITOR_EXT_SERVER_STOP           0x0425
#define MSG_SUBF_MONITOR_EXT_SERVER_INFO           0x0426
#define MSG_SUBF_MONITOR_IS_ALIVE                  0x0440
#define MSG_SUBF_MONITOR_CONN_OMS                  0x0441    // OMCMSGREQUEST : OMS -> OMA 연결알림.
#define MSG_SUBF_MONITOR_CONN_OMA                  0x0442    // OMCMSGREQUEST : OMS -> OMA 연결알림.
#define MSG_SUBF_MONITOR_CONN_GUI                  0x0443    // OMCMSGREQUEST : OMS -> OMA 연결알림.
#define MSG_SUBF_MONITOR_RESET                     0x0444    
#define MSG_SUBF_MONITOR_HA_PROC                   0x0450    // OMCMSGREQUEST
#define MSG_SUBF_MONITOR_HA_STATUS                 0x0451    // OMCMSGREQUEST

// MSG_FUNC_ALARM
#define MSG_SUBF_ALARM_ACK                0x0503    // OMCMSGGUIALARM
#define MSG_SUBF_ALARM_CLOSE              0x0504    // OMCMSGGUIALARM
#define MSG_SUBF_ALARM_REPORT             0x0506    // OMCMSGGUIALARM
#define MSG_SUBF_ALARM_REQUEST            0x0507    // OMCMSGREQUEST
#define MSG_SUBF_ALARM_TEST               0x0508    // OMCMSGREQUEST

/* For MSG_FUNC_STATISTICS */
#define MSG_SUBF_STS_SYS_NOTIFY				0x0601
#define MSG_SUBF_STS_CDR_NOTIFY				0x0602
#define MSG_SUBF_STS_MEDIA_NOTIFY			0x0604
#define MSG_SUBF_STS_CRC_NOTIFY				0x0605
#define MSG_SUBF_STS_ATM_NOTIFY				0x0606
#define MSG_SUBF_STS_H245_NOTIFY			   0x0607

// MSG_FUNC_CTRL
#define MSG_SUBF_CTRL_DBBACKUP_REQ        0x0701
#define MSG_SUBF_CTRL_DBBACKUP_RSP        0x0711
#define MSG_SUBF_CTRL_PKGBACKUP_REQ       0x0702
#define MSG_SUBF_CTRL_PKGBACKUP_RSP       0x0712
#define MSG_SUBF_CTRL_DBRESTORE_REQ       0x0721
#define MSG_SUBF_CTRL_DBRESTORE_RSP       0x0722
#define MSG_SUBF_CTRL_PROCESS_RESTART     0x0703
#define MSG_SUBF_CTRL_PROCESS_START       0x0704
#define MSG_SUBF_CTRL_PROCESS_KILL        0x0705
#define MSG_SUBF_CTRL_PROCESS_START_ALL   0x0706
#define MSG_SUBF_CTRL_PROCESS_KILL_ALL    0x0707
#define MSG_SUBF_CTRL_SYSTEM_RESTART_REQ  0x0708
#define MSG_SUBF_CTRL_SYSTEM_RESTART_RSP  0x0709
#define MSG_SUBF_CTRL_PROCESS_MASK        0x0735
#define MSG_SUBF_CTRL_PROCESS_UNMASK      0x0736
#define MSG_SUBF_CTRL_HA_PROC             0x0740
#define MSG_SUBF_CTRL_HA_STATUS           0x0741
#define MSG_SUBF_CTRL_HA_DEACT            0x0742
#define MSG_SUBF_CTRL_HA_MACTIVE          0x0743
#define MSG_SUBF_CTRL_HA_MSTANDBY         0x0744
#define MSG_SUBF_CTRL_HA_MOFF             0x0745

// MSG_FUNC_TRACE
#define MSG_SUBF_TRACE_NOTIFY             0x0801
#define MSG_SUBF_TRACE_SET_INFO_REQ       0x0802
#define MSG_SUBF_TRACE_SET_INFO_RSP       0x0803
#define MSG_SUBF_TRACE_GET_INFO_REQ       0x0804
#define MSG_SUBF_TRACE_GET_INFO_RSP       0x0805
#define MSG_SUBF_TRACE_INFO_UPDATE        0x0806
#define MSG_SUBF_CONSOLE                  0x080A
#define MSG_SUBF_MMI                      0x080B
#define MSG_SUBF_MMI_TRACE                0x080C

////////////////////////////////////////////////////////////////
//OAM CONFIG
////////////////////////////////////////////////////////////////
typedef struct {
	int		nRackId;
	int		nUseRack;
	char	szRackName[64];
} OAMCONFIG_RACK , *POAMCONFIG_RACK;

typedef struct {
	int		nRackId;
	int		nChassisId;
	int		nUseChassis;	
	char		szChassisName[64];
} OAMCONFIG_CHASSIS , *POAMCONFIG_CHASSIS;

typedef struct {
	int		nRackId;
	int		nChassisId;
	int		nSystemId;	
	int		nUseSystem;	
	char	szSystemName[64];
	int		nUseBoard[MAX_BOARDS];
	int		nTypeBoard[MAX_BOARDS];	
	char	szBoardName[MAX_BOARDS][64];		
} OAMCONFIG_SYSTEM , *POAMCONFIG_SYSTEM;  
  
///////////////////////////////////////////////////////
// Common
///////////////////////////////////////////////////////

enum RESULT {
   OMC_FAIL    = 0,
   OMC_SUCCESS = 1
};

enum REASON {
   canNotAccessFile,
};

typedef struct {
   XTMMSGHDR  h;
} OMCMSGREQUEST, *POMCMSGREQUEST;

typedef struct {
   unsigned short usResult;  // 0:Fail, 1:Success;
   unsigned short usReason;
   char szDetail[256];
} OMCRESPONSEINFO, *POMCRESPONSEINFO;

typedef struct {
   XTMMSGHDR       h;
   OMCRESPONSEINFO Response;
} OMCMSGRESPONSE, *POMCMSGRESPONSE;

typedef struct {
   unsigned short Result;
   unsigned short Reason;
} OMCRESULTINFO, *POMCRESULTINFO;

typedef struct {
   XTMMSGHDR     h;
   OMCRESULTINFO mb;
} OMCMSGRSP , *POMCMSGRSP;

 
////////////////////////////////////////////////////////////////
// MSG_FUNC_ADMIN
////////////////////////////////////////////////////////////////

// MSG_SUBF_LOGIN_NOTOFY     
typedef struct {
   char szOeratorId[32];
   char szMachine[64];     // Computer Name
   char szMacAddress[32];  // Mac
   int  nLoginHistId;      // LOGIN_HIST.LoginHistId
} OMCLOGININFO, *POMCOMCLOGININFO;

typedef struct {
    XTMMSGHDR    h;
    OMCLOGININFO Info;
} OMCMSGLOGINNOTIFY, *POMCMSGLOGINNOTIFY;

// MSG_SUBF_SEND_ADMIN_NOTE
typedef struct {
    int  nOpSessionId;  // if nOpSessionId = 0, to all session.
    char szOeratorId[32];
    char szMessage[GUI_MESSAGE_LEN];
} OMCADMINNOTEINFO, *POMCADMINNOTEINFO;

typedef struct {
    XTMMSGHDR        h;
    OMCADMINNOTEINFO Info;
} OMCMSGADMINNOTE, *POMCMSGADMINNOTE;

// MSG_SUBF_ADMIN_MESSAGE
typedef struct {
    char szMessage[GUI_MESSAGE_LEN];
} OMCADMINMSGINFO, *POMCADMINMSGINFO;

typedef struct {
    XTMMSGHDR       h;
    OMCADMINMSGINFO Info;
} OMCMSGADMINMSG, *POMCMSGADMINMSG;

// MSG_SUBF_KILL_GUI
typedef struct {
    int  nOpSessionId;
    char szMessage[GUI_MESSAGE_LEN];
} OMCKILLGUIINFO, *POMCKILLGUIINFO;

typedef struct {
    XTMMSGHDR      h;
    OMCKILLGUIINFO Info;
} OMCMSGKILLGUI, *POMCMSGKILLGUI;

///////////////////////////////////////////////////////
// MSG_FUNC_ALARM
///////////////////////////////////////////////////////

// MSG_SUBF_ALARM_NOTIFY 

// MSG_SUBF_ALARM_REPORT

typedef struct {
   unsigned int nAlarmId;
   unsigned int nTime;
   ALARM_LEVEL   nLevel;
   unsigned int  nType;
   char          szCode[MAX_ALARM_CODE];
   char          szSrc[MAX_ALARM_SRC];
   char          szText[MAX_ALARM_TEXT];
   char          szCause[MAX_ALARM_CAUSE];
   int           nAck;
   unsigned int nAckTime;
   char          szAckBy[MAX_OP_ID];
   int           nClose;
   unsigned int nCloseTime;
   char          szCloseBy[MAX_OP_ID];
} OMCGUIALARMINFO, *POMCGUIALARMINFO;

typedef struct {
   XTMMSGHDR       h;   
   OMCGUIALARMINFO Alarm;
} OMCMSGGUIALARM, *POMCMSGGUIALARM;

//////////////////////////////////////////////////////////////////////
// MSG_FUNC_STATISTICS
//////////////////////////////////////////////////////////////////////

#define MAX_CALLTYPE	2
#define MAX_TRANSTYPE	4

// MSG_SUBF_STS_CDR_NOTIFY
typedef struct {
    int  nRequest;
    int  nSuccess;
    int  nFail;
    int  nTCType[MAX_TRANSTYPE];
    int  nErrorCode[30];
} OMCCDRINFO;

typedef struct {
    char            Time[32];
    OMCCDRINFO  CDR[MAX_CALLTYPE];
} OMCCDRSTSINFO;

typedef struct {
    XTMMSGHDR           h;
    VMSIVRLINE          li;
    OMCCDRSTSINFO   CDRSts;
} OMCMSGCDRSTS, *POMCMSGCDRSTS;


// MSG_SUBF_STS_MEDIA_NOTIFY
#define MAX_MEDIA_STATUS	10

typedef struct {
     int      nPacketCount_Audio_RX;
     int      nPacketCount_Audio_TX;
     int      nPacketCount_Video_RX;
     int      nPacketCount_Video_TX;
     int      nLen_Audio_RX;
     int      nLen_Audio_TX;
     int      nLen_Video_RX;
     int      nLen_Video_TX;
} OMCMEDIAINFO;

typedef struct {
     int            nMEDIACount;
     char         Time[32];
     OMCMEDIAINFO  MEDIA[MAX_MEDIA_STATUS];
} OMCMEDIASTS;

typedef struct {
     XTMMSGHDR    h;
     VMSIVRLINE  li;
     OMCMEDIASTS    MEDIASts;
} OMCMSGMEDIASTS, *POMCMSGMEDIASTS;


// MSG_SUBF_STS_CRC_NOTIFY
#define MAX_CRC	8
#define MAX_H245_BOARDS	8

typedef struct {
     int      nAudioCRC[MAX_CRC];
     int      nVideoCRC[MAX_CRC];
} OMCCRCINFO;

typedef struct {
     char         Time[32];
     OMCCRCINFO  CRC[MAX_H245_BOARDS];
} OMCCRCSTS;

typedef struct {
     XTMMSGHDR    h;
     VMSIVRLINE  li;
     OMCCRCSTS    CRCSts;
} OMCMSGCRCSTS, *POMCMSGCRCSTS;


// MSG_SUBF_STS_H245_NOTIFY
#define MAX_H245	8
typedef struct {
     int      nRx_MSD;
     int      nTx_MSD;
     int      nRx_MSDAck;
     int      nTx_MSDAck;

     int      nRx_TCS;
     int      nTx_TCS;
     int      nRx_TCSAck;
     int      nTx_TCSAck;

     int      nRx_MES;
     int      nTx_MES;
     int      nRx_MESAck;
     int      nTx_MESAck;

     int      nRx_OLC;
     int      nTx_OLC;
     int      nRx_OLCAck;
     int      nTx_OLCAck;
} OMCH245INFO;

typedef struct {
     char         Time[32];
     OMCH245INFO  H245[MAX_H245_BOARDS];
} OMCH245STS;

typedef struct {
     XTMMSGHDR    h;
     VMSIVRLINE  li;
     OMCH245STS    H245Sts;
} OMCMSGH245STS, *POMCMSGH245STS;

// MSG_SUBF_STS_ATM_NOTIFY
typedef struct {
    int  nRequest;
    int  nSuccess;
    int  nFail;
    int  nTCType[MAX_TRANSTYPE];
    int  nErrorCode[30];
} OMCATMINFO;

typedef struct {
    char            Time[32];
    OMCATMINFO  ATM[MAX_CALLTYPE];
} OMCATMSTSINFO;

typedef struct {
    XTMMSGHDR           h;
    VMSIVRLINE          li;
    OMCATMSTSINFO   ATMSts;
} OMCMSGATMSTS, *POMCMSGATMSTS;

//////////////////////////////////////////////////////////////////////
// MSG_FUNC_CTRL
//////////////////////////////////////////////////////////////////////
typedef struct {
   char       szProcName[MAX_PROCESS_NAME_LEN];
} OMCPROCCONTROL, *POMCPROCCONTROL;

typedef struct {
   XTMMSGHDR       h;
   OMCPROCCONTROL  Proc;
} OMCMSGPROCCONTROL, *POMCMSGPROCCONTROL;

typedef struct {
   char Date[32];
} OMCDATEINFO, *POMCDATEINFO;

//////////////////////////////////////////////////////////////////
// OMC_MSG_TRACE
//////////////////////////////////////////////////////////////////
//#define TRACE_INFO_MAX_LEN 256

typedef struct {
   unsigned int     nStatus; //value : 0(OFF), 1(ON)
   unsigned int     nWho;        //(TRACE_MIN_CALLER:1, TRACE_MIN_CALLED:2)
   char             cMinNo[TRACE_MAX_MIN_LEN];
   unsigned int     nMinNo;
   unsigned int     nCount;
   unsigned int     nStartTime;  //long int : trace start time
   unsigned int     nEndTime;    //           trace end time
   unsigned int     nLevel;
   unsigned int     nUserLevel;
   char             cUserID[32];
} OMCTRACE_INFO, *POMCTRACE_INFO; 

typedef struct {
   int    nOpType;
   OMCTRACE_INFO stInfo;
} OMCTRACE_SET_REQ, *POMCTRACE_SET_REQ; //size 

typedef struct {
   int    nResult;
   int    nTraceId; //Debug Level(0:ALL, 1:Info, 2:Err)
   int    nOpType;
   OMCTRACE_INFO stInfo[TRACE_MAX_MIN];
} OMCTRACE_RSP, *POMCTRACE_RSP; //size 

typedef struct {
  char            szCaller[32];
  char            szCalled[32];
  char            szLog[TRACE_INFO_MAX_LEN];
} OMCTRACE_LOG, *POMCTRACE_LOG;

typedef struct {
   XTMMSGHDR   h;
   VMSIVRLINE  li;
   OMCTRACE_SET_REQ   TraceInfo;
} OMCMSGTRACESET, *POMCMSGTRACESET;

typedef struct {
   XTMMSGHDR   h;
   VMSIVRLINE  li;
   OMCTRACE_RSP   TraceInfo;
} OMCMSGTRACEINFO, *POMCMSGTRACEINFO;

typedef struct {
   XTMMSGHDR   h;
   VMSIVRLINE  li;
   OMCTRACE_LOG   TraceLog;
} OMCMSGTRACELOG, *POMCMSGTRACELOG;

//////////////////////////////////////////////////////////////////////
// MSG_FUNC_MONITOR
//////////////////////////////////////////////////////////////////////
typedef struct {
   int          nIndex; // nIndex = 0,1,2,..., nIndex < 0 : invalid.
   OMCUSAGEINFO Usage;  // in KByte.
} OMCCPUINFO, *POMCCPUINFO;

typedef struct {
   /* Temperature in Voiceboard */  
   unsigned int   unZone1;    //Zone1 (Tsi) Temp  
   unsigned int   unZone2;    //Zone2 (Cpu) Temp  : display value (OMC)
   unsigned int   unZone3;    //Zone3 (H110) Temp 
   unsigned int   unZone4;    //Zone4 (Dsp) Temp
} VBTEMPINFO, *PVBTEMPINFO;

// MSG_SUBF_MONITOR_SYS_START
// @ use OMCMSGREQUEST

// MSG_SUBF_MONITOR_SYS_STOP
// @ use OMCMSGREQUEST

// MSG_SUBF_MONITOR_SYS_INFO
/*
typedef struct {
   OMCCPUINFO       CpuUsage[MAX_CPU];
   OMCUSAGEINFO     MemUsage; // KByte
   OMCPARTITIONINFO Partition[MAX_PARTITION]; // KByte
   OMCNETIFINFO     Net[MAX_NETIF];
} OMCMONITORSYSINFO, *POMCMONITORSYSINFO;
*/

typedef struct {
   int              CpuUsage;
   int              MemUsage; // KByte
   int              CpsUsage; // KByte
   OMCPARTITIONINFO Partition[MAX_PARTITION]; // KByte
   OMCNETIFINFO     Net[MAX_NETIF];
} OMCMONITORSYSINFO, *POMCMONITORSYSINFO;

typedef struct {
   XTMMSGHDR         h;
   OMCMONITORSYSINFO Sys;
} OMCMSGMONITORSYS, *POMCMSGMONITORSYS;

typedef struct {
	int nExtServerCount;
	EXT_SERVER_STATUS ExtServerStatus[MAX_EXT_SERVERS];
} OMCEXTSERVER, *POMCEXTSERVER;

typedef struct {
   XTMMSGHDR   h;
   OMCEXTSERVER  ExtServer;
} OMCMSGEXTSERVER, *POMCMSGEXTSERVER;

// MSG_SUBF_GET_PROCESS_LIST_REQ
// @ use OMCMSGREQUEST

// MSG_SUBF_GET_PROCESS_LIST_RSP
typedef struct {
   int   size;                               // sizeof(PROCESS_STATUS)
   int   valid;                              // valid flag(0: invalid)
   pid_t pid;                                // process ID
   pid_t ppid;                               // parent process ID
   char  szName[MAX_PROCESS_NAME_LEN];       // process name
   char  szAlias[MAX_PROCESS_NAME_LEN];      // process name(alias)
} OMCPROCESSINFO, *POMCMSGPROCESSINFO;

typedef struct {
   XTMMSGHDR      h;
   OMCPROCESSINFO procs[MAX_PROCESSES];
} OMCMSGPROCESS, *POMCMSGPROCESS;

typedef struct {
   char szValue[2048];
} OMCHAMONITOR, *POMCHAMONITOR;

typedef struct {
   XTMMSGHDR      h;
   OMCHAMONITOR Info;
} OMCMSGHAMONITOR, *POMCMSGHAMONITOR;

// MSG_SUBF_GET_PROCESS_LIST_RSP
typedef struct {
   int   size;                               // sizeof(PROCESS_STATUS)
   int   valid;                              // valid flag(0: invalid)
   pid_t pid;                                // process ID
   pid_t ppid;                               // parent process ID
   char  szName[MAX_HA_PROCESS_NAME_LEN];       // process name
   char  szAlias[MAX_HA_PROCESS_NAME_LEN];      // process name(alias)
} OMCHAPROCESSINFO, *POMCMSGHAPROCESSINFO;

typedef struct {
   XTMMSGHDR      h;
   OMCHAPROCESSINFO procs[MAX_HA_PROCESSES];
} OMCMSGHAPROCESS, *POMCMSGHAPROCESS;

////////////////////////////////////////////////////////////////////
// MSG_FUNC_CONFIG
////////////////////////////////////////////////////////////////////

typedef struct {
   char     szKey[32];
   char     szValue[128];
} OMCCONFITEMINFO, *POMCCONFITEMINFO;

typedef struct {
   char     szFilePath[128];
   char     szSection[32];
   OMCCONFITEMINFO Item[MAX_CONFIG_ITEM];
} OMCCONFIGINFO, *POMCCONFIGINFO;

typedef struct {
   XTMMSGHDR      h;
   OMCCONFIGINFO  Config;
} OMCMSGCONFIG, *POMCMSGCONFIG;

// MSG_SUBF_GET_FILE_REQ   
typedef struct {
   char  szFilePath[128];
} OMCGETFILEREQINFO,OMCTRACEFILEINFO, *POMCGETFILEREQINFO, *POMCTRACEFILEINFO;

typedef struct {
   XTMMSGHDR    h;
   OMCGETFILEREQINFO Info;
} OMCMSGGETFILEREQ,OMCMSGTRACEFILEINFO, *POMCMSGTRACEFILEINFO, *POMCMSGGETFILEREQ;

// MSG_SUBF_CONFIG_FILELIST_REQ
typedef struct {
   int  nType;
   char szDirPath[256];
   char szFileName[256];
} OMCFILELISTREQ, *POMCFILELISTREQ;

typedef struct {
   XTMMSGHDR    h;
   OMCFILELISTREQ  FileListReq;
} OMCMSGFILELISTREQ, *POMCMSGFILELISTREQ;

// MSG_SUBF_CONFIG_FILELIST_RSP
typedef struct {
   char szDirPath[256];
   char szFileList[3200];
} OMCFILELISTINFO, *POMCFILELISTINFO;

typedef struct {
   XTMMSGHDR    h;
   OMCFILELISTINFO  FileInfo;
} OMCMSGFILELISTINFO, *POMCMSGFILELISTINFO;

// MSG_SUBF_DO_DBBACKUP_REQ
typedef struct {
   char  szReason[128];
} OMCDODBBAKREQINFO, *POMCDODBBAKREQINFO;

typedef struct {
   XTMMSGHDR         h;
   OMCDODBBAKREQINFO Info;
} OMCMSGDODBBAKREQ, *POMCMSGDODBBAKREQ;

// MSG_SUBF_SET_TPS_THRESHOLD_REQ
typedef struct {
   int nEnable;          // 0: Disable, 1: Enable
   int nThresholdValue;  // Value of Overloaded TPS Threshold
} OMCTHRESHOLDINFO, *POMCTHRESHOLDINFO;

typedef struct {
   XTMMSGHDR           h;
   OMCTHRESHOLDINFO Info;
} OMCMSGTHRESHOLDREQ, *POMCMSGTHRESHOLDREQ;

typedef struct {
   int   Cpu;
   int   Mem;
   int   Disk;
} OMCCRITICALINFO, *POMCCRITICALINFO;

typedef struct {
   XTMMSGHDR         h;
   OMCCRITICALINFO   Minor;
   OMCCRITICALINFO   Major;
   OMCCRITICALINFO   Critical;
} OMCMSGCRITICAL, *POMCMSGCRITICAL;

typedef struct {
   XTMMSGHDR         h;
   OMCCRITICALINFO   Minor;
   OMCCRITICALINFO   Major;
   OMCCRITICALINFO   Critical;
   OMCRESULTINFO      Result;
} OMCMSGCRITICALRESULT, *POMCMSGCRITICALRESULT;

typedef struct {
   int            type;          // 0 : Directory / 1 : File
   char           DirPath[256];
   char           FileName[256];
} CONFIGREQUEST, *PCONFIGREQUEST;

typedef struct {
   char           DirPath[256];
   char           FileList[3600]; // delimeter = ^
} CONFIGFILELIST, *PCONFIGFILELIST;


//////////////////////////////////////////////////////////////////////
// MSG_FUNC_PERFORM
//////////////////////////////////////////////////////////////////////

// MSG_SUBF_PERFORM_SYS_NOTIFY
typedef struct {
   char             szTime[32];
   int              CpuUsage;
   int              CpsUsage;
   OMCUSAGEINFO     MemUsage; // KByte
   OMCPARTITIONINFO Partition[MAX_PARTITION]; // KByte
} OMCSYSPFINFO, *POMCSYSPFINFO;

typedef struct {
   XTMMSGHDR      h;
   OMCSYSPFINFO   SysPerform;
} OMCMSGSYSPF, *POMCMSGSYSPF;

// MSG_SUBF_PERFORM_TRAFFIC_NOTIFY
typedef struct {
   char         szTime[32];
   OMCNETIFINFO NetPerform[MAX_NETIF];
} OMCTRAFFICPFINFO, *PTRAFFICPFINFO;

typedef struct {
   XTMMSGHDR        h;
   OMCTRAFFICPFINFO TrafficPerform;
} OMCMSGTRAFFICPF, *POMCMSGTRAFFICPF;

//////////////////////////////////////////////////////////////////////
// MSG_FUNC_SERVICE 
//////////////////////////////////////////////////////////////////////
typedef struct
{
   int  nSrc;
   int  nService;
   char szCaller[32];
   char szCalled[32];
   char szFilePath[128];
   char szFromTime[32];
   char szToTime[32];   
} OMCCDRREQ, *POMCCDRREQ;

typedef struct
{
   XTMMSGHDR      h;
   VMSIVRLINE     li;
   OMCCDRREQ     Info;
} OMCMSGCDRREQ, *POMCMSGCDRREQ;

typedef struct
{
   int  nService;
   int  nCount;
   int  nResult;
   int  nReason;
   char szLog[1024];
} OMCCDRRSP, *POMCCDRRSP;

typedef struct
{
   XTMMSGHDR      h;
   VMSIVRLINE     li;
   OMCCDRRSP   Info;
} OMCMSGCDRRSP, *POMCMSGCDRRSP;

typedef struct
{
   int  nCmdID;
   int  nSessionID;
   int  nHistID;
   int  nClassID;
   int  nMMIClass;  // 0 : MP MMI, 1 : GUI MMI
   char  szAdminID[32];
   char  szIpAddr[32];
   char  szCmd[256];
} OMCMMICMD, *POMCMMICMD;

typedef struct
{
   XTMMSGHDR      h;
   VMSIVRLINE     li;
   OMCMMICMD      Cmd;
} OMCMSGMMICMD, *POMCMSGMMICMD;

typedef struct
{
   int  nCmdID;
   int  nSessionID;
   int  nHistID;
   int  nResult;
   char szReason[256];
} OMCMMICMDRESULT, *POMCMMICMDRESULT;

typedef struct
{
   XTMMSGHDR      h;
   VMSIVRLINE     li;
   OMCMMICMDRESULT CmdResult;
} OMCMSGMMICMDRESULT, *POMCMSGMMICMDRSULT;

//////////////////////////////////////////////////////////////////////
// OMG
//////////////////////////////////////////////////////////////////////
typedef struct {
   int nProcId;
   int nSocketType;  // 0 : TCP, 1 : IUDP, 2:UUDP
   char szProcName[64];
   char szAddr[256];
   int nPort;
} PROTOCOLLIST, *PPROTOCOLLIST;

typedef struct {
   int  nAlarmLevel;
   char szServerName[64];
   char szAddr[256];
} PINGLIST, *PPINGLIST;

/// CONSOLE
/* 160804 by hak. for R111 PKG FAULT 메시지와 구조체 이름 겹침. 사용하지 않는 구조체로 보여 삭제함. */
#if 0
#define  CONSOLE_INFO_MAX_LEN 256

#define CON_LOG_ERROR  0
#define CON_LOG_WARN   1
#define CON_LOG_INFO   2

typedef struct {
  int             nLevel;
  char            szCaller[16];
  char            szCalled[16];
  char            szLog[CONSOLE_INFO_MAX_LEN];
} OMCCONSOLELOG, *POMCCONSOLELOG;

typedef struct {
  XTMMSGHDR       h;
  ARSLINE         line;
  OMCCONSOLELOG      Info;
} OMCMSGCONSOLELOG, *POMCMSGCONSOLELOG;
#endif
/* 160804 by hak. for R111 PKG FAULT 메시지와 구조체 이름 겹침. 사용하지 않는 구조체로 보여 삭제함. */


#endif /* __OMC_H__ */
