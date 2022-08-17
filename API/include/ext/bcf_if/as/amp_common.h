#ifndef __AMP_COMMON_H__
#define __AMP_COMMON_H__

#ifdef _WINDLL
   #ifdef AMP_EXPORTS
      #define AMP_DLL __declspec(dllexport)
   #else
      #define AMP_DLL __declspec(dllimport)
   #endif
#else 
   #define AMP_DLL   
#endif 

#ifndef NULL
#define NULL 0
#endif

#define AMP_SAFE_DELETE_OBJECT(p) \
   if(p != NULL) { \
      delete p; \
      p = NULL; \
   }

#define AMP_SAFE_DELETE_ARRAY(p) \
   if(p != NULL) { \
      delete[] p; \
      p = NULL; \
   }

#if 0

#ifdef WIN32
#define pthread_self() 0
#else
#include <pthread.h>
#endif

#define AMP_MEM_LEAK_TEST_CREATED(x)  printf("[%d]AMP::%s Created 0x%p\n", (unsigned int)pthread_self(), #x , this);
#define AMP_MEM_LEAK_TEST_DELETED(x)  printf("[%d]AMP::%s Deleted 0x%p\n", (unsigned int)pthread_self(), #x , this);
#define AMP_MEM_LEAK_TEST_OPENED(x)   printf("[%d]AMP::%s Opened  0x%p\n", (unsigned int)pthread_self(), #x , this);
#define AMP_MEM_LEAK_TEST_CLOSED(x)   printf("[%d]AMP::%s Closed  0x%p\n", (unsigned int)pthread_self(), #x , this);

#else

#define AMP_MEM_LEAK_TEST_CREATED(x)    
#define AMP_MEM_LEAK_TEST_DELETED(x)    
#define AMP_MEM_LEAK_TEST_OPENED(x)     
#define AMP_MEM_LEAK_TEST_CLOSED(x)     
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#endif

