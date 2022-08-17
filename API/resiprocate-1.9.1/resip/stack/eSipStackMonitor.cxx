
#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#ifdef ESIP_IN_RESIP

#include <sys/syscall.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

#include "resip/stack/eSipStackMonitor.hxx"
#include "resip/stack/eSipHandler.hxx"
#include "resip/stack/SipStack.hxx"
#include "rutil/Logger.hxx"

namespace resip
{

StackMonitorThread::StackMonitorThread(SipStack & _rclsSipStack)
	: m_clsFifo(0,0), m_rclsSipStack(_rclsSipStack), m_unInterval(0)
{
	run();
}

StackMonitorThread::~StackMonitorThread()
{
}

void StackMonitorThread::setInterval(unsigned int _unInterval)
{
	int unPrevInterval = m_unInterval;
	m_unInterval = _unInterval;

	if( unPrevInterval == 0 )
	{
		int * pnShutdown = new int(1);
		m_clsFifo.add(pnShutdown, resip::TimeLimitFifo<int>::InternalElement);
	}
}

void StackMonitorThread::stop()
{
	shutdown();
	int * pnShutdown = new int(2);
	m_clsFifo.add(pnShutdown, resip::TimeLimitFifo<int>::InternalElement);
}

void StackMonitorThread::thread()
{
	CritLog(<<"StackMonitorThread : thread["<<syscall(SYS_gettid)<<"] : start");

	int * pnResult;
	
	while( !isShutdown() )
	{
		pnResult = m_clsFifo.getNext(m_unInterval);
		if( !pnResult )
			process();
		else
		{
			if( *pnResult == 1 )
			{
				delete pnResult;
			}
			else
			{
				delete pnResult;			
				break;
			}
		}
	}

	CritLog(<<"StackMonitorThread : thread : stop");
}

void StackMonitorThread::process()
{
	if( m_rclsSipStack.getStatMonCallback() )
	{
		StatMonData stStatMonData;
	
		stStatMonData.m_unClientTransactionNum = m_rclsSipStack.getCurrentClientTransactionNum();
		stStatMonData.m_unServerTransactionNum = m_rclsSipStack.getCurrentServerTransactionNum();
		stStatMonData.m_unTransactionTimerFifoSize = m_rclsSipStack.getTransactionTimerFifoSize();
		stStatMonData.m_listTransportTxFifoSize = m_rclsSipStack.getTransportTxFifoSize();
		stStatMonData.m_unMiddleTranscFifoSize = m_rclsSipStack.getMiddleTransactionFifoSize();
		stStatMonData.m_listTranscFifoSize = m_rclsSipStack.getTransactionFifoSize4Monitor();	
		stStatMonData.m_listTuFifoSize = m_rclsSipStack.getTuFifoSize();

		m_rclsSipStack.getStatMonCallback()(stStatMonData);
	}
}

}

#endif

