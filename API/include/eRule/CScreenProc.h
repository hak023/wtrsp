#ifndef ESIP_SCREEN_HXX
#define ESIP_SCREEN_HXX
#include "CScreen.h"
#include "CRuleProc.h"
#include "CMsgApi.h"
#include "RuleLibCom.h"

typedef struct
{
	unsigned int unNumber;
	const char *pszDescrition;
}ScreenRspTable;
void fnScrProcSample();
class ScreenActProc
{
	public:
		ScreenActProc();
		~ScreenActProc();
		bool fnProc(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
												resip::SipMessage * _pclsSndSipMsg,
												unsigned int _uiActID,
												AppLines *_pclsAddLines,
												AppLines *_pclsRemoveLines);
		void fnDisplay(char * _pszLog,unsigned int _unSize)
		{
			if(m_pclsAct == NULL) return;
			m_pclsAct->fnDisplay(_pszLog,_unSize);
		}
		bool fnProcAttri();
		bool fnprocAttriAddHdrValue();
		bool fnprocAttriAddHdrParam();
		bool fnprocAttriDelHdrName();
		bool fnprocAttriDelHdrParam();
		bool fnprocAttriCvtReqUri();
		bool fnprocAttriAddUriGenParam();
		bool fnprocAttriAddUriHdrParam();
		bool fnprocAttriAddUriScheme();
		bool fnprocAttriAddUriUsername();
		bool fnprocAttriAddUriHost();
		bool fnprocAttriAddUriPort();
		bool fnprocAttriAddUriDisplayname();
		bool fnprocAttriAddUriString();
		bool fnprocAttriDelUriDisplayName();
		bool fnprocAttriDelUriGenParam();
		bool fnprocAttriDelUriHdrParam();
		bool fnprocAttriDelUriScheme();
		bool fnprocAttriDelUriUsername();
		bool fnprocAttriDelUriHost();
		bool fnprocAttriDelUriPort();
		bool fnprocAttriDelUriString();
		bool fnprocAttriMovParam();
		bool fnprocAttriMovUriParam();
		bool fnprocAttriMovUriHdr();
		bool fnprocAttriCvtRsp();
		bool fnprocAttriCvtUriScheme();
	private:
		bool fnprocEmptyCheck(unsigned int _unValueNums);
		ScreenAct * m_pclsAct;
		RuleEvent * m_pclsEvent;
		AppLines * m_pclsAddLines;
		AppLines * m_pclsRemoveLines;
		resip::SipMessage * m_pclsRcvMsg;
		resip::SipMessage * m_pclsSndMsg;
};

class ScreenOPProc
{
	public:
		ScreenOPProc();
		~ScreenOPProc();
		bool fnProc(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
												resip::SipMessage * _pclsOrigMsg,
												ScreenOP *_pclsSOP,
												AppLines * _pclsAddLines,
												AppLines * _pclsRemoveLines);
		void fnDisplay(char * _pszLog,unsigned int _unSize)
		{
			if(m_bMatch == false || m_pclsSOP == NULL) return;
			m_pclsSOP->fnDisplay(_pszLog,_unSize);
		}
		bool m_bMatch;
	private:
		ScreenActProc m_clsAction;
		ScreenOP * m_pclsSOP;
};		
class ScreenResult : public RUObject
{
	public:
		ScreenResult(){m_uiScreenID = 0;m_bMatch = false;m_bScreen=false;}
		~ScreenResult(){}
		void fnDisplay(char * _pszLog, unsigned int _unSize)
		{
			//ScreenOP * pclsFind = ScreenMgr::fnGetInstance()->fnFindScreenOP(m_uiScreenID);
			fnRUStrnCat(_pszLog,_unSize,"[%d, Rule=%-5s, Screen=%-5s] ",m_uiScreenID,
													TRUE_SELECT(m_bMatch==true,"TRUE","FALSE"),
													TRUE_SELECT(m_bScreen==true,"TRUE","FALSE"));
		}
		void fnTrace(char * _pszTrace,unsigned int _unSize)
		{
			ScreenOP * pclsFind = ScreenMgr::fnGetInstance()->fnFindScreenOP(m_uiScreenID);
			if(pclsFind)
			{
				fnRUStrnCat(_pszTrace,_unSize,"[ID=%-4d, 이름=%-40s, Rule_정합=%-5s, Screen"\
																					"_결과=%-5s] ",
													m_uiScreenID,fnRUStr(pclsFind->m_pszName),
													TRUE_SELECT(m_bMatch==true,"TRUE","FALSE"),
													TRUE_SELECT(m_bScreen==true,"TRUE","FALSE"));
			}
			else
			{
				fnRUStrnCat(_pszTrace,_unSize,"[ID=%-4d, 이름=%-40s, Rule_정합=%-5s, Screen_"\
																					"결과=%-5s] ",
														m_uiScreenID,"Not Found",
														TRUE_SELECT(m_bMatch==true,"TRUE","FALSE"),
														TRUE_SELECT(m_bScreen==true,"TRUE","FALSE"));
			}
		}
		unsigned int m_uiScreenID;
		bool m_bMatch;
		bool m_bScreen;
};
class ScreenOPSProc
{
	public:
		ScreenOPSProc();
		~ScreenOPSProc();
		bool fnProc(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
												resip::SipMessage * _pclsOrigMsg,
												ScreenOPS *_pclsSOPS);
		void fnResult();
		void fnTrace(char * _pszTrace, unsigned int _unMax);
		AppLines m_clsAddLines;
		AppLines m_clsRemoveLines;
	private:
		void fnAddResult(unsigned int _scrOPID, bool _bMatch, bool _bScreen)
		{
			ScreenResult * pclsNew = new ScreenResult; pclsNew->m_uiScreenID = _scrOPID;
			pclsNew->m_bMatch = _bMatch; pclsNew->m_bScreen = _bScreen;
			m_listResult.fnPushBack(pclsNew);
		}
		ScreenResult * fnBeginResult(){return (ScreenResult*)m_listResult.fnBegin();}
		ScreenResult * fnNextResult(){return (ScreenResult*)m_listResult.fnNext();}
		ScreenResult * fnBeginResult(RuListItem_t &_rclsItem){return (ScreenResult*)m_listResult.fnBegin(_rclsItem);}
		ScreenResult * fnNextResult(RuListItem_t &_rclsItem){return (ScreenResult*)m_listResult.fnNext(_rclsItem);}
		ScreenOPS *m_pclsSOPS;
		RUList m_listResult;
};

class ScreenEvent
{
	public:
		ScreenEvent();
		~ScreenEvent();
		bool fnSrcScreening(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
												resip::SipMessage * _pclsOrigSipMsg,
												unsigned int _unSrcSOPS);
		bool fnDstScreening(StackStateCbInfo_t & _rclsInfo,RuleEvent * _pclsRuleEv,
												resip::SipMessage * _pclsOrigSipMsg,
												unsigned int _unSrcSOPS);
		void fnResult(){m_clsSOPSProc.fnResult();}
		void fnTrace(char * _pszTrace, unsigned int _unMax){m_clsSOPSProc.fnTrace(_pszTrace,_unMax);}
	private:
		ScreenOPSProc m_clsSOPSProc;
};

#endif
