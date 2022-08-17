#ifndef ENUM_IF_H
#define ENUM_IF_H
#include "eSipUtil/flexIf.hxx"
#include "eSipUtil/string.hxx"
/*
Flex IF Tables
***************************
MAIN
{
REQ = ENUM_QUERY_REQ
SID = Value
CALLID = Value
TID = Value
DID = Value
TIME = Value(time_t)
CALLED = Value
CALLER = Value
HLR_ENABLE = ON/OFF
NPDB_ENABLE = ON/OFF
MEDIA_TYPE = AUDIO
}
***************************
MAIN
{
//---------> HDR
REQ = ENUM_QUERY_RSP
SID = "ccm req val"
CALLID = "ccm req val"
TID = "ccm req val"
CALLED = "ccm req val"
HLR_ENABLE = "ccm req val"
NPDB_ENABLE = "ccm req val"
ENUM_RESULT = "404"
//---------> NPDB
NPDB_RESULT = LGT/SKT/KT/... 
NPDB_REASON = OK/FAIL
NPDB_RN = 821020000000
NPDB_RCFDN = 821012345678
PFX_DOMAIN = sktims.net
PFX_NUMBER = 811110
CODE_TYPE = NPDB/FIXED/IMS/CS
//---------> HLR
HLR_RESULT = IMS/CS/H323/CAPA/MSISDN
HLR_REASON = OK/FAIL
HLR_CODEC_INFO = 1/2
HLR_PC = 1A2B
HLR_IMG = 1/2/3
HLR_DRTN = 821012345678
HLR_TRANSFER_NUMBER = 01012345678
HLR_TRANSFER_REASON = 1/2/3
}
***************************
MAIN
{
REQ = ENUM_PING

}
***************************
MAIN
{
REQ = ENUM_PONG
NPDB_STATUS = FBLK/AVAIL
HLR_STATUS = FBLK/AVAIL
}
*/
#define DEF_CCM_KEY_CATEGORY "MAIN"
#define DEF_CCM_KEY_REQ "REQ"
#define DEF_CCM_KEY_SID "SID"
#define DEF_CCM_KEY_CALLID "CALLID"
#define DEF_CCM_KEY_TID "TID"
#define DEF_CCM_KEY_DID "DID"
#define DEF_CCM_KEY_CALLED "CALLED"
#define DEF_CCM_KEY_CALLER "CALLER"
#define DEF_CCM_KEY_TIME "TIME"
#define DEF_CCM_KEY_HLR_ENABLE "HLR_ENABLE"
#define DEF_CCM_KEY_NPDB_ENABLE "NPDB_ENABLE"
#define DEF_CCM_KEY_HLR_RESULT "HLR_RESULT"
#define DEF_CCM_KEY_NPDB_RESULT "NPDB_RESULT"
#define DEF_CCM_KEY_NPDB_STATUS "NPDB_STATUS"
#define DEF_CCM_KEY_HLR_STATUS "HLR_STATUS"
#define DEF_CCM_KEY_HLR_REASON "HLR_REASON"
#define DEF_CCM_KEY_NPDB_REASON "NPDB_REASON"
#define DEF_CCM_KEY_MEDIA_TYPE "MEDIA_TYPE"
#define DEF_CCM_KEY_NPDB_RN   "NPDB_RN"
#define DEF_CCM_KEY_NPDB_RCFDN "NPDB_RCFDN"
#define DEF_CCM_KEY_PFX_DOMAIN "PFX_DOMAIN"
#define DEF_CCM_KEY_PFX_NUMBER "PFX_NUMBER"
#define DEF_CCM_KEY_HLR_CODEC_INFO "HLR_CODEC_INFO"
#define DEF_CCM_KEY_HLR_PC "HLR_PC"
#define DEF_CCM_KEY_HLR_IMG "HLR_IMG"
#define DEF_CCM_KEY_HLR_DRTN "HLR_DRTN"
#define DEF_CCM_KEY_HLR_TRANSFER_NUMBER "HLR_TRANSFER_NUMBER"
#define DEF_CCM_KEY_HLR_TRANSFER_REASON "HLR_TRANSFER_REASON"
#define DEF_CCM_KEY_TEL_IOR_OPT "TEL_IOR_OPT"
#define DEF_CCM_KEY_PFX_CODE_TYPE "CODE_TYPE"
#define DEF_CCM_KEY_ENUM_RESULT "ENUM_RESULT"

#define DEF_CCM_VAL_ENUM_QUERY_REQ "ENUM_QUERY_REQ"
#define DEF_CCM_VAL_ENUM_QUERY_RSP "ENUM_QUERY_RSP"
#define DEF_CCM_VAL_ENUM_QUERY_ACK "ENUM_QUERY_ACK"
#define DEF_CCM_VAL_ENUM_PING "ENUM_PING"
#define DEF_CCM_VAL_ENUM_PONG "ENUM_PONG"
#define DEF_CCM_VAL_ON "ON"
#define DEF_CCM_VAL_OFF "OFF"
#define DEF_CCM_VAL_HLR_3G "3G"
#define DEF_CCM_VAL_HLR_IMS "IMS"
#define DEF_CCM_VAL_NPDB_SKT "SKT"
#define DEF_CCM_VAL_NPDB_LGT "LGT"
#define DEF_CCM_VAL_NPDB_KT "KT"
#define DEF_CCM_VAL_FBLK "FBLK"
#define DEF_CCM_VAL_AVAIL "AVAIL"
#define DEF_CCM_VAL_AUDIO "AUDIO"
#define DEF_CCM_VAL_VIDEO "VIDEO"

#define DEF_CCM_VAL_TIMEOUT "TIMEOUT"                //  -1
#define DEF_CCM_VAL_STATUS_DOWN "ST_DOWN"      //  -2
#define DEF_CCM_VAL_INT_ERR "INT_ERR"                  // -3

#define DEF_CCM_VAL_OK "OK"                                  // 0
#define DEF_CCM_VAL_FAIL "FAIL"

#define DEF_CCM_VAL_HLR_IMS  "IMS"
#define DEF_CCM_VAL_CS "CS"
#define DEF_CCM_VAL_H323 "H323"
#define DEF_CCM_VAL_CAPA "CAPA"
#define DEF_CCM_VAL_MSISDN "MSISDN"
#define DEF_CCM_VAL_WIRE "WIRE"
#define DEF_CCM_VAL_WIRELESS "WIRELESS"

#define DEF_CCM_VAL_CODE_NPDB "NPDB"
#define DEF_CCM_VAL_CODE_FIXED "FIXED"
#define DEF_CCM_VAL_CODE_IMS "IMS"
#define DEF_CCM_VAL_CODE_CS "CS"

typedef enum
{
	E_NPDB_BIND_OK = 0,     // bind success
	E_NPDB_BIND_SYSFAIL = 1,  // system err
	E_NPDB_BIND_AUTH_FAIL = 2,   // auth fail, rel
	E_NPDB_BIND_FORMAT_ERR = 3, // msg format err
}ENpdbBindResult_t;
typedef enum
{
	E_NPDB_QUERY_OK = 0,     // query success
	E_NPDB_QUERY_SYSFAIL = 1,   // system err
	E_NPDB_QUERY_FORMAT_ERR = 3,  // msg format err
	E_NPDB_QUERY_NO_DESTIN = 5,    // unregi number
	E_NPDB_QUERY_RES_SHORT = 7,  // npdb resource fail
	E_NPDB_QUERY_ETC = 9,   // unknown
}ENpdbQueryResult_t;
typedef enum
{
	E_NPDB_REP_SYSFAIL = 1,   // system err
	E_NPDB_REP_AUTH_FAIL = 2,  // auth fail, rel
	E_NPDB_FORMAT_ERR = 3,  // msg format err
	E_NPDB_ETC = 9,   // unknown
}ENpdbReportResult_t;
inline const char * g_fnStringNpdbErr(int _unErr)
{
	switch(_unErr)
	{
		case 0: return "OK";
		case 1: return "SYSFAIL";
		case 2: return "AUTH_FAIL";
		case 3: return "FORMAT_ERR";
		case 4: return "NOT_BOUND";
		case 5: return "NO_DESTIN";
		case 6: return "DISCON_PORTNUM";
		case 7: return "RES_SHORT";
		case 8: return "ETC";
		case 9: return "ETC";
		case -3: return "OVERLOAD";
		case -1: return "TIMEOUT";
		case -2: return "ST_DOWN";
		default: return "ETC";
	}
}
inline int g_fnEnumberNpdbErr(eSipUtil::CmpString _clsCmp)   // "string"
{
	if(_clsCmp == "OK") return 0;
	else if(_clsCmp == "SYSFAIL") return 1;
	else if(_clsCmp == "AUTH_FAIL") return 2;
	else if(_clsCmp == "FORMAT_ERR") return 3;
	else if(_clsCmp == "NOT_BOUND") return 4;
	else if(_clsCmp == "NO_DESTIN") return 5;
	else if(_clsCmp == "DISCON_PORTNUM") return 6;
	else if(_clsCmp == "RES_SHORT") return 7;
	else if(_clsCmp == "ETC") return 9;
	else if(_clsCmp == "OVERLOAD") return -3;
	else if(_clsCmp == "TIMEOUT") return -1;
	else if(_clsCmp == "ST_DOWN") return -2;
	else return 9;
}
class ComIf
{
	public:
		ComIf(){m_pclsIf=NULL;}
		~ComIf(){if(m_pclsIf) delete m_pclsIf;}
		static int m_fnGetLen(const void * pMsg, unsigned int _unMsgLen)
		{
			return eSipUtil::FlexIf::m_fnGetBuildMsgLen(pMsg, _unMsgLen);
		}
		void * m_fnEncoding(unsigned int & _runMsgLen)
		{
			if(m_pclsIf == NULL) m_pclsIf = new eSipUtil::FlexIf(DEF_CCM_KEY_CATEGORY,0);
			return m_pclsIf->m_fnGetBuildMsg(_runMsgLen);
		}
		bool m_fnDecoding(void * _pMsg, unsigned int _unMsgLen, bool _bSkipChkValid=false)
		{
			if(m_pclsIf)  delete m_pclsIf;
			m_pclsIf = new eSipUtil::FlexIf();
			return m_pclsIf->m_fnSetBuildMsg(_pMsg, _unMsgLen, _bSkipChkValid);
		}
		eSipUtil::CmpString m_fnFind(const char * _pszKey)
		{
			if(m_pclsIf == NULL) return NULL;
			unsigned int unValLen = 0; unsigned int unPos=0;
			return (const char*)m_pclsIf->m_fnGetNextItem(DEF_CCM_KEY_CATEGORY,_pszKey, unValLen,unPos);
		}
		static eSipUtil::CmpString m_fnFindPk(void * _pMsg, unsigned int _unMsgLen,const char * _pszKey)
		{
			eSipUtil::FlexIf clsIf; clsIf.m_fnSetBuildMsg(_pMsg,_unMsgLen);
			unsigned int unValLen = 0; unsigned int unPos=0;
			char* pszRet = (char*)clsIf.m_fnGetNextItem(DEF_CCM_KEY_CATEGORY,_pszKey, unValLen,unPos);
			clsIf.m_fnReset(false);
			return pszRet;
		}
		bool m_fnAdd(const char * _pszKey,eSipUtil::KString _clsVal)
		{
			if(m_pclsIf == NULL)
			{
				m_pclsIf = new eSipUtil::FlexIf(DEF_CCM_KEY_CATEGORY,0);
			}
			return m_pclsIf->m_fnAddItem(DEF_CCM_KEY_CATEGORY, _pszKey, (eSipUtil::KCSTR)_clsVal);
		}
		const char * m_fnGetDebug()
		{
			if(m_pclsIf == NULL) return NULL;
			m_clsDebug.m_fnReSize(2048);
			return m_pclsIf->m_fnGetDebugStr((eSipUtil::KSTR)m_clsDebug,2048,false);
		}
		static const char * m_fnGetDebug(const char * _pPacket, unsigned int _unPacketLen, 
															char * _pszStr, unsigned int _unMaxStrLen)
		{
			return eSipUtil::FlexIf::m_fnGetDebugStr(_pPacket,_unPacketLen,_pszStr,_unMaxStrLen,false);
		}
		const char * m_fnTrace(eSipUtil::KString & _rclsTrace)
		{
			if(m_pclsIf==NULL) return NULL;
			const char * pszKey = NULL; const char * pszValue = NULL; unsigned int unItor = 0;
	            do
			{
				if( m_pclsIf->m_fnGetNextAnyItem(DEF_CCM_KEY_CATEGORY, pszKey, pszValue, unItor) )
				{
					_rclsTrace<<pszKey<<": "<<pszValue<<"\r\n";
				}
			}
			while( unItor > 0 );

			
		}
		static const char * m_fnTrace(eSipUtil::KString & _rclsTrace, const char * _pPk, unsigned int _unLen)
		{
			const char * pszKey = NULL; const char * pszValue = NULL; unsigned int unItor = 0;
			eSipUtil::FlexIf clsIf; clsIf.m_fnSetBuildMsg((void*)_pPk,_unLen);
	            do
			{
				if( clsIf.m_fnGetNextAnyItem(DEF_CCM_KEY_CATEGORY, pszKey, pszValue, unItor) )
				{
					_rclsTrace<<pszKey<<": "<<pszValue<<"\r\n";
				}
			}
			while( unItor > 0 );
			clsIf.m_fnReset(false);
			return (eSipUtil::KCSTR)_rclsTrace;
		}
		void m_fnReset(bool _bFree)
		{
			if(m_pclsIf)
			{
				m_pclsIf->m_fnReset(_bFree);
			}
		}
		eSipUtil::FlexIf * m_pclsIf;
		eSipUtil::KString m_clsDebug;
};
#endif

