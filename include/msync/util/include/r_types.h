
#ifndef _RPDK_PDK_TYPE_H_
#define _RPDK_PDK_TYPE_H_
namespace RED
{

#ifdef WIN32
#	define PDK_LITTLE_ENDIAN
#	define DLLEXPORT __declspec(dllexport)
#	define DLLIMPORT __declspec(dllimport)
#	ifdef BUILD_PDK
//#		define DLLPORT	DLLEXPORT
#		define DLLPORT
#	else // !BUILD_PDK
//#		define DLLPORT	DLLIMPORT
#		define DLLPORT
#	endif // BUILD_PDK
#else // !WIN32
#	include <arpa/nameser_compat.h>
#	if defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN)
#		define PDK_LITTLE_ENDIAN
#	endif
#	define DLLEXPORT
#	define DLLIMPORT
#	define DLLPORT
#endif // WIN32

#ifndef __bswap_16
#	define __bswap_constant_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
#	define __bswap_16(x) __bswap_constant_16 (x)
#endif // __bswap_16

#ifndef __bswap_32
#define __bswap_constant_32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
#	define __bswap_32(x) __bswap_constant_32 (x)
#endif // __bswap_32

#ifdef COMPILE_64BIT
#ifndef __bswap_64
#	define __bswap_constant_64(x) ((((x) & 0xff00000000000000ul) >> 56) | (((x) & 0x00ff000000000000ul) >>  40) | (((x) & 0x0000ff0000000000ul) >> 24) | (((x) & 0x000000ff00000000ul) >> 8) | (((x) & 0x00000000ff000000ul) << 8) | (((x) & 0x0000000000ff0000ul) << 24) | (((x) & 0x000000000000ff00ul) << 40)  | (((x) & 0x00000000000000fful) << 56))

#	define __bswap_64(x) __bswap_constant_64 (x)
#	endif // __bswap_64
#else // COMPILE_64BIT
#ifndef __bswap_64
#	define __bswap_constant_64(x) ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
#	define __bswap_64(x) __bswap_constant_64(x)
#	endif // __bswap_64
#endif // COMPILE_64BIT

#ifdef PDK_LITTLE_ENDIAN
#	define hton64(x) __bswap_64(x)
#	define ntoh64(x) __bswap_64(x)
#else // !PDK_LITTLE_ENDIAN
#	define hton64(x)
#	define ntoh64(x)
#endif // PDK_LITTLE_ENDIAN

typedef char PDK8;
typedef short PDK16;
typedef int PDKINT;
typedef int PDK32;
typedef long PDKLONG;
typedef unsigned char PDK8U;
typedef unsigned short PDK16U;
typedef unsigned int PDKUINT;
typedef unsigned int PDK32U;
typedef unsigned long PDKULONG;
#ifndef WIN32
	typedef long long PDK64;
	typedef unsigned long long PDK64U;
#else // !WIN32
	typedef int pid_t;
#	ifndef u_int64_t
#		define u_int64_t	unsigned __int64
#	endif // u_int64_t
#	ifndef uint32_t
#		define uint32_t	PDK32U
#	endif // uint32_t
#	ifndef uint16_t
#		define uint16_t	PDK16U
#	endif // uint16_t
#	ifndef HAVE_MODE_T
#		define HAVE_MODE_T
		typedef unsigned int mode_t;
#	endif // HAVE_MODE_T
#	ifndef mode_t
#		define mode_t	int
#	endif // mode_t
#	ifndef PATH_MAX
#		define PATH_MAX	1024 * 8
#	endif // PATH_MAX
#ifndef strncasecmp
	#define strncasecmp	_strnicmp
#endif
#endif // WIN32
#ifndef WIN32
typedef const char *LPCSTR;
typedef char *LPSTR;
#endif
typedef const unsigned char *LPCUSTR;
typedef unsigned char *LPUSTR;

#ifndef SIZE_MAX
#	define SIZE_MAX ULONG_MAX
#endif
#define _COUNTOF(array) (sizeof(array)/sizeof(array[0]))
#define NUMERIC(c)      ((c) >= '0' && (c) <= '9')
#ifndef MIN
#	define MIN(a, b)       ((a) > (b) ? (b) : (a))
#endif // MIN
#ifndef MAX
#	define MAX(a, b)       ((a) < (b) ? (b) : (a))
#endif // MAX
#ifndef ABS
#define ABS(x)          (((x) >= 0) ? (x) : -(x))
#endif

#include <assert.h>

#ifndef ASSERT
#	define ASSERT(p)       assert(p)
#	define ASSERT_VALID(p) assert((p) != NULL)
#endif // ASSERT

#ifndef PCALLBACK
#	define PCALLBACK
	typedef int (*PCallBack) (void *);
	typedef int (*PCallBack2)(void *, void *);
	typedef int (*PCallBack3)(void *, void *, void *);
#endif // PCALLBACK


#ifdef WIN32
#define EPERM            1      /* Operation not permitted */
#define ENOENT           2      /* No such file or directory */
#define ESRCH            3      /* No such process */
#define EINTR            4      /* Interrupted system call */
#define EIO              5      /* I/O error */
#define ENXIO            6      /* No such device or address */
#define E2BIG            7      /* Arg list too long */
#define ENOEXEC          8      /* Exec format error */
#define EBADF            9      /* Bad file number */
#define ECHILD          10      /* No child processes */
#define EAGAIN          11      /* Try again */
#define ENOMEM          12      /* Out of memory */
#define EACCES          13      /* Permission denied */
#define EFAULT          14      /* Bad address */
#define ENOTBLK         15      /* Block device required */
#define EBUSY           16      /* Device or resource busy */
#define EEXIST          17      /* File exists */
#define EXDEV           18      /* Cross-device link */
#define ENODEV          19      /* No such device */
#define ENOTDIR         20      /* Not a directory */
#define EISDIR          21      /* Is a directory */
#define EINVAL          22      /* Invalid argument */
#define ENFILE          23      /* File table overflow */
#define EMFILE          24      /* Too many open files */
#define ENOTTY          25      /* Not a typewriter */
#ifndef ETXTBSY
#define ETXTBSY         26      /* Text file busy */
#endif
#define EFBIG           27      /* File too large */
#define ENOSPC          28      /* No space left on device */
#define ESPIPE          29      /* Illegal seek */
#define EROFS           30      /* Read-only file system */
#define EMLINK          31      /* Too many links */
#define EPIPE           32      /* Broken pipe */
#define EDOM            33      /* Math argument out of domain of func */
#define ERANGE          34      /* Math result not representable */

#ifndef ETIMEDOUT
#  define ETIMEDOUT 10060	/* This is the value in winsock.h. */
#endif

#endif /*WIN32*/


}; //namespace RED
#endif
