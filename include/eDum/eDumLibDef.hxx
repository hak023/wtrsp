
#ifndef DUMLIB_DEF_HXX
#define DUMLIB_DEF_HXX

#include "rutil/Data.hxx"
#include "rutil/TransportType.hxx"

#include "eSipUtil/string.hxx"

namespace eDum
{

enum
{
	E_CONST_DFT_SIP_MAXFORWARDS = 80,
};

typedef enum
{
	E_DIALOG_METHOD_TYPE_INV = 0, //INVITE
	E_DIALOG_METHOD_TYPE_REGI, //REGISTER
	E_DIALOG_METHOD_TYPE_SUB, //SUBSCRIBE	
	E_DIALOG_METHOD_TYPE_STANDALONE, //StandAlone
	//E_DIALOG_METHOD_TYPE_REFER,

	E_DIALOG_METHOD_TYPE_NUM,
}EDialogMethodType_t;

struct NetTuple
{
	enum
	{
		E_CONST_PRINT_LEN = 100,
	};

	resip::Data m_clsIpAddr;
	int m_nPort;
	resip::TransportType m_eTransportType;

	NetTuple()
	{
		reset();
	}

	void reset()
	{
		m_clsIpAddr = "";
		m_nPort = -1;
		m_eTransportType = resip::UNKNOWN_TRANSPORT;
	}
	
	const char * str(char * _pszPrint, unsigned int _unLen) const
	{
		eSipUtil::strncat(_pszPrint, _unLen, 
												"IP[%s], Port[%d], Transport[%s]",
												m_clsIpAddr.c_str(), m_nPort, resip::toData(m_eTransportType).c_str());

		return _pszPrint;
	}
	
};
typedef struct NetTuple NetTuple_t;

const char * getStatusLineDesc(int _nRspCode);

}

#endif
