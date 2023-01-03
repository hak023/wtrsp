#ifndef API_EVENT_H
#define API_EVENT_H
#include "string.hxx"
#include "transportMgr.hxx"
#include "MainConfig.h"
#include "TargetContent.h"
typedef enum
{
	E_MAIN_EV_NONE = 0,
	E_MAIN_EV_TRSG_CREATE_JOB_REQ,
	E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_CREATED,
	E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_WAITING,
	E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED,
	E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED,
	E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED,
	E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED,
	E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED,
	E_MAIN_EV_SESSION_END,
	E_MAIN_EV_TRSG_TR_TIMER,
	E_MAIN_EV_SESSION_TIMEOUT,
	E_MAIN_EV_GARBAGE_TIMER,
	E_MAIN_EV_SESSION_TC_TIMER,
	E_MAIN_EV_MAX
} EAppEvent_t;
inline const char* g_fnGetAppString(EAppEvent_t _eT)
{
	switch (_eT)
	{
	case E_MAIN_EV_TRSG_CREATE_JOB_REQ:
		return "E_MAIN_EV_TRSG_CREATE_JOB_REQ";
	case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_CREATED:
		return "E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_CREATED";
	case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_WAITING:
		return "E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_WAITING";
	case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED:
		return "E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED";
	case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED:
		return "E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED";
	case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED:
		return "E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED";
	case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED:
		return "E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED";
	case E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED:
		return "E_MAIN_EV_TRSG_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED";
	case E_MAIN_EV_SESSION_END:
		return "E_MAIN_EV_SESSION_END";
	case E_MAIN_EV_TRSG_TR_TIMER:
		return "E_MAIN_EV_TRSG_TR_TIMER";
	case E_MAIN_EV_SESSION_TIMEOUT:
		return "E_MAIN_EV_SESSION_TIMEOUT";
	case E_MAIN_EV_GARBAGE_TIMER:
		return "E_MAIN_EV_GARBAGE_TIMER";
	case E_MAIN_EV_SESSION_TC_TIMER:
		return "E_MAIN_EV_SESSION_TC_TIMER";
	default:
		return "E_MAIN_EV_NONE";
	};
	return "E_MAIN_EV_NONE";
}
class AppBaseEvent
{
public:
	AppBaseEvent()
	{
		m_eT = E_MAIN_EV_NONE;
		memset(&m_stAddr, 0x00, sizeof(eSipUtil::Net5Tuple_t));
	}
	virtual ~AppBaseEvent()
	{
	}
	const char* m_fnGetAddrString()
	{
		if (m_clsAddr.m_unRealLen == 0)
		{
			m_clsAddr << m_stAddr.m_szLocalIp << m_stAddr.m_nLocalPort;
			m_clsAddr << m_stAddr.m_szRemoteIp << m_stAddr.m_nRemotePort;
		}
		return (eSipUtil::KCSTR) m_clsAddr;
	}
	EAppEvent_t m_eT;
	eSipUtil::KString m_clsAddr;
	eSipUtil::Net5Tuple_t m_stAddr;
};

class AppTrsgEvent: public AppBaseEvent
{
public:
	AppTrsgEvent()
	{
		m_unTid = 0;
		m_clsSessionID = KNULL;
		m_clsJobID = KNULL;
		m_clsXml = KNULL;
	}
	~AppTrsgEvent() {}
	unsigned int m_unTid;
	eSipUtil::KString m_clsSessionID;
	eSipUtil::KString m_clsJobID;
	eSipUtil::KString m_clsXml;
};
class AppTrsgTcStartEvent: public AppBaseEvent
{
public:
	AppTrsgTcStartEvent()
	{
		m_unTid = 0;
		m_clsSessionID = KNULL;
		m_clsJobID = KNULL;
		m_pclsTargetContent = NULL;
		m_unFail = 0;
		m_unSuccess = 0;
		m_unTargetIdx = 1;
	}
	~AppTrsgTcStartEvent() {}
	unsigned int m_unTid;
	eSipUtil::KString m_clsSessionID;
	eSipUtil::KString m_clsJobID;
	TargetContent* m_pclsTargetContent;
	unsigned int m_unFail;
	unsigned int m_unSuccess;
	unsigned int m_unTargetIdx;
};
class AppTrsgTcStopEvent: public AppBaseEvent
{
public:
	AppTrsgTcStopEvent()
	{
		m_unTid = 0;
		m_clsSessionID = KNULL;
		m_clsJobID = KNULL;
		m_unFail = 0;
		m_unSuccess = 0;
		m_eTcSt = E_TC_RES_OK;
		m_clsResultDescription = KNULL;
	}
	~AppTrsgTcStopEvent() {}
	unsigned int m_unTid;
	eSipUtil::KString m_clsSessionID;
	eSipUtil::KString m_clsJobID;
	unsigned int m_unFail;
	unsigned int m_unSuccess;
	ETrssCodeSet_t m_eTcSt;
	eSipUtil::KString m_clsResultDescription;
};
class AppTrsgTimerEvent: public AppBaseEvent
{
public:
	AppTrsgTimerEvent()
	{
		m_clsSessionID = KNULL;
		m_unTid = 0;
		m_unTime = 0;
		m_clsJobID = KNULL;
		m_eT = E_MAIN_EV_TRSG_TR_TIMER;
	}
	~AppTrsgTimerEvent() {}
	eSipUtil::KString m_clsSessionID;
	unsigned int m_unTid;
	unsigned int m_unTime;
	eSipUtil::KString m_clsJobID;
};
class AppSessionTimerEvent: public AppBaseEvent
{
public:
	AppSessionTimerEvent()
	{
		m_clsSessionID = KNULL;
		m_clsJobID = KNULL;
		m_unTid = 0;
		m_eT = E_MAIN_EV_SESSION_TIMEOUT;
	}
	~AppSessionTimerEvent() {}
	eSipUtil::KString m_clsSessionID;
	eSipUtil::KString m_clsJobID;
	unsigned int m_unTid;
};
class AppGarbageTimerEvent: public AppBaseEvent
{
public:
	AppGarbageTimerEvent()
	{
		m_unWorkerIdx = 0;
		m_eT = E_MAIN_EV_GARBAGE_TIMER;
	}
	~AppGarbageTimerEvent() {}
	unsigned int m_unWorkerIdx;
};
class AppSessionEndEvent: public AppBaseEvent
{
public:
	AppSessionEndEvent()
	{
		m_clsSessionID = KNULL;
		m_clsJobID = KNULL;
		m_unTid = 0;
		m_eT = E_MAIN_EV_SESSION_END;
	}
	~AppSessionEndEvent() {}
	eSipUtil::KString m_clsSessionID;
	eSipUtil::KString m_clsJobID;
	unsigned int m_unTid;
};
class AppSessionTcTimerEvent: public AppBaseEvent
{
public:
	AppSessionTcTimerEvent()
	{
		m_clsSessionID = KNULL;
		m_clsJobID = KNULL;
		m_unTid = 0;
		m_eT = E_MAIN_EV_SESSION_TC_TIMER;
	}
	~AppSessionTcTimerEvent() {}
	eSipUtil::KString m_clsSessionID;
	eSipUtil::KString m_clsJobID;
	unsigned int m_unTid;
};
#endif
