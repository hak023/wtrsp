
#ifndef TRSG_CDR_MGR_H
#define TRSG_CDR_MGR_H

#include <time.h>
#include <iostream>
#include <fstream>

#include "queueThread.hxx"
#include "define.hxx"
#include "string.hxx"

using namespace eSipUtil;

class TrsgCdrData
{
	public:
		TrsgCdrData(){}
		~TrsgCdrData(){}
		KString m_clsBuff;	
};
class TrsgCdrMgr
{
	public :
	TrsgCdrMgr();
		~TrsgCdrMgr();
		void m_fnInit(const char * _pszName, const char * _pszPath, const unsigned int _unSysIndex, const int _nPeriod);
		bool m_fnAddCdr(TrsgCdrData * _pstData);
	private :
		static void m_fnCbProcessCdr(TrsgCdrData * _pstTrsgData, void * _pObject);
		void m_fnWriteCdr(std::ofstream & m_rclsStm, TrsgCdrData & _rstData);
		QueueThread<TrsgCdrData> * m_pclsCdrThread;
		std::ofstream * m_pclsStm;
		KString m_clsName;
		unsigned int m_unSysIndex;
		KString m_clsPath;
		int m_nYear;
		int m_nMonth;
		int m_nDay;
		int m_nHour;
		int m_nMinute;
		int m_nPeriod;
		int m_nFileMin[60];
		bool m_bInit;
};

#endif


