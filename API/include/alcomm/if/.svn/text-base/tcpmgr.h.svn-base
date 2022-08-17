#ifndef __AL_TCP_MGR_IF_H__
#define __AL_TCP_MGR_IF_H__

#include "tcpif.h"

template < typename Handler, int RecvQueueSize=256 >
class CTCPManager
   : public CUserThread
{
public:
   typedef Handler implementation;
   typedef CQueue<AL_TCP_QUEUE_ELEMENT*,AL_TCP_QUEUE_ELEMENT> DataQueue;
   typedef CALTCPSession<AL_TCP_QUEUE_ELEMENT> TCPSession;
public:
   CTCPManager()
      : m_Impl(new implementation),
        m_pQueue(new DataQueue(RecvQueueSize)),
        m_pTCPSes(new TCPSession)
   {

   }
   ~CTCPManager()
   {
   }
   bool Start( char* szIP,int nPort,int nID,
               int nTCPMode,int nMaxSesCnt,int nSendAlive)
   {
      bool bRet=false;
      bRet=m_pTCPSes->Start(  szIP,nPort,nID,nTCPMode,
                              nMaxSesCnt,m_pQueue,nSendAlive);
      if ( bRet )
      {
         m_Impl->SetControl(m_pTCPSes);
         return CUserThread::Start();
      }
      return bRet;
   }
   TCPSession* GetTCPSession()
   {
      return m_pTCPSes;
   }

   void Stop()
   {
      CUserThread::Stop();
      m_pTCPSes->Stop();
   }
   virtual void* ThreadProc()
   {
      AL_TCP_QUEUE_ELEMENT* pEvent=new AL_TCP_QUEUE_ELEMENT;      
      int nRet=0;
      while(!DoExit()){
         if(m_bStopped) break;
         nRet=m_pQueue->Get_(pEvent);
         if(m_bStopped) break;
         if( nRet != 1 ) 
         {
            MilliSleep(20);
            continue; 
         }
         m_Impl->Notify(pEvent);
         if(m_bStopped) break;
      }
      m_bEscaped=true;
      return NULL;
   }
protected:
   implementation*  m_Impl;
   DataQueue* m_pQueue;
   TCPSession* m_pTCPSes;
};

#endif
