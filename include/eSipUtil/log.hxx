
#ifndef ESIPUTIL_LOG_H
#define ESIPUTIL_LOG_H

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <pthread.h>

namespace eSipUtil
{

typedef enum
{
	E_LOG_DISABLE = 0,
	E_LOG_ERR,
	E_LOG_WARN,
	E_LOG_INFO,
	E_LOG_DEBUG,
	E_LOG_STACK,

	E_LOG_NUM,
}ELogLevel_t;

const char * getLogLevel(ELogLevel_t _eLogLevel);

typedef void (* PfuncQueueFullCallback)(const char * _pszQueueName, int _nCurrentRate, int _nLimitRate);

class AsyncFileLog
{
	public :
		enum
		{
			E_CONST_MAX_INSTANCE_NUM = 10,
			E_CONST_MAX_CATEGORY_NUM = 10,
			E_CONST_MAX_CATEGORY_STR_LEN = 20,	
			E_CONST_MAX_FILE_PATH_LEN = 256,
			E_CONST_MAX_FILE_NAME_LEN = 50,
			E_CONST_MAX_FILE_FULL_NAME_LEN = E_CONST_MAX_FILE_PATH_LEN + E_CONST_MAX_FILE_NAME_LEN + 20,
			E_CONST_DFT_FILE_SIZE = 300000000,
			E_CONST_DFT_SINGLE_LINE_LEN = 1000,
			E_CONST_DFT_QUEUE_SIZE = 10000,
		};

		/* ==== The APIs is used by user. The log of library is written to instance of index 0 ==== */
		//_pszFilePath must include "/" last
		static bool m_fnInit(const char * _pszFilePath, const char * _pszFileName, int & _nLogInstanceIdx,
												unsigned int _unMaxSingleLineLen, unsigned int _unMaxFile = 0, bool _bTbbEnable=true);
		//This function must be invoked. If it is not invoked, all log will be discarded.
		static bool m_fnSetMaxQueueSize(int _nLogInstanceIdx, unsigned int _unMaxQueueSize);
		static unsigned int m_fnGetCurrentQueueSize(int _nLogInstanceIdx);
		static void m_fnSetCategoryStr(int _nLogInstanceIdx, int _nLogCategoryIndx, const char * _pszStr);
		static unsigned int m_fnGetMaxSingleLineLen(int _nLogInstanceIdx);
		static void m_fnSetMaxFileSize(int _nLogInstanceIdx, unsigned int _unSize);
		static bool m_fnSetLogLevel(int _nLogInstanceIdx, int _nLogCategoryIndx, ELogLevel_t _eLogLevel);
		static ELogLevel_t m_fnGetLogLevel(int _nLogInstanceIdx, int _nLogCategoryIndx);
		static bool m_fnCheckLogLevel(int _nLogInstanceIdx, int _nLogCategoryIndx, ELogLevel_t _eLogLevel);
		static bool m_fnSetMaxFileNum(int _nLogInstanceIdx,unsigned int _unMaxFile);  // 0 is unlimited
		static void m_fnSetQueueFullCallback(PfuncQueueFullCallback _pfnQueueFullCb);
		static unsigned int m_fnGetPutLogCnt(int _nLogInstanceIdx);
		static unsigned int m_fnGetGetLogCnt(int _nLogInstanceIdx);
		/* ======================================================= */
		static void m_fnLog(int _nLogInstanceIdx, int _nLogCategoryIndx, ELogLevel_t _eLogLevel, char * _pszLogStr);
		static void m_fnLog(int _nLogInstanceIdx, int _nLogCategoryIndx, char * _pszLogStr);
	private :
		static PfuncQueueFullCallback m_pfnQueueFullCb;
	
};
/***************************** Function Debugging Class *************************************/
class Debugging
{
	public:
		Debugging(const char * _pszFunc,unsigned int _unFuncStackID);
		~Debugging();
		void m_fnSetReason(const char * _pszFormat,...);
		char m_szReason[128];
		char m_szFunc[128];
		unsigned int m_unFuncStackID;
		unsigned int m_unThreadID;
};
#define FUNC_DEBUG(b) Debugging apiDebug(__func__,b)
#define FUNC_ERR(v...) apiDebug.m_fnSetReason(v)
void g_fnDebugPoint();
}

#define LogInternal(_level, _format, ...) \
	do \
	{ \
		if( eSipUtil::AsyncFileLog::m_fnCheckLogLevel(0, 0, _level) ) \
		{ \
			unsigned int unMaxSingleLen = eSipUtil::AsyncFileLog::m_fnGetMaxSingleLineLen(0); \
  		    char * pszLog = (char *)malloc(unMaxSingleLen); \
  		    if( pszLog ) \
			{ \
				snprintf(pszLog, unMaxSingleLen-1,_format, ##__VA_ARGS__); \
				eSipUtil::AsyncFileLog::m_fnLog(0, 0, _level, pszLog); \
 		    } \
		} \
	}while(false)
#define LogInternalNoGuard(_format, ...) \
	do \
	{ \
		unsigned int unMaxSingleLen = eSipUtil::AsyncFileLog::m_fnGetMaxSingleLineLen(0); \
		char * pszLog = (char *)malloc(unMaxSingleLen); \
		if( pszLog ) \
		{ \
			snprintf(pszLog, unMaxSingleLen-1,_format, ##__VA_ARGS__); \
			eSipUtil::AsyncFileLog::m_fnLog(0, 0,pszLog); \
		} \
	}while(false)
#define Log_(_logInstance, _logCategory, _level, _format, ...) \
	do \
	{ \
		if( eSipUtil::AsyncFileLog::m_fnCheckLogLevel(_logInstance, _logCategory, _level) ) \
		{ \
			unsigned int unMaxSingleLen = eSipUtil::AsyncFileLog::m_fnGetMaxSingleLineLen(_logInstance); \
  		    char * pszLog = (char *)malloc(unMaxSingleLen); \
  		    if( pszLog ) \
			{ \
				snprintf(pszLog, unMaxSingleLen-1,_format, ##__VA_ARGS__); \
				eSipUtil::AsyncFileLog::m_fnLog(_logInstance, _logCategory, _level, pszLog); \
 		    } \
		} \
	}while(false)
#ifdef EUTIL_DYNAMIC
#define Log(_logInstance, _logCategory, _level, _format, ...) \
	do \
	{ \
		if( eSipUtil::AsyncFileLog::m_fnCheckLogLevel(_logInstance, _logCategory, _level) ) \
		{ \
			unsigned int unMaxSingleLen = eSipUtil::AsyncFileLog::m_fnGetMaxSingleLineLen(_logInstance); \
  		    char * pszLog = (char *)malloc(unMaxSingleLen); \
  		    if( pszLog ) \
			{ \
				snprintf(pszLog, unMaxSingleLen-1,_format, ##__VA_ARGS__); \
				eSipUtil::AsyncFileLog::m_fnLog(_logInstance, _logCategory, _level, pszLog); \
 		    } \
		} \
	}while(false)
#endif
#define LogNoGuard(_logInstance, _logCategory, _format, ...) \
	do \
	{ \
		unsigned int unMaxSingleLen = eSipUtil::AsyncFileLog::m_fnGetMaxSingleLineLen(_logInstance); \
		char * pszLog = (char *)malloc(unMaxSingleLen); \
		if( pszLog ) \
		{ \
			snprintf(pszLog, unMaxSingleLen-1,_format, ##__VA_ARGS__); \
			eSipUtil::AsyncFileLog::m_fnLog(_logInstance, _logCategory, pszLog); \
		} \
	}while(false)
#endif

