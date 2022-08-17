#ifndef _IBCF_LM_PROC_H_
#define _IBCF_LM_PROC_H_

#include <netinet/in.h>

#include "lm_msg.h"
#include <queue>

#include "eRule/CRuleUtiles.h"
#include "eRte/eSipRteApi.hxx"
#include "IBCDefine.h"
// log
#define LOG_W   LOGGER

using namespace NS_LM;
typedef struct LMQ_t
{
	LMQ_t(){m_pst = NULL;}
	void m_fnTerm(){if(m_pst){delete [] (char*)m_pst;}}
	LM_MSG_t* m_pst;
}LMQ_t;
typedef std::queue<LMQ_t> qLMQueue;

void RteApiPrintLog_t(int _nLevel,char*    _pMsg);

class CLMProc : public resip::ThreadIf
{
public:
	CLMProc();
	~CLMProc();
	static CLMProc *GetInstance();
	static void fnFinalize();
	bool Start(void *pIUdp, void *pServiceApi);  
	void Stop();
	void Add(LMQ_t stRecvData);
	bool Get(LMQ_t *pData);

protected:
	virtual void thread();
	void ProcCSReq(LM_MSG_t *pMsg);
	
	// 의존성 체크
	// Rsq or Srsq를 확인함.
	bool CheckSeqOfRteOp(int nId);
   //modify yun 20151102
	bool ExistIdFromRSQ_SRSQ(int nType,int nId);

protected:
	char m_szDesc[32];
	RULock m_Lock;
	static CLMProc* g_pclsProc;
	qLMQueue m_qLM;	
	void *m_pIUdp;
	void *m_pServiceApi;
	ESipRteApiCallBack_t *m_pCb;
};

#endif

//////////////////////////////////////////////////////////////////////
// EOF
//////////////////////////////////////////////////////////////////////
