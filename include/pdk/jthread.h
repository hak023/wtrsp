/*

   HURIM Copy Right

	1. Declare Synchronize Class

	File : jthread.h
	Create Date : 2001. by Jeong
	Updated : 2002.1 by Windjun
             
*/

#ifndef	__SYNC_CLASS_H__
#define	__SYNC_CLASS_H__

#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include "base.h"

#define SIGNAL_CONTROL
#ifdef SIGNAL_CONTROL
#	include <signal.h>
#endif

#define	MAX_TIMEINTERVAL		((long)0x7fffffff)
//FishDell-2007.08.02 #define CI_QUIT_THREAD 0xf004

//////////////////////////////////////////////////////////////////////////////
class CCritSec
{
public:
	CCritSec()
	{
		pthread_mutexattr_t m_MutexAttr;
		pthread_mutexattr_init(&m_MutexAttr);
		pthread_mutexattr_setpshared(&m_MutexAttr, PTHREAD_PROCESS_PRIVATE);
		pthread_mutex_init(&m_Mutex, &m_MutexAttr);
		pthread_mutexattr_destroy(&m_MutexAttr);
	}
	~CCritSec() { pthread_mutex_destroy(&m_Mutex); }
	void Lock() { pthread_mutex_lock(&m_Mutex); }
	void Unlock() { pthread_mutex_unlock(&m_Mutex); }

protected:
	pthread_mutex_t m_Mutex;
};

//////////////////////////////////////////////////////////////////////////////
class CAutoLock
{
public:
	CAutoLock(CCritSec *pLock) : m_pLock(pLock) { m_pLock->Lock(); }
	~CAutoLock() { m_pLock->Unlock(); }

protected:
	CCritSec *m_pLock;
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

//////////////////////////////////////////////////////////////////////////////
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

#define	DEFAULT_QUEUESIZE	2

template <class T, class ARGTYPE> class CQueue
{
private:
  CCritSec m_PutCritSec;
   //CSemaphore m_PutCritSec;
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
   //pthread_mutex_t m_PutLock;
   //pthread_mutex_t m_GetLock;

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
    //pthread_mutex_destroy(&m_PutLock);
    //pthread_mutex_destroy(&m_GetLock);
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

#if 0
      if(nLength>0)
      {
         nLength = nLength + (4-nLength%4);
      memcpy (m_QueueObjects[nTmpPos], object, nLength);
      }
      else
      {
         memcpy (m_QueueObjects[nTmpPos], object, sizeof(ARGTYPE));
      }
#endif
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
class CThread : public CObject
{
   DECLARE_DYNAMIC(CThread)

public:
	CThread();
	virtual ~CThread();

	static void *InitialThreadProc(void *pParam);

	int Create(int stack, int bDetached);
	int Create(int bDetached = 0);
	virtual void Close();
	void Close_1();
	void Close_2();
	virtual bool IsCreated() { return m_bCreated; }

	int ThreadExists();

protected:
	virtual void *ThreadProc() = 0;
	bool DoExit();
#ifdef SIGNAL_CONTROL
	void SigAllow(int sig);
	void SigPrevent(int sig);
	void SigAllowAll();
	void SigPreventAll();
#endif // SIGNAL_CONTROL

protected:
	pthread_t m_ThreadID;
	int m_bCreated;
	sem_t m_SemStop;
	CCritSec m_Lock;
};

#ifdef SIGNAL_CONTROL
inline void CThread::SigAllow(int sig)
{
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, sig);
#ifdef SIGPROCMASK
	sigprocmask(SIG_UNBLOCK, &sigset, NULL);
#else // SIGPROCMASK
	if (pthread_sigmask(SIG_BLOCK, &sigset, NULL))
		perror("Signal Allow");
#endif // SIGPROCMASK
}

inline void CThread::SigPrevent(int sig)
{
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, sig);
#ifdef SIGPROCMASK
	sigprocmask(SIG_BLOCK, &sigset, NULL);
#else // SIGPROCMASK
	if (pthread_sigmask(SIG_BLOCK, &sigset, NULL))
		perror("Signal Prevent");
#endif // SIGPROCMASK
}
inline void CThread::SigAllowAll()
{
	sigset_t sigset;
	sigfillset(&sigset);
#ifdef SIGPROCMASK
	sigprocmask(SIG_UNBLOCK, &sigset, NULL);
#else // SIGPROCMASK
	if (pthread_sigmask(SIG_BLOCK, &sigset, NULL))
		perror("Signal Allow All");
#endif // SIGPROCMASK
}
inline void CThread::SigPreventAll()
{
	sigset_t sigset;
	sigfillset(&sigset);
	sigdelset(&sigset, SIGSEGV); // don't block abort
	sigdelset(&sigset, SIGABRT); // don't block segfault
#ifdef SIGPROCMASK
	sigprocmask(SIG_BLOCK, &sigset, NULL);
#else // SIGPROCMASK
	if (pthread_sigmask(SIG_UNBLOCK, &sigset, NULL))
		perror("Signal Prevent All");
#endif // SIGPROCMASK
}
#endif // SIGNAL_CONTROL
//////////////////////////////////////////////////////////////////////////////
#endif //__SYNC_CLASS_H__
