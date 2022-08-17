#ifndef CCMDCONFIG_H
#define  CCMDCONFIG_H
#include "eRule/RuleInterface.h"
#include "eRule/CRUFile.h"
#include "eSipCommand.hxx"
#include "eRte/eSipRteApi.hxx"
#include "eRule/CRuCfg.h"
#include "IBCDefine.h"
#ifdef DEF_R131
#include "eSipUtil/ConfigFile.hxx"
#include "eSipUtil/StlMap.hxx"
#include "eSipUtil/string.hxx"
#endif

class RcsRteID : public RUObject
{
	public:
		RcsRteID(){m_unRteID=0;}
		~RcsRteID(){}
		unsigned int m_unRteID;
};
class RcsRteIDs
{
	public:
		RcsRteIDs(){}
		~RcsRteIDs(){}
		void fnClear(){m_clsList.fnClear();}
		void fnParse(const char * _pszValues)
		{
			fnClear();
			RUString clsValues(_pszValues);
			RUArrayString clsArr;
			RUString::fnTokenizeC(clsValues.m_pszString," ,",&clsArr);
			RuListItem_t clsIter;
			char * pszVal = clsArr.fnBegin(clsIter,NULL);
			while(pszVal)
			{
				fnInsertID(fnRUAtoi(pszVal));
				pszVal = clsArr.fnNext(clsIter,NULL);
			}
			fnDebug();
		}
		bool fnInsertID(unsigned int _unID)
		{
			RcsRteID * pclsNew = new RcsRteID;
			pclsNew->fnSetKeyI(_unID);pclsNew->m_unRteID = _unID;
			return m_clsList.fnPushBack(pclsNew,true);
		}
		bool fnIsRcsRte(unsigned int _unID)
		{
			if(m_clsList.fnFindNode(_unID)) return true;
			return false;
		}
		void fnDebug()
		{
			RUString clsDebug(1024);RuListItem_t clsIter;
			RcsRteID * pclsFind = (RcsRteID*)m_clsList.fnBegin(clsIter);
			clsDebug<<"RCS Rte Id List = ";
			while(pclsFind)
			{
				clsDebug<<RUIntString(pclsFind->m_unRteID)<<",";
				pclsFind = (RcsRteID*)m_clsList.fnNext(clsIter);
			}
			RLOGvn((RUSTR)clsDebug);
		}
		RUList m_clsList;
};
#ifdef DEF_R131
class PointCode : public eSipUtil::StlObject
{
	public:
		PointCode(){}
		~PointCode(){}
		eSipUtil::KString m_clsPC;
		eSipUtil::KString m_clsDrtn;
};
class ChargingVectorOption
{
	public:
		ChargingVectorOption(){m_bInviteOrig=false;m_b200OKOrig=false;m_b200OkTerm=false;}
		~ChargingVectorOption(){}
		void m_fnDebug(eSipUtil::KString &_rclsDebug)
		{
			_rclsDebug<<"m_bInviteOrig: "<<m_bInviteOrig<<"\n";
			_rclsDebug<<"m_b200OKOrig: "<<m_b200OKOrig<<"\n";
			_rclsDebug<<"m_b200OkTerm: "<<m_b200OkTerm<<"\n";
		}
		bool m_bInviteOrig;
		bool m_b200OKOrig;
		bool m_b200OkTerm;
};
#endif
class IBCCmdConfig
{
	public:
		IBCCmdConfig();
		~IBCCmdConfig();
		static IBCCmdConfig * fnGetInstance();
		static void fnFinalize();
		static void fnCbkChanged(void * _pvFile);
		static void fnCheckChange();
		void fnLoadCmdFile(const char * _szPath);
		void fnReLoadCmdFile();
		bool fnIsRcsRteID(unsigned int _unID)
		{
			fnRLock();
			bool bRet = m_clsRteIDs.fnIsRcsRte(_unID);
			fnUnlock();
			return bRet;
		}
		static IBCCmdConfig * g_pclsIbcCmdConf;
		bool m_bDebug;
		unsigned int m_unDebugTime;
		unsigned int m_unHoldingTime;
		unsigned int m_unCdrMaxDays;
		unsigned int m_unMaxLogStack;
		unsigned int m_unMaxLogOpt;
		unsigned int m_unMaxLogCore;
		unsigned int m_unMaxLogDump;
		RUString m_clsPath;
		FileChangedInfo m_stChkInfo;
		bool m_bTraceDetail;
		bool m_bRoutingDetail;
		bool m_bScreenDetail;
		bool m_bPdeDetail;
		bool m_bFilterDetail;
		bool m_bTgAsDetail;
		#ifdef DEF_R131
		bool m_bEnumDetail;
		#endif
		bool m_bCheckAppTimer;
		unsigned int m_unTgAsBindTimeOutCnt;
		unsigned int m_unTgASPingTimeOutCnt;
		unsigned int m_unTgASHAWaiteTimeMs;
		unsigned int m_unTgASHBLogEnable;
		unsigned int m_unTgASMaxRate;
		unsigned int m_unTgASAlarmTimeOut;
		#ifdef DEF_TRGW_RETRANS
		unsigned int m_unTgASRetransMs;
		unsigned int m_unTgASRetransTimeOutMs;
		#endif
		#ifdef DEF_PDE_RETRANS
		unsigned int m_unPdeRetransMs;
		unsigned int m_unPdeRetransTimeOutMs;
		#endif
		RcsRteIDs m_clsRteIDs;
		#ifdef DEF_R131
		bool m_bEnableEnumCSFB;
		unsigned int m_unEnumRetryCnt;
		unsigned int m_unEnumRetryTimeMs;
		unsigned int m_unEnumMaxPingCnt;
		unsigned int m_unEnumPingTimeMs;
		eSipUtil::CfgFile m_clsPointCode;
		eSipUtil::StlMap m_mapPointCode;
		ChargingVectorOption m_clsNuguCall;
		ChargingVectorOption m_clsNcbOtherCall;
		ChargingVectorOption m_clsNcbMineCall;
		ChargingVectorOption m_clsBasicVectorOpt;
		void m_fnLoadPointCode();
		static void m_fnCbkChangedPointCode(eSipUtil::CfgFile * _pclsObj);
		const char * m_fnFindDrtn(const char * _pszPointCode, eSipUtil::KString & _rclsRet);
		#endif
	private:
		void fnWrapCfg();
		void fnWLock(){m_Mutex.writelock();}
		void fnRLock(){m_Mutex.readlock();}
		void fnUnlock(){m_Mutex.unlock();}
		resip::RWMutex m_Mutex;
		RuCfgFile m_clsCfg;
};
#endif

