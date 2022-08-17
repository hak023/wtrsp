#ifndef __MPRM_SHM_H__
#define __MPRM_SHM_H__

#include "mp_if.h"

namespace MPRM
{

enum {
	EMG_MAX_MP_BOARD = 10,
	EMG_MAX_MP_MPA_PER_BOARD = 8,
	EMG_MAX_MP_MPA = EMG_MAX_MP_BOARD * EMG_MAX_MP_MPA_PER_BOARD,
	EMG_MAX_MP_SESSION_PER_BOARD = 1000,
	EMG_MAX_MP_SESSION_PER_MPA = 1000,
	EMG_MAX_MP_GROUP_PER_BOARD = EMG_MAX_MP_SESSION_PER_BOARD*2,
	EMG_MAX_MP_GROUP_PER_MPA = EMG_MAX_MP_SESSION_PER_MPA*2,
	EMG_MAX_MP_RM_SESSION = EMG_MAX_MP_BOARD * EMG_MAX_MP_SESSION_PER_BOARD,
};

enum e_mpa_status{ 
	e_mpa_status_ok       = 0x00,
	e_mpa_status_block    = 0x02,
	e_mpa_status_ng       = 0x04,
	e_mpa_status_connect_fail = 0x08,
};

enum e_ses_status{
	e_ses_notuse = 0x00,
	e_ses_idle   = 'I',
	e_ses_start  = 'S',
	e_ses_play   = 'P',
	e_ses_relay  = 'R',
	e_ses_relay_tc = 'T',
	e_ses_record = 'E',
	e_ses_fault = 'F',
};

typedef struct st_mp_base {
	unsigned int uReason;
	unsigned char ucEnabled;
	unsigned char ucValid;
	unsigned char ucID;
	unsigned char ucStatus; // use not useµî  e_status

	char* GetStrStatus(char *pBuf, int nLen) {
		std::string strTemp="";
		if (ucStatus == e_mpa_status_ok)
		{
			strTemp = "OK";
		}
		else 
		{
			if (ucStatus & e_mpa_status_ok) strTemp = "OK";
			if (ucStatus & e_mpa_status_block)
			{
				if (strTemp.size()) strTemp += "&";
				strTemp += "BLK";
			}
			if (ucStatus & e_mpa_status_ng)
			{
				if (strTemp.size()) strTemp += "&";
				strTemp += "NG";
			}
			if (ucStatus & e_mpa_status_connect_fail)
			{
				if (strTemp.size()) strTemp += "&";
				strTemp += "CONN_FAIL";
			}
		}
		snprintf(pBuf, nLen-1, "%s", strTemp.c_str());
		return pBuf;
	}

} st_mp_base, *pst_mp_base;


typedef struct st_mp_bd : public st_mp_base {
	unsigned char ucNumMPA;
	unsigned char ucReserved[7];

	char* str(char *pBuf, int nLen) {
		char szStatus[128];
		GetStrStatus(szStatus, sizeof(szStatus));

		snprintf(pBuf, nLen-1,
			"[MP Board Info]\n"
			"\tVALID       : %02d\n"
			"\tSTATUS      : %s(0x%x)\n"
			"\tID          : %02d\n"
			"\tNUMMPA      : %d\n"
			, ucValid, szStatus, ucStatus, ucID, ucNumMPA);
		return pBuf;
	}
} st_mp_bd, *pst_mp_bd;

typedef struct st_mp_mpa : public st_mp_base {
	unsigned char ucBoardIndex;
	unsigned char ucReserved[3];
	unsigned short usMaxSession;
	unsigned short usMaxGroup;

	char* str(char *pBuf, int nLen) {
		char szStatus[128];
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));

		snprintf(pBuf, nLen-1,
			"[MPA Info]\n"
			"\tBOARD ID       : %02d\n"
			"\tMPA ID         : %02d\n"
			"\tVALID          : %s\n"
			"\tMAX SESSION    : %d\n"
			"\tMAX GROUP      : %d\n"
			,ucBoardIndex, ucID, szStatus, usMaxSession, usMaxGroup);
   	return pBuf;
	}

	char* str2(char *pBuf, int nLen) {
		char szStatus[128];
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));

		snprintf(pBuf, nLen-1,
			"B:%d MPA:%d VALID:%d STATUS:%s(0x%x)"
			,ucBoardIndex, ucID, ucValid, szStatus, ucStatus);
		return pBuf;
	}

} st_mp_mpa, *pst_mp_mpa;


struct st_mprm_shm{
   st_mp_bd stBoard[EMG_MAX_MP_BOARD]; // max 10
   st_mp_mpa stMpa[EMG_MAX_MP_MPA][EMG_MAX_MP_MPA_PER_BOARD]; // max 80
   unsigned char ucSesStatus[EMG_MAX_MP_BOARD][EMG_MAX_MP_MPA_PER_BOARD][EMG_MAX_MP_SESSION_PER_MPA]; // max 500

};

#define MPRM_SHM_PATH         "../ipc/shm/mprm_wshm"


typedef struct {
   //index info, internal session id
   unsigned int uiidx;
   unsigned int uiReserved;
   
   //mpa&tgas ses id
   unsigned int   uitcasesid;
   unsigned int   uitgassesid;
   
   //mpa&mgi id
   unsigned short ustcaid;
   unsigned short ustgasid;

   unsigned char ucBoardIndex;
   unsigned char ucBoardMPAIndex;
   unsigned short usSesIndex;
   
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
	ses_traits_sync_all   stRMSync[EMG_MAX_MP_RM_SESSION]; 
} ses_traits_sync_page, *pses_traits_sync_page;

typedef struct {
	int nIsPage;  // o or 1
	int nReserved;   

	ses_traits_sync_page  stSyncPageA; 
	ses_traits_sync_page  stSyncPageB; 
} st_shm_mprmsync, *pst_shm_mprmsync;

} // name space MPRM

#endif
