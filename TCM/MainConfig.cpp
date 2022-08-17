
#include "MainConfig.h"
#include "main.h"
using namespace eSipUtil;

DefaultCfgItem_t MainDefaultCfg[]=
{
	{"MAIN","WORKER_NUM","8"}, 
	{"MAIN","WORKER_QUEUE_CNT","1000"},
	{"MAIN","ID_MGR_NUM","100"},
	{"TRSE","LOCAL_IP","0.0.0.0"},
	{"TRSE","LOCAL_PORT","30001"},
	{"TRSE","SYSTEM_ID","VWTRSG01"},
	{"TRSE","SYSTEM_INDEX","1"},
	{NULL,NULL,NULL}
};
DefaultCfgItem_t EnvDefaultCfg[]=
{
	{"LOG","UTIL_LEVEL","0"},
	{"LOG","APP_LEVEL","3"},
	{"LOG","TRSE_LEVEL","3"},
	{"LOG","TRSS_LEVEL","3"},
	{"LOG","SKIP_BINARY","1"},
	{"NAS","NAS_INTERNAL","/home/ibc/ibc/src/vIBC/trunk/src/NAS_INTERNAL"},
	{"NAS","DELETE_DECODE_FILE","1"},
	{"NAS","DELETE_ENCODE_FILE","1"},
	{"GBG","GBG_CHECK_CNT","1000"},
	{"GBG","GBG_INTERVAL_TMR","1000"},
	{"GBG","GBG_SES_TMR","60"},
	{"SES","SES_TMR","40000"},
	{"TRSE","ESTABLISH_REQ_TIMEOUT","5000"},
	{"TRSS","ESTABLISH_REQ_TIMEOUT","5000"},
	{"TRSS","JOB_CREATED_TIMEOUT","5000"},
	{"TRSG","LINK_TEST_INTERVAL","30"},
	{"TRSG","LINK_TEST_TIMEOUT","100"},
	{"TRSG","LINK_TEST_CRITICAL_COUNT","3"},
	{"TRSG","MAX_TCP_CONTENT_SIZE_KB","2048"},
	{"TRSG","MAX_TAG_IMAGE_SIZE_KB","2048"},
	{"TRSG","RETRY_CONNECTION_INTERVAL","3"},
	{"TRSG","TRANSACTION_TIMEOUT","0"},
	{"TRSG","MAX_CONNECTION_RETRIES","0"},
	{NULL,NULL,NULL}
};
MainConfig * g_fnCreateMainConfig()
{
   return MainConfig::m_fnGetInstance();
}
MainConfig * MainConfig::m_pclsMy = NULL;
MainConfig::MainConfig()
{
	m_fnLoadFile();
}
MainConfig::~MainConfig(){}
MainConfig * MainConfig::m_fnGetInstance()
{
   if(m_pclsMy==NULL) m_pclsMy = new MainConfig;
   return m_pclsMy;
}
void MainConfig::m_fnDisplayMain()
{
	IFLOG(E_LOG_DEBUG,"============== ../config/TCM/main.cfg ==============");
	IFLOG(E_LOG_DEBUG,"[MAIN]");
	IFLOG(E_LOG_DEBUG,"WORKER_NUM:%d", m_unWorkerNum);
	IFLOG(E_LOG_DEBUG,"WORKER_QUEUE_CNT:%d", m_unWorkerQueueCnt);
	IFLOG(E_LOG_DEBUG,"ID_MGR_NUM:%d\n", m_unIdMgrNum);

	IFLOG(E_LOG_DEBUG,"[TRSE]");
	IFLOG(E_LOG_DEBUG,"LOCAL_IP:%s", (KCSTR)m_clsTrseLocalIP);
	IFLOG(E_LOG_DEBUG,"LOCAL_PORT:%d", m_unTrseLocalPort);
	IFLOG(E_LOG_DEBUG,"SYSTEM_ID:%s", (KCSTR)m_clsSysName);
	IFLOG(E_LOG_DEBUG,"SYSTEM_INDEX:%d",m_unSysIndex);
	IFLOG(E_LOG_DEBUG,"=======================================================");
}
void MainConfig::m_fnDisplayEnv()
{
	IFLOG(E_LOG_DEBUG,"============== ../config/TCM/env.cfg ===============");
	IFLOG(E_LOG_DEBUG,"[LOG]");
	IFLOG(E_LOG_DEBUG,"UTIL_LEVEL:%d", m_eLvUtil);
	IFLOG(E_LOG_DEBUG,"APP_LEVEL:%d", m_eLvApp);
	IFLOG(E_LOG_DEBUG,"TRSE_LEVEL:%d", m_eLvTrse);
	IFLOG(E_LOG_DEBUG,"TRSS_LEVEL:%d", m_eLvTrss);
	IFLOG(E_LOG_DEBUG,"SKIP_BINARY:%d\n", m_bSkipBinary);

	IFLOG(E_LOG_DEBUG,"[NAS]");
	IFLOG(E_LOG_DEBUG,"NAS_INTERNAL:%s", (KCSTR)m_clsNasInternal);
	IFLOG(E_LOG_DEBUG,"DELETE_DECODE_FILE:%d", m_bDeleteDecodeFile);
	IFLOG(E_LOG_DEBUG,"DELETE_ENCODE_FILE:%d\n", m_bDeleteEncodeFile);

	IFLOG(E_LOG_DEBUG,"[GBG]");
	IFLOG(E_LOG_DEBUG,"GBG_CHECK_CNT:%d", m_unGarbageCheckCnt);
	IFLOG(E_LOG_DEBUG,"GBG_INTERVAL_TMR:%d", m_unGarbageInvervalTmr);
	IFLOG(E_LOG_DEBUG,"GBG_SES_TMR:%d\n", m_unGarbageSesTmr);

	IFLOG(E_LOG_DEBUG,"[SES]");
	IFLOG(E_LOG_DEBUG,"SES_TMR:%d\n", m_unSesTmr);

	IFLOG(E_LOG_DEBUG,"[TRSE]");
	IFLOG(E_LOG_DEBUG,"ESTABLISH_REQ_TIMEOUT:%d\n", m_unTrseEstablishTimeout);
	IFLOG(E_LOG_DEBUG,"ESTABLISH_REQ_TIMEOUT:%d\n", m_unTrseEstablishTimeout);

	IFLOG(E_LOG_DEBUG,"[TRSS]");
	IFLOG(E_LOG_DEBUG,"ESTABLISH_REQ_TIMEOUT:%d", m_unTrssEstablishTimeout);
	IFLOG(E_LOG_DEBUG,"JOB_CREATED_TIMEOUT:%d\n", m_unTrssJobCreatedTimeout);

	IFLOG(E_LOG_DEBUG,"[TRSG]");
	IFLOG(E_LOG_DEBUG,"LINK_TEST_INTERVAL:%d", m_unLinkTestInterval);
	IFLOG(E_LOG_DEBUG,"LINK_TEST_TIMEOUT:%d", m_unLinkTestTimeout);
	IFLOG(E_LOG_DEBUG,"LINK_TEST_CRITICAL_COUNT:%d", m_unLinkTestCriticalCount);
	IFLOG(E_LOG_DEBUG,"MAX_TAG_IMAGE_SIZE_KB:%d", m_unMaxTcpContentSizeKB);
	IFLOG(E_LOG_DEBUG,"MAX_TAG_IMAGE_SIZE_KB:%d", m_unMaxTagImageSizeKB);
	IFLOG(E_LOG_DEBUG,"RETRY_CONNECTION_INTERVAL:%d", m_unRetryConnectionInterval);
	IFLOG(E_LOG_DEBUG,"TRANSACTION_TIMEOUT:%d", m_unTransactionTimeout);
	IFLOG(E_LOG_DEBUG,"MAX_CONNECTION_RETRIES:%d", m_unMaxConnectionRetries);
	IFLOG(E_LOG_DEBUG,"=======================================================");
}
void MainConfig::m_fnLoadFile()
{
	m_clsMainCfg.m_fnLoadConfig("../config/TCM/main.cfg", MainDefaultCfg);
	m_clsEnvCfg.m_fnLoadConfig("../config/TCM/env.cfg",EnvDefaultCfg);

	//Apply Config
	m_fnApplyMain();
	m_fnApplyEnv();

	//Display Config
	m_fnDisplayMain();
	m_fnDisplayEnv();

	m_clsEnvCfg.m_fnSetNotifyChangedFile(s_fnCbkEnvChanged, true, this);
}
void MainConfig::m_fnApplyMain()
{
	m_unWorkerNum = (KUINT)m_clsMainCfg.m_fnFindVal("MAIN","WORKER_NUM");
	m_unWorkerQueueCnt = (KUINT)m_clsMainCfg.m_fnFindVal("MAIN","WORKER_QUEUE_CNT");
	m_unIdMgrNum = (KUINT)m_clsMainCfg.m_fnFindVal("MAIN","ID_MGR_NUM");

	m_clsTrseLocalIP = (KCSTR)m_clsMainCfg.m_fnFindVal("TRSE","LOCAL_IP");
	m_unTrseLocalPort = (KUINT)m_clsMainCfg.m_fnFindVal("TRSE","LOCAL_PORT");
	m_clsSysName = (KCSTR)m_clsMainCfg.m_fnFindVal("TRSE","SYSTEM_ID");
	m_unSysIndex = (KUINT)m_clsMainCfg.m_fnFindVal("TRSE","SYSTEM_INDEX");
}
void MainConfig::m_fnApplyEnv()
{
	m_eLvUtil = (eSipUtil::ELogLevel_t)(KINT)m_clsEnvCfg.m_fnFindVal("LOG","UTIL_LEVEL");
	m_eLvApp = (eSipUtil::ELogLevel_t)(KINT)m_clsEnvCfg.m_fnFindVal("LOG","APP_LEVEL");
	m_eLvTrse = (eSipUtil::ELogLevel_t)(KINT)m_clsEnvCfg.m_fnFindVal("LOG","TRSE_LEVEL");
	m_eLvTrss = (eSipUtil::ELogLevel_t)(KINT)m_clsEnvCfg.m_fnFindVal("LOG","TRSS_LEVEL");
	m_bSkipBinary = (((KINT)m_clsEnvCfg.m_fnFindVal("LOG","SKIP_BINARY") == 1) ? true : false);

	//Set Log Level.
	g_fnSetLog(E_VWTRSG_LOG_CATE_UTIL, m_eLvUtil);
	g_fnSetLog(E_VWTRSG_LOG_CATE_APP, m_eLvApp);
	g_fnSetLog(E_VWTRSG_LOG_CATE_TRSE, m_eLvTrse);
	g_fnSetLog(E_VWTRSG_LOG_CATE_TRSS, m_eLvTrss);

	m_clsNasInternal = (KCSTR)m_clsEnvCfg.m_fnFindVal("NAS","NAS_INTERNAL");
	m_bDeleteDecodeFile = (((KINT)m_clsEnvCfg.m_fnFindVal("NAS","DELETE_DECODE_FILE") == 1) ? true : false);
	m_bDeleteEncodeFile = (((KINT)m_clsEnvCfg.m_fnFindVal("NAS","DELETE_ENCODE_FILE") == 1) ? true : false);

	unsigned int unTmp = (KUINT)m_clsEnvCfg.m_fnFindVal("GBG","GBG_CHECK_CNT");
	if(unTmp >m_unIdMgrNum)
	{
		IFLOG(E_LOG_ERR,"Invalid GBG_CHECK_CNT. (GBG_CHECK_CNT must be less than or equal to ID_MGR_NUM) %d->%d", unTmp, m_unIdMgrNum);
		m_unGarbageCheckCnt = m_unIdMgrNum;
	}
	else
	{
		m_unGarbageCheckCnt = unTmp;
	}
	m_unGarbageInvervalTmr = (KUINT)m_clsEnvCfg.m_fnFindVal("GBG","GBG_INTERVAL_TMR");
	m_unGarbageSesTmr = (KUINT)m_clsEnvCfg.m_fnFindVal("GBG","GBG_SES_TMR");

	m_unSesTmr = (KUINT)m_clsEnvCfg.m_fnFindVal("SES","SES_TMR");

	m_unTrseEstablishTimeout = (KUINT)m_clsEnvCfg.m_fnFindVal("TRSE","ESTABLISH_REQ_TIMEOUT");

	m_unTrssEstablishTimeout = (KUINT)m_clsEnvCfg.m_fnFindVal("TRSS","ESTABLISH_REQ_TIMEOUT");
	m_unTrssJobCreatedTimeout = (KUINT)m_clsEnvCfg.m_fnFindVal("TRSS","JOB_CREATED_TIMEOUT");

	m_unLinkTestInterval = (KUINT)m_clsEnvCfg.m_fnFindVal("TRSG","LINK_TEST_INTERVAL");
	m_unLinkTestTimeout = (KUINT)m_clsEnvCfg.m_fnFindVal("TRSG","LINK_TEST_TIMEOUT");
	m_unLinkTestCriticalCount = (KUINT)m_clsEnvCfg.m_fnFindVal("TRSG","LINK_TEST_CRITICAL_COUNT");
	m_unMaxTcpContentSizeKB = (KUINT)m_clsEnvCfg.m_fnFindVal("TRSG","MAX_TCP_CONTENT_SIZE_KB");
	m_unMaxTagImageSizeKB = (KUINT)m_clsEnvCfg.m_fnFindVal("TRSG","MAX_TAG_IMAGE_SIZE_KB");
	m_unRetryConnectionInterval = (KUINT)m_clsEnvCfg.m_fnFindVal("TRSG","RETRY_CONNECTION_INTERVAL");
	m_unTransactionTimeout = (KUINT)m_clsEnvCfg.m_fnFindVal("TRSG","TRANSACTION_TIMEOUT");
	m_unMaxConnectionRetries = (KUINT)m_clsEnvCfg.m_fnFindVal("TRSG","MAX_CONNECTION_RETRIES");
}

void MainConfig::s_fnCbkEnvChanged(CfgFile * _pclsObj)
{
	MainConfig * pclsConifg = (MainConfig*)_pclsObj->m_fnGetUser();
	pclsConifg->m_clsEnvCfg.m_fnReLoadConfig();
	pclsConifg->m_fnApplyEnv();
	pclsConifg->m_fnDisplayEnv();
}
