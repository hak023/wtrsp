#ifndef TRSS_TRANSACTION_H
#define TRSS_TRANSACTION_H
#include "StlMap.hxx"
#include "string.hxx"
#include "AppIf.h"

typedef enum
{
	E_TRSS_TR_ST_NONE = 0,
	E_TRSS_TR_ST_REQ_SND,//CreateJobRequest
	E_TRSS_TR_ST_NOTI_CREATED_RCV,
	E_TRSS_TR_ST_NOTI_WAITING_RCV,
	E_TRSS_TR_ST_NOTI_JOB_STARTED_RCV,
	E_TRSS_TR_ST_NOTI_TC_STARTED_RCV,
	E_TRSS_TR_ST_NOTI_TC_STOPPED_RCV,
	E_TRSS_TR_ST_NOTI_JOB_STOPPED_RCV,
	E_TRSS_TR_ST_NOTI_JOB_DESTROYED_RCV,
	E_TRSS_TR_ST_MAX
} ETrssTranscState_t;

class TrssTransaction : public eSipUtil::StlObject
{
	public:
		TrssTransaction();
		~TrssTransaction();
		void * m_fnGetWorker();
		bool m_fnSendReq(eSipUtil::KString & _rclsReq, eSipUtil::KString & _rclsJobID, eSipUtil::KString & _rclsInternalNasFileName, eSipUtil::KString & _rclsInternalNasImgFileName);//CreateJob Req
		/*recv JobStatusChangedNotify*/
		void m_fnRecvNotiCreated(eSipUtil::KString & _rclsNotify);
		void m_fnRecvNotiWaiting(eSipUtil::KString & _rclsNotify);
		void m_fnRecvNotiJobStarted(eSipUtil::KString & _rclsNotify);
		void m_fnRecvNotiTCStarted(eSipUtil::KString & _rclsNotify);
		void m_fnRecvNotiTCSStopped(eSipUtil::KString & _rclsNotify);
		void m_fnRecvNotiJobStopped(eSipUtil::KString & _rclsNotify);
		void m_fnRecvNotiJobDestroy(eSipUtil::KString & _rclsNotify);
		void m_fnCallLog(bool bSend, eSipUtil::KString _clsXml);
		ETrssTranscState_t m_eSt;
		eSipUtil::Net5Tuple_t m_stTrssAddr;
		void * m_pclsOwner; // TrssTransactionMgr
		void * m_pvSession;
		eSipUtil::KString m_clsReq;
		eSipUtil::KString m_clsJobID;
		eSipUtil::KString m_clsRes;
		eSipUtil::KString m_clsTrsgName;
		eSipUtil::KString m_clsTrssName;
		eSipUtil::KString m_clsSessionID;
};

#endif
