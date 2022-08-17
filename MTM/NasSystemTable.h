#ifndef NAS_SYSTEM_TABLE_H
#define NAS_SYSTEM_TABLE_H
#include "StlMap.hxx"
#include "ColumFile.hxx"
#include "ConfigFile.hxx"
#include "lock.hxx"

void g_fnCreateNasSystemTable();
class NasSystem: public eSipUtil::StlObject
{
public:
	NasSystem();
	~NasSystem();
	char* m_fnDebug(eSipUtil::KString &_rclsDebug);
	NasSystem& operator=(NasSystem &_rclsSrc)
	{
		m_uiKey = _rclsSrc.m_uiKey;
		if (m_pszKey)
		{
			delete[] m_pszKey;
		}
		if (_rclsSrc.m_pszKey)
			m_pszKey = strdup(_rclsSrc.m_pszKey);
		m_clsCode = _rclsSrc.m_clsCode;
		m_clsSourceDir = _rclsSrc.m_clsSourceDir;
		return *this;
	}
	eSipUtil::KString m_clsCode;
	eSipUtil::KString m_clsSourceDir;
};

class NasSystemTable
{
public:
	NasSystemTable();
	~NasSystemTable();
	static NasSystemTable* m_fnGetInstance();
	void m_fnInit();
	void m_fnLoadCfg();
	static void m_fnCbkNasSystemChangedFile(eSipUtil::CfgFile *_pclsObj);
	void m_fnGetSourceDir(eSipUtil::KString & _rclsNasCode, eSipUtil::KString &_rclsSourceDir);
	void m_fnDebug(eSipUtil::KString &_rclsDebug);
	static NasSystemTable *m_pclsMy;
	eSipUtil::KString m_clsNasRoot;
	eSipUtil::CfgFile m_clsNasSystemFile;
	eSipUtil::StlList m_listNasSystem;
	eSipUtil::RwMutex m_clsLock;

};

#endif
