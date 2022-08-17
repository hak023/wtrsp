#ifndef _IBC_MON_LOG_H_
#define _IBC_MON_LOG_H_
#include "eRule/CRuleUtiles.h"
#include "eRule/CDeleteFile.h"
#include "eRule/CRUFile.h"
#include "eRule/CRUTime.h"
#include "IBCDefine.h"
class IbcMonitorLog
{
	public:
		IbcMonitorLog();
		~IbcMonitorLog();
		static IbcMonitorLog * fnGetInstance();
		static void fnFinalize();
		void fnInitFile(const char * _pszPath);
		int fnWriteLog(const char *pFormat, ...);
		static IbcMonitorLog * m_pclsMon;
	private:
		void fnUpdateFile();
		RULock clsFileLock;
		RUFile m_File;
		RUDailyTime m_clsCurrentT;
		RUString m_clsBasePath;
};
#endif

