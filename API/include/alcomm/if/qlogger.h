
#ifndef _QLOGGER_H_
#define _QLOGGER_H_

#include <pthread.h>
#include "logger.h"
#include "eventdefs.h"
#include "qthread.h"

#define LOG_ROOT_PATH               "../bin/log/"
#define MAX_LOG_SIZE (1024*8)

typedef enum 
{
	E_LOGQ_QUIT = 0x2000,
	E_LOGQ_WRITE = 0x2001,
} E_LOGQTYPE;

typedef struct
{
	int nType;
	int nBodyLen;
	char szMsg[MAX_LOG_SIZE];
} LogMsgQueue;

typedef CQueue<LogMsgQueue*, LogMsgQueue> CLogQueue;

class CLogQThread;

class CQLogger
{
public:
   enum LOGLEVEL {
		Critical = 0,
		Error,
		Warn,
		Info,
		Api,
		Boardapi,
		Event,
		RawData,
		MaxLevel,
		MinLevel,      // 9
	};

public:
	CQLogger();
	~CQLogger();
   static CQLogger *GetInstance(void);

	bool Start(int nThreadCnt=1, int nQueueSize=1024);

	int Log(const char *pFormat, ...);
   int Log(LOGLEVEL l, const char *pFormat, ...);
   int Log(LPCSTR file, int line, LOGLEVEL l, const char *pFormat, ...);

	int WriteLog(const char *pLog, int nLen);

   bool CreateLoggerFile(const char *pPrefix, int nQuata);
//@@ Add 08/01
   bool CreateLoggerFile(const char *pPrefix, const char *pPath, int nQuata=0);

   void SetLogLevel(LOGLEVEL l) { m_nLevel = l; }
   LOGLEVEL GetLogLevel() { return m_nLevel; }
   void SetFileLineLogLevel(LOGLEVEL l) { m_nFileLineLevel = l; }

protected:
   int MakeDate();
	bool AddQueue(char *pData, int nLen);
	int GetHashKey(unsigned long key);

protected:
	bool CheckQuata();
   static CQLogger *m_pInstance;
#ifdef USE_MUTEX
   static CCritSec m_Lock;
#else // USE_MUTEX
	static CLock m_Lock;
#endif // USE_MUTEX

   CString m_strPrefix;
   CString m_strRootPath;
	int     m_nQuata;
   CString m_strLogger;
   CFile m_File;

   // Last Creation Day
   int m_nDate; // yyyymmdd
   LOGLEVEL m_nLevel;
   LOGLEVEL m_nFileLineLevel;
   char m_pszLogLevel[MinLevel][20];
   pid_t m_ProcessId;

	// Queue
	int m_nQueueSize;
	int m_nThreadCnt;
	CLogQThread *m_pQThread;
};

#define QLOGGER(v...)  CQLogger::GetInstance()->Log(v)

class CLogQThread : public CThread
{
	DECLARE_DYNAMIC(CLogQThread);
public:
	CLogQThread();
	~CLogQThread();
	
	bool Start(CQLogger *pLogger, int nIndex = 1, int nQueueSize = 1024);
	bool AddQueue(char *pData, int nLen);

protected:
	void *ThreadProc();

	int m_nIndex;
	int m_nReserved;

	CQLogger *m_pLogger;
	CLogQueue *m_pQueue;
#ifdef USE_MUTEX
   CCritSec m_Lock;
#else // USE_MUTEX
	CLock m_Lock;
#endif // USE_MUTEX

};

class CTimeElapse3 {
public:
   CTimeElapse3() {};
   ~CTimeElapse3() {};
   void Start() {
      gettimeofday(&t0, NULL);
      ta = 0;
   };
   int End() {
      gettimeofday(&t1, NULL);
      int us = (t1.tv_sec - t0.tv_sec)*1000000 + (t1.tv_usec - t0.tv_usec);
      return us;
   };

   void BeginAccm() {
      gettimeofday(&t0, NULL);
   };
   int EndAccm() {
      gettimeofday(&t1, NULL);
      ta += (t1.tv_sec - t0.tv_sec)*1000000 + (t1.tv_usec - t0.tv_usec);
      return ta;
   };
   int GetAccm() {
      return ta;
   }

protected:
   struct timeval t0;
   struct timeval t1;
   int    ta;
};

#endif
