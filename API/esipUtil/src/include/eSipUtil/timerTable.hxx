
#ifndef ESIPUTIL_TIMER_TABLE_H
#define ESIPUTIL_TIMER_TABLE_H

#include <time.h>
#include <vector>
#include <tr1/unordered_map>

#include "timeUtil.hxx"
#include "thread.hxx"
#include "lock.hxx"
#include "TimeWheel.hxx"

namespace eSipUtil
{

template <class T>
class TimerTable
{
	public :
		typedef void (* PfuncTimeOutCb)(T *);		//callback is executed when timer is expired

		TimerTable(PfuncTimeOutCb _pfnTimeOutCb, const char * _pszName = NULL,int _nCoreID=-1)
			: m_clsTimerThread(_pszName), m_pfnTimeOutCb(_pfnTimeOutCb), m_lProcessSec(0), m_unTableIndex(0)
		{
			m_clsTimerThread.setProcessCb(process);
			m_clsTimerThread.setObject(this);
			start(_nCoreID);
		}
		
		~TimerTable()
		{
			stop();
		}

		bool setTimer(T * pTimerData, unsigned long _ulAfterMilliSec)
		{
			if( !pTimerData )
				return false;

			if( _ulAfterMilliSec < E_CONST_ACCEPT_MS )
				_ulAfterMilliSec = E_CONST_ACCEPT_MS;

			struct timespec stCurrentTs;
			clock_gettime(CLOCK_REALTIME, &stCurrentTs);
			time_t nExpireSecond = stCurrentTs.tv_sec;
			unsigned long ulExpireMilliSecond = stCurrentTs.tv_nsec/1000000 + _ulAfterMilliSec;
			nExpireSecond += ulExpireMilliSecond / 1000;
			ulExpireMilliSecond = ulExpireMilliSecond % 1000;
			unsigned int unTableIndex = ulExpireMilliSecond / E_CONST_PROCESS_UNIT;
			unsigned int unArrayIndex = ulExpireMilliSecond % E_CONST_PROCESS_UNIT;
			Vec_MilliSecTimer * parrTimerVec = NULL;

			m_arrMapLock[unTableIndex].m_fnLock();			
			typename Map_SecTimer::iterator mapItr = m_arrSecTimerInfoMap[unTableIndex].find(nExpireSecond);
			if( mapItr == m_arrSecTimerInfoMap[unTableIndex].end() )
			{
				parrTimerVec = new Vec_MilliSecTimer[E_CONST_PROCESS_UNIT];
				if(parrTimerVec)
					m_arrSecTimerInfoMap[unTableIndex][nExpireSecond] = parrTimerVec;
			}
			else
				parrTimerVec= mapItr->second;			
			m_arrMapLock[unTableIndex].m_fnUnlock();

			m_arrVecLock[unTableIndex][unArrayIndex].m_fnLock();
			if( parrTimerVec )
			{
				parrTimerVec[unArrayIndex].push_back(pTimerData);
			}
			m_arrVecLock[unTableIndex][unArrayIndex].m_fnUnlock();

			return true;
		}

		/*==== set timeout of specific time ====*/
		bool setTimer(T * pTimerData, struct timespec _stExpireTime, unsigned long _ulMinimumTimeout = 100)
		{
			if( !pTimerData )
				return false;

			struct timespec stCurrentTs, stDiffTs;
			unsigned long ulAfterMilliSec = 0;
			clock_gettime(CLOCK_REALTIME, &stCurrentTs);
			if( _ulMinimumTimeout == 0 )
				_ulMinimumTimeout = E_CONST_ACCEPT_MS;
			
			stDiffTs = subTimespec(_stExpireTime, stCurrentTs);
			if( stDiffTs.tv_sec == 0 && stDiffTs.tv_nsec/1000000 < (long)_ulMinimumTimeout )
			{
				_stExpireTime.tv_sec = stCurrentTs.tv_sec;
				_stExpireTime.tv_nsec = stCurrentTs.tv_nsec;
				ulAfterMilliSec = _ulMinimumTimeout;
			}	

			time_t nExpireSecond = _stExpireTime.tv_sec;
			unsigned long ulExpireMilliSecond = _stExpireTime.tv_nsec/1000000 + ulAfterMilliSec;
			nExpireSecond += ulExpireMilliSecond / 1000;
			ulExpireMilliSecond = ulExpireMilliSecond % 1000;
			unsigned int unTableIndex = ulExpireMilliSecond / E_CONST_PROCESS_UNIT;
			unsigned int unArrayIndex = ulExpireMilliSecond % E_CONST_PROCESS_UNIT;
			Vec_MilliSecTimer * parrTimerVec = NULL;

			m_arrMapLock[unTableIndex].m_fnLock();
			typename Map_SecTimer::iterator mapItr = m_arrSecTimerInfoMap[unTableIndex].find(nExpireSecond);
			if( mapItr == m_arrSecTimerInfoMap[unTableIndex].end() )
			{
				parrTimerVec = new Vec_MilliSecTimer[E_CONST_PROCESS_UNIT];
				if(parrTimerVec)
					m_arrSecTimerInfoMap[unTableIndex][nExpireSecond] = parrTimerVec;
			}
			else
				parrTimerVec= mapItr->second;
			m_arrMapLock[unTableIndex].m_fnUnlock();

			m_arrVecLock[unTableIndex][unArrayIndex].m_fnLock();
			if( parrTimerVec )
			{
				parrTimerVec[unArrayIndex].push_back(pTimerData);
			}
			m_arrVecLock[unTableIndex][unArrayIndex].m_fnUnlock();

			return true;
		}

		void start(int _nCoreID = -1)
		{
			struct timespec stCurrentTs;
			clock_gettime(CLOCK_REALTIME, &stCurrentTs);
			int nCurrentMilliSec = stCurrentTs.tv_nsec/1000000;
			m_lProcessSec = stCurrentTs.tv_sec;
			m_unTableIndex = nCurrentMilliSec / E_CONST_PROCESS_UNIT;
			if(_nCoreID == -1)
				m_clsTimerThread.run();
			else m_clsTimerThread.run_(_nCoreID,-1,false); 
		}

		void stop()
		{
			m_clsTimerThread.shutdown();
			m_clsTimerThread.join();
			clearAll();
		}
		
		void reset()
		{
			stop();
			start();
		}

	private :
		enum
		{
			E_CONST_PROCESS_UNIT = 2,
			E_CONST_ACCEPT_MS = E_CONST_PROCESS_UNIT*10,
			E_CONST_HASH_TABLE_SIZE = 1000/E_CONST_PROCESS_UNIT, //remain must be 0
		};

		typedef std::vector<T *> Vec_MilliSecTimer;		
		typedef std::tr1::unordered_map<time_t, Vec_MilliSecTimer *> Map_SecTimer;

		static void process(TimerTable * _pObject)
		{
			struct timespec stCurrentTs, stProcessTs, stSleepTs;
			typename Map_SecTimer::iterator mapItr;
			typename Vec_MilliSecTimer::iterator vecItr;
			Vec_MilliSecTimer * parrTimerVec = NULL;

			_pObject->m_arrMapLock[_pObject->m_unTableIndex].m_fnLock();
			mapItr = _pObject->m_arrSecTimerInfoMap[_pObject->m_unTableIndex].find(_pObject->m_lProcessSec);
			if( mapItr != _pObject->m_arrSecTimerInfoMap[_pObject->m_unTableIndex].end() )
			{
				parrTimerVec = mapItr->second;
				_pObject->m_arrSecTimerInfoMap[_pObject->m_unTableIndex].erase(mapItr);
			}
			_pObject->m_arrMapLock[_pObject->m_unTableIndex].m_fnUnlock();

			if( parrTimerVec )
			{
				for( int i = 0; i < E_CONST_PROCESS_UNIT; ++i )
				{
					//m_arrVecLock[m_unTableIndex][i].m_fnLock();
					for( vecItr = parrTimerVec[i].begin(); vecItr != parrTimerVec[i].end(); ++vecItr )
					{
						_pObject->m_pfnTimeOutCb(*vecItr);
					}
					//m_arrVecLock[m_unTableIndex][i].m_fnUnlock();
				}
				delete[] parrTimerVec;
			}

			++(_pObject->m_unTableIndex);
			if( _pObject->m_unTableIndex >= E_CONST_HASH_TABLE_SIZE )
			{
				++(_pObject->m_lProcessSec);
				_pObject->m_unTableIndex = 0;
			}

			clock_gettime(CLOCK_REALTIME, &stCurrentTs);
			stProcessTs.tv_sec = _pObject->m_lProcessSec;
			stProcessTs.tv_nsec = _pObject->m_unTableIndex*E_CONST_PROCESS_UNIT*1000000;
			stSleepTs = subTimespec(stProcessTs, stCurrentTs);
			if( stSleepTs.tv_sec != 0 || stSleepTs.tv_nsec > 500000 )
				nanosleep(&stSleepTs, NULL);
		}

		void clearAll()
		{
			m_lProcessSec = 0;
			m_unTableIndex = 0;
		
			typename Vec_MilliSecTimer::iterator vecItr;
			typename Map_SecTimer::iterator mapItr;

			for( int i = 0; i < E_CONST_HASH_TABLE_SIZE; ++i )
			{
				m_arrMapLock[i].m_fnLock();
				for( mapItr = m_arrSecTimerInfoMap[i].begin(); mapItr != m_arrSecTimerInfoMap[i].end(); ++mapItr )
				{
					Vec_MilliSecTimer * parrMilliSecTimerVec = mapItr->second;
					if( parrMilliSecTimerVec )
					{
						for( int j = 0; j < E_CONST_PROCESS_UNIT; ++j )
						{
							m_arrVecLock[i][j].m_fnLock();
							for( vecItr = parrMilliSecTimerVec[j].begin(); vecItr != parrMilliSecTimerVec[j].end(); ++vecItr )
							{
								delete (*vecItr);
							}
							parrMilliSecTimerVec[j].clear();
							m_arrVecLock[i][j].m_fnUnlock();
						}
						delete[] parrMilliSecTimerVec;
					}
				}
				m_arrSecTimerInfoMap[i].clear();
				m_arrMapLock[i].m_fnUnlock();
			}
		}

		NormalThread< TimerTable > m_clsTimerThread;
		PfuncTimeOutCb m_pfnTimeOutCb;

		time_t m_lProcessSec;
		unsigned int m_unTableIndex;
		
		Map_SecTimer m_arrSecTimerInfoMap[E_CONST_HASH_TABLE_SIZE];
		Mutex m_arrMapLock[E_CONST_HASH_TABLE_SIZE];
		Mutex m_arrVecLock[E_CONST_HASH_TABLE_SIZE][E_CONST_PROCESS_UNIT];

};
}

#endif

