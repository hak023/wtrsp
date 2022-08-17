
#ifndef SLB_CCM_MSG_H
#define SLB_CCM_MSG_H

#include <string.h>
#include <stdio.h>

#include "eSipUtil/define.hxx"
#include "eSipUtil/string.hxx"

#define MAGIC_COOKIE 						"ezkib#1"
#define MSG_MAGIC_COOKIE_LEN 		7
//added by khd for multiple via 20181107
#define MAX_ADDITION_VIA_NUMS		7

enum
{
	E_CONST_MAX_CCMID_STR_SIZE = 50,
	E_CONST_MAX_TAG_PREFIX_LEN = 50,
};

struct SlbInternalSipStrData
{
	unsigned int m_unStartIndex;
	unsigned int m_unLen;

	SlbInternalSipStrData()
	{
		m_unStartIndex = 0;
		m_unLen = 0;
	}
};

typedef enum
{
	E_INTERNAL_MSG_TYPE_INIT_REQ = 0,
	E_INTERNAL_MSG_TYPE_INIT_RSP,
	E_INTERNAL_MSG_TYPE_RCV_SIP,
	E_INTERNAL_MSG_TYPE_SND_SIP,
	E_INTERNAL_MSG_TYPE_PING_REQ,
	E_INTERNAL_MSG_TYPE_PING_RSP,
	E_INTERNAL_MSG_TYPE_NOTI_RTE_STATUS,

	E_INTERNAL_MSG_TYPE_NUM,
}EInternalMsgType_t;

typedef enum
{
	E_PARSE_HEADER_STAUS_LINE = 0,
	E_PARSE_HEADER_TO,
	E_PARSE_HEADER_FROM,
	E_PARSE_HEADER_CSEQ,
	E_PARSE_HEADER_CALLID,
	E_PARSE_HEADER_ACCEPTCONTACT,
	E_PARSE_HEADER_VIA,
	E_PARSE_HEADER_MAX_FORWARDS,
	E_PARSE_HEADER_CONTENT_LENGTH,

	E_PARSE_HEADER_NUM,	
}EParseHeader_t;

typedef enum
{
	E_INTERNAL_SIP_STR_TYPE_FROM_USERNAME = 0,
	E_INTERNAL_SIP_STR_TYPE_FROM_PHONECTX,	
	E_INTERNAL_SIP_STR_TYPE_FROM_TAG,
	E_INTERNAL_SIP_STR_TYPE_TO_USERNAME,
	E_INTERNAL_SIP_STR_TYPE_TO_PHONECTX,	
	E_INTERNAL_SIP_STR_TYPE_TO_TAG,
	E_INTERNAL_SIP_STR_TYPE_CALLID,
	E_INTERNAL_SIP_STR_TYPE_VIAIP,
	E_INTERNAL_SIP_STR_TYPE_BRANCH,

	E_INTERNAL_SIP_STR_TYPE_NUM,
}ESlbInternalSipStrType_t;

/*==== equal to MethodTypes of Resip ====*/
typedef enum
{
	E_INTERNAL_SIP_METHOD_UNKNOWN = 0,		//unused
	E_INTERNAL_SIP_METHOD_ACK,
	E_INTERNAL_SIP_METHOD_BYE,
	E_INTERNAL_SIP_METHOD_CANCEL,
	E_INTERNAL_SIP_METHOD_INVITE,
	E_INTERNAL_SIP_METHOD_NOTIFY,
	E_INTERNAL_SIP_METHOD_OPTIONS,
	E_INTERNAL_SIP_METHOD_REFER,
	E_INTERNAL_SIP_METHOD_REGISTER,
	E_INTERNAL_SIP_METHOD_SUBSCRIBE,
	E_INTERNAL_SIP_METHOD_RESPONSE,			//unused
	E_INTERNAL_SIP_METHOD_MESSAGE,
	E_INTERNAL_SIP_METHOD_INFO,
	E_INTERNAL_SIP_METHOD_PRACK,
	E_INTERNAL_SIP_METHOD_PUBLISH,
	E_INTERNAL_SIP_METHOD_SERVICE,
	E_INTERNAL_SIP_METHOD_UPDATE,

	E_INTERNAL_SIP_METHOD_NUM,
}ESlbInternalSipMethodType_t;

typedef enum
{
	E_INTERNAL_MSG_MATCH_RESULT_SUCC = 0,
	E_INTERNAL_MSG_MATCH_RESULT_FAIL_INVALID_ARG,
	E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_LEN,
	E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_BODY_LEN,
	E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_START_MAGIC,

	E_INTERNAL_MSG_MATCH_RESULT_NUM,
}EInternalMsgMatchResult_t;

class TypeInfo
{
	public :
		enum
		{
			E_CONST_MAX_MSG_TYPE__STR_LEN = 15,
			E_CONST_MAX_MATCH_RESULT_STR_LEN = 50,
			E_CONST_MAX_SIP_METHOD_STR_LEN = 15,
		};

		static const char * m_fnGetInternalMsgTypeStr(EInternalMsgType_t _eType)
		{
			switch(_eType)
			{
				case E_INTERNAL_MSG_TYPE_INIT_REQ : return "INIT_REQ"; break;
				case E_INTERNAL_MSG_TYPE_INIT_RSP : return "INIT_RSP"; break;
				case E_INTERNAL_MSG_TYPE_RCV_SIP : return "RCV_SIP"; break;
				case E_INTERNAL_MSG_TYPE_SND_SIP : return "SND_SIP"; break;
				case E_INTERNAL_MSG_TYPE_PING_REQ : return "PING_REQ"; break;
				case E_INTERNAL_MSG_TYPE_PING_RSP : return "PING_RSP"; break;
				case E_INTERNAL_MSG_TYPE_NOTI_RTE_STATUS : return "NOTI_RTE_STATUS"; break;
				default : return "Unknown"; break;
			}
		}

		static const char * m_fnGetInternalMatchResultStr(EInternalMsgMatchResult_t _eType)
		{
			switch(_eType)
			{
				case E_INTERNAL_MSG_MATCH_RESULT_SUCC : return "MATCH_Succ"; break;
				case E_INTERNAL_MSG_MATCH_RESULT_FAIL_INVALID_ARG : return "MATCH_invalid_arg"; break;
				case E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_LEN : return "MATCH_incorrect_len"; break;
				case E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_BODY_LEN : return "MATCH_incorrect_bodyLen"; break;
				case E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_START_MAGIC : return "MATCH_incorrect_startMagic"; break;
				default : return "Unknown"; break;
			}
		}

		static const char * m_fnGetInternalSipMethodTypeStr(ESlbInternalSipMethodType_t _eType)
		{
			switch(_eType)
			{
				case E_INTERNAL_SIP_METHOD_ACK : return "ACK"; break;
				case E_INTERNAL_SIP_METHOD_BYE : return "BYE"; break;
				case E_INTERNAL_SIP_METHOD_CANCEL : return "CANCEL"; break;
				case E_INTERNAL_SIP_METHOD_INVITE : return "INVITE"; break;
				case E_INTERNAL_SIP_METHOD_NOTIFY : return "NOTIFY"; break;
				case E_INTERNAL_SIP_METHOD_OPTIONS : 	return "OPTIONS"; break;
				case E_INTERNAL_SIP_METHOD_REFER : return "REFER"; break;
				case E_INTERNAL_SIP_METHOD_REGISTER : return "REGISTER"; break;
				case E_INTERNAL_SIP_METHOD_SUBSCRIBE : return "SUBSCRIBE"; break;
				case E_INTERNAL_SIP_METHOD_MESSAGE : return "MESSAGE"; break;
				case E_INTERNAL_SIP_METHOD_INFO : return "INFO"; break;
				case E_INTERNAL_SIP_METHOD_PRACK : return "PRACK"; break;
				case E_INTERNAL_SIP_METHOD_PUBLISH : return "PUBLISH"; break;
				case E_INTERNAL_SIP_METHOD_SERVICE : return "SERVICE"; break;
				case E_INTERNAL_SIP_METHOD_UPDATE : return "UPDATE"; break;
				default : return "Unknown"; break;
			}
		}
};

struct SlbInternalSipInfo
{
	enum
	{
		E_CONST_MAX_PRINT_LEN = 150,
	};

	/*==== informatoin of scan ====*/
	SlbInternalSipStrData m_arrHeaderScanInfo[E_PARSE_HEADER_NUM];					//top header of multiHeader
	//added by khd for multiple via 20181107
	SlbInternalSipStrData m_arrAdditionViaScanInfo[MAX_ADDITION_VIA_NUMS];	//additional via headers
	unsigned int m_unAdditionViaNums;																			//additional via headers
	unsigned int m_unTotalHeaderLen;

	/*==== informatoin of SIP ====*/
	ESlbInternalSipMethodType_t m_eMethodType;
	int m_nRspCode;
	bool m_bInitial;
	SlbInternalSipStrData m_arrSipStrData[E_INTERNAL_SIP_STR_TYPE_NUM];
	bool m_bVideo;		//true : video, false : audio
	int m_nViaPort;
	unsigned int m_unMaxForwards;

	/*==== information of etc ====*/
	bool m_bTcpRelay;

	void m_fnReset()
	{
		memset(m_arrHeaderScanInfo, 0x00, sizeof(SlbInternalSipStrData)*E_PARSE_HEADER_NUM);
		//added by khd for multiple via 20181107
		memset(m_arrAdditionViaScanInfo, 0x00, sizeof(SlbInternalSipStrData)*MAX_ADDITION_VIA_NUMS);
		m_unAdditionViaNums = 0;
		m_unTotalHeaderLen = 0;
	
		m_eMethodType = E_INTERNAL_SIP_METHOD_NUM;
		m_nRspCode = -1;
		m_bInitial = false;
		memset(m_arrSipStrData, 0x00, sizeof(SlbInternalSipStrData)*E_INTERNAL_SIP_STR_TYPE_NUM);
		m_bVideo = false;
		m_nViaPort = -1;
		m_unMaxForwards = 0;

		m_bTcpRelay = false;
	}
	
	SlbInternalSipInfo()
	{
		m_fnReset();
	}

	void m_fnSetHeaderScanInfo(const char * _pszSipMsg, EParseHeader_t _eParseHdr, const char * _pHdrPos, unsigned int _unHdrLen)
	{
		if( !_pszSipMsg ) 
			return;

		if( _eParseHdr >= 0 && _eParseHdr < E_PARSE_HEADER_NUM )
		{
			m_arrHeaderScanInfo[_eParseHdr].m_unStartIndex = _pHdrPos - _pszSipMsg;
			m_arrHeaderScanInfo[_eParseHdr].m_unLen = _unHdrLen;
		}
	}

	//added by khd for multiple via 20181107
	void m_fnSetAdditionViaScanInfo(const char * _pszSipMsg, const char * _pHdrPos, unsigned int _unHdrLen)
	{
		if( !_pszSipMsg ) 
			return;

		if( m_unAdditionViaNums >= MAX_ADDITION_VIA_NUMS )
			return;

		m_arrAdditionViaScanInfo[m_unAdditionViaNums].m_unStartIndex = _pHdrPos - _pszSipMsg;
		m_arrAdditionViaScanInfo[m_unAdditionViaNums].m_unLen = _unHdrLen;

		++m_unAdditionViaNums;
	}

	const char * m_fnGetHeaderScanInfo(const char * _pszSipMsg, EParseHeader_t _eParseHdr, unsigned int & _runHdrLen) const
	{
		if( !_pszSipMsg ) 
			return NULL;

		if( _eParseHdr >= 0 && _eParseHdr < E_PARSE_HEADER_NUM )
		{
			_runHdrLen = m_arrHeaderScanInfo[_eParseHdr].m_unLen;
			return m_arrHeaderScanInfo[_eParseHdr].m_unStartIndex + _pszSipMsg;
		}
		else
			return NULL;
	}

	//added by khd for multiple via 20181107
	const char * m_fnGetAdditionViaScanInfo(const char * _pszSipMsg, unsigned int _unAdditionViaIdx, unsigned int & _runHdrLen) const
	{
		if( !_pszSipMsg )
			return NULL;

		if( _unAdditionViaIdx >= MAX_ADDITION_VIA_NUMS )
			return NULL;

		_runHdrLen = m_arrAdditionViaScanInfo[_unAdditionViaIdx].m_unLen;
		return m_arrAdditionViaScanInfo[_unAdditionViaIdx].m_unStartIndex + _pszSipMsg;
	}

	bool m_fnGetHeaderStr(const char * _pszSipMsg, ESlbInternalSipStrType_t _eStrType, char * _pszOut, unsigned int _unMaxOutLen) const
	{
		if( !_pszSipMsg || !_pszOut || _unMaxOutLen == 0 )
			return false;

		if( _eStrType >= 0 && _eStrType < E_INTERNAL_SIP_STR_TYPE_NUM 
			&& m_arrSipStrData[_eStrType].m_unStartIndex > 0 && m_arrSipStrData[_eStrType].m_unLen > 0 )
		{
			unsigned int unStrLen = (m_arrSipStrData[_eStrType].m_unLen > _unMaxOutLen-1) ? _unMaxOutLen-1 : m_arrSipStrData[_eStrType].m_unLen;
			_pszOut[0] = 0x00;
			strncpy(_pszOut, _pszSipMsg+m_arrSipStrData[_eStrType].m_unStartIndex, unStrLen);
			_pszOut[unStrLen] = 0x00;
			return true;
		}
		else
			return false;
	}

	const char * m_fnGetStr(char * _pszStr, unsigned int _unLen, bool _bInitial) const
	{
		static const char * pszBlank = "";

		if( !_pszStr || _unLen == 0 )
			return pszBlank;

		if( _bInitial )
			_pszStr[0] = 0x00;
	
		eSipUtil::strncat(_pszStr, _unLen, "SipInfo(MethodType:%d, RspCode:%d, Initial:%d, Video:%d, ViaPort:%d, MaxForwards:%u, TcpRelay:%d)",
																m_eMethodType, m_nRspCode, m_bInitial, m_bVideo, m_nViaPort, m_unMaxForwards, m_bTcpRelay);

		return _pszStr;
	}
};

struct SlbInitReqBody
{
	unsigned int m_unMaxSessionNum;
	char m_szCcmId[E_CONST_MAX_CCMID_STR_SIZE];
	char m_szTagPrefix[E_CONST_MAX_TAG_PREFIX_LEN];
	char m_szStackTagPrefix[E_CONST_MAX_TAG_PREFIX_LEN];

	void m_fnSetCcmId(const char * _pszId)
	{
		if( _pszId )
		{
			m_szCcmId[0] = 0x00;
			strncpy(m_szCcmId, _pszId, E_CONST_MAX_CCMID_STR_SIZE-1);
			m_szCcmId[E_CONST_MAX_CCMID_STR_SIZE-1] = 0x00;
		}
	}

	void m_fnSetTagPrefix(const char * _pszTag)
	{
		if( _pszTag )
		{
			m_szTagPrefix[0] = 0x00;
			strncpy(m_szTagPrefix, _pszTag, E_CONST_MAX_TAG_PREFIX_LEN-1);
			m_szTagPrefix[E_CONST_MAX_TAG_PREFIX_LEN-1] = 0x00;
		}
	}

	void m_fnSetStackTagPrefix(const char * _pszTag)
	{
		if( _pszTag )
		{
			m_szStackTagPrefix[0] = 0x00;
			strncpy(m_szStackTagPrefix, _pszTag, E_CONST_MAX_TAG_PREFIX_LEN-1);
			m_szStackTagPrefix[E_CONST_MAX_TAG_PREFIX_LEN-1] = 0x00;
		}
	}

	void m_fnReset()
	{
		m_unMaxSessionNum = 0;
		memset(m_szCcmId, 0x00, E_CONST_MAX_CCMID_STR_SIZE);
		memset(m_szTagPrefix, 0x00, E_CONST_MAX_TAG_PREFIX_LEN);
		memset(m_szStackTagPrefix, 0x00, E_CONST_MAX_TAG_PREFIX_LEN);
	}

	unsigned int m_fnGetBodyLen() const
	{
		return sizeof(SlbInitReqBody);
	}

	EInternalMsgMatchResult_t m_fnIsMatch(unsigned int _unBodyLen) const
	{
		if( m_fnGetBodyLen() != _unBodyLen )
			return E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_BODY_LEN;
	
		return E_INTERNAL_MSG_MATCH_RESULT_SUCC;
	}

	const char * m_fnGetStr(char * _pszStr, unsigned int _unLen, bool _bInitial) const
	{
		static const char * pszBlank = "";

		if( !_pszStr || _unLen == 0 )
			return pszBlank;

		if( _bInitial )
			_pszStr[0] = 0x00;
	
		eSipUtil::strncat(_pszStr, _unLen, "InternalMsg(Type:INIT_REQ, MaxSessNum:%u, CcmId:%s, TagPrefix:%s, StackTagPrefix:%s)", 
																m_unMaxSessionNum, m_szCcmId, m_szTagPrefix, m_szStackTagPrefix);

		return _pszStr;
	}
};

struct SlbInitRspBody
{
	typedef enum
	{
		E_INIT_RSP_TYPE_SUCC = 0,
		E_INIT_RSP_TYPE_FAIL_INVALID_CCMID,
		E_INIT_RSP_TYPE_FAIL_ALREADY_CCMID,
		E_INIT_RSP_TYPE_FAIL_UNMATCH_CCMID,
		E_INIT_RSP_TYPE_FAIL_UNKNOWN_TUPLE,
		E_INIT_RSP_TYPE_FAIL_INVALID_CON_STATE,
		E_INIT_RSP_TYPE_FAIL_INTERNAL_ERR,
		
		E_INIT_RSP_TYPE_NUM,
	}EInitRspType_t;

	enum
	{
		E_CONST_MAX_INIT_RSP_TYPE_STR_LEN = 30,
	};

	EInitRspType_t m_eInitRspType;

	static const char * m_fnGetInitRspTypeStr(EInitRspType_t _eType)
	{
		switch(_eType)
		{
			case E_INIT_RSP_TYPE_SUCC : return "INIT_SUCC"; break;
			case E_INIT_RSP_TYPE_FAIL_INVALID_CCMID : return "INIT_invalid_ccmid"; break;
			case E_INIT_RSP_TYPE_FAIL_ALREADY_CCMID : return "INIT_already_ccmid"; break;
			case E_INIT_RSP_TYPE_FAIL_UNMATCH_CCMID : return "INIT_unmatch_ccmid"; break;
			case E_INIT_RSP_TYPE_FAIL_UNKNOWN_TUPLE : return "INIT_unknown_tuple"; break;
			case E_INIT_RSP_TYPE_FAIL_INVALID_CON_STATE : return "INIT_invalid_conState"; break;
			case E_INIT_RSP_TYPE_FAIL_INTERNAL_ERR : return "INIT_internal_error"; break;
			default : return "unknown"; break;
		}
	}
	
	void m_fnReset()
	{
		m_eInitRspType = E_INIT_RSP_TYPE_NUM;
	}

	unsigned int m_fnGetBodyLen() const
	{
		return sizeof(SlbInitRspBody);
	}

	EInternalMsgMatchResult_t m_fnIsMatch(unsigned int _unBodyLen) const
	{
		if( m_fnGetBodyLen() != _unBodyLen )
			return E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_BODY_LEN;
	
		return E_INTERNAL_MSG_MATCH_RESULT_SUCC;
	}

	const char * m_fnGetStr(char * _pszStr, unsigned int _unLen, bool _bInitial) const
	{
		static const char * pszBlank = "";

		if( !_pszStr || _unLen == 0 )
			return pszBlank;

		if( _bInitial )
			_pszStr[0] = 0x00;
	
		eSipUtil::strncat(_pszStr, _unLen, "InternalMsg(Type:INIT_RSP, SubType:%s)", m_fnGetInitRspTypeStr(m_eInitRspType));
		
		return _pszStr;
	}
};


struct SlbRcvSipBody
{
	enum
	{
		E_CONST_MAX_CALLID_LEN = 256,
	};
	
	eSipUtil::Net5Tuple m_stRcvTuple;
	char m_szCallId[E_CONST_MAX_CALLID_LEN];
	SlbInternalSipInfo m_stInternalSipInfo;
	unsigned int m_unSipLen;

	void m_fnReset()
	{
		m_stRcvTuple.reset();
		m_szCallId[0] = 0x00;
		m_stInternalSipInfo.m_fnReset();
		m_unSipLen = 0;
	}

	void m_fnSetCallId(const char * _pszCallId)
	{
		if( _pszCallId )
		{
			m_szCallId[0] = 0x00;
			strncpy(m_szCallId, _pszCallId, E_CONST_MAX_CALLID_LEN-1);
			m_szCallId[E_CONST_MAX_CALLID_LEN-1] = 0x00;
		}
	}

	char * m_fnGetSipMsg()
	{
		return (char *)this + sizeof(SlbRcvSipBody);
	}

	const char * m_fnGetSipMsg() const
	{
		return (char *)this + sizeof(SlbRcvSipBody);
	}

	unsigned int m_fnGetBodyLen() const
	{
		return sizeof(SlbRcvSipBody) + m_unSipLen;
	}

	EInternalMsgMatchResult_t m_fnIsMatch(unsigned int _unBodyLen) const
	{
		if( m_fnGetBodyLen() != _unBodyLen )
			return E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_BODY_LEN;
	
		return E_INTERNAL_MSG_MATCH_RESULT_SUCC;
	}

	const char * m_fnGetStr(char * _pszStr, unsigned int _unLen, bool _bInitial) const
	{ 
		static const char * pszBlank = "";

		if( !_pszStr || _unLen == 0 )
			return pszBlank;

		if( _bInitial )
			_pszStr[0] = 0x00;
	
		eSipUtil::strncat(_pszStr, _unLen, "InternalMsg(Type:RCV_SIP, %s(%d), CallId:%s, RcvSipLen:%u, Rcv",
																TypeInfo::m_fnGetInternalSipMethodTypeStr(m_stInternalSipInfo.m_eMethodType), m_stInternalSipInfo.m_nRspCode, m_szCallId, m_unSipLen);
		m_stRcvTuple.getStr(_pszStr, _unLen, false);
		eSipUtil::strncat(_pszStr, _unLen, ")");

		return _pszStr;
	}
};

struct SlbSndSipBody
{
	enum
	{
		E_CONST_MAX_CALLID_LEN = 256,
	};
	
	eSipUtil::Net5Tuple m_stSndTuple;
	ESlbInternalSipMethodType_t m_eMethodType;
	int m_nRspCode;
	char m_szCallId[E_CONST_MAX_CALLID_LEN];
	unsigned int m_unSipLen;

	void m_fnSetCallId(const char * _pszCallId)
	{
		if( _pszCallId )
		{
			m_szCallId[0] = 0x00;
			strncpy(m_szCallId, _pszCallId, E_CONST_MAX_CALLID_LEN-1);
			m_szCallId[E_CONST_MAX_CALLID_LEN-1] = 0x00;
		}
	}

	const char * m_fnGetCallId() const{ return m_szCallId; }

	void m_fnReset()
	{
		m_stSndTuple.reset();
		m_eMethodType = E_INTERNAL_SIP_METHOD_NUM;
		m_nRspCode = -1;
		m_szCallId[0] = 0x00;
		m_unSipLen = 0;
	}

	char * m_fnGetSipMsg()
	{
		return (char *)this + sizeof(SlbSndSipBody);
	}

	const char * m_fnGetSipMsg() const
	{
		return (char *)this + sizeof(SlbSndSipBody);
	}
	
	unsigned int m_fnGetBodyLen() const
	{
		return sizeof(SlbSndSipBody) + m_unSipLen;
	}

	EInternalMsgMatchResult_t m_fnIsMatch(unsigned int _unBodyLen) const
	{
		if( m_fnGetBodyLen() != _unBodyLen )
			return E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_BODY_LEN;
	
		return E_INTERNAL_MSG_MATCH_RESULT_SUCC;
	}

	const char * m_fnGetStr(char * _pszStr, unsigned int _unLen, bool _bInitial) const
	{
		static const char * pszBlank = "";

		if( !_pszStr || _unLen == 0 )
			return pszBlank;

		if( _bInitial )
			_pszStr[0] = 0x00;
	
		eSipUtil::strncat(_pszStr, _unLen, "InternalMsg(Type:SND_SIP, %s(%d), CallId:%s, SndSipLen:%u, Snd",															
																TypeInfo::m_fnGetInternalSipMethodTypeStr(m_eMethodType), m_nRspCode, m_szCallId, m_unSipLen);
		m_stSndTuple.getStr(_pszStr, _unLen, false);
		eSipUtil::strncat(_pszStr, _unLen, ")");

		return _pszStr;
	}
};

struct SlbPingReqBody
{
	char m_szCcmId[E_CONST_MAX_CCMID_STR_SIZE];
	bool m_bActive;
	unsigned int m_unActiveSessionNum;
	
	void m_fnReset()
	{
		m_szCcmId[0] = 0x00;
		m_bActive = false;
		m_unActiveSessionNum = 0;
	}

	void m_fnSetCcmId(const char * _pszId)
	{
		if( _pszId )
		{
			m_szCcmId[0] = 0x00;
			strncpy(m_szCcmId, _pszId, E_CONST_MAX_CCMID_STR_SIZE-1);
			m_szCcmId[E_CONST_MAX_CCMID_STR_SIZE-1] = 0x00;
		}
	}

	unsigned int m_fnGetBodyLen() const
	{
		return sizeof(SlbPingReqBody);
	}

	EInternalMsgMatchResult_t m_fnIsMatch(unsigned int _unBodyLen) const
	{
		if( m_fnGetBodyLen() != _unBodyLen )
			return E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_BODY_LEN;
	
		return E_INTERNAL_MSG_MATCH_RESULT_SUCC;
	}

	const char * m_fnGetStr(char * _pszStr, unsigned int _unLen, bool _bInitial) const
	{
		static const char * pszBlank = "";

		if( !_pszStr || _unLen == 0 )
			return pszBlank;

		if( _bInitial )
			_pszStr[0] = 0x00;
	
		eSipUtil::strncat(_pszStr, _unLen, "InternalMsg(Type:PING_REQ, CcmId:%s, Active:%d, ActiveNum:%u)", m_szCcmId, m_bActive, m_unActiveSessionNum);

		return _pszStr;
	}
};

struct SlbPingRspBody
{
	typedef enum
	{
		E_PING_RSP_TYPE_SUCC = 0,
		E_PING_RSP_TYPE_FAIL_UNMATCH_CCMID,
		E_PING_RSP_TYPE_FAIL_UNKNOWN_TUPLE,
		E_PING_RSP_TYPE_FAIL_INVALID_CON_STATE,
		E_PING_RSP_TYPE_FAIL_INTERNAL_ERR,
		
		E_PING_RSP_TYPE_NUM,
	}EPingRspType_t;

	enum
	{
		E_CONST_MAX_PING_RSP_TYPE_STR_LEN = 30,
	};

	EPingRspType_t m_ePingRspType;

	static const char * m_fnGetPingRspTypeStr(EPingRspType_t _eType)
	{
		switch(_eType)
		{
			case E_PING_RSP_TYPE_SUCC : return "PING_SUCC"; break;
			case E_PING_RSP_TYPE_FAIL_UNMATCH_CCMID : return "PING_unmatch_ccmid"; break;
			case E_PING_RSP_TYPE_FAIL_UNKNOWN_TUPLE : return "PING_unknown_tuple"; break;
			case E_PING_RSP_TYPE_FAIL_INVALID_CON_STATE : return "PING_invalid_conState"; break;
			case E_PING_RSP_TYPE_FAIL_INTERNAL_ERR : return "PING_internal_error"; break;
			default : return "unknown"; break;
		}
	}
	
	void m_fnReset()
	{
		m_ePingRspType = E_PING_RSP_TYPE_NUM;
	}

	unsigned int m_fnGetBodyLen() const
	{
		return sizeof(SlbPingRspBody);
	}

	EInternalMsgMatchResult_t m_fnIsMatch(unsigned int _unBodyLen) const
	{
		if( m_fnGetBodyLen() != _unBodyLen )
			return E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_BODY_LEN;
	
		return E_INTERNAL_MSG_MATCH_RESULT_SUCC;
	}

	const char * m_fnGetStr(char * _pszStr, unsigned int _unLen, bool _bInitial) const
	{
		static const char * pszBlank = "";

		if( !_pszStr || _unLen == 0 )
			return pszBlank;

		if( _bInitial )
			_pszStr[0] = 0x00;
	
		eSipUtil::strncat(_pszStr, _unLen, "InternalMsg(Type:PING_RSP, SubType:%s)", m_fnGetPingRspTypeStr(m_ePingRspType));
		
		return _pszStr;
	}
};

struct SlbNotiRteStatus
{
	eSipUtil::Net5Tuple m_stRteTuple;
	bool m_bAvail;
		
	void m_fnReset()
	{
		m_stRteTuple.reset();
		m_bAvail = false;
	}	

	unsigned int m_fnGetBodyLen() const
	{
		return sizeof(SlbNotiRteStatus);
	}

	EInternalMsgMatchResult_t m_fnIsMatch(unsigned int _unBodyLen) const
	{
		if( m_fnGetBodyLen() != _unBodyLen )
			return E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_BODY_LEN;
	
		return E_INTERNAL_MSG_MATCH_RESULT_SUCC;
	}

	const char * m_fnGetStr(char * _pszStr, unsigned int _unLen, bool _bInitial) const
	{
		static const char * pszBlank = "";

		if( !_pszStr || _unLen == 0 )
			return pszBlank;

		if( _bInitial )
			_pszStr[0] = 0x00;
	
		eSipUtil::strncat(_pszStr, _unLen, "InternalMsg(Type:NOTI_RTE_STATUS, Rte");
		m_stRteTuple.getStr(_pszStr, _unLen, false);
		eSipUtil::strncat(_pszStr, _unLen, ", Avail:%d)", m_bAvail);

		return _pszStr;
	}
};

struct SlbInternalMsgHdr
{
	enum
	{			
		E_CONST_MAX_STR_LEN = 500,
	};

	char m_szStartMagicCookie[MSG_MAGIC_COOKIE_LEN];
	EInternalMsgType_t m_eMsgType;
	unsigned int m_unBodyLen;

	void m_fnReset()
	{
		memcpy(m_szStartMagicCookie, MAGIC_COOKIE, MSG_MAGIC_COOKIE_LEN);
		m_eMsgType = E_INTERNAL_MSG_TYPE_NUM;
		m_unBodyLen = 0;
	}

	static unsigned int m_fnGetHeaderLen()
	{
		static const unsigned int s_unHeaderLen = sizeof(SlbInternalMsgHdr);

		return s_unHeaderLen;
	}

	unsigned int m_fnGetTotalLen() const
	{
		return m_fnGetHeaderLen() + m_unBodyLen;
	}

	void * m_fnGetBody()
	{
		return (char *)this + sizeof(SlbInternalMsgHdr);
	}

	const void * m_fnGetBody() const
	{
		return (char *)this + sizeof(SlbInternalMsgHdr);
	}

	EInternalMsgMatchResult_t m_fnIsMatch(const char * _pBuffer, unsigned int _unBufferLen) const
	{
		if( !_pBuffer || _unBufferLen == 0 )
			return E_INTERNAL_MSG_MATCH_RESULT_FAIL_INVALID_ARG;

		if( _unBufferLen != m_fnGetTotalLen() )
			return E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_LEN;

		if( strncmp(_pBuffer, MAGIC_COOKIE, MSG_MAGIC_COOKIE_LEN) != 0 )
			return E_INTERNAL_MSG_MATCH_RESULT_FAIL_UNMATCH_START_MAGIC;

		switch(m_eMsgType)
		{
			case E_INTERNAL_MSG_TYPE_INIT_REQ :
			{
				const SlbInitReqBody * pstBody = (const SlbInitReqBody *)(m_fnGetBody());
				return pstBody->m_fnIsMatch(m_unBodyLen);
			}
			break;
			case E_INTERNAL_MSG_TYPE_INIT_RSP :
			{
				const SlbInitRspBody * pstBody = (const SlbInitRspBody *)(m_fnGetBody());
				return pstBody->m_fnIsMatch(m_unBodyLen);
			}
			break;
			case E_INTERNAL_MSG_TYPE_RCV_SIP :
			{
				const SlbRcvSipBody * pstBody = (const SlbRcvSipBody *)(m_fnGetBody());
				return pstBody->m_fnIsMatch(m_unBodyLen);
			}
			break;
			case E_INTERNAL_MSG_TYPE_SND_SIP :
			{
				const SlbSndSipBody * pstBody = (const SlbSndSipBody *)(m_fnGetBody());
				return pstBody->m_fnIsMatch(m_unBodyLen);
			}
			break;
			case E_INTERNAL_MSG_TYPE_PING_REQ :
			{
				const SlbPingReqBody * pstBody = (const SlbPingReqBody *)(m_fnGetBody());
				return pstBody->m_fnIsMatch(m_unBodyLen);
			}
			break;
			case E_INTERNAL_MSG_TYPE_PING_RSP :
			{
				const SlbPingRspBody * pstBody = (const SlbPingRspBody *)(m_fnGetBody());
				return pstBody->m_fnIsMatch(m_unBodyLen);
			}
			break;
			case E_INTERNAL_MSG_TYPE_NOTI_RTE_STATUS :
			{
				const SlbNotiRteStatus * pstBody = (const SlbNotiRteStatus *)(m_fnGetBody());
				return pstBody->m_fnIsMatch(m_unBodyLen);
			}
			break;
			default :	break;
		}

		return E_INTERNAL_MSG_MATCH_RESULT_FAIL_INVALID_ARG;
	}

	const char * m_fnGetStr(char * _pszStr, unsigned int _unLen, bool _bInitial) const
	{
		static const char * pszBlank = "";

		if( !_pszStr || _unLen == 0)
			return pszBlank;

		if( _bInitial )
			_pszStr[0] = 0x00;

		switch(m_eMsgType)
		{
			case E_INTERNAL_MSG_TYPE_INIT_REQ :
			{
				const SlbInitReqBody * pstBody = (const SlbInitReqBody *)(m_fnGetBody());
				return pstBody->m_fnGetStr(_pszStr, _unLen, _bInitial);
			}
			break;
			case E_INTERNAL_MSG_TYPE_INIT_RSP :
			{
				const SlbInitRspBody * pstBody = (const SlbInitRspBody *)(m_fnGetBody());
				return pstBody->m_fnGetStr(_pszStr, _unLen, _bInitial);
			}
			break;
			case E_INTERNAL_MSG_TYPE_RCV_SIP :
			{
				const SlbRcvSipBody * pstBody = (const SlbRcvSipBody *)(m_fnGetBody());
				return pstBody->m_fnGetStr(_pszStr, _unLen, _bInitial);
			}
			break;
			case E_INTERNAL_MSG_TYPE_SND_SIP :
			{
				const SlbSndSipBody * pstBody = (const SlbSndSipBody *)(m_fnGetBody());
				return pstBody->m_fnGetStr(_pszStr, _unLen, _bInitial);
			}
			break;
			case E_INTERNAL_MSG_TYPE_PING_REQ :
			{
				const SlbPingReqBody * pstBody = (const SlbPingReqBody *)(m_fnGetBody());
				return pstBody->m_fnGetStr(_pszStr, _unLen, _bInitial);
			}
			break;
			case E_INTERNAL_MSG_TYPE_PING_RSP :
			{
				const SlbPingRspBody * pstBody = (const SlbPingRspBody *)(m_fnGetBody());
				return pstBody->m_fnGetStr(_pszStr, _unLen, _bInitial);
			}
			break;
			case E_INTERNAL_MSG_TYPE_NOTI_RTE_STATUS :
			{
				const SlbNotiRteStatus * pstBody = (const SlbNotiRteStatus *)(m_fnGetBody());
				return pstBody->m_fnGetStr(_pszStr, _unLen, _bInitial);
			}
			break;
			default :	break;
		}
		return _pszStr;
	}
};

#endif

