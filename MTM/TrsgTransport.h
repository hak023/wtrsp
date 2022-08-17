#ifndef TRSG_TRANSPORT_H
#define TRSG_TRANSPORT_H
#include "SocketCom.hxx"
#include "AppIf.h"
#include "Worker.h"
#include "ApiEvent.h"

class TrsgTransport: public eSipUtil::SocketCom
{
public:
	TrsgTransport();
	virtual ~TrsgTransport();
	static TrsgTransport* m_fnGetInstance();
	void m_fnAddConnection();
	//void m_fnDelConnection();
	void m_fnDelConnection(const eSipUtil::Net5Tuple_t &_rstNet5Tuple);
	//==========> Tcp Event Notify CallBack
	void m_fnCbkNotify(eSipUtil::ETcpNotiEv_t _eT, const eSipUtil::Net5Tuple_t &_rstNet5Tuple, eSipUtil::ETransportErr_t _eDisconErr);
	//==========> Packet Size Decision CallBack
	int m_fnCbkAnalizePacket(const char *_pRcvData, unsigned int _unRcvDataLen, const eSipUtil::Net5Tuple_t &_rstNet5Tuple);
	//==========> Send/Recv Trace CallBack
	void m_fnCbkTracePacket(bool _bSend, const char *_pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t &_rstNet5Tuple, void *_pSndAppData);
	bool m_fnXmlSend(const eSipUtil::Net5Tuple_t &_rstNet5Tuple, eSipUtil::KString _rclsXml);
	const char* m_fnGetAddrString(const eSipUtil::Net5Tuple_t &_rstNet5Tupl);
	eSipUtil::KString m_clsAddrString;
	static TrsgTransport *m_pclsMy;
};
TrsgTransport* g_fnCreateTrsgTransport();
#endif
