#ifndef EMP_MMI_H
#define EMP_MMI_H
#include "lm_msg.h"

#define DEF_R125_TGAS
#define DEF_R126_TGAS
#define DEF_R133_TGAS

#ifdef DEF_R125_TGAS
#include "ConfigFile.hxx"
#include "string.hxx"
#include "HangDefine.h"
#endif

namespace nsEmpMmi
{
/************************* Log Level Defined **********************************************/
typedef enum
{
	E_EMP_LOG_DISABLE = 0,
	E_EMP_LOG_ERR,
	E_EMP_LOG_WARN,
	E_EMP_LOG_INFO,
	E_EMP_LOG_DEBUG,
	E_EMP_LOG_STACK,
	E_EMP_LOG_MAX
}ELogEmpMmi_t;
/************************* ReLoading File Defined *******************************************/
typedef enum
{
	E_EMP_REFRESH_NONE=0,
	E_EMP_REFRESH_AS,
	E_EMP_REFRESH_RATIO,
	E_EMP_REFRESH_TRACE,
	E_EMP_REFRESH_MAX
}ERefresh_t;
inline const char * g_fnGetStringERefresh(ERefresh_t _eT)
{
	switch(_eT)
	{
		case E_EMP_REFRESH_AS: return "E_EMP_REFRESH_AS";
		case E_EMP_REFRESH_RATIO: return "E_EMP_REFRESH_RATIO";
		case E_EMP_REFRESH_TRACE: return "E_EMP_REFRESH_TRACE";
		default: return "Unknown";
	}
	return "Unknown";
}
/*********************** Log CallBack Defined **********************************************/
typedef void (*PFuncEmpMmiLog_t)(ELogEmpMmi_t _eLv, const char * _pszLog);
/*********************** Refresh File Callback Defined ****************************************/
typedef bool (*PFuncEmpRefresh_t)(ERefresh_t _eT);
/*************** sample code **************
EmpMmi * pclsMmi = EmpMmi::m_fnGetInstance();
pclsMmi->m_fnSetLogCallBack(s_fnCbkMmiLog);
pclsMmi->m_fnLoadCfg("../config/TGAS/main.cfg");
****************************************/
typedef void (*PFuncAddThread_t)(const char * _pszName, unsigned int _unThreadID);
typedef void (*PFuncSetThreadID_t)(unsigned int _unThreadID);
typedef void (*PFuncSetThread_t)(void);
class EmpMmi
{
	public:
		EmpMmi();
		~EmpMmi();
		//------> Init Functions
		static EmpMmi * m_fnGetInstance(); 
		static void m_fnDestroyer();
		bool m_fnLoadCfg(const char * _pszPath,unsigned int _unAsKey, unsigned int _unTgkey, unsigned int _unMsrpKey, unsigned int _unShmReadOnlyFlag);
		void m_fnSetLogCallBack(PFuncEmpMmiLog_t  _pfnLog);
		void m_fnSetLogLevel(ELogEmpMmi_t _eLv);    // real time changed...
		void m_fnSetRefreshCallBack(PFuncEmpRefresh_t _pfnRefresh);
		//------> ReLoad File Functions
		bool m_fnRefreshAS();
		bool m_fnRefreshTrace();
		bool m_fnRefreshRatio();
		//-------> Set Thread Info CallBack Functions
		PFuncAddThread_t m_pfnAddRefreshCfg;
		PFuncSetThreadID_t m_pfnSetThreadID;
		PFuncSetThread_t m_pfnSetStartRefreshCfg;
		PFuncSetThread_t m_pfnSetFinishRefreshCfg;
		/************************** MMI ************************************************/
		//------> Codec
		bool m_fnRecvAddCodecReq(NS_LM::Emp_add_sdp_codec_req_t * _pstReq, 
										NS_LM::Emp_add_sdp_codec_rsp * _pstRsp);
		bool m_fnRecvChgCodecReq(NS_LM::Emp_chg_sdp_codec_req_t * _pstReq, 
										NS_LM::Emp_chg_sdp_codec_rsp_t * _pstRsp);
		bool m_fnRecvDelCodecReq(NS_LM::Emp_del_sdp_codec_req_t * _pstReq, 
										NS_LM::Emp_del_sdp_codec_rsp_t * _pstRsp);
		bool m_fnRecvDisCodecReq(NS_LM::Emp_dis_sdp_codec_req_t * _pstReq, 
										NS_LM::Emp_dis_sdp_codec_rsp_t * _pstRsp);
		//------> Rule
		bool m_fnRecvAddRuleReq(NS_LM::Emp_add_sdp_rule_req_t * _pstReq, 
										NS_LM::Emp_add_sdp_rule_rsp * _pstRsp);
		bool m_fnRecvChgRuleReq(NS_LM::Emp_chg_sdp_rule_req_t * _pstReq, 
										NS_LM::Emp_chg_sdp_rule_rsp_t * _pstRsp);
		bool m_fnRecvDelRuleReq(NS_LM::Emp_del_sdp_rule_req_t * _pstReq, 
										NS_LM::Emp_del_sdp_rule_rsp_t * _pstRsp);
		bool m_fnRecvDisRuleReq(NS_LM::Emp_dis_sdp_rule_req_t * _pstReq, 
										NS_LM::Emp_dis_sdp_rule_rsp_t * _pstRsp);
		//------> Maniuplation
		bool m_fnRecvAddManReq(NS_LM::Emp_add_sdp_manipulation_req_t * _pstReq, 
										NS_LM::Emp_add_sdp_manipulation_rsp * _pstRsp);
		bool m_fnRecvChgManReq(NS_LM::Emp_chg_sdp_manipulation_req_t * _pstReq, 
										NS_LM::Emp_chg_sdp_manipulation_rsp_t * _pstRsp);
		bool m_fnRecvDelManReq(NS_LM::Emp_del_sdp_manipulation_req_t * _pstReq, 
										NS_LM::Emp_del_sdp_manipulation_rsp_t * _pstRsp);
		bool m_fnRecvDisManReq(NS_LM::Emp_dis_sdp_manipulation_req_t * _pstReq, 
										NS_LM::Emp_dis_sdp_manipulation_rsp_t * _pstRsp);
		//------> Ua Rule
		bool m_fnRecvAddUaRuleReq(NS_LM::Emp_add_sdp_ua_rule_req_t * _pstReq, 
										NS_LM::Emp_add_sdp_ua_rule_rsp * _pstRsp);
		bool m_fnRecvChgUaRuleReq(NS_LM::Emp_chg_sdp_ua_rule_req_t * _pstReq, 
										NS_LM::Emp_chg_sdp_ua_rule_rsp_t * _pstRsp);
		bool m_fnRecvDelUaRuleReq(NS_LM::Emp_del_sdp_ua_rule_req_t * _pstReq, 
										NS_LM::Emp_del_sdp_ua_rule_rsp_t * _pstRsp);
		bool m_fnRecvDisUaRuleReq(NS_LM::Emp_dis_sdp_ua_rule_req_t * _pstReq, 
										NS_LM::Emp_dis_sdp_ua_rule_rsp_t * _pstRsp);
		//------> Tg Rte
		bool m_fnRecvAddRteReq(NS_LM::Emp_add_rte_req_t * _pstReq, 
										NS_LM::Emp_add_rte_rsp_t * _pstRsp);
		bool m_fnRecvChgRteReq(NS_LM::Emp_chg_rte_req_t * _pstReq, 
										NS_LM::Emp_chg_rte_rsp_t * _pstRsp);
		bool m_fnRecvDelRteReq(NS_LM::Emp_del_rte_req_t * _pstReq, 
										NS_LM::Emp_del_rte_rsp_t * _pstRsp);
		bool m_fnRecvDisRteReq(NS_LM::Emp_dis_rte_req_t * _pstReq, 
										NS_LM::Emp_dis_rte_rsp_t * _pstRsp);
		//-------> VNODE
		bool m_fnRecvDisVnodeEmpReq(NS_LM::VNode_Dis_Emp_Req_t* _pstReq,
									NS_LM::VNode_Dis_Emp_Rsp_t * _pstRsp);
		//------> Env
		bool m_fnRecvChgEnvReq(NS_LM::Emp_chg_env_req_t * _pstReq,
										NS_LM::Emp_chg_env_rsp_t * _pstRsp);
		bool m_fnRecvDisEnvReq(NS_LM::Emp_dis_env_req_t * _pstReq,
										NS_LM::Emp_dis_env_rsp_t * _pstRsp);
#ifdef DEF_R125_TGAS
		bool m_fnRecvDisHangReq(EmpDisDogReq_t * _pstReqMsg, EmpDisDogRsp_t * _pstRspMsg);
		bool m_fnRecvChgHangReq(EmpDisDogReq_t * _pstReqMsg, EmpDisDogRsp_t * _pstRspMsg);
#endif										

#ifdef DEF_R126_TGAS
		bool m_fnRecvAddCodecManagerReq(NS_LM::Emp_add_sdp_codec_manager_req_t * _pstReq,
				NS_LM::Emp_add_sdp_codec_manager_rsp_t * _pstRsp);
		bool m_fnRecvModCodecManagerReq(NS_LM::Emp_chg_sdp_codec_manager_req_t * _pstReq,
				NS_LM::Emp_chg_sdp_codec_manager_rsp_t * _pstRsp);
		bool m_fnRecvDelCodecManagerReq(NS_LM::Emp_del_sdp_codec_manager_req_t * _pstReq,
				NS_LM::Emp_del_sdp_codec_manager_rsp_t * _pstRsp);
		bool m_fnRecvDisCodecManagerReq(NS_LM::Emp_dis_sdp_codec_manager_req_t * _pstReq,
				NS_LM::Emp_dis_sdp_codec_manager_rsp_t * _pstRsp);
#endif

#ifdef DEF_R133_TGAS
		bool m_fnRecvAddSdpTransCodingReq(NS_LM::Emp_add_sdp_transcoding_req_t * _pstReq, NS_LM::Emp_add_sdp_transcoding_rsp * _pstRsp);
		bool m_fnProcSdpTranscodingCheck(st_sdp_transcoding * pstSdpTranscodingOrigInfo, st_sdp_transcoding * pstSdpTranscodingGetInfo);
		bool m_fnRecvModSdpTransCodingReq(NS_LM::Emp_chg_sdp_transcoding_req_t * _pstReq, NS_LM::Emp_chg_sdp_transcoding_rsp_t * _pstRsp);
		bool m_fnRecvDelSdpTransCodingReq(NS_LM::Emp_del_sdp_transcoding_req_t * _pstReq, NS_LM::Emp_del_sdp_transcoding_rsp_t * _pstRsp);
		bool m_fnRecvDisSdpTransCodingReq(NS_LM::Emp_dis_sdp_transcoding_req_t * _pstReq, NS_LM::Emp_dis_sdp_transcoding_rsp_t * _pstRsp);
#endif

};
}
#endif // EMP_MMI_H
