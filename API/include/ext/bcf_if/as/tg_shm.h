#ifndef __TGRM_SHM_H__
#define __TGRM_SHM_H__

#include "tr_msg.h"
#include "msrp_msg.h"

namespace TGRM 
{

enum 
{
	EMG_MAX_TG_ROUTE = 40, // MAX ROUTE
	EMG_MAX_TG_BOARD = 16, // MAX BOARD
	EMG_MAX_TG_POOL_PER_BOARD = 10, // MAX POOL
	EMG_MAX_TG_RSC_PER_BOARD = 80, // MAX RSC
	EMG_MAX_TG_TGA_PER_BOARD = 1, // MAX TGA
	EMG_MAX_TG_TGA = EMG_MAX_TG_BOARD * EMG_MAX_TG_TGA_PER_BOARD, // MAX TGA PER TGRM
	EMG_MAX_TG_SESSION_PER_RSC = 15000, // MAX SESSION PER RSC
	EMG_MAX_TG_SESSION_PER_POOL = 15000, // MAX SESSION PER POOL
	EMG_MAX_TG_SESSION_PER_TGA = EMG_MAX_TG_SESSION_PER_POOL * EMG_MAX_TG_POOL_PER_BOARD, // MAX SESSION PER TGA
	EMG_MAX_TG_SESSION_PER_BOARD = EMG_MAX_TG_SESSION_PER_TGA * EMG_MAX_TG_TGA_PER_BOARD, // MAX SESSION PER BOARD
	EMG_MIN_START_PORT = 5000, // MIN START PORT
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
	e_tga_status_init_fail    = 0x40, // init fail
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
			if (ucStatus & e_tga_status_init_fail)
			{
				if (strTemp.size()) strTemp += " & ";
				strTemp += "INIT_FAIL";
			}
		}
		snprintf(pBuf, nLen-1, "%s", strTemp.c_str());
		return pBuf;
	}

} st_tg_base, *pst_tg_base;

typedef struct st_tg_route : public st_tg_base 
{
	unsigned int uiTRTE; // TRTE
   unsigned char ucLMT_RT; // LMT_RT
	unsigned char ucOOS_LMT_RT; // OOS_LMT_RT
	char cDefIPVer;  // Default IP Ver
	unsigned char ucML_BLK; 

	unsigned char ucF_BLK; 
	char cOwn;  // 0: 타사. 1:자사
	unsigned char ucReserved[6];

	// tg only
	char cSRTP;  
	char cRemoteNATCnt;
	char cChkIPPort;
	char cUseAuth; // SHA-1
	char cUseEnc; // 0:Off 1:AES 2:ARIA
	char cChkPT;
	char cChkSSRC; // not use
	char cChgPT;

	char cChgSSRC; // not use
	char cChkSeq;
	char cChkTS;
	char cOMRAdd;
	char cUseImgAttr;
	char cPID;  // PT Group ID
	char cReserved[2];

	char cEncKey[128]; // 128byte 모두 사용
	////////////

	// msrp only
	unsigned char ucSetup; // msrp용. active, passive, actpass
	unsigned char ucReserved2[7];
	////////////

	unsigned int uiUsedSes;
	unsigned int uiAvailSes;
	unsigned int uiTotalSes;
	unsigned int uiTotalRsc;

	//R103 PKG. 150720 by hak. for Allow Codec List.
	char cChkCodec; // codecList기능 사용여부
	char cCodecList[64]; // 허용할 codec 리스트를 입력받음. ex) 1/2/3/4 
	char cReserved2[7];

   char* GetStrSetup(char *pBuf, int nLen)
   {
      char szSetup[32];
      switch(ucSetup)
      {
         case MSRP::e_setup_actpass: snprintf(szSetup, sizeof(szSetup), "ACTPASS"); break;
         case MSRP::e_setup_active: snprintf(szSetup, sizeof(szSetup), "ACTIVE"); break;
         case MSRP::e_setup_passive: snprintf(szSetup, sizeof(szSetup), "PASSIVE"); break;
         default: snprintf(szSetup, sizeof(szSetup), "NONE(%d)", ucSetup); break;
      }
      snprintf(pBuf, nLen, "%s", szSetup);
      return pBuf;
   }

   char* GetStrEnc(char *pBuf, int nLen)
  	{ 
      char szUseEnc[32];
      switch(cUseEnc)
      {
         case 0: snprintf(szUseEnc, sizeof(szUseEnc), "OFF"); break;
         case 1: snprintf(szUseEnc, sizeof(szUseEnc), "AES"); break;
         case 2: snprintf(szUseEnc, sizeof(szUseEnc), "ARIA"); break;
         default: snprintf(szUseEnc, sizeof(szUseEnc), "NONE(%d)", cUseEnc); break;
      }
      snprintf(pBuf, nLen, "%s", szUseEnc);
      return pBuf;
   }

	char* str(char *pBuf, int nLen) 
	{
		char szStatus[128];
		char szSetup[32];
		memset(szStatus, 0, sizeof(szStatus));
		memset(szSetup, 0, sizeof(szSetup));

		GetStrStatus(szStatus, sizeof(szStatus));
		GetStrSetup(szSetup, sizeof(szSetup));

		snprintf(pBuf, nLen-1,
			"[ROUTE Info]\n"
			"\tROUTE ID          : %02d\n"
			"\tID                : %02d\n"
			"\tNAME              : %s\n"
			"\tENABLE            : %d\n"
			"\tVALID             : %02d\n"
			"\tSTATUS            : %s(0x%x)\n"
			"\tDEFALUT IPVER     : %d\n"
			"\tSRTP              : %s\n"
			"\tSETUP             : %s\n"
			"\tOWN               : %d\n"
			"\tLMT_RT            : %d\n"
			"\tOOS_RT            : %d\n"
			"\tCHECK_CODEC       : %d\n"
			"\tCodecList         : %s\n"
			, uiTRTE, ucID, szName, ucEnabled, ucValid, szStatus, ucStatus,  
			cDefIPVer, cSRTP?"ON":"OFF", szSetup, cOwn, ucLMT_RT, ucOOS_LMT_RT, cChkCodec, cCodecList);
   	return pBuf;
   }

	char* str2(char *pBuf, int nLen) 
	{
		char szStatus[128];
		char szSetup[32];
		char szUseEnc[16];

		memset(szStatus, 0, sizeof(szStatus));
		memset(szSetup, 0, sizeof(szSetup));
		memset(szUseEnc, 0, sizeof(szUseEnc));

		GetStrStatus(szStatus, sizeof(szStatus));
		GetStrSetup(szSetup, sizeof(szSetup));
		GetStrEnc(szUseEnc, sizeof(szUseEnc));

		snprintf(pBuf, nLen-1,
			"[ROUTE %d]\n"
			"\tNAME:%s ENABLE:%d VALID:%d STATUS:%s(0x%x) DEF_IPV:%d PID:%d SETUP:%s OWN:%d\n"
			"\tLMT_RT:%d OOS_RT:%d Ses(Used:%d Avail:%d Total:%d)\n"
			"\tRsc[BLK(ML:%d F:%d) Total:%d]\n"
			"\tSRTP:%d NAT:%d CHK_IPPORT:%d CHK(PT:%d SEQ:%d TS:%d) CHG(PT:%d)\n"
			"\tAUTH:%d ENC(%s Key:%s) OMR_ADD:%d IMGATTR:%d CHK_CODEC:%d CODEC_LIST:%s\n"
			, uiTRTE, szName, ucEnabled, ucValid, szStatus, ucStatus, cDefIPVer, cPID, szSetup, 
			cOwn, ucLMT_RT, ucOOS_LMT_RT, uiUsedSes, uiAvailSes, uiTotalSes, 
			ucML_BLK, ucF_BLK, uiTotalRsc,
			cSRTP, cRemoteNATCnt, cChkIPPort, cChkPT, cChkSeq, cChkTS, cChgPT, 
			cUseAuth, szUseEnc, cEncKey, cOMRAdd, cUseImgAttr, cChkCodec, cCodecList);
   	return pBuf;
   }

} st_tg_route, *pst_tg_route;

typedef struct st_tg_bd : public st_tg_base 
{
	unsigned char ucNumAgent;
	unsigned char ucMode;
	unsigned char ucModeStatus;
	unsigned char ucRecoverBoard; // not use
	unsigned char ucLastRecoverBoard;
	unsigned char ucPairBoardIndex;
	unsigned char ucReserved[2];

	int nRatio;
	int nReserved;

	unsigned int uiBusyCnt;
	unsigned int uiMaxSession;

	//190903 R132 TC PKG by hak.
	unsigned int uiTcBusyCnt;
	unsigned int uiTcMaxSession;

   char* GetStrMode(char *pBuf, int nLen)
   {
      std::string strTemp="";

      if (ucMode & e_bd_mode_active)
         strTemp = "ACT";

      if (ucMode & e_bd_mode_standby)
         strTemp = "SB";

      if (ucMode & e_bd_mode_standalone)
         strTemp = "RD";

      if (ucMode == e_bd_mode_none)
         strTemp = "NONE";

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

	char* str(char *pBuf, int nLen) 
	{
		char szStatus[128];
		char szMode[128];
		char szModeStatus[128];

		GetStrStatus(szStatus, sizeof(szStatus));
		GetStrMode(szMode, sizeof(szMode));
		GetStrModeStatus(szModeStatus, sizeof(szModeStatus));

		snprintf(pBuf, nLen-1,
			"[Board Info]\n"
			"\tID                : %02d\n"
			"\tNAME              : %s\n"
			"\tENABLE            : %d\n"
			"\tVALID             : %d\n"
			"\tSTATUS            : %s(0x%x)\n"
			"\tNUM AGENT         : %d\n"
			"\tMODE              : %s\n"
			"\tMODE STATUS       : %s\n"
			"\tRATIO             : %d\n"
			"\tPAIR BOARD INDDEX : %d\n"
			"\tUSAGE             : %d/%d\n"
			"\tTC                  : %u/%u\n"
			, ucID, szName, ucEnabled, ucValid, szStatus, ucStatus, 
			ucNumAgent, szMode, szModeStatus ,nRatio, ucPairBoardIndex,
			uiBusyCnt, uiMaxSession, uiTcBusyCnt, uiTcMaxSession);
		return pBuf;
	}

	char* str2(char *pBuf, int nLen) 
	{
		char szStatus[128];
		char szMode[128];
		char szModeStatus[128];

		GetStrStatus(szStatus, sizeof(szStatus));
		GetStrMode(szMode, sizeof(szMode));
		GetStrModeStatus(szModeStatus, sizeof(szModeStatus));

		snprintf(pBuf, nLen-1,
			"[Board %d]\n"
			"\tNAME:%s VALID:%d STATUS:%s(0x%x) NUM_AGENT:%d MODE:%s MODE_STATUS:%s\n"
			"\tRATIO:%d PAIR_BOARD_ID:%d USAGE:%d/%d TC:%u/%u\n"
			, ucID, szName, ucValid, szStatus, ucStatus, ucNumAgent, szMode, szModeStatus, 
			nRatio, ucPairBoardIndex, uiBusyCnt, uiMaxSession, uiTcBusyCnt, uiTcMaxSession);
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

	char* str2(char *pBuf, int nLen) 
	{
		char szStatus[128];
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));

	   char szAddr[64]; szAddr[0] = '\0';
		IP.str(szAddr, sizeof(szAddr));

		snprintf(pBuf, nLen-1,
			"[POOL B:%d P:%d ID:%d]\n"
			"\tNAME:%s VALID:%d STATUS:%s(0x%x) IPVER:%d IP:%s\n"
			"\tNIC:%s START_PORT:%d END_PORT:%d MAX_SES:%d\n"
			, ucBoardIndex, ucPoolIndex, ucID, szName, ucValid, szStatus, ucStatus, ucIPVer, szAddr,  
		   szNIC, uiStartPort, uiEndPort, uiMaxSession);
		return pBuf;
	}

} st_tg_pool, *pst_tg_pool;

typedef struct st_tg_agent : public st_tg_base 
{
	unsigned char ucBoardIndex;
	unsigned char ucReserved[3];
	unsigned int  uiMaxSession;

	char* str(char *pBuf, int nLen) 
	{
		char szStatus[128];
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));

		snprintf(pBuf, nLen-1,
			"[AGENT Info]\n"
			"\tBOARD ID          : %02d\n"
			"\tAGENT ID          : %02d\n"
			"\tNAME              : %s\n"
			"\tVALID             : %d\n"
			"\tSTATUS            : %s(0x%x)\n"
			"\tMAX SESSION       : %d\n"
			, ucBoardIndex, ucID, szName, ucValid, szStatus, ucStatus, uiMaxSession);  
   	return pBuf;
	}

	char* str2(char *pBuf, int nLen) 
	{
		char szStatus[128];
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));

		snprintf(pBuf, nLen-1,
			"B:%d A:%d VALID:%d STATUS:%s(0x%x) MAX:%d"
			, ucBoardIndex, ucID, ucValid, szStatus, ucStatus, uiMaxSession);  
   	return pBuf;
	}

} st_tg_agent, *pst_tg_agent;

// poolindex는 st_tg_pool에 있는 pool index와 맞아야 함
// st_tg_pool과 1:n 구조
typedef struct st_tg_rsc : public st_tg_base 
{
	unsigned char ucBoardIndex;
	unsigned char ucPoolIndex;
	unsigned char ucIPVer; // st_tg_pool 정보 참조. 자동 입력
	unsigned char ucRscIndex;
	unsigned char ucReserved[4];

	unsigned int uiTrte; // TRTE
   unsigned int uiRscId; // 사용안함.

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
			"\tRSC   INDEX       : %02d\n"
			"\tROUTE ID          : %02d\n"
			"\tBUSY COUNT        : %d\n"
			"\tMAX SESSION       : %d\n"
			"\tIP                : %s\n"
			, ucID, szName, ucEnabled, ucValid, szStatus, ucStatus,  
		   ucBoardIndex, ucPoolIndex, ucRscIndex, uiTrte, uiBusyCnt, uiMaxSession, szAddr);
   	return pBuf;
	}

	char* str2(char *pBuf, int nLen) 
	{
		char szStatus[128];
		memset(szStatus, 0, sizeof(szStatus));
		GetStrStatus(szStatus, sizeof(szStatus));

	   char szAddr[64]; szAddr[0] = '\0';
		IP.str(szAddr, sizeof(szAddr));

		snprintf(pBuf, nLen-1,
			"[RSC B:%d P:%d R:%d ID:%d]\n"
			"\tTRTE:%d NAME:%s VALID:%d STATUS:%s(0x%x) BUSY:%d MAX_SES:%d IP:%s\n"
			,ucBoardIndex, ucPoolIndex, ucRscIndex, ucID, uiTrte, szName, ucValid, szStatus, ucStatus, 
		   uiBusyCnt, uiMaxSession, szAddr);
   	return pBuf;
	}

} st_tg_rsc, *pst_tg_rsc;

struct st_tgrm_shm
{
	st_tg_route stRoute[EMG_MAX_TG_ROUTE]; // ROUTE INFO
   st_tg_bd stBoard[EMG_MAX_TG_BOARD]; // BOARD INFO
   st_tg_pool stPool[EMG_MAX_TG_BOARD][EMG_MAX_TG_POOL_PER_BOARD]; // Pool INFO 
   st_tg_rsc stRsc[EMG_MAX_TG_BOARD][EMG_MAX_TG_RSC_PER_BOARD]; // RESOURCE INFO
   st_tg_agent stAgent[EMG_MAX_TG_BOARD][EMG_MAX_TG_TGA_PER_BOARD]; // TGA
	unsigned char ucSesStatus[EMG_MAX_TG_BOARD][EMG_MAX_TG_TGA_PER_BOARD][EMG_MAX_TG_SESSION_PER_TGA]; 
};

#define TGRM_SHM_PATH     "../ipc/shm/tgrm_wshm"

}

#endif
