
#include <stdio.h>
#include <unistd.h>

#include "queueMonitor.hxx"

using namespace eSipUtil;

int main()
{
	int nShmKey = 0x00123000;

	QueueMonMgr::getInstance(1, nShmKey);

	unsigned int unQueueMonId = 0;
	if( !QueueMonMgr::getInstance().m_fnAddQueue("test1", 100, unQueueMonId) )
	{
		printf("fail to AddQueue\n");
		return 1;
	}

	unsigned int unCurrentSize = 0;

	while(1)
	{
		QueueMonMgr::getInstance().m_fnTouchQueue(unQueueMonId, unCurrentSize);
		++unCurrentSize;

		//usleep(10);
	}

	return 0;
}
