
#include <stdio.h>
#include <unistd.h>

#include "queueThread.hxx"
#include "log.hxx"

using namespace eSipUtil;

int g_nLogInstanceId = -1;

#define LogTest(_level, _format, args...) Log(g_nLogInstanceId, 0, _level, _format, ##args)

struct QueueData
{
	int i;

	QueueData(int _n)
		: i(_n)
	{
	}
};
typedef struct QueueData QueueData_t;

void callback(QueueData_t * _pstQueueData, void * _pObject)
{
	static int nGetCnt = 0;
	if( _pstQueueData )
	{
		++nGetCnt;
		printf("Callback : GetCnt[%d], Item[%d]\n", nGetCnt, _pstQueueData->i);
		delete _pstQueueData;
	}
}

int main()
{
	AsyncFileLog::m_fnInit("./log/", "testQueueThread", g_nLogInstanceId);
	
	QueueThread< QueueData_t > queueThread(1000, callback, -1, "TestQueueThread");
	queueThread.run();

	for( int i = 101; i < 200; ++i )
	{
		queueThread.put(new QueueData_t(i));
	}

	sleep(5);
	
	queueThread.shutdown();
}

