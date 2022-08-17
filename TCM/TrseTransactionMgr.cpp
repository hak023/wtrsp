#include "TrseTransactionMgr.h"
#include "main.h"
using namespace eSipUtil;
TrseTransactionMgr::TrseTransactionMgr(){m_pclsOwner = NULL;}
TrseTransactionMgr::~TrseTransactionMgr(){}
TrseTransaction * TrseTransactionMgr::m_fnFind(unsigned int _unTid, eSipUtil::KString _clsSessionID)
{
	KString clsKey = g_fnMakeTrseKey(_unTid, _clsSessionID);
	return (TrseTransaction*)m_mapTrse.m_fnFindMap((KCSTR)clsKey);
}
TrseTransaction * TrseTransactionMgr::m_fnSet(unsigned int _unTid, eSipUtil::KString _clsSessionID)
{
	//다른 세션에서 동일한 TID가 올 수 있음으로 SessionID와 묶어서 Key로 사용해야 한다.
	//Key str : SessionID-TransactionID
	//Key sample : {6b4984bf-31b0-4a53-4b8d-fdba06695305}-3399
	KString clsKey = g_fnMakeTrseKey(_unTid, _clsSessionID);

	TrseTransaction * pclsNew = new TrseTransaction;
	pclsNew->m_pclsOwner = (void*) this;
	if (m_mapTrse.m_fnSetMap((KCSTR)clsKey, pclsNew) == false)
	{
		return NULL;
	}
	return pclsNew;
}
bool TrseTransactionMgr::m_fnDel(unsigned int _unTid, eSipUtil::KString _clsSessionID)
{
	KString clsKey = g_fnMakeTrseKey(_unTid, _clsSessionID);
	return m_mapTrse.m_fnDelMap((KCSTR)clsKey);
}
