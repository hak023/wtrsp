#include "TrsgNodeMgr.h"
#include "main.h"
#include "TrsgNode.h"
#include "MainConfig.h"
#include "TrsgTransport.h"

using namespace eSipUtil;
TrsgNodeMgr * g_fnCreateTrsgNodeMgr()
{
	return TrsgNodeMgr::m_fnGetInstance();
}
TrsgNodeMgr * TrsgNodeMgr::m_pclsMy = NULL;
TrsgNodeMgr::TrsgNodeMgr() :m_clsTimer(TrsgNodeMgr::fnCbkTimerOut,"TrsgTimer")
{
	m_unTrGenId = 0;
	m_pclsQ = new QueueThread<TrsgConEv>;
	m_pclsQ->setName("TrsgNode");
	m_pclsQ->setMaxQueueSize(1000);
	m_pclsQ->setObject(this);
	m_pclsQ->setProcessCb(m_fnCbkProc);
	m_pclsQ->run();
   ENodeMgrApiError_t eRR;
   m_clsNode.m_fnLoadConfig("../config/MTM/trsg_node.cfg",&eRR);
   m_clsNode.m_fnLoadStatus("../config/MTM/trsg_node.status", E_NODE_STATUS_ACTION_CLEAR);
   m_clsNode.m_fnSetNotifyChangedFile(m_fnCbkNodeChanged, this);
}
TrsgNodeMgr::~TrsgNodeMgr(){}
TrsgNodeMgr * TrsgNodeMgr::m_fnGetInstance()
{
	if(m_pclsMy==NULL) m_pclsMy = new TrsgNodeMgr;
	return m_pclsMy;
}
unsigned int TrsgNodeMgr::m_fnDecisionNode(Net5Tuple & _rstResult/*, KString & _rclsTrsgName, KString & _rclsSessionID*/)
{
   ENodeMgrApiError_t eT; unsigned int unRet = 0;
   m_clsTrLock.m_fnWriteLock();
   unRet = m_clsNode.m_fnDecisionRteID(1000, _rstResult, &eT);
/*
   NodeConnection * pclsCon = m_clsNode.m_fnFindConnection(_rstResult);

   if(pclsCon && pclsCon->m_pclsUserObject)
   {
      TrssNode * pclsNode = NULL;
      pclsNode = (TrssNode*)pclsCon->m_pclsUserObject;
      _rclsTrsgName = pclsNode->m_clsTrsgName;
      _rclsTrssName = pclsNode->m_clsTrssName;
      _rclsSessionID = pclsNode->m_clsSessionID;
   }
*/
   m_clsTrLock.m_fnUnlock();
   return unRet;
}
bool TrsgNodeMgr::m_fnIsAllDeactive()
{
   m_clsTrLock.m_fnReadLock();
   ListItr_t stItor;
   NodeSeq * pclsSeq = m_clsNode.m_fnBeginSeq(stItor);
   if (pclsSeq == NULL)
   {
      IFLOG(E_LOG_DEBUG, "%s %s %d SEQ is Empty.", __FILE__, __func__, __LINE__);
      m_clsTrLock.m_fnUnlock();
      return true;
   }
   pclsSeq->m_clsLock.m_fnWriteLock();
   NodeRatio * pclsFind = pclsSeq->m_pclsHead;
   for (unsigned int i = 0; i < pclsSeq->m_unCnt; i++)
   {
      if (pclsFind)
      {
         if (pclsFind->m_pclsRte)
         {
            if (pclsFind->m_pclsRte->m_fnGetFBlock() == 0
                  && pclsFind->m_pclsRte->m_unMBlock == 0)
            {
               pclsSeq->m_clsLock.m_fnUnlock();
               m_clsTrLock.m_fnUnlock();
               return false;
            }
         }
         else
         {
            IFLOG(E_LOG_ERR, "%s %s %d Route is NULL(Config Error)",
                  __FILE__, __func__, __LINE__);
            pclsSeq->m_clsLock.m_fnUnlock();
            m_clsTrLock.m_fnUnlock();
            return false;
         }
         pclsFind = pclsFind->m_pclsNext;
      }
      else
      {
         IFLOG(E_LOG_ERR, "%s %s %d Ratio is NULL(Config Error)", __FILE__,
               __func__, __LINE__);
         pclsSeq->m_clsLock.m_fnUnlock();
         m_clsTrLock.m_fnUnlock();
         return false;
      }
   }
   pclsSeq->m_clsLock.m_fnUnlock();
   m_clsTrLock.m_fnUnlock();
   return true;
}
void TrsgNodeMgr::m_fnPutEv(TrsgConEv * _pclsEv)
{
	TrsgNodeMgr * pclsMy = TrsgNodeMgr::m_fnGetInstance();
	if(pclsMy->m_pclsQ->put(_pclsEv) == false)
	{
		IFLOG(E_LOG_ERR,"TrsgNodeMgr Queue Full");
		delete _pclsEv;
	}
}
// CallBack Method
void TrsgNodeMgr::m_fnCbkProc(TrsgConEv *_pclsEv, void *_pvOwner)
{
	TrsgNodeMgr *pclsMy = TrsgNodeMgr::m_fnGetInstance();

	if (_pclsEv->m_eT == E_TRSG_IF_CONNECTED)
	{
		pclsMy->m_fnProcConnected(_pclsEv);
	}
	else if (_pclsEv->m_eT == E_TRSG_IF_DISCONNECTED)
	{
		pclsMy->m_fnProcDisConnected(_pclsEv);
	}
	else if (_pclsEv->m_eT == E_TRSG_IF_ESTABLISH_SESSION_RES)
	{
		pclsMy->m_fnProcRecvEstablishSesRes(_pclsEv);
	}
	else if (_pclsEv->m_eT == E_TRSG_IF_ESTABLISH_SESSION_RES_TMR)
	{
		pclsMy->m_fnProcEstablishSesResTimeOut(_pclsEv);
	}
	else if (_pclsEv->m_eT == E_TRSG_IF_LINK_TEST_RES)
	{
		pclsMy->m_fnProcRecvLinkTestRes(_pclsEv);
	}
	else if (_pclsEv->m_eT == E_TRSG_IF_LINK_TEST_RES_TMR)
	{
		pclsMy->m_fnProcSendLinkTestReq(_pclsEv);
	}
	else if (_pclsEv->m_eT == E_TRSG_IF_RECONNECTING)
	{
		pclsMy->m_fnProcReconnecting(_pclsEv);
	}
	else if (_pclsEv->m_eT == E_TRSG_IF_ADD_CONNECTION)
	{ //add trans
		pclsMy->m_fnProcAddConnection();
	}
	else if (_pclsEv->m_eT == E_TRSG_IF_DEL_CONNECTION)
	{ //dell trans
		pclsMy->m_fnProcDelConnection();
	}
	else
	{
		IFLOG(E_LOG_ERR, "[TrsgNodeMgr]Unknown Event Type(%d)", _pclsEv->m_eT);
	}
	delete _pclsEv;
}
void TrsgNodeMgr::m_fnCbkNodeChanged(ColumFile * _pclsObj)
{
   //LOC 는 수정 할 수 없음. RMT와 RTE, SEQ만 수정 가능함.

   IFLOG(E_LOG_INFO, "Change Trsg Node Config");
   TrsgNodeMgr * pclsMy = (TrsgNodeMgr*)_pclsObj->m_fnGetUser();
   ENodeMgrApiError_t eErr;
   pclsMy->m_clsTrLock.m_fnWriteLock();
   NodeMgr clsOldNodeMgr = pclsMy->m_clsNode;//copy
   pclsMy->m_clsNode.m_fnReLoadConfig(&eErr);

   ListItr_t stOldItor; TrsgTransport * pclsTrans = TrsgTransport::m_fnGetInstance();
   NodeRte * pclsOldRte = clsOldNodeMgr.m_fnBeginRte(stOldItor);
   while(pclsOldRte)//Old Rte 설정을 순회하여 New Rte 설정에 존재하던 설정인지 확인한다.
   {
      KString pclsOldEnable = pclsOldRte->m_fnFindExtParam("ENABLE");
      NodeRte* pclsNewRte = pclsMy->m_clsNode.m_fnFindRteFrom5Tuple(pclsOldRte->m_stAddr);
      if(pclsNewRte)//존재할때
      {
/*
         KString pclsNewEnable = pclsNewRte->m_fnFindExtParam("ENABLE");
         if(pclsOldEnable == pclsNewRte)//같으면 할 작업 없음.
         {
            //none
         }
         else if((KUINT)pclsOldEnable == 1 && (KUINT)pclsNewEnable == 0)//끊어질 경우 Rel Send
         {
            if(pclsMy->m_fnConExists(pclsOldRte->m_stAddr))//존재하는 Connection만
            {
               TrsgConEv * pclsNew = new TrsgConEv;
               pclsNew->m_eT = E_TRSG_IF_CLOSE_SESSION_REQ;
               pclsNew->m_stAddr = pclsOldRte->m_stAddr;
               m_fnPutEv(pclsNew);
            }
         }
         else if((KUINT)pclsOldEnable == 0 && (KUINT)pclsNewEnable == 1)//붙을 경우 add Trans
         {
            if(pclsMy->m_fnConExists(pclsNewRte->m_stAddr) == false)//종료된 커넥션인지 확인
            {
               NpdbTransport * pclsTrans = NpdbTransport::m_fnGetInstance();
               pclsTrans->m_fnAdd(pclsNewRte->m_stAddr,E_TCP_ROLE_CLIENT);
            }
         }
*/
      }
      else//존재하지 않을때
      {
         //삭제된 Rte가 있음. 커넥션 확인 후 Transfer Del.
         if(pclsMy->m_fnConExists(pclsOldRte->m_stAddr))//존재하는 커넥션 확인 후 Del
            pclsTrans->m_fnDelConnection(pclsOldRte->m_stAddr);
      }
      pclsOldRte = clsOldNodeMgr.m_fnNextRte(stOldItor);
   }
   pclsMy->m_clsTrLock.m_fnUnlock();
}
void TrsgNodeMgr::m_fnProcConnected(TrsgConEv *_pclsEv)
{
   KString clsSysSrc, clsPeerSrc;
   KString clsRteName = KNULL;

   m_clsNode.m_fnChangeConnectivity(_pclsEv->m_stAddr,false);
   TrsgNode * pclsNode = m_fnGetTrsgNode(_pclsEv);
   if(pclsNode == NULL) return;

   pclsNode->m_fnConnected();
}
void TrsgNodeMgr::m_fnProcDisConnected(TrsgConEv *_pclsEv)
{
   KString clsRteName = KNULL;
   KString clsSysSrc;

   m_clsNode.m_fnChangeConnectivity(_pclsEv->m_stAddr,false);   
	TrsgNode * pclsNode = m_fnGetTrsgNode(_pclsEv);
	if (pclsNode == NULL) return;

	pclsNode->m_fnDisConnected();
}
void TrsgNodeMgr::m_fnProcRecvEstablishSesRes(TrsgConEv *_pclsEv)
{
   TrsgNode * pclsNode = m_fnGetTrsgNode(_pclsEv);
   if (pclsNode == NULL) return;

   pclsNode->m_fnRecvEstablishSesRes(_pclsEv->m_clsXml);
}
void TrsgNodeMgr::m_fnProcSendLinkTestReq(TrsgConEv *_pclsEv)
{
	TrsgNode * pclsNode = _pclsEv->m_pclsTrsg;
	if (pclsNode == NULL) return;
	pclsNode->m_fnSendLinkTestReq();
}
void TrsgNodeMgr::m_fnProcRecvLinkTestRes(TrsgConEv *_pclsEv)
{ 
	TrsgNode * pclsNode = m_fnGetTrsgNode(_pclsEv);
	if (pclsNode == NULL) return;
	pclsNode->m_fnRecvLinkTestRes(_pclsEv->m_clsXml);
}
void TrsgNodeMgr::m_fnProcEstablishSesResTimeOut(TrsgConEv *_pclsEv)
{
	TrsgNode * pclsNode = _pclsEv->m_pclsTrsg;
	if (pclsNode == NULL) return;
   
	pclsNode->m_fnReSendEstablishSesReq();
}
void TrsgNodeMgr::m_fnProcReconnecting(TrsgConEv * _pclsEv)
{
   TrsgNode * pclsNode = _pclsEv->m_pclsTrsg;
   if(pclsNode == NULL)
   {
      IFLOG(E_LOG_ERR,"TrsgNode Reconnecting Error");
      return;
   }
   pclsNode->m_fnReconnecting();
}
void TrsgNodeMgr::m_fnProcAddConnection()
{
   ListItr_t stItor;
   TrsgTransport * pclsTrans = TrsgTransport::m_fnGetInstance();
   m_clsTrLock.m_fnReadLock();
   NodeRte * pclsRte = m_clsNode.m_fnBeginRte(stItor);
   if(pclsRte == NULL)
   {
      m_clsTrLock.m_fnUnlock();
      return;
   }
   while(pclsRte)
   {
      KString pclsEnable = pclsRte->m_fnFindExtParam("ENABLE");
      if((KINT)pclsEnable == 1)
      {
         eSipUtil::KString clsAddrString;
         clsAddrString.m_fnReSize(1024);
         IFLOG(E_LOG_DEBUG, "TRSG Add Connection[%s]", pclsRte->m_stAddr.getStr((eSipUtil::KSTR)clsAddrString,1024,false));
         pclsTrans->m_fnAdd(pclsRte->m_stAddr,E_TCP_ROLE_CLIENT);
      }
      else
      {
      }
      pclsRte = m_clsNode.m_fnNextRte(stItor);
   }
   m_clsTrLock.m_fnUnlock();
}
void TrsgNodeMgr::m_fnProcDelConnection()
{
   ListItr_t stItor;
   TrsgTransport * pclsTrans = TrsgTransport::m_fnGetInstance();
   m_clsTrLock.m_fnReadLock();
   NodeRte * pclsRte = m_clsNode.m_fnBeginRte(stItor);
   if(pclsRte == NULL)
   {
      m_clsTrLock.m_fnReadLock();
      return;
   }
   while(pclsRte)
   {
      KString pclsEnable = pclsRte->m_fnFindExtParam("ENABLE");
      if((KINT)pclsEnable == 1)
      {
         eSipUtil::KString clsAddrString;
         clsAddrString.m_fnReSize(1024);
         IFLOG(E_LOG_DEBUG, "TRSG Del Connection[%s]", pclsRte->m_stAddr.getStr((eSipUtil::KSTR)clsAddrString,1024,false));
         pclsTrans->m_fnDelConnection(pclsRte->m_stAddr);
      }
      else
      {
      }
      pclsRte = m_clsNode.m_fnNextRte(stItor);
   }
   m_clsTrLock.m_fnUnlock();
}

void TrsgNodeMgr::m_fnGetNodeRteInfo(const eSipUtil::Net5Tuple & _rstAddr, unsigned int * _punRteId, eSipUtil::KString & _rclsRteName, unsigned int * _punEnable)
{
   m_clsTrLock.m_fnReadLock();
   NodeRte * pclsRte = m_clsNode.m_fnFindRteFrom5Tuple(_rstAddr);
   if(pclsRte == NULL)
   {
      m_clsTrLock.m_fnUnlock();
      return;
   }
   if(pclsRte)
   {
      (*_punRteId) = pclsRte->m_unID;
      _rclsRteName = pclsRte->m_clsName;
      KString pclsEnable = pclsRte->m_fnFindExtParam("ENABLE");
      (*_punEnable) = (KUINT)pclsEnable;
   }
   m_clsTrLock.m_fnUnlock();
}
const char * TrsgNodeMgr::m_fnGetNodeRteName(const eSipUtil::Net5Tuple & _rstAddr)
{
   m_clsTrLock.m_fnReadLock();
   NodeRte * pclsRte = m_clsNode.m_fnFindRteFrom5Tuple(_rstAddr);
   if(pclsRte == NULL)
   {
      m_clsTrLock.m_fnUnlock();
      return NULL;
   }
   else
   {
      m_clsTrLock.m_fnUnlock();
      return (eSipUtil::KCSTR)pclsRte->m_clsName;
   }
}
void TrsgNodeMgr::fnCbkTimerOut(TrsgConEv * _pclsData)
{
	m_fnPutEv(_pclsData);
}
bool TrsgNodeMgr::m_fnConExists(const Net5Tuple & _rstAddr)
{
   NodeConnection * pclsCon = m_clsNode.m_fnFindConnection(_rstAddr);
   if(pclsCon)
   {
      if(pclsCon->m_pclsUserObject)
      {
         TrsgNode * pclsNode = (TrsgNode*)pclsCon->m_pclsUserObject;
         if(pclsNode->m_bConnect == true)
         {
            return true;
         }
      }
   }
   return false;
}
TrsgNode * TrsgNodeMgr::m_fnGetTrsgNode(TrsgConEv *_pclsEv)
{
   NodeConnection * pclsCon = m_clsNode.m_fnFindConnection(_pclsEv->m_stAddr);
   if(pclsCon == NULL)
   {
      pclsCon = m_clsNode.m_fnChangeConnectivity(_pclsEv->m_stAddr,false);
      if(pclsCon == NULL)
      {
         IFLOG(E_LOG_INFO,"Get Trsg Node Error");
         return NULL;
      }
   }
   TrsgNode * pclsNode = NULL;
   if(pclsCon->m_pclsUserObject == NULL)
   {
      pclsNode = new TrsgNode;
      pclsNode->m_pclsOwner = pclsCon;
      pclsCon->m_fnSetUserObject(pclsNode);
   }
   else
   {
      pclsNode = (TrsgNode*)pclsCon->m_pclsUserObject;
   }
   return pclsNode;
}
