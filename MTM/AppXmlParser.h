#ifndef APP_XML_PARSER_H
#define APP_XML_PARSER_H
#include <tinyxml.h>
#include "string.hxx"
#include "NasSystemTable.h"
#include "MainConfig.h"
#include "TargetContent.h"
#include "SourceContent.h"
#include "AppIf.h"

using namespace eSipUtil;

class AppXmlParser
{
public:
	AppXmlParser();
	~AppXmlParser();
	static bool m_fnValid(KString &_rclsXml);
	/*Get Value*/
	static bool m_fnGetBodyType(KString &_rclsXml, KString &_rclsBodyType);
	static unsigned int m_fnGetTransactionID(KString &_rclsXml);
	static void m_fnGetSessionID(KString &_rclsXml, KString &_rclsSessionID);
	static void m_fnGetJobID(KString &_rclsXml, KString &_rclsJobID);
	static void m_fnGetToFrom(KString &_rclsXml, KString &_rclsTo, KString &_rclsFrom);
	static void m_fnGetNasCode(KString &_rclsXml, int *_pnSrcNasCode, int *_pnDstNasCode);
	static void m_fnGetSourceContentInfo(KString &_rclsXml, SourceContent *_pclsSourceContent);
	static unsigned int m_fnGetTranscodingList(KString & _rclsXml);
	static void m_fnGetTransSessionID(KString &_rclsXml, KString &_rclsTransSessionID);
	static unsigned int m_fnGetTargetContentList(KString &_rclsXml, StlList *_pclsTargetList);
	static void m_fnMakeTCCommand(KString &_rclsXml, StlList *_pclsTargetList);
	/*Make Msg*/
	static void m_fnMakeLinkTestReq(KString &_rclsFrom, KString &_rclsTo, KString &_rclsSessionID, int _rclsTransactionID, KString &_rclsLinktTestReq);
	static void m_fnMakeEstablishSessionReq(KString &_rclsFrom, KString &_rclsTo, int nTransactionID, KString &_rclsEstablishSessionReq);
	static void m_fnMakeJobStatusChangedNotify_Created(KString &_rclsXml, KString &_rclsJobStatusChangedNotify);
	static void m_fnMakeJobStatusChangedNotify_Waiting(KString &_rclsXml, KString &_rclsJobStatusChangedNotify);
	static void m_fnMakeJobStatusChangedNotify_JobStarted(KString &_rclsXml, unsigned int _unTranscodingList, KString &_rclsJobStatusChangedNotify);
	static void m_fnMakeJobStatusChangedNotify_TC_Started(KString &_rclsXml, SourceContent *_pclsSource, TargetContent *_pclsTarget, unsigned int _unFail, unsigned int _unSuccess, unsigned int _unTargetIdx, KString &_rclsJobStatusChangedNotify);
	static void m_fnMakeJobStatusChangedNotify_TC_Stopped(KString &_rclsXml, ETrssCodeSet_t _eSt, KString &_rclsResultDescription, unsigned int _unFail, unsigned int _unSuccess, KString &_rclsJobStatusChangedNotify);
	static void m_fnMakeJobStatusChangedNotify_JobStopped(KString &_rclsXml, KString &_rclsJobStatusChangedNotify);
	static void m_fnMakeJobStatusChangedNotify_Destroyed(KString &_rclsXml, unsigned int _unTranscodingList, ETrssCodeSet_t _eSt, KString &_rclsJobStatusChangedNotify);
	/*Util*/
	static void m_fnGetCurrentTime(KString &_rclsCurrent);
	static void m_fnXmlFormatter(KString & _rclsXml, KString & _rclsXmlFormat);
};

#endif
