
#ifndef ESIPUTIL_RULE_MGR_H
#define ESIPUTIL_RULE_MGR_H

#include <memory>
#include <map>
#include <list>
#include <string>

#include "lock.hxx"
#include "dataFile.hxx"

namespace eSipUtil
{

class RuleSet;
class RuleOpSet;

typedef enum
{
	E_RULE_RESULT_SUCC =0,
	E_RULE_RESULT_FAIL_invalidArgument,
	E_RULE_RESULT_FAIL_allocMemory,
	E_RULE_RESULT_FAIL_internalInvalidValue,
	E_RULE_RESULT_FAIL_existRuleId,
	E_RULE_RESULT_FAIL_notExistRuleId,
	E_RULE_RESULT_FAIL_existRuleOpId,
	E_RULE_RESULT_FAIL_notExistRuleOpId,
	E_RULE_RESULT_FAIL_overMaxColumnNum,
	E_RULE_RESULT_FAIL_referenceRule,
	E_RULE_RESULT_FAIL_notFindMatchRule,
	E_RULE_RESULT_FAIL_internalErr,

	E_RULE_RESULT_NUM,
}ERuleResult_t;

const char * g_fnGetRuleResultStr(ERuleResult_t eRuleResult);

typedef list_LineData list_RuleData;		//first(key), second(value)
typedef list_LineDataAll list_RuleDataAll;

class RuleUserData
{
	public :
		enum
		{
			E_CONST_MAX_STR_LEN = 1024,
		};

		RuleUserData();
		virtual ~RuleUserData();

		/*==== user must implement function ====*/
		virtual bool m_fnIsValid() const = 0;	//must not use function of RuleMgr internally. Lock is duplicated (can use function including argument of using lock)
		virtual const char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen) const = 0;
		virtual bool m_fnChange(const RuleUserData & _rclsSrc) = 0;
		virtual bool m_fnChange(const list_RuleData & _rlistRuleData) = 0;
		virtual void m_fnGetRuleData(list_RuleData & _rlistRuleData) const = 0;
		virtual RuleUserData & operator=(const RuleUserData & _rclsSrc) = 0;
		virtual RuleUserData * m_fnClone() const = 0;
};
typedef RuleUserData RuleOpUserData;

/*==== define callback ====*/
typedef RuleUserData * (* PfuncCreateUserData)(const list_RuleData & _rlistRuleData);  //RuleUserData must be created by new()
typedef bool (* PfuncIsMatchRule)(const RuleUserData * _pclsRuleUserData, const void * _pEvent);	//It is used only for Rule (not ruleOp)

struct RuleConfig
{
	DataFileConfig m_stDataFileCfg;	
	PfuncCreateUserData m_pfnCbCreateUserData;

	RuleConfig()
	{
		m_pfnCbCreateUserData = NULL;
	}
};
typedef RuleConfig RuleOpConfig;
typedef std::list< RuleOpConfig > list_RuleOpConfig;

struct RuleMgrConfig
{
	//rule
	RuleConfig m_stRuleConfig;
	PfuncIsMatchRule m_pfnCbIsMatchRule;

	//ruleOp
	list_RuleOpConfig m_listRuleOpConfig;

	RuleMgrConfig()
	{
		m_pfnCbIsMatchRule = NULL;
	}
};

typedef std::list< std::pair<bool, int> > list_RuleMix;		//first(true(and), false(or)), second(RuleId)

struct RuleMix
{
	list_RuleMix m_listRuleMix;

	inline unsigned int m_fnGetSize() const{ return m_listRuleMix.size(); }
	inline void m_fnReset(){ m_listRuleMix.clear(); }
	const char * m_fnGetStr(std::string & _rclsStr) const;
	void m_fnAddRule(bool _bAnd, int _nRuleId);
	bool m_fnSetRule(const char * _pszRuleMix);
	
	RuleMix()
	{
		m_fnReset();
	}
};

struct RuleOpUsrData4List
{
	int m_nRuleOpId;
	RuleOpUserData * m_pclsRuleOpUserData;

	RuleOpUsrData4List()
		: m_nRuleOpId(-1), m_pclsRuleOpUserData(NULL)
	{
	}

	~RuleOpUsrData4List()
	{
		delete m_pclsRuleOpUserData;
	}
};

typedef std::list< RuleOpUsrData4List > list_ruleOpUsrData;

struct RuleAllInfo
{
	int m_nId;
	std::string m_clsName;
	RuleUserData * m_pclsUserData;

	RuleAllInfo()
	{
		m_nId = -1;
		m_pclsUserData = NULL;
	}

	~RuleAllInfo()
	{
		delete m_pclsUserData;
	}
};
typedef std::list< RuleAllInfo > list_RuleAllInfo;

struct RuleOpAllInfo
{
	int m_nId;
	std::string m_clsName;
	unsigned int m_unPriority;
	std::string m_clsRuleMix;
	RuleUserData * m_pclsUserData;

	RuleOpAllInfo()
	{
		m_nId = -1;
		m_unPriority = 0;
		m_pclsUserData= NULL;
	}

	~RuleOpAllInfo()
	{
		delete m_pclsUserData;
	}
};
typedef std::list< RuleOpAllInfo > list_RuleOpAllInfo;

class RuleMgr
{
	public :		
		RuleMgr(const char * _pszRuleMgrName);
		~RuleMgr();

		static void m_fnSetRuleMgrLogInstance(int _nLogInstance){ m_nLogInstance = _nLogInstance; }
		static void m_fnSetRuleMgrLogCategory(int _nLogCategory){ m_nLogCategory = _nLogCategory; }

		ERuleResult_t m_fnInit(RuleMgrConfig & _rstRuleMgrConfig);

		/*==== api of rule ====*/
		ERuleResult_t m_fnAddRule(const list_RuleData & _rlistRuleData, bool _bUseLock=true, bool _bFileWrite=true);
		ERuleResult_t m_fnAddRule(int _nRuleId, const char * _pszRuleName, const RuleUserData & _rclsUserData);
		ERuleResult_t m_fnDelRule(const list_RuleData & _rlistRuleData);
		ERuleResult_t m_fnDelRule(int _nRuleId);
		ERuleResult_t m_fnChangeRule(const list_RuleData & _rlistRuleData);
		ERuleResult_t m_fnChangeRule(int _nRuleId, const char * _pszRuleName, const RuleUserData * _pclsUserData);
		ERuleResult_t m_fnFindRule(list_RuleData & _rlistRuleData) const;
		ERuleResult_t m_fnFindRule(int _nRuleId, std::string & _rclsName, RuleUserData & _rclsUserData) const; //in(_nRuleId), out(_rclsName, _rclsUserData)
		ERuleResult_t m_fnFindRuleAll(list_RuleAllInfo & _rlistRuleAllInfo) const;
		bool m_fnIsRule(int _nRuleId, bool _bUseLock) const;
		void m_fnClearAllRule();

		/*==== api of ruleOp ====*/
		/*==== _nRuleOpSetId must be between 0 and size of RuleOp -1 ====*/
		ERuleResult_t m_fnAddRuleOp(int _nRuleOpSetId, const list_RuleData & _rlistRuleData, bool _bUseLock=true, bool _bFileWrite=true);
		ERuleResult_t m_fnAddRuleOp(int _nRuleOpSetId, int _nRuleOpId, const char * _pszRuleOpName, unsigned int _unPriority, const char * _pszRuleMix, const RuleOpUserData & _rclsUserData);
		ERuleResult_t m_fnDelRuleOp(int _nRuleOpSetId, const list_RuleData & _rlistRuleData);
		ERuleResult_t m_fnDelRuleOp(int _nRuleOpSetId, int _nRuleOpId);
		ERuleResult_t m_fnChangeRuleOp(int _nRuleOpSetId, const list_RuleData & _rlistRuleData);
		ERuleResult_t m_fnChangeRuleOp(int _nRuleOpSetId, int _nRuleOpId, const char * _pszRuleOpName, unsigned int _unPriority, const char * _pszRuleMix, const RuleOpUserData * _pclsUserData);
		ERuleResult_t m_fnFindRuleOp(int _nRuleOpSetId, list_RuleData & _rlistRuleData) const;
		ERuleResult_t m_fnFindRuleOp(int _nRuleOpSetId, int _nRuleOpId, std::string & _rclsName, unsigned int & _runPriority, std::string & _rclsRuleMix, RuleOpUserData & _rclsUserData) const; //in(_nRuleOpSetId, _nRuleOpId), out(_rclsName, _unPriority, _rclsRuleMix, _rclsUserData)		
		ERuleResult_t m_fnFindRuleOpAll(int _nRuleOpSetId, list_RuleOpAllInfo & _rlistRuleOpAllInfo) const;
		bool m_fnIsRuleOp(int _nRuleOpSetId, int _nRuleOpId, bool _bUseLock) const;
		void m_fnClearRuleOp(int _nRuleOpSetId);
		void m_fnClearAllRuleOp();
		ERuleResult_t m_fnDecideRuleOpByPriority(int _nRuleOpSetId, const void * _pEvent, int & _nRuleOpId, RuleOpUserData & _rstDecideUserData) const;
		ERuleResult_t m_fnDecideRuleOpById(int _nRuleOpSetId, const void * _pEvent, int _nRuleOpId, RuleOpUserData & _rstDecideUserData) const;
		ERuleResult_t m_fnDecideRuleOpByMultipleId(int _nRuleOpSetId, const void * _pEvent, const std::list< int > & _rlistRuleOpId, list_ruleOpUsrData & _rlistRuleOpUsrData)  const;

		static int m_nLogInstance;
		static int m_nLogCategory;

	private :
		static void m_fnCbApplyRuleSetData(const list_RuleDataAll & _rlistRuleDataAll, void * _pObject1, void * _pObject2);
		static void m_fnCbApplyRuleOpSetData(const list_RuleDataAll & _rlistRuleDataAll, void * _pObject1, void * _pObject2);
		
		const RuleOpConfig * m_fnGetRuleOpSetConfig(int _nRuleOpSetId) const;

		void m_fnWriteFileRuleSetFromMem();
		void m_fnWriteFileRuleOpSetFromMem(int _nRuleOpSetId);		
		
		mutable RwMutex m_clsRuleMgrRwMutex;
		RuleMgrConfig m_stRuleMgrConfig;
		bool m_bInit;
		std::string m_clsRuleMgrName;
		RuleSet * m_pclsRuleSet;
		unsigned int m_unRuleOpSetNum;
		RuleOpSet * m_arrRuleOpSet;

		/*==== for File I/O ====*/
		DataFileMgr m_stRuleSetDataFile;
		DataFileMgr * m_arrRuleOpSetDataFile;
		
};

}

#endif

