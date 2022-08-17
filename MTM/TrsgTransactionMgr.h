#ifndef TRSS_TRANSACTION_MGR_H
#define TRSS_TRANSACTION_MGR_H

#include "StlMap.hxx"
#include "TrsgTransaction.h"
#include "TrsgTransactionTimer.h"
class TrsgTransactionMgr
{
	public:
		TrsgTransactionMgr();
		~TrsgTransactionMgr();
		TrsgTransaction * m_fnFind(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID);
		TrsgTransaction * m_fnSet(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID);
		bool m_fnDel(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID);
		eSipUtil::StlMap m_mapTrsg;
		void * m_pclsOwner;  // Worker;
};
#endif

