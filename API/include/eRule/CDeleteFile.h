#ifndef DELETEFILE_H
#define DELETEFILE_H
#include "CRuleUtiles.h"

#define MAX_DEL_LOG_FILE     8
#define MAX_DEL_LOG_EXPIRE    2592000      // 30 days
typedef enum
{
	E_FMODE_NONE,
	E_FMODE_DIR,
	E_FMODE_FILE,
	E_FMODE_LINK,
	E_FMODE_CHAR,
	E_FMODE_BLOCK,
	E_FMODE_FIFO,
	E_FMODE_SOC,
}EFileMode_t;
extern const char * g_arrDFileMode[E_FMODE_SOC+1];
class DeleteFile
{
	public:
		DeleteFile();
		~DeleteFile();
		void fnInit(const char * _pszDirPath, const char * _pszFileName, 
						unsigned int _unMaxLimit,unsigned int _unExpireTime=0);
		bool fnReadDirectory();    // must function-call periodically(example : 1 sec)
		void fnChangePath(const char * _pszDirPath)
		{
			m_clsLock.Lock();
			m_clsDirName = _pszDirPath;
			m_clsLock.Unlock();
		}
		void fnChangeFileName(const char * _pszFileName)
		{
			m_clsLock.Lock();
			m_clsDirName = _pszFileName;
			m_clsLock.Unlock();
		}
		// Locking Value
		RUString m_clsFileName;
		RUString m_clsDirName;
		// Config Value
		unsigned int m_unExipreSec;
		unsigned int m_unMaxLimitedFileNum;
		bool m_bAllMode;
	private:
		void fnClear();
		bool fnMatchFile(char *_pszFileName);
		bool fnCheckExpireTime(char *_pszFileName,struct stat *pstat);
		bool fnRemoveFile(char * _pszFileName, EFileMode_t _eMode);
		bool fnUpdateLastFile(struct dirent *entry, struct stat *pstat);
		int fnSysCommand(const char *pFormat, ...);
		EFileMode_t fnGetMode(struct stat * pstat);
		RULock m_clsLock;
		// Temporary Value
		unsigned int m_unLastTime;
		RUString m_clsLastDName;
		EFileMode_t m_eLastMode;
		unsigned int m_unFileCnt;
};

#endif
