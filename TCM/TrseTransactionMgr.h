#ifndef TRSE_TRANSACTION_MGR_H
#define TRSE_TRANSACTION_MGR_H
#include "StlMap.hxx"
#include "TrseTransaction.h"
class TrseTransactionMgr
{
	public:
		TrseTransactionMgr();
		~TrseTransactionMgr();
		TrseTransaction * m_fnFind(unsigned int _unTid, eSipUtil::KString _clsSessionID);
		TrseTransaction * m_fnSet(unsigned int _unTid, eSipUtil::KString _clsSessionID);
		bool m_fnDel(unsigned int _unTid, eSipUtil::KString _clsSessionID);
		eSipUtil::StlMap m_mapTrse;
		void * m_pclsOwner;  // Worker;
};
#endif
