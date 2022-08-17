#include "aloxProcInfo.h"
#if 1
#include	<procfs.h>
#else 
#include	<sys/procfs.h>
typedef prstatus_t pstatus_t;
typedef prpsinfo_t psinfo_t;
#endif 

const static char* g_stPSINFO_FORMAT="/proc/%d/psinfo";
const static char* g_stPSSTATUS_FORMAT="/proc/%d/status";

char* CALProcessInfo::GetProcessMemory(char* szBuffer,int nBufferSize){

	if(szBuffer!=NULL) szBuffer[0]=0x00;	

	if ( szBuffer == NULL || nBufferSize < 128 ) return szBuffer;

	char szFileName[256];
	psinfo_t usr_psinfo;
	pstatus_t usr_status;

	memset(m_pInfo,0x00,sizeof(AL_PROC_INFO));

	FILE* p_fp=NULL;
	sprintf(szFileName,g_stPSINFO_FORMAT,m_nPID);
	if( (p_fp=fopen(szFileName,"r")) == NULL ) return szBuffer;

	if ( fread(&usr_psinfo,sizeof(psinfo_t),1,p_fp) !=1 ) {	
		ALOX_CLOSE_FP(p_fp);
		return szBuffer;
	}
	ALOX_CLOSE_FP(p_fp);

	sprintf(szFileName,g_stPSSTATUS_FORMAT,m_nPID);
	if( (p_fp=fopen(szFileName,"r")) == NULL ) return szBuffer;

	if ( fread(&usr_status,sizeof(pstatus_t),1,p_fp) !=1 ) {
		ALOX_CLOSE_FP(p_fp);
		return szBuffer;
	}
	ALOX_CLOSE_FP(p_fp);

	time_t tmpTime=0;
	int tmpHour=0;
	int tmpMin=0;
	int  tmpSec=0;

	CALUtil::GetStrCurrentTime(m_pInfo->szSysTime);
	m_pInfo->uiPID=m_nPID;
	m_pInfo->uiHeap=(usr_status.pr_brksize); //HEAP (B)
	m_pInfo->uiVSZ=usr_psinfo.pr_size;   //VSZ (KB)
	m_pInfo->uiRSS=usr_psinfo.pr_rssize; //RSS  (KB)

	tmpTime=usr_psinfo.pr_time.tv_sec;
	tmpHour = tmpTime/3600;
	tmpTime %= 3600;
	tmpMin = tmpTime/60;
	tmpSec = tmpTime%60;
	sprintf(m_pInfo->szCPUTime,"%02d:%02d:%02d",tmpHour,tmpMin,tmpSec);

	m_pInfo->fCPUPer=((float)usr_psinfo.pr_pctcpu*(float)100)/(float)32768;
	m_pInfo->fMEMPer=((float)usr_psinfo.pr_pctmem*(float)100)/(float)32768;
	strncpy(m_pInfo->szCMD,usr_psinfo.pr_psargs,sizeof(m_pInfo->szCMD));

	if ( m_bFirst ){
		memcpy(m_pBeforeInfo,m_pInfo,sizeof(AL_PROC_INFO));
		m_bFirst=false;
	}

	char szInfoBuf[1024];
	sprintf(szInfoBuf,"HEAP:%d,%d (B), VSZ:%d,%d (KB), MEM:%3.2f%%, CPU:%3.2f%%",
		m_pInfo->uiHeap,(m_pInfo->uiHeap-m_pBeforeInfo->uiHeap),
		m_pInfo->uiVSZ,(m_pInfo->uiVSZ-m_pBeforeInfo->uiVSZ),
		m_pInfo->fMEMPer,m_pInfo->fCPUPer);

	strncpy(szBuffer,szInfoBuf,nBufferSize);

	memcpy(m_pBeforeInfo,m_pInfo,sizeof(AL_PROC_INFO));
	return szBuffer;
};
