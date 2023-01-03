#ifndef TRSG_CDR_H
#define TRSG_CDR_H
#include "TrsgCdrMgr.h"
#include "rutil/Data.hxx"
#include "main.h"
#include "AppXmlParser.h"
#include "TargetContent.h"

using namespace eSipUtil;

class TrsgCdr
{
	public :
		TrsgCdr();
		~TrsgCdr();
		static TrsgCdr * m_fnGetInstance();
		void m_fnAddCdr(eSipUtil::KString& _rclsCdrMsg);

	private :
		static TrsgCdr * m_pclsMy;
		TrsgCdrMgr m_clsTrsgCdrMgr;
};
TrsgCdr * g_fnCreateTrsgCdr();

class CdrRow: public eSipUtil::StlObject
{
public:
	CdrRow(){m_clsLogType="SVC";m_clsSID="000000000000";m_clsSvcName="WTRSP";m_nInputNasCode=0;m_nOutputNasCode=0;}
	~CdrRow(){}
	//default
	KString m_clsSeqID;
	KString m_clsLogTime;
	KString m_clsLogType;//SVC 고정
	KString m_clsSID;//000000000000 고정
	KString m_clsResultCode;
	KString m_clsReqTime;
	KString m_clsResTime;
	KString m_clsClientIP;
	KString m_clsSvcName;//WTRSP 고정
	KString m_clsFrom;
	KString m_clsTrsgIP;
	KString m_clsTrssIP;
	KString m_clsTransType;
	KString m_clsTargetAudioCodec;
	KString m_clsTargetVideoCodec;

	KString m_clsSessionID;
	KString m_clsTransSessionID;
	KString m_clsFileType;

	int m_nInputNasCode;
	KString m_clsInputPath;
	KString m_clsInputContainer;

	int m_nOutputNasCode;
	KString m_clsOutputPath;
	KString m_clsOutputContainer;

	//flow
	KString m_clsRecvTrseCrtJobReq;
	KString m_clsSendTrssCrtJobReq;
	KString m_clsSendTrseCrtJobRes;
	KString m_clsRecvTrssNotiCreated;
	KString m_clsSendTrseNotiCreated;
	KString m_clsRecvTrssNotiWaiting;
	KString m_clsSendTrseNotiWaiting;
	KString m_clsRecvTrssNotiJobStarted;
	KString m_clsSendTrseNotiJobStarted;
	KString m_clsRecvTrssNotiTcStarted;
	KString m_clsSendTrseNotiTcStarted;
	KString m_clsRecvTrssNotiTcStopped;
	KString m_clsSendTrseNotiTcStopped;
	KString m_clsRecvTrssNotiJobStopped;
	KString m_clsSendTrseNotiJobStopped;
	KString m_clsRecvTrssNotiJobDestroyed;
	KString m_clsSendTrseNotiJobDestroyed;

	void m_fnMakeRowCdr(eSipUtil::KString & _rclsCdrInfo)
	{
		_rclsCdrInfo <<
		//default
		"SEQ_ID=" << m_clsSeqID<<
		"|LOG_TIME=" << m_clsLogTime<<
		"|LOG_TYPE=" << m_clsLogType<<
		"|SID=" << m_clsSID<<
		"|RESULT_CODE=" << m_clsResultCode<<
		"|REQ_TIME=" << m_clsReqTime<<
		"|RSP_TIME=" << m_clsResTime<<
		"|CLIENT_IP=" << m_clsClientIP<<
		"|DEV_INFO="<<
		"|OS_INFO="<<
		"|NW_INFO="<<
		"|SVC_NAME="<< m_clsSvcName<<
		"|DEV_MODEL="<<
		"|CARRIER_TYPE="<<
		"|FROM=" << m_clsFrom<<
		"|TRSG_IP=" << m_clsTrsgIP<<
		"|TRSS_IP=" << m_clsTrssIP<<
		"|TRANS_TYPE=" << m_clsTransType<<
		"|TARGET_ACODEC=" << m_clsTargetAudioCodec<<
		"|TARGET_VCODEC=" << m_clsTargetVideoCodec<<

		"|SESSION_ID=" << m_clsSessionID<<
		"|TRANS_SESSION_ID=" << m_clsTransSessionID<<
		"|FILE_TYPE=" << m_clsFileType;

		if (m_nInputNasCode != 0)	_rclsCdrInfo<<"|INPUT_NAS_CODE=" << m_nInputNasCode;
		else 						_rclsCdrInfo<<"|INPUT_NAS_CODE=";
		_rclsCdrInfo<<
		"|INPUT_PATH=" << m_clsInputPath<<
		"|INPUT_CONTAINER=" << m_clsInputContainer;

		if (m_nOutputNasCode != 0)	_rclsCdrInfo<<"|OUTPUT_NAS_CODE=" << m_nOutputNasCode;
		else 						_rclsCdrInfo<<"|OUTPUT_NAS_CODE=";
		_rclsCdrInfo<<
		"|OUTPUT_PATH=" << m_clsOutputPath<<
		"|OUTPUT_CONTAINER=" << m_clsOutputContainer<<

		//flow
		"|TRSE_CREATE_JOB_REQ_RCV=" << m_clsRecvTrseCrtJobReq<<
		"|TRSS_CREATE_JOB_REQ_SND=" << m_clsSendTrssCrtJobReq<<
		"|TRSE_CREATE_JOB_RES_SND=" << m_clsSendTrseCrtJobRes<<
		"|TRSS_NOTI_CREATED_RCV=" << m_clsRecvTrssNotiCreated<<
		"|TRSE_NOTI_CREATED_SND=" << m_clsSendTrseNotiCreated<<
		"|TRSS_NOTI_WAITING_RCV=" << m_clsRecvTrssNotiWaiting<<
		"|TRSE_NOTI_WAITING_SND=" << m_clsSendTrseNotiWaiting<<
		"|TRSS_NOTI_JOB_STARTED_RCV=" << m_clsRecvTrssNotiJobStarted<<
		"|TRSE_NOTI_JOB_STARTED_SND=" << m_clsSendTrseNotiJobStarted<<
		"|TRSS_NOTI_TC_STARTED_RCV=" << m_clsRecvTrssNotiTcStarted<<
		"|TRSE_NOTI_TC_STARTED_SND=" << m_clsSendTrseNotiTcStarted<<
		"|TRSS_NOTI_TC_STOPPED_RCV=" << m_clsRecvTrssNotiTcStopped<<
		"|TRSE_NOTI_TC_STOPPED_SND=" << m_clsSendTrseNotiTcStopped<<
		"|TRSS_NOTI_JOB_STOPPED_RCV=" << m_clsRecvTrssNotiJobStopped<<
		"|TRSE_NOTI_JOB_STOPPED_SND="<< m_clsSendTrseNotiJobStopped<<
		"|TRSS_NOTI_JOB_DESTROYED_RCV="<< m_clsRecvTrssNotiJobDestroyed<<
		"|TRSE_NOTI_JOB_DESTROYED_SND="<< m_clsSendTrseNotiJobDestroyed;
	}
};
class CTrsgCdrInfo
{
public:
	CTrsgCdrInfo()
	{
		m_bWrite=false;
		m_listCdrRow.m_fnClear();
		m_unTranscodingList=0;
		m_unTrssTCStartIdx=0;
		m_unTrseTCStartIdx=0;
		m_unTrssTCStopIdx=0;
		m_unTrseTCStopIdx=0;
	}
	~CTrsgCdrInfo()
	{
		m_listCdrRow.m_fnClear();
	}
	void m_fnRecvTrseCrtJobReq(eSipUtil::KString & _rclsXml, eSipUtil::KString & _rclsSessionID, eSipUtil::KString & _rclsTrsgIP, eSipUtil::KString & _rclsClientIP, unsigned int _unTranscodingList)
	{
		m_bWrite=true;
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent);
		KString clsFormatCurrent = clsCurrent;
		m_fnConvertTimeFormat(clsFormatCurrent);
		CdrRow * pclsCdrRow = NULL;
		m_unTranscodingList = _unTranscodingList;

		KString clsTo, clsFrom;
		AppXmlParser::m_fnGetToFrom(_rclsXml, clsTo, clsFrom);
		resip::Data dataFrom = (KSTR)clsFrom;
		//urn:wtrsp:auth.endpoint: 제거 후 대문자 변환
		dataFrom.replace("urn:wtrsp:auth.endpoint:","");
		clsFrom = dataFrom.uppercase().c_str();

		for(unsigned int i=0; i<m_unTranscodingList; i++)
		{
			pclsCdrRow = new CdrRow;
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsReqTime,17+2,"%s",(KCSTR)clsCurrent);
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsFrom,64+2,"%s",(KCSTR)clsFrom);
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsTrsgIP,15+2,"%s",(KCSTR)_rclsTrsgIP);
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsClientIP,15+2,"%s",(KCSTR)_rclsClientIP);
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsSessionID,38+2,"%s",(KCSTR)_rclsSessionID);
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsRecvTrseCrtJobReq,12+2,"%s",(KCSTR)clsFormatCurrent);
			m_listCdrRow.m_fnPushBack(pclsCdrRow);
		}
	}
	void m_fnSetInputData(int _nSrcNasCode, eSipUtil::KString & _rclsPath, eSipUtil::KString & _rclsFile, eSipUtil::KString & _rclsContainer)
	{
		KString clsFullPath;
		if(_rclsPath.m_unRealLen > 0)
		{
			clsFullPath = _rclsPath;
			if( ((KCSTR)clsFullPath)[clsFullPath.m_unRealLen-1] != '/') clsFullPath.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.
			clsFullPath.m_fnCat((KCSTR)_rclsFile);
		}
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			if(_nSrcNasCode > 0) pclsCdrRow->m_nInputNasCode = _nSrcNasCode;
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsInputPath,128+2,"%s",(KCSTR)clsFullPath);
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsInputContainer,8+2,"%s",(KCSTR)_rclsContainer);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnSetOutputData(eSipUtil::KString & _rclsXml)
	{
		StlList clsTargetList;
		AppXmlParser::m_fnGetTargetContentList(_rclsXml, &clsTargetList);

		Iterator stItor1; TargetContent * pclsTargetContent = (TargetContent*)clsTargetList.m_fnGetNext(stItor1);
		Iterator stItor2; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor2);

		while(pclsCdrRow && pclsTargetContent)
		{
			KString clsFullPath;
			if(pclsTargetContent->m_clsPath.m_unRealLen > 0)
			{
				clsFullPath = pclsTargetContent->m_clsPath;
				if( ((KCSTR)clsFullPath)[clsFullPath.m_unRealLen-1] != '/') clsFullPath.m_fnCat("/");  //마지막 경로 / 없을 때 예외처리.
				clsFullPath.m_fnCat((KCSTR)pclsTargetContent->m_clsFile);
			}

			//TargetContent 파싱한 김에 여기서 요청 구분을 설정함.
			//변환 요청 구분 (1:동영상, 2:이미지, 3:폰트, 4:음원)

			if(pclsTargetContent->m_clsContainer.m_clsID == "JPEG"
					|| pclsTargetContent->m_clsContainer.m_clsID == "BMP"
					|| pclsTargetContent->m_clsContainer.m_clsID == "PNG"
					|| pclsTargetContent->m_clsContainer.m_clsID == "GIF") //이미지 (단, 상용 로그에는 JPEG만 존재 함)
			{
				KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsTransType,1+2,"%d", 2);
			}
			else if(pclsTargetContent->m_clsContainer.m_clsID == "MDF"
					|| pclsTargetContent->m_clsContainer.m_clsID == "UTF")//폰트(단, 요청에 따라 MDF와  UTF만 폰트로 취급함)
			{
				KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsTransType,1+2,"%d", 3);
			}
			else if(pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsID.m_unRealLen > 0  && !(pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsID == "NONE"))//동영상
			{
				//비디오 코덱의 아이디가 존재하면 비디오 변환 요청.
				KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsTransType,1+2,"%d", 1);
			}
			else//그 외 음원으로 취급.
			{
				KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsTransType,1+2,"%d", 4);
			}

			pclsCdrRow->m_nOutputNasCode = (KINT)pclsTargetContent->m_clsNasCode;
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsOutputPath,128+2,"%s",(KCSTR)clsFullPath);
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsOutputContainer,8+2,"%s",(KCSTR)pclsTargetContent->m_clsContainer.m_clsID);

			//Target Audio / Video ID Set.
			if(pclsTargetContent->m_clsContainer.clsAudioCodec.m_clsID.m_unRealLen > 0 && !(pclsTargetContent->m_clsContainer.clsAudioCodec.m_clsID == "NONE"))
			{
				KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsTargetAudioCodec,12+2,"%s",(KCSTR)pclsTargetContent->m_clsContainer.clsAudioCodec.m_clsID);
			}
			if(pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsID.m_unRealLen > 0 && !(pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsID == "NONE"))
			{
				KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsTargetVideoCodec,12+2,"%s",(KCSTR)pclsTargetContent->m_clsContainer.clsVideoCodec.m_clsID);
			}

			pclsTargetContent = (TargetContent*)clsTargetList.m_fnGetNext(stItor1);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor2);
		}
	}
	void m_fnSetFileType(eSipUtil::KString & _rclsFileType)
	{
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsFileType,3+2,"%s",(KCSTR)_rclsFileType);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnSetTrssIP(eSipUtil::KString & _rclsTrssIP)
	{
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsTrssIP,15+2,"%s",(KCSTR)_rclsTrssIP);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnSendTrssCrtJobReq(eSipUtil::KString & _rclsXml)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsSendTrssCrtJobReq,17+2,"%s",(KCSTR)clsCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	/*CREATE_JOB_RES*/
	void m_fnSendTrseCrtJobRes(eSipUtil::KString & _rclsXml, ETrsgCodeSet_t _eSt)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent);
		KString clsFormatCurrent = clsCurrent;
		m_fnConvertTimeFormat(clsFormatCurrent);

		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsResTime,17+2,"%s",(KCSTR)clsCurrent);
			if(pclsCdrRow->m_clsResultCode.m_unRealLen == 0 || pclsCdrRow->m_clsResultCode == (KCSTR)g_fnGetCdrCode(E_JOB_RES_OK))
				pclsCdrRow->m_clsResultCode = (KCSTR)g_fnGetCdrCode(_eSt);
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsSendTrseCrtJobRes,12+2,"%s",(KCSTR)clsFormatCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	/*CREATE_JOB_RES Fail*/
	void m_fnSendTrseCrtJobResFail(eSipUtil::KString & _rclsXml, ETrsgCodeSet_t _eSt)
	{
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			if(pclsCdrRow->m_clsResultCode.m_unRealLen == 0 || pclsCdrRow->m_clsResultCode == (KCSTR)g_fnGetCdrCode(E_JOB_RES_OK))
				pclsCdrRow->m_clsResultCode = (KCSTR)g_fnGetCdrCode(_eSt);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	/*JOB_STATUS_CHANGED_NOTIFY*/
	void m_fnRecvTrssJobChgNotiCreated(eSipUtil::KString & _rclsXml)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsRecvTrssNotiCreated,12+2,"%s",(KCSTR)clsCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnSendTrseJobChgNotiCreated(eSipUtil::KString & _rclsXml)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsSendTrseNotiCreated,12+2,"%s",(KCSTR)clsCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnRecvTrssJobChgNotiWaiting(eSipUtil::KString & _rclsXml)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsRecvTrssNotiWaiting,12+2,"%s",(KCSTR)clsCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnSendTrseJobChgNotiWaiting(eSipUtil::KString & _rclsXml)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsSendTrseNotiWaiting,12+2,"%s",(KCSTR)clsCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnRecvTrssJobChgNotiJobStarted(eSipUtil::KString & _rclsXml)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsRecvTrssNotiJobStarted,12+2,"%s",(KCSTR)clsCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnSendTrseJobChgNotiJobStarted(eSipUtil::KString & _rclsXml)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsSendTrseNotiJobStarted,12+2,"%s",(KCSTR)clsCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnRecvTrssJobChgNotiTcStarted(eSipUtil::KString & _rclsXml, eSipUtil::KString & _rclsTransSessionID)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		if(m_unTrssTCStartIdx >= m_unTranscodingList) return;

		KString::m_fnStrnCat((KSTR)((CdrRow*)m_listCdrRow.m_fnGetIndex(m_unTrssTCStartIdx))->m_clsTransSessionID,42+2,"%s",(KCSTR)_rclsTransSessionID);
		KString::m_fnStrnCat((KSTR)((CdrRow*)m_listCdrRow.m_fnGetIndex(m_unTrssTCStartIdx))->m_clsRecvTrssNotiTcStarted,12+2,"%s",(KCSTR)clsCurrent);
		m_unTrssTCStartIdx++;
	}
	void m_fnSendTrseJobChgNotiTcStarted(eSipUtil::KString & _rclsXml)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);

		if(m_unTrseTCStartIdx >= m_unTranscodingList) return;
		KString::m_fnStrnCat((KSTR)((CdrRow*)m_listCdrRow.m_fnGetIndex(m_unTrseTCStartIdx))->m_clsSendTrseNotiTcStarted,12+2,"%s",(KCSTR)clsCurrent);
		m_unTrseTCStartIdx++;
	}
	void m_fnRecvTrssJobChgNotiTcStopped(eSipUtil::KString & _rclsXml, eSipUtil::KString & _rclsResultCode)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		if(m_unTrssTCStopIdx >= m_unTranscodingList) return;

		if(((CdrRow*)m_listCdrRow.m_fnGetIndex(m_unTrssTCStopIdx))->m_clsResultCode.m_unRealLen == 0 || ((CdrRow*)m_listCdrRow.m_fnGetIndex(m_unTrssTCStopIdx))->m_clsResultCode == (KCSTR)g_fnGetCdrCode(E_JOB_RES_OK))
			((CdrRow*)m_listCdrRow.m_fnGetIndex(m_unTrssTCStopIdx))->m_clsResultCode = g_fnGetCdrCode((ETrsgCodeSet_t)g_fnGetEnumIdx((KCSTR)_rclsResultCode));
		KString::m_fnStrnCat((KSTR)((CdrRow*)m_listCdrRow.m_fnGetIndex(m_unTrssTCStopIdx))->m_clsRecvTrssNotiTcStopped,12+2,"%s",(KCSTR)clsCurrent);
		m_unTrssTCStopIdx++;
	}
	void m_fnSendTrseJobChgNotiTcStopped(eSipUtil::KString & _rclsXml, bool _bRteEncode)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		if(m_unTrseTCStopIdx >= m_unTranscodingList) return;
		KString::m_fnStrnCat((KSTR)((CdrRow*)m_listCdrRow.m_fnGetIndex(m_unTrseTCStopIdx))->m_clsSendTrseNotiTcStopped,12+2,"%s",(KCSTR)clsCurrent);//09:43:40.663
		if(_bRteEncode == false)//binary to base64 변환 실패 시 ResultCode 업데이트.
		{
			if(((CdrRow*)m_listCdrRow.m_fnGetIndex(m_unTrseTCStopIdx))->m_clsResultCode.m_unRealLen == 0 || ((CdrRow*)m_listCdrRow.m_fnGetIndex(m_unTrseTCStopIdx))->m_clsResultCode == (KCSTR)g_fnGetCdrCode(E_JOB_RES_OK))
				((CdrRow*)m_listCdrRow.m_fnGetIndex(m_unTrseTCStopIdx))->m_clsResultCode = g_fnGetCdrCode(E_TCP_BINARY_ENCODING_FAILED);
		}
		m_unTrseTCStopIdx++;
	}
	void m_fnRecvTrssJobChgNotiJobStopped(eSipUtil::KString & _rclsXml)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsRecvTrssNotiJobStopped,12+2,"%s",(KCSTR)clsCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnSendTrseJobChgNotiJobStopped(eSipUtil::KString & _rclsXml)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsSendTrseNotiJobStopped,12+2,"%s",(KCSTR)clsCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnRecvTrssJobChgNotiJobDestroyed(eSipUtil::KString & _rclsXml, eSipUtil::KString & _rclsResultCode)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			if(pclsCdrRow->m_clsResultCode.m_unRealLen == 0 || pclsCdrRow->m_clsResultCode == (KCSTR)g_fnGetCdrCode(E_JOB_RES_OK))
			{
				//정상적인 Flow가 수행되었으면, Result Code가 비어있을 수 없음. 해당 로그가 찍히면 Flow 분석 필요함.
				if(_rclsResultCode == (KCSTR)g_fnGetTrsgCode(E_JOB_IS_LIMITED) )
				{
					pclsCdrRow->m_clsResultCode = (KCSTR)g_fnGetCdrCode(E_JOB_IS_LIMITED);//TRSS 의 Session 이 Full.
					IFLOG(E_LOG_ERR, "[TrsgCdr] Result code empty and TRSS Session Full");
				}
				else
				{
					pclsCdrRow->m_clsResultCode = g_fnGetCdrCode((ETrsgCodeSet_t)g_fnGetEnumIdx((KCSTR)_rclsResultCode));
				}
			}
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsRecvTrssNotiJobDestroyed,12+2,"%s",(KCSTR)clsCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnSendTrseJobChgNotiJobDestroyed(eSipUtil::KString & _rclsXml)
	{
		KString clsCurrent; m_fnGetCurrentTime(clsCurrent); m_fnConvertTimeFormat(clsCurrent);
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsSendTrseNotiJobDestroyed,12+2,"%s",(KCSTR)clsCurrent);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnSendTrssCrtJobReqFail(eSipUtil::KString & _rclsXml)
	{
		//TRSS로 CrtJobReq 전송이 실패할 경우(TCP 전송에 대한 실패)
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			if(pclsCdrRow->m_clsResultCode.m_unRealLen == 0 || pclsCdrRow->m_clsResultCode == (KCSTR)g_fnGetCdrCode(E_JOB_RES_OK))
				pclsCdrRow->m_clsResultCode = (KCSTR)g_fnGetCdrCode(E_TRSS_TCP_REQUEST_FAILED);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnRecvTrssJobCreatedTimeout()
	{
		//TRSS가 Created Noti를 주지 않을 경우
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			if(pclsCdrRow->m_clsResultCode.m_unRealLen == 0 || pclsCdrRow->m_clsResultCode == (KCSTR)g_fnGetCdrCode(E_JOB_RES_OK))
				pclsCdrRow->m_clsResultCode = (KCSTR)g_fnGetCdrCode(E_JOB_CREATED_TIMEOUT);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnRecvSessionTimeOut()
	{
		//JOB Trascode Time Out, TRSS 변환 요청은 했으나, 응답 및 시간 초과
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			if(pclsCdrRow->m_clsResultCode.m_unRealLen == 0 || pclsCdrRow->m_clsResultCode == (KCSTR)g_fnGetCdrCode(E_JOB_RES_OK))
				pclsCdrRow->m_clsResultCode = (KCSTR)g_fnGetCdrCode(E_JOB_TRANSCODE_TIMEOUT);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnRecvTrseDisconSesClear()
	{
		//TRSE가 접속 종료 되었을 경우
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			if(pclsCdrRow->m_clsResultCode.m_unRealLen == 0 || pclsCdrRow->m_clsResultCode == (KCSTR)g_fnGetCdrCode(E_JOB_RES_OK))
				pclsCdrRow->m_clsResultCode = (KCSTR)g_fnGetCdrCode(E_TRSE_TCP_SESSION_CLOSE);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnGarbageClear()
	{
		//Garbage 에 걸린 Session의 경우 CDR 기록 후 무조건 삭제함.
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			if(pclsCdrRow->m_clsResultCode.m_unRealLen == 0 || pclsCdrRow->m_clsResultCode == (KCSTR)g_fnGetCdrCode(E_JOB_RES_OK))
				pclsCdrRow->m_clsResultCode = (KCSTR)g_fnGetCdrCode(E_JOB_TRANSCODE_TIMEOUT);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
	}
	void m_fnGetCurrentTime(KString & _rclsCurrent)
	{
		struct timespec now_monotonic;
		clock_gettime(CLOCK_REALTIME, &now_monotonic);

		struct tm tmTime;
		localtime_r((time_t *)&now_monotonic,&tmTime);

		//ex) 2022051214173122526123  //Len:22
		//parse) 2022-05-12 14:17:31.22526123
		KString::m_fnStrnCat((KSTR)_rclsCurrent,_rclsCurrent.m_unLen,"%04d%02d%02d%02d%02d%02d%08lld", tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
				tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec, now_monotonic.tv_nsec/10);
	}
	void m_fnConvertTimeFormat(KString & _rclsCurrent)
	{
		resip::Data clsformat = (KCSTR)_rclsCurrent;
		_rclsCurrent = KNULL;
		//2022051709314464832231 -> 09:31:44.648
		_rclsCurrent << clsformat.substr(8, 2).c_str() << ":"
				<< clsformat.substr(10, 2).c_str() << ":"
				<< clsformat.substr(12, 2).c_str() << "."
				<< clsformat.substr(14, 3).c_str();
	}
	void m_fnMakeCdrList(KString & _rclsJobID)
	{
		if(m_bWrite == false) return;
		TrsgCdr* pTrsgCdr = TrsgCdr::m_fnGetInstance();
		Iterator stItor; CdrRow * pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		while(pclsCdrRow)
		{
			KString clsCurrent; m_fnGetCurrentTime(clsCurrent);
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsSeqID,21+2,"%s",(KCSTR)clsCurrent);
			KString::m_fnStrnCat((KSTR)pclsCdrRow->m_clsLogTime,17+2,"%s",(KCSTR)clsCurrent);
			KString clsCdrString; pclsCdrRow->m_fnMakeRowCdr(clsCdrString);
			IFLOG(E_LOG_DEBUG, "%s|JOB_ID=%s", (KCSTR)clsCdrString, (KCSTR)_rclsJobID);
			pTrsgCdr->m_fnAddCdr(clsCdrString);
			pclsCdrRow = (CdrRow*)m_listCdrRow.m_fnGetNext(stItor);
		}
		m_listCdrRow.m_fnClear();
		m_bWrite = false;
	}

	bool m_bWrite;
	unsigned int m_unTranscodingList;//CreateJobRequest 의 Transcoding 엘리먼트 개수
	unsigned int m_unTrssTCStartIdx;
	unsigned int m_unTrseTCStartIdx;
	unsigned int m_unTrssTCStopIdx;
	unsigned int m_unTrseTCStopIdx;
	eSipUtil::StlList m_listCdrRow;
};
#endif

