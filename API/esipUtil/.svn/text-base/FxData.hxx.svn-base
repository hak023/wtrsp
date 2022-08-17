#ifndef _FX_DATA_HXX_
#define _FX_DATA_HXX_
#include "string.hxx"
#include "StlMap.hxx"
namespace eSipUtil
{
/***************************** Flexible Package Struct **************************************/
typedef enum
{
	E_FX_NONE = 0,
	E_FX_STRING,
	E_FX_BYTES,
	E_FX_OBJECT,   //FxPks_t
	E_FX_MAX
}EFxType_t;
/******************************** Inline Functions *****************************************/
inline const char * g_fnGetStringEFxType(EFxType_t _eT)
{
	if(E_FX_STRING == _eT) return "String ";
	else if(E_FX_BYTES == _eT) return "Bytes  ";
	else if(E_FX_OBJECT == _eT) return "Object ";
	else return "Unknown";
}
inline int g_fnGetInt(char *_p)
{
	return (((unsigned int)((unsigned char)_p[0]))<<24) + (((unsigned int)((unsigned char)_p[1]))<<16)
		+(((unsigned int)((unsigned char)_p[2]))<<8) + ((unsigned int)((unsigned char)_p[3]));
}
inline void g_fnSetInt(int _nVal,char *_p)
{
	_p[0] = (char)((_nVal >> 24) & 0xff);_p[1] = (char)((_nVal >> 16) & 0xff);
	_p[2] = (char)((_nVal >> 8) & 0xff);_p[3] = (char)((_nVal) & 0xff);
	return;
}
inline int g_fnGetShort(char *_p)
{
	return (((unsigned int)((unsigned char)_p[0]))<<8) + ((unsigned int)((unsigned char)_p[1]));
}
inline void g_fnSetShort(int _nVal,char *_p)
{
	_p[0] = (char)((_nVal >> 8) & 0xff);_p[1] = (char)((_nVal) & 0xff);
	return;
}
inline void g_fnAddTab(char * _szString,unsigned int _unMax, unsigned int _unTapCnt)
{
	for(unsigned int i=0;i<_unTapCnt;i++) KString::m_fnStrnCat(_szString,_unMax,"  ");
}
inline const char * g_fnBuildHex(KString &_rclsBuff,void * _pvData, unsigned int _unLen, unsigned int _unTapCnt)
{
	unsigned int i=0;unsigned int j=0;unsigned int k=0;
	char * pszBody =(char*)_pvData;
	for(unsigned int i=0;i<_unTapCnt;i++) _rclsBuff<<"  ";
	for(j=0,i=0;i<_unLen;i++)
	{
		j = i+1;k = j%32;
		if((k == 0) && (j != 1))
		{
			_rclsBuff.m_fnHexCat((unsigned char)(*(&pszBody[i]))); _rclsBuff<<"\r\n";
			for(unsigned int i=0;i<_unTapCnt;i++) _rclsBuff<<"  ";
		}
		else
		{
			_rclsBuff.m_fnHexCat((unsigned char)(*(&pszBody[i]))); _rclsBuff<<" ";
		}
	}
	_rclsBuff<<"\r\n";return (KCSTR)_rclsBuff;
}
/******************************** Variable Element ****************************************/
void g_fnObjectDebug(char * _pszDebug,unsigned int _unSize, unsigned int _unDepth, void * _pvObject);
typedef struct FxPk_t
{
	char m_usLen[2];     // Total len
	char m_usSubKeyPos[2];
	char m_usValPos[2];
	char m_usValLen[2];
	char m_uiType[4];   // EFxType_t, (string or bytes or object)
	enum{E_PK_HDR_SZ = 12};
	//========================> public Functions <==========================================
	//========================> Param Get/Set Functions
	unsigned int m_fnGetSize(){return E_PK_HDR_SZ + m_fnGetLen();}  // total pk size
	unsigned int m_fnGetLen(){return g_fnGetShort(&m_usLen[0]);}   // body size
	unsigned int m_fnGetValLen(){return g_fnGetShort(&m_usValLen[0]);} // contents len
	EFxType_t m_fnGetType(){return (EFxType_t)g_fnGetInt(&m_uiType[0]);}
	//========================> Get String Functions
	char *m_fnGetKey(){return (char*)(this+1);}
	char *m_fnGetSubKey(){return &(m_fnGetKey()[m_fnGetSubKeyPos()]);}
	char *m_fnGetVal(){return &(m_fnGetKey()[m_fnGetValPos()]);}
	void *m_fnGetVVal(){return &(m_fnGetKey()[m_fnGetValPos()]);}
	//========================> Get Debug String Function
	void m_fnDebug(char * _pszDebug,unsigned int _unSize, unsigned int _unDepth=0)
	{
		if(m_fnGetValLen())
		{
			g_fnAddTab(_pszDebug,_unSize,_unDepth);
			KString::m_fnStrnCat(_pszDebug,_unSize,"Key[%-15s:%-15s] ",
													m_fnGetKey(), m_fnGetSubKey());
			if(m_fnGetType() == E_FX_STRING)
			{
				KString::m_fnStrnCat(_pszDebug,_unSize,"Str Val [%s]\r\n",m_fnGetVal());
			}
			else if(m_fnGetType() == E_FX_BYTES)
			{
				KString::m_fnStrnCat(_pszDebug,_unSize,"Hex Val\r\n");
				g_fnAddTab(_pszDebug,_unSize,_unDepth);
				KString::m_fnStrnCat(_pszDebug,_unSize,"[\r\n");
				KString clsBytes(m_fnGetValLen()*4);
				g_fnBuildHex(clsBytes,m_fnGetVVal(),m_fnGetValLen(),_unDepth+1);
				KString::m_fnStrnCat(_pszDebug,_unSize,(KCSTR)clsBytes);
				g_fnAddTab(_pszDebug,_unSize,_unDepth);
				KString::m_fnStrnCat(_pszDebug,_unSize,"]\r\n");
			}
			else if(m_fnGetType() == E_FX_OBJECT)
			{
				KString::m_fnStrnCat(_pszDebug,_unSize,"Obj Val\r\n");
				g_fnObjectDebug(_pszDebug, _unSize,_unDepth,m_fnGetVVal());
			}
			else
			{
				KString::m_fnStrnCat(_pszDebug,_unSize,"Hex Val\r\n");
				g_fnAddTab(_pszDebug,_unSize,_unDepth);
				KString::m_fnStrnCat(_pszDebug,_unSize,"[\r\n");
				KString clsBytes(m_fnGetValLen()*4);
				g_fnBuildHex(clsBytes,m_fnGetVVal(),m_fnGetValLen(),_unDepth+1);
				KString::m_fnStrnCat(_pszDebug,_unSize,(KCSTR)clsBytes);
				g_fnAddTab(_pszDebug,_unSize,_unDepth);
				KString::m_fnStrnCat(_pszDebug,_unSize,"]\r\n");
			}
		}
	}
	//========================> private functions <==========================================
	void m_fnSetLen(unsigned int _unSize){g_fnSetShort(_unSize,&m_usLen[0]);}
	void m_fnSetSubKeyPos(unsigned int _unSize){g_fnSetShort(_unSize,&m_usSubKeyPos[0]);}
	void m_fnSetValPos(unsigned int _unSize){g_fnSetShort(_unSize,&m_usValPos[0]);}
	void m_fnSetValLen(unsigned int _unSize){g_fnSetShort(_unSize,&m_usValLen[0]);}
	void m_fnSetType(EFxType_t _eT){g_fnSetInt((int)_eT,&m_uiType[0]);}
	unsigned int m_fnGetSubKeyPos(){return g_fnGetShort(&m_usSubKeyPos[0]);}
	unsigned int m_fnGetValPos(){return g_fnGetShort(&m_usValPos[0]);}
	void m_fnSetKey(const char * _pszKey,const char * _pszSubKey)
	{
		m_fnAddRawData(_pszKey,KString::m_fnStrLen(_pszKey));
		m_fnSetSubKeyPos(m_fnGetLen());
		m_fnAddRawData(_pszSubKey,KString::m_fnStrLen(_pszSubKey));
		m_fnSetValPos(m_fnGetLen());
	}
	void m_fnSetData(const void *_pvData,unsigned int _unLen)
	{
		m_fnAddRawData((const char*)_pvData,_unLen); m_fnSetValLen(_unLen);
	}
	bool m_fnFind(const char * _pszKey, const char * _pszSubKey)
	{
		if(KString::m_fnStrCmp(m_fnGetKey(),(char*)_pszKey)==0)
		{
			if(_pszSubKey == NULL) return true;
			if(KString::m_fnStrCmp(m_fnGetSubKey(),(char*)_pszSubKey)==0) return true;
		}
		return false;
	}
	FxPk_t *m_fnNext()
	{
		return (FxPk_t*)(&m_fnGetKey()[m_fnGetLen()]);
	}
	void m_fnAddRawData(const char * _pvData,unsigned int _unLen)
	{
		memcpy(&(m_fnGetKey()[m_fnGetLen()]),_pvData,_unLen); unsigned int unTmpLen = m_fnGetLen();
		unTmpLen += _unLen; unTmpLen++; m_fnSetLen(unTmpLen);
	}
}FxPk_t;
/*************************** Variable Elements Structure *************************************/
typedef struct FxPks_t
{
	char m_usLen[2];
	char m_usHdrNum[2];
	char m_usBufSize[2];
	enum{E_PKS_HDR_SZ = 6, E_PKS_VAR_SZ = 10240};
	//========================> Init Function
	void m_fnInit(unsigned short _unBuffSize)
	{
		memset(&m_usLen[0],0x00,_unBuffSize);
		m_fnSetBufSize(_unBuffSize);
	}
	//========================> Param Get/Set Functions
	unsigned int m_fnGetLen(){return g_fnGetShort(&m_usLen[0]);}    // body size
	unsigned int m_fnGetSize(){return E_PKS_HDR_SZ + m_fnGetLen();}  // total pk size
	unsigned int m_fnGetHdrNum(){return g_fnGetShort(&m_usHdrNum[0]);}   // header nums
	unsigned int m_fnGetBufSize(){return g_fnGetShort(&m_usBufSize[0]);}  // local buffer size
	//========================> Flexible Value Add Functions
	FxPk_t *m_fnAddString(const char * _pszKey,const char * _pszSubKey,const char  *_pszVal)
	{
		unsigned int unKeySz = KString::m_fnStrLen(_pszKey) + KString::m_fnStrLen(_pszSubKey);
		unsigned int unValSz = KString::m_fnStrLen(_pszVal);
		unsigned int unAddSz = FxPk_t::E_PK_HDR_SZ + 3 + unKeySz + unValSz;
		if(m_fnGetSize() + unAddSz > m_fnGetBufSize()) return NULL;
		FxPk_t *pk = (FxPk_t*)(&m_fnStart()[m_fnGetLen()]);pk->m_fnSetType(E_FX_STRING);
		pk->m_fnSetKey(_pszKey,_pszSubKey);pk->m_fnSetData((void*)_pszVal,unValSz);
		unsigned int usTmp = m_fnGetLen();usTmp+=pk->m_fnGetSize();m_fnSetLen(usTmp);
		unsigned int usHdr = m_fnGetHdrNum();usHdr++;m_fnSetHdrNum(usHdr);
		return pk;
	}
	FxPk_t *m_fnAddVarString(const char * _pszKey,const char * _pszSubKey,const char * _pszCat,...)
	{
		char szBuff[E_PKS_VAR_SZ]; memset(szBuff,0x00,E_PKS_VAR_SZ);
		va_list ap;va_start(ap, _pszCat);vsnprintf(szBuff,E_PKS_VAR_SZ-1,_pszCat,ap);va_end(ap);
		unsigned int unKeySz = KString::m_fnStrLen(_pszKey) + KString::m_fnStrLen(_pszSubKey);
		unsigned int unValSz = KString::m_fnStrLen(szBuff);
		unsigned int unAddSz = FxPk_t::E_PK_HDR_SZ + 3 + unKeySz + unValSz;
		if(m_fnGetSize() + unAddSz >= m_fnGetBufSize()) {return NULL;}
		FxPk_t *pk = (FxPk_t*)(&m_fnStart()[m_fnGetLen()]);pk->m_fnSetType(E_FX_STRING);
		pk->m_fnSetKey(_pszKey,_pszSubKey);pk->m_fnSetData(szBuff,unValSz);
		unsigned int usTmp = m_fnGetLen(); usTmp+=pk->m_fnGetSize(); m_fnSetLen(usTmp);
		unsigned int usHdr = m_fnGetHdrNum();usHdr++; m_fnSetHdrNum(usHdr);
		return pk;
	}
	FxPk_t *m_fnAddBytes(const char * _pszKey,const char * _pszSubKey,void *_pvData, unsigned int _unLen)
	{
		unsigned int unKeySz = KString::m_fnStrLen(_pszKey) + KString::m_fnStrLen(_pszSubKey);
		unsigned int unAddSz = FxPk_t::E_PK_HDR_SZ + 3 + unKeySz + _unLen;
		if(m_fnGetSize() + unAddSz > m_fnGetBufSize()) return NULL;
		FxPk_t *pk = (FxPk_t*)(&m_fnStart()[m_fnGetLen()]);pk->m_fnSetType(E_FX_BYTES);
		pk->m_fnSetKey(_pszKey,_pszSubKey);pk->m_fnSetData(_pvData,_unLen);
		unsigned int usTmp = m_fnGetLen();usTmp+=pk->m_fnGetSize();m_fnSetLen(usTmp);
		unsigned int usHdr = m_fnGetHdrNum();usHdr++;m_fnSetHdrNum(usHdr);
		return pk;
	}
	FxPk_t *m_fnAddNewObject(const char * _pszKey,const char * _pszSubKey,FxPks_t * _pstFx)
	{
		unsigned int unKeySz = KString::m_fnStrLen(_pszKey) + KString::m_fnStrLen(_pszSubKey);
		unsigned int unAddSz = FxPk_t::E_PK_HDR_SZ + 3 + unKeySz + _pstFx->m_fnGetSize();
		if(m_fnGetSize() + unAddSz > m_fnGetBufSize()) return NULL;
		FxPk_t *pk = (FxPk_t*)(&m_fnStart()[m_fnGetLen()]);pk->m_fnSetType(E_FX_OBJECT);
		pk->m_fnSetKey(_pszKey,_pszSubKey);pk->m_fnSetData((void*)_pstFx,_pstFx->m_fnGetSize());
		unsigned int usTmp = m_fnGetLen();usTmp+=pk->m_fnGetSize();m_fnSetLen(usTmp);
		unsigned int usHdr = m_fnGetHdrNum();usHdr++;m_fnSetHdrNum(usHdr);
		return pk;
	}
	//========================> Flexible Value Find/Get Functions
	FxPk_t *m_fnGetIndex(unsigned short _uiIndex)
	{
		if(m_fnGetHdrNum() ==0) return NULL;
		FxPk_t *pk = m_fnBegin();
		for(unsigned int i=0;i<m_fnGetHdrNum();i++)
		{
			if(i == _uiIndex) return pk;
			pk  = pk->m_fnNext();
		}
		return NULL;
	}
	char * m_fnFindString(const char * _pszKey, const char * _pszSubKey)
	{
		FxPk_t *pk = m_fnFind(_pszKey,_pszSubKey);
		if(pk && pk->m_fnGetType() == E_FX_STRING)
		{
			return pk->m_fnGetVal();
		}
		return NULL;
	}
	void * m_fnFindBytes(const char * _pszKey, const char * _pszSubKey)
	{
		FxPk_t *pk = m_fnFind(_pszKey,_pszSubKey);
		if(pk && pk->m_fnGetType() == E_FX_BYTES)
		{
			return pk->m_fnGetVVal();
		}
		return NULL;
	}
	FxPks_t * m_fnFindObject(const char * _pszKey, const char * _pszSubKey)
	{
		FxPk_t *pk = m_fnFind(_pszKey,_pszSubKey);
		if(pk && pk->m_fnGetType() == E_FX_OBJECT)
		{
			return (FxPks_t *)pk->m_fnGetVVal();
		}
		return NULL;
	}
	FxPk_t *m_fnFind(const char *_pszKey, const char * _pszSubKey)
	{
		if(m_fnGetHdrNum() ==0) return NULL;
		FxPk_t *pk = m_fnBegin();
		for(unsigned int i=0;i<m_fnGetHdrNum();i++)
		{
			if(pk->m_fnFind(_pszKey,_pszSubKey)) return pk;
			pk  = pk->m_fnNext();
		}
		return NULL;
	}
	//========================> Iterator Functions
	FxPk_t * m_fnBegin(){return (FxPk_t*)(this+1);}
	FxPk_t * m_fnNext(FxPk_t * _pstPrev){return _pstPrev->m_fnNext();}
	//========================> Get Debug String
	char * m_fnDebug(char * _pszDebug,unsigned int _unLen, unsigned int _unDepth=0)
	{
		if(m_fnGetHdrNum() ==0) return _pszDebug;
		if(_unDepth == 0) memset(_pszDebug,0x00,_unLen);
		g_fnAddTab(_pszDebug,_unLen,_unDepth);
		KString::m_fnStrnCat(_pszDebug,_unLen,"{\r\n");
		g_fnAddTab(_pszDebug,_unLen,_unDepth+1);
		KString::m_fnStrnCat(_pszDebug,_unLen,"<Object: StructSize=%u,ElementNum=%u>\r\n",
											m_fnGetSize(),m_fnGetHdrNum());
		FxPk_t *pk = m_fnBegin();
		for(unsigned int i=0;i<m_fnGetHdrNum();i++)
		{
			pk->m_fnDebug(_pszDebug,_unLen,_unDepth+1);
			pk  = pk->m_fnNext();
		}
		g_fnAddTab(_pszDebug,_unLen,_unDepth);
		KString::m_fnStrnCat(_pszDebug,_unLen,"}\r\n");
		return _pszDebug;
	}
	//========================> private functions <==========================================
	void m_fnClear(){m_fnSetLen(E_PKS_HDR_SZ); m_fnSetHdrNum(0);}
	void m_fnSetLen(unsigned int _unLen){g_fnSetShort(_unLen,&m_usLen[0]);}
	void m_fnSetHdrNum(unsigned int _unHdrNum){g_fnSetShort(_unHdrNum,&m_usHdrNum[0]);}
	void m_fnSetBufSize(unsigned int _unSize){g_fnSetShort(_unSize,&m_usBufSize[0]);}
	char *m_fnStart(){return (char*)(this+1);}
}FxPks_t;
inline void  g_fnObjectDebug(char * _pszDebug,unsigned int _unSize, unsigned int _unDepth, void * _pvObject)
{
	FxPks_t * pstPk = (FxPks_t*)_pvObject;
	pstPk->m_fnDebug(_pszDebug,_unSize,_unDepth);
}
}
#endif