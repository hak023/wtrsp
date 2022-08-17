#ifndef __CRUSIPPARSE_H
#define __CRUSIPPARSE_H
#include "CRuleUtiles.h"

/*
Exclamation    !         0x21
Quotation    "            0x22
Number_sign  #         0x23
Dollar_sign      $        0x24
Percent_sign  %        0x25
Ampersand_sign &      0x26
Apostrophe   '            0x27
Left_Parenthesis   (     0x28
Right_Parenthesis    )  0x29
Asterisk      *             0x2A
Plus_sign   +               0x2B
Comma  ,                    0x2C
Hyphen_Minus    -         0x2D
Full_Stop    .                0x2E
Solidus   /                   0x2F
Colon   :                      0x3A
Semicolon  ;                 0x3B
Less_Than_sign      <      0x3C
Equals_sign   =               0x3D
Greater_Than_sign   >     0x3E
Question_Mark   ?           0x3F
Commercial_At   @          0x40
Reverse_Solidus    \        0x5C
Circumflex_Accent   ^     0x5E
Low_Line   _                  0x5F
*/
typedef enum
{
	E_RU_HEX_EXCLAMATION = 0x21,
	E_RU_HEX_QUOTATION = 0x22,
	E_RU_HEX_NUMBER = 0x23,
	E_RU_HEX_DOLLAR = 0x24,
	E_RU_HEX_PERCENT = 0x25,
	E_RU_HEX_AMPERSAND = 0x26,
	E_RU_HEX_APOSTROPHE = 0x27,
	E_RU_HEX_LEFT_PARENTHESIS = 0x28,
	E_RU_HEX_RIGTH_PARENTHESIS = 0x29,
	E_RU_HEX_ASTERISK = 0x2A,
	E_RU_HEX_PLUS = 0x2B,
	E_RU_HEX_COMMA =  0x2C,
	E_RU_HEX_HYPHEN_MINUS =  0x2D,
	E_RU_HEX_FULL_STOP = 0x2E,
	E_RU_HEX_SOLIDUS = 0x2F,
	E_RU_HEX_COLON = 0x3A,
	E_RU_HEX_SEMICOLON = 0x3B,
	E_RU_HEX_LESS_THAN = 0x3C,
	E_RU_HEX_EQUALS = 0x3D,
	E_RU_HEX_GREATER_THAN = 0x3E,
	E_RU_HEX_QUESTION = 0x3D,
	E_RU_HEX_COMMERCAIL_AT = 0x40,
	E_RU_HEX_REVERSE_SOLIDUS = 0x5C,
	E_RU_HEX_CIRCUMFLEX_ACCENT = 0x5E,
	E_RU_HEX_LOW_LINE = 0x5F
}ERuHexChar_t;
class RuSipParse;
class RuSipContentLength;
class RuSipContentsType;
class RuSipCSeq;
class RuSipOtherHeaders;
class RuSipOthers;
class RuSipOther;
class RuSipNameAddrs;
class RuSipNameAddr;
class RuSipHost;
class RuSipVias;
class RuSipVia;
class RuSipParams;
class RuSipParam;
class RuSipFirstLine;
/************************ First Line(Request or Respone) **************************************/
class RuSipFirstLine
{
	public:
		typedef enum 
		{
			E_PARSE_FIRST_NONE = 0,
			E_PARSE_FIRST_SEG1,
			E_PARSE_FIRST_SEG1_SP,
			E_PARSE_FIRST_SEG2,
			E_PARSE_FIRST_SEG2_SP,
			E_PARSE_FIRST_SEG3
		}EParseFirstLine_t;
		typedef bool (*PFuncParseState)(RuSipFirstLine *_pclsObj, const char _cInput);
		RuSipFirstLine();
		~RuSipFirstLine();
		RuSipFirstLine & operator=(RuSipFirstLine & _rclsSrc);
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszDebug, unsigned int _unSize);
		void fnEncode(RUString &_rclsEncode);
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsSeg1," ");
				RUString::fnTrimTailString((RUSTR)m_clsSeg2," ");
				RUString::fnTrimTailString((RUSTR)m_clsSeg3," ");
			}
		}
		EParseFirstLine_t m_eSt;
		RUString m_clsSeg1;   // INVITE or  SIP/2.0
		RUString m_clsSeg2;   // request_uri or Rsp_Code
		RUString m_clsSeg3;   // SIP/2.0 or Rsp_Descrition(OK)
		bool m_bTrim;
	private:
		static bool fnE_PARSE_FIRST_NONE(RuSipFirstLine *_pclsObj,const char _cInput);
		static bool fnE_PARSE_FIRST_SEG1(RuSipFirstLine *_pclsObj,const char _cInput);
		static bool fnE_PARSE_FIRST_SEG1_SP(RuSipFirstLine *_pclsObj,const char _cInput);
		static bool fnE_PARSE_FIRST_SEG2(RuSipFirstLine *_pclsObj,const char _cInput);
		static bool fnE_PARSE_FIRST_SEG2_SP(RuSipFirstLine *_pclsObj,const char _cInput);
		static bool fnE_PARSE_FIRST_SEG3(RuSipFirstLine *_pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_FIRST_SEG3+1];
};
/************************ Common Param **************************************************/
class RuSipParam : public RUObject
{
	public:
		typedef enum
		{
			E_PARSE_PARAM_NONE = 0,
			E_PARSE_PARAM_KEY,
			E_PARSE_PARAM_KEY_SP,
			E_PARSE_PARAM_VAL,
		}EParseRuParam_t;
		typedef bool (*PFuncParseState)(RuSipParam *_pclsObj, const char _cInput);
		RuSipParam();
		~RuSipParam();
		RuSipParam & operator=(RuSipParam & _rclsSrc);
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszDebug, unsigned int _unLen);
		void fnEncode(RUString &_rclsEncode);
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsKey," ");
				RUString::fnTrimTailString((RUSTR)m_clsVal," ");
			}
		}
		RUString m_clsKey;
		RUString m_clsVal;
		EParseRuParam_t m_eSt;
		bool m_bTrim;
	private:
		static bool fnE_PARSE_PARAM_NONE(RuSipParam *_pclsObj,const char _cInput);
		static bool fnE_PARSE_PARAM_KEY(RuSipParam *_pclsObj,const char _cInput);
		static bool fnE_PARSE_PARAM_KEY_SP(RuSipParam *_pclsObj,const char _cInput);
		static bool fnE_PARSE_PARAM_VAL(RuSipParam *_pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_PARAM_VAL+1];
};
/************************ Common Param List ***********************************************/
class RuSipParams
{
	public:
		typedef enum
		{
			E_PARSE_PARAMS_NONE = 0,
			E_PARSE_PARAMS_SP,
			E_PARSE_PARAMS
		}EParseParams_t;
		typedef bool (*PFuncParseState)(RuSipParams *_pclsObj, const char _cInput);
		RuSipParams();
		~RuSipParams();
		RuSipParams & operator=(RuSipParams &_rclsSrc);
		void fnClear(){m_eSt = E_PARSE_PARAMS_NONE;m_pclsCurrentParam=NULL;m_listParams.fnClear();}
		bool fnParse(char _cInput);
		void fnDebug(char * _pszDebug, unsigned int _unLen);
		void fnEncode(RUString &_rclsEncode);
		RuSipParam * fnFindParam(const char * _pszParam);
		RuSipParam * fnAddParam(const char * _pszKey, const char * _pszVal);
		bool fnRemoveParam(const char * _pszKey);
		RuSipParam * fnBegin(){return (RuSipParam *)m_listParams.fnBegin();}
		RuSipParam * fnNext(){return (RuSipParam *)m_listParams.fnNext();}
		RuSipParam * fnBegin(RuListItem_t & _rclsItor){return (RuSipParam *)m_listParams.fnBegin(_rclsItor);}
		RuSipParam * fnNext(RuListItem_t & _rclsItor){return (RuSipParam *)m_listParams.fnNext(_rclsItor);}
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RuListItem_t clsItor;
				RuSipParam *pclsFind = fnBegin(clsItor);
				while(pclsFind)
				{
					pclsFind->fnTrimTailing();
					pclsFind = fnNext(clsItor);
				}
			}
		}
		EParseParams_t m_eSt;
		RUList m_listParams;
		RuSipParam * m_pclsCurrentParam;
		bool m_bTrim;
	private:
		void fnKeyReinit();
		void fnAddParseParam();
		static bool fnE_PARSE_PARAMS_NONE(RuSipParams *_pclsObj,const char _cInput);
		static bool fnE_PARSE_PARAMS_SP(RuSipParams *_pclsObj,const char _cInput);
		static bool fnE_PARSE_PARAMS(RuSipParams *_pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_PARAMS+1];
};
/************************ Via ***********************************************************/
class RuSipVia : public RUObject
{
	public:
		typedef enum
		{
			E_PARSE_VIA_NONE = 0,
			E_PARSE_VIA_PROTO,
			E_PARSE_VIA_PROTO_SP,
			E_PARSE_VIA_VER,
			E_PARSE_VIA_VER_SP,
			E_PARSE_VIA_TRANSPORT,
			E_PARSE_VIA_TRANSPORT_SP,
			E_PARSE_VIA_ADDRESS,
			E_PARSE_VIA_ADDRESS_SP,
			E_PARSE_VIA_PARAMS
		}ESipParseViaSt_t;
		typedef bool (*PFuncParseState)(RuSipVia *_pclsObj, const char _cInput);
		RuSipVia();
		~RuSipVia();
		RuSipVia & operator=(RuSipVia & _rclsSrc);
		void fnClear();
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszDebug,unsigned int _unSize);
		void fnEncode(RUString &_rclsEncode);
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsName," ");
				RUString::fnTrimTailString((RUSTR)m_clsProto," ");
				RUString::fnTrimTailString((RUSTR)m_clsVerNum," ");
				RUString::fnTrimTailString((RUSTR)m_clsTransportType," ");
				RUString::fnTrimTailString((RUSTR)m_clsAddress," ");
				m_clsParams.fnTrimTailing();
			}
		}
		RUString m_clsName;
		RUString m_clsProto;
		RUString m_clsVerNum;
		RUString m_clsTransportType;
		RUString m_clsAddress;
		RuSipParams m_clsParams;
		ESipParseViaSt_t m_eSt;
		bool m_bTrim;
	private:
		static bool fnE_PARSE_VIA_NONE(RuSipVia *_pclsObj, const char _cInput);
		static bool fnE_PARSE_VIA_PROTO(RuSipVia *_pclsObj, const char _cInput);
		static bool fnE_PARSE_VIA_PROTO_SP(RuSipVia *_pclsObj, const char _cInput);
		static bool fnE_PARSE_VIA_VER(RuSipVia *_pclsObj, const char _cInput);
		static bool fnE_PARSE_VIA_VER_SP(RuSipVia *_pclsObj, const char _cInput);
		static bool fnE_PARSE_VIA_TRANSPORT(RuSipVia *_pclsObj, const char _cInput);
		static bool fnE_PARSE_VIA_TRANSPORT_SP(RuSipVia *_pclsObj, const char _cInput);
		static bool fnE_PARSE_VIA_ADDRESS(RuSipVia *_pclsObj, const char _cInput);
		static bool fnE_PARSE_VIA_ADDRESS_SP(RuSipVia *_pclsObj, const char _cInput);
		static bool fnE_PARSE_VIA_PARAMS(RuSipVia *_pclsObj, const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_VIA_PARAMS+1];
};
/************************ Via List ********************************************************/
class RuSipVias
{
	friend class RuSipParse;
	public:
		typedef enum
		{
			E_PARSE_VIAS_NONE = 0,
			E_PARSE_VIAS_SP,
			E_PARSE_VIAS
		}ESipParseViasSt_t;
		typedef bool (*PFuncParseState)(RuSipVias *_pclsObj, const char _cInput);
		RuSipVias();
		~RuSipVias();
		RuSipVias & operator=(RuSipVias & _rclsSrc);
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszDebug,unsigned int _unSize);
		void fnEncode(RUString &_rclsEncode);
		RuSipVia * fnAddVia(bool _bFront=true);
		bool fnDelTopVia();
		RuSipVia * fnBegin(){return (RuSipVia*)m_listVia.fnBegin();}
		RuSipVia * fnNext(){return (RuSipVia*)m_listVia.fnNext();}
		RuSipVia * fnBegin(RuListItem_t & _rclsItor){return (RuSipVia*)m_listVia.fnBegin(_rclsItor);}
		RuSipVia * fnNext(RuListItem_t & _rclsItor){return (RuSipVia*)m_listVia.fnNext(_rclsItor);}
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RuListItem_t clsItor;
				RuSipVia *pclsFind = fnBegin(clsItor);
				while(pclsFind)
				{
					pclsFind->fnTrimTailing();
					pclsFind = fnNext(clsItor);
				}
			}
		}
		RuSipVia *m_pclsCurrentVia;
		RUList m_listVia;	
		ESipParseViasSt_t m_eSt;
		bool m_bTrim;
	private:
		void fnReInitForMultiLine();
		void fnAddParseVia();
		static bool fnE_PARSE_VIAS_NONE(RuSipVias *_pclsObj, const char _cInput);
		static bool fnE_PARSE_VIAS_SP(RuSipVias *_pclsObj, const char _cInput);
		static bool fnE_PARSE_VIAS(RuSipVias *_pclsObj, const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_VIAS+1];
};
/************************ Host **********************************************************/
class RuSipHost : public RUObject
{
	public:
		typedef enum
		{
			E_PARSE_HOST_NONE = 0,
			E_PARSE_HOST_ADDR,
			E_PARSE_HOST_ADDR_SP,
			E_PARSE_HOST_NONEV6_SP,
			E_PARSE_HOST_ADDRV6,
			E_PARSE_HOST_ADDRV6_SP,
			E_PARSE_HOST_PORT,
		}ESipParseHost_t;
		typedef bool (*PFuncParseState)(RuSipHost * _pclsObj, const char _cInput);
		RuSipHost();
		~RuSipHost();
		RuSipHost & operator=(RuSipHost & _rclsSrc);
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszDebug,unsigned int _unSize);
		void fnEncode(RUString &_rclsEncode);
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsAddr," ");
				RUString::fnTrimTailString((RUSTR)m_clsPort," ");
			}
		}
		ESipParseHost_t m_eSt;
		RUString m_clsAddr;
		RUString m_clsPort;
		bool m_bV6;
		bool m_bTrim;
	private:
		static bool fnE_PARSE_HOST_NONE(RuSipHost *_pclsObj, const char _cInput);
		static bool fnE_PARSE_HOST_ADDR(RuSipHost *_pclsObj, const char _cInput);
		static bool fnE_PARSE_HOST_ADDR_SP(RuSipHost *_pclsObj, const char _cInput);
		static bool fnE_PARSE_HOST_NONEV6_SP(RuSipHost *_pclsObj, const char _cInput);
		static bool fnE_PARSE_HOST_ADDRV6(RuSipHost *_pclsObj, const char _cInput);
		static bool fnE_PARSE_HOST_ADDRV6_SP(RuSipHost *_pclsObj, const char _cInput);
		static bool fnE_PARSE_HOST_PORT(RuSipHost *_pclsObj, const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_HOST_PORT+1];
};
/************************ NameAddr ******************************************************/
typedef enum
{
	E_RU_URI_NONE=0,
	E_RU_URI_TEL,
	E_RU_URI_SIP,
	E_RU_URI_SIPS,
	E_RU_URI_FAX,
	E_RU_URI_MODEM,
	E_RU_URI_URN,
	E_RU_URI_UNDEFINED
}ERuSipUri_t;
class RuSipUriScheme
{
	public:
		RuSipUriScheme(){}
		~RuSipUriScheme(){}
		inline static ERuSipUri_t fnEnumScheme(const char * _pszScheme)
		{
			if(fnRUStrnCmp(_pszScheme,"sips",4)==0) return E_RU_URI_SIPS;
			else if(fnRUStrnCmp(_pszScheme,"sip",3)==0) return E_RU_URI_SIP;
			else if(fnRUStrnCmp(_pszScheme,"tel",3)==0) return E_RU_URI_TEL;
			else if(fnRUStrnCmp(_pszScheme,"fax",3)==0) return E_RU_URI_FAX;
			else if(fnRUStrnCmp(_pszScheme,"modem",5)==0) return E_RU_URI_MODEM;
			else if(fnRUStrnCmp(_pszScheme,"urn",3)==0) return E_RU_URI_URN;
			else return E_RU_URI_UNDEFINED;
		}
		inline static const char * fnStringScheme(ERuSipUri_t _eT)
		{
			switch(_eT)
			{
				case E_RU_URI_TEL: return "tel";
				case E_RU_URI_SIP: return "sip";
				case E_RU_URI_SIPS: return "sips";
				case E_RU_URI_FAX: return "fax";
				case E_RU_URI_MODEM: return "modem";
				case E_RU_URI_URN: return "urn";
				default: return "unknown";
			}
			return "unknown";
		}
		inline static bool fnIsDefinedScheme(const char * _pszScheme)
		{
			if(fnEnumScheme(_pszScheme) == E_RU_URI_UNDEFINED) return false;
			return true;
		}
};
class RuSipParseUrn : public RUObject
{
	public:
		typedef enum
		{
			E_PARSE_URN_NONE = 0,
			E_PARSE_URN_NID,
			E_PARSE_URN_NID_SP,
			E_PARSE_URN_NSS
		}ESipParseUrnSt_t;
		typedef bool (*PFuncParseState)(RuSipParseUrn *_pclsObj, const char _cInput);
		RuSipParseUrn();
		~RuSipParseUrn();
		RuSipParseUrn & operator=(RuSipParseUrn & _rclsSrc);
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszSrc,unsigned int _unSize);
		void fnEnconde(RUString &_rclsEncode);
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsNID," ");
				RUString::fnTrimTailString((RUSTR)m_clsNSS," ");
			}
		}
		RUString m_clsNID;
		RUString m_clsNSS;
		ESipParseUrnSt_t m_eSt;
		bool m_bTrim;
	private:
		static bool fnE_PARSE_URN_NONE(RuSipParseUrn *_pclsObj, const char _cInput);
		static bool fnE_PARSE_URN_NID(RuSipParseUrn *_pclsObj, const char _cInput);
		static bool fnE_PARSE_URN_NID_SP(RuSipParseUrn *_pclsObj, const char _cInput);
		static bool fnE_PARSE_URN_NSS(RuSipParseUrn *_pclsObj, const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_URN_NSS+1];
};
class RuSipParseSubHeader : public RUObject
{
	public:
		typedef enum
		{
			E_PARSE_SUB_NONE = 0,
			E_PARSE_SUB_KEY_SP,
			E_PARSE_SUB_KEY,
			E_PARSE_SUB_VALUE_SP,
			E_PARSE_SUB_VALUE
		}ESipParseNameAddrSubSt_t;
		typedef bool (*PFuncParseState)(RuSipParseSubHeader *_pclsObj, const char _cInput);
		RuSipParseSubHeader();
		~RuSipParseSubHeader();
		RuSipParseSubHeader & operator=(RuSipParseSubHeader & _rclsSrc);
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszSrc,unsigned int _unSize);
		void fnEnconde(RUString &_rclsEncode);
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsName," ");
				m_clsParams.fnTrimTailing();
			}
		}
		RUString m_clsName;
		RuSipParams m_clsParams;
		ESipParseNameAddrSubSt_t m_eSt;
		bool m_bTrim;
	private:
		static bool fnE_PARSE_SUB_NONE(RuSipParseSubHeader *_pclsObj, const char _cInput);
		static bool fnE_PARSE_SUB_KEY_SP(RuSipParseSubHeader *_pclsObj, const char _cInput);
		static bool fnE_PARSE_SUB_KEY(RuSipParseSubHeader *_pclsObj, const char _cInput);
		static bool fnE_PARSE_SUB_VALUE_SP(RuSipParseSubHeader *_pclsObj, const char _cInput);
		static bool fnE_PARSE_SUB_VALUE(RuSipParseSubHeader *_pclsObj, const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_SUB_VALUE+1];
};
class RuSipParseSubHeaders : public RUObject
{
	public:
		typedef enum
		{
			E_PARSE_SUB_HDR_NONE = 0,
			E_PARSE_SUB_HDR_SP,
			E_PARSE_SUB_HDR_VALUE
		}ESipParseNameAddrSubSt_t;
		typedef bool (*PFuncParseState)(RuSipParseSubHeaders *_pclsObj, const char _cInput);
		RuSipParseSubHeaders();
		~RuSipParseSubHeaders();
		RuSipParseSubHeaders & operator=(RuSipParseSubHeaders & _rclsSrc);
		void fnClear(){m_pclsCurrentSub = NULL;m_eSt = E_PARSE_SUB_HDR_NONE;m_clslist.fnClear();}
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszSrc,unsigned int _unSize);
		void fnEnconde(RUString &_rclsEncode);
		RuSipParseSubHeader * fnFindHeader(const char * _pszName);
		bool fnRemoveHeader(const char * _pszName);
		RuSipParseSubHeader* fnAddSubHeader(){return fnAddParseSubHeader();}
		RuSipParseSubHeader * fnBegin(){return (RuSipParseSubHeader*)m_clslist.fnBegin();}
		RuSipParseSubHeader * fnNext(){return (RuSipParseSubHeader*)m_clslist.fnNext();}
		RuSipParseSubHeader * fnBegin(RuListItem_t &_rclsIter){return (RuSipParseSubHeader*)m_clslist.fnBegin(_rclsIter);}
		RuSipParseSubHeader * fnNext(RuListItem_t &_rclsIter){return (RuSipParseSubHeader*)m_clslist.fnNext(_rclsIter);}
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RuListItem_t clsItor;
				RuSipParseSubHeader *pclsFind = fnBegin(clsItor);
				while(pclsFind)
				{
					pclsFind->fnTrimTailing();
					pclsFind = fnNext(clsItor);
				}
			}
		}
		RUList m_clslist;
		RuSipParseSubHeader * m_pclsCurrentSub;
		ESipParseNameAddrSubSt_t m_eSt;
		bool m_bTrim;
	private:
		RuSipParseSubHeader* fnAddParseSubHeader();
		static bool fnE_PARSE_SUB_HDR_NONE(RuSipParseSubHeaders *_pclsObj, const char _cInput);
		static bool fnE_PARSE_SUB_HDR_SP(RuSipParseSubHeaders *_pclsObj, const char _cInput);
		static bool fnE_PARSE_SUB_HDR_VALUE(RuSipParseSubHeaders *_pclsObj, const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_SUB_HDR_VALUE+1];
};
// RawDisplay <RawUri> RawParam
class RuSipNameAddr : public  RUObject
{
	public:
		typedef enum
		{
			E_PARSE_NAME_ADDR_NONE = 0,
			E_PARSE_NAME_ADDR_DISPLAY,
			E_PARSE_NAME_ADDR_DISPLAY_SP,
			E_PARSE_NAME_ADDR_TYPE,
			E_PARSE_NAME_ADDR_TYPE_SP,
			E_PARSE_NAME_ADDR_USER,
			E_PARSE_NAME_ADDR_USER_SP,
			E_PARSE_NAME_ADDR_HOST,
			E_PARSE_NAME_ADDR_HOST_SP,
			E_PARSE_NAME_ADDR_URI_PARAMS,
			E_PARSE_NAME_ADDR_URI_SP,
			E_PARSE_NAME_ADDR_PARAMS,
			E_PARSE_NAME_ADDR_URI_QSP,
			E_PARSE_NAME_ADDR_SUBHEADERS,
			E_PARSE_NAME_ADDR_EXT_DISPLAY_SP,
			E_PARSE_NAME_ADDR_EXT_TYPE,
			E_PARSE_NAME_ADDR_EXT_TYPE_SP,
			E_PARSE_NAME_ADDR_URN
		}ESipParseNameAddr_t;
		typedef bool (*PFuncParseState)(RuSipNameAddr * _pclsObj, const char _cInput);
		RuSipNameAddr();
		~RuSipNameAddr();
		RuSipNameAddr & operator=(RuSipNameAddr & _rclsSrc);
		bool fnParse(const char _cInput);
		bool fnCheckParsed();   // after parse, must call this function
		void fnDebug(char * _pszDebug,unsigned int _unSize);
		void fnEncode(RUString &_rclsEncode,bool _bCrLf=true);
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsDisplay," ");
				RUString::fnTrimTailString((RUSTR)m_clsType," ");
				RUString::fnTrimTailString((RUSTR)m_clsUser," ");
				RUString::fnTrimTailString((RUSTR)m_clsHost," ");
				m_clsUriParams.fnTrimTailing();
				m_clsUriSubHeaders.fnTrimTailing();
				m_clsParams.fnTrimTailing();
			}
		}
		const char *fnFindParam(const char * _pszName);
		RuSipParseSubHeader * fnFindSubHeader(const char * _pszName);
		const char * fnFindSubHeaderParam(const char * _pszSubHdr, const char * _pszParam);
		void fnAddUriParam(const char * _pszName, const char * _pszVal);
		bool fnRemoveSubHeader(const char * _pszName);
		RuSipParseSubHeader * fnAddSubHeader(const char * _pszSubName);
		RUString m_clsDisplay;
		RUString m_clsType;
		RUString m_clsUser;
		RUString m_clsHost;
		RuSipParams m_clsUriParams;
		RuSipParams m_clsParams;
		RuSipParseSubHeaders m_clsUriSubHeaders;
		RuSipParseUrn m_clsUrn;
		ESipParseNameAddr_t m_eSt;
		ERuSipUri_t m_eScheme;
		bool m_bThanSign;
		RUString m_clsRawUri;
		RUString m_clsRawParams;
		bool m_bCheckDisplayed;
		bool m_bCheckParsed;
		bool m_bAllStar;
		bool m_bTrim;
	private:
		RuSipParseSubHeader * fnAddParseSubHeader(const char * _pszSubName);
		static bool fnE_PARSE_NAME_ADDR_NONE(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_DISPLAY(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_DISPLAY_SP(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_TYPE(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_TYPE_SP(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_USER(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_USER_SP(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_HOST(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_HOST_SP(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_URI_PARAMS(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_URI_SP(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_PARAMS(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_URI_QSP(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_SUBHEADERS(RuSipNameAddr *_pclsObj, const char _cInput);
		// Exception Handle (no user)
		static bool fnE_PARSE_NAME_ADDR_EXT_DISPLAY_SP(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_EXT_TYPE(RuSipNameAddr *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAME_ADDR_EXT_TYPE_SP(RuSipNameAddr *_pclsObj, const char _cInput);
		// Urn Handle
		static bool fnE_PARSE_NAME_ADDR_URN(RuSipNameAddr *_pclsObj, const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_NAME_ADDR_URN+1];
};
/************************ NameAddrs *****************************************************/
class RuSipNameAddrs
{
	friend class RuSipParse;
	public:
		typedef enum
		{
			E_PARSE_NAMEADDRS_NONE = 0,
			E_PARSE_NAMEADDRS_SP,
			E_PARSE_NAMEADDRS
		}ESipParseNameAddrsSt_t;
		typedef bool (*PFuncParseState)(RuSipNameAddrs *_pclsObj, const char _cInput);
		RuSipNameAddrs();
		~RuSipNameAddrs();
		RuSipNameAddrs & operator=(RuSipNameAddrs & _rclsSrc);
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszDebug,unsigned int _unSize);
		void fnEncode(RUString &_rclsEncode);
		RuSipNameAddr * fnAddNameAddr(bool _bFront=true);
		void fnReInitForMultiLine(const char * _pszName); // next header parse
		bool fnDelNameAddr(bool _bFront=true);
		void fnReverseNameAddr();
		RuSipNameAddr * fnBegin()
		{
			RuSipNameAddr * pclsTmp = (RuSipNameAddr*)m_listNameAddr.fnBegin();
			if(pclsTmp) pclsTmp->fnCheckParsed();
			return pclsTmp;
		}
		RuSipNameAddr * fnNext()
		{
			RuSipNameAddr * pclsTmp = (RuSipNameAddr*)m_listNameAddr.fnNext();
			if(pclsTmp) pclsTmp->fnCheckParsed();
			return pclsTmp;
		}
		RuSipNameAddr * fnBegin(RuListItem_t & _rclsItor)
		{
			RuSipNameAddr * pclsTmp = (RuSipNameAddr*)m_listNameAddr.fnBegin(_rclsItor);
			if(pclsTmp) pclsTmp->fnCheckParsed();
			return pclsTmp;
		}
		RuSipNameAddr * fnNext(RuListItem_t & _rclsItor)
		{
			RuSipNameAddr * pclsTmp = (RuSipNameAddr*)m_listNameAddr.fnNext(_rclsItor);
			if(pclsTmp) pclsTmp->fnCheckParsed();
			return pclsTmp;
		}
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsName," ");
				RuListItem_t clsItor;
				RuSipNameAddr *pclsFind = fnBegin(clsItor);
				while(pclsFind)
				{
					pclsFind->fnTrimTailing();
					pclsFind = fnNext(clsItor);
				}
			}
		}
		RuSipNameAddr *m_pclsCurrentAddr;
		RUString m_clsName;
		RUList m_listNameAddr;
		ESipParseNameAddrsSt_t m_eSt;
		bool m_bTrim;
	private:
		void fnAddParseNameAddr(const char * _pszName);
		static bool fnE_PARSE_NAMEADDRS_NONE(RuSipNameAddrs *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAMEADDRS_SP(RuSipNameAddrs *_pclsObj, const char _cInput);
		static bool fnE_PARSE_NAMEADDRS(RuSipNameAddrs *_pclsObj, const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_NAMEADDRS+1];
};
/************************ Other Header ***************************************************/
class RuSipOther : public RUObject
{
	public:
		RuSipOther();
		~RuSipOther();
		RuSipOther & operator=(RuSipOther & _rclsSrc);
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszDebug, unsigned int _unSize);
		void fnEncode(RUString &_rclsEncode);
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsVal," ");
			}
		}
		RUString m_clsVal;
		bool m_bTrim;
};
/************************ Multi Line Other Header ********************************************/
class RuSipOthers : public RUObject
{
	friend class RuSipParse;
	friend class RuSipOtherHeaders;
	public:
		typedef enum
		{
			E_PARSE_OTHERS_NONE = 0,
			E_PARSE_OTHERS_SP,
			E_PARSE_OTHERS
		}ESipParseOthersSt_t;
		typedef bool (*PFuncParseState)(RuSipOthers *_pclsObj, const char _cInput);
		RuSipOthers();
		~RuSipOthers();
		RuSipOthers & operator=(RuSipOthers & _rclsSrc);
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszDebug, unsigned int _unSize);
		void fnEncode(RUString &_rclsEncode);
		RuSipOther * fnAddHeader(bool _bFront=false);
		RuSipOther * fnBegin(){return (RuSipOther*)m_listOther.fnBegin();}
		RuSipOther * fnNext(){return (RuSipOther*)m_listOther.fnNext();}
		RuSipOther * fnBegin(RuListItem_t & _rclsItor){return (RuSipOther*)m_listOther.fnBegin(_rclsItor);}
		RuSipOther * fnNext(RuListItem_t & _rclsItor){return (RuSipOther*)m_listOther.fnNext(_rclsItor);}
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsKey," ");
				RuListItem_t clsItor;
				RuSipOther *pclsFind = fnBegin(clsItor);
				while(pclsFind)
				{
					pclsFind->fnTrimTailing();
					pclsFind = fnNext(clsItor);
				}
			}
		}
		RUString m_clsKey;
		RuSipOther *m_pclsCurrent;
		RUList m_listOther;
		ESipParseOthersSt_t m_eSt;
		bool m_bTrim;
	private:
		void fnReInitForMultiLine();
		void fnAddParseHeader();
		static bool fnE_PARSE_OTHERS_NONE(RuSipOthers *_pclsObj, const char _cInput);
		static bool fnE_PARSE_OTHERS_SP(RuSipOthers *_pclsObj, const char _cInput);
		static bool fnE_PARSE_OTHERS(RuSipOthers *_pclsObj, const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_OTHERS+1];
};
/************************ Total Other Header ***********************************************/
class RuSipOtherHeaders
{
	friend class RuSipParse;
	public:
		RuSipOtherHeaders();
		~RuSipOtherHeaders();
		RuSipOtherHeaders & operator=(RuSipOtherHeaders & _rclsSrc);
		RuSipOthers * fnFind(const char * _pszHeader);
		RuSipOther * fnAddHeader(const char * _pszHeader,RuSipOthers ** _ppOthers,bool _bFront=false);
		void fnDebug(char * _pszDebug, unsigned int _unSize);
		void fnEncode(RUString &_rclsEncode);
		RuSipOthers * fnBegin(){return (RuSipOthers*)m_listOther.fnBegin();}
		RuSipOthers * fnNext(){return (RuSipOthers*)m_listOther.fnNext();}
		RuSipOthers * fnBegin(RuListItem_t & _rclsItor){return (RuSipOthers*)m_listOther.fnBegin(_rclsItor);}
		RuSipOthers * fnNext(RuListItem_t & _rclsItor){return (RuSipOthers*)m_listOther.fnNext(_rclsItor);}
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RuListItem_t clsItor;
				RuSipOthers *pclsFind = fnBegin(clsItor);
				while(pclsFind)
				{
					pclsFind->fnTrimTailing();
					pclsFind = fnNext(clsItor);
				}
			}
		}
		RUList m_listOther;
		bool m_bTrim;
	private:
		RuSipOthers *fnSet(const char * _pszHeader);
};
/************************ CSeq *********************************************************/
class RuSipCSeq
{
	public:
		typedef enum
		{
			E_PARSE_CSEQ_NONE = 0,
			E_PARSE_CSEQ_NUM,
			E_PARSE_CSEQ_NUM_SP,
			E_PARSE_CSEQ_METHOD,
		}EParseRuCSeq_t;
		typedef bool (*PFuncParseState)(RuSipCSeq *_pclsObj, const char _cInput);
		RuSipCSeq();
		~RuSipCSeq();
		RuSipCSeq & operator=(RuSipCSeq & _rclsSrc);
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszDebug, unsigned int _unLen);
		void fnEncode(RUString &_rclsEncode);
		int fnGetSeqNum();
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsNum," ");
				RUString::fnTrimTailString((RUSTR)m_clsMethod," ");
			}
		}
		RUString m_clsNum;
		RUString m_clsMethod;
		int m_nNum;
		EParseRuCSeq_t m_eSt;
		bool m_bTrim;
	private:
		static bool fnE_PARSE_CSEQ_NONE(RuSipCSeq *_pclsObj,const char _cInput);
		static bool fnE_PARSE_CSEQ_NUM(RuSipCSeq *_pclsObj,const char _cInput);
		static bool fnE_PARSE_CSEQ_NUM_SP(RuSipCSeq *_pclsObj,const char _cInput);
		static bool fnE_PARSE_CSEQ_METHOD(RuSipCSeq *_pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_CSEQ_METHOD+1];
};
/************************ Contents-Type **************************************************/
class RuSipContentsType
{
	public:
		typedef enum
		{
			E_PARSE_CTYPE_NONE = 0,
			E_PARSE_CTYPE_SEG1,
			E_PARSE_CTYPE_SEG1_SP,
			E_PARSE_CTYPE_SEG2,
		}EParseRuCType_t;
		typedef bool (*PFuncParseState)(RuSipContentsType *_pclsObj, const char _cInput);
		RuSipContentsType();
		~RuSipContentsType();
		RuSipContentsType & operator=(RuSipContentsType & _rclsSrc);
		bool fnParse(const char _cInput);
		void fnDebug(char * _pszDebug, unsigned int _unLen);
		void fnEncode(RUString &_rclsEncode);
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsSeg1," ");
				RUString::fnTrimTailString((RUSTR)m_clsSeg2," ");
			}
		}
		RUString m_clsSeg1;
		RUString m_clsSeg2;
		EParseRuCType_t m_eSt;
		bool m_bTrim;
	private:
		static bool fnE_PARSE_CTYPE_NONE(RuSipContentsType *_pclsObj,const char _cInput);
		static bool fnE_PARSE_CTYPE_SEG1(RuSipContentsType *_pclsObj,const char _cInput);
		static bool fnE_PARSE_CTYPE_SEG1_SP(RuSipContentsType *_pclsObj,const char _cInput);
		static bool fnE_PARSE_CTYPE_SEG2(RuSipContentsType *_pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_CTYPE_SEG2+1];
};
/************************ Content-Length *************************************************/
class RuSipContentLength
{
	public:
		RuSipContentLength();
		~RuSipContentLength();
		RuSipContentLength & operator=(RuSipContentLength & _rclsSrc);
		bool fnParse(const char _cInput);
		int fnGetLen();
		void fnDebug(char * _pszDebug, unsigned int _unSize);
		void fnEncode(RUString &_rclsEncode);
		void fnTrimTailing()
		{
			if(m_bTrim == false)
			{
				m_bTrim = true;
				RUString::fnTrimTailString((RUSTR)m_clsVal," ");
			}
		}
		RUString m_clsVal;
		int m_nLen;
		bool m_bTrim;
};
/************************ SIP Parse ******************************************************/
class RuSipParse
{
	public:
		typedef enum
		{
			E_SIP_PARSE_NONE = 0,
			E_SIP_PARSE_FIRSTLINE,
			E_SIP_PARSE_FIRSTLINE_CR,
			E_SIP_PARSE_FIRSTLINE_LF,
			E_SIP_PARSE_H_NAME,
			E_SIP_PARSE_H_NMAE_SP,
			E_SIP_PARSE_H_VAL,
			E_SIP_PARSE_H_FROM,
			E_SIP_PARSE_H_TO,
			E_SIP_PARSE_H_CALLID,
			E_SIP_PARSE_H_CONTACT,
			E_SIP_PARSE_H_R,
			E_SIP_PARSE_H_RR,
			E_SIP_PARSE_H_VIA,
			E_SIP_PARSE_H_CTYPE,
			E_SIP_PARSE_H_CLEN,
			E_SIP_PARSE_H_CSEQ,
			E_SIP_PARSE_H_CR,
			E_SIP_PARSE_H_LF,
			E_SIP_PARSE_H_DOUBLE_CR,
			E_SIP_PARSE_H_DOUBLE_LF,
			E_SIP_PARSE_BODY
		}ESipMsgParse_t;
		typedef bool (*PFuncParseState)(RuSipParse * _pclsObj, const char _cInput);
		RuSipParse();
		~RuSipParse();
		bool fnParse(const char * _pszMsg, unsigned int _unLen);
		void fnDebug(char * _pszDebug, unsigned int _unSize);
		void fnEncode(RUString &_rclsEncode);
		// Parsing Temp Data
		ESipMsgParse_t m_eSt;
		RUString m_clsTmpHeaderName;
		RuSipOthers * m_pclsCurrentOther;
		// Parsing Data
		RuSipFirstLine m_clsFirstLine;             // Request/Response Line
		RuSipNameAddrs m_clsFrom;              // From
		RuSipNameAddrs m_clsTo;                  // To
		RUString m_clsCallID;                       // Call-ID
		RuSipNameAddrs m_clsContact;          // Contact
		RuSipNameAddrs m_clsR;                   // Route
		RuSipNameAddrs m_clsRR;                 // Record-Route
		RuSipVias m_clsVias;                        // Vias
		RuSipCSeq m_clsSeq;                       // CSeq
		RuSipOtherHeaders m_clsOthers;        // Others
		RuSipContentsType m_clsCType;        // Content-Type
		RuSipContentLength m_clsLength;      // Content-Length
		RUString m_clsBody;                        // Body
	private:
		static bool fnE_SIP_PARSE_NONE(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_FIRSTLINE(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_FIRSTLINE_CR(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_FIRSTLINE_LF(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_NAME(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_NMAE_SP(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_VAL(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_FROM(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_TO(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_CALLID(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_CONTACT(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_R(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_RR(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_VIA(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_CTYPE(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_CLEN(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_CSEQ(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_CR(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_LF(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_DOUBLE_CR(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_H_DOUBLE_LF(RuSipParse *_pclsObj, const char _cInput);
		static bool fnE_SIP_PARSE_BODY(RuSipParse *_pclsObj, const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_SIP_PARSE_BODY+1];
};
bool g_fnSipParse(const char * _pszMsg,unsigned int _unLen);
void SipParseTest();
#endif

