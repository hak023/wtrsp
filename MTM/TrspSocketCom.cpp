
#include "ConfigFile.hxx"
#include "Trsplog.hxx"
#include "TrspSocketCom.h"
#include "main.h"

using namespace eSipUtil;

TrspSocketCom::TrspSocketCom(const char * _pszName)
{
	//----------> Transport Mgr Options
	m_szName = _pszName;
	m_stOption.m_pszMgrName = (KCSTR)m_szName;

	m_stOption.m_unMaxTransportSize = 100;
	m_stOption.m_unMaxEpollSize = 1000;
	m_stOption.m_unMaxThreadQueueSize = 60000;

	m_stOption.m_nThreadQueueWaitMs = -1;
	m_stOption.m_bUseSendThread = false;
	m_stOption.m_pfnQueueFullCb = NULL;
	//----------> Socket Options
	m_stSocCfg.m_nBlock = 1;
	m_stSocCfg.m_nUseLinger = 1;
	m_stSocCfg.m_nLingerWaitTime = 0;
	m_stSocCfg.m_nReuse = 1;
	m_stSocCfg.m_nDscp = -1;

	m_stSocCfg.m_nRcvSocketBufferSize = DEF_CFG_1M*30;
	m_stSocCfg.m_nSndSocketBufferSize = DEF_CFG_1M*30;

	//----------> Transport Options
	m_stTransCfg.m_unRcvBufferSize = DEF_CFG_1M; 
	//m_stTransCfg.m_stTcpConfig.m_unSndBufferSize = DEF_CFG_1M;
	m_stTransCfg.m_stTcpConfig.m_unSndBufferSize = 10000000;//10 mbyte
	m_stTransCfg.m_stTcpConfig.m_unMinRcvNotiSize = 4;
	m_stTransCfg.m_pfnRcvProcCb = s_fnCbkRecvPacket;
	m_stTransCfg.m_pfnSndPrevProcCb = s_fnCbkSendPacket;
	m_stTransCfg.m_pfnDelTransportCb = s_fnCbkDisconnected;
	m_stTransCfg.m_stTcpConfig.m_pfnAcceptConnectionCb = s_fnCbkAcceptConnected;
	m_stTransCfg.m_stTcpConfig.m_pfnConnected4NonblockCb = s_fnCbkClientConnected;
	m_stTransCfg.m_stTcpConfig.m_pfnTcpRcvMatchCb = s_fnCbkAnalizePacket;
	//----------> Create TransportMgr
	m_pclsTrans = new TransportMgr(m_stOption);
	m_pclsTrans->setAppData(this);
}
TrspSocketCom::~TrspSocketCom()
{
	delete m_pclsTrans;
}
//==========> Server or Client Transport Add
void TrspSocketCom::m_fnAdd(const Net5Tuple_t & _rstNet5Tuple, ETcpRole_t _eRole)
{
	TransportConfig   stTransCfg = m_stTransCfg; stTransCfg.m_stTcpConfig.m_nTcpRole = _eRole;
	m_pclsTrans->addTransport(_rstNet5Tuple, m_stSocCfg, stTransCfg);
}
//==========> Server or Client Transport Delete
void TrspSocketCom::m_fnDel(const Net5Tuple_t & _rstNet5Tuple)
{
	m_pclsTrans->delTransport(_rstNet5Tuple);
}
//==========> Send Data
bool TrspSocketCom::m_fnSend(const Net5Tuple_t & _rstNet5Tuple,void * _pvData,unsigned int _unSndLen)
{
	ETransportErr_t eTransportErr = E_TRSP_ERR_NONE;
	while(true)
	{
		if(m_pclsTrans->sendData(_rstNet5Tuple, (char*)_pvData,_unSndLen, -1, NULL, &eTransportErr) < 0)
		{
			if(eTransportErr == E_TRSP_ERR_FAIL_SEND_REMAIN_BUFFER)
			{
				//sleep
				IFLOG(E_LOG_ERR, "[%s] SendSocket Buffer Full. Waiting.",(KCSTR)m_szName);
				eTransportErr = E_TRSP_ERR_NONE;
				usleep( 1000 * 1000 );//1sec
				continue;
			}
			else
			{
				delete [] (char*)_pvData; return false;
			}
		}
		else return true;
	}
}
void TrspSocketCom::s_fnCbkAcceptConnected(TransportMgr & _rclsTransportMgr,
										Net5Tuple_t & _rstNet5Tuple)
{
	TrspSocketCom * pclsMy = (TrspSocketCom*)_rclsTransportMgr.getAppData();
	pclsMy->m_fnCbkNotify(E_TCP_NOTI_ACCEPT_CON,_rstNet5Tuple,E_TRSP_ERR_NONE);
}
void TrspSocketCom::s_fnCbkClientConnected(TransportMgr & _rclsTransportMgr,
										const Net5Tuple_t & _rstNet5Tuple)
{
	TrspSocketCom * pclsMy = (TrspSocketCom*)_rclsTransportMgr.getAppData();
	pclsMy->m_fnCbkNotify(E_TCP_NOTI_CLIENT_CON,_rstNet5Tuple,E_TRSP_ERR_NONE);
}
void TrspSocketCom::s_fnCbkDisconnected(TransportMgr & _rclsTransportMgr,
										const Net5Tuple_t & _rstNet5Tuple,
										ETransportErr_t _eTrspErr)
{
	TrspSocketCom * pclsMy = (TrspSocketCom*)_rclsTransportMgr.getAppData();
	pclsMy->m_fnCbkNotify(E_TCP_NOTI_DISCON,_rstNet5Tuple,_eTrspErr);
}
int TrspSocketCom::s_fnCbkAnalizePacket(TransportMgr & _rclsTransportMgr,
										const char * _pRcvData, 
										unsigned int _unRcvDataLen, 
										const Net5Tuple_t & _rstNet5Tuple, 
										void * _pvUser)
{
	TrspSocketCom * pclsMy = (TrspSocketCom*)_rclsTransportMgr.getAppData();
	return pclsMy->m_fnCbkAnalizePacket(_pRcvData,_unRcvDataLen,_rstNet5Tuple);
}
void TrspSocketCom::s_fnCbkSendPacket( TransportMgr & _rclsTransportMgr,
										const char * _pData, 
										unsigned int _unDataLen, 
										const Net5Tuple_t & _rstNet5Tuple,
										void * _pSndAppData)
{
	TrspSocketCom * pclsMy = (TrspSocketCom*)_rclsTransportMgr.getAppData();
	pclsMy->m_fnCbkTracePacket(true,_pData,_unDataLen,_rstNet5Tuple,_pSndAppData);
}
void TrspSocketCom::s_fnCbkRecvPacket(TransportMgr & _rclsTransportMgr,
										char * _pclsData, 
										unsigned int _unDataLen,  
										const Net5Tuple_t & _rstNet5Tuple)
{
	TrspSocketCom * pclsMy = (TrspSocketCom*)_rclsTransportMgr.getAppData();
	pclsMy->m_fnCbkTracePacket(false,_pclsData,_unDataLen,_rstNet5Tuple,NULL);
}
const char * TrspSocketCom::m_fnGetEvString(ETcpNotiEv_t _eT)
{
	if(_eT == E_TCP_NOTI_ACCEPT_CON) return "E_TCP_NOTI_ACCEPT_CON";
	else if(_eT == E_TCP_NOTI_CLIENT_CON) return "E_TCP_NOTI_CLIENT_CON";
	else if(_eT == E_TCP_NOTI_DISCON) return "E_TCP_NOTI_DISCON";
	else return "E_TCP_NOTI_NONE";
}

