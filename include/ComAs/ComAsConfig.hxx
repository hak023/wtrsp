
#ifndef COM_AS_CONFIG_HXX
#define COM_AS_CONFIG_HXX

#include <list>
#include <string>

#include "eSipUtil/thread.hxx"
#include "eSipUtil/ConfigFile.hxx"

namespace ComAs
{

struct MainCfg
{
	int m_nSolutionType;	//0:vic
	std::string m_clsCompName;
	std::string m_clsCompPeerName;
	int m_nFileFormat;		//0:column, 1:json

	MainCfg()
	{
		m_nSolutionType = 0;
		m_nFileFormat = 0;
	}
};

struct ExtAsCfg
{
	int m_nIfType;	//0:FlexStruct interface, 1:new fiex
	unsigned int m_unSipEventThreadNum;
	bool m_bUseReqUri;
	std::list< std::string > m_listHdrName;
	bool m_bUseContent;
	std::list< std::string > m_listCustomKey;
	unsigned int m_unAliveTimeout;					//check for garbage connection(Second)
	unsigned int m_unTryConIntervalMs;		//server(bind), client(connect) retry interval
	unsigned int m_unInitReqIntervalMs;		//only for client
	unsigned int m_unPingReqIntervalMs;		//only for client
	unsigned int m_unWaitCon4ActiveMs;		//wait time for connection with AS when change ACTIVE
	unsigned int m_unFixRouteSeqId;				//0:unused, >1:fixed route sequence id
	
	ExtAsCfg()
	{
		m_nIfType = 0;
		m_unSipEventThreadNum = 2;
		m_bUseReqUri = false;
		m_bUseContent = true;
		m_unAliveTimeout = 10;
		m_unTryConIntervalMs = 1000;
		m_unInitReqIntervalMs = 1000;
		m_unPingReqIntervalMs = 1000;
		m_unWaitCon4ActiveMs = 5000;
		m_unFixRouteSeqId = 0;
	}
};

struct TrspConfig
{
	unsigned int m_unMaxQueueSize;
	unsigned int m_unMaxTransportNum;
	unsigned int m_unSocketRcvBufferSize;
	unsigned int m_unSocketSndBufferSize;
	unsigned int m_unRcvBufferSize;
	unsigned int m_unSndBufferSize;

	TrspConfig()
	{
		m_unMaxQueueSize = 1000;
		m_unMaxTransportNum = 100;
		m_unSocketRcvBufferSize = 0;
		m_unSocketSndBufferSize = 0;
		m_unRcvBufferSize = 20000;
		m_unSndBufferSize = 20000;
	}
};

struct CdrConfig
{
	int m_nCdrEnable;								//0:disable, 1:enable
	std::string m_clsCdrPath;
	std::string m_clsCdrName;
	unsigned int m_unCdrIntervalMin;		//minute

	CdrConfig()
	{
		m_nCdrEnable = 1;
		m_clsCdrPath = "../stat/sip/";
		m_clsCdrName = "SIP_CALL";
		m_unCdrIntervalMin = 1;
	}
};

struct AlarmConfig
{
	std::string m_clsOmpIp;
	int m_nOmpPort;	

	AlarmConfig()
	{
		m_nOmpPort = 0;
	}
};

struct TraceConfig
{
	std::string m_clsOmpIp;
	int m_nOmpPort;

	TraceConfig()
	{
		m_nOmpPort = 0;
	}
};

struct MmiConfig
{
	std::string m_clsLocalIp;
	int m_nLocalPort;
	std::string m_clsRemoteIp;
	int m_nRemotePort;

	MmiConfig()
	{
		m_nLocalPort = 0;
		m_nRemotePort = 0;
	}
};

class ComAsCfgMgr
{
	public :
		enum
		{
			E_CONST_MAX_STR_LEN = 5000,
		};
		
		static ComAsCfgMgr & getInstance();
		~ComAsCfgMgr();

		const char * m_fnGetStaticCfgStr(char * _pszPrint, unsigned int _unLen) const;
		const MainCfg & m_fnGetMainCfg() const{ return m_stMainCfg; }
		const ExtAsCfg & m_fnGetExtAsCfg() const{ return m_stExtAsCfg; }
		const TrspConfig & m_fnGetExtAsTrspCfg() const{ return m_stExtAsTrspCfg; }
		const CdrConfig & m_fnGetCdrCfg() const{ return m_stCdrCfg; }
		const AlarmConfig & m_fnGetAlarmCfg() const{ return m_stAlarmCfg; }
		const TraceConfig & m_fnGetTraceCfg() const{ return m_stTraceCfg; }
		const MmiConfig & m_fnGetMmiCfg() const{ return m_stMmiCfg; }

	private :
		ComAsCfgMgr();

		//realtime config
		static void m_fnCbRefreshProc(ComAsCfgMgr * _pclsCfgMgr);
		void m_fnRefreshLoad();

		//static config
		void m_fnLoad();
		void m_fnLoadMainCfg();
		void m_fnLoadExtAsCfg();
		void m_fnLoadExtAsTrspCfg();
		void m_fnLoadCdrCfg();
		void m_fnLoadAlarmCfg();
		void m_fnLoadTraceCfg();
		void m_fnLoadMmiCfg();

		eSipUtil::CfgFile m_clsCfgFile;
		eSipUtil::CfgFile m_clsRefreshCfgFile;
		time_t m_stRefreshLastModiTime;
		eSipUtil::NormalThread< ComAsCfgMgr > m_clsRefreshThread;

		MainCfg m_stMainCfg;
		ExtAsCfg m_stExtAsCfg;
		TrspConfig m_stExtAsTrspCfg;
		CdrConfig m_stCdrCfg;
		AlarmConfig m_stAlarmCfg;
		TraceConfig m_stTraceCfg;
		MmiConfig m_stMmiCfg;
		
};

}

#endif

