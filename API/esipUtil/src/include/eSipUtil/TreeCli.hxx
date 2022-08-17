#ifndef _ESIPUTIL_TREECLI_
#define _ESIPUTIL_TREECLI_


#include <stdlib.h>
#include <string.h>	
#include <signal.h>
#include <list>
#include <algorithm>
#include <vector>
#include <functional>
#include <queue>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include "queueThread.hxx"
#include "transportMgr.hxx"
#include "string.hxx"
#include "StlMap.hxx"
#include "lock.hxx"


namespace eSipUtil
{
#define DEF_MAX_CLI_LINE_START      40
#define DEF_MAX_CLI_LINE_END       100
#define DEF_MAX_CLI_CHAR     10240
extern char g_szDefClistring[];

#define DEF_MAX_CLI_COMMAND_LINE   1024
#define DEF_MAX_CLI_ARG   50

#define CLI_BS	0x08
#define CLI_DEL 0x7f
#define CLI_ESC 0x1b
#define CLI_TAB 0x09
#define CLI_LF  0x0a
#define CLI_CR  0x0d
#define CLI_F1	0xa0
#define CLI_F2	0xa1
#define CLI_F3	0xa2
#define CLI_F4	0xa3
#define CLI_UP	0xb0
#define CLI_DOWN	0xb1
#define CLI_RIGHT	0xb2
#define CLI_LEFT	0xb3
#define CLI_SPACE	0x20
#define CLI_SLASH 	0x2f
#define CLI_DOT		0x2e
#define CLI_COMMA	0x2c
#define CLI_SEMI	0x3b
#define CLI_COLONE 0x3a
#define DEF_CLI_MAGIC 0x1b2c3d4f
typedef enum
{
	E_CLI_IDLE,
	E_CLI_INSERT,
	E_CLI_EXECUTE,
	E_CLI_MAX,
}ECliState_t;
typedef enum
{
	E_CLI_ST_START,
	E_CLI_ST_IDLE,
	E_CLI_ST_INSERTING,
	E_CLI_ST_EXECUTING,
	E_CLI_ST_SUCCESS,
}ECliHandle_t;
typedef enum ECliMode_t
{
	E_CLI_MOD_NONE = 0,
	E_CLI_MOD_CONSOL,
	E_CLI_MOD_SERVER,
	E_CLI_MOD_CLIENT,
	E_CLI_MOD_MAX
}ECliMode_t;
typedef enum
{
	E_CLI_EVENT_NONE = 0,
	E_CLI_EVENT_STDIN,
	E_CLI_EVENT_STDOUT,
	E_CLI_EVENT_APP,
}ECliEv_t;
/**************************** Cli Consol Data **********************************************/
typedef struct
{
	char m_szCliData[DEF_MAX_CLI_COMMAND_LINE];
	char m_unLen;
	char m_cTemp;
	int m_nCallCnt;
}CliInputData;
/******************************** Cli IPC Data ********************************************/
typedef struct CliHdr_t
{
	char m_unMagic[4];
	char m_unLen[4];
	char m_szPrompt[256];
	Net5Tuple_t m_stAddr;
}CliHdr_t;
typedef struct CliNet_t
{
	CliHdr_t m_stHdr;
	void m_fnInit()
	{
		m_fnSetInt(DEF_CLI_MAGIC,&m_stHdr.m_unMagic[0]);
	}
	char * m_fnBody(){return (char*)(this+1);}
	unsigned int m_fnGetLen(){return m_fnGetInt(&m_stHdr.m_unLen[0]);}
	bool m_fnGetValid()
	{
		if(m_fnGetInt(&m_stHdr.m_unMagic[0]) == DEF_CLI_MAGIC) return true;
		return false;
	}
	ECliEv_t m_fnGetEvType()
	{
		return (ECliEv_t)m_fnGetInt(&m_stHdr.m_unMagic[0]);
	}
	void m_fnSetEvType(ECliEv_t _eT)
	{
		m_fnSetInt(_eT,&m_stHdr.m_unMagic[0]);
	}
	void m_fnSetBody(const char * _pszBody)
	{
		if(_pszBody==NULL) return;
		memcpy(m_fnBody(),_pszBody,strlen(_pszBody));
		m_fnSetInt(sizeof(CliHdr_t)+strlen(m_fnBody())+1,&m_stHdr.m_unLen[0]);
	}
	void m_fnSetPrompt(const char * _pszPrompt)
	{
		if(_pszPrompt==NULL) return;
		strncpy(m_stHdr.m_szPrompt,_pszPrompt,255);
	}
	unsigned int m_fnGetInt( char *p)
	{
		return (((unsigned int)((unsigned char)p[0]))<<24) + (((unsigned int)((unsigned char)p[1]))<<16)
			+(((unsigned int)((unsigned char)p[2]))<<8) + ((unsigned int)((unsigned char)p[3]));
	}
	void m_fnSetInt( int val, char *p)
	{
		p[0] = (char)((val >> 24) & 0xff);p[1] = (char)((val >> 16) & 0xff);
		p[2] = (char)((val >> 8) & 0xff);p[3] = (char)((val) & 0xff);
		return;
	}
	char * m_fnDebug(KString & _rclsDebug)
	{
		char szAddr[128]; m_stHdr.m_stAddr.getStr(szAddr,128);
		_rclsDebug<<"\r\n{\r\nMagic : "<<m_fnGetInt(&m_stHdr.m_unMagic[0])<<"\r\n";
		_rclsDebug<<"unLen : "<<m_fnGetLen()<<"\r\n";
		_rclsDebug<<"Prompt : "<<m_stHdr.m_szPrompt<<"\r\n";
		_rclsDebug<<"Addr : "<<szAddr<<"\r\n";
		_rclsDebug<<"Body : "<<m_fnBody()<<"\r\n}\r\n";
		return (KSTR)_rclsDebug;
	}
}CliNet_t;
inline CliNet_t * g_fnBuildCliString(const char * _pszLine)
{
	unsigned int unLen = KString::m_fnStrLen(_pszLine);
	if(unLen == 0)
	{
		CliNet_t * pstPk = (CliNet_t*)new char [sizeof(CliHdr_t) + 3];
		memset(pstPk,0x00,sizeof(CliHdr_t) + 3);
		pstPk->m_fnInit();	pstPk->m_fnSetBody("\r\n");
		return pstPk;
	}
	else
	{
		CliNet_t * pstPk = (CliNet_t*)new char [sizeof(CliHdr_t) + unLen + 1];
		memset(pstPk,0x00,sizeof(CliHdr_t) + unLen + 1);
		pstPk->m_fnInit();pstPk->m_fnSetBody(_pszLine);
		return pstPk;
	}
	return NULL;
}
inline void g_fnFreeCliString(CliNet_t * _pszPk)
{
	delete [] (char*)_pszPk;
}
/************************** Node Tree CallBack Function Defined *******************************/
class TreeCli;
class CliSession;
// pclsCli - Cli Object
// pclsSess - Cli Server Client Session Object
// pstPk - Recv Packet From Remote Client
// arg - token string(param arrays)
// param_num - param array nums
// local console mod only use >> arg, param_num 
typedef void (*PFuncCli)(TreeCli *pclsCli,CliNet_t * pstPk,char *arg[],int param_num);
/************************************ CLI Basic Functions **********************************/
void g_fnCliCmdView(TreeCli *pCli,char *Arg[],int param_num);
bool g_fnCliChangeDirProcess(TreeCli *pCli,char *arg[],int param_num);
bool g_fnCliViewDirectory(TreeCli *pCli,char *arg[],int param_num);
bool g_fnCliExitProcess(TreeCli *pCli,char *arg[],int param_num);
bool g_fnCliCatProcess(TreeCli *pCli, char *arg[],int param_num);
void g_fnAddClientCliTransport(TransportMgr * _pclsMgr,const char * _pszServerIP,
									unsigned int _unServerPort,
									const char * _pszMyIP, unsigned int _unMyPort);
/********************************** CLI State Functions ************************************/
void g_fnCliIdle(TreeCli *pclsCli,CliSession *_pclsSess, CliNet_t *_pstPk);
void g_fnCliInserting(TreeCli *pCli,CliSession *_pclsSess,CliNet_t *_pstPk);
void g_fnCliExeCuting(TreeCli *pCli,CliSession *_pclsSess,CliNet_t *_pstPk);
void g_fnCliSuccess(TreeCli *pCli,CliSession *_pclsSess,CliNet_t *_pstPk);
void g_fnCliChange(ECliHandle_t Func_num,TreeCli *pCli);
/********************************* CLI Print Functions **************************************/
void g_fnCliPrint_RangeCat(char *strDebug,const char *pFormat, ...);
void g_fnCliPrint_Default(const char *pFormat, ...);
void g_fnCliPrint_Range(const char *pFormat, ...);
void g_fnCliOS_Command(const char *pFormat, ...);
void g_fnCliPrintEv(TreeCli * _pclsCli,CliSession * _pclsSess,CliNet_t * _pstPk,const char * _pszBuff);
void g_fnCliPrintEvVar(TreeCli * _pclsCli,CliSession * _pclsSess,CliNet_t * _pstPk,const char *pFormat, ...);
/********************************* CLI Node handle Structure *******************************/
typedef struct
{
	void *Parents;                                                     // parents handler
	void *Child;                                                         // child handler
	char key[128];
	PFuncCli Cli_Handle;
	char desc[512];
	bool m_fnCliProcess(TreeCli *pCli,char *arg[],int param_num)
	{
		KString clsRsp;
		if(param_num == 0) return true;                    // skipp and return cli
		if(m_fnIsNull()) return true;                                    // skipp and return cli
		if(g_fnCliChangeDirProcess(pCli,arg,param_num)) return true;
		if(g_fnCliViewDirectory(pCli,arg,param_num)) return true;
		if(g_fnCliExitProcess(pCli,arg,param_num)) return true;
		if(g_fnCliCatProcess(pCli,arg,param_num)) return true;
		if(strlen(key) == 0)                                        // not match key = "";
		{
			printf("Not Match Key : %s\r\n",arg[0]);
			if(Cli_Handle) Cli_Handle(pCli,NULL,arg,param_num);
			return true;                                              //catch and return cli
		}
		if((strncmp(key,arg[0],strlen(key))==0) && (strlen(key) == strlen(arg[0])))
		{
			if(Cli_Handle) Cli_Handle(pCli,NULL,arg,param_num);
			else
			{
				printf("is Directory\r\n");
			}
			return true;                                              //catch and return cli
		}
		return false;                                                   // not match and next handle
	}
	char *m_fnStringType()
	{
		if(Child == NULL) return (char*)"EXE";
		else return (char*)"DIR";
	}
	bool m_fnIsNull()
	{
		if((Child == NULL) && (Cli_Handle == NULL) && (strlen(key) == 0)) return true;
		else return false;
	}
}CliParam_t;
/******************************** Cli Client Session ****************************************/
class CliSession : public StlObject
{
	public:
		CliSession(TreeCli* _pclsOwner);
		~CliSession();
		void m_fnProcPk(CliNet_t *_pstPk,char *arg[],int param_num);
		bool m_fnCliChangeDirProcess(CliNet_t *_pstPk,char *arg[],int param_num);
		bool m_fnCliViewDirectory(CliNet_t *_pstPk,char *arg[],int param_num);
		void m_fnCliCmdView(CliNet_t *_pstPk,char *Arg[],int param_num);
		bool m_fnCliOsProcess(CliNet_t *_pstPk,char *arg[],int param_num);
		bool m_fnCliExitProcess(CliNet_t *_pstPk,char *arg[],int param_num);
		bool m_fnCliCatProcess(CliNet_t *_pstPk,char *arg[],int param_num);
		void m_fnSendRsp(CliNet_t * _pstPk, const char * _pszRsp);
		bool m_fnIsNull();
		CliParam_t * m_fnFindDir(CliParam_t * _pstHandle, const char * _pszDirName);
		CliParam_t *m_pclsCurParam;
		KString m_clsPrompt;
		TreeCli * m_pclsOwner;
};
/************************************ Cli Thread ****************************************/
class TreeCli
{
	public:
		typedef void (*PFuncNotify_t)(TreeCli * _pclsCli, CliNet_t * _pstPk);
		TreeCli(CliParam_t *_psthParam,const char * _pszPromptName);
		~TreeCli();
		//=========> Start Function
		void m_fnInit(ECliMode_t _eT, const char * _pszSrvIP, unsigned int _unSrvPort, int nCoreID=-1);
		void m_fnSetAppCallBack(PFuncNotify_t _pfn);
		//==========> Stop Function
		void m_fnStop();
		//==========> Packet Event Put Queue
		void m_fnSetEvent(CliNet_t * _pstData){if(m_pclsQThread) m_pclsQThread->put(_pstData);}
		//==========> Print Display
		void m_fnPrint(CliNet_t * _pstInfo,const char * pFormat,...);
		//==========> Client Transport Functions
		void m_fnConnectClient(const char * _pszIP, unsigned int _unPort,const char * _pszMyIP, unsigned int _unMyPort);
		void m_fnSendToSrv(const char * _pszLine);
		//==========>  Server Session Mgr Functions 
		CliSession * m_fnAddSession(const Net5Tuple_t & _rstAddr);
		CliSession * m_fnFindSession(const Net5Tuple_t & _rstAddr);
		bool m_fnDelSession(const Net5Tuple_t & _rstAddr);
		RwMutex m_clsLock;
		ECliHandle_t Func_num;
		ECliHandle_t Prev_Func;
		CliParam_t *CurrentParam;
		CliParam_t * RootParam;
		char m_szCliName[256];
		char m_szPrompt[256];
		char m_szOrgPrompt[256];
		QueueThread<CliNet_t> * m_pclsQThread;
		NormalThread<TreeCli> * m_pclsGetChr;
		KString m_clsGetBuff;
		TransportMgr *m_pclsTransport;
		StlMap m_mapSes;
		ECliMode_t m_eMod;
		KString m_clsSrvIP;
		unsigned int m_unSrvPort;
		KString m_clsLocalIP;
		unsigned int m_unLocalPort;
		int m_nCoreID;
	protected:
		void m_fnRunFunc(void);
	private:
		PFuncNotify_t m_pfnNotify;
		bool m_fnStart();
		void m_fnAddServer(const char * _pszIP, unsigned int _unPort);
		static void m_fnProcess(CliNet_t* _pstData, void * _pvUser);
		void m_fnProcessMsg(void *_pvMsg);
		void m_fnCliHande(ECliHandle_t func_type);
		static void m_fnCbkGetChr(TreeCli * _pclsT);
};
}

#endif
