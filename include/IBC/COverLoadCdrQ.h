#ifndef _OVERLOAD_CDR_Q_H_
#define _OVERLOAD_CDR_Q_H_
#include "eRule/CRuleUtiles.h"
#include "eRule/CDeleteFile.h"
#include "eRule/CRUFile.h"
#include "eRule/CRUTime.h"
#include "eRule/CStatLog.h"
#include "rutil/TimeLimitFifo.hxx"
#include "IBCDefine.h"
class OverLoadCdrQ : public resip::ThreadIf
{
	public:
		OverLoadCdrQ();
		~OverLoadCdrQ();
		static OverLoadCdrQ * fnGetInstance(RUStatLog * _pclsOwner, unsigned int _unMaxQ);
		static OverLoadCdrQ * fnGetInstance();
		static void fnFinalize();
		bool fnPut(StatLogQ_t *_pstNew);
		void fnStart(RUStatLog * _pclsOwner);
		void fnStop();
		void fnSetMaxFifo(unsigned int _unMaxQNum){mFifo.setMaxFifoSize(_unMaxQNum);}
		unsigned int fnGetMaxFifo(){return mFifo.getMaxFifoSize();}
		unsigned int fnGetCurrentFifo(){return mFifo.getCurrentFifoNum();}
		bool fnIsCongestion(){return mFifo.checkCongestion(NULL,NULL);}
		static OverLoadCdrQ * m_pclsMy;
		RUStatLog *m_pclsOwner;
	private:
		int fnOverloadQueuing(StatLogQ_t * _pstLog);
		void thread();
		bool m_bFinished;
		resip::TimeLimitFifo<StatLogQ_t> mFifo;
};
#endif

