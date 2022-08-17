

#ifndef __PNR_H__
#define __PNR_H__

#include <sys/types.h>
#include <signal.h>
#if !defined(_HPUX_)
#include <wait.h>
#endif

#include "base.h"
#include "list.h"
#include "util.h"
#include "logger.h"
#include "qthread.h"
#include "procinfo.h"

/* typedef bool (*CProcNotify)(void *pInfo); */
#define CProcNotify PCallBack
#define PROC_START_CHILD    0x1001
#define PROC_STOP_CHILD     0x1002
#ifndef MAX_NUM_OF_ARGVS
#	define MAX_NUM_OF_ARGVS	 8
#endif // MAX_NUM_OF_ARGVS

typedef enum {
	PROC_TYPE_NORM,
	PROC_TYPE_HA
} PROCTYPE;

///////////////////////////////////////////////////////////////////////////////
class CPnr;
class CProcess : public CQThread
{
	DECLARE_DYNAMIC(CProcess)

public:
	CProcess();
	~CProcess();
	bool Start(CPnr *pOwner, int nIndex, int nId,
		LPCSTR pszParam, PROCTYPE nProcType = PROC_TYPE_NORM,
		LPCSTR pszExtCmd = NULL);
	bool GetValid() { return m_nValid == 1; }
	int GetIndex() { return m_nIndex; }
	pid_t GetPid()	{ return m_aryPid.GetSize() > 0 ? m_aryPid[0] : -1; }
	bool GetAutoFork() { return m_bAutoFork; }
	void SetAutoFork(bool bAutoFork) { m_bAutoFork = bAutoFork; }
	LPCSTR GetName() { return (LPCSTR)m_strName; }
	LPCSTR GetAlias() { return (LPCSTR)m_strAlias; }
	CArray<CString,CString> &GetParam() { return m_aryParam; }
	PROCTYPE GetType(void) { return m_nProcType; }
	bool IsNoWait() { return m_bNoWait; }
	void StartProc(void) { AddEvent(PROC_START_CHILD, 0); }
	void StopProc(void) { AddEvent(PROC_STOP_CHILD, 0); }

	bool StartTimer(int msec, PDK8U which = 0);
	bool StopTimer(PDK8U which = 0);

protected:
	void *ThreadProc(void);
	pid_t Fork(void);
	void Kill(void);

	int				m_nValid;	// valid flag(if 0, this is an invalid process)
	int				m_nIndex;	// index of the array, PROCESS_STATUS, in the shared memory
	CArray<pid_t,pid_t> m_aryPid;
	bool				m_bAutoFork;
	PROCTYPE			m_nProcType;

	CString			m_strName;	// process name
	CString			m_strAlias;	// process name(alias)
	CArray<CString,CString>	m_aryParam;
	CString			m_strExtCmd;
	bool				m_bNoWait;

	CPnr				*m_pOwner;
	CLogger			*m_pLogger;

private:
	pid_t _Fork(void);
};

///////////////////////////////////////////////////////////////////////////////
class CPnr : public CQThread
{
	DECLARE_DYNAMIC(CPnr)

public:
	CPnr();
	~CPnr();
	bool Start();
	void Stop();
	CProcess *Add(int nIndex, int nId,
		LPCSTR pszParam, PROCTYPE nProcType = PROC_TYPE_NORM,
		LPCSTR pszExtCmd = NULL);
	void Delete(int nIndex);
	CProcess *GetProcess(int nIndex);
	int GetCount(void) { return m_pProcesses->GetCount(); }
	int GetAliveInterval(void) { return m_nAliveInterval; }
	void SetAliveInterval(PDKINT msec) { m_nAliveInterval = msec; }
	void ProcInfoUpdate(void);
	void GetProcInfo(pid_t pid, CArray<pid_t,pid_t> &aryPid);
	void ViewProcTree(pid_t pid) { m_pProcInfo->ViewTree(pid); }
	CProcInfo::PPROC GetProcInfo(pid_t pid) { return m_pProcInfo->GetProc(pid); }
	CProcInfo::PPROC GetProcInfo(LPCSTR cmd) { return m_pProcInfo->GetProc(cmd);}

	bool ProcExist(pid_t pid) { return m_pProcInfo->Exist(pid); }
	void StartProc(int nIndex) { AddEvent(PROC_START_CHILD, nIndex); }
	void StopProc(int nIndex) { AddEvent(PROC_STOP_CHILD, nIndex); }
	void StopAll(void);

	bool StartTimer(int msec, PDK8U which = 0);
	bool StopTimer(PDK8U which = 0);

	CProcNotify		OnNotify;
	PCallBack		OnAliveNotify;
	static CCritSec m_Lock;

protected:
	void *ThreadProc(void);

	CMapPtrToPtr	*m_pProcesses;
	CProcInfo		*m_pProcInfo;
	CLogger			*m_pLogger;
	PDKINT			 m_nAliveInterval;
};

#endif // __PNR_H__
