#include <sys/ipc.h>
#include <sys/msg.h>
#include "nwm_mgr.h"
#include "sharedmemory.h"
#include "aloxConf.h"


#define PI_NWMAPI_SHM_PATH  "../ipc/shm/nwmapi_shm"
#define PI_NWMAPI_CONFIG     "../config/PINWM/PINWM"

//constructor
CNwmMgr::CNwmMgr( )
   : m_pIfCfg(NULL),
     m_pArp(NULL),
     m_pSHM(NULL),
     m_bServer(false)
{
   m_szConf[0]=0x00;
}
//destructor
CNwmMgr::~CNwmMgr()
{
}
bool CNwmMgr::Initialize(int nAPPID, bool bServer )
{
   m_bServer=bServer;
   if( nAPPID < 0 || nAPPID > 2 ){
      LOGGER(PL_ERR,"[NWMAPI] invalid app id %d",nAPPID);
      return false;
   }
   //config path
   sprintf(m_szConf,"%s_%d.cfg",PI_NWMAPI_CONFIG,nAPPID); 

   //shared memory
   struct stat file_stat;
   char szPATH[256];
   sprintf(szPATH,"%s_%d",PI_NWMAPI_SHM_PATH,nAPPID);
   int nRet=stat(szPATH, &file_stat);
   if(nRet < 0 ){
      //create file
      FILE* fp=NULL;
      fp=fopen(szPATH,"w+");
      ALOX_CLOSE_FP(fp);
   }
   //create shm
   m_pSHM=(PINWM_SHM*)CreateShm(szPATH,sizeof(PINWM_SHM));
   if( !m_pSHM ){
      LOGGER(PL_ERR,"[NWMAPI] fail in create shm. path(%s)",szPATH);
      return false;
   }

   //ifconfig 
   m_pIfCfg=new CIfCfg;
   if( (m_pIfCfg->Init()) == false ){
      LOGGER(PL_ERR,"[NWMAPI],fail in init ifconfig");
      return false;
   }
   //arp 
   m_pArp=new CArp;
   if( (m_pArp->Init()) == false ){
      LOGGER(PL_ERR,"[NWMAPI],fail in init arp");
      return false;
   }

   if ( !m_bServer ){
      AppTouch();
      return true;
   }

   //load config
   LoadConfigFile();
   return CUserThread::Start();
}

void CNwmMgr::AppTouch()
{
   gettimeofday(&(m_pSHM->touchTime),NULL);
}

bool CNwmMgr::IsRunNetworkIF(const char* pIfName,const char* pIPAddr)
{
   AppTouch();
   bool bRet=false; 
   if( !pIfName || strlen(pIfName) <= 0 ){
      LOGGER(PL_ERR,"[NWMAPI] fail in IsRunNetworkIF. param error. interface name");
      return false;
   }
   bool bIPv4=true;
   if( strchr(pIPAddr,':') != NULL ) bIPv4=false;

   char szTmpBuf[128];
   char* pTmp=NULL;
   strncpy(szTmpBuf,pIfName,sizeof(szTmpBuf)-1);
   if( !bIPv4 ){
      if( (pTmp=strchr(szTmpBuf,':')) != NULL ){
         *pTmp=0x00;
      }
      if( !pIPAddr || strlen(pIPAddr) <= 0 ){
         LOGGER(PL_ERR,"[NWMAPI] fail in IsRunNetworkIF. param error. ip address");
         return false;
      }
   }

   bRet=m_pIfCfg->IsRunNetworkIF(szTmpBuf,pIPAddr,bIPv4);
   AddUPIf(szTmpBuf,pIPAddr,bRet);
   return bRet;
}

void CNwmMgr::DownIPAddr(const char* pIfName, const char* pIPAddr)
{
   AppTouch();
   if( !pIfName || strlen(pIfName) <= 0 ){
      LOGGER(PL_ERR,"[NWMAPI] fail in DownIPAddr. param error. interface name");
      return ;
   }
   bool bIPv4=true;
   if (  pIPAddr && strlen(pIPAddr) > 0 && 
         strchr(pIPAddr,':') != NULL ) bIPv4=false;
   char szTmpBuf[128];
   char* pTmp=NULL;
   strncpy(szTmpBuf,pIfName,sizeof(szTmpBuf)-1);
   if( !bIPv4 ){
      if( (pTmp=strchr(szTmpBuf,':')) != NULL ){
         *pTmp=0x00;
      }
      if( !pIPAddr || strlen(pIPAddr) <= 0 ){
         LOGGER(PL_ERR,"[NWMAPI] fail in DownIPAddr. param error. ip address");
         return;
      }
   }

   m_pIfCfg->DownIPAddr(pIfName,pIPAddr,bIPv4);
   AddUPIf(szTmpBuf,pIPAddr,false);
}

bool CNwmMgr::UpIPAddr(const char* pIfName,const char* pIPAddr, const char* pIPv4NetMask)
{
   AppTouch();
   bool bRet=false; 
   bool bIPv4=true;
   if( strchr(pIPAddr,':') != NULL ) bIPv4=false;
   if ( bIPv4 ){
      if( !pIfName || !pIPAddr || !pIPv4NetMask ||
         strlen(pIfName) <= 0 || strlen(pIPAddr) <= 0 ||
         strlen(pIPv4NetMask) <= 0  ){
         LOGGER(PL_ERR,"[NWMAPI] fail in UpIPAddr. param error. if name or ip addr or netmask");
         return false;
      }
   }else{
     if( !pIfName || !pIPAddr || 
          strlen(pIfName) <= 0 || strlen(pIPAddr) <= 0 ){
         LOGGER(PL_ERR,"[NWMAPI] fail in UpIPAddr. param error. if name or ip addr");
         return false;
     }
   }
   char szTmpBuf[128];
   char* pTmp=NULL;
   strncpy(szTmpBuf,pIfName,sizeof(szTmpBuf)-1);
   if( !bIPv4 ){
      if( (pTmp=strchr(szTmpBuf,':')) != NULL ){
         *pTmp=0x00;
      }
      if( !pIPAddr || strlen(pIPAddr) <= 0 ){
         LOGGER(PL_ERR,"[NWMAPI] fail in UpIPAddr. param error. ip address");
         return false;
      }
   }
   bRet=m_pIfCfg->UpIPAddr(szTmpBuf,pIPAddr,pIPv4NetMask,bIPv4);
   AddUPIf(pIfName,pIPAddr,bRet,true);
   if ( !bRet ) return false;

   char szMacAddr[128]; szMacAddr[0]=0x00;
   char szBroadCast[128]; szBroadCast[0]=0x00; 

   if( !bIPv4 ){
      bRet=m_pArp->SendNDP(pIPAddr,szTmpBuf);
   }else{ 
      m_pIfCfg->GetHwAddr2(szTmpBuf,szMacAddr,sizeof(szMacAddr)-1);
      m_pIfCfg->GetBroadCast(szTmpBuf,szBroadCast,sizeof(szBroadCast)-1);
      bRet=m_pArp->SendARP(pIPAddr,szMacAddr,szBroadCast,szTmpBuf);
   }
   return true;
}

void CNwmMgr::AddUPIf(const char* pIfName,const char* pIPAddr, bool bUP, bool bFirstUP)
{
   PINWM_INTERFACE* pIf=NULL;
   PINWM_INTERFACE* pEmptyIf=NULL;
   bool bFound=false;

   bool bIPv4=true;
   if( pIPAddr && strlen(pIPAddr) > 0 && strchr(pIPAddr,':') != NULL ) bIPv4=false;
   for( int i= 0 ; i < PINWM_SHM::e_IFMAX_NUM ; i++){
      pIf=&(m_pSHM->nwIf[i]);
      if ( bIPv4 ){
         if ( strlen(pIf->szIFName) <=0 ){
            if( pEmptyIf == NULL ){ 
               pEmptyIf=pIf;
            }
            continue;
         }
      }else{
         if ( strlen(pIf->szIFName) <=0 ||
              strlen(pIf->szIPAddr) <=0 ){
            if( pEmptyIf == NULL ){ 
               pEmptyIf=pIf;
            }
            continue;
         }
      }
      if ( bIPv4 ){
         if( strcmp(pIf->szIFName,pIfName) == 0 ) {
            bFound=true;
            break;
         }

      }else{
         if( strcmp(pIf->szIFName,pIfName) == 0 &&
             strcmp(pIf->szIPAddr,pIPAddr) == 0 ){
            bFound=true;
            break;
         }
      }
   }
   //found 
   if( bFound){
      //ethernet device is down
      if ( !bUP && pIf) {
         pIf->szIFName[0]=0x00;
         pIf->szIPAddr[0]=0x00;
      }
      if ( bUP && pIf){
         if ( pIPAddr && strcmp(pIf->szIPAddr,pIPAddr) != 0 ){
            strncpy(pIf->szIPAddr,pIPAddr,sizeof(pIf->szIPAddr)-1);
         }
         if( bFirstUP ){
            gettimeofday(&(pIf->touchTime),NULL);
         }
      }
      return;
   }
   // not found . up add
   if( bUP ){
      if(!pEmptyIf ){
         LOGGER(PL_ERR,"[NWMAPI] interface is full");
      }else{
         strncpy(pEmptyIf->szIFName,pIfName,sizeof(pIf->szIFName)-1);
         if( bFirstUP ){
            gettimeofday(&(pEmptyIf->touchTime),NULL);
         }
         if ( pIPAddr )
            strncpy(pEmptyIf->szIPAddr,pIPAddr,sizeof(pIf->szIPAddr)-1);
      }
   }
}
void CNwmMgr::AutoDown(struct timeval* pTime, unsigned short usDownSec)
{
   ALTIME_DIFF diff;
   memcpy(&(diff.timev_s),pTime,sizeof(struct timeval));

   unsigned int uiDTime=CALUtil::GetTimeDiff(&diff,false);

   if ( uiDTime <= (unsigned int)usDownSec ) return;

   PINWM_INTERFACE* pIf=NULL;
   for( int i= 0 ; i < PINWM_SHM::e_IFMAX_NUM ; i++){
      pIf=&(m_pSHM->nwIf[i]);
      if ( strlen(pIf->szIFName) <=0 ||
           strlen(pIf->szIPAddr) <=0 ){
         continue;
      }
      LOGGER(PL_ERR,"[NWMAPI] process detach. will down %s, %s",pIf->szIFName,pIf->szIPAddr);
      DownIPAddr(pIf->szIFName,pIf->szIPAddr);
   }
}

void CNwmMgr::AutoARP()
{

   ALTIME_DIFF diff;

   unsigned int uiTime=0;


   PINWM_INTERFACE* pIf=NULL;
   for( int i= 0 ; i < PINWM_SHM::e_IFMAX_NUM ; i++){
      pIf=&(m_pSHM->nwIf[i]);
      if ( strlen(pIf->szIFName) <=0 ||
           strlen(pIf->szIPAddr) <=0 ){
         continue;
      }
      memcpy(&(diff.timev_s),&(pIf->touchTime),sizeof(struct timeval));
      uiTime=CALUtil::GetTimeDiff(&diff,false); 
      if ( uiTime > (unsigned int)m_pSHM->nwmConf.usARPCheckTime ){
         continue;
      }
      
      LOGGER(PL_INF,"[NWMAPI] will send arp %s, %s",pIf->szIFName,pIf->szIPAddr);
      SendARPAndNDP(pIf->szIFName,pIf->szIPAddr);
      CALUtil::MilliSleep((int)m_pSHM->nwmConf.usARPCheckSleep);
   }

}

void CNwmMgr::SendARPAndNDP(char* pIFName, char* pIPAddr)
{
   char szMacAddr[128]; szMacAddr[0]=0x00;
   char szBroadCast[128]; szBroadCast[0]=0x00; 
   bool bIPv4=true;

   if ( strchr(pIPAddr,':') != NULL ){
      bIPv4=false;
   }

   char szTmpBuf[128];
   char* pTmp=NULL;
   strncpy(szTmpBuf,pIFName,sizeof(szTmpBuf)-1);
   if( !bIPv4 ){
      if( (pTmp=strchr(szTmpBuf,':')) != NULL ){
         *pTmp=0x00;
      }
   }

   if( !bIPv4 ){
      m_pArp->SendNDP(pIPAddr,szTmpBuf);
   }else{ 
      m_pIfCfg->GetHwAddr2(pIPAddr,szMacAddr,sizeof(szMacAddr)-1);
      m_pIfCfg->GetBroadCast(szTmpBuf,szBroadCast,sizeof(szBroadCast)-1);
      m_pArp->SendARP(pIPAddr,szMacAddr,szBroadCast,szTmpBuf);
   }
}

void CNwmMgr::PrintSHM()
{
   PINWM_CONF* pCfg=NULL;
   PINWM_INTERFACE* pIf=NULL;
   pCfg=&(m_pSHM->nwmConf);

   char szTime[256]; szTime[0]=0x00;
   char szTime2[256]; szTime2[0]=0x00;
   char szTime3[256]; szTime3[0]=0x00;

   struct timeval tp;
   memset(&tp,0x00,sizeof(tp));
   tp.tv_sec=pCfg->ulLastModifyTime;

   
   CALUtil::GetStrDateTime(&tp,szTime);

   CALUtil::GetStrDateTime(&(m_pSHM->touchTime),szTime2);



   LOGGER(PL_CRI,"[NWMAPI] log=%d, ct=%s, dtime=%dmsec, dflag=%d, at=%s, arp=%dms, sleep=%dms",
         pCfg->uiLogLevel,szTime,pCfg->usAliveCheckTime,pCfg->usAliveCheckDown,szTime2,
         pCfg->usARPCheckTime,pCfg->usARPCheckSleep);

   for( int i= 0 ; i < PINWM_SHM::e_IFMAX_NUM ; i++){
      pIf=&(m_pSHM->nwIf[i]);
      szTime3[0]=0x00;
      CALUtil::GetStrDateTime(&(pIf->touchTime),szTime3); 
      LOGGER(PL_CRI,"[NWMAPI] %02d => Name=%s, IP=%s, TIME=%s",i,pIf->szIFName,pIf->szIPAddr,szTime3);
   }
}

void* CNwmMgr::ThreadProc()
{
   PINWM_CONF* pCfg=NULL;
   struct timeval apptime;
   int nCnt=0;
   while(!DoExit()){
      if(m_bStopped) break;
      SafeSleep(20);
      //check touch file
      if( CheckChangedFile() ){
         LoadConfigFile();
      }

      if ( !m_pSHM ) continue;

      //loglevel
      if ( m_pSHM->nwmConf.uiLogLevel >= 3 ){
         nCnt++;
         nCnt = (nCnt)%100;
         if( nCnt == 0 )
            PrintSHM();         
      }

      AutoARP();

      //down 
      pCfg=&(m_pSHM->nwmConf);
      if ( pCfg->usAliveCheckDown == 0 ) continue;
      if ( pCfg->usAliveCheckTime <= 100 ) continue;
      memcpy(&apptime,&(m_pSHM->touchTime),sizeof(struct timeval));
      AutoDown(&apptime,pCfg->usAliveCheckTime); 

      if(m_bStopped) break;
   }
   m_bEscaped=true;
   return NULL;
}


bool CNwmMgr::CheckChangedFile()
{
   //file length check
   if ( strlen(m_szConf) <= 0 ) return false;

   //get stat
   struct stat file_stat;
   int nret=stat(m_szConf, &file_stat);
   if(nret < 0 ) return false;

   //check file size
   if(file_stat.st_size <= 0 ) return false;

   //get last modify time
   time_t* mtime=&(file_stat.st_mtime);

   //the first modified?
   if (  m_pSHM->nwmConf.ulLastModifyTime == (time_t)0 ){
      //get current time
      struct timeval curtime;
      gettimeofday(&curtime,NULL);
      m_pSHM->nwmConf.ulLastModifyTime=(time_t)(curtime.tv_sec);
   }else{
      if( m_pSHM->nwmConf.ulLastModifyTime >= *mtime){
         return false;
      }
      m_pSHM->nwmConf.ulLastModifyTime=*mtime;
   }
   return true;

}

bool CNwmMgr::LoadConfigFile()
{
   if( !m_pSHM ) return true;

   bool bError=true;
   char szError[256];szError[0]=0x00;
   CALConf conf;
   PINWM_CONF tmpConf;
   memset(&tmpConf,0x00,sizeof(PINWM_CONF));
   tmpConf.ulLastModifyTime=m_pSHM->nwmConf.ulLastModifyTime;
   do{
      LOGGER(PL_INF,"[NWMAPI] load config %s",m_szConf);
      if( !(conf.Init(m_szConf)) ){
         sprintf(szError,"fail load config(%s)",m_szConf);
         break;
      }
      tmpConf.usAliveCheckTime = conf.GetValueInt((char*)"MAIN",(char*)"AliveCheckTime");
      if ( tmpConf.usAliveCheckTime <= 0 ){
         tmpConf.usAliveCheckTime=PINWM_SHM::e_DEFAULT_CHECKTIME;
      }
      LOGGER(PL_INF,"[NWMAPI] AliveCheckTime = %dms",tmpConf.usAliveCheckTime);

      tmpConf.uiLogLevel = conf.GetValueInt((char*)"MAIN",(char*)"LogLevel");
      if( tmpConf.uiLogLevel <= 0 ) tmpConf.uiLogLevel=0;
      LOGGER(PL_INF,"[NWMAPI] LogLevel = %d ",tmpConf.uiLogLevel);

      tmpConf.usAliveCheckDown = conf.GetValueInt((char*)"MAIN",(char*)"AliveCheckDown");
      if( tmpConf.usAliveCheckDown <= 0 ) tmpConf.usAliveCheckDown=0;
      LOGGER(PL_INF,"[NWMAPI] AliveCheckDown = %d ",tmpConf.usAliveCheckDown);

      tmpConf.usARPCheckTime = conf.GetValueInt((char*)"MAIN",(char*)"ARPCheckTime");
      if( tmpConf.usARPCheckTime <= 0 ) tmpConf.usARPCheckTime=0;
      LOGGER(PL_INF,"[NWMAPI] ARPCheckTime = %d ",tmpConf.usARPCheckTime);

      tmpConf.usARPCheckSleep = conf.GetValueInt((char*)"MAIN",(char*)"ARPCheckSleep");
      if( tmpConf.usARPCheckSleep <= 0 ) tmpConf.usARPCheckSleep=0;
      LOGGER(PL_INF,"[NWMAPI] ARPCheckSleep = %d ",tmpConf.usARPCheckSleep);

      
      bError = false;
   }while(0);
   if( bError ) {
      LOGGER(PL_ERR,"[NWMAPI] conf file error. %s",szError);
      return false;
   }
   memcpy(&(m_pSHM->nwmConf),&tmpConf,sizeof(PINWM_CONF));

   CLogger::LOGLEVEL level=CLogger::Info;
   if ( tmpConf.uiLogLevel >= 4 ) tmpConf.uiLogLevel=4;
   level=(CLogger::LOGLEVEL)tmpConf.uiLogLevel;

   CLogger::GetInstance()->SetLogLevel(level);

   return true;
}
