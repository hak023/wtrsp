
#ifndef COM_AS_RULE_HXX
#define COM_AS_RULE_HXX

#include <string>
#include <list>

#include "resip/stack/SipMessage.hxx"
#include "eSipUtil/ruleMgr.hxx"

#include "eDum/eDumLibSession.hxx"

#include "ComAsEvent.hxx"

namespace ComAs
{

typedef enum
{
	E_SNODE_TYPE_mss = 0,
	E_SNODE_TYPE_client,

	E_SNODE_TYPE_NUM,
}ESNodeType;

const char * g_fnGetSNodeTypeStr(int _n);

struct ComAsRuleInfo
{
	enum
	{
		E_CONST_DATA_NUM = 8,
	};

	typedef enum
	{
		E_MAIN_sip_method = 0,
		E_MAIN_sip_reqUri,
		E_MAIN_sip_header,
		E_MAIN_sip_headerExist,
		E_MAIN_sip_initInv,
		E_MAIN_sip_rspCode,
		E_MAIN_rcv_sip,
		E_MAIN_rcv_extAs,
		E_MAIN_localId,

		E_MAIN_NUM,
	}EMain;

	typedef enum
	{
		E_EXIST_true = 0,
		E_EXIST_false,

		E_EXIST_NUM,
	}EExist;

	typedef enum
	{
		E_SUB_RANGE_all = 0,
		E_SUB_RANGE_top,
		E_SUB_RANGE_bottom,

		E_SUB_RANGE_NUM,
	}ESubRange;

	typedef enum
	{
		E_SUB_PART_full = 0,
		E_SUB_PART_part,
		E_SUB_PART_username,
		E_SUB_PART_username_part,
		E_SUB_PART_paramV,
		E_SUB_PART_paramExist,

		E_SUB_PART_NUM,
	}ESubPart;
	
	int m_nMain;								//EMain
	std::string m_clsMainValue;
	int m_nMainExist;						//EExist
	
	int m_nSubPart;						//ESubPart
	int m_nSubRange;					//ESubRange
	std::string m_clsSubValue1;
	int m_nSubExist;						//EExist
	std::string m_clsSubValue2;
	
	void m_fnReset()
	{
		m_nMain = -1;
		m_clsMainValue = "";
		m_nMainExist = -1;
				
		m_nSubPart = -1;
		m_nSubRange = -1;
		m_clsSubValue1 = "";
		m_nSubExist = -1;
		m_clsSubValue2 = "";
	}

	ComAsRuleInfo(){ m_fnReset(); }
	bool m_fnValid();

	static const char * m_fnGetMainStr(int _n);
	static const char * m_fnGetExistStr(int _n);
	static const char * m_fnGetSubRangeStr(int _n);
	static const char * m_fnGetSubPartStr(int _n);
	
	static int m_fnGetMainEnum(const char * _pszStr);
	static int m_fnGetExistEnum(const char * _pszStr);
	static int m_fnGetSubRangeEnum(const char * _pszStr);
	static int m_fnGetSubPartEnum(const char * _pszStr);

	static void m_fnSetMain(ComAsRuleInfo & _rstComAsRuleInfo, const char * _pszStr);
	static void m_fnSetMainValue(ComAsRuleInfo & _rstComAsRuleInfo, const char * _pszStr);
	static void m_fnSetMainExist(ComAsRuleInfo & _rstComAsRuleInfo, const char * _pszStr);	
	static void m_fnSetSubPart(ComAsRuleInfo & _rstComAsRuleInfo, const char * _pszStr);
	static void m_fnSetSubRange(ComAsRuleInfo & _rstComAsRuleInfo, const char * _pszStr);
	static void m_fnSetSubValue1(ComAsRuleInfo & _rstComAsRuleInfo, const char * _pszStr);
	static void m_fnSetSubExist(ComAsRuleInfo & _rstComAsRuleInfo, const char * _pszStr);
	static void m_fnSetSubValue2(ComAsRuleInfo & _rstComAsRuleInfo, const char * _pszStr);

	static std::string m_fnGetMain(const ComAsRuleInfo & _rstComAsRuleInfo);
	static std::string m_fnGetMainValue(const ComAsRuleInfo & _rstComAsRuleInfo);
	static std::string m_fnGetMainExist(const ComAsRuleInfo & _rstComAsRuleInfo);	
	static std::string m_fnGetSubPart(const ComAsRuleInfo & _rstComAsRuleInfo);
	static std::string m_fnGetSubRange(const ComAsRuleInfo & _rstComAsRuleInfo);
	static std::string m_fnGetSubValue1(const ComAsRuleInfo & _rstComAsRuleInfo);
	static std::string m_fnGetSubExist(const ComAsRuleInfo & _rstComAsRuleInfo);
	static std::string m_fnGetSubValue2(const ComAsRuleInfo & _rstComAsRuleInfo);
};

struct ComAsRouteOpInfo
{
	enum
	{
		E_CONST_DATA_NUM = 3,
		E_CONST_MAX_SCROP_NUMS = 32,
	};

	typedef enum
	{
		E_ROUTE_NODE_TYPE_extAs = 0,
		E_ROUTE_NODE_TYPE_sipNode,

		E_ROUTE_NODE_TYPE_NUM,
	}ERouteNodeType;

	int m_nRouteNodeType;		//ERouteNodeType
	unsigned int m_unRouteSeqId;
	int m_arrScrOp[E_CONST_MAX_SCROP_NUMS];
	unsigned int m_unScrOpNum;

	bool m_bUseScrOp;		//only used for checking changing

	void m_fnReset()
	{
		m_nRouteNodeType = -1;
		m_unRouteSeqId = 0;
		for( int i = 0; i < E_CONST_MAX_SCROP_NUMS; ++i )
		{
			m_arrScrOp[i] = -1;
		}
		m_unScrOpNum = 0;

		m_bUseScrOp = false;
	}

	ComAsRouteOpInfo(){ m_fnReset(); }
	bool m_fnValid() const;

	static const char * m_fnGetRouteNodeTypeStr(int _n);	
	static int m_fnGetRouteNodeTypeEnum(const char * _pszStr);

	static void m_fnSetRouteNodeType(void * _pComAsRouteOpInfo, const char * _pszStr);
	static void m_fnSetRouteSeqId(void * _pComAsRouteOpInfo, const char * _pszStr);
	static void m_fnSetScrOpArr(void * _pComAsRouteOpInfo, const char * _pszStr);
	
	static std::string m_fnGetRouteNodeType(const void * _pComAsRouteOpInfo);	
	static std::string m_fnGetRouteSeqId(const void * _pComAsRouteOpInfo);
	static std::string m_fnGetScrOpArr(const void * _pComAsRouteOpInfo);
};

struct ComAsScrOpInfo
{
	enum
	{
		E_CONST_DATA_NUM = 1,
		E_CONST_MAX_SCR_NUMS = 128,
	};

	int m_arrScr[E_CONST_MAX_SCR_NUMS];
	unsigned int m_unScrNum;

	void m_fnReset()
	{
		for( int i = 0; i < E_CONST_MAX_SCR_NUMS; ++i )
		{
			m_arrScr[i] = -1;
		}
		m_unScrNum = 0;
	}

	ComAsScrOpInfo(){ m_fnReset(); }
	bool m_fnValid() const;

	static void m_fnSetScrArr(void * _pComAsScrOpInfo, const char * _pszStr);
	static std::string m_fnGetScrArr(const void * _pComAsScrOpInfo);
};

class ComAsRuleUserData : public eSipUtil::RuleUserData
{
	public :
		enum
		{
			E_CONST_MAX_STR_LEN = 1024,
		};
		
		ComAsRuleUserData();
		virtual ~ComAsRuleUserData();

		static bool m_fnIsMatchRule(const eSipUtil::RuleUserData * _pclsRuleUserData, const void * _pEvent);
		static eSipUtil::RuleUserData * m_fnCreateUserData(const eSipUtil::list_RuleData & _rlistRuleData);

		virtual bool m_fnIsValid() const;
		virtual const char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen) const;
		virtual bool m_fnChange(const eSipUtil::RuleUserData & _rclsSrc);
		virtual bool m_fnChange(const eSipUtil::list_RuleData & _rlistRuleData);
		virtual void m_fnGetRuleData(eSipUtil::list_RuleData & _rlistRuleData) const;
		ComAsRuleUserData & operator=(const ComAsRuleUserData & _rclsSrc);
		virtual eSipUtil::RuleUserData & operator=(const RuleUserData & _rclsSrc);		
		virtual eSipUtil::RuleUserData * m_fnClone() const;

		const ComAsRuleInfo & m_fnGetInfo() const{ return m_stComAsRuleInfo; }
		ComAsRuleInfo & m_fnGetInfo(){ return m_stComAsRuleInfo; }

	private :
		bool m_fnIsMatch4SipMethod(const resip::SipMessage & _rclsSipMsg) const;
		bool m_fnIsMatch4SipReqUri(const resip::SipMessage & _rclsSipMsg) const;
		bool m_fnIsMatch4SipHdr(const resip::SipMessage & _rclsSipMsg) const;
		bool m_fnIsMatch4SipHdrExist(const resip::SipMessage & _rclsSipMsg) const;
		bool m_fnIsMatch4SipInitInv(const ComAsSipEvent & _rstComAsSipEvent) const;
		bool m_fnIsMatch4SipRspCode(const ComAsSipEvent & _rstComAsSipEvent) const;
		bool m_fnIsMatch4RcvSIP(const ComAsSipEvent & _rstComAsSipEvent) const;
		bool m_fnIsMatch4RcvExtAs(const ComAsSipEvent & _rstComAsSipEvent) const;
		bool m_fnIsMatch4LocalId(const ComAsSipEvent & _rstComAsSipEvent) const;		
		
		bool m_fnIsMatch4SipHdrValue(const resip::SipMessage & _rclsSipMsg) const;		
		bool m_fnIsMatch4SipHdrUserName(const resip::SipMessage & _rclsSipMsg) const;
		bool m_fnIsMatch4SipHdrUserNamePart(const resip::SipMessage & _rclsSipMsg) const;
		bool m_fnIsMatch4SipHdrParamV(const resip::SipMessage & _rclsSipMsg) const;
		bool m_fnIsMatch4SipHdrParamExist(const resip::SipMessage & _rclsSipMsg) const;
		
		ComAsRuleInfo m_stComAsRuleInfo;
		
	friend class ComAsRuleMgr;
};

class ComAsRouteOpUserData : public eSipUtil::RuleUserData
{
	public :
		enum
		{
			E_CONST_MAX_STR_LEN = 1024,
		};
		
		ComAsRouteOpUserData();
		virtual ~ComAsRouteOpUserData();

		static eSipUtil::RuleUserData * m_fnCreateUserData(const eSipUtil::list_RuleData & _rlistRuleData);

		virtual bool m_fnIsValid() const;
		virtual const char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen) const;
		virtual bool m_fnChange(const eSipUtil::RuleUserData & _rclsSrc);
		virtual bool m_fnChange(const eSipUtil::list_RuleData & _rlistRuleData);
		virtual void m_fnGetRuleData(eSipUtil::list_RuleData & _rlistRuleData) const;
		ComAsRouteOpUserData & operator=(const ComAsRouteOpUserData & _rclsSrc);
		virtual eSipUtil::RuleUserData & operator=(const RuleUserData & _rclsSrc);		
		virtual eSipUtil::RuleUserData * m_fnClone() const;

		const ComAsRouteOpInfo & m_fnGetInfo() const{ return m_stComAsRouteOpInfo; }
		ComAsRouteOpInfo & m_fnGetInfo(){ return m_stComAsRouteOpInfo; }

	private :		
		ComAsRouteOpInfo m_stComAsRouteOpInfo;
		
	friend class ComAsRuleMgr;
};

class ComAsScrOpUserData : public eSipUtil::RuleUserData
{
	public :
		enum
		{
			E_CONST_MAX_STR_LEN = 1024,
		};
		
		ComAsScrOpUserData();
		virtual ~ComAsScrOpUserData();

		static eSipUtil::RuleUserData * m_fnCreateUserData(const eSipUtil::list_RuleData & _rlistRuleData);

		virtual bool m_fnIsValid() const;
		virtual const char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen) const;
		virtual bool m_fnChange(const eSipUtil::RuleUserData & _rclsSrc);
		virtual bool m_fnChange(const eSipUtil::list_RuleData & _rlistRuleData);
		virtual void m_fnGetRuleData(eSipUtil::list_RuleData & _rlistRuleData) const;
		ComAsScrOpUserData & operator=(const ComAsScrOpUserData & _rclsSrc);
		virtual eSipUtil::RuleUserData & operator=(const RuleUserData & _rclsSrc);		
		virtual eSipUtil::RuleUserData * m_fnClone() const;

		const ComAsScrOpInfo & m_fnGetInfo() const{ return m_stComAsScrOpInfo; }
		ComAsScrOpInfo & m_fnGetInfo(){ return m_stComAsScrOpInfo; }

	private :		
		ComAsScrOpInfo m_stComAsScrOpInfo;
		
	friend class ComAsRuleMgr;
};

struct ComAsRouteOpData4Decide
{
	int m_nRouteOpId;
	ESNodeType m_eSNodeType;
	ComAsRouteOpInfo m_stRouteOpInfo;

	enum
	{
		E_CONST_MAX_PRINT_LEN = 512,
	};

	void m_fnReset()
	{
		m_nRouteOpId = -1;
		m_eSNodeType = E_SNODE_TYPE_NUM;
		m_stRouteOpInfo.m_fnReset();
	}

	ComAsRouteOpData4Decide(){ m_fnReset(); }

	const char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

		_pszPrint[0] = 0x00;
		snprintf(_pszPrint, _unMaxLen-1, "ComAsRouteOpData4Decide : RouteOpId[%d], SNodeType[%s], RouteNodeType[%s], RouteSeqId[%u], Scr[%s]", 
																	m_nRouteOpId,
																	g_fnGetSNodeTypeStr(m_eSNodeType), 
																	ComAsRouteOpInfo::m_fnGetRouteNodeTypeStr(m_stRouteOpInfo.m_nRouteNodeType), 
																	m_stRouteOpInfo.m_unRouteSeqId,
																	ComAsRouteOpInfo::m_fnGetScrOpArr(&m_stRouteOpInfo).c_str());

		return _pszPrint;
	}
};

class ComAsRuleMgr
{
	public :
		typedef enum
		{
			E_RULE_OP_scrOp = 0,
			E_RULE_OP_routeOp,			

			E_RULE_OP_NUM,
		}ERuleOp;

		static ComAsRuleMgr & getInstance();
		~ComAsRuleMgr();

		bool m_fnInit();		
		bool m_fnDecideRouteOp(const ComAsSipEvent & _rstComAsSipEvent, ComAsRouteOpData4Decide & _rstRouteOpData4Decide);
		bool m_fnExecuteScrOp(ComAsSipEvent & _rstComAsSipEvent);
		bool m_fnIsScrOp(int _nScrOpId, bool _bUseRuleMgrLock) const;

		eSipUtil::ERuleResult_t m_fnAddRule(int _nRuleId, const char * _pszRuleName, const ComAsRuleUserData & _rclsUserData);
		eSipUtil::ERuleResult_t m_fnDelRule(int _nRuleId);
		eSipUtil::ERuleResult_t m_fnChangeRule(int _nRuleId, const char * _pszRuleName, const ComAsRuleUserData & _rclsUserData);
		eSipUtil::ERuleResult_t m_fnFindRule(int _nRuleId, std::string & _rclsRuleName, ComAsRuleUserData & _rclsUserData) const;
		eSipUtil::ERuleResult_t m_fnFindRuleAll(eSipUtil::list_RuleAllInfo & _rlistRuleAllInfo);

		eSipUtil::ERuleResult_t m_fnAddRouteOp(int _nRouteOpId, const char * _pszRouteOpName, unsigned int _unPriority, const char * _pszRuleMix, const ComAsRouteOpUserData & _rclsUserData);
		eSipUtil::ERuleResult_t m_fnDelRouteOp(int _nRouteOpId);
		eSipUtil::ERuleResult_t m_fnChangeRouteOp(int _nRouteOpId, const char * _pszRouteOpName, unsigned int _unPriority, const char * _pszRuleMix, const ComAsRouteOpUserData & _rclsUserData);
		eSipUtil::ERuleResult_t m_fnFindRouteOp(int _nRouteOpId, std::string & _rclsRouteOpName, unsigned int & _runPriority, std::string & _rclsRuleMix, ComAsRouteOpUserData & _rclsUserData) const;
		eSipUtil::ERuleResult_t m_fnFindRouteOpAll(eSipUtil::list_RuleOpAllInfo & _rlistRouteOpAllInfo) const;

		eSipUtil::ERuleResult_t m_fnAddScrOp(int _nScrOpId, const char * _pszScrOpName, const char * _pszRuleMix, const ComAsScrOpUserData & _rclsUserData);
		eSipUtil::ERuleResult_t m_fnDelScrOp(int _nScrOpId);
		eSipUtil::ERuleResult_t m_fnChangeScrOp(int _nScrOpId, const char * _pszScrOpName, const char * _pszRuleMix, const ComAsScrOpUserData & _rclsUserData);
		eSipUtil::ERuleResult_t m_fnFindScrOp(int _nScrOpId, std::string & _rclsScrOpName, std::string & _rclsRuleMix, ComAsScrOpUserData & _rclsUserData) const;
		eSipUtil::ERuleResult_t m_fnFindScrOpAll(eSipUtil::list_RuleOpAllInfo & _rlistScrOpAllInfo) const;
		
		void m_fnTestInputRule();

	private :
		ComAsRuleMgr();
		
		eSipUtil::RuleMgr m_clsRuleMgr;

};

}

#endif
