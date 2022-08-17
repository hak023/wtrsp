/*
 *
 */

#ifndef _MCLIENT_H
#define _MCLIENT_H

#include "r_jthread.h"
#include "logmgr.h"
#include "mptcpserver.h"
#include "alivechecker.h"
#include "app_com.h"
#include "app_commsg.h"
#include "r_sock.h"

class CMClient : public RED::CThread
{
public:
	enum MD_CLIMODE{
		MD_NONE = 0,
		MD_HA
	};
	typedef struct InitParm{
		void* queue;
		uint	rmip;
		uint	rmport;
		void clear(){ memset(this,0x00,sizeof(InitParm));}
		InitParm(){clear();}
	}InitParm;

   CMClient();
   ~CMClient();
   int Init(void* pParm);
   int Start();
   void Stop();
   //void Close() { m_Socket.Close(); m_Status = TCP_DISC; }
   
   AM_CONN_STATUS GetConStatus() { return m_Status; }
   void SetConStatus(AM_CONN_STATUS Status) { m_Status = Status; }
	int GetLastRecvTime(){return m_nLastRecvTime;}
	int GetLastSendTime(){return m_nLastSendTime;}
	virtual int Send(void *Buffer, int nLen=0);
   virtual int PutEvent(void* pMsg, int nTryCnt=1);
protected:
   virtual void *ThreadProc();
   char         m_szDesc[32];
   void *  m_pQueue;//sessionmgr queue
   RED::CTcpSocket   m_Socket;
   RED::CCritSec     m_SendLock;
	int			m_nMode;// enum MD_CLIMODE
   char         m_szAddr[16];
   int          m_nPort;
	int            	m_nLastSendTime;
   int            	m_nLastRecvTime;
	int			m_nSetup;
   AM_CONN_STATUS    m_Status;
   CLogMgr      *m_pLogMgr;
};

#endif
////@}

