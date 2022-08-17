#ifndef _ESIPUTIL_DIRECTORY_H_
#define _ESIPUTIL_DIRECTORY_H_
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "StlMap.hxx"
#include "string.hxx"

namespace eSipUtil
{
typedef enum
{
	E_FMOD_NONE,
	E_FMOD_DIR,
	E_FMOD_FILE,
	E_FMOD_LINK,
	E_FMOD_CHAR,
	E_FMOD_BLOCK,
	E_FMOD_FIFO,
	E_FMOD_SOC,
}FileMode_t;
class DirFile;
class Directorys;
typedef void (*PFuncNotiDirInfo)(DirFile * _pclsFile, Directorys * _pclsDir, void * _pvUser);  // Dir == NULL or File == NULL

class DirFile : public StlObject
{
	public:
		DirFile();
		~DirFile();
		void m_fnDebug(KString &_rclsDebug);
		unsigned int m_fnGetLastTime(){return (unsigned int)m_stStat.st_mtime;}
		unsigned int m_fnGetSize(){return (unsigned int)m_stStat.st_size;}
		KString m_clsPath;
		KString m_clsName;
		struct stat m_stStat;
};
class Directorys : public StlObject
{
	public:
		Directorys();
		~Directorys();
		void m_fnClear();
		bool m_fnRead(const char * _pszPath);
		void m_fnDebug(KString &_rclsDebug);
		void m_fnNotifyDirInfo(PFuncNotiDirInfo _pfnNotify,void * _pvUser);
		static FileMode_t m_fnGetMode(struct stat * _pstStat);
		static const char * m_fnGetStringFileMode(struct stat * _pstStat);
		static bool m_fnIsLinkDir(const char * _pszDir);
		static void m_fnRemoveDir(const char * _pszDirPath);
		static void m_fnCatPath(KString &_rclsPath);
		static void m_fnDelExpireTimeFile(const char * _pszDir, unsigned int _unExpSec,unsigned int * _punDepth);
		static void m_fnDelEmptyDir(const char * _pszDirPath,unsigned int * _punDepth);
		Directorys * m_fnBeginDir(ListItr_t &_ritrItem){return (Directorys*)m_listChildDir.m_fnBegin(_ritrItem);}
		Directorys * m_fnNextDir(ListItr_t &_ritrItem){return (Directorys*)m_listChildDir.m_fnNext(_ritrItem);}
		DirFile * m_fnBeginFile(ListItr_t &_ritrItem){return (DirFile*)m_listChildFile.m_fnBegin(_ritrItem);}
		DirFile * m_fnNextFile(ListItr_t &_ritrItem){return (DirFile*)m_listChildFile.m_fnNext(_ritrItem);}
		KString m_clsPath;
	private:
		Directorys * m_fnAddChildDir();
		DirFile * m_fnAddChildFile();
		KString m_clsTmpPath;
		DIR *m_pstDir;
		struct dirent *m_pstEntry;
		struct stat m_stStat;
		StlList m_listChildFile;
		StlList m_listChildDir;
		DirFile * m_pclsTmpFile;
		Directorys *m_pclsTmpDir;
};
}
#endif
