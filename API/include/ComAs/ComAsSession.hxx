
#ifndef COM_AS_SESSION_HXX
#define COM_AS_SESSION_HXX

#include "eRte/eSipRteApi.hxx"

#include "eDum/eDumLibSession.hxx"
#include "eDum/eDumLibDialog.hxx"
#include "eDum/eDumLibFsm.hxx"

#include "ComAsRule.hxx"
#include "ComAsCdr.hxx"

namespace ComAs
{

struct ComAsSessData
{
	enum
	{
		E_CONST_MAX_MIN_LEN = 128,
		E_CONST_MAX_SCROP_NUMS = 32,
	};

	eDum::EDialogInitDir_t m_eCallDir;
	eDum::ESipState_t m_eSipState;
	ECdrStartHost m_eCallStartHost;
	ECdrEndHost m_eCallEndHost;
	char m_szCaller[E_CONST_MAX_MIN_LEN];
	char m_szCallee[E_CONST_MAX_MIN_LEN];
	time_t m_ulCallEndTime;

	//SIP
	unsigned int m_unRseq;

	//RouteOp
	int m_nRouteNodeType;					//ComAsRouteOpInfo::ERouteNodeType
	int m_nRouteOpId;
	unsigned int m_unRouteSeqId;		//SeqId of RTE(SIP) or SeqId of NodeMgr(ExtAs)

	//SipRoute
	ESNodeType m_eSNodeType;
	unsigned int m_unSipRouteId;		//for mss
	char m_szSipRouteName[DEF_RTE_MAXLEN_DESC];		//for mss	

	//ExtAs
	int m_nExtAsId;
	int m_nExtAsSessId;

	//Screen
	int m_arrScrOp[E_CONST_MAX_SCROP_NUMS];
	unsigned int m_unScrOpNum;

	void m_fnReset()
	{
		m_eCallDir = eDum::E_DIALOG_INIT_DIR_ORIG;
		m_eSipState = eDum::E_SIP_STATE_IDLE;
		m_eCallStartHost = E_COM_AS_START_HOST_none;
		m_eCallEndHost = E_COM_AS_END_HOST_none;
		m_szCaller[0] = NULL;
		m_szCallee[0] = NULL;
		m_ulCallEndTime = 0;

		m_unRseq = 0;
	
		m_nRouteNodeType = -1;
		m_nRouteOpId = -1;
		m_unRouteSeqId = 0;

		m_eSNodeType = E_SNODE_TYPE_NUM;
		m_unSipRouteId = 0;
		m_szSipRouteName[0] = 0x00;
		
		m_nExtAsId = -1;
		m_nExtAsSessId = -1;

		for( int i = 0; i < E_CONST_MAX_SCROP_NUMS; ++i )
		{
			m_arrScrOp[i] = -1;
		}
		m_unScrOpNum = 0;
	}
};

class ComAsSessMgr
{
	public :
		static bool m_fnInit();
		static void m_fnInit(eDum::Session & _rclsSession);
		static void m_fnSetCallDir(eDum::Session & _rclsSession, const eDum::Dialog & _rclsDialog);
		static void m_fnSetSipState(eDum::Session & _rclsSession, eDum::ESipState_t _eSipState);
		static void m_fnSetCallStartSipNode(eDum::Session & _rclsSession);
		static void m_fnSetCallStartExtAs(eDum::Session & _rclsSession);
		static void m_fnSetCallEndSipNode(eDum::Session & _rclsSession);
		static void m_fnSetCallEndComAs(eDum::Session & _rclsSession);
		static void m_fnSetCallEndExtAs(eDum::Session & _rclsSession);
		static void m_fnSetCaller(eDum::Session & _rclsSession, const char * _pszCaller);
		static void m_fnSetCallee(eDum::Session & _rclsSession, const char * _pszCallee);
		static void m_fnSetCallEndTime(eDum::Session & _rclsSession);
		static void m_fnSetRSeq(eDum::Session & _rclsSession, unsigned int _unRseq);
		static void m_fnSetRouteNodeType(eDum::Session & _rclsSession, int _nRouteNodeType);
		static void m_fnSetRouteOpId(eDum::Session & _rclsSession, int _nRouteOpId);
		static void m_fnSetRouteSeqId(eDum::Session & _rclsSession, unsigned int _unRouteSeqId);
		static void m_fnSetSNodeType(eDum::Session & _rclsSession, ESNodeType _eSNodeType);
		static void m_fnSetSipRouteInfo(eDum::Session & _rclsSession, const eDum::Dialog & _rclsDialog);
		static void m_fnSetExtAsId(eDum::Session & _rclsSession, unsigned int _unAsId);
		static void m_fnSetExtAsSessId(eDum::Session & _rclsSession, unsigned int _unAsSessId);
		static void m_fnSetScrOp(eDum::Session & _rclsSession, const int * _arrScrOp, unsigned int _unNums);

		static eDum::EDialogInitDir_t m_fnGetCallDir(const eDum::Session & _rclsSession);
		static eDum::ESipState_t m_fnGetSipState(const eDum::Session & _rclsSession);
		static ECdrStartHost m_fnGetCallStartHost(const eDum::Session & _rclsSession);
		static ECdrEndHost m_fnGetCallEndHost(const eDum::Session & _rclsSession);
		static const char * m_fnGetCaller(const eDum::Session & _rclsSession);
		static const char * m_fnGetCallee(const eDum::Session & _rclsSession);
		static const time_t m_fnGetCallEndTime(const eDum::Session & _rclsSession);
		static unsigned int m_fnGetRSeq(const eDum::Session & _rclsSession);
		static int m_fnGetRouteNodeType(const eDum::Session & _rclsSession);
		static int m_fnGetRouteOpId(const eDum::Session & _rclsSession);
		static unsigned int m_fnGetRouteSeqId(const eDum::Session & _rclsSession);
		static ESNodeType m_fnGetSNodeType(const eDum::Session & _rclsSession);
		static unsigned int m_fnGetSipRouteId(const eDum::Session & _rclsSession);
		static const char * m_fnGetSipRouteName(const eDum::Session & _rclsSession);
		static int m_fnGetExtAsId(const eDum::Session & _rclsSession);
		static int m_fnGetExtAsSessId(const eDum::Session & _rclsSession);
		static void m_fnGetScrOp(const eDum::Session & _rclsSession, std::list< int > & _rlistScrOpId);

};

}

#endif

