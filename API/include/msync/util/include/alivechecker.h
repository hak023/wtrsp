#ifndef  __AM_ALIVECHECKER__H__
#define __AM_ALIVECHECKER__H__

#include <list>
#ifndef WIN32
#include "r_util.h" //PDK-UTIL
#endif

#include "r_jthread.h"

class CAMCheckerObject
{
public:
	CAMCheckerObject() {}
	virtual ~CAMCheckerObject() {}

   virtual void CheckingAlive()=0;
};


class CAMAliveChecker: public RED::CThread
{
protected:
   std::list <CAMCheckerObject*> m_List;

public:      
   CAMAliveChecker() {
   };
   ~CAMAliveChecker() {
      
   };

   int AddCheckerObject(CAMCheckerObject * pObj) { 
      m_List.insert(m_List.end(), pObj);
      return 0;
   }
   // Clear...

   bool Start() {
      return Create();
   };

protected:
   void* ThreadProc(){
      CAMCheckerObject * pChkObject;
      while (!DoExit()) { // per 1sec
         std::list <CAMCheckerObject*>::iterator it;
         for(it = m_List.begin(); it != m_List.end() && !DoExit(); it++) {
            if(*it!=NULL) pChkObject = (CAMCheckerObject*)*it;
				if(pChkObject) pChkObject->CheckingAlive();
            RED::MilliSleep(100);
         }
         RED::MilliSleep(500);
      }
      return NULL;
   };

};


#endif //__AM_ALIVECHECKER__H__

