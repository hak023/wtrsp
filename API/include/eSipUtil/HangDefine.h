#ifndef HANGDEF_H
#define HANGDEF_H
#include <time.h>
/******************************* PKG   Defined *******************************************/
#define DEF_HANG_R130
/******************************* Alarm Defined *******************************************/
/*
RTTError=83,
SIPReasonOverflow,
ProcessHangUp = 85,       // Ãß°¡
} ALARM_TYPE, *PALARM_TYPE;
*/
#define DEF_ALARM_ProcessHangUp    86
/******************************* Shared Memory Defined ************************************/
#define DEF_MAX_MONITOR    100
typedef struct QueueThreadInfo_t
{
	char m_szThreadName[32];
	unsigned int m_unThreadId;
	struct timespec m_stReqT;
	struct timespec m_stRspT;
	unsigned int m_unCurrentQ;
	unsigned int m_unMaxQ;
}QueueThreadInfo_t;
typedef struct QueueMonitorInfo_t
{
	char m_szProcName[32];
	unsigned int m_unPid;
	struct timespec m_stBeginT;
	unsigned int m_unCurThreadNum;
	QueueThreadInfo_t m_stInfo[DEF_MAX_MONITOR];
}QueueMonitorInfo_t;
/******************************* MMI Defined ********************************************/
//==========> Base Code
#define DEF_HANG_REQ_MSG_BASE    				0x01000000
#define DEF_HANG_RSP_MSG_BASE    				0x02000000
#define DEF_HANG_PDE_STACK_BASE  				0x10000000
#define DEF_HANG_CS_STACK_BASE    				0x20000000
#define DEF_HANG_EMP_STACK_BASE				0x30000000
#define DEF_HANG_SLB_STACK_BASE                       0x40000000
#define DEF_HANG_VLM_STACK_BASE 				0x50000000		// << for vIBCF
//==========> CM Code
#define DEF_CS_DIS_HANG_REQ   0x00001191
#define DEF_CS_DIS_HANG_RSP    0x00001191
#define DEF_CS_CHG_HANG_REQ  0x00001192
#define DEF_CS_CHG_HANG_RSP   0x00001192
#define DEF_STYPE_CS_DIS_HANG_REQ   DEF_HANG_CS_STACK_BASE | DEF_HANG_REQ_MSG_BASE |  DEF_CS_DIS_HANG_REQ    // 0x21001191
#define DEF_STYPE_CS_DIS_HANG_RSP    DEF_HANG_CS_STACK_BASE | DEF_HANG_RSP_MSG_BASE |  DEF_CS_DIS_HANG_RSP    // 0x22001191
#define DEF_STYPE_CS_CHG_HANG_REQ   DEF_HANG_CS_STACK_BASE | DEF_HANG_REQ_MSG_BASE |  DEF_CS_CHG_HANG_REQ    // 0x21001192
#define DEF_STYPE_CS_CHG_HANG_RSP    DEF_HANG_CS_STACK_BASE | DEF_HANG_RSP_MSG_BASE |  DEF_CS_CHG_HANG_REQ    // 0x22001192

#ifdef DEF_HANG_R130
#define DEF_CS_DIS_CCM_ST_REQ  0x00001201
#define DEF_CS_DIS_CCM_ST_RSP  0x00001201
#define DEF_STYPE_CS_DIS_CCM_ST_REQ   DEF_HANG_CS_STACK_BASE | DEF_HANG_REQ_MSG_BASE |  DEF_CS_DIS_CCM_ST_REQ    // 0x21001201
#define DEF_STYPE_CS_DIS_CCM_ST_RSP    DEF_HANG_CS_STACK_BASE | DEF_HANG_RSP_MSG_BASE |  DEF_CS_DIS_CCM_ST_RSP    // 0x22001201
//==========> SLB Code
#define DEF_CS_DIS_SLB_ST_REQ  0x00001211
#define DEF_CS_DIS_SLB_ST_RSP  0x00001211
#define DEF_STYPE_CS_DIS_SLB_ST_REQ   DEF_HANG_SLB_STACK_BASE | DEF_HANG_REQ_MSG_BASE |  DEF_CS_DIS_SLB_ST_REQ    // 0x41001211
#define DEF_STYPE_CS_DIS_SLB_ST_RSP    DEF_HANG_SLB_STACK_BASE | DEF_HANG_RSP_MSG_BASE |  DEF_CS_DIS_SLB_ST_RSP    // 0x42001211
//==========> PDE Code
#define DEF_PDE_DIS_PCDS_ST_REQ 0x00000043
#define DEF_PDE_DIS_PCDS_ST_RSP 0x00000043
#define DEF_STYPE_PDE_DIS_PCDS_ST_REQ   DEF_HANG_PDE_STACK_BASE | DEF_HANG_REQ_MSG_BASE |  DEF_PDE_DIS_PCDS_ST_REQ    // 0x11000043
#define DEF_STYPE_PDE_DIS_PCDS_ST_RSP    DEF_HANG_PDE_STACK_BASE | DEF_HANG_RSP_MSG_BASE |  DEF_PDE_DIS_PCDS_ST_RSP    // 0x12000043
#endif

//==========> EMP Code
#if 1
#define DEF_EMP_DIS_HANG_REQ  0x00000701
#define DEF_EMP_DIS_HANG_RSP   0x00000701
#define DEF_EMP_CHG_HANG_REQ  0x00000702
#define DEF_EMP_CHG_HANG_RSP   0x00000702
#define DEF_STYPE_EMP_DIS_HANG_REQ DEF_HANG_EMP_STACK_BASE | DEF_HANG_REQ_MSG_BASE | DEF_EMP_DIS_HANG_REQ    //  0x31000701
#define DEF_STYPE_EMP_DIS_HANG_RSP  DEF_HANG_EMP_STACK_BASE | DEF_HANG_RSP_MSG_BASE | DEF_EMP_DIS_HANG_RSP   // 0x32000701
#define DEF_STYPE_EMP_CHG_HANG_REQ DEF_HANG_EMP_STACK_BASE | DEF_HANG_REQ_MSG_BASE | DEF_EMP_CHG_HANG_REQ  // 0x31000702
#define DEF_STYPE_EMP_CHG_HANG_RSP  DEF_HANG_EMP_STACK_BASE | DEF_HANG_RSP_MSG_BASE | DEF_EMP_CHG_HANG_RSP   // 0x32000702
#else
#define DEF_STYPE_EMP_DIS_HANG_REQ 0x00000701    
#define DEF_STYPE_EMP_DIS_HANG_RSP  0x00000702   
#define DEF_STYPE_EMP_CHG_HANG_REQ 0x00000703  
#define DEF_STYPE_EMP_CHG_HANG_RSP  0x00000704 
#endif
#ifdef DEF_HANG_R130
#define DEF_EMP_DIS_TGAS_ST_REQ 0x00000711
#define DEF_EMP_DIS_TGAS_ST_RSP 0x00000712
#define DEF_STYPE_EMP_DIS_TGAS_ST_REQ DEF_HANG_EMP_STACK_BASE | DEF_HANG_REQ_MSG_BASE | DEF_EMP_DIS_TGAS_ST_REQ    //  0x31000711
#define DEF_STYPE_EMP_DIS_TGAS_ST_RSP DEF_HANG_EMP_STACK_BASE | DEF_HANG_REQ_MSG_BASE | DEF_EMP_DIS_TGAS_ST_RSP    //  0x31000712
#endif
//==========> COMMON Code		<< for vIBCF
#define DEF_VLM_DIS_HANG_REQ  0x00000051
#define DEF_VLM_DIS_HANG_RSP   0x00000051
#define DEF_VLM_CHG_HANG_REQ  0x00000052
#define DEF_VLM_CHG_HANG_RSP   0x00000052
#define DEF_STYPE_VLM_DIS_HANG_REQ DEF_HANG_VLM_STACK_BASE | DEF_HANG_REQ_MSG_BASE | DEF_VLM_DIS_HANG_REQ			// 0x51000051
#define DEF_STYPE_VLM_DIS_HANG_RSP  DEF_HANG_VLM_STACK_BASE | DEF_HANG_RSP_MSG_BASE | DEF_VLM_DIS_HANG_RSP			// 0x52000051
#define DEF_STYPE_VLM_CHG_HANG_REQ DEF_HANG_VLM_STACK_BASE | DEF_HANG_REQ_MSG_BASE | DEF_VLM_CHG_HANG_REQ		// 0x51000052
#define DEF_STYPE_VLM_CHG_HANG_RSP  DEF_HANG_VLM_STACK_BASE | DEF_HANG_RSP_MSG_BASE | DEF_VLM_CHG_HANG_RSP		// 0x52000052
//==========> Struct
typedef struct DogInfo_t
{
	int m_nEnableMonitor;
	int m_nDogTime;
	int m_nDogAlarmCnt;
	int m_nEnableHA;
	int m_nEnableReset;
   #ifdef DEF_HANG_R130
   int m_nStatInterval; //millisecond
   int m_nMinorLevel;   // 60 %
   int m_nMajorLevel;   // 70 %
   int m_nCriticalLevel;  // 80 %
   int m_nAlarmPeriod;    // sec
   int m_nAlarmClearT;    // sec
   #endif
}DogInfo_t;
typedef struct CmDisDogReq_t
{
	DogInfo_t m_stInfo;
}CmDisDogReq_t;
typedef struct CmDisDogRsp_t
{
	int m_nResult;
	int m_nReason;
	char m_szDesc[64];
	DogInfo_t m_stInfo;
}CmDisDogRsp_t;
typedef CmDisDogReq_t CmChgDogReq_t; 
typedef CmDisDogRsp_t CmChgDogRsp_t; 
typedef CmDisDogReq_t EmpDisDogReq_t; 
typedef CmDisDogRsp_t EmpDisDogRsp_t; 
typedef CmDisDogReq_t EmpChgDogReq_t; 
typedef CmDisDogRsp_t EmpChgDogRsp_t;

#ifdef DEF_HANG_R130
typedef struct ResourceReq_t
{
	int m_nReq;
}ResourceReq_t;
typedef struct ResourceRsp_t
{
	int m_nResult;
	int m_nReason;
	char m_szDesc[64];
	QueueMonitorInfo_t m_stInfo;
}ResourceRsp_t;
typedef ResourceReq_t SlbResourceReq_t;
typedef ResourceReq_t CcmResourceReq_t;
typedef ResourceReq_t PcdsResourceReq_t;
typedef ResourceReq_t TgasResourceReq_t;
typedef ResourceRsp_t SlbResourceRsp_t;
typedef ResourceRsp_t CcmResourceRsp_t;
typedef ResourceRsp_t PcdsResourceRsp_t;
typedef ResourceRsp_t TgasResourceRsp_t;
#endif
/************************************ Restart Cmd Defined **********************************/
/*
#define MSG_FUNC_CTRL 0x0700
#define MSG_SUBF_CTRL_PROCESS_RESTART  0x0703
-------------------------------------------------------------------------
typedef struct {
char  szProcName[32];
} OMCPROCCONTROL, *POMCPROCCONTROL;
-------------------------------------------------------------------------
typedef struct {
XTMMSGHDR  h;
OMCPROCCONTROL Proc;
} OMCMSGPROCCONTROL, *POMCMSGPROCCONTROL;
-------------------------------------------------------------------------
pReq->h.usType = MSG_FUNC_CTRL;
pReq->h.usSubType = MSG_SUBF_CTRL_PROCESS_RESTART;
pReq->Proc.szProcName = "CCM";
*/
/********************************** HA Deact Cmd Defined **********************************/
/*
#define MSG_FUNC_CTRL 0x0700
#define MSG_SUBF_CTRL_HA_DEACT  0x0742
---------------------------------------------------------------?
typedef struct {
XTMMSGHDR h;
} OMCMSGREQUEST, *POMCMSGREQUEST;
----------------------------------------------------------------
pReq->h.usType = MSG_FUNC_CTRL;
pReq->h.usSubType = MSG_SUBF_CTRL_HA_DEACT;
*/
#endif
