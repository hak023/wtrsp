#include "TrsgTransactionTimer.h"
#include "ApiEvent.h"
#include "Worker.h"

using namespace eSipUtil;

TrsgTrTimerMgr::TrsgTrTimerMgr():m_clsTimer(TrsgTrTimerMgr::fnCbkTimerOut,"TrsgTr")
{
	m_pclsOwner = NULL;
}
TrsgTrTimerMgr::~TrsgTrTimerMgr(){}
void TrsgTrTimerMgr::m_fnSetTimer(const char * _pszSessionID, unsigned int _unTid, const char * _pszJobID, unsigned long _ulMs)
{
	TrsgTrTimerObj * pclsNew = new TrsgTrTimerObj;
	pclsNew->m_pclsOwner = this;
	pclsNew->m_clsSessionID = _pszSessionID;
	pclsNew->m_unTid = _unTid;
	pclsNew->m_clsJobID = _pszJobID;
	m_clsTimer.setTimer(pclsNew,_ulMs);
}
void TrsgTrTimerMgr::fnCbkTimerOut(TrsgTrTimerObj * _pclsData)
{
	if(_pclsData)
	{
		AppTrsgTimerEvent * pclsEv = new AppTrsgTimerEvent;
		pclsEv->m_clsSessionID = _pclsData->m_clsSessionID;
		pclsEv->m_unTid = _pclsData->m_unTid;
		Worker::m_fnPutTrsgTimerEv(pclsEv);
		delete _pclsData;
	}
}
