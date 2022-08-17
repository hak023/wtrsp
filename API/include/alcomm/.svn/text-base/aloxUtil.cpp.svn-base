#include "aloxUtil.h"

#ifndef LINUX
#include <sys/types.h>
#include <sys/stat.h>
#endif

#define	AL_FTOK_ID		'x'
#define	AL_SHM_MODE	0666
#define	AL_SEM_MODE	0666


extern int errno;


CALUtil::CALUtil()	
{

}

CALUtil::~CALUtil()
{

}

bool CALUtil::IsDir(const char *pname)
{
  struct stat buf; 
  if (stat(pname, &buf) != 0)
     return false;
  if ((buf.st_mode & S_IFDIR) == S_IFDIR)
     return true;
  else
     return false;

}

void CALUtil::MicroSleep(int usec)
{
  timespec req;
  req.tv_sec = 0;
  req.tv_nsec = usec * 1000;
  nanosleep(&req, NULL);
}

void CALUtil::MilliSleep(int msec)
{
  int nSec=0;
  if(msec >= 1000)
  {
    nSec = msec/1000;
    msec = msec%1000;
  }
  timespec req;

  req.tv_sec  = nSec;
  req.tv_nsec = msec * 1000000;
  nanosleep(&req, NULL);
}

unsigned int CALUtil::GetTimeDiff(ALTIME_DIFF* time_diff,bool bIsFirst)
{
  unsigned int tt = 0;

  unsigned int lSec=0L;
  unsigned int lUsec=0L;

  if (bIsFirst)
  {
      gettimeofday(&(time_diff->timev_s), NULL);
      return 0;
  }
  gettimeofday(&(time_diff->timev), NULL);

  lSec=time_diff->timev.tv_sec - time_diff->timev_s.tv_sec;
  if ( time_diff->timev.tv_usec >  time_diff->timev_s.tv_usec )
  {
      lUsec=time_diff->timev.tv_usec - time_diff->timev_s.tv_usec;
  }
  else
  {
      lUsec=(time_diff->timev.tv_usec+1000000) - time_diff->timev_s.tv_usec;
      lSec--;
  }
  tt = (lSec*1000) + lUsec/1000;

  return tt; 
}

unsigned int CALUtil::GetTimeDiff(ALTIME_DIFF* time_diff)
{
  unsigned int tt = 0;

  unsigned int lSec=0L;
  unsigned int lUsec=0L;

	if ( time_diff->timev.tv_sec == 0 ||
			time_diff->timev_s.tv_sec == 0 ){
		return 0;
	}

  lSec=time_diff->timev.tv_sec - time_diff->timev_s.tv_sec;
  if ( time_diff->timev.tv_usec >  time_diff->timev_s.tv_usec )
  {
      lUsec=time_diff->timev.tv_usec - time_diff->timev_s.tv_usec;
  }
  else
  {
      lUsec=(time_diff->timev.tv_usec+1000000) - time_diff->timev_s.tv_usec;
      lSec--;
  }
  tt = (lSec*1000) + lUsec/1000;

  return tt; 
}

int CALUtil::CreateFileLock(const char* pFileName,int* pFPLock)
{
  	int fd=0;
	int ret = -1 ;

	fd = open( pFileName, O_RDWR| O_CREAT, 0644 ) ;
	if ( fd == -1 ) {
		ret = -1 ;
	} else {
		ret = lockf( fd , F_TLOCK, 0 ) ;
		if ( ret == -1 ) {
			if ( errno == EACCES || errno == EAGAIN ) {
				ret = 1 ;
			}
		}
	}
	*pFPLock = fd  ;
    return ( ret ) ;
}


char* CALUtil::RTrim(char *str) 
{
	for (int i = strlen(str) - 1; i >= 0; i--)
	{
		if (str[i] == ' ' || str[i] == '\t' ||str[i] == '\r'|| str[i] == '\n')
		{
			str[i] = 0;
		}
		else
		{
			break;
		}
	}
	return (str);
}

char* CALUtil::LTrim(char *str) 
{
	char tmpBuf[1024];
	int trimCnt=0;
	strcpy(tmpBuf,str);

	for (unsigned int i = 0; i < strlen(str); i++)
	{
		if (tmpBuf[i] == ' ' || tmpBuf[i] == '\t' ||tmpBuf[i] == '\r'|| str[i] == '\n' )
		{
			trimCnt++;
		}
		else
		{
			break;
		}
	}
  memcpy(str,tmpBuf+trimCnt,strlen(tmpBuf+trimCnt));
	str[strlen(tmpBuf+trimCnt)]=0x00;
	return str;
}

char* CALUtil::LRTrim(char *str) 
{
  LTrim(str);
	return RTrim(str);
}
int CALUtil::StrTokenFix(char *str,char* cDelemeter,char (*pOut)[128],int outFixCnt)
{
   char tmpStr[1024];
   char* tmpPtr=NULL;
    
   memset(tmpStr,0x00,sizeof(tmpStr));
   strncpy(tmpStr,str,sizeof(tmpStr));
   int outCnt=0;
   char* miscPtr[2];
   LRTrim(tmpStr);

   if( outFixCnt <= 0 ) return -1;

   if( (tmpPtr=strtok_r(tmpStr,cDelemeter,&miscPtr[0]) )==NULL ){
    outCnt=0;
    return -1;
   }
   strncpy(pOut[outCnt],tmpPtr,sizeof(pOut[outCnt])-1);
   LRTrim(pOut[outCnt]);
   outCnt=1;
  
   if ( outCnt >= outFixCnt ) return 0;       
   while(tmpPtr)
   {    
      if ( outCnt+1 >= outFixCnt )
      {
         if ( miscPtr[0] != NULL ){
            strncpy(pOut[outCnt],miscPtr[0],sizeof(pOut[outCnt])-1);
            LRTrim(pOut[outCnt]);
         }else{
            return -1;
         }
         break;
      }
      tmpPtr=NULL;
      tmpPtr=strtok_r(NULL,cDelemeter,&miscPtr[0]);
      if (tmpPtr)
      {
         strncpy(pOut[outCnt],tmpPtr,sizeof(pOut[outCnt])-1);
         LRTrim(pOut[outCnt]);
         outCnt+=1;
      }
   }

   return 0;
}
int CALUtil::StrToken(char *str,const char* cDelemeter,char (*pOut)[128],int& outCnt,int nMaxCnt)
{
 char tmpStr[1024];
 char* tmpPtr=NULL;

 memset(tmpStr,0x00,sizeof(tmpStr));
 strncpy(tmpStr,str,sizeof(tmpStr));
 char* miscPtr[2];
 outCnt=0;
 LRTrim(tmpStr);

 if( (tmpPtr=strtok_r(tmpStr,cDelemeter,&miscPtr[0]) )==NULL ){
	 outCnt=0;
	 return -1;
 }
 strncpy(pOut[outCnt],tmpPtr,sizeof(pOut[outCnt])-1);
 LRTrim(pOut[outCnt]);
 outCnt=1;

 while(tmpPtr)
 {
	  tmpPtr=NULL;
		tmpPtr=strtok_r(NULL,cDelemeter,&miscPtr[0]);
		if (tmpPtr)
		{
			strncpy(pOut[outCnt],tmpPtr,sizeof(pOut[outCnt])-1);
			LRTrim(pOut[outCnt]);
			outCnt+=1;
		}
		if ( outCnt == nMaxCnt )
		{
			break;
		}
 }
 
 return 0;
}


void CALUtil::GetStrCurrentTime(char* buf)
{

	struct tm *t;
	struct timeval tp;
	gettimeofday(&tp, NULL);

	struct tm   strtm;
	t = localtime_r(&tp.tv_sec,&strtm);

	//sprintf( buf, "%02d:%02d:%02d.%03d", t->tm_hour, t->tm_min, t->tm_sec, int(tp.tv_usec / 1000) );
	sprintf( buf, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
}
void CALUtil::GetStrCurrentTimeMs(char* buf)
{

	struct tm *t;
	struct timeval tp;
	gettimeofday(&tp, NULL);

	struct tm   strtm;
	t = localtime_r(&tp.tv_sec,&strtm);

	sprintf( buf, "%02d:%02d:%02d.%03d", t->tm_hour, t->tm_min, t->tm_sec, int(tp.tv_usec / 1000) );
}
void CALUtil::GetStrCurrentDateTime(char* buf)
{

	struct tm *t;
	struct timeval tp;
	gettimeofday(&tp, NULL);

	struct tm   strtm;
	t = localtime_r(&tp.tv_sec,&strtm);

	//sprintf(buf, "%04d/%02d/%02d %02d:%02d:%02d.%03d",
  //       (1900 + t->tm_year),(t->tm_mon + 1), t->tm_mday,t->tm_hour, t->tm_min, t->tm_sec, int(tp.tv_usec / 1000) );
	sprintf(buf, "%04d/%02d/%02d %02d:%02d:%02d",
         (1900 + t->tm_year),(t->tm_mon + 1), t->tm_mday,t->tm_hour, t->tm_min, t->tm_sec);
}

void CALUtil::GetStrDateTime(struct timeval* tp, char* buf){

	struct tm *t;
	struct tm   strtm;
	t = localtime_r(&(tp->tv_sec),&strtm);

	//sprintf(buf, "%04d/%02d/%02d %02d:%02d:%02d.%03d",
  //       (1900 + t->tm_year),(t->tm_mon + 1), t->tm_mday,t->tm_hour, t->tm_min, t->tm_sec, int(tp->tv_usec / 1000) );
	sprintf(buf, "%04d/%02d/%02d %02d:%02d:%02d",
         (1900 + t->tm_year),(t->tm_mon + 1), t->tm_mday,t->tm_hour, t->tm_min, t->tm_sec);

}
void CALUtil::GetStrTime(struct timeval* tp, char* buf){

	struct tm *t;
	struct tm   strtm;
	t = localtime_r(&(tp->tv_sec),&strtm);

	//sprintf(buf, "%02d:%02d:%02d.%03d", t->tm_hour, t->tm_min, t->tm_sec, int(tp->tv_usec / 1000) );
	sprintf(buf, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);

}
time_t CALUtil::GetTimeFromStr(char* timeBuff)
{
   int yy, mm, dd, hour, min, sec;
   struct tm* t;
   struct timeval tp;
   sscanf(timeBuff, "%d/%d/%d %d:%d:%d", &yy, &mm, &dd, &hour, &min, &sec);

   struct tm   strtm;
   gettimeofday(&tp, NULL);
   tp.tv_usec=0;
   t = localtime_r(&tp.tv_sec,&strtm);

   t->tm_year = yy - 1900 ;
   t->tm_mon  = mm -1;
   t->tm_mday = dd;
   t->tm_hour = hour;
   t->tm_min  = min;
   t->tm_sec  = sec;
   return( mktime(t) );
}

int CALUtil::GetMinuteByTimeval(struct timeval* tp)
{
	struct tm *t;
	struct tm   strtm;
	t = localtime_r(&(tp->tv_sec),&strtm);
	return t->tm_min;
}

int CALUtil::GetMinuteByCurrentTime()
{
	struct tm *t;
	struct timeval tp;
	gettimeofday(&tp, NULL);

	struct tm   strtm;
	t = localtime_r(&tp.tv_sec,&strtm);
	return t->tm_min;
}
void CALUtil::GetStrCurrentDate2(char* buf)
{
	struct tm *t;
	struct timeval tp;
	gettimeofday(&tp, NULL);

	struct tm   strtm;
	t = localtime_r(&tp.tv_sec,&strtm);

	sprintf(buf, "%04d%02d%02d", (1900 + t->tm_year),(t->tm_mon + 1), t->tm_mday);

}
void CALUtil::GetStrCurrentTime2(char* buf)
{

	struct tm *t;
	struct timeval tp;
	gettimeofday(&tp, NULL);

	struct tm   strtm;
	t = localtime_r(&tp.tv_sec,&strtm);

	sprintf(buf, "%02d%02d",t->tm_hour, t->tm_min);
}
void CALUtil::GetStrDate2(struct timeval* tp,char* buf)
{
	struct tm *t;
	struct tm   strtm;
	t = localtime_r(&tp->tv_sec,&strtm);

	sprintf(buf, "%04d%02d%02d", (1900 + t->tm_year),(t->tm_mon + 1),t->tm_mday);

}
void CALUtil::GetStrTime2(struct timeval* tp,char* buf)
{
	struct tm *t;
	struct tm   strtm;
	t = localtime_r(&tp->tv_sec,&strtm);

	sprintf(buf, "%02d%02d",t->tm_hour, t->tm_min);

}

void CALUtil::nAddrToszAddr(unsigned int nAddr,char * szAddr)
{
   uint n1st,n2nd,n3rd,n4th;

   n1st = nAddr >> 24;
   n2nd = (nAddr & 0x00ff0000 ) >> 16;
   n3rd = (nAddr & 0x0000ff00 ) >> 8;
   n4th = nAddr & 0x000000ff;
#ifndef LINUX
   sprintf(szAddr,"%d.%d.%d.%d",n1st,n2nd,n3rd,n4th);
#else
   sprintf(szAddr,"%d.%d.%d.%d",n4th,n3rd,n2nd,n1st);
#endif
}
/////////////////////////////////////////////////////////////////

static Singleton_Destroyer<CALTimer> Timer_Destroyer;
bool CALTimer::m_bInitialized = false;
CALTimer *CALTimer::m_pInstance = NULL;
CCritSec CALTimer::m_Lock;

IMPLEMENT_DYNAMIC(CALTimer, CThread)

CALTimer::CALTimer() : 
	CThread::CThread(),
	m_pTimerMap(new CMapPtrToPtr(32 * 32 - 1))
{

}

CALTimer::~CALTimer()
{
	POSITION pos = m_pTimerMap->GetStartPosition();
	while (pos) {
		void *obj;
		PTimeSpec pSpec;
		m_pTimerMap->GetNextAssoc(pos, (void *&)obj, (void *&)pSpec);
		delete pSpec;
	}
	delete m_pTimerMap;
}

CALTimer *CALTimer::GetInstance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new CALTimer;
		m_pInstance->Create();
		while (CALTimer::m_bInitialized == false)
			MilliSleep(10);
		Timer_Destroyer.Register(m_pInstance);
	}
	return m_pInstance;
}

bool CALTimer::StartTimer(void *obj, int msecs, pfnNotifyTimerDone pfn,
                           PDK32U nWhich, PDK8U nSize)
{
	CALTimer::m_Lock.Lock();
	PTimeSpec pSpec = AddElement(obj, msecs, pfn ,nWhich, nSize);
	CALTimer::m_Lock.Unlock();
	assert(pSpec != NULL);

	return pSpec != NULL;
}

bool CALTimer::StopTimer(void *obj, PDK32U nWhich)
{
	CALTimer::m_Lock.Lock();
	bool bRet = RemoveElement(obj, nWhich);
	CALTimer::m_Lock.Unlock();
	return bRet;
}

bool CALTimer::AddTicker(void *obj, int msecs, pfnNotifyTimerDone pfn,
                        PDK32U nWhich, PDK8U nSize)
{
	CALTimer::m_Lock.Lock();
	PTimeSpec pSpec = AddElement(obj, msecs,pfn,nWhich, nSize);
	CALTimer::m_Lock.Unlock();
	assert(pSpec != NULL);
	pSpec->bContinuous = true;

	return pSpec != NULL;
}

bool CALTimer::RemoveTicker(void *obj, PDK32U nWhich)
{
	return StopTimer(obj, nWhich);
}

CALTimer::PTimeSpec CALTimer::AddElement(void *obj, int msecs, pfnNotifyTimerDone pfn,
	                        PDK32U nWhich, PDK8U nSize)
{
	CMapPtrToPtr *pTimerWhich;
	if (!m_pTimerMap->Lookup((void *)obj, (void *&)pTimerWhich)) {
		pTimerWhich = new CMapPtrToPtr;
		m_pTimerMap->SetAt((void *)obj, (void *)pTimerWhich);
	}

	PTimeSpec pSpec;
	if (pTimerWhich->Lookup((void *)nWhich, (void *&)pSpec)) {
		return pSpec;
	}

	pSpec = new TimeSpec;
	memset(pSpec, 0, sizeof(TimeSpec));
	gettimeofday(&pSpec->tv, NULL);
	pSpec->nTerm = msecs;
	pSpec->nWhich = nWhich;
	pSpec->nSize = nSize;
	pSpec->obj = obj;
   pSpec->pfn = pfn;
   

	pTimerWhich->SetAt((void *)nWhich, (void *)pSpec);
	pSpec->pos = m_TimerList.AddTail(pSpec);

	return pSpec;
}
bool CALTimer::RemoveElement(void *obj, PDK32U nWhich)
{
	CMapPtrToPtr *pTimerWhich;
	if (!m_pTimerMap->Lookup((void *)obj, (void *&)pTimerWhich)) {
		return false;
	}

	PTimeSpec pSpec = NULL;
	if (pTimerWhich->Lookup((void *)nWhich, (void *&)pSpec)) {
		m_TimerList.RemoveAt(pSpec->pos);
		if (pTimerWhich->GetCount() < 1) {
			m_pTimerMap->RemoveKey((void *)obj);
			delete pTimerWhich;
		} else {
			pTimerWhich->RemoveKey((void *)nWhich);
		}
		delete pSpec;
		return true;
	} else {
		return false;
	}
}

void *CALTimer::ThreadProc()
{
	PTimeSpec pSpec = NULL;
	struct timeval ntv;
	CMapPtrToPtr *pTimerWhich;

	SigAllowAll();

	CALTimer::m_bInitialized = true;

	while (DoExit() == false) {
		gettimeofday(&ntv, NULL);
		CALTimer::m_Lock.Lock();
		POSITION pos = m_TimerList.GetHeadPosition();
		while (pos) {
			pSpec = m_TimerList.GetNext(pos);
			if (!pSpec) break;

			if (m_pTimerMap->Lookup((void *)pSpec->obj, (void *&)pTimerWhich) &&
					pTimerWhich->Lookup((void *)pSpec->nWhich, (void *&)pSpec) &&
					DIFFTIME(pSpec->tv, ntv) >= pSpec->nTerm) {
				void *Object = pSpec->obj;
            pfnNotifyTimerDone pfn=pSpec->pfn;
				PDK32U nWhich = pSpec->nWhich;
				PDK8U nSize = pSpec->nSize;
				if (pSpec->bContinuous) {
					pSpec->tv.tv_sec += pSpec->nTerm / 1000;
					pSpec->tv.tv_usec+= (pSpec->nTerm % 1000) * 1000;

					//by yun
					if(pfn) pfn(Object,nWhich,nSize);
					continue;
				}
				if (RemoveElement(Object, nWhich)) {
					if(pfn) pfn(Object,nWhich,nSize);
				}
			}
			pSpec = NULL;
		}
		CALTimer::m_Lock.Unlock();

		MilliSleep(1);
	}
	return NULL;
}

//CCondition
void CCondition::Initialize()
{
	pthread_cond_init(&m_Cond, NULL);
}

void CCondition::Wait()
{
	Lock();
	pthread_cond_wait(&m_Cond, &m_Mutex);
	Unlock();
}

void CCondition::Signal()
{
	Lock();
	pthread_cond_signal(&m_Cond);
	Unlock();
}

//safe thread
IMPLEMENT_DYNAMIC(CUserThread, CThread)
CUserThread::CUserThread()
   : m_bStopped(false),
     m_bEscaped(true)
{
}

#define USRTHREAD_STACK_SIZE (1024 * 10 * 8)

bool CUserThread::Start()
{
   bool bRet=false;
   //yun memory tuning
   bRet=(CThread::Create(USRTHREAD_STACK_SIZE, 0) == 1);
   //bRet=(CThread::Create() == 1);
   if ( bRet ) m_bEscaped=false;
   return bRet;
}
#undef USRTHREAD_STACK_SIZE 

void CUserThread::SafeSleep(int ms)
{
   if (ms <= 0) return;

   int nCnt=ms/60;
   int nRemain=ms%60;
   for( int i = 0 ; i < nCnt ; i++)
   {
      if(m_bStopped) return;
      MilliSleep(60);
   }
   if(m_bStopped) return;

   if ( nRemain > 0 ) MilliSleep(nRemain);

}

// 2012.07.01 edit minimei7
// for fast stop..
void CUserThread::Stop( )
{
   m_bStopped=true;
   CThread::Close_1();
   int nCnt=0;
   while( m_bEscaped==false ){
      if((++nCnt) > 100 ){ 
         LOGGER(CLogger::Warn,"[Thread] error thread stop,exceed cnt(%d)", nCnt);
         break;
      }
      //MilliSleep(200);
      MilliSleep(10);
   }
   CThread::Close_2();
}
