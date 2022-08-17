#ifndef IBCFEATURE_H
#define IBCFEATURE_H
#include "ESipDefine.hxx"
#define DEF_ENABLE_IBC
#define DEF_RTE_LIB

/******************************* R 110 Package *******************************************/
#define DEF_R120
#define DEF_FAULT_MSG
#define DEF_CDR_REASON
#define DEF_SUBSCRIBE_ICID_BUG_FIXED
#define DEF_FLEXIBLE_CHARGE_CONFIG
/**************************************************************************************/

/***************************** change com transport mgr util **********************************/
#define DEF_TRGW_TRANSPORT_CHANGED     // RUTCPSERVER -> TRANSPORT MGR
#define DEF_PDE_TRANSPORT_CHANGED
/**************************************************************************************/
#define DEF_CDR_REASON_BUG_FIXED   // 20170629
//#define FEATURE_vIBC

/******************************* R 125 Package *******************************************/
#define DEF_R125
//#define DEF_COM_CONFIG
/******************************* vibc package ********************************************/
#define DEF_VIBC_ENABLE
#define DEF_TRGW_RETRANS
#define DEF_PDE_RETRANS
#define DEF_VIBC_CUSTOMIZE

#define DEF_R126    // sdp as filed minipulation
#define DEF_R130    // resource monitor

#ifdef DEF_IBC_R131
#define DEF_R131    // enum proc
#endif
#define DEF_R132   // modify charging logic
/**************************************************************************************/

#endif
