//GH.Nam
#include <stdio.h>

#include "ColumFile.hxx"
#include "log.hxx"
#include "string.hxx"

using namespace eSipUtil;

int main()
{
	int nLogInstanceId;
	AsyncFileLog::m_fnInit("./log/", "testColumFile", nLogInstanceId, 10000);
	AsyncFileLog::m_fnSetLogLevel(nLogInstanceId, 0, E_LOG_DEBUG);

   ColumFile clsColumFile;
   clsColumFile.m_fnChangeParamKeyword(':');

   clsColumFile.m_fnLoadConfig("./testColumFile.cfg");

   KString clsStr;
   clsColumFile.m_fnDebug(clsStr);

   printf("Test(%d)\n%s\n",  clsColumFile.m_fnGetNums(), (KCSTR)clsStr);

   ListItr_t stItor1;
   ColumParams* pclsColumParams = clsColumFile.m_fnBegin(stItor1);
//T_ID:4, T_TYPE_ID:0x09, T_LEVEL:1, CALLER:010, CALLED:010
   while(pclsColumParams)
   {
      KString clsTid = (KUINT)pclsColumParams->m_fnGetParam("T_ID");
      KString clsCaller= (KCSTR)pclsColumParams->m_fnGetParam("CALLER");
      KString clsCalled = (KCSTR)pclsColumParams->m_fnGetParam("CALLED");
      printf("[TID:%d][CALLER:%s][CALLED:%s]", (KINT)clsTid, (KCSTR)clsCaller, (KCSTR)clsCalled);
      printf("\n\n");
      pclsColumParams = clsColumFile.m_fnNext(stItor1);
   }
	
	return 0;
}

