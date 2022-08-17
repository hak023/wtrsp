
#ifndef RESIP_SIMPLE_PARSING_HXX
#define RESIP_SIMPLE_PARSING_HXX

#ifdef ESIP_IN_RESIP

#include "resip/stack/MethodTypes.hxx"

#include "eSipUtil/bufferControl.hxx"

#include "ext/slb/Slb_Ccm_Msg.hxx"

namespace resip
{

typedef enum
{
	E_PARSE_PARAM_TAG = 0,
	E_PARSE_PARAM_PHONECONTEXT,
	E_PARSE_PARAM_VIDEO,

	E_PARSE_PARAM_NUM,
}EParseParam_t;

typedef enum
{
	E_PARSE_URI_SIP = 0,
	E_PARSE_URI_SIPS,
	E_PARSE_URI_TEL,
	E_PARSE_URI_URN,

	E_PARSE_URI_NUM,
}EParseUriSchem_t;

typedef enum
{
	E_PARSE_VIA_TRANSPORT_UDP = 0,
	E_PARSE_VIA_TRANSPORT_TCP,
	E_PARSE_VIA_TRANSPORT_TLS,

	E_PARSE_VIA_TRANSPORT_NUM,
}EParseViaTransport_t;

struct ScanHeaderData
{
	const char * m_parrTopHeaderStartPos[E_PARSE_HEADER_NUM];		//top header of multiHeader
	unsigned int m_arrTopHeaderLen[E_PARSE_HEADER_NUM];					//top header of multiHeader

	//added by khd for multiple via 20181107
	std::list< std::pair<const char *, unsigned int> > m_listAdditionViaInfo; //additional via headers(first:position, second:length)
	
	unsigned int m_unTotalHeaderLen;

	void m_fnReset()
	{
		memset(m_parrTopHeaderStartPos, 0x00, sizeof(char *)*E_PARSE_HEADER_NUM);
		memset(m_arrTopHeaderLen, 0x00, sizeof(int)*E_PARSE_HEADER_NUM);

		//added by khd for multiple via 20181107
		m_listAdditionViaInfo.clear();
		
		m_unTotalHeaderLen = 0;
	}

	ScanHeaderData()
	{
		m_fnReset();
	}
};

struct SimpleParsingData
{
	enum
	{
		E_CONST_URI_USERNAME_SIZE = 42,
		E_CONST_URI_PARAM_PHONECONTEXT_SIZE = 16,
		E_CONST_URI_PARAM_TAG_SIZE = 128,
		E_CONST_CALLID_SIZE = 256,
		E_CONST_IP_SIZE = 64,

		E_CONST_PRINT_LEN = 100,
	};

	ScanHeaderData m_stScanHeaderData;
	resip::MethodTypes m_eMethodType;
	int m_nRspCode;
	bool m_bInitial;
	char m_szFromUsername[E_CONST_URI_USERNAME_SIZE];
	char m_szFromPhoneCtx[E_CONST_URI_PARAM_PHONECONTEXT_SIZE];
	char m_szFromTag[E_CONST_URI_PARAM_TAG_SIZE];
	char m_szToUsername[E_CONST_URI_USERNAME_SIZE];
	char m_szToPhoneCtx[E_CONST_URI_PARAM_PHONECONTEXT_SIZE];	
	char m_szToTag[E_CONST_URI_PARAM_TAG_SIZE];
	char m_szCallId[E_CONST_CALLID_SIZE];
	bool m_bVideo;		//true : video, false : audio
	char m_szViaIp[E_CONST_IP_SIZE];
	int m_nViaPort;

	void reset();
	void m_fnApplyStr(ESlbInternalSipStrType_t _eSipStrType, const char * _pStr, unsigned int _unLen);
	void m_fnApply(const char * _pszBuffer, unsigned int _unLen, const SlbInternalSipInfo & _rstInternalSipInfo);
	bool m_fnCopyHdrStr(char * _pszDst, unsigned int _unDstMaxLen, EParseHeader_t _eParsingHdr, const char * _pszToTag = NULL) const;
	
	SimpleParsingData()
	{
		reset();
	}

	void log() const;
	void errlog() const;
};
typedef struct SimpleParsingData SimpleParsingData_t;

class SimpleParsing
{
	public :
		static bool parsing(const char * _pszBuffer, unsigned int _unLen, SimpleParsingData_t & _rstSimpleParsingData, TransportType _eTransportType);
		static void scanHeader(const char * _pszOrigBuffer, unsigned int _unOrigLen, ScanHeaderData & _rstScanHeaderData);
		static bool seekHeader(EParseHeader_t _eParseHeader, eSipUtil::BufferControl & _rclsBufferControl);
		static bool seekParam(EParseParam_t _eParseParam, eSipUtil::BufferControl & _rclsBufferControl, char * _pszValue, unsigned int _nValueMaxLen);
		static char * makeResponse(const SimpleParsingData & _rstSimpleParsingData,
																unsigned int _unRspCode, const char * _pszToTag, 
																int _nReasonCause, const char * _pszReasonTxt,
																TransportType _eTransportType);

		/*==== header Parsing Function ====*/
		static bool parsingStartLine(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc);
		static bool parsingTo(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc);
		static bool parsingFrom(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc);
		static bool parsingCSeq(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc);
		static bool parsingCallId(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc);
		static bool parsingAcceptContact(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc);
		static bool parsingVia(const char * _pszHdrBuffer, unsigned int _unHdrLen, SimpleParsingData_t & _rstSimpleParsingData, void * _pEtc);

	private :
		/*==== parsing function ====*/
		static bool  parsingNameAddr(const char * _pszHdrBuffer, unsigned int _unHdrLen,
																	EParseUriSchem_t & _eUriSchem,
																	char * _pszUsername, unsigned int _unUsernameLen,
																	char * _pszPhoneContext, unsigned int _unPhoneContextLen,
																	bool & _bExistTag, char * _pszTag, unsigned int _unTagLen);

};

}

#endif

#endif

