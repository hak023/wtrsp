#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#ifndef WIN32
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "rutil/compat.hxx"
#include "rutil/Data.hxx"
#include "rutil/DnsUtil.hxx"
#include "rutil/Fifo.hxx"
#include "rutil/Logger.hxx"
#include "rutil/Random.hxx"
#include "rutil/Socket.hxx"
#include "rutil/Timer.hxx"
#include "rutil/FdPoll.hxx"

#include "rutil/dns/DnsThread.hxx"
#include "resip/stack/Message.hxx"
#include "resip/stack/ShutdownMessage.hxx"
#include "resip/stack/SipMessage.hxx"
#include "resip/stack/ApplicationMessage.hxx"
#include "resip/stack/SipStack.hxx"
#include "rutil/Inserter.hxx"
#include "resip/stack/StatisticsManager.hxx"
#include "rutil/AsyncProcessHandler.hxx"
#include "resip/stack/TcpTransport.hxx"
#include "resip/stack/UdpTransport.hxx"
#include "resip/stack/TransactionUser.hxx"
#include "resip/stack/TransactionUserMessage.hxx"
#include "resip/stack/TransactionControllerThread.hxx"
#include "resip/stack/TransportSelectorThread.hxx"
#include "rutil/WinLeakCheck.hxx"

#ifdef USE_SSL
#include "resip/stack/ssl/Security.hxx"
#include "resip/stack/ssl/DtlsTransport.hxx"
#include "resip/stack/ssl/TlsTransport.hxx"
#endif

#ifdef ESIP_IN_RESIP
#include <sys/syscall.h>
#include "eSipUtil/timeUtil.hxx"
#endif

#if defined(WIN32) && !defined(__GNUC__)
#pragma warning( disable : 4355 )
#endif

using namespace resip;

#define RESIPROCATE_SUBSYSTEM Subsystem::SIP

#ifdef ESIP_IN_RESIP

eSipHandler * g_pclsEsipHandler = NULL;

const char * resip::g_fnGetStackQueueAlarmType(int _n)
{
	if( _n == E_STACK_QUEUE_ALARM_TYPE_middle )
		return "STACK_QALARM_middle";
	else if( _n == E_STACK_QUEUE_ALARM_TYPE_trsc )
		return "STACK_QALARM_trsc";
	else if( _n == E_STACK_QUEUE_ALARM_TYPE_tu )
		return "STACK_QALARM_tu";
	else
		return "";
}

MiddleTranscFifoThread4Perform::MiddleTranscFifoThread4Perform(SipStack & _rclsSipStack)
	: m_rclsSipStack(_rclsSipStack), m_clsMiddleFifo(NULL), m_clsConsumerMiddleFifo(m_clsMiddleFifo)
{
	Data clsFifoName = "MiddleTrsc";
	m_clsMiddleFifo.setDescription(clsFifoName);
}

MiddleTranscFifoThread4Perform::~MiddleTranscFifoThread4Perform()
{
}

bool MiddleTranscFifoThread4Perform::add(SipStack * _pclsSipStack, SipMessage * _pclsSipMsg)
{
	if( !_pclsSipStack )
	{
		ErrLog(<<"MiddleTrscTh : add : fail : invalid SipStack");
		return false;
	}

	if( !_pclsSipMsg )
	{
		ErrLog(<<"MiddleTrscTh : add : fail : invalid SipMsg");
		return false;
	}

	if( _pclsSipStack->m_pclsMiddleTranscPerformThread )
	{
		DebugLog(<<"MiddleTrscTh : add : send to MiddlePerformThread : CallId["<<_pclsSipMsg->getSimpleCallId()<<"]");

		unsigned int unCurrentSize = 0, unMaxSize = 0;
		if( _pclsSipStack->m_pclsMiddleTranscPerformThread->m_clsMiddleFifo.add4FailNoDelete(_pclsSipMsg, unCurrentSize, unMaxSize) )
			return true;
		else
		{
			ErrLog(<<"MiddleTrscTh : add : fail : can't put MidQ : Current["<<unCurrentSize<<"] : CallId["<<_pclsSipMsg->getSimpleCallId()<<"]");
			if( _pclsSipStack->getQueueAlarmDropCallback() )
				_pclsSipStack->getQueueAlarmDropCallback()(E_STACK_QUEUE_ALARM_TYPE_middle, 0, unCurrentSize);
			return false;
		}
	}
	else
	{
		ErrLog(<<"MiddleTrscTh : add : fail : invalid MiddlePerformThread : CallId["<<_pclsSipMsg->getSimpleCallId()<<"]");
		return false;
	}
}

bool MiddleTranscFifoThread4Perform::distribute4RcvPerform(SipMessage * _pclsSipMsg)
{
	if( !_pclsSipMsg )
	{
		ErrLog(<<"MiddleTrscTh : Distribute4Perform : fail : invalid SipMsg");
		return false;
	}

	int nTrscIdx = -1;
	const Data & rclsCallId = _pclsSipMsg->header(h_CallId).value();
	
	if( m_rclsSipStack.m_pfnTranscBalanceCb1 )
		nTrscIdx = m_rclsSipStack.m_pfnTranscBalanceCb1(rclsCallId);
	else
		nTrscIdx = 0;

	if( nTrscIdx >= 0 && (unsigned int)nTrscIdx < m_rclsSipStack.m_unTuNum )
	{
		if( m_rclsSipStack.m_parrTransactionController && m_rclsSipStack.m_parrTransactionController[nTrscIdx] )
		{
			DebugLog(<<"MiddleTrscTh : Distribute4Perform : send to TrscQ["<<nTrscIdx<<"] : CallId["<<rclsCallId<<"]");
			unsigned int unCurrentSize = 0, unMaxSize = 0;
			if( m_rclsSipStack.m_parrTransactionController[nTrscIdx]->send2(_pclsSipMsg, unCurrentSize, unMaxSize) )
				return true;
			else
			{
				ErrLog(<<"MiddleTrscTh : Distribute4Perform : fail : can't put TrscQ["<<nTrscIdx<<"] : Current["<<unCurrentSize<<"] : CallId["<<rclsCallId.c_str()<<"]");
				if( m_rclsSipStack.getQueueAlarmDropCallback() )
					m_rclsSipStack.getQueueAlarmDropCallback()(E_STACK_QUEUE_ALARM_TYPE_trsc, nTrscIdx, unCurrentSize);
				return false;
			}
		}
		else
		{
			ErrLog(<<"MiddleTrscTh : Distribute4Perform : fail : invalid TransactionController["<<nTrscIdx<<"] : CallId["<<rclsCallId<<"]");
			return false;
		}
	}
	else
	{
		ErrLog(<<"MiddleTrscTh : Distribute4Perform : fail : invalid MiddleThreadIdx : CallId["<<rclsCallId<<"]");
		return false;
	}
}

void MiddleTranscFifoThread4Perform::thread()
{
	int nTid = syscall(SYS_gettid);
	
	CritLog(<<"MiddleTrscTh : Tid["<<nTid<<"] : start");

	/*==== add HangMon ====*/
	if( m_rclsSipStack.getHangMonAddCallback() )
		m_rclsSipStack.getHangMonAddCallback()(E_STACK_HANG_THREAD_middle, nTid, 0, m_clsMiddleFifo.getMaxFifoSize());

	/*==== add QueueAlarm ====*/
	if( m_rclsSipStack.getQueueAlarmAddCallback() )
		m_rclsSipStack.getQueueAlarmAddCallback()(nTid, E_STACK_QUEUE_ALARM_TYPE_middle, 0, m_clsMiddleFifo.getMaxFifoSize());

	eSipUtil::DelayTest stDelayTest;
	TransactionMessage * pclsTranscMsg = NULL;
	SipMessage * pclsSipMsg = NULL;
	TransportType eTrspType = UNKNOWN_TRANSPORT;

	while( !isShutdown() )
	{
		/*==== touch HangMon ====*/
		if( m_rclsSipStack.getHangMonTouchCallback() )
		{
			m_rclsSipStack.getHangMonTouchCallback()(E_STACK_HANG_THREAD_middle, 
																					nTid, 
																					0, 
																					m_clsMiddleFifo.getCurrentFifoNum());
		}
		
		pclsTranscMsg = NULL;
		pclsSipMsg = NULL;
		eTrspType = UNKNOWN_TRANSPORT;

		try
		{
			stDelayTest.setStartDelayTest("Middle:Get");
			pclsTranscMsg = m_clsConsumerMiddleFifo.getNext(10);
			stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC));

			if( pclsTranscMsg )
			{
				pclsSipMsg = dynamic_cast< SipMessage * >(pclsTranscMsg);
				if( pclsSipMsg )
				{
					if( m_rclsSipStack.getStackType() == E_STACK_TYPE_WITH_SLB )
					{
						if( !processWithSlb(*pclsSipMsg) )
							goto ERR_PROC;
					}
					else
					{
						if( pclsSipMsg->getReceivedTransport() )
							eTrspType = pclsSipMsg->getReceivedTransport()->getTuple().getType();

						if( eTrspType == UDP )
						{
							if( !processUdp(*pclsSipMsg) )
								goto ERR_PROC;
						}
						else if( eTrspType == TCP || eTrspType == TLS )
						{
							if( !processConnection(*pclsSipMsg) )
								goto ERR_PROC;
						}
						else
						{
							ErrLog(<<"MiddleTrscTh : Tid["<<nTid<<"] : thread : fail : invalid TransportType : CallId["<<pclsSipMsg->getSimpleCallId()<<"]");
							goto ERR_PROC;
						}
					}
					
					continue;

		ERR_PROC :
					delete pclsSipMsg;
				}
				else
				{
					ErrLog(<<"MiddleTrscTh : Tid["<<nTid<<"] : thread : fail : invalid message");
					delete pclsTranscMsg;
				}
			}
		}
		catch(resip::ParseException & e1)
        {
           ErrLog(<<"MiddleTrscTh : Tid["<<nTid<<"] : thread : fail : ParseException : "<<e1);
		   if( g_pclsEsipHandler && g_pclsEsipHandler->getFaultMsgCallback() && eTrspType == UDP && pclsSipMsg )
           {
              const char * pszSip = pclsSipMsg->getBuffer();
			  unsigned unFaultMsgLen = 0;
			  if( pszSip )
			  {
			     unFaultMsgLen += e1.getMessage().size();
			     unFaultMsgLen += 32;
			     unFaultMsgLen += strlen(pszSip);
                 char * pszFaultMsg = (char *)malloc(unFaultMsgLen);
                 if( pszFaultMsg )
                 {
                    pszFaultMsg[0] = 0x00;
                    eSipUtil::strncat(pszFaultMsg, unFaultMsgLen, "%s\n\n%s", e1.getMessage().c_str(), pszSip);
                    g_pclsEsipHandler->getFaultMsgCallback()(pszFaultMsg);
                 }
                 else
                    ErrLog(<<"MiddleTrscTh : Tid["<<nTid<<"] : thread : send faultMsg : fail : can't create FaultMsg");
		      }
		      else
			     ErrLog(<<"MiddleTrscTh : Tid["<<nTid<<"] : thread : send faultMsg : fail : invalid sip");
           }
		   delete pclsTranscMsg;
        }
		catch(...)
        {
           ErrLog(<<"MiddleTrscTh : Tid["<<nTid<<"] : thread : fail : Exception");
		   delete pclsTranscMsg;
        }
	}
	
	CritLog(<<"MiddleTrscTh : Tid["<<nTid<<"] : stop");
}

void MiddleTranscFifoThread4Perform::shutdown()
{
	ThreadIf::shutdown();
	m_clsMiddleFifo.add(NULL);
}

bool MiddleTranscFifoThread4Perform::processUdp(SipMessage & _rclsSipMsg)
{
	eSipUtil::DelayTest stDelayTest;
	int nRouteId = _rclsSipMsg.getRouteId();
	const TraceInfo_t & rstTraceInfo = _rclsSipMsg.getTraceInfo();
	const Tuple & rclsRemote = _rclsSipMsg.getSource();
	const char * pszSip = _rclsSipMsg.getBuffer();
	const char * pszCallId = _rclsSipMsg.getSimpleCallId().c_str();
	
	if( !pszSip || strlen(pszSip) == 0 )
	{
		ErrLog(<<"MiddleTrscTh : processUdp : fail : invalid SIP");
		return false;
	}
	unsigned int unSipLen = strlen(pszSip);

	/*==== execute scan of resip ====*/
	stDelayTest.setStartDelayTest("Middle:Udp:Scan");
	MsgHeaderScanner clsScanner;
	clsScanner.prepareForMessage(&_rclsSipMsg);
	char *unprocessedCharPtr;
	MsgHeaderScanner::ScanChunkResult eScanChunkResult = clsScanner.scanChunk((char *)pszSip, unSipLen, &unprocessedCharPtr);
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC));
	if( eScanChunkResult != MsgHeaderScanner::scrEnd || _rclsSipMsg.isInvalid() )
	{
		unsigned int unFaultMsgLen = 0;
		bool bHeader = false, bReason = false;
		if( eScanChunkResult != MsgHeaderScanner::scrEnd && clsScanner.getFieldName() )
		{
			ErrLog(<<"MiddleTrscTh : processUdp : scan : CallId["<<pszCallId<<"] : fail : "
						<<Data(Data::Borrow, clsScanner.getFieldName(), clsScanner.getFieldNameLen()));
			
			unFaultMsgLen += 10;
			unFaultMsgLen += clsScanner.getFieldNameLen();
			bHeader = true;
		}

		if( _rclsSipMsg.getReason() )
		{
			ErrLog(<<"MiddleTrscTh : processUdp : scan : CallId["<<pszCallId<<"] : fail : "
						<<*(_rclsSipMsg.getReason())<<" : Remote["<<rclsRemote<<"]\n"<<Data(Data::Borrow, pszSip, unSipLen));
			
			unFaultMsgLen += 10;
			unFaultMsgLen += _rclsSipMsg.getReason()->size();
			bReason = true;
		}
		else
		{
			ErrLog(<<"MiddleTrscTh : processUdp : scan : CallId["<<pszCallId<<"] : fail : Remote["<<rclsRemote<<"]\n"
						<<Data(Data::Borrow, pszSip, unSipLen));
		}

		if( g_pclsEsipHandler && g_pclsEsipHandler->getFaultMsgCallback() )
		{
			unFaultMsgLen += 50;
			unFaultMsgLen += unSipLen;
			char * pszFaultMsg = (char *)malloc(unFaultMsgLen);
			if( pszFaultMsg )
			{
				pszFaultMsg[0] = 0x00;
				eSipUtil::strncat(pszFaultMsg, unFaultMsgLen, "Syntax Scan Error : ");
				if( bHeader )
				{
					char szHdr[clsScanner.getFieldNameLen()+1];
					memcpy(szHdr, clsScanner.getFieldName(), clsScanner.getFieldNameLen());
					szHdr[clsScanner.getFieldNameLen()] = 0x00;
					eSipUtil::strncat(pszFaultMsg, unFaultMsgLen, "Hdr[%s], ", szHdr);
				}
				if( bReason )
				{
					eSipUtil::strncat(pszFaultMsg, unFaultMsgLen, "Reason[%s]", _rclsSipMsg.getReason()->c_str());
				}
				eSipUtil::strncat(pszFaultMsg, unFaultMsgLen, "\n\n%s", pszSip);
				g_pclsEsipHandler->getFaultMsgCallback()(pszFaultMsg);
			}
			else
				ErrLog(<<"MiddleTrscTh : processUdp : send faultMsg : fail : can't create FaultMsg : CallId["<<pszCallId<<"]");
		}

		return false;
	}
	if( unprocessedCharPtr > pszSip )
	{
		unsigned int used = (unsigned int)(unprocessedCharPtr - pszSip);
		if( used < unSipLen )
			_rclsSipMsg.setBody(pszSip+used, unSipLen-used);
	}

	/*==== check SIP ====*/
	stDelayTest.setStartDelayTest("Middle:Udp:CheckSip");	
	if( g_pclsEsipHandler 
		&& !g_pclsEsipHandler->checkSip((Transport *)(_rclsSipMsg.getReceivedTransport()), 
																_rclsSipMsg, pszSip, unSipLen, 
																rclsRemote, nRouteId, rstTraceInfo) )
	{
		return false;
	}
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC));

	stDelayTest.setStartDelayTest("Middle:Udp:stamp");
	Transport::stampReceived(&_rclsSipMsg);
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC));

	stDelayTest.setStartDelayTest("Middle:Udp:put2Transc");
	if( distribute4RcvPerform(&_rclsSipMsg) )
	{
		stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC));
		return true;
	}
	else
		return false;
}

bool MiddleTranscFifoThread4Perform::processConnection(SipMessage & _rclsSipMsg)
{
	int nRouteId = _rclsSipMsg.getRouteId();
	const TraceInfo_t & rstTraceInfo = _rclsSipMsg.getTraceInfo();
	const Tuple & rclsRemote = _rclsSipMsg.getSource();
	const char * pszSip = _rclsSipMsg.getRawSip4Connection();
	
	if( !pszSip || strlen(pszSip) == 0 )
	{
		ErrLog(<<"MiddleTrscTh : processConnection : fail : invalid SIP");
		return false;
	}
	unsigned int unSipLen = strlen(pszSip);

	/*==== check SIP ====*/
	if( g_pclsEsipHandler 
		&& !g_pclsEsipHandler->checkSip((Transport *)(_rclsSipMsg.getReceivedTransport()), 
																_rclsSipMsg, pszSip, unSipLen, 
																rclsRemote, nRouteId, rstTraceInfo) )
	{
		return false;
	}

	Transport::stampReceived(&_rclsSipMsg);

	if( distribute4RcvPerform(&_rclsSipMsg) )
		return true;
	else
		return false;
}

bool MiddleTranscFifoThread4Perform::processWithSlb(SipMessage & _rclsSipMsg)
{
	eSipUtil::DelayTest stDelayTest;
	int nRouteId = _rclsSipMsg.getRouteId();
	const TraceInfo_t & rstTraceInfo = _rclsSipMsg.getTraceInfo();
	const Tuple & rclsRemote = _rclsSipMsg.getSource();
	const char * pszSip = _rclsSipMsg.getBuffer();
	const char * pszCallId = _rclsSipMsg.getSimpleCallId().c_str();
	
	if( !pszSip || strlen(pszSip) == 0 )
	{
		ErrLog(<<"MiddleTrscTh : processWithSlb : fail : invalid SIP");
		return false;
	}
	unsigned int unSipLen = strlen(pszSip);

	/*==== execute scan of resip ====*/
	stDelayTest.setStartDelayTest("Middle:WithSlb:Scan");
	MsgHeaderScanner clsScanner;
	clsScanner.prepareForMessage(&_rclsSipMsg);
	char *unprocessedCharPtr;
	MsgHeaderScanner::ScanChunkResult eScanChunkResult = clsScanner.scanChunk((char *)pszSip, unSipLen, &unprocessedCharPtr);
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC));
	if( eScanChunkResult != MsgHeaderScanner::scrEnd || _rclsSipMsg.isInvalid() )
	{
		unsigned int unFaultMsgLen = 0;
		bool bHeader = false, bReason = false;
		if( eScanChunkResult != MsgHeaderScanner::scrEnd && clsScanner.getFieldName() )
		{
			ErrLog(<<"MiddleTrscTh : processWithSlb : scan : CallId["<<pszCallId<<"] : fail : "
						<<Data(Data::Borrow, clsScanner.getFieldName(), clsScanner.getFieldNameLen()));
			
			unFaultMsgLen += 10;
			unFaultMsgLen += clsScanner.getFieldNameLen();
			bHeader = true;
		}

		if( _rclsSipMsg.getReason() )
		{
			ErrLog(<<"MiddleTrscTh : processWithSlb : scan : CallId["<<pszCallId<<"] : fail : "
						<<*(_rclsSipMsg.getReason())<<" : Remote["<<rclsRemote<<"]\n"<<Data(Data::Borrow, pszSip, unSipLen));
			
			unFaultMsgLen += 10;
			unFaultMsgLen += _rclsSipMsg.getReason()->size();
			bReason = true;
		}
		else
		{
			ErrLog(<<"MiddleTrscTh : processWithSlb : scan : CallId["<<pszCallId<<"] : fail : Remote["<<rclsRemote<<"]\n"
						<<Data(Data::Borrow, pszSip, unSipLen));
		}

		if( g_pclsEsipHandler && g_pclsEsipHandler->getFaultMsgCallback() )
		{
			unFaultMsgLen += 50;
			unFaultMsgLen += unSipLen;
			char * pszFaultMsg = (char *)malloc(unFaultMsgLen);
			if( pszFaultMsg )
			{
				pszFaultMsg[0] = 0x00;
				eSipUtil::strncat(pszFaultMsg, unFaultMsgLen, "Syntax Scan Error : ");
				if( bHeader )
				{
					char szHdr[clsScanner.getFieldNameLen()+1];
					memcpy(szHdr, clsScanner.getFieldName(), clsScanner.getFieldNameLen());
					szHdr[clsScanner.getFieldNameLen()] = 0x00;
					eSipUtil::strncat(pszFaultMsg, unFaultMsgLen, "Hdr[%s], ", szHdr);
				}
				if( bReason )
				{
					eSipUtil::strncat(pszFaultMsg, unFaultMsgLen, "Reason[%s]", _rclsSipMsg.getReason()->c_str());
				}
				eSipUtil::strncat(pszFaultMsg, unFaultMsgLen, "\n\n%s", pszSip);
				g_pclsEsipHandler->getFaultMsgCallback()(pszFaultMsg);
			}
			else
				ErrLog(<<"MiddleTrscTh : processWithSlb : send faultMsg : fail : can't create FaultMsg : CallId["<<pszCallId<<"]");
		}

		return false;
	}
	if( unprocessedCharPtr > pszSip )
	{
		unsigned int used = (unsigned int)(unprocessedCharPtr - pszSip);
		if( used < unSipLen )
			_rclsSipMsg.setBody(pszSip+used, unSipLen-used);
	}

	/*==== check SIP ====*/
	stDelayTest.setStartDelayTest("Middle:WithSlb:CheckSip");	
	if( g_pclsEsipHandler 
		&& !g_pclsEsipHandler->checkSip4Slb(*(m_rclsSipStack.getTransportMgr4Slb()), 
																		_rclsSipMsg, pszSip, unSipLen, 
																		rclsRemote, nRouteId, rstTraceInfo) )
	{
		return false;
	}
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC));

	stDelayTest.setStartDelayTest("Middle:WithSlb:stamp");
	Transport::stampReceived(&_rclsSipMsg);
	stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC));

	stDelayTest.setStartDelayTest("Middle:WithSlb:put2Transc");
	if( distribute4RcvPerform(&_rclsSipMsg) )
	{
		stDelayTest.setEndDelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC, getResetCnt4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC), getLimitMs4DelayTest(E_DELAY_TEST_TYPE_MIDDLE_TRSC));
		return true;
	}
	else
		return false;
}

bool MiddleTranscFifoThread4Perform::checkCongestion(unsigned int & _runCurrentSize, unsigned int & _runMaxSize) const
{
	return m_clsMiddleFifo.checkCongestion(&_runCurrentSize, &_runMaxSize);
}

unsigned int MiddleTranscFifoThread4Perform::getFifoSize() const
{
	return m_clsMiddleFifo.getCurrentFifoNum();
}

void MiddleTranscFifoThread4Perform::setFifoMaxSize(unsigned int _unMax)
{
	m_clsMiddleFifo.setMaxFifoSize(_unMax);
}

const Data & MiddleTranscFifoThread4Perform::getFifoDescription() const
{
	return m_clsMiddleFifo.getDescription();
}

SipStack::SipStack(const SipStackOptions& options) :
        mTUFifo(TransactionController::MaxTUFifoTimeDepthSecs, TransactionController::MaxTUFifoSize),
        mTuSelector(mTUFifo),
        mAppTimers(mTuSelector),
        mStatsManager(*this, options.mstCustomOptions.m_ulStatInterval, options.mstCustomOptions.m_nStatShmKey, options.mstCustomOptions.m_clsStatShmPath),
        m_pfnTranscBalanceCb1(NULL), 
        m_pfnTranscBalanceCb2(NULL),
        m_pfnHangMonAddCb(NULL),
        m_pfnHangMonTouchCb(NULL),
        m_pfnQueueAlarmAddCb(NULL),
        m_pfnQueueAlarmDropCb(NULL),
        m_pfnStatMonCb(NULL),
        m_pclsMiddleTranscPerformThread(NULL)        
{
   // WARNING - don't forget to add new member initialization to the init() method
   init(options);
   mTUFifo.setDescription("SipStack::mTUFifo");
   m_clsHandler4eSip.setStatMgr(&mStatsManager);
   m_clsHandler4eSip.setSipStack(this);
   g_pclsEsipHandler = &m_clsHandler4eSip;
}

#else

SipStack::SipStack(const SipStackOptions& options) :
        mTUFifo(TransactionController::MaxTUFifoTimeDepthSecs, TransactionController::MaxTUFifoSize),
        mTuSelector(mTUFifo),
        mAppTimers(mTuSelector),
        mStatsManager(*this)
{
   // WARNING - don't forget to add new member initialization to the init() method
   init(options);
   mTUFifo.setDescription("SipStack::mTUFifo");
}

SipStack::SipStack(Security* pSecurity,
                   const DnsStub::NameserverList& additional,
                   AsyncProcessHandler* handler,
                   bool stateless,
                   AfterSocketCreationFuncPtr socketFunc,
                   Compression *compression,
                   FdPollGrp *pollGrp) :
   mPollGrp(pollGrp?pollGrp:FdPollGrp::create()),
   mPollGrpIsMine(!pollGrp),
#ifdef USE_SSL
   mSecurity( pSecurity ? pSecurity : new Security()),
#else
   mSecurity(0),
#endif
   mDnsStub(new DnsStub(additional, socketFunc, handler, mPollGrp)),
   mDnsThread(0),
   mCompression(compression ? compression : new Compression(Compression::NONE)),
   mAsyncProcessHandler(handler ? handler : new SelectInterruptor),
   mInterruptorIsMine(!handler),
   mTUFifo(TransactionController::MaxTUFifoTimeDepthSecs,
           TransactionController::MaxTUFifoSize),
   mCongestionManager(0),
   mTuSelector(mTUFifo),
   mAppTimers(mTuSelector),
   mStatsManager(*this),
   mTransactionController(new TransactionController(*this, mAsyncProcessHandler)),
   mTransactionControllerThread(0),
   mTransportSelectorThread(0),
   mRunning(false),
   mShuttingDown(false),
   mStatisticsManagerEnabled(true),
   mSocketFunc(socketFunc)
{
   Timer::getTimeMs(); // initalize time offsets
   Random::initialize();
   initNetwork();
   if (pSecurity)
   {
#ifdef USE_SSL
      pSecurity->preload();
#else
      assert(0);
#endif
   }

   mTransactionController->transportSelector().setPollGrp(mPollGrp);
   mTUFifo.setDescription("SipStack::mTUFifo");

#if 0
  // .kw. originally tried to share common init() for the two
  // constructors, but too much risk for changing sequencing,
  // first prove out new constructor before merging (or obsoleting)
        mTUFifo(TransactionController::MaxTUFifoTimeDepthSecs,
           TransactionController::MaxTUFifoSize),
        mTuSelector(mTUFifo),
        mAppTimers(mTuSelector),
        mStatsManager(*this)
{
   SipStackOptions options;
   options.mSecurity = pSecurity;
   options.mExtraNameserverList = &additional;
   options.mStateless = stateless;
   options.mSocketFunc = socketFunc;
   options.mCompression = compression;
   options.mPollGrp = pollGrp;
   init(options);
#endif
}

#endif

void
SipStack::init(const SipStackOptions& options)
{
   mPollGrpIsMine=false;
   if ( options.mPollGrp )
   {
      mPollGrp = options.mPollGrp;
   }
   else
   {
      mPollGrp = FdPollGrp::create();
      mPollGrpIsMine=true;
   }

#ifdef USE_SSL
   mSecurity = options.mSecurity ? options.mSecurity : new Security();
   mSecurity->preload();
#else
   mSecurity = 0;
   assert(options.mSecurity==0);
#endif

   if(options.mAsyncProcessHandler)
   {
      mAsyncProcessHandler = options.mAsyncProcessHandler;
      mInterruptorIsMine = false;
   }
   else
   {
      mInterruptorIsMine = true;
      mAsyncProcessHandler = new SelectInterruptor;
   }

   mDnsStub = new DnsStub(
         options.mExtraNameserverList
                ? *options.mExtraNameserverList : DnsStub::EmptyNameserverList,
         options.mSocketFunc,
         mAsyncProcessHandler,
         mPollGrp);
   mDnsThread = 0;

   mCompression = options.mCompression
         ? options.mCompression : new Compression(Compression::NONE);

   mCongestionManager = 0;

#ifdef ESIP_IN_RESIP
   m_eStackType = options.mstCustomOptions.m_eStackType;
   m_unMaxServerTransactionSize = options.mstCustomOptions.m_unMaxServerTransactionSize;
   m_unMaxTransportTxFifoSize = options.mstCustomOptions.m_unMaxTransportTxFifoSize;
   UdpTransport::setMaxBufferSize(options.mstCustomOptions.m_unMaxSipLen4Udp);
   m_unTuNum = options.mstCustomOptions.m_unTuNum;

   m_pclsMiddleTranscPerformThread = new MiddleTranscFifoThread4Perform(*this);
   m_pclsMiddleTranscPerformThread->setFifoMaxSize(options.mstCustomOptions.m_unMaxMiddleFifoSize);

   if( m_eStackType == E_STACK_TYPE_WITH_SLB )
   {
      m_pTrspMgrWithSlb = new TransportMgrWithSlb(*this, m_clsHandler4eSip, options.mstCustomOptions.m_pfnQueueFullCb, options.mstCustomOptions.m_unMaxSipLen4Udp);
      m_pclsTransportSelector = new TransportSelector(getSecurity(), getDnsStub(), getCompression(), m_pTrspMgrWithSlb);
   }
   else
   {
      m_pTrspMgrWithSlb = NULL;
      m_pclsTransportSelector = new TransportSelector(getSecurity(), getDnsStub(), getCompression());
      m_pclsTransportSelector->setPollGrp(mPollGrp);
   }
   
   m_parrTransactionController = new TransactionController*[m_unTuNum];
   m_parrTransactionControllerThread = new TransactionControllerThread*[m_unTuNum];	 
   for( unsigned int i = 0; i < m_unTuNum; ++i )
   {
      m_parrTransactionController[i] = new TransactionController(*this, mAsyncProcessHandler, i, *m_pclsTransportSelector, options.mstCustomOptions.m_unMaxTransactionFifoSize);
      m_parrTransactionControllerThread[i] = NULL;
   }
   m_pclsStackMonitorThread = new StackMonitorThread(*this);
#else
   // WATCHOUT: the transaction controller constructor will
   // grab the security, DnsStub, compression and statsManager
   mTransactionController = new TransactionController(*this, mAsyncProcessHandler);
   mTransactionController->transportSelector().setPollGrp(mPollGrp);
   mTransactionControllerThread = 0;
#endif	 
   mTransportSelectorThread = 0;

   mRunning = false;
   mShuttingDown = false;
   mStatisticsManagerEnabled = true;
   mSocketFunc = options.mSocketFunc;

   // .kw. note that stats manager has already called getTimeMs()
   Timer::getTimeMs(); // initalize time offsets
   Random::initialize();
   initNetwork();
}

SipStack::~SipStack()
{
   DebugLog (<< "SipStack::~SipStack()");

   delete mDnsThread;
   mDnsThread=0;
	 
#ifdef ESIP_IN_RESIP
   g_pclsEsipHandler = NULL;
   m_clsHandler4eSip.setStatMgr(NULL);
   m_clsHandler4eSip.setSipStack(NULL);
   delete m_pclsStackMonitorThread;
   delete m_pTrspMgrWithSlb;
   delete m_pclsTransportSelector;
	 
   for( unsigned int i = 0; i < m_unTuNum; ++i )
   {
      delete m_parrTransactionControllerThread[i];
      delete m_parrTransactionController[i];
   }
   delete m_pclsMiddleTranscPerformThread;
   delete[] m_parrTransactionControllerThread;
   delete[] m_parrTransactionController;
#else
   delete mTransactionControllerThread;
   mTransactionControllerThread=0;
   delete mTransactionController;
#endif

   delete mTransportSelectorThread;
   mTransportSelectorThread=0;

#ifdef USE_SSL
   delete mSecurity;
#endif
   delete mCompression;

   delete mDnsStub;
   if (mPollGrpIsMine)
   {
      // delete pollGrp after deleting DNS
      delete mPollGrp;
      mPollGrp=0;
   }

   if(mInterruptorIsMine)
   {
      delete mAsyncProcessHandler;
      mAsyncProcessHandler=0;
   }
}

void 
SipStack::run()
{
   if(mRunning)
   {
      return;
   }

   mRunning=true;

#ifdef ESIP_IN_RESIP
   m_pclsMiddleTranscPerformThread->run();
   for( unsigned int i = 0; i < m_unTuNum; ++i )
   {
      delete m_parrTransactionControllerThread[i];
      m_parrTransactionControllerThread[i] = new TransactionControllerThread(*(m_parrTransactionController[i]));
      m_parrTransactionControllerThread[i]->run();
   }
#else
   delete mDnsThread;
   mDnsThread=new DnsThread(*mDnsStub);
   mDnsThread->run();

   delete mTransactionControllerThread;
   mTransactionControllerThread=new TransactionControllerThread(*mTransactionController);
   mTransactionControllerThread->run();

   delete mTransportSelectorThread;
   mTransportSelectorThread=new TransportSelectorThread(mTransactionController->transportSelector());
   mTransportSelectorThread->run();
#endif

  
}

void
SipStack::shutdown()
{
   InfoLog (<< "Shutting down for SipStack " << this);

   {
      Lock lock(mShutdownMutex);
      assert(!mShuttingDown);
      mShuttingDown = true;
   }

#ifdef ESIP_IN_RESIP
   for( unsigned int i = 0; i < m_unTuNum; ++i )
   {
      if( m_parrTransactionController[i] )
         m_parrTransactionController[i]->shutdown();
   }
   if( m_pclsTransportSelector )
      m_pclsTransportSelector->shutdown();
#else
   mTransactionController->shutdown();
#endif
}

void 
SipStack::shutdownAndJoinThreads()
{
   InfoLog (<< "Shutting down & join thread for SipStack " << this);

   if(mDnsThread)
   {
      mDnsThread->shutdown();
      mDnsThread->join();
   }

#ifdef ESIP_IN_RESIP
   for( unsigned int i = 0; i < m_unTuNum; ++i )
   {
      if( m_parrTransactionControllerThread[i] )
      {
         m_parrTransactionControllerThread[i]->shutdown();
         m_parrTransactionControllerThread[i]->join();
      }
   }
   if( m_pclsMiddleTranscPerformThread )
   {
      m_pclsMiddleTranscPerformThread->shutdown();
      m_pclsMiddleTranscPerformThread->join();
   }	 

   if( m_pclsStackMonitorThread )
   {
      m_pclsStackMonitorThread->stop();
      m_pclsStackMonitorThread->join();
   }

   mStatsManager.stop();
   mStatsManager.join();	 
#else
   if(mTransactionControllerThread)
   {
      mTransactionControllerThread->shutdown();
      mTransactionControllerThread->join();
   }
#endif

   if(mTransportSelectorThread)
   {
      mTransportSelectorThread->shutdown();
      mTransportSelectorThread->join();
   }
   mRunning=false;
}

#ifdef ESIP_IN_RESIP
bool SipStack::addTransport4Slb(const eSipUtil::Net5Tuple_t & _rstNet5Tuple, unsigned int _unSocketRcvBufferSize, unsigned int _unSocketSndBufferSize)
{
	if( m_pTrspMgrWithSlb )
		return m_pTrspMgrWithSlb->m_fnAddTransport(_rstNet5Tuple, _unSocketRcvBufferSize, _unSocketSndBufferSize);
	else
		return false;
}

void SipStack::delTransport4Slb(const eSipUtil::Net5Tuple_t & _rstNet5Tuple)
{
	if( m_pTrspMgrWithSlb )
		m_pTrspMgrWithSlb->m_fnDelTransport(_rstNet5Tuple);
}

void SipStack::setDelTransport4SlbCb(PfuncDelTransport4Slb _pfnCb)
{
	if( m_pTrspMgrWithSlb )
		m_pTrspMgrWithSlb->m_fnSetDelTransportCb(_pfnCb);
}

void SipStack::setConnected4NonBlock4SlbCb(PfuncConnected4NonBlock4Slb _pfnCb)
{
	if( m_pTrspMgrWithSlb )
		m_pTrspMgrWithSlb->m_fnSetConnected4NonBlockCb(_pfnCb);
}

void SipStack::setRcvManageMsg4SlbCb(PfuncRcvLbManageMsg4Slb _pfnCb)
{
	if( m_pTrspMgrWithSlb )
		m_pTrspMgrWithSlb->m_fnSetRcvLbManageMsgCb(_pfnCb);
}
#endif

Transport*
SipStack::addTransport( TransportType protocol,
                        int port,
                        IpVersion version,
                        StunSetting stun,
                        const Data& ipInterface,
                        const Data& sipDomainname,
                        const Data& privateKeyPassPhrase,
                        SecurityTypes::SSLType sslType,
                        unsigned transportFlags,
                        SecurityTypes::TlsClientVerificationMode cvm,
                        bool useEmailAsSIP)
{
   assert(!mShuttingDown);

   // If address is specified, ensure it is valid
   if(!ipInterface.empty())
   {
      if(version == V6)
      {
         if(!DnsUtil::isIpV6Address(ipInterface))
         {
            ErrLog(<< "Failed to create transport, invalid ipInterface specified (IP address required): V6 "
                   << Tuple::toData(protocol) << " " << port << " on "
                   << ipInterface.c_str());
            throw Transport::Exception("Invalid ipInterface specified (IP address required)", __FILE__,__LINE__);
         }
      }
      else // V4
      {
         if(!DnsUtil::isIpV4Address(ipInterface))
         {
            ErrLog(<< "Failed to create transport, invalid ipInterface specified (IP address required): V4 "
                   << Tuple::toData(protocol) << " " << port << " on "
                   << ipInterface.c_str());
            throw Transport::Exception("Invalid ipInterface specified (IP address required)", __FILE__,__LINE__);
         }
      }
   }

   InternalTransport* transport=0;
#ifdef ESIP_IN_RESIP
   try
   {
      switch (protocol)
      {
         case UDP:
            transport = new UdpTransport(port, version, stun, ipInterface, mSocketFunc, *mCompression, transportFlags);
            break;
         case TCP:
            transport = new TcpTransport(port, version, ipInterface, mSocketFunc, *mCompression, transportFlags);
            break;
         case TLS:
#if defined( USE_SSL )
            transport = new TlsTransport(port, version, ipInterface, *mSecurity, sipDomainname, sslType, mSocketFunc,
                                                           *mCompression, transportFlags, cvm, useEmailAsSIP);
#else
            CritLog (<< "addTransport : fail : TLS not supported in this stack. You don't have openssl");
#endif
            break;
         default:
            CritLog (<< "addTransport : fail : unknown Transport Protocol");
            break;
      }
   }
#else
   Fifo<TransactionMessage>& stateMacFifo = mTransactionController->transportSelector().stateMacFifo();
   try
   {
      switch (protocol)
      {
         case UDP:
            transport = new UdpTransport(stateMacFifo, port, version, stun, ipInterface, mSocketFunc, *mCompression, transportFlags);
            break;
         case TCP:
            transport = new TcpTransport(stateMacFifo, port, version, ipInterface, mSocketFunc, *mCompression, transportFlags);
            break;
         case TLS:
#if defined( USE_SSL )
            transport = new TlsTransport(stateMacFifo,
                                         port,
                                         version,
                                         ipInterface,
                                         *mSecurity,
                                         sipDomainname,
                                         sslType,
                                         mSocketFunc,
                                         *mCompression,
                                         transportFlags,
                                         cvm,
                                         useEmailAsSIP);
#else
            CritLog (<< "TLS not supported in this stack. You don't have openssl");
            assert(0);
#endif
            break;
         case DTLS:
#if defined( USE_DTLS )
            transport = new DtlsTransport(stateMacFifo,
                                          port,
                                          version, // !jf! stun
                                          ipInterface,
                                          *mSecurity,
                                          
                                          sipDomainname,
                                          mSocketFunc,
                                          *mCompression);
#else
            CritLog (<< "DTLS not supported in this stack.");
            assert(0);
#endif
            break;
         default:
            assert(0);
            break;
      }
   }
#endif	 
   catch (BaseException& e)
   {
      ErrLog(<< "Failed to create transport: "
             << (version == V4 ? "V4" : "V6") << " "
             << Tuple::toData(protocol) << " " << port << " on "
             << (ipInterface.empty() ? "ANY" : ipInterface.c_str()) 
             << ": " << e);
      throw;
   }
	 
#ifdef ESIP_IN_RESIP
   if( transport )
   {
      Data clsTransportStr;
      clsTransportStr += ipInterface;
      clsTransportStr += ":";
      clsTransportStr += Data(port);
      clsTransportStr += ":";
      clsTransportStr += toData(protocol);
      clsTransportStr += ":Tx";			
      transport->setFifoDescription(clsTransportStr);
      transport->setMaxFifoSize(m_unMaxTransportTxFifoSize);
      transport->setSipStack(this);
   }
#endif

   addTransport(std::auto_ptr<Transport>(transport));
   return transport;
}

void
SipStack::addTransport(std::auto_ptr<Transport> transport)
{
   //.dcm. once addTransport starts throwing, need to back out alias
   if (!transport->interfaceName().empty())
   {
      addAlias(transport->interfaceName(), transport->port());
   }
   else
   {
      // Using INADDR_ANY, get all IP interfaces
      std::list<std::pair<Data, Data> > ipIfs(DnsUtil::getInterfaces());
      if(transport->ipVersion()==V4)
      {
         ipIfs.push_back(std::make_pair<Data,Data>("lo0","127.0.0.1"));
      }
      while(!ipIfs.empty())
      {
         if(DnsUtil::isIpV4Address(ipIfs.back().second)==
                                             (transport->ipVersion()==V4))
         {
            addAlias(ipIfs.back().second, transport->port());
         }
         ipIfs.pop_back();
      }
   }
   mPorts.insert(transport->port());
#ifdef ESIP_IN_RESIP
   m_pclsTransportSelector->addTransport(transport,true);
#else
   mTransactionController->transportSelector().addTransport(transport,true);
#endif
}

#ifndef ESIP_IN_RESIP
Fifo<TransactionMessage>&
SipStack::stateMacFifo()
{
   return mTransactionController->transportSelector().stateMacFifo();
}
#endif

void
SipStack::addAlias(const Data& domain, int port)
{
   int portToUse = (port == 0) ? Symbols::DefaultSipPort : port;

   DebugLog (<< "Adding domain alias: " << domain << ":" << portToUse);
   assert(!mShuttingDown);
   mDomains.insert(domain + ":" + Data(portToUse));


   if(mUri.host().empty())
   {
      mUri.host()=*mDomains.begin();
   }

}

Data
SipStack::getHostname()
{
   // if you change this, please #def old version for windows
   char hostName[1024];
   int err =  gethostname( hostName, sizeof(hostName) );
   if(err != 0)
   {
      ErrLog(<< "gethostname failed with return " << err << " Returning "
            "\"localhost\"");
      assert(0);
      return "localhost";
   }
   
   struct hostent* hostEnt = gethostbyname( hostName );
   if ( !hostEnt )
   {
      // this can fail when there is no name server
      // !cj! - need to decided what error to return
      ErrLog( << "gethostbyname failed - name server is probably down" );
      return "localhost";
   }
   assert( hostEnt );

   struct in_addr* addr = (struct in_addr*) hostEnt->h_addr_list[0];
   assert( addr );

   // if you change this, please #def old version for windows
   char* addrA = inet_ntoa( *addr );
   Data ret(addrA);

   Data retHost( hostEnt->h_name );

   return retHost;
}


Data
SipStack::getHostAddress()
{
   // if you change this, please #def old version for windows
   char hostName[1024];
   int err =  gethostname( hostName, sizeof(hostName) );
   if(err != 0)
   {
      ErrLog(<< "gethostname failed with return " << err << " Returning "
            "\"127.0.0.1\"");
      assert(0);
      return "127.0.0.1";
   }
   
   struct hostent* hostEnt = gethostbyname( hostName );
   if(!hostEnt)
   {
      ErrLog(<< "gethostbyname failed, returning \"127.0.0.1\"");
      assert(0);
      return "127.0.0.1";
   }
   
   struct in_addr* addr = (struct in_addr*) hostEnt->h_addr_list[0];
   if( !addr )
   {
      ErrLog(<< "gethostbyname returned a hostent* with an empty h_addr_list,"
               " returning \"127.0.0.1\"");
      assert(0);
      return "127.0.0.1";
   }
   
   // if you change this, please #def old version for windows 
   char* addrA = inet_ntoa( *addr );
   Data ret(addrA);

   //Data retHost( hostEnt->h_name );

   return ret;
}

bool
SipStack::isMyDomain(const Data& domain, int port) const
{
   return (mDomains.count(domain + ":" +
                          Data(port == 0 ? Symbols::DefaultSipPort : port)) != 0);
}

bool
SipStack::isMyPort(int port) const
{
   return mPorts.count(port) != 0;
}

const Uri&
SipStack::getUri() const
{
   if(mUri.host().empty())
   {
      CritLog(<< "There are no associated transports");
      throw Exception("No associated transports", __FILE__, __LINE__);
   }

   return mUri;
}

#ifdef ESIP_IN_RESIP
bool SipStack::checkCongestion(const Data & _rclsCallId, Data & _rclsDesc) const
{
	unsigned int unCurrentSize = 0;
	unsigned int unMaxSize = 0;
	bool bCongest = false;
	//char szQueueName[128] = {0,};
	EStackQueueAlarmType eStackQueueAlarmType = E_STACK_QUEUE_ALARM_TYPE_NUM;
	unsigned int unMultiIdx = 0;
	int nTrscIdx;

	if( m_pfnTranscBalanceCb1 )
		nTrscIdx = m_pfnTranscBalanceCb1(_rclsCallId);
	else
		nTrscIdx = 0;
	
#if 0
	/*==== check TransportFifo(Tx) ====*/
	Transport * pTransport = NULL;
	InternalTransport * pInternalTransport = NULL;
	if( m_pclsTransportSelector )
	{
		int nTransportNum = m_pclsTransportSelector->mTransports.size();
		for( int i = 0; i < nTransportNum; ++i )
		{
			if( (pTransport = m_pclsTransportSelector->mTransports[i]) 
				&& (pInternalTransport = dynamic_cast< InternalTransport * >(pTransport)) )
			{
				if( pInternalTransport->checkCongestion(unCurrentSize, unMaxSize) )
				{
					unGroupId = 0;
					bCongest = true;
					_rclsDesc = "TxFifo[";
					_rclsDesc += pTransport->getTuple().presentationFormat();
					_rclsDesc += "]";
					//strncpy(szQueueName, _rclsDesc.c_str(), sizeof(szQueueName)-1);
					//szQueueName[sizeof(szQueueName)-1] = 0x00;
					_rclsDesc += " is over. Current[";
					_rclsDesc += Data(unCurrentSize);
					_rclsDesc += "], Max[";
					_rclsDesc += Data(unMaxSize);
					_rclsDesc += "]";
					return bCongest;
				}
			}
		}
	}
#endif

	/*==== check middleTranscFifo ====*/
	if( m_pclsMiddleTranscPerformThread 
		&& m_pclsMiddleTranscPerformThread->checkCongestion(unCurrentSize, unMaxSize) )
	{
		eStackQueueAlarmType = E_STACK_QUEUE_ALARM_TYPE_middle;
		unMultiIdx = 0;
		bCongest = true;
		_rclsDesc = "MiddleTranscFifo";
		//strncpy(szQueueName, _rclsDesc.c_str(), sizeof(szQueueName)-1);
		//szQueueName[sizeof(szQueueName)-1] = 0x00;
		_rclsDesc += " is over. Current[";
		_rclsDesc += Data(unCurrentSize);
		_rclsDesc += "], Max[";
		_rclsDesc += Data(unMaxSize);
		_rclsDesc += "]";
		goto END_CONGESTION;
	}

	/*==== check TranscFifo ====*/
	if( nTrscIdx >= 0 && (unsigned int)nTrscIdx < m_unTuNum
		&& m_parrTransactionController
		&& m_parrTransactionController[nTrscIdx]
		&& m_parrTransactionController[nTrscIdx]->checkCongestion(unCurrentSize, unMaxSize) )
	{
		eStackQueueAlarmType = E_STACK_QUEUE_ALARM_TYPE_trsc;
		unMultiIdx= m_parrTransactionController[nTrscIdx]->getTcId();
		bCongest = true;
		_rclsDesc = "TranscFifo[";
		_rclsDesc += Data(m_parrTransactionController[nTrscIdx]->getTcId());
		_rclsDesc += "]";
		//strncpy(szQueueName, _rclsDesc.c_str(), sizeof(szQueueName)-1);
		//szQueueName[sizeof(szQueueName)-1] = 0x00;
		_rclsDesc += " is over. Current[";
		_rclsDesc += Data(unCurrentSize);
		_rclsDesc += "], Max[";
		_rclsDesc += Data(unMaxSize);
		_rclsDesc += "]";
		goto END_CONGESTION;
	}

	/*==== check TuFifo ====*/
	for(TuSelector::TuList::const_iterator itr = mTuSelector.mTuList.begin(); itr != mTuSelector.mTuList.end(); ++itr)
	{
		if( itr->tu && itr->tu->getTuId() == nTrscIdx )
		{
			if( itr->tu->checkCongestion(unCurrentSize, unMaxSize) )
			{
				eStackQueueAlarmType = E_STACK_QUEUE_ALARM_TYPE_tu;
				unMultiIdx= itr->tu->getTuId();
				bCongest = true;
				_rclsDesc = "Tu[";
				_rclsDesc += Data(itr->tu->getTuId());
				_rclsDesc += "]";
				//strncpy(szQueueName, _rclsDesc.c_str(), sizeof(szQueueName)-1);
				//szQueueName[sizeof(szQueueName)-1] = 0x00;
				_rclsDesc += " is over. Current[";
				_rclsDesc += Data(unCurrentSize);
				_rclsDesc += "], Max[";
				_rclsDesc += Data(unMaxSize);
				_rclsDesc += "]";
				goto END_CONGESTION;
			}
			break;
		}
	}

END_CONGESTION :

	if( bCongest )
	{
		if( m_pfnQueueAlarmDropCb )
		{
			unsigned int unQueueFullCurrentSize = 0;
			if( unCurrentSize <= unMaxSize )
				unQueueFullCurrentSize = unMaxSize;
			else
				unQueueFullCurrentSize = unCurrentSize;
			DebugLog(<<"SipStack : execute QueueAlarmDrop callback : QueueAlarmId["<<g_fnGetStackQueueAlarmType(eStackQueueAlarmType)<<"], MultiIdx["<<unMultiIdx<<"], CurrentQ["<<unQueueFullCurrentSize<<"]");
			m_pfnQueueAlarmDropCb(eStackQueueAlarmType, unMultiIdx, unQueueFullCurrentSize);
		}
	}

	return bCongest;
}

bool SipStack::send(const SipMessage& msg, TransactionUser* tu, unsigned int * _punCurrentSize, unsigned int * _punMaxSize)
{
	DebugLog(<<"SipStack : Send 1 : "<< msg.brief());

	SipMessage * toSend = static_cast<SipMessage*>(msg.clone());

	if( mShuttingDown )
		goto ERR;

	if( toSend )
	{
		int nTuId = 0;
		if( tu )
		{
			toSend->setTransactionUser(tu);
			nTuId = tu->getTuId();
		}
		toSend->setFromTU();
		if( nTuId >= 0 && nTuId < (int)(m_unTuNum) )
		{
			unsigned int unCurrentSize = 0, unMaxSize = 0;
			bool bResult = m_parrTransactionController[nTuId]->send2(toSend, unCurrentSize, unMaxSize);
			if( _punCurrentSize )
				*_punCurrentSize = unCurrentSize;
			if( _punMaxSize )
				*_punMaxSize = unMaxSize;
			if( bResult )
				return true;
			else
			{
				delete toSend;
				return false;
			}
		}
		else
			ErrLog(<<"SipStack : Send 1 : invalid TcId["<<nTuId<<"] : "<< msg.brief());
	}

ERR :
	if( _punCurrentSize )
		*_punCurrentSize = 0;
	if( _punMaxSize )
		*_punMaxSize = 0;
	delete toSend;
	return false;
}

bool SipStack::send(std::auto_ptr<SipMessage> msg, TransactionUser* tu, unsigned int * _punCurrentSize, unsigned int * _punMaxSize)
{
	DebugLog(<<"SipStack : Send 2 : "<< msg->brief());

	if( mShuttingDown )
		goto ERR;

	if( msg.get() )
	{
		int nTuId = 0;
		if( tu )
		{
			msg->setTransactionUser(tu);
			nTuId = tu->getTuId();
		}
		msg->setFromTU();
		if( nTuId >= 0 && nTuId < (int)(m_unTuNum) )
		{
			SipMessage * pclsSip = msg.release();
			unsigned int unCurrentSize = 0, unMaxSize = 0;
			bool bResult = m_parrTransactionController[nTuId]->send2(pclsSip, unCurrentSize, unMaxSize);
			if( _punCurrentSize )
				*_punCurrentSize = unCurrentSize;
			if( _punMaxSize )
				*_punMaxSize = unMaxSize;
			if( bResult )
				return true;
			else
			{
				delete pclsSip;
				return false;
			}
		}
		else
			ErrLog(<<"SipStack : Send 2 : invalid TcId["<<nTuId<<"] : "<< msg->brief());
	}

ERR :
	if( _punCurrentSize )
		*_punCurrentSize = 0;
	if( _punMaxSize )
		*_punMaxSize = 0;
	return false;
}

bool SipStack::sendTo(std::auto_ptr<SipMessage> msg, const Uri& uri, TransactionUser* tu, unsigned int * _punCurrentSize, unsigned int * _punMaxSize)
{
	DebugLog(<<"SipStack : SendTo 1 : "<< msg->brief());

	if( mShuttingDown )
		goto ERR;

	if( msg.get() )
	{
		int nTuId = 0;
		if( tu )
		{
			msg->setTransactionUser(tu);
			nTuId = tu->getTuId();
		}
		msg->setForceTarget(uri);
		msg->setFromTU();
		if( nTuId >= 0 && nTuId < (int)(m_unTuNum) )
		{
			SipMessage * pclsSip = msg.release();
			unsigned int unCurrentSize = 0, unMaxSize = 0;
			bool bResult = m_parrTransactionController[nTuId]->send2(pclsSip, unCurrentSize, unMaxSize);
			if( _punCurrentSize )
				*_punCurrentSize = unCurrentSize;
			if( _punMaxSize )
				*_punMaxSize = unMaxSize;
			if( bResult )
				return true;
			else
			{
				delete pclsSip;
				return false;
			}
		}
		else
			ErrLog(<<"SipStack : SendTo 1 : invalid TcId["<<nTuId<<"] : "<< msg->brief());
	}

ERR :
	if( _punCurrentSize )
		*_punCurrentSize = 0;
	if( _punMaxSize )
		*_punMaxSize = 0;
	return false;
}

bool SipStack::sendTo(std::auto_ptr<SipMessage> msg, const Tuple& destination, TransactionUser* tu, unsigned int * _punCurrentSize, unsigned int * _punMaxSize)
{
	DebugLog(<<"SipStack : SendTo 2 : "<< msg->brief());

	if( mShuttingDown )
		goto ERR;

	if( msg.get() )
	{
		int nTuId = 0;
		if( tu )
		{
			msg->setTransactionUser(tu);
			nTuId = tu->getTuId();
		}
		msg->setDestination(destination);
		msg->setFromTU();
		if( nTuId >= 0 && nTuId < (int)(m_unTuNum) )
		{
			SipMessage * pclsSip = msg.release();
			unsigned int unCurrentSize = 0, unMaxSize = 0;
			bool bResult = m_parrTransactionController[nTuId]->send2(pclsSip, unCurrentSize, unMaxSize);
			if( _punCurrentSize )
				*_punCurrentSize = unCurrentSize;
			if( _punMaxSize )
				*_punMaxSize = unMaxSize;
			if( bResult )
				return true;
			else
			{
				delete pclsSip;
				return false;
			}
		}
		else
			ErrLog(<<"SipStack : SendTo 2 : invalid TcId["<<nTuId<<"] : "<< msg->brief());
	}

ERR :
	if( _punCurrentSize )
		*_punCurrentSize = 0;
	if( _punMaxSize )
		*_punMaxSize = 0;
	return false;
}

bool SipStack::sendTo(const SipMessage& msg, const Uri& uri, TransactionUser* tu, unsigned int * _punCurrentSize, unsigned int * _punMaxSize)
{
	DebugLog(<<"SipStack : SendTo 3 : "<<msg.brief());

	SipMessage * toSend = static_cast<SipMessage*>(msg.clone());

	if( mShuttingDown )
		goto ERR;
	
	if( toSend )
	{
		int nTuId = 0;
		if( tu )
		{
			toSend->setTransactionUser(tu);
			nTuId = tu->getTuId();
		}
		toSend->setForceTarget(uri);
		toSend->setFromTU();
		if( nTuId >= 0 && nTuId < (int)(m_unTuNum) )
		{
			unsigned int unCurrentSize = 0, unMaxSize = 0;
			bool bResult = m_parrTransactionController[nTuId]->send2(toSend, unCurrentSize, unMaxSize);
			if( _punCurrentSize )
				*_punCurrentSize = unCurrentSize;
			if( _punMaxSize )
				*_punMaxSize = unMaxSize;
			if( bResult )
				return true;
			else
			{
				delete toSend;
				return false;
			}
		}
		else
			ErrLog(<<"SipStack : SendTo 3 : invalid TcId["<<nTuId<<"] : "<<msg.brief());
	}

ERR :
	if( _punCurrentSize )
		*_punCurrentSize = 0;
	if( _punMaxSize )
		*_punMaxSize = 0;
	delete toSend;
	return false;
}

bool SipStack::sendTo(const SipMessage& msg, const Tuple& destination, TransactionUser* tu, unsigned int * _punCurrentSize, unsigned int * _punMaxSize)
{
	DebugLog(<<"SipStack : SendTo 4 : "<<msg.brief());

	SipMessage* toSend = static_cast<SipMessage*>(msg.clone());

	if( mShuttingDown )
		goto ERR;

	if( toSend )
	{
		int nTuId = 0;
		if( tu )
		{
			toSend->setTransactionUser(tu);
			nTuId = tu->getTuId();
		}
		toSend->setDestination(destination);
		toSend->setFromTU();
		if( nTuId >= 0 && nTuId < (int)(m_unTuNum) )
		{
			unsigned int unCurrentSize = 0, unMaxSize = 0;
			bool bResult = m_parrTransactionController[nTuId]->send2(toSend, unCurrentSize, unMaxSize);
			if( _punCurrentSize )
				*_punCurrentSize = unCurrentSize;
			if( _punMaxSize )
				*_punMaxSize = unMaxSize;
			if( bResult )
				return true;
			else
			{
				delete toSend;
				return false;
			}
		}
		else
			ErrLog(<<"SipStack : SendTo 4 : invalid TcId["<<nTuId<<"] : "<<msg.brief());
	}

ERR :
	if( _punCurrentSize )
		*_punCurrentSize = 0;
	if( _punMaxSize )
		*_punMaxSize = 0;
	delete toSend;
	return false;
}

#else

void SipStack::send(const SipMessage& msg, TransactionUser* tu)
{
   DebugLog (<< "SEND: " << msg.brief());
   //DebugLog (<< msg);
   //assert(!mShuttingDown);

   SipMessage* toSend = static_cast<SipMessage*>(msg.clone());
   if (tu)
   {
      toSend->setTransactionUser(tu);
   }
   toSend->setFromTU();

   mTransactionController->send(toSend);
}

void SipStack::send(std::auto_ptr<SipMessage> msg, TransactionUser* tu)
{
   DebugLog (<< "SEND: " << msg->brief());

   if (tu)
   {
      msg->setTransactionUser(tu);
   }
   msg->setFromTU();

   mTransactionController->send(msg.release());
}

void SipStack::sendTo(std::auto_ptr<SipMessage> msg, const Uri& uri, TransactionUser* tu)
{
   if (tu) msg->setTransactionUser(tu);
   msg->setForceTarget(uri);
   msg->setFromTU();

   mTransactionController->send(msg.release());
}

void SipStack::sendTo(std::auto_ptr<SipMessage> msg, const Tuple& destination, TransactionUser* tu)
{
   assert(!mShuttingDown);

   if (tu) msg->setTransactionUser(tu);
   msg->setDestination(destination);
   msg->setFromTU();

   mTransactionController->send(msg.release());
}

// this is only if you want to send to a destination not in the route. You
// probably don't want to use it.
void SipStack::sendTo(const SipMessage& msg, const Uri& uri, TransactionUser* tu)
{
   //assert(!mShuttingDown);

   SipMessage* toSend = static_cast<SipMessage*>(msg.clone());
   if (tu) toSend->setTransactionUser(tu);
   toSend->setForceTarget(uri);
   toSend->setFromTU();

   mTransactionController->send(toSend);
}

// this is only if you want to send to a destination not in the route. You
// probably don't want to use it.
void SipStack::sendTo(const SipMessage& msg, const Tuple& destination, TransactionUser* tu)
{
   assert(!mShuttingDown);

   SipMessage* toSend = static_cast<SipMessage*>(msg.clone());
   if (tu) toSend->setTransactionUser(tu);
   toSend->setDestination(destination);
   toSend->setFromTU();

   mTransactionController->send(toSend);
}
#endif

void
SipStack::checkAsyncProcessHandler()
{
   if (mAsyncProcessHandler)
   {
      mAsyncProcessHandler->handleProcessNotification();
   }
}

void
SipStack::post(std::auto_ptr<ApplicationMessage> message)
{
   assert(!mShuttingDown);
   mTuSelector.add(message.release(), TimeLimitFifo<Message>::InternalElement);
}

void
SipStack::post(const ApplicationMessage& message)
{
   assert(!mShuttingDown);
   Message* toPost = message.clone();
   mTuSelector.add(toPost, TimeLimitFifo<Message>::InternalElement);
}

void
SipStack::post(const ApplicationMessage& message,  unsigned int secondsLater,
               TransactionUser* tu)
{
   assert(!mShuttingDown);
   postMS(message, secondsLater*1000, tu);
}

void
SipStack::postMS(const ApplicationMessage& message, unsigned int ms,
                 TransactionUser* tu)
{
   assert(!mShuttingDown);
   Message* toPost = message.clone();
   if (tu) toPost->setTransactionUser(tu);
   Lock lock(mAppTimerMutex);
   mAppTimers.add(ms,toPost);
   //.dcm. timer update rather than process cycle...optimize by checking if sooner
   //than current timeTillNextProcess?
   checkAsyncProcessHandler();
}

void
SipStack::post(std::auto_ptr<ApplicationMessage> message,
               unsigned int secondsLater,
               TransactionUser* tu)
{
   postMS(message, secondsLater*1000, tu);
}


void
SipStack::postMS( std::auto_ptr<ApplicationMessage> message,
                  unsigned int ms,
                  TransactionUser* tu)
{
   assert(!mShuttingDown);
   if (tu) message->setTransactionUser(tu);
   Lock lock(mAppTimerMutex);
   mAppTimers.add(ms, message.release());
   //.dcm. timer update rather than process cycle...optimize by checking if sooner
   //than current timeTillNextProcess?
   checkAsyncProcessHandler();
}

#ifdef ESIP_IN_RESIP
void SipStack::abandonServerTransaction(const Data& tid, int _nTcId)
{
	if( _nTcId >= 0 && (unsigned int)_nTcId < m_unTuNum )
	{
		if( m_parrTransactionController[_nTcId] )
	         m_parrTransactionController[_nTcId]->abandonServerTransaction(tid);
	}
}
#else
void
SipStack::abandonServerTransaction(const Data& tid)
{
   mTransactionController->abandonServerTransaction(tid);

}
#endif

#ifdef ESIP_IN_RESIP
void SipStack::cancelClientInviteTransaction(const Data& tid, int _nTcId)
{
	if( _nTcId >= 0 && (unsigned int)_nTcId < m_unTuNum )
	{
		if( m_parrTransactionController[_nTcId] )
			m_parrTransactionController[_nTcId]->cancelClientInviteTransaction(tid);
	}
}
#else
void
SipStack::cancelClientInviteTransaction(const Data& tid)
{
   mTransactionController->cancelClientInviteTransaction(tid);
}
#endif

bool
SipStack::hasMessage() const
{
   return mTUFifo.messageAvailable();
}

SipMessage*
SipStack::receive()
{
   // Check to see if a message is available and if it is return the
   // waiting message. Otherwise, return 0
   if (mTUFifo.messageAvailable())
   {
      // we should only ever have SIP messages on the TU Fifo
      // unless we've registered for termination messages.
      Message* msg = mTUFifo.getNext();
      SipMessage* sip = dynamic_cast<SipMessage*>(msg);
      if (sip)
      {
         DebugLog (<< "RECV: " << sip->brief());
         return sip;
      }
      else
      {
         // assert(0); // !CJ! removed the assert - happens 1 minute after
         // stack starts up
         delete msg;
         return 0;
      }
   }
   else
   {
      return 0;
   }
}

Message*
SipStack::receiveAny()
{
   // Check to see if a message is available and if it is return the
   // waiting message. Otherwise, return 0
   if (mTUFifo.messageAvailable())
   {
      // application messages can flow through
      Message* msg = mTUFifo.getNext();
      SipMessage* sip=dynamic_cast<SipMessage*>(msg);
      if (sip)
      {
         DebugLog (<< "RECV: " << sip->brief());
      }
      return msg;
   }
   else
   {
      return 0;
   }
}

void
SipStack::setFallbackPostNotify(AsyncProcessHandler *handler) 
{
   mTuSelector.setFallbackPostNotify(handler);
}

/* Called from external epoll (e.g., EventStackThread) */
void
SipStack::processTimers()
{
#ifdef ESIP_IN_RESIP
   if( !mTransportSelectorThread && m_pclsTransportSelector )
   {
      m_pclsTransportSelector->process();
   }
#else
   if(!mShuttingDown && mStatisticsManagerEnabled)
   {
      mStatsManager.process();
   }

   if(!mTransactionControllerThread)
   {
      mTransactionController->process();
   }

   if(!mDnsThread)
   {
      mDnsStub->processTimers();
   }

   if(!mTransportSelectorThread)
   {
      mTransactionController->transportSelector().process();
   }
#endif	 

   mTuSelector.process();
   Lock lock(mAppTimerMutex);
   mAppTimers.process();
}

/* Called for internal epoll and non-epoll (e.g., StackThread) */
void
SipStack::process(FdSet& fdset)
{
   mPollGrp->processFdSet(fdset);
   processTimers();
}

bool 
SipStack::process(unsigned int timeoutMs)
{
   // Go ahead and do this first. Should cut down on how frequently we call 
   // waitAndProcess() with a timeout of 0, which should improve efficiency 
   // somewhat.
   processTimers();
   bool result=mPollGrp->waitAndProcess(resipMin(timeoutMs, getTimeTillNextProcessMS()));
   return result;
}

/// returns time in milliseconds when process next needs to be called
unsigned int
SipStack::getTimeTillNextProcessMS()
{
   Lock lock(mAppTimerMutex);

#ifndef ESIP_IN_RESIP
   unsigned int dnsNextProcess = (mDnsThread ? 
                           INT_MAX : mDnsStub->getTimeTillNextProcessMS());
   unsigned int tcNextProcess = mTransactionControllerThread ? INT_MAX : 
                           mTransactionController->getTimeTillNextProcessMS();
   unsigned int tsNextProcess = mTransportSelectorThread ? INT_MAX : mTransactionController->transportSelector().getTimeTillNextProcessMS();
#endif

   return resipMin(Timer::getMaxSystemTimeWaitMs(),
#ifdef ESIP_IN_RESIP
            resipMin(mTuSelector.getTimeTillNextProcessMS(), mAppTimers.msTillNextTimer()));
#else
            resipMin(dnsNextProcess,
               resipMin(tcNextProcess,
                  resipMin(tsNextProcess,
                     resipMin(mTuSelector.getTimeTillNextProcessMS(), mAppTimers.msTillNextTimer())))));
#endif
}

void
SipStack::buildFdSet(FdSet& fdset)
{
   mPollGrp->buildFdSet(fdset);
}

Security*
SipStack::getSecurity() const
{
    return mSecurity;
}

#ifndef ESIP_IN_RESIP   
void
SipStack::setStatisticsInterval(unsigned long seconds)
{
   mStatsManager.setInterval(seconds);
}
#endif

void
SipStack::zeroOutStatistics()
{
#ifdef ESIP_IN_RESIP
   if(statisticsManagerEnabled())
   {
      for( unsigned int i = 0; i < m_unTuNum; ++i )
      {
         if( m_parrTransactionController[i] )
            m_parrTransactionController[i]->zeroOutStatistics();
      }
   }
#else
   if(statisticsManagerEnabled())
   {
      mTransactionController->zeroOutStatistics();
   }
#endif
}

bool
SipStack::pollStatistics()
{
#ifdef ESIP_IN_RESIP
   if(statisticsManagerEnabled())
   {
      for( unsigned int i = 0; i < m_unTuNum; ++i )
      {
         if( m_parrTransactionController[i] )
            m_parrTransactionController[i]->pollStatistics();
      }
      return true;			
   }
#else
   if(statisticsManagerEnabled())
   {
      mTransactionController->pollStatistics();
      return true;
   }
#endif
   return false;
}

void
SipStack::registerTransactionUser(TransactionUser& tu)
{
   mTuSelector.registerTransactionUser(tu);
}

void
SipStack::requestTransactionUserShutdown(TransactionUser& tu)
{
   mTuSelector.requestTransactionUserShutdown(tu);
   checkAsyncProcessHandler();
}

void
SipStack::unregisterTransactionUser(TransactionUser& tu)
{
   mTuSelector.unregisterTransactionUser(tu);
   checkAsyncProcessHandler();
}

void
SipStack::registerMarkListener(MarkListener* listener)
{
#ifdef ESIP_IN_RESIP
   for( unsigned int i = 0; i < m_unTuNum; ++i )
   {
      if( m_parrTransactionController[i] )
         m_parrTransactionController[i]->registerMarkListener(listener);
   }
#else	 
   mTransactionController->registerMarkListener(listener);
#endif
}

void
SipStack::unregisterMarkListener(MarkListener* listener)
{
#ifdef ESIP_IN_RESIP
   for( unsigned int i = 0; i < m_unTuNum; ++i )
   {
      if( m_parrTransactionController[i] )
         m_parrTransactionController[i]->unregisterMarkListener(listener);
   }
#else	 
   mTransactionController->unregisterMarkListener(listener);
#endif
}

DnsStub&
SipStack::getDnsStub() const
{
   return *mDnsStub;
}

void
SipStack::setEnumSuffixes(const std::vector<Data>& suffixes)
{
   mDnsStub->setEnumSuffixes(suffixes);
}

void
SipStack::setEnumDomains(const std::map<Data,Data>& domains)
{
   mDnsStub->setEnumDomains(domains);
}

void
SipStack::clearDnsCache()
{
   mDnsStub->clearDnsCache();
}

void
SipStack::logDnsCache()
{
   mDnsStub->logDnsCache();
}

void 
SipStack::getDnsCacheDump(std::pair<unsigned long, unsigned long> key, GetDnsCacheDumpHandler* handler)
{
   mDnsStub->getDnsCacheDump(key, handler);
}

volatile bool&
SipStack::statisticsManagerEnabled()
{
   return mStatisticsManagerEnabled;
}

const bool
SipStack::statisticsManagerEnabled() const
{
   return mStatisticsManagerEnabled;
}

#ifdef ESIP_IN_RESIP
EncodeStream&
SipStack::dump(EncodeStream& strm)  const
{
   Lock lock(mAppTimerMutex);
   strm << "SipStack: " << (this->mSecurity ? "with security " : "without security ")
        << std::endl
        << "domains: " << Inserter(this->mDomains)
        << std::endl
        << " TUFifo size=" << this->mTUFifo.size() << std::endl
        << " Timers size=" << getTransactionTimerFifoSize() << std::endl
        << " AppTimers size=" << this->mAppTimers.size() << std::endl
        << " ServerTransactionMap size=" << getCurrentServerTransactionNum() << std::endl
        << " ClientTransactionMap size=" << getCurrentClientTransactionNum() << std::endl
        << " Exact Transports=" << Inserter(m_pclsTransportSelector->mExactTransports) << std::endl
        << " Any Transports=" << Inserter(m_pclsTransportSelector->mAnyInterfaceTransports) << std::endl;
   return strm;
}
#else
EncodeStream&
SipStack::dump(EncodeStream& strm)  const
{
   Lock lock(mAppTimerMutex);
   strm << "SipStack: " << (this->mSecurity ? "with security " : "without security ")
        << std::endl
        << "domains: " << Inserter(this->mDomains)
        << std::endl
        << " TUFifo size=" << this->mTUFifo.size() << std::endl
        << " Timers size=" << this->mTransactionController->mTimers.size() << std::endl
        << " AppTimers size=" << this->mAppTimers.size() << std::endl
        << " ServerTransactionMap size=" << this->mTransactionController->mServerTransactionMap.size() << std::endl
        << " ClientTransactionMap size=" << this->mTransactionController->mClientTransactionMap.size() << std::endl
        << " Exact Transports=" << Inserter(this->mTransactionController->mTransportSelector.mExactTransports) << std::endl
        << " Any Transports=" << Inserter(this->mTransactionController->mTransportSelector.mAnyInterfaceTransports) << std::endl;
   return strm;
}
#endif

EncodeStream&
resip::operator<<(EncodeStream& strm,
const SipStack& stack)
{
   return stack.dump(strm);
}

void 
SipStack::terminateFlow(const resip::Tuple& flow)
{
#ifdef ESIP_IN_RESIP
   for( unsigned int i = 0; i < m_unTuNum; ++i )
   {
      if( m_parrTransactionController[i] )
         m_parrTransactionController[i]->terminateFlow(flow);
   }
#else	
   mTransactionController->terminateFlow(flow);
#endif
}

void 
SipStack::enableFlowTimer(const resip::Tuple& flow)
{
#ifdef ESIP_IN_RESIP
   for( unsigned int i = 0; i < m_unTuNum; ++i )
   {
      if( m_parrTransactionController[i] )
         m_parrTransactionController[i]->enableFlowTimer(flow);
   }
#else	
   mTransactionController->enableFlowTimer(flow);
#endif
}

#ifdef ESIP_IN_RESIP
std::list< std::pair<Data, unsigned int> > SipStack::getTransportTxFifoSize() const
{
	Transport * pTransport = NULL;
	std::list< std::pair<Data, unsigned int> > listTransportTxFifoSize;
	if( m_pclsTransportSelector )
	{
		int nTransportNum = m_pclsTransportSelector->mTransports.size();
		for( int i = 0; i < nTransportNum; ++i )
		{
			if( (pTransport = m_pclsTransportSelector->mTransports[i]) )
				listTransportTxFifoSize.push_back(std::make_pair(pTransport->getFifoDescription(), pTransport->getFifoSize()));
		}
	}

	return listTransportTxFifoSize;
}

unsigned int SipStack::getMiddleTransactionFifoSize() const
{
	if( m_pclsMiddleTranscPerformThread )
		return m_pclsMiddleTranscPerformThread->getFifoSize();
	else
		return 0;
}

std::list< std::pair<unsigned int, unsigned int> > SipStack::getTransactionFifoSize4Monitor() const
{
	std::list< std::pair<unsigned int, unsigned int> > listTranscFifoSize;
	if( m_parrTransactionController )
	{
		for( unsigned int i = 0; i < m_unTuNum; ++i )
		{
			if( m_parrTransactionController[i] )
				listTranscFifoSize.push_back(std::make_pair(i, m_parrTransactionController[i]->getTransactionFifoSize()));
		}
	}
	
	return listTranscFifoSize;
}

std::list< std::pair<unsigned int, unsigned int> > SipStack::getTuFifoSize() const
{
	std::list< std::pair<unsigned int, unsigned int> > listTuFifoSize;
	for(TuSelector::TuList::const_iterator it = mTuSelector.mTuList.begin(); it != mTuSelector.mTuList.end(); it++)
	{
		listTuFifoSize.push_back(std::make_pair(it->tu->getTuId(), it->tu->size()));
	}

	return listTuFifoSize;
}

void SipStack::setDiscardStrayRsp(bool _bDiscard)
{
	if( m_parrTransactionController )
	{
		for( unsigned int i = 0; i < m_unTuNum; ++i )
		{
			if( m_parrTransactionController[i] )
				m_parrTransactionController[i]->setDiscardStrayRsp(_bDiscard);
		}
	}
}
#endif

/* ====================================================================
 * The Vovida Software License, Version 1.0
 *
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * ====================================================================
 *
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 * vi: set shiftwidth=3 expandtab:
 */
