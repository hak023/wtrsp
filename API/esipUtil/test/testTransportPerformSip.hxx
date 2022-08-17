
#ifndef ESIPUTIL_TEST_TRANSPORT_PERFORM_SIP_H
#define ESIPUTIL_TEST_TRANSPORT_PERFORM_SIP_H

#include <stdlib.h>

#include "transportMgr.hxx"
#include "define.hxx"
#include "queueThread.hxx"

struct RcvSipData
{
	eSipUtil::Net5Tuple_t m_stNet5Tuple;
	char * m_pszSip;
	unsigned int m_unSipLen;
	int m_nUasIndex;

	RcvSipData()
	{
		m_pszSip = NULL;
		m_unSipLen = 0;
		m_nUasIndex = -1;
	}

	~RcvSipData()
	{
		free(m_pszSip);
	}
};
typedef struct RcvSipData RcvSipData_t;

class PerformSipUas
{
	public :
		PerformSipUas();
		~PerformSipUas();

		void sendTo(RcvSipData_t * _pstData);

	private :
		eSipUtil::QueueThread< RcvSipData_t > m_clsRcvThread;

};

int getUasIndex4NewTransport();
int cbCheckCompletePerformSip4Rcv(eSipUtil::TransportMgr & _rclsTransportMgr, const char * _pRcvData, unsigned int _unRcvDataLen, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, void * _pAppBuffer);
void cbRecvPerformSip(eSipUtil::TransportMgr & _rclsTransportMgr, char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
void cbSndPerformSip(eSipUtil::TransportMgr & _rclsTransportMgr, const char * _pData, unsigned int _unDataLen, const eSipUtil::Net5Tuple_t & _rstNet5Tuple, void * _pSndAppData);

#endif

