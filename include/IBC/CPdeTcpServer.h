
#ifndef ESIP_PDETCPSERVER_HXX
#define ESIP_PDETCPSERVER_HXX

#include "rutil/ThreadIf.hxx"
#include "rutil/ConfigParse.hxx"
#include "rutil/Data.hxx"

#include "eRule/CRuleUtiles.h"
#include "eRule/CRUSocket.h"
#include "eRule/CRUTcpServer.h"

#include "pde_msg.h"
#include "PdeErrorTable.h"
#include "IBCDefine.h"

/****************************** Tcp Server **********************************************/
extern const char * g_arrPdePkTString[PDE_ACCOUNT_FD+1];
inline const char * g_fnPdePkType(unsigned int _eT)
{
	if(_eT >= PDE_CAEX && _eT <= PDE_ACCOUNT_FD)
		return g_arrPdePkTString[_eT-1];
	else return "Unknown";
}
extern const char * g_arrPdePksTString[PDEST_RES+1];
inline const char * g_fnPdePksType(unsigned int _eT)
{
	if(_eT >= PDEST_INFO && _eT <= PDEST_RES)
		return g_arrPdePksTString[_eT];
	else return "Unknown";
}
extern const char * g_arrRecTypeString[stAccReq::STOP_RECORD+1];
inline const char * g_fnPdeRecType(unsigned short _eRecT)
{
	if(_eRecT >= stAccReq::NONE_RECORD && _eRecT <= stAccReq::STOP_RECORD )
		return g_arrRecTypeString[_eRecT];
	else return "Unknown";
}
typedef enum
{
	E_PDE_SUB_E164=0,
	E_PDE_SUB_IMSI = 1,
	E_PDE_SUB_SIP_URI = 2,
	E_PDE_SUB_NAI = 3,
	E_PDE_SUB_PRIVATE = 4,
}EPdeSubscription_t;
extern const char * g_arrPdeSubString[E_PDE_SUB_PRIVATE+1];
inline const char * g_fnStringPdeSub(unsigned int _eT)
{
	if(_eT >= E_PDE_SUB_E164 && _eT <= E_PDE_SUB_PRIVATE)
		return g_arrPdeSubString[_eT];
	else return "Unknown";
}
typedef enum
{
	E_IBC_OPERATOR_NONE = 0,
	E_IBC_OPERATOR_SKT = 11,
	E_IBC_OPERATOR_KT = 16,
	E_IBC_OPERATOR_LGT = 19,
	E_IBC_OPERATOR_SKTELINK_HDV = 774,
	E_IBC_OPERATOR_SKB = 901
}EIbcOperator_t;
//101:선별통화 102:Group통화 103:법인통화
//203 : MIM 3.0 , 205 : RCS채팅, 207 : RCS통화중 공유
//301 : Non-SIP MIM, 302 : GTS, 303 : MRC, 304 : SIP영상전화,
//305 : SIP MIM, 306 : IBC Call, 307 : VoIP, 309 : 영상통화 EV
//310 : VoLTE, 401 : Video Sharing, 801 : Beacon서비스, 802 : 파자마 5
typedef enum
{
	E_IBC_SERVICE_NONE = 0,
	E_IBC_SERVICE_SELECTIV_CALL = 101,
	E_IBC_SERVICE_GROUP_CALL = 102,
	E_IBC_SERVICE_CORPORATE_CALL = 103,
	E_IBC_SERVICE_MIM_CALL = 203,
	E_IBC_SERVICE_RCS_CALL = 205,
	E_IBC_SERVICE_RCS_SHARE = 207,
	E_IBC_SERVICE_MIM_NON_SIP = 301,
	E_IBC_SERVICE_GTS_CALL = 302,
	E_IBC_SERVICE_MRC_CALL = 303,
	E_IBC_SERVICE_VIDEO_SIP_CALL = 304,
	E_IBC_SERVICE_MIM_SIP = 305,
	E_IBC_SERVICE_IBC_CALL = 306,
	E_IBC_SERVIE_VOIP_CALL = 307,
	E_IBC_SERVICE_VIDEO_CALL = 309,
	E_IBC_SERVICE_VOLTE = 310,
	E_IBC_SERVICE_VIDEO_SHARE = 401,
	E_IBC_SERVICE_BEACON = 801,
	E_IBC_SERVICE_PJM = 802
}EIbcService_t;
//3101 : VoLTE 음성호
//3102 : VoLTE 영상호
//3114 : VoLTE Audio->Video 호전환
//3121 : 영상사서함
typedef enum
{
	E_IBC_SUBSERVICE_NONE = 0,
	E_IBC_SUBSERVICE_VOLTE_VOICE = 3101,
	E_IBC_SUBSERVICE_VOLTE_VIDEO = 3102,
	E_IBC_SUBSERVICE_CALL_SWITCH = 3114,
	E_IBC_SUBSERVICE_VIC = 3121
}EIbcSubService_t;
typedef enum
{
	E_PDE_REQ_NONE  = 0,
	E_PDE_REQ_START = 1,
	E_PDE_REQ_INTERIM = 2,
	E_PDE_REQ_STOP = 3
}EPdeReq_t;
inline const char * g_fnStringPdeReq(EPdeReq_t _eT)
{
	switch(_eT)
	{
		case E_PDE_REQ_NONE: return "E_PDE_REQ_NONE";
		case E_PDE_REQ_START: return "E_PDE_REQ_START";
		case E_PDE_REQ_INTERIM: return "E_PDE_REQ_INTERIM";
		case E_PDE_REQ_STOP: return "E_PDE_REQ_STOP";
	};
	return "NONE";
}
inline const char * g_fnStringIbcOperator(unsigned short _eT)
{
	switch(_eT)
	{
		case E_IBC_OPERATOR_NONE: return "E_IBC_OPERATOR_NONE";
		case E_IBC_OPERATOR_SKT: return "E_IBC_OPERATOR_SKT";
		case E_IBC_OPERATOR_KT: return "E_IBC_OPERATOR_KT";
		case E_IBC_OPERATOR_LGT: return "E_IBC_OPERATOR_LGT";
		case E_IBC_OPERATOR_SKB: return "E_IBC_OPERATOR_SKB";
		default: return "NONE";
	};
	return "NONE";
}
inline const char * g_fnStringIbcService(EIbcService_t _eT)
{
	switch(_eT)
	{
		case E_IBC_SERVICE_NONE: return "E_IBC_SERVICE_NONE";
		case E_IBC_SERVICE_SELECTIV_CALL: return "E_IBC_SERVICE_SELECTIV_CALL";
		case E_IBC_SERVICE_GROUP_CALL: return "E_IBC_SERVICE_GROUP_CALL";
		case E_IBC_SERVICE_CORPORATE_CALL: return "E_IBC_SERVICE_CORPORATE_CALL";
		case E_IBC_SERVICE_MIM_CALL: return "E_IBC_SERVICE_MIM_CALL";
		case E_IBC_SERVICE_RCS_CALL: return "E_IBC_SERVICE_RCS_CALL";
		case E_IBC_SERVICE_RCS_SHARE: return "E_IBC_SERVICE_RCS_SHARE";
		case E_IBC_SERVICE_MIM_NON_SIP: return "E_IBC_SERVICE_MIM_NON_SIP";
		case E_IBC_SERVICE_GTS_CALL: return "E_IBC_SERVICE_GTS_CALL";
		case E_IBC_SERVICE_MRC_CALL: return "E_IBC_SERVICE_MRC_CALL";
		case E_IBC_SERVICE_VIDEO_SIP_CALL: return "E_IBC_SERVICE_VIDEO_SIP_CALL";
		case E_IBC_SERVICE_MIM_SIP: return "E_IBC_SERVICE_MIM_SIP";
		case E_IBC_SERVICE_IBC_CALL: return "E_IBC_SERVICE_IBC_CALL";
		case E_IBC_SERVIE_VOIP_CALL: return "E_IBC_SERVIE_VOIP_CALL";
		case E_IBC_SERVICE_VIDEO_CALL: return "E_IBC_SERVICE_VIDEO_CALL";
		case E_IBC_SERVICE_VOLTE: return "E_IBC_SERVICE_VOLTE";
		case E_IBC_SERVICE_VIDEO_SHARE: return "E_IBC_SERVICE_VIDEO_SHARE";
		case E_IBC_SERVICE_BEACON: return "E_IBC_SERVICE_BEACON";
		case E_IBC_SERVICE_PJM: return "E_IBC_SERVICE_PJM";
		default: return "NONE";
	};
	return "NONE";
}
inline const char * g_fnStringIbcSubService(EIbcSubService_t _eT)
{
	switch(_eT)
	{
		case E_IBC_SUBSERVICE_NONE: return "E_IBC_SUBSERVICE_NONE";
		case E_IBC_SUBSERVICE_VOLTE_VOICE: return "E_IBC_SUBSERVICE_VOLTE_VOICE";
		case E_IBC_SUBSERVICE_VOLTE_VIDEO: return "E_IBC_SUBSERVICE_VOLTE_VIDEO";
		case E_IBC_SUBSERVICE_CALL_SWITCH: return "E_IBC_SUBSERVICE_CALL_SWITCH";
		case E_IBC_SUBSERVICE_VIC: return "E_IBC_SUBSERVICE_VIC";
		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_PDE_CAUSE_CODE_NORMAL = 0,
	E_PDE_CAUSE_CODE_3XX = -300,
	E_PDE_CAUSE_CODE_2XX = -200,
	E_PDE_CAUSE_CODE_REGI = -3,
	E_PDE_CAUSE_CODE_SUBDIALOG = -2,
	E_PDE_CAUSE_CODE_SUCCESS = -1
}EPdeCauseCode_t;
/*
	700 : 일반과금 + (최소 10초 ~ 최대 55초) 추가 과금된 상황 
	800 : 한쪽 NW가 끊겼을경우 일반 과금+10초 추가 과금된 상황(VoLTE추가)
	900 : 양쪽 NW가 모두 끊겼을 경우 일반과금 + (최소 90초 ~ 최대 180초(VoLTE추가)
	추가 과금된 상황 
	[실패]
	1: Unspecified Error
	4xx: 4xx Request fail
*/
inline const char * g_fnStringIbcPdeCauseCode(EPdeCauseCode_t _eT)
{
	switch(_eT)
	{
		case E_PDE_CAUSE_CODE_NORMAL: return "E_PDE_CAUSE_CODE_NORMAL";
		case E_PDE_CAUSE_CODE_3XX: return "E_PDE_CAUSE_CODE_3XX";
		case E_PDE_CAUSE_CODE_2XX: return "E_PDE_CAUSE_CODE_2XX";
		case E_PDE_CAUSE_CODE_REGI: return "E_PDE_CAUSE_CODE_REGI";
		case E_PDE_CAUSE_CODE_SUBDIALOG: return "E_PDE_CAUSE_CODE_SUBDIALOG";
		case E_PDE_CAUSE_CODE_SUCCESS: return "E_PDE_CAUSE_CODE_SUCCESS";
		default: return "NONE";
	};
	return "NONE";
}
typedef enum
{
	E_PDE_ROLE_ORIGINATION = 0,
	E_PDE_ROLE_TERMINATING,
	E_PDE_ROLE_PROXY,
	E_PDE_ROLE_B2BUA,
	E_PDE_ROLE_IBCF,
}EPdeRoleofNode_t;
extern const char * g_arrPdeRoleString[E_PDE_ROLE_IBCF+1];
inline const char * g_fnStringPdeRole(unsigned short _eRole)
{
	if(_eRole >= E_PDE_ROLE_ORIGINATION && _eRole <= E_PDE_ROLE_IBCF)
		return g_arrPdeRoleString[_eRole];
	else return "Unknown";
}
typedef enum
{
	E_PDE_NODE_S_CSCF = 0,
	E_PDE_NODE_P_CSCF,
	E_PDE_NODE_I_CSCF,
	E_PDE_NODE_MRFC,
	E_PDE_NODE_MGCF,
	E_PDE_NODE_BGCF,
	E_PDE_NODE_AS,
	E_PDE_NODE_IBCF
}EPdeNode_t;
extern const char * g_arrPdeNodeString[E_PDE_NODE_IBCF+1];
inline const char * g_fnStringPdeNode(unsigned short _eNode)
{
	if(_eNode >= E_PDE_NODE_S_CSCF && _eNode <= E_PDE_NODE_IBCF)
		return g_arrPdeNodeString[_eNode];
	else return "Unknown";
}
inline char * g_fnStringSKTInfo(stSKTSInfo * _pstSrv,char * _pszDebug,unsigned int _unLen)
{
	fnRUStrnCat(_pszDebug,_unLen,"  <stSKTSInfo>\r\n  {\r\n"\
	"    flag = 0x%x\r\n"\
	"    nRoamingInd = 0x%x\r\n  }\r\n",
	_pstSrv->flag,
	_pstSrv->nRoamingInd);
	return _pszDebug;	
}
inline char * g_fnStringSKBInfo(stSKBSInfo * _pstSrv,char * _pszDebug,unsigned int _unLen)
{
	fnRUStrnCat(_pszDebug,_unLen,"  <stSKBSInfo>\r\n  {\r\n"\
	"    flag = 0x%x\r\n"\
	"    szDialedNum = %s\r\n  }\r\n",
	_pstSrv->flag,
	_pstSrv->szDialedNum);
	return _pszDebug;	
}
inline char * g_fnStringPdeHeader(SVCComMsgHdr * _pstHdr,char * _pszDebug,unsigned int _unLen)
{
/*
hdr.uiType = PDE_ACCOUNT;
		hdr.uiSubType = PDEST_REQ;
*/
	fnRUStrnCat(_pszDebug,_unLen,"<SVCComMsgHdr>\r\n{\r\n"\
	"  ====> uiType Enum\r\n"\
	"  * CAEX:1,ACC:2,RE_ACC:3,DW:4,DP:5,ACC_FD:6\r\n"\
	"  ====> uiSubType Enum\r\n"\
	"  * INFO:0,REQ:1,RES:2\r\n"\
	"  uiMagicCookie = 0x%x\r\n"\
	"  uiMsgLen = %u\r\n"\
	"  uiType = %s(%u)\r\n"\
	"  uiSubType = %s(%u)\r\n"\
	"  uiCompId = %u\r\n"\
	"  uiCompSesId = %u\r\n"\
	"  uiAsId = %u\r\n"\
	"  uiAsSesId = %u\r\n"\
	"  szSesDesc = %s\r\n"\
	"  uiReasonCode = %s(%u,0x%x)\r\n}\r\n",
	_pstHdr->uiMagicCookie,
	_pstHdr->uiMsgLen,
	g_fnPdePkType(_pstHdr->uiType),_pstHdr->uiType,
	g_fnPdePksType(_pstHdr->uiSubType),_pstHdr->uiSubType,
	_pstHdr->uiCompId,
	_pstHdr->uiCompSesId,
	_pstHdr->uiAsId,
	_pstHdr->uiAsSesId,
	_pstHdr->szSesDesc,
	//g_fnStringPdeError(_pstHdr->uiReasonCode),
	fnPdeApiStringError(_pstHdr->uiReasonCode),
	_pstHdr->uiReasonCode,
	_pstHdr->uiReasonCode);
	return _pszDebug;
}
inline char * g_fnStringPdeSrvInfo(stSrvInfo * _pstSrv,char * _pszDebug,unsigned int _unLen)
{
	fnRUStrnCat(_pszDebug,_unLen,"  <stSrvInfo>\r\n  {\r\n"\
	"    ====> flag Enum\r\n"\
	"    * ORIG_IOI:0x01,TERM_IOI:0x02,CAUSE_CODE:0x04\r\n"\
	"    ====> usRoleOfNode Enum\r\n"\
	"    * ORIGINATION_ROLE:0,TERMINATING_ROLE:1,PROXY_ROLE:2,PROXY B2BUA_ROLE:3,IBCF:4\r\n"\
	"    ====> usNodeFunctionality Enum\r\n"\
	"    * S-CSCF:0,P-CSCF:1,I-CSCF:2,MRCF:3,MGCF:4,BGCF:5,AS:6,IBCF:7\r\n"\
	"    ====> uiServiceType Enum\r\n"\
	"    * 203 : MIM 3.0 , 205 : RCS채팅, 207 : RCS통화중 공유\r\n"\
	"    * 301 : Non-SIP MIM, 302 : GTS, 303 : MRC, 304 : SIP영상전화\r\n"\
	"    * 305 : SIP MIM, 306 : IBC Call, 307 : VoIP, 309 : 영상통화 EV\r\n"\
	"    * 310 : VoLTE, 401 : Video Sharing, 801 : Beacon서비스, 802 : 파자마 5\r\n"\
	"    ====> uiServiceSubType Enum\r\n"\
	"    * 3101 : VoLTE 음성호\r\n"\
	"    * 3102 : VoLTE 영상호\r\n"\
	"    * 3114 : VoLTE Audio->Video 호전환\r\n"\
	"    * 3121 : 영상사서함\r\n"\
	"    flag = 0x%x\r\n"\
	"    szSIPMethod = %s\r\n"\
	"    usRoleOfNode = %s(%u)\r\n"\
	"    usNodeFunctionality = %s(%u)\r\n"\
	"    szCallingPartyAddress = %s\r\n"\
	"    szCalledPartyAddress = %s\r\n"\
	"    szAlternateChargedPartyAddress = %s\r\n"\
	"    szRequestPartyAddress = %s\r\n"\
	"    szIMSChargingId = %s\r\n"\
	"    uiServiceType = %s(%u)\r\n"\
	"    uiServiceSubType = %s(%u)\r\n"\
	"    szOrigIOI = %s\r\n"\
	"    szTermIOI = %s\r\n"\
	"    iCauseCode = %u\r\n  }\r\n",
	_pstSrv->flag,
	_pstSrv->szSIPMethod,
	g_fnStringPdeRole(_pstSrv->usRoleOfNode),_pstSrv->usRoleOfNode,
	g_fnStringPdeNode(_pstSrv->usNodeFunctionality),_pstSrv->usNodeFunctionality,
	_pstSrv->szCallingPartyAddress,
	_pstSrv->szCalledPartyAddress,
	_pstSrv->szAlternateChargedPartyAddress,
	_pstSrv->szRequestPartyAddress,
	_pstSrv->szIMSChargingId,
	g_fnStringIbcService((EIbcService_t)_pstSrv->uiServiceType),_pstSrv->uiServiceType,
	g_fnStringIbcSubService((EIbcSubService_t)_pstSrv->uiServiceSubType),_pstSrv->uiServiceSubType,
	_pstSrv->szOrigIOI,
	_pstSrv->szTermIOI,
	_pstSrv->iCauseCode);
	return _pszDebug;
}
/*
inline char * g_fnStringPdeTraceInfo(stPDETraceInfo * _pstTrace,char * _pszDebug,unsigned int _unLen)
{
	fnRUStrnCat(_pszDebug,_unLen,"  <stPDETraceInfo>\r\n  {\r\n"\
	"    nEnable = %u\r\n"\
	"    nTraceId = %u\r\n"\
	"    szCallId = %u\r\n"\
	"    nFromNodeId = %u\r\n"\
	"    nToNodeId = %u\r\n"\
	"    nLevel = %u\r\n"\
	"    nLocation = %u\r\n  }\r\n",
	_pstTrace->nEnable,
	_pstTrace->nTraceId,
	_pstTrace->szCallId,
	_pstTrace->nFromNodeId,
	_pstTrace->nToNodeId,
	_pstTrace->nLevel,
	_pstTrace->nLocation);
	return _pszDebug;
}
*/
typedef enum
{
	E_PDE_CALLRESULT_NONE = 0,
	E_PDE_CALLRESULT_BASIC = 1,
	E_PDE_CALLRESULT_DETAIL = 2,
}EPdeCallResult_t;
extern const char * g_arrPdeCallResultString[E_PDE_CALLRESULT_DETAIL+1];
inline const char * g_fnStringPdeCallResult(int _nResult)
{
	if(E_PDE_CALLRESULT_NONE<= _nResult && _nResult <= E_PDE_CALLRESULT_DETAIL) 
		return g_arrPdeCallResultString[_nResult];
	return "Unknown";
}
inline char * g_fnStringPdeAccReq(stAccReq * _pstAccReq,char * _pszDebug,unsigned int _unLen)
{
	fnRUStrnCat(_pszDebug,_unLen,"<stAccReq>\r\n{\r\n"\
			"  ====> flag Enum\r\n"\
			"  * SKT_SINFO:0x04, SKB_SINFO:0x08\r\n"\
			"  ====> usRecordType Enum\r\n"
			"  * NONE:0,EVENT:1,START:2,INTERIM:3,STOP:4\r\n"\
			"  ====> uiSubscriptionIdType Enum\r\n"\
			"  * 0:E164, 1:IMSI, 2:SIP_URI, 3:NAI, 4:PRIVATE\r\n"\
			"  ====> usOriginalDomainCode,usTermDomainCode Enum\r\n"\
			"  * 011: SKT, 016:KTF, 019:LGT, 901:SKB\r\n"\
			"  ====> uiChargingInd Enum\r\n"\
			"  * 0:비과금 대상, 1:과금 대상\r\n"\
			"  ====> iCallResultType Enum\r\n"\
			"  * CALL_RESULT:0x01, DETAIL_CAUSE_CODE:0x02\r\n"\
			"  flag = 0x%x\r\n"\
			"  uiTransactionId = %u\r\n"\
			"  szCaller = %s\r\n"\
			"  szCalled = %s\r\n"\
			"  szSessionId = %s\r\n"\
			"  usRecordType = %s(%u)\r\n"\
			"  usRecordNum = %u\r\n"\
			"  szUserName = %s\r\n"\
			"  szEventTimestamp = %s\r\n"\
			"  szSubscriptionIdData = %s\r\n"\
			"  uiSubscriptionIdType = %s(%u)\r\n",
			_pstAccReq->flag,
			_pstAccReq->uiTransactionId,
			_pstAccReq->szCaller,
			_pstAccReq->szCalled,
			_pstAccReq->szSessionId,
			g_fnPdeRecType(_pstAccReq->usRecordType),_pstAccReq->usRecordType,
			_pstAccReq->usRecordNum,
			_pstAccReq->szUserName,
			_pstAccReq->szEventTimestamp,
			_pstAccReq->szSubscriptionIdData,
			g_fnStringPdeSub(_pstAccReq->uiSubscriptionIdType),_pstAccReq->uiSubscriptionIdType);
	g_fnStringPdeSrvInfo(&_pstAccReq->srv_info,_pszDebug,_unLen);
	fnRUStrnCat(_pszDebug,_unLen,
			"  szStartTime = %s\r\n"\
			"  szEndTime = %s\r\n"\
			"  usOriginalDomainCode = %s(%u)\r\n"\
			"  usTermDomainCode = %s(%u)\r\n"\
			"  uiChargingInd = %s(%u)\r\n"\
			"  szAccessNetworkInfo = %s\r\n"\
			"  uiRetrans = %u\r\n"\
			"  iCallResultType = %s(%d)\r\n"\
			"  iDetailCauseCode = %u\r\n",
			_pstAccReq->szStartTime,
			_pstAccReq->szEndTime,
			g_fnStringIbcOperator(_pstAccReq->usOriginalDomainCode),_pstAccReq->usOriginalDomainCode,
			g_fnStringIbcOperator(_pstAccReq->usTermDomainCode),_pstAccReq->usTermDomainCode,
			TRUE_SELECT(_pstAccReq->uiChargingInd==0,"Not Charge","Charge Enable"),_pstAccReq->uiChargingInd,
			_pstAccReq->szAccessNetworkInfo,
			_pstAccReq->uiRetrans,
			g_fnStringPdeCallResult(_pstAccReq->iCallResultType),_pstAccReq->iCallResultType,
			_pstAccReq->iDetailCauseCode);
	g_fnStringSKTInfo(&_pstAccReq->skt_sinfo,_pszDebug,_unLen);
	g_fnStringSKBInfo(&_pstAccReq->skb_sinfo,_pszDebug,_unLen);
	//g_fnStringPdeTraceInfo(&_pstAccReq->trc_info,_pszDebug,_unLen);
	fnRUStrnCat(_pszDebug,_unLen,"}\r\n");
	return _pszDebug;
}
inline char * g_fnStringPdeAccRes(stAccRes * _pstAccRes,char * _pszDebug,unsigned int _unLen)
{
	fnRUStrnCat(_pszDebug,_unLen,"<stAccRes>\r\n{\r\n"\
			"  ====> usRecordType Enum\r\n"\
			"  * NONE:0,EVENT:1,START:2,INTERIM:3,STOP:4\r\n"\
			"  uiTransactionId = %u\r\n"\
			"  szSessionId = %u\r\n"\
			"  usRecordType = %s(%u)\r\n"\
			"  usRecordNum = %u\r\n}\r\n",
			_pstAccRes->uiTransactionId,
			_pstAccRes->szSessionId,
			g_fnPdeRecType(_pstAccRes->usRecordType),_pstAccRes->usRecordType,
			_pstAccRes->usRecordNum
			);
	return _pszDebug;
}
typedef struct PdeSetUp_t
{
	SVCComMsgHdr m_stHdr;
	SVCSetupApp m_stBody;
	PdeSetUp_t()
	{
		memset(&m_stHdr,0x00,sizeof(SVCComMsgHdr));memset(&m_stBody,0x00,sizeof(SVCSetupApp));
		m_stHdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE;
		m_stHdr.uiMsgLen = sizeof(SVCComMsgHdr) + sizeof(SVCSetupApp);
		m_stHdr.uiType = SVC_SETUP_APPS;
		m_stBody.uiAppId = 1;
		m_stBody.uiSesnSnt = 100000;
	}
	void m_fnHtoN(){m_stHdr.hton();m_stBody.hton();}
	void m_fnNtoH(){m_stHdr.ntoh();m_stBody.ntoh();}
	void m_fnDebug(char * _pszDebug, unsigned int _unLen)
	{
		fnRUStrnCat(_pszDebug,_unLen,"PdeSetUp_t\r\nuiMagicCookie=0x%x, uiMsgLen=%u, uiType=0x%x\r\n",
												m_stHdr.uiMagicCookie,	m_stHdr.uiMsgLen,	m_stHdr.uiType);
		fnRUStrnCat(_pszDebug,_unLen,"uiAppId = %u, uiSesnSnt = %u\r\n",
															m_stBody.uiAppId,m_stBody.uiSesnSnt);
	}
}PdeSetUp_t;
typedef struct PdeAlive_t
{
	SVCComMsgHdr m_stHdr;
	PdeAlive_t()
	{
		memset(&m_stHdr,0x00,sizeof(SVCComMsgHdr));
		m_stHdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE;
		m_stHdr.uiMsgLen = sizeof(SVCComMsgHdr);
		m_stHdr.uiType = SVC_ALIVE;
	}
	void m_fnHtoN(){m_stHdr.hton();}
	void m_fnNtoH(){m_stHdr.ntoh();}
	void m_fnDebug(char * _pszDebug, unsigned int _unLen)
	{
		fnRUStrnCat(_pszDebug,_unLen,"PdeAlive_t\r\nuiMagicCookie=0x%x, uiMsgLen=%u, uiType=0x%x\r\n",
												m_stHdr.uiMagicCookie,	m_stHdr.uiMsgLen,	m_stHdr.uiType);
	}
}PdeAlive_t;
#define U_SZ(a,b)    (a >= b ? a : b)
#define DEF_PDE_BODY U_SZ(sizeof(stAccReq), sizeof(stAccRes))
typedef struct PDEMsg_t
{
	SVCComMsgHdr hdr;
	char body[DEF_PDE_BODY];
	PDEMsg_t(){memset(this,0x00,sizeof(PDEMsg_t));}
	void hton_body()
	{
		switch(hdr.uiType)
		{
			case PDE_ACCOUNT:
				if(hdr.uiSubType == PDEST_REQ) ((stAccReq*)body)->hton();
				else if(hdr.uiSubType == PDEST_RES) ((stAccRes*)body)->hton();
				break;
			default:
				break;
		}
	}
	void ntoh_body(){
		switch(hdr.uiType)
		{
			case PDE_ACCOUNT:
				if(hdr.uiSubType == PDEST_REQ) ((stAccReq*)body)->ntoh();
				else if(hdr.uiSubType == PDEST_RES) ((stAccRes*)body)->ntoh();
				break;
			default:
				break;
		}
	}
	void hton()
	{
		hton_body();hdr.hton();
	}
	void ntoh()
	{
		hdr.ntoh();ntoh_body();
	}
	long fnLogShift(char *p, unsigned int n)
	{
		if((n >= 32) && (sizeof(long) == 4)) return 0;    // detecting 32 bit cpu
		if(n==0) return ((unsigned long)((unsigned char)p[0]));
		return (((unsigned long)((unsigned char)p[0]))<<n);
	}
	void fnSetShift(long val,char *p,unsigned int n)
	{
		if((n >= 32) && (sizeof(long) == 4)) return;    // detecting 32 bit cpu
		if(n==0) p[0] = (char)((val) & 0xff);
		else p[0] = (char)((val >> n) & 0xff);
	}
	long fnGetLong(char *p)
	{
		if(sizeof(long) == 4)   // detecting 32 bit cpu
		{
			return fnLogShift(&p[0],24) + fnLogShift(&p[1],16)
			+ fnLogShift(&p[2],8) + fnLogShift(&p[3],0);
		}
		return fnLogShift(&p[0],56) + fnLogShift(&p[1],48)
		+ fnLogShift(&p[2],40) + fnLogShift(&p[3],32)
		+ fnLogShift(&p[4],24) + fnLogShift(&p[5],16)
		+ fnLogShift(&p[6],8) + fnLogShift(&p[7],0);
	}
	void fnSetLong(long val,char *p)
	{
		if(sizeof(long) ==4)  // detecting 32 bit cpu
		{
			fnSetShift(val,&p[0],24);fnSetShift(val,&p[1],16);
			fnSetShift(val,&p[2],8);fnSetShift(val,&p[3],0);return;
		}
		fnSetShift(val,&p[0],56);fnSetShift(val,&p[1],48);
		fnSetShift(val,&p[2],40);fnSetShift(val,&p[3],32);
		fnSetShift(val,&p[4],24);fnSetShift(val,&p[5],16);
		fnSetShift(val,&p[6],8);fnSetShift(val,&p[7],0);
	}
	void fnSetReqEventTime(unsigned long _ulEventMs)
	{
		fnSetLong(_ulEventMs, &hdr.Reserved[0]);
	}
	unsigned long fnGetReqEventTime()
	{
		return (unsigned long)fnGetLong(&hdr.Reserved[0]);
	}
	void fnSetRspEventTime(unsigned long _ulEventMs)
	{
		fnSetLong(_ulEventMs, &hdr.Reserved[8]);
	}
	unsigned long fnGetRspEventTime()
	{
		return (unsigned long)fnGetLong(&hdr.Reserved[8]);
	}
	inline char* getstr_type(int nType,char* szType)
	{
		switch(nType)
		{
			case PDE_CAEX:
				sprintf(szType,"PDE_CAEX");
				break;
			case PDE_ACCOUNT:
				sprintf(szType,"PDE_ACCOUNT");
				break;
			case PDE_ACCOUNT_RETRANS:
				sprintf(szType,"PDE_ACCOUNT_RETRANS");
				break;
			case PDE_DW:
				sprintf(szType,"PDE_DW");
				break;
			case PDE_DP:
				sprintf(szType,"PDE_DP");
				break;
			case PDE_ACCOUNT_FD:
				sprintf(szType,"PDE_ACCOUNT_FD");
				break;
			case SVC_ALIVE:
				sprintf(szType,"ALIVE");
				break;
			case SVC_SETUP_APPS:
				sprintf(szType,"SETUP_APPS");
				break;
			default:
				sprintf(szType,"Undefine");
				break;
		}
		return szType;
	}
	inline char* getstr_subtype(int nSubType,char *szSubType)
	{
		switch(nSubType)
		{
			case PDEST_REQ: sprintf(szSubType,"REQ"); break;
			case PDEST_RES: sprintf(szSubType,"RES"); break;
			case PDEST_INFO: sprintf(szSubType,"INFO"); break;
			default: sprintf(szSubType,"Undefine"); break;
		}
		return szSubType;
	}
	char* str(char* strBuff,int nBufLen) 
	{
		char szType[32];char szSubType[32];char szHdr[256];char szBody[10240];
		szHdr[0]='\0';	szBody[0]='\0';
		getstr_type(hdr.uiType,szType);	getstr_subtype(hdr.uiSubType,szSubType);
		snprintf(szHdr, 255, "[LEN:%u][DESC:%s][MSG:%s-%s][A:%d_S:%d C:%d_S:%u][res:%#x(%s)]",
										      	hdr.uiMsgLen, fnRUStr(hdr.szSesDesc), szType, szSubType,
										      	hdr.uiAsId, hdr.uiAsSesId, hdr.uiCompId, hdr.uiCompSesId,
										      	hdr.uiReasonCode,fnPdeApiStringError(hdr.uiReasonCode));
		switch(hdr.uiType)
		{
			case PDE_ACCOUNT:
				if(hdr.uiSubType == PDEST_REQ) ((stAccReq*)body)->str(szBody,sizeof(szBody));
				else if(hdr.uiSubType == PDEST_RES) ((stAccRes*)body)->str(szBody,sizeof(szBody));
				break;
			case PDE_ACCOUNT_FD:
				if(hdr.uiSubType == PDEST_REQ) ((stAccReqFD*)body)->str(szBody,sizeof(szBody));
			default:
				break;
		}
		sprintf(strBuff,"HDR(%d):%s BODY(%d):",(int)sizeof(SVCComMsgHdr),szHdr,
														(int)(hdr.uiMsgLen-sizeof(SVCComMsgHdr)));
		strncat(strBuff,szBody,nBufLen-strlen(strBuff));
		return strBuff;
	}
}PDEMsg_t;
typedef struct PdeStartRecord_t : public PDEMsg_t
{
	PdeStartRecord_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(this,0x00,sizeof(PDEMsg_t));
		hdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE;
		hdr.uiType = PDE_ACCOUNT;
		hdr.uiSubType = PDEST_REQ;
		hdr.uiMsgLen = sizeof(PDEMsg_t);
	}
	void m_fnHtoN(){hton();}
	void m_fnNtoH(){ntoh();}
	stAccReq * m_fnGetBody(){return (stAccReq*)&body[0];}
	unsigned int m_fnGetTotalLen(bool _bOdering)
	{
		unsigned int unLen = hdr.uiMsgLen;
		if(_bOdering) unLen = ntohl(unLen);
		return unLen;
	}
	unsigned int m_fnBodyLen(bool _bOdering)
	{
		return m_fnGetTotalLen(_bOdering) - sizeof(SVCComMsgHdr);
	}
	void m_fnDebug(char* _pszBuff,int _nBufLen)
	{
		str(_pszBuff,_nBufLen);
	}
	char * m_fnTrace(char * _pszTrace, unsigned int _unLen)
	{
		g_fnStringPdeHeader(&hdr,_pszTrace,_unLen);
		if(hdr.uiType == PDE_ACCOUNT)
		{
			if(hdr.uiSubType == PDEST_REQ)
				return g_fnStringPdeAccReq((stAccReq *)body, _pszTrace,_unLen);
			else if(hdr.uiSubType == PDEST_RES)
				return g_fnStringPdeAccRes((stAccRes *)body,_pszTrace,_unLen);
		}
		return _pszTrace;
	}
}PdeStartRecord_t;
typedef struct PdeInterim_t : public PDEMsg_t
{
	PdeInterim_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(this,0x00,sizeof(PDEMsg_t));
		hdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE;
		hdr.uiType = PDE_ACCOUNT;
		hdr.uiSubType = PDEST_REQ;
		hdr.uiMsgLen = sizeof(PDEMsg_t);
	}
	void m_fnHtoN(){hton();}
	void m_fnNtoH(){ntoh();}
	stAccReq * m_fnGetBody(){return (stAccReq*)&body[0];}
	unsigned int m_fnGetTotalLen(bool _bOdering)
	{
		unsigned int unLen = hdr.uiMsgLen;
		if(_bOdering) unLen = ntohl(unLen);
		return unLen;
	}
	unsigned int m_fnBodyLen(bool _bOdering)
	{
		return m_fnGetTotalLen(_bOdering) - sizeof(SVCComMsgHdr);
	}
	void m_fnDebug(char* _pszBuff,int _nBufLen)
	{
		str(_pszBuff,_nBufLen);
	}
	char * m_fnTrace(char * _pszTrace, unsigned int _unLen)
	{
		g_fnStringPdeHeader(&hdr,_pszTrace,_unLen);
		if(hdr.uiType == PDE_ACCOUNT)
		{
			if(hdr.uiSubType == PDEST_REQ)
				return g_fnStringPdeAccReq((stAccReq *)body, _pszTrace,_unLen);
			else if(hdr.uiSubType == PDEST_RES)
				return g_fnStringPdeAccRes((stAccRes *)body,_pszTrace,_unLen);
		}
		return _pszTrace;
	}
}PdeInterim_t;
typedef struct PdeStopRecord_t : public PDEMsg_t
{
	PdeStopRecord_t(){m_fnClear();}
	void m_fnClear()
	{
		memset(this,0x00,sizeof(PDEMsg_t));
		hdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE;
		hdr.uiType = PDE_ACCOUNT;
		hdr.uiSubType = PDEST_REQ;
		hdr.uiMsgLen = sizeof(PDEMsg_t);
	}
	void m_fnHtoN(){hton();}
	void m_fnNtoH(){ntoh();}
	stAccReq * m_fnGetBody(){return (stAccReq*)&body[0];}
	unsigned int m_fnGetTotalLen(bool _bOdering)
	{
		unsigned int unLen = hdr.uiMsgLen;
		if(_bOdering) unLen = ntohl(unLen);
		return unLen;
	}
	unsigned int m_fnBodyLen(bool _bOdering)
	{
		return m_fnGetTotalLen(_bOdering) - sizeof(SVCComMsgHdr);
	}
	void m_fnDebug(char* _pszBuff,int _nBufLen)
	{
		str(_pszBuff,_nBufLen);
	}
	char * m_fnTrace(char * _pszTrace, unsigned int _unLen)
	{
		g_fnStringPdeHeader(&hdr,_pszTrace,_unLen);
		if(hdr.uiType == PDE_ACCOUNT)
		{
			if(hdr.uiSubType == PDEST_REQ)
				return g_fnStringPdeAccReq((stAccReq *)body, _pszTrace,_unLen);
			else if(hdr.uiSubType == PDEST_RES)
				return g_fnStringPdeAccRes((stAccRes *)body,_pszTrace,_unLen);
		}
		return _pszTrace;
	}
}PdeStopRecord_t;
#ifdef DEF_PDE_TRANSPORT_CHANGED
#include "eSipUtil/transportMgr.hxx"
#include "eSipUtil/queueThread.hxx"
#include "eSipUtil/NodeMgr.hxx"
#ifdef DEF_VIBC_CUSTOMIZE
typedef enum
{
	E_PDE_CON_NONE = 0,
	E_PDE_CON_CONNECTED,
	E_PDE_CON_DISCONNECTED,
	E_PDE_CON_RECONNECTING,
	E_PDE_CON_DOWNLINK,
	E_PDE_CON_MAX
}EPdeConEv_t;
inline const char * g_fnStrPdeCon(EPdeConEv_t _eT)
{
	switch(_eT)
	{
		case E_PDE_CON_CONNECTED: return "E_PDE_CON_CONNECTED";
		case E_PDE_CON_DISCONNECTED: return "E_PDE_CON_DISCONNECTED";
		case E_PDE_CON_RECONNECTING: return "E_PDE_CON_RECONNECTING";
		case E_PDE_CON_DOWNLINK: return "E_PDE_CON_DOWNLINK";
		default: return "UNKNOWN";
	}
	return "UNKNOWN";
}
class PdeConEv : public eSipUtil::StlObject
{
	public:
		PdeConEv()
		{
			m_eEvent=E_PDE_CON_NONE;memset(&m_stAddr,0x00,sizeof(eSipUtil::Net5Tuple_t));
		}
		~PdeConEv(){}
	EPdeConEv_t m_eEvent;
	eSipUtil::Net5Tuple_t m_stAddr;
};
#endif

class PdeTcpServer
{
	public:
		PdeTcpServer();
		~PdeTcpServer();
		static PdeTcpServer * fnGetInstance();
		static PdeTcpServer * fnLoadingFromConf();
		static void fnFinalize();
		static void fnPdeUpLink(bool _bWait=false);
		void fnRcvSetupMsg(PdeSetUp_t * _pstSetup);
		void fnSendSetupMsg();
		int fnSendPde(void * _pvData, unsigned int _unLen);
		void fnSendAlive();
		static bool fnAvailablePde();
		static void fnReconnect();
		static void fnPdeDownLink();
		#ifdef DEF_VIBC_CUSTOMIZE
		void m_fnAddEvent(EPdeConEv_t _eT,const eSipUtil::Net5Tuple_t *_pstNet5Tuple=NULL);
		static void m_fnCbkConEvent(PdeConEv * _pclsEv, void * _pvObject);
		bool m_bOnline;
		eSipUtil::QueueThread<PdeConEv> * m_pclsEvProc;
		#endif
		static PdeTcpServer * m_pclsSrv;
		static bool m_bOdering;
		eSipUtil::NodeMgr m_clsMgr;
		
};
#else
#if 0
class PdeTcpServer : public RUTcpServer
{
	public:
		PdeTcpServer();
		~PdeTcpServer();
		static PdeTcpServer * fnGetInstance();
		static PdeTcpServer * fnLoadingFromConf();
		static void fnFinalize();
		static void fnPdeUpLink(bool _bWait=false);
		void fnRcvSetupMsg(PdeSetUp_t * _pstSetup);
		void fnSendAlive();
		static bool fnAvailablePde();
		static PdeTcpServer * m_pclsSrv;
		static bool m_bOdering;
		bool m_bOnline;
	private:
		static unsigned int fnCbkDecideBodyLen(void * _pvPacket);
		static void fnCbkRecvPacket(RUTcpData * _pclsData, void * _pvUser);
		static void fnCbkConnected(RUTcpChannel *_pclsCh,const char * _pszIP, int _nPort);
		static void fnCbkDisConnected(RUTcpChannel *_pclsCh,const char * _pszIP, int _nPort);
};
#endif
#endif
#endif

