#ifndef _KSDPATTR_PARSE_HXX_
#define _KSDPATTR_PARSE_HXX_
#include "string.hxx"

namespace eSipUtil
{
/************************ Common Param **************************************************/
class KSdpParam : public StlObject
{
	public:
		typedef enum
		{
			E_PARSE_PARAM_NONE = 0,
			E_PARSE_PARAM_KEY,
			E_PARSE_PARAM_KEY_SP,
			E_PARSE_PARAM_VAL,
		}EParseRuParam_t;
		typedef bool (*PFuncParseState)(KSdpParam *_pclsObj, const char _cInput);
		KSdpParam();
		~KSdpParam();
		KSdpParam & operator=(KSdpParam & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth);
		void m_fnEncode(KString &_rclsEncode);
		void m_fnTrimTailing();
		KString m_clsKey;
		KString m_clsVal;
		EParseRuParam_t m_eSt;
		bool m_bTrim;
	private:
		static bool m_fnE_PARSE_PARAM_NONE(KSdpParam *_pclsObj,const char _cInput);
		static bool m_fnE_PARSE_PARAM_KEY(KSdpParam *_pclsObj,const char _cInput);
		static bool m_fnE_PARSE_PARAM_KEY_SP(KSdpParam *_pclsObj,const char _cInput);
		static bool m_fnE_PARSE_PARAM_VAL(KSdpParam *_pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_PARAM_VAL+1];
};
/************************ Common Param List ***********************************************/
class KSdpParams
{
	public:
		typedef enum
		{
			E_PARSE_PARAMS_NONE = 0,
			E_PARSE_PARAMS_SP,
			E_PARSE_PARAMS
		}EParseParams_t;
		typedef bool (*PFuncParseState)(KSdpParams *_pclsObj, const char _cInput);
		KSdpParams();
		~KSdpParams();
		KSdpParams & operator=(KSdpParams &_rclsSrc);
		void m_fnClear(){m_eSt = E_PARSE_PARAMS_NONE;m_pclsCurrentParam=NULL;m_listParams.m_fnClear();}
		bool m_fnParse(char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth);
		void m_fnEncode(KString &_rclsEncode);
		KSdpParam * m_fnFindParam(const char * _pszParam);
		KSdpParam * m_fnAddParam(const char * _pszKey, const char * _pszVal);
		bool m_fnRemoveParam(const char * _pszKey);
		KSdpParam * m_fnBegin(ListItr_t & _rclsItor){return (KSdpParam *)m_listParams.m_fnBegin(_rclsItor);}
		KSdpParam * m_fnNext(ListItr_t & _rclsItor){return (KSdpParam *)m_listParams.m_fnNext(_rclsItor);}
		void m_fnTrimTailing();
		EParseParams_t m_eSt;
		StlList m_listParams;
		KSdpParam * m_pclsCurrentParam;
		bool m_bTrim;
		ListItr_t m_stItor;
	private:
		void m_fnKeyReinit();
		void m_fnAddParseParam();
		static bool m_fnE_PARSE_PARAMS_NONE(KSdpParams *_pclsObj,const char _cInput);
		static bool m_fnE_PARSE_PARAMS_SP(KSdpParams *_pclsObj,const char _cInput);
		static bool m_fnE_PARSE_PARAMS(KSdpParams *_pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_PARSE_PARAMS+1];
};
typedef enum
{
	E_KSDP_A_NONE = 0,
	E_KSDP_A_RTPMAP,
	E_KSDP_A_FMTP,
	E_KSDP_A_MAX
}ESdpAttr_t;
class KSdpAttr : public StlObject
{
	public:
		KSdpAttr(){}
		~KSdpAttr(){}
	private:
		
};
class KSdpAttrRtpMap : public KSdpAttr
{
	public:
		//a=rtpmap:<payload type> <encoding name>/<clock rate> [/<encoding parameters>]
		typedef enum
		{
			E_SDP_A_RTPMAP_NONE=0,
			E_SDP_A_RTPMAP_PT,
			E_SDP_A_RTPMAP_PT_SP,
			E_SDP_A_RTPMAP_ENCODING,
			E_SDP_A_RTPMAP_ENCODING_SP,
			E_SDP_A_RTPMAP_CLOCK,
			E_SDP_A_RTPMAP_CLOCK_SP,
			E_SDP_A_RTPMAP_PARAMS,
			E_SDP_A_RTPMAP_MAX
		}EKSdpParse_t;
		typedef bool (*PFuncParseState)(KSdpAttrRtpMap* _pclsObj, const char _cInput);
		KSdpAttrRtpMap();
		~KSdpAttrRtpMap();
		KSdpAttrRtpMap & operator=(KSdpAttrRtpMap & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		EKSdpParse_t m_eSt;
		KString m_clsPayloadType;
		KString m_clsEncodingName;
		KString m_clsClockRate;
		KString m_clsEncodingParams;
	private:
		static bool m_fnE_SDP_A_RTPMAP_NONE(KSdpAttrRtpMap * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_RTPMAP_PT(KSdpAttrRtpMap * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_RTPMAP_PT_SP(KSdpAttrRtpMap * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_RTPMAP_ENCODING(KSdpAttrRtpMap * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_RTPMAP_ENCODING_SP(KSdpAttrRtpMap * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_RTPMAP_CLOCK(KSdpAttrRtpMap * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_RTPMAP_CLOCK_SP(KSdpAttrRtpMap * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_RTPMAP_PARAMS(KSdpAttrRtpMap * _pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_SDP_A_RTPMAP_MAX];
};
class KSdpAttrFmtp : public KSdpAttr
{
	public:
		//a=fmtp:<format> <format specific parameters>,    key=val;key1=val;.....
		typedef enum
		{
			E_SDP_A_FMTP_NONE=0,
			E_SDP_A_FMTP_FORMAT,
			E_SDP_A_FMTP_FORMAT_SP,
			E_SDP_A_FMTP_PARAMS,
			E_SDP_A_FMTP_MAX
		}EKSdpParse_t;
		typedef bool (*PFuncParseState)(KSdpAttrFmtp* _pclsObj, const char _cInput);
		KSdpAttrFmtp();
		~KSdpAttrFmtp();
		KSdpAttrFmtp & operator=(KSdpAttrFmtp & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth);
		void m_fnEncode(KString & _rclsEncode);
		EKSdpParse_t m_eSt;
		KString m_clsFormat;
		KSdpParams m_clsParams;
	private:
		static bool m_fnE_SDP_A_FMTP_NONE(KSdpAttrFmtp * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_FMTP_FORMAT(KSdpAttrFmtp * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_FMTP_FORMAT_SP(KSdpAttrFmtp * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_FMTP_PARAMS(KSdpAttrFmtp * _pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_SDP_A_FMTP_MAX];
};
class KSdpAttrImageAttr : public KSdpAttr
{
	public:
		// RFC 6236 - Negotiation of Generic Image Attributes
		/*
		image-attr = "imageattr:" PT 1*2( 1*WSP ( "send" / "recv" )1*WSP attr-list )
	       PT = 1*DIGIT / "*"
	       attr-list = ( set *(1*WSP set) ) / "*"
	         ;  WSP and DIGIT defined in [RFC5234]
	       set= "[" "x=" xyrange "," "y=" xyrange *( "," key-value ) "]"
	                  ; x is the horizontal image size range (pixel count)
	                  ; y is the vertical image size range (pixel count)
	       key-value = ( "sar=" srange )
	                 / ( "par=" prange )
	                 / ( "q=" qvalue )
	                  ; Key-value MAY be extended with other keyword
	                  ;  parameters.
	                  ; At most, one instance each of sar, par, or q
	                  ;  is allowed in a set.
	                  ;
	                  ; sar (sample aspect ratio) is the sample aspect ratio
	                  ;  associated with the set (optional, MAY be ignored)
	                  ; par (picture aspect ratio) is the allowed
	                  ;  ratio between the display's x and y physical
	                  ;  size (optional)
	                  ; q (optional, range [0.0..1.0], default value 0.5)
	                  ;  is the preference for the given set,
	                  ;  a higher value means a higher preference
	       onetonine = "1" / "2" / "3" / "4" / "5" / "6" / "7" / "8" / "9"
	                  ; Digit between 1 and 9
	       xyvalue = onetonine *5DIGIT
	                  ; Digit between 1 and 9 that is
	                  ; followed by 0 to 5 other digits
	       step = xyvalue
	       xyrange = ( "[" xyvalue ":" [ step ":" ] xyvalue "]" )
	                  ; Range between a lower and an upper value
	                  ; with an optional step, default step = 1
	                  ; The rightmost occurrence of xyvalue MUST have a
	                  ; higher value than the leftmost occurrence.
	               / ( "[" xyvalue 1*( "," xyvalue ) "]" )
	                  ; Discrete values separated by ','
	               / ( xyvalue )
	                  ; A single value
	       spvalue = ( "0" "." onetonine *3DIGIT )
	                  ; Values between 0.1000 and 0.9999
	               / ( onetonine "." 1*4DIGIT )
	                  ; Values between 1.0000 and 9.9999
	       srange =  ( "[" spvalue 1*( "," spvalue ) "]" )
	                  ; Discrete values separated by ','.
	                  ; Each occurrence of spvalue MUST be
	                  ; greater than the previous occurrence.
	               / ( "[" spvalue "-" spvalue "]" )
	                  ; Range between a lower and an upper level (inclusive)
	                  ; The second occurrence of spvalue MUST have a higher
	                  ; value than the first
	               / ( spvalue )
	                  ; A single value
	       prange =  ( "[" spvalue "-" spvalue "]" )
	                  ; Range between a lower and an upper level (inclusive)
	                  ; The second occurrence of spvalue MUST have a higher
	                  ; value than the first
	       qvalue  = ( "0" "." 1*2DIGIT )
	               / ( "1" "." 1*2("0") )
	                  ; Values between 0.00 and 1.00
		*/
		//a=imageattr:<payload type> send [x=320,y=240,sar=[0.91,1.0,1.09,1.45]] recv [x=320,y=240]
		KSdpAttrImageAttr(){}
		~KSdpAttrImageAttr(){}
		KSdpAttrImageAttr & operator=(KSdpAttrImageAttr & _rclsSrc)
		{
			return *this;
		}

};
class KSdpAttrVisitedRealm : public KSdpAttr
{
	public:
		// 3GPP TS 29.079 [37] Optimal Media Routing
		//a=visited-realm: <instance> <realm> <addrtype> <addr> <port>
		// a=visited-realm:1 abcX.sktims.net IN IP4 192.0.2.1 1000 net-type nat
		KSdpAttrVisitedRealm(){}
		~KSdpAttrVisitedRealm(){}
		KSdpAttrVisitedRealm & operator=(KSdpAttrVisitedRealm & _rclsSrc)
		{
			m_clsInstance = _rclsSrc.m_clsInstance;
			m_clsRealm = _rclsSrc.m_clsRealm;
			m_clsAddrType = _rclsSrc.m_clsAddrType;
			m_clsAddr = _rclsSrc.m_clsAddr;
			m_clsPort = m_clsPort;
			return *this;
		}
		KString m_clsInstance;
		KString m_clsRealm;
		KString m_clsAddrType;
		KString m_clsAddr;
		KString m_clsPort;
};
/*
OMR etc Attributes...
a=omr-m-cksum: <hexNumber>
a=omr-s-cksum: <hexNumber>
a=omr-codecs:<instance>:<proto> 1*<codec>
a=omr-m-att: <instance>:1* <attribute>
a=omr-s-att: <instance>:1* <attribute>
a=omr-m-bw:<instance>:<bandwidth>
a=omr-s-bw:<instance>:<bandwidth>
*/
}
#endif

