
#ifndef COM_AS_CDR_H
#define COM_AS_CDR_H

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "eSipUtil/queueThread.hxx"
#include "eSipUtil/define.hxx"

#include "eDum/eDumLibSession.hxx"
#include "eSipFilter.hxx"

namespace ComAs
{

typedef enum
{
	E_COM_AS_START_HOST_none =0,
	E_COM_AS_START_HOST_sipNode,
	E_COM_AS_START_HOST_extAs,

	E_COM_AS_START_HOST_NUM,
}ECdrStartHost;

typedef enum
{
	E_COM_AS_END_HOST_none =0,
	E_COM_AS_END_HOST_sipNode,
	E_COM_AS_END_HOST_comAs,
	E_COM_AS_END_HOST_extAs,

	E_COM_AS_END_HOST_NUM,
}ECdrEndHost;

const char * g_fnGetCdrCallStartHostStr(int _n);
const char * g_fnGetCdrCallEndHostStr(int _n);

struct CdrWriteData
{
	std::string * m_pclsCdrStr;
	time_t m_ulTime;

	CdrWriteData()
	{
		m_pclsCdrStr = NULL;
		m_ulTime = 0;
	}

	~CdrWriteData()
	{
		delete m_pclsCdrStr;
	}
};

class ComAsCdrMgr
{
	public :
		static ComAsCdrMgr & getInstance();
		~ComAsCdrMgr();

		bool m_fnInit();
		bool m_fnAddOverloadCdr(const eSIP::OverloadCdr_t & _rstOverloadCdrData);
		bool m_fnAddCdr(const eDum::Session & _rclsSession);
		
		static void m_fnCbProcOverloadCdr(const eSIP::OverloadCdr_t & _rstOverloadCdr);
		
	private :
		ComAsCdrMgr();

		static void m_fnCbProcessMiddle4OverloadCdr(eSIP::OverloadCdr_t * _pstOverloadCdrData, void * _pObject);
		static void m_fnCbProcessCdr(CdrWriteData * _pstCdrWriteData, void * _pObject);
		
		std::string * m_fnBuildOverloadCdr(const eSIP::OverloadCdr_t & _rstOverloadCdrData);
		std::string * m_fnBuildCdr(const eDum::Session & _rclsSession);		
		
		eSipUtil::QueueThread< CdrWriteData > m_clsCdrThread;
		eSipUtil::QueueThread< eSIP::OverloadCdr_t > m_clsOverloadCdrThread;
		
		int m_nYear;
		int m_nMonth;
		int m_nDay;
		int m_nHour;
		int m_nMinute;
		std::ofstream * m_pclsStm;
		
};

}

#endif

