#ifndef __AM_TCP_SERVER__H__
#define __AM_TCP_SERVER__H__

//#include "jthread.h" //pdk
#include "logmgr.h"
#include "r_util.h"


#include "amsock.h"
#include "alivechecker.h"

enum AM_CONN_TYPE {
   ACT_APPS       	= 1,
   //ACT_BOARD
};


enum AM_CONN_STATUS {
   ACS_IDLE       	= 1,
   ACS_CONNECTING 	   ,
   ACS_CONNECTED  	   ,
   ACS_READY      	   ,
   ACS_DISCONNECTING	 
};


class CAMReceiverObject
{
public:
   CAMReceiverObject() {}
   virtual ~CAMReceiverObject() {}

   virtual int RecvProc()=0;
};

class CAMSenderObject
{
public:
   CAMSenderObject() {}
   virtual ~CAMSenderObject() {}

   virtual int SendProc()=0;
};


class CAMWorkerManager
{
public:
	CAMWorkerManager() {}
	virtual ~CAMWorkerManager() {}
//   virtual int AssignWorker(int nSockFD, MTM_CONN_TYPE nLsnrType)=0;
   virtual int AssignWorker(int nSockFD, AM_CONN_TYPE nConType)=0;
};


class CAMSuspender
{
protected:
   RED::CSemaphore * m_pSemWait;
   RED::CSemaphore * m_pSemTick;
   bool      m_bSuspended;

protected:
   void CheckSuspension() {
      if(m_bSuspended) {
         m_pSemWait->Wait();
         m_pSemTick->Signal(); 
      }
   };
   
public:

   CAMSuspender() {
      m_bSuspended = true;
      m_pSemWait = new RED::CSemaphore(0);
      m_pSemTick = new RED::CSemaphore(1);
   };

   ~CAMSuspender() {
	   delete m_pSemWait;
	   delete m_pSemTick;
   }
   
   void Resume() {
      m_bSuspended = false;
      if(m_pSemTick->WillBlock() == false) {
         m_pSemWait->Signal();
      }
   }; 

   void Suspend() {
      m_bSuspended = true;
   }
};

class CAMReceiver : public RED::CThread, public CAMSuspender
{
protected:
   int m_nId;
   char m_szDesc[64];
   CAMReceiverObject * m_pObject;
   CLogMgr * m_pLogMgr;
   
protected:
   void * ThreadProc() {
      int rc;
      while(!DoExit()) {
         CheckSuspension();
         //LOGMGR(PL_INFO, "%s::ThreadProc", m_szDesc);
         rc = m_pObject->RecvProc();
         if(rc < 1) {
            RED::MilliSleep(10);   
         }
      }
      return NULL;   
   }

public:
   CAMReceiver(CAMReceiverObject * pObject) {
      m_pObject = pObject;
      m_pLogMgr = CLogMgr::GetInstance();
   };
   
   ~CAMReceiver() {};   

   bool Start(int nId) { 
      m_nId = nId;
	   sprintf(m_szDesc,"[RCV_%-11d] ", m_nId);
      LOGMGR(PL_API, "%sStart", m_szDesc);
      return Create(); 
   };
   

   void Resume() {
      LOGMGR(PL_API, "%s%s, suspended=%d",m_szDesc,__func__, m_bSuspended);
      CAMSuspender::Resume();
   }; 

   void Suspend() {
      LOGMGR(PL_API, "%s%s, suspended=%d",m_szDesc,__func__,  m_bSuspended);
      CAMSuspender::Suspend();
   }
   
};

class CAMSender : public RED::CThread
{
protected:
   int  m_nId;
   char m_szDesc[64];
   CAMSenderObject * m_pObject;

   CLogMgr * m_pLogMgr;

protected:
   CAMSender(CAMSenderObject * pObject) {
      m_pObject = pObject;
      m_pLogMgr = CLogMgr::GetInstance();
   };

   void * ThreadProc() {
      int rc;
      while(!DoExit()) {
         //LOGMGR(PL_INFO, "%s::ThreadProc()", m_szDesc);
         rc = m_pObject->SendProc();
         if(rc < 1) {
            RED::MilliSleep(10);   
         }
      }
      return NULL;   
   };

   bool Start(int nId) { 
      m_nId = nId;
      sprintf(m_szDesc, "[SND_%-11d] ", m_nId);
      LOGMGR(PL_INFO, "%sStart", m_szDesc);
      return Create() == 0; 
   };
};

class CAMListener : public RED::CThread
{
private:
   int      m_nSockFD;
   char     m_szIpAddr[128];
   int      m_nPort;

   CAMWorkerManager * m_pManager;
   AM_CONN_TYPE      m_nUserID;  

   CLogMgr * m_pLogMgr;
   
protected:
   void * ThreadProc();

public:
   CAMListener(CAMWorkerManager * pManager, AM_CONN_TYPE nUserID); // pManager->AssignWorker
   ~CAMListener();

   void CloseSocket();
   bool Start(const char * szIPAddr, int nPort);
   int  GetSockFd() { return m_nSockFD; };
   void Terminate(); 
};

#endif //__AR_TCP_SERVER__H__

