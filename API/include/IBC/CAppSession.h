#ifndef APPSESSION_H
#define APPSESSION_H

#include "eDum/eDumLibDef.hxx"
#include "eDum/eDumLibStack.hxx"
#include "eDum/eDumLibSession.hxx"

#include "eRule/RuleInterface.h"
#include "eRule/CStatLog.h"
#include "eRule/CRUPools.h"

#include "eRte/eSipRteApi.hxx"
#include "eRte/eSipRteErr.hxx"
#include "eSipErr.hxx"

#include "CIbcConfig.h"
#include "CSipConfig.h"
#include "CTrgwTcpServer.h"
#include "CPdeTcpServer.h"
#include "CPdeTimer.h"
#include "CTrGWTimer.h"
#include "IBCErrorTable.h"
#include "IBCDefine.h"
#include "eSipUtil/string.hxx"

/************************* Global RTE Error Table *******************************************/
typedef enum 
{
	E_SRV_REASON_SYSTEM_OVERLOAD_CAUSE       = SipReasonCode_t::E_SYSTEM_OVERLOAD_CAUSE         ,
	E_SRV_REASON_CPS_OVERLOAD_CAUSE          = SipReasonCode_t::E_CPS_OVERLOAD_CAUSE            , 
	E_SRV_REASON_CPU_OVERLOAD_CAUSE          = SipReasonCode_t::E_CPU_OVERLOAD_CAUSE            ,
	E_SRV_REASON_MEM_OVERLOAD_CAUSE          = SipReasonCode_t::E_MEM_OVERLOAD_CAUSE            ,
	E_SRV_REASON_UNKNOWN_ROUTE_CAUSE         = SipReasonCode_t::E_UNKNOWN_ROUTE_CAUSE           ,
	E_SRV_REASON_ROUTING_FAIL_CAUSE          = SipReasonCode_t::E_ROUTING_FAIL_CAUSE            ,
	E_SRV_REASON_INTERNAL_ERR_CAUSE          = SipReasonCode_t::E_INTERNAL_ERR_CAUSE            ,
	E_SRV_REASON_INCORRECT_MSG_CAUSE         = SipReasonCode_t::E_INCORRECT_MSG_CAUSE           ,
	E_SRV_REASON_FILTER_DENY_CAUSE           = SipReasonCode_t::E_FILTER_DENY_CAUSE             ,
	E_SRV_REASON_SESSION_NOT_EXIST_CAUSE     = SipReasonCode_t::E_SESSION_NOT_EXIST_CAUSE       ,
	E_SRV_REASON_TIMEOUT_ERR_CAUSE           = SipReasonCode_t::E_TIMEOUT_ERR_CAUSE             ,
	E_SRV_REASON_MP_NOTCONT_CAUSE            = SipReasonCode_t::E_MP_NOTCONT_CAUSE              ,
	E_SRV_REASON_TRGW_NEGO_FAIL_CAUSE        = SipReasonCode_t::E_TRGW_NEGO_FAIL_CAUSE          ,
	E_SRV_REASON_TRGW_NO_RTP_FAIL_CAUSE      = SipReasonCode_t::E_TRGW_NO_RTP_FAIL_CAUSE        ,
	E_SRV_REASON_TRGW_INSUFFICIENT_RSC_CAUSE = SipReasonCode_t::E_TRGW_INSUFFICIENT_RSC_CAUSE   ,
	E_SRV_REASON_TRGW_SDP_INCORRECT_CAUSE    = SipReasonCode_t::E_TRGW_SDP_INCORRECT_CAUSE      ,
	E_SRV_REASON_TRGW_INCORRECT_CAUSE        = SipReasonCode_t::E_TRGW_INCORRECT_CAUSE          ,
	E_SRV_REASON_ROUTE_STS_DOWN_CAUSE        = SipReasonCode_t::E_ROUTE_STS_DOWN_CAUSE          ,
	E_SRV_REASON_CAUSE_MAX                   = SipReasonCode_t::E_CAUSE_MAX                     
}ESrvReason_t;
extern const char * g_arrReason[E_SRV_REASON_CAUSE_MAX+1];
namespace name_convreason
{
	typedef EIBCErrors_t (*PfuncTable_t)();
	EIBCErrors_t fnExe(ESrvReason_t _eReason);
};
typedef enum
{
	E_ROUTING_NONE = 0,
	E_ROUTING_SUCCESS,
	E_ROUTING_FAIL,
	E_ROUTING_RULE_FAIL,
	E_ROUTING_RTE_ST_DOWN,
}ERoutingResult_t;
inline const char * g_fnStringERoutingResult(int _eT)
{
	switch(_eT)
	{
		case E_ROUTING_NONE: return "E_ROUTING_NONE";
		case E_ROUTING_SUCCESS: return "E_ROUTING_SUCCESS";
		case E_ROUTING_FAIL: return "E_ROUTING_FAIL";
		case E_ROUTING_RULE_FAIL: return "E_ROUTING_RULE_FAIL";
		case E_ROUTING_RTE_ST_DOWN: return "E_ROUTING_RTE_ST_DOWN";
		default: return "E_ROUTING_NONE";
	}
	return "E_ROUTING_NONE";
}
/************************* Service Error Struct ********************************************/
typedef struct AppReason_t
{
	AppReason_t(){fnClear();}
	void fnClear(){memset(this,0x00,sizeof(AppReason_t));}
	void fnSet(unsigned int _uiRteID, ESrvReason_t _eReason)
	{
		m_uiRteID = _uiRteID;m_eReason = _eReason;
	}
	unsigned int m_uiRteID;
	ESrvReason_t m_eReason;
	int m_nCauseCode;
	int m_nRspCode;
	char m_szCauseText[DEF_RTE_MAXLEN_STRIUNAVAIL];
	char m_szError[256];
}AppReason_t;
typedef enum
{
	E_APP_SESS_EV_NONE = 0,
	E_APP_SESS_EV_TRGW_ADD_TO,
	E_APP_SESS_EV_TRGW_MOD_TO,
	E_APP_SESS_EV_TRGW_DEL_REQ,
	E_APP_SESS_EV_TRGW_DEL_RSP,
	E_APP_SESS_EV_TRGW_DEL_TO,
	E_APP_SESS_EV_TRGW_CHG_TO,
	E_APP_SESS_EV_TRGW_ARM_IND,
	#ifdef DEF_TRGW_RETRANS
	E_APP_SESS_EV_TRGW_ADD_RE,
	E_APP_SESS_EV_TRGW_MOD_RE,
	E_APP_SESS_EV_TRGW_DEL_RE,
	#endif
	E_APP_SESS_EV_PDE_START_TO,
	E_APP_SESS_EV_PDE_INTERIM_TO,
	E_APP_SESS_EV_PDE_STOP_TO,
	E_APP_SESS_EV_PDE_START_RSP,
	E_APP_SESS_EV_PDE_STOP_RSP,
	E_APP_SESS_EV_PDE_INTERIM_RSP,
	E_APP_SESS_EV_PDE_REPRESH,
	#ifdef DEF_PDE_RETRANS
	E_APP_SESS_EV_PDE_START_RE,
	E_APP_SESS_EV_PDE_INTERIM_RE,
	E_APP_SESS_EV_PDE_REFRESH_RE,
	E_APP_SESS_EV_PDE_STOP_RE,
	#endif
	#ifdef DEF_R131
	E_APP_SESS_EV_ENUM_RE,
	#endif
	E_APP_SESS_EV_MAX,
}EAppSessionEvent_t;
extern const char  * g_arrSessionEvent[E_APP_SESS_EV_MAX];

/***************************** IBC CDR Data *********************************************/
typedef enum
{
	E_IBC_CDR_IDL = 0,                //Idle
	E_IBC_CDR_TRYING,                // try  --> uas_inv
	E_IBC_CDR_PROCEEDING,        // pro --> uac_inv
	E_IBC_CDR_EARLY,                 // ear --> uac_proceeding/Rel
	E_IBC_CDR_EST,                      // est --> uac_accept
	E_IBC_CDR_TER                      // Terminated
}ECdrState_t;
typedef enum
{
	E_IBC_END_HOST_NON = 0,
	E_IBC_END_HOST_IBC,
	E_IBC_END_HOST_TRG,
	E_IBC_END_HOST_OUT
}ECdrEndHost_t;
typedef enum
{
	E_CDR_OFF = 0,
	E_CDR_ON = 1
}ECdrOnOff_t;
typedef enum
{
	E_CDR_MEDIA_RTP = 0,
	E_CDR_MEDIA_MSRP
}ECdrMedia_t;
extern const char * g_arrCdrMedia[E_CDR_MEDIA_MSRP+1];
extern const char * g_arrCdrState[E_IBC_CDR_TER+1];
extern const char * g_arrCdrEndHost[E_IBC_END_HOST_OUT+1];
extern const char * g_arrCdrOnOff[E_CDR_ON+1];
typedef struct IBCCdrData_t
{
	IBCCdrData_t(){fnClear();}
	~IBCCdrData_t(){}
	void fnClear(){memset(this,0x00,sizeof(IBCCdrData_t));}
	void fnStart()
	{
		A_R = (unsigned int)E_SRV_REASON_CAUSE_MAX;
		D_E_Nd = E_IBC_END_HOST_NON;
		m_eCMError = E_IBC_ERROR_CM_NONE;
	}
	unsigned int D_S_IDx;     // Dialog Stat Index
	unsigned int S_IDx;          // Dialog Stat Index
	unsigned int D_ID;            // Dialog ID
	unsigned int M_D_ID;       //Dailog ID
	unsigned int Cr_Nd;         // Caller RMT
	unsigned int Cd_Nd;         // Called RMT
	unsigned int RRS;             // RO ID
	unsigned int RS;                // RSEQ ID
	unsigned int SR;               // SSEQ ID
	unsigned int RT;                // Route ID
	char Cr[64];                    // Caller Number
	char Cd[64];                   // Called Number
	unsigned int BT;              // Begin Time
	unsigned int ET;              // End Time
	ECdrState_t D_ET;          // Call Process State 
	ECdrEndHost_t D_E_Nd;      // Call EndHost
	unsigned int A_R;           // Abnormal_Reason
	unsigned int VDU;
	unsigned int ASSES;
	unsigned int CR_RTE;       // Caller Route
	unsigned int CD_RTE;       //  Called Route
	EIBCErrors_t m_eCMError;     // must implement
	EIBCErrors_t m_eCoreError;   // not defined
	EIBCErrors_t m_eTrGWError;  // add or mod or del response result code
	EIBCErrors_t m_ePdeError;    // start response reason code 
	char m_szTgAsReason[1024];
	void fnWriteCdr(RUStatLog * _pclsCdr,char * _pszSipMsgHistory,int _nCoreReason)
	{
		RUString clsLog(2048);
		fnBuildCdr(clsLog,_pszSipMsgHistory,_nCoreReason);
		_pclsCdr->fnWriteLog((RUSTR)clsLog);
	}
	void fnBuildCdr(RUString &_rclsLog,char * _pszSipMsgHistory,int _nCoreReason)
	{
		if(D_E_Nd == E_IBC_END_HOST_NON) D_E_Nd = E_IBC_END_HOST_OUT;
		char szAR[64];memset(szAR,0x00,64);
		fnRUStrnCat(szAR,64,"0x%x",_nCoreReason);
		#ifndef DEF_R120
		_rclsLog<<"D_S_IDx="<<RUIntString(D_S_IDx)
			<<",S_IDx="<<RUIntString(S_IDx)
			<<",D_ID="<<RUIntString(D_ID)
			<<",M_D_ID="<<RUIntString(M_D_ID)
			<<",Cr_Nd="<<RUIntString(Cr_Nd)
		#else
		_rclsLog<<"Cr_Nd="<<RUIntString(Cr_Nd)
		#endif
			<<",Cd_Nd="<<RUIntString(Cd_Nd)
			<<",RRS="<<RUIntString(RRS)
			<<",RS="<<RUIntString(RS)
			<<",SR="<<RUIntString(SR)
			<<",RT="<<RUIntString(RT)
			<<",Cr="<<Cr
			<<",Cd="<<Cd
			<<",BT="<<RUIntString(BT)
			<<",ET="<<RUIntString(ET)
			<<",D_ET="<<g_arrCdrState[D_ET]
			<<",D_E_Nd="<<g_arrCdrEndHost[D_E_Nd]
			<<",A_R="<<szAR
			#if defined(DEF_R120) && defined(DEF_CDR_REASON)
			<<","<<fnRUStr(_pszSipMsgHistory)
			<<","<<fnRUStr(m_szTgAsReason);
			#else
			<<","<<fnRUStr(_pszSipMsgHistory)
			<<","<<fnRUStr(m_szTgAsReason)<<"\r\n";
			#endif
	}
	void fnBuildTraceCdr(RUString &_rclsLog,char * _pszSipMsgHistory,int _nCoreReason)
	{
		if(D_E_Nd == E_IBC_END_HOST_NON) D_E_Nd = E_IBC_END_HOST_OUT;
		char szAR[64];memset(szAR,0x00,64);
		fnRUStrnCat(szAR,64,"0x%x",_nCoreReason);
		#ifndef DEF_R131
		IBCErrorElement_t *pAR = IBCErrors::fnGetInstance()->fnFind((EIBCErrors_t)_nCoreReason);
		#endif
		_rclsLog<<"<CDR Information>\r\n"
			#ifndef DEF_R120
			<<"D_S_IDx(통계인덱스)="<<RUIntString(D_S_IDx)<<"\r\n"
			<<"S_IDx(다이얼로그 통계 Index)="<<RUIntString(S_IDx)<<"\r\n"
			<<"D_ID(다이얼로그 ID)="<<RUIntString(D_ID)<<"\r\n"
			<<"M_D_ID(다이얼로그 ID)="<<RUIntString(M_D_ID)<<"\r\n"
			#endif
			<<"Cr_Nd(발신대국노드 ID)="<<RUIntString(Cr_Nd)<<"\r\n"
			<<"Cd_Nd(착신대국노드 ID)="<<RUIntString(Cd_Nd)<<"\r\n"
			<<"RRS(라우트 오퍼레이션 ID)="<<RUIntString(RRS)<<"\r\n"
			<<"RS(라우트 시퀀스 ID)="<<RUIntString(RS)<<"\r\n"
			<<"SR(스페셜 라우트 시권스 ID)="<<RUIntString(SR)<<"\r\n"
			<<"RT(라우트 ID)="<<RUIntString(RT)<<"\r\n"
			<<"Cr(발신자)="<<Cr<<"\r\n"
			<<"Cd(착신자)="<<Cd<<"\r\n"
			<<"BT(시작시각)="<<RUIntString(BT)<<"\r\n"
			<<"ET(종료시각)="<<RUIntString(ET)<<"\r\n"
			<<"D_ET(최종 호진행단계)="<<g_arrCdrState[D_ET]<<"\r\n"
			<<"D_E_Nd(종료주체)="<<g_arrCdrEndHost[D_E_Nd]<<"\r\n"
			<<"A_R(종료코드)="<<szAR;
			#ifndef DEF_R131
			if(pAR)
			{
				_rclsLog<<", \""<< pAR->m_pszDesc<<"\"\r\n"
						<<"                       \""<<pAR->m_pszAction<<"\"\r\n";
			}
			else _rclsLog<<"\r\n";
			#else
			_rclsLog<<"\r\n";
			#endif
		_rclsLog<<"<SIP 메시지 History>\r\n"
			<<TRUE_SELECT(fnRUStrLen(_pszSipMsgHistory) > 0,_pszSipMsgHistory,"없음")<<"\r\n"
			<<"<미디어 정보>\r\n"
			<<TRUE_SELECT(fnRUStrLen(m_szTgAsReason)>0,m_szTgAsReason,"없음")<<"\r\n";
	}
	void fnSetTrGWReason(const char * _pszReason)
	{
		if(m_szTgAsReason[0]==0)
		{
			fnRUStrnCpy(m_szTgAsReason,_pszReason,1024-1);
		}
		else
		{
			LOGGER(_WARNING,"Already Set TgasReason : [prev:%s]\r\n[next:%s]",
													m_szTgAsReason,fnRUStr((char*)_pszReason));
		}
	}
	void fnSetEndHost(ECdrEndHost_t _eT)
	{
		LOGGER(_DEBUG,"Event End Host : prv=%s,event=%s",g_arrCdrEndHost[D_E_Nd],g_arrCdrEndHost[_eT]);
		if(D_E_Nd == E_IBC_END_HOST_NON) D_E_Nd = _eT;
	}
}IBCCdrData_t;
/***************************** IBC RTE Data **********************************************/
typedef enum
{
	E_IBC_CALL_DIR_NONE = 0,
	E_IBC_CALL_DIR_INT,
	E_IBC_CALL_DIR_EXT
}EIbcCallDirection_t;
extern const char *g_arrIbcCallDirection[E_IBC_CALL_DIR_EXT+1];
typedef struct App5Tuple_t
{
	App5Tuple_t(){fnClear();}
	void fnClear(){memset(this,0x00,sizeof(App5Tuple_t));}
	resip::IpVersion m_eIpVersion;
	resip::TransportType m_eTransType;    
	char m_szLocalIP[DEF_RTE_MAXLEN_IP];
	unsigned int m_unLocalPort;	
	char m_szRemoteIP[DEF_RTE_MAXLEN_IP];
	unsigned int m_unRemotePort;	
	void fnSetInfo(const eSIP::Net5Tuple_t & _pstInfo)
	{
		fnClear();
		m_eIpVersion = _pstInfo.m_eIpVersion;
		m_eTransType = _pstInfo.m_eTransportType;
		fnRUStrnCat(m_szLocalIP,DEF_RTE_MAXLEN_IP,_pstInfo.m_clsLocalIp.c_str());
		m_unLocalPort = _pstInfo.m_nLocalPort;
		fnRUStrnCat(m_szRemoteIP,DEF_RTE_MAXLEN_IP,_pstInfo.m_clsRemoteIp.c_str());
		m_unRemotePort = _pstInfo.m_nRemotePort;
		char szDebug[1024];memset(szDebug,0x00,1024);
		fnDebug(szDebug,1024);
		LOGGER(_DEBUG,szDebug);
	}
	void fnDebug(char * _pszDebug, unsigned int _unLen)
	{
		fnRUStrnCat(_pszDebug,_unLen,"5Tuple Info\r\nLocal=%s,%u\r\nRemote=%s,%u=\r\n",
											m_szLocalIP,m_unLocalPort,
											m_szRemoteIP,m_unRemotePort);
	}
}App5Tuple_t;
typedef struct SimpleLocal_t
{
	SimpleLocal_t(){fnClear();}
	void fnClear(){memset(this,0x00,sizeof(SimpleLocal_t));}
	unsigned int   m_uiID;                 
	char m_szDesc[DEF_RTE_MAXLEN_DESC]; 
	char m_szDomain[DEF_RTE_MAXLEN_DOMAIN];
	resip::IpVersion m_eIpVersion;
	resip::TransportType m_eTransType;    
	char m_szIP[DEF_RTE_MAXLEN_IP];
	unsigned int m_unPort;
	void fnSetInfo(SipNodeLocal_t * _pstInfo)
	{
		fnClear();
		m_uiID = _pstInfo->m_uiID;
		fnRUStrnCat(m_szDesc,DEF_RTE_MAXLEN_DESC,_pstInfo->m_szDesc);
		fnRUStrnCat(m_szDomain,DEF_RTE_MAXLEN_DOMAIN,_pstInfo->m_szDomain);
		if(_pstInfo->m_usIPver == 4) m_eIpVersion = resip::V4;
		else if(_pstInfo->m_usIPver == 6) m_eIpVersion = resip::V6;
		if(_pstInfo->m_ucProto <= E_ESIP_TRANSPORT_DTLS) 
			m_eTransType = (resip::TransportType)_pstInfo->m_ucProto;
		fnRUStrnCat(m_szIP,DEF_RTE_MAXLEN_IP,_pstInfo->m_szIP);
		m_unPort = _pstInfo->m_usPort;
	}
}SimpleLocal_t;
typedef struct SimpleRemote_t
{
	SimpleRemote_t(){fnClear();}
	void fnClear(){memset(this,0x00,sizeof(SimpleLocal_t));}
	unsigned int   m_uiID;                 
	char m_szDesc[DEF_RTE_MAXLEN_DESC]; 
	char m_szDomain[DEF_RTE_MAXLEN_DOMAIN];
	resip::IpVersion m_eIpVersion;
	resip::TransportType m_eTransType;    
	char m_szIP[DEF_RTE_MAXLEN_IP];
	unsigned int m_unPort;
	void fnSetInfo(SipNodeRemote_t * _pstInfo)
	{
		fnClear();
		m_uiID = _pstInfo->m_uiID;
		fnRUStrnCat(m_szDesc,DEF_RTE_MAXLEN_DESC,_pstInfo->m_szDesc);
		fnRUStrnCat(m_szDomain,DEF_RTE_MAXLEN_DOMAIN,_pstInfo->m_szDomain);
		if(_pstInfo->m_usIPver == 4) m_eIpVersion = resip::V4;
		else if(_pstInfo->m_usIPver == 6) m_eIpVersion = resip::V6;
		if(_pstInfo->m_ucProto <= E_ESIP_TRANSPORT_DTLS) 
			m_eTransType = (resip::TransportType)_pstInfo->m_ucProto;
		fnRUStrnCat(m_szIP,DEF_RTE_MAXLEN_IP,_pstInfo->m_szIP);
		m_unPort = _pstInfo->m_usPort;
	}
}SimpleRemote_t;
typedef struct SimpleRoute_t
{
	SimpleRoute_t(){fnClear();}
	void fnClear()
	{
		memset(this,0x00,sizeof(SimpleRoute_t));
	}
	unsigned int m_uiID;
	char m_szDesc[DEF_RTE_MAXLEN_DESC];
	SipRoute_t::ENodeType_t m_eType;
	SipRoute_t::EMgwRouteFlag_t m_eRoute;
	unsigned int m_uiLOC;
	unsigned int m_uiRMT;
	unsigned int m_uiTRTE;
	#ifdef DEF_R126
	unsigned int m_uiGID;
	#endif
	void fnSetInfo(SipRoute_t * _pstRoute)
	{
		memcpy(m_szDesc,_pstRoute->m_szDesc,DEF_RTE_MAXLEN_DESC);
		m_uiID = _pstRoute->m_uiID;
		m_eType = (SipRoute_t::ENodeType_t)_pstRoute->m_ucType;
		m_eRoute = (SipRoute_t::EMgwRouteFlag_t)_pstRoute->m_ucDoRouteMedia;
		m_uiLOC = _pstRoute->m_uiLID;
		m_uiRMT = _pstRoute->m_uiRID;
		m_uiTRTE = _pstRoute->m_uiTRTE;
		#ifdef DEF_R126
		m_uiGID = (unsigned int)_pstRoute->m_nRouteGroup;
		#endif
	}
}Simpleroute_t;
typedef struct IBCRteInfo_t
{
	IBCRteInfo_t(){fnClear();}
	void fnClear(){memset(this,0x00,sizeof(IBCRteInfo_t));}
	EIbcCallDirection_t m_eIbcDir;

	SimpleLocal_t m_stICLocal;
	SimpleLocal_t m_stOGLocal;
	SimpleRemote_t m_stICRemote;
	SimpleRemote_t m_stOGRemote;
	
	SimpleRoute_t m_stICRTE;
	SimpleRoute_t m_stOGRTE;
	
	char m_szRteError[256];
}IBCRteInfo_t;
/***************************** IBC TrGW Data *********************************************/
#define DEF_TRGW_POOL_NUM   6
typedef enum
{
	E_MEDIA_NONE = 0,
	E_MEDIA_ADD_REQ = 1,
	E_MEDIA_ADD_RSP = 2,
	E_MEDIA_MOD_REQ = 3,
	E_MEDIA_MOD_RSP = 4,
	E_MEDIA_DEL_REQ = 5,
	E_MEDIA_DEL_RSP = 6,
	E_MEDIA_CHG_REQ = 7,
	E_MEDIA_CHG_RSP = 8
}ETrgwMedia_t;
extern const char * g_arrTrgwMedia[E_MEDIA_CHG_RSP+1];
extern const char * g_arrSipState[eDum::E_SIP_STATE_NUM+1];
typedef struct TrGWResource_t
{
	TrGWResource_t(){fnClear();}
	~TrGWResource_t(){}
	void fnClear()
	{
		memset(this,0x00,sizeof(TrGWResource_t));
		m_eSeqMethod = resip::MAX_METHODS;
	}
	void fnReLoadTimer()
	{
		if(m_eType == E_MEDIA_ADD_REQ)
		{
			TrGWTimer::fnSetReTransaction(E_TRGWTIMER_ADD,NULL, m_unSessionID,
										m_unTransID,
										&m_stReqTO);
		}
		else if(m_eType == E_MEDIA_MOD_REQ)
		{
			TrGWTimer::fnSetReTransaction(E_TRGWTIMER_MOD,NULL, m_unSessionID,
										m_unTransID,
										&m_stReqTO);
		}
		else if(m_eType == E_MEDIA_DEL_REQ)
		{
			TrGWTimer::fnSetReTransaction(E_TRGWTIMER_DEL,NULL, m_unSessionID,
										m_unTransID,
										&m_stReqTO);
		}
		else if(m_eType == E_MEDIA_CHG_REQ)
		{
			TrGWTimer::fnSetReTransaction(E_TRGWTIMER_CHG,NULL, m_unSessionID,
										m_unTransID,
										&m_stReqTO);
		}
	}
	unsigned int m_unTransID;
	ETrgwMedia_t m_eType;
	unsigned int m_unSessionID;
	unsigned long m_ulEventTime;
	struct timespec m_stReqTO;
	eDum::ESipState_t m_eSipState;
	unsigned int m_unIndex;
	resip::MethodTypes m_eSeqMethod;
	int m_nSeqRsp;
	void fnDebug(RUString & _rclsDebug)
	{
		if(m_unTransID == 0) return;
		_rclsDebug<<"    < TrGW Resource\r\n"
			<<"      m_unTransID = "<<RUIntString(m_unTransID)<<"\r\n"
			<<"      m_eType = "<<g_arrTrgwMedia[m_eType]<<"\r\n"
			<<"      m_unSessionID = "<<RUIntString(m_unSessionID)<<"\r\n"
			<<"      m_ulEventTime = "<<RUIntString(m_ulEventTime)<<"\r\n"
			<<"      m_Req_tv_sec = "<<RUIntString(m_stReqTO.tv_sec)<<"\r\n"
			<<"      m_Req_tv_nsec = "<<RUIntString(m_stReqTO.tv_nsec/1000000)<<"\r\n"
			<<"      m_eSipState = "<<g_arrSipState[m_eSipState]<<"\r\n"
			<<"      m_unIndex = "<<RUIntString(m_unIndex)<<"\r\n"
			<<"      m_eSeqMethod = "<<resip::getMethodName(m_eSeqMethod).c_str()<<"\r\n"
			<<"    >\r\n";
	}
}TrGWResource_t;
typedef struct TrGWPool_t
{
	TrGWPool_t(){fnClear();}
	~TrGWPool_t(){fnClear();}
	void fnClear()
	{
		m_unHistoryNum=0;m_unUseNum = 0;
		for(int i=0; i<DEF_TRGW_POOL_NUM;i++)
			m_stPool[i].fnClear();
	}
	void fnStart()
	{
		for(int i=0; i<DEF_TRGW_POOL_NUM;i++) m_stPool[i].m_unIndex = i;
	}
	NS_CS::TrgwAddReqMsg_t * fnGetAddReq(unsigned int _unSessionID, unsigned int _unTransID,
																		unsigned int _unSize=2048)
	{
		TrGWResource_t * pstResource = fnAlloc(_unSessionID,_unTransID);
		if(pstResource == NULL) return NULL;
		pstResource->m_eType = E_MEDIA_ADD_REQ;
		return NS_CS::g_fnCreateAddReq(_unSize);
	}
	NS_CS::TrgwModReqMsg_t * fnGetModReq(unsigned int _unSessionID, unsigned int _unTransID,
																		unsigned int _unSize=2048)
	{
		TrGWResource_t * pstResource = fnAlloc(_unSessionID,_unTransID);
		if(pstResource == NULL) return NULL;
		pstResource->m_eType = E_MEDIA_MOD_REQ;
		return NS_CS::g_fnCreateModReq(_unSize);
	}
	NS_CS::TrgwDelReqMsg_t * fnGetDelReq(unsigned int _unSessionID, unsigned int _unTransID,
																		unsigned int _unSize=2048)
	{
		TrGWResource_t * pstResource = fnAlloc(_unSessionID,_unTransID);
		if(pstResource == NULL) return NULL;
		pstResource->m_eType = E_MEDIA_DEL_REQ;
		return NS_CS::g_fnCreateDelReq(_unSize);
	}
	NS_CS::TrgwChgReqMsg_t * fnGetChgReq(unsigned int _unSessionID, unsigned int _unTransID,
																		unsigned int _unSize=2048)
	{
		TrGWResource_t * pstResource = fnAlloc(_unSessionID,_unTransID);
		if(pstResource == NULL) return NULL;
		pstResource->m_eType = E_MEDIA_CHG_REQ;
		return NS_CS::g_fnCreateChgReq(_unSize);
	}
	TrGWResource_t * fnFindTrans(unsigned int _unTransID)
	{
		for(int i=0; i<DEF_TRGW_POOL_NUM;i++)
		{
			if(m_stPool[i].m_unTransID == _unTransID) {return &m_stPool[i];}
		}
		return NULL;
	}
	TrGWResource_t * fnAlloc(unsigned int _unSessionID, unsigned int _unTransID)
	{
		if(m_unUseNum==DEF_TRGW_POOL_NUM) return NULL;
		for(int i=0; i<DEF_TRGW_POOL_NUM;i++)
		{
			if(m_stPool[i].m_eType == E_MEDIA_NONE) 
			{
				m_unHistoryNum++;m_unUseNum++;
				m_stPool[i].m_unSessionID = _unSessionID;m_stPool[i].m_unTransID = _unTransID;
				return &m_stPool[i];
			}
		}
		return NULL;
	}
	bool fnFree(unsigned int _unTransID)
	{
		if(m_unUseNum==0) return false;
		for(int i=0; i<DEF_TRGW_POOL_NUM;i++)
		{
			if(m_stPool[i].m_unTransID == _unTransID)
			{
				m_stPool[i].fnClear();m_unUseNum--;
				return true;
			}
		}
		return false;
	}
	void fnReloadTimer()
	{
		for(int i=0; i<DEF_TRGW_POOL_NUM;i++)
		{
			if(m_stPool[i].m_eType != E_MEDIA_NONE) 
			{
				m_stPool[i].fnReLoadTimer();
			}
		}
	}
	unsigned int m_unHistoryNum;
	unsigned int m_unUseNum;
	TrGWResource_t m_stPool[DEF_TRGW_POOL_NUM];
	void fnDebug(RUString &_rclsDebug)
	{
		_rclsDebug<<"  < TrGW Pools\r\n"
			<<"    m_unHistoryNum = "<<RUIntString(m_unHistoryNum)<<"\r\n"
			<<"    m_unUseNum = "<<RUIntString(m_unUseNum)<<"\r\n";
		for(int i=0;i<DEF_TRGW_POOL_NUM;i++) m_stPool[i].fnDebug(_rclsDebug);
		_rclsDebug<<"  >\r\n";
	}
}TrGWPool_t;
typedef struct IBCTrGWInfo_t
{
	IBCTrGWInfo_t(){fnClear();}
	void fnClear()
	{
		m_stPool.fnClear();
		memset(this,0x00,sizeof(IBCTrGWInfo_t));
	}
	void fnReLoad(unsigned int _unSessionID)
	{
		m_stPool.fnReloadTimer();
	}
	void fnStart(){m_stPool.fnStart();}
	NS_CS::TrgwAddReqMsg_t * fnGetAddReq(unsigned int _unSessionID, unsigned int _unTransID,
																		unsigned int _unSize=2048)
	{
		return m_stPool.fnGetAddReq(_unSessionID,_unTransID,_unSize);
	}
	NS_CS::TrgwModReqMsg_t * fnGetModReq(unsigned int _unSessionID, unsigned int _unTransID,
																		unsigned int _unSize=2048)
	{
		return m_stPool.fnGetModReq(_unSessionID,_unTransID,_unSize);
	}
	NS_CS::TrgwDelReqMsg_t * fnGetDelReq(unsigned int _unSessionID, unsigned int _unTransID,
																		unsigned int _unSize=2048)
	{
		return m_stPool.fnGetDelReq(_unSessionID,_unTransID,_unSize);
	}
	NS_CS::TrgwChgReqMsg_t * fnGetChgReq(unsigned int _unSessionID, unsigned int _unTransID,
																		unsigned int _unSize=2048)
	{
		return m_stPool.fnGetChgReq(_unSessionID,_unTransID,_unSize);
	}
	bool fnSetReqTime(unsigned int _unTransID,unsigned int _unTimeOutMs)
	{
		TrGWResource_t * pstResource = m_stPool.fnFindTrans(_unTransID);
		if(pstResource == NULL) return false;
		fnRUIncreTimeSpec(_unTimeOutMs,&pstResource->m_stReqTO);
		return true;
	}
	bool fnIsMediaEnable()
	{
		if(m_stPool.m_unHistoryNum > 0) return true;
		return false;
	}
	unsigned int m_unTrgwIndex;
	unsigned int m_unTgAsID;
	unsigned int m_unTgAsEqID;
	int m_nReason;
	bool m_bSndDel;
	#ifdef DEF_TRGW_TRANSPORT_CHANGED
	eSipUtil::Net5Tuple m_stAddr;
	#endif
	TrGWPool_t m_stPool;
	void fnDebug(RUString &_rclsDebug)
	{
		_rclsDebug<<"{ TrGW Info\r\n"
					<<"  m_unTrgwIndex = "<<RUIntString(m_unTrgwIndex)<<"\r\n"
					<<"  m_unTgAsID = "<<RUIntString(m_unTgAsID)<<"\r\n"
					<<"  m_unTgAsEqID = "<<RUIntString(m_unTgAsEqID)<<"\r\n"
					<<"  m_nReason = "<<RUIntString(m_nReason)<<"\r\n"
					<<"  m_bSndDel = "<<RUIntString(m_bSndDel)<<"\r\n";
		m_stPool.fnDebug(_rclsDebug);
		_rclsDebug<<"}\r\n";
	}
}IBCTrGWInfo_t;
/***************************** IBC PDE Data **********************************************/
#define PDE_POOL_MAX 4
#define PDE_INTERIM_TIME     90000   // Sec
typedef enum
{
	E_PDE_ST_NONE = 0,
	E_PDE_ST_START_REQ,
	E_PDE_ST_START_RSP,
	E_PDE_ST_INTERIM,
	E_PDE_ST_STOP_REQ,
	E_PDE_ST_STOP_RSP,
}EPdeStatus;
extern const char * g_arrPdeStatus[E_PDE_ST_STOP_RSP+1];
typedef enum
{
	E_PDE_EV_NONE = 0,
	E_PDE_EV_START_REQ,
	E_PDE_EV_START_RSP,
	E_PDE_EV_START_TO,
	E_PDE_EV_INTERIM_REQ,
	E_PDE_EV_INTERIM_RSP,
	E_PDE_EV_INTERIM_TO,
	E_PDE_EV_STOP_REQ,
	E_PDE_EV_STOP_RSP,
	E_PDE_EV_STOP_TO,
	E_PDE_EV_REFRESH_TO,
	E_PDE_EV_REFRESH_REQ
}EPdeEvent;
extern const char * g_arrPdeEv[E_PDE_EV_REFRESH_REQ+1];
typedef struct PdePool_t
{
	PdePool_t(){fnClear();}
	~PdePool_t(){}
	void fnClear()
	{
		memset(this,0x00,sizeof(PdePool_t));
	}
	void fnAlloc(){m_bUse = 1;}
	void fnFree(){m_bUse= 0 ;}
	void fnSetTimer(unsigned int _unTimeOutMs,EPdeTimer_t _eT, RUTcpChannel * _pclsCh)
	{
		if(m_bUse)
		{
			m_eTimer = _eT;
			fnRUIncreTimeSpec(_unTimeOutMs,&m_stTimeSpec);
			PdeTimer::fnSetTransaction(_eT,_pclsCh, m_unSessionID,m_unTransID);
		}
	}
	void fnReloadTimer()
	{
		if(m_bUse && m_unTransID)
			PdeTimer::fnSetReTransaction(m_eTimer,NULL,m_unSessionID,m_unTransID,&m_stTimeSpec);
	}
	void fnSetEventTime()
	{
		if(m_bUse && m_unTimeT == 0)
		{
			RUDailyTime clsDaily;memset(m_szEventTime,0x00,16);
			m_unTimeT = (unsigned int)clsDaily.m_stTime;
			fnRUStrCat(m_szEventTime,"%04u%02u%02u%02u%02u%02u%u",
															clsDaily.m_nYr,clsDaily.m_nMon,
															clsDaily.m_nDay,clsDaily.m_nHr,
															clsDaily.m_nMin,clsDaily.m_nSec, 
															(clsDaily.m_nMSec/100));
			m_szEventTime[15] = 0x00;
		}
	}
	PdeStartRecord_t * fnGetStartMsg()
	{
		if(m_bUse && m_eType == E_PDE_REQ_START)
		{
			fnSetEventTime();return (PdeStartRecord_t*)&m_stPacket;
		}
		return NULL;
	}
	PdeStopRecord_t * fnGetStopMsg()
	{
		if(m_bUse && m_eType == E_PDE_REQ_STOP)
		{	
			fnSetEventTime();return (PdeStopRecord_t*)&m_stPacket;
		}
		return NULL;
	}
	PdeInterim_t * fnGetInterimMsg()
	{
		if(m_bUse && m_eType == E_PDE_REQ_INTERIM)
		{
			fnSetEventTime();	return (PdeInterim_t*)&m_stPacket;
		}
		return NULL;
	}
	void fnReLoadTimer()
	{
		if(m_bUse && m_unTransID)
		{
			PdeTimer::fnSetReTransaction(m_eTimer,NULL,m_unSessionID,m_unTransID,&m_stTimeSpec);
		}
	}
	unsigned int m_bUse;
	unsigned int m_unTransID;
	unsigned int m_unSessionID;
	EPdeReq_t m_eType;
	char m_szEventTime[16];
	unsigned int m_unTimeT;
	EPdeTimer_t m_eTimer;
	unsigned int m_unSeq;
	struct timespec m_stTimeSpec;
	unsigned long m_ulEventTime;
	PDEMsg_t m_stPacket;   // PdeStartRecord_t, PdeStopRecord_t, PdeInterim_t
	void fnDebug(RUString &_rclsDebug)
	{
		if(m_bUse == 0) return;
		_rclsDebug<<"    < Pde Pool Info\r\n"
			<<"      m_bUse = "<<RUIntString(m_bUse)<<"\r\n"
			<<"      m_unTransID = "<<RUIntString(m_unTransID)<<"\r\n"
			<<"      m_unSessionID = "<<RUIntString(m_unSessionID)<<"\r\n"
			<<"      m_eType = "<<g_fnStringPdeReq(m_eType)<<"\r\n"
			<<"      m_szEventTime = "<<m_szEventTime<<"\r\n"
			<<"      m_unTimeT = "<<RUIntString(m_unTimeT)<<"\r\n"
			<<"      m_eTimer = "<<g_arrPdeTimer[m_eTimer]<<"\r\n"
			<<"      m_unSeq = "<<RUIntString(m_unSeq)<<"\r\n"
			<<"      m_stTimeSpec.tv_sec = "<<RUIntString(m_stTimeSpec.tv_sec)<<"\r\n"
			<<"      m_stTimeSpec.tv_nsec = "<<RUIntString(m_stTimeSpec.tv_nsec/1000000)<<"\r\n"
			<<"      m_ulEventTime = "<<RUIntString(m_ulEventTime)<<"\r\n"
			<<"    >\r\n";
	}
}PdePool_t;
typedef struct PdePools_t
{
	PdePools_t(){}
	~PdePools_t(){}
	void fnClear(){memset(this,0x00, sizeof(PdePools_t));}
	void fnReLoad(unsigned int _unSessionID)
	{
		m_unSessionID = _unSessionID;
		fnReloadTimer();
	}
	PdePool_t * fnAllocTrans()
	{
		for(unsigned int i=0;i<PDE_POOL_MAX;i++)
		{
			if(m_arrSession[i].m_bUse == false){m_arrSession[i].fnAlloc();return &m_arrSession[i];}
		}
		return NULL;
	}
	PdePool_t * fnFindTrans(unsigned int _unTransID)
	{
		for(unsigned int i=0;i<PDE_POOL_MAX;i++)
		{
			if(m_arrSession[i].m_bUse && m_arrSession[i].m_unTransID == _unTransID)
				return &m_arrSession[i];
		}
		return NULL;
	}
	PdePool_t * fnFindType(EPdeReq_t _eT)
	{
		for(unsigned int i=0;i<PDE_POOL_MAX;i++)
		{
			if(m_arrSession[i].m_bUse && m_arrSession[i].m_eType == _eT)
				return &m_arrSession[i];
		}
		return NULL;
	}
	bool fnFreeType(EPdeReq_t _eT)
	{
		PdePool_t * pstFind = fnFindType(_eT);
		if(pstFind == NULL) return false;
		pstFind->fnFree();
	}
	bool fnFreeTrans(unsigned int _unTransID)
	{
		PdePool_t *pstFind =  fnFindTrans(_unTransID);
		if(pstFind == NULL) return false;
		pstFind->fnFree();
		return true;		
	}
	PdePool_t * fnFindStartPool(unsigned int _unTransID)
	{
		return fnFindType(E_PDE_REQ_START);
	}
	PdePool_t * fnFindStopPool(unsigned int _unTransID)
	{
		return fnFindType(E_PDE_REQ_STOP);
	}
	PdePool_t * fnFindInterimPool(unsigned int _unTransID)
	{
		PdePool_t * pstFind = fnFindTrans(_unTransID);
		if(pstFind == NULL) return NULL;
		if(pstFind->m_eType == E_PDE_REQ_INTERIM) return pstFind;
		return NULL;
	}
	PdePool_t * fnAllocStartPool(unsigned int _unSessionID, unsigned int _unTransID)
	{
		PdePool_t * pFind = fnFindStartPool(_unTransID);
		if(pFind == NULL) pFind = fnAllocTrans();
		if(pFind)
		{
			pFind->fnClear();
			pFind->fnAlloc();
			pFind->m_eType = E_PDE_REQ_START;
			pFind->m_unTransID = _unTransID;
			pFind->m_unSessionID = _unSessionID;
			PdeStartRecord_t * pstStart = pFind->fnGetStartMsg();
			pstStart->m_fnClear();
			pstStart->hdr.uiAsSesId = _unSessionID;
			pstStart->hdr.uiCompSesId = 0;
			stAccReq * pstAccReq = pstStart->m_fnGetBody();
			pstAccReq->uiTransactionId = _unTransID;
			pstAccReq->usRecordType = stAccReq::START_RECORD;
			pstAccReq->usRecordNum = 1;m_unSeq = 1;
		}
		return pFind;
	}
	PdePool_t * fnAllocStopPool(unsigned int _unSessionID, unsigned int _unTransID)
	{
		PdePool_t * pFind = fnFindStopPool(_unTransID);
		if(pFind == NULL) pFind = fnAllocTrans();
		if(pFind)
		{
			pFind->fnClear();
			pFind->fnAlloc();
			pFind->m_eType = E_PDE_REQ_STOP;
			pFind->m_unTransID = _unTransID;
			pFind->m_unSessionID = _unSessionID;
			PdeStopRecord_t * pstStop = pFind->fnGetStopMsg();
			pstStop->m_fnClear();
			pstStop->hdr.uiAsSesId = _unSessionID;
			pstStop->hdr.uiCompSesId = 0;
			stAccReq * pstAccReq = pstStop->m_fnGetBody();
			pstAccReq->uiTransactionId = _unTransID;
			pstAccReq->usRecordType = stAccReq::STOP_RECORD;
			m_unSeq++;
			pstAccReq->usRecordNum = m_unSeq;
		}
		return pFind;
	}
	PdePool_t * fnAllocInterimPool(unsigned int _unSessionID, unsigned int _unTransID)
	{
		PdePool_t * pFind = fnFindInterimPool(_unTransID);
		if(pFind == NULL) pFind = fnAllocTrans();
		if(pFind)
		{
			pFind->fnClear();
			pFind->fnAlloc();
			pFind->m_eType = E_PDE_REQ_INTERIM;
			pFind->m_unTransID = _unTransID;
			pFind->m_unSessionID = _unSessionID;
			PdeInterim_t * pstInterim = pFind->fnGetInterimMsg();
			pstInterim->m_fnClear();
			pstInterim->hdr.uiAsSesId = _unSessionID;
			pstInterim->hdr.uiCompSesId = 0;
			//===============> Mandatory
			stAccReq * pstAccReq = pstInterim->m_fnGetBody();
			pstAccReq->uiTransactionId = _unTransID; 
			pstAccReq->usRecordType = stAccReq::INTERIM_RECORD;
			m_unSeq++;
			pstAccReq->usRecordNum = m_unSeq;
		}
		return pFind;
	}
	void fnReloadTimer()
	{
		for(unsigned int i=0;i<PDE_POOL_MAX;i++)
		{
			m_arrSession[i].fnReLoadTimer();
		}
		if(m_unRefreshTransID > 0)
			PdeTimer::fnSetReTransaction(E_PDETIMER_REFRESH,NULL,m_unSessionID,m_unRefreshTransID,&m_stRefreshT);
	}
	void fnSetRefreshTimer(unsigned int _unTimeOutMs,RUTcpChannel * _pclsCh,
								unsigned int _unSessionID, unsigned int _unTransID)
	{	
		m_unRefreshTransID = _unTransID;
		memset(&m_stRefreshT,0x00,sizeof(struct timespec));
		fnRUIncreTimeSpec(_unTimeOutMs,&m_stRefreshT);
		PdeTimer::fnSetReTransaction(E_PDETIMER_REFRESH,_pclsCh,_unSessionID,_unTransID,&m_stRefreshT);
	}
	void fnStopRefresh()
	{
		m_unRefreshTransID = 0;
		for(unsigned int i=0;i<PDE_POOL_MAX;i++)
		{
			if(m_arrSession[i].m_eType == E_PDE_REQ_INTERIM) m_arrSession[i].fnClear();
		}
	}
	bool fnIsProcRefresh(unsigned int _unTransID)
	{
		if(m_eState >= E_PDE_ST_STOP_REQ) return false;
		if(_unTransID == m_unRefreshTransID) return true;
		return false;
	}
	bool fnIsEstablishCharging()
	{
		if(m_eState >= E_PDE_ST_START_REQ && m_eState <= E_PDE_ST_INTERIM) return true;
		return false;
	}
	bool fnIsExistTimer(unsigned int _unTransID)
	{
		if(_unTransID ==0) return false;
		for(unsigned int i=0;i<PDE_POOL_MAX;i++)
		{
			if(m_arrSession[i].m_bUse && m_arrSession[i].m_unTransID == _unTransID) return true;
		}
		return false;
	}
	bool fnProcStatus(EPdeStatus _eSt)
	{
		if(m_eState == E_PDE_ST_NONE)
		{
			if(_eSt == E_PDE_ST_START_REQ)
			{
				m_eState = _eSt;	return true;
			}
		}
		else if(m_eState == E_PDE_ST_START_REQ)
		{
			if(_eSt >= E_PDE_ST_START_RSP)
			{
				m_eState = _eSt;	return true;
			}
		}
		else if(m_eState == E_PDE_ST_START_RSP)
		{
			if(_eSt >= E_PDE_ST_INTERIM)
			{
				m_eState = _eSt;	return true;
			}
		}
		else if(m_eState == E_PDE_ST_INTERIM)
		{
			if(_eSt >= E_PDE_ST_INTERIM)
			{
				m_eState = _eSt;	return true;
			}
		}
		else if(m_eState == E_PDE_ST_STOP_REQ)
		{
			if(_eSt == E_PDE_ST_STOP_RSP)
			{
				m_eState = _eSt;	return true;
			}
		}
		else if(m_eState == E_PDE_ST_STOP_RSP)
		{
			return false;
		}
		return false;
	}
	unsigned int m_unSeq;
	unsigned int m_unSessionID;
	unsigned int m_unRefreshTransID;
	struct timespec m_stRefreshT;
	EPdeStatus m_eState;
	PdePool_t m_arrSession[PDE_POOL_MAX];
	stAccReq m_stReq;
	void fnDebug(RUString & _rclsDebug)
	{
		_rclsDebug<<"  < Pde Pools Info\r\n"
			<<"    m_unSeq = "<<RUIntString(m_unSeq)<<"\r\n"
			<<"    m_unSessionID = "<<RUIntString(m_unSessionID)<<"\r\n"
			<<"    m_unRefreshTransID = "<<RUIntString(m_unRefreshTransID)<<"\r\n"
			<<"    m_stRefreshT.tv_sec = "<<RUIntString(m_stRefreshT.tv_sec)<<"\r\n"
			<<"    m_stRefreshT.tv_nsec = "<<RUIntString(m_stRefreshT.tv_nsec/1000000)<<"\r\n"
			<<"    m_eState = "<<g_arrPdeStatus[m_eState]<<"\r\n";
		for(int i=0;i<PDE_POOL_MAX;i++) m_arrSession[i].fnDebug(_rclsDebug);
		_rclsDebug<<"  >\r\n";
	}
}PdePools_t;
typedef struct IBCPdeInfo_t
{		
	IBCPdeInfo_t(){fnClear();}
	void fnClear()
	{
		memset(this,0x00,sizeof(IBCPdeInfo_t));
	}
	void fnStart()
	{
		m_eCauseCode = E_PDE_CAUSE_CODE_2XX;
		m_eSubServiceReInv = -1;
	}
	void fnSetNetworkInfo(const char * _pszNetString)
	{
		fnRUStrnCpy(m_szNetworkInfo,_pszNetString,256);
	}
	void fnReLoad(unsigned int _unSessionID){m_stPools.fnReLoad(_unSessionID);}
	bool fnIsExistTimer(unsigned int _unTransID){return m_stPools.fnIsExistTimer(_unTransID);}
	unsigned int m_unPdeID;
	unsigned int m_uiCharged;
	EIbcSubService_t m_eSubService;
	#if defined(DEF_R120) && defined(DEF_FLEXIBLE_CHARGE_CONFIG)
	unsigned int m_eICOperator;
	unsigned int m_eOGOperator;
	#else
	EIbcOperator_t m_eICOperator;
	EIbcOperator_t m_eOGOperator;
	#endif
	EPdeCauseCode_t m_eCauseCode;
	#if defined(DEF_R120) && defined(DEF_FLEXIBLE_CHARGE_CONFIG)
	unsigned int m_unChargingIDC;
	unsigned int m_unRoamingIDC;
	#endif
	int m_eSubServiceReInv;         // change name      m_eSubServiceReInv --> 
	char m_szRemoteICID[256];
	char m_szLocalICID[256];
	char m_szNetworkInfo[256];
	PdePools_t m_stPools;
	void fnDebug(RUString &_rclsDebug)
	{
		_rclsDebug<<"{\r\n  PDE Info\r\n"
					<<"  m_unPdeID = "<<RUIntString(m_unPdeID)<<"\r\n"
					<<"  m_uiCharged = "<<RUIntString(m_uiCharged)<<"\r\n"
					<<"  m_eSubService = "<<g_fnStringIbcSubService(m_eSubService)<<"\r\n"
					<<"  m_eICOperator = "<<g_fnStringIbcOperator(m_eICOperator)<<"\r\n"
					<<"  m_eOGOperator = "<<g_fnStringIbcOperator(m_eOGOperator)<<"\r\n"
					<<"  m_eCauseCode = "<<g_fnStringIbcPdeCauseCode(m_eCauseCode)<<"\r\n"
					//<<"  m_eSubServiceReInv = "<<RUIntString(m_eSubServiceReInv)<<"\r\n"
					<<"  m_szRemoteICID = "<<m_szRemoteICID<<"\r\n"
					<<"  m_szLocalICID = "<<m_szLocalICID<<"\r\n"
					<<"  m_szNetworkInfo = "<<m_szNetworkInfo<<"\r\n";
					m_stPools.fnDebug(_rclsDebug);
		_rclsDebug<<"}\r\n";
	}	
}IBCPdeInfo_t;
/***************************** IBC App Session Data ***************************************/
#ifdef DEF_R131
typedef enum
{
	E_ENUM_CFG_ENABLE_NPDB = 0,
	E_ENUM_CFG_ENABLE_HLR,
	E_ENUM_CFG_QUERY_OK,
	E_ENUM_CFG_NPDB_RESULT,
	E_ENUM_CFG_HLR_RESULT,
	E_ENUM_CFG_NPDB_REASON,
	E_ENUM_CFG_HLR_REASON,
	E_ENUM_CFG_NPDB_RN,
	E_ENUM_CFG_NPDB_RCFDN,
	E_ENUM_CFG_PFX_NUMBER,
	E_ENUM_CFG_PFX_DOMAIN,
	E_ENUM_CFG_PFX_CODE_TYPE,
	E_ENUM_CFG_HLR_CODEC_INFO,
	E_ENUM_CFG_HLR_PC,
	E_ENUM_CFG_HLR_IMG,
	E_ENUM_CFG_HLR_DRTN,
	E_ENUM_CFG_HLR_TRANSFER_NUMBER,
	E_ENUM_CFG_HLR_TRANSFER_REASON,
	E_ENUM_CFG_REQUEST_NUMBER,
	E_ENUM_CFG_TEL_IOR_OPT,
	E_ENUM_CFG_ORIG_IOI,
	E_ENUM_CFG_MAX
}EEnumConf_t;
#endif
typedef struct IBCAppData_t
{
	IBCAppData_t()
	{
		m_uiROID=0;m_eSdpState = eDum::E_SDP_STATE_IDLE;
		m_eRoute = E_ROUTE_OP_NONE;
	}
	~IBCAppData_t(){}
	//===========> CDR Info
	IBCCdrData_t m_stCDR;
	//==========> Route Operation Info
	unsigned int m_uiROID;
	ERouteOp_t m_eRoute;
	//==========> Stack Info
	eDum::ESdpState_t m_eSdpState;
	App5Tuple_t m_stICNode;
	App5Tuple_t m_stOGNode;
	//==========>   RTE Info
	IBCRteInfo_t m_stRTE;
	//===========> PDE Info
	IBCPdeInfo_t m_stPDE;
	//==========> TrGW Info
	IBCTrGWInfo_t m_stTrGW;
	EIbcCallDirection_t m_eOrigDir;
	EIbcCallDirection_t m_eTermDir;
	char m_szOrigName[40];
	char m_szTermName[40];
	SipRoute_t::EMgwRouteFlag_t m_bOrigRouted;   // media
	SipRoute_t::EMgwRouteFlag_t m_bTermRouted; // media
	char m_szUA[128];
	#ifdef DEF_R131
	bool m_bEnableHlr;
	bool m_bEnableNpdb;
	bool m_bQueryOk;
	unsigned int m_unQueryErrCode;
	unsigned int  m_unHlrCodecInfo;
	unsigned int m_unHlrImg;
	unsigned int m_unHlrTransferReason;
	bool m_bNuguPrefixCall;
	char m_szRequestNumber[60];
	char m_szNpdbResult[60];
	char m_szHlrResult[60];
	char m_szNpdbReason[60];
	char m_szHlrReason[60];
	char m_szNpdbRN[60];
	char m_szNpdbRCFDN[60];
	char m_szPfxDomain[60];
	char m_szPfxNumber[60];
	char m_szPfxCodeType[60];
	char m_szHlrPC[60];
	char m_szHlrDrtn[60];
	char m_szHlrTransferNumber[60];
	char m_szTelIorOpt[60];
	
	void m_fnCdrEnum(RUString & _rclsBuild)
	{
		if(m_bEnableNpdb)
		{
			if(m_bQueryOk)
			{
				if(strcmp(m_szNpdbReason,"OK")==0)
					_rclsBuild<<"NPDB_RESULT=1,";
				else
					_rclsBuild<<"NPDB_RESULT=0,";
				if(m_szNpdbResult[0] !=0x00)
					_rclsBuild<<"NPDB_CODE="<<m_szNpdbResult<<",";	
			}
			else
			{
				_rclsBuild<<"NPDB_RESULT=0,";
			}
		}
		if(m_bEnableHlr)
		{
			if(strcmp(m_szHlrReason,"OTHER_DOMAIN") !=0)
			{
				if(m_bQueryOk)
				{
					if(strcmp(m_szHlrReason,"OK")==0)
						_rclsBuild<<"HLR_RESULT=1,";
					else
						_rclsBuild<<"HLR_RESULT=0,";
					if(m_szHlrResult[0] !=0x00)
						_rclsBuild<<"HLR_CODE="<<m_szHlrResult<<",";
				}
				else
				{
					_rclsBuild<<"HLR_RESULT=0,";
				}
			}
		}
	}
	void m_fnSetEnumInfo(EEnumConf_t _eT, eSipUtil::KString _clsVal)
	{
		switch (_eT)
		{
			case E_ENUM_CFG_ENABLE_NPDB:
			{
				m_bEnableNpdb = (eSipUtil::KBOOL)_clsVal;
			}
			break;
			case E_ENUM_CFG_ENABLE_HLR:
			{
				m_bEnableHlr = (eSipUtil::KBOOL)_clsVal;
			}
			break;
			case E_ENUM_CFG_QUERY_OK:
			{
				m_bQueryOk = (eSipUtil::KBOOL)_clsVal;
			}
			break;
			case E_ENUM_CFG_NPDB_RESULT:
			{
				memset(&m_szNpdbResult[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szNpdbResult[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_HLR_RESULT:
			{
				memset(&m_szHlrResult[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szHlrResult[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_NPDB_REASON:
			{
				memset(&m_szNpdbReason[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szNpdbReason[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_HLR_REASON:
			{
				memset(&m_szHlrReason[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szHlrReason[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_NPDB_RN:
			{
				memset(&m_szNpdbRN[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szNpdbRN[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_NPDB_RCFDN:
			{
				memset(&m_szNpdbRCFDN[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szNpdbRCFDN[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_PFX_NUMBER:
			{
				memset(&m_szPfxNumber[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szPfxNumber[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_PFX_DOMAIN:
			{
				memset(&m_szPfxDomain[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szPfxDomain[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_PFX_CODE_TYPE:
			{
				memset(&m_szPfxCodeType[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szPfxCodeType[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_HLR_CODEC_INFO:
			{
				m_unHlrCodecInfo = (eSipUtil::KUINT)_clsVal;
			}
			break;
			case E_ENUM_CFG_HLR_PC:
			{
				memset(&m_szHlrPC[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szHlrPC[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_HLR_IMG:
			{
				m_unHlrImg = (eSipUtil::KUINT)_clsVal;
			}
			break;
			case E_ENUM_CFG_HLR_DRTN:
			{
				memset(&m_szHlrDrtn[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szHlrDrtn[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_HLR_TRANSFER_NUMBER:
			{
				memset(&m_szHlrTransferNumber[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szHlrTransferNumber[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_HLR_TRANSFER_REASON:
			{
				m_unHlrTransferReason = (eSipUtil::KUINT)_clsVal;
			}
			break;
			case E_ENUM_CFG_REQUEST_NUMBER:
			{
				memset(&m_szRequestNumber[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szRequestNumber[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			case E_ENUM_CFG_TEL_IOR_OPT:
			{
				memset(&m_szTelIorOpt[0],0x00,60);
				if(_clsVal.m_unRealLen!=0)
					strncpy(&m_szTelIorOpt[0],(eSipUtil::KCSTR)_clsVal,59);
			}
			break;
			default:
			break;
		}
	}
	#endif

	void fnReLoad(unsigned int _unSessionID)
	{
		m_stPDE.fnReLoad(_unSessionID);
		m_stTrGW.fnReLoad(_unSessionID);
	}
	void fnStart()
	{
		m_stCDR.fnStart();
		m_stPDE.fnStart();
		m_stTrGW.fnStart();
	}
	void fnDebug(RUString &_rclsDebug)
	{
		_rclsDebug<<"App Info\r\nm_szOrigName = "
					<<m_szOrigName<<", m_szTermName = "<<m_szTermName<<"\r\n"
					<<"m_uiROID = "<<RUIntString(m_uiROID)
					<<", m_eRoute = "<<fnRuleApiStringRouteOp(m_eRoute)<<"\r\n";
		m_stTrGW.fnDebug(_rclsDebug);
		m_stPDE.fnDebug(_rclsDebug);
	}
}IBCAppData_t;
#ifdef DEF_R131
typedef enum
{
	E_VECTOR_OPT_NONE = 0,
	E_VECTOR_OPT_NUGU_PREFIX_CALL,
	E_VECTOR_OPT_NCB_MINE_CALL,
	E_VECTOR_OPT_NCB_OTHER_CALL,
	E_VECTOR_OPT_NORMAL,
	E_VECTOR_OPT_MAX
}EVectorOpt;
inline const char * g_fnStringVerctorOpt(EVectorOpt _eT)
{
	switch(_eT)
	{
		case E_VECTOR_OPT_NONE: return "E_VECTOR_OPT_NONE";
		case E_VECTOR_OPT_NUGU_PREFIX_CALL: return "E_VECTOR_OPT_NUGU_PREFIX_CALL";
		case E_VECTOR_OPT_NCB_MINE_CALL: return "E_VECTOR_OPT_NCB_MINE_CALL";
		case E_VECTOR_OPT_NCB_OTHER_CALL: return "E_VECTOR_OPT_NCB_OTHER_CALL";
		case E_VECTOR_OPT_NORMAL: return "E_VECTOR_OPT_NORMAL";
		case E_VECTOR_OPT_MAX: return "E_VECTOR_OPT_MAX";
		default: return "E_VECTOR_OPT_NONE";
	};
	return "E_VECTOR_OPT_NONE";
}
#endif

class TempSDPInfo : public RUObject
{
	public:
		TempSDPInfo();
		~TempSDPInfo();
		void fnClear();
		bool fnSetOrigInfo(resip::SipMessage * _pclsMsg,unsigned int _unTransID);
		bool fnSetTgaInfo(const char * _pszSDP,unsigned int _unSDPLen,unsigned int _unTransID);
		bool fnMatch(resip::SipMessage * _pclsMsg,unsigned int _unTransID);
		RUString m_clsOrigSDP;
		RUString m_clsTgaSDP;
		unsigned int m_unOrigSDPLen;
		unsigned int m_unTgaSDPLen;
		unsigned int m_unTransID;
		bool m_bRequest;
		unsigned int m_unRspCode;
		resip::MethodTypes m_eMethod;
};
class TempSDPInfos
{
	public:
		TempSDPInfos();
		~TempSDPInfos();
		void fnClear();
		bool fnSetOrigInfo(resip::SipMessage * _pclsMsg,unsigned int _unTransID);
		bool fnSetTgaInfo(const char * _pszSDP,unsigned int _unSDPLen,unsigned int _unTransID);
		TempSDPInfo * fnFindInfo(resip::SipMessage * _pclsMsg,unsigned int _unTransID);
	private:
		RUList m_listInfo;
};
#if defined(DEF_R120) && defined(DEF_CDR_REASON)
class CdrReason
{
	public:
		CdrReason(){m_bSet = false;}
		~CdrReason(){}
		#ifdef DEF_CDR_REASON_BUG_FIXED
		void m_fnClear(){m_clsCause = NULL;m_clsText=NULL;m_bSet=false;}
		#endif
		void m_fnParse(resip::SipMessage * _pclsMsg)
		{
			if(_pclsMsg==NULL || m_bSet==true) return;
			if(fnMsgApiIsExistHeader(_pclsMsg,"Reason"))
			{
				m_bSet = true;
				RUArrayString clsReason;RuListItem_t clsItor;
				fnMsgApiGetHeaderLine(_pclsMsg,"Reason",&clsReason);
				char * pszReason = clsReason.fnBegin(clsItor, NULL);
				char * pszOptReason = NULL;
				while(pszReason)
				{
					pszOptReason = RUString::fnSkipString(pszReason," "); // Reason:         Q.850  <-- Skipp White Space
					if(pszOptReason && fnRUStrnCmp(pszOptReason,"Q.850",5)==0)
					{
						RuSipParams clsParams;RuListItem_t clsIter;
						RuSipParam *pclsCause = NULL; RuSipParam *pclsText = NULL;
						for(int i =0;i<fnRUStrLen(pszOptReason);i++)
							clsParams.fnParse(pszOptReason[i]);
						pclsCause = clsParams.fnFindParam("cause");
						if(pclsCause) m_clsCause = (RUSTR)pclsCause->m_clsVal;
						pclsText = clsParams.fnFindParam("text");
						if(pclsText)
						{
							char * pszTextOpt = RUString::fnGetOptimizeString((RUSTR)pclsText->m_clsVal, " \"");
							if(pszTextOpt)
							{
								m_clsText = pszTextOpt;
								m_fnReplaceChar((RUSTR)m_clsText);
							}
						}
						break;
					}
					pszReason = clsReason.fnNext(clsItor,NULL);
				}
			}
		}
		RUString m_clsCause;
		RUString m_clsText;
	private:
		void m_fnReplaceChar(char * _pszSrc)
		{
			for(int i=0;i<fnRUStrLen(_pszSrc);i++)
			{
				_pszSrc[i] = toupper(_pszSrc[i]);
				if(_pszSrc[i]==' ' || _pszSrc[i]==',') _pszSrc[i] = '_';
			}
		}
		bool m_bSet;
};
#endif
class AppSession
{
	public:
		AppSession();
		~AppSession();
		void fnLoadSession(void * _pvAppData,unsigned int _unIndex,
																eDum::EDialogMethodType_t _eType);
		void fnReLoadSession();
		void fnClear(bool _bNodeClear = false);
		void fnWriteCdr(RUStatLog * _pclsCdr,char * _pszSipHistory);
		ERoutingResult_t fnSetRO(eSIP::EventResult & _rclsEventResult,ERouteOp_t _eOP,unsigned int _uiROID, RouteOP * _pclsROP = NULL);
		RouteOP * fnGetRO();
		void fnSetTrGWEquipment(unsigned int _unTrGWIndex);
		void fnSetOGNode();
		void fnStartSession();
		void fnFinishSession();
		void fnSetTrGWReason(const char * _pszReason,int _nReason);
		int fnCopyRTEInfo(ERouteOp_t _eRoute, unsigned int _uiSeqID);
		void fnCopyUasInviteNetworkInfo(resip::SipMessage * _pclsMsg);
		void fnCopyRemoteChargingICID(resip::SipMessage * _pclsMsg);
		void fnBuildUacInviteChargingICID(resip::SipMessage * _pclsMsg);
		void fnBuildExistChargingICID(StackStateCbInfo_t & _rclsInfo);
		void fnCopyUserAgent(resip::SipMessage * _pclsMsg);
		RUTcpChannel * fnGetTrGWChannel();
		RUTcpChannel * fnGetPdeChannel();
		eDum::Session * fnGetDumSession();
		const char * fnGetCallID();
		const char * fnGetCaller();
		const char * fnGetCallee();
		void fnGetFromUri(char * _pszSrc, unsigned int _unLen);
		void fnGetToUri(char * _pszSrc, unsigned int _unLen);
		bool fnGetDialogLOC(eDum::Dialog * _pclsDialog,int * _piRteID,EIbcCallDirection_t * _peLOC,
																char ** _ppszName, bool * _pbRouted);
		const resip::TraceInfo_t & fnGetTraceInfo();
		bool fnIsTrace();
		unsigned int fnGetCdrSipMsgNum();
		char *fnBuildCdrSipMsg(char * _pszLog,unsigned int _unSize);
		void fnSetEndHost(ECdrEndHost_t _eT);
		void fnSetIBCError(EIBCErrors_t _eErr,eSIP::EventResult & _rclsEventResult);
		#if defined(DEF_R120) && defined(DEF_FAULT_MSG)
		void fnSetDetailError(EIBCNewErr_t _eErr,const char * _pszErr,...);
		void fnSendFault();
		#endif
		void fnBuildBasicChargingICID(StackStateCbInfo_t & _rclsInfo,bool _bInitReq);
		#ifdef DEF_R131
		void fnGetRteGroup(bool _bIncomming, SipRouteGroup_t * _pstGroup);
		void fnBuildVectorOpt(StackStateCbInfo_t & _rclsInfo, EVectorOpt _eOpt, bool _bInitReq);
		#endif
		IBCAppData_t * m_pclsData;		      // Shared Memory Pointer,                                      NOT CHANGED
		unsigned int m_unMyIndex;                  // Session Index Static Value,                                 NOT CHANGED
		eDum::EDialogMethodType_t m_eType;   // Session Method Type(Inv,Reg,Sub,StandAlone),    NOT CHANGED
		TempSDPInfos m_clsTmpSDPInfo;
		RouteOP * m_pclsRouteOp;
		void fnDebug(RUString &_rclsDebug)
		{
			_rclsDebug<<"========== App Session Info ["<<eDum::getDialogMethodTypeString(m_eType)
						<<":"<<RUIntString(m_unMyIndex)<<"]============\r\n";
			m_pclsData->fnDebug(_rclsDebug);			
		}
		#if defined(DEF_R120) && defined(DEF_CDR_REASON)
		CdrReason m_clsReason;
		#endif
		#if defined(DEF_R120) && defined(DEF_FAULT_MSG)
		void fnGetInfo(RUString &_rclsString);
		#endif
};

#define DEF_COMPILE_CHECK(a,b)    (a>=b ?1:0)
#define DEF_DUM_SESSION_SIZE   eDum::Session::E_CONST_MAX_SESSION_APP_DATA_SIZE
#define DEF_APP_SESSION_SIZE sizeof(IBCAppData_t)
#define DEF_SESS_CHECK  DEF_COMPILE_CHECK(DEF_APP_SESSION_SIZE,DEF_DUM_SESSION_SIZE)
#endif

