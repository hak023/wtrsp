#ifndef VWTRSS_SESSION_H
#define VWTRSS_SESSION_H
#include "queueThread.hxx"
#include "StlMap.hxx"
#include "TrsgTransaction.h"
#include "NasSystemTable.h"
#include "AppXmlParser.h"
#include "TargetContent.h"
#include "SourceContent.h"
#include "ApiEvent.h"

using namespace eSipUtil;

typedef enum
{
	E_SESS_ST_NONE = 0,
	E_SESS_ST_TRSG_CREATE_JOB_REQ_RCV,
	E_SESS_ST_TRSG_JOB_CHG_NOTI_CREATED_SND,
	E_SESS_ST_TRSG_JOB_CHG_NOTI_WAITING_SND,
	E_SESS_ST_TRSG_JOB_CHG_NOTI_JOB_STARTED_SND,
	E_SESS_ST_TRSG_JOB_CHG_NOTI_TC_STARTED_SND,
	E_SESS_ST_TRSG_JOB_CHG_NOTI_TC_STOPPED_SND,
	E_SESS_ST_TRSG_JOB_CHG_NOTI_JOB_STOPPED_SND,
	E_SESS_ST_TRSG_JOB_CHG_NOTI_JOB_DESTROYED_SND,
	E_SESS_ST_TERMINATING,
	E_SESS_ST_MAX
} ESessState_t;

class Session: public StlObject
{
public:
	Session();
	~Session();
	/*CREATE_JOB_REQ*/
	void m_fnRecvTrsgCrtJobReq(KString &_rclsXml);
	/*JOB_STATUS_CHANGED_NOTIFY*/
	void m_fnSendTrsgJobChgNotiCreated();
	void m_fnSendTrsgJobChgNotiWaiting();
	void m_fnSendTrsgJobChgNotiJobStarted();
	void m_fnSendTrsgJobChgNotiTcStarted(TargetContent* _pclsTarget, unsigned int _unFail, unsigned int _unSuccess, unsigned int _unTargetIdx);
	void m_fnSendTrsgJobChgNotiTcStopped(unsigned int _unFail, unsigned int _unSuccess,
											ETrssCodeSet_t _eSt, KString &_rclsResultDescription);
	void m_fnSendTrsgJobChgNotiJobStopped();
	void m_fnSendTrsgJobChgNotiJobDestroyed();
	void m_fnProcTrsgTranscoding();

	KString m_fnGetFileName();
	void * m_fnGetWorker();
	ETrssCodeSet_t m_fnExecuteTranscoding(KString & _rclsTcCommand, KString &_rclsSourceContainerID, TargetContent* _pclsTarget, KString & _rclsResultDescription);
	/* RECV MSG */
	void m_fnRecvSessionTimeOut();
	void m_fnCreateSessionEndEv();

	// ÇÊ¿ä¾øÀ½
//	void m_fnRecvTrsgTrTimeout();
//	void m_fnRecvTrsgDisconSesClear();	// Trsg ²÷°åÀ»¶§ Session Clear
//	void m_fnGarbageClear();	// TrsgCdr ¿ë

	void * m_pclsOwner;
	TrsgTransaction * m_pclsTrsgTr;
	unsigned int m_unTid;
	KString m_clsSessionID;
	KString m_clsJobID;
	ESessState_t m_eSt;
	int m_nAllocSes;
	KString m_clsLastSendXml;
	SourceContent* m_pclsSourceContent;
	KString m_clsImageData;
	KString m_clsMetaImage;
	KString m_clsMetaCommand;
	unsigned int m_unTranscodingList;
	unsigned int m_unTargetContentList;
	eSipUtil::StlList m_clsTargetList;
	eSipUtil::StlList m_clsCodecList;
	KString m_clsInputFile;		// NasCode+/+SrcPath+/+SrcFile
	ETrssCodeSet_t m_eSrcSt;
	// Worker IdMgr
	int m_nAllocWorker;

private:
	// Session QueueThread
	static void m_fnCbkProcess(AppBaseEvent *_pclsEv, void *_pvOwner);
	void m_fnCreateTrsgEv(EAppEvent_t _eT);
	void m_fnPutSessionTcEv(AppTrsgEvent *_pclsEv);
	eSipUtil::QueueThread<AppBaseEvent> *m_pclsQ;
};

#endif
