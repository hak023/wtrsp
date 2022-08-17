#include "TrsgTransactionMgr.h"
#include "main.h"
using namespace eSipUtil;
TrsgTransactionMgr::TrsgTransactionMgr()
{
	m_pclsOwner = NULL;
}
TrsgTransactionMgr::~TrsgTransactionMgr()
{
}
TrsgTransaction* TrsgTransactionMgr::m_fnFind(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID)
{
	KString clsKey = g_fnMakeTrsgKey(_unTid, _clsJobID, _clsSessionID);
	return (TrsgTransaction*) m_mapTrsg.m_fnFindMap((KCSTR) clsKey);
}
TrsgTransaction* TrsgTransactionMgr::m_fnSet(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID)
{
	//다른 세션에서 동일한 TID가 올 수 있음으로 SessionID와 묶어서 Key로 사용해야 한다.
	//Key str : SessionID-TransactionID-JobID
	//Key sample : {6b4984bf-31b0-4a53-4b8d-fdba06695305}-3399-{6b4984bf-31b0-4a53-4b8d-aaasdeqsdwew}
	KString clsKey = g_fnMakeTrsgKey(_unTid, _clsJobID, _clsSessionID);

	TrsgTransaction *pclsNew = new TrsgTransaction;
	pclsNew->m_pclsOwner = (void*) this;
	pclsNew->m_clsSessionID = _clsSessionID;
	if (m_mapTrsg.m_fnSetMap((KCSTR) clsKey, pclsNew) == false)
	{
		return NULL;
	}
	return pclsNew;
}
bool TrsgTransactionMgr::m_fnDel(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID)
{
	KString clsKey = g_fnMakeTrsgKey(_unTid, _clsJobID, _clsSessionID);
	return m_mapTrsg.m_fnDelMap((KCSTR) clsKey);
}
