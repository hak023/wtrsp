/*
 *
*/
#ifndef RECONFIG_H
#define RECONFIG_H

#include "r_inifile.h"
#include "logmgr.h"
#include "r_jthread.h"
#include "r_util.h"
#include <sys/types.h>
#include <sys/stat.h>

typedef bool(*fpReconfigHandler)(void* arg1,void* arg2);
class CReconfig : public RED::CThread
{
public:
	CReconfig() : m_nCheckInterval(5000)
	{
		memset(m_pArg,0x00,sizeof(m_pArg));
		m_pLogger = CLogMgr::GetInstance();
		sprintf(m_szDesc,"[RECONF]");
		memset(m_szConfFile,0x00,sizeof(m_szConfFile));
	}
	~CReconfig(){}

	bool Init(char* szConfFile, RED::CIniFile* pConfig,int nCheckInterval=5000)
	{
		if(!szConfFile || !pConfig) return false;
		strncpy(m_szConfFile,szConfFile,128);
		m_pConfig = pConfig;
		if(nCheckInterval > 0) m_nCheckInterval = nCheckInterval;
	}
	bool Start(){ return Create();}
	void Stop(){ Close(); }
	void RegistHandler(fpReconfigHandler fpHandler,void* parg1=NULL,void* parg2=NULL)
	{
		m_fpReconfigHandler = fpHandler;
		if(parg1) m_pArg[0] = parg1;
		if(parg2) m_pArg[1] = parg2;
		m_pLogger->Log(CLogMgr::Info,
			"%s %s DONE",m_szDesc,__func__);
	}
	bool CheckFileDate()
	{
		struct stat buf;
		if(stat(m_szConfFile,&buf)!=0 || ((buf.st_mode & S_IFDIR) == S_IFDIR))
		{
			m_pLogger->Log(CLogMgr::Error,
				"%s %s file[%s] error",m_szDesc,__func__,m_szConfFile);
			return false;
		}
		time_t curtime = time(NULL);
		if((buf.st_mtime + m_nCheckInterval/1000) >= curtime)
		{
			m_pLogger->Log(CLogMgr::Api,
				"%s %s mtime(%u) curtime(%u) valid date for operation",
					m_szDesc,__func__,buf.st_mtime,curtime);
			return true;
		}
		return false;
	}
protected:
	fpReconfigHandler m_fpReconfigHandler;
	void* ThreadProc()
	{
		while(!DoExit())
		{
			if(CheckFileDate())
			{	
				if(!m_pConfig) continue;
				if(m_pConfig->ReadFile()==false) continue;
				if(m_fpReconfigHandler) (*m_fpReconfigHandler)(m_pArg[0],m_pArg[1]);
			}
			RED::MilliSleep(m_nCheckInterval);
		}
		return NULL;
	}
	RED::CIniFile* m_pConfig;
	CLogMgr* m_pLogger;
	int m_nCheckInterval;
	void* m_pArg[2];
	char m_szDesc[16];	
	char m_szConfFile[128];
};
#endif
////@}
