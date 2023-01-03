
#include <sys/time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Trsplog.hxx"
#include "lock.hxx"
#include "queueThread.hxx"
#include "ConfigFile.hxx"
#include "DelLog.hxx"

namespace eSipUtil
{

struct LogLevelInfo
{
	ELogLevel_t m_eLogLevel;
	const char * m_pszDesc;
};

static LogLevelInfo s_arrLogLevelInfo[E_LOG_NUM] =
{
	{E_LOG_DISABLE, 		"DISABLE"},
	{E_LOG_ERR, 				"ERR  "},
	{E_LOG_WARN,			"WARN "},
	{E_LOG_INFO, 				"INFO "},
	{E_LOG_DEBUG, 			"DEBUG"},
	{E_LOG_STACK, 			"STACK"},
};

const char * getLogLevel(ELogLevel_t _eLogLevel)
{
	static const char * pszUnknown = "Unknown_LogLevel";
		
	if( _eLogLevel >= 0 && _eLogLevel < E_LOG_NUM )
		return s_arrLogLevelInfo[_eLogLevel].m_pszDesc;
	else
		return pszUnknown;
}

class LogInstance;
struct InternalLogData
{
	LogInstance * m_pclsLogInstance;
	struct timeval m_stTv;
	int m_nLogCategoryIdx;
	ELogLevel_t m_eLogLevel;
	bool m_bNoGuard;
	char * m_pszLogStr;
	pthread_t m_stThreadID;    //pthread_self();

	InternalLogData()
		: m_pclsLogInstance(NULL), m_nLogCategoryIdx(-1), m_eLogLevel(E_LOG_DEBUG),m_bNoGuard(false), m_pszLogStr(NULL)
	{
		memset(&m_stTv, 0x00, sizeof(struct timeval));
	}

	~InternalLogData()
	{
		free(m_pszLogStr);
	}
};

class LogInstance
{
	public :
		LogInstance();
		~LogInstance();

		bool m_fnInit(const char * _pszFilePath, const char * _pszFileName, 
									int _nLogInstanceIdx, unsigned int _unMaxSingleLineLen, bool _bTbbEnable);
		bool m_fnSetMaxQueueSize(unsigned int _unMaxQueueSize);
		int m_fnGetMaxQueueSize() const;		
		int m_fnGetCurrentQueueSize() const;
		void m_fnSetCategoryStr(int _nLogCategoryIndx, const char * _pszStr);
		inline unsigned int m_fnGetMaxSingleLineLen() const{ return m_unMaxSingleLineLen; }
		void m_fnSetMaxFileSize(unsigned int _unSize);
		bool m_fnSetLogLevel(int _nLogCategoryIndx, ELogLevel_t _eLogLevel);
		ELogLevel_t m_fnGetLogLevel(int _nLogCategoryIndx);
		bool m_fnCheckLogLevel(int _nLogCategoryIndx, ELogLevel_t _eLogLevel);

		bool m_fnAddLogStr(InternalLogData * _pstLogData);
		inline bool m_fnGetUse() const{ return m_bUse; }
		inline const char * m_fnGetFilePath() const{ return m_szFilePath; }
		inline const char * m_fnGetFileName() const{ return m_szFileName; }

	private :
		static void m_fnLogProc(InternalLogData * _pstLogData, void * _pObject);
		static void m_fnWriteLogFormat(FILE * _pFile, InternalLogData & _rstLogData);
		static void m_fnWriteLog(FILE * _pFile, InternalLogData & _rstLogData);

		bool m_bUse;
		int m_nYear;
		int m_nMonth;
		int m_nDay;
		unsigned m_unFileIndex;
		FILE * m_pFile;
		char m_szFilePath[AsyncFileLog::E_CONST_MAX_FILE_PATH_LEN];
		char m_szFileName[AsyncFileLog::E_CONST_MAX_FILE_NAME_LEN];
		char m_szCurrentFileFullName[AsyncFileLog::E_CONST_MAX_FILE_FULL_NAME_LEN];
		unsigned int m_unMaxSingleLineLen;
		unsigned int m_unMaxFileSize;
		ELogLevel_t m_arrLogLevel[AsyncFileLog::E_CONST_MAX_CATEGORY_NUM];
		char m_arrCategoryStr[AsyncFileLog::E_CONST_MAX_CATEGORY_NUM][AsyncFileLog::E_CONST_MAX_CATEGORY_STR_LEN];
		QueueThread< InternalLogData > * m_pclsLogProcThread;
		
};

static Mutex s_clsMutex;
static LogInstance s_arrLogInstance[AsyncFileLog::E_CONST_MAX_INSTANCE_NUM];

PfuncQueueFullCallback AsyncFileLog::m_pfnQueueFullCb = NULL;

bool AsyncFileLog::m_fnInit(const char * _pszFilePath, const char * _pszFileName, int & _nLogInstanceIdx,
													unsigned int _unMaxSingleLineLen, unsigned int _unMaxFile, bool _bTbbEnable)
{
	CfgFile::m_fnCreateDir(_pszFilePath);
	Lock clsLock(s_clsMutex);

	if(KString::m_fnStrLen(_pszFilePath)==0 || KString::m_fnStrLen(_pszFileName)==0)
		return false;

	for( int i = 0; i < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM; ++i )
	{
		if( !s_arrLogInstance[i].m_fnGetUse() && s_arrLogInstance[i].m_fnInit(_pszFilePath, _pszFileName, i, _unMaxSingleLineLen, _bTbbEnable) )
		{
			_nLogInstanceIdx = i;
			g_fnCreateDelLog(_pszFilePath, _pszFileName,_unMaxFile,0);
			return true;
		}
	}

	return false;
}

bool AsyncFileLog::m_fnSetMaxQueueSize(int _nLogInstanceIdx, unsigned int _unMaxQueueSize)
{
	if( _nLogInstanceIdx >= 0 && _nLogInstanceIdx < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM )
		return s_arrLogInstance[_nLogInstanceIdx].m_fnSetMaxQueueSize(_unMaxQueueSize);
	else
		return false;
}

void AsyncFileLog::m_fnSetCategoryStr(int _nLogInstanceIdx, int _nLogCategoryIndx, const char * _pszStr)
{
	if( _nLogInstanceIdx >= 0 && _nLogInstanceIdx < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM )
		s_arrLogInstance[_nLogInstanceIdx].m_fnSetCategoryStr(_nLogCategoryIndx, _pszStr);
}

unsigned int AsyncFileLog::m_fnGetMaxSingleLineLen(int _nLogInstanceIdx)
{
	if( _nLogInstanceIdx >= 0 && _nLogInstanceIdx < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM )
		return s_arrLogInstance[_nLogInstanceIdx].m_fnGetMaxSingleLineLen();
	else
		return AsyncFileLog::E_CONST_DFT_SINGLE_LINE_LEN;
}

bool AsyncFileLog::m_fnSetMaxFileNum(int _nLogInstanceIdx,unsigned int _unMaxFile)  // 0 is unlimited
{
	Lock clsLock(s_clsMutex);
	if( _nLogInstanceIdx >= 0 && _nLogInstanceIdx < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM )
	return g_fnChangedDelLog(s_arrLogInstance[_nLogInstanceIdx].m_fnGetFilePath(),
							s_arrLogInstance[_nLogInstanceIdx].m_fnGetFileName(), 
							_unMaxFile,-1);
	return false;
}

void AsyncFileLog::m_fnSetMaxFileSize(int _nLogInstanceIdx, unsigned int _unSize)
{
	if( _nLogInstanceIdx >= 0 && _nLogInstanceIdx < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM )
		s_arrLogInstance[_nLogInstanceIdx].m_fnSetMaxFileSize(_unSize);
}

bool AsyncFileLog::m_fnSetLogLevel(int _nLogInstanceIdx, int _nLogCategoryIndx, ELogLevel_t _eLogLevel)
{
	if( _nLogInstanceIdx >= 0 && _nLogInstanceIdx < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM )
		return s_arrLogInstance[_nLogInstanceIdx].m_fnSetLogLevel(_nLogCategoryIndx, _eLogLevel);
	else
		return false;
}

ELogLevel_t AsyncFileLog::m_fnGetLogLevel(int _nLogInstanceIdx, int _nLogCategoryIndx)
{
	if( _nLogInstanceIdx >= 0 && _nLogInstanceIdx < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM )
		return s_arrLogInstance[_nLogInstanceIdx].m_fnGetLogLevel(_nLogCategoryIndx);
	else
		return E_LOG_NUM;
}

bool AsyncFileLog::m_fnCheckLogLevel(int _nLogInstanceIdx, int _nLogCategoryIndx, ELogLevel_t _eLogLevel)
{
	if( _nLogInstanceIdx >= 0 && _nLogInstanceIdx < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM )
		return s_arrLogInstance[_nLogInstanceIdx].m_fnCheckLogLevel(_nLogCategoryIndx, _eLogLevel);
	else
		return false;
}

void AsyncFileLog::m_fnSetQueueFullCallback(PfuncQueueFullCallback _pfnQueueFullCb)
{
	m_pfnQueueFullCb = _pfnQueueFullCb;
}

unsigned int AsyncFileLog::m_fnGetCurrentQueueSize(int _nLogInstanceIdx)
{
	if( _nLogInstanceIdx >= 0 && _nLogInstanceIdx < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM )
		return s_arrLogInstance[_nLogInstanceIdx].m_fnGetCurrentQueueSize();
	else
		return 0;
}

void AsyncFileLog::m_fnLog(int _nLogInstanceIdx, int _nLogCategoryIndx, ELogLevel_t _eLogLevel, char * _pszLogStr)
{
	if( _nLogInstanceIdx >= 0 && _nLogInstanceIdx < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM
		&& s_arrLogInstance[_nLogInstanceIdx].m_fnGetUse() )
	{
		InternalLogData * pstInternalLogData = new InternalLogData();
		if( pstInternalLogData )
		{		
			pstInternalLogData->m_pclsLogInstance = &(s_arrLogInstance[_nLogInstanceIdx]);
			gettimeofday(&(pstInternalLogData->m_stTv), NULL);
			pstInternalLogData->m_nLogCategoryIdx = _nLogCategoryIndx;
			pstInternalLogData->m_eLogLevel = _eLogLevel;
			pstInternalLogData->m_pszLogStr = _pszLogStr;
			pstInternalLogData->m_stThreadID = pthread_self();
			if( !s_arrLogInstance[_nLogInstanceIdx].m_fnAddLogStr(pstInternalLogData) && m_pfnQueueFullCb )
				m_pfnQueueFullCb(s_arrLogInstance[_nLogInstanceIdx].m_fnGetFileName(), s_arrLogInstance[_nLogInstanceIdx].m_fnGetCurrentQueueSize(), s_arrLogInstance[_nLogInstanceIdx].m_fnGetMaxQueueSize());
			
			return;
		}
	}

	free(_pszLogStr);
}
void AsyncFileLog::m_fnLog(int _nLogInstanceIdx, int _nLogCategoryIndx, char * _pszLogStr)
{
	if( _nLogInstanceIdx >= 0 && _nLogInstanceIdx < AsyncFileLog::E_CONST_MAX_INSTANCE_NUM
		&& s_arrLogInstance[_nLogInstanceIdx].m_fnGetUse() )
	{
		InternalLogData * pstInternalLogData = new InternalLogData();
		if( pstInternalLogData )
		{		
			pstInternalLogData->m_pclsLogInstance = &(s_arrLogInstance[_nLogInstanceIdx]);
			gettimeofday(&(pstInternalLogData->m_stTv), NULL);
			pstInternalLogData->m_nLogCategoryIdx = _nLogCategoryIndx;
			pstInternalLogData->m_bNoGuard = true;
			pstInternalLogData->m_pszLogStr = _pszLogStr;
			pstInternalLogData->m_stThreadID = pthread_self();
			if( !s_arrLogInstance[_nLogInstanceIdx].m_fnAddLogStr(pstInternalLogData) && m_pfnQueueFullCb )
				m_pfnQueueFullCb(s_arrLogInstance[_nLogInstanceIdx].m_fnGetFileName(), s_arrLogInstance[_nLogInstanceIdx].m_fnGetCurrentQueueSize(), s_arrLogInstance[_nLogInstanceIdx].m_fnGetMaxQueueSize());
			
			return;
		}
	}

	free(_pszLogStr);
}
LogInstance::LogInstance()
{
	m_bUse = false;
	m_nYear = 0; m_nMonth = 0; m_nDay = 0;
	m_unFileIndex = 0;
	m_pFile = NULL;
	m_szFilePath[0] = 0x00;
	m_szFileName[0] =  0x00;
	m_szCurrentFileFullName[0] = 0x00;
	m_unMaxFileSize = AsyncFileLog::E_CONST_DFT_FILE_SIZE;
	for( int i = 0; i < AsyncFileLog::E_CONST_MAX_CATEGORY_NUM; ++i )
	{
		m_arrLogLevel[i] = E_LOG_DEBUG;
		m_arrCategoryStr[i][0] = 0x00;
	}
	m_pclsLogProcThread = NULL;
	m_unMaxSingleLineLen = AsyncFileLog::E_CONST_DFT_SINGLE_LINE_LEN;
}

LogInstance::~LogInstance()
{
	if( m_pclsLogProcThread )
	{
		m_pclsLogProcThread->shutdown();
		m_pclsLogProcThread->join();
		delete m_pclsLogProcThread;
	}

	if( m_pFile )
		fclose(m_pFile);
}

bool LogInstance::m_fnInit(const char * _pszFilePath, const char * _pszFileName, 
													int _nLogInstanceIdx, unsigned int _unMaxSingleLineLen, bool _bTbbEnable)
{
	if (m_bUse || !_pszFilePath || !_pszFileName)
		return false;

	char szName[30];
	snprintf(szName, sizeof(szName) - 1, "LogInst%d", _nLogInstanceIdx);

	m_pclsLogProcThread = new QueueThread< InternalLogData >(AsyncFileLog::E_CONST_DFT_QUEUE_SIZE, m_fnLogProc, -1, szName, false, _bTbbEnable);
	if (m_pclsLogProcThread)
	{
		m_pclsLogProcThread->run();
		m_bUse = true;
		m_szFilePath[0] = 0x00;
		strncpy(m_szFilePath, _pszFilePath, AsyncFileLog::E_CONST_MAX_FILE_PATH_LEN - 1);
		m_szFilePath[AsyncFileLog::E_CONST_MAX_FILE_PATH_LEN - 1] = 0x00;
		m_szFileName[0] = 0x00;
		strncpy(m_szFileName, _pszFileName, AsyncFileLog::E_CONST_MAX_FILE_NAME_LEN - 1);
		m_szFileName[AsyncFileLog::E_CONST_MAX_FILE_NAME_LEN - 1] = 0x00;
		m_unMaxSingleLineLen = _unMaxSingleLineLen;
		return true;
	}
	else
		return false;
}

bool LogInstance::m_fnSetMaxQueueSize(unsigned int _unMaxQueueSize)
{
	if( m_pclsLogProcThread )
	{
		m_pclsLogProcThread->setMaxQueueSize(_unMaxQueueSize);
		return true;
	}
	else
		return false;
}

int LogInstance::m_fnGetMaxQueueSize() const
{
	if( m_pclsLogProcThread )
	{
		return m_pclsLogProcThread->getMaxQueueSize();
	}

	return 0;
}

int LogInstance::m_fnGetCurrentQueueSize() const
{
	if( m_pclsLogProcThread )
	{
		return m_pclsLogProcThread->getCurrentQueueSize();
	}

	return 0;
}

void LogInstance::m_fnSetCategoryStr(int _nLogCategoryIndx, const char * _pszStr)
{
	if( m_bUse && _nLogCategoryIndx >= 0 && _nLogCategoryIndx < AsyncFileLog::E_CONST_MAX_CATEGORY_NUM )
	{
		strncpy(m_arrCategoryStr[_nLogCategoryIndx], _pszStr, AsyncFileLog::E_CONST_MAX_CATEGORY_STR_LEN-1);
		m_arrCategoryStr[_nLogCategoryIndx][AsyncFileLog::E_CONST_MAX_CATEGORY_STR_LEN-1] = 0x00;
	}
}

void LogInstance::m_fnSetMaxFileSize(unsigned int _unSize)
{
	if( m_bUse )
		m_unMaxFileSize = _unSize;
}

bool LogInstance::m_fnSetLogLevel(int _nLogCategoryIndx, ELogLevel_t _eLogLevel)
{
	if( m_bUse && _nLogCategoryIndx >= 0 && _nLogCategoryIndx < AsyncFileLog::E_CONST_MAX_CATEGORY_NUM 
		&& _eLogLevel >= E_LOG_DISABLE && _eLogLevel < E_LOG_NUM )
	{
		m_arrLogLevel[_nLogCategoryIndx] = _eLogLevel;
		return true;
	}
	else
		return false;
}

ELogLevel_t LogInstance::m_fnGetLogLevel(int _nLogCategoryIndx)
{
	if( m_bUse && _nLogCategoryIndx >= 0 && _nLogCategoryIndx < AsyncFileLog::E_CONST_MAX_CATEGORY_NUM )
		return m_arrLogLevel[_nLogCategoryIndx];
	else
		return E_LOG_NUM;
}

bool LogInstance::m_fnCheckLogLevel(int _nLogCategoryIndx, ELogLevel_t _eLogLevel)
{
	if( m_bUse && _nLogCategoryIndx >= 0 && _nLogCategoryIndx < AsyncFileLog::E_CONST_MAX_CATEGORY_NUM 
		&& _eLogLevel >= E_LOG_ERR && _eLogLevel < E_LOG_NUM )
	{
		if( m_arrLogLevel[_nLogCategoryIndx] != E_LOG_DISABLE && m_arrLogLevel[_nLogCategoryIndx] >= _eLogLevel )
			return true;
		else
			return false;
	}
	else
		return false;
}

bool LogInstance::m_fnAddLogStr(InternalLogData * _pstLogData)
{
	if( m_bUse && _pstLogData && m_pclsLogProcThread )
	{	
		if( m_pclsLogProcThread->put(_pstLogData) )
		{		
			return true;
		}
		else
		{		
			delete _pstLogData;
			return false;
		}
	}
	else
	{
		delete _pstLogData;
		return true;
	}
}

void LogInstance::m_fnLogProc(InternalLogData * _pstLogData, void * _pObject)
{
	if( !_pstLogData 
		|| !_pstLogData->m_pclsLogInstance 
		|| !_pstLogData->m_pclsLogInstance->m_fnGetUse()
		|| !_pstLogData->m_pszLogStr )
	{
		delete _pstLogData;
		return;
	}

	LogInstance & rclsLogInstance = *(_pstLogData->m_pclsLogInstance);

	/*==== check changing day ====*/
	time_t currentTime = time(NULL);
	struct tm currentTm;
	localtime_r(&currentTime, &currentTm);
	bool bChangeTime = false;
	if( rclsLogInstance.m_nYear != currentTm.tm_year || rclsLogInstance.m_nMonth != currentTm.tm_mon || rclsLogInstance.m_nDay != currentTm.tm_mday )
		bChangeTime = true;

	/*==== check current log file ====*/
	if( rclsLogInstance.m_pFile )
	{
		if( strlen(rclsLogInstance.m_szCurrentFileFullName) > 0 )
		{
			struct stat stFileInfo;
			if( stat(rclsLogInstance.m_szCurrentFileFullName, &stFileInfo) != 0 || !S_ISREG(stFileInfo.st_mode) )
			{
				fclose(rclsLogInstance.m_pFile);
				rclsLogInstance.m_pFile = NULL;
				rclsLogInstance.m_szCurrentFileFullName[0] = 0x00;
			}
		}
		else
		{
			fclose(rclsLogInstance.m_pFile);
			rclsLogInstance.m_pFile = NULL;
			rclsLogInstance.m_szCurrentFileFullName[0] = 0x00;
		}
	}

	/*==== create new log file ====*/
	if( !rclsLogInstance.m_pFile 
		|| (ftell(rclsLogInstance.m_pFile)+strlen(_pstLogData->m_pszLogStr)) >= rclsLogInstance.m_unMaxFileSize 
		|| bChangeTime )
	{
		snprintf(rclsLogInstance.m_szCurrentFileFullName, sizeof(rclsLogInstance.m_szCurrentFileFullName)-1, "%s%s.%04d%02d%02d", 
						rclsLogInstance.m_szFilePath, 
						rclsLogInstance.m_szFileName, 
						currentTm.tm_year+1900, 
						currentTm.tm_mon+1, 
						currentTm.tm_mday);

		if( bChangeTime )
		{
			rclsLogInstance.m_unFileIndex = 0;
			rclsLogInstance.m_nYear = currentTm.tm_year;
			rclsLogInstance.m_nMonth = currentTm.tm_mon;
			rclsLogInstance.m_nDay = currentTm.tm_mday;
		}

		/*==== get last index of file ====*/
		if( !rclsLogInstance.m_pFile )
		{
			DIR * pDir = NULL;
			struct dirent * pEntry = NULL;
			struct stat stFileInfo;
			unsigned int unLastIndex = 0;               
			if( (pDir = opendir(rclsLogInstance.m_szFilePath)) )
			{
				while( (pEntry = readdir(pDir)) )
				{
					if( pEntry->d_ino == 0 )  //deleted file
						continue;

					char szTmpFullFileName[AsyncFileLog::E_CONST_MAX_FILE_PATH_LEN+AsyncFileLog::E_CONST_MAX_FILE_NAME_LEN+20];
					#if 1 // bug fixed
					memset(szTmpFullFileName,0x00,AsyncFileLog::E_CONST_MAX_FILE_PATH_LEN+AsyncFileLog::E_CONST_MAX_FILE_NAME_LEN+20);
					#endif
					snprintf(szTmpFullFileName, sizeof(szTmpFullFileName)-1, "%s%s", rclsLogInstance.m_szFilePath, pEntry->d_name);
					if( stat(szTmpFullFileName, &stFileInfo) == 0 && S_ISREG(stFileInfo.st_mode) )  //file
					{
						if( strncmp(szTmpFullFileName, rclsLogInstance.m_szCurrentFileFullName, strlen(rclsLogInstance.m_szCurrentFileFullName)) == 0 )
						{
							const char * pLastIndex = szTmpFullFileName+strlen(rclsLogInstance.m_szCurrentFileFullName)+1;
							if( strlen(pLastIndex) > 0 )
							{
								unsigned int unFileIndex = atoi(pLastIndex);
								if( unFileIndex > unLastIndex )
									unLastIndex = unFileIndex;
							}
						}
					}
				}
				
				closedir(pDir);
			}
			rclsLogInstance.m_unFileIndex = unLastIndex;
		}
		strncat(rclsLogInstance.m_szCurrentFileFullName, sizeof(rclsLogInstance.m_szCurrentFileFullName), ".%d", rclsLogInstance.m_unFileIndex);
		++(rclsLogInstance.m_unFileIndex);

		if( rclsLogInstance.m_pFile )
			fclose(rclsLogInstance.m_pFile);		
		rclsLogInstance.m_pFile = fopen(rclsLogInstance.m_szCurrentFileFullName, "a");
		if( rclsLogInstance.m_pFile )
		{
			chmod(rclsLogInstance.m_szCurrentFileFullName, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
			setvbuf(rclsLogInstance.m_pFile, NULL, _IOFBF, 12000);
		}
		else
			rclsLogInstance.m_szCurrentFileFullName[0] = 0x00;
	}

	if( rclsLogInstance.m_pFile )
	{
		if(_pstLogData->m_bNoGuard == false) m_fnWriteLogFormat(rclsLogInstance.m_pFile, *_pstLogData);
		else m_fnWriteLog(rclsLogInstance.m_pFile, *_pstLogData);
	}

	delete _pstLogData;
}

void LogInstance::m_fnWriteLogFormat(FILE * _pFile, InternalLogData & _rstLogData)
{	
	struct tm tmTemp;
	time_t m_time = (time_t)_rstLogData.m_stTv.tv_sec;
	long m_usec = _rstLogData.m_stTv.tv_usec;
	int m_msec = (int)((m_usec + 500) / 1000);
	localtime_r(&m_time, &tmTemp);
	char szTime[20]; memset(szTime,0x00,20);
	snprintf(szTime, sizeof(szTime)-1, "%02d:%02d:%02d.%03d", 
										tmTemp.tm_hour, tmTemp.tm_min, tmTemp.tm_sec, m_msec);
	fwrite(szTime, 1, strlen(szTime), _pFile);
	fwrite(") ", 1, 2, _pFile);
	char szTid[30]; memset(szTid,0x00,sizeof(szTid));
	snprintf(szTid, sizeof(szTid)-1,"TID-0x%08x) ",(unsigned int)_rstLogData.m_stThreadID);
	fwrite(szTid,1, strlen(szTid),_pFile);
	fwrite(getLogLevel(_rstLogData.m_eLogLevel), 1, strlen(getLogLevel(_rstLogData.m_eLogLevel)), _pFile);
	fwrite(") ", 1, 2, _pFile);
	if( _rstLogData.m_pclsLogInstance
		&& _rstLogData.m_nLogCategoryIdx >= 0 
		&& _rstLogData.m_nLogCategoryIdx < AsyncFileLog::E_CONST_MAX_CATEGORY_NUM
		&& strlen(_rstLogData.m_pclsLogInstance->m_arrCategoryStr[_rstLogData.m_nLogCategoryIdx]) > 0 )
	{
		fwrite(_rstLogData.m_pclsLogInstance->m_arrCategoryStr[_rstLogData.m_nLogCategoryIdx], 1, 
			strlen(_rstLogData.m_pclsLogInstance->m_arrCategoryStr[_rstLogData.m_nLogCategoryIdx]), _pFile);
		fwrite(") ", 1, 2, _pFile);
	}
	fwrite(_rstLogData.m_pszLogStr, 1, strlen(_rstLogData.m_pszLogStr), _pFile);
	fwrite("\n", 1, strlen("\n"), _pFile);
	fflush(_pFile);
}
void LogInstance::m_fnWriteLog(FILE * _pFile, InternalLogData & _rstLogData)
{
	if(_rstLogData.m_bNoGuard == false) return;	
	fwrite(_rstLogData.m_pszLogStr, 1, strlen(_rstLogData.m_pszLogStr), _pFile);
	fwrite("\n", 1, strlen("\n"), _pFile);
	fflush(_pFile);
}
#include <execinfo.h>
#include "string.hxx"
Debugging::Debugging(const char * _pszFunc,unsigned int _unFuncStackID)
{
	m_unFuncStackID = _unFuncStackID;
	memset(m_szFunc,0x00,128);KString::m_fnStrnCat(m_szFunc,128,_pszFunc);
	memset(m_szReason,0x00,128);
	pthread_t tid = pthread_self(); m_unThreadID = (unsigned int)tid;
	LogInternal(E_LOG_ERR,"%s[T(%u):S(%u)] Start",m_szFunc,m_unThreadID,m_unFuncStackID);
}
Debugging::~Debugging()
{
	if(m_szReason[0] !=0)
		LogInternal(E_LOG_ERR,"%s[T(%u):S(%u)] End (Reason : %s)",m_szFunc,m_unThreadID,m_unFuncStackID,m_szReason);
	else
		LogInternal(E_LOG_ERR,"%s[T(%u):S(%u)] End (Reason : Normal)",m_szFunc,m_unThreadID,m_unFuncStackID);
}
void Debugging::m_fnSetReason(const char * _pszFormat,...)
{
	memset(m_szReason,0x00,128);
	va_list ap;
	va_start(ap, _pszFormat);
	vsnprintf(m_szReason,127,_pszFormat, ap);
	va_end(ap);
}
void g_fnDebugPoint()
{
	void  *array[100];int i = 0;
	int  nsize = backtrace(array, 100);if ( nsize <= 0 ) return;
	char **symbols = backtrace_symbols(array, nsize);if( symbols == NULL) return;
	char szLog[10240]; memset(szLog,0x00,10240);
	pthread_t tid = pthread_self(); KString::m_fnStrnCat(szLog,10240,"T(%u)\r\n",(unsigned int)tid);
	for(i=0 ;i < nsize ; i++)
	{
		if( symbols[nsize] )
		{
			KString::m_fnStrnCat(szLog,10240," %2.i: %s\r\n", i + 1, symbols[i]);
		}
	}  
	free(symbols);LogInternal(E_LOG_ERR,szLog);
}
}

