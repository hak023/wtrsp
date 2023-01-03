#include "MainConfig.h"
#include "main.h"
using namespace eSipUtil;

DefaultCfgItem_t MainDefaultCfg[] =
{
{ "MAIN", "ID_MGR_NUM", "1000" },
{ "MAIN", "WORKER_ID_MGR_NUM", "24" },
{ "MAIN", "WORKER_QUEUE_CNT", "100" },
{ "MAIN", "WORKER_NUM", "8" },
{ "MAIN", "LOCAL_FROM", "urn:wtrsp:system:wtrss" },
{ "MAIN", "DST_TO", "urn:wtrsp:system:wtrsg" },
{ NULL, NULL, NULL } };
DefaultCfgItem_t EnvDefaultCfg[] =
{
{ "LOG", "UTIL_LEVEL", "0" },
{ "LOG", "APP_LEVEL", "3" },
{ "LOG", "TRSG_LEVEL", "3" },
{ "LOG", "XML_FORMATTER","0" },
{ "NAS", "NAS_INTERNAL", "/home/ibc/ibc/src/vIBC/trunk/src/NAS_INTERNAL" },
{ "GBG", "GBG_CHECK_CNT", "1000" },
{ "GBG", "GBG_INTERVAL_TMR", "1000" },
{ "GBG", "GBG_SES_TMR", "60" },
{ "SES", "SES_TMR", "450000" },
{ "TRSG", "ESTABLISH_SESSION_RES_TMR", "5000" },
{ "TRSG", "ESTABLISH_SESSION_RETRY_CNT", "3" },
{ "TRSG", "LINK_TEST_RES_TMR", "6000" },
{ "TRSG", "LINK_TEST_RETRY_CNT", "3" },
{ "TRSG", "RECON_TMR", "5000" },
{ "TC", "TC_CMD_TMR", "45" },	
{ NULL, NULL, NULL } };
MainConfig* g_fnCreateMainConfig()
{
	MainConfig * pMainConfig = MainConfig::m_fnGetInstance();
	pMainConfig->m_fnApplyEnv();
	pMainConfig->m_fnDisplayEnv();
	return pMainConfig;
}
MainConfig *MainConfig::m_pclsMy = NULL;
MainConfig::MainConfig()
{
	m_listCodec.m_fnClear();
	m_mapACodec.m_fnClear();
	m_mapVCodec.m_fnClear();
	m_fnLoadFile();
}
MainConfig::~MainConfig()
{
}
MainConfig* MainConfig::m_fnGetInstance()
{
	if (m_pclsMy == NULL)
		m_pclsMy = new MainConfig;
	return m_pclsMy;
}
void MainConfig::m_fnDisplayMain()
{
	IFLOG(E_LOG_DEBUG, "============== ../config/MTM/main.cfg ==============");
	IFLOG(E_LOG_DEBUG, "[MAIN]");
	IFLOG(E_LOG_DEBUG, "WORKER_NUM:%d", m_unWorkerNum);
	IFLOG(E_LOG_DEBUG, "WORKER_QUEUE_CNT:%d", m_unWorkerQueueCnt);
	IFLOG(E_LOG_DEBUG, "ID_MGR_NUM:%d", m_unIdMgrNum);
	IFLOG(E_LOG_DEBUG, "WORKER_ID_MGR_NUM:%d", m_unWorkerIdMgrNum);
	IFLOG(E_LOG_DEBUG, "LOCAL_FROM:%s", (KCSTR)m_clsLocName);
	IFLOG(E_LOG_DEBUG, "DST_TO:%s\n", (KCSTR)m_clsDstName);
	IFLOG(E_LOG_DEBUG, "=======================================================");
}
void MainConfig::m_fnDisplayEnv()
{
	IFLOG(E_LOG_DEBUG, "============== ../config/MTM/env.cfg ===============");
	IFLOG(E_LOG_DEBUG, "[LOG]");
	IFLOG(E_LOG_DEBUG, "UTIL_LEVEL:%d", m_eLvUtil);
	IFLOG(E_LOG_DEBUG, "APP_LEVEL:%d", m_eLvApp);
	IFLOG(E_LOG_DEBUG, "TRSG_LEVEL:%d", m_eLvTrsg);
	IFLOG(E_LOG_DEBUG, "XML_FORMATTER:%d\n", m_bXmlFormatter);

	IFLOG(E_LOG_DEBUG, "[NAS]");
	IFLOG(E_LOG_DEBUG, "NAS_INTERNAL:%s\n", (KCSTR) m_clsNasInternal);

	IFLOG(E_LOG_DEBUG, "[GBG]");
	IFLOG(E_LOG_DEBUG, "GBG_CHECK_CNT:%d", m_unGarbageCheckCnt);
	IFLOG(E_LOG_DEBUG, "GBG_INTERVAL_TMR:%d", m_unGarbageIntervalTmr);
	IFLOG(E_LOG_DEBUG, "GBG_SES_TMR:%d\n", m_unGarbageSesTmr);

	IFLOG(E_LOG_DEBUG, "[SES]");
	IFLOG(E_LOG_DEBUG, "SES_TMR:%d\n", m_unSesTmr);

	IFLOG(E_LOG_DEBUG, "[TRSG]");
	IFLOG(E_LOG_DEBUG, "ESTABLISH_SESSION_RES_TMR:%d", m_unEstablishSesResTimeout);
	IFLOG(E_LOG_DEBUG, "ESTABLISH_SESSION_RETRY_CNT:%d", m_unEstablishSesRetryCnt);
	IFLOG(E_LOG_DEBUG, "LINK_TEST_RES_TMR:%d", m_unLinkTestResTimeout);
	IFLOG(E_LOG_DEBUG, "LINK_TEST_RETRY_CNT:%d", m_unLinkTestRetryCnt);
	IFLOG(E_LOG_DEBUG, "RECON_TMR:%d", m_unReconnTmr);

	IFLOG(E_LOG_DEBUG, "[TC]");
	IFLOG(E_LOG_DEBUG, "TC_CMD_TMR:%d\n", m_unCmdTimeout);
	IFLOG(E_LOG_DEBUG, "=======================================================");
}
void MainConfig::m_fnLoadFile()
{
	m_clsMainCfg.m_fnLoadConfig("../config/MTM/main.cfg", MainDefaultCfg);
	m_clsEnvCfg.m_fnLoadConfig("../config/MTM/env.cfg", EnvDefaultCfg);

	if (!m_clsTcCodecJson.m_fnLoadFile("../config/MTM/tc_codec.json"))
	{
		IFLOG(E_LOG_DEBUG, "[MainConfig] Failed to Load tc_codec.json File!");
		return;
	}

	//Apply Config
	m_fnApplyMain();
	m_fnApplyEnv();
	m_fnApplyCodec();

	//Display Config
	m_fnDisplayMain();
	m_fnDisplayEnv();

	m_clsEnvCfg.m_fnSetNotifyChangedFile(s_fnCbkEnvChanged, true, this);
	m_clsTcCodecJson.m_fnSetNotifyChangedFile(s_fnCbkTcCodecJsonChanged, true, this);
}
void MainConfig::m_fnApplyMain()
{
	m_unIdMgrNum = (KUINT) m_clsMainCfg.m_fnFindVal("MAIN", "ID_MGR_NUM");
	m_unWorkerIdMgrNum = (KUINT) m_clsMainCfg.m_fnFindVal("MAIN", "WORKER_ID_MGR_NUM");
	m_unWorkerQueueCnt = (KUINT) m_clsMainCfg.m_fnFindVal("MAIN", "WORKER_QUEUE_CNT");
	m_unWorkerNum = (KUINT) m_clsMainCfg.m_fnFindVal("MAIN", "WORKER_NUM");
	m_clsLocName = (KSTR) m_clsMainCfg.m_fnFindVal("MAIN", "LOCAL_FROM");
	m_clsDstName = (KSTR) m_clsMainCfg.m_fnFindVal("MAIN", "DST_TO");
}
void MainConfig::m_fnApplyEnv()
{
	m_eLvUtil = (eSipUtil::ELogLevel_t) (KINT) m_clsEnvCfg.m_fnFindVal("LOG", "UTIL_LEVEL");
	m_eLvApp = (eSipUtil::ELogLevel_t) (KINT) m_clsEnvCfg.m_fnFindVal("LOG", "APP_LEVEL");
	m_eLvTrsg = (eSipUtil::ELogLevel_t) (KINT) m_clsEnvCfg.m_fnFindVal("LOG", "TRSG_LEVEL");
	m_bXmlFormatter = (((KINT)m_clsEnvCfg.m_fnFindVal("LOG","XML_FORMATTER") == 1) ? true : false);

	//Set Log Level.
	g_fnSetLog(E_VWTRSS_LOG_CATE_UTIL, m_eLvUtil);
	g_fnSetLog(E_VWTRSS_LOG_CATE_APP, m_eLvApp);
	g_fnSetLog(E_VWTRSS_LOG_CATE_TRSG, m_eLvTrsg);

	m_clsNasInternal = (KCSTR) m_clsEnvCfg.m_fnFindVal("NAS", "NAS_INTERNAL");

	unsigned int unTmp = (KUINT) m_clsEnvCfg.m_fnFindVal("GBG", "GBG_CHECK_CNT");
	if(unTmp > m_unIdMgrNum)
	{
		IFLOG(E_LOG_ERR,"Invalid GBG_CHECK_CNT. (GBG_CHECK_CNT must be less than or equal to ID_MGR_NUM) %d->%d", unTmp, m_unIdMgrNum);
		m_unGarbageCheckCnt = m_unIdMgrNum;
	}
	else
	{
		m_unGarbageCheckCnt = unTmp;
	}

	m_unGarbageIntervalTmr = (KUINT) m_clsEnvCfg.m_fnFindVal("GBG", "GBG_INTERVAL_TMR");
	m_unGarbageSesTmr = (KUINT) m_clsEnvCfg.m_fnFindVal("GBG", "GBG_SES_TMR");

	m_unSesTmr = (KUINT) m_clsEnvCfg.m_fnFindVal("SES", "SES_TMR");

	m_unEstablishSesResTimeout = (KUINT) m_clsEnvCfg.m_fnFindVal("TRSG", "ESTABLISH_SESSION_RES_TMR");
	m_unEstablishSesRetryCnt = (KUINT) m_clsEnvCfg.m_fnFindVal("TRSG", "ESTABLISH_SESSION_RETRY_CNT");
	m_unLinkTestResTimeout = (KUINT) m_clsEnvCfg.m_fnFindVal("TRSG", "LINK_TEST_RES_TMR");
	m_unLinkTestRetryCnt = (KUINT) m_clsEnvCfg.m_fnFindVal("TRSG", "LINK_TEST_RETRY_CNT");
	m_unReconnTmr = (KUINT) m_clsEnvCfg.m_fnFindVal("TRSG", "RECON_TMR");

	m_unCmdTimeout = (KUINT) m_clsEnvCfg.m_fnFindVal("TC", "TC_CMD_TMR");
}
void MainConfig::m_fnApplyCodec()
{
	m_clsLock.m_fnWriteLock();
	m_listCodec.m_fnClear();
	m_mapACodec.m_fnClear();
	m_mapVCodec.m_fnClear();
	JsonFileObject * pclsSourceGrp = m_clsTcCodecJson.m_fnGetObject("SOURCE");
	if(pclsSourceGrp)
	{
		ListItr_t stItor; JsonFileObject * pclsSourceItem = pclsSourceGrp->m_fnBeginItor(stItor);
		while(pclsSourceItem)
		{
			KString clsSourceID = (KSTR)pclsSourceItem->m_fnGetVal("_Codec");

			JsonFileObject * pclsTargetGrp = pclsSourceItem->m_fnGetObject("T_CODEC");
			ListItr_t stItorTarget; JsonFileObject * pclsTargetItem = pclsTargetGrp->m_fnBeginItor(stItorTarget);
			while(pclsTargetItem)
			{
				TcCodec * pclsTcCodec = new TcCodec;
				pclsTcCodec->m_clsSrcID = clsSourceID;
				pclsTcCodec->m_clsType = (KSTR)pclsTargetItem->m_fnGetVal("_Type");
				pclsTcCodec->m_clsTargetID = (KSTR)pclsTargetItem->m_fnGetVal("_TargetID");
				pclsTcCodec->m_clsACodecID = (KSTR)pclsTargetItem->m_fnGetVal("_AudioCodecID");
				pclsTcCodec->m_clsVCodecID = (KSTR)pclsTargetItem->m_fnGetVal("_VideoCodecID");
				pclsTcCodec->m_clsFormat = (KSTR)pclsTargetItem->m_fnGetVal("_Format");
				pclsTcCodec->m_clsACodec = (KSTR)pclsTargetItem->m_fnGetVal("_ACodec");
				pclsTcCodec->m_clsVCodec = (KSTR)pclsTargetItem->m_fnGetVal("_VCodec");
				pclsTcCodec->m_clsTool = (KSTR)pclsTargetItem->m_fnGetVal("_Tool");
				pclsTcCodec->m_clsMetadata = (KSTR)pclsTargetItem->m_fnGetVal("_Metadata");
				pclsTcCodec->m_clsEtc = (KSTR)pclsTargetItem->m_fnGetVal("_Etc");
				pclsTcCodec->m_clsTimeStamp = (KSTR)pclsTargetItem->m_fnGetVal("_TimeStamp");

				//add list
				KString clsDebug;
				pclsTcCodec->m_fnDebug(clsDebug);
				IFLOG(E_LOG_DEBUG, (KSTR)clsDebug);
				m_listCodec.m_fnPushBack(pclsTcCodec);

				if(pclsTcCodec->m_clsACodecID.m_unRealLen > 0)
				{
					Codec* pclsCodec = new Codec;
					pclsCodec->m_clsCodecID = (KCSTR)pclsTcCodec->m_clsACodecID;
					//ex) Key:AMR Value:Codec class
					m_mapACodec.m_fnSetMap((KCSTR)pclsCodec->m_clsCodecID, pclsCodec);
				}
				if(pclsTcCodec->m_clsVCodecID.m_unRealLen > 0)
				{
					Codec* pclsCodec = new Codec;
					pclsCodec->m_clsCodecID = (KCSTR)pclsTcCodec->m_clsVCodecID;
					m_mapVCodec.m_fnSetMap((KCSTR)pclsCodec->m_clsCodecID, pclsCodec);
				}

				// next
				pclsTargetItem = pclsTargetGrp->m_fnNextItor(stItorTarget);
			}
			pclsSourceItem = pclsSourceGrp->m_fnNextItor(stItor);
		}
	}

	m_clsLock.m_fnUnlock();
}
void MainConfig::s_fnCbkEnvChanged(CfgFile *_pclsObj)
{
	MainConfig *pclsConfig = (MainConfig*) _pclsObj->m_fnGetUser();
	pclsConfig->m_clsEnvCfg.m_fnReLoadConfig();
	pclsConfig->m_fnApplyEnv();
	pclsConfig->m_fnDisplayEnv();
}
void MainConfig::s_fnCbkTcCodecJsonChanged(JsonFile *_pclsObj)
{
   MainConfig *pclsConfig = (MainConfig*) _pclsObj->m_fnGetUser();
   pclsConfig->m_clsTcCodecJson.m_fnReLoadFile();
   pclsConfig->m_fnApplyCodec();
}
ETrssCodeSet_t MainConfig::m_fnChkTargetCodec(KString &_rclsTargetAudioCodec, KString &_rclsTargetVideoCodec)
{
	m_clsLock.m_fnReadLock();
	// Font CASE
	if(_rclsTargetAudioCodec == "MDF" || _rclsTargetAudioCodec == "UTF" || _rclsTargetVideoCodec == "MDF" || _rclsTargetVideoCodec == "UTF")
	{
		m_clsLock.m_fnUnlock();
		return E_MEDIA_ENCODING_ERROR;
	}

	if(_rclsTargetAudioCodec.m_unRealLen > 0 && _rclsTargetVideoCodec.m_unRealLen > 0)//Video
	{
		if(m_mapVCodec.m_fnFindMap((KCSTR)_rclsTargetVideoCodec) == NULL)
		{
			m_clsLock.m_fnUnlock();
         IFLOG(E_LOG_ERR,"Not Supported Codec(E_NOT_SUPPORT_VIDEO_ENCODEC_TYPE)");
			return E_NOT_SUPPORT_VIDEO_ENCODEC_TYPE;
		}
		else
		{
			if(m_mapACodec.m_fnFindMap((KCSTR)_rclsTargetAudioCodec) == NULL)
			{
				m_clsLock.m_fnUnlock();
            IFLOG(E_LOG_ERR,"Not Supported Codec(E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE)");
				return E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE;
			}
			else//둘다 찾았다.
			{
				m_clsLock.m_fnUnlock();
				return E_TC_RES_OK;
			}
		}
	}
	else if(_rclsTargetAudioCodec.m_unRealLen == 0 && _rclsTargetVideoCodec.m_unRealLen > 0)//Image
	{
		if(m_mapVCodec.m_fnFindMap((KCSTR)_rclsTargetVideoCodec) == NULL)
		{
			m_clsLock.m_fnUnlock();
         IFLOG(E_LOG_ERR,"Not Supported Codec(E_NOT_SUPPORT_VIDEO_ENCODEC_TYPE)");
			return E_NOT_SUPPORT_VIDEO_ENCODEC_TYPE;
		}
		else
		{
			m_clsLock.m_fnUnlock();
			return E_TC_RES_OK;
		}
	}
	else if(_rclsTargetAudioCodec.m_unRealLen > 0 && _rclsTargetVideoCodec.m_unRealLen == 0)//Audio
	{
		if(m_mapACodec.m_fnFindMap((KCSTR)_rclsTargetAudioCodec) == NULL)
		{
			m_clsLock.m_fnUnlock();
         IFLOG(E_LOG_ERR,"Not Supported Codec(E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE)");
			return E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE;
		}
		else
		{
			m_clsLock.m_fnUnlock();
			return E_TC_RES_OK;
		}
	}
	else
	{
		// No Codec Info
		IFLOG(E_LOG_ERR, "MainConfig No Codec Info");
	}

	m_clsLock.m_fnUnlock();
   IFLOG(E_LOG_ERR,"Not Supported Codec(E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE)");
	return E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE;
}

bool MainConfig::m_fnGetTargetCodec(KString &_rclsSourceID, KString &_rclsTargetID,
		KString &_rclsTargetAudioCodec, KString &_rclsTargetVideoCodec, TcCodec &_rclsCodec)
{
	m_clsLock.m_fnReadLock();
	Iterator stItor;
	TcCodec * pclsTcCodec = (TcCodec*)m_listCodec.m_fnGetNext(stItor);
	while (pclsTcCodec)
	{
		if (pclsTcCodec->m_clsSrcID == _rclsSourceID && pclsTcCodec->m_clsTargetID == _rclsTargetID)
		{
			if (_rclsTargetVideoCodec.m_unRealLen > 0 && _rclsTargetAudioCodec.m_unRealLen > 0)
			{
				// Video
				if (pclsTcCodec->m_clsACodecID == _rclsTargetAudioCodec && pclsTcCodec->m_clsVCodecID == _rclsTargetVideoCodec)
				{

					_rclsCodec = *pclsTcCodec;
					m_clsLock.m_fnUnlock();
					return true;
				}
			}
			else if(_rclsTargetVideoCodec.m_unRealLen > 0 && _rclsTargetAudioCodec.m_unRealLen == 0)
			{
				// Image
				if (pclsTcCodec->m_clsVCodecID == _rclsTargetVideoCodec)
				{
					_rclsCodec = *pclsTcCodec;
					m_clsLock.m_fnUnlock();
					return true;
				}
			}
			else
			{
				// Audio
				if (pclsTcCodec->m_clsACodecID == _rclsTargetAudioCodec)
				{
					_rclsCodec = *pclsTcCodec;
					m_clsLock.m_fnUnlock();
					return true;
				}
			}
		}
		//next
		pclsTcCodec = (TcCodec*) m_listCodec.m_fnGetNext(stItor);
	}

	m_clsLock.m_fnUnlock();
	return false;
}

