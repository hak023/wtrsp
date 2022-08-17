
#include <stdio.h>

#include "log.hxx"
#include "shm.hxx"

using namespace eSipUtil;

const char * g_pszShmPathName = "./shm1";
int g_nLogInstanceId = -1;

#define LogTest(_level, _format, args...) Log(g_nLogInstanceId, 0, _level, _format, ##args)

int main()
{
	int nShmId;
	void * pShmMem = NULL;

	AsyncFileLog::m_fnInit("./log/", "testShm", g_nLogInstanceId);
	
	pShmMem = createShm(g_pszShmPathName, 10000000, false, nShmId);
	if( !pShmMem )
	{
		printf("1\n");
		return 1;
	}

	pShmMem = createShm(g_pszShmPathName, 10000000, false, nShmId);
	if( !pShmMem )
	{
		printf("2\n");
		return 1;
	}

	if( !destroyShm(g_pszShmPathName, pShmMem) )
	{
		printf("3\n");
		return 1;
	}

/*
	pShmMem = createShm(g_pszShmPathName, 20000000, false, nShmId);
	if( !pShmMem )
	{
		printf("4\n");
		return 1;
	}
*/
}
