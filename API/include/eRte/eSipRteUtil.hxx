/**
 * @file  eSipRteUtil.hxx
 * the sip route util header
 *
 * @attention Copyright (c) 2014 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2014.11.12   hc_yun   1.0     first written
 */

#ifndef __ESIP_CORE_SIP_ROUTE_UTILITY__
#define __ESIP_CORE_SIP_ROUTE_UTILITY__




#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define  ESIPRTEUTIL_MEM_FREE(a)          if ( (a) ) { free( (a) ); a = NULL; }
#define  ESIPRTEUTIL_CLOSE_FD(a)          if ( (a) ) { close( (a) ); a=0 ; }
#define  ESIPRTEUTIL_CLOSE_FP(a)          if ( (a) ) { fclose( (a) ); a=NULL ; } 

namespace eSIP
{

typedef struct __SIPRTEUTIL_TIME_DIFF_
{   
  struct timeval timev_s;
  struct timeval timev;
}CTimeDiff,*PCTimeDiff;

class CSipRteUtil
{
public:
///defines the CSipRteUtilValue_t
struct CSipRteUtilValue_t
{
   enum { E_MAXLEN_VALUE=128 };
   char            m_szData[E_MAXLEN_VALUE];
};
public:
CSipRteUtil();
~CSipRteUtil();
public:
static void    m_fnLTrim(char*& _pStr);
static void    m_fnRTrim(char* _pStr);
static void    m_fnLRTrim(char* _pStr);
static void    m_fnStrToken(char* _pStr, CSipRteUtilValue_t* _pstData ,int& _rCnt,char _cDelemter='|');
static bool    m_fnIsNumberString(char* _pStr);
static bool    m_fnIsIPv4String(char* _pStr);
static bool    m_fnIsIPv6String(char* _pStr);
static bool    m_fnIsIPversion(unsigned short _usIPver);
static bool    m_fnIsPort(unsigned int _uiPort);
static bool    m_fnIsID(unsigned int _uiID);
static bool    m_fnIsNodeStatus(int _nNodeStatus);
static bool    m_fnIsSipProtcolType(unsigned short _usProto);
static void    m_fnMakeSipNodeKey(char* _pIP,unsigned short _usPort, unsigned char _ucType,char* _pOutKey);
static void    m_fnMakeRouteNodeKey(unsigned int _uiLID, unsigned int _uiRID,char* _pOutKey);

//common
static bool IsDir(const char *pname);
static void MakeDir(char* _pszPath);
static void MicroSleep(int usec);
static void MilliSleep(int msec);
static unsigned int GetTimeDiff(CTimeDiff* time_diff,bool bIsFirst);
static unsigned int GetTimeDiff(CTimeDiff* time_diff);

static char* LTrim(char *str);
static char* LRTrim(char *str);
static char* RTrim(char *str);
static int StrTokenFix(char *str,char* cDelemeter,char (*pOut)[128],int outFixCnt);
static int StrToken(char *str,char* cDelemeter,char (*pOut)[128],int& outCnt,int nMaxCnt); 

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
//shared memory
static void* m_fnCreateShm(const char* _pszPath, int _nsize, key_t& _key, int _nRdOnly=0);
static void* m_fnCreateShm(int _nSize, key_t _key, int _nRdOnly=0);
static int m_fnDestroyShm(const char *pszPath);

static int axtoi(char* pszHex);
//cpu,mem
static int m_fnGetCpuUsage();
static void m_fnGetRealMemInfo(int* total, int* free);
//@vibc
static unsigned int m_fnCheckSum(char *buffer, size_t len);

};



}; //end namespace

#endif

