#ifndef __TGRM_SHM_H__
#define __TGRM_SHM_H__

#include "tr_msg.h"

namespace TGRM
{

enum 
{
	EMG_MAX_TG_ROUTE = 10, // MAX ROUTE
	EMG_MAX_TG_BOARD = 10, // MAX BOARD
	EMG_MAX_TG_POOL_PER_BOARD = 10, // MAX POOL
	EMG_MAX_TG_RSC_PER_BOARD = 10, // MAX RSC
	EMG_MAX_TG_TGA_PER_BOARD = 1, // MAX TGA
	EMG_MAX_TG_TGA = EMG_MAX_TG_BOARD * EMG_MAX_TG_TGA_PER_BOARD, // MAX TGA PER TGRM
	EMG_MAX_TG_SESSION_PER_RSC = 15000, // MAX SESSION PER RSC
	EMG_MAX_TG_SESSION_PER_POOL = 15000, // MAX SESSION PER POOL
	EMG_MAX_TG_SESSION_PER_TGA = EMG_MAX_TG_SESSION_PER_POOL * EMG_MAX_TG_POOL_PER_BOARD, // MAX SESSION PER TGA
	EMG_MAX_TG_SESSION_PER_BOARD = EMG_MAX_TG_SESSION_PER_TGA * EMG_MAX_TG_TGA_PER_BOARD, // MAX SESSION PER BOARD
	EMG_MIN_START_PORT = 1000, // MIN START PORT
	EMG_MAX_END_PORT = 65000, // MAX END PORT
	EMG_MAX_TG_SYNC_SESSION = 180000, // MAX SYNC SESSION
	EMG_MAX_TG_RM_SESSION = EMG_MAX_TG_BOARD * EMG_MAX_TG_SESSION_PER_BOARD,
};

enum e_tga_status
{
	e_tga_status_ok           = 0x00, // available
	e_tga_status_ng           = 0x01, //
	e_tga_status_block        = 0x02, // maintenance block
	e_tga_status_connect_fail = 0x04, // connect fail
	e_tga_status_not_active   = 0x08, // wait status...
	e_tga_status_hw_fail      = 0x10, // cpu, mem fail
	e_tga_status_nic_fail     = 0x20, // nic fail
};

enum e_ses_status
{
	e_ses_notuse = 0x00, // available
	e_ses_idle   = 'I',  // init
	e_ses_start  = 'S',  // recv add
	e_ses_relay  = 'R',  // recv mod
	e_ses_block  = 'M',  // maintenance block
	e_ses_fault  = 'F',  // network fault
};

typedef struct st_tg_base 
{
	unsigned int  uiReason;
	unsigned char ucEnabled;
	unsigned char ucValid;
	unsigned char ucStatus; // e_tga_status
	unsigned char ucID;
   unsigned char ucReserved[4];
	char szName[64];

	char* GetStrStatus(char *pBuf, int nLen) 
	{
		std::string strTemp="";
		if (ucStatus == e_tga_status_ok)
		{
			strTemp = "IDLE";
		}
		else 
		{
			if (ucStatus & e_tga_status_block)
			{
				if (strTemp.size()) strTemp += " & ";
				strTemp += "BLK";
			}
			if (ucStatus & e_tga_status_ng)
			{
				if (strTemp.size()) strTemp += " & ";
				strTemp += "NG";
			}
			if (ucStatus & e_tga_status_connect_fail)
			{
				if (strTemp.size()) strTemp += " & ";
				strTemp += "CONN_FAIL";
			}
			if (ucStatus & e_tga_status_not_active)
			{
				if (strTemp.size()) strTemp += " & ";
				strTemp += "NOT_ACTIVE";
			}
			if (ucStatus & e_tga_status_hw_fail)
			{
				if (strTemp.size()) strTemp += " & ";
				strTemp += "HW_FAIL";
			}
			if (ucStatus & e_tga_status_nic_fail)
			{
				if (strTemp.size()) strTemp += " & ";
				strTemp += "NIC_FAIL";
			}
		}
		snprintf(pBuf, nLen-1, "%s", strTemp.c_str());
		return pBuf;
	}

} st_tg_base, *pst_tg_base;

typedef struct st_tg_route : public st_tg_base 
{
	unsigned int uiTrte; // TRTE
   unsigned int uiLmtrt; // LMT_RT
	unsigned int uiOOSLmtrt; // OOS_LMT_RT

	unsigned char ucDefaultIPVer; 
	unsigned char ucML_BLK; 
	unsigned char ucF_BLK; 
	unsigned char ucSrtp;

	unsigned int uiUsedSes;
	unsigned int uiAvailSes;
	unsigned int uiTotalSes;
	unsigned int uiTotalRsc;

	char* str(char *pBuf, int nLen) 
	{
		char szStatus[128];
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));

		snprintf(pBuf, nLen-1,
			"[ROUTE Info]\n"
			"\tID                : %02d\n"
			"\tNAME              : %s\n"
			"\tENABLE            : %d\n"
			"\tVALID             : %02d\n"
			"\tSTATUS            : %s(0x%x)\n"
			"\tDEFALUT IPVER     : %d\n"
			"\tSRTP              : %s\n"
			"\tROUTE ID          : %02d\n"
			"\tLMT_RT            : %d\n"
			"\tOOS_RT            : %d\n"
			, ucID, szName, ucEnabled, ucValid, szStatus, ucStatus,  
			   ucDefaultIPVer, ucSrtp?"ON":"OFF", uiTrte, uiLmtrt, uiOOSLmtrt);
   	return pBuf;
   }
} st_tg_route, *pst_tg_route;

typedef struct st_tg_bd : public st_tg_base 
{
	unsigned char ucNumTGA;
	unsigned char ucMode;
	unsigned char ucRecoverBoard;
	unsigned char ucReserved;
	int nRatio;

	unsigned int uiBusyCnt;
	unsigned int uiMaxSession;

	char* GetStrMode(char *pBuf, int nLen) 
	{
		std::string strTemp="";
		if (ucMode & e_bd_mode_active)
		{
			strTemp = "ACTIVE";
			if (ucMode & e_bd_mode_fail)
			{
				if (strTemp.size()) strTemp += " & ";
				strTemp += "FAIL";
			}
		}
		else if (ucMode & e_bd_mode_redundancy)
		{
			strTemp = "REDUNDANCY";
			if (ucMode & e_bd_mode_fail)
			{
				if (strTemp.size()) strTemp += " & ";
				strTemp += "FAIL";
			}
		}
		else
			strTemp = "UNKNOWN";
		snprintf(pBuf, nLen-1, "%s", strTemp.c_str());
		return pBuf;
	}

	char* str(char *pBuf, int nLen) 
	{
		char szMode[32];
		char szStatus[128];
		memset(szMode, 0, sizeof(szMode));
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));
		GetStrMode(szMode, sizeof(szMode));

		snprintf(pBuf, nLen-1,
			"[Board Info]\n"
			"\tID                : %02d\n"
			"\tNAME              : %s\n"
			"\tENABLE            : %d\n"
			"\tVALID             : %d\n"
			"\tSTATUS            : %s(0x%x)\n"
			"\tNUMTGA            : %d\n"
			"\tMODE              : %s(%d)\n"
			"\tRATIO             : %d\n"
			"\tBusyCnt           : %u\n"
			"\tMaxSession        : %u\n"
			, ucID, szName, ucEnabled, ucValid, szStatus, ucStatus, 
			   ucNumTGA, szMode, ucMode, nRatio, uiBusyCnt, uiMaxSession); 
		return pBuf;
	}
} st_tg_bd, *pst_tg_bd;

// board별 ip단위로 부여.. kernel 모듈 id와 1:1매치
typedef struct st_tg_pool : public st_tg_base
{
	unsigned char ucBoardIndex;
	unsigned char ucPoolIndex; // kernel id
	unsigned char ucIPVer; 
	unsigned char ucReserved[5];
	char szNIC[64];

	unsigned int uiStartPort; // S_PORT
	unsigned int uiEndPort; // E_PORT
	unsigned int uiMaxSession; // MAX_CNT
	unsigned int uiReserved;

   tr_addr  IP;

	char* str(char *pBuf, int nLen) 
	{
		char szStatus[128];
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));

	   char szAddr[64]; szAddr[0] = '\0';
		IP.str(szAddr, sizeof(szAddr));

		snprintf(pBuf, nLen-1,
			"[POOL Info]\n"
			"\tID                : %02d\n"
			"\tNAME              : %s\n"
			"\tENABLE            : %d\n"
			"\tVALID             : %d\n"
			"\tSTATUS            : %s(0x%x)\n"
			"\tBOARD INDEX       : %02d\n"
			"\tPOOL INDEX        : %02d\n"
			"\tIPVER             : %d\n"
			"\tIP                : %s\n"
			"\tNIC               : %s\n"
			"\tSTART PORT        : %d\n"
			"\tEND PORT          : %d\n"
			"\tMAX SESSION       : %d\n"
			, ucID, szName, ucEnabled, ucValid, szStatus, ucStatus,  
		   ucBoardIndex, ucPoolIndex, ucIPVer, szAddr, szNIC, uiStartPort, uiEndPort, uiMaxSession);
		return pBuf;
	}
} st_tg_pool, *pst_tg_pool;

typedef struct st_tg_tga : public st_tg_base 
{
	unsigned char ucBoardIndex;
	unsigned char ucReserved[7];

	char* str(char *pBuf, int nLen) 
	{
		char szStatus[128];
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));

		snprintf(pBuf, nLen-1,
			"[TGA Info]\n"
			"\tID                : %02d\n"
			"\tNAME              : %s\n"
			"\tENABLE            : %d\n"
			"\tVALID             : %d\n"
			"\tSTATUS            : %s(0x%x)\n"
			"\tBOARD INDEX       : %02d\n"
			, ucID, szName, ucEnabled, ucValid, szStatus, ucStatus,  
			   ucBoardIndex);
   	return pBuf;
	}
} st_tg_tga, *pst_tg_tga;

// poolindex는 st_tg_pool에 있는 pool index와 맞아야 함
// st_tg_pool과 1:n 구조
typedef struct st_tg_rsc : public st_tg_base 
{
	unsigned char ucBoardIndex;
	unsigned char ucPoolIndex;
	unsigned char ucIPVer; // st_tg_pool 정보 참조. 자동 입력
	unsigned char ucReserved[5];

	unsigned int uiTrte; // TRTE
   unsigned int uiRscId; // RSCID

   unsigned int uiBusyCnt; // BUSY_CNT
	unsigned int uiMaxSession; // MAX_CNT
	tr_addr  IP; // st_tg_pool 정보 참조. 자동입력.

	char* str(char *pBuf, int nLen) 
	{
		char szStatus[128];
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));

	   char szAddr[64]; szAddr[0] = '\0';
		IP.str(szAddr, sizeof(szAddr));

		snprintf(pBuf, nLen-1,
			"[RSC Info]\n"
			"\tID                : %02d\n"
			"\tNAME              : %s\n"
			"\tENABLE            : %d\n"
			"\tVALID             : %d\n"
			"\tSTATUS            : %s(0x%x)\n"
			"\tBOARD INDEX       : %02d\n"
			"\tPOOL  INDEX       : %02d\n"
			"\tROUTE ID          : %02d\n"
			"\tRESOURCE ID       : %02d\n"
			"\tBUSY COUNT        : %d\n"
			"\tMAX SESSION       : %d\n"
			"\tIP                : %s\n"
			, ucID, szName, ucEnabled, ucValid, szStatus, ucStatus,  
		   ucBoardIndex, ucPoolIndex, uiTrte, uiRscId, uiBusyCnt, uiMaxSession, szAddr);
   	return pBuf;
	}
} st_tg_rsc, *pst_tg_rsc;

struct st_tgrm_shm
{
	st_tg_route stRoute[EMG_MAX_TG_ROUTE]; // ROUTE INFO
   st_tg_bd stBoard[EMG_MAX_TG_BOARD]; // BOARD INFO
   st_tg_pool stPool[EMG_MAX_TG_BOARD][EMG_MAX_TG_POOL_PER_BOARD]; // Pool INFO 
   st_tg_rsc stRsc[EMG_MAX_TG_BOARD][EMG_MAX_TG_RSC_PER_BOARD]; // RESOURCE INFO
	
   st_tg_tga stTga[EMG_MAX_TG_BOARD][EMG_MAX_TG_TGA_PER_BOARD]; // TGA
   tr_session stSes[EMG_MAX_TG_BOARD][EMG_MAX_TG_TGA_PER_BOARD][EMG_MAX_TG_SESSION_PER_TGA]; // SESSION

	unsigned char ucSesStatus[EMG_MAX_TG_BOARD][EMG_MAX_TG_POOL_PER_BOARD][EMG_MAX_TG_SESSION_PER_POOL]; 
};

#define TGRM_SHM_PATH     "../ipc/shm/tgrm_wshm"

typedef struct 
{
   // index info, internal session id
   unsigned int  uiidx;
   unsigned int  uiReserved;
   
   // tga&tgas ses id
   unsigned int  uitgasesid;  // tga session id
   unsigned int  uitgassesid;  // tgas session id
   
   // tga&tgas id
   unsigned short  ustgaid;  // tga id
   unsigned short  ustgasid;  // tgas id

   // tg_session
   struct  tr_session ses;

   unsigned char  ucBoardIndex;  // board index
   unsigned char  ucBoardTGAIndex;  // tga index
	unsigned char  ucRecoverBoard;  // recover board index
   unsigned char  ucPoolSIndex;  // Pool Src index
   unsigned char  ucPoolDIndex;  // Pool Dst index
	unsigned char  ucTCEnable;    // TC Enable Flag
   unsigned char  ucTCPoolSIndex;  // TC Pool Src index
   unsigned char  ucTCPoolDIndex;  // TC Pool Dst index
   //unsigned char  ucReserved[2];  

   unsigned short usSesSIndex;  // session Src index
   unsigned short usSesDIndex;  // session Dst index
   unsigned short usTCSesSIndex;  // TC session Src index
   unsigned short usTCSesDIndex;  // TC session Dst index
	//unsigned short usReserved[2];
   
   struct timeval start_time;  // start time
   unsigned char Status[2][2];  // STATUS_TYPE_REQ, RSP

} ses_traits_sync, *pses_traits_sync;

enum 
{ 
	SYNC_TYPE_INIT = 0,
	SYNC_TYPE_SET = 1,
	SYNC_TYPE_DEL = 2
};

typedef struct ses_traits_sync_all 
{
	unsigned char ucChanged;
	unsigned char ucSyncType;
	unsigned char ucReserved[6];
	struct timeval change_time;
	ses_traits_sync ses_sync;
} ses_traits_sync_all, *pses_traits_sync_all;

typedef struct ses_traits_sync_page
{
	int nIsWrite;
	int nReserved;
	ses_traits_sync_all   stRMSync[EMG_MAX_TG_SYNC_SESSION]; 
} ses_traits_sync_page, *pses_traits_sync_page;

typedef struct 
{
	int nIsPage;  // o or 1
	int nReserved;   

	ses_traits_sync_page  stSyncPageA; 
	ses_traits_sync_page  stSyncPageB; 
} st_shm_tgrmsync, *pst_shm_tgrmsync;

} // name space TGRM

#endif
