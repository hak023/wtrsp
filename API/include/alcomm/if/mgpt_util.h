#include <string.h>
#include <sys/timeb.h>
//common define ////////////////////////////////
#define PHONE_ID_LEN                  16
#define MAX_PATH_LEN                  256
#define MAX_IP_ADDR_LEN               32 
#define TIME_STR_LEN                  16
#define DESC_LEN                      16

enum STATUS_TYPE {
   STATUS_TYPE_REQ = 0,
   STATUS_TYPE_RSP = 1,
   STATUS_TYPE_MAX = 2
};

#define STATUS_IDLE            0
#define STATUS_MGI_ADD      0x1
#define STATUS_MGI_H245     0x2
#define STATUS_MGI_MODIFY   0x4
#define STATUS_MGI_DEL      0x8

#define STATUS_TDM_INIT        0x1
#define STATUS_TDM_NEGO        0x2
#define STATUS_TDM_PLAY        0x4
#define STATUS_TDM_RELAY       0x8
#define STATUS_TDM_CLOSE       0x10

#define STATUS_MPA_START       0x1
#define STATUS_MPA_RELAY       0x2
#define STATUS_MPA_CLOSE       0x4

#define MPA_GROUP0_BIT         0x1
#define MPA_GROUP1_BIT         0x2

#define MAX_AUDIO_CODEC_CNT    10
#define MAX_VIDEO_CODEC_CNT    2

char* getstr_mpacmd(int nType, char *szType);
char* getstr_dtmfmethod(int nMethod);
const char* getstr_mgistatus(unsigned char ucStatus, bool bMGT = false);

#if 0
// for trace ÇÑ±Û
char* getstr_mpacmd_kor(int nType, char *szKorType);
char* getstr_mgicmd_kor(int nType, char *szKorType);
char* getstr_tdmcmd_kor(int nType, char *szKorType);
#endif

int codecTC2IMGC(int Codec);
int codecIMGC2TC(int Codec);
int GetPayloadType(int Codec);

using namespace std;

string StrFormat(LPCSTR pszFormat, ...);
string GetCurDateTime();
string GetCurDateTime2();
string GetCurDateYYYYMMDD();
string GetCurDateMMDD();
string GetCurDateHHMMSS(); // for statlog
string YMDTorfc1123Date(const string& d1);
int StrToInt(const string& s);
unsigned int StrToUInt(const string& s);
int StrToInt(const char* s);
unsigned int StrToUInt(const char* s);
string IntToStr(int i);
float StrToFloat(const string& s);
float StrToFloat(char* s);
string mgTrimLeft(const string& src);
string mgTrimRight(const string& src);
string mgTrim(const string& src);
string mgMakeFile(const string& path, int nPos, const string& name, int nIndex);
string mgGetFileName(string sFilePath);
int ParseMIN(string& sUA, string& sMIN, bool& bIsIRM, int& nUASystemId);
bool CheckMIN(string sMIN);
string MakeCaller(unsigned int nCaller);
string GetMgMsgStr(unsigned short usType, unsigned short usSubType);
string GetMgCmdStr(unsigned short usType, unsigned short usSubType);
string GetStatusStr(int nStatus);
string GetCommandResult(const char *szCmd);
unsigned long GetMountFileSystemAvailableSize(char *szPath);
void DelFile(string sFileName);
