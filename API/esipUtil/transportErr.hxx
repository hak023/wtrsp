
#ifndef ESIPUTIL_TRANSPORT_ERR_H
#define ESIPUTIL_TRANSPORT_ERR_H

namespace eSipUtil
{

typedef enum
{
	E_TRSP_ERR_NONE = 0,
	E_TRSP_ERR_TRSP_MGR_NOT_ACTIVE,
	E_TRSP_ERR_INVALID_ARG,
	E_TRSP_ERR_QUEUE_FULL,
	E_TRSP_ERR_INTERNAL_ERR,
	E_TRSP_ERR_FAIL_CREATE_INTERNAL_MSG,
	E_TRSP_ERR_FAIL_ADD_TRSP_DATA,
	E_TRSP_ERR_FAIL_ADD_MAP_INFO,	
	E_TRSP_ERR_NOT_FOUND_TRSP_DATA_IDX,
	E_TRSP_ERR_TRSP_DATA_UNUSED,
	E_TRSP_ERR_INVALID_SND_BUFFER,
	E_TRSP_ERR_INVALID_RCV_BUFFER,
	E_TRSP_ERR_FAIL_CREATE_RCV_BUFFER,
	E_TRSP_ERR_SEND_BUFFER_FULL,
	E_TRSP_ERR_INVALID_TCP_STATE,
	E_TRSP_ERR_TRSP_NOT_INIT,
	E_TRSP_ERR_TRSP_ALREADY_INIT,
	E_TRSP_ERR_FAIL_CREATE_TRSP,
	E_TRSP_ERR_FAIL_CREATE_SOCKET,
	E_TRSP_ERR_FAIL_BIND_SOCKET,
	E_TRSP_ERR_FAIL_ACCEPT_SOCKET,
	E_TRSP_ERR_FAIL_SYNC_CONNECT_SOCKET,
	E_TRSP_ERR_FAIL_ASYNC_CONNECT_SOCKET,
	E_TRSP_ERR_FAIL_SEND_SOCKET,
	E_TRSP_ERR_FAIL_RECV_SOCKET,
	E_TRSP_ERR_FAIL_SEND_REMAIN_BUFFER,
	E_TRSP_ERR_TCP_CLOSE_BYPEER,
	E_TRSP_ERR_INTERRUPT,

	E_TRSP_ERR_NUM,
}ETransportErr_t;

const char * g_fnGetTransportErrStr(ETransportErr_t _eTrspErr);
bool g_fnIsDelTransport(ETransportErr_t _eTrspErr);

}

#endif