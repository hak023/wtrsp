/******************************* GCC Include ********************************************/
#include <iostream>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/resource.h>
#include <cstdlib>
/******************************* Util Include ********************************************/
#include "ConfigFile.hxx"
#include "string.hxx"
#include "log.hxx"
#include "rutil/XMLCursor.hxx"
#include "rutil/DataStream.hxx"
#include "rutil/Inserter.hxx"
#include "rutil/Random.hxx"
#include "rutil/Time.hxx"
/******************************* Process Include *****************************************/
#include "main.h"
#include "MainConfig.h"
#include "Worker.h"
#include "tinyxml/tinyxml.h"
#include "AppXmlParser.h"
#include "NasSystemTable.h"
/******************************* Using Name Space ***************************************/
using namespace eSipUtil;
/******************************** Global Instance ****************************************/
KString g_clsProcName;
unsigned int g_unPid = 0;
KString g_clsBuildDate;
int g_unVwtrssLogInst = 0;

/********************************* Main Local Function Defines *******************************/
static const char *s_fnStringSignal(int sig);
static void s_fnSignalHandle(int sig);
static void s_fnSetSignal();
static void s_fnSetProcName(int argc, char ** argv);
void main_memory_init()
{
	struct rlimit rlim;
	rlim.rlim_cur = MAIN_PROCESS_STACK_SIZE;
	rlim.rlim_max = MAIN_PROCESS_STACK_SIZE;
	setrlimit(RLIMIT_STACK, &rlim);
}

int g_fnGetLog()
{
	return g_unVwtrssLogInst;
}
void s_fnCreateLog(const char *_pszProcName)
{
	KString clsLogPath; clsLogPath<<(KCSTR)"../log/"<<_pszProcName<<(KCSTR)"/";
	std::cout << "clsLogPath:" << (KCSTR)clsLogPath << "\n";
	AsyncFileLog::m_fnInit((KCSTR)clsLogPath,(KCSTR)g_clsProcName, g_unVwtrssLogInst,819200,10);
	AsyncFileLog::m_fnSetCategoryStr(g_unVwtrssLogInst, E_VWTRSS_LOG_CATE_UTIL,"UTIL  ");
	AsyncFileLog::m_fnSetCategoryStr(g_unVwtrssLogInst, E_VWTRSS_LOG_CATE_APP,"APP   ");
	g_fnSetLog(E_VWTRSS_LOG_CATE_UTIL,E_LOG_DISABLE);
	g_fnSetLog(E_VWTRSS_LOG_CATE_APP,E_LOG_DEBUG);
	IFLOG(E_LOG_ERR,"CREATE MODULE : [%-30s] End",__func__);
}
bool g_fnCheckLog( eSipUtil::ELogLevel_t _eLogLevel)
{
	return AsyncFileLog::m_fnCheckLogLevel(g_unVwtrssLogInst, E_VWTRSS_LOG_CATE_APP, _eLogLevel);
}
bool g_fnCheckTrsgLog( eSipUtil::ELogLevel_t _eLogLevel)
{
   return AsyncFileLog::m_fnCheckLogLevel(g_unVwtrssLogInst, E_VWTRSS_LOG_CATE_TRSG, _eLogLevel);
}
void g_fnSetLog(EVwtrssLogCate_t _eCate,eSipUtil::ELogLevel_t _eLv)
{
	AsyncFileLog::m_fnSetLogLevel(g_unVwtrssLogInst,_eCate,_eLv);
}
/********************************** MAIN **********************************************/
int main(int argc, char ** argv)
{
   /*
    * Test
    */
/*
	g_fnCreateNasSystemTable();

   KString clsTest;
   clsTest.m_fnCat("VOX");
   printf("1_clsTest:%s\n", (KSTR)clsTest);

   NasSystemTable *pclsNasSystem = NasSystemTable::m_fnGetInstance();
   KString clsNasCode = "67";
   clsTest.m_fnReSize(10240);
   printf("clsNasCode:%s\n", (KCSTR)clsNasCode);
   pclsNasSystem->m_fnGetSourceDir(clsNasCode, clsTest);
   printf("2_clsTest:%s\n", (KCSTR)clsTest);
   
   //if(clsTest.m_unRealLen == 0) printf("true");
   //if(KString::m_fnStrCmp((KSTR)clsTest,"VOX")) printf("true");
   //else printf("false");

   KString clsTest;
   clsTest = NULL;
   printf("clsTest:%d", (KSHORT)clsTest);
   printf("\n");
   printf("clsTest:%d", (KINT)clsTest);
   printf("\n");
   
   printf("clsTest:%s", (KCSTR)clsTest);
   printf("\n");
   
   
   int res;
   res = access("/home/ibc/music_original5/source/0100003428/01000034289848.mp3", F_OK);
   printf("res:%d", res);
   return 0;

   KString clsStr=KNULL;
   KString _rclsJobStatusChangedNotify;
   KString::m_fnStrnCat((KSTR)_rclsJobStatusChangedNotify,10240,"From=\"%s\" To=\"%s\"",(KCSTR)clsStr, (KCSTR)clsStr);

   resip::Data tmp = (KCSTR)_rclsJobStatusChangedNotify;
   std::cout << tmp.c_str() << std::endl;

   int rte = tmp.replace("(null)","");
   _rclsJobStatusChangedNotify = tmp.c_str();

   std::cout << (KCSTR)_rclsJobStatusChangedNotify << std::endl;
   //std::cout << "replace Cnt:" << rte << std::endl;
 
   // command ½ÇÇà
  //    int res = std::system("timeout 30s /home/ibc/song/humt_4.2/ffmpeg-4.2.2/ffmpeg -i /home/ibc/music_original5/source/0100003428/6430001449.mp3 -f mp3 -acodec mp3 -ac 2 -ar 44100 -ab 192000  -y /home/ibc/music/mig/test/lhj/20220711_testfile.mp3 &");

  // KString clsTcCommand = "wma";
  // KString clsTest = "WMA";
  // int res = KString::m_fnStrCaseCmp((KSTR)clsTcCommand, (KSTR)clsTest);
   //printf("%d", res);


   // 1. Normal FFMPEG
	//std::string cmd  = "timeout 30s /home/ibc/song/humt_4.2/ffmpeg-4.2.2/ffmpeg -i /home/ibc/music_original5/source/0100003428/6430001449.mp3 -f mp3 -acodec mp3 -ac 2 -ar 44100 -ab 192000  -y /home/ibc/music/mig/test/lhj/20220711_testfile.mp3 2>&1 &";
   // 2. Timeout FFM
	//std::string cmd  = "timeout 3s /home/ibc/song/humt_4.2/ffmpeg-4.2.2/ffmpeg -i /home/ibc/music_original5/source/0100003428/6430001449.mp3 -f mp3 -acodec mp3 -ac 2 -ar 44100 -ab 192000  -y /home/ibc/music/mig/test/lhj/20220711_testfile.mp3 2>&1 &";
   // 3. Failed FFM
	//std::string cmd  = "timeout 30s /home/ibc/song/humt_4.2/ffmpeg-4.2.2/ffmpeg -i /home/ibc/music_original5/source/0100003428/6430001449.mp3 -f mp3 -acodec mp3 -ac 2 -ar 44100 -ab 192000 2>&1 &";
   // 4. Normal VOX
	//std::string cmd = "timeout 30s /home/ibc/util/vox1_1/vox -b 16 /home/ibc/music/mig/test/lhj/tc02_pcm_8000.s16le 2>&1 &";
   // 5. Failed VOX
	//std::string cmd = "timeout 30s /home/ibc/util/vox1_1/vox -b /home/ibc/hak/wtrsp/sample/test/11896670_pcm_8000.s16le  /home/ibc/music/miggg/11896670_pcm_8000_c_vox.vpm 2>&1 &";
   // 5. MCONV
   std::string cmd = "timeout 3s /home/ibc/song/humt_4.2/dist/amapps/mconv/MCONV fc -sf 3GP NULL /home/ibc/hak/wtrsp/sample/Sample_02_WB_23850.3gp -df MP4 NULL /home/ibc/hak/wtrsp/sample/Sample_02_WB_23850.mp4 -dv H264 24 1000 1280 720 -da QCP13K 8000 1 16 13000"
	FILE *pipe = popen(cmd.c_str(), "r");
   int stTime = time(NULL);
	if (pipe == NULL)
		perror("popen() fail");

   KString buffer; buffer.m_fnReSize(10240);
   KString clsSize; clsSize.m_fnReSize(10240);
   KString clsTime; clsTime.m_fnReSize(10240);
	while (fgets(buffer, 10240, pipe) != NULL)
   {
      if(KString::m_fnStrStr((KSTR)buffer, "(Audio+Video)SizekB:") != NULL)
      {
         KString::m_fnRmChr((KSTR)clsSize, (KSTR)buffer, "by.ghNam >>>>> (Audio+Video)SizekB:");
         //printf("clsSize ::: %s", (KSTR)clsSize);
         if(KString::m_fnAtoi(clsSize) <= 0)
            return 0;   //Failed
      }
      else
      {
         // Failed
      }

      //printf("clsSize ::: %s", (KSTR)clsSize);
     
      if(KString::m_fnAtoi(clsSize) > 0)
      {
         if(KString::m_fnStrStr((KSTR)buffer, "TotalTimeSec:") != NULL)
         {
            KString::m_fnRmChr((KSTR)clsTime, (KSTR)buffer, "by.ghNam >>>>> TotalTimeSec:");
            clsTime = KString::m_fnSkipString((KSTR)buffer, "by.ghNam >>>>> TotalTimeSec:");
            printf("clsTime ::: %s\n", (KSTR)clsTime);
         }
         else
         {
            // Failed
         }

         // Check Timeout
         if(KString::m_fnStrStr((KSTR)buffer, "received signal") != NULL)
         {
            // Failed
            clsSize.m_fnReSize(10240);
            clsTime.m_fnReSize(10240);
            printf("TIMEOUT!!!");
            pclose(pipe);
            return 0;
         }
      }

      buffer.m_fnReSize(10240);
      // while
   }
	pclose(pipe);
   int endTime = time(NULL);
   printf("duration=%d\n", endTime - stTime);

	return 0;

	*/
	main_memory_init();
	g_unPid = (unsigned int) getpid();
	g_clsBuildDate << (KCSTR) __DATE__ << " - " << (KCSTR) __TIME__;
	s_fnSetProcName(argc, argv);
	s_fnSetSignal();
	s_fnCreateLog((KCSTR)g_clsProcName);
	g_fnCreateMainConfig();
	g_fnCreateNasSystemTable();
	g_fnCreateWorker();
	g_fnCreateTrsgTransport();//media

/*
   KString clsTargetAudioCodec;
   KString clsTargetVideoCodec;
   ETrssCodeSet_t nCodecResCode;//1
   MainConfig *pclsConf = MainConfig::m_fnGetInstance();
   
   clsTargetAudioCodec = "AMR";
   clsTargetVideoCodec = "MPEG4";
   //0
   printf("Result:%d\n", pclsConf->m_fnChkTargetCodec(clsTargetAudioCodec, clsTargetVideoCodec));


   clsTargetAudioCodec = "";
   clsTargetVideoCodec = "JPEG4";
   //0
   printf("Result:%d\n", pclsConf->m_fnChkTargetCodec(clsTargetAudioCodec, clsTargetVideoCodec));

   
   clsTargetAudioCodec = "PCM";
   clsTargetVideoCodec = "";
   //4
   printf("Result:%d\n", pclsConf->m_fnChkTargetCodec(clsTargetAudioCodec, clsTargetVideoCodec));


   clsTargetAudioCodec = "";
   clsTargetVideoCodec = "JPEG5";
   //5
   printf("Result:%d\n", pclsConf->m_fnChkTargetCodec(clsTargetAudioCodec, clsTargetVideoCodec));

   clsTargetAudioCodec = "PCM";
   clsTargetVideoCodec = "MPEG4";
   //5
   printf("Result:%d\n", pclsConf->m_fnChkTargetCodec(clsTargetAudioCodec, clsTargetVideoCodec));
*/

	pid_t ppid;
	while(1)
	{
		ppid=getppid();
		if(ppid==1 )
		{
			printf("PPID is  1, we will exit\r\n");
			break;
		}
		sleep(1);
	}
	return 0;
}
/**********************************Main Local Functions ************************************/
const char *s_fnStringSignal(int sig)
{
	switch(sig)
	{
		case SIGINT: return "SIGINIT";
		case SIGKILL: return "SIGKILL";
		case SIGTERM: return "SIGTERM";
		case SIGHUP: return "SIGHUP ";
		case SIGPIPE: return "SIGPIPE";
		default: return "NONE   ";
	};
	return "NONE   ";
}
void g_fnKillMyself()
{
	pid_t mypid=getpid();
	if( mypid )
	{
		kill(mypid,SIGKILL);
		usleep(2000*1000);
		kill(mypid,SIGKILL);
		usleep(2000*1000);
		exit(0);
	}
}
void s_fnSignalHandle(int sig)
{
   IFLOG(E_LOG_ERR,"recv signal(%s)\r\n",s_fnStringSignal(sig));
   if((sig == SIGINT) || (sig == SIGKILL) || (sig == SIGTERM))
   {
      printf("Finished %s Process(%s)\r\n",(KCSTR)g_clsProcName, s_fnStringSignal(sig));
      sleep(1);
      g_fnKillMyself();
   }
}
void s_fnSetSignal()
{
	signal(SIGINT, s_fnSignalHandle);	signal(SIGKILL, s_fnSignalHandle);
	signal(SIGTERM, s_fnSignalHandle);	signal(SIGHUP, s_fnSignalHandle);
	signal(SIGPIPE, s_fnSignalHandle);
}
void s_fnSetProcName(int argc, char ** argv)
{
	char *tok = NULL;
	char Temp_AppName[128];memset(Temp_AppName,0x00,128);
	char Temp_AppName2[128];memset(Temp_AppName2,0x00,128);
	strcpy(Temp_AppName,argv[0]);
	if (!(tok = strtok(Temp_AppName, "/"))) 
	{
		perror("Illegal command\r\n");exit(-1);
	}
	do 
	{
		Temp_AppName2[0]=0;	strcpy(Temp_AppName2,tok);
	} while ((tok = strtok(NULL, "/")));
	g_clsProcName<<(KSTR)Temp_AppName2;
}
