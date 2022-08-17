
#include <stdio.h>

#include "flexIf.hxx"
#include "log.hxx"

using namespace eSipUtil;

int main()
{
	int nLogInstanceId;
	AsyncFileLog::m_fnInit("./log/", "testFlexIf", nLogInstanceId, 10000);
	AsyncFileLog::m_fnSetLogLevel(nLogInstanceId, 0, E_LOG_DEBUG);

	/*==== Encode ====*/
	FlexIf clsEncodeIf("sip", "sipBody", "custom", NULL);

	if( !clsEncodeIf.m_fnAddItem("sip", "contact", "sip:hdkim1@iok.co.kr") )
		printf("fail : Encode addItem : sip-contact 1\n");	

	if( !clsEncodeIf.m_fnAddItem("sop", "From", "sip:hdkim@iok.co.kr") )
		printf("fail : Encode addItem : sop-from\n");

	if( !clsEncodeIf.m_fnAddItem("custom", "hdkim", "test1") )
		printf("fail : Encode addItem : custom-hdkim 1\n");

	if( !clsEncodeIf.m_fnAddItem("custom", "merry", "dummy") )
		printf("fail : Encode addItem : custom-merry \n");

	if( !clsEncodeIf.m_fnAddItem("custom", "hdkim", "test2") )
		printf("fail : Encode addItem : custom-hdkim 2 \n");

	if( !clsEncodeIf.m_fnAddItem("custom", "key1", "value1") )
		printf("fail : Encode addItem : custom-key 1 \n");

	if( !clsEncodeIf.m_fnAddItem("custom", "key2", "value2") )
		printf("fail : Encode addItem : custom-key 2 \n");

	if( !clsEncodeIf.m_fnAddItem("custom", "key3", "value3") )
		printf("fail : Encode addItem : custom-key 3 \n");

	if( !clsEncodeIf.m_fnAddItem("custom", "key4", "value4") )
		printf("fail : Encode addItem : custom-key 4 \n");

	if( !clsEncodeIf.m_fnAddItem("sip", "contact", "sip:hdkim2@iok.co.kr") )
		printf("fail : Encode addItem : sip-contact 2\n");

	char szPrint[10000];
	printf("------Encode------\n%s\n\n", clsEncodeIf.m_fnGetSuccPacketStr(szPrint, sizeof(szPrint)));
	strcpy(szPrint, "TestEncode : ");
	printf("%s\n", clsEncodeIf.m_fnGetDebugStr(szPrint, sizeof(szPrint), false));

	unsigned int unMsgLen = 0;
	void * pMsg = clsEncodeIf.m_fnGetBuildMsg(unMsgLen);

	/*==== Decode ====*/
	FlexIf clsDecodeIf;
	if( !clsDecodeIf.m_fnSetBuildMsg(pMsg, unMsgLen) )
	{
		printf("fail : SetBuildMsg\n");
		return 1;
	}

	if( !clsDecodeIf.m_fnAddItem("sipBody", "text", "1234567890") )
		printf("fail : Decode addItem : sipBody-text\n");

	unsigned int unPos = 0;
	do
	{
		const char * pszValue = clsDecodeIf.m_fnGetNextItem("sip", "contact", unPos);
		if( pszValue )
			printf("Decode GetNextItem : sip-contact : %s\n", pszValue);		
	}
	while( unPos > 0 );

	unPos = 0;
	do
	{
		const char * pszValue = clsDecodeIf.m_fnGetNextItem("custom", "hdkim", unPos);
		if( pszValue )
			printf("Decode GetNextItem : custom-hdkim : %s\n", pszValue);		
	}
	while( unPos > 0 );

	unPos = 0;
	do
	{
		const char * pszValue = clsDecodeIf.m_fnGetNextItem("custom", "key3", unPos);
		if( pszValue )
			printf("Decode GetNextItem : custom-key3 : %s\n", pszValue);		
	}
	while( unPos > 0 );

	unPos = 0;
	do
	{
		const char * pszValue = clsDecodeIf.m_fnGetNextItem("custom", "key4", unPos);
		if( pszValue )
			printf("Decode GetNextItem : custom-key3 : %s\n", pszValue);		
	}
	while( unPos > 0 );

	unPos = 0;
	do
	{
		const char * pszKey = NULL;
		const char * pszValue = NULL;
		if( clsDecodeIf.m_fnGetNextAnyItem("sip", pszKey, pszValue, unPos) )
			printf("Decode GetNextAnyItem : sip : %s / %s\n", pszKey, pszValue);		
	}
	while( unPos > 0 );

	unPos = 0;
	do
	{
		const char * pszKey = NULL;
		const char * pszValue = NULL;
		if( clsDecodeIf.m_fnGetNextAnyItem("custom", pszKey, pszValue, unPos) )
			printf("Decode GetNextAnyItem : custom : %s / %s\n", pszKey, pszValue);		
	}
	while( unPos > 0 );
	
	printf("------Decode------\n%s\n\n", clsDecodeIf.m_fnGetSuccPacketStr(szPrint, sizeof(szPrint)));
	strcpy(szPrint, "TestDecode : ");
	printf("%s\n", clsDecodeIf.m_fnGetDebugStr(szPrint, sizeof(szPrint), false));
	
	return 0;
}

