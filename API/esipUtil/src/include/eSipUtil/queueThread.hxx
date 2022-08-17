
#ifndef ESIPUTIL_QUEUE_THREAD_H
#define ESIPUTIL_QUEUE_THREAD_H

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "thread.hxx"
#include "queue.hxx"
#include "lock.hxx"
#include "log.hxx"
#include "ConfigFile.hxx"
#include "shm.hxx"
#include "HangDefine.h"

namespace eSipUtil
{

template<class T> class QueueThread;

typedef enum
{
	E_QMON_EV_NONE=0,
	E_QMON_EV_ADD,
	E_QMON_EV_ADD_INTERNAL,
	E_QMON_EV_DEL_ALL,
	E_QMON_EV_RESET,
	E_QMON_EV_KICK_ALIVE,
	E_QMON_EV_APP_ENABLE,
	E_QMON_EV_APP_DISABLE,
	E_QMON_EV_SET_INFO,
}EQMon_t;

typedef struct stDiffSpec_t
{
	unsigned int m_unSec;
	unsigned int m_unMsec;
}stDiffSpec_t;

typedef void (*PFuncPutProc_t)(void * _pclsQThread, const char * _pszThreadName, unsigned int _unThreadId);
typedef void (*PFuncHangNotify_t)(const char * _pszThreadName, bool _bClear, bool _bEnableHa, bool _bEnableReset);
typedef void (*PFuncStatNotify_t)(const QueueMonitorInfo_t & _rstQueueMonInfo);
typedef void (*PFuncHangResult_t)(EQMon_t _eEv,unsigned int _unShmIdx, const QueueMonitorInfo_t * _pstQueueMonInfo, void * _pvUser);

/****************************** Queue Monitor Event Class ***********************************/
class QueueMonEvent
{
	public:
		QueueMonEvent();
		~QueueMonEvent();
		
		KString m_clsName;
		unsigned int m_unID;
		void * m_pvUser;
		EQMon_t m_eT;
		PFuncPutProc_t m_pfnPut;
		PFuncHangResult_t m_pfnResult;
		unsigned int m_unMaxQueue;
		unsigned int m_unCurrentQueue;
};

/******************************* Queue Monitor Info Class ************************************/
class QueueMonInfo : public StlObject
{
	public:
		QueueMonInfo();
		~QueueMonInfo();
		void m_fnIncreCnt();
		void m_fnDecreCnt();
		void m_fnDebug(KString & _rclsDebug);
		
		void * m_pvUser;
		KString m_clsName;
		unsigned int m_unID;
		struct timespec m_stEvTime;
		struct timespec m_stRspTime;
		PFuncPutProc_t m_pfnPut;
		bool m_bUtilQThread;
		unsigned int m_unDogCnt;
		unsigned int m_unMaxQueue;
		unsigned int m_unCurrentQueue;
		bool m_bAlarm;

		int m_nShmIdx;
};

/***************************** Queue Monitor Class ****************************************/
class QueueMon
{
	public:
		//==========> Deamon Initalize
		static QueueMon & m_fnGetInstance(const char * _pszProcName=NULL, //---------- Process Name
												int _nShmKey=-1,  //---------------------- Shm Key, -1 is Disable
												PFuncHangNotify_t _pfnHangNotify=NULL, //-- Hang Notify CallBack
												PFuncStatNotify_t _pfnStatNotify=NULL); //--Statistics Notify CallBack
		~QueueMon();
		void m_fnStart();
		/***************************** EVENT Functions ************************************/
		//==========>  (E_QMON_EV_APP_ENABLE/E_QMON_EV_APP_DISABLE)
		void m_fnSetAppEnable(bool _bEnable, bool _bSync=false, PFuncHangResult_t _pfnHangResult=NULL);
		//==========> Add User Qeue Thread (E_QMON_EV_ADD)
		void m_fnRegisterQThread(const char * _pszName, //----------- Thread Name
								unsigned int _unID, //---------------------- Thread ID. must be bigger than 0
								unsigned int _unMaxQ, //------------------- Max Queue Buffer Nums
								void * _pclsQThread, //-------------------- Thread Object Pointer
								PFuncPutProc_t _pfnPutProc, //------------- Thread Put Call Back
								PFuncHangResult_t _pfnHangResult=NULL); //-Event Result Call Back 		
		//==========> Add Utile Internal Queue Thread (E_QMON_EV_ADD_INTERNAL)
		void m_fnRegisterInternalQThread(const char * _pszName, //--- Thread Name
						unsigned int _unID, //----------------------------- Thread ID. must be bigger than 0
						unsigned int _unMaxQ, //-------------------------- Max Queue Buffer Nums
						void * _pclsQThread, //--------------------------- Thread Object Pointer
						PFuncHangResult_t _pfnHangResult=NULL); //---------Event Result Call Back 
		//==========> Delete all User Qeue Thread (E_QMON_EV_DEL_ALL)
		void m_fnDelAllQThread(PFuncHangResult_t _pfnHangResult=NULL); //---Event Result Call Back 
		//==========> Reset info for User Qeue Thread (maintain User Qeue Thread) (E_QMON_EV_RESET)
		void m_fnResetQThread(PFuncHangResult_t _pfnHangResult=NULL); //---Event Result Call Back 
		//==========> Deamon <----Alive_Msg----- [User_Thread]  (E_QMON_EV_KICK_ALIVE)
		void m_fnKickAlive(unsigned int _unID, //------------------------ Thread ID. must be bigger than 0
							unsigned int _unCurrent, //-------------------- Current Queue Num / Optional
							PFuncHangResult_t _pfnHangResult=NULL);  //----Event Result Call Back 
		void m_fnSetThreadInfo(unsigned int _unID, //------------------------ Thread ID. must be bigger than 0
							unsigned int _unCurrent, //-------------------- Current Queue Num / Optional
							PFuncHangResult_t _pfnHangResult=NULL);  //----Event Result Call Back 
		/****************************** GET Functions *************************************/
		const DogInfo_t & m_fnGetDogInfo() const{ return m_stHangCfgData; }
		int m_fnGetKernelTid() const;
		QueueMonitorInfo_t * m_fnGetShm();
		const QueueMonitorInfo_t * m_fnGetShm() const;
	private:
		QueueMon(const char * _pszProcName, 
									int _nShmKey, 
									PFuncHangNotify_t _pfnHangNotify, 
									PFuncStatNotify_t _pfnStatNotify);
		
		//==========> Internal Proc
		static void m_fnCbkProc(QueueMonEvent * _pclsEv, void * _pvUser);

		void m_fnOnOff(bool _bOn);
		void m_fnAddMon(QueueMonEvent & _rclsEv);
		void m_fnDelAllMon(QueueMonEvent & _rclsEv);
		void m_fnResetMon(QueueMonEvent & _rclsEv);
		void m_fnRefreshMon(const QueueMonEvent & _rclsEv);
		void m_fnRefreshThreadInfo(const QueueMonEvent & _rclsEv);
		void m_fnSetAppEnableSync(const QueueMonEvent & _rclsEv);
		void m_fnPeriodic();
		void m_fnStat();
		void m_fnWriteStatus();
		//==========> Config Proc
		static void m_fnChangeHangCfg(CfgFile * _pclsObj);
		void m_fnInitHangConf();
		void m_fnLoadCfg();

		CfgFile m_clsHangCfg;
		DogInfo_t m_stHangCfgData;
		StlMap m_mapQMonInfo;		//key : PID, value : QueueMonInfo
		KString m_clsName;
		bool m_bAppEnable;
		bool m_bAlarm;
		struct timespec m_stProcT;  //MONOTONIC
		PFuncHangNotify_t m_pfnHangNotify;
		struct timespec m_stStatT;  //MONOTONIC
		PFuncStatNotify_t m_pfnStatNotify;
		QueueMonitorInfo_t * m_pstHangShm;
		QueueThread< QueueMonEvent > * m_pclsProcThread;
		Mutex m_clsMapMutex;
		
};

/******************************** Queue Data Class ****************************************/
template <class T>
class InternalQData
{
	public:
		InternalQData(){m_bDogKick=false;m_pvData=NULL;m_unMax=0;m_unCurrent=0;}
		virtual ~InternalQData(){delete m_pvData;}
		
		bool m_bDogKick;
		T * m_pvData;
		unsigned int m_unMax;
		unsigned int m_unCurrent;
};

/******************************** Queue Thread *******************************************/
template <class T>
class QueueThread : public BaseThread
{
	public:
		typedef void(*PfuncProcess)(T *, void *);
		//_nQueueWaitMs : -1(indefinitely), 0(return right now), 0<(sleep during _nMs)
		QueueThread(unsigned int _unMaxQueueSize = 1000,
										PfuncProcess _pfnProcessCb = NULL,
										int _nQueueWaitMs = -1,
										const char * _pszThreadName = NULL,
										bool _bQueueMonitor = false,
										bool _bUseTbb = true)
		: BaseThread(_pszThreadName),
			m_pObject(NULL),
			m_clsLimitQueue(_unMaxQueueSize, _bQueueMonitor, _pszThreadName, _bUseTbb), 
			m_pfnProcessCb(_pfnProcessCb), m_nQueueWaitMs(_nQueueWaitMs),
			m_bDisableMonitor(true)
		{
		}
		virtual ~QueueThread()
		{
		}
		
		void shutdown(){BaseThread::shutdown();m_clsLimitQueue.wake();}
		void enableMonitor(){m_bDisableMonitor = false;}
		bool run()
		{
			m_clsLimitQueue.clear();
			bool bRet =  BaseThread::run();			
			return bRet;
		}
		bool run_(int _nCoreID, int _nStackSize, bool _bDetached)
		{
			m_clsLimitQueue.clear();
			bool bRet = BaseThread::run_(_nCoreID, _nStackSize, _bDetached);			
			return bRet;
		}
		void clearQueue()
		{
			m_clsLimitQueue.clear();
		}
		void setObject(void * _pObject) { m_pObject = _pObject; }
		void setMaxQueueSize(unsigned int _unMaxQueueSize) 
		{ 
			m_clsLimitQueue.setMaxQueueSize(_unMaxQueueSize); 
		}
		void setProcessCb(PfuncProcess _pfnProcessCb) { m_pfnProcessCb = _pfnProcessCb; }
		void setQueueWaitMs(int _nQueueWaitMs) { m_nQueueWaitMs = _nQueueWaitMs; }
		unsigned int getMaxQueueSize() const { return m_clsLimitQueue.getMaxQueueSize(); }
		unsigned int getCurrentQueueSize() const { return m_clsLimitQueue.getCurrentQueueSize(); }
		//void clearQueue() { m_clsLimitQueue.clear(); }
		bool put(T * _pclsData)
		{
			InternalQData<T> * pclsNew = new InternalQData<T>;
			pclsNew->m_pvData = _pclsData;
			bool bRet = m_clsLimitQueue.put(pclsNew);
			if(bRet == false)
			{
				pclsNew->m_pvData = NULL;
				delete pclsNew;
			}
			return bRet;
		}
		bool putWatchDog()
		{
			if(m_bDisableMonitor) return true;
			InternalQData<T> * pclsNew = new InternalQData<T>;
			pclsNew->m_bDogKick = true;
			bool bRet = m_clsLimitQueue.put(pclsNew);
			if(bRet == false)
			{
				delete pclsNew;
			}
			return bRet;
		}
		
	private:
		void process()
		{
			m_nKernelTid = syscall(SYS_gettid);
			
			if( m_bDisableMonitor == false )
			{
				QueueMon & rclsMon = QueueMon::m_fnGetInstance();
				rclsMon.m_fnRegisterInternalQThread(m_szThreadName, m_nKernelTid, getCurrentQueueSize(),this);
			}
			InternalQData<T> * pclsData = NULL;
			
			LogInternal(E_LOG_INFO, "QueueThread[%s]. Tid[%d] : process : start", m_szThreadName, m_nKernelTid);
			
			while( !isShutdown() )
			{
				pclsData = m_clsLimitQueue.get(m_nQueueWaitMs);
				
				if( isShutdown() )
				{
					delete pclsData;
					break;
				}
				
				if( m_pclsInfo ) m_pclsInfo->m_fnSetStartEvTime();
				
				if( pclsData && pclsData->m_bDogKick )
				{
					QueueMon & rclsQMon = QueueMon::m_fnGetInstance();
					LogInternal(E_LOG_STACK, "%s : Tid[%d] : Kick HangMon", m_szThreadName, m_nKernelTid);
					rclsQMon.m_fnKickAlive(m_nKernelTid,getCurrentQueueSize());
				}
				else if( m_pfnProcessCb )
				{
					if( pclsData )
					{
						m_pfnProcessCb(pclsData->m_pvData, m_pObject);
						pclsData->m_pvData = NULL;
					}
					else
						m_pfnProcessCb(NULL, m_pObject);
				}
				
				delete pclsData;
				
				if( m_pclsInfo ) m_pclsInfo->m_fnSetFinishEvTime();
			}
			
			LogInternal(E_LOG_INFO, "QueueThread[%s]. Tid[%d] : process : stop", m_szThreadName, m_nKernelTid);
		}
		
		void * m_pObject;
		LimitedQueue< InternalQData<T> > m_clsLimitQueue;
		PfuncProcess m_pfnProcessCb;
		int m_nQueueWaitMs;
		bool m_bDisableMonitor;
};

}

#define INIT_QMONITOR(NAME,SHMID,HANG_NOTIFY) \
do\
{\
	eSipUtil::QueueMon::m_fnGetInstance(NAME,SHMID,HANG_NOTIFY);\
}while(false)

//ID must be bigger than 0
#define ADD_QMONITOR(NAME,ID,MAXQ,OBJECT,FUNC) \
do\
{\
	eSipUtil::QueueMon::m_fnGetInstance().m_fnRegisterQThread(NAME,ID,MAXQ,OBJECT,FUNC);\
}while(false)

#define KICK_QMONITOR(ID,CURQ) \
do\
{\
	eSipUtil::QueueMon::m_fnGetInstance().m_fnKickAlive(ID,CURQ);\
}while(false)

#define START_QMONITOR() eSipUtil::QueueMon::m_fnGetInstance().m_fnStart()

#define SET_APP_ENABLE_QMONITOR(bEnable, bSync) eSipUtil::QueueMon::m_fnGetInstance().m_fnSetAppEnable(bEnable, bSync)

#define RESET_QMONITOR() eSipUtil::QueueMon::m_fnGetInstance().m_fnResetQThread()

#define DEL_QMONITOR() eSipUtil::QueueMon::m_fnGetInstance().m_fnDelAllQThread()

#endif

