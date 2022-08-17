/*
   HURIM Copy Right

   File : util.h

   1. Declare utility 

   Create Date : 2001.  by Jeong
   Updated     : 2002.1 by WindJun - VMS
*/

#ifndef	__UTIL_H__
#define	__UTIL_H__

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#include "list.h"
//#include "jthread.h"
#include "sock.h"

//@@ Add 02/15/02
//////////////////////////////////////////////////////////////////////////////
void SplitPath(const char *pname, CString &head, CString &tail,
			   const char spliter = '/', int order = 0);
bool IsDir(const char *pname);
void MakeDirs(const char *pname, mode_t mode = 0777);
void GetHostIPAddr(CString &strHostIP);
LPCSTR GetHostIPAddr();
void Tokenize(LPSTR pszLine, CArray<CString,CString> &value, LPSTR delim=" \t");

//////////////////////////////////////////////////////////////////////////////
void MicroSleep(int usec);
void MilliSleep(int msec);
void Sleep(int sec);
int kbhit();

time_t GetCurrentTime();
int    GetCurrentTimeInSec();
int    PingCheck(unsigned long nAddr);

//////////////////////////////////////////////////////////////////////////////
typedef	struct _mem_item {
	int nLength;
	char *pBuffer;
	_mem_item *pNext;
} MEMITEM, *PMEMITEM;

//////////////////////////////////////////////////////////////////////////////
class CMemList
{
public:
	CMemList(int nItems = 2);
	~CMemList();

	void SetItems(int nItems);
	void Reset();
	void Add(int nLength, char *pBuffer);

	void SetSerialize();
	PMEMITEM Get();

protected:
	void FreeAllItems();

protected:
	int m_nItems;
	int m_nCurItems;
	PMEMITEM m_pHead;
	PMEMITEM m_pTail;
	PMEMITEM m_pCur;
};

//////////////////////////////////////////////////////////////////////////////
#define  QNODE_FREE			      0
#define  QNODE_USE			      1

#define  QNODE_SUCCESS		      0
#define  QNODE_BUFFER_FULL	      -1
#define  QNODE_BUFFER_EMPTY	   -2

//////////////////////////////////////////////////////////////////////////////
// CCircularQueue
template <class TYPE, int MAX> class CCircularQueue
{
protected:
	struct QNode {
		TYPE	m_Data;
		bool	m_bStatus; //0, 1
	};

	QNode	               m_QNode[MAX];
	unsigned int	      m_nFront;
	unsigned int	      m_nRear;
	int		            m_nMax;

	CSemaphore	         m_SemPut;
	CSemaphore	         m_SemGet;
	CCritSec 	         m_CritSec;

	void	Initialize();

public:
	CCircularQueue() { Initialize(); }
	virtual ~CCircularQueue()  { Unlockq(); }

	void Unlockq() {
			m_SemPut.Unlock();
			m_SemGet.Unlock();
			//m_CritSec.Unlock();
	}

	void Reset();

	int Putq(TYPE* data);
	int Getq(TYPE* pType);

	void GetQPos(unsigned int& nFront, unsigned int& nRear) {
		nFront = m_nFront;
		nRear  = m_nRear;
	}
	int  GetQLength() { return m_nMax; }
	bool IsEmpty() { return (m_nFront == m_nRear); } //Buffer Empty
};

template<class TYPE, int MAX>
inline void CCircularQueue<TYPE, MAX>::Reset()
{
	Initialize();
}

template<class TYPE, int MAX>
inline void CCircularQueue<TYPE, MAX>::Initialize()
{
   m_nMax = MAX;
   for (int i=0; i<MAX; i++) {
      memset(&m_QNode[i].m_Data, 0, sizeof(TYPE));
      m_QNode[i].m_bStatus = QNODE_FREE;
   }
   m_nFront = m_nRear = 1;

   m_SemPut.ReCreateSemaphore(MAX);
}

template<class TYPE, int MAX>
inline int CCircularQueue<TYPE, MAX>::Putq(TYPE* data)
{
   m_SemPut.Lock();

   m_CritSec.Lock();
   //Addq (circular queue).................................................
   m_nRear = (m_nRear+1) % MAX;
   if (m_nFront == m_nRear) { //Buffer Full
      m_nRear = (m_nRear-1) % MAX;
      m_CritSec.Unlock();
      return QNODE_BUFFER_FULL;
   }
   int nIndex = m_nRear;
   m_CritSec.Unlock();

   memcpy(&m_QNode[nIndex].m_Data, data, sizeof(TYPE));
   m_QNode[nIndex].m_bStatus = QNODE_USE;

	m_SemGet.Unlock();
   return QNODE_SUCCESS;
}

template<class TYPE, int MAX>
inline int CCircularQueue<TYPE, MAX>::Getq(TYPE* pType)
{
   m_SemGet.Lock();

   m_CritSec.Lock();
   //Delq (circular queue)......................................................
   if (m_nFront == m_nRear) { //Buffer Empty
      m_CritSec.Unlock();
      return QNODE_BUFFER_EMPTY;
   }
   m_nFront = (m_nFront+1) % MAX;
   int nIndex = m_nFront;
   m_CritSec.Unlock();

   //pType = &m_QNode[nIndex].m_Data; //remove 2001/10/16
   memcpy(pType, &m_QNode[nIndex].m_Data, sizeof(TYPE)); //add 2001/10/16

   pType = &m_QNode[nIndex].m_Data;
   memset(&m_QNode[nIndex].m_Data, 0, sizeof(TYPE));
   m_QNode[nIndex].m_bStatus = QNODE_FREE;

	m_SemPut.Unlock();
   return QNODE_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
#include "list.h"
#include "jthread.h"
#include "destroyer.h"
//FishDell-2007.08.02 #define CI_TIMER_DONE	0xFFFF
#define DIFFTIME(tvp, uvp) \
	abs((tvp.tv_sec  - uvp.tv_sec) * 1000 + \
	    (tvp.tv_usec - uvp.tv_usec) / 1000)
#define DIFFTIME1(tvp, uvp) abs(tvp.tv_sec - uvp.tv_sec)

class CQThread;
class CGTimer : public CThread
{
	DECLARE_DYNAMIC(CGTimer)

public:
	CGTimer();
	~CGTimer();
	static CGTimer *GetInstance();
	bool StartTimer(CQThread *obj, int msecs, PDK32U nWhich = 0, PDK8U nSize=2);
	bool StopTimer(CQThread *obj, PDK32U nWhich = 0);
	bool AddTicker(CQThread *obj, int msecs, PDK32U nWhich = 0, PDK8U nSize=2);
	bool RemoveTicker(CQThread *obj, PDK32U nWhich = 0);

protected:
	typedef struct {
		struct timeval tv;
		PDKINT nTerm;
		bool 	 bContinuous;
		PDK32U nWhich;
		PDK8U  nSize;
		CQThread *obj;
		POSITION pos;
	} TimeSpec, *PTimeSpec;

	void *ThreadProc();
	PTimeSpec AddElement(CQThread *obj, int msecs, PDK32U nWhich = 0, PDK8U nSize=2);
	bool RemoveElement(CQThread *obj, PDK32U nWhich = 0);

	static bool		 m_bInitialized;
	static CGTimer *m_pInstance;
	static CCritSec m_Lock;
	CMapPtrToPtr 	*m_pTimerMap;
	CTypedPtrList<CPtrList, PTimeSpec> m_TimerList;

	CLogger			*m_pLogger;
};
bool StartTimer(CQThread *obj, int msecs, PDK32U nWhich = 0, PDK8U nSize=2);
bool StopTimer(CQThread *obj, PDK32U nWhich = 0);
bool AddTicker(CQThread *obj, int msecs, PDK32U nWhich = 0, PDK8U nSize=2);
bool RemoveTicker(CQThread *obj, PDK32U nWhich = 0);
void StopGTimer();
void ToLower(LPCSTR pszstr);
void ToUpper(LPCSTR pszstr);
LPSTR MemSeek(LPSTR pszBuffer, PDKINT nBuffSize,
	LPSTR pszNeedle, PDKINT nLen, PDK8U chLastMask = 0);
bool IsDigit(char *str);
int axtoi(LPCSTR pszHex);

//////////////////////////////////////////////////////////////////////////////
#endif //__UTIL_H__
