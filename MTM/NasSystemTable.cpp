#include "NasSystemTable.h"
#include "MainConfig.h"
#include "AppIf.h"
#include "main.h"

using namespace eSipUtil;

void g_fnCreateNasSystemTable()
{
	NasSystemTable * pclsObj = NasSystemTable::m_fnGetInstance();
	pclsObj->m_fnInit();
}
NasSystemTable * NasSystemTable::m_pclsMy = NULL;

/************************** NasSystem **************************/
NasSystem::NasSystem(){}
NasSystem::~NasSystem(){}
char * NasSystem::m_fnDebug(KString & _rclsDebug)
{
	_rclsDebug<<"CODE="<<m_clsCode;
	_rclsDebug<<", SOURCE_DIR="<<m_clsSourceDir;
	_rclsDebug<<"\n";
	return (KSTR)_rclsDebug;
}
NasSystemTable::NasSystemTable()
{
	m_listNasSystem.m_fnClear();
}
NasSystemTable::~NasSystemTable()
{
	m_listNasSystem.m_fnClear();
}
NasSystemTable * NasSystemTable::m_fnGetInstance()
{
	if(m_pclsMy==NULL) m_pclsMy = new NasSystemTable;
	return m_pclsMy;
}
void NasSystemTable::m_fnInit()
{
	m_clsNasSystemFile.m_fnLoadConfig("../config/MTM/nas_system_table.cfg",NULL);
	m_clsNasSystemFile.m_fnSetNotifyChangedFile(m_fnCbkNasSystemChangedFile,true,this);
	m_fnLoadCfg();
	KString clsDebug;m_fnDebug(clsDebug);
	IFLOG(E_LOG_INFO,"<----- [Init] NAS System Tables ----->\n%s",(KCSTR)clsDebug);

}
void NasSystemTable::m_fnLoadCfg()
{
	m_clsLock.m_fnWriteLock();
	m_listNasSystem.m_fnClear();
	m_clsNasRoot = (KCSTR)m_clsNasSystemFile.m_fnFindVal("MAIN","NAS_ROOT_DIR");
	unsigned int unNasCnt = (KUINT)m_clsNasSystemFile.m_fnFindVal("MAIN","NAS_NUM");
	KString clsTmp; NasSystem * pclsNasPoint = NULL;
	for(unsigned int i=0;i<unNasCnt;i++)
	{
		clsTmp = KNULL;clsTmp<<"NAS_SYSTEM_"<<i;
		pclsNasPoint = new NasSystem;
		pclsNasPoint->m_clsCode = (KCSTR)m_clsNasSystemFile.m_fnFindVal((KCSTR)clsTmp,"CODE");
		pclsNasPoint->m_clsSourceDir = (KCSTR)m_clsNasSystemFile.m_fnFindVal((KCSTR)clsTmp,"SOURCE_DIR");

		pclsNasPoint->m_fnSetKey((KCSTR)m_clsNasSystemFile.m_fnFindVal((KCSTR)clsTmp,"CODE"));

		m_listNasSystem.m_fnPushBack(pclsNasPoint);
	}
	m_clsLock.m_fnUnlock();
}
void NasSystemTable::m_fnGetSourceDir(KString &_rclsNasCode, KString &_rclsSourceDir)
{
	m_clsLock.m_fnReadLock();
	Iterator stItor;
	NasSystem *pclsNasSystem = (NasSystem*) m_listNasSystem.m_fnGetNext(stItor);
	while (pclsNasSystem)
	{
		//NasCode È®ÀÎ
		_rclsSourceDir.m_fnReSize(10240);
		if (KString::m_fnStrCmp((KCSTR) pclsNasSystem->m_clsCode, (KCSTR) _rclsNasCode) == 0)
		{
			_rclsSourceDir.m_fnCat((KCSTR)m_clsNasRoot); 
			if(((KCSTR)m_clsNasRoot)[m_clsNasRoot.m_unRealLen - 1] != '/')
					_rclsSourceDir.m_fnCat("/");
			_rclsSourceDir.m_fnCat((KCSTR)pclsNasSystem->m_clsSourceDir);
			IFLOG(E_LOG_INFO, "NasSystemTable [Code:%s, SourceDir:%s]", (KCSTR) _rclsNasCode, (KCSTR) _rclsSourceDir);
			break;
		}
		else
		{
			_rclsSourceDir = "UNKNOWN";
		}
		pclsNasSystem = (NasSystem*) m_listNasSystem.m_fnGetNext(stItor);
	}
   m_clsLock.m_fnUnlock();
}
void NasSystemTable::m_fnDebug(KString & _rclsDebug)
{
	m_clsLock.m_fnReadLock();
	_rclsDebug<<"<NAS_SYSTEM_TABLE>\n";
	_rclsDebug<<"NAS_ROOT_DIR" << "=" << m_clsNasRoot << "\n";
	Iterator stItor;
	NasSystem * pclsNasPoint = (NasSystem*)m_listNasSystem.m_fnGetNext(stItor);
	while(pclsNasPoint)
	{
		pclsNasPoint->m_fnDebug(_rclsDebug);
		pclsNasPoint = (NasSystem*)m_listNasSystem.m_fnGetNext(stItor);
	}
	_rclsDebug<<"</NAS_SYSTEM_TABLE>\n";

	m_clsLock.m_fnUnlock();
}
void NasSystemTable::m_fnCbkNasSystemChangedFile(CfgFile * _pclsObj)
{
	_pclsObj->m_fnReLoadConfig();
	NasSystemTable * pclsMy = (NasSystemTable*)_pclsObj->m_fnGetUser();
	pclsMy->m_fnLoadCfg();
	KString clsDebug;pclsMy->m_fnDebug(clsDebug);
	IFLOG(E_LOG_INFO,"<----- [Change] NAS System Tables ----->\n%s", (KCSTR)clsDebug);
}
		
