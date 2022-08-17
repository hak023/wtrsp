#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include "rutil/Socket.hxx"
#include "rutil/Logger.hxx"
#include "resip/stack/Connection.hxx"
#include "resip/stack/ConnectionManager.hxx"
#include "resip/stack/InteropHelper.hxx"
#include "resip/stack/SipMessage.hxx"
#include "resip/stack/TcpBaseTransport.hxx"
#include "rutil/WinLeakCheck.hxx"

#ifdef USE_SSL
#include "resip/stack/ssl/Security.hxx"
#endif

#ifdef USE_SIGCOMP
#include <osc/Stack.h>
#include <osc/SigcompMessage.h>
#endif

#ifdef ESIP_IN_RESIP
#include "resip/stack/eSipHandler.hxx"
#endif

using namespace resip;

volatile bool Connection::mEnablePostConnectSocketFuncCall = false;

#ifdef ESIP_IN_RESIP
extern eSipHandler * g_pclsEsipHandler;
#endif

#define RESIPROCATE_SUBSYSTEM Subsystem::TRANSPORT

Connection::Connection(Transport* transport,const Tuple& who, Socket socket,
                       Compression &compression)
   : ConnectionBase(transport,who,compression),
     mRequestPostConnectSocketFuncCall(false),
     mInWritable(false),
     mFlowTimerEnabled(false),
     mPollItemHandle(0)
{
   mWho.mFlowKey=(FlowKey)socket;
#ifdef ESIP_IN_RESIP
   DebugLog (<< "Connection::Connection: new connection created to who: " << mWho);
#else
   InfoLog (<< "Connection::Connection: new connection created to who: " << mWho);
#endif

   if(mWho.mFlowKey && ConnectionBase::transport())
   {
      getConnectionManager().addConnection(this);
   }
}

Connection::~Connection()
{
   if(mWho.mFlowKey && ConnectionBase::transport())
   {
      getConnectionManager().removeConnection(this);
      // remove first then close, since conn manager may need socket
      closeSocket(mWho.mFlowKey);
   }
}

void
Connection::requestWrite(SendData* sendData)
{
   mOutstandingSends.push_back(sendData);
   if (isWritable())
   {
      ensureWritable();
   }
}

void 
Connection::removeFrontOutstandingSend()
{
   delete mOutstandingSends.front();
   mOutstandingSends.pop_front();

   if (mOutstandingSends.empty())
   {
      assert(mInWritable);
      getConnectionManager().removeFromWritable(this);
      mInWritable = false;
   }
}

int
Connection::performWrite()
{
   if(transportWrite())
   {
      assert(mInWritable);
      getConnectionManager().removeFromWritable(this);
      mInWritable = false;
      return 0; // What does this transportWrite() mean?
   }

   assert(!mOutstandingSends.empty());
   switch(mOutstandingSends.front()->command)
   {
   case SendData::CloseConnection:
      // .bwc. Close this connection.
      return -1;
      break;
   case SendData::EnableFlowTimer:
      enableFlowTimer();
      removeFrontOutstandingSend();
      return 0;
      break;
   default:
      // do nothing
      break;
   }

   const Data& sigcompId = mOutstandingSends.front()->sigcompId;

   if(mSendingTransmissionFormat == Unknown)
   {
      if (sigcompId.size() > 0 && mCompression.isEnabled())
      {
         mSendingTransmissionFormat = Compressed;
      }
      else
      {
         mSendingTransmissionFormat = Uncompressed;
      }
   }


#ifdef USE_SIGCOMP
   // Perform compression here, if appropriate
   if (mSendingTransmissionFormat == Compressed
       && !(mOutstandingSends.front()->isAlreadyCompressed))
   {
      const Data& uncompressed = mOutstandingSends.front()->data;
      osc::SigcompMessage *sm = 
        mSigcompStack->compressMessage(uncompressed.data(), uncompressed.size(),
                                       sigcompId.data(), sigcompId.size(),
                                       true);
      DebugLog (<< "Compressed message from "
                << uncompressed.size() << " bytes to " 
                << sm->getStreamLength() << " bytes");

      SendData *oldSd = mOutstandingSends.front();
      SendData *newSd = new SendData(oldSd->destination,
                                     Data(sm->getStreamMessage(),
                                          sm->getStreamLength()),
                                     oldSd->transactionId,
                                     oldSd->sigcompId,
                                     true);
      mOutstandingSends.front() = newSd;
      delete oldSd;
      delete sm;
   }
#endif

   if(mEnablePostConnectSocketFuncCall && mRequestPostConnectSocketFuncCall)
   {
       // Note:  The first time the socket is available for write, is when the TCP connect call is completed
      mRequestPostConnectSocketFuncCall = false;
      mTransport->callSocketFunc(getSocket());
   }

#ifdef ESIP_IN_RESIP
   SendData * pclsSndData = mOutstandingSends.front();
   if( !pclsSndData )
   {
      ErrLog(<<"Connection : performWrite : invalid SndData");
      return 0;
   }
   if( pclsSndData->m_nDSCP < 0 )
      pclsSndData->m_nDSCP = 0;
   unsigned int unDSCP = (unsigned int)(pclsSndData->m_nDSCP);
   unDSCP = unDSCP<<2;
   if( setsockopt(this->getSocket(), IPPROTO_IP, IP_TOS, (const void *)&unDSCP, sizeof(unDSCP)) != 0 )
   {
      ErrLog(<<"Connection : performWrite : fail to set DSCP : Local["<<mTransport->getTuple()<<"], Remote["<<mWho<<"]: DSCP["<<pclsSndData->m_nDSCP<<"] : "<<strerror(errno));
   }
   const Data& data = pclsSndData->data;
#else
   const Data& data = mOutstandingSends.front()->data;
#endif

   int nBytes = write(data.data() + mSendPos,int(data.size() - mSendPos));

   //DebugLog (<< "Tried to send " << data.size() - mSendPos << " bytes, sent " << nBytes << " bytes");

   if (nBytes < 0)
   {
      if(errno!=EAGAIN)
      {
         //fail(data.transactionId);
#ifdef ESIP_IN_RESIP
         ErrLog(<<"Connection : fail to write : Err["<<errno<<","<<strerror(errno)<<"] : Fd["<<this->getSocket()<<"], DataSize["<<pclsSndData->data.size()<<"], Local["<<mTransport->getTuple()<<"], Dst["<<mWho<<"] : closing connection");
#else         
         InfoLog(<< "Write failed on socket: " << this->getSocket() << ", closing connection");
#endif
         return -1;
      }
      else
      {
         return 0;
      }
   }
   else
   {
      // Safe because of the conditional above ( < 0 ).
      Data::size_type bytesWritten = static_cast<Data::size_type>(nBytes);
      mSendPos += bytesWritten;
      if (mSendPos == data.size())
      {
#ifdef ESIP_IN_RESIP
         if( g_pclsEsipHandler )
         {
            TraceInfo_t * pstTraceInfo = NULL;
            if( pclsSndData->m_stTraceInfo.m_bTraceFlag )
               pstTraceInfo = &(pclsSndData->m_stTraceInfo);	
            else
               pstTraceInfo = NULL;
            g_pclsEsipHandler->onSend(pclsSndData->data, pclsSndData->data.size(), pclsSndData->m_eMethodType, pclsSndData->m_nRspCode,
                                                          Tuple::inet_ntop(mTransport->getTuple()).c_str(), mTransport->getTuple().getPort(), 
                                                          Tuple::inet_ntop(mWho).c_str(), mWho.getPort(), mTransport->getTuple().getType(), 
                                                          pclsSndData->m_unLocalNodeId, pclsSndData->m_unRemoteNodeId, pclsSndData->m_unRouteId,
                                                          pstTraceInfo, pclsSndData->m_bRetrans);
         }			
#endif      
         mSendPos = 0;
         removeFrontOutstandingSend();
      }
      return bytesWritten;
   }
}


bool 
Connection::performWrites(unsigned int max)
{
   int res;
   // if max==0, we will overflow into UINT_MAX. This is intentional.
   while((res=performWrite())>0 && !mOutstandingSends.empty() && --max!=0)
   {;}

   if(res<0)
   {
      delete this;
      return false;
   }
   return true;
}

void 
Connection::ensureWritable()
{
   if(!mInWritable)
   {
      assert(!mOutstandingSends.empty());
      getConnectionManager().addToWritable(this);
      mInWritable = true;
   }
}

ConnectionManager&
Connection::getConnectionManager() const
{
   TcpBaseTransport* transport = static_cast<TcpBaseTransport*>(ConnectionBase::transport());
   
   return transport->getConnectionManager();
}
            
EncodeStream& 
resip::operator<<(EncodeStream& strm, const resip::Connection& c)
{
   strm << "CONN: " << &c << " " << int(c.getSocket()) << " " << c.mWho;
   return strm;
}

int
Connection::read()
{
   std::pair<char*, size_t> writePair = getWriteBuffer();
   size_t bytesToRead = resipMin(writePair.second, 
                                 static_cast<size_t>(Connection::ChunkSize));
         
   assert(bytesToRead > 0);

   int bytesRead = read(writePair.first, (int)bytesToRead);
   if (bytesRead <= 0)
   {
      return bytesRead;
   }  
   // mBuffer might have been reallocated inside read()
   writePair = getCurrentWriteBuffer();

   getConnectionManager().touch(this);

#ifdef USE_SIGCOMP
   // If this is the first data we read, determine whether the
   // connection is compressed.
   if(mReceivingTransmissionFormat == Unknown)
   {
     if (((writePair.first[0] & 0xf8) == 0xf8) && mCompression.isEnabled())
     {
       mReceivingTransmissionFormat = Compressed;
     }
     else
     {
       mReceivingTransmissionFormat = Uncompressed;
     }
   }

   // SigComp compressed messages are handed very differently
   // than non-compressed messages: they are guaranteed to
   // be framed within SigComp, and each frame contains
   // *exactly* one SIP message. Processing looks a lot like
   // it does for Datagram-oriented transports.

   if (mReceivingTransmissionFormat == Compressed)
   {
     decompressNewBytes(bytesRead);
   }
   else
#endif
   {
     if(!preparseNewBytes(bytesRead))
     {
        // Iffy; only way we have right now to indicate that this connection has
        // gone away.
        bytesRead=-1;
     }
   }
   return bytesRead;
}

bool 
Connection::performReads(unsigned int max)
{
   int bytesRead;

   // if max==0, we will overflow into UINT_MAX. This is intentional.
   while((bytesRead = read())>0 && --max!=0)
   {
      DebugLog(<< "Connection::performReads() " << " read=" << bytesRead);
   }

   if ( bytesRead < 0 ) 
   {
      DebugLog(<< "Closing connection bytesRead=" << bytesRead);
      delete this;
      return false;
   }
   return true;
}

void
Connection::enableFlowTimer()
{
   if(!mFlowTimerEnabled)
   {
      mFlowTimerEnabled = true;

      // ensure connection is in a FlowTimer LRU list on the connection manager
      getConnectionManager().moveToFlowTimerLru(this);
   }
}

void
Connection::onDoubleCRLF()
{
   // !bwc! TODO might need to make this more efficient.
   // !bwc! Need to make this sigcomp-friendly
   if(InteropHelper::getOutboundVersion()>=8)
   {
      DebugLog(<<"Sending response CRLF (aka pong).");
      requestWrite(new SendData(mWho,Symbols::CRLF,Data::Empty,Data::Empty));
   }
}

void
Connection::onSingleCRLF()
{
   DebugLog(<<"Received response CRLF (aka pong).");
   mTransport->keepAlivePong(mWho);
}

bool 
Connection::hasDataToRead()
{
   return true;
}

bool 
Connection::isGood()
{
   return true;
}

bool 
Connection::isWritable()
{
   return true;
}

/**
    Virtual function of FdPollItemIf, called to process io events
**/
void
Connection::processPollEvent(FdPollEventMask mask) {
   /* The original code in ConnectionManager.cxx didn't check
    * for error events unless no writable event. (e.g., writable
    * masked error. Why?)
    */
   if ( mask & FPEM_Error ) 
   {
      Socket fd = getSocket();
      int errNum = getSocketError(fd);
#ifdef ESIP_IN_RESIP
      DebugLog(<< "Exception on socket " << fd << " code: " << errNum << "; closing connection");
#else
      InfoLog(<< "Exception on socket " << fd << " code: " << errNum << "; closing connection");
#endif
      setFailureReason(TransportFailure::ConnectionException, errNum);
      delete this;
      return;
   }
   if ( mask & FPEM_Write ) 
   {
      if(!performWrites())
      {
         // Just deleted self
         return;
      }
   }
   if ( mask & FPEM_Read ) 
   {
      performReads();
   }
}

/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000
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
