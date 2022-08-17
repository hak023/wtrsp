#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include "resip/stack/AbandonServerTransaction.hxx"
#include "resip/stack/ApplicationMessage.hxx"
#include "resip/stack/CancelClientInviteTransaction.hxx"
#include "resip/stack/Helper.hxx"
#include "resip/stack/TerminateFlow.hxx"
#include "resip/stack/EnableFlowTimer.hxx"
#include "resip/stack/ZeroOutStatistics.hxx"
#include "resip/stack/PollStatistics.hxx"
#include "resip/stack/ShutdownMessage.hxx"
#include "resip/stack/SipMessage.hxx"
#include "resip/stack/TransactionController.hxx"
#include "resip/stack/TransactionState.hxx"
#ifdef USE_SSL
#include "resip/stack/ssl/Security.hxx"
#endif
#include "rutil/CongestionManager.hxx"
#include "rutil/DnsUtil.hxx"
#include "rutil/Logger.hxx"
#include "resip/stack/SipStack.hxx"
#include "rutil/WinLeakCheck.hxx"

#ifdef ESIP_IN_RESIP
#include <sys/syscall.h>
#include "resip/stack/eSipHandler.hxx"
#include "resip/stack/TransactionControllerThread.hxx"
#endif

using namespace resip;

#define RESIPROCATE_SUBSYSTEM Subsystem::TRANSACTION

#if defined(WIN32) && !defined (__GNUC__)
#pragma warning( disable : 4355 ) // using this in base member initializer list 
#endif

unsigned int TransactionController::MaxTUFifoSize = 0;
unsigned int TransactionController::MaxTUFifoTimeDepthSecs = 0;

#ifdef ESIP_IN_RESIP
extern eSipHandler * g_pclsEsipHandler;
static const unsigned int s_unMaxProcTranscAtOnce = 16;

void TransactionControllerThread::thread()
{
   int nTid = syscall(SYS_gettid);
	char szThreadName[256];
	snprintf(szThreadName, sizeof(szThreadName)-1, "TrscTh_%d", mController.getTcId());

   CritLog(<<szThreadName<<" : Tid["<<nTid<<"] : start");	

   /*==== add HangMon ====*/
	if( mController.mStack.getHangMonAddCallback() )
		mController.mStack.getHangMonAddCallback()(E_STACK_HANG_THREAD_trsc, nTid, mController.getTcId(), mController.mStateMacFifo.getMaxFifoSize());

	/*==== add QueueAlarm ====*/
    if( mController.mStack.getQueueAlarmAddCallback() )
       mController.mStack.getQueueAlarmAddCallback()(nTid, E_STACK_QUEUE_ALARM_TYPE_trsc, mController.getTcId(), mController.mStateMacFifo.getMaxFifoSize());

   while(!isShutdown())
   {
		/*==== touch HangMon ====*/
      if( mController.mStack.getHangMonTouchCallback() )
      {
         mController.mStack.getHangMonTouchCallback()(E_STACK_HANG_THREAD_trsc, nTid,
				 																			mController.getTcId(), 
				 																			mController.mStateMacFifo.getCurrentFifoNum());
      }
	 
      mController.process(25);
   }

   CritLog(<<szThreadName<<" : Tid["<<nTid<<"] : stop");
}

TransactionController::TransactionController(SipStack& stack, 
														                              AsyncProcessHandler* handler,
														                              int _rnTcId,
														                              TransportSelector & _rclsTransportSelector,
														                              unsigned int m_unMaxTransactionFifoSize) :
   mStack(stack),
   mDiscardStrayResponses(false),
   mFixBadDialogIdentifiers(true),
   mFixBadCSeqNumbers(true),
   mStateMacFifo(handler),
   mStateMacFifoOutBuffer(mStateMacFifo),
   mCongestionManager(0),
   m_nTcId(_rnTcId),
   mTuSelector(stack.mTuSelector),
   mTransportSelector(_rclsTransportSelector),
   mTimers(mTimerFifo), 
   mShuttingDown(false),
   mStatsManager(stack.mStatsManager),
   mHostname(DnsUtil::getLocalHostName())
{
   Data clsFifoName = "Transc[";
   clsFifoName += Data(m_nTcId);
   clsFifoName += "]";
   mStateMacFifo.setDescription(clsFifoName);
   mStateMacFifo.setMaxFifoSize(m_unMaxTransactionFifoSize);
}
#else
TransactionController::TransactionController(SipStack& stack, 
                                                AsyncProcessHandler* handler) :
   mStack(stack),
   mDiscardStrayResponses(true),
   mFixBadDialogIdentifiers(true),
   mFixBadCSeqNumbers(true),
   mStateMacFifo(handler),
   mStateMacFifoOutBuffer(mStateMacFifo),
   mCongestionManager(0),
   mTuSelector(stack.mTuSelector),
   mTransportSelector(mStateMacFifo,
                      stack.getSecurity(),
                      stack.getDnsStub(),
                      stack.getCompression()),
   mTimers(mTimerFifo), 
   mShuttingDown(false),
   mStatsManager(stack.mStatsManager),
   mHostname(DnsUtil::getLocalHostName())
{
   mStateMacFifo.setDescription("TransactionController::mStateMacFifo");
}
#endif

#if defined(WIN32) && !defined(__GNUC__)
#pragma warning( default : 4355 )
#endif

TransactionController::~TransactionController()
{
   if(mClientTransactionMap.size())
   {
      WarningLog(<< "On shutdown, there are Client TransactionStates remaining!");
   }

   if(mServerTransactionMap.size())
   {
      WarningLog(<< "On shutdown, there are Server TransactionStates remaining!");
   }
}


bool 
TransactionController::isTUOverloaded() const
{
   return !mTuSelector.wouldAccept(TimeLimitFifo<Message>::EnforceTimeDepth);
}

void
TransactionController::shutdown()
{
   mShuttingDown = true;
#ifndef ESIP_IN_RESIP	 
   mTransportSelector.shutdown();
#endif
}

#ifdef ESIP_IN_RESIP
void TransactionController::fnProcTrTimer()
{
	unsigned int unTimeOutNums = mTimers.m_clsTimeWheel.fnProcessTimeOut(this,fnCbkTimeOut);
	if(unTimeOutNums > 500)
	{
		//ErrLog(<<"@@@ Time Out Over Proc Nums: "<<unTimeOutNums);
	}
}
void TransactionController::fnCbkTimeOut(void * _pvUser,eSipUtil::UtilTimeBase * pclsTO)
{
	TransactionController * pclsUser = (TransactionController*)_pvUser;
	TimerMessage * pclsNew = 	new TimerMessage(((TrTimeBase*)(pclsTO))->m_pclsTrTime->getTransactionId(), 
											((TrTimeBase*)(pclsTO))->m_pclsTrTime->getType(),
											((TrTimeBase*)(pclsTO))->m_pclsTrTime->getDuration());
	TransactionState::processTimer(*pclsUser,pclsNew);
}
#endif

void
TransactionController::process(int timeout)
{
#ifdef ESIP_IN_RESIP
	int nTid = syscall(SYS_gettid);

	try
	{
#endif	
		if(mShuttingDown && 
			//mTimers.empty() && 
			!mStateMacFifoOutBuffer.messageAvailable() && // !dcm! -- see below 
			!mStack.mTUFifo.messageAvailable() &&
			mTransportSelector.isFinished())
			// !dcm! -- why would one wait for the Tu's fifo to be empty before delivering a
			// shutdown message?
		{
			//!dcm! -- send to all?
			mTuSelector.add(new ShutdownMessage, TimeLimitFifo<Message>::InternalElement);
		}
		else
		{
#ifndef ESIP_IN_RESIP
			unsigned int nextTimer(mTimers.msTillNextTimer());
			timeout=resipMin((int)nextTimer, timeout);
			if(timeout==0)
			{
				// *sigh*
				timeout=-1;
			}
#endif
			// If non-zero is passed for timeout, we understand that the caller is ok
			// with us waiting up to that long on this call. A non-zero timeout is
			// passed by TransactionControllerThread, for example. This gets us 
			// something approximating a blocking wait on both the state machine fifo 
			// and the timer queue.
#ifdef ESIP_IN_RESIP
			TransactionMessage* message=mStateMacFifoOutBuffer.getNext(1);

			fnProcTrTimer();    // new time wheel		

			if( message )
			{
				// Only do 16 at a time; don't let the timer queue (or other 
				// processing) starve.
				int runs=s_unMaxProcTranscAtOnce;

				while(message)
				{
					TransactionState::process(*this, message);

					if(--runs==0)
					{
						break;
					}

					message = mStateMacFifoOutBuffer.getNext(-1);

					fnProcTrTimer();    // new time wheel
				}
			}
#else
			TransactionMessage* message=mStateMacFifoOutBuffer.getNext(timeout);
			// If we either had timers ready to go at the beginning of this call, or
			// the getNext() call above timed out, our timer queue is likely ready to 
			// be serviced.

			if(!message || nextTimer==0)
			{
				mTimers.process();
				TimerMessage* timer;				 
				while ((timer=mTimerFifo.getNext(-1)))
				{
					TransactionState::processTimer(*this,timer);			
				}			 
			}

			if(message)
			{
				// Only do 16 at a time; don't let the timer queue (or other 
				// processing) starve. 
				int runs=16;
				while(message)
				{
					gettimeofday(&(startTv1), NULL);

					TransactionState::process(*this, message);
					if(--runs==0)
					{
						break;
					} 						
					message = mStateMacFifoOutBuffer.getNext(-1);
				}
				mTransportSelector.poke();
			}
#endif			
		}
#ifdef ESIP_IN_RESIP	
	}
	catch(...)
	{
		ErrLog(<<"TrscCtl["<<m_nTcId<<"] : Tid["<<nTid<<"] : occur exception");
	}
#endif	
}

unsigned int 
TransactionController::getTimeTillNextProcessMS()
{
   if ( mStateMacFifoOutBuffer.messageAvailable() ) 
   {
      return 0;
   }
   return mTimers.msTillNextTimer();
} 

void
TransactionController::send(SipMessage* msg)
{
   if(msg->isRequest() && 
      msg->method() != ACK && 
      getRejectionBehavior()!=CongestionManager::NORMAL)
   {
      // Need to 503 this.
      SipMessage* resp(Helper::makeResponse(*msg, 503));
      resp->header(h_RetryAfter).value()=(UInt32)mStateMacFifo.expectedWaitTimeMilliSec()/1000;
      resp->setTransactionUser(msg->getTransactionUser());
      mTuSelector.add(resp, TimeLimitFifo<Message>::InternalElement);
      delete msg;
      return;
   }
   mStateMacFifo.add(msg);
}

#ifdef ESIP_IN_RESIP
bool TransactionController::send2(SipMessage* msg, unsigned int & _runCurrentSize, unsigned int & _runMaxSize)
{
	return mStateMacFifo.add4FailNoDelete(msg, _runCurrentSize, _runMaxSize);
}
#endif

unsigned int 
TransactionController::getTuFifoSize() const
{
   return mTuSelector.size();
}

unsigned int 
TransactionController::sumTransportFifoSizes() const
{
   return mTransportSelector.sumTransportFifoSizes();
}

unsigned int 
TransactionController::getTransactionFifoSize() const
{
   // Should we include the stuff in mStateMacFifoOutBuffer here too? This is
   // likely to be called from other threads...
#ifdef ESIP_IN_RESIP
   return mStateMacFifo.getCurrentFifoNum();
#else   
   return mStateMacFifo.size();
#endif
}

unsigned int 
TransactionController::getNumClientTransactions() const
{
   return mClientTransactionMap.size();
}

unsigned int 
TransactionController::getNumServerTransactions() const
{
   return mServerTransactionMap.size();
}

unsigned int 
TransactionController::getTimerQueueSize() const
{
#ifdef ESIP_IN_RESIP
   return mTimers.m_clsTimeWheel.m_unTotalTrCount;
#else
   return mTimers.size();
#endif
}

void 
TransactionController::zeroOutStatistics()
{
   mStateMacFifo.add(new ZeroOutStatistics());
}

void 
TransactionController::pollStatistics()
{
   mStateMacFifo.add(new PollStatistics());
}

void
TransactionController::registerMarkListener(MarkListener* listener)
{
   mTransportSelector.registerMarkListener(listener);
}

void TransactionController::unregisterMarkListener(MarkListener* listener)
{
   mTransportSelector.unregisterMarkListener(listener);
}

void 
TransactionController::abandonServerTransaction(const Data& tid)
{
   mStateMacFifo.add(new AbandonServerTransaction(tid));
}

void 
TransactionController::cancelClientInviteTransaction(const Data& tid)
{
   mStateMacFifo.add(new CancelClientInviteTransaction(tid));
}

void
TransactionController::terminateFlow(const resip::Tuple& flow)
{
   mStateMacFifo.add(new TerminateFlow(flow));
}

void
TransactionController::enableFlowTimer(const resip::Tuple& flow)
{
   mStateMacFifo.add(new EnableFlowTimer(flow));
}

void 
TransactionController::setInterruptor(AsyncProcessHandler* handler)
{
   mStateMacFifo.setInterruptor(handler);
}

/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2004 Vovida Networks, Inc.  All rights reserved.
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
