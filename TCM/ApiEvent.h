#ifndef API_EVENT_H
#define API_EVENT_H
#include "string.hxx"
#include "transportMgr.hxx"
typedef enum
{
	/*순서 상관없음. Worker 에서 이벤트 구분 용도로 사용함*/
	E_MAIN_EV_NONE = 0,
	E_MAIN_EV_TRSE_CREATE_JOB_REQ,//TRSE
	E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_CREATED,				//TRSS
	E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_WAITING ,				//TRSS
	E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED,			//TRSS
	E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED,	//TRSS
	E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED,	//TRSS
	E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED,			//TRSS
	E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED,			//TRSS
	E_MAIN_EV_SESSION_END,
	E_MAIN_EV_TRSS_TR_TIMER,
	E_MAIN_EV_SESSION_TIMEOUT,
	E_MAIN_EV_GARAGE_TIMER,
	E_MAIN_EV_TRSE_DISCON,
	E_MAIN_EV_MAX
}EAppEvent_t;
inline const char * g_fnGetAppString(EAppEvent_t _eT)
{
	switch(_eT)
	{
		case E_MAIN_EV_TRSE_CREATE_JOB_REQ:									return "E_MAIN_EV_TRSE_CREATE_JOB_REQ";
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_CREATED:				return "E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_CREATED";
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_WAITING:				return "E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_WAITING";
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED:			return "E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED";
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED:	return "E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED";
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED:	return "E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED";
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED:			return "E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED";
		case E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED:		return "E_MAIN_EV_TRSS_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED";
		case E_MAIN_EV_SESSION_END:											return "E_MAIN_EV_SESSION_END";
		case E_MAIN_EV_TRSS_TR_TIMER:										return "E_MAIN_EV_TRSS_TR_TIMER";
		case E_MAIN_EV_SESSION_TIMEOUT:										return "E_MAIN_EV_SESSION_TIMEOUT";
		case E_MAIN_EV_GARAGE_TIMER:										return "E_MAIN_EV_GARAGE_TIMER";
		case E_MAIN_EV_TRSE_DISCON:										return "E_MAIN_EV_TRSE_DISCON";
		default: 															return "E_MAIN_EV_NONE";
	};
	return "E_MAIN_EV_NONE";
}
class AppBaseEvent
{
	public:
		AppBaseEvent(){m_eT=E_MAIN_EV_NONE; memset(&m_stAddr,0x00,sizeof(eSipUtil::Net5Tuple_t));}
		virtual ~AppBaseEvent(){}
		const char * m_fnGetAddrString()
		{
			if(m_clsAddr.m_unRealLen==0)
			{
				m_clsAddr<<m_stAddr.m_szLocalIp<<m_stAddr.m_nLocalPort;
				m_clsAddr<<m_stAddr.m_szRemoteIp<<m_stAddr.m_nRemotePort;
			}
			return (eSipUtil::KCSTR)m_clsAddr;
		}
		EAppEvent_t m_eT;
		eSipUtil::KString m_clsAddr;
		eSipUtil::Net5Tuple_t m_stAddr;
};

class AppTrseEvent : public AppBaseEvent
{
	public:
		AppTrseEvent(){m_unTid=0;m_clsSessionID=KNULL;m_clsJobID=KNULL;m_clsXml=KNULL;}
		~AppTrseEvent(){}
		unsigned int m_unTid;
		eSipUtil::KString m_clsSessionID;
		eSipUtil::KString m_clsJobID; //Trse Tr 생성 시 Job ID 생성 함
		eSipUtil::KString m_clsXml;   // xml Data
};
class AppTrssEvent : public AppBaseEvent
{
	public:
		AppTrssEvent(){m_unTid=0;m_clsSessionID=KNULL;m_clsJobID=KNULL;m_clsXml=KNULL;}
		~AppTrssEvent(){}
		unsigned int m_unTid;
		eSipUtil::KString m_clsSessionID;
		eSipUtil::KString m_clsJobID;
		eSipUtil::KString m_clsXml;   // xml Data
};
class AppTrssTimerEvent : public AppBaseEvent
{
	public:
		AppTrssTimerEvent(){m_clsSessionID = KNULL;m_unTid=0;m_unTime=0;m_clsJobID=KNULL;m_eT=E_MAIN_EV_TRSS_TR_TIMER;}
		~AppTrssTimerEvent(){}
		eSipUtil::KString m_clsSessionID;
		unsigned int m_unTid;
		unsigned int m_unTime;
      eSipUtil::KString m_clsJobID;
};
class AppSessionTimerEvent : public AppBaseEvent
{
	public:
		AppSessionTimerEvent(){m_clsSessionID = KNULL;m_unTid=0;m_eT=E_MAIN_EV_SESSION_TIMEOUT;}
		~AppSessionTimerEvent(){}
		eSipUtil::KString m_clsSessionID;
		unsigned int m_unTid;
};
class AppGarbageTimerEvent : public AppBaseEvent
{
	public:
		AppGarbageTimerEvent(){m_unWorkerIdx=0;m_eT=E_MAIN_EV_GARAGE_TIMER;}
		~AppGarbageTimerEvent(){}
		unsigned int m_unWorkerIdx;
};
class AppTrseDisconEvent : public AppBaseEvent
{
	public:
		AppTrseDisconEvent(){m_unWorkerIdx=0;m_clsSessionID = KNULL;m_eT=E_MAIN_EV_TRSE_DISCON;}
		~AppTrseDisconEvent(){}
      unsigned int m_unWorkerIdx;
		eSipUtil::KString m_clsSessionID;
};
class AppSessionEndEvent : public AppBaseEvent
{
	public:
	AppSessionEndEvent(){m_clsSessionID = KNULL;m_unTid=0;m_eT=E_MAIN_EV_SESSION_END;}
		~AppSessionEndEvent(){}
		eSipUtil::KString m_clsSessionID;
		unsigned int m_unTid;
};
#endif
