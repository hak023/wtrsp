#ifndef _KSDP_PARSE_HXX_
#define _KSDP_PARSE_HXX_
#include "string.hxx"
#include "KSdpAttrParse.hxx"
/*
	RFC 4566 Parser

*/
namespace eSipUtil
{
void g_fnTestKSdp();
/************************************* V **********************************************/
class KSdpV : public StlObject
{
	public: 
		// <type>=<value>
		KSdpV();
		~KSdpV();
		KSdpV & operator=(KSdpV & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		KString m_clsVal;
};
/************************************* O **********************************************/
class KSdpO : public StlObject
{
	public: 
		//o=<username> <session id> <version> <network type> <address type>  <address>
		//o=pointi 1 1-1 IN IP4 192.168.0.86
		typedef enum
		{
			E_SDP_O_NONE = 0,
			E_SDP_O_USERNAME,
			E_SDP_O_USERNAME_SP,
			E_SDP_O_SESSIONID,
			E_SDP_O_SESSIONID_SP,
			E_SDP_O_VERSION,
			E_SDP_O_VERSION_SP,
			E_SDP_O_NETTYPE,
			E_SDP_O_NETTYPE_SP,
			E_SDP_O_ADDRTYPE,
			E_SDP_O_ADDRTYPE_SP,
			E_SDP_O_ADDR,
			E_SDP_O_MAX
		}EKSdpParse_t;
		typedef bool (*PFuncParseState)(KSdpO * _pclsObj, const char _cInput);
		KSdpO();
		~KSdpO();
		KSdpO & operator=(KSdpO & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		EKSdpParse_t m_eSt;
		KString m_clsUserName;
		KString m_clsSessionID;
		KString m_clsVersion;
		KString m_clsNetType;
		KString m_clsAddrType;
		KString m_clsAddr;
		KString m_clsVal;
	private:
		static bool m_fnE_SDP_O_NONE(KSdpO * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_O_USERNAME(KSdpO * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_O_USERNAME_SP(KSdpO * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_O_SESSIONID(KSdpO * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_O_SESSIONID_SP(KSdpO * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_O_VERSION(KSdpO * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_O_VERSION_SP(KSdpO * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_O_NETTYPE(KSdpO * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_O_NETTYPE_SP(KSdpO * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_O_ADDRTYPE(KSdpO * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_O_ADDRTYPE_SP(KSdpO * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_O_ADDR(KSdpO * _pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_SDP_O_MAX];
};
/***************************************** S ******************************************/
class KSdpS : public StlObject
{
	public: 
		// <type>=<value>
		KSdpS();
		~KSdpS();
		KSdpS & operator=(KSdpS & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		KString m_clsVal;
};
/**************************************** I *******************************************/
class KSdpI : public StlObject
{
	public: 
		KSdpI();
		~KSdpI();
		KSdpI & operator=(KSdpI & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		KString m_clsVal;
};
/*************************************** U *******************************************/
class KSdpU : public StlObject
{
	public: 
		// <type>=<value>
		KSdpU();
		~KSdpU();
		KSdpU & operator=(KSdpU & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		KString m_clsVal;
};
/**************************************** E *******************************************/
class KSdpE : public StlObject
{
	public: 
		// <type>=<value>
		KSdpE();
		~KSdpE();
		KSdpE & operator=(KSdpE & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		KString m_clsVal;
};
/*************************************** P ********************************************/
class KSdpP : public StlObject
{
	public: 
		// <type>=<value>
		KSdpP();
		~KSdpP();
		KSdpP & operator=(KSdpP & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		KString m_clsVal;
};
/**************************************** C *******************************************/
class KSdpC : public StlObject
{
	public: 
		//c=<network type> <address type> <connection address>
		//c=IN IP4 192.168.0.86
		typedef enum
		{
			E_SDP_C_NONE = 0,
			E_SDP_C_NETTYPE,
			E_SDP_C_NETTYPE_SP,
			E_SDP_C_ADDRTYPE,
			E_SDP_C_ADDRTYPE_SP,
			E_SDP_C_CONNECTION,
			E_SDP_C_MAX
		}EKSdpParse_t;
		typedef bool (*PFuncParseState)(KSdpC * _pclsObj, const char _cInput);
		KSdpC();
		~KSdpC();
		KSdpC & operator=(KSdpC & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		EKSdpParse_t m_eSt;
		KString m_clsNetType;
		KString m_clsAddrType;
		KString m_clsConnection;
		KString m_clsVal;
	private:
		static bool m_fnE_SDP_C_NONE(KSdpC * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_C_NETTYPE(KSdpC * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_C_NETTYPE_SP(KSdpC * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_C_ADDRTYPE(KSdpC * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_C_ADDRTYPE_SP(KSdpC * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_C_CONNECTION(KSdpC * _pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_SDP_C_MAX];
};
/*************************************** B ********************************************/
class KSdpB : public StlObject
{
	public: 
		//b=<modifier>:<bandwidth-value>
		//b=AS:1000
		typedef enum
		{
			E_SDP_B_NONE = 0,
			E_SDP_B_MODIFIER,
			E_SDP_B_MODIFIER_SP,
			E_SDP_B_BANDWIDTH,
			E_SDP_B_MAX
		}EKSdpParse_t;
		typedef bool (*PFuncParseState)(KSdpB * _pclsObj, const char _cInput);
		KSdpB();
		~KSdpB();
		KSdpB & operator=(KSdpB & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		EKSdpParse_t m_eSt;
		KString m_clsMod;
		KString m_clsBandwidth;
		KString m_clsVal;
	private:
		static bool m_fnE_SDP_B_NONE(KSdpB * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_B_MODIFIER(KSdpB * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_B_MODIFIER_SP(KSdpB * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_B_BANDWIDTH(KSdpB * _pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_SDP_B_MAX];
};
class KSdpBs : public StlObject
{
	public: 
		//b=<modifier>:<bandwidth-value>
		//b=AS:1000
		KSdpBs();
		~KSdpBs();
		KSdpBs & operator=(KSdpBs & _rclsSrc);   //Store 
		KSdpB & operator[](unsigned int _unIndex); // Array
		KSdpBs & operator+=(KSdpB &_rclsSrc);     // Add
		unsigned int m_fnGetNum(){return m_list.m_fnGetSize();}
		bool m_fnParse(const char _cInput,bool _bNew=false);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnAddB(const char * _pszModifier, const char * _pszBandwidth);
		void m_fnEncode(KString &_rclsEncode);
		StlList m_list;
		KSdpB m_clsNull;
		KSdpB * m_pclsCurrent;
};
/**************************************** Z *******************************************/
class KSdpZ : public StlObject
{
	public: 
		// z=<adjustment time> <offset> <adjustment time> <offset> ....
		KSdpZ();
		~KSdpZ();
		KSdpZ & operator=(KSdpZ & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		KString m_clsVal;
};
/*************************************** K ********************************************/
class KSdpK : public StlObject
{
	public: 
		// <type>=<value>
		KSdpK();
		~KSdpK();
		KSdpK & operator=(KSdpK & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		KString m_clsVal;
};
/*************************************** A ********************************************/
class KSdpA : public StlObject
{
	public: 
		//a=<key string>: <val>
		//a=opt1: opt_val_string
		typedef enum
		{
			E_SDP_A_NONE = 0,
			E_SDP_A_KEY,
			E_SDP_A_KEY_SP,
			E_SDP_A_VAL,
			E_SDP_A_EXT_RTPMAP,
			E_SDP_A_EXT_FMTP,
			E_SDP_A_MAX
		}EKSdpParse_t;
		typedef bool (*PFuncParseState)(KSdpA* _pclsObj, const char _cInput);
		KSdpA();
		~KSdpA();
		KSdpA & operator=(KSdpA & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		//==========> Extended Parsing Instance
		KSdpAttrRtpMap * m_fnCreateRtpMap();
		KSdpAttrFmtp * m_fnCreateFmtp();
		EKSdpParse_t m_eSt;
		KString m_clsAttrName;
		KString m_clsAttrVal;  // Other Attribute Value
		KString m_clsVal;       // Parsing Raw Data
		ESdpAttr_t m_eAttrT; // Attr type
		KSdpAttr * m_pclsAttr;  // Attr Object
	private:
		void m_fnClear();
		static bool m_fnE_SDP_A_NONE(KSdpA * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_KEY(KSdpA * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_KEY_SP(KSdpA * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_VAL(KSdpA * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_EXT_RTPMAP(KSdpA * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_A_EXT_FMTP(KSdpA * _pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_SDP_A_MAX];
};
class KSdpAs : public StlObject
{
	public: 
		//t=0 0
		KSdpAs();
		~KSdpAs();
		KSdpAs & operator=(KSdpAs & _rclsSrc);    // Store
		KSdpA & operator[](unsigned int _unIndex); // Array
		KSdpAs & operator+=(KSdpA &_rclsSrc);     // Add
		bool m_fnParse(const char _cInput,bool _bNew=false);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		StlList m_list;
		KSdpA * m_pclsCurrent;
		KSdpA m_clsNull;
};
/*************************************** T ********************************************/
class KSdpT : public StlObject
{
	public: 
		// t=<start-time> <stop-time>
		typedef enum
		{
			E_SDP_T_NONE = 0,
			E_SDP_T_1,
			E_SDP_T_1_SP,
			E_SDP_T_2,
			E_SDP_T_MAX
		}EKSdpParse_t;
		typedef bool (*PFuncParseState)(KSdpT * _pclsObj, const char _cInput);
		KSdpT();
		~KSdpT();
		KSdpT & operator=(KSdpT & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		EKSdpParse_t m_eSt;
		KString m_clsT1;
		KString m_clsT2;
		KString m_clsVal;
	private:
		static bool m_fnE_SDP_T_NONE(KSdpT * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_T_1(KSdpT * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_T_1_SP(KSdpT * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_T_2(KSdpT * _pclsObj,const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_SDP_T_MAX];
};
/*************************************** R ********************************************/
class KSdpR : public StlObject
{
	public: 
		// r=<repeat interval> <active duration> <offsets from start-time>
		KSdpR();
		~KSdpR();
		KSdpR & operator=(KSdpR & _rclsSrc);
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		KString m_clsVal;
};
/******************************** Time Descriptions (T/R)[n] *********************************/
class KSdpTDesc : public StlObject
{
	public:
		KSdpTDesc();
		~KSdpTDesc();
		KSdpTDesc & operator=(KSdpTDesc & _rclsSrc);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth);
		void m_fnEncode(KString &_rclsEncode);
		KSdpT m_clsT;     // mandatory
		KSdpR m_clsR;     // opt
};
class KSdpTDescs
{
	public:
		KSdpTDescs();
		~KSdpTDescs();
		KSdpTDescs & operator=(KSdpTDescs & _rclsSrc);   // Store
		KSdpTDesc & operator[](unsigned int _unIndex);        // Array
		unsigned int m_fnGetNum(){return m_listTs.m_fnGetSize();}
		bool m_fnParseT(const char _cInput,bool _bNew=false);
		bool m_fnParseR(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth);
		void m_fnEncode(KString &_rclsEncode);
		KSdpTDesc * m_fnCreateAdd();
		StlList m_listTs;
		KSdpTDesc * m_pclsCurrent;
		KSdpTDesc m_clsNull;
};
/******************************* Media Fmtp List *****************************************/
class KSdpMFmtItem : public StlObject
{
	public:
		KSdpMFmtItem(){}
		~KSdpMFmtItem(){}
		KSdpMFmtItem & operator=(KSdpMFmtItem & _rclsSrc)
		{
			m_clsVal = _rclsSrc.m_clsVal; return *this;
		}
		KString m_clsVal;
};
/******************************************** M ***************************************/
class KSdpM : public StlObject
{
	public: 
		//m=<media> <port> <transport> <fmt list>
		//m=audio 7010 RTP/AVP 97 98 100 35 99 102
		typedef enum
		{
			E_SDP_M_NONE = 0,
			E_SDP_M_MEDIA,
			E_SDP_M_MEDIA_SP,
			E_SDP_M_PORT,
			E_SDP_M_PORT_SP,
			E_SDP_M_TRANSPORT,
			E_SDP_M_TRANSPORT_SP,
			E_SDP_M_FMT,
			E_SDP_M_FMT_SP,
			E_SDP_M_MAX
		}EKSdpParse_t;
		typedef bool (*PFuncParseState)(KSdpM * _pclsObj, const char _cInput);
		KSdpM();
		~KSdpM();
		KSdpM & operator=(KSdpM & _rclsSrc);                  // Store
		KString & operator[](unsigned int _unIndex);            // Codec Array
		KSdpM & operator+=(unsigned int _unCodec);         // Add Codec
		unsigned int m_fnGetCodecNum(){return m_listFmt.m_fnGetSize();}
		bool m_fnParse(const char _cInput);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		/*
		      Media description, if present
		         m=  (media name and transport address)
		         i=* (media title)
		         c=* (connection information -- optional if included at
		              session level)
		         b=* (zero or more bandwidth information lines)
		         k=* (encryption key)
		         a=* (zero or more media attribute lines)
		*/
		// headers
		EKSdpParse_t m_eSt;
		KSdpI m_clsI;      // opt
		KSdpC m_clsC;     // opt
		KSdpBs m_clsBs;   // opt
		KSdpK m_clsK;      // opt
		KSdpAs m_clsAs;   // opt
		// m line params
		KString m_clsMedia;
		KString m_clsPort;
		KString m_clsTransport;
		StlList m_listFmt;
		KString m_clsVal;
		KSdpMFmtItem * m_pclsCurrentFmtp;
		KSdpMFmtItem m_clsNull;
	private:
		static bool m_fnE_SDP_M_NONE(KSdpM * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_M_MEDIA(KSdpM * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_M_MEDIA_SP(KSdpM * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_M_PORT(KSdpM * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_M_PORT_SP(KSdpM * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_M_TRANSPORT(KSdpM * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_M_TRANSPORT_SP(KSdpM * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_M_FMT(KSdpM * _pclsObj,const char _cInput);
		static bool m_fnE_SDP_M_FMT_SP(KSdpM * _pclsObj,const char _cInput);		
		static PFuncParseState m_pfnParseHandle[E_SDP_M_MAX];
};
class KSdpMs : public StlObject
{
	public: 
		KSdpMs();
		~KSdpMs();
		KSdpMs & operator=(KSdpMs & _rclsSrc);    // Store
		KSdpM & operator[](unsigned int _unIndex); // Array
		unsigned int m_fnGetNum(){return m_list.m_fnGetSize();}
		bool m_fnParse(const char _cInput,bool _bNew=false);
		void m_fnDebug(KString &_rclsDebug,unsigned int _unDepth=0);
		void m_fnEncode(KString &_rclsEncode);
		KSdpM * m_fnAddM();
		KString m_clsVal;
		StlList m_list;
		KSdpM *m_pclsCurrent;
		KSdpM m_clsNull;
};
/************************ SIP Parse ******************************************************/
class KSdpParse
{
	public:
		typedef enum
		{
			E_SDP_NONE = 0,
			E_SDP_CR,
			E_SDP_LF,
			E_SDP_NAME,
			E_SDP_NAME_SP,
			E_SDP_V,
			E_SDP_O,
			E_SDP_S,
			E_SDP_I,
			E_SDP_U,
			E_SDP_E,
			E_SDP_P,
			E_SDP_C,
			E_SDP_B,
			E_SDP_Z,
			E_SDP_K,
			E_SDP_A,
			E_SDP_T,
			E_SDP_R,
			E_SDP_M,
			E_SDP_MAX
		}EKSdpParse_t;
		typedef bool (*PFuncParseState)(KSdpParse * _pclsObj, const char _cInput);
		KSdpParse();
		~KSdpParse();
		KSdpParse & operator=(KSdpParse & _rclsSrc);		
		bool m_fnParse(const char * _pszMsg, unsigned int _unLen);
		void m_fnDebug(KString &_rclsDebug);
		void m_fnEncode(KString &_rclsEncode);
		// Parsing Temp Data
		EKSdpParse_t m_eSt;
		KString m_clsTmpHeaderName;
		/*
		      Session description
		      {
		         v=  (protocol version)
		         o=  (originator and session identifier)
		         s=  (session name)
		         i=* (session information)
		         u=* (URI of description)
		         e=* (email address)
		         p=* (phone number)
		         c=* (connection information -- not required if included in all media)
		         b=* (zero or more bandwidth information lines)
		         One or more time descriptions ("t=" and "r=" lines; see below)
		         z=* (time zone adjustments)
		         k=* (encryption key)
		         a=* (zero or more session attribute lines)
		         Zero or more media descriptions
			}
		      Time description
		      {
		         t=  (time the session is active)
		         r=* (zero or more repeat times)
			}
		      Media description, if present
		      {
		         m=  (media name and transport address)
		         i=* (media title)
		         c=* (connection information -- optional if included at session level)
		         b=* (zero or more bandwidth information lines)
		         k=* (encryption key)
		         a=* (zero or more media attribute lines)
		       }
		*/
		//==>session description
		KSdpV m_clsV;          // mandatory
		KSdpO m_clsO;         // mandatory
		KSdpS m_clsS;         // mandatory
		KSdpI m_clsI;           // opt
		KSdpU m_clsU;         // opt
		KSdpE m_clsE;          // opt
		KSdpP m_clsP;          // opt
		KSdpC m_clsC;          // opt
		KSdpBs m_clsBs;        // opt
		KSdpTDescs m_clsTs; // mandatory, time descriptions
		KSdpZ m_clsZ;          // opt
		KSdpK m_clsK;          // opt
		KSdpAs m_clsAs;       // opt
		KSdpMs m_clsMs;      // opt , media descriptions
	private:
		static bool m_fnE_SDP_NONE(KSdpParse *_pclsObj, const char _cInput);
		static bool m_fnE_SDP_CR(KSdpParse *_pclsObj, const char _cInput);
		static bool m_fnE_SDP_LF(KSdpParse *_pclsObj, const char _cInput);
		static bool m_fnE_SDP_NAME(KSdpParse *_pclsObj, const char _cInput);
		static bool m_fnE_SDP_NAME_SP(KSdpParse *_pclsObj, const char _cInput);
		static bool m_fnE_SDP_V(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_O(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_S(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_I(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_U(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_E(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_P(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_C(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_B(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_Z(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_K(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_A(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_T(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_R(KSdpParse * _pclsObj, const char _cInput);
		static bool m_fnE_SDP_M(KSdpParse * _pclsObj, const char _cInput);
		static PFuncParseState m_pfnParseHandle[E_SDP_MAX];
};
}
#endif

