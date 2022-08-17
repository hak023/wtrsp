#ifndef _ALOX_VMS_COMMON_H_
#define _ALOX_VMS_COMMON_H_

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

#include "base.h"
#include "jthread.h"
#include "sock.h"
#include "logger.h"
#include "util.h"

//log
#ifndef LOGGER
#define LOGGER(v...)     CLogger::GetInstance()->Log(v)
#endif

//loglevel
#define PL_CRI     CLogger::Critical
#define PL_ERR     CLogger::Error
#define PL_WAR     CLogger::Warn
#define PL_INF     CLogger::Info
#define PL_DEB     CLogger::Api

//define Macro 
#define ALOX_UTIL_ISNUMERIC(c)           ((c) >= '0' && (c) <= '9')
#define ALOX_UTIL_RET_MIN(a, b)          ((a) > (b) ? (b) : (a))
#define ALOX_UTIL_RET_MAX(a, b)          ((a) < (b) ? (b) : (a))
#define ALOX_UTIL_RET_ABS(x)             (((x) >= 0) ? (x) : -(x))
#define	ALOX_MEM_FREE(a)		         if ( (a) ) { free( (a) ); a = NULL; }
#define	ALOX_CLOSE_FD(a)                 if ( (a) ) { close( (a) ); a=0 ; }
#define	ALOX_CLOSE_FP(a)                 if ( (a) ) { fclose( (a) ); a=NULL ; }	



#endif
