/************************************************************
   2011.04.14 minimei7 for MSG G/W
************************************************************/
#include<time.h>
#include "util.h"
#include "cdrlog.h"

CCdrLog::CCdrLog():
   m_pLogger(CLogger::GetInstance())
{
   memset(m_szDesc, 0x00, sizeof(m_szDesc));
   memset(m_szTime, 0x00, sizeof(m_szTime));
  
   m_bIsOpen = false; 
   m_pLogFile = new CFile;
	m_sCdrFileName = "";
   m_pConfig = new CIniFile;

   memset(m_szLogDirPath, 0x00, sizeof(m_szLogDirPath));
   memset(m_szFullPath, 0x00, sizeof(m_szFullPath));
}

CCdrLog::~CCdrLog()
{
   delete m_pLogFile;
}

/*
 * Name   : Init()
 * Param  : config file path 
 * Return : int
 * Desc   : initialize value and make log directory
 */
bool CCdrLog::Init(const char* szConfigPath)
{
   sprintf(m_szDesc, "[%s]", "CdrLog");

   m_pConfig->SetPath(szConfigPath);
   if ( m_pConfig->ReadFile() == false )
   {
      m_pLogger->Log(CLogger::Info, "%s Init() config open fail(%s) "
         "Cdr LogPath:%s", m_szDesc, szConfigPath);
      return false; 
   }
   
	m_nEnable = m_pConfig->GetValueI("CDR", "Enable");

   // set stat log path ///////// 
   sprintf(m_szLogDirPath, "%s", (LPCSTR)m_pConfig->GetValue("CDR", "CDR_PATH"));
   m_pLogger->Log(CLogger::Info, "%s Init() "
                  "Cdr Enable:%d LogPath:%s", m_szDesc, m_nEnable, m_szLogDirPath);
   
   MakeDirs(m_szLogDirPath, 0766); 
   m_nPeriod = m_pConfig->GetValueI("CDR", "PERIOD");
   m_sCdrFileName = m_pConfig->GetValue("CDR", "FILENAME");
   m_pLogger->Log(CLogger::Info, "%s Init() "
                  "Cdr Period:%d FileName:%s", m_szDesc, m_nPeriod, m_sCdrFileName.c_str());
   if (m_nPeriod == 0)
   {
      m_pLogger->Log(CLogger::Info, "%s Init() Period is NULL", m_szDesc);
      return false; 
   }

#if 0
	// read flag
   m_stCdrField.nUseFlag[CDR_FIELD_SYSID] = m_pConfig->GetValueI("CDR", "SYSID");
   m_stCdrField.nUseFlag[CDR_FIELD_BID] = m_pConfig->GetValueI("CDR", "BID");
   m_stCdrField.nUseFlag[CDR_FIELD_TID] = m_pConfig->GetValueI("CDR", "TID");
   m_stCdrField.nUseFlag[CDR_FIELD_CID] = m_pConfig->GetValueI("CDR", "CID");
   m_stCdrField.nUseFlag[CDR_FIELD_STIME] = m_pConfig->GetValueI("CDR", "STIME");
   m_stCdrField.nUseFlag[CDR_FIELD_ETIME] = m_pConfig->GetValueI("CDR", "ETIME");
   m_stCdrField.nUseFlag[CDR_FIELD_ATIME] = m_pConfig->GetValueI("CDR", "ATIME");
   m_stCdrField.nUseFlag[CDR_FIELD_WTIME] = m_pConfig->GetValueI("CDR", "WTIME");
   m_stCdrField.nUseFlag[CDR_FIELD_CTIME] = m_pConfig->GetValueI("CDR", "CTIME");
   m_stCdrField.nUseFlag[CDR_FIELD_DTIME] = m_pConfig->GetValueI("CDR", "DTIME");
   m_stCdrField.nUseFlag[CDR_FIELD_PTIME] = m_pConfig->GetValueI("CDR", "PTIME");
   m_stCdrField.nUseFlag[CDR_FIELD_ECODE] = m_pConfig->GetValueI("CDR", "ECODE");
   m_stCdrField.nUseFlag[CDR_FIELD_DIR] = m_pConfig->GetValueI("CDR", "DIR");
   m_stCdrField.nUseFlag[CDR_FIELD_W_VENDOR] = m_pConfig->GetValueI("CDR", "W_VENDOR");
   m_stCdrField.nUseFlag[CDR_FIELD_CALLER] = m_pConfig->GetValueI("CDR", "CALLER");
   m_stCdrField.nUseFlag[CDR_FIELD_CALLED] = m_pConfig->GetValueI("CDR", "CALLED");
   m_stCdrField.nUseFlag[CDR_FIELD_ASID] = m_pConfig->GetValueI("CDR", "ASID");
   m_stCdrField.nUseFlag[CDR_FIELD_ASSES] = m_pConfig->GetValueI("CDR", "ASSES");
   m_stCdrField.nUseFlag[CDR_FIELD_S_ACODEC] = m_pConfig->GetValueI("CDR", "S_ACODEC");
   m_stCdrField.nUseFlag[CDR_FIELD_S_VCODEC] = m_pConfig->GetValueI("CDR", "S_VCODEC");
   m_stCdrField.nUseFlag[CDR_FIELD_W_ACODEC] = m_pConfig->GetValueI("CDR", "W_ACODEC");
   m_stCdrField.nUseFlag[CDR_FIELD_W_VCODEC] = m_pConfig->GetValueI("CDR", "W_VCODEC");
#endif
   // all set
   for(int i=0; i<CDR_FIELD_MAX; i++)
      m_stCdrField.nUseFlag[i] = 1;

	// set key
	m_stCdrField.sKey[CDR_FIELD_SYSID] = "SYSID=";
	m_stCdrField.sKey[CDR_FIELD_BID] = "BID=";
	m_stCdrField.sKey[CDR_FIELD_TID] = "TID=";
	m_stCdrField.sKey[CDR_FIELD_CID] = "CID=";
	m_stCdrField.sKey[CDR_FIELD_STIME] = "STIME=";
	m_stCdrField.sKey[CDR_FIELD_ETIME] = "ETIME=";
	m_stCdrField.sKey[CDR_FIELD_ATIME] = "ATIME=";
	m_stCdrField.sKey[CDR_FIELD_WTIME] = "WTIME=";
	m_stCdrField.sKey[CDR_FIELD_CTIME] = "CTIME=";
	m_stCdrField.sKey[CDR_FIELD_DTIME] = "DTIME=";
	m_stCdrField.sKey[CDR_FIELD_PTIME] = "PTIME=";
	m_stCdrField.sKey[CDR_FIELD_ECODE] = "ECODE=";
	m_stCdrField.sKey[CDR_FIELD_DIR] = "DIR=";
	m_stCdrField.sKey[CDR_FIELD_W_VENDOR] = "W_VENDOR=";
	m_stCdrField.sKey[CDR_FIELD_CALLER] = "CALLER=";
	m_stCdrField.sKey[CDR_FIELD_CALLED] = "CALLED=";
	m_stCdrField.sKey[CDR_FIELD_ASID] = "ASID=";
	m_stCdrField.sKey[CDR_FIELD_ASSES] = "ASSES=";
	m_stCdrField.sKey[CDR_FIELD_S_ACODEC] = "S_ACODEC=";
	m_stCdrField.sKey[CDR_FIELD_S_VCODEC] = "S_VCODEC=";
	m_stCdrField.sKey[CDR_FIELD_W_ACODEC] = "W_ACODEC=";
	m_stCdrField.sKey[CDR_FIELD_W_VCODEC] = "W_VCODEC=";
	m_stCdrField.sKey[CDR_FIELD_TC] = "TC=";

   return true;
}


/*
 * Name   : Clear()
 * Param  : pCdrLog -> operation log data 
 * Return : void
 * Desc   : clear and initialize CDR_t struct
 */
void CCdrLog::Clear(CdrLog_t* pCdrLog)
{
	for(int i=0;i<CDR_FIELD_MAX;i++)
		pCdrLog->sValue[i] = "";

	// stime write..
   timeval stime;
   gettimeofday(&stime, NULL);
	SetValueTime(pCdrLog, CDR_FIELD_STIME, stime);
	SetValueI(pCdrLog, CDR_FIELD_TC, CDR_TC_NONE);
}

void CCdrLog::SetValueI(CdrLog_t* pCdrLog, int nField, int nValue)
{
	pCdrLog->sValue[nField] = IntToStr(nValue);
}

void CCdrLog::SetValue(CdrLog_t* pCdrLog, int nField, string sValue)
{
	pCdrLog->sValue[nField] = sValue;
}

void CCdrLog::SetFileName(CdrLog_t* pCdrLog, int nField, string sFileName)
{
	string sValue="";
	size_t found;

	found = sFileName.rfind("/");
	if (found != string::npos)
		sValue = sFileName.substr(found+1);
	else
	{
		if (sFileName.length())
			sValue = sFileName;
	}

	SetValue(pCdrLog, nField, sValue);
}

void CCdrLog::SetValueTime(CdrLog_t* pCdrLog, int nField, timeval time)
{
	//string sValue="";
   char szValue[64];
   memset(szValue, 0, sizeof(szValue));

   tm res;
   localtime_r(&time.tv_sec, &res);

   strftime(szValue, 64, "%Y/%m/%d %H:%M:%S", &res);

	SetValue(pCdrLog, nField, szValue);
}



/*
 * Name   : WriteLog()
 * Param  : CdrLog_t -> CdrLog_t struct 
 *        : nResult -> operation result 
 * Return : void
 * Desc   : write httpchild operating result to file. 
 */
void CCdrLog::WriteLog(CdrLog_t* pCdrLog)
{
	char szFullPath[MAX_PATH_LEN];
	char szTime[TIME_STR_LEN];
	char szDir[MAX_PATH_LEN];
	int i, nLen;
	string sCdr="";

	memset(szFullPath, 0x00, sizeof(szFullPath));
	memset(szTime, 0x00, TIME_STR_LEN);
	memset(szDir, 0x00, sizeof(szDir));

   if (m_nEnable == 0)
      return;

	m_CdrLock.Lock();

	struct tm tmTime;
	time_t nowtime;
	time(&nowtime);
	localtime_r(&nowtime, &tmTime);

	int nMin = (tmTime.tm_min / m_nPeriod) * m_nPeriod;

	sprintf(szDir, "%s/%04d%02d%02d", 
      m_szLogDirPath, tmTime.tm_year + 1900, tmTime.tm_mon+1, tmTime.tm_mday);
	sprintf(szTime, "%02d%02d", tmTime.tm_hour, nMin);

	if ( strcmp(m_szTime, szTime) != 0 ) {
		if ( m_bIsOpen == true ) {
			m_pLogFile->Close();
			m_pLogger->Log(CLogger::Info, "%s WriteLog():Close() Path:%s",
                       m_szDesc, m_szFullPath);
		}

		memset(m_szTime, 0x00, sizeof(m_szTime));
		strcpy(m_szTime, szTime);
		memset(m_szFullPath, 0x00, sizeof(m_szFullPath));
		sprintf(m_szFullPath,"%s/%s.%s", szDir ,m_sCdrFileName.c_str(), m_szTime);
      MakeDirs(szDir, 0777);

		m_bIsOpen = m_pLogFile->Open(m_szFullPath, CFile::modeWrite | 
                                                CFile::modeNoTruncate);
		m_pLogger->Log(CLogger::Info, "%s WriteLog():Open(%s) Path:%s",
                    m_szDesc, (m_bIsOpen==true)?"ok":"fail", m_szFullPath);
	}

   size_t found;

	// open file is success then write operating result
	if( m_bIsOpen == true ) {
		for(i=0; i<CDR_FIELD_MAX;i++)
		{
			if(m_stCdrField.nUseFlag[i])
         {
				sCdr += m_stCdrField.sKey[i] + pCdrLog->sValue[i] + ", ";
         }
		}
		sCdr = mgTrimRight(sCdr);
      found = sCdr.find_last_of(",");
      sCdr = sCdr.substr(0, found);      
		sCdr += "\n";

		nLen = m_pLogFile->Write(sCdr.c_str(), sCdr.length());
	}
	m_CdrLock.Unlock();

	m_pLogger->Log(CLogger::Info, "%s WriteLog(%s) %s",
                 m_szDesc, (nLen > 0)?"ok":"fail",
                 sCdr.c_str());
}

