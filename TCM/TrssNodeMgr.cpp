#include "TrssNodeMgr.h"
#include "main.h"
#include "TrssNode.h"
#include "MainConfig.h"
#include "TrssTransport.h"

using namespace eSipUtil;
TrssNodeMgr * g_fnCreateTrssNodeMgr()
{
	return TrssNodeMgr::m_fnGetInstance();
}
TrssNodeMgr * TrssNodeMgr::m_pclsMy = NULL;
TrssNodeMgr::TrssNodeMgr() :m_clsTimer(TrssNodeMgr::fnCbkTimerOut,"TrssTimer")
{
	m_unTrGenId = 0;

	m_pclsQ = new QueueThread<TrssConEv>;
	m_pclsQ->setName("TrssNodeMgr");
	m_pclsQ->setMaxQueueSize(1000);
	m_pclsQ->setObject(this);
	m_pclsQ->setProcessCb(m_fnCbkProc);
	m_pclsQ->run();

	ENodeMgrApiError_t eRR;
	m_clsNode.m_fnLoadConfig("../config/TCM/trss_node.cfg",&eRR);
	m_clsNode.m_fnLoadStatus("../config/TCM/trss_node.status", E_NODE_STATUS_ACTION_APPLY);
	m_clsNode.m_fnSetNotifyChangedFile(m_fnCbkNodeChanged, this);
}
TrssNodeMgr::~TrssNodeMgr(){}
TrssNodeMgr * TrssNodeMgr::m_fnGetInstance()
{
	if(m_pclsMy==NULL) m_pclsMy = new TrssNodeMgr;
	return m_pclsMy;
}
unsigned int TrssNodeMgr::m_fnGenTid()
{
	unsigned int unId = 0; DailyTime curTime;
	TrssNodeMgr * pclsMy = TrssNodeMgr::m_fnGetInstance();
	pclsMy->m_clsTrLock.m_fnLock();
	if(pclsMy->m_clsTime.m_fnIsCompareTime(DailyTime::E_TIME_CMP_DAY,curTime)== false)
	{
		pclsMy->m_clsTime.m_fnUpdateTime();
		pclsMy->m_unTrGenId = 0;
		unId = 0;
	}
	else
	{
		unId = pclsMy->m_unTrGenId;
		++(pclsMy->m_unTrGenId);
	}
	pclsMy->m_clsTrLock.m_fnUnlock();
	return unId;
}
unsigned int TrssNodeMgr::m_fnDecisionNode(Net5Tuple & _rstResult, KString & _rclsTrsgName, KString & _rclsTrssName, KString & _rclsSessionID)
{
	ENodeMgrApiError_t eT; unsigned int unRet = 0;
	m_clsTrLock.m_fnWriteLock();
	unRet = m_clsNode.m_fnDecisionRteID(1000, _rstResult, &eT);

	NodeConnection * pclsCon = m_clsNode.m_fnFindConnection(_rstResult);
	if(pclsCon && pclsCon->m_pclsUserObject)
	{
		TrssNode * pclsNode = NULL;
		pclsNode = (TrssNode*)pclsCon->m_pclsUserObject;
		_rclsTrsgName = pclsNode->m_clsTrsgName;
		_rclsTrssName = pclsNode->m_clsTrssName;
		_rclsSessionID = pclsNode->m_clsSessionID;
	}
	m_clsTrLock.m_fnUnlock();
	return unRet;
}
bool TrssNodeMgr::m_fnIsAllDeactive()
{
   m_clsTrLock.m_fnReadLock();
   ListItr_t stItor;
   NodeSeq * pclsSeq = m_clsNode.m_fnBeginSeq(stItor);
   if (pclsSeq == NULL)
   {
      IFLOG(E_LOG_DEBUG, "%s %s %d SEQ is Empty.", __FILE__, __func__,
            __LINE__);
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
void TrssNodeMgr::m_fnPutEv(TrssConEv * _pclsEv)
{
	TrssNodeMgr * pclsMy = TrssNodeMgr::m_fnGetInstance();
	if(pclsMy->m_pclsQ->put(_pclsEv) == false)
	{
		IFLOG(E_LOG_ERR,"TrssNodeMgr Queue Full");
		delete _pclsEv;
	}
}
void TrssNodeMgr::m_fnCbkProc(TrssConEv * _pclsEv, void * _pvOwner)
{
	TrssNodeMgr * pclsMy = TrssNodeMgr::m_fnGetInstance();

	if(_pclsEv->m_eT == E_TRSS_IF_CONNECTED)
	{
		pclsMy->m_fnProcConnected(_pclsEv);
	}
	else if(_pclsEv->m_eT == E_TRSS_IF_DISCONNECTED)
	{
		pclsMy->m_fnProcDisConnected(_pclsEv);
	}
	else if(_pclsEv->m_eT == E_TRSS_IF_ESTABLISH_SESSION_REQ)
	{
		pclsMy->m_fnProcRecvEstablishSesReq(_pclsEv);
	}
	else if(_pclsEv->m_eT == E_TRSS_IF_LINK_TEST_REQ)
	{
		pclsMy->m_fnProcRecvLinkTestReq(_pclsEv);
	}
	else if(_pclsEv->m_eT == E_TRSS_IF_ESTABLISH_SESSION_TMR)
	{
		pclsMy->m_fnProcRecvEstablishSesTimeOut(_pclsEv);
	}
	else
	{
		IFLOG(E_LOG_ERR,"[TrssNodeMgr]Unknown Event Type(%d)", _pclsEv->m_eT);
	}
	delete _pclsEv;
}
void TrssNodeMgr::m_fnCbkNodeChanged(ColumFile * _pclsObj)
{
	//LOC 는 수정 할 수 없음. RMT와 RTE, SEQ만 수정 가능함.
	IFLOG(E_LOG_INFO, "Change Trss Node Config");
	TrssNodeMgr * pclsMy = (TrssNodeMgr*)_pclsObj->m_fnGetUser();
	ENodeMgrApiError_t eErr;
	pclsMy->m_clsTrLock.m_fnWriteLock();
	NodeMgr clsOldNodeMgr = pclsMy->m_clsNode;//copy
	pclsMy->m_clsNode.m_fnReLoadConfig(&eErr);

	ListItr_t stOldItor; TrssTransport * pclsTrans = TrssTransport::m_fnGetInstance();
	NodeRte * pclsOldRte = clsOldNodeMgr.m_fnBeginRte(stOldItor);
	while(pclsOldRte)//Old Rte 설정을 순회하여 New Rte 설정에 존재하던 설정인지 확인한다.
	{
		KString clsAddrString2;
		IFLOG(E_LOG_INFO, "TRSS CbkNodeChanged OldRte : [%s]:ConExists[%d]", pclsOldRte->m_stAddr.getStr((KSTR )clsAddrString2, 128, false), pclsMy->m_fnConExists(pclsOldRte->m_stAddr));
		NodeRte* pclsNewRte = pclsMy->m_clsNode.m_fnFindRteFrom5Tuple(pclsOldRte->m_stAddr);
		if(pclsNewRte)//존재할때
		{
			//Config 재로딩 되었음으로 할 작업 없음
		}
		else//존재하지 않을때
		{
			KString clsAddrString;
			IFLOG(E_LOG_ERR, "TRSS CbkNodeChanged Rte Deleted: [%s]:ConExists[%d]", pclsOldRte->m_stAddr.getStr((KSTR )clsAddrString, 128, false), pclsMy->m_fnConExists(pclsOldRte->m_stAddr));
			//삭제된 Rte가 있음. 커넥션 확인 후 Transfer Del.
			if (pclsMy->m_fnConExists(pclsOldRte->m_stAddr))//존재하는 커넥션 확인 후 Del
				pclsTrans->m_fnDelConnection(pclsOldRte->m_stAddr);
		}
		pclsOldRte = clsOldNodeMgr.m_fnNextRte(stOldItor);
	}
	pclsMy->m_clsTrLock.m_fnUnlock();
	IFLOG(E_LOG_INFO, "Change Trss Node Config end.");
}
void TrssNodeMgr::m_fnProcConnected(TrssConEv *_pclsEv)
{
	m_clsNode.m_fnChangeConnectivity(_pclsEv->m_stAddr,false);//Establish Res 전송 이후 true 설정.
	TrssNode * pclsNode = m_fnGetTrssNode(_pclsEv);
	if(pclsNode == NULL) return;

	//loc, rmt가 일치하는지 rte를 순회하며 체크
	ListItr_t stItor;TrssTransport * pclsTrans = TrssTransport::m_fnGetInstance();
	m_clsTrLock.m_fnReadLock();
	NodeRte * pclsRte = m_clsNode.m_fnBeginRte(stItor);
	if(pclsRte == NULL)
	{
		//rte 설정이 없음
		eSipUtil::KString clsAddrString;
		IFLOG(E_LOG_ERR, "TRSS Connection Not Allow(empty rte)[%s]", _pclsEv->m_stAddr.getStr((eSipUtil::KSTR)clsAddrString,128,false));
		pclsTrans->m_fnDelConnection(_pclsEv->m_stAddr);
		m_clsTrLock.m_fnUnlock();
		return;
	}
	bool bFind = false;
	while(pclsRte)
	{
		//MBlock 되어있어도, 접속은 허용함. 단, 분배하지 않는 방향으로 구현 함.
		if(_pclsEv->m_stAddr == pclsRte->m_stAddr)//RTE에 존재 함.
		{
			eSipUtil::KString clsAddrString;
			IFLOG(E_LOG_INFO, "TRSS Connection Allow[%s]", _pclsEv->m_stAddr.getStr((eSipUtil::KSTR)clsAddrString,128,false));
			bFind = true;
			break;
		}
		pclsRte = m_clsNode.m_fnNextRte(stItor);
	}
	if(bFind == false)//RTE 설정되지 않은 RMT로 접속 시도 할 경우 Connection Del.
	{
		eSipUtil::KString clsAddrString;
		IFLOG(E_LOG_ERR, "TRSS Connection Not Allow[%s]", _pclsEv->m_stAddr.getStr((eSipUtil::KSTR)clsAddrString,128,false));
		pclsTrans->m_fnDelConnection(_pclsEv->m_stAddr);
		m_clsTrLock.m_fnUnlock();
	}
	else
	{
		m_clsTrLock.m_fnUnlock();
		pclsNode->m_fnConnected();
	}

}
void TrssNodeMgr::m_fnProcDisConnected(TrssConEv *_pclsEv)
{
	m_clsNode.m_fnChangeConnectivity(_pclsEv->m_stAddr,false);
	TrssNode * pclsNode = m_fnGetTrssNode(_pclsEv);
	if(pclsNode == NULL) return;

	pclsNode->m_fnDisConnected();
	m_fnDelTrssNode(_pclsEv);
}
/*********************************************/
void TrssNodeMgr::m_fnProcRecvEstablishSesReq(TrssConEv *_pclsEv)
{
	TrssNode * pclsNode = m_fnGetTrssNode(_pclsEv);
	if (pclsNode == NULL) return;
	pclsNode->m_fnRecvEstablishSesReq(_pclsEv->m_clsXml);
}
void TrssNodeMgr::m_fnProcRecvLinkTestReq(TrssConEv *_pclsEv)
{
	TrssNode * pclsNode = m_fnGetTrssNode(_pclsEv);
	if (pclsNode == NULL) return;
	pclsNode->m_fnRecvLinkTestReq(_pclsEv->m_clsXml);
}
void TrssNodeMgr::m_fnProcRecvEstablishSesTimeOut(TrssConEv *_pclsEv)
{
	TrssNode * pclsNode = m_fnGetTrssNode(_pclsEv);
	if (pclsNode == NULL) return;
	pclsNode->m_fnRecvEstablishSesTimeOut();
}
const char * TrssNodeMgr::m_fnGetNodeRteName(const eSipUtil::Net5Tuple & _rstAddr)
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
void TrssNodeMgr::fnCbkTimerOut(TrssConEv * _pclsData)
{
	m_fnPutEv(_pclsData);
}
bool TrssNodeMgr::m_fnConExists(const Net5Tuple & _rstAddr)
{
	NodeConnection * pclsCon = m_clsNode.m_fnFindConnection(_rstAddr);
	if(pclsCon)
	{
		if(pclsCon->m_pclsUserObject)
		{
			TrssNode * pclsNode = (TrssNode*)pclsCon->m_pclsUserObject;
			if(pclsNode->m_bConnect == true)
			{
				//m_clsTrLock.m_fnUnlock();
				return true;
			}
		}
	}
	return false;
}
TrssNode * TrssNodeMgr::m_fnGetTrssNode(TrssConEv *_pclsEv)
{
	m_clsTrLock.m_fnReadLock();
	NodeConnection * pclsCon = m_clsNode.m_fnFindConnection(_pclsEv->m_stAddr);
	if(pclsCon == NULL)
	{
		pclsCon = m_clsNode.m_fnChangeConnectivity(_pclsEv->m_stAddr,false);
		if(pclsCon == NULL)
		{
			m_clsTrLock.m_fnUnlock();
			IFLOG(E_LOG_ERR,"Get Trss Node Error");
			return NULL;
		}
	}
	TrssNode * pclsNode = NULL;
	if(pclsCon->m_pclsUserObject == NULL)
	{
		pclsNode = new TrssNode;
		pclsNode->m_pclsOwner = pclsCon;
		pclsCon->m_fnSetUserObject(pclsNode);
	}
	else
	{
		pclsNode = (TrssNode*)pclsCon->m_pclsUserObject;
	}
	m_clsTrLock.m_fnUnlock();
	return pclsNode;
}
bool TrssNodeMgr::m_fnDelTrssNode(TrssConEv *_pclsEv)
{
	m_clsTrLock.m_fnLock();
	bool bRet = m_clsNode.m_fnDelCon(_pclsEv->m_stAddr);
	m_clsTrLock.m_fnUnlock();
	return bRet;
}
