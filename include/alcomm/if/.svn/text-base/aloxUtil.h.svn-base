#ifndef __ALOX_UTIL_H
#define __ALOX_UTIL_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "aloxCommon.h"
#include "list.h"

typedef struct __AL_TIME_DIFF_
{   
  struct timeval timev_s;
  struct timeval timev;
}ALTIME_DIFF,*PALTIME_DIFF;

class CALUtil
{
public:
 CALUtil();
 ~CALUtil();
 static bool IsDir(const char *pname);
 static void MicroSleep(int usec);
 static void MilliSleep(int msec);
 static unsigned int GetTimeDiff(ALTIME_DIFF* time_diff,bool bIsFirst);
 static unsigned int GetTimeDiff(ALTIME_DIFF* time_diff);


 //file lock 
 // -1 : create lock file fail
 //  0 : success
 //  1 : Duplicate
 static int CreateFileLock(const char* pFileName,int* pFPLock);

 static char* LTrim(char *str);
 static char* LRTrim(char *str);
 static char* RTrim(char *str);
 static int StrTokenFix(char *str,char* cDelemeter,char (*pOut)[128],int outFixCnt);
 static int StrToken(char *str,const char* cDelemeter,char (*pOut)[128],int& outCnt,int nMaxCnt); 

 //time
 static void GetStrCurrentTime(char* buf);
 static void GetStrCurrentTimeMs(char* buf);
 static void GetStrCurrentDateTime(char* buf);
 static void GetStrDateTime(struct timeval* tp, char* buf);	
 static void GetStrTime(struct timeval* tp, char* buf);	
 static int GetMinuteByTimeval(struct timeval* tp);
 static int GetMinuteByCurrentTime();
 static void GetStrCurrentDate2(char* buf);
 static void GetStrCurrentTime2(char* buf);
 static void GetStrDate2(struct timeval* tp,char* buf);
 static void GetStrTime2(struct timeval* tp,char* buf);
 //timeBuff: YYYY/MM/DD HH:MI:SS 
 static time_t GetTimeFromStr(char* timeBuff);


 //ip	
 static void nAddrToszAddr(unsigned int nAddr,char * szAddr);

};
///////////////////
typedef void (*pfnNotifyTimerDone)(void* obj, int nTimerID, unsigned char ucWhere);
class CALTimer : public CThread
{
  DECLARE_DYNAMIC(CALTimer)
private:
public:
  CALTimer();
  ~CALTimer();
  static CALTimer *GetInstance();
  bool StartTimer(void *obj, int msecs, pfnNotifyTimerDone pfn=NULL,
                  PDK32U nWhich = 0, PDK8U nSize=2);
  bool StopTimer(void *obj, PDK32U nWhich = 0);
  bool AddTicker(void *obj, int msecs, pfnNotifyTimerDone pfn=NULL,
                  PDK32U nWhich = 0, PDK8U nSize=2);
  bool RemoveTicker(void *obj, PDK32U nWhich = 0);

protected:
  typedef struct {
    struct timeval tv;
    PDKINT nTerm;
    bool   bContinuous;
    PDK32U nWhich;
    PDK8U  nSize;
    pfnNotifyTimerDone pfn;
    void *obj;
    POSITION pos;
  } TimeSpec, *PTimeSpec;

  void *ThreadProc();
  PTimeSpec AddElement(void *obj, int msecs, pfnNotifyTimerDone pfn,
                        PDK32U nWhich = 0, PDK8U nSize=2);
  bool RemoveElement(void *obj, PDK32U nWhich = 0);

  static bool    m_bInitialized;
  static CALTimer *m_pInstance;
  static CCritSec m_Lock;

  CMapPtrToPtr  *m_pTimerMap;
  CTypedPtrList<CPtrList, PTimeSpec> m_TimerList;
};

//Conditional Variable
class CCondition : public CCritSec
{
public:
	CCondition(){};
	virtual ~CCondition(){
	pthread_cond_destroy(&m_Cond);
	};

	void Wait();
	void Signal();

	void Initialize();

protected:
	pthread_cond_t m_Cond;
};

///the uncopyable
class CUncopyable
{
protected:
   CUncopyable(){}
   ~CUncopyable(){}

private:
   CUncopyable(const CUncopyable&);
   CUncopyable& operator=(const CUncopyable&);
};


///safe thread
class CUserThread
   : public CThread,
     private CUncopyable
{
   DECLARE_DYNAMIC(CUserThread)
public:
   CUserThread();
   ~CUserThread()
   {
   }
   bool Start();
   void Stop();
   void SafeSleep(int ms);
protected:
   bool m_bStopped;
   bool m_bEscaped;
};

#include "sock.h"

class CUUDPIF: public CUserThread
{
public:
   CUUDPIF()
      : m_bOpen(false),
        m_pfnRecvCallBack(NULL)
        
   {
      strcpy(m_szLogHdr,"[UUIPIF]");
      memset(m_szLocalPath,0x00,sizeof(m_szLocalPath));
      memset(m_szDestPath,0x00,sizeof(m_szDestPath));
   }
   ~CUUDPIF()
   {
   }
   void Stop()
   {
      //Close();
   }

   void* ThreadProc() {
      m_bStopped=false;
      m_bEscaped=false;
      int nRetSelect = 0;
      int len = 0;
      sockaddr_un rFrom;

      static int st_nudpmax_buffer=(1024*4);

      char* pszBuffer=new char[st_nudpmax_buffer];
      if(pszBuffer == NULL ) return NULL;
      while (DoExit() == false) {
         if( m_bStopped ) break;
         if (m_bOpen == false) {
            SafeSleep(300);
            continue;
         }
         
         nRetSelect = m_sockUnixUdp.Select(20);
         if (nRetSelect ==  0) continue;
         if (nRetSelect == -1) break;
         
         memset(pszBuffer,0x00,st_nudpmax_buffer);
         len   =  m_sockUnixUdp.RecvFrom((char *)pszBuffer, (st_nudpmax_buffer-1), &rFrom);
         if( len == 0 ) {
            SafeSleep(200); 
            continue;
         }
         if ( len == -1 ){
            LOGGER(CLogger::Error,"%s RecvFrom Error. now exit ",m_szLogHdr);
            break;
         }
         if( m_pfnRecvCallBack == NULL  ) continue;
         m_pfnRecvCallBack((void*)this,(void*)pszBuffer,(void*)&len);
      }
      m_bEscaped=true;
      return NULL;
   }
   
   bool Start(char* srcName, char* szLocalPath, char* szDestPath, PCallBack3 pFunc=NULL)
   {
      if ( srcName == NULL || srcName[0] == 0x00 ){
         LOGGER(CLogger::Error,"%s source name is null ",m_szLogHdr);
      }
      sprintf(m_szLogHdr,"[%s]",srcName);
      //check param
      if( szLocalPath== NULL || szLocalPath[0] == 0x00 ){
         LOGGER(CLogger::Error,"%s local path is null ",m_szLogHdr);
         return false;
      }
      //check param
      if( szDestPath== NULL || szDestPath[0] == 0x00 ){
         LOGGER(CLogger::Error,"%s destination path is null ",m_szLogHdr);
         return false;
      }
      //check open
      if( m_bOpen ){
         m_sockUnixUdp.Close();
         m_bOpen = false;
      }

      if( pFunc ) m_pfnRecvCallBack=pFunc;
      strcpy(m_szLocalPath,szLocalPath);
      strcpy(m_szDestPath,szDestPath);

      if ( m_sockUnixUdp.Open(m_szLocalPath) == false) {
         LOGGER(CLogger::Error,"%s fail in Unix UDP %s",m_szLogHdr,m_szLocalPath);
         return false;
      }
      m_bOpen=CUserThread::Start();
      LOGGER(CLogger::Info,"%s Start Unix UDP %s",m_szLogHdr,m_szLocalPath);
      return m_bOpen;
   }
   void SendMsg(char* pData,int nSize,char* szDestPath=NULL)
   {
      if( szDestPath == NULL ) szDestPath=m_szDestPath;
      int nRet=0;
      if ( m_bOpen == false ){
         LOGGER(CLogger::Error,"%s Fail Send Data to %s . not yet open",
               m_szLogHdr,szDestPath);
         return;
      }

      nRet= m_sockUnixUdp.SendTo(pData,nSize,szDestPath);
      if ( nRet != nSize )
      {
         LOGGER(CLogger::Error,"%s Fail Send Data to %s Ret(%d)",
               m_szLogHdr,szDestPath,nRet);
      }
   }

protected:
   CUnixUdp m_sockUnixUdp;
   bool m_bOpen;
   PCallBack3 m_pfnRecvCallBack;
   char m_szLogHdr[20];
   char m_szLocalPath[256];
   char m_szDestPath[256];
};



#endif

