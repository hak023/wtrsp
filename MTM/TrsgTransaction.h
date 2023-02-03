#ifndef TRSG_TRANSACTION_H
#define TRSG_TRANSACTION_H
#include "StlMap.hxx"
#include "string.hxx"
#include "DailyTime.h"
#include "AppIf.h"
#include "TargetContent.h"
#include "SourceContent.h"

typedef enum
{
	E_TRSG_TR_ST_NONE = 0,
	E_TRSG_TR_ST_REQ_RCV, // CreatedJobReqeust
	E_TRSG_TR_ST_NOTI_CREATED_SND,
	E_TRSG_TR_ST_NOTI_WAITING_SND,
	E_TRSG_TR_ST_NOTI_JOB_STARTED_SND,
	E_TRSG_TR_ST_NOTI_TC_STARTED_SND,
	E_TRSG_TR_ST_NOTI_TC_STOPPED_SND,
	E_TRSG_TR_ST_NOTI_JOB_STOPPED_SND,
	E_TRSG_TR_ST_NOTI_JOB_DESTROYED_SND,
	E_TRSG_TR_ST_MAX
} ETrsgTranscState_t;
class TrsgTransaction: public eSipUtil::StlObject
{
public:
	TrsgTransaction();
	~TrsgTransaction();
	void* m_fnGetWorker();
	void m_fnRecvCrtJobReq(eSipUtil::KString &_rclsXml);
	void m_fnSendNotiCreated(eSipUtil::KString &_rclsXml, eSipUtil::KString &_rclsNotify);
	void m_fnSendNotiWaiting(eSipUtil::KString &_rclsXml, eSipUtil::KString &_rclsNotify);
	void m_fnSendNotiJobStarted(eSipUtil::KString &_rclsXml, unsigned int _unTranscodingList, eSipUtil::KString &_rclsNotify);
	void m_fnSendNotiTcStarted(eSipUtil::KString &_rclsXml, SourceContent * _pclsSource, TargetContent * _pclsTarget, unsigned int _unFail, unsigned int _unSuccess, unsigned int _unTargetIdx, eSipUtil::KString &_rclsNotify);
	void m_fnSendNotiTcStopped(eSipUtil::KString &_rclsXml, unsigned int _unFail, unsigned int _unSuccess, ETrssCodeSet_t _eSt,
								eSipUtil::KString &_rclsResultDescription, eSipUtil::KString &_rclsNotify);
	void m_fnSendNotiJobStopped(eSipUtil::KString &_rclsXml, eSipUtil::KString &_rclsNotify);
	void m_fnSendNotiJobDestroy(eSipUtil::KString &_rclsXml, unsigned int _unTranscodingList, ETrssCodeSet_t _eSt, eSipUtil::KString &_rclsNotify);
	void m_fnCallLog(bool bSend, eSipUtil::KString _clsXml);

	ETrsgTranscState_t m_eSt;
	eSipUtil::Net5Tuple_t m_stTrsgAddr;
	void * m_pclsOwner; // TrsgTransactionMgr
	eSipUtil::KString m_clsSessionID;
	// 특수문자 오류 해결
	eSipUtil::KString m_clsServiceName;
};

#endif
