#ifndef ESIP_RULEFILE_HXX
#define ESIP_RULEFILE_HXX
#include "CRuleUtiles.h"

class RuleFileParam;
typedef void * (*PfuncRFileBuilder)(RuleFileParam *,unsigned int *);  // return userdata, usertype
typedef void (*PfuncRFileTerminator)(void *, unsigned int);   // User Data, User_Type
typedef void (*PfuncRFileAction)(void *,unsigned int);    // User Data, user type;

/************************* Rule File Param Constructor ************************************/
class RuleFileParam : public RUObject
{
	public:
		RuleFileParam();
		~RuleFileParam();
		RUArrayStringObject & operator[](unsigned int _unIndex)
									{return m_clsString[_unIndex];}
		bool fnParse(const char * _szLine);
		void fnSetCbk(PfuncRFileBuilder _pfnBuilder,
							PfuncRFileTerminator _pfnTerminator,PfuncRFileAction _pfnAction);
		char *fnGetParam(unsigned int _uiIndex);
		void fnBuild(char * _szBuild);
		void fnBuild(char * _szBuild, RUArrayNums * pclsNums);
		void fnUserAction();
		void fnDebug();
		char * fnBegin(){return m_clsString.fnBegin();}
		char * fnNext(){return m_clsString.fnNext();}
		char * fnBegin(RuListItem_t &_rclsItem){return m_clsString.fnBegin(_rclsItem);}
		char * fnNext(RuListItem_t &_rclsItem){return m_clsString.fnNext(_rclsItem);}
		char *m_pszType;
		char *m_pszID;
		RUString m_clsString;
		void *m_pvUser;
		unsigned int m_uiUserType;
		PfuncRFileTerminator m_pfnUserTerminator;
		PfuncRFileAction m_pfnUserAction;
};

/************************** Rule File Category Constructor **********************************/
class RuleFileCategory : public RUObject
{
	public:
		RuleFileCategory();
		~RuleFileCategory();
		void fnSetComment(const char * _szComment);
		bool fnAddParam(RuleFileParam * _pstParam);
		bool fnDelParam(const char * _szKey);
		RuleFileParam *fnFindParam(const char * _szKey);
		void fnBuild(char * _szBuild);
		void fnDebug();
		void fnSortParam(PfuncSortList_t _pfnSort);
		RuleFileParam* fnBegin(){return (RuleFileParam*)m_listLine.fnBegin();}
		RuleFileParam * fnNext(){return (RuleFileParam*)m_listLine.fnNext();}
		RuleFileParam* fnBegin(RuListItem_t &_rclsItem){return (RuleFileParam*)m_listLine.fnBegin(_rclsItem);}
		RuleFileParam * fnNext(RuListItem_t &_rclsItem){return (RuleFileParam*)m_listLine.fnNext(_rclsItem);}
		void fnUserAction();
		RUList m_listLine;                                 // RuleFileParam List
		char *m_pszName;
		char *m_pszComment;
		RUArrayNums m_clsNums;
	private:
		static void fnCbkUserAction(unsigned long _ulData, void *_pvUser);
};

/************************** Rule File  Constructor ****************************************/
class RuleFile
{
	public:
		enum{E_RULE_FILE_SZ = DEF_RU_1M*5,E_RULE_FILE_SZ_LINE = DEF_RU_1K*10};
		RuleFile();
		virtual ~RuleFile();
		bool fnLoadFile(const char * _szPath);
		bool fnDefaultWriteFile(const char * _szPath, const char * _pszDefRaw);
		bool fnReLoadFile(const char * _szPath);
		bool fnLoadMem(const char * _pszMem);
		bool fnReLoadMem(const char * _pszMem);
		bool fnWriteFile(const char * _szPath);
		bool fnWriteMem(char * _pszMem, unsigned int _unSize);
		void fnDebug();
		  // ||param1||param2||....	
		bool fnAddParamVa(const char *_pszCategory, const char *_pszKey,const char * _szLine,...);
		  // ||category||key||param1||param2||....	
		bool fnAddParamLine(const char * _pszLine);
		RuleFileParam *fnFindParam(const char * _szCategory, const char * _szKey);
		RuleFileParam & fnGetVal(const char * _szCategory,const char * _szKey);
		bool fnDelCategory(const char * _szCategory);
		bool fnDelParam(const char * _szCategory, const char * _szKey);
		RuleFileCategory * fnBegin(){return (RuleFileCategory *)m_listCategory.fnBegin();}
		RuleFileCategory * fnNext(){return (RuleFileCategory *)m_listCategory.fnNext();}
		RuleFileCategory * fnBegin(RuListItem_t &_rclsItem){return (RuleFileCategory *)m_listCategory.fnBegin(_rclsItem);}
		RuleFileCategory * fnNext(RuListItem_t &_rclsItem){return (RuleFileCategory *)m_listCategory.fnNext(_rclsItem);}
		unsigned int m_unMemSize;
	protected:
		void fnInit(PfuncRFileBuilder _pfnBuilder, PfuncRFileTerminator _pfnTerminator,
																	PfuncRFileAction _pfnAction);
		RuleFileCategory * fnFindCategory(const char * _szCategory);
		void fnClear();
		bool fnBuildRaw();
		void fnUserAction();
		bool fnArrageCategory(const char * _szCatgegory, RUArrayNums * _pclsNums);   // vertical line
		bool fnArrageCategory(const char * _szCategory,unsigned int _nVarNum, ...);
		void fnSortCategory(const char * _pszCategory, PfuncSortList_t _pfnSort);   // parameter id sorting
		RUList m_listCategory;                                        // RuleFileCategory    List
		char * m_pszRawData;                                       // Raw File String
		PfuncRFileBuilder m_pfnUserCreator;
		PfuncRFileTerminator m_pfnUserTerminator;
		PfuncRFileAction m_pfnUserAction;
	private:
		void fnCreateDefDir(const char * _szPath);
		RuleFileParam m_clsNullParam;
		unsigned int m_nCommentCnt;
		bool fnAddParseParam(const char * _szLine);
		RuleFileCategory * fnAddCategory(const char * _szCategory);
		static void fnCbkUserAction(unsigned long _ulData, void * _pvUser);
};
#endif

