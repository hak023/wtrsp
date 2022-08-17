#include "mp_if.h"
#include "mgpt_util.h"
#include "mgievent.h"
#include "logger.h"
#include "util.h"

char* getstr_mpacmd(int nType, char *szType)
{
   switch(nType) {
   case AMT_INTERNAL      : sprintf(szType, "INTERNAL"       ); break;
   case AMT_SETUP_AGNT    : sprintf(szType, "SETUP_AGENT"    ); break;
   case AMT_SETUP_APPS    : sprintf(szType, "SETUP_APPS"     ); break;
   case AMT_ALIVE         : sprintf(szType, "ALIVE"      ); break;
   case AMT_WARN_SES      : sprintf(szType, "WARN_SES"      ); break;
   case AMT_CLOSED_SES    : sprintf(szType, "CLOSED_SES"      ); break;
   case AMT_START_SES     : sprintf(szType, "START_SES"      ); break;
   case AMT_START_SES_DUMP     : sprintf(szType, "START_SES_DUMP" ); break;
   case AMT_STOP_SES      : sprintf(szType, "STOP_SES"      ); break;
   case AMT_PLAY_READY    : sprintf(szType, "PLAY_READY"    ); break;
   case AMT_PLAY          : sprintf(szType, "PLAY"          ); break;
   case AMT_PLAY_SIMPLE   : sprintf(szType, "PLAY_SIMPLE"   ); break;
   case AMT_PLAY_MULTI    : sprintf(szType, "PLAY_MULTI"   ); break;
   case AMT_PLAY_STOP     : sprintf(szType, "PLAY_STOP"     ); break;
   case AMT_PLAY_DONE     : sprintf(szType, "PLAY_DONE"     ); break;
   case AMT_RECORD        : sprintf(szType, "RECORD"        ); break;
   case AMT_RECORD_STOP   : sprintf(szType, "RECORD_STOP"   ); break;
   case AMT_RELAY         : sprintf(szType, "RELAY"         ); break;
   case AMT_RELAY_TC      : sprintf(szType, "RELAY_TC"      ); break;
   case AMT_RELAY_STOP    : sprintf(szType, "RELAY_STOP"    ); break;
   case AMT_DTMF_SET      : sprintf(szType,"AMT_DTMF_SET"   ); break;
   case AMT_DTMF_GEN      : sprintf(szType,"AMT_DTMF_GEN"); break;
   case AMT_DTMF_INFO     : sprintf(szType,"AMT_DTMF_INFO"); break;
   case AMT_DTMF_GENINFO  : sprintf(szType,"AMT_DTMF_GENINFO"); break;
   case AMT_MM_BLOCK      : sprintf(szType,"AMT_MM_BLOCK"); break;
   case AMT_MM_UNBLOCK    : sprintf(szType,"AMT_MM_UNBLOCK"); break;
   case AMT_MM_RESET      : sprintf(szType,"AMT_MM_RESET"); break;
   case AMT_MM_BD_STS     : sprintf(szType,"AMT_MM_BD_STS"); break;
   case AMT_MM_SES_STS    : sprintf(szType,"AMT_MM_SES_STS"); break;
   case AMT_MM_BD_INF     : sprintf(szType,"AMT_MM_BD_INF"); break;
   case AMT_MM_SES_INF    : sprintf(szType,"AMT_MM_SES_INF"); break;
   case AMT_MM_RM_INF    : sprintf(szType,"AMT_MM_RM_INF"); break;
   case AMT_MM_CLEAR      : sprintf(szType,"AMT_MM_CLEAR"); break;
   default: sprintf(szType, "Unknown(%d:0x%x)", nType, nType); break;
   } 
   return szType;
}

#if 0
char* getstr_mpacmd_kor(int nType, char *szKorType)
{
   switch(nType) {
   case AMT_START_SES     : sprintf(szType, "Packet 자원 요청 시작"      ); break;
   case AMT_START_SES_DUMP     : sprintf(szType, "Packet 자원 요청 시작(Dump포함)" ); break;
   case AMT_STOP_SES      : sprintf(szType, "Packet 자원 중지"      ); break;
   case AMT_RELAY         : sprintf(szType, "Packet 릴레이 시작"         ); break;
   case AMT_RELAY_TC      : sprintf(szType, "Packet 트랜스코딩 & 릴레이 시작"      ); break;
   case AMT_RELAY_STOP    : sprintf(szType, "Packet 릴레이 중단"    ); break;
   case AMT_DTMF_SET      : sprintf(szType, "Packet DTMF Setting"   ); break;
   case AMT_DTMF_GEN      : sprintf(szType, "Packet DTMF 생성"); break;
   case AMT_DTMF_INFO     : sprintf(szType, "Packet DTMF 감지"); break;
   case AMT_MM_CLEAR      : sprintf(szType, "Packet 자원 Claer"); break;
   default: sprintf(szType, "알수없는 메시지(%d:0x%x)", nType, nType); break;
   } 
   return szType;
}

char* getstr_mgicmd_kor(int nType, char *szKorType)
{
   switch(nType) {
   case AddRSCRQ          :
   case AddRSCRP          : sprintf(szType, "영상통화 자원 요청 시작"      ); break;
   case H245RestartRQ     :
   case H245StartRQ       : sprintf(szType, "영상통화 네고 시작" ); break;
   case H245EndNoti       : sprintf(szType, "영상통화 네고" ); break;
   case ModifyRSCRQ       : sprintf(szType, "영상통화 연결 시작"         ); break;
   case ModifyRSCRP       : sprintf(szType, "영상통화 연결"         ); break;
   case DelRSCRQ          : sprintf(szType, "영상통화 종료 요청"); break;
   case DelRSCRP          : sprintf(szType, "영상통화 종료"); break;
   case H245USERINPUT     : sprintf(szType, "영상통화 H245 메시지"); break;
   case NotifyDtmf        : sprintf(szType, "영상통화 DTMF 보고"); break;
   case H245VFURQ         : sprintf(szType, "영상통화 Video Fast Update 요청"); break;
   case ChannelSync       : sprintf(szType, "영상통화 채널 동기화 요청"); break;
   default: sprintf(szType, "알수없는 메시지(%d:0x%x)", nType, nType); break;
   } 
   return szType;
}

char* getstr_tdmcmd_kor(int nType, char *szKorType)
{
   switch(nType) {
   case e_tdm_init        : sprintf(szType, "Circuit 자원 요청 시작"      ); break;
   case e_tdm_init_ack    : sprintf(szType, "Circuit 자원 요청"      ); break;
   case e_tdm_renego_start: 
   case e_tdm_nego_start  : sprintf(szType, "Circuit 자원 네고 시작"      ); break;
   case e_tdm_nego_end    : sprintf(szType, "Circuit 자원 네고"      ); break;
   case e_tdm_play        : sprintf(szType, "Circuit 자원 Play 요청"      ); break;
   case e_tdm_play_stop   : sprintf(szType, "Circuit 자원 Play 종료 요청"      ); break;
   case e_tdm_play_done   : sprintf(szType, "Circuit 자원 Play 종료"      ); break;
   case e_tdm_relay       : sprintf(szType, "Circuit 자원 릴레이 요청"      ); break;
   case e_tdm_dtmf_info   : sprintf(szType, "Circuit 자원 DTMF 정보"      ); break;
   case e_tdm_vendor_info : sprintf(szType, "Circuit 자원 제조사 정보"      ); break;
   case e_tdm_text_info   : sprintf(szType, "Circuit 자원 TEXT 정보"      ); break;
   case e_tdm_vfu_info    : sprintf(szType, "Circuit 자원 Video Fast Update 요청"      ); break;
   case e_tdm_msg_trace   : sprintf(szType, "Circuit 자원 트레이스 정보 메시지"      ); break;
   case e_tdm_cmd_flushmedia    : sprintf(szType, "Circuit 자원 Clear 요청"      ); break;
   default: sprintf(szType, "알수없는 메시지(%d:0x%x)", nType, nType); break;
   } 
   return szType;
}
#endif


char* getstr_dtmfmethod(int nMethod)
{
   char szBuf[32];
   memset(szBuf, 0, sizeof(szBuf));

   switch(nMethod)
   {
      case DTMF_METHOD_RFC2833: strcpy(szBuf, "DTMF_METHOD_RFC2833"); break;
      case DTMF_METHOD_IN_BAND: strcpy(szBuf, "DTMF_METHOD_INBAND"); break;
      case DTMF_METHOD_OUT_OF_BAND: strcpy(szBuf, "DTMF_METHOD_OUT_OF_BAND"); break;
      default: strcpy(szBuf, "DTMF_METHOD_UNDEF"); break;
   }
   return szBuf;
}

const char* getstr_mgistatus(unsigned char ucStatus, bool bMGT)
{
   CString strTemp = "";

   strTemp += "Add:";
   if (ucStatus & STATUS_MGI_ADD)
      strTemp += "On";
   else
      strTemp += "Off";

   if (bMGT == false)
   {
      strTemp += " H245:";
      if (ucStatus & STATUS_MGI_H245)
         strTemp += "On";
      else
         strTemp += "Off";
   }

   strTemp += " Modify:";
   if (ucStatus & STATUS_MGI_MODIFY)
      strTemp += "On";
   else
      strTemp += "Off";

   strTemp += " Del:";
   if (ucStatus & STATUS_MGI_DEL)
      strTemp += "On";
   else
      strTemp += "Off";

   return (LPCSTR)strTemp;
}

int codecTC2IMGC(int Codec)
{
   switch(Codec)
   {
      case CTI_H261:
         return CT_H261;
      case CTI_H263:
         return CT_H263;
      case CTI_MPEG4:
         return CT_MPEG4;
      case CTI_G7231:
         return CT_G7231;
      case CTI_G726_40:
         return CT_G726;
      case CTI_G729I:
         return CT_G729;
      case CTI_AMR_IF2:
         return CT_AMR_IF2;
      case CTI_AMR_RFC:
         return CT_AMR_RFC;
      case CTI_G711A:
         return CT_G711A;
      case CTI_G711U:
         return CT_G711U;
      case CTI_AAC:
         return CT_AAC;
      case CTI_EVRC:
         return CT_EVRC;
   }
	return CTI_UNKNOWN;
}

int codecIMGC2TC(int Codec)
{
   switch(Codec)
   {
      case CT_H261:
         return CTI_H261;
      case CT_H263:
         return CTI_H263;
      case CT_MPEG4:
         return CTI_MPEG4;
      case CT_G7231:
         return CTI_G7231;
      case CT_G726:
         return CTI_G726_40;
      case CT_G729:
         return CTI_G729I;
      case CT_AMR_IF2:
         return CTI_AMR_IF2;
      case CT_AMR_RFC:
         return CTI_AMR_RFC;
      case CT_G711A:
         return CTI_G711A;
      case CT_G711U:
         return CTI_G711U;
      case CT_AAC:
         return CTI_AAC;
      case CT_EVRC:
         return CTI_EVRC;
   }
	return CT_UNDEF;
}
int GetPayloadType(int Codec)
{
	int nPayload = 0;
	switch(Codec)
	{
		case CTI_G711A: nPayload = 8; break;
		case CTI_AMR_IF2: nPayload = 127; break;
		case CTI_AMR_RFC: nPayload = 98; break;
		case CTI_H263: nPayload = 34; break;
		case CTI_G7231: nPayload = 4; break;
	}
	return nPayload;
}


/*
 * Name   : StrFormat()        
 * Param  : pszFormat ->  
 * Return : string 
 * Desc   : make output str with input str
 */
string StrFormat(LPCSTR pszFormat, ...)
{
   assert(pszFormat != NULL);
   char szBuf[16*1024];
   va_list argList;
   va_start(argList, pszFormat);
   vsprintf(szBuf, pszFormat, argList);
   va_end(argList);
   return szBuf;
}

int SockSelect(int sock, int timeout)
{
   if ( sock == INVALID_SOCKET )
       return -1;

   fd_set  fd_read;
   int     selnum;
   int     fdisset;

   FD_ZERO(&fd_read);                               
   FD_SET(sock, &fd_read);
  
   if ( timeout > 0 ) {
      timeval sel_timeout;
      sel_timeout.tv_sec = timeout / 1000;
      sel_timeout.tv_usec = (timeout % 1000) * 1000;

      selnum = select(sock + 1, &fd_read, 0, 0, &sel_timeout);
   }
   else {
      selnum = select(sock + 1, &fd_read, 0, 0, NULL);
   }

   if ( selnum == -1 )
      return -1;//0;

   //time out : return 0
   fdisset = FD_ISSET(sock, &fd_read);
   if( (selnum == 0) || (fdisset == 0) )
      return 0;

   return 1;
}

string GetCurDateTime()
{
   char buf[64];
   timeb cur_t;
   tm res;
   string stime = "";

   ftime(&cur_t);
   localtime_r(&cur_t.time, &res);
   
   // Mon, 02 Sep 2002 10:06:14 +0900 (KST) 
   // Fri, 12 Jul 2002 14:28:36 +0900
   strftime(buf, 64, "%a, %d %b %Y %H:%M:%S %Z", &res);
   stime = buf;
   return stime;
}

string GetCurDateTime2()
{
   char buf[64];
   timeb cur_t;
   tm res;
   string stime = "";

   ftime(&cur_t);
   localtime_r(&cur_t.time, &res);
   
   //YYYYMMDDNNHHSS
   strftime(buf, 64, "%Y%m%d%H%M%S", &res);
   stime = buf;
   return stime;
}

string GetCurDateYYYYMMDD()
{
   char buf[64];
   timeb cur_t;
   tm res;
   string stime = "";

   ftime(&cur_t);
   localtime_r(&cur_t.time, &res);

   //YYYYMMDD
   strftime(buf, 64, "%Y%m%d", &res);
   stime = buf;
   return stime;
}

string GetCurDateMMDD()
{
   char buf[64];
   timeb cur_t;
   tm res;
   string stime = "";

   ftime(&cur_t);
   localtime_r(&cur_t.time, &res);

   //MMDD
   strftime(buf, 64, "%m%d", &res);
   stime = buf;
   return stime;
}

string GetCurDateHHMMSS()
{
   char buf[64];
   timeb cur_t;
   tm res;
   string stime = "";

   ftime(&cur_t);
   localtime_r(&cur_t.time, &res);

   //HHMMDD
   strftime(buf, 64, "%H%M%S", &res);
   stime = buf;
   return stime;
}

string rfc1123DateToYMD(const string& d1) {
   // Sun, 06 Nov 1994 08:49:37 GMT
   string d2 = "";
   tm t1;
   char* p;
   p = strptime(d1.c_str(), "%a, %d %b %Y %H:%M:%S", &t1);
//   if(p == NULL) return d2;
   time_t t2;
   t2 = mktime(&t1);
   t2 += 9 * 60 * 60;
   tm t3;
    localtime_r(&t2, &t3);
    
   char buf[64];
   strftime(buf, 64, "%Y%m%d%H%M%S", &t3);
   d2 = buf;   
   return d2;   
} 

string YMDTorfc1123Date(const string& d1) {
   // 20031231135500
   // Sun, 06 Nov 1994 08:49:37 GMT
   string d2 = "";
   tm t1;
   char* p;
   p = strptime(d1.c_str(), "%Y%m%d%H%M%S", &t1);
//   if(p == NULL) return d2;
   time_t t2;
   t2 = mktime(&t1);
   t2 -= 9 * 60 * 60;
   tm t3;
    localtime_r(&t2, &t3);
    
   char buf[64];
   strftime(buf, 64, "%a, %d %b %Y %H:%M:%S GMT", &t3);
   d2 = buf;   
   return d2;   
} 

int StrToInt(const char* s) {
   if ( s[0] == '0' ) {
      if ( (s[1] == 'x') || (s[1] == 'X') ) {
         return strtol(s, NULL, 16);
      }
   }
   return atoi(s);
}

int StrToInt(const string& s) {
   if ( s.length() == 0 ) {
      return 0;   
   }
   char b[128];
   strncpy(b, s.c_str(), sizeof(b)-1);
   return StrToInt(b);   
}

unsigned int StrToUInt(const string& s) {
   if ( s.length() == 0 ) {
      return 0;
   }
   char b[128];
   strncpy(b, s.c_str(), sizeof(b)-1);
   return StrToUInt(b);
}

unsigned int StrToUInt(const char* s) {
   if ( s[0] == '0' ) {
      if ( (s[1] == 'x') || (s[1] == 'X') ) {
         return strtoul(s, NULL, 16);
      }
   }
   return strtoul(s, NULL, 10);
}

string IntToStr(int i) {
   string s = "";
   s = StrFormat("%d", i);
   return s;   
}

float StrToFloat(const string& s) {
   if ( s.length() == 0 ) {
      return 0;   
   }
   char b[128];
   strncpy(b, s.c_str(), sizeof(b)-1);
   return StrToFloat(b);   
}

float StrToFloat(char* s) {
   return atof(s);
}

string mgTrimLeft(const string& src)
{
   string dest = "";
   char c;
   unsigned int i=0;
   while ( i < src.length() ) {
       c = src[i];
       if( (c != 0x09) && (c != 0x0B) && (c != 0x20) ) {
          break;
       }
       i++;
   }
   dest = src.substr(i, src.length()-i);
   return dest;
}

string mgTrimRight(const string& src)
{
   string dest = "";
   char c;
   int i=src.length()-1;
   while ( i >= 0 ) {
       c = src[i];
       if ( (c != 0x09) && (c != 0x0B) && (c != 0x20)) {
          break;
       }
       i--;
   }
   dest = src.substr(0,i+1);
   return dest;
}

string mgTrim(const string& src)
{
   string dest = "";
   string s;
   s = mgTrimLeft(src);
   dest = mgTrimRight(s);
   return dest;
}

string mgMakeFile(const string& path, int nPos, const string& name, int nIndex)
{
   //int rc = 0;
   //string fullpath = path + "/temp";
   string fullpath = "";
   MakeDirs(fullpath.c_str(), 0766);
   fullpath =  path + "/" + name + "_" + IntToStr(nIndex)+ "_" + IntToStr(nPos) + "_" + GetCurDateTime2() + ".3gp";
   return fullpath; 
}

string mgGetFileName(string sFilePath)
{
        size_t found1, found2;
        string sTemp="";

        found1 = sFilePath.rfind("/");
        found2 = sFilePath.rfind(".3gp");

        if ( (found1 != string::npos) &&
                        (found2 != string::npos) )
        {
                sTemp = sFilePath.substr(found1+1, found2-(found1+1));
        }
        return sTemp;
}

