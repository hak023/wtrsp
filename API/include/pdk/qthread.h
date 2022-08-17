
#ifndef _THREAD_WITH_QUEUE_H_
#define _THREAD_WITH_QUEUE_H_

#include "eventdefs.h"

class CQThread : public CThread
{
   DECLARE_DYNAMIC(CQThread)

public:
   CQThread();
   CQThread(int nBufferLen);
   ~CQThread();

   virtual bool Start();
   virtual void Stop();
   virtual void Close();

   bool Add(PEVENTINFO pEvent);
   PEVENTINFO Remove();
   bool Remove(PEVENTINFO pEvent);
   PEVENTINFO Remove_();
	int GetLength() { return m_pQueue->GetLength(); }

   virtual bool AddEvent(PDK16U type, PDK16U stype, char *pValue = NULL, int nLen = 0);
   virtual bool AddEvent(PDK16U type, PDK16U stype, PDK32U value);
protected:
   virtual bool AddQuitEvent();

protected:
   CEventQueue *m_pQueue;
};

inline bool CQThread::AddQuitEvent()
   { return AddEvent(CI_QUIT_THREAD, 0); }

#endif
