#ifndef _KPACKET_HXX_
#define _KPACKET_HXX_
#include "FxData.hxx"
namespace eSipUtil
{
/*********************************** K Packet API ****************************************/
class KPacket : public StlObject
{
	public:
		KPacket();
		~KPacket();
		KPacket & operator=(KPacket & _rclsSrc);
		//==========> Add Element
		void m_fnAddString(const char * _pszKey, const char * _pszSubKey,const char * _pszVal);
		void m_fnAddVarString(const char * _pszKey, const char * _pszSubKey,const char * _pszCat,...);
		void m_fnAddBytes(const char * _pszKey, const char * _pszSubKey,void * _pvBytes, unsigned int _unLen);
		KPacket * m_fnAddNewObject(const char * _pszKey, const char * _pszSubKey);
		//==========> Find Element
		const char * m_fnFindString(const char * _pszKey, const char * _pszSubKey);
		const void * m_fnFindByte(const char * _pszKey,const char * _pszSubKey);
		const FxPks_t * m_fnFindObject(const char * _pszKey,const char * _pszSubKey);
		//==========> Handle Packet Data
		FxPks_t * m_fnGetPacket();
		FxPks_t * m_fnAutoPtr();
		//==========> Debug
		const char * m_fnDebug(char * _pszDebug,unsigned int _unLen);
	private:
		void * m_fnFind(EFxType_t _eT,const char * _pszKey, const char * _pszSubKey);
		void m_fnReAlloc(unsigned int _unAddSz);
		void m_fnClear();
		void m_fnAddObjectInternal(const char * _pszKey, const char * _pszSubKey,FxPks_t * _pstPk);
		void m_fnBuild();
		FxPks_t * m_pstPk;
		StlList m_listChild;
		KString m_clsChildKey;
		KString m_clsChildSubKey;
		bool m_bBuild;
};
void g_fnKPacketTest();   // test sample code
}
#endif
