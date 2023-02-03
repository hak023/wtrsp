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
#include <fstream>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <cstdio>
/******************************* Util Include ********************************************/
#include "ConfigFile.hxx"
#include "string.hxx"
#include "Trsplog.hxx"
#include "rutil/XMLCursor.hxx"
#include "rutil/DataStream.hxx"
#include "rutil/Inserter.hxx"
#include "rutil/Random.hxx"
#include "rutil/Time.hxx"
/******************************* Process Include *****************************************/
#include "main.h"
#include "MainConfig.h"
#include "Worker.h"
#include "TrseNodeMgr.h"
#include "TrssNodeMgr.h"
#include "tinyxml.h"
#include "AppXmlParser.h"
#include "CBase64.h"
#include "TrsgCdr.h"
#include "CZip.h"
#include "TargetContent.h"
/******************************* Using Name Space ***************************************/
using namespace eSipUtil;
/******************************** Global Instance ****************************************/
KString g_clsProcName;
unsigned int g_unPid = 0;
KString g_clsBuildDate;
int g_unVwtrsgLogInst = 0;

/********************************* Main Local Function Defines *******************************/
static const char *s_fnStringSignal(int sig);
static void s_fnSignalHandle(int sig);
static void s_fnSetSignal();
static void s_fnSetProcName(int argc, char ** argv);
void get_process_id_list(std::vector<pid_t> &list)
{
	char command[256]; memset(command, 0x00, sizeof(command));
	char result[256]; memset(result, 0x00, sizeof(result));
	char *ptr = NULL;

	sprintf(command, "pidof %s", (KCSTR)g_clsProcName);
	// get "pidof" result as string
	FILE *fp = popen(command, "r");
	list.clear();
	if (fp)
	{
		size_t ret = fread(result, 1, 256, fp);
		pclose(fp);
		if (ret <= 0)
		{
			printf("failed to get pidof command result\n");
			return;
		}
		// convert as std::string and remove new line character(\n)
		std::string szres(result);
		szres.replace(szres.find('\n'), 1, "");
		// split string and push pid values in vector (delimeter = ' ')
		std::string szpid;
		std::istringstream stream(szres);
		while (getline(stream, szpid, ' '))
		{
			pid_t pid = strtol(szpid.c_str(), &ptr, 10);
			list.push_back(pid);
		}
	}
}
void main_memory_init()
{
	struct rlimit rlim;
	rlim.rlim_cur = MAIN_PROCESS_STACK_SIZE;
	rlim.rlim_max = MAIN_PROCESS_STACK_SIZE;
	setrlimit(RLIMIT_STACK, &rlim);
}

int g_fnGetLog()
{
	return g_unVwtrsgLogInst;
}
void s_fnCreateLog(const char *_pszProcName)
{
	KString clsLogPath; clsLogPath<<(KCSTR)"/logs/"<<_pszProcName<<(KCSTR)"/";
	std::cout << "clsLogPath:" << (KCSTR)clsLogPath << "\n";
	AsyncFileLog::m_fnInit((KCSTR)clsLogPath,(KCSTR)g_clsProcName, g_unVwtrsgLogInst,819200,0);
	AsyncFileLog::m_fnSetCategoryStr(g_unVwtrsgLogInst, E_VWTRSG_LOG_CATE_UTIL,"UTIL");
	AsyncFileLog::m_fnSetCategoryStr(g_unVwtrsgLogInst, E_VWTRSG_LOG_CATE_APP, "APP ");
	AsyncFileLog::m_fnSetCategoryStr(g_unVwtrsgLogInst, E_VWTRSG_LOG_CATE_TRSE,  "TRSE");
	AsyncFileLog::m_fnSetCategoryStr(g_unVwtrsgLogInst, E_VWTRSG_LOG_CATE_TRSS,  "TRSS");
	g_fnSetLog(E_VWTRSG_LOG_CATE_UTIL,E_LOG_DISABLE);
	g_fnSetLog(E_VWTRSG_LOG_CATE_APP,E_LOG_DEBUG);
	g_fnSetLog(E_VWTRSG_LOG_CATE_TRSE,E_LOG_DEBUG);
	g_fnSetLog(E_VWTRSG_LOG_CATE_TRSS,E_LOG_DEBUG);
	IFLOG(E_LOG_ERR,"CREATE MODULE : [%-30s] End",__func__);
}
bool g_fnCheckLog( eSipUtil::ELogLevel_t _eLogLevel)
{
	return AsyncFileLog::m_fnCheckLogLevel(g_unVwtrsgLogInst, E_VWTRSG_LOG_CATE_APP, _eLogLevel);
}
bool g_fnCheckTrseLog( eSipUtil::ELogLevel_t _eLogLevel)
{
	return AsyncFileLog::m_fnCheckLogLevel(g_unVwtrsgLogInst, E_VWTRSG_LOG_CATE_TRSE, _eLogLevel);
}
bool g_fnCheckTrssLog( eSipUtil::ELogLevel_t _eLogLevel)
{
	return AsyncFileLog::m_fnCheckLogLevel(g_unVwtrsgLogInst, E_VWTRSG_LOG_CATE_TRSS, _eLogLevel);
}
void g_fnSetLog(EVwtrsgLogCate_t _eCate,eSipUtil::ELogLevel_t _eLv)
{
	AsyncFileLog::m_fnSetLogLevel(g_unVwtrsgLogInst,_eCate,_eLv);
}
/********************************** MAIN **********************************************/
int main(int argc, char ** argv)
{
/******* Test *******/
/*
   const char* demoStart =
      "<?xml version=\"1.0\"  standalone='no' >\n"
      "<ToDo>\n"
      "<Item priority=\"2\" distance=\"aaaaa &amp;quot; aa\" </Item>"
      "</ToDo>";

   TiXmlDocument doc;
   doc.Parse(demoStart, 0, TIXML_ENCODING_LEGACY);

   TiXmlElement* rootElement = doc.RootElement();
   printf("root=%s\n\n", rootElement->Value());
   KString clsValue = rootElement->FirstChildElement("Item")->Attribute("distance");

   printf("get distance=%s\n\n", (KCSTR)clsValue);

   rootElement->FirstChildElement("Item")->SetAttribute("newAttr1", "abcd &quot; &amp; aaa");
   rootElement->FirstChildElement("Item")->SetAttribute("newAttr2", "ccc &\" ddd");
   TiXmlPrinter printer;
   doc.Accept( &printer );
   fprintf( stdout, "%s", printer.CStr() );

   return 0;
*/
/******* Test *******/

	main_memory_init();
	g_unPid = (unsigned int) getpid();
	g_clsBuildDate << (KCSTR) __DATE__ << " - " << (KCSTR) __TIME__;
	s_fnSetProcName(argc, argv);
	//중복 실행 방지
	std::vector<pid_t> pid_list;
	get_process_id_list(pid_list);
	if(pid_list.size() > 1)
	{
		printf("%s is already running\n", (KCSTR)g_clsProcName);
		return 0;
	}

	s_fnSetSignal();
	s_fnCreateLog((KCSTR)g_clsProcName);
	IFLOG(E_LOG_ERR,"Process Start [%s]", (KCSTR)g_clsProcName);
	g_fnCreateMainConfig();
	g_fnCreateAclSystemTable();
	g_fnCreateTrsgCdr();
	g_fnCreateWorker();
	g_fnCreateTrseNodeMgr();
	g_fnCreateTrssNodeMgr();
	g_fnCreateTrssTransport();//media
	g_fnCreateTrseTransport();
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
