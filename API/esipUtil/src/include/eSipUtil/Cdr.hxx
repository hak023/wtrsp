
#ifndef _ESIP_CDR_H
#define _ESIP_CDR_H

#include <time.h>
#include <iostream>
#include <fstream>

#include "queueThread.hxx"
#include "define.hxx"
#include "string.hxx"

namespace eSipUtil
{
class CdrData
{
	public:
		CdrData(){}
		~CdrData(){}
		KString m_clsBuff;	
};
class CdrMgr
{
	public :
		CdrMgr();
		~CdrMgr();
		void m_fnInit(const char * _pszName, const char * _pszPath);
		bool m_fnAddCdr(CdrData * _pstData);
	private :
		static void m_fnCbProcessCdr(CdrData * _pstData, void * _pObject);
		void m_fnWriteCdr(std::ofstream & m_rclsStm, CdrData & _rstData);		
		QueueThread<CdrData> * m_pclsCdrThread;
		std::ofstream * m_pclsStm;
		KString m_clsName;
		KString m_clsPath;
		int m_nYear;
		int m_nMonth;
		int m_nDay;
		int m_nHour;
		int m_nMinute;
		bool m_bInit;
};
}
#endif


