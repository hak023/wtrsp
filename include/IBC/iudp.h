#ifndef _IBCF_LM_IUDP_H_
#define _IBCF_LM_IUDP_H_

#include <netinet/in.h>
#include "lm_msg.h"
#include "lmproc.h"
#include "eRule/CRUInetUdp.h"
#include "IBCDefine.h"

class CIUdpThread : public CRUInetUdp
{
	public:
		CIUdpThread();
		~CIUdpThread();
		static CIUdpThread *GetInstance();
		static void fnFinalize();
		int   Init(void *pProc,const char *pszMyIp, int nMyPort, const char *pszLMIp, int nLMPort,
																	unsigned int _unSocBuff); 
		int   Start(void *pProc);
		void  Stop();
		int   SendMsg(NS_LM::LM_MSG_t *pMsg);
	private:
		bool AddQueue(NS_LM::LM_MSG_t *pMsg);
		void fnRecv(char * _pszRcvData,int _nRcvLen,char * _pszRecvIp, int _nRecvPort);
		void MakeLogString(NS_LM::LM_MSG_t *pMsg, char *pLog, int nLen);
		static CIUdpThread* g_pclsIUdp;
		void *m_pProc;
};


#endif

