#include <string>
#include "inifile.h"
#include "mgpt_util.h"

enum {
   CDR_FIELD_SYSID = 0, // 시스템 ID
   CDR_FIELD_BID, // Board ID
   CDR_FIELD_TID, // Trunk ID
   CDR_FIELD_CID, // Channel ID
   CDR_FIELD_STIME, // 시작 시간
   CDR_FIELD_ETIME, // 호 종료시간
   CDR_FIELD_ATIME, // 호 Alert 시간
   CDR_FIELD_WTIME, // 호 Answer 시간
   CDR_FIELD_CTIME, // 호 연결 완료시간
   CDR_FIELD_DTIME, // 전체 통화시간
   CDR_FIELD_PTIME, // 통화 시간
   CDR_FIELD_ECODE, // 호종료 코드
   CDR_FIELD_DIR,   // 호종류
   CDR_FIELD_W_VENDOR,   // W 단말 Vendor
   CDR_FIELD_CALLER,
   CDR_FIELD_CALLED,
   CDR_FIELD_ASID, // MGP, MGT APPID
   CDR_FIELD_ASSES, // MGP, MGT 세션 ID
   CDR_FIELD_S_ACODEC, // SIP Audio Codec(MGP), CG Audio Codec(MGT)
   CDR_FIELD_S_VCODEC, // SIP Video Codec(MGP), CG Video Codec(MGT)
   CDR_FIELD_W_ACODEC, // W단말 Audio Codec(MGP), CD Audio Codec(MGT)
   CDR_FIELD_W_VCODEC, // W단말 Video Codec(MGP), CD Video Codec(MGT)
   CDR_FIELD_TC, // Transcoding 0:TC없음, 1:일반적인 TC(Audio Only) 2:소방관제청 등(Audio+Video)
   CDR_FIELD_MAX
};

enum {
   CDR_DIR_W_SIP = 0,  // W - SIP, SIP - W
   CDR_DIR_SIP_SIP = 1 // SIP - SIP
};

enum {
   CDR_TC_NONE = 0,  
   CDR_TC_NORMAL = 1,
   CDR_TC_CASE = 2
};

typedef struct {
   std::string sKey[CDR_FIELD_MAX];
   int nUseFlag[CDR_FIELD_MAX];
} CdrField_t;

typedef struct {
   std::string sValue[CDR_FIELD_MAX];
} CdrLog_t;


using namespace std;

// class CCdrLog   / ////////////////////////////////////
/////////////////////////////////////////////////////////
class CCdrLog
{
private:

public:
   CCdrLog();
   ~CCdrLog();
   
   bool Init(const char* szConfigPath);
   void Clear(CdrLog_t* pStatLog);
   //void WriteLog(CdrLog_t* pStatLog, int nResult, timeval startTime);
   void WriteLog(CdrLog_t* pStatLog);
   void SetValueI(CdrLog_t* pStatLog, int nField, int nValue);
   void SetValue(CdrLog_t* pStatLog, int nField, string sValue);
   void SetCalled(string sCalled);
   void SetFileName(CdrLog_t* pStatLog, int nField, string sFileName);
   void SetValueTime(CdrLog_t* pStatLog, int nField, timeval time);

private:
   char m_szTime[16];

   bool m_bIsOpen;
   CFile* m_pLogFile;
   CIniFile* m_pConfig;
   CCritSec m_CdrLock;
   char m_szLogDirPath[MAX_PATH_LEN];
   char m_szFullPath[MAX_PATH_LEN];
   int m_nEnable;
   int m_nPeriod;
   string m_sCdrFileName;
   CdrField_t m_stCdrField;
   char m_szDesc[16];
   CLogger*    m_pLogger;
};
