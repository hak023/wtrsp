#ifndef RULELIBCOM_H
#define RULELIBCOM_H
#include <pthread.h>
//====> eSipCore Lib 
#include "eSipDef.hxx"
//====> Log,List,Map,String
#include "CRuleUtiles.h"
//====> tcp server
#include "CRUTcpServer.h"
//====> tcp client
#include "CRUTcpClient.h"
//====> pool buffer
#include "CRUPools.h"
//====> stack sip message interface
#include "CMsgApi.h"
#include "CReferenceTable.h"

const char * g_fnStringSipState(eDum::ESipState_t _eState);
const char * g_fnStringSdpState(eDum::ESdpState_t _eState);
const char * g_fnStringSipEvent(eSIP::ESipEventType_t _eEv);
/*************************** eSIP State CallBack Info ***************************************/
#define DEF_ST_ARGUMENT eSIP::ProcSipParam_t & _rstCbParam, eSIP::SuspendResult_t & _rstSuspendResult,\
																eSIP::EventResult & _rclsEventResult
#define DEF_ST_ARGUMENT_CONV _rstCbParam,_rstSuspendResult,_rclsEventResult										
typedef struct StackStateCbInfo_t
{
	StackStateCbInfo_t(DEF_ST_ARGUMENT)
	{
		m_pstCbParam = &_rstCbParam;
		m_pstSuspendResult = &_rstSuspendResult;
		m_pclsEventResult = &_rclsEventResult;
		if(g_fnCheckLogLevel(resip::Log::Debug))
		{
			char szLog[DEF_RU_SZ_LOG]; memset(szLog,0x00,DEF_RU_SZ_LOG);bool bRetrans = false;
			if(m_pstCbParam->m_pclsSipMsg && m_pstCbParam->m_pclsSipMsg->getRetransFlag())
				bRetrans = true;
			fnRUStrnCat(szLog,DEF_RU_SZ_LOG,
							"<ASync CallBack Parameter>\r\nCbParam=%p,SusPend=%p,EventResult=%p"\
							"RetransMsg=%s",
							m_pstCbParam,m_pstSuspendResult,m_pclsEventResult,
							TRUE_SELECT(bRetrans==true,"true","false"));
							
			RLOGb(szLog);
		}
	}
	StackStateCbInfo_t & operator=(StackStateCbInfo_t & _rclsSrc)
	{
		m_pstCbParam = _rclsSrc.m_pstCbParam;
		m_pstSuspendResult = _rclsSrc.m_pstSuspendResult;
		m_pclsEventResult = _rclsSrc.m_pclsEventResult;
		return * this;
	}
	eSIP::ProcSipParam_t *m_pstCbParam;
	eSIP::SuspendResult_t *m_pstSuspendResult;
	eSIP::EventResult * m_pclsEventResult;
}StackStateCbInfo_t;
#define DEF_ST_ARG_CREATE StackStateCbInfo_t l_stStCbInfo(DEF_ST_ARGUMENT_CONV)
#define DEF_ST_ARG_INSTANCE    l_stStCbInfo
#define DEF_ST_PARG_INSTANCE  &l_stStCbInfo
/*************************** Process Global Functions ***************************************/
void g_fnDialogLog(ESrvLog_t _eLog,eDum::Dialog * _pclsDialog,const char * _szLog,...);
void g_fnSessionLog(ESrvLog_t _eLog,eDum::Session * _pclsSession,const char * _szLog,...);
#endif
