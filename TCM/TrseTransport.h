#ifndef TRSE_TRANSPORT_H
#define TRSE_TRANSPORT_H

#include "TrspSocketCom.h"
#include "AppIf.h"
#include "Worker.h"
#include "ApiEvent.h"

class TrseTransport : public TrspSocketCom
{
	public:
		TrseTransport();
		virtual ~TrseTransport();
		static TrseTransport * m_fnGetInstance();
		void m_fnAddConnection();
		void m_fnDelConnection(const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		//==========> Tcp Event Notify CallBack
		void m_fnCbkNotify(ETcpNotiEv_t _eT, const eSipUtil::Net5Tuple_t & _rstNet5Tuple,
													eSipUtil::ETransportErr_t _eDisconErr);
		//==========> Packet Size Decision CallBack
		int m_fnCbkAnalizePacket(const char * _pRcvData, 
												unsigned int _unRcvDataLen, 
												const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		//==========> Send/Recv Trace CallBack
		void m_fnCbkTracePacket(bool _bSend, const char * _pData, 
												unsigned int _unDataLen, 
												const eSipUtil::Net5Tuple_t & _rstNet5Tuple,
												void * _pSndAppData);
		bool m_fnXmlSend(const eSipUtil::Net5Tuple_t & _rstNet5Tuple, eSipUtil::KString  _rclsXml);
		static TrseTransport * m_pclsMy;
};
TrseTransport * g_fnCreateTrseTransport();
#endif
