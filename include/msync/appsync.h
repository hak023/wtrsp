/**
 * @file  eSipRte.h
 * the sip route message header
 *
 * @attention Copyright (c) 2014 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2015.02.10   hc_yun   1.0     first written
 * @par 2015.10.16   hc_yun   1.2     add SetUserSyncMode, callback
 * a) SetUserSyncMode : enable user sync mode. app can send SYNC_END
 * @par 2017.03.16   hc_yun   1.3     add open (shm key: file path -> key ) @vibc
 */

#ifndef _APPSYNC_H_
#define _APPSYNC_H_

#include <sys/time.h>
#include <stdio.h>

struct SyncSecHdr
{
   unsigned int   usr_sts;
   struct timeval wri_time;
   unsigned int   data_len;
};

typedef void (*PfuncMSyncApiPrintLog_t)(
   int      _nLevel,
   char*    _pMsg);

typedef void (*PfuncMSyncApiChageHAStatus_t)(
   int      _nCurStatus,         //0: NONE, 1:ACT, 2: STANDBY
   int      _nBeforeStauts);

typedef void (*PfuncMSyncApiChageHAStatus_UserSyncing_t)(
   int      _nCurStatus,         //0: NONE, 1:ACT, 2: STANDBY
   int      _nBeforeStauts);

typedef void (*PfuncMSyncApiFetchOne_t)(
   int nSectionNum,    //OUT: section number
   SyncSecHdr *phdr,   //OUT: secttion header
   void * pData,       //OUT: buffer 
   int nDataSize);     //OUT: data size


class CAppSync
{
public:
	CAppSync();
	~CAppSync();
   //must invoke this func. before Open
   void SetUpCallBack(PfuncMSyncApiPrintLog_t      pLog,
                      PfuncMSyncApiChageHAStatus_t pHAStatus,
                      PfuncMSyncApiFetchOne_t      pFetch,
                      PfuncMSyncApiChageHAStatus_UserSyncing_t pUserSync=NULL);
   //szName        : IN, sync point name
   //nSecCnt       : IN, the count of the sync section 
   //nSecSize      : IN, the size of the section 
   //bHaServerMode : IN, the ha sever mode or client mode. not using param.
   //nHaPort       : IN, the active tcp port, if 0, not use.
   //bUseHaBackup  : IN, use ha backup , if false, not use.
   //bNoFetch      : IN, standby side no fetch flag. if enable, must call Read instead of FetchOne
   int Open(char* szName, int nSecCnt, int nSecSize,
            bool bHaServerMode, int nHaPort, bool bUseHaBackup, bool bNoFetch);
   //the param bHaServerMode is the flag tcp server or not.
   //the api decides the flag

   //@vibc add 20170316 
   int Open(unsigned int uiShmKey, int nSecCnt, int nSecSize,
            bool bHaServerMode, int nHaPort, bool bUseHaBackup, bool bNoFetch);


   //user sync mode
   void SetUserSyncMode();
   void UserSendSync(); //act->standby, send sync

   //the app don't use this func. tunning network bandwitdh
   void ModifyPerform(int nSendIntervalMS, int nSendCntInNoSync, int nSendIntervalMSInNoSync);
   
   //direct attach data
   int AttachData(int nSectionNum,void*& pvData);
   int WriteTouch(int nSectionNum,int nDataSize,int nUsrStatus=0);
  #if 1 // cwkim add section lock api
  void LockSection(int nSectionNum);
  void UnlockSection(int nSectionNum);
  #endif
   //attach header
   SyncSecHdr* GetHDR(int nSectionNum);

   //if active, send data to standby side. 
   int Write(int nSectionNum,void* pBuf,int nDataSize,unsigned int usr_sts=0);
	int WriteWithHDR(int nSectionNum, SyncSecHdr * phdr,void* pBuf);

   //if standby, read data. or nothing
	int Read(int nSectionNum, SyncSecHdr * phdr,void * pBuf,int bufsize);
	void ClearDataList();

   //get ha status. return  0: NONE, 1:ACT, 2: STANDBY
   int GetCurrenHAStatus(); 

   //get current sync count
   int GetCurrentSyncCount();
	int ClearData(int nSectionNum);
	SyncSecHdr* GetHdr(int nSectionNum);

private:
   void * _ctx;
};

#endif //_APPSYNC_H_
