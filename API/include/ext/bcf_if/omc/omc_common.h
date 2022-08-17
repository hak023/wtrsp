#ifndef __OMC_COMMON_H__
#define __OMC_COMMON_H__

#include "omc.h"

//defines path
#define IPC_UUDP_OMC                      "../ipc/udp"
#define IPC_UUDP_OMC_ALARM IPC_UUDP_OMC   "/oma"
#define IPC_UUDP_OMC_TRACE IPC_UUDP_OMC   "/oma_trace"


//control pnr
typedef OMCMSGPROCCONTROL  OMCPROCMGR;
typedef POMCMSGPROCCONTROL POMCPROCMGR;
//control alarm msg
//typedef OMCMSGALARM OMCALARM;
//typedef POMCMSGALARM POMCALARM;
typedef struct {
   XTMMSGHDR     h;
   VMSIVRLINE      li;
   OMCALARMINFO  alarm; //OMCALARMINFO Alarm
} OMCALARM, *POMCALARM;

//defines message type
//=========================================================
// h.usType
//=========================================================
#define MSG_FUNC_CONTROL               MSG_FUNC_CTRL //process contrl: 0x0700
#define MSG_FUNC_ALARM                 0x0500 //alarm
//defines omc.h
//#define MSG_FUNC_TRACE                 0x0800 //trace

#define MSG_FUNC_MAINT_TGAS            0x0400 // MMI TGAS 
#define MSG_FUNC_MAINT_TGRM            0x0600 // MMI TGRM
#define MSG_FUNC_MAINT_MSRPRM          0x0700 // MMI MSRPRM

//defines message sub-type
//=========================================================
// h.usSubType
//=========================================================
#define MSG_SUBF_PROCESS_RESTART       MSG_SUBF_CTRL_PROCESS_RESTART // process restart:0x0703
#define MSG_SUBF_PROCESS_START         MSG_SUBF_CTRL_PROCESS_START // process start:0x0704
#define MSG_SUBF_PROCESS_KILL          MSG_SUBF_CTRL_PROCESS_KILL // process kill: 0x0705

#define MSG_SUBF_ALARM_NOTIFY          0x0502 // alarm notify sub-type
//defines omc.h
//#define MSG_SUBF_TRACE_LOG             0x0C11 // trace sub-type

// TGRM
#define MSG_SUBF_RTE_BLK_REQ           0x0601 // CLI -> TGRM,MSRPRM
#define MSG_SUBF_RTE_BLK_RES           0x0602 // CLI <- TGRM,MSRPRM
#define MSG_SUBF_RTE_UNBLK_REQ         0x0603 // CLI -> TGRM,MSRPRM
#define MSG_SUBF_RTE_UNBLK_RES         0x0604 // CLI <- TGRM,MSRPRM

#define MSG_SUBF_BD_BLK_REQ            0x0611 // CLI -> TGRM,MSRPRM
#define MSG_SUBF_BD_BLK_RES            0x0612 // CLI <- TGRM,MSRPRM
#define MSG_SUBF_BD_UNBLK_REQ          0x0613 // CLI -> TGRM,MSRPRM
#define MSG_SUBF_BD_UNBLK_RES          0x0614 // CLI <- TGRM,MSRPRM

#define MSG_SUBF_POOL_BLK_REQ          0x0621 // CLI -> TGRM,MSRPRM
#define MSG_SUBF_POOL_BLK_RES          0x0622 // CLI <- TGRM,MSRPRM
#define MSG_SUBF_POOL_UNBLK_REQ        0x0623 // CLI -> TGRM,MSRPRM
#define MSG_SUBF_POOL_UNBLK_RES        0x0624 // CLI <- TGRM,MSRPRM

#define MSG_SUBF_RSC_BLK_REQ           0x0631 // CLI -> TGRM
#define MSG_SUBF_RSC_BLK_RES           0x0632 // CLI <- TGRM
#define MSG_SUBF_RSC_UNBLK_REQ         0x0633 // CLI -> TGRM
#define MSG_SUBF_RSC_UNBLK_RES         0x0634 // CLI <- TGRM

#define MSG_SUBF_BD_SWITCH_REQ         0x0641 // CLI -> TGRM,MSRPRM
#define MSG_SUBF_BD_SWITCH_RES         0x0642 // CLI <- TGRM,MSRPRM
#define MSG_SUBF_BD_CLEAR_REQ          0x0643 // CLI -> TGRM
#define MSG_SUBF_BD_CLEAR_RES          0x0644 // CLI <- TGRM

#define MSG_SUBF_RTE_ADD_REQ           0x0651 // CLI -> TGRM,MSRPRM
#define MSG_SUBF_RTE_ADD_RES           0x0652 // CLI <- TGRM,MSRPRM
#define MSG_SUBF_RTE_MOD_REQ           0x0653 // CLI -> TGRM,MSRPRM
#define MSG_SUBF_RTE_MOD_RES           0x0654 // CLI <- TGRM,MSRPRM
#define MSG_SUBF_RTE_DEL_REQ           0x0655 // CLI -> TGRM,MSRPRM
#define MSG_SUBF_RTE_DEL_RES           0x0656 // CLI <- TGRM,MSRPRM

#define MSG_SUBF_BD_ADD_REQ            0x0661 // CLI -> TGRM
#define MSG_SUBF_BD_ADD_RES            0x0662 // CLI <- TGRM
#define MSG_SUBF_BD_MOD_REQ            0x0663 // CLI -> TGRM
#define MSG_SUBF_BD_MOD_RES            0x0664 // CLI <- TGRM
#define MSG_SUBF_BD_DEL_REQ            0x0665 // CLI -> TGRM
#define MSG_SUBF_BD_DEL_RES            0x0666 // CLI <- TGRM

#define MSG_SUBF_POOL_ADD_REQ          0x0671 // CLI -> TGRM
#define MSG_SUBF_POOL_ADD_RES          0x0672 // CLI <- TGRM
#define MSG_SUBF_POOL_MOD_REQ          0x0673 // CLI -> TGRM
#define MSG_SUBF_POOL_MOD_RES          0x0674 // CLI <- TGRM
#define MSG_SUBF_POOL_DEL_REQ          0x0675 // CLI -> TGRM
#define MSG_SUBF_POOL_DEL_RES          0x0676 // CLI <- TGRM

#define MSG_SUBF_RSC_ADD_REQ           0x0681 // CLI -> TGRM
#define MSG_SUBF_RSC_ADD_RES           0x0682 // CLI <- TGRM
#define MSG_SUBF_RSC_MOD_REQ           0x0683 // CLI -> TGRM
#define MSG_SUBF_RSC_MOD_RES           0x0684 // CLI <- TGRM
#define MSG_SUBF_RSC_DEL_REQ           0x0685 // CLI -> TGRM
#define MSG_SUBF_RSC_DEL_RES           0x0686 // CLI <- TGRM

#define MSG_SUBF_SDP_PTGR_ADD_REQ      0x0411 // CLI -> TGAS
#define MSG_SUBF_SDP_PTGR_ADD_RES      0x0412 // CLI <- TGAS
#define MSG_SUBF_SDP_PTGR_DEL_REQ      0x0413 // CLI -> TGAS
#define MSG_SUBF_SDP_PTGR_DEL_RES      0x0414 // CLI <- TGAS
#define MSG_SUBF_SDP_PTGR_MOD_REQ      0x0415 // CLI -> TGAS
#define MSG_SUBF_SDP_PTGR_MOD_RES      0x0416 // CLI <- TGAS

#define MSG_SUBF_SDP_CODEC_ADD_REQ     0x0421 // CLI -> TGAS
#define MSG_SUBF_SDP_CODEC_ADD_RES     0x0422 // CLI <- TGAS
#define MSG_SUBF_SDP_CODEC_DEL_REQ     0x0423 // CLI -> TGAS
#define MSG_SUBF_SDP_CODEC_DEL_RES     0x0424 // CLI <- TGAS
#define MSG_SUBF_SDP_CODEC_MOD_REQ     0x0425 // CLI -> TGAS
#define MSG_SUBF_SDP_CODEC_MOD_RES     0x0426 // CLI <- TGAS

#define MSG_SUBF_SDP_RULE_ADD_REQ       0x0431 // CLI -> TGAS
#define MSG_SUBF_SDP_RULE_ADD_RES       0x0432 // CLI <- TGAS
#define MSG_SUBF_SDP_RULE_DEL_REQ       0x0433 // CLI -> TGAS
#define MSG_SUBF_SDP_RULE_DEL_RES       0x0434 // CLI <- TGAS
#define MSG_SUBF_SDP_RULE_MOD_REQ       0x0435 // CLI -> TGAS
#define MSG_SUBF_SDP_RULE_MOD_RES       0x0436 // CLI <- TGAS

//by hak. for R104 PKG 150903 Start.
#define MSG_SUBF_SDP_MANIPULATION_ADD_REQ       0x0441 // CLI -> TGAS
#define MSG_SUBF_SDP_MANIPULATION_ADD_RES       0x0442 // CLI <- TGAS
#define MSG_SUBF_SDP_MANIPULATION_DEL_REQ       0x0443 // CLI -> TGAS
#define MSG_SUBF_SDP_MANIPULATION_DEL_RES       0x0444 // CLI <- TGAS
#define MSG_SUBF_SDP_MANIPULATION_MOD_REQ       0x0445 // CLI -> TGAS
#define MSG_SUBF_SDP_MANIPULATION_MOD_RES       0x0446 // CLI <- TGAS

#define MSG_SUBF_SDP_UA_RULE_ADD_REQ       0x0451 // CLI -> TGAS
#define MSG_SUBF_SDP_UA_RULE_ADD_RES       0x0452 // CLI <- TGAS
#define MSG_SUBF_SDP_UA_RULE_DEL_REQ       0x0453 // CLI -> TGAS
#define MSG_SUBF_SDP_UA_RULE_DEL_RES       0x0454 // CLI <- TGAS
#define MSG_SUBF_SDP_UA_RULE_MOD_REQ       0x0455 // CLI -> TGAS
#define MSG_SUBF_SDP_UA_RULE_MOD_RES       0x0456 // CLI <- TGAS
//by hak. for R104 PKG 150903 End.

//by hak. for R126 PKG 180703 Start.
#define MSG_SUBF_SDP_CODEC_MANAGER_ADD_REQ       0x0461 // CLI -> TGAS
#define MSG_SUBF_SDP_CODEC_MANAGER_ADD_RES       0x0462 // CLI <- TGAS
#define MSG_SUBF_SDP_CODEC_MANAGER_DEL_REQ       0x0463 // CLI -> TGAS
#define MSG_SUBF_SDP_CODEC_MANAGER_DEL_RES       0x0464 // CLI <- TGAS
#define MSG_SUBF_SDP_CODEC_MANAGER_MOD_REQ       0x0465 // CLI -> TGAS
#define MSG_SUBF_SDP_CODEC_MANAGER_MOD_RES       0x0466 // CLI <- TGAS
//by hak. for R126 PKG 180703 End.

#endif

