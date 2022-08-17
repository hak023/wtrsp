#ifndef __SHARED_MEMORY_H__
#define __SHARED_MEMORY_H__

#include <unistd.h>

//////////////////////////////////
#include "calltrace.h"
//////////////////////////////////

////////////////////////////////////////////////////////////////////////
// common ( callagent )
////////////////////////////////////////////////////////////////////////
#define SHM_MAGIC_CODE 0xa5a55a5a
#define MAX_RACKS                2
#define MAX_CHASSIS             4
#define MAX_SYSTEMS             8
#define MAX_BOARDS              6
#define MAX_SERVERS             10
#define MAX_PROCESSES           32

#define MAX_CM_SERVERS          2

#define MAX_PROCESSES           32
#define MAX_PROCESS_NAME_LEN    32 // including the NULL character!!
#define MAX_NUM_OF_ARGVS        10
#define MAX_ARGV_LEN            16
#define MAX_ALARM_COUNT         64

#define MAX_CPU                16
#define MAX_PARTITION          16
#define MAX_PARTITION_NAME     32
#define MAX_NETIF              8
#define MAX_NETIF_NAME         32

////////////////////////////////////////////////////////////////////////
// Process Status Information
////////////////////////////////////////////////////////////////////////
typedef struct {
   unsigned short size;
   unsigned short startflag;                 
   int valid;                                // valid flag(0: invalid)
   pid_t pid;                                // process ID
   pid_t ppid;                               // parent process ID
   char szName[MAX_PROCESS_NAME_LEN];        // process name
   char szAlias[MAX_PROCESS_NAME_LEN];       // process name(alias)
   int argc;                                 // same as argc of main()
   char argv[MAX_NUM_OF_ARGVS * MAX_ARGV_LEN +
             MAX_NUM_OF_ARGVS * sizeof(int) ];
                                             // same as argv of main()
   int alarm[MAX_ALARM_COUNT];               // alarm type
} PROCESS_STATUS, *PPROCESS_STATUS;

typedef struct {
   unsigned int dwMagicCode; // it must be 0xa5a55a5a
   short          major; // major version number(must be 1)
   short          minor; // minor version number(must be 0)
   unsigned int dwSysId; //system id

   PROCESS_STATUS procs[MAX_PROCESSES];

   char reserved[10*1024];
} SHM, *PSHM;

#define PNR_SHM_PATH         "../ipc/shm/top"

extern PSHM g_pShm;







///////////// HA SHM ///////////////////////
#define MAX_HA_SERVERS             2 
#define MAX_HA_PROCESSES           32
#define MAX_HA_IVR_BOARDS          8
#define MAX_HA_E1_LINES_PER_BOARD  16 //8
#define MAX_HA_CHANNELS_PER_LINE   32

#define MAX_HA_PROCESS_NAME_LEN    128 // including the NULL character!!
#define MAX_HA_NUM_OF_ARGVS        8
#define MAX_HA_ARGV_LEN            16
#define MAX_HA_ALARM_COUNT         64 // old : 16

////////////////////////////////////////////////////////////////////////
// Process Status Information
////////////////////////////////////////////////////////////////////////
typedef struct {
   int size;                                 // sizeof(PROCESS_STATUS)
   int valid;                                // valid flag(0: invalid)
   pid_t pid;                                // process ID
   pid_t ppid;                               // parent process ID
   char szName[MAX_HA_PROCESS_NAME_LEN];        // process name
   char szAlias[MAX_HA_PROCESS_NAME_LEN];       // process name(alias)
   int argc;                                 // same as argc of main()
   char argv[MAX_HA_NUM_OF_ARGVS * MAX_HA_ARGV_LEN +
             MAX_HA_NUM_OF_ARGVS * sizeof(char *)];
                                             // same as argv of main()
   //int acount;                               // alarm count
   int alarm[MAX_ALARM_COUNT];               // alarm type
} HA_PROCESS_STATUS, *PHA_PROCESS_STATUS;

typedef struct {
   char status[MAX_HA_E1_LINES_PER_BOARD][MAX_HA_CHANNELS_PER_LINE];
} HA_CHSTS, *PHA_CHSTS;

typedef struct {
  unsigned char status[MAX_HA_E1_LINES_PER_BOARD][MAX_HA_CHANNELS_PER_LINE];
} HA_CIRCUIT_STS, *PHA_CIRCUIT_STS;

typedef struct {
   int   HBTStatus;
   int   MandatoryFlag;
   int   MandatoryStatus;
   char  Reserved[1012];
} HA_STS, *PHA_STS;

typedef struct {
   unsigned long  dwMagicCode; // it must be 0xa5a55a5a
   short          major; // major version number(must be 1)
   short          minor; // minor version number(must be 0)
   unsigned long  dwSysId; //system id

   HA_PROCESS_STATUS procs[MAX_HA_PROCESSES];

   HA_CHSTS             ChStatus[MAX_HA_IVR_BOARDS]; //W: From ARS Process
   HA_CIRCUIT_STS       CircuitStatus[MAX_HA_IVR_BOARDS]; //W: From Call-Agent Process
   HA_STS            HaStatus;

   char Reserved[2 * 1024]; // for the future use
} HASHM, *PHASHM;

#define PNR_HASHM_PATH "../ha/ipc/shm/top"

extern PHASHM g_pHaShm;





#define MAX_EXT_SERVERS        32   //연동 서버 최대 개수

typedef struct {
	char szName[32];      // 연동 장비 명
	char szProcName[32];      // 연동 프로세스 명
	int	nValid;	// 사용 여부 (0 : 미사용, 1:사용)
    int nStatus;      // 상태 (0 : Inactive, 1 : Active)
    int nPort;         // nport (for the future use)
    int nCnt;          // 세션 개수 (for the future use)
    char szIp[32];      // ip
    char szDesc[128];  // (for the future use)
} EXT_SERVER_STATUS, *PEXT_SERVER_STATUS;

typedef struct {
    int nExtServerCount;  // 외부 연동 상태 정보 개수
	EXT_SERVER_STATUS ExtServerStatus[MAX_EXT_SERVERS];
char Reserved[10*1024]; // for the future use 
} EXT_SHM, *PEXT_SHM;

#define PNR_EXT_SHM_PATH "../ipc/shm/ExtTop"

extern PEXT_SHM g_pExtShm;

////////////////////////////////////////////////////////////////////////
// TS Status Information
////////////////////////////////////////////////////////////////////////
/*
#define  MAX_TS_NUM        2
#define  MAX_FAN_NUM       8
#define  MAX_POWER_NUM     8
#define  MAX_DISK_NUM      8

typedef struct {
   unsigned short usFanStatus_Rear[MAX_FAN_NUM];           //1: OK, 0: Fail
   unsigned short usFanStatus_Front[MAX_FAN_NUM];           //1: OK, 0: Fail
   unsigned short usPowerStatus[MAX_POWER_NUM];       //1: Ok, 0: Fail
} TSMON_STATUS, *PTSMON_STATUS;

typedef struct {
    TSMON_STATUS TsMonStatus[MAX_TS_NUM];

    char Reserved[2 * 1024]; // for the future use
} TSSHM, *PTSSHM;

#define PNR_TS_SHM_PATH "../ipc/shm/tstop"

extern PTSSHM g_pTSShm;
*/


//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
// OAM STATUS SHM
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// SYSTEM Status
//////////////////////////////////////////////////////////////////////
typedef struct {
   int   nTotal;
   int   nUsed;
} OMCUSAGEINFO, *POMCUSAGEINFO;

typedef struct {
   char  szName[MAX_PARTITION_NAME];
   OMCUSAGEINFO Usage; // in KByte.
} OMCPARTITIONINFO, *POMCPARTITIONINFO;

typedef struct {
   char  szName[MAX_NETIF_NAME];
   int   nTotal; // KByte.
   int   nIn;
   int   nOut;
} OMCNETIFINFO, *POMCNETIFINFO;

typedef struct {
   int              valid;
   int              CpuUsage;
   int              MemUsage; // KByte
   OMCPARTITIONINFO    Partition[MAX_PARTITION]; // KByte
   OMCNETIFINFO        Net[MAX_NETIF];
} SYSTEM_STATUS, *PSYSTEM_STATUS;

#define MAX_EXT_SERVERS 32

typedef struct {
    char szName[32];      // 연동 장비 명
    char szProcName[32];      // 연동 프로세스 명
    int nValid; // 사용 여부 (0 : 미사용, 1:사용)
    int nStatus;      // 상태 (0 : Inactive, 1 : Active)
    int nPort;         // nport (for the future use)
    int nCnt;          // 세션 개수 (for the future use)
    char szIp[32];      // ip     
    char szDesc[128];  // (for the future use)
} OMC_EXT_SERVER_STATUS, *POMC_EXT_SERVER_STATUS;

typedef struct {
    int nAreaId;
    int nSystemId;
    int nExtServerCount;
    OMC_EXT_SERVER_STATUS  status[MAX_EXT_SERVERS];
} OMC_EXT_SERVER_STS, *POMC_EXT_SERVER_STS;

typedef struct {
	// OMP
	SYSTEM_STATUS     SysStatus_OMP[MAX_SERVERS];
	PROCESS_STATUS    ProcStatus_OMP[MAX_SERVERS][MAX_PROCESSES];
		
	// SYSTEM
	SYSTEM_STATUS     SysStatus[MAX_SYSTEMS][MAX_SERVERS];
	PROCESS_STATUS    ProcStatus[MAX_SYSTEMS][MAX_SERVERS][MAX_PROCESSES];
	HA_PROCESS_STATUS    HaProcStatus[MAX_SYSTEMS][MAX_HA_SERVERS][MAX_HA_PROCESSES];
} STATUSSHM, *PSTATUSSHM;

#define STATUS_SHM_PATH         "../ipc/shm/status_top"

extern PSTATUSSHM g_pStatusShm;



#endif /*  __SHARED_MEMORY_H__  */

