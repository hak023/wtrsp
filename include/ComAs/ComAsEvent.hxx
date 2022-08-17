
#ifndef COM_AS_EVENT_HXX
#define COM_AS_EVENT_HXX

#include "eSipCallback.hxx"
#include "eSipTu.hxx"

#include "ComAsCdr.hxx"
#include "ComAsExtAsIf.hxx"

namespace ComAs
{

typedef enum
{
	E_ASYNC_EVENT_extAs_rcvSipEvent = 0,

	E_ASYNC_EVENT_NUM,
}EAsyncEventType;

const char * g_fnGetAsyncEventStr(int _nAsyncEvent);

/*==== Event for Rcv/Snd SIP (from TU) ====*/
struct ComAsSipEvent
{
	enum
	{
		E_CONST_MAX_PRINT_LEN = 1024,
	};

	ECdrStartHost m_eCallStartHost;
	eSIP::ProcSipParam m_stSipParam;
	
	ComAsSipEvent();	

	const char * m_fnGetStr(char * _pszStr, unsigned int _unMaxLen) const;	
};

/*==== Event for Async ====*/
struct ComAsAsyncEvent
{	
	EAsyncEventType m_eAsyncEventType;
	const void * m_pAsyncData;
	unsigned int m_unAsyncLen;

	ComAsAsyncEvent();	

	const char * m_fnGetStr(char * _pszStr, unsigned int _unMaxLen) const;	
};

/*==== Event of manage for ExtAs ====*/
struct ExtAsManageEvent
{
	typedef enum
	{
		E_CON_STATUS_idle = 0,
		E_CON_STATUS_connect,
		E_CON_STATUS_close,
	}EConnStatus;

	eSipUtil::Net5Tuple m_stNet5Tuple;
	ExtAsIfComInfo m_stExtAsIfComInfo;
	EConnStatus m_eConStatus;
	void * m_pExtAsPacket;
	unsigned int m_unExtAsPacketLen;
	
	ExtAsManageEvent();
	~ExtAsManageEvent();	

	const char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen) const;	
};


/*==== Event of SIP from As ====*/
struct ExtAsSipEvent
{
	eSipUtil::Net5Tuple m_stNet5Tuple;
	ExtAsIfComInfo m_stExtAsIfComInfo;
	void * m_pExtAsPacket;
	unsigned int m_unExtAsPacketLen;

	ExtAsSipEvent();	
	~ExtAsSipEvent();	

	const char * m_fnGetStr(char * _pszPrint, unsigned int _unMaxLen) const;	
};

}

#endif

