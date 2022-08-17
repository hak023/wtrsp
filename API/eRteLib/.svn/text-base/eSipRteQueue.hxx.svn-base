/**
 * @file  eSipRteQueue.hxx
 * the sip route queue
 *
 * @attention Copyright (c) 2014 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2014.11.12   hc_yun   1.0     first written from by pdk-3.2.0 jthread.h
 */


#ifndef	__ESIP_RTE_JTREAD_H__
#define  __ESIP_RTE_JTREAD_H__

#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include "rutil/RWMutex.hxx"
#include <signal.h>

namespace eSIP
{

//////////////////////////////////////////////////////////////////////////////
class CCritSec
{
public:
	CCritSec(){ } 
   ~CCritSec() { }
	void Lock() { m_Mutex.lock();}
	void Unlock() { m_Mutex.unlock(); }

protected:
	resip::RWMutex m_Mutex;
};

//////////////////////////////////////////////////////////////////////////////
class CSemaphore
{
public:
	CSemaphore(unsigned value = 1);
	virtual ~CSemaphore();

	bool ReCreateSemaphore(unsigned value = 1);

	int Wait() { return sem_wait(&m_Semaphore); }
	int Wait(long msecs); // in MilliSeconds

	void Signal() { sem_post(&m_Semaphore); }

	int WillBlock() const { return sem_trywait((sem_t *)&m_Semaphore) != 0; }

	bool Unlock();
	bool Unlock(long lCount, int* lprevCount = NULL);
	bool Lock();
   bool Lock(unsigned int dwTimeout);

protected:
   bool CreateObject(unsigned value);

	sem_t m_Semaphore;
   bool  m_bCreate;
};


#define	ESIP_RTE_DEFAULT_QUEUESIZE	  (2)

template <class T, class ARGTYPE> class CQueue
{
private:
  CCritSec m_PutCritSec;
  CCritSec m_GetCritSec;
  ushort m_nMax;
  ushort m_nFrontPos;
  ushort m_nRearPos;
  T *m_QueueObjects;
  ARGTYPE *m_pBuffer;
  struct timespec tv;
  pthread_mutex_t m_Mutex;
  pthread_cond_t m_Cond;
  struct timeval m_val;

public:


  void Initialize(int n)
  {
      m_nFrontPos = m_nRearPos  = 0;
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
      pthread_mutexattr_destroy(&m_MutexAttr);
      m_val.tv_sec = 0;
      m_val.tv_usec = 10000;
  }

  CQueue() {Initialize(ESIP_RTE_DEFAULT_QUEUESIZE);}
  CQueue(int n) {Initialize(n); }
  ~CQueue()
  {
    Free();
  }
  void Free()
  {
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
     if(m_nRearPos >= m_nFrontPos)
        return m_nRearPos - m_nFrontPos;
     else
        return m_nMax - (m_nFrontPos-m_nRearPos)+1;
  }


  int Put(T object,int nLength=0)
  {
  int nTmpPos;

   m_PutCritSec.Lock();
   //pthread_mutex_lock(&m_PutLock);

  for(int nIndex = 0; ; nIndex ++)  {
    nTmpPos = m_nRearPos + 1;
    if(nTmpPos >= m_nMax)
      nTmpPos = 0;
    if(nTmpPos == m_nFrontPos)  {// Queue FULL
      if ( nIndex == 3) {// retry for 3 times
        m_PutCritSec.Unlock();
            //pthread_mutex_unlock(&m_PutLock);
        return -1;//Queue FULL, discard DATA
      }
      else  {
        //tv.tv_sec = (time_t) 0;
        //tv.tv_nsec = (long) 10000000;
          //nanosleep (&tv, &tv);
         select(0, 0, 0, 0, &m_val);
        continue;
      }
    }
    else break;
  }// end of for

    if(nLength>0)
       memcpy (m_QueueObjects[nTmpPos], object, nLength);
    else
       memcpy (m_QueueObjects[nTmpPos], object, sizeof(ARGTYPE));

    m_nRearPos = nTmpPos;

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
      //pthread_mutex_lock(&m_GetLock);
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

    m_GetCritSec.Unlock();
      //pthread_mutex_unlock(&m_GetLock);
    return 1;
  }
  T Get()
  {
    if (!m_QueueObjects) return 0;
    m_GetCritSec.Lock();
      //pthread_mutex_lock(&m_GetLock);

    pthread_mutex_lock(&m_Mutex);
    while(m_nFrontPos==m_nRearPos)
      pthread_cond_wait(&m_Cond,&m_Mutex);
    pthread_mutex_unlock(&m_Mutex);

    int nTmpPos=m_nFrontPos+1;
    if(nTmpPos >=m_nMax)
      nTmpPos = 0;
    m_nFrontPos=nTmpPos;
    m_GetCritSec.Unlock();
      //pthread_mutex_unlock(&m_GetLock);
    return m_QueueObjects[nTmpPos];
  }

   int Get_(void *obj)
  {
    //int nTmpPos;
    if (!m_QueueObjects) return 0;
    if(m_nFrontPos==m_nRearPos)//Queue Empty
    {
      tv.tv_sec = (time_t) 0;
      tv.tv_nsec = (long) 20000;
         nanosleep (&tv, &tv);
         return 0;
    }
    else
    {
      m_GetCritSec.Lock();
         //pthread_mutex_lock(&m_GetLock);
      int nTmpPos = m_nFrontPos +1;
      if(nTmpPos >= m_nMax)
        nTmpPos = 0;
      memcpy ((T)obj,m_QueueObjects[nTmpPos], sizeof(ARGTYPE));
      m_nFrontPos = nTmpPos;
     m_GetCritSec.Unlock();
         //pthread_mutex_unlock(&m_GetLock);
      return 1;
    }
  }

};


};//end namespace

#endif //__SYNC_CLASS_H__
