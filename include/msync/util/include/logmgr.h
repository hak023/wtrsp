
#ifndef _LOGMGR_H_
#define _LOGMGR_H_

#include <pthread.h>
#include "r_base.h"
#include "r_jthread.h"
#include "r_destroyer.h"
#include "gtticker.h"

#define MAX_LOG_SIZE (1024*8)
#define MAX_LOGBUF_SIZE MAX_LOG_SIZE * 50

#define PL_CRITICAL (CLogMgr::Critical)
#define PL_ERROR (CLogMgr::Error)
#define PL_WARN  (CLogMgr::Warn)
#define PL_INFO  (CLogMgr::Info)
#define PL_BAPI  (CLogMgr::Boardapi)
#define PL_API   (CLogMgr::Api)
#define PL_EVENT (CLogMgr::Event)
#define PL_RAW   (CLogMgr::RawData)
#define MOD_FS "[%-15s] "
#define _IF_LOGLV(pLogMgr,logLevel) if( pLogMgr->GetLogLevel() >= CLogMgr::logLevel ){
#define _IF_LV(logLevel) if(CLogMgr::GetInstance()->GetLogLevel() >= logLevel ){
#define _ELSE_LOGLV } else {
#define _ELSE_LV } else {
#define _ENDIF_LOGLV }
#define _ENDIF_LV }

#define LOG_ROOT_PATH               "../bin/log/"

#define USE_MUTEX
#ifndef USE_MUTEX
class CLock : public CSemaphore
{
public:
	CLock() : CSemaphore(1) {}
	void Lock() { Wait(); }
	void Unlock() { Signal(); }
};
#endif // USE_MUTEX

enum {
	LMSTS_W,
	LMSTS_R,
};

typedef struct cLogBuff{
	struct timeval tv;
	int status;
	int len;
	char* buf;
}cLogBuff;


//del yun
//class CLogMgr : public RED::CThread
class CLogMgr
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
	virtual ~CLogMgr();
   static CLogMgr *GetInstance(void);

	virtual int Log(const char *pFormat, ...);
   virtual int Log(LOGLEVEL l, const char *pFormat, ...);
   virtual int Log(RED::LPCSTR file, int line, LOGLEVEL l, const char *pFormat, ...);

   bool CreateLoggerFile(const char *pPrefix, int nQuata);
//@@ Add 08/01
   bool CreateLoggerFile(const char *pPrefix, const char *pPath, int nQuata=0);
//FishAdd 20080314
	bool CreateLoggerFile_Time(const char *pPrefix, int nQuata);
	bool CreateLoggerFile_Time(const char *pPrefix, const char *pPath, int nQuata=0);

   void SetLogLevel(LOGLEVEL l) { m_nLevel = l; }
   LOGLEVEL GetLogLevel() { return m_nLevel; }
   void SetFileLineLogLevel(LOGLEVEL l) { m_nFileLineLevel = l; }
	char* GetTimeStr(char * str,const char * format);
	void BWrite(char * buf,unsigned int len);
	void FWrite(int nIndex);
	void SetFlushMS(int flushms){m_nFlushMS = flushms;}

protected:
   CLogMgr();
   int MakeDate();
//FishAdd 20080314
	int MakeDate_Time();

	void* ThreadProc();
	cLogBuff* m_plogbuf;
	unsigned int m_nFlushMS;
	bool CheckQuata();
   static CLogMgr *m_pInstance;
#ifdef USE_MUTEX
   static RED::CCritSec m_Lock;
#else // USE_MUTEX
	static CLock m_Lock;
#endif // USE_MUTEX

   RED::CString m_strPrefix;
   RED::CString m_strRootPath;
	int     m_nQuata;
   RED::CString m_strLogger;
   RED::CFile m_File;

   // Last Creation Day
   int m_nDate; // yyyymmdd
   LOGLEVEL m_nLevel;
   LOGLEVEL m_nFileLineLevel;
   char m_pszLogLevel[MinLevel][20];
   pid_t m_ProcessId;
};

#ifdef LINUX
#define LOGMGR(v, ... ) CLogMgr::GetInstance()->Log(v, __VA_ARGS__)
#else
#define LOGMGR CLogMgr::GetInstance()->Log
#endif
#endif//_LOGMGR_H_
