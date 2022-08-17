
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>

#include "eSipUtil/ConfigFile.hxx"

#include "testTransport.hxx"
#include "testTransportData.hxx"
#include "testTransportPerformSip.hxx"
#include "testVCM.hxx"
#include "socket.hxx"
#include "log.hxx"

using namespace eSipUtil;

static const char * s_pszTopMenu = "\n1: Add\n2: Del\n3: Show\n4: Send\n5: PerformSipUas\n6: PerformVcm\n7: Set LogLevel\n8: Quit";
static const char * s_pszPrompt = "==> ";

char g_szProcName[100] = {0,};
int g_nLogInstanceId = -1;

#define LogTest(_level, _format, args...) Log(g_nLogInstanceId, 0, _level, _format, ##args)

TransportMgrInfo g_clsTransportMgrInfo;
TransportEntityInfo g_clsTransportEntityInfo;

pthread_mutex_t s_LogLock = PTHREAD_MUTEX_INITIALIZER;

void setPrompt(int & _rnI)
{
	char szInt[50];
	printf("%s", s_pszPrompt);
	fgets(szInt, sizeof(szInt)-1, stdin);
	szInt[strlen(szInt) - 1] = 0x00;

	if( strlen(szInt) == 1 && szInt[0] == '0' )
		_rnI = 0;
	else
	{
		_rnI = atoi(szInt);
		if( _rnI == 0 )
			_rnI = -1;
	}
}

void setPrompt(char * _pszStr, unsigned int _unMaxLen)
{
	printf("%s", s_pszPrompt);
	fgets(_pszStr, _unMaxLen-1, stdin);
	_pszStr[strlen(_pszStr) - 1] = 0x00;
}

bool TransportMgrInfo::addTransportMgr(const char * _pszMgrName, unsigned int _unMaxTransportSize, unsigned int _unMaxEpollSize, unsigned int _unMaxThreadQueueSize, int _nThreadQueueWaitMs)
{
	TransportMgrOption stTrspMgrOpt;
	stTrspMgrOpt.m_bUseSendThread = false;
	stTrspMgrOpt.m_pszMgrName = _pszMgrName;
	stTrspMgrOpt.m_unMaxTransportSize = _unMaxTransportSize;
	stTrspMgrOpt.m_unMaxEpollSize = _unMaxEpollSize;
	stTrspMgrOpt.m_unMaxThreadQueueSize = _unMaxThreadQueueSize;
	stTrspMgrOpt.m_nThreadQueueWaitMs = _nThreadQueueWaitMs;

	for( int i = 0; i < E_MAX_TRANSPORT_MGR_NUM; ++i )
	{
		if( g_parrTransportMgr[i] == NULL )
		{			
			g_parrTransportMgr[i] = new TransportMgr(stTrspMgrOpt);
			if( g_parrTransportMgr[i] )
				return true;
			else
				return false;
		}
	}

	return false;
}

int TransportMgrInfo::getTransportMgrIdx(const char * _pszMgrName)
{
	if( !_pszMgrName )
		return -1;

	for( int i = 0; i < E_MAX_TRANSPORT_MGR_NUM; ++i )
	{
		if( strcmp(g_parrTransportMgr[i]->getMgrName(), _pszMgrName) == 0 )
			return i;
	}

	return -1;
}

TransportMgr * TransportMgrInfo::getTransportMgr(int _nMgrIdx)
{
	if( _nMgrIdx < 0 || _nMgrIdx >= E_MAX_TRANSPORT_MGR_NUM )
		return NULL;

	return g_parrTransportMgr[_nMgrIdx];
}

const char * TransportEntity::getStr(char * _pszOutStr, unsigned int _unMaxStrLen, bool _bInitial) const
{
	if( !_pszOutStr || _unMaxStrLen == 0 )
		return getBlankStr();

	if( _bInitial )
		_pszOutStr[0] = 0x00;

	const char * pszMgrName = NULL;
	if( m_nMgrIndex >= 0 && m_nMgrIndex < TransportMgrInfo::E_MAX_TRANSPORT_MGR_NUM && g_clsTransportMgrInfo.getTransportMgr(m_nMgrIndex) )
		pszMgrName = g_clsTransportMgrInfo.getTransportMgr(m_nMgrIndex)->getMgrName();
	if( !pszMgrName )
		pszMgrName = "NULL";

	strncat(_pszOutStr, _unMaxStrLen-1, "Use[%d], Mgr[%s], ", m_bUse, pszMgrName);
	m_stNet5Tuple.getKeyStr(_pszOutStr, _unMaxStrLen, false);
	if( m_stNet5Tuple.m_eTransportType == E_TRANSPORT_TYPE_tcp || m_stNet5Tuple.m_eTransportType == E_TRANSPORT_TYPE_tls )
		strncat(_pszOutStr, _unMaxStrLen-1, ", TcpRole[%d]", m_nTcpRole);		

	return _pszOutStr;
}

int TransportEntityInfo::addTransportEntity(const TransportEntity_t & _rstTransportEntity)
{
	unsigned int unTransportIdx = 0;

	pthread_mutex_lock(&m_TransportEntityLock);

	for( unTransportIdx = 0; unTransportIdx < E_MAX_TRANSPORT_NUM; ++unTransportIdx )
	{
		if( !m_arrTransportEntity[unTransportIdx].m_bUse )
			break;
	}

	if( unTransportIdx == E_MAX_TRANSPORT_NUM )
	{
		LogTest(E_LOG_ERR, "addTransportEntity : fail : Max Transport\n");
		unTransportIdx = -1;
	}
	else
	{
		m_arrTransportEntity[unTransportIdx] = _rstTransportEntity;
		m_arrTransportEntity[unTransportIdx].m_bUse = true;
	}

	pthread_mutex_unlock(&m_TransportEntityLock);

	return unTransportIdx;
}

void TransportEntityInfo::delTransportEntity(int _nEntityIdx)
{
	if( _nEntityIdx < 0 || _nEntityIdx >= E_MAX_TRANSPORT_NUM )
		return;

	pthread_mutex_lock(&m_TransportEntityLock);

	m_arrTransportEntity[_nEntityIdx].reset();

	pthread_mutex_unlock(&m_TransportEntityLock);
}

int TransportEntityInfo::getTransportEntityIdx(const Net5Tuple_t & _rstNet5Tuple)
{
	pthread_mutex_lock(&m_TransportEntityLock);

	for( int i =0; i < E_MAX_TRANSPORT_NUM; ++i )
	{
		if( m_arrTransportEntity[i].m_bUse && m_arrTransportEntity[i].m_stNet5Tuple.checkSame4Transport(_rstNet5Tuple) )
		{
			pthread_mutex_unlock(&m_TransportEntityLock);
			return i;
		}
	}

	pthread_mutex_unlock(&m_TransportEntityLock);

	return -1;
}

bool TransportEntityInfo::getTransportEntity(const Net5Tuple_t & _rstNet5Tuple, TransportEntity & _rstTransportEntity)
{
	pthread_mutex_lock(&m_TransportEntityLock);

	for( int i =0; i < E_MAX_TRANSPORT_NUM; ++i )
	{
		if( m_arrTransportEntity[i].m_bUse && m_arrTransportEntity[i].m_stNet5Tuple.checkSame4Transport(_rstNet5Tuple) )
		{
			_rstTransportEntity = m_arrTransportEntity[i];
			pthread_mutex_unlock(&m_TransportEntityLock);			
			return true;
		}
	}

	pthread_mutex_unlock(&m_TransportEntityLock);

	return false;
}

bool TransportEntityInfo::getTransportEntity(int _nEntityIdx, TransportEntity & _rstTransportEntity)
{
	if( _nEntityIdx < 0 || _nEntityIdx >= E_MAX_TRANSPORT_NUM )
		return false;

	pthread_mutex_lock(&m_TransportEntityLock);
	
	_rstTransportEntity = m_arrTransportEntity[_nEntityIdx];

	pthread_mutex_unlock(&m_TransportEntityLock);
	
	return true;
}

bool sendData(const Net5Tuple_t & _rstNet5Tuple, char * _pData, unsigned int _unDataLen)
{
	char szNet5Tuple[Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};
	TransportEntity_t stTransportEntity;
	
	if( !g_clsTransportEntityInfo.getTransportEntity(_rstNet5Tuple, stTransportEntity) )
	{
		LogTest(E_LOG_ERR, "sendData : fail : not found TransportEntity : %s", _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
		free(_pData);
		return false;
	}

	if( !stTransportEntity.m_bUse )
	{
		LogTest(E_LOG_ERR, "sendData : fail : TransportEntity is not using : %s", _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
		free(_pData);
		return false;
	}
	
	if( stTransportEntity.m_nMgrIndex >= 0 && stTransportEntity.m_nMgrIndex < TransportMgrInfo::E_MAX_TRANSPORT_MGR_NUM
		&& g_clsTransportMgrInfo.getTransportMgr(stTransportEntity.m_nMgrIndex)
		&& g_clsTransportMgrInfo.getTransportMgr(stTransportEntity.m_nMgrIndex)->sendData(_rstNet5Tuple, _pData, _unDataLen) )
	{
		return true;
	}
	else
	{
		LogTest(E_LOG_ERR, "sendData : fail : MgrIdx[%d] : %s", stTransportEntity.m_nMgrIndex, _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));
		free(_pData);
		return false;
	}
}

int cbCheckCompleteData4Rcv(TransportMgr & _rclsTransportMgr, const char * _pRcvData, unsigned int _unRcvDataLen, const Net5Tuple_t & _rstNet5Tuple, void * _pAppBuffer)
{
	if( !_pRcvData || _unRcvDataLen == 0 )
		return -1;

	SendingData_t * pstSendingData = (SendingData_t *)_pRcvData;
	if( _unRcvDataLen < pstSendingData->getHeaderLength() )
		return 0;
	else
		return pstSendingData->getLength();
}

void cbRecvData(TransportMgr & _rclsTransportMgr, char * _pData, unsigned int _unDataLen, const Net5Tuple_t & _rstNet5Tuple)
{
	if( !_pData || _unDataLen == 0 )
	{
		LogTest(E_LOG_ERR, "TestRecvData callback : fail : invalid argument : Data[%p], DataLen[%u]", _pData, _unDataLen);
		free(_pData);
		return;
	}

	SendingData_t * pstSendingData = (SendingData_t *)_pData;
	char szNet5Tuple[Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};
	
	if( pstSendingData->getLength() != _unDataLen )
	{
		LogTest(E_LOG_ERR, "TestRecvData callback : fail : mismatch length : LenInMsg[%u], RcvLen[%u]", pstSendingData->getLength(), _unDataLen);
		free(_pData);
		return;
	}

	LogTest(E_LOG_DEBUG, "TestRecvData callback : %s, Data[%s], DataLen[%u]", _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)), _pData, _unDataLen);

	if( pstSendingData->m_eTestTransportDataType == E_TEST_TRANSPORT_DATA_TYPE_simpleMsg )
	{
		printf("\n%s%s (Rcv SimpleMsg : Net5Tuple[%s], Len[%u])\n", 
				s_pszPrompt, pstSendingData->m_Data, _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)), _unDataLen);
	}
	else if( pstSendingData->m_eTestTransportDataType == E_TEST_TRANSPORT_DATA_TYPE_perform )
	{
		
	}
	else
	{
		LogTest(E_LOG_ERR, "TestRecvData callback : fail : unknown DataType : Type[%d]", pstSendingData->m_eTestTransportDataType);
	}

	free(_pData);
	
}

void cbAcceptConnection(TransportMgr & _rclsTransportMgr, Net5Tuple_t & _rstNet5Tuple)
{
	char szNet5Tuple[Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};
	
	LogTest(E_LOG_DEBUG, "TestAccept callback : Mgr[%s], %s", _rclsTransportMgr.getMgrName(), _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)));

	TransportEntity_t stTransportEntity;
	stTransportEntity.m_stNet5Tuple = _rstNet5Tuple;
	stTransportEntity.m_nTcpRole = 2;
	stTransportEntity.m_nMgrIndex = g_clsTransportMgrInfo.getTransportMgrIdx(_rclsTransportMgr.getMgrName());
	stTransportEntity.m_nPerformSipUasIndex = getUasIndex4NewTransport();
	g_clsTransportEntityInfo.addTransportEntity(stTransportEntity);
}

void cbDelTransport(TransportMgr & _rclsTransportMgr, const Net5Tuple_t & _rstNet5Tuple, ETransportErr_t _eTrspErr)
{
	char szNet5Tuple[Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};
	
	LogTest(E_LOG_DEBUG, "TestDelTransport callback : Mgr[%s], %s : %s", 
											_rclsTransportMgr.getMgrName(), _rstNet5Tuple.getStr(szNet5Tuple, sizeof(szNet5Tuple)), g_fnGetTransportErrStr(_eTrspErr));

	int nTransportEntityIdx = g_clsTransportEntityInfo.getTransportEntityIdx(_rstNet5Tuple);
	g_clsTransportEntityInfo.delTransportEntity(nTransportEntityIdx);
}

//_nMode : 0(normal), 1(perform sip for uas)
bool cmdAddTransport(int _nMode)
{
	static unsigned int unAddCnt = 0;

	Net5Tuple_t stNet5Tuple;
	SocketCfg_t stSocketCfg;
	TransportConfig_t stTransportCfg;

	printf("Input TransportType (0:UDP, 1:TCP) :\n");
	setPrompt((int &)(stNet5Tuple.m_eTransportType));
	if( stNet5Tuple.m_eTransportType < 0 || stNet5Tuple.m_eTransportType >= E_TRANSPORT_TYPE_NUM )
	{
		printf("invalid Transport Type\n");
		return false;
	}

	printf("Input LocalIp :\n");
	setPrompt(stNet5Tuple.m_szLocalIp, Net5Tuple_t::E_CONST_MAX_IPADDR_LEN);
	if( !Socket::isIpAddress(stNet5Tuple.m_szLocalIp) )
	{
		printf("invalid IP address\n");
		return false;
	}

	printf("Input LocalPort :\n");
	setPrompt(stNet5Tuple.m_nLocalPort);
	if( stNet5Tuple.m_nLocalPort < 0 || stNet5Tuple.m_nLocalPort > 65535 )
	{
		printf("invalid Port\n");
		return false;
	}

	stSocketCfg.m_nBlock = 0;
	stSocketCfg.m_nReuse = 0;
	stSocketCfg.m_nRcvSocketBufferSize = 60000000;
	stSocketCfg.m_nSndSocketBufferSize = 60000000;

	stTransportCfg.m_unRcvBufferSize = 20000;
	if( _nMode == 1 )
	{
		stTransportCfg.m_pfnRcvProcCb = cbRecvPerformSip;
		stTransportCfg.m_pfnSndAfterProcCb = cbSndPerformSip;
	}
	else
		stTransportCfg.m_pfnRcvProcCb = cbRecvData;
	stTransportCfg.m_pfnDelTransportCb = cbDelTransport;	

	if( stNet5Tuple.m_eTransportType == E_TRANSPORT_TYPE_tcp || stNet5Tuple.m_eTransportType == E_TRANSPORT_TYPE_tls )
	{
		printf("Input Role (0:server, 1:client) :\n");
		setPrompt(stTransportCfg.m_stTcpConfig.m_nTcpRole);
		stTransportCfg.m_stTcpConfig.m_unSndBufferSize = 20000;
		if( stTransportCfg.m_stTcpConfig.m_nTcpRole == 1 )
		{
			printf("Input RemoteIp :\n");
			setPrompt(stNet5Tuple.m_szRemoteIp, Net5Tuple_t::E_CONST_MAX_IPADDR_LEN);
			if( !Socket::isIpAddress(stNet5Tuple.m_szRemoteIp) )
			{
				printf("invalid IP address\n");
				return false;
			}

			printf("Input RemotePort :\n");
			setPrompt(stNet5Tuple.m_nRemotePort);
			if( stNet5Tuple.m_nRemotePort < 0 || stNet5Tuple.m_nRemotePort > 65535 )
			{
				printf("invalid Port\n");
				return false;
			}
		}

		stSocketCfg.m_nReuse = 1;

		if( _nMode == 1 )
			stTransportCfg.m_stTcpConfig.m_pfnTcpRcvMatchCb = cbCheckCompletePerformSip4Rcv;
		else
			stTransportCfg.m_stTcpConfig.m_pfnTcpRcvMatchCb = cbCheckCompleteData4Rcv;
		stTransportCfg.m_stTcpConfig.m_pfnAcceptConnectionCb = cbAcceptConnection;
	}

	int nTransportEntityIdx = -1;
	TransportEntity_t stTransportEntity;
	stTransportEntity.m_stNet5Tuple = stNet5Tuple;
	stTransportEntity.m_nTcpRole = stTransportCfg.m_stTcpConfig.m_nTcpRole;
	stTransportEntity.m_nMgrIndex = unAddCnt%TransportMgrInfo::E_MAX_TRANSPORT_MGR_NUM;
	if( _nMode == 1 )
		stTransportEntity.m_nPerformSipUasIndex = getUasIndex4NewTransport();
	if( (nTransportEntityIdx = g_clsTransportEntityInfo.addTransportEntity(stTransportEntity)) < 0 )
	{
		return false;
	}

	if( g_clsTransportMgrInfo.getTransportMgr(stTransportEntity.m_nMgrIndex) )
	{
		g_clsTransportMgrInfo.getTransportMgr(stTransportEntity.m_nMgrIndex)->addTransport(stTransportEntity.m_stNet5Tuple, stSocketCfg, stTransportCfg);
		char szNet5Tuple[Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};
		LogTest(E_LOG_DEBUG, "cmdAddTransport : success : %s, MgrIdx[%d], PerformSipUasIndex[%d]", 
										stTransportEntity.m_stNet5Tuple.getKeyStr(szNet5Tuple, sizeof(szNet5Tuple)), stTransportEntity.m_nMgrIndex, stTransportEntity.m_nPerformSipUasIndex);
		++unAddCnt;
		return true;		
	}
	else
	{
		g_clsTransportEntityInfo.delTransportEntity(nTransportEntityIdx);
		return false;
	}
}

bool cmdAddTransport4Vcm(const char * _pszCfgPath)
{
	static unsigned int unAddCnt = 0;
	
	Net5Tuple_t stTermSipNet5Tuple;
	char szOrigSipLocalIp[64] = {0,};
	int nOrigSipLocalPort = -1;
	ETransportType_t eOrigSipTransportType = E_TRANSPORT_TYPE_NUM;

	Net5Tuple_t stInternalNet5Tuple;
	SocketCfg_t stInternalSocketCfg;
	TransportConfig_t stInternalTransportCfg;

	if( _pszCfgPath && strlen(_pszCfgPath) > 0 )
	{
		bool bResult = false;
		unsigned int unTmp = 0;
		char * pszTmp = NULL;
		eSipUtil::CfgFile m_clsCfgFile;
		
		m_clsCfgFile.m_fnLoadConfig(_pszCfgPath, NULL, false);

		/*==== internal ===*/
		pszTmp = (eSipUtil::KSTR)m_clsCfgFile.m_fnFindVal("internal", "LocalIp", &bResult);
		if( bResult && pszTmp )
		{
			strncpy(stInternalNet5Tuple.m_szLocalIp, pszTmp, eSipUtil::Net5Tuple_t::E_CONST_MAX_IPADDR_LEN-1);
			stInternalNet5Tuple.m_szLocalIp[eSipUtil::Net5Tuple_t::E_CONST_MAX_IPADDR_LEN-1] = 0x00;
		}
		
		unTmp = (eSipUtil::KUINT)m_clsCfgFile.m_fnFindVal("internal", "LocalPort", &bResult);
		if( bResult )
			stInternalNet5Tuple.m_nLocalPort = (int)unTmp;
		
		pszTmp = (eSipUtil::KSTR)m_clsCfgFile.m_fnFindVal("internal", "RemoteIp", &bResult);
		if( bResult && pszTmp )
		{
			strncpy(stInternalNet5Tuple.m_szRemoteIp, pszTmp, eSipUtil::Net5Tuple_t::E_CONST_MAX_IPADDR_LEN-1);
			stInternalNet5Tuple.m_szRemoteIp[eSipUtil::Net5Tuple_t::E_CONST_MAX_IPADDR_LEN-1] = 0x00;
		}
		
		unTmp = (eSipUtil::KUINT)m_clsCfgFile.m_fnFindVal("internal", "RemotePort", &bResult);
		if( bResult )
			stInternalNet5Tuple.m_nRemotePort = (int)unTmp;

		stInternalNet5Tuple.m_eTransportType = E_TRANSPORT_TYPE_tcp;

		/*==== SIP ===*/
		pszTmp = (eSipUtil::KSTR)m_clsCfgFile.m_fnFindVal("sip", "OrigLocalIp", &bResult);
		if( bResult && pszTmp )
		{
			strncpy(szOrigSipLocalIp, pszTmp, eSipUtil::Net5Tuple_t::E_CONST_MAX_IPADDR_LEN-1);
			szOrigSipLocalIp[eSipUtil::Net5Tuple_t::E_CONST_MAX_IPADDR_LEN-1] = 0x00;
		}
		
		unTmp = (eSipUtil::KUINT)m_clsCfgFile.m_fnFindVal("sip", "OrigLocalPort", &bResult);
		if( bResult )
			nOrigSipLocalPort = (int)unTmp;

		unTmp = (eSipUtil::KUINT)m_clsCfgFile.m_fnFindVal("sip", "OrigTrsp", &bResult);
		if( bResult && unTmp >= 0 && unTmp < E_TRANSPORT_TYPE_NUM )
			eOrigSipTransportType = (ETransportType_t)unTmp;

		pszTmp = (eSipUtil::KSTR)m_clsCfgFile.m_fnFindVal("sip", "TermLocalIp", &bResult);
		if( bResult && pszTmp )
		{
			strncpy(stTermSipNet5Tuple.m_szLocalIp, pszTmp, eSipUtil::Net5Tuple_t::E_CONST_MAX_IPADDR_LEN-1);
			stTermSipNet5Tuple.m_szLocalIp[eSipUtil::Net5Tuple_t::E_CONST_MAX_IPADDR_LEN-1] = 0x00;
		}
		
		unTmp = (eSipUtil::KUINT)m_clsCfgFile.m_fnFindVal("sip", "TermLocalPort", &bResult);
		if( bResult )
			stTermSipNet5Tuple.m_nLocalPort = (int)unTmp;
		
		pszTmp = (eSipUtil::KSTR)m_clsCfgFile.m_fnFindVal("sip", "TermRemoteIp", &bResult);
		if( bResult && pszTmp )
		{
			strncpy(stTermSipNet5Tuple.m_szRemoteIp, pszTmp, eSipUtil::Net5Tuple_t::E_CONST_MAX_IPADDR_LEN-1);
			stTermSipNet5Tuple.m_szRemoteIp[eSipUtil::Net5Tuple_t::E_CONST_MAX_IPADDR_LEN-1] = 0x00;
		}
		
		unTmp = (eSipUtil::KUINT)m_clsCfgFile.m_fnFindVal("sip", "TermRemotePort", &bResult);
		if( bResult )
			stTermSipNet5Tuple.m_nRemotePort = (int)unTmp;

		unTmp = (eSipUtil::KUINT)m_clsCfgFile.m_fnFindVal("sip", "TermTrsp", &bResult);
		if( bResult && unTmp >= 0 && unTmp < E_TRANSPORT_TYPE_NUM )
			stTermSipNet5Tuple.m_eTransportType = (ETransportType_t)unTmp;
	}
	else
	{
		printf("Input LocalIp for internal :\n");
		setPrompt(stInternalNet5Tuple.m_szLocalIp, Net5Tuple_t::E_CONST_MAX_IPADDR_LEN);
		if( !Socket::isIpAddress(stInternalNet5Tuple.m_szLocalIp) )
		{
			printf("invalid IP address\n");
			return false;
		}

		printf("Input LocalPort for internal :\n");
		setPrompt(stInternalNet5Tuple.m_nLocalPort);
		if( stInternalNet5Tuple.m_nLocalPort < 0 || stInternalNet5Tuple.m_nLocalPort > 65535 )
		{
			printf("invalid Port\n");
			return false;
		}
		
		printf("Input RemoteIp for internal :\n");
		setPrompt(stInternalNet5Tuple.m_szRemoteIp, Net5Tuple_t::E_CONST_MAX_IPADDR_LEN);
		if( !Socket::isIpAddress(stInternalNet5Tuple.m_szRemoteIp) )
		{
			printf("invalid IP address\n");
			return false;
		}

		printf("Input RemotePort for internal :\n");
		setPrompt(stInternalNet5Tuple.m_nRemotePort);
		if( stInternalNet5Tuple.m_nRemotePort < 0 || stInternalNet5Tuple.m_nRemotePort > 65535 )
		{
			printf("invalid Port\n");
			return false;
		}

		stInternalNet5Tuple.m_eTransportType = E_TRANSPORT_TYPE_tcp;

		printf("Input Orig SIP LocalIp :\n");
		setPrompt(szOrigSipLocalIp, Net5Tuple_t::E_CONST_MAX_IPADDR_LEN);
		if( !Socket::isIpAddress(szOrigSipLocalIp) )
		{
			printf("invalid IP address\n");
			return false;
		}

		printf("Input Orig SIP LocalPort :\n");
		setPrompt(nOrigSipLocalPort);
		if(nOrigSipLocalPort < 0 || nOrigSipLocalPort > 65535 )
		{
			printf("invalid Port\n");
			return false;
		}

		printf("Input Orig SIP TransportType (0:UDP, 1:TCP) :\n");
		setPrompt((int &)(eOrigSipTransportType));
		if( eOrigSipTransportType < 0 || eOrigSipTransportType >= E_TRANSPORT_TYPE_NUM )
		{
			printf("invalid Transport Type\n");
			return false;
		}

		printf("Input Term SIP LocalIp :\n");
		setPrompt(stTermSipNet5Tuple.m_szLocalIp, Net5Tuple_t::E_CONST_MAX_IPADDR_LEN);
		if( !Socket::isIpAddress(stTermSipNet5Tuple.m_szLocalIp) )
		{
			printf("invalid IP address\n");
			return false;
		}

		printf("Input Term SIP LocalPort :\n");
		setPrompt(stTermSipNet5Tuple.m_nLocalPort);
		if( stTermSipNet5Tuple.m_nLocalPort < 0 || stTermSipNet5Tuple.m_nLocalPort > 65535 )
		{
			printf("invalid Port\n");
			return false;
		}

		printf("Input Term SIP RemoteIp :\n");
		setPrompt(stTermSipNet5Tuple.m_szRemoteIp, Net5Tuple_t::E_CONST_MAX_IPADDR_LEN);
		if( !Socket::isIpAddress(stTermSipNet5Tuple.m_szRemoteIp) )
		{
			printf("invalid IP address\n");
			return false;
		}

		printf("Input Term SIP RemotePort :\n");
		setPrompt(stTermSipNet5Tuple.m_nRemotePort);
		if( stTermSipNet5Tuple.m_nRemotePort < 0 || stTermSipNet5Tuple.m_nRemotePort > 65535 )
		{
			printf("invalid Port\n");
			return false;
		}

		printf("Input Term SIP TransportType (0:UDP, 1:TCP) :\n");
		setPrompt((int &)(stTermSipNet5Tuple.m_eTransportType));
		if( stTermSipNet5Tuple.m_eTransportType < 0 || stTermSipNet5Tuple.m_eTransportType >= E_TRANSPORT_TYPE_NUM )
		{
			printf("invalid Transport Type\n");
			return false;
		}
	}

	stInternalSocketCfg.m_nBlock = 0;
	stInternalSocketCfg.m_nUseLinger = 1;
	stInternalSocketCfg.m_nLingerWaitTime = 0;
	stInternalSocketCfg.m_nReuse = 1;
	stInternalSocketCfg.m_nRcvSocketBufferSize = 60000000;
	stInternalSocketCfg.m_nSndSocketBufferSize = 60000000;
	
	stInternalTransportCfg.m_stTcpConfig.m_nTcpRole = 1;
	stInternalTransportCfg.m_stTcpConfig.m_unSndBufferSize = 20000;
	stInternalTransportCfg.m_stTcpConfig.m_unMinRcvNotiSize = sizeof(SlbInternalMsgHdr);
	stInternalTransportCfg.m_stTcpConfig.m_pfnTcpRcvMatchCb = SimulVCM::cbCheckCompleteSlbMsg4Rcv;
	stInternalTransportCfg.m_stTcpConfig.m_pfnConnected4NonblockCb = SimulVCM::cbConnected4NonBlock;
	stInternalTransportCfg.m_unRcvBufferSize = 20000;
	stInternalTransportCfg.m_pfnRcvProcCb = SimulVCM::cbRecv;
	stInternalTransportCfg.m_pfnSndAfterProcCb = SimulVCM::cbSnd;
	stInternalTransportCfg.m_pfnDelTransportCb = cbDelTransport;

	int nTransportEntityIdx = -1;
	TransportEntity_t stTransportEntity;
	stTransportEntity.m_stNet5Tuple = stInternalNet5Tuple;
	stTransportEntity.m_nTcpRole = stInternalTransportCfg.m_stTcpConfig.m_nTcpRole;
	stTransportEntity.m_nMgrIndex = unAddCnt%TransportMgrInfo::E_MAX_TRANSPORT_MGR_NUM;
	stTransportEntity.m_stTermSipNet5Tuple = stTermSipNet5Tuple;
	strcpy(stTransportEntity.m_szOrigSipLocalIp, szOrigSipLocalIp);
	stTransportEntity.m_nOrigSipLocalPort = nOrigSipLocalPort;
	stTransportEntity.m_eOrigSipTransportType = eOrigSipTransportType;
	if( (nTransportEntityIdx = g_clsTransportEntityInfo.addTransportEntity(stTransportEntity)) < 0 )
	{
		return false;
	}
	
	if( g_clsTransportMgrInfo.getTransportMgr(stTransportEntity.m_nMgrIndex) )		
	{
		g_clsTransportMgrInfo.getTransportMgr(stTransportEntity.m_nMgrIndex)->addTransport(stTransportEntity.m_stNet5Tuple, stInternalSocketCfg, stInternalTransportCfg);
		char szNet5Tuple1[Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};
		char szNet5Tuple2[Net5Tuple_t::E_CONST_MAX_STR_LEN] = {0,};
		LogTest(E_LOG_INFO, "cmdAddTransportVCM : success : MgrIdx[%d] : Internal[%s], TermSip[%s]", 
											stTransportEntity.m_nMgrIndex,
											stTransportEntity.m_stNet5Tuple.getKeyStr(szNet5Tuple1, sizeof(szNet5Tuple1)),
											stTransportEntity.m_stTermSipNet5Tuple.getKeyStr(szNet5Tuple2, sizeof(szNet5Tuple2)));
		++unAddCnt;
		return true;
	}
	else
	{
		g_clsTransportEntityInfo.delTransportEntity(nTransportEntityIdx);
		return false;
	}
}

bool cmdSendSimpleMsg()
{
	int nTransportId;
	TransportEntity_t stTransportEntity;
	SendingData_t * pstSendingData = NULL;

	printf("Input Transport Id :\n");
	setPrompt(nTransportId);
	
	if( !g_clsTransportEntityInfo.getTransportEntity(nTransportId, stTransportEntity) )
	{
		printf("invalid Transport Id\n");
		return false;
	}

	if( !stTransportEntity.m_bUse )
	{
		printf("unusing Transport Id\n");
		return false;
	}

	if( stTransportEntity.m_stNet5Tuple.m_eTransportType == E_TRANSPORT_TYPE_udp )
	{
		printf("Input RemoteIp :\n");
		setPrompt(stTransportEntity.m_stNet5Tuple.m_szRemoteIp, Net5Tuple_t::E_CONST_MAX_IPADDR_LEN);
		if( !Socket::isIpAddress(stTransportEntity.m_stNet5Tuple.m_szRemoteIp) )
		{
			printf("invalid IP address\n");
			return false;
		}

		printf("Input RemotePort :\n");
		setPrompt(stTransportEntity.m_stNet5Tuple.m_nRemotePort);
		if( stTransportEntity.m_stNet5Tuple.m_nRemotePort < 0 || stTransportEntity.m_stNet5Tuple.m_nRemotePort > 65535 )
		{
			printf("invalid Port\n");
			return false;
		}
	}

	pstSendingData = (SendingData_t *)malloc(sizeof(SendingData_t));
	if( !pstSendingData )
	{
		printf("can't create sending data\n");
		return false;
	}
	pstSendingData->reset();
	
	printf("Input Sending Msg :\n");
	setPrompt(pstSendingData->m_Data, SendingData_t::E_CONST_MAX_DATA_LEN);
	pstSendingData->m_eTestTransportDataType = E_TEST_TRANSPORT_DATA_TYPE_simpleMsg;
	pstSendingData->m_unDataLen = strlen(pstSendingData->m_Data)+1;
	
	if( stTransportEntity.m_nMgrIndex >= 0 && stTransportEntity.m_nMgrIndex < TransportMgrInfo::E_MAX_TRANSPORT_MGR_NUM
		&& g_clsTransportMgrInfo.getTransportMgr(stTransportEntity.m_nMgrIndex)
		&& g_clsTransportMgrInfo.getTransportMgr(stTransportEntity.m_nMgrIndex)->sendData(stTransportEntity.m_stNet5Tuple, (char *)(pstSendingData), pstSendingData->getLength()) )
	{
		return true;
	}
	else
	{
		printf("fail to sending message\n");
		free(pstSendingData);
		return false;
	}
}

void cmdShowTransport()
{
	char szTransportEntity[TransportEntity::E_CONST_MAX_STR_LEN] = {0,};
	TransportEntity stTransportEntity;

	for( int i = 0; i < TransportEntityInfo::E_MAX_TRANSPORT_NUM; ++i )
	{
		if( g_clsTransportEntityInfo.getTransportEntity(i, stTransportEntity) && stTransportEntity.m_bUse )
		{
			szTransportEntity[0] = 0x00;
			printf("Index[%d] : %s\n", i, stTransportEntity.getStr(szTransportEntity, sizeof(szTransportEntity)));
		}
	}
}

void cmdDelTransport()
{
	int nTransportId;
	TransportEntity_t stTransportEntity;

	printf("Input Transport Id :\n");
	setPrompt(nTransportId);
	
	if( !g_clsTransportEntityInfo.getTransportEntity(nTransportId, stTransportEntity) )
	{
		printf("invalid Transport Id\n");
		return;
	}

	if( !stTransportEntity.m_bUse )
	{
		printf("unusing Transport Id\n");
		return;
	}

	if( stTransportEntity.m_nMgrIndex >= 0 && stTransportEntity.m_nMgrIndex < TransportMgrInfo::E_MAX_TRANSPORT_MGR_NUM
		&& g_clsTransportMgrInfo.getTransportMgr(stTransportEntity.m_nMgrIndex) )
	{
		g_clsTransportMgrInfo.getTransportMgr(stTransportEntity.m_nMgrIndex)->delTransport(stTransportEntity.m_stNet5Tuple);
	}
}

void cmdSetLogLevel()
{
	int nLogLevel;

	printf("Input LogLevel (0:Disable, 1:Err, 2:Warn, 3:Info, 4:Debug, 5:Stack) :\n");
	setPrompt(nLogLevel);
	if( nLogLevel >= 0 && nLogLevel <= 4 )
	{
		AsyncFileLog::m_fnSetLogLevel(g_nLogInstanceId, 0, (ELogLevel_t)nLogLevel);
		printf("Success to change LogLevel\n");
	}
	else
		printf("Invalid LogLevel\n");
}

void kill_No9_Myself()
{
	pid_t mypid=getpid();
	if( mypid ){		
		kill(mypid,SIGKILL);
		usleep(2000*1000);
		kill(mypid,SIGKILL);
		usleep(2000*1000);
		exit(0);
	}
}

int main(int _argc, char ** _argv)	
{
	if( _argc < 2 )
	{
		printf("input process name\n");
		return 1;
	}

	snprintf(g_szProcName, sizeof(g_szProcName)-1, "%s", _argv[1]);	
	AsyncFileLog::m_fnInit("./log/", g_szProcName, g_nLogInstanceId, 10000);
	AsyncFileLog::m_fnSetLogLevel(g_nLogInstanceId, 0, E_LOG_ERR);
	
	for( unsigned int i = 0; i < TransportMgrInfo::E_MAX_TRANSPORT_MGR_NUM; ++i )
	{
		char szMgrName[100] = {0,};
		snprintf(szMgrName, sizeof(szMgrName)-1, "%s_%d", _argv[1], i);		

		if( !g_clsTransportMgrInfo.addTransportMgr(szMgrName, TransportEntityInfo::E_MAX_TRANSPORT_NUM, 1000, 10000, -1) )
		{
			printf("fail to create Mgr !!\n");
			return 1;
		}
	}
	
	SimulVCM::m_fnInit();

	if( _argc >= 3 )
	{
		if( cmdAddTransport4Vcm(_argv[2]) )
			printf("success\n");
		else
			printf("fail\n");

		while(1)
		{
			sleep(100);
		}
	}
	else
	{
		int nMenuId;
		while(1)
		{
			printf("%s\n", s_pszTopMenu);
			setPrompt(nMenuId);
			
			if( nMenuId == 1 )
			{
				if( cmdAddTransport(0) )
					printf("success\n");
				else
					printf("fail\n");
			}
			else if( nMenuId == 2 )
			{
				cmdDelTransport();
			}
			else if( nMenuId == 3 )
			{
				cmdShowTransport();
			}
			else if( nMenuId == 4 )
			{
				if( cmdSendSimpleMsg() )
					printf("success\n");
				else
					printf("fail\n");
			}
			else if( nMenuId == 5 )
			{
				if( cmdAddTransport(1) )
					printf("success\n");
				else
					printf("fail\n");
			}
			else if( nMenuId == 6 )
			{
				if( cmdAddTransport4Vcm(NULL) )
					printf("success\n");
				else
					printf("fail\n");
			}
			else if( nMenuId == 7 )
			{
				cmdSetLogLevel();
			}
			else if( nMenuId == 8 )
			{
				kill_No9_Myself();
			}
			else
			{
				printf("invalid menu\n");
			}
		}
	}
}

