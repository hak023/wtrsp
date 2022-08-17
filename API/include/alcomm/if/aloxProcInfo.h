
#ifndef __ALOX_PROC_INFO_H__
#define __ALOX_PROC_INFO_H__

#include "aloxUtil.h"

typedef struct __AL_PROC_INFO__
{
	char				szSysTime[128];       //system time
	unsigned int 	uiPID;                //PID
	unsigned int 	uiHeap;               //heap memory size by /proc/%d/status (Byte)
	unsigned int 	uiVSZ;                //virtual memory size (Byte)
	unsigned int 	uiRSS;                //real memory size (Byte)
	char				szCPUTime[128];       //cpu time
	float				fCPUPer;              //cpu %
	float				fMEMPer;              //memory %
	char           szCMD[128];           //command
}AL_PROC_INFO,*PAL_PROC_INFO;

//classs
class CALProcessInfo
{

public:
	CALProcessInfo()
	{
		m_pInfo=new AL_PROC_INFO;
		m_pBeforeInfo=new AL_PROC_INFO;
		memset(m_pInfo,0x00,sizeof(AL_PROC_INFO));
		memset(m_pBeforeInfo,0x00,sizeof(AL_PROC_INFO));
		
		m_nPID=getpid();
		m_bFirst=true;
	};
	~ CALProcessInfo(){};


	char* GetProcessMemory(char* szBuffer,int nBufferSize);

protected:
	unsigned int m_nPID;
	AL_PROC_INFO* m_pInfo;
	AL_PROC_INFO* m_pBeforeInfo;
	bool m_bFirst;
};
#endif
