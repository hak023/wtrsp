
#ifndef _PDK_TYPE_H_
#define _PDK_TYPE_H_

typedef char PDK8;
typedef short PDK16;
typedef int PDKINT;
typedef long PDK32;
typedef unsigned char PDK8U;
typedef unsigned short PDK16U;
typedef unsigned int PDKUINT;
typedef unsigned long PDK32U;

typedef const char *LPCSTR;
typedef char *LPSTR;

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
#define ABS(x)          (((x) >= 0) ? (x) : -(x))

#include <assert.h>

#define ASSERT(p)       assert(p)
#define ASSERT_VALID(p) assert((p) != NULL)

#ifndef PCALLBACK
#	define PCALLBACK
	// added by kmj at 11/01/03
	typedef int (*PCallBack) (void *);
	typedef int (*PCallBack2)(void *, void *);
	typedef int (*PCallBack3)(void *, void *, void *);
#endif // PCALLBACK

#endif
