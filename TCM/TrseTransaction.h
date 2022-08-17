#ifndef TRSE_TRANSACTION_H
#define TRSE_TRANSACTION_H
#include "StlMap.hxx"
#include "string.hxx"
#include "AppIf.h"
#include "AppXmlParser.h"

typedef enum
{
	E_TRSE_TR_ST_NONE = 0,
	E_TRSE_TR_ST_REQ_RCV,
	E_TRSE_TR_ST_RSP_SND,
	E_TRSE_TR_ST_NOTI_CREATED_SND,
	E_TRSE_TR_ST_NOTI_WAITING_SND,
	E_TRSE_TR_ST_NOTI_JOB_STARTED_SND,
	E_TRSE_TR_ST_NOTI_TC_STARTED_SND,
	E_TRSE_TR_ST_NOTI_TC_STOPPED_SND,
	E_TRSE_TR_ST_NOTI_JOB_STOPPED_SND,
	E_TRSE_TR_ST_NOTI_JOB_DESTROYED_SND,
	E_TRSE_TR_ST_MAX
} ETrseTranscState_t;
class TrseTransaction : public eSipUtil::StlObject
{
	public:
		TrseTransaction();
		~TrseTransaction();
		void * m_fnGetWorker();
		void m_fnRecvReq(eSipUtil::KString & _rclsReq, eSipUtil::KString & _rclsJobID);//CreateJob Req
		void m_fnSendRes(ETrsgCodeSet_t _eSt);//CreateJob Res
		/*recv JobStatusChangedNotify*/
		void m_fnSendNotiCreated(eSipUtil::KString & _rclsNotify);
		void m_fnSendNotiWaiting(eSipUtil::KString & _rclsNotify);
		void m_fnSendNotiJobStarted(eSipUtil::KString & _rclsNotify);
		void m_fnSendNotiTCStarted(eSipUtil::KString & _rclsNotify);
		void m_fnSendNotiTCSStopped(eSipUtil::KString & _rclsNotify, KString & _rclsResultCode);
		void m_fnSendNotiJobStopped(eSipUtil::KString & _rclsNotify);
		void m_fnSendNotiJobDestroy(eSipUtil::KString & _rclsNotify);
		void m_fnCallLog(bool bSend, eSipUtil::KString _clsXml);
		ETrseTranscState_t m_eSt;
		eSipUtil::Net5Tuple_t m_stTrseAddr;
		void * m_pclsOwner; // TrseTransactionMgr
		void * m_pvSession;
		eSipUtil::KString m_clsReq;
		eSipUtil::KString m_clsTrseName;
		eSipUtil::KString m_clsTrsgName;
		eSipUtil::KString m_clsJobID;
		eSipUtil::KString m_clsRes;
};

#endif
