#ifndef __ALARM_H__
#define __ALARM_H__

#include "apptypes.h"

#define MSG_SUBF_ALARM_NOTIFY             0x0502    // OMCMSGALARM
#define MSG_SUBF_ALARM_CONSOLE            0x050A    // OMCMSGCONSOLE

/// CONSOLE
#define  MAX_CONSOLE_CODE 64
#define  MAX_CONSOLE_SRC 64
#define  MAX_CONSOLE_LOG 1024
#define  MAX_CONSOLE_SOLUTION 1024

// nLevel
#define CON_LOG_FAULT  0
#define CON_LOG_WARN   1
#define CON_LOG_INFO   2


//////////////////////////////////////////////////////////////////////////////
typedef enum {
ALARM_CLEARED,
	ALARM_MINOR,
	ALARM_MAJOR,
	ALARM_CRITICAL,
	ALARM_INFO,
} ALARM_LEVEL, *PALARM_LEVEL;

typedef enum {
    CommunicationFail,
    UnixSocketError,
    InetSocketError,
    DatabaseError,
    IPCShmError,
    IPCMsgqError,
    IPCSemError,
    TcpClosedError,
    ProcessError,
    ConfigError,
    SignalError = 10,
    ProcessKilled,
    CPUOverflow,
    DiskOverload,
    MemoryOverflow,
    SwapOverflow,
    E1boardError,
    E1portError,
    SS7SystemError,
    SS7BoardError,
    SS7LinkError = 20,
    NowHacking,
    DTMFNotify,
    StatusChange,
    CpuUtilChange,
    MemUtilChange,
    DiskUtilChange,
    ProcShutdown,
    VersionNotify,
    ThreadKilled,
    QueueFull = 30,
    FileError,
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
    BoardStatusError = 53,
    TermperatureStatusError = 54,
    FANStatusError = 55,
    ThresholdOverflow = 56,
    OverloadDrop = 57,
    CPSOverflow = 58,
    SessionOverflow = 59,
 
    /* define by KBELL */
    CPSThresholdLimitedNear = 60,
    ConnectionDeActive,
    ConfigChagneFail,
    HaStatusChange,
    AbnormalDialogTerminated,
    DosDetect,
    BlackList,
 
    LMTOverflow  = 72,
    OOSLMTOverflow = 73,
    LongCallNotify = 74,
    SuccRateError = 75,
    CommRateError = 76,
    CompRateError = 77,
    MediaTimeTooShort = 78,
    MediaKbpsTooLow = 79,
    SipOptionDeActive = 80,
    SipSyntaxError = 81,
    TemporaryFailed=82,
    RTTError=83,
    SIPReasonOverflow,
    HaConfigChange,
	ProcessHangUp = 86  // IBC R125 Added

} ALARM_TYPE, *PALARM_TYPE;


#define MAX_ALARM_SRC   64
#define MAX_ALARM_TEXT  512
#define MAX_ALARM_CAUSE 128
#define MAX_ALARM_CODE 64

typedef struct {
	unsigned int  nTime; 
	ALARM_LEVEL   nLevel;
	ALARM_TYPE	 nType;
	char          szSrc[MAX_ALARM_SRC];
	char          szText[MAX_ALARM_TEXT];
	char          szCause[MAX_ALARM_CAUSE];
} OMCALARMINFO, *POMCALARMINFO;

typedef struct {
	XTMMSGHDR     h;   
	VMSIVRLINE      li;	 
	OMCALARMINFO  Alarm;
} OMCMSGALARM, *POMCMSGALARM;

typedef struct {
  unsigned int    nTime;  // nTime 이 0 이면 OMP 에서 메시지를 받은 시간을 적재
  int             nLevel; // 0 : FAULT, 1 : WARN, 2 : INFO
  char            szCode[MAX_CONSOLE_CODE]; 
  char            szSrc[MAX_CONSOLE_SRC]; 
  char            szLog[MAX_CONSOLE_LOG]; 
  char            szSolution[MAX_CONSOLE_SOLUTION]; // Optional
} OMCCONSOLELOG, *POMCCONSOLELOG;

typedef struct {
  XTMMSGHDR       h;
  VMSIVRLINE          li;
  OMCCONSOLELOG      Info;
} OMCMSGCONSOLELOG, *POMCMSGCONSOLELOG;


//////////////////////////////////////////////////////////////////////////////
#endif //__ALARM_H__
