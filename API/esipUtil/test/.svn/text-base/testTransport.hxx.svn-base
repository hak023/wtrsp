
#ifndef ESIPUTIL_TEST_TRANSPORT_H
#define ESIPUTIL_TEST_TRANSPORT_H

#include <pthread.h>

#include "transportMgr.hxx"

class TransportMgrInfo
{
	public :
		enum
		{
			E_MAX_TRANSPORT_MGR_NUM = 2,
		};

		TransportMgrInfo()
		{
			for( int i = 0; i < E_MAX_TRANSPORT_MGR_NUM; ++i )
			{
				g_parrTransportMgr[i] = NULL;
			}
		}

		~TransportMgrInfo()
		{
			for( int i = 0; i < E_MAX_TRANSPORT_MGR_NUM; ++i )
			{
				delete g_parrTransportMgr[i];
			}
		}

		bool addTransportMgr(const char * _pszMgrName, unsigned int _unMaxTransportSize, unsigned int _unMaxEpollSize, unsigned int _unMaxThreadQueueSize, int _nThreadQueueWaitMs);
		int getTransportMgrIdx(const char * _pszMgrName);
		eSipUtil::TransportMgr * getTransportMgr(int _nMgrIdx);

	private :
		eSipUtil::TransportMgr * g_parrTransportMgr[E_MAX_TRANSPORT_MGR_NUM];
		
};

struct TransportEntity
{
	bool m_bUse;
	eSipUtil::Net5Tuple_t m_stNet5Tuple;
	int m_nTcpRole; 								//0(server), 1(client), 2(accepted)
	int m_nMgrIndex;

	/*==== Perform SIP (uas) ====*/
	int m_nPerformSipUasIndex;

	/*==== Simul of VCM ====*/
	eSipUtil::Net5Tuple_t m_stTermSipNet5Tuple;
	char m_szOrigSipLocalIp[64];
	int m_nOrigSipLocalPort;
	eSipUtil::ETransportType_t m_eOrigSipTransportType;

	enum
	{
		E_CONST_MAX_STR_LEN =  eSipUtil::Net5Tuple_t::E_CONST_MAX_STR_LEN+100,
	};

	TransportEntity()
	{
		reset();
	}

	void reset()
	{
		m_bUse = false;
		m_stNet5Tuple.reset();
		m_nTcpRole = 0;
		m_nMgrIndex = -1;
		
		m_nPerformSipUasIndex = -1;
		
		m_stTermSipNet5Tuple.reset();
		m_szOrigSipLocalIp[0] = 0x00;
		m_nOrigSipLocalPort = -1;
		m_eOrigSipTransportType = eSipUtil::E_TRANSPORT_TYPE_NUM;
	}

	const char * getStr(char * _pszOutStr, unsigned int _unMaxStrLen, bool _bInitial = true) const;
	
};
typedef struct TransportEntity TransportEntity_t;

class TransportEntityInfo
{
	public :
		enum
		{
			E_MAX_TRANSPORT_NUM = 50000,
		};

		TransportEntityInfo()
		{
			pthread_mutex_init(&m_TransportEntityLock, 0);
		}
		~TransportEntityInfo()
		{
			pthread_mutex_destroy(&m_TransportEntityLock);
		}

		int addTransportEntity(const TransportEntity_t & _rstTransportEntity);
		void delTransportEntity(int _nEntityIdx);
		int getTransportEntityIdx(const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		bool getTransportEntity(const eSipUtil::Net5Tuple_t & _rstNet5Tuple, TransportEntity & _rstTransportEntity);
		bool getTransportEntity(int _nEntityIdx, TransportEntity & _rstTransportEntity);

	private :
		TransportEntity_t m_arrTransportEntity[E_MAX_TRANSPORT_NUM];
		pthread_mutex_t m_TransportEntityLock;
		
};


#endif

