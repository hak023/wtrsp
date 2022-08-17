#ifndef MAIN_CONFIG_H
#define MAIN_CONFIG_H

#include "main.h"
#include "ConfigFile.hxx"
#include "JsonFile.hxx"
#include "log.hxx"
#include "lock.hxx"
#include "AppIf.h"

class Codec: public eSipUtil::StlObject
{
public:
	Codec(){};
	~Codec(){};
	eSipUtil::KString m_clsCodecID;
};

class TcCodec: public eSipUtil::StlObject
{
public:
	TcCodec(){};
	~TcCodec(){};
	TcCodec & operator=(TcCodec & _rclsSrc)
    {
       m_uiKey = _rclsSrc.m_uiKey;
       if(m_pszKey) {delete [] m_pszKey;}
       if(_rclsSrc.m_pszKey) m_pszKey = strdup(_rclsSrc.m_pszKey);
       m_clsSrcID = _rclsSrc.m_clsSrcID;
       m_clsType = _rclsSrc.m_clsType;
       m_clsTargetID = _rclsSrc.m_clsTargetID;
       m_clsACodecID = _rclsSrc.m_clsACodecID;
       m_clsVCodecID = _rclsSrc.m_clsVCodecID;
       m_clsFormat = _rclsSrc.m_clsFormat;
       m_clsACodec = _rclsSrc.m_clsACodec;
       m_clsVCodec = _rclsSrc.m_clsVCodec;
       m_clsTool = _rclsSrc.m_clsTool;
       m_clsMetadata = _rclsSrc.m_clsMetadata;
       m_clsEtc = _rclsSrc.m_clsEtc;
       return * this;
    }
	void m_fnDebug(eSipUtil::KString &_rclsDebug)
	{
		_rclsDebug<<"Available Codec [Name:" << m_clsSrcID << "->" << m_clsTargetID;
		_rclsDebug<< ", Type: " << m_clsType << ", AudioCodecID: " << m_clsACodecID
				<< ", VideoCodecID: " << m_clsVCodecID << ", Format: " << m_clsFormat
				<< ", ACodec: " << m_clsACodec << ", VCodec: " << m_clsVCodec
				<<", Tool: " << m_clsTool << ", Metadata: " << m_clsMetadata << ", Etc: " << m_clsEtc << "]\n";
	}
	eSipUtil::KString m_clsSrcID;
	eSipUtil::KString m_clsType;
	eSipUtil::KString m_clsTargetID;
	eSipUtil::KString m_clsACodecID;
	eSipUtil::KString m_clsVCodecID;
	eSipUtil::KString m_clsFormat;
	eSipUtil::KString m_clsACodec;
	eSipUtil::KString m_clsVCodec;
	eSipUtil::KString m_clsTool;
	eSipUtil::KString m_clsMetadata;
	eSipUtil::KString m_clsEtc;
};
class MainConfig
{
public:
	MainConfig();
	~MainConfig();
	static MainConfig* m_fnGetInstance();
	void m_fnDisplayMain();
	void m_fnDisplayEnv();
	void m_fnLoadFile();
	void m_fnApplyMain();
	void m_fnApplyEnv();
	void m_fnApplyCodec();
	static void s_fnCbkEnvChanged(eSipUtil::CfgFile *_pclsObj);
	static void s_fnCbkTcCodecJsonChanged(eSipUtil::JsonFile *_pclsObj);
	eSipUtil::CfgFile m_clsMainCfg;
	eSipUtil::CfgFile m_clsEnvCfg;

	eSipUtil::JsonFile m_clsTcCodecJson;

	//----> main.cfg
	//[MAIN]
	unsigned int m_unWorkerNum;
	unsigned int m_unWorkerQueueCnt;
	unsigned int m_unIdMgrNum;
	unsigned int m_unWorkerIdMgrNum;
	eSipUtil::KString m_clsSysName;
	unsigned int m_unSysIndex;
	eSipUtil::KString m_clsLocName;
	eSipUtil::KString m_clsDstName;

	//----> env.cfg
	//[LOG]
	eSipUtil::ELogLevel_t m_eLvUtil;
	eSipUtil::ELogLevel_t m_eLvApp;
	eSipUtil::ELogLevel_t m_eLvTrsg;
	//[NAS]
	eSipUtil::KString m_clsNasInternal;
	//[GRG]
	unsigned int m_unGarbageSesTmr;
	unsigned int m_unGarbageInvervalTmr;
	unsigned int m_unGarbageCheckCnt;
	//[SES]
	unsigned int m_unSesTmr;
	//[TRSG]
	unsigned int m_unTransactionTimeout;
	unsigned int m_unTransactionRetryCnt;
	unsigned int m_unEstablishSesResTimeout;
	unsigned int m_unEstablishSesRetryCnt;
	unsigned int m_unLinkTestResTimeout;
	unsigned int m_unLinkTestRetryCnt;
	unsigned int m_unReconnTimeout;
	unsigned int m_unMaxTcpContentSizeKB;
	unsigned int m_unMaxTagImageSizeKB;
	//[TC]
	unsigned int m_unCmdTimeout;

	//----> tc_codec.json
	eSipUtil::StlList m_listCodec;
	eSipUtil::StlMap m_mapACodec;
	eSipUtil::StlMap m_mapVCodec;
	eSipUtil::RwMutex m_clsLock;
	ETrssCodeSet_t m_fnChkTargetCodec(eSipUtil::KString &_rclsTargetAudioCodec, eSipUtil::KString &_rclsTargetVideoCodec);
	bool m_fnGetTargetCodec(eSipUtil::KString &_rclsSourceID, eSipUtil::KString &_rclsTargetID,
				eSipUtil::KString &_rclsTargetAudioCodec, eSipUtil::KString &_rclsTargetVideoCodec, TcCodec &_rclsCodec);

	static MainConfig *m_pclsMy;
};
MainConfig* g_fnCreateMainConfig();
#endif

