#include "GarbageTimer.h"
#include "ApiEvent.h"
#include "Worker.h"
#include "main.h"
using namespace eSipUtil;

GarbageTimerMgr::GarbageTimerMgr():m_clsTimer(GarbageTimerMgr::fnCbkTimerOut,"Garbage")
{
	m_pclsOwner = NULL;
}
GarbageTimerMgr::~GarbageTimerMgr(){}
void GarbageTimerMgr::m_fnSetTimer(const unsigned int _unWorkerIdx, unsigned long _ulMs)
{
	GarbageTimerObj * pclsNew = new GarbageTimerObj;
	pclsNew->m_pclsOwner = this;
	pclsNew->m_unWorkerIdx = _unWorkerIdx;
	m_clsTimer.setTimer(pclsNew,_ulMs);
}
void GarbageTimerMgr::fnCbkTimerOut(GarbageTimerObj * _pclsData)
{
	if(_pclsData)
	{
		AppGarbageTimerEvent * pclsEv = new AppGarbageTimerEvent;
		pclsEv->m_unWorkerIdx = _pclsData->m_unWorkerIdx;
		Worker::m_fnPutGarbageClear(pclsEv);
		delete _pclsData;
	}
}