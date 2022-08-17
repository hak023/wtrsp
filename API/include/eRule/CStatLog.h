#ifndef CSTATLOG_H
#define CSTATLOG_H
#include "CRuleUtiles.h"
#include "CDeleteFile.h"
#include "CRUFile.h"
#include "CRUTime.h"
#include "rutil/TimeLimitFifo.hxx"

typedef struct StatLogQ_t
{
	StatLogQ_t(){m_bIsShutDown=false;m_clsEndTime.fnGetAllTime();}
	bool m_bIsShutDown;
	RUDailyTime m_clsEndTime;
	RUString m_clsString;
}StatLogQ_t;
class RUStatLog;
class RUStatLogQ : public resip::ThreadIf
{
	public:
		RUStatLogQ();
		~RUStatLogQ();
		bool fnPut(StatLogQ_t *_pstNew);
		void fnStart(RUStatLog * _pclsOwner);
		void fnStop();
		void fnSetMaxFifo(unsigned int _unMaxQNum){mFifo.setMaxFifoSize(_unMaxQNum);}
		unsigned int fnGetMaxFifo(){return mFifo.getMaxFifoSize();}
		unsigned int fnGetCurrentFifo(){return mFifo.getCurrentFifoNum();}
		bool fnIsCongestion(){return mFifo.checkCongestion(NULL,NULL);}
	private:
		void thread();
		bool m_bFinished;
		resip::TimeLimitFifo<StatLogQ_t> mFifo;
		RUStatLog *m_pclsOwner;
};
class RUStatLog
{
	public: 
		typedef void (*PfuncPeriodicEvent_t)(RUStatLog * _pclsLog);
		RUStatLog();
		~RUStatLog();
		static RUStatLog * fnGetInstance(const char *_pszStatPath,
												const char *_pszFileName, 
												unsigned int _unLimitMins,
												unsigned int _unMaxQ=1000);
		void fnInitFile();
		int fnWriteLog(const char *pFormat, ...);
		int fnWriteQueuing(StatLogQ_t * _pstLog);
		int fnWriteLogEndTime(StatLogQ_t * _pstLog);   // for Queueing WriteLog
		void fnSetLogPerMins(unsigned int _unLogPerMins)
			{m_unLimiteMins = (unsigned int)_unLogPerMins;}
		void fnSetMaxQNum(unsigned int _unMaxQ)
		{
			m_clsQ.fnSetMaxFifo(_unMaxQ);
		}
		unsigned int fnGetCurrentQ(){return m_clsQ.fnGetCurrentFifo();}
		void fnFileChk();    // must function-call periodically ( example : 1000 ms)
		PfuncPeriodicEvent_t m_pfnPeriodicEvent;
	private:
		void fnUpdate();	
		void fnUpdateCurrentFile(RUDailyTime & _rclsT);
		RULock stat_lock;
		RUFile m_File;
		char m_szFileName[128];
		char m_szBasePath[128];
		unsigned int m_unLimiteMins;
		unsigned int m_unMaxDays;
		RUDailyTime m_clsEvTime;
		bool m_bOnOff;
		RUStatLogQ m_clsQ;
};
#endif

