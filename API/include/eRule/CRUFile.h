#ifndef CRUFILE_H
#define CRUFILE_H
#include "CRuleUtiles.h"

inline bool g_fnIsDir(const char *pname)
{
   struct stat buf;
   if (stat(pname, &buf) != 0) return false;
   if ((buf.st_mode & S_IFDIR) == S_IFDIR) return true;
   else return false;
}
inline void g_fnCreateDir(const char * _pszPath)
{
	// ../log/abc/def  
	// ./log/abc/def
	// log/abc/def
	// /home/abc/def
	 unsigned int unSlashCnt = 0;
	 for(int i = 0; i< fnRUStrLen(_pszPath);i++)
 	{
		if(_pszPath[i] == '/')
		{
			unSlashCnt++;
			if(unSlashCnt >= 1)
			{
				char szTmp[1024];memcpy(szTmp,_pszPath,i+1);szTmp[i+1]=0;
				if (g_fnIsDir(szTmp) == false){umask(0); mkdir(szTmp,0777);}
			}			
		}
		
 	}
	if (g_fnIsDir(_pszPath) == false){umask(0); mkdir(_pszPath,0777);}
}
inline void g_fnCreateDirExeptFile(const char * _szPath)
{
	unsigned int unLen = fnRUStrLen(_szPath);
	unsigned int unSlashCnt = 0;
	unsigned int unLastSlashPos = 0;
	char szTmp[512];memset(szTmp,0x00,512);
	fnRUStrnCatCpy(szTmp,_szPath,512);
	szTmp[unLen]=0;
	for(unsigned int i=0;i<unLen;i++)
	{
		if(szTmp[i] == '/')
		{
			unSlashCnt++; 
			unLastSlashPos = i;
		}
	}
	if(unSlashCnt > 1)
	{
		szTmp[unLastSlashPos] = 0;
		g_fnCreateDir(szTmp);
	}
}
inline unsigned int g_fnGetDirFileNum(const char * _pszPath)
{
	//char szTemp[1024]; memset(szTemp,0x00,1024); struct stat f_stat;
	 DIR *dp = NULL;struct dirent *entry = NULL; unsigned int unCnt = 0;
	if((dp=opendir(_pszPath)) == NULL) return 0;
	while((entry = readdir(dp)) != NULL)
	{
		//memset(szTemp,0x00,256);
		//fnRUStrnCat(szTemp,256,"%s%s",_pszPath,entry->d_name);
		//stat(szTemp, &f_stat);
		//if(S_ISREG(f_stat.st_mode)) unCnt++;
		unCnt++;
	}
	closedir(dp);
	//if(unCnt >= 2) unCnt-=2;
	return unCnt;
}
inline bool g_fnIsNotExistFile(const char * _pszPath, const char * _pszFile)
{
	DIR *dp = NULL;struct dirent *entry = NULL;struct stat f_stat;
	char szTemp[256]; memset(szTemp,0x00,256);
	if((dp=opendir(_pszPath)) == NULL) return false;
	while((entry = readdir(dp)) != NULL)
	{
		fnRUStrnCat(szTemp,256,"%s%s",_pszPath,entry->d_name);	
		//lstat(entry->d_name,&f_stat);
		memset(&f_stat,0x00,sizeof(struct stat)); stat(szTemp, &f_stat);
		if((S_ISREG(f_stat.st_mode)))
		{
			if(fnRUStrnCmp(entry->d_name,_pszFile,fnRUStrLen(_pszFile))>0)
			{ closedir(dp);return true;}
		}
	}
	closedir(dp);
	return false;
}
inline void g_fnRmDir_RF(const char * _pszDirName) 
{
	DIR * dirHandle = NULL; dirent * entry = NULL; struct stat f_stat;
	chmod(_pszDirName,0777);
	dirHandle = opendir(_pszDirName);
	if (dirHandle) 
	{
		while ((entry = readdir(dirHandle)) != NULL) 
		{
			if (fnRUStrCmp(entry->d_name,".")==0 || fnRUStrCmp(entry->d_name,"..")==0) continue;
			char * pszTmp = new char [256]; memset(pszTmp,0x00,256);
			fnRUStrnCat(pszTmp,256,"%s/%s",_pszDirName,entry->d_name);
			memset(&f_stat,0x00,sizeof(struct stat));stat(pszTmp,&f_stat);
			if(S_ISDIR(f_stat.st_mode)) g_fnRmDir_RF(pszTmp);
			else{chmod(pszTmp,0777); unlink(pszTmp);}
			delete [] pszTmp;
		}
		closedir(dirHandle);
		rmdir(_pszDirName);
	}
}
class RUFile
{
	public:
		enum BufferCommand { bufferRead, bufferWrite, bufferCommit, bufferCheck };
		enum OpenFlags 
		{
			modeRead =        0x0000,
			modeWrite =       0x0001,
			modeReadWrite =   0x0002,
			modeCreate =      0x1000,
			modeNoTruncate =  0x2000,
			modeNoAppend   =  0x5000,
			typeText =        0x4000,
			typeBinary = (int)0x8000
		};
		enum SeekPosition 
		{
			begin = SEEK_SET,
			current = SEEK_CUR,
			end = SEEK_END
		};
		enum { hFileNull = -1, MAX_FILE_NAME = 256 };
		RUFile();
		RUFile(int hFile);
		RUFile(const char* lpszFileName, unsigned int nOpenFlags, mode_t rmode=0);
		~RUFile();
		operator int() const;
		unsigned long GetPosition() const;
		char * GetFileName() {return m_strFileName;}
		bool Open(const char* lpszFileName, unsigned int nOpenFlags, mode_t rmode=0);
		static void Rename(const char* lpszOldName, const char* lpszNewName);
		static void Remove(const char* lpszFileName);
		unsigned long SeekToEnd();
		void SeekToBegin();
		unsigned long ReadHuge(void *lpBuffer, unsigned long dwCount);
		void WriteHuge(const void *lpBuffer, unsigned long dwCount);
		RUFile *Duplicate() const;
		unsigned long Seek(unsigned long lOff, unsigned int nFrom);
		unsigned long GetLength() const;
		unsigned int Read(void *lpBuf, unsigned int nCount);
		unsigned int Write(const void *lpBuf, unsigned int nCount);
		void Flush();
		void Close();
		void Abort();
		unsigned int GetBufferPtr(unsigned int nCommand, unsigned int nCount = 0,
		    void **ppBufStart = NULL, void **ppBufMax = NULL);
		int m_hFile;
		bool m_bCloseOnDelete;
		char m_strFileName[MAX_FILE_NAME];
};
inline RUFile::operator int() const{ return m_hFile; }
inline unsigned long RUFile::ReadHuge(void *lpBuffer, unsigned long dwCount)
	{ return (unsigned long)Read(lpBuffer, (unsigned int)dwCount); }
inline void RUFile::WriteHuge(const void *lpBuffer, unsigned long dwCount)
	{ Write(lpBuffer, (unsigned int)dwCount); }
inline unsigned long RUFile::SeekToEnd(){ return Seek(0, RUFile::end); }
inline void RUFile::SeekToBegin(){ Seek(0, RUFile::begin); }

typedef void (*PFuncFileChanged_t)(void * _pvFile);
typedef struct FileChangedInfo
{
	FileChangedInfo(){fnClear();}
	void fnClear()
	{
		m_bOn = 0;memset(path,0x00,256);	m_nPrevInfo = 0;	
		m_nNextInfo = 0;m_pvConfig = NULL;m_pfnChanged = NULL;
	}
	void fnSet(char *mpath,void *mCfg,PFuncFileChanged_t pFn)
	{
		if(fnRUStrLen(mpath) == 0) return;
		m_bOn = 1;fnRUStrCpy(path,mpath);stat(path,&m_stFileInfo);m_nNextInfo = (int)(m_stFileInfo.st_mtime);
		m_nPrevInfo = m_nNextInfo;m_pvConfig = mCfg;m_pfnChanged = pFn;
	}
	bool fnIsActive()
	{
		if(!m_bOn) return false;
		if(fnRUStrLen(path) == 0) return false;
		if(fnGetFileSize() == 0) return false;  
		return true;
	}
	unsigned int fnGetFileSize()
	{
		if(!m_bOn) return 0;
		if(fnRUStrLen(path) == 0) return 0;
		struct stat st;int rc = stat(path, &st);
		if( rc != 0 ) return 0;
		if(st.st_size == 0) return 0;
		return (unsigned int)(st.st_size);
	}
	bool fnCheckProc()
	{
		if(!fnIsActive()) return false;
		stat(path,&m_stFileInfo);m_nNextInfo = (int)(m_stFileInfo.st_mtime);
		if(m_nPrevInfo != m_nNextInfo)
		{
			m_nPrevInfo = m_nNextInfo;
			if(m_pfnChanged) m_pfnChanged(m_pvConfig);
			return true;
		}
		m_nPrevInfo = m_nNextInfo;
		return false;
	}
	char m_bOn;
	char path[256];
	struct stat m_stFileInfo;
	int m_nPrevInfo;
	int m_nNextInfo;
	void *m_pvConfig;
	PFuncFileChanged_t m_pfnChanged;
}FileChangedInfo;

#endif

