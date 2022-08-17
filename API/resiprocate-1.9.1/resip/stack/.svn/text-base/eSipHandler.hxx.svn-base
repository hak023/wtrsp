	
#ifndef RESIP_ESIP_HANDLER_HXX
#define RESIP_ESIP_HANDLER_HXX

#ifdef ESIP_IN_RESIP

#include <time.h>

#include "rutil/TransportType.hxx"
#include "resip/stack/MethodTypes.hxx"
#include "resip/stack/Tuple.hxx"
#include "resip/stack/HeaderTypes.hxx"
#include "resip/stack/eSipSimpleParsing.hxx"

#include "eRte/eSipRteApi.hxx"
#include "eRte/eSipRteErr.hxx"

#include "eSipUtil/transportMgr.hxx"
#include "eSipUtil/define.hxx"
#include "ext/slb/Slb_Ccm_Msg.hxx"

namespace resip
{

class SipMessage;
class StatisticsManager;
class Transport;
class SipStack;
class InternalTransport;
class SendData;
class TransactionController;
class TransportMgrWithSlb;

typedef enum
{
	E_DROP_TYPE_acl = 0,						//if req, send err rsp
	E_DROP_TYPE_cps,							//if req, send err rsp
	E_DROP_TYPE_tps,							//if req, send err rsp
	E_DROP_TYPE_cpuOver,					//if req, send err rsp
	E_DROP_TYPE_memOver,				//if req, send err rsp
	E_DROP_TYPE_msgSizeErr,				//if req, send err rsp
	E_DROP_TYPE_msgSizeDrop,			//drop
	E_DROP_TYPE_sipCallIdError,			//drop
	E_DROP_TYPE_sipViaError,				//drop
	E_DROP_TYPE_sipFromError,			//drop
	E_DROP_TYPE_sipToError,				//drop
	E_DROP_TYPE_sipCseqError,			//drop
	E_DROP_TYPE_sipContactError,		//drop
	E_DROP_TYPE_sipContentLenError,	//drop
	E_DROP_TYPE_sipReqLineError,		//drop
	E_DROP_TYPE_sipStatusLineError,	//drop
	E_DROP_TYPE_sipOtherError,			//drop
	E_DROP_TYPE_systemShutdown,	//drop
	
	E_DROP_TYPE_NUM,
}EMsgDropType_t;

const char * getMsgDropDesc(EMsgDropType_t _eMsgDropType);
EMsgDropType_t getMsgDropType2HeaderType(Headers::Type _eHeader);
Headers::Type getHeaderType2MsgDropType(EMsgDropType_t _eMsgDropType);

typedef enum
{
	E_DELAY_TEST_TYPE_TRSP = 0,
	E_DELAY_TEST_TYPE_MIDDLE_TRSC,
	E_DELAY_TEST_TYPE_MONITOR,

	E_DELAY_TEST_TYPE_NUM,
}EDelayTestType_t;

unsigned int getResetCnt4DelayTest(EDelayTestType_t _eDelayTestType);
unsigned int getLimitMs4DelayTest(EDelayTestType_t _eDelayTestType);

struct TraceInfo
{
	enum
	{
		E_CONST_URI_USERNAME_SIZE = 42,
		E_CONST_CALLID_SIZE = 256,
	};

	bool m_bTraceFlag;
	int m_nTraceNum;
	SipCoreTrace_t m_arrTrace[SipTraceDataBase_t::E_MAXNUM];
	char m_szCaller[E_CONST_URI_USERNAME_SIZE];
	char m_szCallee[E_CONST_URI_USERNAME_SIZE];
	char m_szCallId[E_CONST_CALLID_SIZE];

	void reset()
	{
		m_bTraceFlag = false;
		m_nTraceNum = 0;
		memset(m_arrTrace, 0x00, sizeof(SipCoreTrace_t)*SipTraceDataBase_t::E_MAXNUM);
		m_szCaller[0] = 0x00;
		m_szCallee[0] = 0x00;
		m_szCallId[0] = 0x00;
	}

	TraceInfo()
	{
		reset();
	}

	TraceInfo & operator=(const TraceInfo & rhs)
	{
		m_bTraceFlag = rhs.m_bTraceFlag;
		strcpy(m_szCaller, rhs.m_szCaller);
		strcpy(m_szCallee, rhs.m_szCallee);
		strcpy(m_szCallId, rhs.m_szCallId);

		if( rhs.m_bTraceFlag )
		{
			m_nTraceNum = rhs.m_nTraceNum;
			memcpy(m_arrTrace, rhs.m_arrTrace, sizeof(SipCoreTrace_t)*rhs.m_nTraceNum);
		}
		else
		{
			m_nTraceNum = 0;
		}

		return *this;
	}
};
typedef struct TraceInfo TraceInfo_t;

struct SipParsingInfo
{
	resip::MethodTypes m_eMethodType;
	int m_nRspCode;
	bool m_bInitial;
	bool m_bVideo;		//true : video, false : audio
	const char * m_pszFromUsername;
	const char * m_pszFromPhoneCtx;
	const char * m_pszFromTag;	
	const char * m_pszToUsername;
	const char * m_pszToPhoneCtx;
	const char * m_pszToTag;
	const char * m_pszCallId;

	void apply(const SipMessage & _rclsSipMsg);
	void reset();
};

typedef resip::Data (* PfunGenerateStackTag)(int _nRspCode, const char * _pszAdditionToTag);
typedef void (* PfuncRcvTraceCallback)(const char * _pszSipString, unsigned int _unLen,
																			const SipNodeLocal_t & _rstNodeLocal, const SipNodeRemote_t & _rstNodeRemote, const SipRoute_t & _rstRoute,
																			const SimpleParsingData & _rstSimpleParsing, TraceInfo_t & _rstTraceInfo);
typedef void (* PfuncSndTraceCallback)(const char * _pSipString, unsigned int _unLen,
																			const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType,
																			unsigned int _unLocalNodeId, unsigned int _unRemoteNodeId, unsigned int _unRouteId,
																			const TraceInfo_t & _rstTraceInfo);
typedef EMsgDropType_t (* PfuncCheckOverloadCallback)(resip::Transport * _pclsTransport, const char * _pBuffer, unsigned int _unLen,
																										const Tuple & _rclsLocalTuple, const Tuple & _rclsRemoteTuple,
																										const SipNodeLocal_t & _rstNodeLocal, const SipNodeRemote_t & _rstNodeRemote, const SipRoute_t & _rstRoute,
																										const SimpleParsingData & _rstSimpleParsing, const TraceInfo_t & _rstTraceInfo, bool & _bTcpRelay, bool & _bNoDropCnt);
typedef EMsgDropType_t (* PfuncCheckOverload4SlbCallback)(TransportMgrWithSlb & _rclsTrspMgr, const char * _pBuffer, unsigned int _unLen,
																													const eSipUtil::Net5Tuple & _rstRcvNet5Tuple,
																													const Tuple & _rclsLocalTuple, const Tuple & _rclsRemoteTuple,
																													const SipNodeLocal_t & _rstNodeLocal, const SipNodeRemote_t & _rstNodeRemote, const SipRoute_t & _rstRoute,
																													const SimpleParsingData & _rstSimpleParsing, const TraceInfo_t & _rstTraceInfo, bool & _bTcpRelay, bool & _bNoDropCnt);
typedef EMsgDropType_t (* PfuncCheckSipCallback)(Transport * _pclsTransport, const SipMessage & _rclsSipMsg, const char * _pBuffer, unsigned int _unLen,
																								const Tuple & _rclsRemote, int _nRouteId, const TraceInfo_t & _rstTrace);
typedef EMsgDropType_t (* PfuncCheckSip4SlbCallback)(TransportMgrWithSlb & _rclsTrspMgr, const SipMessage & _rclsSipMsg, const char * _pBuffer, unsigned int _unLen,
																										const Tuple & _rclsRemote, int _nRouteId, const TraceInfo_t & _rstTrace);

typedef void (* PfuncFaultMsgCallback)(char * _pszDesc);

TransportType convertNetTrspType2ResipType(eSipUtil::ETransportType_t _eNetTrspType);
bool convertNet5TupleToResipTuple(const eSipUtil::Net5Tuple & _rstNet5Tuple, Tuple & _rclsLocal, Tuple & _rclsRemote);

class eSipHandler
{
	public :
		eSipHandler();
		~eSipHandler();

		bool doRcvTransportJob(SipMessage & _rclsSipMsg, const char * _pszSip);
		bool doRcvTransportJob4WithSlb(SipMessage & _rclsSipMsg, const char * _pszSip, const SlbInternalSipInfo & _rstInternalSipInfo);
		bool checkSip(Transport * _pclsTransport, 
										SipMessage & _rclsSipMsg, const char * _pBuffer, unsigned int _unLen, 
										const Tuple & _rclsRemote, int _nRouteId, const TraceInfo_t & _rstTrace);
		bool checkSip4Slb(TransportMgrWithSlb & _rclsTrspMgr, 
												SipMessage & _rclsSipMsg, const char * _pBuffer, unsigned int _unLen, 
												const Tuple & _rclsRemote, int _nRouteId, const TraceInfo_t & _rstTrace);

		/*==== Callback ====*/
		inline void setGenToTag4RspCallback(PfunGenerateStackTag _pfnGenTagCb){ m_pfnGenerateToTag4Rsp = _pfnGenTagCb; }
		inline PfunGenerateStackTag getGenToTag4RspCallback() const{ return m_pfnGenerateToTag4Rsp; }
		inline void setRcvTraceCallback(PfuncRcvTraceCallback _pfnRcvTraceCb){ m_pfnRcvTraceCb = _pfnRcvTraceCb; }
		inline void setSndTraceCallback(PfuncSndTraceCallback _pfnSndTraceCb){ m_pfnSndTraceCb = _pfnSndTraceCb; }		
		inline void setCheckOverloadCallback(PfuncCheckOverloadCallback _pfnCheckOverloadCb){ m_pfnCheckOverload = _pfnCheckOverloadCb; }
		inline void setCheckOverload4SlbCallback(PfuncCheckOverload4SlbCallback _pfnCheckOverloadCb){ m_pfnCheckOverload4Slb = _pfnCheckOverloadCb; }
		inline void setCheckSipCallback(PfuncCheckSipCallback _pfnCheckSipCb){ m_pfnCheckSip = _pfnCheckSipCb; }
		inline void setCheckSip4SlbCallback(PfuncCheckSip4SlbCallback _pfnCheckSipCb){ m_pfnCheckSip4Slb = _pfnCheckSipCb; }
		inline void setFaultMsgCallback(PfuncFaultMsgCallback _pfnFaultMsgCb){ m_pfnFaultMsgCb = _pfnFaultMsgCb; }
		inline PfuncFaultMsgCallback getFaultMsgCallback(){ return m_pfnFaultMsgCb; }

		/*==== On Rcv/Snd ====*/
		void onRecv(const SipMessage & _rclsSipMsg, const char * _pszSipString, unsigned int _unLen,
								const SipNodeLocal_t & _rstNodeLocal, const SipNodeRemote_t & _rstNodeRemote, const SipRoute_t & _rstRoute,
								const SimpleParsingData & _rstSimpleParsing, TraceInfo_t & _rstTraceInfo);
		void onRecvDrop(EMsgDropType_t eMsgDropType, MethodTypes _eMsgMethodType, int _nRspCode,
											const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType);
		void onRecvDropByTransc(EMsgDropType_t eMsgDropType, MethodTypes _eMethodType, int _nRspCode, 
																const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType);
		void onRecvRetrans(const SipMessage & _rclsSipMsg);
		void onSend(const Data & _rclsSipMsg, unsigned int _unLen, MethodTypes _eMsgMethodType, int _nRspCode,
									const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType,
									unsigned int _unLocalNodeId, unsigned int _unRemoteNodeId, unsigned int _unRouteId,
									const TraceInfo_t * _pstTraceInfo, bool _bRetrans);
		void onSend(const char * _pszSipMsg, unsigned int _unLen, MethodTypes _eMsgMethodType, int _nRspCode,
									const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType,
									unsigned int _unLocalNodeId, unsigned int _unRemoteNodeId, unsigned int _unRouteId,
									const TraceInfo_t * _pstTraceInfo, bool _bRetrans);

		inline void setDumpSip(bool _bDumpFlag){ m_bDumpSip = _bDumpFlag; }
		void setDumpSip4Cps(bool _bDumpFlag4Cps);
		inline bool getDumpSip4Cps() const{ return m_bDumpSip4Cps; }
		inline void setStatMgr(StatisticsManager * _pclsStatMgr){ m_pclsStatMgr = _pclsStatMgr; }
		void setSipStack(SipStack * _pclsSipStack);

		/*==== stack Test ====*/
		void setTimeTestFlag(bool _bTimeTest);
	
	private :
		void dumpSip(bool _bRcv, const char * _pszSipString, unsigned int _unLen,
									const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType);
		void setRte(const SimpleParsingData & _rstParsingData,
								const Tuple & _rclsLocal, const Tuple & _rclsRemote, 
								SipNodeLocal_t & _rstNodeLocal, SipNodeRemote_t & _rstNodeRemote, SipRoute_t & _rstRoute);
		bool checkOverload(Transport * _pclsTransport, 
													SipMessage & _rclsSipMsg, const char * _pBuffer, unsigned int _unLen,
													const Tuple & _rclsLocal, const Tuple & _rclsRemote, 
													const SipNodeLocal_t & _rstNodeLocal, const SipNodeRemote_t & _rstNodeRemote, const SipRoute_t & _rstRoute,
													const SimpleParsingData & _rstSimpleParsing, const TraceInfo_t & _rclsTrace);
		bool checkOverload4Slb(TransportMgrWithSlb & _rclsTrspMgr,
															SipMessage & _rclsSipMsg, const char * _pBuffer, unsigned int _unLen,
															const Tuple & _rclsLocal, const Tuple & _rclsRemote, 
															const SipNodeLocal_t & _rstNodeLocal, const SipNodeRemote_t & _rstNodeRemote, const SipRoute_t & _rstRoute,
															const SimpleParsingData & _rstSimpleParsing, const TraceInfo_t & _rclsTrace);
		
		/*==== callback ====*/
		PfuncRcvTraceCallback m_pfnRcvTraceCb;
		PfuncSndTraceCallback m_pfnSndTraceCb;
		PfunGenerateStackTag m_pfnGenerateToTag4Rsp;
		PfuncCheckOverloadCallback m_pfnCheckOverload;
		PfuncCheckOverload4SlbCallback m_pfnCheckOverload4Slb;
		PfuncCheckSipCallback m_pfnCheckSip;
		PfuncCheckSip4SlbCallback m_pfnCheckSip4Slb;
		PfuncFaultMsgCallback m_pfnFaultMsgCb;

		/*==== dump sip ====*/
		bool m_bDumpSip;
		bool m_bDumpSip4Cps;

		StatisticsManager * m_pclsStatMgr;
		SipStack * m_pclsSipStack;
		
};

typedef void (* PfuncQueueFull)(const char * _pszQueueName, int _nCurrentRate, int _nLimitRate);
typedef void (* PfuncDelTransport4Slb)(TransportMgrWithSlb & _rclsTrspMgrWithSlb, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, eSipUtil::ETransportErr_t _eTrspErr);
typedef void (* PfuncConnected4NonBlock4Slb)(TransportMgrWithSlb & _rclsTrspMgrWithSlb, const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
typedef void (* PfuncRcvLbManageMsg4Slb)(TransportMgrWithSlb & _rclsTrspMgrWithSlb, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, SlbInternalMsgHdr * _pstInternalMsgHdr);

class TransportMgrWithSlb
{
	public :
		TransportMgrWithSlb(SipStack & _rclsSipStack, eSipHandler & _rclsEsipHandler, PfuncQueueFull _pfnQueueFullCb, unsigned int _unMaxSipLen4Udp);
		~TransportMgrWithSlb();

		/*==== callback ====*/
		inline void m_fnSetDelTransportCb(PfuncDelTransport4Slb _pfnCb){ m_pfnDelTransportCb = _pfnCb; }
		inline void m_fnSetConnected4NonBlockCb(PfuncConnected4NonBlock4Slb _pfnCb){ m_pfnConnected4NonblockCb = _pfnCb; }
		inline void m_fnSetRcvLbManageMsgCb(PfuncRcvLbManageMsg4Slb _pfnCb){ m_pfnRcvLbManageMsgCb = _pfnCb; }
		
		bool m_fnAddTransport(const eSipUtil::Net5Tuple_t & _rstNet5Tuple, unsigned int _unSocketRcvBufferSize, unsigned int _unSocketSndBufferSize);
		void m_fnDelTransport(const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		void m_fnSetBlock(bool _bBlock){ m_bBlock = _bBlock; }
		bool m_fnGetBlock() const{ return m_bBlock; }
		
		bool m_fnSendSip(const eSipUtil::Net5Tuple_t & _rstSlbTuple, const char * _pszSipMsg, unsigned int _unSipLen, 
												MethodTypes _eMethodType, int _nRspCode, const char * _pszCallId,
												unsigned int _unLocalNodeId, unsigned int _unRemoteNodeId, unsigned int _unRouteId,
												const TraceInfo_t & _rstTraceInfo, bool _bRetrans);
		bool m_fnSendInit(const char * _pszVcmId, const char * _pszTagPrefix, const char * _pszStackTagPrefix, unsigned int _unMaxSessNum);
		bool m_fnSendPing(const char * _pszVcmId, bool _bActive, unsigned int _unActiveSessNum);
		
		eSipHandler & m_fnGetEsipHandler(){ return m_rclsEsipHandler; }

	private :
		struct SndSipInfo
		{
			MethodTypes m_eMsgMethodType;
			int m_nRspCode;
			eSipUtil::Net5Tuple m_stSlbSndTuple;
			unsigned int m_unLocalNodeId;
			unsigned int m_unRemoteNodeId;
			unsigned int m_unRouteId;
			TraceInfo_t m_stTraceInfo;
			bool m_bRetrans;

			void reset()
			{
				m_eMsgMethodType = UNKNOWN;
				m_nRspCode = 0;
				m_stSlbSndTuple.reset();
				m_unLocalNodeId = 0;
				m_unRemoteNodeId = 0;
				m_unRouteId = 0;
				m_stTraceInfo.reset();
				m_bRetrans = false;
			}
		};
		
		static void m_fnCbRecv(eSipUtil::TransportMgr & _rclsTransportMgr, char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstRcvTuple);
		static void m_fnCbSndSucc(eSipUtil::TransportMgr & _rclsTransportMgr, const char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstSndTuple, void * _pSndAppData);
		static void m_fnCbDelTransport(eSipUtil::TransportMgr & _rclsTransportMgr, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, eSipUtil::ETransportErr_t _eTrspErr);
		static void m_fnCbConnected4NonBlock(eSipUtil::TransportMgr & _rclsTransportMgr, const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		static int m_fnCbCheckCompleteMsg4Rcv(eSipUtil::TransportMgr & _rclsTransportMgr, const char * _pRcvData, unsigned int _unRcvDataLen, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, void * _pAppBuffer);
		
		SipStack & m_rclsSipStack;
		eSipHandler & m_rclsEsipHandler;
		eSipUtil::TransportMgr * m_pclsInternalTrspMgr;

		PfuncDelTransport4Slb m_pfnDelTransportCb;
		PfuncConnected4NonBlock4Slb m_pfnConnected4NonblockCb;
		PfuncRcvLbManageMsg4Slb m_pfnRcvLbManageMsgCb;
		
		unsigned int m_unRcvBufferSize;
		unsigned int m_unSndBufferSize;
		unsigned int m_unSocketRcvBufferSize;
		unsigned int m_unSocketSndBufferSize;
		eSipUtil::Net5Tuple_t  m_stNet5Tuple;

		bool m_bBlock;
		
};

}

#endif

#endif

