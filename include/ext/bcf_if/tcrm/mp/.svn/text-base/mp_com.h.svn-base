/*
 * mp_com.h
 */
#ifndef MP_COM_H
#define MP_COM_H

//#include "logger.h"

#define  DUMP_RTP_LOG 0

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

#if 0
#define PRINTMSG(LEVEL, SRC, DESC, PMSG)   \
   if(m_pLogger->GetLogLevel() >= LEVEL) { \
      { char* szBuff = new char[10240];    \
        (PMSG)->str(szBuff, 10240);        \
        m_pLogger->Log(LEVEL, "%s %s%s", SRC, DESC, szBuff);  \
         delete[] szBuff; \
      }  \
   }
#endif
#endif//MP_COM_H

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif
////@}

