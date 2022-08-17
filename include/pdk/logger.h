
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <pthread.h>
#include "base.h"
#include "jthread.h"
#include "destroyer.h"

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

class CLogger
{
public:
   enum LOGLEVEL {
#if 0
      Critical = 0,
      Error,
      Warn,
      Info,
      Boardapi,
      Api,
      Event,
      MaxLevel,
#else
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
#endif
   };

public:
	virtual ~CLogger() { }
   static CLogger *GetInstance(void);

	virtual int Log(const char *pFormat, ...);
   virtual int Log(LOGLEVEL l, const char *pFormat, ...);
   virtual int Log(LPCSTR file, int line, LOGLEVEL l, const char *pFormat, ...);

   bool CreateLoggerFile(const char *pPrefix, int nQuata);
//@@ Add 08/01
   bool CreateLoggerFile(const char *pPrefix, const char *pPath, int nQuata=0);
//FishAdd 20080314
	bool CreateLoggerFile_Time(const char *pPrefix, int nQuata);
	bool CreateLoggerFile_Time(const char *pPrefix, const char *pPath, int nQuata=0);

   void SetLogLevel(LOGLEVEL l) { m_nLevel = l; }
   LOGLEVEL GetLogLevel() { return m_nLevel; }
   void SetFileLineLogLevel(LOGLEVEL l) { m_nFileLineLevel = l; }

protected:
   CLogger();
   int MakeDate();
//FishAdd 20080314
	int MakeDate_Time();

protected:
	bool CheckQuata();
   static CLogger *m_pInstance;
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
};

#endif
