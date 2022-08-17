#ifndef __MSRP_SHM_H__
#define __MSRP_SHM_H__

#include <string>
#include "tg_shm.h"
#include "msrp_msg.h"

namespace MSRP
{

enum {
	E_MAX_MSRP_BOARD = TGRM::EMG_MAX_TG_BOARD, //8
	E_MAX_MSRP_MSRPA_PER_BOARD = 1,
	E_MAX_MSRP_SESSION_PER_MSRPA = 50000,
	E_MAX_MSRP_TRACE = 10,
	E_MAX_MSRP_RM_SESSION = E_MAX_MSRP_BOARD * E_MAX_MSRP_MSRPA_PER_BOARD * E_MAX_MSRP_SESSION_PER_MSRPA, // 50000*10
};


struct st_msrprm_shm{
	TGRM::st_tg_route stRoute[TGRM::EMG_MAX_TG_ROUTE];
   TGRM::st_tg_bd stBoard[TGRM::EMG_MAX_TG_BOARD]; // max 10
   TGRM::st_tg_agent stAgent[E_MAX_MSRP_BOARD][E_MAX_MSRP_MSRPA_PER_BOARD]; 
	TGRM::st_tg_pool stPool[TGRM::EMG_MAX_TG_BOARD][TGRM::EMG_MAX_TG_POOL_PER_BOARD];
	TGRM::st_tg_rsc stRsc[E_MAX_MSRP_BOARD][TGRM::EMG_MAX_TG_RSC_PER_BOARD];

   unsigned char ucSesStatus[E_MAX_MSRP_BOARD][E_MAX_MSRP_MSRPA_PER_BOARD][E_MAX_MSRP_SESSION_PER_MSRPA]; 
};

#define MSRPRM_SHM_PATH         "../ipc/shm/msrprm_wshm"

} // name space MSRP
#endif
