#ifndef RESIP_StatisticsManager_hxx
#define RESIP_StatisticsManager_hxx

#include "rutil/Timer.hxx"
#include "rutil/Data.hxx"
#include "resip/stack/StatisticsMessage.hxx"
#include "resip/stack/StatisticsHandler.hxx"

#ifdef ESIP_IN_RESIP
#include <sys/time.h>
#include "eSipUtil/shm.hxx"
#endif

namespace resip
{
class SipStack;
class SipMessage;
class TransactionController;
class Tuple;

#ifdef ESIP_IN_RESIP

#define E_CONST_STAT_MAX_TRANSPORT 500

struct StatShm
{
	char m_szMagicCookie[50];
	int m_cWriteFlag;			//0: A, 1:B
	time_t m_stNextWritTime;
	StatByTuple_t m_arrStatA[E_CONST_STAT_MAX_TRANSPORT];
	StatByTuple_t m_arrStatB[E_CONST_STAT_MAX_TRANSPORT];
	unsigned int m_unStatATransportNumber;
	unsigned int m_unStatBTransportNumber;	

	void reset();
};
typedef struct StatShm StatShm_t;

class StatisticsManager
{
	public:     
		StatisticsManager(SipStack & _rclsStack, unsigned long _ulIntervalSecs, int _nShmKey, const resip::Data & _rclsShmPath);
		~StatisticsManager();

		static void * procThreadWrapper(void * _pStatMgr);
		void stop();
		void join();		
		void process();

		void setExternalStatsHandler(ExternalStatsHandler *handler)
		{
			mExternalHandler = handler;
		}
		void zeroOut();

	private:
		friend class TransactionState;
		friend class eSipHandler;
		friend class SipStack;

		bool sent(MethodTypes _eMethodType, int _nRspCode, 
							const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType);
		bool retransmitted(MethodTypes _eMethodType, int _nRspCode, 
												const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType);
		bool received(MethodTypes _eMethodType, int _nRspCode, 
									const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType);
		bool receivedDrop(EMsgDropType_t eMsgDropType, MethodTypes _eMethodType, int _nRspCode, 
											const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType);
		bool receivedDropByTransc(EMsgDropType_t eMsgDropType, MethodTypes _eMethodType, int _nRspCode, 
																	const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType);
		bool receivedRetrans(const SipMessage & _rclsSipMsg);

		void poll(); // force an update

		SipStack& mStack;
		UInt64 mInterval;
		ExternalStatsHandler *mExternalHandler;
		time_t m_stNextTimeTick;
		Mutex m_clsStatLock;
		pthread_t m_procTh;
		bool m_bShutdown;

		int m_nShmKey;
		resip::Data m_clsShmPath;
		StatShm_t * m_pstStatShm;

};

#else

/**
   @brief Keeps track of various statistics on the stack's operation, and 
      periodically issues a StatisticsMessage to the TransactionUser (or, if the
      ExternalStatsHandler is set, it will be sent there).
*/
class StatisticsManager : public StatisticsMessage::Payload
{
   public:
      // not implemented
      typedef enum
      {
         TransportFifoSize,
         TUFifoSize,
         ActiveTimers,
         OpenTcpConnections,
         ActiveClientTransactions,
         ActiveServerTransactions,
         PendingDnsQueries,
         StatsMemUsed
      } Measurement;
      
      StatisticsManager(SipStack& stack, unsigned long intervalSecs=60);
      ~StatisticsManager();

      void process();
      // not stricly thread-safe; needs to be called through the fifo somehow
      void setInterval(unsigned long intvSecs);

      /**
         @ingroup resip_config
         @brief Allows the application to set the ExternalStatsHandler for this
            StatisticsManager.
      */
      void setExternalStatsHandler(ExternalStatsHandler *handler)
      {
         mExternalHandler = handler;
      }

   private:
      friend class TransactionState;
      bool sent(SipMessage* msg);
      bool retransmitted(MethodTypes type, bool request, unsigned int code);
      bool received(SipMessage* msg);

      void poll(); // force an update

      SipStack& mStack;
      UInt64 mInterval;
      UInt64 mNextPoll;

      ExternalStatsHandler *mExternalHandler;

      //
      // When statistics are published, a copy of values are made
      // and copied into this member, and then reference to this is
      // published thru both ExternalHandler and posted to stack as message.
      // This payload is mutex protected.
      StatisticsMessage::AtomicPayload *mPublicPayload;

};

#endif

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
 */
