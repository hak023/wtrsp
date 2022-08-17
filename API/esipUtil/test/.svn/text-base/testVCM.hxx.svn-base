
#ifndef ESIPUTIL_TEST_VCM_H
#define ESIPUTIL_TEST_VCM_H

#include <stdlib.h>
#include <string>

#include "ext/slb/Slb_Ccm_Msg.hxx"

#include "eSipUtil/timerTable.hxx"

#include "transportMgr.hxx"
#include "define.hxx"
#include "queueThread.hxx"

struct TransportEntity;

struct RcvInfo
{
	enum
	{
		E_CONST_MAX_CALLID_LEN = 256,
	};

	eSipUtil::Net5Tuple_t m_stRcvNet5Tuple;
	char m_szCallId[E_CONST_MAX_CALLID_LEN];
	SlbInternalMsgHdr * m_pRcvInternalMsgHdr;

	RcvInfo()
	{
		m_szCallId[0] = 0x00;
		m_pRcvInternalMsgHdr = NULL;
	}

	~RcvInfo()
	{
		free(m_pRcvInternalMsgHdr);
	}

	SlbInternalMsgHdr * m_fnGetHdrOwn()
	{
		SlbInternalMsgHdr * pTmp = m_pRcvInternalMsgHdr;
		m_pRcvInternalMsgHdr = NULL;
		return pTmp;
	}

	char * m_fnGetCallId(){ return m_szCallId; }
	const char * m_fnGetCallId() const{ return m_szCallId; }
};

struct SndInfo
{
	eSipUtil::Net5Tuple_t m_stSndNet5Tuple;
	void * m_pSndVcmInfo;

	SndInfo()
	{
		m_pSndVcmInfo = NULL;
	}

	~SndInfo()
	{
		free(m_pSndVcmInfo);
	}
};

typedef enum
{
	E_TIMER_TYPE_ping = 0,

	E_TIMER_TYPE_NUM,
}ETimerType;

struct TimerInfo
{
	ETimerType m_eTimerType;
	unsigned int m_unTimerMillicSec;
	eSipUtil::Net5Tuple m_stNet5Tuple;

	TimerInfo()
	{
		m_eTimerType = E_TIMER_TYPE_NUM;
		m_unTimerMillicSec = 0;
	}
};

class SimulVCM
{
	public :
		enum
		{
			E_CONST_WORKER_NUM = 4,
		};

		static void m_fnInit();
		static void m_fnSendTo(SndInfo * _pstSndData);

		static int cbCheckCompleteSlbMsg4Rcv(eSipUtil::TransportMgr & _rclsTransportMgr, const char * _pRcvData, unsigned int _unRcvDataLen, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, void * _pAppBuffer);
		static void cbRecv(eSipUtil::TransportMgr & _rclsTransportMgr, char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstRcvTuple);
		static void cbSnd(eSipUtil::TransportMgr & _rclsTransportMgr, const char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstSndTuple, void * _pAppBuffer);
		static void cbConnected4NonBlock(eSipUtil::TransportMgr & _rclsTransportMgr, const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		
	private :
		static void m_fnCbProcTimer(TimerInfo * _pstTimerInfo);
		static bool m_fnChangeTag(bool _bAdd, const char * _pHdrStartPos, const char * _pHdrEndPos, const char ** _pTagStartPos, const char ** _pTagEndPos, std::string & _rclsNewTag);
		static bool m_fnChangeSip4Snd(const char * _pszSrcSip, char * _pszDstSip);
		static void m_fnRecvProc(RcvInfo * _pstRcvData, void * _pObject);
		static void m_fnRecvProc4Sip(SlbInternalMsgHdr & _rstRcvMsgHdr, const eSipUtil::Net5Tuple & _rstInternalRcvTuple, const char * _pszCallId);
		static SlbInternalMsgHdr * m_fnMakeInternalMsg(EInternalMsgType_t _eMsgType, const eSipUtil::Net5Tuple * _pstSndTuple, unsigned int * _punSipLen);
		static void m_fnDetermineTargetTuple4SndSip(const TransportEntity & _rstTrspEntity, const SlbInternalSipInfo & _rstInternalSipInfo, const char * _pszRcvSip, eSipUtil::Net5Tuple & _rstTagetTuple);
		
		static eSipUtil::QueueThread< RcvInfo > m_arrRcvThread[E_CONST_WORKER_NUM];
		static eSipUtil::TimerTable< TimerInfo > m_clsTimerTable;
		static char m_szTagPrefix[128];
		static char m_szStackTagPrefix[128];

};

#endif

