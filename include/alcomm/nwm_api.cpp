#include "nwm_api.h"

static CNwmMgr* g_pMgr=NULL;

bool  PINWM_API_Initialize(int nAPPID, bool bServer)
{
   if( g_pMgr ) return true;
   g_pMgr=new CNwmMgr;

   return g_pMgr->Initialize(nAPPID,bServer);
}

void  PINWM_API_AppTouch()
{
   if( !g_pMgr ) return ;
   g_pMgr->AppTouch();

}


bool PINWM_API_IsNETIFAlive(const char* pIfName,const char* pIPAddr)
{
   if( !g_pMgr ) return false;

   return g_pMgr->IsRunNetworkIF(pIfName,pIPAddr);
}

bool  PINWM_API_UpIPAddr(const char* pIfName,const char* pIPAddr, const char* pIPv4NetMask)
{

   if( !g_pMgr ) return false;

   return g_pMgr->UpIPAddr(pIfName,pIPAddr,pIPv4NetMask);
}

void PINWM_API_DownIPAddr(const char* pIfName, const char* pIPAddr)
{

   if( !g_pMgr ) return ;

   return g_pMgr->DownIPAddr(pIfName,pIPAddr);
}
