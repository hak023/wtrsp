#ifndef __MSRPRM_SHM_H__
#define __MSRPRM_SHM_H__

#include <string>
#include "tgrm_shm.h"
#include "msrp_msg.h"

namespace MSRP
{

enum {
	E_MAX_MSRP_ROUTE = TGRM::EMG_MAX_TG_ROUTE,
	E_MAX_MSRP_BOARD = TGRM::EMG_MAX_TG_BOARD, //8
	E_MAX_MSRP_MSRPA_PER_BOARD = 1,
	E_MAX_MSRP_SESSION_PER_MSRPA = 50000,
	E_MAX_MSRP_TRACE = 10,
	E_MAX_MSRP_RM_SESSION = E_MAX_MSRP_BOARD * E_MAX_MSRP_MSRPA_PER_BOARD * E_MAX_MSRP_SESSION_PER_MSRPA, // 50000*10
};

typedef struct st_msrp_route : public TGRM::st_tg_route {
	unsigned char ucSetup; // active or passive
	unsigned char ucReserved[7]; // active or passive

	char* GetStrSetup(char *pBuf, int nLen)
	{
		char szSetup[32];
      switch(ucSetup)
      {
         case e_setup_actpass: snprintf(szSetup, sizeof(szSetup), "ACTPASS"); break;
         case e_setup_active: snprintf(szSetup, sizeof(szSetup), "ACTIVE"); break;
         case e_setup_passive: snprintf(szSetup, sizeof(szSetup), "PASSIVE"); break;
         default: snprintf(szSetup, sizeof(szSetup), "NONE(%d)", ucSetup); break;
      }
		snprintf(pBuf, nLen, "%s", szSetup);
		return pBuf;
	}

	char* str_add(char *pBuf, int nLen) {
		char szStr[512];
		char szSetup[32];

		str(szStr, sizeof(szStr));
		GetStrSetup(szSetup, sizeof(szSetup));

		snprintf(pBuf, nLen, "%s"
			"\tSETUP             : %s\n",
			szStr, szSetup);

   	return pBuf;
	}

} st_msrp_route, *pst_msrp_route;

typedef struct st_msrp_bd : public TGRM::st_tg_base {
	unsigned char ucNumMsrpa;
	unsigned char ucMode; // active or redundancy
	unsigned char ucModeStatus; // alloc|fail|not alloc
	unsigned char ucRecoverBoard; // Redundancy|alloc ÀÎ °æ¿ì
	unsigned char ucLastRecoverBoard;
	unsigned char ucReserved[3];

	int nRatio;
   unsigned short usBusyCnt;
   unsigned short usMaxSession;

	char* GetStrMode(char *pBuf, int nLen) 
	{
		std::string strTemp="";

		if (ucMode & e_bd_mode_active)
			strTemp = "ACT";

		if (ucMode & e_bd_mode_redundancy)
			strTemp = "RD";

		snprintf(pBuf, nLen-1, "%s", strTemp.c_str());
		return pBuf;
	}

	char* GetStrModeStatus(char *pBuf, int nLen) 
	{
		std::string strTemp="";

		if (ucModeStatus & e_bd_ip_alloc)
			strTemp += "ALLOC";

		if (ucModeStatus & e_bd_ip_fail)
		{
			if (strTemp.size())
				strTemp += "|";

			strTemp += "FAIL";
		}

		if (ucModeStatus & e_bd_ip_not_alloc)
		{
			if (strTemp.size())
				strTemp += "|";

			strTemp += "NOT ALLOC";
		}

		if (ucModeStatus == 0)
			strTemp = "NONE";

		snprintf(pBuf, nLen-1, "%s", strTemp.c_str());
		return pBuf;
	}

	char* str(char *pBuf, int nLen) {
		char szStatus[128];
		char szMode[128];
		char szModeStatus[128];

		GetStrStatus(szStatus, sizeof(szStatus));
		GetStrMode(szMode, sizeof(szMode));
		GetStrModeStatus(szModeStatus, sizeof(szModeStatus));

		snprintf(pBuf, nLen-1,
			"[MSRP Board Info]\n"
			"\tVALID          : %02d\n"
			"\tSTATUS         : %s(0x%x)\n"
			"\tID             : %02d\n"
			"\tNUM MSRPA      : %d\n"
			"\tMODE           : %s\n"
			"\tMODE STATUS    : %s(0x%x)\n"
			"\tRATIO          : %d\n"
			"\tRECOVER BOARD  : (CUR:%d LAST:%d)\n"
			"\tUSAGE          : %d/%d\n"
			, ucValid, szStatus, ucStatus, ucID, ucNumMsrpa, 
			szMode, szModeStatus , ucModeStatus, nRatio, ucRecoverBoard, ucLastRecoverBoard,
         usBusyCnt, usMaxSession);
   	return pBuf;
	}

} st_msrp_bd, *pst_msrp_bd;

typedef struct st_msrp_pool : public TGRM::st_tg_pool {
	char ucRecoverPool[E_MAX_MSRP_BOARD];
	char ucReserved[6];

	char* str_add(char *pBuf, int nLen) {
		char szStr[1024];
		char szTmp[128];
		std::string strTemp="\tREDUNDANY POOL    : ";
		str(szStr, sizeof(szStr));

		for(int nboard=0;nboard<E_MAX_MSRP_BOARD;nboard++)
		{
			if (ucRecoverPool[nboard] == 0)
				continue;
			snprintf(szTmp, sizeof(szTmp), "(B_%d P_%d)", nboard+1, ucRecoverPool[nboard]);
			strTemp += szTmp;
			strTemp += " ";
		}
		snprintf(pBuf, nLen-1, "%s %s", szStr, strTemp.c_str());

		return pBuf;
	}

} st_msrp_pool, *pst_msrp_pool;

typedef struct st_msrp_msrpa : public TGRM::st_tg_base {
	unsigned char ucBoardIndex;
	unsigned char ucReserved[3];
	unsigned short usMaxSession;
	unsigned short usReserved;

	char* str(char *pBuf, int nLen) {
		char szStatus[128];
		GetStrStatus(szStatus, sizeof(szStatus));

		snprintf(pBuf, nLen-1,
			"[MSRP Info]\n"
			"\tBOARD ID       : %02d\n"
			"\tMSRPA ID       : %02d\n"
			"\tVALID          : %02d\n"
			"\tSTATUS         : %s(0x%x)\n"
			"\tMAX SESSION    : %d\n"
			, ucBoardIndex, ucID, ucValid, szStatus, ucStatus, usMaxSession);
   	return pBuf;
	}

	char* str2(char *pBuf, int nLen) {
		char szStatus[128];
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));

		snprintf(pBuf, nLen-1,
			"B:%d M:%d VALID:%d STATUS:%s(0x%x) MAX:%d"
			,ucBoardIndex, ucID, ucValid, szStatus, ucStatus, usMaxSession);
		return pBuf;
	}

} st_msrp_msrpa, *pst_msrp_msrpa;


struct st_msrprm_shm{
	st_msrp_route stRoute[E_MAX_MSRP_ROUTE];
   st_msrp_bd stBoard[E_MAX_MSRP_BOARD]; // max 10
   st_msrp_msrpa stMsrpa[E_MAX_MSRP_BOARD][E_MAX_MSRP_MSRPA_PER_BOARD]; // max 32
	st_msrp_pool stPool[E_MAX_MSRP_BOARD][TGRM::EMG_MAX_TG_POOL_PER_BOARD];
	TGRM::st_tg_rsc stRsc[E_MAX_MSRP_BOARD][TGRM::EMG_MAX_TG_RSC_PER_BOARD];

   unsigned char ucSesStatus[E_MAX_MSRP_BOARD][E_MAX_MSRP_MSRPA_PER_BOARD][E_MAX_MSRP_SESSION_PER_MSRPA]; 
};

#define MSRPRM_SHM_PATH         "../ipc/shm/msrprm_wshm"


typedef struct {
   //index info, internal session id
   unsigned int uiidx;
   unsigned int uiReserved;
   
   //tgas&msrpa ses id
   unsigned int   uimsrpasesid;
   unsigned int   uitgassesid;
   
   //tgas&msrpa id
   unsigned short usmsrpaid;
   unsigned short ustgasid;

   unsigned char ucBoardIndex;
   unsigned char ucMsrpaIndex;
   unsigned short usSesIndex;

   unsigned char ucCh;
   unsigned char ucCallerRscIndex;
   unsigned char ucCalledRscIndex;
   unsigned char ucCallerPoolIndex;
   unsigned char ucCalledPoolIndex;
   unsigned char ucIPVer;
   unsigned char ucReserved[2];

   st_msrp_sdp_info stsdp_cr;
   st_msrp_sdp_info stalloc_cd;
   
   struct timeval start_time;
   unsigned char Status[2][2]; // STATUS_TYPE_REQ, RSP

} ses_traits_sync, *pses_traits_sync;

enum { 
	SYNC_TYPE_INIT = 0,
	SYNC_TYPE_SET = 1,
	SYNC_TYPE_DEL = 2
};

typedef struct ses_traits_sync_all {
	unsigned char ucChanged;
	unsigned char ucSyncType;
	unsigned char ucReserved[6];
	struct timeval change_time;
	ses_traits_sync ses_sync;
} ses_traits_sync_all, *pses_traits_sync_all;

typedef struct ses_traits_sync_page{
	int nIsWrite;
	int nReserved;
	ses_traits_sync_all   stRMSync[E_MAX_MSRP_RM_SESSION]; 
} ses_traits_sync_page, *pses_traits_sync_page;

typedef struct {
	int nIsPage;  // o or 1
	int nReserved;   

	ses_traits_sync_page  stSyncPageA; 
	ses_traits_sync_page  stSyncPageB; 
} st_shm_msrprmsync, *pst_shm_msrprmsync;

} // name space MSRP
#endif
