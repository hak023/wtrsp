#ifndef CIBCCONFIG_H
#define  CIBCCONFIG_H
#include "eRule/RuleInterface.h"
#include "eSipCommand.hxx"
#include "eRte/eSipRteApi.hxx"
#include "eRule/CRuCfg.h"
#include "IBCDefine.h"
#ifdef DEF_COM_CONFIG
#include "string.hxx"
#endif
#ifdef DEF_R131
#include "ColumFile.hxx"
#endif
#define DEF_MAX_TGAS    10
typedef enum
{
	E_IBC_CATEGORY_PROCESS = 0,
	E_IBC_CATEGORY_TGAS, 
	E_IBC_CATEGORY_TRGW,
	E_IBC_CATEGORY_STACK
}EIbcCategory_t;
typedef struct IbcTgAs_t
{
	IbcTgAs_t(){m_fnClear();}
	void m_fnClear()
	{
		m_unRetrans=0;m_unTimeOut=0;
		m_unSocBuff = DEF_RU_1K*10; m_unMaxClient = 0;m_bByteOdering = false;	m_bEnableRetransSDP = false;
		m_unLocPort = 0; m_unRmtBasePort = 0;
	}
	unsigned int m_unSocBuff;
	unsigned int m_unMaxClient;	
	unsigned int m_unRetrans;
	unsigned int m_unTimeOut;
	bool m_bByteOdering;
	bool m_bEnableRetransSDP;
	eSipUtil::KString m_clsLocIP;
	unsigned int m_unLocPort;
	unsigned int m_unRmtBasePort;
	void m_fnDebug(RUString & _rclsDebug)
	{
		_rclsDebug<<"<<<TGAS>>>\r\n"
					<<"m_unSocBuff= "<<RUIntString(m_unSocBuff)
					<<", m_unMaxClient= "<<RUIntString(m_unMaxClient)
					<<", Byte Odering="<<RUIntString((int)m_bByteOdering)
					<<", m_unRetrans= "<<RUIntString(m_unRetrans) 
					<<", m_unTimeOut= "<<RUIntString(m_unTimeOut)
					<<", m_bEnableRetransSDP= "<<RUIntString(m_bEnableRetransSDP)
					<<", m_clsLocIP= "<<(eSipUtil::KCSTR)m_clsLocIP
					<<", m_unLocPort = "<<RUIntString(m_unLocPort)
					<<", m_unRmtBasePort = "<<RUIntString(m_unRmtBasePort)
					<<"\r\n";
	}
}IbcTgAs_t;
typedef struct IbcPde_t
{
	IbcPde_t(){m_fnClear();}
	void m_fnClear()
	{
		m_unPort = 0; m_unByteOdering=0;m_unSocBuffer=0;m_unTimeOut=0;m_unWriteQNum=100;
		m_clsIP=NULL; m_clsFailChargingPath = NULL;m_bEnableFailCharging = false;
		#ifdef DEF_PDE_TRANSPORT_CHANGED
		m_clsRemoteIP = NULL; m_unRemotePort = 0;
		#endif
	}
	void m_fnSetIP(const char * _pszIP)
	{
		m_clsIP = _pszIP;
	}
	void m_fnSetPath(const char * _pszPath)
	{
		m_clsFailChargingPath = _pszPath;
	}
	#ifdef DEF_PDE_TRANSPORT_CHANGED
	void m_fnSetRemoteIP(const char * _pszIP)
	{
		m_clsRemoteIP = _pszIP;
	}
	#endif
	void m_fnDebug(RUString & _rclsDebug)
	{
		_rclsDebug<<"<<<PDE>>>\r\n"
					<<"m_clsFailChargingPath = "<<(RUSTR)m_clsFailChargingPath
					<<", m_bEnableFaillCharging"<<RUIntString(m_bEnableFailCharging)
					<<", m_clsIP = "<<(RUSTR)m_clsIP
					<<", m_unPort = "<<RUIntString(m_unPort)
					<<", m_clsRemoteIP = "<<(RUSTR)m_clsRemoteIP
					<<", m_unRemotePort = "<<RUIntString(m_unRemotePort)
					<<", m_unByteOdering = "<<RUIntString(m_unByteOdering)
					<<", m_unSocBuffer = "<<RUIntString(m_unSocBuffer)
					<<", m_unTimeOut = "<<RUIntString(m_unTimeOut)<<"\r\n";
	}
	RUString m_clsIP;
	RUString m_clsFailChargingPath;
	bool m_bEnableFailCharging;
	unsigned int m_unPort;
	unsigned int m_unByteOdering;
	unsigned int m_unSocBuffer;
	unsigned int m_unTimeOut;
	unsigned int m_unWriteQNum;
	#ifdef DEF_PDE_TRANSPORT_CHANGED
	RUString m_clsRemoteIP;
	unsigned int m_unRemotePort;
	#endif
}IbcPde_t;
typedef struct IBCAddress_t : public eSIP::Net5Tuple_t
{
	/*
	resip::Data m_clsLocalIp;
	int m_nLocalPort;
	resip::Data m_clsRemoteIp;
	int m_nRemotePort;
	resip::TransportType m_eTransportType;
	*/
	void fnClear()
	{
		
	}
}IBCAddress_t ;
inline const char * g_fnStringTransport(unsigned char _Proto)
{
	switch(_Proto)
	{
		case E_ESIP_TRANSPORT_UDP: return "UDP";
		case E_ESIP_TRANSPORT_TCP: return "TCP";
		case E_ESIP_TRANSPORT_TLS: return "TLS";
		case E_ESIP_TRANSPORT_SCTP: return "SCTP";
		default: return "NONE";
	};
	return "NONE";
}
typedef struct IBCLOC_t : public SipNodeLocal_t
{
	IBCLOC_t(){memset(this,0x00, sizeof(SipNodeLocal_t));}
	char* m_fnDebug(char* _pBuf,int _nLen)
	{
		fnRUStrnCat(_pBuf,_nLen,
		"<<<LOC Info>>>\r\nID:%u, DESC:%s, Domain:%s, IP/Port(%u):%s/%u/%s, S/U/Idx:%s(%d)/%u/%u\r\n",
								m_uiID,fnRUStr(m_szDesc),fnRUStr(m_szDomain),
								m_usIPver,fnRUStr(m_szIP),m_usPort,g_fnStringTransport(m_ucProto),
								TRUE_SELECT(m_ucStatus == E_ESIP_STATUS_AVAIL,"AVAIL","UNAVAIL"),
								m_ucStatus,m_ucUsed,m_nIndex);
		return _pBuf;
	}
}IBCLOC_t;
typedef struct IBCRMT_t : public SipNodeRemote_t
{
	IBCRMT_t(){memset(this,0x00, sizeof(SipNodeRemote_t));}
	 char* m_fnDebug(char* _pBuf,int _nLen)
	{
		fnRUStrnCat(_pBuf,_nLen,
		"<<<RMT Info>>>\r\nID:%u, DESC:%s, Domain:%s, IP/Port(%u):%s/%u/%s, NAT/DSCP/IP/"\
		"Port=%d/%d/%s/%d, S/U/Idx:%s(%d)/%u/%u\r\n",
												m_uiID,fnRUStr(m_szDesc),fnRUStr(m_szDomain),
												m_usIPver,m_szIP,m_usPort,
												g_fnStringTransport(m_ucProto),
												m_sNATOn,m_nDSCP,m_szNATIP,m_usNATPort,
												m_ucStatus == E_ESIP_STATUS_AVAIL?"AVAIL":"UNAVAIL",
												m_ucStatus,m_ucUsed, m_nIndex);
		return _pBuf;
	}
}IBCRMT_t;
inline const char * g_fnStringRteStatus(short _eStatus)
{
	if(_eStatus == SipRoute_t::E_MANAGE_BYPASS ) return "BYPASS";
	else if(_eStatus == SipRoute_t::E_MANAGE_DEACT ) return "DEACT";
	return "NONE";
}
typedef struct IBCRTE_t : public SipRoute_t
{
	IBCRTE_t(){memset(this,0x00, sizeof(SipRoute_t));}
	char* m_fnDebug(char* _pBuf,int _nLen)
	{
		fnRUStrnCat(_pBuf,_nLen,
		"<<<RTE Info>>>\r\nID:%u, DESC:%s, LID/RID/TRTE:%u/%u/%u, Type/Media/Group:%s/%s/%u,"\
		" OPT_T/R/A/SES:%d/%d/%s/%d, S/U:%s(%d)/%u, CNT_T/IC/OG/Retry:%u/%u/%u/%u, Idx:%u\r\n",
		m_uiID,fnRUStr(m_szDesc),m_uiLID,m_uiRID, m_uiTRTE,
		m_ucType == E_TYPE_MINE ?"MINE":"OTHER",
		m_ucDoRouteMedia == E_MGW_ROUTED ?"ROUTE":"DIRECT",
		m_nRouteGroup, m_sOptTime,m_sOptRetry,g_fnStringRteStatus(m_sOptAction),m_sSesRefreshTime,
		m_ucStatus== E_ESIP_STATUS_AVAIL?"AVAIL":"UNAVAIL",m_ucStatus,m_ucUsed,
		m_uiBusyCnt,m_uiICCnt,m_uiOGCnt,m_nCurRetry,m_nIndex);
		return _pBuf;
	} 
}IBCRTE_t;
typedef struct IBCTrGWRTE_t : public TrGWRoute_t
{
	char* m_fnDebug(char* _pBuf,int _nLen)
	{
		char szTmpBuf[256]; szTmpBuf[0]=0x00;	char szStatus[128]; szStatus[0]=0x00;
		snprintf(szTmpBuf,sizeof(szTmpBuf)-1,"S(%s/%u), C(%s/%u)",
											m_szSvrIP,m_usSvrPort,m_szTrGWIP,m_usTrGWPort);
		if( m_nStatus == E_TRGW_STATUS_AVAIL )
		{
			strcpy(szStatus,"AVAIL");
		}
		else
		{
			if( (m_nStatus&E_TRGW_STATUS_MBLOCK) && (m_nStatus&E_TRGW_STATUS_FBLOCK))
			{
				strcpy(szStatus,"M&F_BLK");
			}
			else if ((m_nStatus&E_TRGW_STATUS_MBLOCK)) 
			{
				strcpy(szStatus,"M_BLK");
			}
			else if ((m_nStatus&E_TRGW_STATUS_FBLOCK)) 
			{
				strcpy(szStatus,"F_BLK");
			}
		}
		fnRUStrnCat(_pBuf,_nLen,
								"ID:%u, DESC:%s,%s, S:%s, Rate:%u, Cnt:%u/%u, U/Idx:%d/%d", 
								m_uiID,fnRUStr(m_szDesc),szTmpBuf,szStatus,m_nCallRate,m_nCurCount,m_nMaxCount,
								m_ucUsed,m_nIndex);
		return _pBuf;
	} 
}IBCTrGWRTE_t;
typedef struct IBCTrGWDataBase_t : public TrGWDataBase_t
{
	char * m_fnDebug(char * _pszDebug, unsigned int _unLen)
	{
		IBCTrGWRTE_t * pstRTE = (IBCTrGWRTE_t*)&m_stTrGW[0];
		fnRUStrnCat(_pszDebug,_unLen,"<<<IBCTrGW>>> m_nNumber=%d\r\n",m_nNumber);
		for(int i = 0;i<m_nNumber;i++) pstRTE[i].m_fnDebug(_pszDebug,_unLen);
		return _pszDebug;
	}
}IBCTrGWDataBase_t;
class IBCConfig
{
	public:
		IBCConfig();
		~IBCConfig();
		static IBCConfig * fnGetInstance();
		static void fnFinalize();
		void fnLoadIBCFile(const char * _szPath);
		void fnDebugIBC();
		static IBCConfig * g_pclsIbcConf;
		//========> System Info
		RUString m_clsSysName;
		RUString m_clsSysID;
		RUString m_clsCompName;
		RUString m_clsCompID;
		RUString m_clsCompSide;
		RUString m_clsCompPeerSide;
		RUString m_clsVmID;
		RUString m_clsMyOmcSrc;
		RUString m_clsPeerOmcSrc;
		unsigned int m_unSystemID;
		unsigned int m_unBoxID;
		unsigned int m_unTrgwNum;
		unsigned int m_unVmEnable;
		#ifdef DEF_R131
		RUString m_clsIpcListPath;
		eSipUtil::ColumFile m_clsIpcList;
		unsigned int m_unMaxCM;
		static void fnCbkIpcChanged(eSipUtil::ColumFile * _pclsObj);
		#endif
		//========> CDR Info
		bool m_bEnableCDR;
		RUString m_clsCdrPath;
		RUString m_clsCdrFileName;
		unsigned int m_unCdrWriteQNum;
		//========> TGAS
		IbcTgAs_t m_stTgAs;
		//========> PDE
		IbcPde_t m_stPde;
		RUString m_clsPath;
		//========> Rte Lib Info
		TrGWDataBase_t m_stTrGWLib;
		SipLocalNodeDataBase_t m_stLoc;
		//========> Service On/Off
		bool m_bEnableScreen;
		bool m_bEnableFilter;
		bool m_bEnableTrGW;
		bool m_bEnablePde;
		bool m_bEnableTrace;
		#ifdef DEF_R131
		bool m_bEnableEnum;
		#endif
		//========> Layer Manager
		RUString m_clsLmLocalIP;
		unsigned int m_unLmLocalPort;
		RUString m_clsLmRemoteIP;
		unsigned int m_unLmRemotePort;
		//========> OMC
		RUString m_clsOmcLocalPath;
		RUString m_clsOmcRemotePath;
		RUString m_clsOmcEquipName;
		RUString m_clsOmcProcName;
		RUString m_clsOmcTraceLocalIp;
		unsigned int m_unOmcTraceLocalPort;
		RUString m_clsOmcTraceRemoteIp;
		unsigned int m_unOmcTraceRemotePort;
		//========> Cfg Path
		RUString m_clsComPath;
		RUString m_clsCmdPath;
		RUString m_clsRulePath;
		RUString m_clsScreenPath;
		RUString m_clsRoutePath;
		RUString m_clsFilterPath;
		//========> Cfg Path
		RUString m_clsLogDir;
		RUString m_clsLogStackName;
		RUString m_clsLogCoreName;
		RUString m_clsLogOptName;
		RUString m_clsLogDumpName;
		RuCfgFile m_clsCfgCom;
		#ifdef DEF_R131
		//========> EnumIf
		RUString m_clsEnumLocalIP;
		unsigned int m_unEnumLocalPort;
		RUString m_clsEnumRemoteIP;
		unsigned int m_unEnumRemotePort;
		unsigned int m_unEnumSocBuff;
		unsigned int m_unEnumQNums;
		#endif
	private:
		RuCfgFile m_clsCfg;
		void fnWrapCfg();
};

#endif

