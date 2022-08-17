#include "TrssTransactionMgr.h"
#include "main.h"
using namespace eSipUtil;
TrssTransactionMgr::TrssTransactionMgr(){m_pclsOwner = NULL;}
TrssTransactionMgr::~TrssTransactionMgr(){}
TrssTransaction * TrssTransactionMgr::m_fnFind(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID)
{
	KString clsKey = g_fnMakeTrssKey(_unTid, _clsJobID, _clsSessionID);
	return (TrssTransaction*)m_mapTrss.m_fnFindMap((KCSTR)clsKey);
}
TrssTransaction * TrssTransactionMgr::m_fnSet(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID, eSipUtil::KString _clsTrsgName, eSipUtil::KString _clsTrssName)
{
	//다른 세션에서 동일한 TID가 올 수 있음으로 SessionID와 묶어서 Key로 사용해야 한다.
	//Key str : SessionID-TransactionID-JobID
	//Key sample : {6b4984bf-31b0-4a53-4b8d-fdba06695305}-3399-{6b4984bf-31b0-4a53-4b8d-aaasdeqsdwew}
	KString clsKey = g_fnMakeTrssKey(_unTid, _clsJobID, _clsSessionID);

	TrssTransaction * pclsNew = new TrssTransaction;
	pclsNew->m_pclsOwner = (void*) this;
	pclsNew->m_clsTrsgName = _clsTrsgName;
	pclsNew->m_clsTrssName = _clsTrssName;
	pclsNew->m_clsSessionID = _clsSessionID;
	if (m_mapTrss.m_fnSetMap((KCSTR)clsKey, pclsNew) == false)
	{
		return NULL;
	}
	return pclsNew;
}
bool TrssTransactionMgr::m_fnDel(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID)
{
	KString clsKey = g_fnMakeTrssKey(_unTid, _clsJobID, _clsSessionID);
	return m_mapTrss.m_fnDelMap((KCSTR)clsKey);
}
