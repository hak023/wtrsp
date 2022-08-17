
#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#ifdef ESIP_IN_RESIP

#include <string.h>
#include <stdlib.h>

#include "rutil/Logger.hxx"

#include "eSipUtil/string.hxx"

#include "eSipSimpleParsing.hxx"

namespace resip
{

static const char * g_pszWhiteSpaceList = " \t";
static const char * g_pszWhiteSpaceAllList = " \t\r\n";
static const char * g_pszDigitList = "0123456789";
static const char * g_pszCRLF = "\r\n";
static const char * g_pszColon = ": ";
static const char * g_pszSemiColon = ";";
static const char * g_pszEqual = "=";
static const unsigned int s_unMaxResponseSize = 10000;

typedef enum
{
	E_PARAM_TYPE_NOVALUE = 0,
	E_PARAM_TYPE_VALUE,
}EParamType;

typedef bool (* PfuncParsingHdr)(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstParsingData, void * _pEtc);

typedef struct 
{
	EParseHeader_t m_eParseHeader;
	const char * m_pszStr;
	unsigned int m_unStrLen;
	const char * m_pszStr2;
	unsigned int m_unStrLen2;
	PfuncParsingHdr m_pfnParsingHdr;
}ParseHeaderInfo_t;

typedef struct 
{
	EParseParam_t m_eParseParam;
	const char * m_pszStr;
	unsigned int m_unStrLen;
	EParamType m_eParamType;
}ParseParamInfo_t;

typedef struct 
{
	EParseUriSchem_t m_eParseUriSchem;
	const char * m_pszStr;
	unsigned int m_unStrLen;
}ParseUriSchemInfo_t;

typedef struct 
{
	EParseViaTransport_t m_eParseViaTransport;
	const char * m_pszStr;
	unsigned int m_unStrLen;
}ParseViaTransportInfo_t;

static const ParseHeaderInfo_t s_arrParseHeaderInfo[E_PARSE_HEADER_NUM] =
{
	{E_PARSE_HEADER_STAUS_LINE,				"SIP/2.0",					strlen("SIP/2.0"),				"SIPS/2.0",	strlen("SIPS/2.0"),	SimpleParsing::parsingStartLine},
	{E_PARSE_HEADER_TO,							"To",							strlen("To"), 						"",					0,								SimpleParsing::parsingTo},
	{E_PARSE_HEADER_FROM,						"From",						strlen("From"), 					"",					0,								SimpleParsing::parsingFrom},
	{E_PARSE_HEADER_CSEQ,						"CSeq",						strlen("CSeq"), 					"",					0,								SimpleParsing::parsingCSeq},
	{E_PARSE_HEADER_CALLID,						"Call-ID",					strlen("Call-ID"), 				"",					0,								SimpleParsing::parsingCallId},
	{E_PARSE_HEADER_ACCEPTCONTACT,		"Accept-Contact",		strlen("Accept-Contact"), 	"",					0,								SimpleParsing::parsingAcceptContact},
	{E_PARSE_HEADER_VIA,							"Via",						strlen("Via"), 					"",					0,								SimpleParsing::parsingVia},
	{E_PARSE_HEADER_MAX_FORWARDS,		"",								0, 										"",					0,								NULL},
	{E_PARSE_HEADER_CONTENT_LENGTH,	"",								0, 										"",					0,								NULL},
};

static const ParseParamInfo_t s_arrParseParamInfo[E_PARSE_PARAM_NUM] =
{
	{E_PARSE_PARAM_TAG,						"tag",					strlen("tag")						,E_PARAM_TYPE_VALUE			},
	{E_PARSE_PARAM_PHONECONTEXT,		"phone-context",	strlen("phone-context")	,E_PARAM_TYPE_VALUE			},
	{E_PARSE_PARAM_VIDEO,					"video",					strlen("video")					,E_PARAM_TYPE_NOVALUE		},
};

static const ParseUriSchemInfo_t s_arrParseUriSchemInfo[E_PARSE_URI_NUM] =
{
	{E_PARSE_URI_SIP,								"sip",				strlen("sip") 		},
	{E_PARSE_URI_SIPS,							"sips",			strlen("sips")		},
	{E_PARSE_URI_TEL,								"tel",				strlen("tel")		},
	{E_PARSE_URI_URN,							"urn",			strlen("urn")		},
};

static const ParseViaTransportInfo_t s_arrParseViaTransportInfo[E_PARSE_VIA_TRANSPORT_NUM] =
{
	{E_PARSE_VIA_TRANSPORT_UDP,			"SIP/2.0/UDP",			strlen("SIP/2.0/UDP") 	},
	{E_PARSE_VIA_TRANSPORT_TCP,			"SIP/2.0/TCP",			strlen("SIP/2.0/TCP")		},
	{E_PARSE_VIA_TRANSPORT_TLS,			"SIP/2.0/TLS",			strlen("SIP/2.0/TLS")		},
};

void g_fnGetResponseCodeDesc(int _nRspCode, char * _pszStr, unsigned int _unLen, bool _bInitial)
{
	if( !_pszStr || _unLen == 0)
		return;

	if( _bInitial )
		_pszStr[0] = 0x00;

	const char * pszTmp = "";

	switch(_nRspCode)
	{
		case 100: pszTmp = "Trying"; break;
		case 180: pszTmp = "Ringing"; break;
		case 181: pszTmp = "Call Is Being Forwarded"; break;
		case 182: pszTmp = "Queued"; break;
		case 183: pszTmp = "Session Progress"; break;
		case 200: pszTmp = "OK"; break;
		case 202: pszTmp = "Accepted"; break;
		case 300: pszTmp = "Multiple Choices"; break;
		case 301: pszTmp = "Moved Permanently"; break;
		case 302: pszTmp = "Moved Temporarily"; break;
		case 305: pszTmp = "Use Proxy"; break;
		case 380: pszTmp = "Alternative Service"; break;
		case 400: pszTmp = "Bad Request"; break;
		case 401: pszTmp = "Unauthorized"; break;
		case 402: pszTmp = "Payment Required"; break;
		case 403: pszTmp = "Forbidden"; break;
		case 404: pszTmp = "Not Found"; break;
		case 405: pszTmp = "Method Not Allowed"; break;
		case 406: pszTmp = "Not Acceptable"; break;
		case 407: pszTmp = "Proxy Authentication Required"; break;
		case 408: pszTmp = "Request Timeout"; break;
		case 410: pszTmp = "Gone"; break;
		case 412: pszTmp = "Precondition Failed"; break;
		case 413: pszTmp = "Request Entity Too Large"; break;
		case 414: pszTmp = "Request-URI Too Long"; break;
		case 415: pszTmp = "Unsupported Media Type"; break;
		case 416: pszTmp = "Unsupported URI Scheme"; break;
		case 420: pszTmp = "Bad Extension"; break;
		case 421: pszTmp = "Extension Required"; break;
		case 422: pszTmp = "Session Interval Too Small"; break;
		case 423: pszTmp = "Interval Too Brief"; break;
		case 430: pszTmp = "Flow failed"; break;
		case 439: pszTmp = "First Hop Lacks Outbound Support"; break;
		case 480: pszTmp = "Temporarily Unavailable"; break;
		case 481: pszTmp = "Call/Transaction Does Not Exist"; break;
		case 482: pszTmp = "Loop Detected"; break;
		case 483: pszTmp = "Too Many Hops"; break;
		case 484: pszTmp = "Address Incomplete"; break;
		case 485: pszTmp = "Ambiguous"; break;
		case 486: pszTmp = "Busy Here"; break;
		case 487: pszTmp = "Request Terminated"; break;
		case 488: pszTmp = "Not Acceptable Here"; break;
		case 489: pszTmp = "Event Package Not Supported"; break;
		case 491: pszTmp = "Request Pending"; break;
		case 493: pszTmp = "Undecipherable"; break;
		case 500: pszTmp = "Server Internal Error"; break;
		case 501: pszTmp = "Not Implemented"; break;
		case 502: pszTmp = "Bad Gateway"; break;
		case 503: pszTmp = "Service Unavailable"; break;
		case 504: pszTmp = "Server Time-out"; break;
		case 505: pszTmp = "Version Not Supported"; break;
		case 513: pszTmp = "Message Too Large"; break;
		case 600: pszTmp = "Busy Everywhere"; break;
		case 603: pszTmp = "Decline"; break;
		case 604: pszTmp = "Does Not Exist Anywhere"; break;
		case 606: pszTmp = "Not Acceptable"; break;
		default : break;
	}

	eSipUtil::strncat(_pszStr, _unLen, pszTmp);
}


unsigned int g_fnGetSipHdrNameLen(EParseHeader_t _eType)
{
	if( _eType >= 0 && _eType < E_PARSE_HEADER_NUM )
		return s_arrParseHeaderInfo[_eType].m_unStrLen;
	else
		return 0;
}

const char * g_fnGetSipHdrStr(EParseHeader_t _eType)
{
	static const char * pszUnknown = "Unknown";
	if( _eType >= 0 && _eType < E_PARSE_HEADER_NUM )
		return s_arrParseHeaderInfo[_eType].m_pszStr;
	else
		return pszUnknown;
}

unsigned int g_fnGetSipParamNameLen(EParseParam_t _eType)
{
	if( _eType >= 0 && _eType < E_PARSE_PARAM_NUM )
		return s_arrParseParamInfo[_eType].m_unStrLen;
	else
		return 0;
}

const char * g_fnGetSipParamStr(EParseParam_t _eType)
{
	static const char * pszUnknown = "Unknown";
	if( _eType >= 0 && _eType < E_PARSE_PARAM_NUM )
		return s_arrParseParamInfo[_eType].m_pszStr;
	else
		return pszUnknown;
}

const char * g_fnGetSipViaTrspStr(EParseViaTransport_t _eType)
{
	static const char * pszUnknown = "Unknown";
	if( _eType >= 0 && _eType < E_PARSE_VIA_TRANSPORT_NUM )
		return s_arrParseViaTransportInfo[_eType].m_pszStr;
	else
		return pszUnknown;
}

const char * g_fnGetSipUriSchemStr(EParseUriSchem_t _eType)
{
	static const char * pszUnknown = "Unknown";
	if( _eType >= 0 && _eType < E_PARSE_URI_NUM )
		return s_arrParseUriSchemInfo[_eType].m_pszStr;
	else
		return pszUnknown;
}

void SimpleParsingData::reset()
{
	m_stScanHeaderData.m_fnReset();
	m_eMethodType = resip::UNKNOWN;
	m_nRspCode = 0;
	m_bInitial = false;
	m_szFromUsername[0] = 0x00;
	m_szFromPhoneCtx[0] = 0X00;
	m_szFromTag[0] = 0x00;
	m_szToUsername[0] = 0x00;
	m_szToPhoneCtx[0] = 0X00;
	m_szToTag[0] = 0x00;
	m_szCallId[0] = 0x00;
	m_bVideo = false;
	m_szViaIp[0] = 0x00;
	m_nViaPort = -1;
}

void SimpleParsingData::m_fnApplyStr(ESlbInternalSipStrType_t _eSipStrType, const char * _pStr, unsigned int _unLen)
{
	unsigned int unApplyLen = 0;

	switch(_eSipStrType)
	{
		case E_INTERNAL_SIP_STR_TYPE_FROM_USERNAME :
		{
			unApplyLen = (E_CONST_URI_USERNAME_SIZE > _unLen) ? _unLen : E_CONST_URI_USERNAME_SIZE-1;
			strncpy(m_szFromUsername, _pStr, unApplyLen);
			m_szFromUsername[unApplyLen] = 0x00;
		}
		break;
		case E_INTERNAL_SIP_STR_TYPE_FROM_PHONECTX :
		{
			unApplyLen = (E_CONST_URI_PARAM_PHONECONTEXT_SIZE > _unLen) ? _unLen : E_CONST_URI_PARAM_PHONECONTEXT_SIZE-1;
			strncpy(m_szFromPhoneCtx, _pStr, unApplyLen);
			m_szFromPhoneCtx[unApplyLen] = 0x00;
		}
		break;
		case E_INTERNAL_SIP_STR_TYPE_FROM_TAG :
		{
			unApplyLen = (E_CONST_URI_PARAM_TAG_SIZE > _unLen) ? _unLen : E_CONST_URI_PARAM_TAG_SIZE-1;
			strncpy(m_szFromTag, _pStr, unApplyLen);
			m_szFromTag[unApplyLen] = 0x00;
		}
		break;
		case E_INTERNAL_SIP_STR_TYPE_TO_USERNAME :
		{
			unApplyLen = (E_CONST_URI_USERNAME_SIZE > _unLen) ? _unLen : E_CONST_URI_USERNAME_SIZE-1;
			strncpy(m_szToUsername, _pStr, unApplyLen);
			m_szToUsername[unApplyLen] = 0x00;
		}
		break;
		case E_INTERNAL_SIP_STR_TYPE_TO_PHONECTX :
		{
			unApplyLen = (E_CONST_URI_PARAM_PHONECONTEXT_SIZE > _unLen) ? _unLen : E_CONST_URI_PARAM_PHONECONTEXT_SIZE-1;
			strncpy(m_szToPhoneCtx, _pStr, unApplyLen);
			m_szToPhoneCtx[unApplyLen] = 0x00;
		}
		break;
		case E_INTERNAL_SIP_STR_TYPE_TO_TAG :
		{
			unApplyLen = (E_CONST_URI_PARAM_TAG_SIZE > _unLen) ? _unLen : E_CONST_URI_PARAM_TAG_SIZE-1;
			strncpy(m_szToTag, _pStr, unApplyLen);
			m_szToTag[unApplyLen] = 0x00;
		}
		break;
		case E_INTERNAL_SIP_STR_TYPE_CALLID :
		{
			unApplyLen = (E_CONST_CALLID_SIZE > _unLen) ? _unLen : E_CONST_CALLID_SIZE-1;
			strncpy(m_szCallId, _pStr, unApplyLen);
			m_szCallId[unApplyLen] = 0x00;
		}
		break;
		case E_INTERNAL_SIP_STR_TYPE_VIAIP :
		{
			unApplyLen = (E_CONST_IP_SIZE > _unLen) ? _unLen : E_CONST_IP_SIZE-1;
			strncpy(m_szViaIp, _pStr, unApplyLen);
			m_szViaIp[unApplyLen] = 0x00;
		}
		break;
		default :	break;
	}
}

void SimpleParsingData::m_fnApply(const char * _pszBuffer, unsigned int _unLen, const SlbInternalSipInfo & _rstInternalSipInfo)
{
	const char * pEndBufferPose = _pszBuffer + _unLen;

	for( int i = 0; i < E_PARSE_HEADER_NUM; ++i )
	{
		m_stScanHeaderData.m_parrTopHeaderStartPos[i] = _rstInternalSipInfo.m_fnGetHeaderScanInfo(_pszBuffer, (EParseHeader_t)i, m_stScanHeaderData.m_arrTopHeaderLen[i]);
	}
	//added by khd for multiple via 20181107
	for( unsigned int i = 0; i < _rstInternalSipInfo.m_unAdditionViaNums; ++i )
	{
		const char * pAdditionViaPos = NULL;
		unsigned int unAdditionViaLen = 0;
		pAdditionViaPos = _rstInternalSipInfo.m_fnGetAdditionViaScanInfo(_pszBuffer, i, unAdditionViaLen);
		if( pAdditionViaPos &&  unAdditionViaLen > 0 )
			m_stScanHeaderData.m_listAdditionViaInfo.push_back(std::make_pair(pAdditionViaPos, unAdditionViaLen));		
	}
	m_stScanHeaderData.m_unTotalHeaderLen = _rstInternalSipInfo.m_unTotalHeaderLen;

	m_eMethodType = (MethodTypes)(_rstInternalSipInfo.m_eMethodType);
	m_nRspCode = _rstInternalSipInfo.m_nRspCode;
	m_bInitial = _rstInternalSipInfo.m_bInitial;

	for( int i = 0; i < E_INTERNAL_SIP_STR_TYPE_NUM; ++i )
	{
		if( _rstInternalSipInfo.m_arrSipStrData[i].m_unStartIndex > 0
			&& _rstInternalSipInfo.m_arrSipStrData[i].m_unLen > 0
			&& (_pszBuffer+_rstInternalSipInfo.m_arrSipStrData[i].m_unStartIndex+_rstInternalSipInfo.m_arrSipStrData[i].m_unLen <= pEndBufferPose) )
		{
			m_fnApplyStr((ESlbInternalSipStrType_t)i, _pszBuffer+_rstInternalSipInfo.m_arrSipStrData[i].m_unStartIndex, _rstInternalSipInfo.m_arrSipStrData[i].m_unLen);
		}
	}

	m_bVideo = _rstInternalSipInfo.m_bVideo;
	m_nViaPort = _rstInternalSipInfo.m_nViaPort;
}

bool SimpleParsingData::m_fnCopyHdrStr(char * _pszDst, unsigned int _unDstMaxLen, EParseHeader_t _eParsingHdr, const char * _pszToTag) const
{
	if( !_pszDst || _unDstMaxLen == 0	|| _eParsingHdr < 0 || _eParsingHdr >= E_PARSE_HEADER_NUM )
	{
		return false;
	}

	unsigned int unDstCurrentLen = strlen(_pszDst);

	if( m_stScanHeaderData.m_parrTopHeaderStartPos[_eParsingHdr] && m_stScanHeaderData.m_arrTopHeaderLen[_eParsingHdr] > 0 )
	{
		//modified by khd for multiple via 20181107
		if( _eParsingHdr == E_PARSE_HEADER_VIA )
		{
			unsigned int unExpectLen = unDstCurrentLen
															+ g_fnGetSipHdrNameLen(E_PARSE_HEADER_VIA)
															+ strlen(g_pszColon)
															+ m_stScanHeaderData.m_arrTopHeaderLen[E_PARSE_HEADER_VIA]
															+ strlen(g_pszCRLF);
			if( unExpectLen < _unDstMaxLen )
			{
				strcat(_pszDst, g_fnGetSipHdrStr(E_PARSE_HEADER_VIA));
				unDstCurrentLen += g_fnGetSipHdrNameLen(E_PARSE_HEADER_VIA);
				strcat(_pszDst, g_pszColon);
				unDstCurrentLen += strlen(g_pszColon);
				memcpy(_pszDst+unDstCurrentLen, m_stScanHeaderData.m_parrTopHeaderStartPos[E_PARSE_HEADER_VIA], m_stScanHeaderData.m_arrTopHeaderLen[E_PARSE_HEADER_VIA]);
				unDstCurrentLen += m_stScanHeaderData.m_arrTopHeaderLen[E_PARSE_HEADER_VIA];
				_pszDst[unDstCurrentLen] = 0x00;
				strcat(_pszDst, g_pszCRLF);
				unDstCurrentLen += strlen(g_pszCRLF);
				
				for( std::list< std::pair<const char *, unsigned int> >::const_iterator listItr = m_stScanHeaderData.m_listAdditionViaInfo.begin(); 
						listItr != m_stScanHeaderData.m_listAdditionViaInfo.end(); ++listItr )
				{
					if( !listItr->first || listItr->second == 0 )
					{
						ErrLog(<<"SimpleParsingData : CopyHdrStr : fail : AdditionVia : invalid ScanData");
						continue;
					}
					
					unExpectLen = unDstCurrentLen
											+ g_fnGetSipHdrNameLen(E_PARSE_HEADER_VIA)
											+ strlen(g_pszColon)
											+ listItr->second
											+ strlen(g_pszCRLF);
					if( unExpectLen < _unDstMaxLen )
					{
						strcat(_pszDst, g_fnGetSipHdrStr(E_PARSE_HEADER_VIA));
						unDstCurrentLen += g_fnGetSipHdrNameLen(E_PARSE_HEADER_VIA);
						strcat(_pszDst, g_pszColon);
						unDstCurrentLen += strlen(g_pszColon);
						memcpy(_pszDst+unDstCurrentLen, listItr->first, listItr->second);
						unDstCurrentLen += listItr->second;
						_pszDst[unDstCurrentLen] = 0x00;
						strcat(_pszDst, g_pszCRLF);
						unDstCurrentLen += strlen(g_pszCRLF);
					}
					else
					{
						ErrLog(<<"SimpleParsingData : CopyHdrStr : fail : "<<g_fnGetSipHdrStr(E_PARSE_HEADER_VIA)
									<<" : Length is over(CurrentLen:"<<unDstCurrentLen<<", HdrNameLen:"<<g_fnGetSipHdrNameLen(E_PARSE_HEADER_VIA)
									<<", HdrLen:"<<listItr->second<<", MaxLen:"<<_unDstMaxLen<<")");
						return false;
					}
				}

				return true;
			}
			else
			{
				ErrLog(<<"SimpleParsingData : CopyHdrStr : fail : "<<g_fnGetSipHdrStr(E_PARSE_HEADER_VIA)
							<<" : Length is over(CurrentLen:"<<unDstCurrentLen<<", HdrNameLen:"<<g_fnGetSipHdrNameLen(E_PARSE_HEADER_VIA)
							<<", HdrLen:"<<m_stScanHeaderData.m_arrTopHeaderLen[E_PARSE_HEADER_VIA]<<", MaxLen:"<<_unDstMaxLen<<")");

				return false;
			}
		}
		else
		{
			unsigned int unExpectLen = unDstCurrentLen+g_fnGetSipHdrNameLen(_eParsingHdr)+strlen(g_pszColon)+m_stScanHeaderData.m_arrTopHeaderLen[_eParsingHdr]+strlen(g_pszCRLF);
			if( _eParsingHdr == E_PARSE_HEADER_TO && _pszToTag && strlen(_pszToTag) > 0 )
			{
				unExpectLen += strlen(g_pszSemiColon);
				unExpectLen += g_fnGetSipParamNameLen(E_PARSE_PARAM_TAG);
				unExpectLen += strlen(g_pszEqual);
				unExpectLen += strlen(_pszToTag);
			}
			
			if( unExpectLen < _unDstMaxLen )
			{
				strcat(_pszDst, g_fnGetSipHdrStr(_eParsingHdr));
				unDstCurrentLen += g_fnGetSipHdrNameLen(_eParsingHdr);
				strcat(_pszDst, g_pszColon);
				unDstCurrentLen += strlen(g_pszColon);
				memcpy(_pszDst+unDstCurrentLen, m_stScanHeaderData.m_parrTopHeaderStartPos[_eParsingHdr], m_stScanHeaderData.m_arrTopHeaderLen[_eParsingHdr]);
				unDstCurrentLen += m_stScanHeaderData.m_arrTopHeaderLen[_eParsingHdr];
				_pszDst[unDstCurrentLen] = 0x00;			
				if( _eParsingHdr == E_PARSE_HEADER_TO && _pszToTag && strlen(_pszToTag) > 0 )
					eSipUtil::strncat(_pszDst, _unDstMaxLen, "%s%s%s%s", g_pszSemiColon, g_fnGetSipParamStr(E_PARSE_PARAM_TAG), g_pszEqual, _pszToTag);
				strcat(_pszDst, g_pszCRLF);
				return true;
			}
			else
			{
				ErrLog(<<"SimpleParsingData : CopyHdrStr : fail : "<<g_fnGetSipHdrStr(_eParsingHdr)
							<<" : Length is over(CurrentLen:"<<unDstCurrentLen<<", HdrNameLen:"<<g_fnGetSipHdrNameLen(_eParsingHdr)
							<<", HdrLen:"<<m_stScanHeaderData.m_arrTopHeaderLen[_eParsingHdr]<<", MaxLen:"<<_unDstMaxLen<<")");

				return false;
			}
		}
	}
	else
	{
		ErrLog(<<"SimpleParsingData : CopyHdrStr : fail : "<<g_fnGetSipHdrStr(_eParsingHdr));

		return false;
	}
}

void SimpleParsingData::log() const
{
	DebugLog(<<"SimpleParsing : MethodType["<<resip::getMethodName(m_eMethodType)<<"], RspCode["<<m_nRspCode<<"], Initial["<<m_bInitial<<"], FromUser["<<m_szFromUsername<<"]"
					<<", FromPhoneCtx["<<m_szFromPhoneCtx<<"], FromTag["<<m_szFromTag<<"], ToUser["<<m_szToUsername<<"], ToPhoneCtx["<<m_szToPhoneCtx<<"], ToTag["<<m_szToTag<<"], CallID["<<m_szCallId<<"]"
					<<", Video["<<m_bVideo<<"], ViaIp["<<m_szViaIp<<"], ViaPort["<<m_nViaPort<<"]");
}

void SimpleParsingData::errlog() const
{
	ErrLog(<<"SimpleParsing : Err : MethodType["<<resip::getMethodName(m_eMethodType)<<"], RspCode["<<m_nRspCode<<"], Initial["<<m_bInitial<<"], FromUser["<<m_szFromUsername<<"]"
				<<", FromPhoneCtx["<<m_szFromPhoneCtx<<"], FromTag["<<m_szFromTag<<"], ToUser["<<m_szToUsername<<"], ToPhoneCtx["<<m_szToPhoneCtx<<"], ToTag["<<m_szToTag<<"], CallID["<<m_szCallId<<"]"
				<<", Video["<<m_bVideo<<"], ViaIp["<<m_szViaIp<<"], ViaPort["<<m_nViaPort<<"]");
}

bool SimpleParsing::parsing(const char * _pszBuffer, unsigned int _unLen, SimpleParsingData_t & _rstSimpleParsingData, TransportType _eTransportType)
{
	if( !_pszBuffer || _unLen == 0 )
		return false;

	const char * pHdrPos = NULL;
	unsigned int unHdrLen = 0;
	bool bResult = false;

	scanHeader(_pszBuffer, _unLen, _rstSimpleParsingData.m_stScanHeaderData);

	for( int i = 0; i < E_PARSE_HEADER_NUM; ++i )
	{
		if( s_arrParseHeaderInfo[i].m_pfnParsingHdr )
		{
			pHdrPos = _rstSimpleParsingData.m_stScanHeaderData.m_parrTopHeaderStartPos[i];
			unHdrLen = _rstSimpleParsingData.m_stScanHeaderData.m_arrTopHeaderLen[i];

			if( i == E_PARSE_HEADER_VIA )
				bResult = s_arrParseHeaderInfo[i].m_pfnParsingHdr(pHdrPos, unHdrLen, _rstSimpleParsingData, &_eTransportType);
			else
				bResult = s_arrParseHeaderInfo[i].m_pfnParsingHdr(pHdrPos, unHdrLen, _rstSimpleParsingData, NULL);

			if( bResult )
				DebugLog(<<"SimpleParsing : Parsing : "<<g_fnGetSipHdrStr((EParseHeader_t)i)<<" : Top Hdr Length["<<unHdrLen<<"]");
			else
				ErrLog(<<"SimpleParsing : Parsing : fail : "<<g_fnGetSipHdrStr((EParseHeader_t)i)<<" : Top Hdr Length["<<unHdrLen<<"]");
		}
	}

	return true;
}

void SimpleParsing::scanHeader(const char * _pszOrigBuffer, unsigned int _unOrigLen, ScanHeaderData & _rstScanHeaderData)
{
	const char * pLineStartPos, * pLineEndPos, * pColonPos;
	eSipUtil::BufferControl clsSipBufferControl(_pszOrigBuffer, _unOrigLen);
	eSipUtil::BufferControl clsHdrBufferControl, clsTmpBufferControl;

	if( !(pLineStartPos = clsSipBufferControl.findStr(g_pszCRLF)) )
		return;

	/*==== scan StartLine ====*/
	_rstScanHeaderData.m_parrTopHeaderStartPos[E_PARSE_HEADER_STAUS_LINE] = _pszOrigBuffer;
	_rstScanHeaderData.m_arrTopHeaderLen[E_PARSE_HEADER_STAUS_LINE] = pLineStartPos - _pszOrigBuffer;
	
	pLineStartPos += strlen(g_pszCRLF);
	clsSipBufferControl.setStartPos(pLineStartPos);

	/*=== scan other Header ====*/
	while( !clsSipBufferControl.eof() )
	{
		if( clsSipBufferControl.isStr(g_pszCRLF) )
			break;

		if( !(pLineEndPos = clsSipBufferControl.findStr(g_pszCRLF)) )
			return;
		pLineStartPos = clsSipBufferControl.getStartPos();		
		clsHdrBufferControl.setBuffer(pLineStartPos, pLineEndPos - pLineStartPos);

		if( !(pColonPos = clsHdrBufferControl.findChar(':')) )
			goto LOOP_NEXT;

		clsTmpBufferControl.setBuffer(pLineStartPos, pColonPos - pLineStartPos);
		clsTmpBufferControl.skipWhitespace(true);

		for( int i = 0; i < E_PARSE_HEADER_NUM; ++i )
		{
			if( strlen(s_arrParseHeaderInfo[i].m_pszStr) > 0
				&& strncasecmp(s_arrParseHeaderInfo[i].m_pszStr, clsTmpBufferControl.getStartPos(), clsTmpBufferControl.getSize()) == 0 )
			{
				if( _rstScanHeaderData.m_parrTopHeaderStartPos[i] == NULL )
				{
					clsHdrBufferControl.setStartPos(pColonPos);
					clsHdrBufferControl.move(1);
					clsHdrBufferControl.skipWhitespace();
					if( clsHdrBufferControl.eof() )
						goto LOOP_NEXT;
				
					_rstScanHeaderData.m_parrTopHeaderStartPos[i] = clsHdrBufferControl.getStartPos();
					_rstScanHeaderData.m_arrTopHeaderLen[i] = pLineEndPos - clsHdrBufferControl.getStartPos();
				}
				//added by khd for multiple via 20181107
				else if( i == E_PARSE_HEADER_VIA )
				{
					clsHdrBufferControl.setStartPos(pColonPos);
					clsHdrBufferControl.move(1);
					clsHdrBufferControl.skipWhitespace();
				
					if( !clsHdrBufferControl.eof() )
						_rstScanHeaderData.m_listAdditionViaInfo.push_back(std::make_pair(clsHdrBufferControl.getStartPos(), pLineEndPos - clsHdrBufferControl.getStartPos()));
				}
				
				break;
			}
		}

LOOP_NEXT :
		pLineEndPos += strlen(g_pszCRLF);
		clsSipBufferControl.setStartPos(pLineEndPos);
	}

	if( !clsSipBufferControl.eof() )
		_rstScanHeaderData.m_unTotalHeaderLen = clsSipBufferControl.getStartPos() - _pszOrigBuffer + strlen(g_pszCRLF);
}

bool SimpleParsing::seekHeader(EParseHeader_t _eParseHeader, eSipUtil::BufferControl & _rclsBufferControl)
{
	if( _rclsBufferControl.eof() || _eParseHeader < 0 || _eParseHeader >= E_PARSE_HEADER_NUM )
		return false;

	const char * pCurrentPos, * pEndPos;
	unsigned int unHeaderLen = strlen("\r\n") + s_arrParseHeaderInfo[_eParseHeader].m_unStrLen;
	char szHeaderName[unHeaderLen+1];
	szHeaderName[0] = 0x00;
	snprintf(szHeaderName, unHeaderLen, "\r\n%s", s_arrParseHeaderInfo[_eParseHeader].m_pszStr);

	while( !_rclsBufferControl.eof() )
	{
		if( (pCurrentPos = _rclsBufferControl.findStr(szHeaderName)) )
		{
			_rclsBufferControl.setStartPos(pCurrentPos);
			_rclsBufferControl.move(unHeaderLen);
			_rclsBufferControl.skipWhitespace();
			if( _rclsBufferControl.isChar(':') )
			{
				_rclsBufferControl.move(1);
				if( (pEndPos = _rclsBufferControl.findStr("\r\n")) )
				{
					_rclsBufferControl.setEndPos(pEndPos);
					_rclsBufferControl.skipWhitespace(true);
					_rclsBufferControl.skipWhitespace();
					return !_rclsBufferControl.eof();
				}
				else
					return false;
			}
		}
		else
			break;
	}

	return false;
}

bool SimpleParsing::seekParam(EParseParam_t _eParseParam, eSipUtil::BufferControl & _rclsBufferControl, char * _pszValue, unsigned int _nValueMaxLen)
{
	if( _eParseParam < 0 || _eParseParam >= E_PARSE_PARAM_NUM )
		return false;

	const char * pCurrentPos;
	const char * pszParamName = s_arrParseParamInfo[_eParseParam].m_pszStr;
	unsigned unParamLen = s_arrParseParamInfo[_eParseParam].m_unStrLen;
	EParamType eParamType = s_arrParseParamInfo[_eParseParam].m_eParamType;

	_rclsBufferControl.skipWhitespace();
	while( !_rclsBufferControl.eof() )
	{
		if( _rclsBufferControl.isStr(pszParamName) )
		{
			_rclsBufferControl.move(unParamLen);
			_rclsBufferControl.skipWhitespace();
			if( eParamType == E_PARAM_TYPE_VALUE )
			{
				if( _rclsBufferControl.isChar('=') )
				{
					if( !_pszValue )
						return true;
				
					_rclsBufferControl.move(1);
					_rclsBufferControl.skipWhitespace();
					if( (pCurrentPos = _rclsBufferControl.findChar(';')) )
					{
						int nCpyLen = (pCurrentPos-_rclsBufferControl.getStartPos() > _nValueMaxLen-1) ? _nValueMaxLen-1 : pCurrentPos-_rclsBufferControl.getStartPos();
						memcpy(_pszValue, _rclsBufferControl.getStartPos(), nCpyLen);
						_pszValue[nCpyLen] = 0x00;
					}
					else
					{
						int nCpyLen = (_rclsBufferControl.getSize() > _nValueMaxLen-1) ? _nValueMaxLen-1 : _rclsBufferControl.getSize();
						memcpy(_pszValue, _rclsBufferControl.getStartPos(), nCpyLen);
						_pszValue[nCpyLen] = 0x00;
					}
					
					return true;
				}
			}			
			else
			{
				if( _rclsBufferControl.isChar(';') || _rclsBufferControl.isChars(g_pszWhiteSpaceAllList) || _rclsBufferControl.eof() )
					return true;
			}
		}

		if( (pCurrentPos = _rclsBufferControl.findChar(';')) )
		{
			_rclsBufferControl.setStartPos(pCurrentPos);
			_rclsBufferControl.move(1);
			_rclsBufferControl.skipWhitespace();
		}
		else
			break;
	}

	return false;
}

char * SimpleParsing::makeResponse(const SimpleParsingData & _rstSimpleParsingData,
																		unsigned int _unRspCode, const char * _pszToTag, 
																		int _nReasonCause, const char * _pszReasonTxt,
																		TransportType _eTransportType)
{
	char * pszResponse = NULL;
	const char * pszToTag = NULL;

	if( _rstSimpleParsingData.m_nRspCode != 0 || _unRspCode < 100 )
	{
		ErrLog(<<"SimpleParsing : makeResponse : fail : invalid argument : CallId["<<_rstSimpleParsingData.m_szCallId<<"]");
		goto ERR_PROC;
	}

	pszResponse = (char *)malloc(s_unMaxResponseSize);
	if( !pszResponse )
	{
		ErrLog(<<"SimpleParsing : makeResponse : fail : can't create Response : CallId["<<_rstSimpleParsingData.m_szCallId<<"]");
		goto ERR_PROC;
	}
	pszResponse[0] = 0x00;
	
	/*==== StatusLine ====*/
	if( _eTransportType == TLS )
		eSipUtil::strncat(pszResponse, s_unMaxResponseSize, "%s %d ", s_arrParseHeaderInfo[E_PARSE_HEADER_STAUS_LINE].m_pszStr2, _unRspCode);
	else
		eSipUtil::strncat(pszResponse, s_unMaxResponseSize, "%s %d ", s_arrParseHeaderInfo[E_PARSE_HEADER_STAUS_LINE].m_pszStr, _unRspCode);
	g_fnGetResponseCodeDesc(_unRspCode, pszResponse, s_unMaxResponseSize, false);
	eSipUtil::strncat(pszResponse, s_unMaxResponseSize, "%s", g_pszCRLF);

	/*==== Via ====*/
	if( !_rstSimpleParsingData.m_fnCopyHdrStr(pszResponse, s_unMaxResponseSize, E_PARSE_HEADER_VIA) )	
	{
		ErrLog(<<"SimpleParsing : makeResponse : fail : Via : CallId["<<_rstSimpleParsingData.m_szCallId<<"]");
		goto ERR_PROC;
	}

	/*==== From ====*/
	if( !_rstSimpleParsingData.m_fnCopyHdrStr(pszResponse, s_unMaxResponseSize, E_PARSE_HEADER_FROM) )	
	{
		ErrLog(<<"SimpleParsing : makeResponse : fail : From : CallId["<<_rstSimpleParsingData.m_szCallId<<"]");
		goto ERR_PROC;
	}

	/*==== To ====*/
	if( strlen(_rstSimpleParsingData.m_szToTag) > 0 )
		pszToTag = NULL;
	else
		pszToTag = _pszToTag;
	if( !_rstSimpleParsingData.m_fnCopyHdrStr(pszResponse, s_unMaxResponseSize, E_PARSE_HEADER_TO, pszToTag) )	
	{
		ErrLog(<<"SimpleParsing : makeResponse : fail : To : CallId["<<_rstSimpleParsingData.m_szCallId<<"]");
		goto ERR_PROC;
	}

	/*==== CallID ====*/
	if( !_rstSimpleParsingData.m_fnCopyHdrStr(pszResponse, s_unMaxResponseSize, E_PARSE_HEADER_CALLID) )	
	{
		ErrLog(<<"SimpleParsing : makeResponse : fail : CallId : CallId["<<_rstSimpleParsingData.m_szCallId<<"]");
		goto ERR_PROC;
	}

	/*==== CSeq ====*/
	if( !_rstSimpleParsingData.m_fnCopyHdrStr(pszResponse, s_unMaxResponseSize, E_PARSE_HEADER_CSEQ) )	
	{
		ErrLog(<<"SimpleParsing : makeResponse : fail : CSeq : CallId["<<_rstSimpleParsingData.m_szCallId<<"]");
		goto ERR_PROC;
	}

	/*==== Reason ====*/
	if( _nReasonCause > 0 || (_pszReasonTxt && strlen(_pszReasonTxt) > 0) )
	{
		eSipUtil::strncat(pszResponse, s_unMaxResponseSize, "Reason: Q.850");
		if( _nReasonCause > 0 )
			eSipUtil::strncat(pszResponse, s_unMaxResponseSize, ";cause=%d", _nReasonCause);
		if( _pszReasonTxt && strlen(_pszReasonTxt) > 0 )
			eSipUtil::strncat(pszResponse, s_unMaxResponseSize, ";text=\"%s\"", _pszReasonTxt);
		eSipUtil::strncat(pszResponse, s_unMaxResponseSize, "%s", g_pszCRLF);
	}

	eSipUtil::strncat(pszResponse, s_unMaxResponseSize, "%s", g_pszCRLF);

	return pszResponse;

ERR_PROC :
	free(pszResponse);
	return NULL;
	
}

bool SimpleParsing::parsingStartLine(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc)
{
	if( !_pszHdrBuffer || _unHdrLen == 0 )
	{
		ErrLog(<<"simpleParsing : StartLine : fail : not found");
		return false;
	}

	const char * pTmpPos;
	bool bSipSchem = false, bSipsSchem = false;
	eSipUtil::BufferControl clsSipBufferControl(_pszHdrBuffer, _unHdrLen);
	
	/*==== check Request/MethodTypes/RspCode ====*/
	if( (bSipSchem = clsSipBufferControl.isStr(s_arrParseHeaderInfo[E_PARSE_HEADER_STAUS_LINE].m_pszStr))
		|| (bSipsSchem = clsSipBufferControl.isStr(s_arrParseHeaderInfo[E_PARSE_HEADER_STAUS_LINE].m_pszStr2)) )	//response
	{
		if( bSipSchem )
			clsSipBufferControl.move(s_arrParseHeaderInfo[E_PARSE_HEADER_STAUS_LINE].m_unStrLen);
		else
			clsSipBufferControl.move(s_arrParseHeaderInfo[E_PARSE_HEADER_STAUS_LINE].m_unStrLen2);
		clsSipBufferControl.skipWhitespace();
		if( !(pTmpPos = clsSipBufferControl.findChars(g_pszWhiteSpaceAllList)) )
			pTmpPos = clsSipBufferControl.getEndPos();
		if( pTmpPos - clsSipBufferControl.getStartPos() != 3 )
		{
			ErrLog(<<"simpleParsing : StartLine for rsp : fail : not found response-code");
			return false;
		}
		char szRspCode[4];
		memcpy(szRspCode, clsSipBufferControl.getStartPos(), 3);
		szRspCode[3] = 0x00;
		_rstSimpleParsingData.m_nRspCode = atoi(szRspCode);
		if( _rstSimpleParsingData.m_nRspCode < 100 || _rstSimpleParsingData.m_nRspCode > 699 )
		{
			ErrLog(<<"simpleParsing : StartLine for rsp : fail : invalid response-code");
			return false;
		}				
	}
	else		//request
	{
		_rstSimpleParsingData.m_nRspCode = 0;
		if( (pTmpPos = clsSipBufferControl.findChars(g_pszWhiteSpaceList)) )
		{
			_rstSimpleParsingData.m_eMethodType = resip::getMethodType(clsSipBufferControl.getStartPos(), pTmpPos - clsSipBufferControl.getStartPos());
			if( _rstSimpleParsingData.m_eMethodType == UNKNOWN || _rstSimpleParsingData.m_eMethodType == RESPONSE )
			{
				ErrLog(<<"simpleParsing : StartLine for req : fail : invalid method");
				return false;
			}
		}
		else
		{
			ErrLog(<<"simpleParsing : StartLine for req : fail : not found method");
			return false;
		}
	}

	return true;
}

bool SimpleParsing::parsingTo(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc)
{
	if( !_pszHdrBuffer || _unHdrLen == 0 )
	{
		ErrLog(<<"simpleParsing : To : fail : not found");
		return false;
	}

	bool bExistTag = false;
	EParseUriSchem_t eUriSchem = E_PARSE_URI_NUM;
	
	if( parsingNameAddr(_pszHdrBuffer, _unHdrLen, eUriSchem,
									_rstSimpleParsingData.m_szToUsername, SimpleParsingData_t::E_CONST_URI_USERNAME_SIZE, 
									_rstSimpleParsingData.m_szToPhoneCtx, SimpleParsingData_t::E_CONST_URI_PARAM_PHONECONTEXT_SIZE,
									bExistTag, _rstSimpleParsingData.m_szToTag, SimpleParsingData_t::E_CONST_URI_PARAM_TAG_SIZE) )
	{
		if( _rstSimpleParsingData.m_nRspCode == 0 )
		{
			if( bExistTag )
				_rstSimpleParsingData.m_bInitial = false;
			else
				_rstSimpleParsingData.m_bInitial = true;
		}
	}
	else
	{
		ErrLog(<<"simpleParsing : To : fail : parsing NameAddr");
		return false;
	}

	return true;
}

bool SimpleParsing::parsingFrom(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc)
{
	if( !_pszHdrBuffer || _unHdrLen == 0 )
	{
		ErrLog(<<"simpleParsing : From : fail : not found");
		return false;
	}

	bool bExistTag = false;
	EParseUriSchem_t eUriSchem = E_PARSE_URI_NUM;

	if( parsingNameAddr(_pszHdrBuffer, _unHdrLen, eUriSchem,
									_rstSimpleParsingData.m_szFromUsername, SimpleParsingData_t::E_CONST_URI_USERNAME_SIZE, 
									_rstSimpleParsingData.m_szFromPhoneCtx, SimpleParsingData_t::E_CONST_URI_PARAM_PHONECONTEXT_SIZE,
									bExistTag, _rstSimpleParsingData.m_szFromTag, SimpleParsingData_t::E_CONST_URI_PARAM_TAG_SIZE) ) 
	{
		if( !bExistTag )
		{
			ErrLog(<<"simpleParsing : From : fail : not found Tag");
			return false;
		}
	}
	else
	{
		ErrLog(<<"simpleParsing : From : fail : parsing NameAddr");
		return false;
	}

	return true;
}

bool SimpleParsing::parsingCSeq(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc)
{
	if( !_pszHdrBuffer || _unHdrLen == 0 )
	{
		ErrLog(<<"simpleParsing : CSeq : fail : not found");
		return false;
	}

	eSipUtil::BufferControl clsSipBufferControl(_pszHdrBuffer, _unHdrLen);
	clsSipBufferControl.skipChars(g_pszDigitList);
	clsSipBufferControl.skipWhitespace();
	if( !clsSipBufferControl.eof() )
	{
		MethodTypes eMethodType = resip::getMethodType(clsSipBufferControl.getStartPos(), clsSipBufferControl.getSize());
		if( eMethodType == UNKNOWN || eMethodType == RESPONSE )
		{
			ErrLog(<<"simpleParsing : CSeq : fail : invalid method");
			return false;
		}
		else if( _rstSimpleParsingData.m_nRspCode == 0 )
		{
			if( _rstSimpleParsingData.m_eMethodType != eMethodType )
			{
				ErrLog(<<"simpleParsing : CSeq : fail : unMatch Method with ReqLine : CSeq["<<getMethodName(eMethodType)
							<<"] != ReqLine["<<getMethodName(_rstSimpleParsingData.m_eMethodType)<<"]");
				return false;
			}
		}
		else
			_rstSimpleParsingData.m_eMethodType = eMethodType;
	}
	else
	{
		ErrLog(<<"simpleParsing : CSeq : fail : not found method");
		return false;
	}

	return true;
}

bool SimpleParsing::parsingCallId(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc)
{
	if( !_pszHdrBuffer || _unHdrLen == 0 )
	{
		ErrLog(<<"simpleParsing : Call-Id : fail : not found");
		return false;
	}

	int nCopyLen = (_unHdrLen > SimpleParsingData::E_CONST_CALLID_SIZE - 1) ? SimpleParsingData::E_CONST_CALLID_SIZE - 1 : _unHdrLen;
	memcpy(_rstSimpleParsingData.m_szCallId, _pszHdrBuffer, nCopyLen);
	_rstSimpleParsingData.m_szCallId[nCopyLen] = 0x00;

	return true;
}

bool SimpleParsing::parsingAcceptContact(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc)
{
	if( !_pszHdrBuffer || _unHdrLen == 0 )
	{
		DebugLog(<<"simpleParsing : Accept-Contact : fail : not found");
		return true;
	}

	eSipUtil::BufferControl clsSipBufferControl(_pszHdrBuffer, _unHdrLen);

	const char * pCurrentPos = clsSipBufferControl.findChar(',');
	if( pCurrentPos )
		clsSipBufferControl.setEndPos(pCurrentPos);

	if( seekParam(E_PARSE_PARAM_VIDEO, clsSipBufferControl, NULL, 0) )
		_rstSimpleParsingData.m_bVideo = true;
	else
		_rstSimpleParsingData.m_bVideo = false;

	return true;
}

bool SimpleParsing::parsingVia(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc)
{
	if( !_pszHdrBuffer || _unHdrLen == 0 )
	{
		ErrLog(<<"simpleParsing : Via : fail : not found");
		return false;
	}

	const char * pStartPos, * pEndPos;
	eSipUtil::BufferControl clsSipBufferControl(_pszHdrBuffer, _unHdrLen);
	TransportType eTrspType = *((TransportType *)_pEtc);

	const char * pCurrentPos = clsSipBufferControl.findChar(',');
	if( pCurrentPos )
		clsSipBufferControl.setEndPos(pCurrentPos);

	const char * pszTransport = NULL;
	unsigned int unTransportLen = 0;
	if( eTrspType == TCP )
	{
		pszTransport = s_arrParseViaTransportInfo[E_PARSE_VIA_TRANSPORT_TCP].m_pszStr;
		unTransportLen = s_arrParseViaTransportInfo[E_PARSE_VIA_TRANSPORT_TCP].m_unStrLen;
	}
	else if( eTrspType == TLS )
	{
		pszTransport = s_arrParseViaTransportInfo[E_PARSE_VIA_TRANSPORT_TLS].m_pszStr;
		unTransportLen = s_arrParseViaTransportInfo[E_PARSE_VIA_TRANSPORT_TLS].m_unStrLen;
	}
	else
	{
		pszTransport = s_arrParseViaTransportInfo[E_PARSE_VIA_TRANSPORT_UDP].m_pszStr;
		unTransportLen = s_arrParseViaTransportInfo[E_PARSE_VIA_TRANSPORT_UDP].m_unStrLen;
	}
	pStartPos = clsSipBufferControl.findStr(pszTransport);
	if( !pStartPos )
	{
		ErrLog(<<"simpleParsing : Via : fail : not found Transport");	
		return false;
	}
	pStartPos += unTransportLen;
	
	//get IP
	clsSipBufferControl.setStartPos(pStartPos);
	clsSipBufferControl.skipWhitespace();
	pStartPos = clsSipBufferControl.getStartPos();
	pEndPos = clsSipBufferControl.findChar(':');
	if( !pEndPos ||pEndPos - pStartPos >= SimpleParsingData::E_CONST_IP_SIZE )
	{
		ErrLog(<<"simpleParsing : Via : fail : invalid IP");	
		return false;
	}
	strncpy(_rstSimpleParsingData.m_szViaIp, pStartPos, pEndPos - pStartPos);
	_rstSimpleParsingData.m_szViaIp[pEndPos - pStartPos] = 0x00;

	//get Port
	clsSipBufferControl.setStartPos(pEndPos);
	clsSipBufferControl.move(1);
	pStartPos = clsSipBufferControl.getStartPos();
	pEndPos = clsSipBufferControl.findChar(';');
	if( !pEndPos || pEndPos - pStartPos >= 6 )
	{
		ErrLog(<<"simpleParsing : Via : fail : invalid Port");	
		return false;
	}
	char szPortStr[6] = {0,};
	strncpy(szPortStr, pStartPos, pEndPos - pStartPos);
	szPortStr[pEndPos - pStartPos] = 0x00;		
	_rstSimpleParsingData.m_nViaPort = atoi(szPortStr);

	return true;
}

bool SimpleParsing::parsingNameAddr(const char * _pszHdrBuffer, unsigned int _unHdrLen,
																		EParseUriSchem_t & _eUriSchem,
																		char * _pszUsername, unsigned int _unUsernameLen,
																		char * _pszPhoneContext, unsigned int _unPhoneContextLen,
																		bool & _bExistTag, char * _pszTag, unsigned int _unTagLen)
{
	_bExistTag = false;
	_eUriSchem = E_PARSE_URI_SIP;
	
	eSipUtil::BufferControl clsSipBufferControl(_pszHdrBuffer, _unHdrLen);

	const char * pCurrentPos, * pUriStartPos, * pUriEndPos;
	if( clsSipBufferControl.isChar('\"') )
	{
		clsSipBufferControl.move(1);
		clsSipBufferControl.skipWhitespace();
		if( (pCurrentPos = clsSipBufferControl.findChar('\"')) )
		{
			clsSipBufferControl.setStartPos(pCurrentPos);
			clsSipBufferControl.move(1);
			clsSipBufferControl.skipWhitespace();
			if( clsSipBufferControl.eof() )
			{
				ErrLog(<<"simpleParsing : NameAddr : fail : invalid data after double quot");
				return false;
			}
		}
		else
		{
			ErrLog(<<"simpleParsing : NameAddr : fail : invalid double quot");
			return false;
		}
	}

	if( (pCurrentPos = clsSipBufferControl.findChar(',')) )
		clsSipBufferControl.setEndPos(pCurrentPos);
	
	if( (pCurrentPos = clsSipBufferControl.findChar('<')) )
	{
		clsSipBufferControl.setStartPos(pCurrentPos);
		clsSipBufferControl.move(1);
		clsSipBufferControl.skipWhitespace();
		if( (pUriEndPos = clsSipBufferControl.findChar('>')) )
		{
			pUriStartPos = clsSipBufferControl.getStartPos();
			clsSipBufferControl.setStartPos(pUriEndPos);
			clsSipBufferControl.move(1);
			clsSipBufferControl.skipWhitespace();
			if( clsSipBufferControl.isChar(';') )
				clsSipBufferControl.move(1);
		}
		else
		{
			ErrLog(<<"simpleParsing : NameAddr : fail : not found '>'");
			return false;
		}
	}
	else
	{
		if( (pUriEndPos = clsSipBufferControl.findChar(';')) )
		{
			pUriStartPos = clsSipBufferControl.getStartPos();
			clsSipBufferControl.setStartPos(pUriEndPos);
			clsSipBufferControl.move(1);
		}
		else
		{
			pUriStartPos = clsSipBufferControl.getStartPos();
			pUriEndPos = clsSipBufferControl.getEndPos();
			
			clsSipBufferControl.setStartPos(clsSipBufferControl.getEndPos());
		}
	}
	
	//get UserName
	EParseUriSchem_t eUriSchem = E_PARSE_URI_NUM;
	eSipUtil::BufferControl clsUriBufferControl(pUriStartPos, pUriEndPos - pUriStartPos);
	clsUriBufferControl.skipWhitespace(true);
	if( clsUriBufferControl.eof() )
	{
		ErrLog(<<"simpleParsing : NameAddr : fail : invalid uri");
		return false;
	}
	if( clsUriBufferControl.isStr(s_arrParseUriSchemInfo[E_PARSE_URI_SIP].m_pszStr) )
	{
		clsUriBufferControl.move(s_arrParseUriSchemInfo[E_PARSE_URI_SIP].m_unStrLen);
		eUriSchem = s_arrParseUriSchemInfo[E_PARSE_URI_SIP].m_eParseUriSchem;
	}
	else if( clsUriBufferControl.isStr(s_arrParseUriSchemInfo[E_PARSE_URI_SIPS].m_pszStr) )
	{
		clsUriBufferControl.move(s_arrParseUriSchemInfo[E_PARSE_URI_SIPS].m_unStrLen);
		eUriSchem = s_arrParseUriSchemInfo[E_PARSE_URI_SIPS].m_eParseUriSchem;
	}
	else if( clsUriBufferControl.isStr(s_arrParseUriSchemInfo[E_PARSE_URI_TEL].m_pszStr) )
	{
		clsUriBufferControl.move(s_arrParseUriSchemInfo[E_PARSE_URI_TEL].m_unStrLen);
		eUriSchem = s_arrParseUriSchemInfo[E_PARSE_URI_TEL].m_eParseUriSchem;
	}
	else if( clsUriBufferControl.isStr(s_arrParseUriSchemInfo[E_PARSE_URI_URN].m_pszStr) )
	{
		clsUriBufferControl.move(s_arrParseUriSchemInfo[E_PARSE_URI_URN].m_unStrLen);
		eUriSchem = s_arrParseUriSchemInfo[E_PARSE_URI_URN].m_eParseUriSchem;
	}
	else
	{
		ErrLog(<<"simpleParsing : NameAddr : fail : invalid scheme of uri");
		return false;
	}
	clsUriBufferControl.skipWhitespace();
	if( clsUriBufferControl.isChar(':') )
	{
		clsUriBufferControl.move(1);
		clsUriBufferControl.skipWhitespace();
		if( clsUriBufferControl.eof() )
		{
			ErrLog(<<"simpleParsing : NameAddr : fail : invalid data after scheme of uri");
			return false;
		}
	}
	else
	{
		ErrLog(<<"simpleParsing : NameAddr : fail : not found ':' after scheme of uri");
		return false;
	}
	_eUriSchem = eUriSchem;
	if( _eUriSchem == E_PARSE_URI_SIP || _eUriSchem == E_PARSE_URI_SIPS )
	{
		if( (pCurrentPos = clsUriBufferControl.findChar('@')) )
		{
			int nCpyLen = (pCurrentPos-clsUriBufferControl.getStartPos() > _unUsernameLen-1) ? _unUsernameLen-1 : pCurrentPos-clsUriBufferControl.getStartPos();
			memcpy(_pszUsername, clsUriBufferControl.getStartPos(), nCpyLen);
			_pszUsername[nCpyLen] = 0x00;			
			clsUriBufferControl.setStartPos(pCurrentPos);
			clsUriBufferControl.move(1);
		}
		if( (pCurrentPos = clsUriBufferControl.findChar(';')) )
		{
			if( pCurrentPos - clsUriBufferControl.getStartPos() <= 0 )
			{
				ErrLog(<<"simpleParsing : NameAddr : fail : invalid host 1 of SipUri");
				return false;
			}
			clsUriBufferControl.setStartPos(pCurrentPos);
			clsUriBufferControl.move(1);
			clsUriBufferControl.skipWhitespace();
		}
		else
		{
			if( clsUriBufferControl.eof() )
			{
				ErrLog(<<"simpleParsing : NameAddr : fail : invalid host 2 of SipUri");
				return false;
			}
			clsUriBufferControl.setStartPos(clsUriBufferControl.getEndPos());
		}
	}
	else
	{
		if( (pCurrentPos = clsUriBufferControl.findChar(';')) )
		{
			if( pCurrentPos - clsUriBufferControl.getStartPos() <= 0 )
			{
				ErrLog(<<"simpleParsing : NameAddr : fail : invalid username 1 of TelUri");
				return false;
			}

			int nCpyLen = (pCurrentPos-clsUriBufferControl.getStartPos() > _unUsernameLen-1) ? _unUsernameLen-1 : pCurrentPos-clsUriBufferControl.getStartPos();
			memcpy(_pszUsername, clsUriBufferControl.getStartPos(), nCpyLen);
			_pszUsername[nCpyLen] = 0x00;		
			clsUriBufferControl.setStartPos(pCurrentPos);
			clsUriBufferControl.move(1);
			clsUriBufferControl.skipWhitespace();
		}
		else
		{
			if( clsUriBufferControl.eof() )
			{
				ErrLog(<<"simpleParsing : NameAddr : fail : invalid username 2 of TelUri");
				return false;
			}

			int nCpyLen = (clsUriBufferControl.getSize() > _unUsernameLen-1) ? _unUsernameLen-1 : clsUriBufferControl.getSize();
			memcpy(_pszUsername, clsUriBufferControl.getStartPos(), nCpyLen);
			_pszUsername[nCpyLen] = 0x00;			
			clsUriBufferControl.setStartPos(clsUriBufferControl.getEndPos());
		}
	}

	//get Phone-Context parameter
	if( _eUriSchem == E_PARSE_URI_TEL )
		seekParam(E_PARSE_PARAM_PHONECONTEXT, clsUriBufferControl, _pszPhoneContext, _unPhoneContextLen);

	//get Tag parameter
	if( seekParam(E_PARSE_PARAM_TAG, clsSipBufferControl, _pszTag, _unTagLen) )
		_bExistTag = true;

	return true;
}

}

#endif

