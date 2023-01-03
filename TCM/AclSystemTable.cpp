#include "AclSystemTable.h"
#include "MainConfig.h"
#include "AppIf.h"
#include "main.h"

using namespace eSipUtil;

void g_fnCreateAclSystemTable()
{
	AclSystemTable * pclsObj = AclSystemTable::m_fnGetInstance();
	pclsObj->m_fnInit();
}
AclSystemTable * AclSystemTable::m_pclsMy = NULL;

/************************** AclSystem **************************/
AclSystem::AclSystem(){}
AclSystem::~AclSystem(){}
char * AclSystem::m_fnDebug(eSipUtil::KString & _rclsDebug)
{

	_rclsDebug<<"ID="<<m_clsID;
	_rclsDebug<<", PWD="<<m_clsPWD;
	_rclsDebug<<",\n";
	TokStrings clsResult;
	KString clsIPList = m_clsIP;
	KString::m_fnTokenizeC((KSTR)clsIPList, ";", clsResult);
	_rclsDebug<< " [IP LIST CNT = " << clsResult.m_fnGetNum() << "]\n";
	for(unsigned int i=0;i<clsResult.m_fnGetNum();i++)
	{
		_rclsDebug<< " IP[" << i << "] =";
		_rclsDebug << clsResult[i] << "\n";
	}
	return (KSTR)_rclsDebug;
}
AclSystemTable::AclSystemTable()
{
	m_listAclSystem.m_fnClear();
}
AclSystemTable::~AclSystemTable()
{
   m_listAclSystem.m_fnClear();
}
AclSystemTable * AclSystemTable::m_fnGetInstance()
{
	if(m_pclsMy==NULL) m_pclsMy = new AclSystemTable;
	return m_pclsMy;
}
void AclSystemTable::m_fnInit()
{
	m_clsAclSystemFile.m_fnLoadConfig("../config/TCM/acl_system_table.cfg",NULL);
	m_clsAclSystemFile.m_fnSetNotifyChangedFile(m_fnCbkAclSystemChangedFile,true,this);
	m_fnLoadDB();
	KString clsDebug;m_fnDebug(clsDebug);
	IFLOG(E_LOG_INFO,"<----- [Init] Acl System Tables ----->\n%s",(KCSTR)clsDebug);

}
void AclSystemTable::m_fnLoadDB()
{
	m_clsLock.m_fnWriteLock();
	m_listAclSystem.m_fnClear();
	unsigned int unEndPoint = (KUINT)m_clsAclSystemFile.m_fnFindVal("MAIN","ACL_NUM");
	KString clsTmp; AclSystem * pclsEndPoint = NULL;
	for(unsigned int i=0;i<unEndPoint;i++)
	{
		clsTmp = KNULL;clsTmp<<"ACL_SYSTEM_"<<i;
		pclsEndPoint = new AclSystem;
		pclsEndPoint->m_clsID = (KCSTR)m_clsAclSystemFile.m_fnFindVal((KCSTR)clsTmp,"ID");
		pclsEndPoint->m_clsPWD = (KCSTR)m_clsAclSystemFile.m_fnFindVal((KCSTR)clsTmp,"PWD");
		pclsEndPoint->m_clsIP = (KCSTR)m_clsAclSystemFile.m_fnFindVal((KCSTR)clsTmp,"IP");
		//pclsEndPoint->m_fnSetKey((KCSTR)m_clsAclSystemFile.m_fnFindVal((KCSTR)clsTmp,"ID"));

		m_listAclSystem.m_fnPushBack(pclsEndPoint);
	}
	m_clsLock.m_fnUnlock();
}
ETrsgCodeSet_t AclSystemTable::m_fnCheckLogin(KString & _rclsID, KString & _rclsPWD, KString & _rclsIP)
{
	/*
	1006	IP 접근 권한 실패
	1007	ID/PW 접근 권한 실패
	*/
	m_clsLock.m_fnReadLock();
	Iterator stItor;
	AclSystem * pclsAclSystem = (AclSystem*)m_listAclSystem.m_fnGetNext(stItor);
	bool bAccountFind = false;
	while(pclsAclSystem)
	{
		//ID/PWD 만 확인.
		if(KString::m_fnStrCmp((KCSTR)pclsAclSystem->m_clsID, (KCSTR)_rclsID) == 0
				&& KString::m_fnStrCmp((KCSTR)pclsAclSystem->m_clsPWD, (KCSTR)_rclsPWD) == 0)
		{
			IFLOG(E_LOG_INFO,"AclSystemTable ID/PW OK [ID:%s, PW:%s, IP:%s]", (KCSTR)_rclsID, (KCSTR)_rclsPWD, (KCSTR)_rclsIP);
			bAccountFind = true;
			break;
		}
		pclsAclSystem = (AclSystem*)m_listAclSystem.m_fnGetNext(stItor);
	}
	if(bAccountFind == false)
	{
		IFLOG(E_LOG_INFO,"AclSystemTable ID/PW Fail [ID:%s, PW:%s, IP:%s]", (KCSTR)_rclsID, (KCSTR)_rclsPWD, (KCSTR)_rclsIP);
		m_clsLock.m_fnUnlock();
		return E_ID_PASSWORD_FAILED;
	}
	else//IP는 확인하지 않기로 요청이 들어와서 수정함. 단, config 로딩 부분은 그대로 남겨 둠.
	{
		m_clsLock.m_fnUnlock();
		return E_JOB_RES_OK;
	}

	//IP 확인.
	/*
	 아래 모든 설정 형태 상관없이 잘 동작 함.
	 IP=1.1.1.1;
	 IP=1.1.1.1
	 IP=1.1.1.1;2.2.2.2;3.3.3.3;
	 */
	TokStrings clsResult;
	KString clsIPList = pclsAclSystem->m_clsIP;
	KString::m_fnTokenizeC((KSTR)clsIPList, ";", clsResult);

	for(unsigned int i=0;i<clsResult.m_fnGetNum();i++)
	{
		if(KString::m_fnStrCmp(clsResult[i], (KCSTR)_rclsIP) == 0)
		{
			IFLOG(E_LOG_INFO,"AclSystemTable IP OK [ID:%s, PW:%s, IP:%s]", (KCSTR)_rclsID, (KCSTR)_rclsPWD, (KCSTR)_rclsIP);
			m_clsLock.m_fnUnlock();
			return E_JOB_RES_OK;
		}
	}
	IFLOG(E_LOG_ERR,"AclSystemTable IP Fail [ID:%s, PW:%s, IP:%s]", (KCSTR)_rclsID, (KCSTR)_rclsPWD, (KCSTR)_rclsIP);
	m_clsLock.m_fnUnlock();
	return E_IP_ACCESS_FAILED;
}
void AclSystemTable::m_fnDebug(eSipUtil::KString & _rclsDebug)
{
	m_clsLock.m_fnReadLock();
	_rclsDebug<<"<ACL_SYSTEM_TABLE>\n";
	Iterator stItor;
	AclSystem * pclsEndPoint = (AclSystem*)m_listAclSystem.m_fnGetNext(stItor);
	while(pclsEndPoint)
	{
		pclsEndPoint->m_fnDebug(_rclsDebug);
		pclsEndPoint = (AclSystem*)m_listAclSystem.m_fnGetNext(stItor);
	}
	_rclsDebug<<"</ACL_SYSTEM_TABLE>\n";

	m_clsLock.m_fnUnlock();
}
void AclSystemTable::m_fnCbkAclSystemChangedFile(eSipUtil::CfgFile * _pclsObj)
{
	_pclsObj->m_fnReLoadConfig();
	AclSystemTable * pclsMy = (AclSystemTable*)_pclsObj->m_fnGetUser();
	pclsMy->m_fnLoadDB();
	KString clsDebug;pclsMy->m_fnDebug(clsDebug);
	IFLOG(E_LOG_INFO,"<----- [Change] Acl System Tables ----->\n%s", (KCSTR)clsDebug);
}
		
