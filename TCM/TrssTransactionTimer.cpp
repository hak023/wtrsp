#include "TrssTransactionTimer.h"
#include "ApiEvent.h"
#include "Worker.h"

using namespace eSipUtil;

TrssTrTimerMgr::TrssTrTimerMgr():m_clsTimer(TrssTrTimerMgr::fnCbkTimerOut,"TrssTr")
{
	m_pclsOwner = NULL;
}
TrssTrTimerMgr::~TrssTrTimerMgr(){}
void TrssTrTimerMgr::m_fnSetTimer(const char * _pszSessionID, const char * _pszJobID, unsigned int _unTid, unsigned long _ulMs)
{
	TrssTrTimerObj * pclsNew = new TrssTrTimerObj;
	pclsNew->m_pclsOwner = this;
	pclsNew->m_clsSessionID = _pszSessionID;
	pclsNew->m_clsJobID = _pszJobID;
	pclsNew->m_unTid = _unTid;
	m_clsTimer.setTimer(pclsNew,_ulMs);
}
void TrssTrTimerMgr::fnCbkTimerOut(TrssTrTimerObj * _pclsData)
{
	if(_pclsData)
	{
		AppTrssTimerEvent * pclsEv = new AppTrssTimerEvent;
		pclsEv->m_clsSessionID = _pclsData->m_clsSessionID;
		pclsEv->m_clsJobID = _pclsData->m_clsJobID;
		pclsEv->m_unTid = _pclsData->m_unTid;
		Worker::m_fnPutTrssTimerEv(pclsEv);
		delete _pclsData;
	}
}
