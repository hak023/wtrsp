
#ifndef ESIP_CORE_CDR_HXX
#define ESIP_CORE_CDR_HXX

#include <string.h>
#include <sys/time.h>

#include "resip/stack/MethodTypes.hxx"

namespace eDum
{

struct CdrMethodInfo
{
	resip::MethodTypes m_eMethodType;
	const char * m_pszMethodName;
};
typedef struct CdrMethodInfo CdrMethodInfo_t;

struct CdrData
{
	enum
	{
		E_CONST_MAX_CDR_DATA_STR_LEN = 100,
	};
	
	unsigned char m_ucMethodType;
	unsigned short m_usRspCode;
	bool m_bRcv;
	char m_cCallDir;		//0:none, 1:caller, 2:callee
	struct timeval m_tvTime;

	CdrData()
	{
		reset();
	}

	void reset()
	{
		m_ucMethodType = 0;
		m_usRspCode = 0;
		m_bRcv = true;
		m_cCallDir = 0;
		memset(&m_tvTime, 0x00, sizeof(struct timeval));
	}

	bool isRecordCdr() const
	{
		if( m_ucMethodType != 0 && m_tvTime.tv_sec != 0 )
			return true;
		else
			return false;
	}

	void encode(char * _pszPrint, unsigned int _unLen) const;
};
typedef struct CdrData CdrData_t;

}

#endif

