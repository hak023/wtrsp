
#ifndef ESIPUTIL_FLEX_IF_H
#define ESIPUTIL_FLEX_IF_H

#include <stdarg.h>

#include "string.hxx"

namespace eSipUtil
{

class FlexIf
{
	public :
		FlexIf();	//for decode
		FlexIf(const char * _pszCategory1, ...);	//for encode
		virtual ~FlexIf();

		static unsigned int m_fnGetMinMsgLen();
		//return value : -1(invalid msg), 0(unknown yet), >0(length of msg)
		static int m_fnGetBuildMsgLen(const void * pMsg, unsigned int _unMsgLen);
 
		bool m_fnAddItem(const char * pszCategory, const char * _pszKey, KString _clsValue);
		bool m_fnAddItem(const char * pszCategory, const char * _pszKey, const void * _pValue, unsigned int _unValueLen);
		CmpString m_fnGetNextItem(const char * pszCategory, const char * _pszKey) const;
		//_unPos : in-out argument. In(position for getting Item. 0 is first Item), Out(postion to get next Item. 0 is no Item)
		CmpString m_fnGetNextItem(const char * pszCategory, const char * _pszKey, unsigned int & _runPos) const;
		//_runValueLen : out argument. length of returned value
		const void * m_fnGetNextItem(const char * pszCategory, const char * _pszKey, unsigned int & _runValueLen, unsigned int & _runPos) const;
		bool m_fnGetNextAnyItem(const char * pszCategory, const char *& _rpszKey, const char *& _rpszValue, unsigned int & _runPos) const;
		bool m_fnGetNextAnyItem(const char * pszCategory, const char *& _rpszKey, const void *& _rpValue, unsigned int & _runValueLen, unsigned int & _runPos) const;

		//return value : returned value is pointer of build msg. 
		//ownership of _pMsg changes from object to invoker. invoker has responsibility for free(msg).
		void * m_fnGetBuildMsg(unsigned int & _runMsgLen);
		//_pMsg must be created by malloc(). 
		//If result is success, ownership of _pMsg changes from invoker to object. Therefore invoker must not control _pMsg after invoking this function.
		//If result is fail, ownership of _pMsg doesn't change. Therefore invoker has responsibility for free(msg).
		//_unMsgLen is real length of _pMsg
		bool m_fnSetBuildMsg(void * _pMsg, unsigned int _unMsgLen, bool _bSkipChkValid=false);
		const char * m_fnGetSuccPacketStr(char * _pszStr, unsigned int _unMaxStrLen) const;
		static const char * m_fnGetFailPacketStr(const void * _pFailPacket, unsigned int _unPacketLen, char * _pszStr, unsigned int _unMaxStrLen);
		static const char * m_fnGetDebugStr(const char * _pPacket, unsigned int _unPacketLen, char * _pszStr, unsigned int _unMaxStrLen, bool _bInit=true);
		const char * m_fnGetDebugStr(char * _pszStr, unsigned int _unMaxStrLen, bool _bInit=true) const;
      void m_fnReset(bool _bFree); // cwkim
	private :
		bool m_fnValid() const;
		
		void * m_pMsg;
		unsigned int m_unMsgLen;
		unsigned int m_unMaxMsgLen;
		bool m_bValid;
		unsigned int m_unCategoryNum;
		//bool m_bSetMsg;

};

}

#endif
