#ifndef SESSION_MGR_H
#define SESSION_MGR_H
#include "StlMap.hxx"
#include "Session.h"
#include "idManager.hxx"

typedef struct SesStruct
{
	SesStruct()
	{
		m_pclsSes = NULL;
		stStartTime = time(NULL);
		m_clsSessionID = KNULL;
		m_unTid = 0;
	}
	Session* m_pclsSes;
	time_t stStartTime;
	eSipUtil::KString m_clsSessionID;
	unsigned int m_unTid;
} SesStruct;

class SessionMgr
{
	public:
		SessionMgr();
		~SessionMgr();
		Session * m_fnSessionFind(unsigned int _unTid, eSipUtil::KString _clsSessionID);
		Session * m_fnSessionSet(unsigned int _unTid, eSipUtil::KString _clsSessionID, void * _pvTrseTr);
		bool m_fnTrseDel(unsigned int _unTid, eSipUtil::KString _clsSessionID);
		void m_fnSesAllClear();
		bool m_fnIsOverload();
		eSipUtil::StlMap m_mapSession;
		void * m_pclsOwner;   // Worker
		void m_fnGarbageClear();
		void m_fnTrseDisconSesClear(eSipUtil::KString _clsSessionID);

	private:
		eSipUtil::IdMgr *m_pIdMgr;
		unsigned int m_unStartIdx;
		unsigned int m_unEndIdx;
		unsigned int m_unGarbageCheckCnt;
		unsigned int m_unIdMgrNum;
		SesStruct* m_pclsSesStruct;
};

#endif
