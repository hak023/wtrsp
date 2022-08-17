
#ifndef MAIN_CONFIG_H
#define MAIN_CONFIG_H

#include "main.h"
#include "ConfigFile.hxx"
#include "log.hxx"
class MainConfig
{
	public:
		MainConfig();
		~MainConfig();
		static MainConfig * m_fnGetInstance();
		void m_fnDisplayMain();
		void m_fnDisplayEnv();
		void m_fnLoadFile();
		void m_fnApplyMain();
		void m_fnApplyEnv();
		static void s_fnCbkEnvChanged(eSipUtil::CfgFile * _pclsObj);
		eSipUtil::CfgFile m_clsMainCfg;
		eSipUtil::CfgFile m_clsEnvCfg;

		//----> main.cfg
		//[MAIN]
		unsigned int m_unWorkerNum;
		unsigned int m_unWorkerQueueCnt;
		unsigned int m_unIdMgrNum;
		eSipUtil::KString m_clsSysName;
		unsigned int m_unSysIndex;
		//[TRSE]
		eSipUtil::KString m_clsTrseLocalIP;
		unsigned int m_unTrseLocalPort;

		//----> env.cfg
		//[LOG]
		eSipUtil::ELogLevel_t m_eLvUtil;
		eSipUtil::ELogLevel_t m_eLvApp;
		eSipUtil::ELogLevel_t m_eLvTrse;
		eSipUtil::ELogLevel_t m_eLvTrss;
		bool m_bSkipBinary;
		//[NAS]
		eSipUtil::KString m_clsNasInternal;
		bool m_bDeleteDecodeFile;
		bool m_bDeleteEncodeFile;
		//[GRG]
		unsigned int m_unGarbageSesTmr;
		unsigned int m_unGarbageInvervalTmr;
		unsigned int m_unGarbageCheckCnt;
		//[SES]
		unsigned int m_unSesTmr;
		//[TRSE]
		unsigned int m_unTrseEstablishTimeout;
		//[TRSS]
		unsigned int m_unTrssEstablishTimeout;
		unsigned int m_unTrssJobCreatedTimeout;

		//[TRSG]
		unsigned int m_unLinkTestInterval;
		unsigned int m_unLinkTestTimeout;
		unsigned int m_unLinkTestCriticalCount;
		unsigned int m_unMaxTcpContentSizeKB;
		unsigned int m_unMaxTagImageSizeKB;
		unsigned int m_unRetryConnectionInterval;
		unsigned int m_unTransactionTimeout;
		unsigned int m_unMaxConnectionRetries;

		static MainConfig * m_pclsMy;
};
MainConfig * g_fnCreateMainConfig();
#endif

