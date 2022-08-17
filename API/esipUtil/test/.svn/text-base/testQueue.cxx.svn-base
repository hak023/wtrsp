
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "queue.hxx"

using namespace eSipUtil;

struct TestData
{
	TestData(int _n)
		: i(_n)
	{
	}
	
	int i;
};

LimitedQueue<TestData> g_testQueue1(1000);

void * getProc1(void * arg)
{
	TestData * pclsTestData = NULL;
	int nGetCnt = 0;
	
	while( (pclsTestData = g_testQueue1.get(1000*10)) )
	{
		printf("Queue test 1 : get() : %d\n", pclsTestData->i);
		delete pclsTestData;
		pclsTestData = NULL;
		++nGetCnt;
	}

	printf("Queue test 1 : current size[%u]\n", g_testQueue1.getCurrentQueueSize());	
	printf("Queue test 1 : getCnt[%d]\n", nGetCnt);

	return NULL;
}

void * getProc2(void * arg)
{
	TestData * pclsTestData = NULL;
	int nGetCnt = 0;
	
	while( (pclsTestData = g_testQueue1.get(1000*10)) )
	{
		printf("Queue test 2 : get() : %d\n", pclsTestData->i);
		delete pclsTestData;
		pclsTestData = NULL;
		++nGetCnt;
	}

	printf("Queue test 2 : current size[%u]\n", g_testQueue1.getCurrentQueueSize());	
	printf("Queue test 2 : getCnt[%d]\n", nGetCnt);

	return NULL;
}

void * putProc1(void * arg)
{
	TestData * pclsTestData = NULL;
	
	for( int i = 0; i < 500; ++i )
	{
		pclsTestData = new TestData(i);
		if( !g_testQueue1.put(pclsTestData) )
		{
			printf("Queue test : fail : put() : index[%d]\n", i);
			delete pclsTestData;
			pclsTestData = NULL;
		}
	}
	
	return NULL;
}

void * putProc2(void * arg)
{
	TestData * pclsTestData = NULL;
	
	for( int i = 0; i < 600; ++i )
	{
		pclsTestData = new TestData(i);
		if( !g_testQueue1.put(pclsTestData) )
		{
			printf("Queue test : fail : put() : index[%d]\n", i);
			delete pclsTestData;
			pclsTestData = NULL;
		}
	}
	
	return NULL;
}

int main()
{
	pthread_t threadId1, threadId2, threadId3, threadId4;

	pthread_create(&threadId1, 0, getProc1, NULL);
	pthread_create(&threadId2, 0, getProc2, NULL);

	sleep(5);
	
	pthread_create(&threadId3, 0, putProc1, NULL);	
	pthread_create(&threadId4, 0, putProc2, NULL);	

	sleep(30);
	
	return 0;
}

