#ifndef _IBC_DEL_LOG_H
#define _IBC_DEL_LOG_H
#include "eRule/CRuleUtiles.h"
#include "rutil/TimeLimitFifo.hxx"
#include "eRule/CDeleteFile.h"
#include "eRule/CRUFile.h"
#include "eRule/CRUTime.h"
#include "IBCDefine.h"
typedef enum
{
	E_IBC_DELLOG_STACK = 0,
	E_IBC_DELLOG_OPTION,
	E_IBC_DELLOG_CORE,
	E_IBC_DELLOG_CDR,
	E_IBC_DELLOG_DUMP,
	E_IBC_DELLOG_MAX
}EIbcDelLog_t;
inline const char * g_fnStringEIbcDelLog(unsigned int _eT)
{
	switch(_eT)
	{
		case E_IBC_DELLOG_STACK: return "E_IBC_DELLOG_STACK";
		case E_IBC_DELLOG_OPTION: return "E_IBC_DELLOG_OPTION";
		case E_IBC_DELLOG_CORE: return "E_IBC_DELLOG_CORE";
		case E_IBC_DELLOG_CDR: return "E_IBC_DELLOG_CDR";
		case E_IBC_DELLOG_DUMP: return "E_IBC_DELLOG_DUMP";
		case E_IBC_DELLOG_MAX: return "E_IBC_DELLOG_MAX";
		default: return "Unknown";
	}
	return "Unknown";
}
class IbcDelLog : public resip::ThreadIf
{
	public:
		IbcDelLog();
		~IbcDelLog();
		static IbcDelLog * fnGetInstance();
		static void fnFinalize();
		static IbcDelLog * m_pclsMy;
		DeleteFile m_clsDelLog[E_IBC_DELLOG_MAX];
	private:
		void fnStart();
		void fnStop();
		void thread();
		bool m_bFinished;
};
#endif
