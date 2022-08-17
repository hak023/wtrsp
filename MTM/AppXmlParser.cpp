#include <fstream>
#include "AppXmlParser.h"
#include "main.h"
#include "CBase64.h"
#include "AppIf.h"

using namespace eSipUtil;

AppXmlParser::AppXmlParser(){}
AppXmlParser::~AppXmlParser(){}

bool AppXmlParser::m_fnValid(KString & _rclsXml)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml);
	if(doc.Error())
	{
		IFLOG(E_LOG_ERR,"[AppXmlParser]Error in [%s]", doc.ErrorDesc());
		return false;
	}

	TiXmlElement* rootElement = doc.RootElement();
	if (rootElement == NULL)
	{
		IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[WTRS.MSG]");
		return false;
	}

	TiXmlNode* headerNode = rootElement->FirstChild("HEADER");
	if (headerNode == NULL)
	{
		IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[HEADER]");
		return false;
	}
	else
	{
		if(headerNode->ToElement()->Attribute("SessionID") == NULL)
		{
			IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[HEADER-SessionID]");
			return false;
		}
		if(headerNode->ToElement()->Attribute("From") == NULL)
		{
			IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[HEADER-From]");
			return false;
		}
		if(headerNode->ToElement()->Attribute("To") == NULL)
		{
			IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[HEADER-To]");
			return false;
		}
		if(headerNode->ToElement()->Attribute("TransactionID") == NULL)
		{
			IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[HEADER-TransactionID]");
			return false;
		}
		TiXmlNode* errorNode = headerNode->ToElement()->FirstChild("Error");
		if (errorNode == NULL)
		{
			IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[Error]");
			return false;
		}
		else
		{
			if(errorNode->ToElement()->Attribute("Code") == NULL)
			{
				IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[Error-Code]");
				return false;
			}
			if(errorNode->ToElement()->Attribute("Source") == NULL)
			{
				IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[Error-Source]");
				return false;
			}
			if(errorNode->ToElement()->Attribute("Description") == NULL)
			{
				IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[Error-Description]");
				return false;
			}
		}
	}

	TiXmlNode* bodyNode = rootElement->FirstChild("BODY");
	if (bodyNode == NULL)
	{
		IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[BODY]");
		return false;
	}
	IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Valid OK");
	return true;
}
bool AppXmlParser::m_fnGetBodyType(KString & _rclsXml, KString & _rclsBodyType)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml);
	TiXmlElement* rootElement = doc.RootElement();

	TiXmlElement* bodyTypeElement = rootElement->FirstChildElement("BODY")->FirstChildElement();
   if (bodyTypeElement == NULL)
   {
      _rclsBodyType = "UNKNOWN";
      return false;
   }
   else
   {
      if(KString::m_fnStrCmp(bodyTypeElement->Value(), DEF_JOB_STATUS_CHANGED_NOTIFY) == 0)
      {
         TiXmlElement* jobStateElement = bodyTypeElement->FirstChildElement("JobState");
         if (jobStateElement == NULL)
         {
            _rclsBodyType = "UNKNOWN";
            return false;
         }         

         CmpString clsStatus = jobStateElement->Attribute("Status");
         if(clsStatus == "Created")
         {
        	 _rclsBodyType = DEF_JOB_STATUS_CHANGED_NOTIFY_CREATED;
         }
         else if(clsStatus == "Waiting")
         {
        	 _rclsBodyType = DEF_JOB_STATUS_CHANGED_NOTIFY_WAITING;
         }
         else if(clsStatus == "JobStarted")
         {
        	 _rclsBodyType = DEF_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED;
         }
         else if(clsStatus == "TranscodingStarted")
         {
        	 _rclsBodyType = DEF_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED;
         }
         else if(clsStatus == "TranscodingStopped")
         {
        	 _rclsBodyType = DEF_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED;
         }
         else if(clsStatus == "JobStopped")
         {
        	 _rclsBodyType = DEF_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED;
         }
         else if(clsStatus == "Destroyed")
         {
        	 _rclsBodyType = DEF_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED;
         }
         else
         {
            _rclsBodyType = "UNKNOWN";
            return false;
         }
      }
      else
      {
         _rclsBodyType = bodyTypeElement->Value();
      }
   }
   return true;
}
unsigned int AppXmlParser::m_fnGetTransactionID(KString & _rclsXml)
{
	unsigned int unTid=0;
	int nTid = 0;

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");
	if(headerElement->QueryIntAttribute("TransactionID",&nTid) == TIXML_SUCCESS)
	{
		if(nTid > 0) unTid = nTid;
		else unTid = 0;

		return unTid;
	}
	else
		return 0;
}
void AppXmlParser::m_fnGetSessionID(KString & _rclsXml, KString & _rclsSessionID)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");
	_rclsSessionID = headerElement->Attribute("SessionID");
}
void AppXmlParser::m_fnGetJobID(KString & _rclsXml, KString & _rclsJobID)
{
   TiXmlDocument doc;
   doc.Parse((KCSTR)_rclsXml);
   TiXmlElement* rootElement = doc.RootElement();
   TiXmlElement* bodyElement =  rootElement->FirstChildElement("BODY");

   TiXmlElement* jobStatusChangedNotifyElement = bodyElement->FirstChildElement("JobStatusChangedNotify");

   if(jobStatusChangedNotifyElement == NULL)
   {
      if(bodyElement->FirstChildElement("CreateJobRequest")->FirstChildElement("Job") != NULL)
            _rclsJobID = bodyElement->FirstChildElement("CreateJobRequest")->FirstChildElement("Job")->Attribute("ID");
      else
         _rclsJobID = "UNKNOWN";
   }
   else _rclsJobID = jobStatusChangedNotifyElement->FirstChildElement("JobState")->Attribute("ID");
}
void AppXmlParser::m_fnGetToFrom(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom)
{
   TiXmlDocument doc;
   doc.Parse((KCSTR)_rclsXml);
   TiXmlElement* rootElement = doc.RootElement();
   TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

   _rclsFrom = headerElement->Attribute("From");
   _rclsTo = headerElement->Attribute("To");
}
void AppXmlParser::m_fnGetNasCode(KString & _rclsXml, int* _pnSrcNasCode, int* _pnDstNasCode)
{
   KString clsBodyType;
   if(AppXmlParser::m_fnGetBodyType(_rclsXml, clsBodyType))
   {
      TiXmlDocument doc;
      doc.Parse((KCSTR) _rclsXml);
      TiXmlElement* rootElement = doc.RootElement();;
      TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");

      if(clsBodyType == DEF_BODY_CREATE_JOB_REQ)
      {
         TiXmlElement* jobElement = bodyElement->FirstChildElement(
               "CreateJobRequest")->FirstChildElement("Job");

         if(_pnSrcNasCode)
         {
            TiXmlElement* sourceContentElement = jobElement->FirstChildElement("SourceContentMap")->
               FirstChildElement("SourceContent");

            if(sourceContentElement->Attribute("NASCode"))
            {
               sourceContentElement->Attribute("NASCode", _pnSrcNasCode);
               IFLOG(E_LOG_INFO, "[AppXmlParser] %s Src Nas Type NASCode[%d]", (KCSTR)clsBodyType, *_pnSrcNasCode);
            }
            else
            {
               *_pnSrcNasCode = -1;
               IFLOG(E_LOG_INFO, "[AppXmlParser] %s Src TCP Type NASCode[%d]", (KCSTR)clsBodyType, *_pnSrcNasCode);
            }
         }
         if(_pnDstNasCode)
         {
            TiXmlElement* sourceContentElement = jobElement->FirstChildElement("TargetContentMap")->
               FirstChildElement("TargetContent");

            if(sourceContentElement->Attribute("NASCode"))
            {
               sourceContentElement->Attribute("NASCode", _pnDstNasCode);
               IFLOG(E_LOG_INFO, "[AppXmlParser] %s Dst Nas Type NASCode[%d]", (KCSTR)clsBodyType, *_pnDstNasCode);
            }
            else
            {
               *_pnDstNasCode = -1;
               IFLOG(E_LOG_INFO, "[AppXmlParser] %s Dst TCP Type NASCode[%d]", (KCSTR)clsBodyType, *_pnDstNasCode);
            }
         }
      }
      else if(clsBodyType == DEF_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED
            || clsBodyType == DEF_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED)
      {
         TiXmlElement* transcodeElement =
            bodyElement->FirstChildElement("JobStatusChangedNotify")->FirstChildElement(
                  "JobState")->FirstChildElement("Transcodes")->FirstChildElement(
                     "Transcode");

         if(_pnSrcNasCode)
         {
            TiXmlElement* sourceElement = transcodeElement->FirstChildElement("Source");

            if(sourceElement->Attribute("NASCode"))
            {
               sourceElement->Attribute("NASCode", _pnSrcNasCode);
               IFLOG(E_LOG_INFO, "[AppXmlParser] %s Src Nas Type NASCode[%d]", (KCSTR)clsBodyType, *_pnSrcNasCode);
            }
            else
            {
               *_pnSrcNasCode = -1;
               IFLOG(E_LOG_INFO, "[AppXmlParser] %s Src TCP Type NASCode[%d]", (KCSTR)clsBodyType, *_pnSrcNasCode);
            }
         }
         if(_pnDstNasCode)
         {
            TiXmlElement* targetElement = transcodeElement->FirstChildElement("Target");

            if(targetElement->Attribute("NASCode"))
            {
               targetElement->Attribute("NASCode", _pnDstNasCode);
               IFLOG(E_LOG_INFO, "[AppXmlParser] %s Dst Nas Type NASCode[%d]", (KCSTR)clsBodyType, *_pnDstNasCode);

            }
            else
            {
               *_pnDstNasCode = -1;
               IFLOG(E_LOG_INFO, "[AppXmlParser] %s Dst TCP Type NASCode[%d]", (KCSTR)clsBodyType, *_pnDstNasCode);
            }
         }
      }
   }
}
void AppXmlParser::m_fnGetSourceContentInfo(KString & _rclsXml, SourceContent *_pclsSourceContent)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR) _rclsXml);
	TiXmlElement *rootElement = doc.RootElement();

	TiXmlElement *bodyElement = rootElement->FirstChildElement("BODY");
	TiXmlElement *jobElement = bodyElement->FirstChildElement("CreateJobRequest")->FirstChildElement("Job");

	TiXmlElement *sourceContentElement = jobElement->FirstChildElement("SourceContentMap")->FirstChildElement("SourceContent");

	// CreateJobReq 가 BinaryData로 들어온 경우, 데이터 있음
	if (sourceContentElement->Attribute("BinaryData"))
		_pclsSourceContent->m_clsBinaryData = sourceContentElement->Attribute("BinaryData");

	// 외부 NAS 로 들어온경우,
	if (sourceContentElement->Attribute("NASCode"))
		_pclsSourceContent->m_clsNasCode = sourceContentElement->Attribute("NASCode");

	if (sourceContentElement->Attribute("Path"))
	{
		_pclsSourceContent->m_clsPath = sourceContentElement->Attribute("Path");
		if(_pclsSourceContent->m_clsPath.m_unRealLen > 0)
		{
			_pclsSourceContent->m_clsFilePath.m_fnCat((KSTR)_pclsSourceContent->m_clsPath);
			if (((KCSTR)_pclsSourceContent->m_clsPath)[_pclsSourceContent->m_clsPath.m_unRealLen - 1] != '/')
						_pclsSourceContent->m_clsFilePath.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.
		}
	}

	if (sourceContentElement->Attribute("File"))
	{
		_pclsSourceContent->m_clsFile = sourceContentElement->Attribute("File");
		_pclsSourceContent->m_clsFilePath.m_fnCat((KSTR)_pclsSourceContent->m_clsFile);
	}

	TiXmlElement *containerElement = sourceContentElement->FirstChildElement("Container");
	if (containerElement->Attribute("ID"))
	{
		_pclsSourceContent->m_clsID = containerElement->Attribute("ID");
		IFLOG(E_LOG_INFO, "[AppXmlParser] Src ContainerID[%s]", (KCSTR)_pclsSourceContent->m_clsID);
	}

	TiXmlElement *metadataElement = containerElement->FirstChildElement("Metadata");
	if(metadataElement != NULL)
	{
		_pclsSourceContent->m_clsMetadata.m_clsAlbum = metadataElement->Attribute("Album");
		_pclsSourceContent->m_clsMetadata.m_clsArtist = metadataElement->Attribute("Artist");
		_pclsSourceContent->m_clsMetadata.m_clsCopyright = metadataElement->Attribute("Copyright");
		_pclsSourceContent->m_clsMetadata.m_clsDate = metadataElement->Attribute("Date");
		_pclsSourceContent->m_clsMetadata.m_clsGenre = metadataElement->Attribute("Genre");
		_pclsSourceContent->m_clsMetadata.m_clsImageData = metadataElement->Attribute("ImageData");
		_pclsSourceContent->m_clsMetadata.m_clsImageType = metadataElement->Attribute("ImageType");
		_pclsSourceContent->m_clsMetadata.m_clsPlayDevice = metadataElement->Attribute("PlayDevice");
		_pclsSourceContent->m_clsMetadata.m_clsPlayTime = metadataElement->Attribute("PlayTime");
		_pclsSourceContent->m_clsMetadata.m_clsSite = metadataElement->Attribute("Site");
		_pclsSourceContent->m_clsMetadata.m_clsTitle = metadataElement->Attribute("Title");
		_pclsSourceContent->m_clsMetadata.m_clsTrack = metadataElement->Attribute("Track");
		_pclsSourceContent->m_clsMetadata.m_clsYear = metadataElement->Attribute("Year");
	}
	KString clsDebug;
	_pclsSourceContent->m_fnDebug(clsDebug);
	IFLOG(E_LOG_DEBUG, (KCSTR)clsDebug);

}
unsigned int AppXmlParser::m_fnGetTranscodingList(KString & _rclsXml)
{
   unsigned int nTranscoding = 0;
   
   TiXmlDocument doc;
   doc.Parse((KCSTR)_rclsXml);
   TiXmlElement* rootElement = doc.RootElement();
   TiXmlElement* bodyTypeElement = rootElement->FirstChildElement("BODY");
   TiXmlElement* jobElement = bodyTypeElement->FirstChildElement("CreateJobRequest")->FirstChildElement("Job");

   TiXmlElement* transcodingListElement = jobElement->FirstChildElement("TranscodingList");
   TiXmlElement* transcodingElement = transcodingListElement->FirstChildElement();
   while(transcodingElement)
   {
      nTranscoding++;
      //printf("Searching..[%s]\n", transcodingElement->Value());
      transcodingElement = transcodingElement->NextSiblingElement();
   }
   return nTranscoding;
}
void AppXmlParser::m_fnGetTransSessionID(KString & _rclsXml, KString & _rclsTransSessionID)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyTypeElement = rootElement->FirstChildElement("BODY");
	TiXmlElement* transcodesElement = bodyTypeElement->FirstChildElement(
				"JobStatusChangedNotify")->FirstChildElement("JobState")
						->FirstChildElement("Transcodes");

	_rclsTransSessionID = transcodesElement->Attribute("TransSessionID");
	IFLOG(E_LOG_DEBUG, "[AppXmlParser] T/C Start TransSessionID[%s]", (KCSTR)_rclsTransSessionID);
}
unsigned int AppXmlParser::m_fnGetTargetContentList(KString & _rclsXml, StlList * _pclsTargetList)
{
	if(_pclsTargetList) _pclsTargetList->m_fnClear();
//	else return 0;

	unsigned int unTargetContent = 0;

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyTypeElement = rootElement->FirstChildElement("BODY");

	TiXmlElement* jobElement = bodyTypeElement->FirstChildElement("CreateJobRequest")->FirstChildElement("Job");

	TiXmlElement* targetContentMapElement = jobElement->FirstChildElement("TargetContentMap");
	TiXmlElement* targetContentElement = targetContentMapElement->FirstChildElement("TargetContent");
	while(targetContentElement)
	{
		unTargetContent++;

		TargetContent * pclsTargetContent = new TargetContent;
//		pclsTargetContent->m_clsBinaryData = targetContentElement->Attribute("BinaryData"); //not used
		pclsTargetContent->m_clsFile = targetContentElement->Attribute("File");
		pclsTargetContent->m_clsID = targetContentElement->Attribute("ID");
		pclsTargetContent->m_clsNasCode = targetContentElement->Attribute("NASCode");
		pclsTargetContent->m_clsPath = targetContentElement->Attribute("Path");
		pclsTargetContent->m_clsSize = targetContentElement->Attribute("Size");

		if(pclsTargetContent->m_clsPath.m_unRealLen > 0)
		{
			pclsTargetContent->m_clsFilePath.m_fnCat((KCSTR)pclsTargetContent->m_clsPath);
			if (((KCSTR)pclsTargetContent->m_clsPath)[pclsTargetContent->m_clsPath.m_unRealLen - 1] != '/')
				pclsTargetContent->m_clsFilePath.m_fnCat("/");
			pclsTargetContent->m_clsFilePath.m_fnCat((KCSTR)pclsTargetContent->m_clsFile);
		}

		TiXmlElement* containerElement = targetContentElement->FirstChildElement("Container");
		if(containerElement)
		{
			pclsTargetContent->m_clsContainer.m_clsID = containerElement->Attribute("ID");

			TiXmlElement* audioCodecElement = containerElement->FirstChildElement("AudioCodec");
			if(audioCodecElement)
			{
				pclsTargetContent->m_clsContainer.clsAudioCodec.m_clsBitPerSample = audioCodecElement->Attribute("BitPerSample");
				pclsTargetContent->m_clsContainer.clsAudioCodec.m_clsBitRate = audioCodecElement->Attribute("BitRate");
				pclsTargetContent->m_clsContainer.clsAudioCodec.m_clsChannel = audioCodecElement->Attribute("Channel");
				pclsTargetContent->m_clsContainer.clsAudioCodec.m_clsID = audioCodecElement->Attribute("ID");
				pclsTargetContent->m_clsContainer.clsAudioCodec.m_clsSampleRate = audioCodecElement->Attribute("SampleRate");
			}

			TiXmlElement* videoCodecElement = containerElement->FirstChildElement("VideoCodec");
			if(videoCodecElement)
			{
				pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsBitRate = videoCodecElement->Attribute("BitRate");
				pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsHeight = videoCodecElement->Attribute("Height");
				pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsID = videoCodecElement->Attribute("ID");
				pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsMaxFrameRate = videoCodecElement->Attribute("MaxFrameRate");
//				pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsVariableFrameRate = videoCodecElement->Attribute("VariableFrameRate");	// not used
				pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsWidth = videoCodecElement->Attribute("Width");
			}
		}

		//add list
		KString clsDebug;
		pclsTargetContent->m_fnDebug(clsDebug);
		IFLOG(E_LOG_DEBUG, (KCSTR)clsDebug);
		_pclsTargetList->m_fnPushBack(pclsTargetContent);
		//next
		targetContentElement = targetContentElement->NextSiblingElement("TargetContent");
	}
	return unTargetContent;
}
void AppXmlParser::m_fnMakeLinkTestReq(KString &_rclsFrom, KString &_rclsTo, KString & _rclsSessionID, int _rclsTransactionID, KString & _rclsLinkTestReq)
{
/*
 <?xml version="1.0"?>
 <WTRS.MSG Version="1.0">
     <HEADER From="urn:wtrsp:auth.endpoint:MMSC_ACPT" SessionID="{2cb8e94e-8100-405f-b3ad-c8ce07fd0837}" To="urn:wtrsp:system:wtrsg" TransactionID="14618">
         <Error Code="" Description="" Source="" />
     </HEADER>

     <BODY>
         <LinkTestRequest />
     </BODY>
 </WTRS.MSG>

 <?xml version="1.0"?>
 <WTRS.MSG Version="1.0">
     <HEADER From="urn:wtrsp:system:wtrsg" SessionID="{2cb8e94e-8100-405f-b3ad-c8ce07fd0837}" To="urn:wtrsp:auth.endpoint:MMSC_ACPT" TransactionID="14618">
         <Error Code="0000" Description="OK" Source="" />
     </HEADER>

     <BODY>
         <LinkTestResponse />
     </BODY>
 </WTRS.MSG>
 */
	KString clsSessionID = _rclsSessionID;

	KString::m_fnStrnCat((KSTR) _rclsLinkTestReq, 10240, "<?xml version=\"1.0\"?>"
			"<WTRS.MSG Version=\"1.0\">"
			"<HEADER From=\"%s\" SessionID=\"%s\" To=\"%s\" TransactionID=\"%d\">"
			"<Error Code=\"\" Description=\"\" Source=\"\" />"
			"</HEADER>"
			"<BODY>"
			"<LinkTestRequest />"
			"</BODY>"
			"</WTRS.MSG>", (KSTR)_rclsFrom, (KSTR) clsSessionID, (KSTR)_rclsTo, _rclsTransactionID);
}
void AppXmlParser::m_fnMakeEstablishSessionReq(KString &_rclsFrom, KString &_rclsTo, int _nTransactionID, KString & _rclsEstablishSessionReq)
{
/*
<?xml version="1.0"?>
<WTRS.MSG Version="1.0">
	<HEADER From="urn:wtrsp:auth.endpoint:mcc" SessionID="" To="urn:wtrsp:system:wtrsg" TransactionID="603">
		<Error Code="" Description="" Source=""/>
	</HEADER>
	<BODY>
		<EstablishSessionRequest>
			<Endpoint ID="urn:wtrsp:auth.endpoint:mcc" IP="211.60.220.142" PWD="mccmcc"/>
		</EstablishSessionRequest>
	</BODY>
</WTRS.MSG>

<?xml version="1.0"?>
<WTRS.MSG Version="1.0">
	<HEADER From="urn:wtrsp:system:wtrsg" SessionID="{f0bc6c2d-0454-430e-a14b-3759ce33fb16}" To="urn:wtrsp:auth.endpoint:mcc" TransactionID="603">
		<Error Code="0000" Description="OK" Source="" />
	</HEADER>
	<BODY>
		<EstablishSessionResponse>
			<WTRS.CONFIG>
				<CONFIG_SYSTEM_CLIENT LinkTestInterval="30" LinkTestTimeout="100" LinkTestTimeoutCriticalCount="3" MaxTCPContentSizeKB="2048" MaxTagImageSizeKB="2048" RetryConnectionInterval="3" SystemID="VWTRSG01" SystemIndex="1" TransactionTimeout="0" MaxConnectionRetries="0" />
			</WTRS.CONFIG>
		</EstablishSessionResponse>
	</BODY>
</WTRS.MSG>
*/
   KString clsEndpointID;
   KString clsEndpointIP;
   KString clsEndpointPW;

   KString::m_fnStrnCat((KSTR)_rclsEstablishSessionReq,10240,
		"<?xml version=\"1.0\"?>"
		"<WTRS.MSG Version=\"1.0\">"
    		"<HEADER From=\"%s\" SessionID=\"\" To=\"%s\" TransactionID=\"%d\">"
    			"<Error Code=\"\" Description=\"\" Source=\"\" />"
    		"</HEADER>"
    		"<BODY>"
            "<EstablishSessionRequest>"
               "<Endpoint ID=\"\" IP=\"\" PWD=\"\"/>"
            "</EstablishSessionRequest>"
    		"</BODY>"
		"</WTRS.MSG>", (KSTR)_rclsFrom, (KSTR)_rclsTo, _nTransactionID/*,(KSTR)clsEndpointID, (KSTR)clsEndpointIP, (KSTR) clsEndpointPW*/);
}
void AppXmlParser::m_fnMakeJobStatusChangedNotify_Created(KString &_rclsXml, KString &_rclsJobStatusChangedNotify)
{
   // _rclsXml == CrtJobReq
   TiXmlDocument doc;
   doc.Parse((KCSTR) _rclsXml);
   TiXmlElement* rootElement = doc.RootElement();
   TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");
   TiXmlElement* bodyElement =  rootElement->FirstChildElement("BODY");
   TiXmlElement* jobElement =  bodyElement->FirstChildElement("CreateJobRequest")->FirstChildElement("Job");

   KString clsTo;
   KString clsSessionID;
   KString clsFrom;
   KString clsTransactionID;
   KString clsJobID;
   KString clsServiceName;
   KString clsCurrTime; m_fnGetCurrentTime(clsCurrTime);

   clsTo = headerElement->Attribute("To");
   clsSessionID = headerElement->Attribute("SessionID");
   clsFrom = headerElement->Attribute("From");
   clsTransactionID = headerElement->Attribute("TransactionID");
   clsJobID = jobElement->Attribute("ID");
   clsServiceName = jobElement->Attribute("ServiceName");

   // To, From swap
   KString::m_fnStrnCat((KSTR)_rclsJobStatusChangedNotify,10240,
   "<?xml version=\"1.0\"?>"
   "<WTRS.MSG Version=\"1.0\">"
      "<HEADER From=\"%s\" SessionID=\"%s\" To=\"%s\" TransactionID=\"%s\">"
           "<Error Code=\"0000\" Description=\"\" Source=\"\" />"
      "</HEADER>"
      "<BODY>"
           "<JobStatusChangedNotify>"
               "<JobState ID=\"%s\" Kind=\"Realtime\" Status=\"Created\" ServiceName=\"%s\" StartTime=\"%s\" EndTime=\"\" ResultCode=\"0000\" />"
           "</JobStatusChangedNotify>"
      "</BODY>"
   "</WTRS.MSG>", (KSTR)clsTo, (KSTR)clsSessionID, (KSTR)clsFrom, (KSTR)clsTransactionID, (KSTR)clsJobID, (KSTR)clsServiceName, (KSTR)clsCurrTime);
}

void AppXmlParser::m_fnMakeJobStatusChangedNotify_Waiting(KString &_rclsXml, KString &_rclsJobStatusChangedNotify)
{
   TiXmlDocument doc;
   doc.Parse((KCSTR) _rclsXml);
   TiXmlElement* rootElement = doc.RootElement();
   TiXmlElement* bodyElement =  rootElement->FirstChildElement("BODY");
   TiXmlElement* jobStatusChangedNotifyElement = bodyElement->FirstChildElement("JobStatusChangedNotify");

   jobStatusChangedNotifyElement->FirstChildElement("JobState")->SetAttribute("Status", "Waiting");
   TiXmlPrinter printer;
   printer.SetStreamPrinting();
   doc.Accept(&printer);
   _rclsJobStatusChangedNotify = printer.CStr();

}

void AppXmlParser::m_fnMakeJobStatusChangedNotify_JobStarted(KString &_rclsXml, unsigned int _unTranscodingList, KString &_rclsJobStatusChangedNotify)
{
   TiXmlDocument doc;
   doc.Parse((KCSTR) _rclsXml);
   TiXmlElement* rootElement = doc.RootElement();
   TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");
   TiXmlElement* bodyElement =  rootElement->FirstChildElement("BODY");
   TiXmlElement* jobStatusChangedNotifyElement = bodyElement->FirstChildElement("JobStatusChangedNotify");

   KString clsTo;
   KString clsSessionID;
   KString clsFrom;
   KString clsTransactionID;
   KString clsJobID;
   KString clsServiceName;
   KString clsStartTime;

   clsTo = headerElement->Attribute("To");
   clsSessionID = headerElement->Attribute("SessionID");
   clsFrom = headerElement->Attribute("From");
   clsTransactionID = headerElement->Attribute("TransactionID");
   if(jobStatusChangedNotifyElement == NULL) clsJobID = "UNKNOWN";
   else clsJobID = jobStatusChangedNotifyElement->FirstChildElement("JobState")->Attribute("ID");
   clsServiceName = jobStatusChangedNotifyElement->FirstChildElement("JobState")->Attribute("ServiceName");
   clsStartTime = jobStatusChangedNotifyElement->FirstChildElement("JobState")->Attribute("StartTime");

   KString::m_fnStrnCat((KSTR)_rclsJobStatusChangedNotify,10240,
   "<?xml version=\"1.0\"?>"
   "<WTRS.MSG Version=\"1.0\">"
      "<HEADER From=\"%s\" SessionID=\"%s\" To=\"%s\" TransactionID=\"%s\">"
           "<Error Code=\"0000\" Description=\"\" Source=\"\" />"
      "</HEADER>"
      "<BODY>"
           "<JobStatusChangedNotify>"
               "<JobState ID=\"%s\" Kind=\"Realtime\" Status=\"JobStarted\" ServiceName=\"%s\" StartTime=\"%s\" EndTime=\"\" ResultCode=\"0000\" >"
                  "<Transcodes Cancel=\"0\" Current=\"0\" Fail=\"0\" Success=\"0\" Total=\"%d\" />"
               "</JobState>"
           "</JobStatusChangedNotify>"
      "</BODY>"
   "</WTRS.MSG>", (KSTR)clsFrom, (KSTR)clsSessionID, (KSTR)clsTo, (KSTR)clsTransactionID, (KSTR)clsJobID, (KSTR)clsServiceName, (KSTR)clsStartTime, _unTranscodingList);
}

void AppXmlParser::m_fnMakeJobStatusChangedNotify_TC_Started(KString & _rclsXml,
		SourceContent * _pclsSource, TargetContent * _pclsTarget, unsigned int _unFail, unsigned int _unSuccess, unsigned int _unTargetIdx, KString & _rclsJobStatusChangedNotify)
{
   TiXmlDocument doc;
   doc.Parse((KCSTR) _rclsXml);		//_rclsXml : JobStarted
   TiXmlElement* rootElement = doc.RootElement();
   TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");
   TiXmlElement* bodyElement =  rootElement->FirstChildElement("BODY");
   TiXmlElement* jobStatusChangedNotifyElement = bodyElement->FirstChildElement("JobStatusChangedNotify");
   TiXmlElement* jobStateElement = jobStatusChangedNotifyElement->FirstChildElement("JobState");

   KString clsTo;
   KString clsSessionID;
   KString clsFrom;
   KString clsTransactionID;
   KString clsJobID;
   KString clsServiceName;
   KString clsStartTime;
   KString clsTransSessionID;
   KString clsTotal;
   KString clsCurrTime; m_fnGetCurrentTime(clsCurrTime);

   clsTo = headerElement->Attribute("To");
   clsSessionID = headerElement->Attribute("SessionID");
   clsFrom = headerElement->Attribute("From");
   clsTransactionID = headerElement->Attribute("TransactionID");
   if(jobStatusChangedNotifyElement == NULL) clsJobID = "UNKNOWN";
   else clsJobID = jobStatusChangedNotifyElement->FirstChildElement("JobState")->Attribute("ID");
   clsServiceName = jobStatusChangedNotifyElement->FirstChildElement("JobState")->Attribute("ServiceName");
   clsStartTime = jobStatusChangedNotifyElement->FirstChildElement("JobState")->Attribute("StartTime");
   KString::m_fnStrnCat((KSTR)clsTransSessionID,10240,"%s_%d",(KSTR)clsJobID, _unTargetIdx);
   clsTotal = jobStateElement->FirstChildElement("Transcodes")->Attribute("Total");

   KString::m_fnStrnCat((KSTR)_rclsJobStatusChangedNotify,10240,
   "<?xml version=\"1.0\"?>"
   "<WTRS.MSG Version=\"1.0\">"
      "<HEADER From=\"%s\" SessionID=\"%s\" To=\"%s\" TransactionID=\"%s\">"
           "<Error Code=\"0000\" Description=\"\" Source=\"\" />"
      "</HEADER>"
      "<BODY>"
           "<JobStatusChangedNotify>"
               "<JobState ID=\"%s\" Kind=\"Realtime\" Status=\"TranscodingStarted\" ServiceName=\"%s\" StartTime=\"%s\" EndTime=\"\" ResultCode=\"0000\" >"
                  "<Transcodes StartTime=\"%s\" EndTime=\"%s\" TransSessionID=\"%s\" Cancel=\"0\" Current=\"%d\" Fail=\"%d\" Success=\"%d\" Total=\"%s\" ResultCode=\"0000\" ResultDescription=\"0\" >"
                     "<Transcode>"
                        "<Source NASCode=\"%s\" File=\"%s\" WTRSSPath=\"%s\" BinaryData=\"%s\">"
                        "<Container ID=\"%s\" />"
                        "</Source>"
                        "<Target NASCode=\"%s\" File=\"%s\" WTRSSPath=\"%s\" BinaryData=\"%s\" Size=\"%s\">"
                            "<Container ID=\"%s\">"
                                "<VideoCodec Width=\"%s\" Height=\"%s\" MaxFrameRate=\"%s\" BitRate=\"%s\" VariableFrameRate=\"\" ID=\"%s\" />"
                                "<AudioCodec Channel=\"%s\" SampleRate=\"%s\" BitRate=\"%s\" BitPerSample=\"%s\" ID=\"%s\" />"
                            "</Container>"
                        "</Target>"
                    "</Transcode>"
                  "</Transcodes>"
               "</JobState>"
           "</JobStatusChangedNotify>"
      "</BODY>"
   "</WTRS.MSG>", (KSTR)clsFrom, (KSTR)clsSessionID, (KSTR)clsTo, (KSTR)clsTransactionID,
   (KSTR)clsJobID, (KSTR)clsServiceName, (KSTR)clsStartTime,
   (KSTR)clsStartTime, (KSTR)clsCurrTime, (KSTR)clsTransSessionID, _unTargetIdx, _unFail, _unSuccess, (KSTR)clsTotal,
   (KSTR)_pclsSource->m_clsNasCode, (KSTR)_pclsSource->m_clsFilePath, (KSTR)_pclsSource->m_clsFilePath, (KSTR)_pclsSource->m_clsBinaryData, (KSTR)_pclsSource->m_clsID,
   (KSTR)_pclsTarget->m_clsNasCode, (KSTR)_pclsTarget->m_clsFilePath, (KSTR)_pclsTarget->m_clsFilePath, (KSTR)_pclsTarget->m_clsBinaryData, (KSTR)_pclsTarget->m_clsSize,
   (KSTR)_pclsTarget->m_clsContainer.m_clsID,
   (KSTR)_pclsTarget->m_clsContainer.clsVideoCodec.m_clsWidth, (KSTR)_pclsTarget->m_clsContainer.clsVideoCodec.m_clsHeight, (KSTR)_pclsTarget->m_clsContainer.clsVideoCodec.m_clsMaxFrameRate, (KSTR)_pclsTarget->m_clsContainer.clsVideoCodec.m_clsBitRate, (KSTR)_pclsTarget->m_clsContainer.clsVideoCodec.m_clsID,
   (KSTR)_pclsTarget->m_clsContainer.clsAudioCodec.m_clsChannel, (KSTR)_pclsTarget->m_clsContainer.clsAudioCodec.m_clsSampleRate, (KSTR)_pclsTarget->m_clsContainer.clsAudioCodec.m_clsBitRate, (KSTR)_pclsTarget->m_clsContainer.clsAudioCodec.m_clsBitPerSample, (KSTR)_pclsTarget->m_clsContainer.clsAudioCodec.m_clsID);

   resip::Data convertNullStr = (KCSTR)_rclsJobStatusChangedNotify;
   convertNullStr.replace("(null)", "");
   _rclsJobStatusChangedNotify = convertNullStr.c_str();

}

void AppXmlParser::m_fnMakeJobStatusChangedNotify_TC_Stopped(KString &_rclsXml, ETrssCodeSet_t _eSt, KString &_rclsResultDescription,
		unsigned int _unFail, unsigned int _unSuccess, KString &_rclsJobStatusChangedNotify)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR) _rclsXml);	// _rclsXml : TCStarted
	TiXmlElement *rootElement = doc.RootElement();
	TiXmlElement *bodyElement = rootElement->FirstChildElement("BODY");
	TiXmlElement *jobStatusChangedNotifyElement = bodyElement->FirstChildElement("JobStatusChangedNotify");
	TiXmlElement *jobStateElement = jobStatusChangedNotifyElement->FirstChildElement("JobState");
	TiXmlElement *transcodesElement = jobStateElement->FirstChildElement("Transcodes");

	jobStateElement->SetAttribute("Status", "TranscodingStopped");
	KString clsCode = g_fnGetTrssCode(_eSt);
	jobStateElement->SetAttribute("ResultCode", (KCSTR)clsCode);
	KString clsCurrTime; m_fnGetCurrentTime(clsCurrTime);
	transcodesElement->SetAttribute("EndTime", (KSTR)clsCurrTime);
	transcodesElement->SetAttribute("Fail", _unFail);
	transcodesElement->SetAttribute("Success", _unSuccess);
	transcodesElement->SetAttribute("ResultCode", (KCSTR)clsCode);
	transcodesElement->SetAttribute("ResultDescription", (KUINT)_rclsResultDescription);

	if(_eSt != E_TC_RES_OK)
	{
		TiXmlElement *targetElement = transcodesElement->FirstChildElement("Transcode")->FirstChildElement("Target");
		targetElement->SetAttribute("BinaryData", "");
	}

	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	doc.Accept(&printer);
	_rclsJobStatusChangedNotify = printer.CStr();

}

void AppXmlParser::m_fnMakeJobStatusChangedNotify_JobStopped(KString &_rclsXml, KString &_rclsJobStatusChangedNotify)
{
   TiXmlDocument doc;
   doc.Parse((KCSTR) _rclsXml);
   TiXmlElement* rootElement = doc.RootElement();
   TiXmlElement* headerElement = rootElement->FirstChildElement("HEADER");
   TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");
   TiXmlElement* jobStatusChangedNotifyElement = bodyElement->FirstChildElement("JobStatusChangedNotify");
   TiXmlElement* jobStateElement = jobStatusChangedNotifyElement->FirstChildElement("JobState");

   KString clsTo;
   KString clsSessionID;
   KString clsFrom;
   KString clsTransactionID;
   KString clsJobID;
   KString clsServiceName;
   KString clsStartTime;
   KString clsCurrTime; m_fnGetCurrentTime(clsCurrTime);

   KString clsFail;
   KString clsSuccess;
   KString clsTotal;

   clsTo = headerElement->Attribute("To");
   clsSessionID = headerElement->Attribute("SessionID");
   clsFrom = headerElement->Attribute("From");
   clsTransactionID = headerElement->Attribute("TransactionID");
   if(jobStatusChangedNotifyElement == NULL) clsJobID = "UNKNOWN";
   else clsJobID = jobStatusChangedNotifyElement->FirstChildElement("JobState")->Attribute("ID");
   clsServiceName = jobStatusChangedNotifyElement->FirstChildElement("JobState")->Attribute("ServiceName");
   clsStartTime = jobStatusChangedNotifyElement->FirstChildElement("JobState")->Attribute("StartTime");

   clsFail = jobStateElement->FirstChildElement("Transcodes")->Attribute("Fail");
   clsSuccess = jobStateElement->FirstChildElement("Transcodes")->Attribute("Success");
   clsTotal = jobStateElement->FirstChildElement("Transcodes")->Attribute("Total");

   KString::m_fnStrnCat((KSTR)_rclsJobStatusChangedNotify,10240,
   "<?xml version=\"1.0\"?>"
   "<WTRS.MSG Version=\"1.0\">"
      "<HEADER From=\"%s\" SessionID=\"%s\" To=\"%s\" TransactionID=\"%s\">"
           "<Error Code=\"0000\" Description=\"\" Source=\"\" />"
      "</HEADER>"
      "<BODY>"
           "<JobStatusChangedNotify>"
               "<JobState ID=\"%s\" Kind=\"Realtime\" Status=\"JobStopped\" ServiceName=\"%s\" StartTime=\"%s\" EndTime=\"%s\" ResultCode=\"0000\" >"
                  "<Transcodes Cancel=\"0\" Current=\"0\" Fail=\"%s\" Success=\"%s\" Total=\"%s\" />"
               "</JobState>"
           "</JobStatusChangedNotify>"
      "</BODY>"
   "</WTRS.MSG>", (KSTR)clsFrom, (KSTR)clsSessionID, (KSTR)clsTo, (KSTR)clsTransactionID,
   (KSTR)clsJobID, (KSTR)clsServiceName, (KSTR)clsStartTime, (KSTR)clsCurrTime,
   (KSTR)clsFail, (KSTR)clsSuccess, (KSTR)clsTotal);
}

void AppXmlParser::m_fnMakeJobStatusChangedNotify_Destroyed(KString &_rclsXml, unsigned int _unTranscodingList, ETrssCodeSet_t _eSt, KString &_rclsJobStatusChangedNotify)
{
	// CASE 1. JobStopped
	// CASE 2. 나머지MSG (SessionFull) /Session Timeout

	KString clsBodyType;
	AppXmlParser::m_fnGetBodyType(_rclsXml, clsBodyType);
	IFLOG(E_LOG_DEBUG, "Before Destroyed Msg's BodyType:%s", (KSTR)clsBodyType);

	TiXmlDocument doc;
	doc.Parse((KCSTR) _rclsXml);
	TiXmlElement *rootElement = doc.RootElement();
	TiXmlElement *headerElement = rootElement->FirstChildElement("HEADER");
	TiXmlElement *bodyElement = rootElement->FirstChildElement("BODY");

	if (clsBodyType == DEF_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED)
	{
		TiXmlElement *jobStatusChangedNotifyElement = bodyElement->FirstChildElement("JobStatusChangedNotify");
		jobStatusChangedNotifyElement->FirstChildElement("JobState")->SetAttribute("Status", "Destroyed");

		TiXmlPrinter printer;
		printer.SetStreamPrinting();
		doc.Accept(&printer);
		_rclsJobStatusChangedNotify = printer.CStr();
	}
	else
	{
		TiXmlElement *jobElement;
		if(clsBodyType == DEF_BODY_CREATE_JOB_REQ)
			jobElement = bodyElement->FirstChildElement("CreateJobRequest")->FirstChildElement("Job");
		else
			jobElement = bodyElement->FirstChildElement("JobStatusChangedNotify")->FirstChildElement("JobState");
		KString clsTo;
		KString clsSessionID;
		KString clsFrom;
		KString clsTransactionID;
		KString clsJobID;
		KString clsServiceName;
		KString clsCurrTime;
		m_fnGetCurrentTime(clsCurrTime);

		clsTo = headerElement->Attribute("To");
		clsSessionID = headerElement->Attribute("SessionID");
		clsFrom = headerElement->Attribute("From");
		clsTransactionID = headerElement->Attribute("TransactionID");
		clsJobID = jobElement->Attribute("ID");
		clsServiceName = jobElement->Attribute("ServiceName");

		KString clsCode = g_fnGetTrssCode(_eSt);
		KString clsDescription = g_fnGetTrssCodeDesc((KCSTR)clsCode);
		IFLOG(E_LOG_DEBUG, "Destroyed Reason:%s", (KCSTR)clsCode);

		// To, From swap
		KString::m_fnStrnCat((KSTR) _rclsJobStatusChangedNotify, 10240,
		"<?xml version=\"1.0\"?>"
		"<WTRS.MSG Version=\"1.0\">"
			"<HEADER From=\"%s\" SessionID=\"%s\" To=\"%s\" TransactionID=\"%s\">"
				"<Error Code=\"%s\" Description=\"%s\" Source=\"\" />"
			"</HEADER>"
			"<BODY>"
				"<JobStatusChangedNotify>"
					"<JobState ID=\"%s\" Kind=\"Realtime\" Status=\"Destroyed\" ServiceName=\"%s\" StartTime=\"%s\" EndTime=\"%s\" ResultCode=\"%s\">"
						"<Transcodes Cancel=\"0\" Current=\"0\" Fail=\"%d\" Success=\"0\" Total=\"%d\"/>"
					"</JobState>"
				"</JobStatusChangedNotify>"
			"</BODY>"
		"</WTRS.MSG>", (KSTR) clsTo, (KSTR) clsSessionID, (KSTR) clsFrom, (KSTR) clsTransactionID,
		(KCSTR) clsCode, (KCSTR) clsDescription,
		(KSTR) clsJobID, (KSTR) clsServiceName, (KSTR) clsCurrTime, (KSTR) clsCurrTime, (KCSTR) clsCode,
		_unTranscodingList, _unTranscodingList);
	}
}
void AppXmlParser::m_fnGetCurrentTime(KString & _rclsCurrent)
{
   struct timespec now_monotonic;
   clock_gettime(CLOCK_REALTIME, &now_monotonic);

   time_t stTime;
   struct tm tmTime;
   stTime  = time(NULL);
   localtime_r(&stTime,&tmTime);

   //ex) 2022051214173122526123  //Len:22
   //parse) 2022-05-12 14:17:31.22526123
   KString::m_fnStrnCat((KSTR)_rclsCurrent,_rclsCurrent.m_unLen,"%04d-%02d-%02d %02d:%02d:%02d", tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
         tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
}
