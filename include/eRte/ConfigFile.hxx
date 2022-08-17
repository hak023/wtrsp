#ifndef _ESIPUTIL_CFG_H_
#define _ESIPUTIL_CFG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#ifdef _LINUX_
#include <sys/time.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>

#include "StlMap.hxx"
#include "string.hxx"

namespace eSipUtil
{
#define DEF_CFG_1K 1024
#define DEF_CFG_1M 1048576
#define DEF_CFG_1G 1073741824
#define DEF_MAX_CFG_FILE_SIZE    DEF_CFG_1M
#define DEF_CFG_MAGIC_CODE    "wldudehdbsfkdhs"
#define DEF_CFG_TRUE_SELECT(a,b,c)    ((a) >= 1 ? (b) : (c))
class CfgFile;
typedef enum
{
	E_CFG_TYPE_NONE,
	E_CFG_TYPE_COMMENT,
	E_CFG_TYPE_CATEGORY,
	E_CFG_TYPE_ITEM,
	E_CFG_TYPE_NOT_DEFINED,
}ECfgType_t;
typedef struct CfgTimerData_t
{
	CfgTimerData_t(){}
	~CfgTimerData_t(){}
	CfgFile * m_pclsCfg;
}CfgTimerData_t;
typedef struct DefaultCfgItem_t
{
	const char *subject;
	const char *key;
	const char *val;
}DefaultCfgItem_t;
inline const char * g_fnStringCfg(ECfgType_t _eT)
{
	if(_eT == E_CFG_TYPE_COMMENT) return "COMMENT";
	else if(_eT == E_CFG_TYPE_CATEGORY) return "CATEGORY";
	else if(_eT == E_CFG_TYPE_ITEM) return "PARAM";
	else if(_eT == E_CFG_TYPE_NOT_DEFINED) return "NOT_DEFINED";
	return "Unknown";
}
/********************************** File Time Info Class ************************************/
class CfgFileInfo
{
	public:
		CfgFileInfo();
		~CfgFileInfo();
		void m_fnInit(const char * _pszPath);
		bool m_fnUpdateTime();   // return value is changed modi time
		bool m_fnUpdateCRC();   // return value is changed crc
		void m_fnChangeLastTime(unsigned int _unTime);
		void m_fnChangeLastTime(const char * _pszTime);
		unsigned int m_fnGetFileSize();
		bool m_fnWriteFile(char * _pszBody,unsigned int _unLen,bool _bAsync=false);
		//------> File Changed Check
		void m_fnEnableCheckChanged(bool _bEnhanced = false);    // _bEnhanced Option is time, size + crc check 
		bool m_fnCheckChanged();
		//------> File Time Utils
		static char * m_fnGetTimeString(unsigned int _unTime,char * _pszSource);
		static unsigned int m_fnGetTimeInt(char * _pszSource);
		static void m_fnChangeTouchTime(const char * _pszPath,unsigned int _unTime);
		static void m_fnChangeTouchTime(const char * _pszPath,const char * _pszTime);		
		static unsigned long m_fnGetCRC(unsigned int _unCrc, char * _pszSrc, unsigned int _unSize);
		bool m_bIsDir;
		bool m_bEnhanced;
		KString m_clsPath;
		struct stat m_stFileInfo;
		tm m_stTm;
		char m_szTime[20];     // 2016-04-19-02-46-20
		int m_nYr;
		int m_nMon;
		int m_nDay;
		int m_nHr;
		int m_nMin;
		int m_nSec;	
		char * m_pszRawData;      //file raw data
		unsigned int m_unReadSz;  // file read size
		unsigned int m_unCurrentModiTime;
		unsigned long m_ulCurrentCrc;        // file crc code
		unsigned int m_unCurrentSize;    // file stat size
	private:
		void m_fnClear();
		unsigned int m_unPrevModiTime;
		unsigned long m_unPrevCrc;
		unsigned int m_unPrevSize;
		bool m_bCheckInit;
};
/***************************** Temporary Parse Class ***************************************/
class CfgParseLineString : public StlObject
{
	public:
		CfgParseLineString(){m_pszLine = NULL;}
		~CfgParseLineString(){}
		char * m_pszLine;
};
class CfgParseLineStrings
{
	public:
		CfgParseLineStrings(){}
		~CfgParseLineStrings(){}
		CfgParseLineStrings & operator+=(const char * _pszV)
		{
			if(KString::m_fnStrLen(_pszV)==0) return *this;
			CfgParseLineString *pNew = new CfgParseLineString;
			pNew->m_pszLine = (char*)_pszV;
			m_listString.m_fnPushBack(pNew);
			return *this;
		}
		CfgParseLineString & operator[](unsigned int _unIndex)
		{
			CfgParseLineString * pclsStr = (CfgParseLineString*)m_listString.m_fnGetIndex(_unIndex);
			if(pclsStr==NULL) return m_clsExept;
			return *pclsStr;
		}
		char * m_fnBegin(ListItr_t & _ritrItem)
		{
			CfgParseLineString * pclsFind = (CfgParseLineString*)m_listString.m_fnBegin(_ritrItem);
			if(pclsFind==NULL) return NULL;
			return pclsFind->m_pszLine;
		}
		char * m_fnNext(ListItr_t & _ritrItem)
		{
			CfgParseLineString * pclsFind = (CfgParseLineString*)m_listString.m_fnNext(_ritrItem);
			if(pclsFind==NULL) return NULL;
			return pclsFind->m_pszLine;
		}
		CfgParseLineString m_clsExept;
		StlList m_listString;
};
/********************* Config File Value Class ***********************************************/
class CfgValue : public StlObject
{
	friend class CfgFile;
	public:
		CfgValue();
		~CfgValue();
		operator KSTR()
		{
			KString::m_fnTrimTailString((KSTR)m_clsVal," ");
			return (KSTR)m_clsVal;
		}
		operator KCSTR()
		{
			KString::m_fnTrimTailString((KSTR)m_clsVal," ");
			return (KCSTR)m_clsVal;
		}
		operator KINT(){m_fnTrim();return (int)m_fnGetInt();}
		operator KUINT(){m_fnTrim();return m_fnGetInt();}
		operator KBOOL(){m_fnTrim();return m_fnGetBool();}
		bool operator==(KSTR _pszInput)
		{
			m_fnTrim();
			if(m_clsVal == (const char *)_pszInput) return true;
			else return false;
		}
		bool operator==(KCSTR _pszInput)
		{
			m_fnTrim();
			if(m_clsVal == (const char *)_pszInput) return true;
			else return false;
		}
		bool operator==(KUINT _unInput)
		{
			m_fnTrim();
			if(m_fnGetInt() == _unInput) return true;
			else return false;
		}
		bool operator==(KBOOL _bInput)
		{
			m_fnTrim();
			if(m_fnGetBool() == _bInput) return true;
			else return false;
		}
		CfgValue & operator=(KSTR _pszInput)
		{
			m_clsVal=KNULL;m_clsVal = (const char *)_pszInput;return *this;
		}
		CfgValue & operator=(KCSTR _pszInput)
		{
			m_clsVal=KNULL;m_clsVal = (const char *)_pszInput;return *this;
		}
		CfgValue & operator=(KUINT _unInput)
		{
			m_clsVal=KNULL;m_clsVal<<(KUINT)_unInput;return *this;
		}
		CfgValue & operator=(KBOOL _bInput)
		{
			m_clsVal=KNULL;
			if(_bInput == true) m_clsVal = (const char *)"1";
			else m_clsVal = (const char *)"0";
			return *this;
		}
		void m_fnDebug(char * _pszDebug,unsigned int _unSize);
		KString m_clsKey;
		KString m_clsVal;
		KString m_clsCategoryName;
		KString m_clsTemplateVal;
		ECfgType_t m_eType;
		bool m_bTrim;
	private:
		unsigned int m_fnGetInt();
		bool m_fnGetBool();
		void m_fnBuild(KString &_rclsBuild);		
		void m_fnSetVal(ECfgType_t _eT, const char * _pszCategory, 
														const char * _pszKey, const char * _pszVal);
		bool m_fnIsEqual(ECfgType_t _eT, const char * _pszCategory,const char * _pszKey);
		void m_fnTrim();
};
class CfgCategoryNum : public StlObject
{
	public:
		CfgCategoryNum(){m_unParamNums=0;}
		~CfgCategoryNum(){}
		unsigned int m_unParamNums;
};
typedef void (*PFuncCfgChanged_t)(CfgFile * _pclsObj);
/******************************Usage of CfgFile *******************************************/
/*
//======> 1. Default Config Sample
DefaultCfgItem_t DefaultCfg[]=
{
	{"main","log","1"},
	{"main","dellog_maxfile","0"},
	{NULL,NULL,NULL}
};
//======> 2. Loading
CfgFile mConf; mConf.m_fnLoadConfig("../config/CCM/CCM.cfg",NULL,false);
//======> 3. Get Value
unsigned int unVal = (KUINT)mConf.m_fnFindVal("main","ping_time");
bool bEnableTrace = (KBOOL)mConf.m_fnFindVal("main","enable_trace");
const char * pszIP = (KCSTR)mConf.m_fnFindVal("main","tcp_ip");
//======> 4. Diff Value
if(mConf.m_fnFindVal("main","ping_time") == (KUINT)2000)
if(mConf.m_fnFindVal("main","enable_trace") == (KBOOL)true)
if(mConf.m_fnFindVal("main","tcp_ip") == (KCSTR)"1.1.1.1")
5. Set Value
mConf.m_fnSetVal("main","ip","1.1.1.1");  // Add or Mod
mConf.m_fnSetVal("main","ha_enable",true);  // Add or Mod
mConf.m_fnSetVal("main","ping_time",1000);  // Add or Mod
mConf.m_fnFindVal("main","ping_time") = (KUINT)1000;   // Mod Only
*/
class CfgFile  
{
	public:
		CfgFile();
		~CfgFile();
		//------> Instance Init 
		void m_fnLoadConfig(const char * _pszPath,DefaultCfgItem_t *pDefault, bool _bAutoRemove=false);
		//------> Notify Changed. and User Must Reloading
		void m_fnReLoadConfig();
		//------> Write File
		bool m_fnWriteFile();
		//------> Find/Mod Function
		CfgValue & m_fnFindVal(const char *_pszCategory, const char *_pszKey, bool * _pbResult=NULL);
		//------> Get Common Config Template
		KString & m_fnGetComVal(CfgFile * _pclsCom, const char * _pszCategory, const char * _pszKey,bool * _pbResult=NULL);
		//------> Add/Mod Function
		void m_fnSetVal(const char *StrSubject, const char *StrKey,KString _clsVal);   // Don't Care Add Mod 
		//------> Get Debug String Function
		void m_fnDebug(char * _pszDebug,unsigned int _unSize);
		//------> Get Path String Function
		const char * m_fnGetPath(){return (KCSTR)m_clsFileName;}
		//------> Get User Instance
		void * m_fnGetUser(){return m_pvUser;};
		//------> Register Changed File CallBack Function
		void m_fnSetNotifyChangedFile(PFuncCfgChanged_t pFn,bool _bEnhanced = false,void * _pvUser=NULL);
		void m_fnSetNotifyChangedFileWithRead(PFuncCfgChanged_t pFn,bool _bEnhanced = false,void * _pvUser=NULL);
		//------> Internal File Time Check Function
		bool m_fnIsChangedFile();
		bool m_fnTimerCheckProc();
		//------> parse util function
		static bool m_fnTokenizeC(char *_pszOrig,const char *_pszTrimChr,CfgParseLineStrings &_rclsResult);
		static bool m_fnTokenizeS(char *_pszOrig,const char *_pszTrimStr,CfgParseLineStrings &_rclsResult);
		static bool m_fnTokenizeP(char *_pszOrig, const char *_pszPattern,CfgParseLineStrings &_rclsResult);
		static bool m_fnIsDir(const char *pname);
		static void m_fnCreateDir(const char * _pszPath);
		static void m_fnCreateDirExeptFile(const char * _szPath);
		static bool m_fnGetCommonCfg(const char * _pszOrigValue, std::string & _rclsCommonValue,CfgFile * _pclsCom);
		CfgFileInfo m_clsFileInfo;
	private:
		int m_fnReadFile();
		void m_fnParse(char * _pszRaw);
		void m_fnTokenizeParam(char * _pszOrig, char **_ppszKey, char ** _ppszVal);
		void m_fnClearConfig();
		bool m_fnStoreDefaultVal(const char *StrSubject, char *StrKey,char *StrVal);
		bool m_fnAutoStoreDefaultCfg(DefaultCfgItem_t *pDefault);
		bool m_fnAutoRemoveNotDefineCfg();
		CfgValue * m_fnAddVal(ECfgType_t _eT,const char *_pszCategory, 
															const char *_pszKey,const char *_pszVal);
		CfgValue * m_fnInsertVal(ECfgType_t _eT,const char *_pszCategory, 
										const char *_pszKey,const char *_pszVal,unsigned int _unIndex);
		CfgValue * m_fnFindParam(const char *_pszCategory, const char *_pszKey);
		bool m_fnIsExistCategory(const char * _pszCategory);
		unsigned int m_fnGetCategoryLineIndex(const char * _pszCategory);
		CfgCategoryNum * m_fnAddCateInfo(const char * _pszCategory);
		CfgCategoryNum * m_fnFindCateInfo(const char * _pszCategory);
		bool m_fnIsDefinedCategory(const char * _pszCategory);
		bool m_fnIsDefinedItem(const char * _pszCategory,const char *_pszKey);
		CfgValue * m_fnBegin(ListItr_t & _rclsItem);
		CfgValue * m_fnNext(ListItr_t & _rclsItem);
		CfgCategoryNum * m_fnBeginCate(ListItr_t & _rclsItem);
		CfgCategoryNum * m_fnNextCate(ListItr_t & _rclsItem);
		void m_fnBuild(KString &_rclsBuild);
		bool m_bAutoRemove;
		KString m_clsFileName;
		unsigned int m_unRealSize;
		unsigned int m_unReadSize;
		DefaultCfgItem_t *m_pstDefaultParams;
		StlList m_list;
		StlList m_listCateNum;
		CfgValue m_clsExepVal;
		void * m_pvUser;
		PFuncCfgChanged_t m_pfnChanged;
		bool m_bReadChanged;
};
}
#endif
