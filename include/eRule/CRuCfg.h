#ifndef _CRU_CFG_H_
#define _CRU_CFG_H_

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
#include "CRUFile.h"

extern char SDK_DefaultCfg[];
#define DEF_MAX_RU_CFG_FILE_SIZE    DEF_RU_1M
#define DEF_RU_CFG_MAGIC_CODE    "wldudehdbsfkdhs"
typedef enum
{
	E_RU_CFG_TYPE_NONE,
	E_RU_CFG_TYPE_COMMENT,
	E_RU_CFG_TYPE_CATEGORY,
	E_RU_CFG_TYPE_ITEM,
	E_RU_CFG_TYPE_NOT_DEFINED,
}ERuCfgType_t;
typedef struct DefaultCfgVal_t
{
	const char *subject;
	const char *key;
	const char *val;
}DefaultCfgVal_t;
inline const char * fnStringRuCfg(ERuCfgType_t _eT)
{
	if(_eT == E_RU_CFG_TYPE_COMMENT) return "COMMENT";
	else if(_eT == E_RU_CFG_TYPE_CATEGORY) return "CATEGORY";
	else if(_eT == E_RU_CFG_TYPE_ITEM) return "PARAM";
	else if(_eT == E_RU_CFG_TYPE_NOT_DEFINED) return "NOT_DEFINED";
	return "Unknown";
}
class RuCfgVal : public RUObject
{
	public:
		RuCfgVal();
		~RuCfgVal();
		void fnBuild(char * _pszBuild,unsigned int _unSize);
		void fnDebug(char * _pszDebug,unsigned int _unSize);
		void fnSetVal(ERuCfgType_t _eT, const char * _pszCategory, 
														const char * _pszKey, const char * _pszVal);
		bool fnIsEqual(ERuCfgType_t _eT, const char * _pszCategory,const char * _pszKey);
		RUString m_clsKey;
		RUString m_clsVal;
		RUString m_clsCategoryName;
		ERuCfgType_t m_eType;
};
class RuCfgCategoryNum : public RUObject
{
	public:
		RuCfgCategoryNum(){m_unParamNums=0;}
		~RuCfgCategoryNum(){}
		unsigned int m_unParamNums;
};
class RuCfgFile  
{
	public:
		RuCfgFile();
		~RuCfgFile();
		int fnReadFile(const char *filename=NULL);
		bool fnWriteFile(const char * _pszPath=NULL);
		void fnLoadConfig(const char * _pszPath,DefaultCfgVal_t *pDefault);
		void fnReLoadConfig();
		char * fnFindVal(const char *_pszCategory, const char *_pszKey);
		unsigned int fnGetByte(const char *StrSubject,const  char *StrKey);
		void fnSetVal(const char *StrSubject, char *StrKey,char *StrVal);
		void fnDebug(char * _pszDebug,unsigned int _unSize);
	private:
		void fnParse();
		void fnTokenizeParam(char * _pszOrig, char **_ppszKey, char ** _ppszVal);
		void fnClearConfig();
		bool fnStoreDefaultVal(const char *StrSubject, char *StrKey,char *StrVal);
		bool fnAutoStoreDefaultCfg(DefaultCfgVal_t *pDefault);
		bool fnAutoRemoveNotDefineCfg();
		RuCfgVal * fnAddVal(ERuCfgType_t _eT,const char *_pszCategory, 
															const char *_pszKey,const char *_pszVal);
		RuCfgVal * fnInsertVal(ERuCfgType_t _eT,const char *_pszCategory, 
										const char *_pszKey,const char *_pszVal,unsigned int _unIndex);
		RuCfgVal * fnFindParam(const char *_pszCategory, const char *_pszKey);
		bool fnIsExistCategory(const char * _pszCategory);
		unsigned int fnGetCategoryLineIndex(const char * _pszCategory);
		RuCfgCategoryNum * fnAddCateInfo(const char * _pszCategory);
		RuCfgCategoryNum * fnFindCateInfo(const char * _pszCategory);
		bool fnIsDefinedCategory(const char * _pszCategory);
		bool fnIsDefinedItem(const char * _pszCategory,const char *_pszKey);
		RuCfgVal * fnBegin(RuListItem_t & _rclsItem);
		RuCfgVal * fnNext(RuListItem_t & _rclsItem);
		RuCfgCategoryNum * fnBeginCate(RuListItem_t & _rclsItem);
		RuCfgCategoryNum * fnNextCate(RuListItem_t & _rclsItem);
		void fnBuild(char * _pszBuild, unsigned int _unSize);
		RUString m_clsFileName;
		char raw_data[DEF_MAX_RU_CFG_FILE_SIZE];
		DefaultCfgVal_t *pDefault_Param;
		RUList m_list;
		RUList m_listCateNum;
};

#endif
