#ifndef FAILCHARGING_H
#define FAILCHARGING_H
#include "eRule/CRUFile.h"
#include "pde_msg.h"
#include "CPdeTcpServer.h"
#include "eRule/CRUTime.h"
#include "rutil/TimeLimitFifo.hxx"
#include "IBCDefine.h"
typedef struct ChargingFailData_t
{
	ChargingFailData_t(){m_uiPdeID=0;memset(m_szFileName,0x00,256);m_unEvTime=0;}
	PDEMsg_t m_stRecord;
	unsigned int m_uiPdeID;
	char m_szFileName[256];
	unsigned int m_unEvTime;    // time_t
}ChargingFailData_t;
class FailChargingQ;
class FailCharging;
class FailChargingQ : public resip::ThreadIf
{
	public:
		FailChargingQ();
		~FailChargingQ();
		bool fnPut(ChargingFailData_t *_pstNew);
		void fnStart(FailCharging * _pclsOwner);
		void fnStop();
		void fnSetMaxFifo(unsigned int _unMaxQNum){mFifo.setMaxFifoSize(_unMaxQNum);}
		unsigned int fnGetMaxFifo(){return mFifo.getMaxFifoSize();}
		unsigned int fnGetCurrentFifo(){return mFifo.getCurrentFifoNum();}
		bool fnIsCongestion(){return mFifo.checkCongestion(NULL,NULL);}
	private:
		void thread();
		bool m_bFinished;
		resip::TimeLimitFifo<ChargingFailData_t> mFifo;
		FailCharging * m_pclsOwner;
};
class FailCharging
{
	public:
		FailCharging();
		~FailCharging();
		static FailCharging * fnGetInstance(unsigned int _unMaxQ,const char * _pszPath=NULL);
		static void fnFinalize();
		void fnSetPath(const char * _pszPath);
		bool fnWriteFail(const char * _pszFileName,const void * _pszBuf);
		void fnWrite(ChargingFailData_t * _pstData);
		unsigned int fnGetCurrentQ(){return m_clsQ.fnGetCurrentFifo();}
	private:
		void fnUpdateDirPerMin();
		void fnSetYears(unsigned int _unY);
		void fnSetMon(unsigned int _unM);
		void fnSetDay(unsigned int _unDay);
		void fnSetHour(unsigned int _unHours);
		void fnSetMins(unsigned int _unMins);
		void fnSetFileName(const char * _pszFileName);
		RUString m_clsPath;
		char m_szFullPath[4096];
		// 2 0 1 5 0 3 2 9 2 3  /  0   0    /   filename.ext    ==> years month days hours / mins / filename
		// 0 1 2 3 4 5 6 7 8 9 10 11 12  13  14 
		char * m_pszTimeBase;  // &m_szFullPath[DirLen]
		char * m_pszYr;             // 4 Digits    & m_pszTimeBase[0]
		char * m_pszMon;          // 2 Digits    & m_pszTimeBase[4]
		char * m_pszDay;           // 2 Digits   & m_pszTimeBase[6]
		char * m_pszHours;        // 2 Digits   & m_pszTimeBase[8]
		char * m_pszMins;         // 2 Digits   & m_pszTimeBase[11]   (10 is slash)
		char * m_pszFileName;   // file string & m_pszTimeBase[14]   (13 is slash)
		RUDailyTime m_clsDailyTime;
		resip::RWMutex m_Mutex;
		static FailCharging * m_pclsFailCharging;
		FailChargingQ m_clsQ;
		unsigned int m_unQNum;
};

#endif

