#ifndef __SS7_CONTROL_H__
#define __SS7_CONTROL_H__

#include "eventdefs.h"

#define S7_MAX_CHAN                   32

#define S7_MAX_MTP2_LINK_NAME         32
#define S7_MAX_MTP3_LINK_NAME         32
#define S7_MAX_MTP3_LINKSET_NAME      S7_MAX_MTP3_LINK_NAME
#define S7_MAX_MTP3_ROUTE_NAME        S7_MAX_MTP3_LINK_NAME
#define S7_MAX_ISUP_CIRCUIT_NAME      32
#define S7_MAX_ISUP_INTERFACE_NAME    32

#define S7_MAX_MTP2_LINK              64
#define S7_MAX_MTP3_LINK              64
#define S7_MAX_MTP3_LINKSET           64
#define S7_MAX_MTP3_ROUTE             64

#define S7_MAX_ISUP_CIRCUIT           320
#define S7_MAX_ISUP_CICMAP            S7_MAX_ISUP_CIRCUIT
#define S7_MAX_ISUP_INTERFACE         64

#define S7_ISUP_BLOCK_IDLE            0
#define S7_ISUP_BLOCK_LOCAL           1
#define S7_ISUP_BLOCK_REMOTE          2
#define S7_ISUP_BLOCK_LOCALREMOTE     3

////////////////////////////////////////////
// define main message about SS7 (usType)
////////////////////////////////////////////
#define S7_MSG_FUNC_CALL              0x0001
#define S7_MSG_FUNC_CTRL              0x0002
#define S7_MSG_FUNC_CONF              0x0003
#define S7_MSG_FUNC_INFO              0x0004

////////////////////////////////////////////
// define sub message about SS7 (usSubType)
////////////////////////////////////////////

/* SS7_MSG_FUNC_CONFIG */
#define S7_MSG_SUBF_LINK_GET          0x0001
#define S7_MSG_SUBF_LINK_INFO         0x0002
#define S7_MSG_SUBF_LINK_SET          0x0003
#define S7_MSG_SUBF_LINK_ADD          0x0004
#define S7_MSG_SUBF_LINK_DEL          0x0005
#define S7_MSG_SUBF_LINK_STAT         0x0006
#define S7_MSG_SUBF_LINK_STATE_SET    0x0007

#define S7_MSG_SUBF_LINKSET_GET       0x0011
#define S7_MSG_SUBF_LINKSET_INFO      0x0012
#define S7_MSG_SUBF_LINKSET_SET       0x0013
#define S7_MSG_SUBF_LINKSET_ADD       0x0014
#define S7_MSG_SUBF_LINKSET_DEL       0x0015
#define S7_MSG_SUBF_LINKSET_STATE_SET 0x0017

#define S7_MSG_SUBF_ROUTE_GET         0x0021
#define S7_MSG_SUBF_ROUTE_INFO        0x0022
#define S7_MSG_SUBF_ROUTE_SET         0x0023
#define S7_MSG_SUBF_ROUTE_ADD         0x0024
#define S7_MSG_SUBF_ROUTE_DEL         0x0025

#define S7_MSG_SUBF_CIC_GET           0x0031
#define S7_MSG_SUBF_CIC_INFO          0x0032
#define S7_MSG_SUBF_CIC_SET           0x0033
#define S7_MSG_SUBF_CIC_ADD           0x0034
#define S7_MSG_SUBF_CIC_DEL           0x0035

/* CIC-CHANNEL MAP */
#define S7_MSG_SUBF_CICMAP_GET        0x0041
#define S7_MSG_SUBF_CICMAP_INFO       0x0042

#define S7_MSG_SUBF_ISUPIF_GET        0x0051
#define S7_MSG_SUBF_ISUPIF_INFO       0x0052

/* mtp2 link */
#define S7_MSG_SUBF_MTP2LINK_GET       0x0061
#define S7_MSG_SUBF_MTP2LINK_INFO      0x0062
#define S7_MSG_SUBF_MTP2LINK_STATE_SET 0x0067

#define S7_MSG_SUBF_MASTER_GET        0x0071  // REQUEST
#define S7_MSG_SUBF_MASTER_INFO       0x0072  // RESPONSE

/* SS7_MSG_FUNC_CONTROL */
#define S7_MSG_SUBF_RELOAD_CFG_REQ    0x0001
#define S7_MSG_SUBF_RELOAD_CFG_RSP    0x1001
#define S7_MSG_SUBF_ADAPTER_RESET_REQ 0x0002
#define S7_MSG_SUBF_ADAPTER_RESET_RSP 0x1002
#define S7_MSG_SUBF_RESTORE_CFG       0x0003

#define S7_MSG_SUBF_M_BLOCK           0x0011
#define S7_MSG_SUBF_M_UNBLOCK         0x0012
#define S7_MSG_SUBF_M_GBLOCK          0x0013
#define S7_MSG_SUBF_M_GUNBLOCK        0x0014
#define S7_MSG_SUBF_H_BLOCK           0x0015
#define S7_MSG_SUBF_H_UNBLOCK         0x0016
#define S7_MSG_SUBF_M_RESET           0x0017
#define S7_MSG_SUBF_M_GRESET          0x0018

#define S7_MSG_SUBF_M_BLOCK_ACK       0x1100
#define S7_MSG_SUBF_M_UNBLOCK_ACK     0x1200
#define S7_MSG_SUBF_M_GBLOCK_ACK      0x1300
#define S7_MSG_SUBF_M_GUNBLOCK_ACK    0x1400
#define S7_MSG_SUBF_H_BLOCK_ACK       0x1500
#define S7_MSG_SUBF_H_UNBLOCK_ACK     0x1600
#define S7_MSG_SUBF_RESET_ACK         0x1700
#define S7_MSG_SUBF_GRESET_ACK        0x1800

///////////////////////////////////
// define structure
///////////////////////////////////

//////////////////////////////////////////////////////
// MTP2 LINK INFO
//////////////////////////////////////////////////////
typedef struct {
   unsigned int   hMtp2Link;
   char           szMtp2LinkName[S7_MAX_MTP2_LINK_NAME];
   unsigned int   uidMtp2Link;     // MTP2LINK HANDLE
   unsigned char  Mode;            // 1:STANDALONE, 2: MTP3, 3:HSL STANDALONE, 4: HSL MTP3
   unsigned char  Protocol;        // 1:ITU88, 2:ITU92, 3:ANSI88, 4:ANSI92, 5:TTC, 6:NTT
   unsigned char  DPCLength;       // 1:14BITS, 2:16BITS, 3:24BITS
   unsigned char  Timeslot;        // 1:64KPBS, 2:56KPBS, 3:48KPBS
   unsigned int   hTrunk;          // LSM(Line Service Manager) HANDLE
   unsigned int   hMtp2;           // MTP2 HANDLE
} SS7MTP2LINKCONF, *PSS7MTP2LINKCONF;

typedef struct {
   int             nCount;         // MTP2LINK COUNT
   SS7MTP2LINKCONF Mtp2LinkConf[S7_MAX_MTP2_LINK];
} SS7MTP2LINKINFO, *PSS7MTP2LINKINFO; 

typedef struct {
   XTMMSGHDR       h;
   SS7MTP2LINKINFO Mtp2LinkInfo;
} SS7MSGMTP2LINK, *PSS7MSGMTP2LINK;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// MTP3 LINK INFO
//////////////////////////////////////////////////////
typedef struct {
   unsigned int   hLink;            // LINK HANDLE
   unsigned int   hMtp2Link;        // MTP2LINK HANDLE
   unsigned int   hLinkset;         // LINKSET HANDLE
   char           szLinkName[S7_MAX_MTP3_LINK_NAME];
   char           szMtp2LinkName[S7_MAX_MTP2_LINK_NAME];
   unsigned int   uidMtp3Link;      // MTP3LINK ID
   unsigned int   uidMtp2Link;      // MTP2LINK ID
   unsigned int   unMaxFrameLength;
   unsigned char  LinkPriority;     // 1:PRIORITY 0, 2:PRIORITY 1, 3:PRIORITY 2, 4:PRIORITY 3
   unsigned char  MessagePriority;  // 1:NOT USED, 2:PRIORITY 0, 3:PRIORITY 1, 4:PRIORITY 2, 5:PRIORITY 3
   unsigned char  bIsA_C_link;
   unsigned char  DiscardPriority;  // 1:PRIORITY 0, 2:PRIORITY 1, 3:PRIORITY 2, 4:PRIORITY 3
   unsigned int   unMaxSLTMRetry;
   unsigned char  LinkTestSLC;      // SLC
   unsigned char  bFlushContinueFlags;
   unsigned char  unSysId;          // SS7 id included MTP2LINK (1:SS701, 2:SS702)
   unsigned char  Timeslot;
} SS7LINKCONF, *PSS7LINKCONF;

typedef struct {
   unsigned char  LinkState;        // 1:UP, 2:DOWN, 3:NOT AVAILABLE
   unsigned char  bLocallyInhibited;
   unsigned char  bRemotelyInhibited;
   unsigned char  bLocallyBlocked;
   unsigned char  bRemotelyBlocked;
   unsigned char  bCongested;
   unsigned char  bEmergencyIndication;
   unsigned char  CongestionPriority; // 1:PRIORITY 0, 2:PRIORITY 1, 3:PRIORITY 2, 4:PRIORITY 3
   unsigned int   unRTxQueueSize;
   unsigned int   unTxQueueSize;
} SS7LINKSTATE, *PSS7LINKSTATE;

typedef struct {
   int            nCount;
   SS7LINKCONF    LinkConf[S7_MAX_MTP3_LINK];
   SS7LINKSTATE   LinkState[S7_MAX_MTP3_LINK];
} SS7LINKINFO, *PSS7LINKINFO;

typedef struct {
   XTMMSGHDR      h;
   SS7LINKINFO    LinkInfo;
} SS7MSGLINK, *PSS7MSGLINK;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// MTP3LINK STATE MODIFY
//////////////////////////////////////////////////////
typedef struct {
   unsigned int  hLink;  // LINK HANDLE
   unsigned char bLocallyInhibited; 
   unsigned char _reserved[3];
} SS7LINKSTATEINFO, *PSS7LINKSTATEINFO;

typedef struct {
   XTMMSGHDR     h;
   SS7LINKSTATEINFO LinkStateInfo;
} SS7MSGLINKSTATE, *PSS7MSGLINKSTATE;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// MTP2LINK STATE MODIFY
//////////////////////////////////////////////////////
typedef struct {
   unsigned int  hMtp2Link;                 // MTP2LINK HANDLE
   unsigned char LocalCongestionState;      // 1:NO CHANGE, 2:ON, 3:OFF
   unsigned char DatalinkState;             // 1:NO CHANGE, 2:ENABLE, 3:DISABLE
   unsigned char AlarmGenerationState;      // 1:NO CHANGE, 2:ACTIVATE, 3:DEACTIVATE
   unsigned char LocalProcessorOutageState; // NO CHANGE, 2:ACTIVATE, 3:DEACTIVATE
} SS7MTP2LINKSTATEINFO, *PSS7MTP2LINKSTATEINFO;

typedef struct {
   XTMMSGHDR            h;
   SS7MTP2LINKSTATEINFO Mtp2LinkStateInfo;
} SS7MSGMTP2LINKSTATE, *PSS7MSGMTP2LINKSTATE;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// MTP3 LINKSET INFO
//////////////////////////////////////////////////////
typedef struct {
   unsigned int   hLinkset;  // LINKSET HANDLE
   char           szLinksetName[S7_MAX_MTP3_LINKSET_NAME];
   unsigned int   unSwitchId;
   unsigned int   unOpc;
   unsigned int   unAdjDpc;
   unsigned int   unNbActiveLinks;
} SS7LINKSETCONF, *PSS7LINKSETCONF;

typedef struct {
   unsigned char  LinksetStatus;  // 1:ACTIVE, 2:INACTIVE, 3:ABNORMAL, 4:HALF NORMARL, 5:NORMAL, 6:FULL NORMAL
   unsigned char  bTransmitProhibitState;
   unsigned char  bTransmitRestrictedState;
   unsigned char  _reserved[1];
} SS7LINKSETSTATE, *PSS7LINKSETSTATE;

typedef struct {
   int             nCount;
   SS7LINKSETCONF  LinksetConf[S7_MAX_MTP3_LINKSET];
   SS7LINKSETSTATE LinksetState[S7_MAX_MTP3_LINKSET];
} SS7LINKSETINFO, *PSS7LINKSETINFO;

typedef struct {
   XTMMSGHDR      h;
   SS7LINKSETINFO LinksetInfo;
} SS7MSGLINKSET, *PSS7MSGLINKSET;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// MTP3 LINKSET STATE MODIFY 
//////////////////////////////////////////////////////
typedef struct {
   unsigned int  hLinkset;  // LINKSET HANDLE
   unsigned int  hRoute;    // ROUTE HANDLE
   unsigned char bActive; 
   unsigned char _reserved[3];
} SS7LINKSETSTATEINFO, *PSS7LINKSETSTATEINFO;

typedef struct {
   XTMMSGHDR           h;
   SS7LINKSETSTATEINFO LinksetStateInfo;
} SS7MSGLINKSETSTATE, *PSS7MSGLINKSETSTATE;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// MTP3 ROUTE INFO
//////////////////////////////////////////////////////
typedef struct {
   unsigned int   hRoute;  // ROUTE HANDLE
   char           szRouteName[S7_MAX_MTP3_ROUTE_NAME];
   unsigned int   unPointCode;           // Direction == UP ? DPC : OPC
   unsigned char  SignalingPointType;    // 1:SP, 2:STP
   unsigned char  Direction;             // 1:UP, 2:DOWN
   unsigned char  bRouteToAdjacentSP;
   unsigned char  bBroadcast;
   unsigned char  RestartProcedure;      // 1:NONE, 2:ITU88, 3:ITU92, 4:ANSI
   unsigned char  SlsRange;              // 1:ITU, 2:ANSI 5BITS, 3:ANSI 8BIT, 4:NTT
   unsigned char  StpSlsLinksetSelector; // 1,2,4,8  Display to hexacode (ex: 0x01)
   unsigned char  bSupportsMultiMsgPriority;
   unsigned char  bSupportRouteSetCongestionTest; 
   unsigned char  _reserved[3];
   unsigned int   unNbLinkset;
   unsigned int   ahLinkset[S7_MAX_MTP3_LINKSET];
   unsigned char  aLinksetPriority[S7_MAX_MTP3_LINKSET]; // 1:PRIORITY 0, 2:PRIORITY 1, 3:PRIORITY 2, 4:PRIORITY 3
   unsigned char  aLinksetStatus[S7_MAX_MTP3_LINKSET]; // 1:ACTIVE, 2:INACTIVE, 3:ABNORMAL, 4:HALF NORMARL, 5:NORMAL, 6:FULL NORMAL
} SS7ROUTECONF, *PSS7ROUTECONF;

typedef struct {
   unsigned char  bPaused;
   unsigned char  bCongested;
   unsigned char  bDpcRestart;
   unsigned char  CongestionPriority;  // 1:PRIORITY 0, 2:PRIORITY 1, 3:PRIORITY 2, 4:PRIORITY 3
} SS7ROUTESTATE, *PSS7ROUTESTATE;

typedef struct {
   int            nCount;
   SS7ROUTECONF   RouteConf[S7_MAX_MTP3_ROUTE];
   SS7ROUTESTATE  RouteState[S7_MAX_MTP3_ROUTE];
} SS7ROUTEINFO, *PSS7ROUTEINFO;

typedef struct {
   XTMMSGHDR      h;
   SS7ROUTEINFO   RouteInfo;
} SS7MSGROUTE, *PSS7MSGROUTE;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// ISUP CIC INFO
//////////////////////////////////////////////////////
typedef struct {
   unsigned int   hCircuit;
   char           szCircuitName[S7_MAX_ISUP_CIRCUIT_NAME];
   unsigned int   unCircuitId;
   unsigned int   hIsupInterface;
   unsigned int   unMscCic;       // cic code(0..4095) -> MSC-PCM
   unsigned short unIpCic;        // cic code(0..8161) -> IP-PCM
   unsigned short Options;
   unsigned short unFirstCic;
   unsigned short unNbCircuitInGroup;
   unsigned char  ControlType;    // 1:INCOMING, 2:OUTGOING, 3:BOTHWAY
   unsigned char  bContChkForOutgoing;
   unsigned char  SlotId;
   unsigned char  bDualSeizureCtrlMRate;
   unsigned char  bNonSs7Conn;
   unsigned char  unSysId;        //
   unsigned char  _reserved[2];
} SS7CICCONF, *PSS7CICCONF;

typedef struct {
   int            nCount;
   SS7CICCONF     CicConf[S7_MAX_ISUP_CIRCUIT];
} SS7CICINFO, *PSS7CICINFO;

typedef struct {
   XTMMSGHDR      h;
   SS7CICINFO     CicInfo;
} SS7MSGCIC, *PSS7MSGCIC;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// ISUP INTERFACE INFO
//////////////////////////////////////////////////////
typedef struct {
   unsigned int   hIsupInterface;
   unsigned int   hIsupUserpart;
   unsigned int   hIsupNetwork;
   char           szIsupIfName[S7_MAX_ISUP_CIRCUIT_NAME];
   unsigned int   TrunkType;
   unsigned int   unOpc;
   unsigned int   unDpc;
   char           PauseAction;  // 1:CLEAR ALL, 2:CLEAR TRANSIENT, 3:CLEAR AFTER TIMER
   char           bAvailTest;
   char           MultiRateTableCheck;
   char           SlsSelector;
} SS7ISUPIFCONF, *PSS7ISUPIFCONF;

typedef struct {
   int            nCount;
   SS7ISUPIFCONF  IsupIfConf[S7_MAX_ISUP_INTERFACE];
} SS7ISUPIFINFO, *PSS7ISUPIFINFO;

typedef struct {
   XTMMSGHDR      h;
   SS7ISUPIFINFO  IsupIfInfo;
} SS7MSGISUPIF, *PSS7MSGISUPIF;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// ISUP CICMAP INFO
//////////////////////////////////////////////////////
typedef struct {
   unsigned int   hCircuit;
   char           szMscName[S7_MAX_ISUP_CIRCUIT_NAME];
   char           szCicName[S7_MAX_ISUP_CIRCUIT_NAME];
   unsigned short unMscCic;
   unsigned short unIpCic;
   unsigned char  unBoardId;     // BOARD NO
   unsigned char  unTrunkId;     // TRUNK NO
   unsigned char  unStartChannel;
   unsigned char  unChannelCnt;  // 31
   unsigned int   unOpc;
   unsigned int   unDpc;
   unsigned int   unStartCic;    // MSC START CIC
   unsigned short unMscPCM;
   unsigned short unIpPCM;
} SS7CICMAPCONF, *PSS7CICMAPCONF;

typedef struct {
   int            nCount;
   SS7CICMAPCONF  CicMapConf[S7_MAX_ISUP_CIRCUIT];
} SS7CICMAPINFO, *PSS7CICMAPINFO;

typedef struct {
   XTMMSGHDR      h;
   SS7CICMAPINFO  CicMapInfo;
} SS7MSGCICMAP, *PSS7MSGCICMAP;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// MTP3,ISUP  ADD/DEL
//////////////////////////////////////////////////////
typedef struct {
   char  szLinkName[S7_MAX_MTP3_LINK_NAME];
   char  szMtp2LinkName[S7_MAX_MTP2_LINK_NAME];
   char  szOPC[32];    // 0x0000
   char  szDPC[32];    // 0x0000
   char  szSLC[32];    // 1~
} SS7LINK, *PSS7LINK;

typedef struct {
   char  szLinksetName[S7_MAX_MTP3_LINKSET_NAME];
   char  szOPC[32];    // 0x0000
   char  szDPC[32];    // 0x0000
} SS7LINKSET, *PSS7LINKSET;

typedef struct {
   char  szRouteName[S7_MAX_MTP3_ROUTE_NAME];
   char  szLinksetName[S7_MAX_MTP3_LINKSET_NAME];
   char  szPointCode[32]; // 0.0.0 (3bit.8bit.3bit)
   char  szSPType[32];    // 1~
   char  szDirection[32]; // 1~
   char  szPriority[32];  // 1~
} SS7ROUTE, *PSS7ROUTE;

typedef struct {
   char  szMscName[S7_MAX_ISUP_CIRCUIT_NAME];
   char  szIPOPC[32];
   char  szMSCDPC[32];
   char  szMSCPCM[32]; 
   char  szIPPCM[32]; 
} SS7CIC, *PSS7CIC;

//// MTP3, ISUP ADD/DELETE/MODIFY
typedef struct {
   SS7LINK    Link;
   SS7LINKSET Linkset;
   SS7ROUTE   Route;
   SS7CIC     Cic;
} SS7MTP3ISUPINFO, *PSS7MTP3ISUPINFO;

typedef struct {
   XTMMSGHDR       h;
   SS7MTP3ISUPINFO Mtp3IsupInfo;
} SS7MSGMTP3ISUP, *PSS7MSGMTP3ISUP;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// SS7XTM MESSAGE HEADER
//////////////////////////////////////////////////////
typedef struct {
   unsigned int   uMagicCookie;
   int            nMsgLen;
   XTMADDR        stReceiver;
   XTMADDR        stSender;
   short          snHopCnt;
   unsigned short usTransactionId;
   short          snSerNo;
   unsigned short _reserved;
   unsigned short usType;
   unsigned short usSubType;
} SS7XTMMSGHDR, *SS7XTMMSGHDRPTR;
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// SS7 CONTROL MESSAGE
//////////////////////////////////////////////////////
typedef struct {
   int nMsgType;
   unsigned char  ucHostId;
   unsigned char  ucBoardId;
   unsigned char  ucE1No;
   unsigned char  ucChan;
   unsigned int   uMask;
   unsigned short usOvld;
   unsigned short usResult;
   unsigned short usReason;
   unsigned char  ucInfo[S7_MAX_CHAN];
   unsigned short _reserved;
} S7MSGCTRL;

typedef struct {
   XTMMSGHDR h;
   S7MSGCTRL    ctrl;
} S7MSGSIMPLE, *S7MSGSIMPLEPTR;
//////////////////////////////////////////////////////

#endif  /*  __SS7_CONTROL_H__  */
