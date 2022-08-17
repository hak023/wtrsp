#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include "rutil/Logger.hxx"
#include "resip/stack/StatisticsManager.hxx"
#include "resip/stack/SipMessage.hxx"
#include "resip/stack/TransactionController.hxx"
#include "resip/stack/SipStack.hxx"
#include "resip/stack/Tuple.hxx"

using namespace resip;
using std::vector;

#define RESIPROCATE_SUBSYSTEM Subsystem::TRANSACTION

#ifdef ESIP_IN_RESIP

#define STAT_SHM_MAGIC_COOKIE "esipStat_qzqj1209njfiaolp[h*"

void StatShm::reset()
{
	strcpy(m_szMagicCookie, STAT_SHM_MAGIC_COOKIE);
	m_cWriteFlag = 0;
	m_stNextWritTime = 0;
	for( int i = 0; i < E_CONST_STAT_MAX_TRANSPORT; ++i )
	{
		m_arrStatA[i].reset();
		m_arrStatB[i].reset();
	}
	m_unStatATransportNumber = 0;
	m_unStatBTransportNumber = 0;
}

StatisticsManager::StatisticsManager(SipStack & _rclsStack, unsigned long _ulIntervalSecs, int _nShmKey, const resip::Data & _rclsShmPath) 
   : mStack(_rclsStack),
     mInterval(_ulIntervalSecs),
     mExternalHandler(NULL),
     m_stNextTimeTick(0),
     m_bShutdown(false),
     m_nShmKey(_nShmKey),
     m_clsShmPath(_rclsShmPath),
     m_pstStatShm(NULL)
{
	if( mInterval <= 0 )
		mInterval = 300;	//minimum 5 min

	int nShmId = -1;
	
	if( m_nShmKey != 0 )
		m_pstStatShm = (StatShm_t *)eSipUtil::createShmAuto(m_nShmKey, sizeof(StatShm_t), nShmId);
	else if( m_clsShmPath.size() > 0 )
		m_pstStatShm = (StatShm_t *)eSipUtil::createShmAuto(m_clsShmPath.c_str(), sizeof(StatShm_t), nShmId);
	else
	{
		ErrLog(<<"setShmPath for stat : fail : invalid argument : ShmKey["<<_nShmKey<<"], ShmPath["<<_rclsShmPath<<"]");
		return;
	}

	if( !m_pstStatShm )
	{
		ErrLog(<<"setShmPath for stat : fail : can't create Shm : ShmKey["<<_nShmKey<<"], ShmPath["<<_rclsShmPath<<"]");
		return;
	}
	
	InfoLog(<<"StatisticsMgr start : ShmKey["<<m_nShmKey<<"], ShmPath["<<m_clsShmPath<<"] : ShmId["<<nShmId<<"]");
	
	if( strcmp(m_pstStatShm->m_szMagicCookie, STAT_SHM_MAGIC_COOKIE) != 0 )
	{
		InfoLog(<<"StatisticsMgr shm reset");
		m_pstStatShm->reset();
	}

	if( m_pstStatShm->m_stNextWritTime > 0 )
		m_stNextTimeTick = m_pstStatShm->m_stNextWritTime / mInterval;

	pthread_create(&m_procTh, NULL, &procThreadWrapper, this);
}

StatisticsManager::~StatisticsManager()
{
}

void StatisticsManager::stop()
{
	m_bShutdown = true;
}

void StatisticsManager::join()
{
	if( m_pstStatShm )
		pthread_join(m_procTh, NULL);
}

void 
StatisticsManager::poll()
{
	StatByTuple_t * parrStatByTuple;
	unsigned int unTransportNumber;

	m_clsStatLock.lock();
	if( m_pstStatShm->m_cWriteFlag == 0 )
	{
		m_pstStatShm->m_cWriteFlag = 1;
		unTransportNumber = m_pstStatShm->m_unStatATransportNumber;
		m_pstStatShm->m_unStatATransportNumber = 0;
		parrStatByTuple = m_pstStatShm->m_arrStatA;
	}
	else
	{
		m_pstStatShm->m_cWriteFlag = 0;
		unTransportNumber = m_pstStatShm->m_unStatBTransportNumber;
		m_pstStatShm->m_unStatBTransportNumber = 0;
		parrStatByTuple = m_pstStatShm->m_arrStatB;
	}
	m_clsStatLock.unlock();

	if( mExternalHandler )
	{
		(*mExternalHandler)(parrStatByTuple, unTransportNumber, (time_t)(m_stNextTimeTick*mInterval));
	}

	for( unsigned int i = 0; i < unTransportNumber; ++i )
	{
		parrStatByTuple[i].reset();
	}
}

void * StatisticsManager::procThreadWrapper(void * _pStatMgr)
{
   if( _pStatMgr )
      ((StatisticsManager *)_pStatMgr)->process();
   return NULL;
}

void 
StatisticsManager::process()
{
	InfoLog(<<"StatisticsThread start");

	if( m_stNextTimeTick > 0 )
	{
		time_t stCurrentTime, stCurrentTimeTick;
		time(&stCurrentTime);
		stCurrentTimeTick = stCurrentTime / mInterval;

		if( stCurrentTimeTick >= m_stNextTimeTick )
		{
			poll();
			m_stNextTimeTick = 0;
			m_pstStatShm->m_stNextWritTime = 0;
		}
	}

	while(!m_bShutdown)
	{
		time_t stCurrentTime, stCurrentTimeTick;
		time(&stCurrentTime);
		stCurrentTimeTick = stCurrentTime / mInterval;

		if( m_stNextTimeTick == 0 )
		{
			m_stNextTimeTick = stCurrentTimeTick + 1;
			m_pstStatShm->m_stNextWritTime = m_stNextTimeTick*mInterval;
		}

		if( stCurrentTimeTick >= m_stNextTimeTick )
		{
			m_stNextTimeTick = stCurrentTimeTick;
			poll();
			++m_stNextTimeTick;
			m_pstStatShm->m_stNextWritTime = m_stNextTimeTick*mInterval;
		}

		sleep(1);
	}
	
	InfoLog(<<"StatisticsThread stop");	
}

void StatisticsManager::zeroOut()
{
	if( !m_pstStatShm )
		return;

	StatByTuple_t * parrStatByTuple;
	unsigned int unTransportNumber;

	m_clsStatLock.lock();
	if( m_pstStatShm->m_cWriteFlag == 0 )
	{
		m_pstStatShm->m_cWriteFlag = 1;
		unTransportNumber = m_pstStatShm->m_unStatATransportNumber;
		m_pstStatShm->m_unStatATransportNumber = 0;
		parrStatByTuple = m_pstStatShm->m_arrStatA;
	}
	else
	{
		m_pstStatShm->m_cWriteFlag = 0;
		unTransportNumber = m_pstStatShm->m_unStatBTransportNumber;
		m_pstStatShm->m_unStatBTransportNumber = 0;
		parrStatByTuple = m_pstStatShm->m_arrStatB;
	}
	m_clsStatLock.unlock();

	for( unsigned int i = 0; i < unTransportNumber; ++i )
	{		
		parrStatByTuple[i].reset();
	}
}

bool 
StatisticsManager::sent(MethodTypes _eMethodType, int _nRspCode, 
										const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType)
{
	if( !m_pstStatShm )
		return false;

	unsigned int unStatIndex;
	bool bResult = false;
	StatByTuple_t * parrStatByTuple;
	unsigned int unTransportNumber;

	m_clsStatLock.lock();
	
	if( m_pstStatShm->m_cWriteFlag == 0 )
	{
		unTransportNumber = m_pstStatShm->m_unStatATransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatA;
	}
	else
	{
		unTransportNumber = m_pstStatShm->m_unStatBTransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatB;
	}

	for( unStatIndex = 0; unStatIndex < unTransportNumber; ++unStatIndex )
	{
		if( parrStatByTuple[unStatIndex].m_eTransportType == _eTransportType
			&& strcmp(parrStatByTuple[unStatIndex].m_szLocalIpAddr, _pszLocalIp) == 0
			&& parrStatByTuple[unStatIndex].m_nLocalPort == _nLocalPort
			&& strcmp(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, _pszRemoteIp) == 0
			&& parrStatByTuple[unStatIndex].m_nRemotePort == _nRemotePort )
		{
			bResult = true;
			break;
		}
	}

	if( unStatIndex == unTransportNumber && unStatIndex < E_CONST_STAT_MAX_TRANSPORT )
	{
		parrStatByTuple[unStatIndex].m_eTransportType = _eTransportType;
		strncpy(parrStatByTuple[unStatIndex].m_szLocalIpAddr, _pszLocalIp, Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szLocalIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nLocalPort = _nLocalPort;
		strncpy(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, _pszRemoteIp, Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szRemoteIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nRemotePort = _nRemotePort;
		if( m_pstStatShm->m_cWriteFlag == 0 )
			++m_pstStatShm->m_unStatATransportNumber;
		else
			++m_pstStatShm->m_unStatBTransportNumber;
		bResult = true;
	}

	if( bResult )
	{
		if( _nRspCode == 0 )
		{
			++(parrStatByTuple[unStatIndex].m_stStatData.m_arrCntReqSent[_eMethodType]);
		}
		else
		{
			parrStatByTuple[unStatIndex].m_stStatData.m_mapCntRspSent[_eMethodType].incRspCnt(_nRspCode, 1);
		}
	}
	
	m_clsStatLock.unlock();

	return bResult;
}

bool 
StatisticsManager::retransmitted(MethodTypes _eMethodType, int _nRspCode, 
															const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType)
{
	if( !m_pstStatShm )
		return false;

	unsigned int unStatIndex;
	bool bResult = false;

	StatByTuple_t * parrStatByTuple;
	unsigned int unTransportNumber;

	m_clsStatLock.lock();
	
	if( m_pstStatShm->m_cWriteFlag == 0 )
	{
		unTransportNumber = m_pstStatShm->m_unStatATransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatA;
	}
	else
	{
		unTransportNumber = m_pstStatShm->m_unStatBTransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatB;
	}

	for( unStatIndex = 0; unStatIndex < unTransportNumber; ++unStatIndex )
	{
		if( parrStatByTuple[unStatIndex].m_eTransportType == _eTransportType
			&& strcmp(parrStatByTuple[unStatIndex].m_szLocalIpAddr, _pszLocalIp) == 0
			&& parrStatByTuple[unStatIndex].m_nLocalPort == _nLocalPort
			&& strcmp(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, _pszRemoteIp) == 0
			&& parrStatByTuple[unStatIndex].m_nRemotePort == _nRemotePort )
		{
			bResult = true;
			break;
		}
	}

	if( unStatIndex == unTransportNumber && unStatIndex < E_CONST_STAT_MAX_TRANSPORT )
	{
		parrStatByTuple[unStatIndex].m_eTransportType = _eTransportType;
		strncpy(parrStatByTuple[unStatIndex].m_szLocalIpAddr, _pszLocalIp, Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szLocalIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nLocalPort = _nLocalPort;
		strncpy(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, _pszRemoteIp, Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szRemoteIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nRemotePort = _nRemotePort;
		if( m_pstStatShm->m_cWriteFlag == 0 )
			++m_pstStatShm->m_unStatATransportNumber;
		else
			++m_pstStatShm->m_unStatBTransportNumber;
		bResult = true;
	}

	if( bResult )
	{
		if( _nRspCode == 0 )
		{
			++(parrStatByTuple[unStatIndex].m_stStatData.m_arrCntReqRetransmitted[_eMethodType]);
		}
		else
		{
			parrStatByTuple[unStatIndex].m_stStatData.m_mapCntRspRetransmitted[_eMethodType].incRspCnt(_nRspCode, 1);
		}
	}

	m_clsStatLock.unlock();

	return bResult;
}

bool
StatisticsManager::received(MethodTypes _eMethodType, int _nRspCode, 
													const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType)
{
	if( !m_pstStatShm )
		return false;

	unsigned int unStatIndex;
	bool bResult = false;
	StatByTuple_t * parrStatByTuple;
	unsigned int unTransportNumber;

	m_clsStatLock.lock();
	
	if( m_pstStatShm->m_cWriteFlag == 0 )
	{
		unTransportNumber = m_pstStatShm->m_unStatATransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatA;
	}
	else
	{
		unTransportNumber = m_pstStatShm->m_unStatBTransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatB;
	}

	for( unStatIndex = 0; unStatIndex < unTransportNumber; ++unStatIndex )
	{
		if( parrStatByTuple[unStatIndex].m_eTransportType == _eTransportType
			&& strcmp(parrStatByTuple[unStatIndex].m_szLocalIpAddr, _pszLocalIp) == 0
			&& parrStatByTuple[unStatIndex].m_nLocalPort == _nLocalPort
			&& strcmp(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, _pszRemoteIp) == 0
			&& parrStatByTuple[unStatIndex].m_nRemotePort == _nRemotePort )
		{
			bResult = true;
			break;
		}
	}

	if( unStatIndex == unTransportNumber && unStatIndex < E_CONST_STAT_MAX_TRANSPORT )
	{
		parrStatByTuple[unStatIndex].m_eTransportType = _eTransportType;
		strncpy(parrStatByTuple[unStatIndex].m_szLocalIpAddr, _pszLocalIp, Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szLocalIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nLocalPort = _nLocalPort;
		strncpy(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, _pszRemoteIp, Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szRemoteIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nRemotePort = _nRemotePort;
		if( m_pstStatShm->m_cWriteFlag == 0 )
			++m_pstStatShm->m_unStatATransportNumber;
		else
			++m_pstStatShm->m_unStatBTransportNumber;
		bResult = true;
	}

	if( bResult )
	{
		if( _nRspCode == 0 )
		{
			++(parrStatByTuple[unStatIndex].m_stStatData.m_arrCntReqReceived[_eMethodType]);
		}
		else
		{
			parrStatByTuple[unStatIndex].m_stStatData.m_mapCntRspReceived[_eMethodType].incRspCnt(_nRspCode, 1);
		}
	}

	m_clsStatLock.unlock();

	return bResult;
}

bool
StatisticsManager::receivedDrop(EMsgDropType_t eMsgDropType, MethodTypes _eMethodType, int _nRspCode, 
															const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType)
{
	if( !m_pstStatShm )
		return false;

	unsigned int unStatIndex;
	bool bResult = false;
	StatByTuple_t * parrStatByTuple;
	unsigned int unTransportNumber;

	m_clsStatLock.lock();
	
	if( m_pstStatShm->m_cWriteFlag == 0 )
	{
		unTransportNumber = m_pstStatShm->m_unStatATransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatA;
	}
	else
	{
		unTransportNumber = m_pstStatShm->m_unStatBTransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatB;
	}

	for( unStatIndex = 0; unStatIndex < unTransportNumber; ++unStatIndex )
	{
		if( parrStatByTuple[unStatIndex].m_eTransportType == _eTransportType
			&& strcmp(parrStatByTuple[unStatIndex].m_szLocalIpAddr, _pszLocalIp) == 0
			&& parrStatByTuple[unStatIndex].m_nLocalPort == _nLocalPort
			&& strcmp(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, _pszRemoteIp) == 0
			&& parrStatByTuple[unStatIndex].m_nRemotePort == _nRemotePort )
		{
			bResult = true;
			break;
		}
	}

	if( unStatIndex == unTransportNumber && unStatIndex < E_CONST_STAT_MAX_TRANSPORT )
	{
		parrStatByTuple[unStatIndex].m_eTransportType = _eTransportType;
		strncpy(parrStatByTuple[unStatIndex].m_szLocalIpAddr, _pszLocalIp, Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szLocalIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nLocalPort = _nLocalPort;
		strncpy(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, _pszRemoteIp, Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szRemoteIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nRemotePort = _nRemotePort;
		if( m_pstStatShm->m_cWriteFlag == 0 )
			++m_pstStatShm->m_unStatATransportNumber;
		else
			++m_pstStatShm->m_unStatBTransportNumber;
		bResult = true;
	}

	if( bResult )
	{
		if( _nRspCode == 0 )
		{
			++(parrStatByTuple[unStatIndex].m_stStatData.m_arrCntReqReceivedDrop[eMsgDropType][_eMethodType]);
		}
		else
		{
			parrStatByTuple[unStatIndex].m_stStatData.m_mapCntRspReceivedDrop[eMsgDropType][_eMethodType].incRspCnt(_nRspCode, 1);
		}
	}

	m_clsStatLock.unlock();

	return bResult;
}

bool
StatisticsManager::receivedDropByTransc(EMsgDropType_t eMsgDropType, MethodTypes _eMethodType, int _nRspCode, 
																				const char * _pszLocalIp, int _nLocalPort, const char * _pszRemoteIp, int _nRemotePort, TransportType _eTransportType)
{
	if( !m_pstStatShm )
		return false;

	unsigned int unStatIndex;
	bool bResult = false;
	StatByTuple_t * parrStatByTuple;
	unsigned int unTransportNumber;

	m_clsStatLock.lock();
	
	if( m_pstStatShm->m_cWriteFlag == 0 )
	{
		unTransportNumber = m_pstStatShm->m_unStatATransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatA;
	}
	else
	{
		unTransportNumber = m_pstStatShm->m_unStatBTransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatB;
	}

	for( unStatIndex = 0; unStatIndex < unTransportNumber; ++unStatIndex )
	{
		if( parrStatByTuple[unStatIndex].m_eTransportType == _eTransportType
			&& strcmp(parrStatByTuple[unStatIndex].m_szLocalIpAddr, _pszLocalIp) == 0
			&& parrStatByTuple[unStatIndex].m_nLocalPort == _nLocalPort
			&& strcmp(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, _pszRemoteIp) == 0
			&& parrStatByTuple[unStatIndex].m_nRemotePort == _nRemotePort )
		{
			bResult = true;
			break;
		}
	}

	if( unStatIndex == unTransportNumber && unStatIndex < E_CONST_STAT_MAX_TRANSPORT )
	{
		parrStatByTuple[unStatIndex].m_eTransportType = _eTransportType;
		strncpy(parrStatByTuple[unStatIndex].m_szLocalIpAddr, _pszLocalIp, Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szLocalIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nLocalPort = _nLocalPort;
		strncpy(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, _pszRemoteIp, Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szRemoteIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nRemotePort = _nRemotePort;
		if( m_pstStatShm->m_cWriteFlag == 0 )
			++m_pstStatShm->m_unStatATransportNumber;
		else
			++m_pstStatShm->m_unStatBTransportNumber;
		bResult = true;
	}

	if( bResult )
	{
		if( _nRspCode == 0 )
		{
			++(parrStatByTuple[unStatIndex].m_stStatData.m_arrCntReqReceivedDrop[eMsgDropType][_eMethodType]);
		}
		else
		{
			parrStatByTuple[unStatIndex].m_stStatData.m_mapCntRspReceivedDrop[eMsgDropType][_eMethodType].incRspCnt(_nRspCode, 1);
		}
	}

	m_clsStatLock.unlock();

	return bResult;
}



bool 
StatisticsManager::receivedRetrans(const SipMessage & _rclsSipMsg)
{
	if( !m_pstStatShm )
		return false;

	MethodTypes eMethod = _rclsSipMsg.method();

	unsigned int unStatIndex;
	bool bResult = false;
	
	const Tuple & rclsRemoteTuple = _rclsSipMsg.getSource();
	TransportType eTransportType = rclsRemoteTuple.getType();
	resip::Data clsRemoteIp = Tuple::inet_ntop(rclsRemoteTuple);
	int nRemotePort = rclsRemoteTuple.getPort();
	resip::Data clsLocalIp;
	int nLocalPort = -1;

	if( _rclsSipMsg.getReceivedTransport() )
	{
		clsLocalIp = Tuple::inet_ntop(_rclsSipMsg.getReceivedTransport()->getTuple());
		nLocalPort = _rclsSipMsg.getReceivedTransport()->getTuple().getPort();
	}
	else
	{
		clsLocalIp = Tuple::inet_ntop(_rclsSipMsg.getReceivedLocalTuple());
		nLocalPort = _rclsSipMsg.getReceivedLocalTuple().getPort();
	}

	StatByTuple_t * parrStatByTuple;
	unsigned int unTransportNumber;

	m_clsStatLock.lock();
	
	if( m_pstStatShm->m_cWriteFlag == 0 )
	{
		unTransportNumber = m_pstStatShm->m_unStatATransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatA;
	}
	else
	{
		unTransportNumber = m_pstStatShm->m_unStatBTransportNumber;
		parrStatByTuple = m_pstStatShm->m_arrStatB;
	}

	for( unStatIndex = 0; unStatIndex < unTransportNumber; ++unStatIndex )
	{
		if( parrStatByTuple[unStatIndex].m_eTransportType == eTransportType
			&& strcmp(parrStatByTuple[unStatIndex].m_szLocalIpAddr, clsLocalIp.c_str()) == 0
			&& parrStatByTuple[unStatIndex].m_nLocalPort == nLocalPort
			&& strcmp(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, clsRemoteIp.c_str()) == 0
			&& parrStatByTuple[unStatIndex].m_nRemotePort == nRemotePort )
		{
			bResult = true;
			break;
		}
	}

	if( unStatIndex == unTransportNumber && unStatIndex < E_CONST_STAT_MAX_TRANSPORT )
	{
		parrStatByTuple[unStatIndex].m_eTransportType = eTransportType;
		strncpy(parrStatByTuple[unStatIndex].m_szLocalIpAddr, clsLocalIp.c_str(), Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szLocalIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nLocalPort = nLocalPort;
		strncpy(parrStatByTuple[unStatIndex].m_szRemoteIpAddr, clsRemoteIp.c_str(), Tuple::E_CONST_MAX_IP_SIZE-1);
		parrStatByTuple[unStatIndex].m_szRemoteIpAddr[Tuple::E_CONST_MAX_IP_SIZE-1] = 0x00;
		parrStatByTuple[unStatIndex].m_nRemotePort = nRemotePort;
		if( m_pstStatShm->m_cWriteFlag == 0 )
			++m_pstStatShm->m_unStatATransportNumber;
		else
			++m_pstStatShm->m_unStatBTransportNumber;
		bResult = true;
	}

	if( bResult )
	{
		if (_rclsSipMsg.isRequest())
		{
			--(parrStatByTuple[unStatIndex].m_stStatData.m_arrCntReqReceived[eMethod]);
			++(parrStatByTuple[unStatIndex].m_stStatData.m_arrCntReqReceivedRetrans[eMethod]);
		}
		else
		{
			int nCode = _rclsSipMsg.const_header(h_StatusLine).statusCode();
			parrStatByTuple[unStatIndex].m_stStatData.m_mapCntRspReceived[eMethod].decRspCnt(nCode, 1);
			parrStatByTuple[unStatIndex].m_stStatData.m_mapCntRspReceivedRetrans[eMethod].incRspCnt(nCode, 1);
		}
	}

	m_clsStatLock.unlock();

	return bResult;
}

#else

StatisticsManager::StatisticsManager(SipStack& stack, unsigned long intervalSecs) 
   : StatisticsMessage::Payload(),
     mStack(stack),
     mInterval(intervalSecs*1000),
     mNextPoll(Timer::getTimeMs() + mInterval),
     mExternalHandler(NULL),
     mPublicPayload(NULL)
{}

StatisticsManager::~StatisticsManager()
{
   if ( mPublicPayload )
       delete mPublicPayload;
}

void 
StatisticsManager::setInterval(unsigned long intervalSecs)
{
   mInterval = intervalSecs * 1000;
}

void 
StatisticsManager::poll()
{
   // get snapshot data now..
   tuFifoSize = mStack.mTransactionController->getTuFifoSize();
   transportFifoSizeSum = mStack.mTransactionController->sumTransportFifoSizes();
   transactionFifoSize = mStack.mTransactionController->getTransactionFifoSize();
   activeTimers = mStack.mTransactionController->getTimerQueueSize();
   activeClientTransactions = mStack.mTransactionController->getNumClientTransactions();
   activeServerTransactions = mStack.mTransactionController->getNumServerTransactions();

   // .kw. At last check payload was > 146kB, which seems too large
   // to alloc on stack. Also, the post'd message has reference
   // to the appStats, so not safe queue as ref to stack element.
   // Converted to dynamic memory allocation.
   if ( mPublicPayload==NULL )
   {
       mPublicPayload = new StatisticsMessage::AtomicPayload;
       // re-used each time, free'd in destructor
   }
   mPublicPayload->loadIn(*this);

   bool postToStack = true;
   StatisticsMessage msg(*mPublicPayload);
   // WATCHOUT: msg contains reference to the payload, and this reference
   // is preserved thru clone().

   if( mExternalHandler )
   {
      postToStack = (*mExternalHandler)(msg);
   }

   if( postToStack )
   {
      // let the app do what it wants with it
      mStack.post(msg);
   }
   
   // !bwc! TODO maybe change this? Or is a flexible implementation of 
   // CongestionManager::logCurrentState() enough?
   if(mStack.mCongestionManager)
   {
      mStack.mCongestionManager->logCurrentState();
   }
}

void 
StatisticsManager::process()
{
   if (Timer::getTimeMs() >= mNextPoll)
   {
      poll();
      mNextPoll += mInterval;
   }
}

bool
StatisticsManager::sent(SipMessage* msg)
{
   MethodTypes met = msg->method();

   if (msg->isRequest())
   {
      ++requestsSent;
      ++requestsSentByMethod[met];
   }
   else if (msg->isResponse())
   {
      int code = msg->const_header(h_StatusLine).statusCode();
      if (code < 0 || code >= MaxCode)
      {
         code = 0;
      }

      ++responsesSent;
      ++responsesSentByMethod[met];
      ++responsesSentByMethodByCode[met][code];
   }
   
   return false;
}

bool 
StatisticsManager::retransmitted(MethodTypes met, 
                                 bool request, 
                                 unsigned int code)
{
   if(request)
   {
      ++requestsRetransmitted;
      ++requestsRetransmittedByMethod[met];
   }
   else
   {
      ++responsesRetransmitted;
      ++responsesRetransmittedByMethod[met];
      ++responsesRetransmittedByMethodByCode[met][code];
   }
   return false;
}

bool
StatisticsManager::received(SipMessage* msg)
{
   MethodTypes met = msg->header(h_CSeq).method();

   if (msg->isRequest())
   {
      ++requestsReceived;
      ++requestsReceivedByMethod[met];
   }
   else if (msg->isResponse())
   {
      ++responsesReceived;
      ++responsesReceivedByMethod[met];
      int code = msg->const_header(h_StatusLine).statusCode();
      if (code < 0 || code >= MaxCode)
      {
         code = 0;
      }
      ++responsesReceivedByMethodByCode[met][code];
   }

   return false;
}

#endif

/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000-2005 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */
