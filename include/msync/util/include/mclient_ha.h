/*
 *
 */

#ifndef HACLIENT_H
#define HACLIENT_H

#include "mclient.h"
#include "haevent.h"

class CMClientHA : public CMClient
{
public:
   CMClientHA();
   ~CMClientHA();
   int Init(void* pParm);
   int Start();
   void Stop();
	int Send(void * pMsg,int len=0);
   int PutEvent(void* pMsg, int nTryCnt=1);
protected:
   void *ThreadProc();
	HAEvent* m_pQEvent;
	HAEvent* m_pRcvEvent;
};

#endif
////@}

