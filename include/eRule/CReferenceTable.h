#ifndef CREFERENCE_TABLE_H
#define CREFERENCE_TABLE_H
#include "CRuleUtiles.h"
typedef enum
{
	E_SRV_CFG_CATEGORY_NONE = 0,
	E_SRV_CFG_CATEGORY_RULE,
	E_SRV_CFG_CATEGORY_ROP,
	E_SRV_CFG_CATEGORY_FOP,
	E_SRV_CFG_CATEGORY_FOPS,
	E_SRV_CFG_CATEGORY_FACT,
	E_SRV_CFG_CATEGORY_SOP,
	E_SRV_CFG_CATEGORY_SOPS,
	E_SRV_CFG_CATEGORY_SACT,
}ESrvCfgCategory_t;
const char * g_fnStringReference(ESrvCfgCategory_t _eEv);
class Reference : public RUObject
{
	public:
		Reference(){m_unReferID=0;m_eReferT=E_SRV_CFG_CATEGORY_NONE;}
		~Reference(){}
		void fnDebug(RUString & _rclsDebug)
		{
			_rclsDebug<<",("<<RUIntString(m_unReferID)<<","<<g_fnStringReference( m_eReferT)<<")";
		}
		unsigned int m_unReferID;
		ESrvCfgCategory_t m_eReferT;
};
class CfgReference : public RUObject
{
	public:
		CfgReference();
		~CfgReference();
		void fnAddReference(unsigned int _unReferID, ESrvCfgCategory_t _eReferT);
		void fnDeleteReference(unsigned int _unReferID, ESrvCfgCategory_t _eReferT);
		Reference * fnBegin(){return (Reference*)m_listReference.fnBegin();}
		Reference * fnNext(){return (Reference*)m_listReference.fnNext();}
		unsigned int fnGetNum(){return m_listReference.fnGetSize();}
		void fnDebug(RUString & _rclsDebug);
		unsigned int m_unID;
		ESrvCfgCategory_t m_eT;
		
	private:
		RUList m_listReference;
};
class ReferenceTable
{
	public:
		ReferenceTable();
		~ReferenceTable();
		static ReferenceTable * fnGetInstance();
		static void fnFinalize();
		void fnAddReference(unsigned int _unMyID,ESrvCfgCategory_t _eMyT,
									unsigned int _unReferID,ESrvCfgCategory_t _eReferT);
		void fnDeleteReference(unsigned int _unMyID,ESrvCfgCategory_t _eMyT,
									unsigned int _unReferID,ESrvCfgCategory_t _eReferT);
		bool fnIsUsingReference(unsigned int _unMyID,ESrvCfgCategory_t _eMyT);
		static ReferenceTable * g_clsReferenceTable;
	private:
		RUMap m_mapReference;
};
#endif
