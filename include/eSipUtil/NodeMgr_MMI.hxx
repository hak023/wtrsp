#ifndef _NODEMGR_MMI_H_
#define _NODEMGR_MMI_H_
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

namespace eSipUtil
{
#define DEF_LM_DESC_LEN                128
typedef struct NodeMgrRsp_t
{
	int m_nResult;
	int m_nReason;
	char m_szReasonDesc[DEF_LM_DESC_LEN];
	int m_nNumber;
	void m_fnSetRsp(bool _bSuccess, int _nReason, const char * _pszDesc)
	{
		if(_bSuccess) m_nResult=0;
		else m_nResult = -1;
		m_nReason=_nReason;snprintf(m_szReasonDesc,DEF_LM_DESC_LEN-1,_pszDesc);
	}
}NodeMgrRsp_t;
typedef struct NodeMgrLocReq_t
{
	enum{E_IP_SZ=64,E_NAME_SZ=64};
	NodeMgrLocReq_t()
	{
		m_nID = -1;memset(m_szName,0x00,E_NAME_SZ);memset(m_szIp,0x00,E_IP_SZ);
		m_nPort = -1; m_eProto = -1; m_eMode = -1;
	}
	int m_nID;
	char m_szName[E_NAME_SZ];
	char m_szIp[E_IP_SZ];
	int m_nPort;     
	int m_eProto;     // tcp=0/udp=1/tls=2   
	int m_eMode;     // server=0/client=1/accepted=2
}NodeMgrLocReq_t;
typedef struct NodeMgrLocRsp_t
{
	NodeMgrRsp_t m_stRsp;
	NodeMgrLocReq_t m_stLoc;
	void m_fnSetRsp(bool _bSuccess,int _nReason, const char * _pszDesc)
	{
		m_stRsp.m_fnSetRsp(_bSuccess,_nReason,_pszDesc);
	}
	void m_fnCopyReq(void * _pstReq)
	{
		memcpy(&m_stLoc,_pstReq,sizeof(NodeMgrLocReq_t));
	}
}NodeMgrAddLocRsp_t;
typedef struct NodeMgrRmtReq_t
{
	enum{E_IP_SZ=64,E_NAME_SZ=64};
	NodeMgrRmtReq_t()
	{
		m_nID = -1; memset(m_szName,0x00, E_NAME_SZ);memset(m_szIp,0x00,E_IP_SZ);
		m_nPort = -1; m_eProto = -1;
	}
	int m_nID;
	char m_szName[E_NAME_SZ];
	char m_szIp[E_IP_SZ];
	int m_nPort;     
	int m_eProto;     // tcp=0/udp=1/tls=2   
}NodeMgrRmtReq_t;
typedef struct NodeMgrRmtRsp_t
{
	NodeMgrRsp_t m_stRsp;
	NodeMgrRmtReq_t m_stRmt;
	void m_fnSetRsp(bool _bSuccess,int _nReason, const char * _pszDesc)
	{
		m_stRsp.m_fnSetRsp(_bSuccess,_nReason,_pszDesc);
	}
	void m_fnCopyReq(void * _pstReq)
	{
		memcpy(&m_stRmt,_pstReq,sizeof(NodeMgrRmtReq_t));
	}
}NodeMgrRmtRsp_t;
typedef struct NodeMgrRteReq_t
{
	enum{E_NAME_SZ=64};
	NodeMgrRteReq_t()
	{
		m_nID = -1; memset(m_szName,0x00,E_NAME_SZ);m_nLocID = -1; m_nRmtID = -1; m_nMBlock = -1;	
		m_nFBlock = -1; m_nCnt = -1;
	}
	int m_nID;
	char m_szName[E_NAME_SZ];
	int m_nLocID;
	int m_nRmtID;
	int m_nMBlock;       // 1-block, 0 - unblock
	int m_nFBlock;       //  1-block, 0- unblock
	int m_nCnt;           // Route Usage Session Count
}NodeMgrRteReq_t;
typedef struct NodeMgrRteRsp_t
{
	NodeMgrRsp_t m_stRsp;
	NodeMgrRteReq_t m_stRte;
	void m_fnSetRsp(bool _bSuccess,int _nReason, const char * _pszDesc)
	{
		m_stRsp.m_fnSetRsp(_bSuccess,_nReason,_pszDesc);
	}
	void m_fnCopyReq(void * _pstReq)
	{
		memcpy(&m_stRte,_pstReq,sizeof(NodeMgrRteReq_t));
	}
}NodeMgrRteRsp_t;
typedef struct NodeMgrRatio_t
{
	NodeMgrRatio_t()
	{
		m_nRteID = -1; m_nRatio = -1;	
	}
	int m_nRteID;
	int m_nRatio;
}NodeMgrRatio_t;
typedef struct NodeMgrSeqReq_t
{
	enum{E_NAME_SZ=64,E_RATE_NUMS=100};
	NodeMgrSeqReq_t()
	{
		m_nID = -1;memset(m_szName,0x00,E_NAME_SZ); m_nRatioNum = -1;
	}
	int m_nID;
	char m_szName[E_NAME_SZ];
	int m_nRatioNum;
	NodeMgrRatio_t m_arrRatio[E_RATE_NUMS];
}NodeMgrSeqReq_t;
typedef struct NodeMgrSeqRsp_t
{
	NodeMgrRsp_t m_stRsp;
	NodeMgrSeqReq_t m_stSeq;
	void m_fnSetRsp(bool _bSuccess,int _nReason, const char * _pszDesc)
	{
		m_stRsp.m_fnSetRsp(_bSuccess,_nReason,_pszDesc);
	}
	void m_fnCopyReq(void * _pstReq)
	{
		memcpy(&m_stSeq,_pstReq,sizeof(NodeMgrSeqReq_t));
	}
}NodeMgrSeqRsp_t;
/********************** LOC Defined *****************************************************/
typedef NodeMgrLocReq_t NodeMgrAddLocReq_t;
typedef NodeMgrLocRsp_t NodeMgrAddLocRsp_t;
//typedef NodeMgrLocReq_t NodeMgrModLocReq_t;
//typedef NodeMgrLocRsp_t NodeMgrModLocRsp_t;
typedef NodeMgrLocReq_t NodeMgrDelLocReq_t;
typedef NodeMgrLocRsp_t NodeMgrDelLocRsp_t;
typedef NodeMgrLocReq_t NodeMgrDisLocReq_t;
typedef struct NodeMgrDisLocRsp_t
{
	enum{E_DIS_NUMS=100};
	NodeMgrRsp_t m_stReason;
	NodeMgrLocReq_t m_arrLoc[E_DIS_NUMS];
}NodeMgrDisLocRsp_t;
typedef NodeMgrRmtReq_t NodeMgrAddRmtReq_t;
typedef NodeMgrRmtRsp_t NodeMgrAddRmtRsp_t;
//typedef NodeMgrRmtReq_t NodeMgrModRmtReq_t;
//typedef NodeMgrRmtRsp_t NodeMgrModRmtRsp_t;
typedef NodeMgrRmtReq_t NodeMgrDelRmtReq_t;
typedef NodeMgrRmtRsp_t NodeMgrDelRmtRsp_t;
typedef NodeMgrRmtReq_t NodeMgrDisRmtReq_t;
typedef struct NodeMgrDisRmtRsp_t
{
	enum{E_DIS_NUMS=100};
	NodeMgrRsp_t m_stReason;
	NodeMgrRmtReq_t m_arrRmt[E_DIS_NUMS];
}NodeMgrDisRmtRsp_t;
typedef NodeMgrRteReq_t NodeMgrAddRteReq_t;
typedef NodeMgrRteRsp_t NodeMgrAddRteRsp_t;
//typedef NodeMgrRteReq_t NodeMgrModRteReq_t;
//typedef NodeMgrRteRsp_t NodeMgrModRteRsp_t;
typedef struct NodeMgrBlockRteReq_t
{
	int m_nID;
	int m_nMBlock;
}NodeMgrBlockRteReq_t;
typedef NodeMgrRteRsp_t NodeMgrBlockRteRsp_t;
typedef NodeMgrRteReq_t NodeMgrDelRteReq_t;
typedef NodeMgrRteRsp_t NodeMgrDelRteRsp_t;
typedef NodeMgrRteReq_t NodeMgrDisRteReq_t;
typedef struct NodeMgrDisRteRsp_t
{
	enum{E_DIS_NUMS=100};
	NodeMgrRsp_t m_stReason;
	NodeMgrRteReq_t m_arrRte[E_DIS_NUMS];
}NodeMgrDisRteRsp_t;
typedef NodeMgrSeqReq_t NodeMgrAddSeqReq_t;
typedef NodeMgrSeqRsp_t NodeMgrAddSeqRsp_t;
typedef NodeMgrSeqReq_t NodeMgrModSeqReq_t;
typedef NodeMgrSeqRsp_t NodeMgrModSeqRsp_t;
typedef NodeMgrSeqReq_t NodeMgrDelSeqReq_t;
typedef NodeMgrSeqRsp_t NodeMgrDelSeqRsp_t;
typedef NodeMgrSeqReq_t NodeMgrDisSeqReq_t;
typedef struct NodeMgrDisSeqRsp_t
{
	enum{E_DIS_NUMS=100};
	NodeMgrRsp_t m_stReason;
	NodeMgrSeqReq_t m_arrSeq[E_DIS_NUMS];
}NodeMgrDisSeqRsp_t;
}
#endif


