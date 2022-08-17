
#ifndef ESIPUTIL_QUEUE_H
#define ESIPUTIL_QUEUE_H

#include <string>
#include <pthread.h>
#include <time.h>
#include <string.h>

#include "queueMonitor.hxx"
#include "Tbb.h"

namespace eSipUtil
{

#ifdef TBB_USE

struct QueueData4Tbb
{
	QueueData4Tbb()
		: m_QueueData(NULL)
	{}

	QueueData4Tbb(void * _pQueueData)
		: m_QueueData(_pQueueData)
	{}

	~QueueData4Tbb()
	{
	}

	void * m_QueueData;
};

#endif

/*==== operate as FIFO ====*/
template <class T>
class LimitedQueue
{
	public :
		LimitedQueue(unsigned int _unMaxQueueSize = 1000, bool _bMonitor = false, const char * _pszName = NULL, bool _bUseTbb = true);
		virtual ~LimitedQueue();

		void wake();
		void clear();
		bool put(T * _pData);
		T * get(int _nMs = -1);		//_nMs : -1(indefinitely), 0(return right now), 0<(sleep during _nMs. To use for TBB is not recommanded)
		void setMaxQueueSize(unsigned int _unMaxQueueSize);
		unsigned int getMaxQueueSize() const;
		unsigned int getCurrentQueueSize() const;
	
	private :
		/*==== for normal ====*/
		void createQueue4Normal(unsigned int _unMaxQueueSize);
		void deleteQueue4Normal();
		void wake4Normal();
		void clear4Normal();
		bool put4Normal(T * _pData);
		T * get4Normal(int _nMs = -1);		//_nMs : -1(indefinitely), 0(return right now), 0<(sleep during _nMs)
		void setMaxQueueSize4Normal(unsigned int _unMaxQueueSize);
		unsigned int getMaxQueueSize4Normal() const;
		unsigned int getCurrentQueueSize4Normal() const;			

		/*==== for Tbb ====*/		
		void wake4Tbb();
		void clear4Tbb(){}		//not support
		bool put4Tbb(T * _pData);
		T * get4Tbb(int _nMs = -1);		//_nMs : -1(indefinitely), 0(return right now), 0<(sleep during _nMs. can't wake as if normal. only sleep)
		void setMaxQueueSize4Tbb(unsigned int _unMaxQueueSize);
		unsigned int getMaxQueueSize4Tbb() const;
		unsigned int getCurrentQueueSize4Tbb() const;

		std::string m_clsName;
		int m_nQueueMonId;
		bool m_bUseTbb;

		/*==== for normal ====*/
		T ** m_parrQueue;		
		unsigned int m_unMaxQueueSize;
		unsigned int m_unCurrentQueueSize;
		unsigned int m_unPutCurrentPos;
		unsigned int m_unGetCurrentPos;		
		pthread_mutex_t m_QueueMutex;
		pthread_cond_t m_QueueWakeCondition;

		/*==== for Tbb ====*/
#ifdef TBB_USE
		int m_nWaitMs;
		tbb::concurrent_bounded_queue< QueueData4Tbb > m_clsTbbQueue;
#endif
};

template<class T>
LimitedQueue<T>::LimitedQueue(unsigned int _unMaxQueueSize, bool _bMonitor, const char * _pszName, bool _bUseTbb)
	: m_nQueueMonId(-1), m_bUseTbb(_bUseTbb),
	m_parrQueue(NULL), 	m_unMaxQueueSize(0), m_unCurrentQueueSize(0), m_unPutCurrentPos(0), m_unGetCurrentPos(0)
{
	pthread_mutex_init(&m_QueueMutex, NULL);
	pthread_cond_init(&m_QueueWakeCondition, NULL);

#ifdef TBB_USE
	m_nWaitMs = -1;
	if( m_bUseTbb )
	{
		setMaxQueueSize4Tbb(_unMaxQueueSize);
	}
	else
#endif
	{		
		createQueue4Normal(_unMaxQueueSize);
	}

	if( _pszName && strlen(_pszName) > 0 )
		m_clsName = _pszName;

	if( _bMonitor )
		QueueMonMgr::getInstance().m_fnAddQueue(m_clsName.c_str(), getMaxQueueSize(), (unsigned int &)m_nQueueMonId);
}

template<class T> 
LimitedQueue<T>::~LimitedQueue()
{
#ifdef TBB_USE
	if( m_bUseTbb )
	{
		tbb::concurrent_bounded_queue< QueueData4Tbb >::iterator itr;
		for( itr = m_clsTbbQueue.unsafe_begin(); itr != m_clsTbbQueue.unsafe_end(); ++itr )
		{
			delete (T *)(itr->m_QueueData);
		}
	}
	else
#endif
	{
		deleteQueue4Normal();		
	}

	pthread_cond_destroy(&m_QueueWakeCondition);
	pthread_mutex_destroy(&m_QueueMutex);
}

template<class T>
void LimitedQueue<T>::wake()
{
#ifdef TBB_USE
	if( m_bUseTbb )
		wake4Tbb();
	else
#endif
		wake4Normal();
}

template<class T>
void LimitedQueue<T>::clear()
{
#ifdef TBB_USE
	if( m_bUseTbb )
		clear4Tbb();
	else
#endif
		clear4Normal();
}

template<class T>
bool LimitedQueue<T>::put(T * _pData)
{
#ifdef TBB_USE
	if( m_bUseTbb )
		return put4Tbb(_pData);
	else
#endif
		return put4Normal(_pData);
}

template<class T>
T * LimitedQueue<T>::get(int _nMs)
{
#ifdef TBB_USE
	if( m_bUseTbb )
		return get4Tbb(_nMs);
	else
#endif
		return get4Normal(_nMs);
}

template<class T>
void LimitedQueue<T>::setMaxQueueSize(unsigned int _unMaxQueueSize)
{
#ifdef TBB_USE
	if( m_bUseTbb )
		setMaxQueueSize4Tbb(_unMaxQueueSize);
	else
#endif
		setMaxQueueSize4Normal(_unMaxQueueSize);
}

template<class T>
unsigned int LimitedQueue<T>::getMaxQueueSize() const
{
#ifdef TBB_USE
	if( m_bUseTbb )
		return getMaxQueueSize4Tbb();
	else
#endif
		return getMaxQueueSize4Normal();
}

template<class T>
unsigned int LimitedQueue<T>::getCurrentQueueSize() const
{
#ifdef TBB_USE
	if( m_bUseTbb )
		return getCurrentQueueSize4Tbb();
	else
#endif
		return getCurrentQueueSize4Normal();
}

template<class T>
void LimitedQueue<T>::createQueue4Normal(unsigned int _unMaxQueueSize)
{
	if( m_parrQueue )
		return;

	m_unMaxQueueSize = 0;
	m_unCurrentQueueSize = 0;
	m_unPutCurrentPos = 0;
	m_unGetCurrentPos = 0;

	unsigned int unMaxQueueSize = 1000;
	if( _unMaxQueueSize > 0 )
		unMaxQueueSize = _unMaxQueueSize;

	m_parrQueue = new T*[unMaxQueueSize];
	if (!m_parrQueue)
		return;	

	memset(m_parrQueue, 0x00, sizeof(T*)*unMaxQueueSize);
	m_unMaxQueueSize = unMaxQueueSize;
}

template<class T>
void LimitedQueue<T>::deleteQueue4Normal()
{
	if( m_parrQueue )
	{
		for( unsigned int i = 0; i < m_unMaxQueueSize; ++i )
		{
			delete m_parrQueue[i];
		}
		
		delete[] m_parrQueue;
		m_parrQueue = NULL;
	}

	m_unMaxQueueSize = 0;
	m_unCurrentQueueSize = 0;
	m_unPutCurrentPos = 0;
	m_unGetCurrentPos = 0;
}

template<class T>
void LimitedQueue<T>::wake4Normal()
{
	pthread_mutex_lock(&m_QueueMutex);
	pthread_cond_signal(&m_QueueWakeCondition);
	pthread_mutex_unlock(&m_QueueMutex);
}

template<class T>
void LimitedQueue<T>::clear4Normal()
{
	if( !m_parrQueue )
		return;

	pthread_mutex_lock(&m_QueueMutex);

	unsigned int unCurrentPos = m_unGetCurrentPos;
	unsigned int unDelCnt = 0;

	for( unsigned int i = 0; i < m_unMaxQueueSize; ++i )
	{
		if( unDelCnt >= m_unCurrentQueueSize )
			break;
	
		if( unCurrentPos >= m_unMaxQueueSize )
			unCurrentPos = 0;
	
		if( m_parrQueue[unCurrentPos] )
		{
			delete m_parrQueue[unCurrentPos];
			++unDelCnt;
		}
		
		++unCurrentPos;
	}

	memset(m_parrQueue, 0x00, sizeof(T*)*m_unMaxQueueSize);
	m_unCurrentQueueSize = 0;
	m_unPutCurrentPos = 0;
	m_unGetCurrentPos = 0;

	pthread_mutex_unlock(&m_QueueMutex);
}

template<class T>
bool LimitedQueue<T>::put4Normal(T * _pData)
{
	if( !m_parrQueue )
		return false;

	pthread_mutex_lock(&m_QueueMutex);

	if( m_unCurrentQueueSize >= m_unMaxQueueSize )
	{
		pthread_mutex_unlock(&m_QueueMutex);
		return false;
	}

	delete m_parrQueue[m_unPutCurrentPos];
	m_parrQueue[m_unPutCurrentPos] = _pData;
	++m_unCurrentQueueSize;
	++m_unPutCurrentPos;
	if( m_unPutCurrentPos >= m_unMaxQueueSize )
		m_unPutCurrentPos = 0;

	pthread_cond_signal(&m_QueueWakeCondition);	

	pthread_mutex_unlock(&m_QueueMutex);

	if( m_nQueueMonId >= 0 )
		QueueMonMgr::getInstance().m_fnTouchQueue(m_nQueueMonId, m_unCurrentQueueSize);

	return true;
}

template<class T>
T * LimitedQueue<T>::get4Normal(int _nMs)
{
	if( !m_parrQueue )
		return NULL;

	T * pData = NULL;

	pthread_mutex_lock(&m_QueueMutex);

	if( m_unCurrentQueueSize == 0 )
	{
		if( _nMs == 0 )
		{
			pData = NULL;
			goto END_GET;
		}
		else if( _nMs < 0 )
		{
			pthread_cond_wait(&m_QueueWakeCondition, &m_QueueMutex);
		}
		else
		{
			struct timespec ts;
			clock_gettime(CLOCK_REALTIME, &ts);
			ts.tv_sec += _nMs / 1000;
			ts.tv_nsec += (_nMs % 1000)*1000*1000;
			ts.tv_sec += ts.tv_nsec / (1000*1000*1000);
			ts.tv_nsec = ts.tv_nsec % (1000*1000*1000);
			pthread_cond_timedwait(&m_QueueWakeCondition, &m_QueueMutex, &ts);
		}
	}

	if( m_unCurrentQueueSize == 0 )
	{
		pData = NULL;
		goto END_GET;
	}

	pData = m_parrQueue[m_unGetCurrentPos];
	m_parrQueue[m_unGetCurrentPos] = NULL;
	--m_unCurrentQueueSize;
	++m_unGetCurrentPos;
	if( m_unGetCurrentPos >= m_unMaxQueueSize )
		m_unGetCurrentPos = 0;

END_GET:
	
	pthread_mutex_unlock(&m_QueueMutex);

	if( m_nQueueMonId >= 0 )
		QueueMonMgr::getInstance().m_fnTouchQueue(m_nQueueMonId, m_unCurrentQueueSize);

	return pData;
}

template<class T>
void LimitedQueue<T>::setMaxQueueSize4Normal(unsigned int _unMaxQueueSize)
{
	pthread_mutex_lock(&m_QueueMutex);

	deleteQueue4Normal();
	createQueue4Normal(_unMaxQueueSize);

	pthread_mutex_unlock(&m_QueueMutex);

	if( m_nQueueMonId >= 0 )
		QueueMonMgr::getInstance().m_fnSetMaxSize(m_nQueueMonId, _unMaxQueueSize);
}

template<class T>
unsigned int LimitedQueue<T>::getMaxQueueSize4Normal() const
{
	return m_unMaxQueueSize;
}

template<class T> 
unsigned int LimitedQueue<T>::getCurrentQueueSize4Normal() const
{
	return m_unCurrentQueueSize;
}

#ifdef TBB_USE

template<class T> 
void LimitedQueue<T>::wake4Tbb()
{
	put4Tbb(NULL);
}

template<class T> 
bool LimitedQueue<T>::put4Tbb(T * _pData)
{
	QueueData4Tbb stQueueData4Tbb(_pData);
	bool bResult = m_clsTbbQueue.try_push(stQueueData4Tbb);
	if( bResult && m_nWaitMs > 0 )
	{
		pthread_mutex_lock(&m_QueueMutex);
		pthread_cond_signal(&m_QueueWakeCondition);
		pthread_mutex_unlock(&m_QueueMutex);
	}
	return bResult;
}

template<class T> 
T * LimitedQueue<T>::get4Tbb(int _nMs)
{
	QueueData4Tbb stQueueData4Tbb;

	m_nWaitMs = _nMs;

	if( _nMs < 0 )
	{
		m_clsTbbQueue.pop(stQueueData4Tbb);
		return (T *)(stQueueData4Tbb.m_QueueData);
	}	
	else
	{
		if( _nMs > 0 )
		{
			pthread_mutex_lock(&m_QueueMutex);
			if( getCurrentQueueSize4Tbb() == 0 )
			{
				struct timespec ts;
				clock_gettime(CLOCK_REALTIME, &ts);
				ts.tv_sec += _nMs / 1000;
				ts.tv_nsec += (_nMs % 1000)*1000*1000;
				ts.tv_sec += ts.tv_nsec / (1000*1000*1000);
				ts.tv_nsec = ts.tv_nsec % (1000*1000*1000);
				pthread_cond_timedwait(&m_QueueWakeCondition, &m_QueueMutex, &ts);
			}
			pthread_mutex_unlock(&m_QueueMutex);
		}
	
		m_clsTbbQueue.try_pop(stQueueData4Tbb);
		return (T *)(stQueueData4Tbb.m_QueueData);
	}	
}

template<class T> 
void LimitedQueue<T>::setMaxQueueSize4Tbb(unsigned int _unMaxQueueSize)
{
	m_clsTbbQueue.set_capacity(_unMaxQueueSize);
}

template<class T> 
unsigned int LimitedQueue<T>::getMaxQueueSize4Tbb() const
{
	return m_clsTbbQueue.capacity();
}

template<class T> 
unsigned int LimitedQueue<T>::getCurrentQueueSize4Tbb() const
{
	return m_clsTbbQueue.size();
}

#endif

}

#endif

