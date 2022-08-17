#ifndef ESIP_RULEPROC_HXX
#define ESIP_RULEPROC_HXX
#include "CRule.h"


#include "resip/stack/SipMessage.hxx"
#include "rutil/Logger.hxx"
#include "rutil/TransportType.hxx"
#include "rutil/TimeLimitFifo.hxx"

#include "eDum/eDumLibDef.hxx"
#include "eDum/eDumLibStack.hxx"
#include "eDum/eDumLibSession.hxx"

#include "eSipType.hxx"
#include "eSipCallback.hxx"
#include "eSipTu.hxx"
#include "eSipAppMessage.hxx"
#include "CMsgApi.h"

class MatchPositions : public RUObject
{
	public:
		MatchPositions(){}
		~MatchPositions(){}
		void fnAddBool(bool _bBool){m_arrBools.fnAddBool(NULL,_bBool);}
		void fnDisplay(char * _pszLog,unsigned int _unSize){m_arrBools.fnDisplay(_pszLog,_unSize);}
		RUArrayBools m_arrBools;
};
class MatchTable
{
	public:
		MatchTable(){m_pclsCurrent=NULL;m_bAndOperation=false;}
		~MatchTable(){}
		MatchPositions * fnCreate()
		{
			MatchPositions * pclsNew = new MatchPositions;
			if(!m_listParam.fnPushBack(pclsNew)) return NULL;
			m_pclsCurrent = pclsNew;
			return pclsNew;
		}
		MatchPositions * fnCopyCreate(RUArrayBools * _pclsBools)
		{
			MatchPositions * pclsNew = new MatchPositions;
			if(!m_listParam.fnPushBack(pclsNew)) return NULL;
			pclsNew->m_arrBools = *_pclsBools; 
			m_pclsCurrent = pclsNew;
			return pclsNew;
		}
		RUArrayBools * fnOR()
		{
			RuListItem_t clsIter;
			RUArrayBools * pclsTmp = fnBegin(clsIter);
			if(pclsTmp) m_arrBools = *pclsTmp;
			pclsTmp = fnNext(clsIter);
			while(pclsTmp)
			{
				(m_arrBools)|(*pclsTmp);
				pclsTmp = fnNext(clsIter);
			}
			return &m_arrBools;
		}
		RUArrayBools * fnAND()
		{
			RuListItem_t clsIter;
			RUArrayBools * pclsTmp = fnBegin(clsIter);
			if(pclsTmp) m_arrBools = *pclsTmp;
			pclsTmp = fnNext(clsIter);
			while(pclsTmp)
			{
				(m_arrBools)&(*pclsTmp);
				pclsTmp = fnNext(clsIter);
			}
			return &m_arrBools;
		}
		RUArrayBools * fnBegin()
		{
			MatchPositions * pclsTmp = (MatchPositions *)m_listParam.fnBegin();
			if(pclsTmp==NULL) return NULL;
			else return &(pclsTmp->m_arrBools);
		}
		RUArrayBools * fnNext()
		{
			MatchPositions * pclsTmp = (MatchPositions *)m_listParam.fnNext();
			if(pclsTmp==NULL) return NULL;
			else return &(pclsTmp->m_arrBools);
		}
		RUArrayBools * fnBegin(RuListItem_t & _rclsItem)
		{
			MatchPositions * pclsTmp = (MatchPositions *)m_listParam.fnBegin(_rclsItem);
			if(pclsTmp==NULL) return NULL;
			else return &(pclsTmp->m_arrBools);
		}
		RUArrayBools * fnNext(RuListItem_t & _rclsItem)
		{
			MatchPositions * pclsTmp = (MatchPositions *)m_listParam.fnNext(_rclsItem);
			if(pclsTmp==NULL) return NULL;
			else return &(pclsTmp->m_arrBools);
		}
		void fnDisplay(char * _pszLog, unsigned int _unSize)
		{
			fnRUStrnCat(_pszLog,_unSize,"<<<Match Position Tables>>>\r\n");
			RuListItem_t clsIter;unsigned int unCnt = 0;
			RUArrayBools * pclsTmp = fnBegin(clsIter);
			while(pclsTmp)
			{
				fnRUStrnCat(_pszLog,_unSize,"Depend Rule_%u=> ",unCnt);
				pclsTmp->fnDisplay(_pszLog,_unSize);
				pclsTmp = fnNext(clsIter);
				unCnt++;
			}
			fnRUStrnCat(_pszLog,_unSize,"<<<Match Operation Merge>>> Operation %s\r\n",
													TRUE_SELECT(m_bAndOperation==true,"OR","AND"));
			m_arrBools.fnDisplay(_pszLog,_unSize);
		}
		void fnClear(){m_listParam.fnClear();m_pclsCurrent = NULL;m_bAndOperation=false;}
		MatchPositions * m_pclsCurrent;
		RUArrayBools m_arrBools;
		RUList m_listParam;
		bool m_bAndOperation;
};

class RuleEvent;
class RuleMatch
{
	public:
		RuleMatch();
		~RuleMatch();
		static char m_szTrue[3];
		static char m_szFalse[3];
		RUArrayBools m_arrBools;
		eDum::ESipState_t m_eSipState;
		eDum::Dialog  * m_pclsDialog;
		eSIP::ESipEventType_t m_eSipEventType;
		resip::SipMessage *m_pclsRcvSipMsg;
		eSIP::Net5Tuple_t *m_pst5Tuple;
		Rule * m_pclsRule;
		RuleEvent * m_pclsEv;
		bool fnMatch(eDum::ESipState_t _eSipState,eDum::Dialog *_pclsDialog,
												eSIP::ESipEventType_t _eSipEventType,
												resip::SipMessage *_pclsRcvSipMsg,
												eSIP::Net5Tuple_t *_pst5Tuple,
												Rule * _pclsRule, RuleEvent * _pclsEv); 
		bool fnMatchAttribute();
		bool fnMatchCallringDomain();
		bool fnMatchCallerDomain();
		bool fnMatchCalleeDomain();
		bool fnMatchCallerPrefix();
		bool fnMatchCalleePrefix();
		bool fnMatchCallerRealm();
		bool fnMatchCalleeRealm();
		bool fnMatchHeaderName();
		bool fnMatchHeaderValue();
		bool fnMatchRecvLOC();
		bool fnMatchRequest();
		bool fnMatchResponseMC();
		bool fnMatchHeaderGenValue();
		bool fnMatchDisplayName();
		bool fnMatchURIGenValue();
		bool fnMatchURIGenHeader();
		bool fnMatchURIUserName();
		bool fnMatchHost();
		bool fnMatchURIRealm();
		bool fnMatchPort();
		bool fnMatchURIScheme();
		bool fnMatchURIString();
		bool fnMatchExtHdrGen();
		bool fnMatchURISubValue();
		bool fnMatchMatchLocID();
		bool fnMatchMatchRmtID();
		bool fnMatchMatchRteID();
		bool fnMatchDirection();
		#ifdef DEF_RULE_R131
		bool fnMatchNpdbResult();
		bool fnMatchHlrResult();
		bool fnMatchPointCode();
		#endif
		bool fnFullProc(ERuleMatch_t _eMatch, const char *_szPkMsg, const char * _szRule,
																			unsigned int _unPos = 0);
		bool fnPosProc(EMsgPos_t _ePos, ERuleMatch_t _eMatch, RUArrayString *_pclsString, 
																			const char * _pszVal);
		bool fnEmptyCheck(unsigned int _unValueNums);
		
};

class RuleHistoryInfo : public RUObject
{
	public:
		RuleHistoryInfo();
		~RuleHistoryInfo();
		void fnDisplay(char * _pszLog,unsigned int _unSize);
		unsigned int m_uiID;
		bool m_bMatch;
		RUArrayBools m_clsMatchPos;
};

class RuleEvent
{
	public:
		RuleEvent();
		~RuleEvent();
		bool fnRuleMatch(eDum::ESipState_t _eSipState,eDum::Dialog *_pclsDialog,
												eSIP::ESipEventType_t _eSipEventType,
												resip::SipMessage * _pclsRcvSipMsg,
												eSIP::Net5Tuple_t  *_pst5Tuple,
												Rule * _pclsRule);
		bool fnRuleSetMatch(eDum::ESipState_t _eSipState,eDum::Dialog * _pclsDialog,
												eSIP::ESipEventType_t _eSipEventType,
												resip::SipMessage *_pclsRcvSipMsg,
												eSIP::Net5Tuple_t  *_pst5Tuple,
												RuleSet * _pclsRuleSet);
		void fnClearHistory(){m_mapRuleHistory.fnClear();}
		RuleHistoryInfo *fnBegin(){return (RuleHistoryInfo*)m_mapRuleHistory.fnBeginI();}
		RuleHistoryInfo *fnNext(){return (RuleHistoryInfo*)m_mapRuleHistory.fnNextI();}
		RuleHistoryInfo *fnBegin(RuMapIntItem_t &_rclsItem){return (RuleHistoryInfo*)m_mapRuleHistory.fnBeginI(_rclsItem);}
		RuleHistoryInfo *fnNext(RuMapIntItem_t &_rclsItem){return (RuleHistoryInfo*)m_mapRuleHistory.fnNextI(_rclsItem);}
		void fnDisplayHistory();
		RUMap m_mapRuleHistory;
		RuleMatch m_clsMatch;
		RuleSet *m_pclsRuleSet;
		bool m_bCurrentMatch;
		unsigned int m_unMine;

		eDum::ESipState_t m_eSipState;
		eDum::Dialog * m_pclsDialog;
		eSIP::ESipEventType_t m_eSipEventType;
		resip::SipMessage *m_pclsRcvSipMsg;
		eSIP::Net5Tuple_t *m_pst5Tuple;
		RUArrayBools * m_pclsCurrentDepend;
		RUArrayBools m_clsRSetHistory;
		MatchTable m_tblDependMatch;

		unsigned int m_unMatchLocID;
		unsigned int m_unMatchRmtID;
		unsigned int m_unMatchRteID;
		bool m_bInitRequest;
		bool m_bSnd;
		#ifdef DEF_RULE_R131
		RUString m_clsNpdbResult;
		RUString m_clsHlrResult;
		RUString m_clsPointCode;
		#endif
	private:
		RuleHistoryInfo *fnFindHistory(Rule * _pclsRule);
		bool fnAddHistory(bool _bMatch, Rule *_pclsRule);
		static void * fnCbkCreator();
		static void fnCbkTermHistory(unsigned long _ulData);
		static void fnCbkProcNextRule(unsigned long _ulData, void * _pvUser);
		
};

void fnRuleProcSample();
#endif

