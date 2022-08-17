#ifndef TRSS_TRANSACTION_MGR_H
#define TRSS_TRANSACTION_MGR_H

#include "StlMap.hxx"
#include "TrssTransaction.h"
#include "TrssTransactionTimer.h"
class TrssTransactionMgr
{
	public:
		TrssTransactionMgr();
		~TrssTransactionMgr();
		TrssTransaction * m_fnFind(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID);
		TrssTransaction * m_fnSet(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID, eSipUtil::KString _clsTrsgName, eSipUtil::KString _clsTrssName);
		bool m_fnDel(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID);
		eSipUtil::StlMap m_mapTrss;
		void * m_pclsOwner;  // Worker;
};
#endif

