#include "main.h"
#include "TrsgCdr.h"
#include "MainConfig.h"

using namespace eSipUtil;

TrsgCdr * TrsgCdr::m_pclsMy = NULL;
TrsgCdr* g_fnCreateTrsgCdr()
{
	IFLOG(E_LOG_INFO,"CREATE TRSG CDR MODULE : [%-30s] Start",__func__);
	return TrsgCdr::m_fnGetInstance();
}
TrsgCdr * TrsgCdr::m_fnGetInstance()
{
	if(m_pclsMy==NULL) m_pclsMy = new TrsgCdr;
	return m_pclsMy;
}
TrsgCdr::TrsgCdr()
{
	IFLOG(E_LOG_INFO, "========== Trsg Cdr init ==========");
	m_clsTrsgCdrMgr.m_fnInit("TRSG_MSG", "../stat/TRSG", MainConfig::m_fnGetInstance()->m_unSysIndex, 5);
}
TrsgCdr::~TrsgCdr()
{
}
void TrsgCdr::m_fnAddCdr(KString& _clsCdrMsg)
{
	TrsgCdrData* pclsTrsgCdrData = new TrsgCdrData;
	pclsTrsgCdrData->m_clsBuff = (KCSTR)_clsCdrMsg;
	if(m_clsTrsgCdrMgr.m_fnAddCdr(pclsTrsgCdrData) == false)
	{
		IFLOG(E_LOG_INFO, "Trsg Cdr Write Fail[%s]", (KCSTR)_clsCdrMsg);
	}
}
