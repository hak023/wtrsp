
#ifndef SIP_IF_MSG_H
#define SIP_IF_MSG_H

#include <stdarg.h>
#include <string.h>
#include <list>
#include <string>

#include "common_msg.h"

#define FX_IN
#define FX_OUT
#define FX_IN_OUT

#define FX_MAX_FLEX_DATA_LEN 20000		//Length of MaxFlexData must not exceed 65535
#define FX_MAX_IP_LEN 64

typedef std::list< std::pair<std::string, std::string> > list_FlexData;		//key, value

typedef enum
{
	E_SIPIF_FX_MSG_initReq = 0,
	E_SIPIF_FX_MSG_initRsp,
	E_SIPIF_FX_MSG_pingReq,
	E_SIPIF_FX_MSG_pingRsp,
	E_SIPIF_FX_MSG_sip,
	E_SIPIF_FX_MSG_internalTerm,

	E_SIPIF_FX_MSG_NUM,
}ESipIfFxMsgType_t;

typedef enum
{
	E_SIPIF_FX_RSP_succ = 0,
	E_SIPIF_FX_RSP_fail_invalid_compId,
	E_SIPIF_FX_RSP_fail_invalid_appId,
	E_SIPIF_FX_RSP_fail_unknown_tuple,
	E_SIPIF_FX_RSP_fail_invalid_status,
	E_SIPIF_FX_RSP_fail_internal_err,
	
	E_SIPIF_FX_RSP_NUM,
}ESipIfFxRspResult_t;

typedef enum
{
	E_SIPIF_FX_SIP_CONTENT_empty= 0,
	E_SIPIF_FX_SIP_CONTENT_app_sdp,
	E_SIPIF_FX_SIP_CONTENT_app_dtmp_relay,
	E_SIPIF_FX_SIP_CONTENT_text,
	E_SIPIF_FX_SIP_CONTENT_other,

	E_SIPIF_FX_SIP_CONTENT_NUM,
}ESipIfFxSipContentType_t;

typedef enum
{
	E_SIPIF_FX_SIP_DIR_incoming = 0,
	E_SIPIF_FX_SIP_DIR_outgoing,

	E_SIPIF_FX_SIP_DIR_NUM,
}ESipIfFxSipDir_t;

typedef enum
{
	E_SIPIF_FX_SIP_SESS_invite = 0,
	E_SIPIF_FX_SIP_SESS_regi,
	E_SIPIF_FX_SIP_SESS_sub,
	E_SIPIF_FX_SIP_SESS_standalone,

	E_SIPIF_FX_SIP_SESS_NUM,
}ESipIfFxSipSessType_t;

typedef enum
{
	E_SIPIF_FX_SIP_DIALOG_in = 0,
	E_SIPIF_FX_SIP_DIALOG_out,

	E_SIPIF_FX_SIP_DIALOG_NUM,
}ESipIfFxSipDialogType_t;

typedef enum 
{
	E_SIPIF_FX_TRSP_unknown = 0,
	E_SIPIF_FX_TRSP_tls,
	E_SIPIF_FX_TRSP_tcp,
	E_SIPIF_FX_TRSP_udp,
	E_SIPIF_FX_TRSP_sctp,
	E_SIPIF_FX_TRSP_dccp,
	E_SIPIF_FX_TRSP_dtls,

	E_SIPIF_FX_TRSP_NUM,
} ESipIfFxTransportType_t;

typedef enum
{
	E_SIPIF_FX_SIP_METHOD_UNKNOWN = 0,
	E_SIPIF_FX_SIP_METHOD_ACK,
	E_SIPIF_FX_SIP_METHOD_BYE,
	E_SIPIF_FX_SIP_METHOD_CANCEL,
	E_SIPIF_FX_SIP_METHOD_INVITE,
	E_SIPIF_FX_SIP_METHOD_NOTIFY,
	E_SIPIF_FX_SIP_METHOD_OPTIONS,
	E_SIPIF_FX_SIP_METHOD_REFER,
	E_SIPIF_FX_SIP_METHOD_REGISTER,
	E_SIPIF_FX_SIP_METHOD_SUBSCRIBE,
	E_SIPIF_FX_SIP_METHOD_RESPONSE,			//not use
	E_SIPIF_FX_SIP_METHOD_MESSAGE,
	E_SIPIF_FX_SIP_METHOD_INFO,
	E_SIPIF_FX_SIP_METHOD_PRACK,
	E_SIPIF_FX_SIP_METHOD_PUBLISH,
	E_SIPIF_FX_SIP_METHOD_SERVICE,
	E_SIPIF_FX_SIP_METHOD_UPDATE,

	E_SIPIF_FX_SIP_METHOD_NUM,
}ESipIfFxSipMethod_t;

inline const char * g_fnGetSipIfFxMsgTypeStr(unsigned int _n)
{
	switch(_n)
	{
		case E_SIPIF_FX_MSG_initReq: return "InitReq";
		case E_SIPIF_FX_MSG_initRsp: return "InitRsp";
		case E_SIPIF_FX_MSG_pingReq: return "PingReq";
		case E_SIPIF_FX_MSG_pingRsp: return "PingRsp";
		case E_SIPIF_FX_MSG_sip: return "SIP";
		case E_SIPIF_FX_MSG_internalTerm : return "InternalTerm";
		default: return "";
	};
	
	return "";
}

inline const char * g_fnGetSipIfFxRspResultStr(unsigned int _n)
{
	switch(_n)
	{
		case E_SIPIF_FX_RSP_succ: return "succ";
		case E_SIPIF_FX_RSP_fail_invalid_compId: return "fail(invalid compId)";
		case E_SIPIF_FX_RSP_fail_invalid_appId: return "fail(invalid appId)";
		case E_SIPIF_FX_RSP_fail_unknown_tuple: return "fail(unknown tuple)";
		case E_SIPIF_FX_RSP_fail_invalid_status: return "fail(invalid status)";
		case E_SIPIF_FX_RSP_fail_internal_err: return "fail(internal err)";
		default: return "";
	};
	
	return "";
}

inline const char * g_fnGetSipIfFxSipContentTypeStr(unsigned int _n)
{
	switch(_n)
	{
		case E_SIPIF_FX_SIP_CONTENT_empty: return "";
		case E_SIPIF_FX_SIP_CONTENT_app_sdp: return "application/sdp";
		case E_SIPIF_FX_SIP_CONTENT_app_dtmp_relay: return "application/dtmf-relay";
		case E_SIPIF_FX_SIP_CONTENT_text: return "text/plain";
		case E_SIPIF_FX_SIP_CONTENT_other: return "";
		default: return "";
	};
	
	return "";
}

inline ESipIfFxSipContentType_t g_fnGetSipIfFxSipContentTypeEnum(const char * _pszContentType)
{
	if( _pszContentType && strlen(_pszContentType) > 0 )
	{
		if( strcasecmp(_pszContentType, "application/sdp") == 0 )
			return E_SIPIF_FX_SIP_CONTENT_app_sdp;
		else if( strcasecmp(_pszContentType, "application/dtmf-relay") == 0 )
			return E_SIPIF_FX_SIP_CONTENT_app_dtmp_relay;
		else if( strcasecmp(_pszContentType, "text/plain") == 0 )
			return E_SIPIF_FX_SIP_CONTENT_text;
		else
			return E_SIPIF_FX_SIP_CONTENT_other;
	}

	return E_SIPIF_FX_SIP_CONTENT_empty;
}

inline const char * g_fnGetSipIfFxSipDirStr(unsigned int _n)
{
	switch(_n)
	{
		case E_SIPIF_FX_SIP_DIR_incoming: return "sip_incoming";
		case E_SIPIF_FX_SIP_DIR_outgoing: return "sip_outgoing";
		default: return "";
	};
	
	return "";
}

inline const char * g_fnGetSipIfFxSipSessTypeStr(unsigned int _n)
{
	switch(_n)
	{
		case E_SIPIF_FX_SIP_SESS_invite: return "InvSess";
		case E_SIPIF_FX_SIP_SESS_regi: return "RegiSess";
		case E_SIPIF_FX_SIP_SESS_sub: return "SubSess";
		case E_SIPIF_FX_SIP_SESS_standalone: return "StandSess";
		default: return "";
	};
	
	return "";
}

inline const char * g_fnGetSipIfFxSipDialogTypeStr(unsigned int _n)
{
	switch(_n)
	{
		case E_SIPIF_FX_SIP_DIALOG_in: return "in Dialog";
		case E_SIPIF_FX_SIP_DIALOG_out: return "outof Dialog";
		default: return "";
	};
	
	return "";
}

inline const char * g_fnGetSipIfFxTrspType(unsigned int _n)
{
	switch(_n)
	{
		case E_SIPIF_FX_TRSP_unknown : return "";
		case E_SIPIF_FX_TRSP_tls : return "TLS";
		case E_SIPIF_FX_TRSP_tcp : return "TCP";
		case E_SIPIF_FX_TRSP_udp : return "UDP";
		case E_SIPIF_FX_TRSP_sctp : return "SCTP";
		case E_SIPIF_FX_TRSP_dccp : return "DCCP";
		case E_SIPIF_FX_TRSP_dtls : return "DTLS";
		default: return "";
	};
	
	return "";
}

inline const char * g_fnGetSipIfFxSipMethodStr(unsigned int _n)
{
	switch(_n)
	{
		case E_SIPIF_FX_SIP_METHOD_UNKNOWN : return "";
		case E_SIPIF_FX_SIP_METHOD_ACK : return "ACK";
		case E_SIPIF_FX_SIP_METHOD_BYE : return "BYE";
		case E_SIPIF_FX_SIP_METHOD_CANCEL : return "CANCEL";
		case E_SIPIF_FX_SIP_METHOD_INVITE : return "INVITE";
		case E_SIPIF_FX_SIP_METHOD_NOTIFY : return "NOTIFY";
		case E_SIPIF_FX_SIP_METHOD_OPTIONS : return "OPTIONS";
		case E_SIPIF_FX_SIP_METHOD_REFER : return "REFER";
		case E_SIPIF_FX_SIP_METHOD_REGISTER : return "REGISTER";
		case E_SIPIF_FX_SIP_METHOD_SUBSCRIBE : return "SUBSCRIBE";
		case E_SIPIF_FX_SIP_METHOD_RESPONSE : return "";
		case E_SIPIF_FX_SIP_METHOD_MESSAGE : return "MESSAGE";
		case E_SIPIF_FX_SIP_METHOD_INFO : return "INFO";
		case E_SIPIF_FX_SIP_METHOD_PRACK : return "PRACK";
		case E_SIPIF_FX_SIP_METHOD_PUBLISH : return "PUBLISH";
		case E_SIPIF_FX_SIP_METHOD_SERVICE : return "SERVICE";
		case E_SIPIF_FX_SIP_METHOD_UPDATE : return "UPDATE";
		default: return "";
	};
	
	return "";
}

inline char * sipif_fx_strncat(char * _pszSrc, unsigned int _unMaxSrcLen, const char * _pszFormat, ...)
{
	if( !_pszSrc || _unMaxSrcLen == 0 )
		return _pszSrc;
	
	unsigned int unCurrentLen = strlen(_pszSrc);
	if( unCurrentLen >= _unMaxSrcLen -1 )
	{
		_pszSrc[_unMaxSrcLen-1] = 0x00;
	}
	else
	{
		char * pszTemp = &(_pszSrc[unCurrentLen]);
		va_list ap;
		va_start(ap, _pszFormat);
		vsnprintf(pszTemp, _unMaxSrcLen-unCurrentLen-1,_pszFormat, ap);
		va_end(ap);
	}
	
	return _pszSrc;
}

typedef struct
{
	unsigned char m_ucData[2];
	
	void m_fnSet(unsigned short _us)
	{
		m_ucData[0] = (unsigned char)(_us >> 8);
  		m_ucData[1] = (unsigned char)(_us);
	}

	unsigned short m_fnGet() const{ return (unsigned short)(m_ucData[0] << 8) + (unsigned short)(m_ucData[1]); }

	void m_fnAdd(unsigned short _us){ m_fnSet(m_fnGet()+_us); }

	void m_fnReset()
	{
		m_ucData[0] = 0;
		m_ucData[1] = 0;
	}
	
}USHORT_t;

typedef struct
{
	unsigned char m_ucData[4];

	void m_fnSet(unsigned int _ui)
	{
		m_ucData[0] = (unsigned char)(_ui >> 24);
  		m_ucData[1] = (unsigned char)(_ui >> 16);
		m_ucData[2] = (unsigned char)(_ui >> 8);
		m_ucData[3] = (unsigned char)(_ui);
	}

	unsigned int m_fnGet() const
	{
		return (unsigned int)(m_ucData[0] << 24) + (unsigned int)(m_ucData[1] << 16) + (unsigned int)(m_ucData[2] << 8) + (unsigned int)(m_ucData[3]);
	}

	void m_fnReset()
	{
		m_ucData[0] = 0;
		m_ucData[1] = 0;
		m_ucData[2] = 0;
		m_ucData[3] = 0;
	}
	
}UINT_t;

struct FxKeyValueListEntity
{
	USHORT_t m_stKeyLength;
	USHORT_t m_stValueLength;
	USHORT_t m_stNextPos;

	unsigned short m_fnGetKeyLen() const{ return m_stKeyLength.m_fnGet(); }
	static unsigned short m_fnGetKeyOffset(){ return sizeof(struct FxKeyValueListEntity); }
	unsigned short m_fnGetValueLen() const{ return m_stValueLength.m_fnGet(); }
	unsigned short m_fnGetValueOffset() const{ return m_fnGetKeyOffset()+m_fnGetKeyLen()+1; }
	unsigned short m_fnGetEntityTotalLen() const{ return m_fnGetValueOffset()+m_stValueLength.m_fnGet()+1;}

	void m_fnReset()
	{
		m_stKeyLength.m_fnReset();
		m_stValueLength.m_fnReset();
		m_stNextPos.m_fnReset();
	}

	bool m_fnValid(const char * _pEntityMem, unsigned int _unRemainLen, char * _pszErr=NULL, unsigned int _unMaxErrLen=0) const
	{
		if( !_pEntityMem || _unRemainLen == 0 )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, "KeyValueListEntity : invalid : EntityMem[%p], RemainLen[%u]", _pEntityMem, _unRemainLen);
			return false;
		}

		if( m_fnGetKeyLen() == 0 || m_fnGetEntityTotalLen() > _unRemainLen )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, "KeyValueListEntity : invalid : KeyLen[%d], EntityTotalLen[%d], RemainLen[%u]", 
																				m_fnGetKeyLen(), m_fnGetEntityTotalLen(), _unRemainLen);
			return false;
		}

		if( m_fnGetKeyLen() != strlen(_pEntityMem+m_fnGetKeyOffset()) )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, "KeyValueListEntity : invalid : Key[%s], KeyLen[%d!=%d]", 
																				_pEntityMem+m_fnGetKeyOffset(), m_fnGetKeyLen(), strlen(_pEntityMem+m_fnGetKeyOffset()));
			return false;
		}

		if( m_fnGetValueLen() != strlen(_pEntityMem+m_fnGetValueOffset()) )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, "KeyValueListEntity : invalid : Value[%s], ValueLen[%d!=%d]", 
																				_pEntityMem+m_fnGetValueOffset(), m_fnGetValueLen(), strlen(_pEntityMem+m_fnGetValueOffset()));
			return false;
		}

		return true;
	}

	bool m_fnSetData(char * _pEntityMem, unsigned int _unRemainLen, const char * _pszKey, const char * _pszValue)
	{
		if( !_pEntityMem || _unRemainLen == 0 || !_pszKey || strlen(_pszKey) == 0 )
			return false;

		unsigned short usKeyLen = (unsigned short)(strlen(_pszKey));
		unsigned short usValueLen = 0;
		if( _pszValue )
			usValueLen = (unsigned short)(strlen(_pszValue));

		m_stKeyLength.m_fnSet(usKeyLen);
		m_stValueLength.m_fnSet(usValueLen);

		if( m_fnGetEntityTotalLen() > _unRemainLen )
		{
			m_stKeyLength.m_fnReset();
			m_stValueLength.m_fnReset();
			return false;
		}

		char * pszKey = _pEntityMem+m_fnGetKeyOffset();
		memcpy(pszKey, _pszKey, usKeyLen);
		pszKey[usKeyLen] = 0x00;
		char * pszValue = _pEntityMem+m_fnGetValueOffset();
		memcpy(pszValue, _pszValue, usValueLen);
		pszValue[usValueLen] = 0x00;

		return true;
	}

	const char * m_fnGetKey(const char * _pEntityMem) const
	{
		if( !_pEntityMem )
			return NULL;

		return _pEntityMem+m_fnGetKeyOffset();
	}

	const char * m_fnGetValue(const char * _pEntityMem) const
	{
		if( !_pEntityMem )
			return NULL;
	
		return _pEntityMem+m_fnGetValueOffset();
	}
};
typedef struct FxKeyValueListEntity FxKeyValueListEntity_t;

inline bool g_fnGetAllFlexListEntity(const char * _pFlexStartMem, unsigned char _ucEntityNums, unsigned short _usFirstFlexEntityPos, list_FlexData & _rlistFlexData)
{
	if( !_pFlexStartMem )
		return false;

	if( _ucEntityNums == 0 )
		return true;

	_rlistFlexData.clear();

	unsigned char ucEntityCnt = 0;
	const FxKeyValueListEntity_t * pstKeyValueEntity = NULL;
	unsigned short _usNextFlexEntityPos = _usFirstFlexEntityPos;
	unsigned short _usPrevFlexEntityPos = 0;
	std::string clsKey, clsValue;
	while( _usNextFlexEntityPos < FX_MAX_FLEX_DATA_LEN )
	{
		if( _usNextFlexEntityPos > 0 && _usNextFlexEntityPos <= _usPrevFlexEntityPos )
		{
			_rlistFlexData.clear();
			return false;
		}
		
		if( ucEntityCnt >= _ucEntityNums )
		{
			_rlistFlexData.clear();
			return false;
		}
	
		pstKeyValueEntity = (const FxKeyValueListEntity_t *)(_pFlexStartMem+_usNextFlexEntityPos);
		if( !pstKeyValueEntity->m_fnValid((const char *)pstKeyValueEntity, FX_MAX_FLEX_DATA_LEN-_usNextFlexEntityPos) )
		{
			_rlistFlexData.clear();
			return false;
		}

		clsKey = pstKeyValueEntity->m_fnGetKey((const char *)pstKeyValueEntity);
		clsValue = pstKeyValueEntity->m_fnGetValue((const char *)pstKeyValueEntity);
		_rlistFlexData.push_back(std::make_pair(clsKey, clsValue));
		++ucEntityCnt;
		_usPrevFlexEntityPos = _usNextFlexEntityPos;
		_usNextFlexEntityPos = pstKeyValueEntity->m_stNextPos.m_fnGet();
		if( _usNextFlexEntityPos == 0 )
			break;
	}
	
	if( ucEntityCnt == _ucEntityNums )
		return true;
	else
	{
		_rlistFlexData.clear();
		return false;
	}
}

inline char * g_fnGetFlexListStr(char * _pszPrint, unsigned int _unMaxLen,
																const char * _pFlexStartMem, unsigned char _ucEntityNums, unsigned short _usFirstFlexEntityPos)
{
	if( !_pszPrint || _unMaxLen == 0 || !_pFlexStartMem || _ucEntityNums == 0 )
		return _pszPrint;

	const char * pszKey = NULL;
	const char * pszValue = NULL;
	unsigned char ucEntityCnt= 0;
	const FxKeyValueListEntity_t * pstKeyValueEntity = NULL;
	unsigned short _usNextFlexEntityPos = _usFirstFlexEntityPos;
	unsigned short _usPrevFlexEntityPos = 0;
	while( _usNextFlexEntityPos < FX_MAX_FLEX_DATA_LEN )
	{
		if( _usNextFlexEntityPos > 0 && _usNextFlexEntityPos <= _usPrevFlexEntityPos )
			break;
	
		if( ucEntityCnt >= _ucEntityNums )
			break;
	
		pstKeyValueEntity = (const FxKeyValueListEntity_t *)(_pFlexStartMem+_usNextFlexEntityPos);
		if( !pstKeyValueEntity->m_fnValid((const char *)pstKeyValueEntity, FX_MAX_FLEX_DATA_LEN-_usNextFlexEntityPos) )
			break;
		
		pszKey = pstKeyValueEntity->m_fnGetKey((const char *)pstKeyValueEntity);
		pszValue = pstKeyValueEntity->m_fnGetValue((const char *)pstKeyValueEntity);
#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "%s : %s\n", pszKey, pszValue);
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "%s(%s),", pszKey, pszValue);
#endif
		
		++ucEntityCnt;
		_usPrevFlexEntityPos = _usNextFlexEntityPos;
		_usNextFlexEntityPos = pstKeyValueEntity->m_stNextPos.m_fnGet();
		if( _usNextFlexEntityPos == 0 )
			break;
	}

	return _pszPrint;
}

inline unsigned short g_fnSetFlexListEntity(char * _pFlexStartMem, unsigned short _usCurrentFlexPos, const char * _pszKey, const char * _pszValue)
{
	if( !_pFlexStartMem || !_pszKey || strlen(_pszKey) == 0 )
		return 0;

	FxKeyValueListEntity stFxKeyValueListEntity;
	stFxKeyValueListEntity.m_fnReset();
	if( stFxKeyValueListEntity.m_fnSetData(_pFlexStartMem+_usCurrentFlexPos, FX_MAX_FLEX_DATA_LEN-_usCurrentFlexPos, _pszKey, _pszValue) )
	{
		memcpy(_pFlexStartMem+_usCurrentFlexPos, &stFxKeyValueListEntity, sizeof(FxKeyValueListEntity_t));
		return stFxKeyValueListEntity.m_fnGetEntityTotalLen();
	}
	else
		return 0;
}

inline bool g_fnExistFlexListKey(const char * _pFlexStartMem, const char * _pszKey, unsigned char _ucEntityNums, unsigned short _usFirstFlexEntityPos)
{
	if( !_pFlexStartMem || !_pszKey || strlen(_pszKey) == 0 || _ucEntityNums == 0 )
		return false;

	const char * pszKey = NULL;
	unsigned char ucEntityCnt= 0;
	const FxKeyValueListEntity_t * pstKeyValueEntity = NULL;
	unsigned short _usNextFlexEntityPos = _usFirstFlexEntityPos;
	unsigned short _usPrevFlexEntityPos = 0;
	while( _usNextFlexEntityPos < FX_MAX_FLEX_DATA_LEN )
	{
		if( _usNextFlexEntityPos > 0 && _usNextFlexEntityPos <= _usPrevFlexEntityPos )
			break;
	
		if( ucEntityCnt >= _ucEntityNums )
			break;
	
		pstKeyValueEntity = (const FxKeyValueListEntity_t *)(_pFlexStartMem+_usNextFlexEntityPos);
		if( !pstKeyValueEntity->m_fnValid((const char *)pstKeyValueEntity, FX_MAX_FLEX_DATA_LEN-_usNextFlexEntityPos) )
			break;
		
		pszKey = pstKeyValueEntity->m_fnGetKey((const char *)pstKeyValueEntity);
		if( pszKey && strcasecmp(pszKey, _pszKey) == 0 )
			return true;
		
		++ucEntityCnt;
		_usPrevFlexEntityPos = _usNextFlexEntityPos;
		_usNextFlexEntityPos = pstKeyValueEntity->m_stNextPos.m_fnGet();
		if( _usNextFlexEntityPos == 0 )
			break;
	}

	return false;
}

inline const char * g_fnFindFlexListValue(const char * _pFlexStartMem, const char * _pszKey, unsigned char _ucEntityNums, unsigned short _usFirstFlexEntityPos)
{
	if( !_pFlexStartMem || !_pszKey || strlen(_pszKey) == 0 || _ucEntityNums == 0 )
		return NULL;

	const char * pszKey = NULL;
	unsigned char ucEntityCnt= 0;
	const FxKeyValueListEntity_t * pstKeyValueEntity = NULL;
	unsigned short _usNextFlexEntityPos = _usFirstFlexEntityPos;
	unsigned short _usPrevFlexEntityPos = 0;
	while( _usNextFlexEntityPos < FX_MAX_FLEX_DATA_LEN )
	{
		if( _usNextFlexEntityPos > 0 && _usNextFlexEntityPos <= _usPrevFlexEntityPos )
			break;
	
		if( ucEntityCnt >= _ucEntityNums )
			break;
	
		pstKeyValueEntity = (const FxKeyValueListEntity_t *)(_pFlexStartMem+_usNextFlexEntityPos);
		if( !pstKeyValueEntity->m_fnValid((const char *)pstKeyValueEntity, FX_MAX_FLEX_DATA_LEN-_usNextFlexEntityPos) )
			break;
		
		pszKey = pstKeyValueEntity->m_fnGetKey((const char *)pstKeyValueEntity);
		if( pszKey && strcasecmp(pszKey, _pszKey) == 0 )
			return pstKeyValueEntity->m_fnGetValue((const char *)pstKeyValueEntity);
		
		++ucEntityCnt;
		_usPrevFlexEntityPos = _usNextFlexEntityPos;
		_usNextFlexEntityPos = pstKeyValueEntity->m_stNextPos.m_fnGet();
		if( _usNextFlexEntityPos == 0 )
			break;
	}

	return NULL;
}

inline bool g_fnValidFlexKeyValueList(const char * _pFlexStartMem, unsigned char _ucEntityNums, unsigned short _usFirstFlexEntityPos, char * _pszErr=NULL, unsigned int _unMaxErrLen=0)
{
	if( !_pFlexStartMem )
		return false;

	if( _ucEntityNums == 0 )
		return true;

	unsigned char ucEntityCnt = 0;
	const FxKeyValueListEntity_t * pstKeyValueEntity = NULL;
	unsigned short _usNextFlexEntityPos = _usFirstFlexEntityPos;
	unsigned short _usPrevFlexEntityPos = 0;
	while( _usNextFlexEntityPos < FX_MAX_FLEX_DATA_LEN )
	{
		if( _usNextFlexEntityPos > 0 && _usNextFlexEntityPos <= _usPrevFlexEntityPos )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, "ValidFlexKeyValueList : invalid : next[%d] <= prev[%d]", _usNextFlexEntityPos, _usPrevFlexEntityPos);
			return false;
		}
		
		if( ucEntityCnt >= _ucEntityNums )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, "ValidFlexKeyValueList : invalid : over entity nums : EntityCnt[%d >= %d]", ucEntityCnt, _ucEntityNums);
			return false;
		}
	
		pstKeyValueEntity = (const FxKeyValueListEntity_t *)(_pFlexStartMem+_usNextFlexEntityPos);
		if( !pstKeyValueEntity->m_fnValid((const char *)pstKeyValueEntity, FX_MAX_FLEX_DATA_LEN-_usNextFlexEntityPos, _pszErr, _unMaxErrLen) )
			return false;
		
		++ucEntityCnt;
		_usPrevFlexEntityPos = _usNextFlexEntityPos;
		_usNextFlexEntityPos = pstKeyValueEntity->m_stNextPos.m_fnGet();
		if( _usNextFlexEntityPos == 0 )
			break;
	}
	
	if( ucEntityCnt == _ucEntityNums )
		return true;
	else
	{
		sipif_fx_strncat(_pszErr, _unMaxErrLen, "ValidFlexKeyValueList : invalid : unmatch entity nums : EntityCnt[%d != %d]", ucEntityCnt, _ucEntityNums);
		return false;
	}
}

inline unsigned short g_fnSetFlexStrValue(char * _pFlexStartMem, unsigned short _usCurrentFlexPos, const char * _pValue, unsigned int _unValueLen)
{
	if( !_pFlexStartMem || !_pValue || _unValueLen == 0 )
		return 0;

	if( _usCurrentFlexPos + _unValueLen > FX_MAX_FLEX_DATA_LEN )
		return 0;

	char * pszValue = _pFlexStartMem+_usCurrentFlexPos;
	memcpy(pszValue, _pValue, _unValueLen);
	pszValue[_unValueLen] = 0x00;
	
	return (unsigned short)(_unValueLen+1);
}

inline bool g_fnValidFlexStr(const char * _pFlexStartMem, unsigned short _usFlexStrPos, unsigned short _usFlexStrLen, char * _pszErr=NULL, unsigned int _unMaxErrLen=0)
{
	if( !_pFlexStartMem )
	{
		sipif_fx_strncat(_pszErr, _unMaxErrLen, "ValidFlexStr : invalid : FlexStartMem");
		return false;
	}

	if( _usFlexStrLen == 0 )
		return true;

	if( (_usFlexStrPos+_usFlexStrLen) >= FX_MAX_FLEX_DATA_LEN )
	{
		sipif_fx_strncat(_pszErr, _unMaxErrLen, "ValidFlexStr : invalid : over len : %d >= %d", _usFlexStrPos+_usFlexStrLen, FX_MAX_FLEX_DATA_LEN);
		return false;
	}

	const char * pszStrValue = _pFlexStartMem+_usFlexStrPos;
	if( _usFlexStrLen == strlen(pszStrValue) )
		return true;
	else
	{
		sipif_fx_strncat(_pszErr, _unMaxErrLen, "ValidFlexStr : invalid : unmatch string len : %d != %d", _usFlexStrLen, strlen(pszStrValue));
		return false;
	}
}

inline const char * g_fnFindFlexStrValue(const char * _pFlexStartMem, unsigned short _usFlexStrPos, unsigned short _usFlexStrLen)
{
	if( _usFlexStrLen == 0 )
		return NULL;

	if( !g_fnValidFlexStr(_pFlexStartMem, _usFlexStrPos, _usFlexStrLen) )
		return NULL;
	
	return _pFlexStartMem+_usFlexStrPos;
}


/*================ SIP ================*/
typedef struct FxKeyValueListEntity FxSipHdrListEntity_t;

typedef struct
{
	unsigned char m_ucSessType;				//ESipIfFxSipSessType_t
	unsigned char m_ucDialogType;			//ESipIfFxSipDialogType_t
	unsigned char m_ucSipDir;					//ESipIfFxSipDir_t
	char m_szLocalIp[FX_MAX_IP_LEN];
	USHORT_t m_stLocalPort;
	char m_szRemoteIp[FX_MAX_IP_LEN];
	USHORT_t m_stRemotePort;
	unsigned char m_ucTransportType;		//ESipIfFxTransportType_t
	unsigned char m_ucSipMethod;			//ESipIfFxSipMethod_t
	USHORT_t m_stSipRsp;						//request(0), response(>0)
	unsigned char m_ucContentType;		//ESipIfFxSipContentType_t

	//request-uri info
	USHORT_t m_stReqUriFlexStrPos;
	USHORT_t m_stReqUriLen;

	//header list info
	unsigned char m_ucHdrEntityNums;
	USHORT_t m_stHdrFirstFlexEntityPos;
	USHORT_t m_stHdrLastFlexEntityPos;

	void m_fnReset()
	{
		m_ucSessType = E_SIPIF_FX_SIP_SESS_invite;
		m_ucDialogType = E_SIPIF_FX_SIP_DIALOG_in;
		m_ucSipDir = E_SIPIF_FX_SIP_DIR_incoming;
		memset(m_szLocalIp, 0x00, sizeof(m_szLocalIp));
		m_stLocalPort.m_fnReset();
		memset(m_szRemoteIp, 0x00, sizeof(m_szRemoteIp));
		m_stRemotePort.m_fnReset();
		m_ucTransportType = E_SIPIF_FX_TRSP_unknown;
		m_ucSipMethod = E_SIPIF_FX_SIP_METHOD_UNKNOWN;
		m_stSipRsp.m_fnReset();
		m_ucContentType = E_SIPIF_FX_SIP_CONTENT_empty;		
	
		m_stReqUriFlexStrPos.m_fnReset();
		m_stReqUriLen.m_fnReset();
		
		m_ucHdrEntityNums = 0;
		m_stHdrFirstFlexEntityPos.m_fnReset();
		m_stHdrLastFlexEntityPos.m_fnReset();
	}

	bool m_fnValid(const char * _pFlexStartMem, char * _pszErr=NULL, unsigned int _unMaxErrLen=0) const
	{		
		if( !g_fnValidFlexStr(_pFlexStartMem, m_stReqUriFlexStrPos.m_fnGet(), m_stReqUriLen.m_fnGet(), _pszErr, _unMaxErrLen) )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, " : ReqUri");
			return false;
		}

		if( !g_fnValidFlexKeyValueList(_pFlexStartMem, m_ucHdrEntityNums, m_stHdrFirstFlexEntityPos.m_fnGet(), _pszErr, _unMaxErrLen) )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, " : SipInfo");
			return false;
		}

		return true;
	}

	const char * m_fnGetReqUriStr(const char * _pFlexStartMem) const
	{		
		const char * pszReqUri = g_fnFindFlexStrValue(_pFlexStartMem, m_stReqUriFlexStrPos.m_fnGet(), m_stReqUriLen.m_fnGet());
		if( pszReqUri )
			return pszReqUri;
		else
			return "";
	}
	
	bool m_fnExistHdr(const char * _pFlexStartMem, const char * _pszHdrName) const
	{
		return g_fnExistFlexListKey(_pFlexStartMem, _pszHdrName, m_ucHdrEntityNums, m_stHdrFirstFlexEntityPos.m_fnGet());
	}

	bool m_fnGetAllHdr(const char * _pFlexStartMem, list_FlexData & _rlistFlexData) const
	{
		return g_fnGetAllFlexListEntity(_pFlexStartMem, m_ucHdrEntityNums, m_stHdrFirstFlexEntityPos.m_fnGet(), _rlistFlexData);
	}
	
	const char * m_fnGetHdrValue(const char * _pFlexStartMem, const char * _pszHdrName) const
	{
		const char * pszHdrValue = g_fnFindFlexListValue(_pFlexStartMem, _pszHdrName, m_ucHdrEntityNums, m_stHdrFirstFlexEntityPos.m_fnGet());
		if( pszHdrValue )
			return pszHdrValue;
		else
			return "";
	}

	unsigned short m_fnSetReqUriStr(char * _pFlexStartMem, unsigned short _usCurrentFlexPos, const char * _pszReqUri)
	{
		if( m_stReqUriLen.m_fnGet() > 0 )
			return 0;
	
		unsigned short usSetFlexLen = 0;
		if( (usSetFlexLen = g_fnSetFlexStrValue(_pFlexStartMem, _usCurrentFlexPos, _pszReqUri, strlen(_pszReqUri))) > 0 )
		{
			m_stReqUriFlexStrPos.m_fnSet(_usCurrentFlexPos);
			m_stReqUriLen.m_fnSet((unsigned short)strlen(_pszReqUri));
			return usSetFlexLen;
		}
		else
			return 0;
	}
	
	unsigned short m_fnSetHdr(char * _pFlexStartMem, unsigned short _usCurrentFlexPos, const char * _pszHdrName, const char * _pszHdrValue)
	{
		if( !_pszHdrValue || strlen(_pszHdrValue) == 0 )
			return 0;
	
		unsigned short usSetFlexLen = 0;
		if( (usSetFlexLen = g_fnSetFlexListEntity(_pFlexStartMem, _usCurrentFlexPos, _pszHdrName, _pszHdrValue)) > 0 )
		{
			if( m_ucHdrEntityNums == 0 )
			{
				m_stHdrFirstFlexEntityPos.m_fnSet(_usCurrentFlexPos);
				m_stHdrLastFlexEntityPos.m_fnSet(_usCurrentFlexPos);
			}
			else
			{
				FxSipHdrListEntity_t * pstSipHdrListEntity = (FxSipHdrListEntity_t * )(_pFlexStartMem+m_stHdrLastFlexEntityPos.m_fnGet());
				pstSipHdrListEntity->m_stNextPos.m_fnSet(_usCurrentFlexPos);
				m_stHdrLastFlexEntityPos.m_fnSet(_usCurrentFlexPos);
			}
			++m_ucHdrEntityNums;
			return usSetFlexLen;
		}
		else
			return 0;
	}

	char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen, const char * _pFlexStartMem) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;
#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "SessType[%s]\nDlgType[%s]\nSipDir[%s]\nLocalIp[%s]\nLocalPort[%d]\nIp[%s]\nPort[%d]\nTrspType[%s]\nMethod[%s]\nRspCode[%d]\nContentType[%s]\nReqUri[%s]\n",
																			g_fnGetSipIfFxSipSessTypeStr(m_ucSessType),
																			g_fnGetSipIfFxSipDialogTypeStr(m_ucDialogType),
																			g_fnGetSipIfFxSipDirStr(m_ucSipDir), 
																			m_szLocalIp, m_stLocalPort.m_fnGet(), m_szRemoteIp, m_stRemotePort.m_fnGet(), g_fnGetSipIfFxTrspType(m_ucTransportType),
																			g_fnGetSipIfFxSipMethodStr(m_ucSipMethod), 
																			m_stSipRsp.m_fnGet(), 
																			g_fnGetSipIfFxSipContentTypeStr(m_ucContentType),
																			m_fnGetReqUriStr(_pFlexStartMem));
			
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n<Header Info>\n");
		g_fnGetFlexListStr(_pszPrint, _unMaxLen, _pFlexStartMem, m_ucHdrEntityNums, m_stHdrFirstFlexEntityPos.m_fnGet());
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "SipInfo[SessType(%s),DlgType(%s),SipDir(%s),LocalIp(%s),LocalPort(%d),RemoteIp(%s),RemotePort(%d),TrspType(%s),Method(%s),RspCode(%d),ContentType(%s),ReqUri(%s),",
																			g_fnGetSipIfFxSipSessTypeStr(m_ucSessType),
																			g_fnGetSipIfFxSipDialogTypeStr(m_ucDialogType),
																			g_fnGetSipIfFxSipDirStr(m_ucSipDir), 
																			m_szLocalIp, m_stLocalPort.m_fnGet(), m_szRemoteIp, m_stRemotePort.m_fnGet(), g_fnGetSipIfFxTrspType(m_ucTransportType),
																			g_fnGetSipIfFxSipMethodStr(m_ucSipMethod), 
																			m_stSipRsp.m_fnGet(), 
																			g_fnGetSipIfFxSipContentTypeStr(m_ucContentType),
																			m_fnGetReqUriStr(_pFlexStartMem));
		g_fnGetFlexListStr(_pszPrint, _unMaxLen, _pFlexStartMem, m_ucHdrEntityNums, m_stHdrFirstFlexEntityPos.m_fnGet());
		sipif_fx_strncat(_pszPrint, _unMaxLen, "]");
#endif
		
		return _pszPrint;
	}	
}FxSipInfo_t;


/*================ SIP Content ================*/
typedef struct
{
	USHORT_t m_stSipContentFlexStrPos;					//Content string position
	USHORT_t m_stSipContentLen;

	void m_fnReset()
	{
		m_stSipContentFlexStrPos.m_fnReset();
		m_stSipContentLen.m_fnReset();
	}

	bool m_fnValid(const char * _pFlexStartMem, char * _pszErr=NULL, unsigned int _unMaxErrLen=0) const
	{
		if( !g_fnValidFlexStr(_pFlexStartMem, m_stSipContentFlexStrPos.m_fnGet(), m_stSipContentLen.m_fnGet(), _pszErr, _unMaxErrLen) )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, " : SipContent");
			return false;
		}

		return true;
	}

	const char * m_fnGetContent(const char * _pFlexStartMem) const
	{
		return g_fnFindFlexStrValue(_pFlexStartMem, m_stSipContentFlexStrPos.m_fnGet(), m_stSipContentLen.m_fnGet());
	}
	
	unsigned short m_fnSetContent(char * _pFlexStartMem, unsigned short _usCurrentFlexPos, const char * _pSipContent, unsigned int _unContentLen)
	{
		if( m_stSipContentLen.m_fnGet() > 0 )
			return 0;
	
		unsigned short usSetFlexLen = 0;
		if( (usSetFlexLen = g_fnSetFlexStrValue(_pFlexStartMem, _usCurrentFlexPos, _pSipContent, _unContentLen)) > 0 )
		{
			m_stSipContentFlexStrPos.m_fnSet(_usCurrentFlexPos);
			m_stSipContentLen.m_fnSet((unsigned short)_unContentLen);
			return usSetFlexLen;
		}
		else
			return 0;
	}

	char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen, const char * _pFlexStartMem) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;
#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "<Content Info>\n" \
																	"%s\n",
																	m_fnGetContent(_pFlexStartMem));
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "ContentInfo[%s]", m_fnGetContent(_pFlexStartMem));
#endif
		return _pszPrint;
	}
}FxSipContentInfo_t;


/*================ Custom ================*/
typedef struct FxKeyValueListEntity FxCustomListEntity_t;

typedef struct
{
	//Custom list info
	unsigned char m_ucCustomNums;
	USHORT_t m_stCustomFirstFlexEntityPos;
	USHORT_t m_stCustomLastFlexEntityPos;

	void m_fnReset()
	{
		m_ucCustomNums = 0;
		m_stCustomFirstFlexEntityPos.m_fnReset();
		m_stCustomLastFlexEntityPos.m_fnReset();
	}

	bool m_fnValid(const char * _pFlexStartMem, char * _pszErr=NULL, unsigned int _unMaxErrLen=0) const
	{
		if( !g_fnValidFlexKeyValueList(_pFlexStartMem, m_ucCustomNums, m_stCustomFirstFlexEntityPos.m_fnGet(), _pszErr, _unMaxErrLen) )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, " : Custom");
			return false;
		}

		return true;
	}

	bool m_fnExistCustom(const char * _pFlexStartMem, const char * _pszKey) const
	{
		return g_fnExistFlexListKey(_pFlexStartMem, _pszKey, m_ucCustomNums, m_stCustomFirstFlexEntityPos.m_fnGet());
	}

	bool m_fnGetAllCustom(const char * _pFlexStartMem, list_FlexData & _rlistFlexData) const
	{
		return g_fnGetAllFlexListEntity(_pFlexStartMem, m_ucCustomNums, m_stCustomFirstFlexEntityPos.m_fnGet(), _rlistFlexData);
	}
	
	const char * m_fnGetCustomValue(const char * _pFlexStartMem, const char * _pszKey, bool & _bResult) const
	{
		const char * pszHdrValue = g_fnFindFlexListValue(_pFlexStartMem, _pszKey, m_ucCustomNums, m_stCustomFirstFlexEntityPos.m_fnGet());
		if( pszHdrValue )
		{
			_bResult = true;
			return pszHdrValue;
		}
		else
		{
			_bResult = false;
			return "";
		}
	}
	
	unsigned short m_fnSetCustom(char * _pFlexStartMem, unsigned short _usCurrentFlexPos, const char * _pszKey, const char * _pszValue)
	{
		unsigned short usSetFlexLen = 0;
		if( (usSetFlexLen = g_fnSetFlexListEntity(_pFlexStartMem, _usCurrentFlexPos, _pszKey, _pszValue)) > 0 )
		{
			if( m_ucCustomNums == 0 )
			{
				m_stCustomFirstFlexEntityPos.m_fnSet(_usCurrentFlexPos);
				m_stCustomLastFlexEntityPos.m_fnSet(_usCurrentFlexPos);
			}
			else
			{
				FxCustomListEntity_t * pstCustomListPrevEntity = (FxCustomListEntity_t * )(_pFlexStartMem+m_stCustomLastFlexEntityPos.m_fnGet());
				pstCustomListPrevEntity->m_stNextPos.m_fnSet(_usCurrentFlexPos);
				m_stCustomLastFlexEntityPos.m_fnSet(_usCurrentFlexPos);			
			}
			++m_ucCustomNums;
			return usSetFlexLen;
		}
		else
			return 0;
	}

	char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen, const char * _pFlexStartMem) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "<Custom Info>\n");
		g_fnGetFlexListStr(_pszPrint, _unMaxLen, _pFlexStartMem, m_ucCustomNums, m_stCustomFirstFlexEntityPos.m_fnGet());
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "CustomInfo[");
		g_fnGetFlexListStr(_pszPrint, _unMaxLen, _pFlexStartMem, m_ucCustomNums, m_stCustomFirstFlexEntityPos.m_fnGet());
		sipif_fx_strncat(_pszPrint, _unMaxLen, "]");
#endif
		
		return _pszPrint;
	}
}FxCustomInfo_t;


/*================ SIP Body ================*/
struct SipIfFxSipBody
{
	char m_cTuId;	
	
	/*==== fixed data ====*/	
	FxSipInfo_t m_stSipInfo;
	FxSipContentInfo_t m_stSipContentInfo;
	FxCustomInfo_t m_stCustomInfo;
	USHORT_t m_stFlexDataLen;

	/*==== flexible data ====*/
	char m_FlexData[FX_MAX_FLEX_DATA_LEN];

	unsigned short m_fnGetSizeExceptFlex() const{ return (unsigned short)(sizeof(struct SipIfFxSipBody) -FX_MAX_FLEX_DATA_LEN); }
	unsigned short m_fnGetFlexDataLen() const{ return m_stFlexDataLen.m_fnGet(); }
	unsigned short m_fnGetSize() const{ return m_fnGetSizeExceptFlex()+m_fnGetFlexDataLen(); }

	void m_fnReset()
	{
		m_cTuId = -1;

		m_stSipInfo.m_fnReset();
		m_stSipContentInfo.m_fnReset();
		m_stCustomInfo.m_fnReset();
		m_stFlexDataLen.m_fnReset();
		memset(m_FlexData, 0x00, FX_MAX_FLEX_DATA_LEN);
	}

	bool m_fnValid(char * _pszErr=NULL, unsigned int _unMaxErrLen=0) const
	{
		if( !m_stSipInfo.m_fnValid(m_FlexData, _pszErr, _unMaxErrLen) )
			return false;

		if( !m_stSipContentInfo.m_fnValid(m_FlexData, _pszErr, _unMaxErrLen) )
			return false;

		if( !m_stCustomInfo.m_fnValid(m_FlexData, _pszErr, _unMaxErrLen) )
			return false;

		if( m_stFlexDataLen.m_fnGet() > FX_MAX_FLEX_DATA_LEN )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, "SipIfFxSipBody : over len : %d > %d", m_stFlexDataLen.m_fnGet(), FX_MAX_FLEX_DATA_LEN);
			return false;
		}

		return true;
	}

	char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "<Body for SIP>\nTuId[%d]\nTotalBodyLen[%d]\nFlexDataLen[%d]\n", m_cTuId, m_fnGetSize(), m_fnGetFlexDataLen());
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n");
		m_stSipInfo.m_fnGetStr(_pszPrint, _unMaxLen, m_FlexData);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n");
		m_stSipContentInfo.m_fnGetStr(_pszPrint, _unMaxLen, m_FlexData);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n");
		m_stCustomInfo.m_fnGetStr(_pszPrint, _unMaxLen, m_FlexData);
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "SipBody{TuId[%d],TotalBodyLen[%d],FlexDataLen[%d],", m_cTuId, m_fnGetSize(), m_fnGetFlexDataLen());
		m_stSipInfo.m_fnGetStr(_pszPrint, _unMaxLen, m_FlexData);
		sipif_fx_strncat(_pszPrint, _unMaxLen, ",");
		m_stSipContentInfo.m_fnGetStr(_pszPrint, _unMaxLen, m_FlexData);
		sipif_fx_strncat(_pszPrint, _unMaxLen, ",");
		m_stCustomInfo.m_fnGetStr(_pszPrint, _unMaxLen, m_FlexData);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "}");
#endif

		return _pszPrint;
	}

	/*==== Get Function ===*/
	const char * m_fnGetReqUriStr() const{ return m_stSipInfo.m_fnGetReqUriStr(m_FlexData); }
	bool m_fnExistHdrKey(const char * _pszHdrName) const{ return m_stSipInfo.m_fnExistHdr(m_FlexData, _pszHdrName); }
	bool m_fnGetAllHdr(list_FlexData & _rlistFlexData) const{ return m_stSipInfo.m_fnGetAllHdr(m_FlexData, _rlistFlexData); }	
	const char * m_fnGetHdrValue(const char * _pszHdrName) const{ return m_stSipInfo.m_fnGetHdrValue(m_FlexData, _pszHdrName); }
	const char * m_fnGetContent() const{ return m_stSipContentInfo.m_fnGetContent(m_FlexData); }
	bool m_fnExistCustom(const char * _pszKey) const{ return m_stCustomInfo.m_fnExistCustom(m_FlexData, _pszKey); }
	bool m_fnGetAllCustom(list_FlexData & _rlistFlexData) const{ return m_stCustomInfo.m_fnGetAllCustom(m_FlexData, _rlistFlexData); }	
	const char * m_fnGetCustomValue(const char * _pszKey, bool & _bResult) const{ return m_stCustomInfo.m_fnGetCustomValue(m_FlexData, _pszKey, _bResult); }

	/*==== Set Function ====*/
	bool m_fnSetReqUriStr(const char * _pszReqUri)
	{
		unsigned short usSetFlexLen = m_stSipInfo.m_fnSetReqUriStr(m_FlexData, m_stFlexDataLen.m_fnGet(), _pszReqUri);
		if( usSetFlexLen > 0 )
		{
			m_stFlexDataLen.m_fnAdd(usSetFlexLen);
			return true;
		}
		else
			return false;
	}
	bool m_fnSetHdr(const char * _pszHdrName, const char * _pszHdrValue)
	{	
		unsigned short usSetFlexLen = m_stSipInfo.m_fnSetHdr(m_FlexData, m_stFlexDataLen.m_fnGet(), _pszHdrName, _pszHdrValue);
		if( usSetFlexLen > 0 )
		{
			m_stFlexDataLen.m_fnAdd(usSetFlexLen);
			return true;
		}
		else
			return false;
	}
	bool m_fnSetContent(const char * _pSipContent, unsigned int _unContentLen)
	{
		unsigned short usSetFlexLen = m_stSipContentInfo.m_fnSetContent(m_FlexData, m_stFlexDataLen.m_fnGet(), _pSipContent, _unContentLen);
		if( usSetFlexLen > 0 )
		{
			m_stFlexDataLen.m_fnAdd(usSetFlexLen);
			return true;
		}
		else
			return false;
	}
	bool m_fnSetCustom(const char * _pszKey, const char * _pszValue)
	{
		unsigned short usSetFlexLen = m_stCustomInfo.m_fnSetCustom(m_FlexData, m_stFlexDataLen.m_fnGet(), _pszKey, _pszValue);
		if( usSetFlexLen > 0 )
		{
			//printf("@@@@ setcustom 1 : key(%s), value(%s) : setFlexLen(%d), curFlexLen(%d)\n", _pszKey, _pszValue, usSetFlexLen, m_stFlexDataLen.m_fnGet());
			m_stFlexDataLen.m_fnAdd(usSetFlexLen);
			//printf("@@@@ setcustom 2 : key(%s), value(%s) : setFlexLen(%d), curFlexLen(%d)\n", _pszKey, _pszValue, usSetFlexLen, m_stFlexDataLen.m_fnGet());
			return true;
		}
		else
			return false;
	}
};
typedef struct SipIfFxSipBody SipIfFxSipBody_t;


/*================ PingReq Body ================*/
struct SipIfFxPingReqBody
{
	bool m_bEnable;
	UINT_t m_stCurrentSessCnt;

	unsigned short m_fnGetSize() const{ return sizeof(struct SipIfFxPingReqBody); }

	void m_fnReset()
	{
		m_bEnable = false;
		m_stCurrentSessCnt.m_fnReset();
	}

	bool m_fnValid() const{ return true; }

	char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "<Body for PingReq>\nEnable[%d]\nCurSessCnt[%u]\n", m_bEnable, m_stCurrentSessCnt.m_fnGet());
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "PingReqBody{Enable[%d],CurSessCnt[%u]}", m_bEnable, m_stCurrentSessCnt.m_fnGet());
#endif

		return _pszPrint;
	}
};
typedef struct SipIfFxPingReqBody SipIfFxPingReqBody_t;


/*================ InitReq Body ================*/
struct SipIfFxInitReqBody
{
	SVCSetupApp m_stSetup;

	unsigned short m_fnGetSize() const{ return sizeof(struct SipIfFxInitReqBody); }

	void m_fnReset()
	{
		memset(&m_stSetup, 0x00, sizeof(SVCSetupApp));
	}

	bool m_fnValid() const{ return true; }

	void m_fnHtoN(){ m_stSetup.hton(); }
	void m_fnNtoH(){ m_stSetup.ntoh(); }

	char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "<Body for InitReq>\nAppId[%u]\nMaxSessCnt[%u]\n", m_stSetup.uiAppId, m_stSetup.uiSesnSnt);
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "InitReqBody{AppId[%u],MaxSessCnt[%u]}", m_stSetup.uiAppId, m_stSetup.uiSesnSnt);
#endif

		return _pszPrint;
	}
};
typedef struct SipIfFxInitReqBody SipIfFxInitReqBody_t;


/*================ Rsp Body ================*/
struct SipIfFxRspBody
{
	unsigned char m_ucResult;		//ESipIfFxRspResult_t enum

	unsigned short m_fnGetSize() const{ return sizeof(struct SipIfFxRspBody); }

	void m_fnReset()
	{
		m_ucResult = E_SIPIF_FX_RSP_succ;
	}

	bool m_fnValid() const{ return true; }

	char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "<Body for Rsp>\nResult[%s]\n", g_fnGetSipIfFxRspResultStr(m_ucResult));
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "RspBody{Result[%s]}", g_fnGetSipIfFxRspResultStr(m_ucResult));
#endif

		return _pszPrint;
	}
};
typedef struct SipIfFxRspBody SipIfFxRspBody_t;


/*================ SIP Interface message ================*/
typedef struct
{
	SVCComMsgHdr m_stHdr;
	SipIfFxSipBody_t m_stBody;

	void m_fnInit()
	{
		memset(&m_stHdr, 0x00, sizeof(struct SVCComMsgHdr));
		m_stHdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE;
		m_stHdr.uiType = E_SIPIF_FX_MSG_sip;
		m_stBody.m_fnReset();
	}

	bool m_fnValid(char * _pszErr=NULL, unsigned int _unMaxErrLen=0) const
	{
		if( m_stHdr.uiMagicCookie != SVC_MSG_MAGIC_COOKIE )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, "SVC_Fx_Sip_t : invalid MagicCookie[%u]", m_stHdr.uiMagicCookie);
			return false;
		}

		if( m_stHdr.uiMsgLen != sizeof(struct SVCComMsgHdr)+m_stBody.m_fnGetSize() )
		{
			sipif_fx_strncat(_pszErr, _unMaxErrLen, "SVC_Fx_Sip_t : unmatch msg len[%u != %u]", m_stHdr.uiMsgLen, sizeof(struct SVCComMsgHdr)+m_stBody.m_fnGetSize());
			return false;
		}

		if( !m_stBody.m_fnValid(_pszErr, _unMaxErrLen) )
			return false;

		return true;
	}

	void m_fnHtoN(){ m_stHdr.hton(); }
	void m_fnNtoH(){ m_stHdr.ntoh(); }
	

	/*========================================================*/
	/* Recommand to execute Get-Function after checking by m_fnValid()                                  */
	/*========================================================*/
	/*==== Get function for ComHdr ====*/	
	unsigned int m_fnGetMsgLen() const{ return m_stHdr.uiMsgLen; }	
	int m_fnGetCmd() const{ return m_stHdr.uiType; }
	int m_fnGetCompSessId() const{ return m_stHdr.uiCompSesId; }
	int m_fnGetCompId() const{ return m_stHdr.uiCompId; }
	int m_fnGetAsSessId() const{ return m_stHdr.uiAsSesId; }
	int m_fnGetAsId() const{ return m_stHdr.uiAsId; }
	int m_fnGetReason() const{ return m_stHdr.uiReasonCode; }

	/*==== Set function for ComHdr ====*/
	unsigned int m_fnSetMsgSize()
	{
		m_stHdr.uiMsgLen = sizeof(struct SVCComMsgHdr);
		m_stHdr.uiMsgLen += m_stBody.m_fnGetSize();
		return m_stHdr.uiMsgLen;
	}
	void m_fnSetCompSessId(FX_IN unsigned int _unCompSessId){ m_stHdr.uiCompSesId = _unCompSessId; }
	void m_fnSetCompId(FX_IN unsigned int _unCompId){ m_stHdr.uiCompId = _unCompId; }
	void m_fnSetAsSessId(FX_IN unsigned int _unAsSessId){ m_stHdr.uiAsSesId = _unAsSessId; }
	void m_fnSetAsId(FX_IN unsigned int _unAsId){ m_stHdr.uiAsId = _unAsId; }
	void m_fnSetReason(FX_IN unsigned int _unReason){ m_stHdr.uiReasonCode = _unReason; }

	/*==== Get function for Body(SIP) ====*/
	unsigned char m_fnGetSessType() const{ return m_stBody.m_stSipInfo.m_ucSessType; }
	unsigned char m_fnGetDialogType() const{ return m_stBody.m_stSipInfo.m_ucDialogType; }
	unsigned char m_fnGetSipDir() const{ return m_stBody.m_stSipInfo.m_ucSipDir; }
	const char * m_fnGetLocalIp() const{ return m_stBody.m_stSipInfo.m_szLocalIp; }
	const char * m_fnGetRemoteIp() const{ return m_stBody.m_stSipInfo.m_szRemoteIp; }
	unsigned short m_fnGetLocalPort() const{ return m_stBody.m_stSipInfo.m_stLocalPort.m_fnGet(); }
	unsigned short m_fnGetRemotePort() const{ return m_stBody.m_stSipInfo.m_stRemotePort.m_fnGet(); }
	unsigned char m_fnGetTransportType() const{ return m_stBody.m_stSipInfo.m_ucTransportType; }
	unsigned char m_fnGetSipMethod() const{ return m_stBody.m_stSipInfo.m_ucSipMethod; }
	unsigned short m_fnGetSipRspCode() const{ return m_stBody.m_stSipInfo.m_stSipRsp.m_fnGet(); }
	unsigned char m_fnGetContentType() const{ return m_stBody.m_stSipInfo.m_ucContentType; }	
	const char * m_fnGetRequestUri() const{ return m_stBody.m_fnGetReqUriStr(); }
	unsigned char m_fnGetSipHdrNums() const{ return m_stBody.m_stSipInfo.m_ucHdrEntityNums; }		//exception Request-Uri
	bool m_fnExistHdr(FX_IN const char * _pszHdrName) const{ return m_stBody.m_fnExistHdrKey(_pszHdrName); }
	bool m_fnGetAllHdr(FX_OUT list_FlexData & _rlistFlexData) const{ return m_stBody.m_fnGetAllHdr(_rlistFlexData); }	
	const char * m_fnGetHdrValue(FX_IN const char * _pszHdrName) const{ return m_stBody.m_fnGetHdrValue(_pszHdrName); }

	/*==== Set function for Body(SIP) ====*/
	bool m_fnSetSessType(FX_IN unsigned char _ucSessType){ m_stBody.m_stSipInfo.m_ucSessType = _ucSessType; return true; }
	bool m_fnSetDialogType(FX_IN unsigned char _ucDialogType){ m_stBody.m_stSipInfo.m_ucDialogType = _ucDialogType; return true; }
	bool m_fnSetSipDir(FX_IN unsigned char _ucSipDir){ m_stBody.m_stSipInfo.m_ucSipDir = _ucSipDir; return true; }
	bool m_fnSetLocalIp(FX_IN const char * _pszIp)
	{
		if( _pszIp && strlen(_pszIp) > 0 )
		{
			strncpy(m_stBody.m_stSipInfo.m_szLocalIp, _pszIp, sizeof(m_stBody.m_stSipInfo.m_szLocalIp)-1);
			m_stBody.m_stSipInfo.m_szLocalIp[sizeof(m_stBody.m_stSipInfo.m_szLocalIp)-1] = 0x00;
			return true;
		}
		else
			return false;
	}
	bool m_fnSetRemoteIp(FX_IN const char * _pszIp)
	{
		if( _pszIp && strlen(_pszIp) > 0 )
		{
			strncpy(m_stBody.m_stSipInfo.m_szRemoteIp, _pszIp, sizeof(m_stBody.m_stSipInfo.m_szRemoteIp)-1);
			m_stBody.m_stSipInfo.m_szRemoteIp[sizeof(m_stBody.m_stSipInfo.m_szRemoteIp)-1] = 0x00;
			return true;
		}
		else
			return false;
	}
	bool m_fnSetLocalPort(FX_IN unsigned short _usPort){ m_stBody.m_stSipInfo.m_stLocalPort.m_fnSet(_usPort); return true; }
	bool m_fnSetRemotePort(FX_IN unsigned short _usPort){ m_stBody.m_stSipInfo.m_stRemotePort.m_fnSet(_usPort); return true; }
	bool m_fnSetTransportType(FX_IN unsigned char _ucTransportType){ m_stBody.m_stSipInfo.m_ucTransportType = _ucTransportType; return true; }
	bool m_fnSetSipMethod(FX_IN unsigned char _ucSipMethod){ m_stBody.m_stSipInfo.m_ucSipMethod = _ucSipMethod; return true; }
	bool m_fnSetSipRspCode(FX_IN unsigned short _usSipRspCode){ m_stBody.m_stSipInfo.m_stSipRsp.m_fnSet(_usSipRspCode); return true; }
	bool m_fnSetContentType(FX_IN unsigned char _ucContentType){ m_stBody.m_stSipInfo.m_ucContentType = _ucContentType; return true; }
	bool m_fnSetRequestUri(FX_IN const char * _pszReqUri){ return m_stBody.m_fnSetReqUriStr(_pszReqUri); }
	bool m_fnSetHdr(FX_IN const char * _pszHdrName, FX_IN const char * _pszHdrValue){ return m_stBody.m_fnSetHdr(_pszHdrName, _pszHdrValue); }

	/*==== function for Body(Content) ====*/
	const char * m_fnGetContent() const{ return m_stBody.m_fnGetContent(); }	
	bool m_fnSetContent(FX_IN const char * _pszSipContent){ return m_fnSetContent(_pszSipContent, strlen(_pszSipContent)); }
	bool m_fnSetContent(FX_IN const char * _pSipContent, FX_IN unsigned int _unContentLen){ return m_stBody.m_fnSetContent(_pSipContent, _unContentLen); }

	/*==== function for Body(Custom) ====*/
	unsigned char m_fnGetCustomNums() const{ return m_stBody.m_stCustomInfo.m_ucCustomNums; }
	bool m_fnExistCustom(FX_IN const char * _pszKey) const{ return m_stBody.m_fnExistCustom(_pszKey); }
	bool m_fnGetAllCustom(FX_OUT list_FlexData & _rlistFlexData) const{ return m_stBody.m_fnGetAllCustom(_rlistFlexData); }	
	const char * m_fnGetCustomValue(FX_IN const char * _pszKey, bool & _bResult) const{ return m_stBody.m_fnGetCustomValue(_pszKey, _bResult); }
	bool m_fnSetCustom(FX_IN const char * _pszKey, FX_IN const char * _pszValue){ return m_stBody.m_fnSetCustom(_pszKey, _pszValue); }
	const char * m_fnGetCaller(){ bool bResult; return m_stBody.m_fnGetCustomValue("cr", bResult); };
	bool m_fnSetCaller(FX_IN const char * _pszCaller){ return m_stBody.m_fnSetCustom("cr", _pszCaller); };
	const char * m_fnGetCalled(){ bool bResult; return m_stBody.m_fnGetCustomValue("cd", bResult); };
	bool m_fnSetCalled(FX_IN const char * _pszCalled){ return m_stBody.m_fnSetCustom("cd", _pszCalled); };

	/*==== function for Body(Etc) ====*/
	char m_fnGetTuId() const{ return m_stBody.m_cTuId; }
	bool m_fnSetTuId(char _cTuId){ m_stBody.m_cTuId = _cTuId; return true; }

	char * m_fnGetStr(FX_OUT char * _pszPrint, FX_IN unsigned int _unMaxLen) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

		if( !m_fnValid() )
			return _pszPrint;

		_pszPrint[0] = 0x00;

#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "==# FxMsg(SipEvent) #=================================================\n<ComHdr>\n" \
																			"TotalLength[%u]\nCmd[%s]\nComSessId[%u]\nComId[%u]\nAsSessId[%u]\nAsId[%u]\nReason[%u]\n",
																			m_fnGetMsgLen(), g_fnGetSipIfFxMsgTypeStr(m_fnGetCmd()), m_fnGetCompSessId(), m_fnGetCompId(), m_fnGetAsSessId(), m_fnGetAsId(), m_fnGetReason());
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n");
		m_stBody.m_fnGetStr(_pszPrint, _unMaxLen);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n===================================================================");
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "FxMsg(SipEvent){ComHdr{TotalLength[%u],Cmd[%s],ComSessId[%u],ComId[%u],AsSessId[%u],AsId[%u],Reason[%u]},",
																			m_fnGetMsgLen(), g_fnGetSipIfFxMsgTypeStr(m_fnGetCmd()), m_fnGetCompSessId(), m_fnGetCompId(), m_fnGetAsSessId(), m_fnGetAsId(), m_fnGetReason());
		m_stBody.m_fnGetStr(_pszPrint, _unMaxLen);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "}");		
#endif
		
		return _pszPrint;
	}

/*
	char * m_fnGetHexStr(FX_OUT char * _pszPrint, FX_IN unsigned int _unMaxLen) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

		_pszPrint[0] = 0x00;

		sipif_fx_strncat(_pszPrint, _unMaxLen, "==# FxMsg(SipEvent) #====================\n");

		for( unsigned int i = 0; i < _unMaxLen; ++i )
		{
			sipif_fx_strncat(_pszPrint, _unMaxLen, "0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x"
			
		}
		
		return _pszPrint;
	}
*/
}SVC_Fx_Sip_t;


/*================ PingReq Interface message ================*/
typedef struct
{
	SVCComMsgHdr m_stHdr;
	SipIfFxPingReqBody_t m_stBody;

	void m_fnInit()
	{
		memset(&m_stHdr, 0x00, sizeof(struct SVCComMsgHdr));
		m_stHdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE;
		m_stHdr.uiType = E_SIPIF_FX_MSG_pingReq;
		m_stBody.m_fnReset();
	}

	bool m_fnValid() const
	{
		if( m_stHdr.uiMagicCookie != SVC_MSG_MAGIC_COOKIE )
			return false;

		if( m_stHdr.uiMsgLen != sizeof(struct SVCComMsgHdr)+m_stBody.m_fnGetSize() )
			return false;

		if( !m_stBody.m_fnValid() )
			return false;

		return true;
	}

	void m_fnHtoN(){ m_stHdr.hton(); }
	void m_fnNtoH(){ m_stHdr.ntoh(); }

	/*==== function for ComHdr(Get) ====*/	
	unsigned int m_fnGetMsgLen() const{ return m_stHdr.uiMsgLen; }	
	int m_fnGetCmd() const{ return m_stHdr.uiType; }
	int m_fnGetCompSessId() const{ return m_stHdr.uiCompSesId; }
	int m_fnGetCompId() const{ return m_stHdr.uiCompId; }
	int m_fnGetAsSessId() const{ return m_stHdr.uiAsSesId; }
	int m_fnGetAsId() const{ return m_stHdr.uiAsId; }
	int m_fnGetReason() const{ return m_stHdr.uiReasonCode; }

	/*==== function for ComHdr(Set) ====*/
	unsigned int m_fnSetMsgSize()
	{
		m_stHdr.uiMsgLen = sizeof(struct SVCComMsgHdr);
		m_stHdr.uiMsgLen += m_stBody.m_fnGetSize();
		return m_stHdr.uiMsgLen;
	}
	void m_fnSetCompSessId(FX_IN unsigned int _unCompSessId){ m_stHdr.uiCompSesId = _unCompSessId; }
	void m_fnSetCompId(FX_IN unsigned int _unCompId){ m_stHdr.uiCompId = _unCompId; }
	void m_fnSetAsSessId(FX_IN unsigned int _unAsSessId){ m_stHdr.uiAsSesId = _unAsSessId; }
	void m_fnSetAsId(FX_IN unsigned int _unAsId){ m_stHdr.uiAsId = _unAsId; }
	void m_fnSetReason(FX_IN unsigned int _unReason){ m_stHdr.uiReasonCode = _unReason; }

	/*==== function for Body ====*/
	bool m_fnGetEnable() const{ return m_stBody.m_bEnable; }
	bool m_fnSetEnable(FX_IN bool _bEnable){ m_stBody.m_bEnable = _bEnable; return true; }
	unsigned int m_fnGetCurrentSessCnt() const{ return m_stBody.m_stCurrentSessCnt.m_fnGet(); }
	bool m_fnSetCurrentSessCnt(unsigned int _unSessCnt){ m_stBody.m_stCurrentSessCnt.m_fnSet(_unSessCnt); return true; }

	char * m_fnGetStr(FX_OUT char * _pszPrint, FX_IN unsigned int _unMaxLen) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

		if( !m_fnValid() )
			return _pszPrint;

		_pszPrint[0] = 0x00;

#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "==# FxMsg(PingReq) #==============================================\n<ComHdr>\n" \
																	"TotalLength[%u]\nCmd[%s]\nComSessId[%u]\nComId[%u]\nAsSessId[%u]\nAsId[%u]\nReason[%u]\n",
																	m_fnGetMsgLen(), g_fnGetSipIfFxMsgTypeStr(m_fnGetCmd()), m_fnGetCompSessId(), m_fnGetCompId(), m_fnGetAsSessId(), m_fnGetAsId(), m_fnGetReason());
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n");
		m_stBody.m_fnGetStr(_pszPrint, _unMaxLen);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n===============================================================");
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "FxMsg(PingReq){ComHdr{TotalLength[%u],Cmd[%s],ComSessId[%u],ComId[%u],AsSessId[%u],AsId[%u],Reason[%u]},",
																			m_fnGetMsgLen(), g_fnGetSipIfFxMsgTypeStr(m_fnGetCmd()), m_fnGetCompSessId(), m_fnGetCompId(), m_fnGetAsSessId(), m_fnGetAsId(), m_fnGetReason());
		m_stBody.m_fnGetStr(_pszPrint, _unMaxLen);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "}");
#endif

		return _pszPrint;
	}
}SVC_Fx_PingReq_t;


/*================ InternalTerm Interface message ================*/
typedef struct
{
	SVCComMsgHdr m_stHdr;

	void m_fnInit()
	{
		memset(&m_stHdr, 0x00, sizeof(struct SVCComMsgHdr));
		m_stHdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE;
		m_stHdr.uiType = E_SIPIF_FX_MSG_internalTerm;
	}

	bool m_fnValid() const
	{
		if( m_stHdr.uiMagicCookie != SVC_MSG_MAGIC_COOKIE )
			return false;

		if( m_stHdr.uiMsgLen != sizeof(struct SVCComMsgHdr) )
			return false;

		return true;
	}

	void m_fnHtoN(){ m_stHdr.hton(); }
	void m_fnNtoH(){ m_stHdr.ntoh(); }

	/*==== function for ComHdr(Get) ====*/	
	unsigned int m_fnGetMsgLen() const{ return m_stHdr.uiMsgLen; }	
	int m_fnGetCmd() const{ return m_stHdr.uiType; }
	int m_fnGetCompSessId() const{ return m_stHdr.uiCompSesId; }
	int m_fnGetCompId() const{ return m_stHdr.uiCompId; }
	int m_fnGetAsSessId() const{ return m_stHdr.uiAsSesId; }
	int m_fnGetAsId() const{ return m_stHdr.uiAsId; }
	int m_fnGetReason() const{ return m_stHdr.uiReasonCode; }

	/*==== function for ComHdr(Set) ====*/
	unsigned int m_fnSetMsgSize()
	{
		m_stHdr.uiMsgLen = sizeof(struct SVCComMsgHdr);
		return m_stHdr.uiMsgLen;
	}
	void m_fnSetCompSessId(FX_IN unsigned int _unCompSessId){ m_stHdr.uiCompSesId = _unCompSessId; }
	void m_fnSetCompId(FX_IN unsigned int _unCompId){ m_stHdr.uiCompId = _unCompId; }
	void m_fnSetAsSessId(FX_IN unsigned int _unAsSessId){ m_stHdr.uiAsSesId = _unAsSessId; }
	void m_fnSetAsId(FX_IN unsigned int _unAsId){ m_stHdr.uiAsId = _unAsId; }
	void m_fnSetReason(FX_IN unsigned int _unReason){ m_stHdr.uiReasonCode = _unReason; }

	char * m_fnGetStr(FX_OUT char * _pszPrint, FX_IN unsigned int _unMaxLen) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

		if( !m_fnValid() )
			return _pszPrint;

		_pszPrint[0] = 0x00;

#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "==# FxMsg(PingReq) #==============================================\n<ComHdr>\n" \
																	"TotalLength[%u]\nCmd[%s]\nComSessId[%u]\nComId[%u]\nAsSessId[%u]\nAsId[%u]\nReason[%u]\n",
																	m_fnGetMsgLen(), g_fnGetSipIfFxMsgTypeStr(m_fnGetCmd()), m_fnGetCompSessId(), m_fnGetCompId(), m_fnGetAsSessId(), m_fnGetAsId(), m_fnGetReason());
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n");
		m_stBody.m_fnGetStr(_pszPrint, _unMaxLen);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n===============================================================");
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "FxMsg(InternalTerm){ComHdr{TotalLength[%u],Cmd[%s],ComSessId[%u],ComId[%u],AsSessId[%u],AsId[%u],Reason[%u]}}",
																			m_fnGetMsgLen(), g_fnGetSipIfFxMsgTypeStr(m_fnGetCmd()), m_fnGetCompSessId(), m_fnGetCompId(), m_fnGetAsSessId(), m_fnGetAsId(), m_fnGetReason());
#endif

		return _pszPrint;
	}
}SVC_Fx_InternalTerm_t;


/*================ InitReq Interface message ================*/
typedef struct
{
	SVCComMsgHdr m_stHdr;
	SipIfFxInitReqBody_t m_stBody;

	void m_fnInit()
	{
		memset(&m_stHdr, 0x00, sizeof(struct SVCComMsgHdr));
		m_stHdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE;
		m_stHdr.uiType = E_SIPIF_FX_MSG_initReq;
		m_stBody.m_fnReset();
	}

	bool m_fnValid() const
	{
		if( m_stHdr.uiMagicCookie != SVC_MSG_MAGIC_COOKIE )
			return false;

		if( m_stHdr.uiMsgLen != sizeof(struct SVCComMsgHdr)+m_stBody.m_fnGetSize() )
			return false;

		if( !m_stBody.m_fnValid() )
			return false;

		return true;
	}

	void m_fnHtoN(){ m_stHdr.hton(); m_stBody.m_fnHtoN(); }
	void m_fnNtoH(){ m_stHdr.ntoh(); m_stBody.m_fnNtoH(); }

	/*==== function for ComHdr(Get) ====*/	
	unsigned int m_fnGetMsgLen() const{ return m_stHdr.uiMsgLen; }	
	int m_fnGetCmd() const{ return m_stHdr.uiType; }
	int m_fnGetCompSessId() const{ return m_stHdr.uiCompSesId; }
	int m_fnGetCompId() const{ return m_stHdr.uiCompId; }
	int m_fnGetAsSessId() const{ return m_stHdr.uiAsSesId; }
	int m_fnGetAsId() const{ return m_stHdr.uiAsId; }
	int m_fnGetReason() const{ return m_stHdr.uiReasonCode; }

	/*==== function for ComHdr(Set) ====*/
	unsigned int m_fnSetMsgSize()
	{
		m_stHdr.uiMsgLen = sizeof(struct SVCComMsgHdr);
		m_stHdr.uiMsgLen += m_stBody.m_fnGetSize();
		return m_stHdr.uiMsgLen;
	}
	void m_fnSetCompSessId(FX_IN unsigned int _unCompSessId){ m_stHdr.uiCompSesId = _unCompSessId; }
	void m_fnSetCompId(FX_IN unsigned int _unCompId){ m_stHdr.uiCompId = _unCompId; }
	void m_fnSetAsSessId(FX_IN unsigned int _unAsSessId){ m_stHdr.uiAsSesId = _unAsSessId; }
	void m_fnSetAsId(FX_IN unsigned int _unAsId){ m_stHdr.uiAsId = _unAsId; }
	void m_fnSetReason(FX_IN unsigned int _unReason){ m_stHdr.uiReasonCode = _unReason; }

	/*==== function for Body ====*/
	unsigned int m_fnGetAppId() const{ return m_stBody.m_stSetup.uiAppId; }
	bool m_fnSetAppId(FX_IN unsigned int _nAppId){ m_stBody.m_stSetup.uiAppId = _nAppId; return true; }
	unsigned int m_fnGetMaxSessCnt() const{ return m_stBody.m_stSetup.uiSesnSnt; }
	bool m_fnSetMaxSessCnt(unsigned int _unSessCnt){ m_stBody.m_stSetup.uiSesnSnt = _unSessCnt; return true; }

	char * m_fnGetStr(FX_OUT char * _pszPrint, FX_IN unsigned int _unMaxLen) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

		if( !m_fnValid() )
			return _pszPrint;

		_pszPrint[0] = 0x00;

#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "==# FxMsg(InitReq) #========================================\n<ComHdr>\n" \
																	"TotalLength[%u]\nCmd[%s]\nComSessId[%u]\nComId[%u]\nAsSessId[%u]\nAsId[%u]\nReason[%u]\n",
																	m_fnGetMsgLen(), g_fnGetSipIfFxMsgTypeStr(m_fnGetCmd()), m_fnGetCompSessId(), m_fnGetCompId(), m_fnGetAsSessId(), m_fnGetAsId(), m_fnGetReason());
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n");
		m_stBody.m_fnGetStr(_pszPrint, _unMaxLen);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n========================================================");
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "FxMsg(InitReq){ComHdr{TotalLength[%u],Cmd[%s],ComSessId[%u],ComId[%u],AsSessId[%u],AsId[%u],Reason[%u]},",
																			m_fnGetMsgLen(), g_fnGetSipIfFxMsgTypeStr(m_fnGetCmd()), m_fnGetCompSessId(), m_fnGetCompId(), m_fnGetAsSessId(), m_fnGetAsId(), m_fnGetReason());
		m_stBody.m_fnGetStr(_pszPrint, _unMaxLen);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "}");
#endif

		return _pszPrint;
	}
}SVC_Fx_InitReq_t;


/*================ Rsp Interface message ================*/
typedef struct
{
	SVCComMsgHdr m_stHdr;
	SipIfFxRspBody_t m_stBody;

	void m_fnInit(int _nType)		//_nType : 0(initRsp), 1(pingRsp)
	{
		memset(&m_stHdr, 0x00, sizeof(struct SVCComMsgHdr));
		m_stHdr.uiMagicCookie = SVC_MSG_MAGIC_COOKIE;
		if( _nType == 0 )
			m_stHdr.uiType = E_SIPIF_FX_MSG_initRsp;
		else
			m_stHdr.uiType = E_SIPIF_FX_MSG_pingRsp;
		m_stBody.m_fnReset();
	}

	bool m_fnValid() const
	{
		if( m_stHdr.uiMagicCookie != SVC_MSG_MAGIC_COOKIE )
			return false;

		if( m_stHdr.uiMsgLen != sizeof(struct SVCComMsgHdr)+m_stBody.m_fnGetSize() )
			return false;

		if( !m_stBody.m_fnValid() )
			return false;

		return true;
	}

	void m_fnHtoN(){ m_stHdr.hton(); }
	void m_fnNtoH(){ m_stHdr.ntoh(); }

	/*==== function for ComHdr(Get) ====*/	
	unsigned int m_fnGetMsgLen() const{ return m_stHdr.uiMsgLen; }	
	int m_fnGetCmd() const{ return m_stHdr.uiType; }
	int m_fnGetCompSessId() const{ return m_stHdr.uiCompSesId; }
	int m_fnGetCompId() const{ return m_stHdr.uiCompId; }
	int m_fnGetAsSessId() const{ return m_stHdr.uiAsSesId; }
	int m_fnGetAsId() const{ return m_stHdr.uiAsId; }
	int m_fnGetReason() const{ return m_stHdr.uiReasonCode; }

	/*==== function for ComHdr(Set) ====*/
	unsigned int m_fnSetMsgSize()
	{
		m_stHdr.uiMsgLen = sizeof(struct SVCComMsgHdr);
		m_stHdr.uiMsgLen += m_stBody.m_fnGetSize();
		return m_stHdr.uiMsgLen;
	}
	void m_fnSetCompSessId(FX_IN unsigned int _unCompSessId){ m_stHdr.uiCompSesId = _unCompSessId; }
	void m_fnSetCompId(FX_IN unsigned int _unCompId){ m_stHdr.uiCompId = _unCompId; }
	void m_fnSetAsSessId(FX_IN unsigned int _unAsSessId){ m_stHdr.uiAsSesId = _unAsSessId; }
	void m_fnSetAsId(FX_IN unsigned int _unAsId){ m_stHdr.uiAsId = _unAsId; }
	void m_fnSetReason(FX_IN unsigned int _unReason){ m_stHdr.uiReasonCode = _unReason; }

	/*==== function for Body ====*/
	unsigned char m_fnGetResult() const{ return m_stBody.m_ucResult; }
	bool m_fnSetResult(FX_IN unsigned char _ucResult){ m_stBody.m_ucResult = _ucResult; return true; }

	char * m_fnGetStr(FX_OUT char * _pszPrint, FX_IN unsigned int _unMaxLen) const
	{
		if( !_pszPrint || _unMaxLen == 0 )
			return _pszPrint;

		if( !m_fnValid() )
			return _pszPrint;

		_pszPrint[0] = 0x00;

#if 0
		sipif_fx_strncat(_pszPrint, _unMaxLen, "==# FxMsg(Rsp) #=======================================\n<ComHdr>\n" \
																	"TotalLength[%u]\nCmd[%s]\nComSessId[%u]\nComId[%u]\nAsSessId[%u]\nAsId[%u]\nReason[%u]\n",
																	m_fnGetMsgLen(), g_fnGetSipIfFxMsgTypeStr(m_fnGetCmd()), m_fnGetCompSessId(), m_fnGetCompId(), m_fnGetAsSessId(), m_fnGetAsId(), m_fnGetReason());
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n");
		m_stBody.m_fnGetStr(_pszPrint, _unMaxLen);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "\n======================================================");
#else
		sipif_fx_strncat(_pszPrint, _unMaxLen, "FxMsg(Rsp){ComHdr{TotalLength[%u],Cmd[%s],ComSessId[%u],ComId[%u],AsSessId[%u],AsId[%u],Reason[%u]},",
																			m_fnGetMsgLen(), g_fnGetSipIfFxMsgTypeStr(m_fnGetCmd()), m_fnGetCompSessId(), m_fnGetCompId(), m_fnGetAsSessId(), m_fnGetAsId(), m_fnGetReason());
		m_stBody.m_fnGetStr(_pszPrint, _unMaxLen);
		sipif_fx_strncat(_pszPrint, _unMaxLen, "}");
#endif

		return _pszPrint;
	}
}SVC_Fx_Rsp_t;


inline bool m_fnSampleTest()
{
	SVC_Fx_Sip_t stSipMsg;
	stSipMsg.m_fnInit();

	/*==== Comhdr set ====*/
	stSipMsg.m_fnSetCompSessId(100);
	stSipMsg.m_fnSetCompId(10);
	stSipMsg.m_fnSetAsSessId(200);
	stSipMsg.m_fnSetAsId(20);
	stSipMsg.m_fnSetReason(5);
	/*==== SIP set ====*/
	stSipMsg.m_fnSetTuId(7);
	stSipMsg.m_fnSetSipDir(E_SIPIF_FX_SIP_DIR_incoming);
	if( !stSipMsg.m_fnSetRemoteIp("1.1.1.1") ){ printf("fail : set IP\n"); return false; }
	stSipMsg.m_fnSetRemotePort(5060);
	stSipMsg.m_fnSetSipMethod(E_SIPIF_FX_SIP_METHOD_INVITE);
	stSipMsg.m_fnSetSipRspCode(200);
	stSipMsg.m_fnSetContentType(E_SIPIF_FX_SIP_CONTENT_app_sdp);	
	if( !stSipMsg.m_fnSetHdr("From", "sip:010111@sktims.net:5060;tag=7c8d3ca4") ){ printf("fail : set From Hdr\n"); return false; }
	if( !stSipMsg.m_fnSetRequestUri("sip:sktims.net") ){ printf("fail : set ReqUri\n"); return false; }
	if( !stSipMsg.m_fnSetHdr("Accept-Contact", "*;+g.3gpp.icsi-ref=\"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel\";require;explicit") ){ printf("fail : set Accept-Contact Hdr\n"); return false; }
	if( !stSipMsg.m_fnSetHdr("Content-Type", "application/sdp") ){ printf("fail : set Content-Type Hdr\n"); return false; }
	if( !stSipMsg.m_fnSetHdr("Allow", "INVITE,BYE,UPDATE,CANCEL") ){ printf("fail : set Allow Hdr\n"); return false; }
	if( !stSipMsg.m_fnSetHdr("Via", "SIP/2.0/UDP 223.33.173.11:5067;branch=z9hG4bK7d6718b6_2.1.2.24.194475.0..,SIP/2.0/UDP 223.33.173.11:5064;branch=z9hG4bK7d6718b2_3.0.2.24.363659.1..") ){ printf("fail : set Via Hdr\n"); return false; }
	/*==== Content set ====*/
	if( !stSipMsg.m_fnSetContent("v=0\n" \
													"o=TIS 5 5 IN IP4 113.217.234.216\n" \
													"s=msc session\n" \
													"c=IN IP4 113.217.234.216\n" \
													"t=0 0\n" \
													"m=audio 32026 RTP/AVP 100 98 8 0 101\n" \
													"a=rtpmap:100 AMR-WB/16000/1\n" \
													"a=fmtp:100 mode-set=8; octet-align=1\n" \
													"a=rtpmap:98 AMR/8000/1\n" \
													"a=fmtp:98 mode-set=7; octet-align=1\n" \
													"a=rtpmap:8 PCMA/8000/1\n" \
													"a=rtpmap:0 PCMU/8000/1\n" \
													"a=rtpmap:101 telephone-event/8000\n" \
													"a=fmtp:101 0-15\n" \
													"a=sendrecv\n") ){ printf("fail : set Content\n"); return false; }
	/*==== Custom set ====*/
	if( !stSipMsg.m_fnSetCustom("Test1", "TestValue1") ){ printf("fail : set Custom1\n"); return false; }
	if( !stSipMsg.m_fnSetCustom("Test2", "TestValue2") ){ printf("fail : set Custom2\n"); return false; }
	if( !stSipMsg.m_fnSetCustom("root", NULL) ){ printf("fail : set Custom3\n"); return false; }
	if( !stSipMsg.m_fnSetCaller("0101") ){ printf("fail : set Caller\n"); return false; }
	if( !stSipMsg.m_fnSetCalled("0102") ){ printf("fail : set Called\n"); return false; }
	
	stSipMsg.m_fnSetMsgSize();
	stSipMsg.m_fnHtoN();

	/*==== get ====*/
	stSipMsg.m_fnNtoH();
	if( !stSipMsg.m_fnValid() ){ printf("fail : invalid msg\n"); return false; }

	list_FlexData listFlex;
	if( !stSipMsg.m_fnGetAllHdr(listFlex) ){ printf("fail : get all hdr\n"); return false; }
	for( list_FlexData::const_iterator itrList = listFlex.begin(); itrList != listFlex.end(); ++itrList )
	{
		printf("Get All Hdr : %s : %s\n", itrList->first.c_str(), itrList->second.c_str());
	}

	if( !stSipMsg.m_fnGetAllCustom(listFlex) ){ printf("fail : get all custom\n"); return false; }
	for( list_FlexData::const_iterator itrList = listFlex.begin(); itrList != listFlex.end(); ++itrList )
	{
		printf("Get All Custom : %s : %s\n", itrList->first.c_str(), itrList->second.c_str());
	}
	
	char szPrint[10000];
	printf("%s\n", stSipMsg.m_fnGetStr(szPrint, sizeof(szPrint)));

	bool bResult;
	
	printf("Hdr : From : %s\n", stSipMsg.m_fnGetHdrValue("From"));
	printf("Hdr : Content-Type : %s\n", stSipMsg.m_fnGetHdrValue("content-type"));
	printf("Hdr : Contact : Exist[%d]\n", stSipMsg.m_fnExistHdr("Contact"));
	printf("Hdr : Allow : Exist[%d]\n", stSipMsg.m_fnExistHdr("allow"));
	printf("Custom : Caller : %s\n", stSipMsg.m_fnGetCaller());
	printf("Custom : Test1 : %s\n", stSipMsg.m_fnGetCustomValue("Test1", bResult));
	printf("Custom : root : %s\n", stSipMsg.m_fnGetCustomValue("root", bResult));
	printf("Custom : root : Exist[%d]\n", stSipMsg.m_fnExistCustom("root"));
	printf("Custom : trax : Exist[%d]\n", stSipMsg.m_fnExistCustom("trax"));	

	return true;
}

#endif

