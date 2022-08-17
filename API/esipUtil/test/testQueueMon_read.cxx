
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "queueMonitor.hxx"
#include "shm.hxx"

using namespace eSipUtil;

int main()
{
	int nShmKey = 0x00123000;
	int nShmId;
	
	void * pShm = createShm(nShmKey, 0, true, nShmId);
	if( !pShm )
	{
		printf("fail to getShm\n");
		return 1;
	}

	const QueueMonShm * pstQueueMonShm = (const QueueMonShm *)pShm;
	const QueueMonEntity * arrQueueMonEntity = pstQueueMonShm->m_fnGetQueueMonEntityArray();

	printf("Current QueueNum[%u] : QueueName[%s], MaxSize[%u]\n", 
				pstQueueMonShm->m_unCurrentQueueNum,
				arrQueueMonEntity[0].m_szQueueName, arrQueueMonEntity[0].m_unMaxSize);

	sleep(5);

	time_t ulPrevLastTouch = 0;

	while(1)
	{
		printf("CurSize[%u], Touch[%lu]\n", 
				arrQueueMonEntity[0].m_unCurrentSize, arrQueueMonEntity[0].m_ulLastTouch);

		if( ulPrevLastTouch != arrQueueMonEntity[0].m_ulLastTouch && ulPrevLastTouch + 1 != arrQueueMonEntity[0].m_ulLastTouch )
			printf("ERR : prevTimer[%lu], curTime[%lu]\n", ulPrevLastTouch, arrQueueMonEntity[0].m_ulLastTouch);

		ulPrevLastTouch = arrQueueMonEntity[0].m_ulLastTouch;
	}

	return 0;
}

