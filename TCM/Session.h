#ifndef VWTRSG_SESSION_H
#define VWTRSG_SESSION_H
#include "StlMap.hxx"
#include "TrseTransaction.h"
#include "TrssTransaction.h"
#include "AclSystemTable.h"
#include "AppXmlParser.h"
#include "TrsgCdr.h"

typedef enum
{
	E_SESS_ST_NONE = 0,
	E_SESS_ST_TRSE_CREATE_JOB_REQ_RCV,
	E_SESS_ST_TRSS_CREATE_JOB_REQ_SND,
	//E_SESS_ST_TRSS_CREATE_JOB_RES_RCV, 자체적으로 trse로 응답함.(trss는 CreateJobRes를 보내주지 않음.)
	E_SESS_ST_TRSE_CREATE_JOB_RES_SND,
	E_SESS_ST_TRSS_JOB_CHG_NOTI_CREATED_RCV,
	E_SESS_ST_TRSE_JOB_CHG_NOTI_CREATED_SND,
	E_SESS_ST_TRSS_JOB_CHG_NOTI_WAITING_RCV,
	E_SESS_ST_TRSE_JOB_CHG_NOTI_WAITING_SND,
	E_SESS_ST_TRSS_JOB_CHG_NOTI_JOB_STARTED_RCV,
	E_SESS_ST_TRSE_JOB_CHG_NOTI_JOB_STARTED_SND,
	E_SESS_ST_TRSS_JOB_CHG_NOTI_TC_STARTED_RCV,
	E_SESS_ST_TRSE_JOB_CHG_NOTI_TC_STARTED_SND,
	E_SESS_ST_TRSS_JOB_CHG_NOTI_TC_STOPPED_RCV,
	E_SESS_ST_TRSE_JOB_CHG_NOTI_TC_STOPPED_SND,
	E_SESS_ST_TRSS_JOB_CHG_NOTI_JOB_STOPPED_RCV,
	E_SESS_ST_TRSE_JOB_CHG_NOTI_JOB_STOPPED_SND,
	E_SESS_ST_TRSS_JOB_CHG_NOTI_JOB_DESTROYED_RCV,
	E_SESS_ST_TRSE_JOB_CHG_NOTI_JOB_DESTROYED_SND,
	E_SESS_ST_TERMINATING,
	E_SESS_ST_MAX
}ESessState_t;

//T/C 의 결과에 따른 Cnt를 관리하기 위한 class
class CTranscodesCnt
{
public:
	CTranscodesCnt()
	{
		m_fnInit();
	}
	~CTranscodesCnt()
	{
		m_fnInit();
	}
	void m_fnInit()
	{
		nCancel = 0;// Cancel은 현재 사용되지 않으나 명시적으로 선언함.
		nFail = 0;// T/C Stop 의 결과가 실패일 경우 증가함.
		nCurrent = 0;// T/C Start 수신시마다 Current 증가.(JobStop 부터 다시  0)
		nSuccess = 0;// T/C Stop 의 결과가 성공일 경우 증가함.
		nTotal = 0;//CreateJobReq Transcoding list의 수를 저장함.
	}
	int nCancel;
	int nCurrent;
	int nFail;
	int nSuccess;
	int nTotal;
};
class Session : public eSipUtil::StlObject
{
	public:
		Session();
		~Session();
		/*CREATE_JOB_REQ*/
		void m_fnRecvTrseCrtJobReq(eSipUtil::KString & _rclsXml);
		void m_fnSendTrssCrtJobReq(eSipUtil::KString & _rclsXml, eSipUtil::KString & _rclsInternalNasFile, eSipUtil::KString & _rclsInternalNasImgFile);
		/*CREATE_JOB_RES*/
		void m_fnSendTrseCrtJobRes(eSipUtil::KString & _rclsXml, ETrsgCodeSet_t _eSt);
		/*JOB_STATUS_CHANGED_NOTIFY*/
		void m_fnRecvTrssJobChgNotiCreated(eSipUtil::KString & _rclsXml);
		void m_fnSendTrseJobChgNotiCreated(eSipUtil::KString & _rclsXml);
		void m_fnRecvTrssJobChgNotiWaiting(eSipUtil::KString & _rclsXml);
		void m_fnSendTrseJobChgNotiWaiting(eSipUtil::KString & _rclsXml);
		void m_fnRecvTrssJobChgNotiJobStarted(eSipUtil::KString & _rclsXml);
		void m_fnSendTrseJobChgNotiJobStarted(eSipUtil::KString & _rclsXml);
		void m_fnRecvTrssJobChgNotiTcStarted(eSipUtil::KString & _rclsXml);
		void m_fnSendTrseJobChgNotiTcStarted(eSipUtil::KString & _rclsXml);
		void m_fnRecvTrssJobChgNotiTcStopped(eSipUtil::KString & _rclsXml);
		void m_fnSendTrseJobChgNotiTcStopped(eSipUtil::KString & _rclsXml, KString & _rclsResultCode);
		void m_fnRecvTrssJobChgNotiJobStopped(eSipUtil::KString & _rclsXml);
		void m_fnSendTrseJobChgNotiJobStopped(eSipUtil::KString & _rclsXml);
		void m_fnRecvTrssJobChgNotiJobDestroyed(eSipUtil::KString & _rclsXml);
		void m_fnSendTrseJobChgNotiJobDestroyed(eSipUtil::KString & _rclsXml);
		/*RECV EVENT*/
		void m_fnRecvTrssTrTimeout();
		void m_fnRecvSessionTimeOut();
		void m_fnRecvTrseDisconSesClear();
		void m_fnGarbageClear();
		void m_fnCreateSessionEndEv();

		eSipUtil::KString m_fnGetFileName();
		eSipUtil::KString m_fnGetImgFileName();
		void * m_fnGetWorker();
		void * m_pclsOwner; // SessionMgr
		TrseTransaction * m_pclsTrseTr;
		TrssTransaction * m_pclsTrssTr;
		unsigned int m_unTid;
		eSipUtil::KString m_clsSessionID;
		eSipUtil::KString m_clsJobID;
		ESessState_t m_eSt;
		int m_nAllocSes;
		eSipUtil::KString m_clsInternalNasFileName;
		eSipUtil::KString m_clsInternalNasImgFileName;
		CTranscodesCnt m_clsTranscodesCnt;
		CTrsgCdrInfo * m_pclsCTrsgCdrInfo;
		int m_nAllocWorker;
};

#endif
