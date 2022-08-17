#ifndef _ESIPUTIL_DELLOG_H_
#define _ESIPUTIL_DELLOG_H_
#include "Directory.hxx"
#include "lock.hxx"

namespace eSipUtil
{
/***************************** API Functions **********************************************/
//----> create dellog function
void g_fnCreateDelLog(const char * _pszDirPath, const char * _pszFileName, 
												unsigned int _unMaxLimit,unsigned int _unExpireTime);
//----> remove dellog function
void g_fnTerminateDelLog(const char * _pszDirPath, const char * _pszFileName);
//----> change Info, -1(no changing)
bool g_fnChangedDelLog(const char * _pszDirPath, const char * _pszFileName, 
																int _nMaxLimit,int _nExpireTime);
/***************************** DelLog File Info *********************************************/
class DelLogMgr;
class DelLog : public StlObject
{
	friend class DelLogMgr;
	public:
		DelLog();
		~DelLog();
		void m_fnInit(const char * _pszDirPath, const char * _pszFileName, 
						unsigned int _unMaxLimit,unsigned int _unExpireTime=0);
		bool m_fnReadAndDelete();    // must function-call periodically(example : 1 sec)
		KString m_clsFileName;
		KString m_clsDirName;
		unsigned int m_unExipreSec;
		unsigned int m_unMaxLimitedFileNum;
		bool m_bAllMode;
	private:
		void m_fnClear();
		bool m_fnMatchFile(char *_pszFileName);
		bool m_fnCheckExpireTime(char *_pszFileName,struct stat *pstat);
		bool m_fnRemoveFile(char * _pszFileName, FileMode_t _eMode);
		bool m_fnUpdateLastFile(struct dirent *entry, struct stat *pstat);
		// Temporary Value
		unsigned int m_unLastTime;
		KString m_clsLastDName;
		FileMode_t m_eLastMode;
		unsigned int m_unFileCnt;
};
/**************************** Del Log Manager *********************************************/
class DelLogMgr
{
	public:
		DelLogMgr();
		~DelLogMgr();
		bool m_fnAdd(const char * _pszDirPath, const char * _pszFileName, 
								unsigned int _unMaxLimit,unsigned int _unExpireTime=0);
		DelLog * m_fnFind(const char * _pszDirPath,const char * _pszFileName);
		bool m_fnDel(const char * _pszDirPath,const char * _pszFileName);
		bool m_fnChangeInfo(const char * _pszDirPath, const char * _pszFileName, 
																int _nMaxLimit, int _nExpireTime);
		void m_fnClear();
		void m_fnReadAndDelete();
		static bool m_fnGenKey(const char * _pszPath, const char * _pszFileName, KString &_rclsResult);
	private:
		DelLog * m_fnBegin(ListItr_t &_ritrItem){return (DelLog*)m_listDelFile.m_fnBegin(_ritrItem);}
		DelLog * m_fnNext(ListItr_t &_ritrItem){return (DelLog*)m_listDelFile.m_fnNext(_ritrItem);}
		RwMutex m_clsLock;
		StlList m_listDelFile;
};

}
#endif

