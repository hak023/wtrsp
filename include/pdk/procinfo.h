
#ifndef __PROCINFO_H__
#define __PROCINFO_H__

#include <stdio.h>
/*
#if defined(SUNOS)
#	include <kstat.h>
#	ifdef SWIG_GLOBAL
#		undef _FILE_OFFSET_BITS
#	endif // SWIG_GLOBAL
#		include <procfs.h>
#	ifdef SWIG_GLOBAL
#		define _FILE_OFFSET_BITS 64
#	endif // SWIG_GLOBAL
#elif defined(_LINUX_)
#	include <fstream.h>
#endif

#include <string.h>
*/
//fish 추가
#if defined(_LINUX_)
#	include <fstream>
#elif defined(UNIX_SVR4)
#	include <kstat.h>
#	ifdef SWIG_GLOBAL
#		undef _FILE_OFFSET_BITS
#	endif // SWIG_GLOBAL
#		include <procfs.h>
#	ifdef SWIG_GLOBAL
#		define _FILE_OFFSET_BITS 64
#	endif // SWIG_GLOBAL
#endif
#include <string>

//fish 수정end

#include <dirent.h>
#include <sys/param.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "base.h"
#include "list.h"
#include "logger.h"

#define MAXLINE 512

class CProcInfo : public CObject
{
public:
	CProcInfo();
	~CProcInfo();

	typedef struct {
		pid_t uid, pid, ppid, pgid;
		pid_t parent, child, sibling;
		char cmd[MAXLINE];
	} PROC, *PPROC;

	int Update(void);
	bool Exist(pid_t pid);
	PPROC GetProc(pid_t pid);
	PPROC GetChild(pid_t pid);
	PPROC GetParent(pid_t pid);
	PPROC GetSibling(pid_t pid);
#ifdef _LINUX_
	PPROC GetProc(LPCSTR pszcmd, pid_t pid = 0);
#else // _LINUX_
	PPROC GetProc(LPCSTR pszcmd, pid_t pid = 1);
#endif // _LINUX_

#if 0
	CMapPtrToPtr *GetProcMap(void) { return m_pProcMap; }
#endif

	int GetCount(void) { return m_pProcMap->GetCount(); }
	void ViewTree(pid_t pid, int nDepth = 0);
protected:
	void MakeTree(void);
	void Clear(void);

#ifdef _LINUX_
	istream &getline(istream &is, CString &str) {
		char buf[2048];
		is.getline(buf, 2048);
		str = buf;
		return is;
	}
#endif
	DIR				*m_pProcDir;
	CMapPtrToPtr	*m_pProcMap;
	CLogger			*m_pLogger;
};

#endif // __PROCINFO_H__
