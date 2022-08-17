/*

   HURIM Copy Right

	1. Declare Synchronize Class

	File : sharedmemory.h
	Create Date : 2004.

*/

#ifndef	__GSHM_CLASS_H__
#define	__GSHM_CLASS_H__

#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "base.h"
#include "eventdefs.h"
#include "jthread.h"
#include "logger.h" // bgchoi

#ifndef SHM_MODE
#	define SHM_MODE 0666
#	define FTOK_ID  'x'
#endif // SHM_MODE

#ifndef EVENT_QUEUE_LENGTH
#	define EVENT_QUEUE_LENGTH 56 // ideal
#endif // EVENT_QUEUE_LENGTH

void *CreateShm(const char *pszPath, int size, int fRdOnly = 0);
int DestroyShm(const char *pszPath);

template <class T> class CSharedMemory
{
protected:
	pthread_mutex_t *m_pMutexId;
	CString	m_strPath;
	int		m_nShmId;
	int		m_nSize;
	bool		m_bOrigin;
	T		  *m_pBuffer;

	int Initialize(int key)
	{
		if (m_nSize == -1)
			m_nSize = sizeof(pthread_mutex_t) + sizeof(T);
		else
			m_nSize += sizeof(pthread_mutex_t);
		if ((m_nShmId = shmget(key, m_nSize, 0)) < 0) {
			if (errno != ENOENT) {
				perror("shmget() failed");
				throw CException("shmget() failed %d:%s", errno, strerror(errno));
			}
			if ((m_nShmId = shmget(key,m_nSize,SHM_MODE|IPC_CREAT|IPC_EXCL)) < 0) {
				perror("shmget() failed");
				throw CException("shmget() failed");
			}
			m_bOrigin = false;
		} else {
			/* int nOld = nShmId; */
			if ((m_nShmId = shmget(key, m_nSize, SHM_MODE)) < 0) {
				perror("shmget() failed with old id");
				throw CException("shmget() failed with old id");
			}
			m_bOrigin = true;
		}
		Attach();
		return m_nShmId;
	}
	int CheckFile()
	{
		int nFile = -1;
		struct stat buf;
		if (stat((LPCSTR)m_strPath, &buf) != 0) {
			if ((nFile = open((LPCSTR)m_strPath, O_RDWR|O_CREAT, SHM_MODE)) == -1)
				perror("shm file open error");
			close(nFile);
		}
		return nFile;
	}

public:
	CSharedMemory(int key)
		: m_strPath(""), m_nShmId(-1),
		  m_nSize(-1), m_bOrigin(false), m_pBuffer(NULL)
	{
		Initialize(key);
	}
	CSharedMemory(int key, int nSize)
		: m_strPath(""), m_nShmId(-1),
		  m_nSize(nSize), m_bOrigin(false), m_pBuffer(NULL)
	{
		Initialize(key);
	}
	CSharedMemory(char *pszPath)
		: m_strPath(pszPath), m_nShmId(-1),
		  m_nSize(-1), m_bOrigin(false), m_pBuffer(NULL)
	{
		CheckFile();

		int key;
		if ((key = ftok(pszPath, FTOK_ID)) < 0)
			perror("ftok error");
		Initialize(key);
	}
	CSharedMemory(char *pszPath, int nSize)
		: m_strPath(pszPath), m_nShmId(-1),
		  m_nSize(nSize), m_bOrigin(false), m_pBuffer(NULL)
	{
		CheckFile();

		int key;
		if ((key = ftok(pszPath, FTOK_ID)) < 0)
			perror("ftok error");
		Initialize(key);
	}
	~CSharedMemory() { Unlock(); Detach(); }
	int Destroy()
	{
		pthread_mutex_destroy(m_pMutexId);
		if (Control(IPC_RMID, 0) < 0) {
			perror("shmctl IPC_RMID failed");
			return -1;
		}
		if (m_strPath.GetLength() > 1)
			unlink((LPCSTR)m_strPath);
		return 1;
	}
	LPCSTR GetPath() { return (LPCSTR)m_strPath; }
	int GetShmId() { return m_nShmId; }
	int GetSize() { return m_nSize; }
	bool GetOrigin() { return m_bOrigin; }
	T *Attach(int shmflg = 0)
	{
		if (m_pBuffer) return m_pBuffer;
		m_pMutexId = (pthread_mutex_t *)shmat(m_nShmId, 0, shmflg);
		if (m_pMutexId == (pthread_mutex_t *)-1)
			perror("CSharedMemory Attach");
		m_pBuffer = (T *)((PDK32U)m_pMutexId + sizeof(pthread_mutex_t));
		if (!m_bOrigin) {
			pthread_mutexattr_t m_MutexAttr;
			pthread_mutexattr_init(&m_MutexAttr);
			pthread_mutexattr_setpshared(&m_MutexAttr, PTHREAD_PROCESS_SHARED);
			if (pthread_mutex_init(m_pMutexId, &m_MutexAttr) != 0)
				perror("pthread_mutex_init error");
			pthread_mutexattr_destroy(&m_MutexAttr);
			memset(m_pBuffer, 0, sizeof(T));
		}
		return m_pBuffer;
	}
	int Detach() { m_pBuffer = NULL; return shmdt((char *)m_pMutexId); }
	T *Data() { return Attach(); }
	int Control(int cmd, struct shmid_ds *buf)
	{
		fprintf(stderr, "IPC_RMID[%d]\n", m_nShmId);
		return shmctl(m_nShmId, cmd, buf);
	}
	void Lock() { pthread_mutex_lock(m_pMutexId); }
	void Unlock() { pthread_mutex_unlock(m_pMutexId); }
};

/* type: T */
template <class T> class CShmQueue
{
protected:
	typedef struct {
		sem_t		m_SemPut;
		sem_t		m_SemGet;
		int		m_nMax;
		PDKUINT	m_nNextPut;
		PDKUINT	m_nNextGet;
		int		m_nCurQueueLen;
		char		m_szData;
	} TShmBuffer;

	int	  m_nKey;
	CString m_strPath;
	CSharedMemory<TShmBuffer> *m_pSharedMemory;
	TShmBuffer *m_pShmBuffer;
	T **m_QueueObjects;

	void Initialize(int n, bool bForce = false)
	{
		int nSize = sizeof(TShmBuffer) - sizeof(char) + sizeof(T) * n;
		if (m_nKey != -1)
			m_pSharedMemory = new CSharedMemory<TShmBuffer>(m_nKey, nSize);
		else
			m_pSharedMemory = new CSharedMemory<TShmBuffer>((LPSTR)m_strPath, nSize);
		m_pShmBuffer = m_pSharedMemory->Data();
		if (!m_pSharedMemory->GetOrigin() || bForce) {
			if (sem_init(&m_pShmBuffer->m_SemPut, 1, n) == -1)
				perror("sem_init error m_SemPut");
			if (sem_init(&m_pShmBuffer->m_SemGet, 1, 0) == -1)
				perror("sem_init error m_SemGet");
			m_pShmBuffer->m_nNextPut =
			m_pShmBuffer->m_nNextGet =
			m_pShmBuffer->m_nCurQueueLen = 0;
			m_pShmBuffer->m_nMax = n;
		}
		if (bForce) return;

		m_QueueObjects = new T*[n];
		for (int i = 0; i < n; i++) {
			m_QueueObjects[i] = (T *)(&m_pShmBuffer->m_szData + sizeof(T) * i);
		}
	}

public:
	CShmQueue(char *pszPath) : m_nKey(-1), m_strPath(pszPath)
		{ Initialize(EVENT_QUEUE_LENGTH); }
	CShmQueue(char *pszPath, int n) : m_nKey(-1), m_strPath(pszPath)
		{ Initialize(n); }
	CShmQueue(int nKey) : m_nKey(nKey), m_strPath("")
		{ Initialize(EVENT_QUEUE_LENGTH); }
	CShmQueue(int nKey, int n) : m_nKey(nKey), m_strPath("")
		{ Initialize(n); }
	~CShmQueue()
	{
      sem_post(&m_pShmBuffer->m_SemGet);
		delete [] m_QueueObjects;
      m_QueueObjects = NULL;
		delete m_pSharedMemory;
	}
	int Destroy() {
		sem_destroy(&m_pShmBuffer->m_SemPut);
		sem_destroy(&m_pShmBuffer->m_SemGet);
		return m_pSharedMemory->Destroy();
	}
	void Reset() { Initialize(m_pShmBuffer->m_nMax, true); }
	int GetKey() { return m_nKey; }
	bool PutEvent()
	{
		bool bRet = false;
		m_pSharedMemory->Lock();
		if (m_pShmBuffer->m_nCurQueueLen > 1)
			bRet = true;
		m_pSharedMemory->Unlock();
		sem_post(&m_pShmBuffer->m_SemGet);
		return bRet;
	}
	int Put(T *object, int nLen = 0)
	{
		if (sem_trywait(&m_pShmBuffer->m_SemPut) == 0) {
			m_pSharedMemory->Lock();
			int mSlot = m_pShmBuffer->m_nNextPut++ % m_pShmBuffer->m_nMax;
			m_pShmBuffer->m_nCurQueueLen++;
			m_pSharedMemory->Unlock();

			if (!nLen) nLen = sizeof(T);
			memcpy(m_QueueObjects[mSlot], object, nLen);

			sem_post(&m_pShmBuffer->m_SemGet);
			return 0;
		}
		else {
			return -1;
		}
	}
	T *Get()
	{
SIG_IGNORE:
		sem_wait(&m_pShmBuffer->m_SemGet);
		m_pSharedMemory->Lock();
		if (!m_pShmBuffer->m_nCurQueueLen) {
			m_pSharedMemory->Unlock();
			goto SIG_IGNORE;
		}
		m_pShmBuffer->m_nCurQueueLen--;
		int mSlot = m_pShmBuffer->m_nNextGet++ % m_pShmBuffer->m_nMax;
		m_pSharedMemory->Unlock();

      if (!m_QueueObjects) return NULL;

#if 0 // become an memory problem
		T *object = m_QueueObjects[mSlot];
#else // malloc at here, must be deleted
		T *object = new T;
		memcpy(object, m_QueueObjects[mSlot], sizeof(T));
#endif

		sem_post(&m_pShmBuffer->m_SemPut);
		return object;
	}
	int Get_(T **obj)
	{
		if (sem_trywait(&m_pShmBuffer->m_SemGet) == 0) {
			m_pSharedMemory->Lock();
			m_pShmBuffer->m_nCurQueueLen--;
			int mSlot = m_pShmBuffer->m_nNextGet++ % m_pShmBuffer->m_nMax;
			m_pSharedMemory->Unlock();

#if 0 // become an memory problem
			*obj = m_QueueObjects[mSlot];
#else // malloc at here, must be deleted
			*obj = new T;
			memcpy(*obj, m_QueueObjects[mSlot], sizeof(T));
#endif
			sem_post(&m_pShmBuffer->m_SemPut);

			return 1;
		}
		else
			return 0;
	}
	int Get_(T **obj, int nWaitTimeout) // in seconds
	{
		int nWaitTime;
		timeval tv;

		gettimeofday(&tv, NULL);

		nWaitTime = tv.tv_sec + nWaitTimeout;

		while (Get_(obj) == 0) {
			timespec req;
			req.tv_sec = 0;
			req.tv_nsec = 1000 * 1000;
			nanosleep(&req, NULL);
			gettimeofday(&tv, NULL);
			if (nWaitTime < tv.tv_sec)
				return 0;
		}
		return 1;
	}
	int Peek(char *pValue, int len, int offset)
	{
		if (m_pShmBuffer->m_nCurQueueLen > 0) {
			m_pSharedMemory->Lock();

			int bOk = 0;
			int mSlot;
			char *pSrc;
			for (int i = 0; i < m_pShmBuffer->m_nCurQueueLen; i++) {
				mSlot = (m_pShmBuffer->m_nNextGet + i) % m_pShmBuffer->m_nMax;
				pSrc = (char *)m_QueueObjects[mSlot];
				if (memcmp(pSrc + offset, pValue, len) == 0) {
					bOk = 1;
					break;
				}
			}

			m_pSharedMemory->Unlock();
			return bOk;
		}
		else
			return 0;
	}
	int GetLength() { return m_pShmBuffer->m_nCurQueueLen; }

};

template <class T>
class CShmQInterface
{
public:
	CShmQInterface(char *pPath) :
		m_pQueue(new CShmQueue<T>(pPath, EVENT_QUEUE_LENGTH)) { }
	CShmQInterface(char *pPath, int nBufferLen) :
		m_pQueue(new CShmQueue<T>(pPath, nBufferLen)) { }
	CShmQInterface(int nKey) :
		m_pQueue(new CShmQueue<T>(nKey, EVENT_QUEUE_LENGTH)) { }
	CShmQInterface(int nKey, int nBufferLen) :
		m_pQueue(new CShmQueue<T>(nKey, nBufferLen)) { }
	virtual ~CShmQInterface() { if (m_pQueue) delete m_pQueue; }
	int Destroy() { return m_pQueue->Destroy(); }
   virtual bool Add(T *pEvent, PDKINT nLen = 0)
		{ return m_pQueue->Put(pEvent, nLen) == 0; }
	virtual T *Remove() { return m_pQueue->Get(); }
	virtual T *Remove_();
   virtual bool AddEvent(PDK16U type, PDK16U stype, char *pValue = NULL, int nLen = 0);
	virtual bool AddEvent(PDK16U type, PDK16U stype, PDK32U value);
	int GetLength() { return m_pQueue->GetLength(); }
	int GetKey() { return m_pQueue->GetKey(); }

protected:
	virtual bool AddQuitEvent();

	CShmQueue<T>	*m_pQueue;
};

template <class T>
inline bool CShmQInterface<T>::AddQuitEvent()
	{ return AddEvent(CI_QUIT_THREAD, 0); }

template <class T>
inline T *CShmQInterface<T>::Remove_()
{
	T *pEvent;
	if (m_pQueue->Get_(&pEvent) == 1)
		return pEvent;
	else
		return NULL;
}

template <class T>
inline bool CShmQInterface<T>::AddEvent(PDK16U type, PDK16U stype, char *pValue, int nLen)
{
#if 0
   T Event;
   T *pEvent = &Event;
	memset(pEvent, 0, sizeof(T));
   EVENTINFO_TYPE(pEvent) = type;
   EVENTINFO_SUBTYPE(pEvent) = stype;
   if (pValue != NULL && nLen > 0)
      memcpy(pEvent->Value, pValue, nLen);
   EVENTINFO_LENGTH(pEvent) = EVENTINFO_HEADER_LENGTH + nLen;
   return Add(pEvent);
#else
	return true;
#endif
}

template <class T>
inline bool CShmQInterface<T>::AddEvent(PDK16U type, PDK16U stype, PDK32U value)
{
#if 0
   T Event;
   T *pEvent = &Event;
	memset(pEvent, 0, sizeof(EVENTINFO));
   EVENTINFO_TYPE(pEvent) = type;
   EVENTINFO_SUBTYPE(pEvent) = stype;
   *(PDK32U *)(&pEvent->Value[0]) = value;
   EVENTINFO_LENGTH(pEvent) = EVENTINFO_HEADER_LENGTH + sizeof(PDK32U);
   return Add(pEvent);
#else
	return true;
#endif
}

////////////////////////////////////////////////////////////////////////////////
template <class T>
class CShmQThread : public CShmQInterface<T>, public CThread
{
private:
	class CCongestThread : public CThread
	{
	public:
		CCongestThread(CShmQueue<T> *pQueue) : m_pQueue(pQueue) { }
	protected:
		CShmQueue<T>	*m_pQueue;
		void *ThreadProc() {
			while (!DoExit()) {
				if (!m_pQueue->PutEvent()) break;
				usleep(1);
			}
			return NULL;
		}
	};
	CCongestThread *m_pCongThread;
public:
	CShmQThread(char *pPath) : CShmQInterface<T>(pPath), CThread() { }
	CShmQThread(char *pPath, int nBufferLen) :
		CShmQInterface<T>(pPath, nBufferLen), CThread() { }
	CShmQThread(int nKey) : CShmQInterface<T>(nKey), CThread() { }
	CShmQThread(int nKey, int nBufferLen) :
		CShmQInterface<T>(nKey, nBufferLen), CThread() { }
	virtual ~CShmQThread() { Stop(); }
   virtual bool Start() {
		m_pCongThread = NULL;
		if (CShmQInterface<T>::m_pQueue->GetLength()) {
			m_pCongThread = new CCongestThread(CShmQInterface<T>::m_pQueue);
			m_pCongThread->Create();
		}
		return CThread::Create() == 1;
	}
	virtual void Stop() {
		if (m_pCongThread) {
			delete m_pCongThread;
			m_pCongThread = NULL;
		}
		if (CThread::IsCreated() == false)
			return;
		Close();
	}
	virtual void Close() {
		if (CThread::IsCreated() == false)
			return;
		CThread::Close_1();
		CShmQInterface<T>::AddQuitEvent();
		CThread::Close_2();
	}
};

template <class T> class CTrace
{
protected:
	T				m_Variable;
public:
	typedef void (*PCallBack)(T &);
	CTrace() : OnTrace(NULL) { }
	CTrace(PCallBack OnEvent) : OnTrace(OnEvent) { }
	~CTrace() { }
	CTrace<T> &operator=(T v) {
		m_Variable = v;
		if (OnTrace) OnTrace(m_Variable);
		return *this;
	}
	CTrace<T> &operator+=(T v) {
		m_Variable += v;
		if (OnTrace) OnTrace(m_Variable);
		return *this;
	}
	operator T() { return m_Variable; }
	T operator++() { return m_Variable++; }
	T operator--() { return m_Variable--; }
	PCallBack	OnTrace;
};

template <class T> class CShmTrace : public CThread
{
protected:
	typedef struct {
		sem_t		m_Semaphore;
		T			m_tData;
	} TShmBuffer;

	int	  m_nKey;
	CString m_strPath;
	CSharedMemory<TShmBuffer> *m_pSharedMemory;
	TShmBuffer *m_pShmBuffer;
	T			  *m_pVariable;

	void Initialize()
	{
		if (m_nKey != -1)
			m_pSharedMemory = new CSharedMemory<TShmBuffer>(m_nKey);
		else
			m_pSharedMemory = new CSharedMemory<TShmBuffer>((LPSTR)m_strPath);
		m_pShmBuffer = m_pSharedMemory->Data();
		if (!m_pSharedMemory->GetOrigin()) {
			if (sem_init(&m_pShmBuffer->m_Semaphore, 1, 0) == -1)
				perror("sem_init error m_Semaphore");
		}

		m_pVariable = &m_pShmBuffer->m_tData;
	}
	void *ThreadProc() {
		while (!DoExit()) {
			sem_wait(&m_pShmBuffer->m_Semaphore);
			if (OnTrace)
				OnTrace(*m_pVariable);
		}
		return NULL;
	}
public:
	typedef void (*PCallBack)(T &);
	CShmTrace(char *pszPath)
 		: m_nKey(-1), m_strPath(pszPath), OnTrace(NULL)
	{
		Initialize();
	}
	CShmTrace(char *pszPath, PCallBack OnEvent)
		: m_nKey(-1), m_strPath(pszPath), OnTrace(OnEvent)
	{
		Initialize();
	}
	CShmTrace(int nKey)
		: m_nKey(nKey), m_strPath(""), OnTrace(NULL)
	{
		Initialize();
	}
	CShmTrace(int nKey, PCallBack OnEvent)
		: m_nKey(nKey), m_strPath(""), OnTrace(OnEvent)
	{
		Initialize();
	}
	~CShmTrace() {
		delete m_pSharedMemory;
	}
	int Destroy() {
		sem_destroy(&m_pShmBuffer->m_Semaphore);
		return m_pSharedMemory->Destroy();
	}
	virtual bool Start() { return Create() == 1; }
	virtual void Stop() { Close(); }

	CShmTrace<T> &operator=(T v) {
		*m_pVariable = v;
		sem_post(&m_pShmBuffer->m_Semaphore);
		return *this;
	}
	CShmTrace<T> &operator+=(T v) {
		*m_pVariable += v;
		sem_post(&m_pShmBuffer->m_Semaphore);
		return *this;
	}
	operator T() { return *m_pVariable; }
	T operator++() {
		*m_pVariable++;
		sem_post(&m_pShmBuffer->m_Semaphore);
		return *m_pVariable;
	}
	T operator--() {
		*m_pVariable--;
		sem_post(&m_pShmBuffer->m_Semaphore);
		return *m_pVariable;
	}
	PCallBack	OnTrace;
};
//////////////////////////////////////////////////////////////////////////////
#endif //__GSHM_CLASS_H__
