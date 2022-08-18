
#ifndef ESIPUTIL_QUEUE_MONITOR_H
#define ESIPUTIL_QUEUE_MONITOR_H

#include "queue_Shm.h"
#include "lock.hxx"

namespace eSipUtil
{

class QueueMonMgr
{
	public :
		static QueueMonMgr & getInstance(unsigned int _unMaxQueueNum=0, int _nShmKey=0);
		
		~QueueMonMgr();

		void m_fnResetCurrentSize();
		bool m_fnAddQueue(const char * _pszQueueName, unsigned int _unMaxSize, unsigned int & _runQueueMonId);
		bool m_fnSetQueueName(unsigned int _unQueueMonId, const char * _pszQueueName);
		bool m_fnSetMaxSize(unsigned int _unQueueMonId, unsigned int _unMaxSize);
		bool m_fnTouchQueue(unsigned int _unQueueMonId, unsigned int _unCurrentSize);			

	private :
		QueueMonMgr(unsigned int _unMaxQueueNum, int _nShmKey);
		
		unsigned int m_unMaxQueueNum;
		int m_nShmKey;
		QueueMonShm * m_pstQueueMonShm;
		Mutex m_clsAddMutex;
};

}

#endif

