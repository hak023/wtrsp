
#ifndef ESIPUTIL_QUEUE_SHM_H
#define ESIPUTIL_QUEUE_SHM_H

#include <sys/ipc.h>
#include <sys/shm.h>

struct QueueMonEntity
{
	enum
	{
		E_CONST_MAX_QUEUE_NAME_LEN = 128,
	};

	//static
	char m_szQueueName[E_CONST_MAX_QUEUE_NAME_LEN];
	unsigned int m_unMaxSize;

	//dynamic
	unsigned int m_unCurrentSize;
	time_t m_ulLastTouch;

	void m_fnReset()
	{
		m_szQueueName[0] = 0x00;
		m_unMaxSize = 0;
		m_unCurrentSize = 0;
		m_ulLastTouch = 0;
	}
};

struct QueueMonShm
{
	unsigned int m_unMaxQueueNum;
	unsigned int m_unCurrentQueueNum;

	static size_t m_fnGetHdrShmSize()
	{
		return sizeof(unsigned int) + sizeof(unsigned int);
	}

	static size_t m_fnGetMaxShmSize(unsigned int _unMaxQueueNum)
	{
		return m_fnGetHdrShmSize() + sizeof(QueueMonEntity)*_unMaxQueueNum;
	}

	size_t m_fnGetCurrentShmSize() const
	{
		return m_fnGetHdrShmSize() + sizeof(QueueMonEntity)*m_unCurrentQueueNum;
	}

	bool m_fnValid(int _nShmId) const
	{
		struct shmid_ds stShmInfo;
		if( shmctl(_nShmId, IPC_STAT, &stShmInfo) != 0 )
			return false;

		if( m_fnGetMaxShmSize(m_unMaxQueueNum) <= stShmInfo.shm_segsz && m_unCurrentQueueNum <= m_unMaxQueueNum )
			return true;
		else
			return false;
	}	

	QueueMonEntity * m_fnGetQueueMonEntityArray()
	{
		return (QueueMonEntity *)((char *)this + m_fnGetHdrShmSize());
	}

	const QueueMonEntity * m_fnGetQueueMonEntityArray() const
	{
		return (QueueMonEntity *)((char *)this + m_fnGetHdrShmSize());
	}

	void m_fnReset(unsigned int _unMaxQueueNum)
	{
		m_unMaxQueueNum = _unMaxQueueNum;
		m_unCurrentQueueNum = 0;
		
		QueueMonEntity * arrQueueMonEntity = m_fnGetQueueMonEntityArray();
		for(unsigned int i = 0; i < m_unMaxQueueNum; ++i )
		{
			arrQueueMonEntity[i].m_fnReset();
		}
	}

	void m_fnResetCurrentSize()
	{
		QueueMonEntity * arrQueueMonEntity = m_fnGetQueueMonEntityArray();
		for(unsigned int i = 0; i < m_unCurrentQueueNum; ++i )
		{
			arrQueueMonEntity[i].m_unCurrentSize = 0;
			arrQueueMonEntity[i].m_ulLastTouch = 0;
		}
	}
};

#endif

