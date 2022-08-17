
#ifndef _ESIP_RULEUTILES_HXX
#define _ESIP_RULEUTILES_HXX
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <iterator>
#include <list>
#include <sys/types.h>
#include <ext/hash_map>

#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>
#include <ctype.h>
#include <sys/stat.h>
#include <strings.h>
#if defined(_LINUX_)
#include <sys/ioctl.h>
#include <linux/sockios.h>
#elif defined(_SUNOS_)
#	include <sys/sockio.h>
#endif 
#include <net/if.h>
#include <semaphore.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

#include "rutil/Logger.hxx"
#include "rutil/RWMutex.hxx"
#include "rutil/Lock.hxx"
#include "RuleApi.h"
#include "eSipLog.hxx"

#define RUDUMP_EMT "===============================================================================================\r\n"
#define RUDUMP_CHR "\r\n============================================= CHAR ============================================\r\n"
#define RUDUMP_HEX "\r\n============================================= HEX  ============================================\r\n"
#define RUBOX_RAW_NUM    98
#define RUBOX_HEX_NUM     33
#define RUBOX_RAW "|--------------------------------------------------------------------------------------------------|\r\n"
#define RUBOX_CHR "\r\n|-------------------------------------------- CHAR ------------------------------------------------|\r\n"
#define RUBOX_HEX "\r\n|-------------------------------------------- HEX -------------------------------------------------|\r\n"
#define RUBOX_EMT "|                                                                                                  |\r\n"

#include "RuleError.h"

/***************************** MMI Result Structure ****************************************/
#define D_SZ(a,b)    (a >= b ? a : b)
#define DEF_RULE_CATEGORY_SZ D_SZ(D_SZ(D_SZ(D_SZ(D_SZ(D_SZ(D_SZ(sizeof(LmRule_t),\
	sizeof(LmScreenOp_t)),sizeof(LmScreenOpSet_t)),sizeof(LmScreenAct_t)),\
	sizeof(LmRouteOp_t)),sizeof(LmFilterOp_t)),sizeof(LmFilterAct_t)),sizeof(LmFilterOpSet_t))
typedef struct LmRuleResult_t
{
	LmRuleResult_t(){m_fnClear();}
	void m_fnClear()
	{
		m_eCategory = E_RULE_CATEGORY_NONE;
		m_iReason = -1;
		m_iSubReason = -1;
		memset(&m_cBody,0x00, DEF_RULE_CATEGORY_SZ);
	}
	void m_fnHtoN()
	{
		switch(m_eCategory)
		{
			case E_RULE_CATEGORY_NONE:          break;                   
			case E_RULE_CATEGORY_RULE:         ((LmRule_t*)&m_cBody[0])->m_fnHtoN();break;        
			case E_RULE_CATEGORY_SCR_OP:     ((LmScreenOp_t*)&m_cBody[0])->m_fnHtoN();break;            
			case E_RULE_CATEGORY_SCR_OPS:      ((LmScreenOpSet_t*)&m_cBody[0])->m_fnHtoN();break;            
			case E_RULE_CATEGORY_SCR_ACT:      ((LmScreenAct_t*)&m_cBody[0])->m_fnHtoN();break;            
			case E_RULE_CATEGORY_RTE_OP:       ((LmRouteOp_t*)&m_cBody[0])->m_fnHtoN();break;            
			case E_RULE_CATEGORY_FILTER_OP:    ((LmFilterOp_t*)&m_cBody[0])->m_fnHtoN();break;         
			case E_RULE_CATEGORY_FILTER_ACT:   ((LmFilterAct_t*)&m_cBody[0])->m_fnHtoN();break;        
			case E_RULE_CATEGORY_MAX:          break;        
			default: break;       
		};
		m_eCategory = htonl(m_eCategory);
		m_iReason = htonl(m_iReason);
		m_iSubReason = htonl(m_iSubReason);
	}
	void m_fnNtoH()
	{
		m_eCategory = ntohl(m_eCategory);
		m_iReason = ntohl(m_iReason);
		m_iSubReason = ntohl(m_iSubReason);
		switch(m_eCategory)
		{
			case E_RULE_CATEGORY_NONE:          break;                   
			case E_RULE_CATEGORY_RULE:         ((LmRule_t*)&m_cBody[0])->m_fnNtoH();break;        
			case E_RULE_CATEGORY_SCR_OP:     ((LmScreenOp_t*)&m_cBody[0])->m_fnNtoH();break;            
			case E_RULE_CATEGORY_SCR_OPS:      ((LmScreenOpSet_t*)&m_cBody[0])->m_fnNtoH();break;            
			case E_RULE_CATEGORY_SCR_ACT:      ((LmScreenAct_t*)&m_cBody[0])->m_fnNtoH();break;            
			case E_RULE_CATEGORY_RTE_OP:       ((LmRouteOp_t*)&m_cBody[0])->m_fnNtoH();break;            
			case E_RULE_CATEGORY_FILTER_OP:    ((LmFilterOp_t*)&m_cBody[0])->m_fnNtoH();break;         
			case E_RULE_CATEGORY_FILTER_ACT:   ((LmFilterAct_t*)&m_cBody[0])->m_fnNtoH();break;        
			case E_RULE_CATEGORY_MAX:          break;        
			default: break;       
		};
	}
	void m_fnSetReason(int _iReason)
	{
		if(_iReason == E_ERR_RULE_SUCCESS)
		{
			m_iReason = 0;
			m_iSubReason = _iReason;
		}
		else
		{
			m_iReason = -1;
			m_iSubReason = _iReason;
		}
	}
	void m_fnSetBody(LmRule_t * _pstSrc)
	{
		m_fnClear();
		m_eCategory = E_RULE_CATEGORY_RULE;
		memcpy(&m_cBody[0],_pstSrc, sizeof(LmRule_t));
	}
	void m_fnSetBody(LmScreenOp_t * _pstSrc)
	{
		m_fnClear();
		m_eCategory = E_RULE_CATEGORY_SCR_OP;
		memcpy(&m_cBody[0],_pstSrc, sizeof(LmScreenOp_t));
	}
	void m_fnSetBody(LmScreenOpSet_t * _pstSrc)
	{
		m_fnClear();
		m_eCategory = E_RULE_CATEGORY_SCR_OPS;
		memcpy(&m_cBody[0],_pstSrc, sizeof(LmScreenOpSet_t));
	}
	void m_fnSetBody(LmScreenAct_t * _pstSrc)
	{
		m_fnClear();
		m_eCategory = E_RULE_CATEGORY_SCR_ACT;
		memcpy(&m_cBody[0],_pstSrc, sizeof(LmScreenAct_t));
	}
	void m_fnSetBody(LmRouteOp_t * _pstSrc)
	{
		m_fnClear();
		m_eCategory = E_RULE_CATEGORY_RTE_OP;
		memcpy(&m_cBody[0],_pstSrc, sizeof(LmRouteOp_t));
	}
	void m_fnSetBody(LmFilterOp_t * _pstSrc)
	{
		m_fnClear();
		m_eCategory = E_RULE_CATEGORY_FILTER_OP;
		memcpy(&m_cBody[0],_pstSrc, sizeof(LmFilterOp_t));
	}
	void m_fnSetBody(LmFilterOpSet_t * _pstSrc)
	{
		m_fnClear();
		m_eCategory = E_RULE_CATEGORY_FILTER_OPS;
		memcpy(&m_cBody[0],_pstSrc, sizeof(LmFilterOpSet_t));
	}
	void m_fnSetBody(LmFilterAct_t * _pstSrc)
	{
		m_fnClear();
		m_eCategory = E_RULE_CATEGORY_FILTER_ACT;
		memcpy(&m_cBody[0],_pstSrc, sizeof(LmFilterAct_t));
	}
	unsigned int m_eCategory; // ERuleApiCategory_t
	int m_iReason;
	int m_iSubReason;
	char m_cBody[DEF_RULE_CATEGORY_SZ];
	char * m_fnStr(char * _pszBuff,unsigned int _unSize)
	{
		return _pszBuff;
	}
}LmRuleResult_t;
/**************************** Rule Util Definition *******************************************/
#define DEF_RU_SZ_LOG       DEF_RU_1K*10
#define RESIPROCATE_SUBSYSTEM resip::Subsystem::APP
#define DEF_RU_1K 1024
#define DEF_RU_1M 1048576
#define DEF_RU_1G 1073741824
typedef char * RUSTR;
typedef char RUCHR;
typedef unsigned int RUINT;
typedef int RINT;
typedef unsigned long RULONG;
typedef bool RUBOOL;
typedef void * RUVOID;
typedef unsigned int RUKEYNUM;
typedef char * RUKEYSTR;
void RUMilliSleep(int msec);
void RUMicroSleep(int usec);
/************************************ Log *********************************************/
bool g_fnCheckLogLevel(resip::Log::Level _eLevel) ;
bool fnIsUtilLogLevel(eSipUtil::ELogLevel_t _eLevel);
void fnSetUtilLogConf(int _nLogInstance, int _nLogCategory);
unsigned int g_fnLogTime(char *_szDst);
typedef enum
{
	_NONE = eSipUtil::E_LOG_DISABLE,
	_ERR = eSipUtil::E_LOG_ERR,
	_WARNING = eSipUtil::E_LOG_WARN,
	_INFO = eSipUtil::E_LOG_INFO,
	_DEBUG = eSipUtil::E_LOG_DEBUG,
	_STACK = eSipUtil::E_LOG_STACK,
}ESrvLog_t;
inline const char * fnStringSrvLog(ESrvLog_t _eLog)
{
	if(_eLog == _ERR) return "[ERR    ][SVC] ";
	else if(_eLog == _WARNING) return "[WARNING][SVC] ";
	else if(_eLog == _INFO) return   "[INFO   ][SVC] ";
	else if(_eLog == _DEBUG) return "[DEBUG  ][SVC] ";
	else if(_eLog == _STACK) return "[STACK  ][SVC] ";

	else return "[NONE   ][SVC] ";
}
class RLog : public resip::Log
{
	public:
		RLog(){}
		~RLog(){}
		static void fnLock(){_arrAppMutex[E_LOG_APP_CORE].lock();}
		static void fnUnlock(){_arrAppMutex[E_LOG_APP_CORE].unlock();}
		static void fnBuildGuard(unsigned int _unMax, char *_szDst,ESrvLog_t _eLevel)
		{
			memset(_szDst,0x00,_unMax);
			fnGuardTime(_unMax,_szDst);
			strcat(_szDst,fnStringLogGuard(_eLevel));
		}
		static unsigned int fnGuardTime(unsigned int _unMax,char *_szDst)
		{
			struct timeval tval;
			struct tm tmTemp;
			gettimeofday(&tval, NULL);
			time_t m_time = (time_t)tval.tv_sec;
			long m_usec = tval.tv_usec;
			int m_msec = (int)((m_usec + 500) / 1000);
			_szDst[0] = '[';
			if (localtime_r(&m_time, &tmTemp) == NULL) _szDst[1] = '\0';
			if (strftime(&_szDst[1], _unMax, "%T", &tmTemp) == 0) _szDst[1] = '\0';
			int n = strlen(_szDst);
			if (n > 0)
			sprintf(_szDst + n, ".%03d", m_msec);
			n = strlen(_szDst);
			_szDst[n] = ']'; n++;
			return (unsigned int)n;
		} 
		static const char * fnStringLogGuard(ESrvLog_t _eLog)
		{
			if(_eLog == _ERR) return "[CRIT   ][SVC] ";
			else if(_eLog == _ERR) return "[ERR    ][SVC] ";
			else if(_eLog == _WARNING) return "[WARNING][SVC] ";
			else if(_eLog == _INFO) return   "[INFO   ][SVC] ";
			else if(_eLog == _DEBUG) return "[DEBUG  ][SVC] ";
			else if(_eLog == _STACK) return "[STACK  ][SVC] ";
			else return "[NONE   ][SVC] ";
		}
};
class AppAutoLog
{
	public:
		AppAutoLog(){RLog::fnLock();}
		~AppAutoLog(){RLog::fnUnlock();}
};
void RLOGb(const char *_szLog);
void RLOGvn(const char *_szLog, ...);
void RLOGv(const char *_szLog, ...);
void RLOGn(const char *_szLog);
void RLOG(const char *_szLog);
void RLoger(eSipUtil::ELogLevel_t _eLevel,const char *_szLog, ...);
#define LOGGER(_eLevel,args_...) \
do\
{\
if( fnIsUtilLogLevel((eSipUtil::ELogLevel_t)_eLevel))\
{\
RLoger((eSipUtil::ELogLevel_t)_eLevel,args_);\
}\
}while(false)

/******************************* Utile Functions ******************************************/
#define TRUE_SELECT(a,b,c)    ((a) >= 1 ? (b) : (c))

int fnRUStrCmp(const char * _sz1, const char * _sz2);
int fnRUStrnCmp(const char * _sz1, const char * _sz2,unsigned int _unLen);
int fnRUStrCaseCmp(const char * _sz1, const char * _sz2);
int fnRUStrnCaseCmp(const char * _sz1, const char * _sz2, unsigned int _unLen);
int fnRUStrLen(const char * _szString);
int fnRUStrCpy(char * _szDst, const char * _szSrc);
void fnRUStrnCpy(char * _pszDst,const char * _pszSrc, unsigned int _unLen);
int fnRUAtoi(const char * _szString);
char *fnRUStrStr(const char * _sz1, const char * _sz2);
char *fnRUStrCaseStr(const char * _sz1, const char * _sz2);
char * fnRUCreateCopy(const char * _pszSrc);
char * fnRUCreatenCopy(const char * _pszSrc,unsigned int _unLen);
int fnRUGetInt(char *_p);
void fnRUSetInt(int _nVal,char *_p);
int fnRUGetShort(char *_p);
void fnRUSetShort(int _nVal,char *_p);
const char *fnRUStr(char *_szStr);
const char * fnRUSpace(const char * _pszStr);
void fnRUStrCat(char *_szSrc,const char * _szVar,...);
int fnRUStrnCat(char *_szSrc,unsigned int _unMaxSz,const char * _szVar,...);
void fnRUStrnCatCpy(char * dst, const char * src, int _nMax);
bool fnRUIsASCII(char _chr);
void fnRUChrCat(void * _vData, char * _szResult,unsigned int _uiLen);
void fnRUHexCat(void * _vData, char * _szResult,unsigned int _uiLen);
void fnRUHexnCat(void * _vData,unsigned int _unHexLen, char * _szResult,unsigned int _unResultSize);
char *fnRUChrnBox(char *_szResource,unsigned int _unSrcLen,void *_pvRawData,int _unRawLen);
char *fnRUHexnBox(char *_szResource,unsigned int _unSrcLen,void *_pvRawData,int _unRawLen);
void fnRUIncreTimeSpec(unsigned int _unAddMs, struct timespec * _pstSrcTime);
/******************************** Mutex Lock *******************************************/
class RULock
{
	public:
		RULock(){}
		~RULock() {}
		void Lock() {m_clsMutex.lock();}
		void Unlock() { m_clsMutex.unlock(); }
		resip::Mutex m_clsMutex;
};
class RUAutoLock
{
	public:
		RUAutoLock(){m_pclsRWMutex = NULL;m_pclsMutex=NULL;}
		RUAutoLock(resip::RWMutex & _rclsMutex, bool _bWrite)
		{
			if(_bWrite) _rclsMutex.writelock();
			else _rclsMutex.readlock();
			m_pclsRWMutex = & _rclsMutex;m_pclsMutex = NULL;
		}
		RUAutoLock(resip::Mutex & _rclsMutex)
		{
			_rclsMutex.lock();m_pclsMutex = & _rclsMutex;m_pclsRWMutex = NULL;
		}
		~RUAutoLock()
		{
			if(m_pclsRWMutex) m_pclsRWMutex->unlock();
			if(m_pclsMutex) m_pclsMutex->unlock();
		}
	private:
		resip::RWMutex * m_pclsRWMutex;
		resip::Mutex * m_pclsMutex;
};
/**************************** Map, List Class ********************************************/
typedef void (*PfuncActionList_t)(unsigned long, void *);   // list data, user data
typedef void (*PfuncActionList2_t)(unsigned long, void *, void *);   // list data, user data1, user data2
typedef void (*PfuncActionList3_t)(unsigned long, void *, void *,void *);   // list data, user data1, user data2
typedef bool (*PfuncSortList_t)(unsigned long, unsigned long);    // list data, list data
typedef std::list<unsigned long> ListLong_t;
typedef ListLong_t::iterator RuListItem_t;
typedef  std::map<unsigned int, unsigned long> MapInt_t;
typedef std::map<std::string, unsigned long> MapStr_t;
typedef MapInt_t::iterator RuMapIntItem_t;
typedef MapStr_t::iterator RuMapStrItem_t;
typedef void (*PfuncActionMap_t)(unsigned long,void *);
typedef void (*PfuncTermMap_t)(unsigned long);
typedef enum
{
	E_RU_OBJECT_VAL_NONE,
	E_RU_OBJECT_VAL_INT,	
	E_RU_OBJECT_VAL_STRING,
	E_RU_OBJECT_VAL_OBJECT,
}ERUObjectVal_t;
typedef enum
{
	E_RU_OBJECT_KEY_NONE,
	E_RU_OBJECT_KEY_INT,
	E_RU_OBJECT_KEY_STRING,
	E_RU_OBJECT_KEY_OBJECT,
}ERUObjectKey_t;

/******************* Base Object of (RUList or RUMap) ***************************************/ 
// Support Int key and String key
class RUObject
{
	public:
		RUObject(){m_uiKey=0;m_pszKey = NULL;}
		virtual ~RUObject(){if(m_pszKey) delete [] m_pszKey;}
		bool operator==(unsigned int _uiKey)
		{
			if(m_uiKey == _uiKey) return true;
			return false;
		}
		bool operator==(const char * _pszKey)
		{
			if(fnRUStrCmp(m_pszKey,_pszKey)==0) return true;
			return false;
		}
		operator RUKEYNUM(){return m_uiKey;}
		operator RUKEYSTR(){return m_pszKey;}
		void fnSetKeyI(unsigned int _uiKey){m_uiKey = _uiKey;}
		void fnSetKeyS(const char * _pszKey)
		{
			if(m_pszKey) delete [] m_pszKey;
			m_pszKey = fnRUCreateCopy(_pszKey);
		}
		unsigned int m_uiKey;
		char *m_pszKey;
};
/******************************* RU stl list ********************************************/
class RUList
{
	public:
		RUList();
		~RUList();
		bool  fnPushBack(RUObject *_pclsObj,bool _bChk=false);
		bool  fnPushFront(RUObject *_pclsObj,bool _bChk=false);
		void * fnGetHead();
		void * fnGetTail();
		bool fnDelHead();
		bool fnDelTail();
		void * fnFindNode(unsigned int _uiKey);
		void * fnFindNode(const char * _pszKey);
		bool fnDelNode(unsigned int _uiKey);
		bool fnDelNode(const char * _pszKey);
		bool fnDelIndex(unsigned int _uiIndex);
		bool fnExtractIndex(RUObject ** _ppExtract, unsigned int _uiIndex);
		bool fnInsertIndex(RUObject * _pclsObj,unsigned int _uiIndex,bool _bChk);
		bool fnClear();
		void fnDebug();
		void * fnGetIndex(unsigned int _uiIndex);
		bool fnAction(PfuncActionList_t _pfn, void *_pvUser);
		bool fnAction2(PfuncActionList2_t _pfn, void *_pvUser1, void * _pvUser2);
		bool fnAction3(PfuncActionList3_t _pfn, void *_pvUser1, void * _pvUser2, void * _pvUser3);
		void fnSort(PfuncSortList_t _pfn);
		void fnRegiAction(PfuncActionList_t _pfn);
		void fnRegiSort(PfuncSortList_t _pfn);
		void fnReverse();
		void * fnBegin();
		void * fnNext();
		bool fnIsEnd();
		void * fnBegin(RuListItem_t & _rclsItem);
		void * fnNext(RuListItem_t & _rclsItem);
		bool fnIsEnd(RuListItem_t & _rclsItem);
		unsigned int fnGetSize();
		ListLong_t m_listR;
	private:
		void *fnFindNode(RUObject *_pclsObj);
		PfuncActionList_t m_pfnAction;
		PfuncSortList_t m_pfnSort;
		ListLong_t::iterator m_listItor;
};
/******************************* RU stl map *******************************************/
class RUMap
{
	public:
		RUMap();
		~RUMap();
		//void operator=(RUMap & _map);
		void * fnFindMap(unsigned int _nKey);
		void * fnFindMap(const char * _pszKey);
		bool fnSetMap(unsigned int _nKey, RUObject * _pclsObj);  
		bool fnSetMap(const char * _pszKey,RUObject * _pclsObj);
		bool fnDelMap(unsigned int _nKey);
		bool fnDelMap(const char * _pszKey);
		void fnAction(PfuncActionMap_t _pfn, void *_pvUser);
		void fnClear();
		void * fnBeginI();
		void *fnNextI();
		bool fnIsEndI();
		void * fnBeginI(RuMapIntItem_t & _rclsItem);
		void * fnNextI(RuMapIntItem_t & _rclsItem);
		bool fnIsEndI(RuMapIntItem_t & _rclsItem);
		void *fnBeginS();
		void * fnNextS();
		bool fnIsEndS();
		void * fnBeginS(RuMapStrItem_t & _rclsItem);
		void * fnNextS(RuMapStrItem_t & _rclsItem);
		bool fnIsEndS(RuMapStrItem_t & _rclsItem);
		unsigned int fnGetSize();
		MapInt_t m_mapR;
		MapStr_t m_mapS;
	private:
		MapInt_t::iterator m_mapItorInt;
		MapStr_t::iterator m_mapItorStr;
};

/********************************** RU Arrays ******************************************/
class RUArrayStringObject : public RUObject
{
	public:
		RUArrayStringObject();
		~RUArrayStringObject();
		void operator=(const char * _pszV);
		operator RUSTR();
		operator RUINT();
		void fnSet(const char * _pszVal);
		void fnSetnCpy(const char * _pszVal, unsigned int _unLen);
		void fnAddressing(const char * _pszVal);
		bool fnIsEmpty();
		void fnClear();
		char * m_pszVal;
		bool m_bAlloc;
};
class RUArrayString
{
	public:
		RUArrayString();
		~RUArrayString();
		RUArrayString & operator=(RUArrayString & _rclsSrc);
		RUArrayString & operator+=(const char * _pszV);
		RUArrayStringObject & operator[](unsigned int _unIndex);
		void fnSetName(const char * _pszName);
		// Add String Key & Val Funcs
		void fnAdd(const char * _pszKey,const char * _pszVal, bool bChk=false);   // alloc copy Back
		void fnAddnCpy(const char * _pszKey,const char * _pszVal,unsigned int _unLen,bool bChk=false);
		void fnAddressing(const char * _pszKey,const char * _pszVal, bool bChk=false);  // pointer copy Back
		void fnAddF(const char * _pszKey,const char * _pszVal, bool bChk=false);   // alloc copy Front
		void fnAddressingF(const char * _pszKey,const char * _pszVal, bool bChk=false);  // pointer copy Front
		char * fnFind(const char * _pszKey);
		char * fnBegin(char ** _ppszResultKey);
		char * fnNext(char ** _ppszResultKey);
		char * fnBegin(RuListItem_t &_rclsItem,char ** _ppszResultKey);
		char * fnNext(RuListItem_t &_rclsItem,char ** _ppszResultKey);
		RUArrayStringObject * fnStart();
		RUArrayStringObject * fnContinue();
		bool fnIsEmpty();
		static bool fnIsEmptyElement(const char * _pszVal);
		void fnDebug();
		void fnDisplay(char * _pszBuild, unsigned int _unSize);
		void fnDebugB();
		void fnClear();
		unsigned int fnGetSize();
		char * m_pszName;
		RUList m_listStr;
		static char m_szEmpty[3];
		static char m_szTrue[3];
		static char m_szFalse[3];
	private:
		RUArrayStringObject m_clsExept;
		static const char * fnStr(const char * _pszVal);
};
class RUArrayNum : public RUObject
{
	public:
		RUArrayNum();
		~RUArrayNum();
		RUArrayNum & operator=(unsigned long _unNum);
		bool operator==(unsigned long _unNum);
		operator RULONG();
		void fnSet(unsigned long _nNum);
		unsigned long m_Num;		
};
class RUArrayNums
{
	public:
		RUArrayNums();
		~RUArrayNums();
		RUArrayNums & operator=(RUArrayNums & _rSrc);
		RUArrayNums & operator+=(unsigned long _ulNum);
		RUArrayNum & operator[](unsigned int _unIndex);
		void fnAdd(unsigned long _ulNum);
		unsigned long  fnBegin();
		unsigned long fnNext();
		RUArrayNum * fnStart();
		RUArrayNum * fnContinue();
		bool fnIsEnd();
		unsigned long  fnBegin(RuListItem_t &_rclsItem);
		unsigned long fnNext(RuListItem_t &_rclsItem);
		RUArrayNum * fnStart(RuListItem_t &_rclsItem);
		RUArrayNum * fnContinue(RuListItem_t &_rclsItem);
		bool fnIsEnd(RuListItem_t &_rclsItem);
		void fnDebug();
		void fnClear();
		unsigned int fnGetSize();
		RUList m_listLong;
	private:
		RUArrayNum m_clsExept;
		
};
class RUArrayBool : public RUObject
{
	public:
		RUArrayBool();
		~RUArrayBool();
		RUArrayBool & operator=(bool _bBool);
		bool operator==(bool _bBool);
		operator RUBOOL();
		bool m_bBool;
};
class RUArrayBools
{
	public:
		RUArrayBools();
		~RUArrayBools();
		RUArrayBools & operator=(RUArrayBools & _rclsSrc);  // copy operator
		RUArrayBools & operator+=(bool _bBool);
		RUArrayBool & operator[](unsigned int _unIndex);
		RUArrayBools & operator&(RUArrayBools & _rclsSrc);  // and operator
		RUArrayBools & operator|(RUArrayBools & _rclsSrc);  // or operator
		RUArrayBools & operator||(RUArrayBools & _rclsSrc);  // xor operator
		void fnAddBool(const char * _pszKey,bool _bBool, bool _bChk=false);
		bool  fnBegin(char ** _ppszKey);
		bool fnNext(char ** _ppszKey);
		RUArrayBool * fnStart();
		RUArrayBool * fnContinue();
		bool fnIsEnd();
		bool  fnBegin(RuListItem_t &_rclsItem,char ** _ppszKey);
		bool fnNext(RuListItem_t &_rclsItem,char ** _ppszKey);
		RUArrayBool * fnStart(RuListItem_t &_rclsItem);
		RUArrayBool * fnContinue(RuListItem_t &_rclsItem);
		bool fnIsEnd(RuListItem_t &_rclsItem);
		void fnDebug();
		void fnDisplay(char * _pszLog, unsigned int _unSize);
		void fnClear();
		unsigned int fnGetSize();
		RUList m_listLong;
	private:
		RUArrayBool m_clsExept;
		static const char * fnStr(bool _bBool);
		static const char * fnStr(const char * _pszVal);
		void fnDefault(RUArrayBools * _pclsB);
		static const char m_szTrue[3];
		static const char m_szFalse[3];
};
class RUBuff : public RUObject
{
	public:
		RUBuff();
		~RUBuff();
		RUBuff & operator=(const char * _pszString);
		operator RUSTR();
		operator RUVOID();
		void * fnAlloc(const char * _pszKey, unsigned int _unSize);
		void * fnAllocCopy(const char * _pszKey, const void * _pvVal, unsigned int _unSize);
		void fnModi(void * _pszBuff, unsigned int _unSize);
		void fnClear();
		char * m_pszBuff;
		unsigned int m_unSize;
};
class RUBuffers
{
	public:
		RUBuffers();
		~RUBuffers();
		RUBuffers & operator=(RUBuffers & _rclsSrc);
		RUBuff & operator[](unsigned int _unIndex);
		RUBuffers & operator+=(const char * _pszString);
		void * fnGetAllocBuff(const char * _pszKey, unsigned int _unSize, bool _bChk=false);
		void * fnGetAllocCopyBuff(const char * _pszKey, const void * _pvVal, unsigned int _unSize, 
																				bool _bChk=false);
		void * fnFindBuff(const char * _pszKey);
		bool fnModiBuff(const char * _pszKey, void * _pszBuff, unsigned int _unSize);
		bool fnDelBuff(const char * _pszKey);
		RUBuff * fnStart();
		RUBuff * fnContinue();
		void fnReset();
		unsigned int unGetNum();
		RUList m_listBuff;	
	private:
		RUBuff m_clsExept;
};
/****************************** RU String *********************************************/
class RUIntString
{
	public:
		enum{E_RU_SZ_NUM = 64};
		RUIntString();
		RUIntString(long _ulNum);
		~RUIntString();
		RUIntString &operator=(long _ulNum);
		operator RUSTR(){return m_szNum;}
	private:
		void fnClear();
		char m_szNum[E_RU_SZ_NUM];
};
class RUPrint
{
	public:
		enum{E_RU_SZ_NUM = 64};
		RUPrint();
		RUPrint(int _nSpace, const char * _pszSrc);
		RUPrint(int _nSpace, long _lNum);
		~RUPrint();
		operator RUSTR();
		unsigned int fnSize();
		int m_nSpace;
		char * m_pszSrc;
		long  m_lNum;
		char * m_pszBuff;
};
class RUString
{
	public:
		enum{E_RU_SZ_NUM = 64,E_RU_SZ_CAT = DEF_RU_1K*10};
		RUString();
		RUString(const char * _szSrc);
		RUString(unsigned int unSize);
		~RUString();
		bool fnIsNull();
		RUString & operator=(const char * _szSrc);
		bool operator==(const char * _szSrc);
		bool operator==(RUString & _rclsCmp);
		RUString & operator+=(const char * _szSrc);                    // Add String Array Element
		RUString & operator<<(const char * _szSrc);                    // String Cat
		RUString & operator<<(const char  _ucInput);                   // Charicter Cat
		RUString & operator<<(RUIntString & _rclsInt);                  // Long Interger String Cat
		RUString & operator<<(RUPrint & _rclsPrint);                     // %5s, %-5s   String Cat
		RUArrayStringObject & operator[](unsigned int _unIndex);
		operator RUSTR(){return m_pszString;}
		char * fnGetString(){return m_pszString;}
		static char * fnGetOptimizeString(char * _pszSrc, const char * _pszTrim);
		static char * fnSkipString(const char * _pszSrc,const char * _pszTrim);
		static char * fnTrimTailString(char * _pszSrc, const char * _pszTrim);
		static bool fnTokenizeC(char *_pszOrig,const char *_pszTrimChr,RUArrayString *_pclsResult);  // char
		static bool fnTokenizeS(char *_pszOrig,const char *_pszTrimStr,RUArrayString *_pclsResult);   // string
		static bool fnTokenizeP(char *_pszOrig, const char *_pszPattern,RUArrayString *_pclsResult);  // pattern
		static unsigned int fnFixedStrCat(char * _pszDst,int _nSpace,const char * _pszSrc);
		static void fnBoxLog(const char *_szLog);
		char * fnBoxingChr(void *_pvSrc, unsigned int _unLen);
		char * fnBoxingHex(void *_pvSrc, unsigned int _unLen);
		unsigned int fnGetInt(){return fnRUAtoi(m_pszString);}
		void fnSetInt(unsigned long _ulNum);
		void fnDebug();
		char * fnGetIndexString(unsigned int _uiIndex);   // tok 파싱된 param Index로 접근
		bool fnTokChr(const char *_pszOrig,const char *_pszTrimChr);  //문자 단위 분리   입력 데이터 Parsing
		bool fnTokStr(const char *_pszOrig,const char *_pszTrimStr);  //문자열 단위 분리   입력 데이터 Parsing
		/*      pattern <:@>  Input <sip:1234@sam.net>                   */
		bool fnTokPattern(const char *_pszOrig, const char *pszPattern);  // 패턴 형태로 Parsing
		unsigned int fnGetParamSize();
		void fnStrCat(const char * _szVar,...);
		void fnStrnCat(unsigned int unSize,const char * _szVar,...);
		char *fnBegin(); // parsing 문자열 리스트 순차적으로 처리
		char *fnNext();   // parsing 문자열 리스트 순차적으로 처리
		char *fnBegin(RuListItem_t &_rclsItem); // parsing 문자열 리스트 순차적으로 처리
		char *fnNext(RuListItem_t &_rclsItem);   // parsing 문자열 리스트 순차적으로 처리
		void fnStoreData(const void * _pvData, unsigned int _unLen);
		char *m_pszString;
		unsigned int m_unLen;
		unsigned int m_unRealLen;
		RUArrayString m_Array;
	private:
		static void fnCbkDebug(unsigned long _uiData,void *_pvUser);
		static int fnStrStr2(char *_pszSrc,char * _pszKey);
		void fnClear();
		void fnCreateString(unsigned int _uiSize);
		void fnCreateString(const char * _szString);
		void fnIntCat(long _ulNum);
		void fnCat(const char * _szSrc);      // cat string
		void fnCat(const char _ucInput);    // cat charicter
		void fnSpaceCat(int _nSpace, const char * _szSrc);   // %-5s , %5s , string cat	
		void fnTrimParam();
};
/***************************** Ratio Control **********************************************/
class RateElement
{
	public:
		RateElement()
		{m_bConnected = false; m_unHBCnt=1;m_unRatio=1;m_unRateCnt=0;m_pclsNext= NULL;m_unIndex = 0;}
		~RateElement(){}
		bool fnIsAssign()
		{
			if(m_unRatio > m_unRateCnt){m_unRateCnt++; return true;}
			else if(m_unRatio == m_unRateCnt){m_unRateCnt++;return true;}
			else	{m_unRateCnt = 0; return false;}
		}
		void fnBlock(){m_bConnected = false; m_unHBCnt=1;m_unRatio=1;m_unRateCnt=0;}
		void fnUnBlock(){m_bConnected = true;}
		void fnSetRate(unsigned int _unRatio){m_unRatio = _unRatio;}
		bool m_bConnected;
		unsigned int m_unIndex;
		unsigned int m_unHBCnt;
		unsigned int m_unRatio;
		unsigned int m_unRateCnt;
		RateElement * m_pclsNext;
};
class RateControl
{
	public:
		RateControl(){m_unNums=0;m_pclsBegin=NULL;m_pclsCurrent=NULL;}
		~RateControl(){if(m_pclsBegin) delete [] m_pclsBegin;}
		void fnInit(unsigned int _unNums)
		{
			m_unNums = _unNums;
			m_pclsBegin = new RateElement[_unNums];
			m_pclsCurrent= m_pclsBegin;
			for(unsigned int i=0;i<m_unNums;i++)
			{
				m_pclsBegin[i].m_unIndex = i;
				if(i == (m_unNums-1)) m_pclsBegin[i].m_pclsNext = &m_pclsBegin[0];
				m_pclsBegin[i].m_pclsNext = &m_pclsBegin[i+1];
			}
		}
		RateElement * fnDesideDestination()
		{
			fnLock();
			if(IsAllDeActive()) {fnUnlock(); return NULL;}
			RateElement * pclsTmp = m_pclsCurrent;
			while(1)
			{
				if(pclsTmp->m_bConnected && pclsTmp->fnIsAssign())
				{
					m_pclsCurrent = pclsTmp;fnUnlock();return pclsTmp;
				}
				else{pclsTmp = pclsTmp->m_pclsNext;}
			}
			fnUnlock();
			return NULL;
		}
		bool fnSetRatio(unsigned int _unIndex, unsigned int _unRatio)
		{
			if(_unIndex > (m_unNums-1)) return false;
			m_pclsBegin[_unIndex].fnSetRate(_unRatio);
			return true;
		}
		bool fnBlock(unsigned int _unIndex)
		{
			fnLock();
			if(_unIndex > (m_unNums-1)) {fnUnlock();return false;}
			m_pclsBegin[_unIndex].fnBlock();
			fnUnlock();
			return true;
		}
		bool fnUnBlock(unsigned int _unIndex)
		{
			fnLock();
			if(_unIndex > (m_unNums-1)) {fnUnlock();return false;}
			m_pclsBegin[_unIndex].fnUnBlock();
			fnUnlock();
			return true;
		}
		bool IsAllDeActive(bool _bLock = false)
		{
			if(_bLock) fnLock();
			for(unsigned int i=0;i<m_unNums;i++)
			{
				if (m_pclsBegin[i].m_bConnected == true) {if(_bLock) fnUnlock(); return false;}
			}
			if(_bLock) fnUnlock();
			return true;
		}
		unsigned int m_unNums;
	private:
		void fnLock(){m_clsLock.Lock();}
		void fnUnlock(){m_clsLock.Unlock();}
		RateElement * m_pclsBegin;
		RateElement * m_pclsCurrent;
		RULock m_clsLock;
};
class BoxString
{
	public:
		BoxString()
		{
			m_pszOrig=NULL;m_pszBox = NULL;m_unOrigLen=0;m_unAssignLen=0;m_unOrigCnt=0;m_unAddCnt=0;
			m_unLineLenCnt=0;
		}
		BoxString(const char * _pszString)
		{
			m_pszOrig=NULL;m_pszBox = NULL;m_unOrigLen=0;m_unAssignLen=0;m_unOrigCnt=0;m_unAddCnt=0;
			m_unLineLenCnt=0;
			m_pszOrig = (char*)_pszString;
			m_unOrigLen = fnRUStrLen(_pszString); m_unAssignLen = m_unOrigLen*10;
			m_pszBox = new char[m_unAssignLen]; memset(m_pszBox,0x00,m_unAssignLen);
			fnBoxing();
		}
		~BoxString(){ if(m_pszBox) delete [] m_pszBox;}
		BoxString & operator=(const char * _pszString)
		{
			fnClear(); m_pszOrig = (char*)_pszString;
			m_unOrigLen = fnRUStrLen(_pszString); m_unAssignLen = m_unOrigLen*10;
			m_pszBox = new char[m_unAssignLen]; memset(m_pszBox,0x00,m_unAssignLen);
			fnBoxing();
			return *this;		
		}
		operator RUSTR(){return m_pszBox;}
	private:
		void fnClear()
		{
			if(m_pszBox) delete [] m_pszBox;m_pszOrig=NULL;m_unLineLenCnt=0;
			m_pszBox = NULL;m_unOrigLen=0;m_unAssignLen=0;m_unOrigCnt=0;m_unAddCnt=0;
		}
		void fnBoxing()
		{
			fnAddChr('\r');fnAddChr('\n');
			fnAddChr('|');	for(unsigned int i = 0;i<RUBOX_RAW_NUM;i++) fnAddChr('-');
			fnAddChr('|');fnAddChr('\r');fnAddChr('\n');
			fnCopyLine();
		}
		void fnCopyLine()
		{
			while(fnGetCurChar() != 0) fnAddLine();
			fnAddChr('|');	for(unsigned int i = 0;i<RUBOX_RAW_NUM;i++) fnAddChr('-');
			fnAddChr('|');fnAddChr('\r');fnAddChr('\n');
		}
		char fnGetCurChar(){return m_pszOrig[m_unOrigCnt];}
		void fnAddLine()
		{
			if(m_unOrigCnt >= m_unOrigLen) return;
			fnAddChr('|');
			while(1)
			{
				if(m_unLineLenCnt < (RUBOX_RAW_NUM-1))
				{
					if(fnGetCurChar() == '\r') fnChangeChr(' ');
					else if(fnGetCurChar() == '\n')
					{
						fnChangeChr(' ');fnAttach();	m_unLineLenCnt = 0;return;
					}
					else if(fnGetCurChar() == 0)
					{
						fnChangeChr(' ');fnAttach();	m_unLineLenCnt = 0;return;
					}
					else fnCopyChr();
					m_unLineLenCnt++;
				}
				else if(m_unLineLenCnt == (RUBOX_RAW_NUM-1))
				{
					fnCopyChr();fnAddChr('|');fnAddChr('\r');fnAddChr('\n');m_unLineLenCnt = 0;return;
				}
			}
		}
		void fnAttach()
		{
			fnAttachSpace(RUBOX_RAW_NUM - m_unLineLenCnt-1);m_unLineLenCnt=0;
			fnAddChr('|');fnAddChr('\r');fnAddChr('\n');
		}
		void fnAttachSpace(unsigned int _unRemain)
		{
			for(unsigned int i = 0; i<_unRemain;i++) fnAddChr(' ');
		}
		void fnChangeChr(const char _cInput)
		{
			fnAddChr(_cInput); m_unOrigCnt++;
		}
		void fnCopyChr()
		{
			fnAddChr(m_pszOrig[m_unOrigCnt++]);
		}
		bool fnAddChr(const char _cInput)
		{
			if(m_unAddCnt >= m_unAssignLen)
			{
				m_pszBox[m_unAssignLen-3] = '\r';
				m_pszBox[m_unAssignLen-2] = '\n';
				m_pszBox[m_unAssignLen-1] = 0;
				return false;
			}
			m_pszBox[m_unAddCnt++] = _cInput;
			return true;
		}
		unsigned int m_unOrigLen;
		unsigned int m_unAssignLen;
		unsigned int m_unOrigCnt;
		unsigned int m_unLineLenCnt;
		unsigned int m_unAddCnt;
		char * m_pszOrig;
		char * m_pszBox;
};
void fnRUReplaceString(char * _pszSrc,const char * _pszTrimStr,const char * _pszConvStr,
																			RUString &_rclsResult);
void fnUnEscapeToString(const char * _pszSrc,RUString &_rclsResult);
void fnEscapeToString(const char * _pszSrc, const char *_pszChangeChr, RUString &_rclsResult);
/***************************** Flexible Package Struct **************************************/
/*
	Package Sample Code
	char Data[10240];	Fps_t *pk = (Fps_t*)&Data[0];
	pk->Init(10240);
	pk->fnAddPK("hdr1", (void*)"Data1",5);
	pk->fnAddPK("hdr22", (void*)"Data22",6);
	pk->fnAddPK("hdr333", (void*)"Data333",7);
	pk->fnAddPK("hdr4444", (void*)"Data4444",8);
	char Debug[10240];
	pk->fnDebug(Debug);
	printf("Package\r\n%s",Debug);
	pk->fnRUawDebug(Debug);
	printf("\r\n%s",Debug);

	Fp_t *pfind= pk->fnFind("hdr22");
	if(pfind)
	{
		printf("Find Key = %s, Data = %s",pfind->fnKey(),pfind->fnData());
	}
*/
typedef struct Fp_t
{
	char m_usLen[2];
	char m_usDataLen[2];
	char m_usDataPos[2];
	char m_uiType[4];
	char m_uiSubType[4];
	enum{E_PK_HDR_SZ = 14};
	//======> public Functions
	void fnClear()
	{
		fnRUSetShort(0,&m_usLen[0]);fnRUSetShort(0,&m_usDataLen[0]);fnRUSetShort(0,&m_usDataPos[0]);
		fnRUSetInt(0,&m_uiType[0]);fnRUSetInt(0,&m_uiSubType[0]);
	}
	unsigned int fnSize(){return E_PK_HDR_SZ+fnRUGetShort(&m_usLen[0]);}
	char *fnKey(){return (char*)(this+1);}
	char *fnData(){return &(fnKey()[fnRUGetShort(&m_usDataPos[0])]);}
	void *fnVData(){return &(fnKey()[fnRUGetShort(&m_usDataPos[0])]);}
	unsigned int fnType(){return fnRUGetInt(&m_uiType[0]);}
	unsigned int fnSubType(){return fnRUGetInt(&m_uiSubType[0]);}
	unsigned int fnDataSize(){return fnRUGetShort(&m_usDataLen[0]);}
	void fnSetType(unsigned int _type){fnRUSetInt(_type,&m_uiType[0]);}
	void fnSetSubType(unsigned int _type){fnRUSetInt(_type,&m_uiSubType[0]);}
	void fnDebug(char * _szDebug,unsigned int _unSize)
	{
		if(fnRUGetShort(&m_usDataLen[0]))
		{
			fnRUStrnCat(_szDebug,_unSize,
				"{\r\n  <TLen=%d,DLen=%d,Data_Pos=%d,Type=%d,SubType=%d>\r\n",
				fnSize(),fnDataSize(),fnDataPos(),fnType(),fnSubType());
			fnRUStrnCat(_szDebug,_unSize,"  <Key=%s>\r\n  <Contents=%s>\r\n}\r\n",fnKey(),fnData());
		}
	}
	//======> private functions
	unsigned int fnDataPos(){return fnRUGetShort(&m_usDataPos[0]);}
	char *fnCurrentBody(){return &(fnKey()[fnRUGetShort(&m_usLen[0])]);}
	void *fnCurrentVBody(){return &(fnKey()[fnRUGetShort(&m_usLen[0])]);}
	void fnSetKey(const char * _szKey)
	{
		fnRUStrCpy(fnKey(),(char*)_szKey);	fnSetBodyLen(fnRUStrLen((char*)_szKey));
		fnNextString();fnSetDataPos(fnGetBodyLen());
	}
	void fnSetData(void *_vData,unsigned int _uiLen)
	{
		memcpy((void*)(fnData()),_vData,_uiLen); fnSetDataLen(_uiLen);
		fnIncreBodyLen(_uiLen);fnNextString();
	}
	void fnSetKeyLen(unsigned int _unLen){fnRUSetShort(_unLen,&m_usLen[0]);}
	void fnSetDataPos(unsigned int _unPos){fnRUSetShort(_unPos,&m_usDataPos[0]);}
	void fnSetDataLen(unsigned int _unLen){fnRUSetShort(_unLen,&m_usDataLen[0]);}
	void fnSetBodyLen(unsigned int _unLen){fnRUSetShort(_unLen,&m_usLen[0]);}
	unsigned short fnGetBodyLen(){return fnRUGetShort(&m_usLen[0]);}
	void fnIncreBodyLen(unsigned int _unLen)
	{
		unsigned short tmp = fnGetBodyLen();tmp+=_unLen;fnSetBodyLen(tmp);
	}
	void fnNextString()
	{
		char * pszCurr = fnCurrentBody();pszCurr[0]=0x00; fnIncreBodyLen(1);
	}
	bool fnFind(const char *_szKey)
	{
		if(fnRUStrCmp(fnKey(),(char*)_szKey)) return true;
		return false;
	}
	Fp_t *fnNext()
	{
		return (Fp_t*)(&fnKey()[fnRUGetShort(&m_usLen[0])]);
	}
}Fp_t;
typedef struct Fps_t
{
	unsigned short m_usLen;
	unsigned short m_usHdrNum;
	unsigned short m_usBufSize;
	enum{E_PKS_HDR_SZ = 6};
	void fnInit(unsigned short _BuffSize)
	{
		m_usBufSize = _BuffSize;fnClear();
		memset(fnBegin(),0x00,m_usBufSize-E_PKS_HDR_SZ);
	}
	void fnClear()
	{
		m_usLen = E_PKS_HDR_SZ;m_usHdrNum=0;Fp_t *pk = (Fp_t*)fnBegin();pk->fnClear();
	}
	char *fnBegin(){return (char*)(this+1);}
	Fp_t *fnAddPK(const char * _szKey,void *_vData, unsigned int _uiLen)
	{
		unsigned short us_AddLen = strlen((char*)_szKey) + _uiLen + 3 + Fp_t::E_PK_HDR_SZ;
		if(m_usLen + us_AddLen + 10 > m_usBufSize) return NULL;
		Fp_t *pk = (Fp_t*)(&fnBegin()[m_usLen-E_PKS_HDR_SZ]);
		pk->fnClear();pk->fnSetKey(_szKey);pk->fnSetData(_vData,_uiLen);
		m_usLen+= pk->fnSize();m_usHdrNum++;
		return pk;
	}
	Fp_t *fnAddVarPK(const char * _szKey,const char * _pszCat,...)
	{
		unsigned short us_AddLen = fnRUStrLen((char*)_szKey) + 2 + Fp_t::E_PK_HDR_SZ;
		if(m_usLen + us_AddLen  > m_usBufSize) return NULL;
		Fp_t *pk = (Fp_t*)(&fnBegin()[m_usLen-E_PKS_HDR_SZ]);
		pk->fnClear();pk->fnSetKey(_szKey);
		unsigned int unCurrentLen = pk->fnGetBodyLen(); unCurrentLen++;
		char * pszData = pk->fnData();
		unsigned short usTotLen = m_usLen+pk->fnSize();
		unsigned short usRemain = m_usBufSize - usTotLen;
		va_list ap;va_start(ap, _pszCat);vsnprintf(pszData,usRemain-1,_pszCat,ap);va_end(ap);
		unsigned int unLastLen = fnRUStrLen(pszData);
		pk->fnSetDataLen(unLastLen);
		pk->fnSetBodyLen(unCurrentLen+unLastLen);
		m_usLen+= pk->fnSize();m_usHdrNum++;
		return pk;
	}
	Fp_t *fnGetIndex(unsigned short _uiIndex)
	{
		if(m_usHdrNum ==0) return NULL;
		Fp_t *pk = (Fp_t*)fnBegin();
		if(_uiIndex==0) return pk;
		pk  = pk->fnNext();
		for(unsigned short i=1;i<m_usHdrNum;i++)
		{
			if(i == _uiIndex) return pk;
			pk  = pk->fnNext();
		}
		return NULL;
	}
	Fp_t *fnFind(const char *_szKey)
	{
		if(m_usHdrNum ==0) return NULL;
		Fp_t *pk = (Fp_t*)fnBegin();
		if(pk->fnFind(_szKey)) return pk;
		pk  = pk->fnNext();
		for(unsigned short i=1;i<m_usHdrNum;i++)
		{
			if(pk->fnFind(_szKey)) return pk;
			pk  = pk->fnNext();
		}
		return NULL;
	}
	Fp_t *fnFindType(unsigned int _uiType)
	{
		if(m_usHdrNum ==0) return NULL;
		Fp_t *pk = (Fp_t*)fnBegin();
		if(pk->fnType() == _uiType) return pk;
		pk  = pk->fnNext();
		for(unsigned short i=1;i<m_usHdrNum;i++)
		{
			if(pk->fnType() == _uiType) return pk;
			pk  = pk->fnNext();
		}
		return NULL;
	}
	Fp_t *fnFindSubType(unsigned int _uiType)
	{
		if(m_usHdrNum ==0) return NULL;
		Fp_t *pk = (Fp_t*)fnBegin();
		if(pk->fnSubType() == _uiType) return pk;
		pk  = pk->fnNext();
		for(unsigned short i=1;i<m_usHdrNum;i++)
		{
			if(pk->fnSubType() == _uiType) return pk;
			pk  = pk->fnNext();
		}
		return NULL;
	}
	void fnDebug(char * _szDebug,unsigned int _unLen)
	{
		if(m_usHdrNum ==0) return;
		memset(_szDebug,0x00,_unLen);
		fnRUStrnCat(_szDebug,_unLen,
							"<---------------------------------------------- PK ------------------"\
							"---------------------------->\r\nLen=%d,HdrNum=%d\r\n",
							m_usLen,m_usHdrNum);
		Fp_t *pk = (Fp_t*)fnBegin();pk->fnDebug(_szDebug,_unLen);pk  = pk->fnNext();
		for(unsigned short i=1;i<m_usHdrNum;i++)
		{
			pk->fnDebug(_szDebug,_unLen);pk  = pk->fnNext();
		}
		fnRUStrnCat(_szDebug,_unLen,
							"<-------------------------------------------------------------------"\
							"------------------------------->\r\n");
	}
	void fnRUawDebug(char * _szDebug, unsigned int _unLen)
	{
		memset(_szDebug,0x00,_unLen);
		fnRUChrnBox(_szDebug,_unLen,this,fnSize());
		fnRUHexnBox(_szDebug,_unLen,this,fnSize());
	}
	unsigned int fnSize(){return m_usLen;}
}Fps_t;

#endif

