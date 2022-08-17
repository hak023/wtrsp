#include "SessionMgr.h"
#include "TrsgTransaction.h"
#include "MainConfig.h"
#include "main.h"
#include "Worker.h"

using namespace eSipUtil;
SessionMgr::SessionMgr()
{
	m_pclsOwner = NULL;
	m_unIdMgrNum = MainConfig::m_fnGetInstance()->m_unIdMgrNum;	// 10000
	m_unGarbageCheckCnt = MainConfig::m_fnGetInstance()->m_unGarbageCheckCnt;
	m_pIdMgr = new IdMgr(m_unIdMgrNum);
	m_pclsSesStruct = new SesStruct[m_unIdMgrNum];

	m_unStartIdx = 0;
	m_unEndIdx = m_unStartIdx + m_unGarbageCheckCnt;
}
SessionMgr::~SessionMgr()
{
	if(m_pIdMgr) delete m_pIdMgr;
	if(m_pclsSesStruct) delete[] m_pclsSesStruct;
}
Session * SessionMgr::m_fnSessionFind(unsigned int _unTid, KString _clsJobID, KString _clsSessionID)
{
   KString clsKey = g_fnMakeTrsgKey(_unTid, _clsJobID, _clsSessionID);
   return (Session*)m_mapSession.m_fnFindMap((KCSTR)clsKey);
}
Session * SessionMgr::m_fnSessionSet(unsigned int _unTid, KString _clsJobID, KString _clsSessionID, void * _pvTrsgTr)
{
	Session * pclsNew = new Session;
	pclsNew->m_pclsOwner = (void*)this;
	pclsNew->m_pclsTrsgTr = (TrsgTransaction*)_pvTrsgTr;
	pclsNew->m_unTid = _unTid;
	pclsNew->m_clsSessionID = _clsSessionID;
	pclsNew->m_clsJobID = _clsJobID;
	pclsNew->m_nAllocSes = m_pIdMgr->getAvailableId();
	KString clsKey = g_fnMakeTrsgKey(_unTid, _clsJobID, _clsSessionID);
	
	if(m_mapSession.m_fnSetMap((KCSTR)clsKey, pclsNew) == false)
	{
		if(pclsNew->m_nAllocSes >=0)
		{
			m_pIdMgr->releaseId(pclsNew->m_nAllocSes);
		}
		return NULL;
	}
	if(pclsNew->m_nAllocSes < 0) 
	{
		m_mapSession.m_fnDelMap((KCSTR)clsKey);
		return NULL;
	}

	m_pclsSesStruct[pclsNew->m_nAllocSes].m_pclsSes = pclsNew;
	m_pclsSesStruct[pclsNew->m_nAllocSes].m_clsSessionID = _clsSessionID;
	m_pclsSesStruct[pclsNew->m_nAllocSes].m_unTid = _unTid;
	m_pclsSesStruct[pclsNew->m_nAllocSes].m_clsJobID = _clsJobID;
	m_pclsSesStruct[pclsNew->m_nAllocSes].stStartTime = time(NULL);
	return pclsNew;
}
bool SessionMgr::m_fnIsOverload()
{
	return (m_pIdMgr->getAvailableIdNum() == 0);
}
bool SessionMgr::m_fnTrsgDel(unsigned int _unTid, KString _clsJobID, KString _clsSessionID)
{
	KString clsKey = g_fnMakeTrsgKey(_unTid, _clsJobID, _clsSessionID);
	Session* pclsSes = (Session*)m_mapSession.m_fnFindMap((KCSTR)clsKey);
	if(pclsSes == NULL) return false;

	if(pclsSes->m_nAllocSes >=0)
	{
		m_pIdMgr->releaseId(pclsSes->m_nAllocSes);
	}
	m_pclsSesStruct[pclsSes->m_nAllocSes].m_pclsSes = NULL;
	m_pclsSesStruct[pclsSes->m_nAllocSes].m_clsSessionID = _clsSessionID;
	m_pclsSesStruct[pclsSes->m_nAllocSes].m_unTid = _unTid;
	m_pclsSesStruct[pclsSes->m_nAllocSes].m_clsJobID = _clsJobID;
	m_pclsSesStruct[pclsSes->m_nAllocSes].stStartTime = time(NULL);

	return m_mapSession.m_fnDelMap((KCSTR)clsKey);
}
void SessionMgr::m_fnSesAllClear()
{
	if(m_pclsSesStruct) delete[] m_pclsSesStruct;
	m_pclsSesStruct = new SesStruct[m_unIdMgrNum];

	m_mapSession.m_fnClear();
	m_pIdMgr->reset();
}
void SessionMgr::m_fnGarbageClear()
{
	Worker *pclsWorker = (Worker*) m_pclsOwner;
	IFLOG(E_LOG_ERR, "Garbage Check Worker[%u], getAvailableIdNum(%d)", pclsWorker->m_unIdx, pclsWorker->m_pWorkerIdMgr->getAvailableIdNum());
	time_t stCurrentTime = time(NULL);
	for(unsigned int unIdx = m_unStartIdx; unIdx < m_unEndIdx; unIdx++)
	{
		if (m_pclsSesStruct[unIdx].m_pclsSes != NULL)
		{
			if ((stCurrentTime - m_pclsSesStruct[unIdx].stStartTime) >= MainConfig::m_fnGetInstance()->m_unGarbageSesTmr) //garbage timer..
			{
				KString clsCurrentTime;
				KString clsStartTime;
				DailyTime clsGetTime;
				clsGetTime.m_fnGetTimetToStr((unsigned int) stCurrentTime, clsCurrentTime);
				clsGetTime.m_fnGetTimetToStr((unsigned int) m_pclsSesStruct[unIdx].stStartTime, clsStartTime);
				IFLOG(E_LOG_ERR, "Garbage Clear WORKER[%d] ID[%u] SessionID[%s] Tid[%d] JobID[%s] stCurrentTime[%s], stStartTime[%s], diff[%d]", pclsWorker->m_unIdx, unIdx, (KSTR) m_pclsSesStruct[unIdx].m_clsSessionID, m_pclsSesStruct[unIdx].m_unTid, (KSTR) m_pclsSesStruct[unIdx].m_clsJobID, (KCSTR) clsCurrentTime, (KCSTR) clsStartTime, MainConfig::m_fnGetInstance()->m_unGarbageSesTmr);

				m_pIdMgr->releaseId(unIdx);
				KString clsKey = g_fnMakeTrsgKey(m_pclsSesStruct[unIdx].m_unTid, m_pclsSesStruct[unIdx].m_clsJobID, m_pclsSesStruct[unIdx].m_clsSessionID);
				m_mapSession.m_fnDelMap((KSTR) clsKey);
				//init
				m_pclsSesStruct[unIdx].m_pclsSes = NULL;
				m_pclsSesStruct[unIdx].stStartTime = time(NULL);
				m_pclsSesStruct[unIdx].m_clsSessionID = KNULL;
				m_pclsSesStruct[unIdx].m_clsJobID = KNULL;
				m_pclsSesStruct[unIdx].m_unTid = 0;
			}
			else
			{
				//none..
			}
		}
		else
		{
			//none..
		}
	}
	m_unGarbageCheckCnt = MainConfig::m_fnGetInstance()->m_unGarbageCheckCnt;

	m_unStartIdx += m_unGarbageCheckCnt;
	m_unEndIdx += m_unGarbageCheckCnt;
	if(m_unEndIdx > m_unIdMgrNum)//끝에 남는 배열 처리.
	{
		m_unEndIdx = m_unIdMgrNum;
	}
	if(m_unStartIdx > m_unIdMgrNum-1)//시작 배열이 Max index 넘을 경우 처음부터.
	{
		m_unStartIdx = 0;
		m_unEndIdx = m_unStartIdx + m_unGarbageCheckCnt;
	}

}
