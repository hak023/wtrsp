
#ifndef APP_COM_H
#define APP_COM_H

#include "logmgr.h"
#include <pthread.h>

#ifndef DEF_COMMON_TYPE
#define DEF_COMMON_TYPE
typedef unsigned char uchar;
typedef unsigned char uint8;
typedef unsigned short ushort;
typedef unsigned short uint16;
typedef unsigned short int16;
typedef unsigned int uint;
typedef unsigned int uint32;
typedef unsigned long ulong;
typedef unsigned long uint64;
#if COMPILER != COMPILER_MICROSOFT
typedef uint16      WORD;
typedef uint32      DWORD;
#endif //COMPILER
typedef uint64 OBJECT_HANDLE;
#endif

#define APP_THREAD_STACK_SIZE 1024 * 1024 * 2//1024 * 2 kbyte

#define PRINTMSG_HA(LEVEL, SRC, DESC, PMSG)   \
   if((CLogMgr::GetInstance())->GetLogLevel() >= LEVEL) { \
      { char szBuff[10240];    \
        (PMSG)->str(szBuff, 10240);        \
        LOGMGR(LEVEL, "[%-15s] %s %s", SRC, DESC, szBuff);  \
      }  \
   }

#endif//APP_COM_H
////@}

