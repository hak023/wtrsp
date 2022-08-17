
#ifndef DUMLIB_SESSION_HXX
#define DUMLIB_SESSION_HXX

#include <list>
#include <string>

#include "resip/stack/SipMessage.hxx"
#include "resip/stack/eSipHandler.hxx"
#include "rutil/Data.hxx"
#include "rutil/Mutex.hxx"
#include "rutil/Lock.hxx"

#include "eSipUtil/idManager.hxx"

#include "eRte/eSipRteApi.hxx"

#include "eDumLibDialog.hxx"
#include "eDumLibDef.hxx"

namespace eDum
{

class Session
{
	public :
		enum
		{
			E_CONST_INVALID_SESSION = -1,
			E_CONST_MAX_SESSION_APP_DATA_SIZE = 16000,
			E_CONST_MAX_CDR_UPDATE_NUM = 20,
			E_CONST_PRINT_LEN = 1024,
		};
		
		void reset();
		const char * str(char * _pszPrint, unsigned int _unLen) const;
		const char * strBrief(char * _pszPrint, unsigned int _unLen) const;
		inline void setUsage(bool _bUse){ m_bUse = _bUse; }
		inline EDialogMethodType_t getDialogMethodType() const{ return m_eDialogMethodType; }
		inline int getSessionId() const{ return m_nSessionId; }
		inline void setTuId(int _nTuId){ m_nTuId = _nTuId; }
		inline int getTuId() const{ return m_nTuId; }
		inline bool getUsage() const{ return m_bUse; }
		inline void setCaller(const resip::Data & _rclsCaller){ m_clsCaller = _rclsCaller; }
		inline const char * getCaller() const
		{ 
			if( strlen(m_stTraceInfo.m_szCaller) != 0 )
				return m_stTraceInfo.m_szCaller;
			else
				return m_clsCaller.c_str();
		}
		inline void setCallee(const resip::Data & _rclsCallee){ m_clsCallee = _rclsCallee; }
		inline const char * getCallee() const
		{ 
			if( strlen(m_stTraceInfo.m_szCallee) != 0 )
				return m_stTraceInfo.m_szCallee;
			else
				return m_clsCallee.c_str(); 
		}
		inline void setCallId(const resip::Data & _rclsCallId){ m_clsCallId = _rclsCallId; }
		inline const resip::Data & getCallId() const{ return m_clsCallId; }
		/*====================================
		add dialog-id about init-direction
		====================================*/
		inline void addOrigDialog(int _nDialogId)
		{
			m_nOrigDialogId = _nDialogId;
		}
		inline bool addTermDialog4Req(int _nDialogId, int & _rnForkingId)
		{
			for( int i = 0; i < Dialog::E_CONST_MAX_FORKING_NUMBER; ++i )
			{
				if( m_arrTermDialogId[i] == Dialog::E_CONST_INVALID_DIALOG )
				{
					m_arrTermDialogId[i] = _nDialogId;
					++m_nActiveTermDialogSize;
					_rnForkingId = i;
					return true;
				}
			}
			return false;
		}
		inline bool addTermDialog4Ha(int _nDialogId, int  _nForkingId)
		{
			if( _nForkingId >= 0 && _nForkingId < Dialog::E_CONST_MAX_FORKING_NUMBER )
			{
				if( m_arrTermDialogId[_nForkingId] == Dialog::E_CONST_INVALID_DIALOG )
					++m_nActiveTermDialogSize;
				m_arrTermDialogId[_nForkingId] = _nDialogId;
				return true;
			}
			return false;
		}
		void delDialog(EDialogInitDir_t _eDialogInitDir, int _nDialogId, int _nForkingId = 0);
		bool isExistDialog(EDialogInitDir_t _eDialogInitDir, int _nDialogId, int _nForkingId = 0) const;
		Dialog * findMatchDialog(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSipMsg, bool & _rbNeedEarlyDialog, int & _nEarlyDialogIndex) const;
		inline int getOrigDialogId() const{ return m_nOrigDialogId; }
		int getTermDialogId(int _nForkingIdx = -1) const;
		Dialog * getOppositeDialog(const Dialog & _rclsDialog, int _nEarlyDialogIdx, int & _nOppositeEarlyDialogIdx) const;
		inline const int * getTermDialogIdArray() const{ return m_arrTermDialogId; }
		inline int getDialogNumber() const
		{ 
			int nDialogNumber = 0;

			if( m_nOrigDialogId >= 0 )
				++nDialogNumber;
			
			for( int i = 0; i < Dialog::E_CONST_MAX_FORKING_NUMBER; ++i )
			{
				if( m_arrTermDialogId[i] >= 0 )
					++nDialogNumber;
			}
		
			return nDialogNumber;
		}
		inline int getActiveTermDialogSize() const{ return m_nActiveTermDialogSize; }
		inline bool checkDelSession(bool _bForce)
		{
			return getUsage() && (_bForce || getDialogNumber() <= 0);
		}
		inline time_t getStartTime() const{ return m_stStartTime; }
		inline time_t getEndTime() const{ return m_stEndTime; }
		inline void setStartTime(const time_t & _rstTime){ m_stStartTime = _rstTime; }
		inline void setEndTime(const time_t & _rstTime){ m_stEndTime = _rstTime; }
		inline void setGarbageTime()
		{
			if( D_DIALOG_MGR.getDialogTimerConfig().getGarbageTimer() > 0 )
			{
				time(&m_stGarbageTime);
				m_stGarbageTime += D_DIALOG_MGR.getDialogTimerConfig().getGarbageTimer();
			}
			else
				m_stGarbageTime = 0;
		}
		inline void setGarbageTime(const time_t & _rstTime){ m_stGarbageTime = _rstTime; }
		inline time_t getGarbageTime() const{ return m_stGarbageTime; }
		inline void setTraceInfo(const resip::TraceInfo_t & _rstTraceInfo){ m_stTraceInfo = _rstTraceInfo; }
		inline const resip::TraceInfo_t & getTraceInfo() const{ return m_stTraceInfo; }
		inline resip::TraceInfo_t & getTraceInfo(){ return m_stTraceInfo; }
		bool setTermReason(int _nTermReason);
		inline void setForceTermReason(int _nTermReason){ m_nTermReason = _nTermReason; }
		inline int getTermReason() const{ return m_nTermReason; }
		inline char * getAppData(){ return m_arrAppData; }
		inline const char * getAppData() const{ return m_arrAppData; }
		bool applyCdr(const Dialog * _pclsDialog, const resip::SipMessage & _rclsSipMsg, bool _bRcv);
		inline const List_Cdr_t & getCdrList() const{ return m_listCdr; }
		void addCdr(const CdrData_t & _rstCdrData);
		inline unsigned int getUpdateCdrNumber() const
		{ 
			if( m_unUpdateCdrNumber > E_CONST_MAX_CDR_UPDATE_NUM )
				return E_CONST_MAX_CDR_UPDATE_NUM;
			else
				return m_unUpdateCdrNumber;
		}
		inline const CdrData_t * getUpdateCdrList(unsigned int & _runListSize) const
		{
			_runListSize = getUpdateCdrNumber();
			return m_arrUpdateCdr;
		}
		void addUpdateCdr(const CdrData_t & _rstCdrData);
		const char * encodeCdrList(char * _pszPrint, unsigned int _unLen) const;
		void encodeCdrList(std::string & _rclsStr) const;
		inline void setFaultDesc(char * _pszFaultDesc)
		{
			free(m_pszFaultDesc);
			m_pszFaultDesc = _pszFaultDesc;
		}
		inline const char * getFaultDesc() const{ return m_pszFaultDesc; }
		inline char * getFaultDescOwn()
		{
			char * pszFaultDesc = m_pszFaultDesc;
			m_pszFaultDesc = NULL;
			return pszFaultDesc;
		}
		
	private :
		Session();
		~Session();

		inline void setDialogMethodType(EDialogMethodType_t _eDialogMethodType){ m_eDialogMethodType = _eDialogMethodType; }
		inline void setSessionId(int _nSessionId){ m_nSessionId = _nSessionId; }

		EDialogMethodType_t m_eDialogMethodType; //not change
		int m_nSessionId; //not change

		int m_nTuId;
		bool m_bUse;
		resip::Data m_clsCaller;
		resip::Data m_clsCallee;
		resip::Data m_clsCallId;
		int m_nOrigDialogId;
		int m_arrTermDialogId[Dialog::E_CONST_MAX_FORKING_NUMBER];
		int m_nActiveTermDialogSize;
		time_t m_stStartTime;
		time_t m_stEndTime;
		time_t m_stGarbageTime;
		resip::TraceInfo_t m_stTraceInfo;
		int m_nTermReason;
		List_Cdr_t m_listCdr;
		CdrData_t m_arrUpdateCdr[E_CONST_MAX_CDR_UPDATE_NUM];
		unsigned int m_unUpdateCdrNumber;
		char * m_pszFaultDesc;

		char m_arrAppData[E_CONST_MAX_SESSION_APP_DATA_SIZE];

		friend class SessionMgr;

};

class SessionConfig
{
	public :
		SessionConfig();
		~SessionConfig();

		SessionConfig & operator=(const SessionConfig & _rstSrcSessionConfig);

		void setInvSessionMaxNumber(int _nMaxNumber);
		void setRegSessionMaxNumber(int _nMaxNumber);
		void setSubSessionMaxNumber(int _nMaxNumber);
		void setStandaloneSessionMaxNumber(int _nMaxNumber);
		inline int getSessionMaxNumber(EDialogMethodType_t _eDialogMethodType) const
		{
			if( isValidDialogMethodType(_eDialogMethodType) )
				return m_arrMaxSessionNumber[_eDialogMethodType];
			else
				return -1;
		}		

	private :
		enum
		{
			E_CONST_DFT_INV_NUMBER = 100000,
			E_CONST_DFT_REG_NUMBER = 500,
			E_CONST_DFT_SUB_NUMBER = 500,
			E_CONST_DFT_STANDALONE_NUMBER = 500,
		};

		int m_arrMaxSessionNumber[E_DIALOG_METHOD_TYPE_NUM];

};

class SessionMgr
{
	public :		
		static SessionMgr & getInstance(const SessionConfig * _pclsSessionConfig = NULL);
		~SessionMgr();

		bool popSessionIdFromIdMgr(EDialogMethodType_t _eDialogMethodType, int _nSessionId);
		Session * getAvailSession(EDialogMethodType_t _eDialogMethodType);
		Session * getAvailSession(EDialogMethodType_t _eDialogMethodType, int _nSessionId);
		void releaseSession(EDialogMethodType_t _eDialogMethodType, int _nSessionId);
		bool releaseSessionData(EDialogMethodType_t _eDialogMethodType, int _nSessionId);
		void releaseSessionId(EDialogMethodType_t _eDialogMethodType, int _nSessionId);
		void resetSessionIdAll();
		void resetSessionDataAll();
		inline const Session * findSession(EDialogMethodType_t _eDialogMethodType, int _nSessionId) const
		{
			if( !isValidDialogMethodType(_eDialogMethodType) )
				return NULL;
			if( _nSessionId < 0 || _nSessionId >= m_clsSessionConfig.getSessionMaxNumber(_eDialogMethodType) )
				return NULL;
			return &(m_arrSession[_eDialogMethodType][_nSessionId]);
		}
		inline Session * findSession(EDialogMethodType_t _eDialogMethodType, int _nSessionId)
		{
			if( !isValidDialogMethodType(_eDialogMethodType) )
				return NULL;
			if( _nSessionId < 0 || _nSessionId >= m_clsSessionConfig.getSessionMaxNumber(_eDialogMethodType) )
				return NULL;
			return &(m_arrSession[_eDialogMethodType][_nSessionId]);
		}
		inline unsigned int getMaxSessionNumber(EDialogMethodType_t _eDialogMethodType) const
		{
			return m_clsSessionConfig.getSessionMaxNumber(_eDialogMethodType);
		}
		inline unsigned int getAvailSessionNumber(EDialogMethodType_t _eDialogMethodType) const
		{
			if( !isValidDialogMethodType(_eDialogMethodType) )
				return 0;

			return m_parrSessionIdMgr[_eDialogMethodType]->getAvailableIdNum();
		}
		inline int getUsingSessionNumber(EDialogMethodType_t _eDialogMethodType) const
		{
			if( !isValidDialogMethodType(_eDialogMethodType) )
				return -1;

			return m_clsSessionConfig.getSessionMaxNumber(_eDialogMethodType) - m_parrSessionIdMgr[_eDialogMethodType]->getAvailableIdNum();
		}
	private :
		SessionMgr(const SessionConfig * _pclsSessionConfig = NULL);

		SessionConfig m_clsSessionConfig;
		Session * m_arrSession[E_DIALOG_METHOD_TYPE_NUM];
		eSipUtil::IdMgr * m_parrSessionIdMgr[E_DIALOG_METHOD_TYPE_NUM];

};

#define D_SESS_MGR SessionMgr::getInstance()


}

#endif

