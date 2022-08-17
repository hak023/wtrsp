#ifndef ACL_SYSTEM_TABLE_H
#define ACL_SYSTEM_TABLE_H
#include "StlMap.hxx"
#include "ColumFile.hxx"
#include "ConfigFile.hxx"
#include "lock.hxx"
#include "AppIf.h"

void g_fnCreateAclSystemTable();

class AclSystem : public eSipUtil::StlObject
{
	public:
		AclSystem();
		~AclSystem();
		char * m_fnDebug(eSipUtil::KString & _rclsDebug);
		AclSystem & operator=(AclSystem & _rclsSrc)
		{
			m_uiKey = _rclsSrc.m_uiKey;
			if(m_pszKey) {delete [] m_pszKey;}
			if(_rclsSrc.m_pszKey) m_pszKey = strdup(_rclsSrc.m_pszKey);
			m_clsIP = _rclsSrc.m_clsIP;
			m_clsID = _rclsSrc.m_clsID;
			m_clsPWD = _rclsSrc.m_clsPWD;
			return * this;
		}
		eSipUtil::KString m_clsIP;
		eSipUtil::KString m_clsID;
		eSipUtil::KString m_clsPWD;
};

class AclSystemTable
{
	public:
		AclSystemTable();
		~AclSystemTable();
		static AclSystemTable * m_fnGetInstance();
		void m_fnInit();
		void m_fnLoadDB();
		static void m_fnCbkAclSystemChangedFile(eSipUtil::CfgFile * _pclsObj);
		ETrsgCodeSet_t m_fnCheckLogin(eSipUtil::KString & _rclsID, eSipUtil::KString & _rclsPWD, eSipUtil::KString & _rclsIP);
		void m_fnDebug(eSipUtil::KString & _rclsDebug);
		static AclSystemTable * m_pclsMy;
		eSipUtil::CfgFile m_clsAclSystemFile;
		eSipUtil::StlList m_listAclSystem;
		eSipUtil::RwMutex m_clsLock;
		
};

#endif
