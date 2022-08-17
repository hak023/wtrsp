/*
   HURIM Copy Right

   File : util.h

   1. Declare utility 

   Create Date : 2001.  by Jeong
   Updated     : 2002.1 by WindJun - VMS
*/

#ifndef	__RPDK_UTIL_H__
#define	__RPDK_UTIL_H__

#include <fcntl.h>

#ifndef _MSC_VER
   #include <unistd.h>
   #include <sys/types.h>
#endif

#include "r_base.h"
#include "r_list.h"
#include "r_jthread.h"
#include "r_destroyer.h"

namespace RED
{
struct CTimeDiff_st{
  struct timeval timev_s;
  struct timeval timev;
};
void GetStrTime_Util(struct timeval* tp, char* buf);
unsigned int GetTimeDiff_Util(CTimeDiff_st* time_diff,bool bIsFirst=false);

//@@ Add 02/15/02
//////////////////////////////////////////////////////////////////////////////
void SplitPath(const char *pname, CString &head, CString &tail,
			   const char spliter = '/', int order = 0);
bool IsDir(const char *pname);
void MakeDirs(const char *pname, mode_t mode = 0777);
void Tokenize(LPSTR pszLine, CArray<CString,CString> &value, LPSTR delim=" \t");

//////////////////////////////////////////////////////////////////////////////
void MicroSleep(int usec);
void MilliSleep(int msec);
void Sleep(int sec);
int kbhit();

#ifndef _MSC_VER
   time_t GetCurrentTime();
#endif
int    GetCurrentTimeInSec();
int    PingCheck(unsigned long nAddr);

//////////////////////////////////////////////////////////////////////////////
typedef	struct _mem_item {
	int nLength;
	char *pBuffer;
	_mem_item *pNext;
} MEMITEM, *PMEMITEM;

//////////////////////////////////////////////////////////////////////////////
#define  QNODE_FREE			      0
#define  QNODE_USE			      1

#define  QNODE_SUCCESS		      0
#define  QNODE_BUFFER_FULL	      -1
#define  QNODE_BUFFER_EMPTY	   -2

///////////////////////////
//#define CI_TIMER_DONE	0xFFFF
#define DIFFTIME(tvp, uvp) abs((tvp.tv_sec  - uvp.tv_sec) * 1000 + (tvp.tv_usec - uvp.tv_usec) / 1000)
#define DIFFTIME1(tvp, uvp) abs(tvp.tv_sec - uvp.tv_sec)

int strncmp_rev(char* s1,char* s2,int len);
void ToLower(LPCSTR pszstr);
void ToUpper(LPCSTR pszstr);
LPSTR MemSeek(LPSTR pszBuffer, PDKINT nBuffSize,
	LPSTR pszNeedle, PDKINT nLen, PDK8U chLastMask = 0);
bool IsDigit(char *str);
int axtoi(LPCSTR pszHex);
}; //namespace RED
#endif //__UTIL_H__
