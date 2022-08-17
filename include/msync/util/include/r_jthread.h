#ifndef  __RPDK_SYNC_CLASS_H__
#define  __RPDK_SYNC_CLASS_H__

#define  MAX_TIMEINTERVAL     ((long)0x7fffffff)
//#define CI_QUIT_THREAD 0xf004

#include "r_base.h"

#define DEF_Q_LENGTH_BUG  // cwkim

#ifdef WIN32
typedef CRITICAL_SECTION pdk_mutex_t;
typedef HANDLE           p_pdk_sem_t;
typedef struct
{
    HANDLE handle;
    int i_wait_count;
} pdk_pthread_t;

#else /*WIN32*/
#include <semaphore.h>
#include <pthread.h>
// 2011.03.08 by minimei7
#define SIGNAL_CONTROL
#define PDK_RED_USE_TIMED_SEM_WAIT 1
#ifdef SIGNAL_CONTROL
# include <signal.h>
#endif
///////

typedef pthread_mutex_t  pdk_mutex_t;
typedef sem_t*           p_pdk_sem_t;
typedef pthread_t        pdk_pthread_t;

#define GetCurrentThreadId() pthread_self()

#endif /*WIN32*/


namespace RED
{
class DLLPORT CCritSec
{
public:
   CCritSec();
   ~CCritSec();
   void Lock();
   void Unlock();
   int TryLock();

protected:
   pdk_mutex_t  m_Mutex;
	//pthread_cond_t m_Cond;
};


class DLLPORT CAutoLock
{
public:
   CAutoLock(CCritSec *pLock) : m_pLock(pLock) { m_pLock->Lock(); }
   ~CAutoLock() { m_pLock->Unlock(); }

protected:
   CCritSec *m_pLock;
};


class DLLPORT CSemaphore
{
public:
   CSemaphore(unsigned value=1);

   virtual ~CSemaphore();

   bool ReCreateSemaphore(unsigned value = 1);

   int Wait();
   int Wait(long msecs); // in MilliSeconds
   int TryWait();

   int Signal();

   int WillBlock();

   bool Unlock();
   bool Unlock(long lCount, int* lprevCount = NULL);
   bool Lock();
   bool Lock(unsigned int dwTimeout);

protected:
   bool CreateObject(unsigned value);

   p_pdk_sem_t m_pSemaphore;
   bool  m_bCreate;
#if(PDK_RED_USE_TIMED_SEM_WAIT)
	pthread_mutex_t m_Mutex;
	pthread_cond_t m_Cond;
#endif
};

class CAutoSemaphore
{
public:
   CAutoSemaphore(CSemaphore *pSema) : m_pSema(pSema) { m_pSema->Wait(); }
   ~CAutoSemaphore() { m_pSema->Signal(); }

protected:
   CSemaphore *m_pSema;
};


class CSync : public CSemaphore
{
public:
   CSync() : CSemaphore(0) {}
};

class CSyncAck : public CSync
{
public:
   virtual void Signal() { CSync::Signal(); m_Ack.Wait(); }
   void Ack() { m_Ack.Signal(); }

protected:
   CSync m_Ack;
};

enum {DEFAULT_QUEUESIZE=20};


template <class T, class ARGTYPE> class CQueue
{
private:
   CCritSec m_PutCritSec;
   CCritSec m_GetCritSec;
   int m_nMax;
   int m_nFrontPos;
   int m_nRearPos;
   T *m_QueueObjects;
   ARGTYPE *m_pBuffer;
   struct timespec tv;
   pthread_mutex_t m_Mutex;
   pthread_cond_t m_Cond;
	struct timeval m_val;
	#ifdef DEF_Q_LENGTH_BUG // cwkim GetLength Bug
	CCritSec m_clsCountLock;
   unsigned int m_unCnt;
   #endif

public:


   void Initialize(int n)
   {
      m_nFrontPos = m_nRearPos  = 0;
	  #ifdef DEF_Q_LENGTH_BUG // cwkim GetLength Bug
       m_unCnt = 0;
	#endif
      m_nMax = n;
      m_QueueObjects = new T[n];
      m_pBuffer = new ARGTYPE[n];
      if (m_QueueObjects != NULL && m_pBuffer != NULL) {
        for (int i=0; i<n; i++) {
         m_QueueObjects[i] = &m_pBuffer[i];
        }
      }
      pthread_mutex_init(&m_Mutex,NULL);
      pthread_cond_init(&m_Cond,NULL);
		pthread_mutexattr_t m_MutexAttr;
      pthread_mutexattr_init(&m_MutexAttr);
      pthread_mutexattr_setpshared(&m_MutexAttr, PTHREAD_PROCESS_PRIVATE);
      //pthread_mutex_init(&m_PutLock, &m_MutexAttr);
      //pthread_mutex_init(&m_GetLock,&m_MutexAttr);
      pthread_mutexattr_destroy(&m_MutexAttr);
		m_val.tv_sec = 0;
		m_val.tv_usec = 10000;
   }

   CQueue() {Initialize(DEFAULT_QUEUESIZE);}
   CQueue(int n) {Initialize(n); }
   ~CQueue()
   {
      Free();
   }
   void Free()
   {
      /*delete [] m_QueueObjects;
      m_QueueObjects=NULL;
      delete [] m_pBuffer;
      m_pBuffer=NULL;*/
      pthread_cond_destroy(&m_Cond);
      pthread_mutex_destroy(&m_Mutex);
   }

   inline bool IsFull()
   {
      if(m_nFrontPos==(m_nRearPos+1)%m_nMax)
         return true;
      else
         return false;
   }

   inline bool IsEmpty()
   {
      if(m_nFrontPos==m_nRearPos)
         return true;
      else
         return false;
   }
	inline int GetLength()
  {
  	#ifdef DEF_Q_LENGTH_BUG // cwkim GetLength Bug
      return m_unCnt;
  #else  // orig code
     if(m_nRearPos >= m_nFrontPos)
        return m_nRearPos - m_nFrontPos;
     else
        return m_nMax - (m_nFrontPos-m_nRearPos);
	 #endif
  }
  inline int GetMaxLen()
  {
		return m_nMax;
	}
   int Put(T object,int nLen=0)
   {
      int nTmpPos;

      m_PutCritSec.Lock();

      for(int nIndex = 0; ; nIndex ++) {
         nTmpPos = m_nRearPos + 1;
         if(nTmpPos >= m_nMax)
            nTmpPos = 0;
         if(nTmpPos == m_nFrontPos) {// Queue FULL
            if ( nIndex == 3) {// retry for 3 times
               m_PutCritSec.Unlock();
               return -1;//Queue FULL, discard DATA
            }
            else  {
               //tv.tv_sec = (time_t) 0;
               //tv.tv_nsec = (long) 10000000;
               //nanosleep (&tv, &tv);
					select(0,0,0,0,&m_val);
               continue;
            }
         }
         else break;
      }// end of for
      if(!nLen) nLen = sizeof(ARGTYPE);
      memcpy (m_QueueObjects[nTmpPos], object, nLen);
      m_nRearPos = nTmpPos;
     #ifdef DEF_Q_LENGTH_BUG // cwkim GetLength Bug
     m_clsCountLock.Lock();
    ++m_unCnt;
	m_clsCountLock.Unlock();
    #endif
      pthread_mutex_lock(&m_Mutex);
      pthread_cond_signal(&m_Cond);
      pthread_mutex_unlock(&m_Mutex);

      m_PutCritSec.Unlock();

      return 0;

   }

   int Get(void *obj)
   {
      if (!m_QueueObjects) return 0;
      m_GetCritSec.Lock();

      pthread_mutex_lock(&m_Mutex);
      while(m_nFrontPos==m_nRearPos)
         pthread_cond_wait(&m_Cond,&m_Mutex);
      pthread_mutex_unlock(&m_Mutex);

      int nTmpPos=m_nFrontPos+1;
      if(nTmpPos >=m_nMax)
         nTmpPos = 0;
      memcpy((T)obj,m_QueueObjects[nTmpPos],sizeof(ARGTYPE));
      memset(m_QueueObjects[nTmpPos],0x00,sizeof(ARGTYPE));
      m_nFrontPos=nTmpPos;
	 #ifdef DEF_Q_LENGTH_BUG // cwkim GetLength Bug
	 m_clsCountLock.Lock();
	 if(m_unCnt > 0) --m_unCnt;
	 m_clsCountLock.Unlock();
    #endif
      m_GetCritSec.Unlock();
      return 1;
   }
   T Get()
  {
    if (!m_QueueObjects) return 0;
    m_GetCritSec.Lock();

    pthread_mutex_lock(&m_Mutex);
    while(m_nFrontPos==m_nRearPos)
      pthread_cond_wait(&m_Cond,&m_Mutex);
    pthread_mutex_unlock(&m_Mutex);

    int nTmpPos=m_nFrontPos+1;
    if(nTmpPos >=m_nMax)
      nTmpPos = 0;
    m_nFrontPos=nTmpPos;
	 #ifdef DEF_Q_LENGTH_BUG // cwkim GetLength Bug
	 m_clsCountLock.Lock();
    if(m_unCnt > 0) --m_unCnt;
	m_clsCountLock.Unlock();
    #endif
    m_GetCritSec.Unlock();
    return m_QueueObjects[nTmpPos];
  }

   int Get_(void *obj)
   {
      //int nTmpPos;
      if (!m_QueueObjects) return 0;
      if(m_nFrontPos==m_nRearPos)//Queue Empty
      {
         /*tv.tv_sec = (time_t) 0;
         tv.tv_nsec = (long) 1000000;
         nanosleep (&tv, &tv);*/
         return 0;
      }
      else
      {
         m_GetCritSec.Lock();
         int nTmpPos = m_nFrontPos +1;
         if(nTmpPos >= m_nMax)
            nTmpPos = 0;
         memcpy ((T)obj,m_QueueObjects[nTmpPos], sizeof(ARGTYPE));
         m_nFrontPos = nTmpPos;
    #ifdef DEF_Q_LENGTH_BUG  // cwkim GetLength Bug
	m_clsCountLock.Lock();
    if(m_unCnt > 0) --m_unCnt;
	m_clsCountLock.Unlock();
    #endif
         m_GetCritSec.Unlock();
         return 1;
      }
   }

   int Get_(void *obj, int nWaitTimeout) // in seconds
   {

      int nWaitTime;
      timeval tv;
      gettimeofday(&tv, NULL);

      nWaitTime = tv.tv_sec + nWaitTimeout;

      while (Get_(obj) == 0) {
         gettimeofday(&tv, NULL);
         if (nWaitTime < tv.tv_sec)
            return 0;
      }
      return 1;
   }
};

template <class T> class CQueue1To1
{
private:
   sem_t m_SemGet;
   CCritSec m_CritSec;
   int m_nMax;
   int m_nNextPut;
   int m_nNextGet;
   int m_nCurQueueLen;
   T *m_QueueObjects;

   void Initialize(int n)
   {
      m_nNextPut = m_nNextGet = m_nCurQueueLen = 0;
      m_nMax = n;

      sem_init(&m_SemGet, 1, 0);

      m_QueueObjects = new T[n];
   }

public:
   CQueue1To1() { Initialize(DEFAULT_QUEUESIZE); }
   CQueue1To1(int n) { Initialize(n); }
   ~CQueue1To1()
   {
      sem_destroy(&m_SemGet);
      delete [] m_QueueObjects;
   }
   void Put(T object)
   {
      m_CritSec.Lock();
      m_nCurQueueLen++;
      m_CritSec.Unlock();

      int mSlot = m_nNextPut++ % m_nMax;
      m_QueueObjects[mSlot] = object;

      sem_post(&m_SemGet);
   }
   T Get()
   {
      sem_wait(&m_SemGet);

      m_CritSec.Lock();
      m_nCurQueueLen--;
      m_CritSec.Unlock();
      int mSlot = m_nNextGet++ % m_nMax;
      T object = m_QueueObjects[mSlot];

      return object;
   }
   int Get_(T *obj)
   {
      if (sem_trywait(&m_SemGet) == 0) {
         m_CritSec.Lock();
         m_nCurQueueLen--;
         m_CritSec.Unlock();

         int mSlot = m_nNextGet++ % m_nMax;
         *obj = m_QueueObjects[mSlot];

         return 1;
      }
      else
         return 0;
   }
   int GetLength() { return m_nCurQueueLen; }
};

//////////////////////////////////////////////////////////////////////////////
class CThread
{
public:
   CThread();
   virtual ~CThread();

   static void *InitialThreadProc(void *pParam);

   int Create(int stack, int bDetached);
   int Create(int bDetached = 0) {return Create(0, bDetached); };
   virtual void Close();
   void Close_1();
   void Close_2();
   virtual bool IsCreated() { return m_bCreated == 1; }

   int ThreadExists();

protected:
   virtual void *ThreadProc() = 0;
   bool DoExit();
#ifdef SIGNAL_CONTROL
   void SigAllow(int sig);
   void SigPrevent(int sig);
   void SigAllowAll();
   void SigPreventAll();
#else
   void SigAllow(int sig){};
   void SigPrevent(int sig){};
   void SigAllowAll(){};
   void SigPreventAll(){};
#endif // SIGNAL_CONTROL

protected:
   pdk_pthread_t m_ThreadID;
   int m_bCreated;
   CSemaphore *m_pSemStop;
   CCritSec m_Lock;
};

inline int thread_to_core(int core_id) {                                                            
   int num_cores = sysconf(_SC_NPROCESSORS_ONLN);                                                       
   if (core_id >= num_cores)  // core_id = 0, 1, ... n-1 if system has n cores                          
      return -1;                                                                                    
                                                                                                        
   cpu_set_t cpuset;                                                                                    
   CPU_ZERO(&cpuset);                                                                                   
   CPU_SET(core_id, &cpuset);                                                                           
                                                                                                        
   pthread_t current_thread = pthread_self();                                                           
   return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);                           
}
}; //namespace RED


#endif /*__SYNC_CLASS_H__*/
