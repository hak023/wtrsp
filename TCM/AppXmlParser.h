#ifndef APP_XML_PARSER_H
#define APP_XML_PARSER_H
#include <tinyxml.h>
#include "string.hxx"
#include "AppIf.h"
#include "AclSystemTable.h"

using namespace eSipUtil;

class CTranscodesCnt;
class AppXmlParser
{
public:
	AppXmlParser();
	~AppXmlParser();
	/*Valid Check*/
	static void m_fnGetCurrentTime(KString &_rclsCurrent);
	static bool m_fnValid(KString & _rclsXml);//default valid check
	static ETrsgCodeSet_t m_fnCreateJobValid(KString & _rclsXml);//CreateJobRequest valid check
	/*Get Value*/
	static bool m_fnGetBodyType(KString & _rclsXml, KString & _rclsBodyType);
	static unsigned int m_fnGetTransactionID(KString & _rclsXml);
	static void m_fnGetSessionID(KString & _rclsXml, KString & _rclsSessionID);
	static void m_fnGetJobID(KString & _rclsXml, KString & _rclsJobID);
	static void m_fnGetToFrom(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom);
	static void m_fnGetNasCode(KString & _rclsXml, int* _pnSrcNasCode, int* _pnDstNasCode);
	static void m_fnGetSourceContentInfo(KString & _rclsXml, KString & _rclsPath, KString & _rclsFile, KString & _rclsContainer);
	static void m_fnGetAuthInfo(KString & _rclsXml, KString & _rclsID, KString & _rclsPWD, KString & _rclsIP);
	static unsigned int m_fnGetTranscodingList(KString & _rclsXml);
	static void m_fnGetJobStateResultCode(KString & _rclsXml, KString & _rclsResultCode);
	static void m_fnGetTransSessionID(KString & _rclsXml, KString & _rclsTransSessionID);
	static void m_fnGetTransCnt(KString & _rclsXml, int* _pnCancel, int* _pnCurrent, int* _pnFail, int* _pnSuccess, int* _pnTotal);
	static void m_fnGetTargetContentList(KString & _rclsXml, StlList * _pclsTargetList);
	/*Make Msg*/
	static void m_fnMakeLinkTestRes(KString & _rclsXml, KString & _rclsLinkTestRes);
	static void m_fnMakeEstablishSessionRes(KString & _rclsXml, KString & _rclsSessionID, ETrsgCodeSet_t _eSt, KString & _rclsEstablishSessionRes);
	static void m_fnMakeCloseSessionRes(KString & _rclsXml, KString & _rclsCloseSessionRes);
	static void m_fnMakeCreateJobReq(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, KString & _rclsJobID, KString & _rclsInternalNasFileName, KString & _rclsInternalNasImgFileName, KString & _rclsCreateJobReq);
	static void m_fnMakeCreateJobRes(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsJobID, ETrsgCodeSet_t _eSt, KString & _rclsCreateJobRes);
	static void m_fnMakeJobStatusChangedNotify_Created(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, KString & _rclsJobStatusChgNotify);
	static void m_fnMakeJobStatusChangedNotify_Waiting(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, KString & _rclsJobStatusChgNotify);
	static void m_fnMakeJobStatusChangedNotify_JobStarted(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, CTranscodesCnt & _rclsTranscodesCnt, KString & _rclsJobStatusChgNotify);
	static void m_fnMakeJobStatusChangedNotify_TC_Started(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, CTranscodesCnt & _rclsTranscodesCnt, KString & _rclsJobStatusChgNotify);
	static bool m_fnMakeJobStatusChangedNotify_TC_Stopped(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, CTranscodesCnt & _rclsTranscodesCnt, KString & _rclsResultCode, KString & _rclsJobStatusChgNotify);
	static void m_fnMakeJobStatusChangedNotify_JobStopped(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, CTranscodesCnt & _rclsTranscodesCnt, KString & _rclsJobStatusChgNotify);
	static void m_fnMakeJobStatusChangedNotify_Destroyed(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, CTranscodesCnt & _rclsTranscodesCnt, KString & _rclsJobStatusChgNotify);
	static void m_fnMakeJobStatusChangedNotify_Destroyed(KString & _rclsXml, KString & _rclsJobStatusChgNotify, CTranscodesCnt & _rclsTranscodesCnt, ETrsgCodeSet_t _eSt);//자체적으로 Destroy를 만들 때.(Overloading)
	/*Write File*/
	static ETrsgCodeSet_t m_fnWriteSourceFile(KString & _rclsXml, KString & _rclsPath, KString & _rclsFileName);
	static bool m_fnWriteImageFile(KString & _rclsXml, KString & _rclsPath, KString & _rclsImgFileName);
	/*Util*/
	static void m_fnSetLongLogSkip(KString & _rclsXml, KString & _rclsSkipXml);
	static void m_fnSetTranscodesCnt(TiXmlElement* pclsBodyElement, CTranscodesCnt & _rclsTranscodesCnt);
};

#endif
