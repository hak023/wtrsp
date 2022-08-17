#ifndef ENUM_TRANSPORT_H
#define ENUM_TRANSPORT_H
#include "eSipUtil/transportMgr.hxx"
#include "eSipUtil/queueThread.hxx"
#include "eSipUtil/timerTable.hxx"
#include "eSipUtil/lock.hxx"
#include "eRule/CRUTime.h"
#include "EnumIF.h"
#include "main.h"
#include "CAppSession.h"
#include "eSipCallback.hxx"
#include "IBCDefine.h"
#ifdef DEF_R131
#define DEF_ERROR_ENUM_TID 1000000000
typedef enum
{
	E_CCM_ENUM_TYPE_NOME = 0,
	E_CCM_ENUM_TYPE_SERVER = 0,
	E_CCM_ENUM_TYPE_CLIENT = 0,
}ECcmEnumType_t;
typedef enum
{
	E_CCM_ENUM_CON_NONE = 0,
	E_CCM_ENUM_CON_ACCEPT,
	E_CCM_ENUM_CON_CONNECT,
	E_CCM_ENUM_CON_DISCONNECT,
	E_CCM_ENUM_CON_RECONNECTING,
	E_CCM_ENUM_CON_PING,
	E_CCM_ENUM_CON_PONG,
	E_CCM_ENUM_CON_RETRY,
	E_CCM_ENUM_ADD,
	E_CCM_ENUM_DEL,
	E_CCM_ENUM_CON_MAX
}ECcmConEv_t;
typedef struct CcmEnumEv_t
{
	CcmEnumEv_t(){m_eT = E_CCM_ENUM_CON_NONE;m_unRetryCnt=0;m_pszData=NULL;m_unDataLen=0;}
	~CcmEnumEv_t(){if(m_pszData) delete [] m_pszData;}
	void m_fnAllocCopy(char * _pszData, unsigned int _unLen)
	{
		if(m_pszData) delete [] m_pszData;
		m_pszData = new char[_unLen+1];
		memcpy(m_pszData,_pszData,_unLen); m_pszData[_unLen]=0x00;
		m_unDataLen = _unLen;
	}
	void m_fnFree()
	{
		if(m_pszData)
		{
			delete [] m_pszData;
			m_pszData = NULL;
			
		}
		m_eT = E_CCM_ENUM_CON_NONE;
		m_unDataLen = 0;
		m_unRetryCnt = 0;
	}
	ECcmConEv_t m_eT;
	char * m_pszData;
	unsigned int m_unDataLen;
	unsigned int m_unRetryCnt;
}CcmEnumEv_t;
class EnumTransport
{
	public:
		EnumTransport();
		~EnumTransport();
		static EnumTransport * m_fnGetInstance();
		void m_fnStart();
		void m_fnAdd();
		void m_fnDel();
		static void m_fnUp();
		static void m_fnDown();
		void m_fnSendPing();
		unsigned int m_fnQuery(unsigned int _unSesId,
								unsigned int _unDialogId,
								const char * _pszCallId,
								const char * _pszCaller,
								const char * _pszCalled,
								bool _bHlr,
								bool _bNpdb,
								const char * _pszMedia);
		void m_fnRecvQueryAck(char * _pszData,unsigned int _unLen);
		void m_fnRecvPong(CcmEnumEv_t * _pclsEv);
		void m_fnSend( char * _pSndData, unsigned int _unSndLen);
		unsigned int m_fnGetTid(unsigned int _unTime);
		void m_fnSendToTu(void * _pvData, unsigned int _unLen,
								const eSIP::PfuncStateResumeCallback_t  _pfn,
								bool _bTimeOut);    // ENUM  --> TU
		static eSIP::EStateCbResultType_t s_fnProcSndTuEvent(StackStateCbInfo_t & _rclsInfo);
		static eSIP::EStateCbResultType_t s_fnProcRcvTuEvent(StackResumCbInfo_t & _rclsInfo);
		static eSIP::EStateCbResultType_t s_fnProcRcvTuTimeOutEvent(StackResumCbInfo_t & _rclsInfo);
		static eSIP::EAsyncCbResultType_t m_fnRetransEnum(StackAsyncCbInfo_t & _rclsInfo);
		static eSIP::EStateCbResultType_t s_fnProcUacInvite(StackStateCbInfo_t & _rclsInfo);
		static void m_fnCbkProcConEv(CcmEnumEv_t * _pclsEv, void * _pvOwner);
		static void m_fnCbkProcTimeOut(CcmEnumEv_t * _pclsEv);
		static void s_fnCbkEnumRecvPacket(eSipUtil::TransportMgr & _rclsTransportMgr, 
													char * _pData, 
													unsigned int _unDataLen, 
													const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		static void s_fnCbkEnumSendPacket(eSipUtil::TransportMgr & _rclsTransportMgr, 
													const char * _pData, unsigned int _unDataLen, 
													const eSipUtil::Net5Tuple_t & _rstNet5Tuple, 
													void * _pSndAppData);
		static void s_fnCbkEnumDisconnected(eSipUtil::TransportMgr & _rclsTransportMgr, 
													const eSipUtil::Net5Tuple_t & _rstNet5Tuple, 
													eSipUtil::ETransportErr_t _eTrspErr);
		static void s_fnCbkEnumAcceptConnected(eSipUtil::TransportMgr & _rclsTransportMgr, 
														eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		static void s_fnCbkEnumClientConnected(eSipUtil::TransportMgr & _rclsTransportMgr, 
														const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		static int s_fnCbkEnumAnalizePacket(eSipUtil::TransportMgr & _rclsTransportMgr, 
													const char * _pRcvData, 
													unsigned int _unRcvDataLen, 
													const eSipUtil::Net5Tuple_t & _rstNet5Tuple, 
													void * _pAppBuffer);
		eSipUtil::TimerTable<CcmEnumEv_t> m_clsEnumTimer;
		eSipUtil::TransportConfig_t m_stTransportConfig;
		eSipUtil::Net5Tuple_t m_stAddr;
		eSipUtil::TransportMgrOption m_stOption;
		eSipUtil::SocketCfg_t m_stSocketConfig;
		eSipUtil::TransportMgr * m_pclsTransport;
		eSipUtil::KString m_clsName;
		eSipUtil::QueueThread<CcmEnumEv_t> *m_pclsQ;
		bool m_bConnected;
		eSipUtil::RwMutex m_clsLock;
		static EnumTransport * m_pclsMy;
		unsigned int m_unTid;
		#ifdef DEF_R131
		unsigned int m_unMax;
		unsigned int m_unConvCnt;
		#endif
		RUDailyTime m_clsTime;
		unsigned int m_unPingCnt;
		bool m_bAvailNpdb;
		bool m_bAvailHlr;
};
void g_fnCreateEnumIf();
#endif  // def DEF_R131
#endif
