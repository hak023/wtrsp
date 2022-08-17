
#ifndef ESIP_MOD_CLI_MSG_H
#define ESIP_MOD_CLI_MSG_H

#include <string.h>

#include "eSipUtil/mem.hxx"
#include "eSipUtil/string.hxx"

#include "eDum/eDumLibDialog.hxx"
#include "eDum/eDumLibSession.hxx"

namespace eSIP
{

static const char * s_pszMagicCookie = "z8f1Esip9o";

typedef enum
{
	E_CLI_MSG_TYPE_REQ_SESSION_NUM = 0,
	E_CLI_MSG_TYPE_RSP_SESSION_NUM,
	E_CLI_MSG_TYPE_REQ_SESSION_ACTIVE_LIST,
	E_CLI_MSG_TYPE_RSP_SESSION_ACTIVE_LIST,
	E_CLI_MSG_TYPE_REQ_SESSION_DESC,
	E_CLI_MSG_TYPE_RSP_SESSION_DESC,

	E_CLI_MSG_TYPE_REQ_DIALOG_NUM,
	E_CLI_MSG_TYPE_RSP_DIALOG_NUM,
	E_CLI_MSG_TYPE_REQ_DIALOG_ACTIVE_LIST,
	E_CLI_MSG_TYPE_RSP_DIALOG_ACTIVE_LIST,
	E_CLI_MSG_TYPE_REQ_DIALOG_DESC,
	E_CLI_MSG_TYPE_RSP_DIALOG_DESC,

	E_CLI_MSG_TYPE_NUM,
}ECliMsgType_t;

enum
{
	E_CONST_CLI_MAGIC_LEN = 10,
	E_CONST_CLI_MAX_BODY_LEN = 10000,
};

typedef struct
{
	ECliMsgType_t m_eCliMsgType;
	const char * m_szDesc;
}CliMsgTypeInfo_t;

inline const char * getCliMsgTypeString(ECliMsgType_t _eCliMsgType)
{
	static CliMsgTypeInfo_t s_arrMsgTypeInfo[E_CLI_MSG_TYPE_NUM] =
	{
		{E_CLI_MSG_TYPE_REQ_SESSION_NUM, "CLI_REQ_SESSION_NUM"},
		{E_CLI_MSG_TYPE_RSP_SESSION_NUM, "CLI_RSP_SESSION_NUM"},
		{E_CLI_MSG_TYPE_REQ_SESSION_ACTIVE_LIST, "CLI_MSG_TYPE_REQ_SESSION_ACTIVE_LIST"},
		{E_CLI_MSG_TYPE_RSP_SESSION_ACTIVE_LIST, "CLI_MSG_TYPE_RSP_SESSION_ACTIVE_LIST"},
		{E_CLI_MSG_TYPE_REQ_SESSION_DESC, "CLI_REQ_SESSION_DESC"},
		{E_CLI_MSG_TYPE_RSP_SESSION_DESC, "CLI_RSP_SESSION_DESC"},
		{E_CLI_MSG_TYPE_REQ_DIALOG_NUM, "CLI_REQ_DIALOG_NUM"},
		{E_CLI_MSG_TYPE_RSP_DIALOG_NUM, "CLI_RSP_DIALOG_NUM"},
		{E_CLI_MSG_TYPE_REQ_DIALOG_ACTIVE_LIST, "CLI_REQ_DIALOG_ACTIVE_LIST"},
		{E_CLI_MSG_TYPE_RSP_DIALOG_ACTIVE_LIST, "CLI_RSP_DIALOG_ACTIVE_LIST"},
		{E_CLI_MSG_TYPE_REQ_DIALOG_DESC, "CLI_REQ_DIALOG_DESC"},
		{E_CLI_MSG_TYPE_RSP_DIALOG_DESC, "CLI_RSP_DIALOG_DESC"},
	};

	if( _eCliMsgType >= 0 && _eCliMsgType < E_CLI_MSG_TYPE_NUM )
		return s_arrMsgTypeInfo[_eCliMsgType].m_szDesc;
	else
		return "";
}

struct CliMsgHeader
{
	char m_szMagicCookie[E_CONST_CLI_MAGIC_LEN];
	ECliMsgType_t m_eMsgType;
	unsigned int m_unBodyLen;
};
typedef struct CliMsgHeader CliMsgHeader_t;

struct CliMsg
{
	CliMsgHeader_t m_stHeader;

	char m_Body[E_CONST_CLI_MAX_BODY_LEN];
};
typedef struct CliMsg CliMsg_t;

/*==== Active ID Number ====*/
struct CliMsgBodyReqActiveIdNum
{
	eDum::EDialogMethodType_t m_eDialogMethodType;

	const char * str(char * _pszPrint, unsigned int _unLen) const
	{
		if( _pszPrint )
			_pszPrint[0] = 0x00;
	
		eSipUtil::strncat(_pszPrint, _unLen, "DialogType[%s]", eDum::getDialogMethodTypeString(m_eDialogMethodType));

		return _pszPrint;
	}
};
typedef struct CliMsgBodyReqActiveIdNum CliMsgBodyReqActiveIdNum_t;
struct CliMsgBodyRspActiveIdNum
{
	eDum::EDialogMethodType_t m_eDialogMethodType;
	int m_nActiveIdNumber;

	const char * str(char * _pszPrint, unsigned int _unLen) const
	{
		if( _pszPrint )
			_pszPrint[0] = 0x00;
	
		eSipUtil::strncat(_pszPrint, _unLen, "DialogType[%s], ActiveIdNumber[%d]", eDum::getDialogMethodTypeString(m_eDialogMethodType), m_nActiveIdNumber);

		return _pszPrint;
	}
};
typedef struct CliMsgBodyRspActiveIdNum CliMsgBodyRspActiveIdNum_t;

/*==== ID description ====*/
struct CliMsgBodyReqIdDesc
{
	eDum::EDialogMethodType_t m_eDialogMethodType;
	int m_nId;

	const char * str(char * _pszPrint, unsigned int _unLen) const
	{
		if( _pszPrint )
			_pszPrint[0] = 0x00;
	
		eSipUtil::strncat(_pszPrint, _unLen, "DialogType[%s], Id[%d]", eDum::getDialogMethodTypeString(m_eDialogMethodType), m_nId);

		return _pszPrint;
	}
};
typedef struct CliMsgBodyReqIdDesc CliMsgBodyReqIdDesc_t;
struct CliMsgBodyRspIdDesc
{
	eDum::EDialogMethodType_t m_eDialogMethodType;
	int m_nId;
	char szDesc[eDum::Dialog::E_CONST_PRINT_LEN];

	const char * str(char * _pszPrint, unsigned int _unLen) const
	{
		if( _pszPrint )
			_pszPrint[0] = 0x00;
	
		eSipUtil::strncat(_pszPrint, _unLen, "DialogType[%s], Id[%d], Desc[%s]", eDum::getDialogMethodTypeString(m_eDialogMethodType), m_nId, szDesc);

		return _pszPrint;
	}
};
typedef struct CliMsgBodyRspIdDesc CliMsgBodyRspIdDesc_t;

/*==== Active list ====*/
struct CliMsgBodyReqActiveList
{
	eDum::EDialogMethodType_t m_eDialogMethodType;
	int m_nIdRange;

	const char * str(char * _pszPrint, unsigned int _unLen) const
	{
		if( _pszPrint )
			_pszPrint[0] = 0x00;
	
		eSipUtil::strncat(_pszPrint, _unLen, "DialogType[%s], IdRange[%d]", eDum::getDialogMethodTypeString(m_eDialogMethodType), m_nIdRange);

		return _pszPrint;
	}
};
typedef struct CliMsgBodyReqActiveList CliMsgBodyReqActiveList_t;
struct CliMsgBodyRspActiveList
{
	enum
	{
		E_CONST_MAX_ID_NUMBER = 4000,
		E_CONST_MAX_TU_NUMBER = 50,
	};

	eDum::EDialogMethodType_t m_eDialogMethodType;
	int m_nIdRange;
	char m_arrIdUseInfo[E_CONST_MAX_ID_NUMBER];
	int m_nActiveIdNumber;
	int m_arrIdNumberOfTu[E_CONST_MAX_TU_NUMBER];
	int m_nTuNumber;
	
	const char * str(char * _pszPrint, unsigned int _unLen) const
	{
		if( _pszPrint )
			_pszPrint[0] = 0x00;
	
		eSipUtil::strncat(_pszPrint, _unLen, 
												"DialogType[%s], IdRange[%d], ActiveIdNumber[%d], TuNumber[%d]", 
												eDum::getDialogMethodTypeString(m_eDialogMethodType), m_nIdRange, m_nActiveIdNumber, m_nTuNumber);

		return _pszPrint;
	}
};
typedef struct CliMsgBodyRspActiveList CliMsgBodyRspActiveList_t;

typedef void (* PfuncProcessRcvMsg)(int _nFd, CliMsg_t * _pstCliMsg);

inline bool makeCliMsgHeader(ECliMsgType_t _eCliMsgType, CliMsg_t & _rstCliMsg, unsigned int & _unMsgLen)
{
	memset(&_rstCliMsg, 0x00, sizeof(CliMsg_t));
	memcpy(_rstCliMsg.m_stHeader.m_szMagicCookie, s_pszMagicCookie, E_CONST_CLI_MAGIC_LEN);
	_rstCliMsg.m_stHeader.m_eMsgType = _eCliMsgType;
	_unMsgLen = sizeof(CliMsgHeader_t);
	
	switch(_eCliMsgType)
	{
		case E_CLI_MSG_TYPE_REQ_SESSION_NUM :
		case E_CLI_MSG_TYPE_REQ_DIALOG_NUM :
		{
			_rstCliMsg.m_stHeader.m_unBodyLen = sizeof(CliMsgBodyReqActiveIdNum_t);
		}
		break;
		case E_CLI_MSG_TYPE_RSP_SESSION_NUM :
		case E_CLI_MSG_TYPE_RSP_DIALOG_NUM :
		{
			_rstCliMsg.m_stHeader.m_unBodyLen = sizeof(CliMsgBodyRspActiveIdNum_t);
		}
		break;
		case E_CLI_MSG_TYPE_REQ_SESSION_ACTIVE_LIST :
		case E_CLI_MSG_TYPE_REQ_DIALOG_ACTIVE_LIST :
		{
			_rstCliMsg.m_stHeader.m_unBodyLen = sizeof(CliMsgBodyReqActiveList_t);
		}
		break;
		case E_CLI_MSG_TYPE_RSP_SESSION_ACTIVE_LIST :
		case E_CLI_MSG_TYPE_RSP_DIALOG_ACTIVE_LIST :
		{
			_rstCliMsg.m_stHeader.m_unBodyLen = sizeof(CliMsgBodyRspActiveList_t);
		}
		break;
		case E_CLI_MSG_TYPE_REQ_SESSION_DESC :
		case E_CLI_MSG_TYPE_REQ_DIALOG_DESC :
		{
			_rstCliMsg.m_stHeader.m_unBodyLen = sizeof(CliMsgBodyReqIdDesc_t);
		}
		break;
		case E_CLI_MSG_TYPE_RSP_SESSION_DESC :
		case E_CLI_MSG_TYPE_RSP_DIALOG_DESC :
		{
			_rstCliMsg.m_stHeader.m_unBodyLen = sizeof(CliMsgBodyRspIdDesc_t);
		}
		break;
		default :
		{
			_unMsgLen = 0;
			return false;
		}
		break;
	}

	_unMsgLen += _rstCliMsg.m_stHeader.m_unBodyLen;
	return true;
}
		
inline int checkCliMsg(const char * _pSrcData, unsigned int _unSrcLen)
{
	if( !_pSrcData || _unSrcLen == 0 )
		return -1;

	if( _unSrcLen < sizeof(CliMsgHeader_t) )
		return 0;

	if( memcmp(_pSrcData, s_pszMagicCookie, strlen(s_pszMagicCookie)) != 0 )
		return -1;
	
	return sizeof(CliMsgHeader_t) + ((const CliMsgHeader_t *)_pSrcData)->m_unBodyLen;
}

}

#endif
