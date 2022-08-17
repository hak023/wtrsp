
#ifndef ESIPUTIL_COUNT_EVENT_MON_H
#define ESIPUTIL_COUNT_EVENT_MON_H

#include <time.h>

#include "HangDefine.h"
#include "queueThread.hxx"
#include "timerTable.hxx"
#include "lock.hxx"

namespace eSipUtil
{

struct CountEventTimerData;

typedef enum
{
	E_COUNT_ALARM_LEVEL_idle = 0,
	E_COUNT_ALARM_LEVEL_minor,
	E_COUNT_ALARM_LEVEL_major,
	E_COUNT_ALARM_LEVEL_crit,
}ECountAlarmLevel;

typedef void (* PfuncAlarmEvent)(int _nItemId, bool _bClear, ECountAlarmLevel _eCntAlarmLevel, unsigned int _unCurrentCnt, unsigned int _unMaxCnt, unsigned int _unDropCnt, void * _pUsrData);

class CountEventMon
{
	public :
		enum
		{
			E_CONST_MAX_ITEM_NUMS = DEF_MAX_MONITOR,
		};

		struct ItemInfo
		{
			unsigned int m_unMaxCnt;
			unsigned int m_unCritRate;
			unsigned int m_unMajorRate;
			unsigned int m_unMinorRate;
			unsigned int m_unSndAlarmIntervalSec;
			unsigned int m_unAlarmClearTimerSec;
			PfuncAlarmEvent m_pfnCbAlarmEvent;
			void * m_pUsrData;

			//only queue-thread;
			char m_szQueueThreadName[32];
			unsigned int m_unThreadId;

			void m_fnReset()
			{
				m_unMaxCnt = 0;
				m_unCritRate = 0;
				m_unMajorRate = 0;
				m_unMinorRate = 0;
				m_unSndAlarmIntervalSec = 0;
				m_unAlarmClearTimerSec = 0;
				m_pfnCbAlarmEvent = NULL;
				m_pUsrData = NULL;

				m_szQueueThreadName[0] = 0x00;
				m_unThreadId = 0;
			}

			bool m_fnValid() const
			{
				if( m_unMaxCnt == 0 
					|| m_unCritRate == 0 
					|| m_unMajorRate == 0 
					|| m_unMinorRate == 0
					|| m_unSndAlarmIntervalSec == 0
					|| m_unAlarmClearTimerSec == 0
					|| !m_pfnCbAlarmEvent )
				{
					return false;
				}

				if( m_unCritRate < m_unMajorRate )
					return false;

				if( m_unMajorRate < m_unMinorRate )
					return false;

				return true;
			}
			
			ItemInfo()
			{
				m_fnReset();
			}
		};
		
		CountEventMon();
		~CountEventMon();
		
		bool m_fnStart(const char * _pszQueueShmProcName=NULL,int _nQueueShmKey=-1);
		//_nItemId must be 0 ~ E_CONST_MAX_ITEM_NUMS-1
		bool m_fnAddItem(int _nItemId, const CountEventMon::ItemInfo & _rstItemInfo);
		bool m_fnModItem(int _nItemId, const CountEventMon::ItemInfo & _rstItemInfo);
		void m_fnTouchItem(int _nItemId, unsigned int _unCurrentCnt);
		void m_fnDrop(int _nItemId);
		void m_fnDrop(int _nItemId, unsigned int _unCurrentCnt);
		const QueueMonitorInfo_t * m_fnGetShm() const;

	private :
		struct ItemInternalInfo
		{
			bool m_bUse;
			
			unsigned int m_unCurrentCnt;
			ECountAlarmLevel m_ePeakLevel;
			unsigned int m_unDropCnt;
			time_t m_ulStartEventTime;
			time_t m_ulLastEventTime;
			bool m_bClearing;
			
			ItemInfo m_stItemInfo;
			Mutex m_clsMutex;

			ItemInternalInfo()
			{
				m_bUse = false;
				
				m_unCurrentCnt = 0;
				m_ePeakLevel = E_COUNT_ALARM_LEVEL_idle;
				m_unDropCnt = 0;
				m_ulStartEventTime = 0;
				m_ulLastEventTime = 0;
				m_bClearing = false;
				
				m_stItemInfo.m_fnReset();
			}
		};

		static void m_fnCbTimer(CountEventTimerData * _pstTimerData);
		static void m_fnCbProc(CountEventTimerData * _pstTimerData, void * _pObject);
		
		ECountAlarmLevel m_fnUpdateLevel(int _nItemId);
		bool m_fnValidTimer(const CountEventTimerData & _rclsTimerData);

		QueueThread< CountEventTimerData > m_clsProcThread;
		TimerTable< CountEventTimerData > m_clsTimerTable;
		ItemInternalInfo m_arrInternalItem[E_CONST_MAX_ITEM_NUMS];

		//only queue-thread
		QueueMonitorInfo_t * m_pstQueueShm;
		Mutex m_clsQueueShmCntLock;

};

}

#endif

