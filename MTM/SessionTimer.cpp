#include "ApiEvent.h"
#include "Worker.h"
#include "main.h"

using namespace eSipUtil;

SessionTimerMgr::SessionTimerMgr():m_clsTimer(SessionTimerMgr::fnCbkTimerOut,"Sess")
{
	m_pclsOwner = NULL;
}
SessionTimerMgr::~SessionTimerMgr(){}
void SessionTimerMgr::m_fnSetTimer(const char * _pszSessionID, unsigned int _unTid, const char * _pszJobID, unsigned long _ulMs)
{
	SessionTimerObj * pclsNew = new SessionTimerObj;
	pclsNew->m_pclsOwner = this;
	pclsNew->m_clsSessionID = _pszSessionID;
	pclsNew->m_unTid = _unTid;
	pclsNew->m_clsJobID = _pszJobID;
	m_clsTimer.setTimer(pclsNew,_ulMs);
}
void SessionTimerMgr::fnCbkTimerOut(SessionTimerObj * _pclsData)
{
	if(_pclsData)
	{
		AppSessionTimerEvent * pclsEv = new AppSessionTimerEvent;
		pclsEv->m_clsSessionID = _pclsData->m_clsSessionID;
		pclsEv->m_unTid = _pclsData->m_unTid;
		pclsEv->m_clsJobID = _pclsData->m_clsJobID;
		pclsEv->m_eT = E_MAIN_EV_SESSION_TIMEOUT;
		Worker::m_fnPutSessionTimerEv(pclsEv);
		delete _pclsData;
	}
}
