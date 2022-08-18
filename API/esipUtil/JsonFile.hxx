#ifndef JSONFILE_HXX
#define JSONFILE_HXX
#include "JsonK.hxx"
#include "ConfigFile.hxx"
namespace eSipUtil
{
typedef enum
{
	E_JSON_FILE_NONE=0,
	E_JSON_FILE_ITEM,
	E_JSON_FILE_OBJECT,  // object
	E_JSON_FILE_LIST,      // object 
	E_JSON_FILE_MAX
}EJsonFile_t;
class JsonFile;
typedef void (*PFuncJsonChanged_t)(JsonFile * _pclsObj);
/*
	static void s_fnCbk(JsonFile *_pclsObj)
	{
		void * _pvUser = _pclsObj->m_fnGetUser();
		// user coding
	}
*/
typedef struct JsonTimerData_t
{
	JsonTimerData_t(){m_pclsJsonFile=NULL;}
	~JsonTimerData_t(){}
	JsonFile * m_pclsJsonFile;
}JsonTimerData_t;
/************************** Json File Object **********************************************/
class JsonFileObject : public JsonObject
{
	public:
		JsonFileObject();
		~JsonFileObject();
		JsonFileObject * m_fnGetObject(const char * _pszKey);
		KString & m_fnGetVal(const char * _pszKey,bool * _pbResult = NULL);
		bool m_fnGetComVal(CfgFile * _pclsCom,const char * _pszKey,KString &_rclsRetVal);
		bool m_fnAddItem(const char * _pszKey,KString _clsVal);
		JsonFileObject * m_fnAddList(const char * _pszKey);
		JsonFileObject * m_fnAddObject(const char * _pszKey);
		bool m_fnDel(const char * _pszKey);
		EJsonFile_t m_fnGetType();
		JsonFileObject * m_fnBeginItor(ListItr_t & _rstItor);
		JsonFileObject * m_fnNextItor(ListItr_t & _rstItor);	
		KString m_clsTemplateVal;
};
/*************************** Json File ***************************************************/
class JsonFile : public JsonK
{
	public:
		JsonFile();
		~JsonFile();
		//==========> File I/O
		bool m_fnLoadFile(const char * _pszPath,const char * _pszDefaultComment=NULL);
		bool m_fnReLoadFile(const char * _pszDefaultComment = NULL);
		char * m_fnBuildFile();
		bool m_fnWriteFile(const char * _pszBuild=NULL);
		void * m_fnGetUser(){return m_pvUser;}
		const char * m_fnGetPath(){return (KCSTR)m_clsFileName;}
		//==========> Register Changed File CallBack Function
		void m_fnSetNotifyChangedFile(PFuncJsonChanged_t _pFn,bool _bEnhanced = false,void * _pvUser=NULL);
		void m_fnSetNotifyChangedFileWithRead(PFuncJsonChanged_t _pFn,bool _bEnhanced = false,void * _pvUser=NULL);
		//==========> Internal File Time Check Function
		bool m_fnIsChangedFile();
		bool m_fnTimerCheckProc();
		//==========> Json Functions
		JsonFileObject * m_fnGetObject(const char * _pszKey);
		KString & m_fnGetVal(const char * _pszKey, bool * _pbResult = NULL);
		bool m_fnGetComVal(CfgFile * _pclsCom,const char * _pszKey, KString &_rclsRetVal);
		bool m_fnAddItem(const char * _pszKey,KString _clsVal);
		JsonFileObject * m_fnAddList(const char * _pszKey);
		JsonFileObject * m_fnAddObject(const char * _pszKey);
		bool m_fnDel(const char * _pszKey);
		//==========> Json Iterator
		JsonFileObject * m_fnBeginItor(ListItr_t & _rstItor);
		JsonFileObject * m_fnNextItor(ListItr_t & _rstItor);		
		CfgFileInfo m_clsFile;	
		void *m_pvUser;
		PFuncJsonChanged_t m_pfnChanged;
		KString m_clsFileName;
		bool m_bReadChanged;
};
}
#endif
