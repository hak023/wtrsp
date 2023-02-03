#include <fstream>
#include "AppXmlParser.h"
#include "main.h"
#include "CBase64.h"
#include "MainConfig.h"
#include "AppIf.h"
#include "TargetContent.h"
#include "Session.h"

using namespace eSipUtil;

AppXmlParser::AppXmlParser(){}
AppXmlParser::~AppXmlParser(){}
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
bool AppXmlParser::m_fnValid(KString & _rclsXml)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
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


	CmpString clsRootEleName = rootElement->Value();
	if((clsRootEleName == "WTRS.MSG") == false)
	{
		IFLOG(E_LOG_ERR,"[AppXmlParser]Xml Format error[rootElement Invalid:%s]", (KCSTR)clsRootEleName);
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
	IFLOG(E_LOG_DEBUG,"[AppXmlParser]Xml Valid OK");
	return true;
}
ETrsgCodeSet_t AppXmlParser::m_fnCreateJobValid(KString & _rclsXml)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyTypeElement = rootElement->FirstChildElement("BODY");
	TiXmlElement* jobElement = bodyTypeElement->FirstChildElement(
				"CreateJobRequest")->FirstChildElement("Job");

	TiXmlElement* sourceContentMapElement = jobElement->FirstChildElement("SourceContentMap");
	if(sourceContentMapElement == NULL) return E_SOURCE_CONTENT_ERR;

	TiXmlElement* sourceContentElement = sourceContentMapElement->FirstChildElement("SourceContent");
	if(sourceContentElement == NULL) return E_SOURCE_CONTENT_ERR;

	TiXmlElement* targetContentMapElement = jobElement->FirstChildElement("TargetContentMap");
	if(targetContentMapElement == NULL) return E_TARGET_CONTENT_ERR;

	TiXmlElement* targetContentElement = targetContentMapElement->FirstChildElement("TargetContent");
	if(targetContentElement == NULL) return E_TARGET_CONTENT_ERR;

	TiXmlElement* transcodingListElement = jobElement->FirstChildElement("TranscodingList");
	if(transcodingListElement == NULL) return E_TRANSCODING_ERR;

	TiXmlElement* transcodingElement = transcodingListElement->FirstChildElement("Transcoding");
	if(transcodingElement == NULL) return E_TRANSCODING_ERR;

	return E_JOB_RES_OK;
}
bool AppXmlParser::m_fnGetBodyType(KString & _rclsXml, KString & _rclsBodyType)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
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
			//JobState attr JobState.
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
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
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
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");
	_rclsSessionID = headerElement->Attribute("SessionID");
}
void AppXmlParser::m_fnGetJobID(KString & _rclsXml, KString & _rclsJobID)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyElement =  rootElement->FirstChildElement("BODY");
	TiXmlElement* jobStatusChangedNotifyElement = bodyElement->FirstChildElement("JobStatusChangedNotify");

	if(jobStatusChangedNotifyElement == NULL) _rclsJobID = "UNKNOWN";
	else _rclsJobID = jobStatusChangedNotifyElement->FirstChildElement("JobState")->Attribute("ID");
}
void AppXmlParser::m_fnGetToFrom(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	_rclsFrom = headerElement->Attribute("From");
	_rclsTo = headerElement->Attribute("To");
}
void AppXmlParser::m_fnGetAuthInfo(KString & _rclsXml, KString & _rclsID, KString & _rclsPWD, KString & _rclsIP)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyElement =  rootElement->FirstChildElement("BODY");
	if(bodyElement == NULL) return;
	TiXmlElement* establishSessionRequestElement = bodyElement->FirstChildElement("EstablishSessionRequest");
	if(establishSessionRequestElement ==  NULL) return;
	TiXmlElement* endpointElement = establishSessionRequestElement->FirstChildElement("Endpoint");
	if(endpointElement ==  NULL) return;

	_rclsID = endpointElement->Attribute("ID");
	_rclsPWD = endpointElement->Attribute("PWD");
	_rclsIP = endpointElement->Attribute("IP");
}
void AppXmlParser::m_fnGetNasCode(KString & _rclsXml, int* _pnSrcNasCode, int* _pnDstNasCode)
{
	KString clsBodyType;
	if(AppXmlParser::m_fnGetBodyType(_rclsXml, clsBodyType))
	{
		TiXmlDocument doc;
		doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
		TiXmlElement* rootElement = doc.RootElement();;
		TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");

		if(clsBodyType == DEF_BODY_CREATE_JOB_REQ)//Target이 여러개라도 첫 번째 TargetContent 의 NASCode만 _pnDstNasCode에 저장됨에 유의.
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
void AppXmlParser::m_fnGetSourceContentInfo(KString & _rclsXml, KString & _rclsPath, KString & _rclsFile, KString & _rclsContainer)
{
/*
<?xml version="1.0"?>
<WTRS.MSG Version="1.0">
    <HEADER From="urn:wtrsp:auth.endpoint:mcc" SessionID="{c2f618bd-aac5-4a96-b1f9-c857445e0990}" To="urn:wtrsp:system:wtrsg" TransactionID="64">
        <Error Code="" Description="" Source="" />
    </HEADER>

    <BODY>
        <CreateJobRequest>
            <Job Kind="Realtime" ScheduledTime="" ServiceName="62712003|15320699|">
                <SourceContentMap>
                    <SourceContent File="82473327.jpg" ID="1" NASCode="27" Path="album/images/605/8247/3327/">
                        <Container ID="JPEG" />
                    </SourceContent>
                </SourceContentMap>
                <TargetContentMap>
                    <TargetContent File="82473327.jpg" ID="602" NASCode="27" Path="album/images/602/8247/3327/" Size="40KB">
                        <Container ID="JPEG">
                            <AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate="" />
                            <VideoCodec BitRate="64000" Height="240" ID="JPEG" MaxFrameRate="" VariableFrameRate="" Width="240" />
                        </Container>
                    </TargetContent>
                    <TargetContent File="82473327.jpg" ID="604" NASCode="27" Path="album/images/604/8247/3327/" Size="40KB">
                        <Container ID="JPEG">
                            <AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate="" />
                            <VideoCodec BitRate="64000" Height="240" ID="JPEG" MaxFrameRate="" VariableFrameRate="" Width="240" />
                        </Container>
                    </TargetContent>
                    <TargetContent File="82473327.jpg" ID="620" NASCode="27" Path="album/images/620/8247/3327/" Size="40KB">
                        <Container ID="JPEG">
                            <AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate="" />
                            <VideoCodec BitRate="64000" Height="120" ID="JPEG" MaxFrameRate="" VariableFrameRate="" Width="120" />
                        </Container>
                    </TargetContent>
                    <TargetContent File="82473327.jpg" ID="621" NASCode="27" Path="album/images/621/8247/3327/" Size="40KB">
                        <Container ID="JPEG">
                            <AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate="" />
                            <VideoCodec BitRate="64000" Height="90" ID="JPEG" MaxFrameRate="" VariableFrameRate="" Width="90" />
                        </Container>
                    </TargetContent>
                    <TargetContent File="82473327.jpg" ID="622" NASCode="27" Path="album/images/622/8247/3327/" Size="40KB">
                        <Container ID="JPEG">
                            <AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate="" />
                            <VideoCodec BitRate="64000" Height="50" ID="JPEG" MaxFrameRate="" VariableFrameRate="" Width="50" />
                        </Container>
                    </TargetContent>
                    <TargetContent File="82473327.jpg" ID="623" NASCode="27" Path="album/images/623/8247/3327/" Size="40KB">
                        <Container ID="JPEG">
                            <AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate="" />
                            <VideoCodec BitRate="64000" Height="25" ID="JPEG" MaxFrameRate="" VariableFrameRate="" Width="25" />
                        </Container>
                    </TargetContent>
                    <TargetContent File="82473327.jpg" ID="629" NASCode="27" Path="album/images/629/8247/3327/" Size="40KB">
                        <Container ID="JPEG">
                            <AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate="" />
                            <VideoCodec BitRate="64000" Height="600" ID="JPEG" MaxFrameRate="" VariableFrameRate="" Width="600" />
                        </Container>
                    </TargetContent>
                </TargetContentMap>
                <TranscodingList>
                    <Transcoding SourceContentID="1" TargetContentID="602" />
                    <Transcoding SourceContentID="1" TargetContentID="604" />
                    <Transcoding SourceContentID="1" TargetContentID="620" />
                    <Transcoding SourceContentID="1" TargetContentID="621" />
                    <Transcoding SourceContentID="1" TargetContentID="622" />
                    <Transcoding SourceContentID="1" TargetContentID="623" />
                    <Transcoding SourceContentID="1" TargetContentID="629" />
                </TranscodingList>
            </Job>
        </CreateJobRequest>
    </BODY>
</WTRS.MSG>
 */

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();;
	TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");
	TiXmlElement* jobElement = bodyElement->FirstChildElement(
			"CreateJobRequest")->FirstChildElement("Job");


	TiXmlElement* sourceContentElement = jobElement->FirstChildElement("SourceContentMap")->
			FirstChildElement("SourceContent");

	if(sourceContentElement->Attribute("Path"))
	{
		_rclsPath = sourceContentElement->Attribute("Path");
		if(_rclsPath.m_unRealLen > 0 ) IFLOG(E_LOG_INFO, "[AppXmlParser] Src Path[%s]", (KCSTR)_rclsPath);
	}
	if(sourceContentElement->Attribute("File"))
	{
		_rclsFile = sourceContentElement->Attribute("File");
		if(_rclsFile.m_unRealLen > 0 ) IFLOG(E_LOG_INFO, "[AppXmlParser] Src File[%s]", (KCSTR)_rclsFile);
	}

	TiXmlElement* containerElement = sourceContentElement->FirstChildElement("Container");
	if(containerElement->Attribute("ID"))
	{
		_rclsContainer = containerElement->Attribute("ID");
	}

}
unsigned int AppXmlParser::m_fnGetTranscodingList(KString & _rclsXml)
{
	/*
<?xml version="1.0"?>
<WTRS.MSG Version="1.0">
    <HEADER From="urn:wtrsp:auth.endpoint:tts" SessionID="{a4f02508-c789-4c47-baf9-0206492bff3d}" To="urn:wtrsp:system:wtrsg" TransactionID="1882">
        <Error Code="" Description="" Source="" />
    </HEADER>

    <BODY>
        <CreateJobRequest>
            <Job Kind="Realtime" ScheduledTime="" ServiceName="FEELRING-TTS29563428">
                <SourceContentMap>
                    <SourceContent BinaryData="\\172.17.107.5\nas_wtrsp\temp\2022012705564457280387_decode.tmp" File="" ID="01" NASCode="" Path="">
                        <Container ID="WAV">
                            <Metadata Album="" Artist="" Category="" Comment="" Copyright="" Date="" Genre="" Grade="" ImageData="" ImageType="" Lyric="" PlayCounter="" PlayDevice="" PlayTime="" Popularimeter="" Publisher="" Site="" Title="" Track="" ValidPeriod="" Year="" />
                            <AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate="" />
                            <VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width="" />
                        </Container>
                    </SourceContent>
                </SourceContentMap>
                <TargetContentMap>
                    <TargetContent BinaryData="" File="" ID="1" NASCode="" Path="" Size="">
                        <Container ID="WMA">
                            <AudioCodec BitPerSample="16" BitRate="32000" Channel="1" ID="WMA2" SampleRate="44100" />
                            <VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width="" />
                        </Container>
                    </TargetContent>
                    <TargetContent BinaryData="" File="" ID="3" NASCode="" Path="" Size="">
                        <Container ID="VOX">
                            <AudioCodec BitPerSample="4" BitRate="0" Channel="1" ID="VOX" SampleRate="8000" />
                            <VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width="" />
                        </Container>
                    </TargetContent>
                    <TargetContent BinaryData="" File="" ID="4" NASCode="" Path="" Size="">
                        <Container ID="AMR">
                            <AudioCodec BitPerSample="16" BitRate="23850" Channel="1" ID="AMRWB" SampleRate="16000" />
                            <VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width="" />
                        </Container>
                    </TargetContent>
                    <TargetContent BinaryData="" File="" ID="5" NASCode="" Path="" Size="">
                        <Container ID="AMR">
                            <AudioCodec BitPerSample="16" BitRate="12200" Channel="1" ID="AMR" SampleRate="8000" />
                            <VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width="" />
                        </Container>
                    </TargetContent>
                    <TargetContent BinaryData="" File="" ID="6" NASCode="" Path="" Size="">
                        <Container ID="G711">
                            <AudioCodec BitPerSample="16" BitRate="64000" Channel="1" ID="G711" SampleRate="8000" />
                            <VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width="" />
                        </Container>
                    </TargetContent>
                    <TargetContent BinaryData="" File="" ID="7" NASCode="" Path="" Size="40">
                        <Container ID="MMF">
                            <AudioCodec BitPerSample="16" BitRate="32000" Channel="1" ID="MMF" SampleRate="8000" />
                            <VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width="" />
                        </Container>
                    </TargetContent>
                </TargetContentMap>
                <TranscodingList>  <--몇개인지 리턴.
                    <Transcoding SourceContentID="01" TargetContentID="1" />
                    <Transcoding SourceContentID="01" TargetContentID="3" />
                    <Transcoding SourceContentID="01" TargetContentID="4" />
                    <Transcoding SourceContentID="01" TargetContentID="5" />
                    <Transcoding SourceContentID="01" TargetContentID="6" />
                    <Transcoding SourceContentID="01" TargetContentID="7" />
                </TranscodingList>
            </Job>
        </CreateJobRequest>
    </BODY>
</WTRS.MSG>
	 */
	unsigned int nTranscoding = 0;

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyTypeElement = rootElement->FirstChildElement("BODY");
	TiXmlElement* jobElement = bodyTypeElement->FirstChildElement(
				"CreateJobRequest")->FirstChildElement("Job");

	TiXmlElement* transcodingListElement = jobElement->FirstChildElement("TranscodingList");
	if(transcodingListElement)
	{
		TiXmlElement* transcodingElement = transcodingListElement->FirstChildElement("Transcoding");
		while(transcodingElement)
		{
			nTranscoding++;
			transcodingElement = transcodingElement->NextSiblingElement("Transcoding");
		}
	}
	return nTranscoding;
}
void AppXmlParser::m_fnGetJobStateResultCode(KString & _rclsXml, KString & _rclsResultCode)
{
	/*
<?xml version="1.0"?>
<WTRS.MSG Version="1.0">
    <HEADER From="urn:wtrsp:system:wtrsg" SessionID="{f0bc6c2d-0454-430e-a14b-3759ce33fb16}" To="urn:wtrsp:auth.endpoint:mcc" TransactionID="2441">
        <Error Code="0000" Description="" Source="" />
    </HEADER>

    <BODY>
        <JobStatusChangedNotify>
            <JobState ID="{d95e0e26-dbb4-4d28-9524-b78adc6a057a}" Kind="Realtime" Status="TranscodingStopped" ServiceName="62713794|102616734|" StartTime="2022-01-27 12:49:06" EndTime="" ResultCode="4001">
                <Transcodes StartTime="2022-01-27 12:49:06" EndTime="2022-01-27 12:49:08" TransSessionID="{d95e0e26-dbb4-4d28-9524-b78adc6a057a}_1" Cancel="0" Current="1" Fail="1" Success="0" Total="1" ResultCode="4001" ResultDescription="0">
                    <Transcode>
                        <Source NASCode="11" File="vodtemp/404803/7936/102616734_1.k3g" WTRSSPath="" BinaryData="">
                        <Container ID="3GP" />
                        </Source>
                        <Target NASCode="11" File="vodtemp/404803/7936/102616734_1.wma" WTRSSPath="" BinaryData="">
                            <Container ID="WMA">
                                <VideoCodec Width="0" Height="0" MaxFrameRate="0" BitRate="0" VariableFrameRate="0" ID="NONE" />
                                <AudioCodec Channel="2" SampleRate="44100" BitRate="64000" BitPerSample="16" ID="WMA2" />
                            </Container>
                        </Target>
                    </Transcode>
                </Transcodes>
            </JobState>
        </JobStatusChangedNotify>
    </BODY>
</WTRS.MSG>
	 */
	/*
	 <?xml version="1.0"?>
	<WTRS.MSG Version="1.0">
		<HEADER From="urn:wtrsp:system:wtrsg" SessionID="{0c9d039c-13f9-4c43-b7af-700930e37475}" To="urn:wtrsp:auth.endpoint:mcc" TransactionID="783">
			<Error Code="0000" Description="" Source="" />
		</HEADER>

		<BODY>
			<JobStatusChangedNotify>
				<JobState ID="{70721ae8-1d3a-4887-8bf3-0712f12b27f2}" Kind="Realtime" Status="Destroyed" ServiceName="62708416|15319725|" StartTime="2022-01-26 12:22:37" EndTime="2022-01-26 12:22:38" ResultCode="0000">
					<Transcodes Cancel="0" Current="0" Fail="0" Success="7" Total="7" />
				</JobState>
			</JobStatusChangedNotify>
		</BODY>
	</WTRS.MSG>
	 */
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyTypeElement = rootElement->FirstChildElement("BODY");
	TiXmlElement* jobStateElement = bodyTypeElement->FirstChildElement(
				"JobStatusChangedNotify")->FirstChildElement("JobState");

	_rclsResultCode = jobStateElement->Attribute("ResultCode");
	IFLOG(E_LOG_DEBUG, "[AppXmlParser] T/C Stop ResultCode[%s]", (KCSTR)_rclsResultCode);
}
void AppXmlParser::m_fnGetTransSessionID(KString & _rclsXml, KString & _rclsTransSessionID)
{
	/*
<?xml version="1.0"?>
<WTRS.MSG Version="1.0">
    <HEADER From="urn:wtrsp:system:wtrsg" SessionID="{f0bc6c2d-0454-430e-a14b-3759ce33fb16}" To="urn:wtrsp:auth.endpoint:mcc" TransactionID="2489">
        <Error Code="0000" Description="" Source="" />
    </HEADER>

    <BODY>
        <JobStatusChangedNotify>
            <JobState ID="{9c80b292-32bf-4d48-942a-ea5a987446c6}" Kind="Realtime" Status="TranscodingStarted" ServiceName="62713867|86401486|" StartTime="2022-01-27 13:03:13" EndTime="" ResultCode="0000">
                <Transcodes StartTime="2022-01-27 13:03:13" EndTime="2022-01-27 13:03:13" TransSessionID="{9c80b292-32bf-4d48-942a-ea5a987446c6}_1" Cancel="0" Current="1" Fail="0" Success="0" Total="1" ResultCode="0000" ResultDescription="0">
                    <Transcode>
                        <Source NASCode="55" File="feelring/64/20/00/14/6420001486.mp3" WTRSSPath="C:\TRSS\temp\202201271303138825560_52886280_src.MP3" BinaryData="">
                        <Container ID="MP3" />
                        </Source>
                        <Target NASCode="55" File="feelring/64/20/00/14/6420001486.vpm" WTRSSPath="C:\TRSS\temp\202201271303138983025_52885752_tar.VOX" BinaryData="">
                            <Container ID="VOX">
                                <VideoCodec Width="0" Height="0" MaxFrameRate="0" BitRate="0" VariableFrameRate="0" ID="NONE" />
                                <AudioCodec Channel="1" SampleRate="8000" BitRate="32000" BitPerSample="4" ID="VOX" />
                            </Container>
                        </Target>
                    </Transcode>
                </Transcodes>
            </JobState>
        </JobStatusChangedNotify>
    </BODY>
</WTRS.MSG>
	 */
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyTypeElement = rootElement->FirstChildElement("BODY");
	TiXmlElement* transcodesElement = bodyTypeElement->FirstChildElement(
				"JobStatusChangedNotify")->FirstChildElement("JobState")
						->FirstChildElement("Transcodes");

	_rclsTransSessionID = transcodesElement->Attribute("TransSessionID");
	IFLOG(E_LOG_DEBUG, "[AppXmlParser] T/C Start TransSessionID[%s]", (KCSTR)_rclsTransSessionID);
}
void AppXmlParser::m_fnGetTransCnt(KString & _rclsXml, int* _pnCancel, int* _pnCurrent, int* _pnFail, int* _pnSuccess, int* _pnTotal)
{
	/*
<?xml version="1.0"?>
<WTRS.MSG Version="1.0">
    <HEADER From="urn:wtrsp:system:wtrsg" SessionID="{2cb8e94e-8100-405f-b3ad-c8ce07fd0837}" To="urn:wtrsp:auth.endpoint:MMSC_ACPT" TransactionID="15444">
        <Error Code="0000" Description="" Source="" />
    </HEADER>

    <BODY>
        <JobStatusChangedNotify>
            <JobState ID="{bcd93258-9e76-4f34-889c-64cb30f04666}" Kind="Realtime" Status="TranscodingStopped" ServiceName="Z1yqB93S" StartTime="2021-05-25 22:09:11" EndTime="" ResultCode="0000">
                <Transcodes StartTime="2021-05-25 22:09:11" EndTime="2021-05-25 22:09:12" TransSessionID="{bcd93258-9e76-4f34-889c-64cb30f04666}_1" Cancel="0" Current="1" Fail="0" Success="1" Total="1" ResultCode="0000" ResultDescription="12">
                    <Transcode>
                        <Source NASCode="" File="" WTRSSPath="C:\TRSS\temp\202105252209112388780_35584528_src.K3G" BinaryData="\\172.17.107.5\nas_wtrsp\temp\2021052522091524324200_decode.tmp">
                        <Container ID="K3G" />
                        </Source>
                        <Target NASCode="" File="" WTRSSPath="C:\TRSS\temp\202105252209112693652_35583984_tar.K3G" BinaryData="AAAAGGZ0eXBrM2cxAAAAAWlAAtAAAAAAAAAAAAANFTk1PS1RGIAAAAAA=">
                            <Container ID="K3G">
                                <VideoCodec Width="176" Height="144" MaxFrameRate="15" BitRate="256000" VariableFrameRate="0" ID="MPEG4" />
                                <AudioCodec Channel="1" SampleRate="8000" BitRate="12200" BitPerSample="16" ID="AMR" />
                            </Container>
                        </Target>
                    </Transcode>
                </Transcodes>
            </JobState>
        </JobStatusChangedNotify>
    </BODY>
</WTRS.MSG>
	 */
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyTypeElement = rootElement->FirstChildElement("BODY");
	TiXmlElement* transcodesElement = bodyTypeElement->FirstChildElement(
				"JobStatusChangedNotify")->FirstChildElement("JobState")
						->FirstChildElement("Transcodes");

	if(_pnCancel != NULL) transcodesElement->Attribute("Cancel", _pnCancel);
	if(_pnCurrent != NULL) transcodesElement->Attribute("Current", _pnCurrent);
	if(_pnFail != NULL) transcodesElement->Attribute("Fail", _pnFail);
	if(_pnSuccess != NULL) transcodesElement->Attribute("Success", _pnSuccess);
	if(_pnTotal != NULL) transcodesElement->Attribute("Total", _pnTotal);
}
void AppXmlParser::m_fnGetTargetContentList(KString & _rclsXml, StlList * _pclsTargetList)
{
	if(_pclsTargetList) _pclsTargetList->m_fnClear();
	else return;

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyTypeElement = rootElement->FirstChildElement("BODY");
	TiXmlElement* targetContentMapElement = bodyTypeElement->FirstChildElement(
			"CreateJobRequest")->FirstChildElement("Job")->FirstChildElement(
			"TargetContentMap");

	TiXmlElement* targetContentElement = targetContentMapElement->FirstChildElement("TargetContent");
	while(targetContentElement)
	{
		TargetContent * pclsTargetContent = new TargetContent;
		//pclsTargetContent->m_clsBinaryData = targetContentElement->Attribute("BinaryData");// not used
		pclsTargetContent->m_clsFile = targetContentElement->Attribute("File");
		pclsTargetContent->m_clsID = targetContentElement->Attribute("ID");
		pclsTargetContent->m_clsNasCode = targetContentElement->Attribute("NASCode");
		pclsTargetContent->m_clsPath = targetContentElement->Attribute("Path");
		pclsTargetContent->m_clsSize = targetContentElement->Attribute("Size");

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
				pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsVariableFrameRate = videoCodecElement->Attribute("VariableFrameRate");
				pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsWidth = videoCodecElement->Attribute("Width");
			}
		}

		//add list
//		KString clsDebug;
//		pclsTargetContent->m_fnDebug(clsDebug);
//		IFLOG(E_LOG_ERR, (KCSTR)clsDebug);
		_pclsTargetList->m_fnPushBack(pclsTargetContent);
		//next
		targetContentElement = targetContentElement->NextSiblingElement("TargetContent");
	}
}
void AppXmlParser::m_fnMakeLinkTestRes(KString & _rclsXml, KString & _rclsLinkTestRes)
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
	KString clsFrom;
	KString clsSessionID;
	KString clsTo;
	KString clsTransactionID;

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	clsFrom = headerElement->Attribute("From");
	clsSessionID = headerElement->Attribute("SessionID");
	clsTo = headerElement->Attribute("To");
	clsTransactionID = headerElement->Attribute("TransactionID");

	//To, From swap
    KString::m_fnStrnCat((KSTR)_rclsLinkTestRes,_rclsLinkTestRes.m_unLen,
		"<?xml version=\"1.0\"?>"
		"<WTRS.MSG Version=\"1.0\">"
    		"<HEADER From=\"%s\" SessionID=\"%s\" To=\"%s\" TransactionID=\"%s\">"
    			"<Error Code=\"0000\" Description=\"OK\" Source=\"\" />"
    		"</HEADER>"
    		"<BODY>"
    			"<LinkTestResponse />"
    		"</BODY>"
		"</WTRS.MSG>", (KSTR)clsTo, (KSTR)clsSessionID, (KSTR)clsFrom, (KSTR)clsTransactionID);
}
void AppXmlParser::m_fnMakeEstablishSessionRes(KString & _rclsXml, KString & _rclsSessionID, ETrsgCodeSet_t _eSt, KString & _rclsEstablishSessionRes)
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
	MainConfig* pclsConfig = MainConfig::m_fnGetInstance();
	KString clsFrom;
	KString clsSessionID = _rclsSessionID;
	KString clsTo;
	KString clsTransactionID;
	KString clsCode;
	KString clsDescription;

//	E_JOB_RES_OK          "0000" "OK"
//	E_IP_ACCESS_FAILED    "1006" "IP_ACCESS_FAILED"
//	E_ID_PASSWORD_FAILED  "1007" "ID_PASSWORD_FAILED"
	clsCode = g_fnGetTrsgCode(_eSt); clsDescription = g_fnGetTrsgCodeDesc((KCSTR)clsCode);

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	clsFrom = headerElement->Attribute("From");
	clsTo = headerElement->Attribute("To");
	clsTransactionID = headerElement->Attribute("TransactionID");

	//To, From swap
    KString::m_fnStrnCat((KSTR)_rclsEstablishSessionRes,_rclsEstablishSessionRes.m_unLen,
		"<?xml version=\"1.0\"?>"
		"<WTRS.MSG Version=\"1.0\">"
    		"<HEADER From=\"%s\" SessionID=\"%s\" To=\"%s\" TransactionID=\"%s\">"
    			"<Error Code=\"%s\" Description=\"%s\" Source=\"\" />"
    		"</HEADER>"
    		"<BODY>"
    			"<EstablishSessionResponse>"
    				"<WTRS.CONFIG Version=\"1.0\">"
    					"<CONFIG_SYSTEM_CLIENT LinkTestInterval=\"%d\" LinkTestTimeout=\"%d\" LinkTestTimeoutCriticalCount=\"%d\" MaxTCPContentSizeKB=\"%d\" MaxTagImageSizeKB=\"%d\" RetryConnectionInterval=\"%d\" SystemID=\"%s\" SystemIndex=\"%d\" TransactionTimeout=\"%d\" MaxConnectionRetries=\"0\" />"
    				"</WTRS.CONFIG>"
    			"</EstablishSessionResponse>"
    		"</BODY>"
		"</WTRS.MSG>", (KCSTR)clsTo, (KCSTR)clsSessionID, (KCSTR)clsFrom, (KCSTR)clsTransactionID,
		(KCSTR)clsCode, (KCSTR)clsDescription,
		pclsConfig->m_unLinkTestInterval, pclsConfig->m_unLinkTestTimeout, pclsConfig->m_unLinkTestCriticalCount,
		pclsConfig->m_unMaxTcpContentSizeKB, pclsConfig->m_unMaxTagImageSizeKB, pclsConfig->m_unRetryConnectionInterval,
		(KSTR)pclsConfig->m_clsSysName, pclsConfig->m_unSysIndex, pclsConfig->m_unTransactionTimeout, pclsConfig->m_unMaxConnectionRetries);
}
void AppXmlParser::m_fnMakeCloseSessionRes(KString & _rclsXml, KString & _rclsCloseSessionRes)
{
/*
<?xml version="1.0"?>
<WTRS.MSG Version="1.0">
    <HEADER From="urn:wtrsp:auth.endpoint:MMSC_ACPT" SessionID="" To="urn:wtrsp:system:wtrsg" TransactionID="1">
        <Error Code="" Description="" Source="" />
    </HEADER>

    <BODY>
        <CloseSessionRequest/>

    </BODY>
</WTRS.MSG>

<?xml version="1.0"?>
<WTRS.MSG Version="1.0">
    <HEADER From="urn:wtrsp:auth.endpoint:MMSC_ACPT" SessionID="" To="urn:wtrsp:system:wtrsg" TransactionID="1">
        <Error Code="0000" Description="OK" Source="" />
    </HEADER>

    <BODY>
        <CloseSessionResponse/>
    </BODY>
</WTRS.MSG>
 */
	KString clsFrom;
	KString clsSessionID;
	KString clsTo;
	KString clsTransactionID;

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	clsFrom = headerElement->Attribute("From");
	clsSessionID = headerElement->Attribute("SessionID");
	clsTo = headerElement->Attribute("To");
	clsTransactionID = headerElement->Attribute("TransactionID");

	//To, From swap
    KString::m_fnStrnCat((KSTR)_rclsCloseSessionRes,_rclsCloseSessionRes.m_unLen,
		"<?xml version=\"1.0\"?>"
		"<WTRS.MSG Version=\"1.0\">"
    		"<HEADER From=\"%s\" SessionID=\"%s\" To=\"%s\" TransactionID=\"%s\">"
    			"<Error Code=\"0000\" Description=\"OK\" Source=\"\" />"
    		"</HEADER>"
    		"<BODY>"
    			"<CloseSessionResponse />"
    		"</BODY>"
		"</WTRS.MSG>", (KSTR)clsTo, (KSTR)clsSessionID, (KSTR)clsFrom, (KSTR)clsTransactionID);
}
void AppXmlParser::m_fnMakeCreateJobReq(KString & _rclsXml, KString & _rclsTo,
		KString & _rclsFrom, KString & _rclsSessionID, KString & _rclsJobID,
		KString & _rclsInternalNasFileName,
		KString & _rclsInternalNasImgFileName, KString & _rclsCreateJobReq)
{
/*
 <?xml version="1.0"?>
<WTRS.MSG Version="1.0">
    <HEADER From="urn:wtrsp:auth.endpoint:MMSC_ACPT" SessionID="{2cb8e94e-8100-405f-b3ad-c8ce07fd0837}" To="urn:wtrsp:system:wtrsg" TransactionID="15250"> <-- trss 통신 기준으로 From, To, SessionID 변경.
        <Error Code="" Description="" Source="" />
    </HEADER>

    <BODY>
        <CreateJobRequest>
            <Job Kind="Realtime" ServiceName="Z2yqcJhv" ID="{2cb8e94e-8100-405f-b3ad-c8ce07fd0837}">  <--add Jod ID
                <SourceContentMap>
                    <SourceContent BinaryData="AA...이후생략" File="" ID="0" NASCode="" Path="">
                        <Container ID="K3G">
                            <VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width="" />
                            <AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate="" />
                            <Metadata Album="" Artist="" Comment="" Genre="" Title="" Track="" Year="" />
                        </Container>
                    </SourceContent>
                </SourceContentMap>
                <TargetContentMap>
                    <TargetContent BinaryData="" File="" ID="0" NASCode="" Path="">
                        <Container ID="K3G">
                            <VideoCodec BitRate="256000" Height="144" ID="MPEG4" MaxFrameRate="15" VariableFrameRate="" Width="176" />
                            <AudioCodec BitPerSample="16" BitRate="12200" Channel="1" ID="AMR" SampleRate="8000" />
                            <Metadata Album="" Artist="" Comment="" Genre="" Title="" Track="" Year="" />
                        </Container>
                    </TargetContent>
                </TargetContentMap>
                <TranscodingList>
                    <Transcoding SourceContentID="0" TargetContentID="0" />
                </TranscodingList>
            </Job>
        </CreateJobRequest>
    </BODY>
</WTRS.MSG>
*/
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	headerElement->SetAttribute("From", (KCSTR) _rclsFrom);//Replace From
	headerElement->SetAttribute("To", (KCSTR) _rclsTo);//Replace To
	headerElement->SetAttribute("SessionID", (KCSTR) _rclsSessionID);//Replace To

	TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");
	TiXmlElement* jobElement = bodyElement->FirstChildElement(
			"CreateJobRequest")->FirstChildElement("Job");
	jobElement->SetAttribute("ID", (KCSTR) _rclsJobID);//Job ID Add

	TiXmlElement* sourceContentElement = jobElement->FirstChildElement("SourceContentMap")->
			FirstChildElement("SourceContent");

	//TCP Type의 연동방식 일 경우에만 수정 함.
	//NAS 경로 빼고 파일명만 줘야함.
	if(_rclsInternalNasFileName.m_unRealLen != 0) sourceContentElement->SetAttribute("BinaryData", (KCSTR) _rclsInternalNasFileName);//BinaryData replace

	//image data 파일경로 형태로 넣어줘야함.
	if(_rclsInternalNasImgFileName.m_unRealLen != 0)
	{
		TiXmlElement* containerElement = sourceContentElement->FirstChildElement( "Container" );
		if (containerElement != NULL)
		{
			TiXmlElement* metadataElement = containerElement->FirstChildElement( "Metadata" );
			if (metadataElement != NULL)
			{
				metadataElement->SetAttribute("ImageData", (KCSTR) _rclsInternalNasImgFileName);//BinaryData replace
			}
		}
	}

	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	doc.Accept(&printer);
	_rclsCreateJobReq = printer.CStr();
}
void AppXmlParser::m_fnMakeCreateJobRes(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsJobID, ETrsgCodeSet_t _eSt, KString & _rclsCreateJobRes)
{
/*
 <?xml version="1.0"?>
<WTRS.MSG Version="1.0">
    <HEADER From="urn:wtrsp:auth.endpoint:MMSC_ACPT" SessionID="{2cb8e94e-8100-405f-b3ad-c8ce07fd0837}" To="urn:wtrsp:system:wtrsg" TransactionID="15250">
        <Error Code="" Description="" Source="" />
    </HEADER>

    <BODY>
        <CreateJobRequest>
            <Job Kind="Realtime" ServiceName="Z2yqcJhv">
                <SourceContentMap>
                    <SourceContent BinaryData="AA...이후생략" File="" ID="0" NASCode="" Path="">
                        <Container ID="K3G">
                            <VideoCodec BitRate="" Height="" ID="" MaxFrameRate="" VariableFrameRate="" Width="" />
                            <AudioCodec BitPerSample="" BitRate="" Channel="" ID="" SampleRate="" />
                            <Metadata Album="" Artist="" Comment="" Genre="" Title="" Track="" Year="" />
                        </Container>
                    </SourceContent>
                </SourceContentMap>
                <TargetContentMap>
                    <TargetContent BinaryData="" File="" ID="0" NASCode="" Path="">
                        <Container ID="K3G">
                            <VideoCodec BitRate="256000" Height="144" ID="MPEG4" MaxFrameRate="15" VariableFrameRate="" Width="176" />
                            <AudioCodec BitPerSample="16" BitRate="12200" Channel="1" ID="AMR" SampleRate="8000" />
                            <Metadata Album="" Artist="" Comment="" Genre="" Title="" Track="" Year="" />
                        </Container>
                    </TargetContent>
                </TargetContentMap>
                <TranscodingList>
                    <Transcoding SourceContentID="0" TargetContentID="0" />
                </TranscodingList>
            </Job>
        </CreateJobRequest>
    </BODY>
</WTRS.MSG>

<?xml version="1.0"?>
<WTRS.MSG Version="1.0">
    <HEADER From="urn:wtrsp:system:wtrsg" SessionID="{2cb8e94e-8100-405f-b3ad-c8ce07fd0837}" To="urn:wtrsp:auth.endpoint:MMSC_ACPT" TransactionID="15250">
        <Error Code="0000" Description="OK" Source="" />
    </HEADER>

    <BODY>
        <CreateJobResponse>
            <Job ID="{7672967e-63d3-4aa1-bcab-c8b78d8b548f}" ServiceName="Z2yqcJhv" Kind="Realtime" />
        </CreateJobResponse>
    </BODY>
</WTRS.MSG>
*/
	KString clsSessionID;
	KString clsTransactionID;
	KString clsServiceName;

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");
	TiXmlElement* bodyElement =  rootElement->FirstChildElement("BODY");
	TiXmlElement* jobElement = bodyElement->FirstChildElement("CreateJobRequest")->FirstChildElement("Job");

	clsSessionID = headerElement->Attribute("SessionID");
	clsTransactionID = headerElement->Attribute("TransactionID");
	clsServiceName = jobElement->Attribute("ServiceName");

	KString clsCode = g_fnGetTrsgCode(_eSt);
	KString clsDescription = g_fnGetTrsgCodeDesc((KCSTR)clsCode);

	//ServiceName 돌려줘야 함.
	//Kind 는 Realtime 고정임. 그 외 지원 안함.
	//Job의 ID 는 신규 생성해야함.

	KString clsTmpMessage; clsTmpMessage.m_fnReSize(10240);
	//To, From swap
    KString::m_fnStrnCat((KSTR)clsTmpMessage,clsTmpMessage.m_unLen,
		"<?xml version=\"1.0\"?>"
		"<WTRS.MSG Version=\"1.0\">"
    		"<HEADER From=\"%s\" SessionID=\"%s\" To=\"%s\" TransactionID=\"%s\">"
    			"<Error Code=\"%s\" Description=\"%s\" Source=\"\" />"
    		"</HEADER>"
    		"<BODY>"
				"<CreateJobResponse>"
					"<Job ID=\"%s\" ServiceName=\"ServiceName\" Kind=\"Realtime\" />"
				"</CreateJobResponse>"
    		"</BODY>"
		"</WTRS.MSG>", (KSTR)_rclsFrom, (KSTR)clsSessionID, (KSTR)_rclsTo, (KSTR)clsTransactionID,
		(KCSTR)clsCode, (KCSTR)clsDescription, (KSTR)_rclsJobID);

	/* ServiceName을 SetAttribute 로 처리하여 특수문자 깨지는 현상을 방지하기 위한 과정 */
	TiXmlDocument docTmp;
	docTmp.Parse((KCSTR)clsTmpMessage, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement *rootElementTmp = docTmp.RootElement();
	TiXmlElement *JobTmp = rootElementTmp->FirstChildElement("BODY")->FirstChildElement("CreateJobResponse")->FirstChildElement("Job");
	JobTmp->SetAttribute("ServiceName", (KCSTR)clsServiceName);

	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	docTmp.Accept(&printer);
	_rclsCreateJobRes = printer.CStr();

}
void AppXmlParser::m_fnMakeJobStatusChangedNotify_Created(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, KString & _rclsJobStatusChgNotify)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	headerElement->SetAttribute("From", (KCSTR) _rclsFrom);//Replace From
	headerElement->SetAttribute("To", (KCSTR) _rclsTo);//Replace To
	headerElement->SetAttribute("SessionID", (KCSTR) _rclsSessionID);//Replace SessionID

	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	doc.Accept(&printer);
	_rclsJobStatusChgNotify = printer.CStr();
}
void AppXmlParser::m_fnMakeJobStatusChangedNotify_Waiting(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, KString & _rclsJobStatusChgNotify)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	headerElement->SetAttribute("From", (KCSTR) _rclsFrom);//Replace From
	headerElement->SetAttribute("To", (KCSTR) _rclsTo);//Replace To
	headerElement->SetAttribute("SessionID", (KCSTR) _rclsSessionID);//Replace SessionID

	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	doc.Accept(&printer);
	_rclsJobStatusChgNotify = printer.CStr();
}
void AppXmlParser::m_fnMakeJobStatusChangedNotify_JobStarted(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, CTranscodesCnt & _rclsTranscodesCnt, KString & _rclsJobStatusChgNotify)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	headerElement->SetAttribute("From", (KCSTR) _rclsFrom);//Replace From
	headerElement->SetAttribute("To", (KCSTR) _rclsTo);//Replace To
	headerElement->SetAttribute("SessionID", (KCSTR) _rclsSessionID);//Replace SessionID

	TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");
	if(bodyElement != NULL) m_fnSetTranscodesCnt(bodyElement, _rclsTranscodesCnt);

	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	doc.Accept(&printer);
	_rclsJobStatusChgNotify = printer.CStr();
}
void AppXmlParser::m_fnMakeJobStatusChangedNotify_TC_Started(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, CTranscodesCnt & _rclsTranscodesCnt, KString & _rclsJobStatusChgNotify)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	headerElement->SetAttribute("From", (KCSTR) _rclsFrom);//Replace From
	headerElement->SetAttribute("To", (KCSTR) _rclsTo);//Replace To
	headerElement->SetAttribute("SessionID", (KCSTR) _rclsSessionID);//Replace SessionID

	TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");
	if(bodyElement != NULL) m_fnSetTranscodesCnt(bodyElement, _rclsTranscodesCnt);

	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	doc.Accept(&printer);
	_rclsJobStatusChgNotify = printer.CStr();
}
bool AppXmlParser::m_fnMakeJobStatusChangedNotify_TC_Stopped(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, CTranscodesCnt & _rclsTranscodesCnt, KString & _rclsResultCode, KString & _rclsJobStatusChgNotify)
{
	bool bRtn = true;
	int nDstNasCode = -1;
	AppXmlParser::m_fnGetNasCode(_rclsXml, NULL , &nDstNasCode);

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	headerElement->SetAttribute("From", (KCSTR) _rclsFrom);//Replace From
	headerElement->SetAttribute("To", (KCSTR) _rclsTo);//Replace To
	headerElement->SetAttribute("SessionID", (KCSTR) _rclsSessionID);//Replace SessionID

	if( (nDstNasCode == -1) && (_rclsResultCode == g_fnGetTrsgCode(E_JOB_RES_OK)) )//TCP 방식이므로 , 파일을 읽어서 base64로 binary 설정해야 함. and 변환 성공일 경우에만 진행하여야 함.
	{
		TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");
		TiXmlElement* transcodeElement = bodyElement->FirstChildElement("JobStatusChangedNotify")->
				FirstChildElement("JobState")->
				FirstChildElement("Transcodes")->
				FirstChildElement("Transcode");

		TiXmlElement* targetElement = transcodeElement->FirstChildElement("Target");

		MainConfig* pclsConfig = MainConfig::m_fnGetInstance();
		KString tmpFile;
		tmpFile.m_fnCat((KCSTR)pclsConfig->m_clsNasInternal);
		if( ((KCSTR)tmpFile)[tmpFile.m_unRealLen-1] != '/') tmpFile.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.
		KString clsFileName = targetElement->Attribute("BinaryData");
		tmpFile.m_fnCat((KCSTR)clsFileName);

		std::ifstream fin((KCSTR)tmpFile, std::ios::in|std::ios::binary);
		if(fin.is_open() && (clsFileName.m_unRealLen > 0) )
		{
			//파일 사이즈 구함.
			fin.seekg(0, std::ios::end);
			int length = fin.tellg();
			//fd 원복
			fin.seekg(0, std::ios::beg);
			char* buf = new char[length];
			//파일 내용 읽음
			fin.read(buf, length);
			fin.close();

			char* dst = NULL;
			//파일 내용 base64 encode
			int nLen = CBase64::m_fnEncode(buf, length, &dst);
			if(nLen == 0 )//base64 encoding 실패.
			{
				bRtn = false;
				_rclsTranscodesCnt.nSuccess--;
				_rclsTranscodesCnt.nFail++;
				IFLOG(E_LOG_DEBUG, "[AppXmlParser] BinaryData encode len[%d]", nLen);
				targetElement->RemoveAttribute("BinaryData");//BinaryData Attr 값 빈값으로 설정하기 위해 지우고 새로 생성함.
				targetElement->SetAttribute("BinaryData", "");
				//ResultCode 변경 작업.
				TiXmlElement* jobStateElement = bodyElement->FirstChildElement(
							"JobStatusChangedNotify")->FirstChildElement("JobState");
				TiXmlElement *transcodesElement = jobStateElement->FirstChildElement("Transcodes");
				jobStateElement->SetAttribute("ResultCode", g_fnGetTrsgCode(E_TCP_BINARY_ENCODING_FAILED));
				transcodesElement->SetAttribute("ResultCode", g_fnGetTrsgCode(E_TCP_BINARY_ENCODING_FAILED));
			}
			else
			{
				IFLOG(E_LOG_ERR, "[AppXmlParser] BinaryData encode len[%d]", nLen);
				targetElement->SetAttribute("BinaryData", dst);
			}
			if(buf) delete buf;
			if(dst) delete dst;
		}
		else//변환된 파일 open 실패.
		{
			bRtn = false;
			_rclsTranscodesCnt.nSuccess--;
			_rclsTranscodesCnt.nFail++;
			IFLOG(E_LOG_ERR,"[AppXmlParser] File open fail[%s]", (KCSTR)tmpFile);
			targetElement->RemoveAttribute("BinaryData");//BinaryData Attr 값 빈값으로 설정하기 위해 지우고 새로 생성함.
			targetElement->SetAttribute("BinaryData", "");
			//ResultCode 변경 작업.
			TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");
			TiXmlElement* jobStateElement = bodyElement->FirstChildElement(
						"JobStatusChangedNotify")->FirstChildElement("JobState");
			TiXmlElement *transcodesElement = jobStateElement->FirstChildElement("Transcodes");

			jobStateElement->SetAttribute("ResultCode", g_fnGetTrsgCode(E_TCP_BINARY_ENCODING_FAILED));
			transcodesElement->SetAttribute("ResultCode", g_fnGetTrsgCode(E_TCP_BINARY_ENCODING_FAILED));
		}
	}
	TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");
	if(bodyElement != NULL) m_fnSetTranscodesCnt(bodyElement, _rclsTranscodesCnt);

	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	doc.Accept(&printer);
	_rclsJobStatusChgNotify.m_fnReSize(KString::m_fnStrLen(printer.CStr()));
	_rclsJobStatusChgNotify = printer.CStr();
	return bRtn;
}
void AppXmlParser::m_fnMakeJobStatusChangedNotify_JobStopped(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, CTranscodesCnt & _rclsTranscodesCnt, KString & _rclsJobStatusChgNotify)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	headerElement->SetAttribute("From", (KCSTR) _rclsFrom);//Replace From
	headerElement->SetAttribute("To", (KCSTR) _rclsTo);//Replace To
	headerElement->SetAttribute("SessionID", (KCSTR) _rclsSessionID);//Replace SessionID

	TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");
	if(bodyElement != NULL) m_fnSetTranscodesCnt(bodyElement, _rclsTranscodesCnt);

	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	doc.Accept(&printer);
	_rclsJobStatusChgNotify = printer.CStr();
}
void AppXmlParser::m_fnMakeJobStatusChangedNotify_Destroyed(KString & _rclsXml, KString & _rclsTo, KString & _rclsFrom, KString & _rclsSessionID, CTranscodesCnt & _rclsTranscodesCnt, KString & _rclsJobStatusChgNotify)
{//Trss로 부터 Destroy를 받아서 전달하는 경우.
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* headerElement =  rootElement->FirstChildElement("HEADER");

	headerElement->SetAttribute("From", (KCSTR) _rclsFrom);//Replace From
	headerElement->SetAttribute("To", (KCSTR) _rclsTo);//Replace To
	headerElement->SetAttribute("SessionID", (KCSTR) _rclsSessionID);//Replace SessionID

	TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");
	if(bodyElement != NULL) m_fnSetTranscodesCnt(bodyElement, _rclsTranscodesCnt);

	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	doc.Accept(&printer);
	_rclsJobStatusChgNotify = printer.CStr();
}
void AppXmlParser::m_fnMakeJobStatusChangedNotify_Destroyed(KString & _rclsXml, KString & _rclsJobStatusChgNotify, CTranscodesCnt & _rclsTranscodesCnt, ETrsgCodeSet_t _eSt)
{// Trsg에서 자체적으로 Destroy를 보내야 하는 경우.(CreateJobResponse 메시지를 기반으로 Destroy를 생성함) (Overloading)
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement *rootElement = doc.RootElement();
	TiXmlElement *headerElement = rootElement->FirstChildElement("HEADER");
	TiXmlElement *bodyElement = rootElement->FirstChildElement("BODY");

	TiXmlElement *jobElement = bodyElement->FirstChildElement("CreateJobResponse")->FirstChildElement("Job");
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

	KString clsCode;
	KString clsDescription;
	clsCode = g_fnGetTrsgCode(_eSt); clsDescription = g_fnGetTrsgCodeDesc((KCSTR)clsCode);

	KString clsTmpMessage; clsTmpMessage.m_fnReSize(10240);
	// To, From 유지. (CreateJobResponse 기반의 메시지임.)
	KString::m_fnStrnCat((KSTR) clsTmpMessage, clsTmpMessage.m_unLen,
			"<?xml version=\"1.0\"?>"
					"<WTRS.MSG Version=\"1.0\">"
					"<HEADER From=\"%s\" SessionID=\"%s\" To=\"%s\" TransactionID=\"%s\">"
					"<Error Code=\"%s\" Description=\"%s\" Source=\"\" />"
					"</HEADER>"
					"<BODY>"
					"<JobStatusChangedNotify>"
					"<JobState ID=\"%s\" Kind=\"Realtime\" Status=\"Destroyed\" ServiceName=\"ServiceName\" StartTime=\"%s\" EndTime=\"%s\" ResultCode=\"%s\">"
					"<Transcodes Cancel=\"0\" Current=\"0\" Fail=\"%d\" Success=\"%d\" Total=\"%d\" />"
					"</JobState>"
					"</JobStatusChangedNotify>"
					"</BODY>"
					"</WTRS.MSG>", (KSTR) clsFrom, (KSTR) clsSessionID,
			(KSTR) clsTo, (KSTR) clsTransactionID, (KCSTR)clsCode, (KCSTR)clsDescription, (KSTR) clsJobID,
			(KSTR) clsCurrTime, (KSTR) clsCurrTime,
			(KCSTR) clsCode, _rclsTranscodesCnt.nFail, _rclsTranscodesCnt.nSuccess, _rclsTranscodesCnt.nTotal);

	/* ServiceName을 SetAttribute 로 처리하여 특수문자 깨지는 현상을 방지하기 위한 과정 */
	TiXmlDocument docTmp;
	docTmp.Parse((KCSTR)clsTmpMessage, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement *rootElementTmp = docTmp.RootElement();
	TiXmlElement *JobStateTmp = rootElementTmp->FirstChildElement("BODY")->FirstChildElement("JobStatusChangedNotify")->FirstChildElement("JobState");
	JobStateTmp->SetAttribute("ServiceName", (KCSTR)clsServiceName);

	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	docTmp.Accept(&printer);
	_rclsJobStatusChgNotify = printer.CStr();
}
ETrsgCodeSet_t AppXmlParser::m_fnWriteSourceFile(KString & _rclsXml, KString & _rclsPath, KString & _rclsFileName)
{
	KString clsCloseSessionRes; clsCloseSessionRes.m_fnReSize(10240);
	KString clsFrom;
	KString clsSessionID;
	KString clsTo;
	KString clsTransactionID;

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyElement =  rootElement->FirstChildElement("BODY");

	TiXmlNode* sourceContentNode = bodyElement->
	FirstChild( "CreateJobRequest" )->
    FirstChild( "Job" )->
    FirstChild( "SourceContentMap" )->
    FirstChild( "SourceContent" );

	if (sourceContentNode == NULL)
	{
		IFLOG(E_LOG_ERR, "[AppXmlParser]Xml Format error[SourceContent]");
		return E_SOURCE_CONTENT_ERR;
	}

	char* pBase64Data = (char*)sourceContentNode->ToElement()->Attribute("BinaryData");
	if (pBase64Data == NULL)
	{
		IFLOG(E_LOG_ERR, "[AppXmlParser]Xml Format error[SourceContent-BinaryData]");
		return E_TCP_BINARY_DECODING_FAILED;
	}

	unsigned char* baseDecode = new unsigned char[KString::m_fnStrLen(pBase64Data)];
	int len = CBase64::m_fnDecode(pBase64Data, baseDecode, KString::m_fnStrLen(pBase64Data));
	if(len == 0)
	{
		IFLOG(E_LOG_ERR, "[AppXmlParser]CBase64 decode fail");
		if(baseDecode) delete baseDecode;
		return E_TCP_BINARY_DECODING_FAILED;
	}

	//path, filename valid check!!!
	KString tmpFile;
	KString::m_fnStrnCat((KSTR)tmpFile, tmpFile.m_unLen, "%s/%s", (KSTR)_rclsPath, (KSTR)_rclsFileName);
	std::ofstream fout((KCSTR)tmpFile, std::ios::out);
	if(fout.is_open())
	{
		fout.write((const char*) baseDecode, len);
		fout.close();
		if(baseDecode) delete baseDecode;
		return E_JOB_RES_OK;
	}
	else
	{
		if(baseDecode) delete baseDecode;
		return E_INTERNAL_NAS_ACCESS_ERR;
	}
}
bool AppXmlParser::m_fnWriteImageFile(KString & _rclsXml, KString & _rclsPath, KString & _rclsImgFileName)
{
	KString clsCloseSessionRes; clsCloseSessionRes.m_fnReSize(10240);
	KString clsFrom;
	KString clsSessionID;
	KString clsTo;
	KString clsTransactionID;

	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlElement* rootElement = doc.RootElement();
	TiXmlElement* bodyElement =  rootElement->FirstChildElement("BODY");

	TiXmlNode* sourceContentNode = bodyElement->
	FirstChild( "CreateJobRequest" )->
    FirstChild( "Job" )->
    FirstChild( "SourceContentMap" )->
    FirstChild( "SourceContent" );

	if (sourceContentNode == NULL)
	{
		IFLOG(E_LOG_ERR, "[AppXmlParser]Xml Format error(Image)[SourceContent]");
		return false;
	}

	TiXmlNode* containerNode = sourceContentNode->FirstChild( "Container" );
	if (containerNode == NULL)
	{
		IFLOG(E_LOG_ERR, "[AppXmlParser]Xml Format error(Image)[Container]");
		return false;
	}

	TiXmlNode* metadataNode = containerNode->FirstChild( "Metadata" );
	if (metadataNode == NULL)
	{
		IFLOG(E_LOG_ERR, "[AppXmlParser]Xml Format error(Image)[Metadata]");
		return false;
	}

	char* pBase64Data = (char*)metadataNode->ToElement()->Attribute("ImageData");
	if (pBase64Data == NULL)
	{
		IFLOG(E_LOG_ERR, "[AppXmlParser]Xml Format error(Image)[Metadata-ImageData]");
		return false;
	}

	unsigned char* baseDecode = new unsigned char[KString::m_fnStrLen(pBase64Data)];
	int len = CBase64::m_fnDecode(pBase64Data, baseDecode, KString::m_fnStrLen(pBase64Data));
	if(len == 0)
	{
		IFLOG(E_LOG_ERR, "[AppXmlParser]CBase64 decode fail(Image)");
		if(baseDecode) delete baseDecode;
		return false;
	}

	//path, filename valid check!!!
	KString tmpFile;
	KString::m_fnStrnCat((KSTR)tmpFile, tmpFile.m_unLen, "%s/%s", (KSTR)_rclsPath, (KSTR)_rclsImgFileName);
	std::ofstream fout((KCSTR)tmpFile, std::ios::out);
	if(fout.is_open())
	{
		fout.write((const char*) baseDecode, len);
		fout.close();
		if(baseDecode) delete baseDecode;
		IFLOG(E_LOG_INFO, "[AppXmlParser]WriteImageFile OK(%s/%s)", (KSTR)_rclsPath, (KSTR)_rclsImgFileName);
		return true;
	}
	else
	{
		if(baseDecode) delete baseDecode;
		IFLOG(E_LOG_ERR, "[AppXmlParser]WriteImageFile  Fail(%s/%s)", (KSTR)_rclsPath, (KSTR)_rclsImgFileName);
		return false;
	}
}
void AppXmlParser::m_fnSetLongLogSkip(KString & _rclsXml, KString & _rclsSkipXml)
{
	KString clsBodyType;
	if(AppXmlParser::m_fnGetBodyType(_rclsXml, clsBodyType))
	{
		TiXmlDocument doc;
		doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
		TiXmlElement* rootElement = doc.RootElement();
		TiXmlElement* bodyElement = rootElement->FirstChildElement("BODY");
		if(clsBodyType == DEF_BODY_CREATE_JOB_REQ)//trsg <- trse 수신 시 출력 간소화
		{
			TiXmlNode* sourceContentMapNode = bodyElement->
			FirstChild( "CreateJobRequest" )->
		    FirstChild( "Job" )->FirstChild( "SourceContentMap" );
			if(sourceContentMapNode == NULL)
			{
				_rclsSkipXml = _rclsXml;
				return;
			}

			TiXmlNode* sourceContentNode = sourceContentMapNode->FirstChild( "SourceContent" );
			if(sourceContentNode == NULL)
			{
				_rclsSkipXml = _rclsXml;
				return;
			}
			char* pBase64Data =
					(char*) sourceContentNode->ToElement()->Attribute(
							"BinaryData");
			if(pBase64Data == NULL)
			{
				_rclsSkipXml = _rclsXml;
			}
			else if(KString::m_fnStrLen(pBase64Data) <= 1024)
			{
				_rclsSkipXml = _rclsXml;
			}
			else
			{
				KString clsBinaryInfo; clsBinaryInfo.m_fnReSize(64);
				KString::m_fnStrnCat((KSTR)clsBinaryInfo,clsBinaryInfo.m_unLen,"SKIP_BINARY_LEN:%d", KString::m_fnStrLen(pBase64Data));
				sourceContentNode->ToElement()->SetAttribute("BinaryData", (KCSTR)clsBinaryInfo);//BinaryData replace

				TiXmlPrinter printer;
				printer.SetStreamPrinting();
				doc.Accept(&printer);
				_rclsSkipXml = printer.CStr();
			}
			//ImageData 출력 간소화.
			TiXmlNode* containerNode = sourceContentNode->FirstChild( "Container" );
			if (containerNode != NULL)
			{
				TiXmlNode* metadataNode = containerNode->FirstChild( "Metadata" );
				if (metadataNode != NULL)
				{
					char* pBase64ImgData = (char*)metadataNode->ToElement()->Attribute("ImageData");
					if(pBase64ImgData != NULL)
					{
						if(KString::m_fnStrLen(pBase64ImgData) > 1024)//이 조건이 없으면 파일 경로가 skip 되서 로그에 안보일 수 있음.
						{
							KString clsImageDataInfo; clsImageDataInfo.m_fnReSize(64);
							KString::m_fnStrnCat((KSTR)clsImageDataInfo,clsImageDataInfo.m_unLen,"SKIP_IMAGE_LEN:%d", KString::m_fnStrLen(pBase64ImgData));
							metadataNode->ToElement()->SetAttribute("ImageData", (KCSTR)clsImageDataInfo);//ImageData replace

							TiXmlPrinter printer;
							printer.SetStreamPrinting();
							doc.Accept(&printer);
							_rclsSkipXml = printer.CStr();
						}
					}
				}
			}
		}
		else if(clsBodyType == DEF_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED)//trsg -> trse 전송 시 출력 간소화
		{
/*
<?xml version="1.0"?>
<WTRS.MSG Version="1.0">
    <HEADER From="urn:wtrsp:system:wtrsg" SessionID="{2cb8e94e-8100-405f-b3ad-c8ce07fd0837}" To="urn:wtrsp:auth.endpoint:MMSC_ACPT" TransactionID="15444">
        <Error Code="0000" Description="" Source="" />
    </HEADER>

    <BODY>
        <JobStatusChangedNotify>
            <JobState ID="{bcd93258-9e76-4f34-889c-64cb30f04666}" Kind="Realtime" Status="TranscodingStopped" ServiceName="Z1yqB93S" StartTime="2021-05-25 22:09:11" EndTime="" ResultCode="0000">
                <Transcodes StartTime="2021-05-25 22:09:11" EndTime="2021-05-25 22:09:12" TransSessionID="{bcd93258-9e76-4f34-889c-64cb30f04666}_1" Cancel="0" Current="1" Fail="0" Success="1" Total="1" ResultCode="0000" ResultDescription="12">
                    <Transcode>
                        <Source NASCode="" File="" WTRSSPath="C:\TRSS\temp\202105252209112388780_35584528_src.K3G" BinaryData="\\172.17.107.5\nas_wtrsp\temp\2021052522091524324200_decode.tmp">
                        <Container ID="K3G" />
                        </Source>
                        <Target NASCode="" File="" WTRSSPath="C:\TRSS\temp\202105252209112693652_35583984_tar.K3G" BinaryData="AAAAGGZ0eXBrM2cxAAAAAWlAAtAAAAAAAAAAAAA5NTwAAAAEAAAAAAAAAAAAAAANFTk1PS1RGIAAAAAA="> <--이 부분 출력 간소화
                            <Container ID="K3G">
                                <VideoCodec Width="176" Height="144" MaxFrameRate="15" BitRate="256000" VariableFrameRate="0" ID="MPEG4" />
                                <AudioCodec Channel="1" SampleRate="8000" BitRate="12200" BitPerSample="16" ID="AMR" />
                            </Container>
                        </Target>
                    </Transcode>
                </Transcodes>
            </JobState>
        </JobStatusChangedNotify>
    </BODY>
</WTRS.MSG>
 */
			TiXmlNode* targetNode = bodyElement->
			FirstChild( "JobStatusChangedNotify" )->
		    FirstChild( "JobState" )->
		    FirstChild( "Transcodes" )->
		    FirstChild( "Transcode" )->
			FirstChild( "Target" );

			char* pBase64Data =
					(char*) targetNode->ToElement()->Attribute(
							"BinaryData");
			if(pBase64Data == NULL)
			{
				_rclsSkipXml = _rclsXml;
				return;
			}
			else if(KString::m_fnStrLen(pBase64Data) <= 1024)
			{
				_rclsSkipXml = _rclsXml;
				return;
			}
			else
			{
				KString clsBinaryInfo; clsBinaryInfo.m_fnReSize(64);
				KString::m_fnStrnCat((KSTR)clsBinaryInfo,clsBinaryInfo.m_unLen,"SKIP_BINARY_LEN:%d", KString::m_fnStrLen(pBase64Data));
				targetNode->ToElement()->SetAttribute("BinaryData", (KCSTR)clsBinaryInfo);//BinaryData replace

				TiXmlPrinter printer;
				printer.SetStreamPrinting();
				doc.Accept(&printer);
				_rclsSkipXml = printer.CStr();
			}
		}
		else
		{
			_rclsSkipXml = _rclsXml;
			return;
		}
	}
	else
	{
		_rclsSkipXml = _rclsXml;
		return;
	}
}
void AppXmlParser::m_fnXmlFormatter(KString & _rclsXml, KString & _rclsXmlFormat)
{
	TiXmlDocument doc;
	doc.Parse((KCSTR)_rclsXml, 0, TIXML_ENCODING_LEGACY);
	TiXmlPrinter printer;
	doc.Accept(&printer);
	_rclsXmlFormat = printer.CStr();
}
void AppXmlParser::m_fnSetTranscodesCnt(TiXmlElement* pclsBodyElement, CTranscodesCnt & _rclsTranscodesCnt)
{
	TiXmlElement* bodyTypeElement = pclsBodyElement->FirstChildElement();
	if(bodyTypeElement == NULL) return;

	if(KString::m_fnStrCmp(bodyTypeElement->Value(), DEF_JOB_STATUS_CHANGED_NOTIFY) == 0)
	{
		//JobState attr JobState.
		TiXmlElement* jobStateElement = bodyTypeElement->FirstChildElement("JobState");
		if (jobStateElement == NULL) return;

		CmpString clsStatus = jobStateElement->Attribute("Status");

		//Transcodes 의 Cnt 정보는 아래의 Status일 경우에만 설정한다.
		if ( (clsStatus == "JobStarted")
				|| (clsStatus == "TranscodingStarted")
				|| (clsStatus == "TranscodingStopped")
				|| (clsStatus == "JobStopped")
				|| (clsStatus == "Destroyed") )
		{
			TiXmlElement* transcodesElement = jobStateElement->FirstChildElement("Transcodes");
			if(transcodesElement == NULL) return;

			transcodesElement->SetAttribute("Cancel", _rclsTranscodesCnt.nCancel);
			transcodesElement->SetAttribute("Current", _rclsTranscodesCnt.nCurrent);
			transcodesElement->SetAttribute("Fail", _rclsTranscodesCnt.nFail);
			transcodesElement->SetAttribute("Success", _rclsTranscodesCnt.nSuccess);
			transcodesElement->SetAttribute("Total", _rclsTranscodesCnt.nTotal);
		}
	}
}
