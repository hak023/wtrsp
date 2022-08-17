
#ifndef DUMLIB_DIALOG_HXX
#define DUMLIB_DIALOG_HXX

#include <list>
#include <time.h>

#include "resip/stack/SipMessage.hxx"
#include "rutil/Data.hxx"
#include "rutil/Mutex.hxx"
#include "rutil/Lock.hxx"

#include "eSipUtil/idManager.hxx"
#include "eSipUtil/timeUtil.hxx"

#include "eDumLibFsm.hxx"
#include "eDumLibDef.hxx"
#include "eDumLibDialogTimer.hxx"
#include "eDumLibCdr.hxx"

namespace eDum
{

typedef enum
{
	E_DIALOG_TYPE_UA = 0,
	E_DIALOG_TYPE_PROXY,

	E_DIALOG_TYPE_NUM,
}EDialogType_t;

typedef enum
{
	E_DIALOG_INIT_DIR_ORIG = 0,
	E_DIALOG_INIT_DIR_TERM,

	E_DIALOG_INIT_DIR_NUM,
}EDialogInitDir_t;

typedef enum
{
	E_INVITE_DIALOG_TYPE_INIT = 0,
	E_INVITE_DIALOG_TYPE_EARLY, //early dialog
	E_INVITE_DIALOG_TYPE_ESTABLISH, //establish dialog
	
	E_INVITE_DIALOG_TYPE_NUM,
}EInviteDialogType_t;

/*==== Dialog Type ====*/
bool isValidDialogType(EDialogType_t _eDialogType);
const char * getDialogTypeString(EDialogType_t _eDialogType);
/*==== Dialog Method Type ====*/
bool isValidDialogMethodType(EDialogMethodType_t _eDialogMethodType);
const char * getDialogMethodTypeString(EDialogMethodType_t _eDialogMethodType);
EDialogMethodType_t convertResipMethod2DialogMethod(resip::MethodTypes _eResipMethodType);
bool isGeneralDlgCreateMethod(resip::MethodTypes _eResipMethodType);
bool isInvStateMethod(resip::MethodTypes _eResipMethodType);
bool isInvOtherMethod(resip::MethodTypes _eResipMethodType);
bool isSubStateMethod(resip::MethodTypes _eResipMethodType);
bool isSubOtherMethod(resip::MethodTypes _eResipMethodType);
bool isStandAloneDlgCreateMethod(resip::MethodTypes _eResipMethodType);
/*==== Dialog Init Dir ====*/
bool isValidDialogInitDir(EDialogInitDir_t _eDialogInitDir);
const char * getDialogInitDirString(EDialogInitDir_t _eDialogInitDir);
/*==== INVITE Dialog Type ====*/
bool isValidInviteDialogType(EInviteDialogType_t _eInviteDialogType);
const char * getInviteDialogTypeString(EInviteDialogType_t _eInviteDialogType);

struct InDialogReqInfo
{
	EFsmEventType_t m_eFsmEventType;
	resip::SipMessage * m_pclsInDialogReq;

	InDialogReqInfo()
		: m_eFsmEventType(E_FSM_EVENT_NUM), m_pclsInDialogReq(NULL)
	{
	}
};
typedef struct InDialogReqInfo InDialogReqInfo_t;
typedef std::list< InDialogReqInfo_t > List_InDialogReqInfo_t;
void addInDialogReqInfo(List_InDialogReqInfo_t & _rList, EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSipMsg);
void delFirstInDialogReqInfo(List_InDialogReqInfo_t & _rList, EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSipMsg);
void delInDialogReqInfo(List_InDialogReqInfo_t & _rList, EFsmEventType_t _eFsmEventType, resip::MethodTypes _eMethodType, unsigned int _unCseq);
/*==== It is used on sending SipMsg ====*/
const resip::SipMessage * findInDialogRequest(const List_InDialogReqInfo_t & _rList, EFsmEventType_t _eFsmEventType, resip::MethodTypes _eMethodType);
/*==== It is used on receiving SipMsg ====*/
const resip::SipMessage * findInDialogRequest(const List_InDialogReqInfo_t & _rList, EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSipMsg);
/*==== It is used on receiving SipMsg for retrans ====*/
const resip::SipMessage * findInDialogPrackRequest(const List_InDialogReqInfo_t & _rList, EFsmEventType_t _eFsmEventType, 
																									unsigned int _unRSeq, unsigned int _unInvSeq,
																									const char * _pszFromTag, const char * _pszToTag);
const resip::SipMessage * findInDialogAckRequest(const List_InDialogReqInfo_t & _rList, EFsmEventType_t _eFsmEventType, 
																								unsigned int _unInvSeq,
																								const char * _pszFromTag, const char * _pszToTag);

typedef std::list< resip::Via > List_Via_t;
typedef std::list< CdrData_t > List_Cdr_t;
typedef std::list< resip::NameAddr > List_RecordRoute_t;
typedef std::list< resip::NameAddr > List_RouteSet_t;

class Dialog
{
	public :	
		enum
		{
			E_CONST_INVALID_DIALOG = -1,
			E_CONST_MAX_FORKING_NUMBER = 1,
			E_CONST_PRINT_LEN = 4000,
		};

		static resip::Data generateCallId(EDialogInitDir_t _eDialogInitDir, const resip::Data * _pclsReplaceString = NULL);
		static resip::Data generateTag(const resip::Data * _pclsOrigTag = NULL);
		static resip::Data generateStackTag(int _nRspCode, const char * _pszAdditionToTag);
		static bool isStackTag(const resip::Data & _rclsTag, int & _rnRspCode, const char * _pszAdditionToTag);

		virtual void reset();
		virtual const char * str(char * _pszPrint, unsigned int _unLen) const;
		virtual const char * strBrief(char * _pszPrint, unsigned int _unLen) const;
		virtual void copy(const Dialog & _rclsSrcDialog);
		/*=========================================
		"isMatch" function is practically used on receiving SipMsg.
		The case of sending SipMsg, because SipMsg is preferentially applied by DialogData , this function is not needed.
		=========================================*/
		virtual bool isMatch(EFsmEventType_t _eFsmEventType, resip::MethodTypes _eSipMethodType, bool _bRequest,
											const resip::Data & _rclsCallId, const resip::Data & _rclsFromTag, const resip::Data & _rclsToTag,
											bool & _bNeedEarlyDialog, int & _nEarlyDialogIndex) const;
		virtual bool isMatch(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSipMsg,
											bool & _bNeedEarlyDialog, int & _nEarlyDialogIndex) const = 0;
		
		virtual int setTimer(const resip::SipMessage * _pclsSipMsg, DialogSetTimerResult_t * _parrDialogSetTimerResult, bool _bSetGarbage) = 0;

		inline void setUsage(bool _bUse){ m_bUse = _bUse; }
		inline bool getUsage() const{ return m_bUse; }
		inline int getDialogId() const{ return m_nDialogId; }
		inline void setTuId(int _nTuId){ m_nTuId = _nTuId; }
		inline int getTuId() const{ return m_nTuId; }
		inline void setSessionId(int _nSessionId){ m_nSessionId = _nSessionId; }
		inline int getSessionId() const{ return m_nSessionId; }
		inline void setForkingIdx(int _nForingIdx){ m_nForkingIdx = _nForingIdx; }
		inline int getForkingIdx() const{ return m_nForkingIdx; }
		inline EDialogMethodType_t getDialogMethodType() const{ return m_eDialogMethodType; }
		inline void setDialogInitDir(EDialogInitDir_t _eDialogInitDir){ m_eDialogInitDir = _eDialogInitDir; }
		inline EDialogInitDir_t getDialogInitDir() const{ return m_eDialogInitDir; }
		inline void applyNextSipState(ESipState_t _eNextSipState)
		{
			if( m_eSipCurrentState != _eNextSipState )
			{
				m_eSipPrevState = m_eSipCurrentState;
				m_eSipCurrentState = _eNextSipState;
			}
		}
		inline void setSipPrevState(ESipState_t _eSipPrevState){ m_eSipPrevState = _eSipPrevState; }
		inline ESipState_t getSipPrevState() const{ return m_eSipPrevState; }
		inline void setSipCurrentState(ESipState_t _eSipCurrentState){ m_eSipCurrentState = _eSipCurrentState; }
		inline ESipState_t getSipCurrentState() const{ return m_eSipCurrentState; }
		inline void setLocalIp(const char * _pszLocalIp){ m_stLocalTuple.m_clsIpAddr = _pszLocalIp; }
		inline void setLocalPort(int _nLocalPort){ m_stLocalTuple.m_nPort = _nLocalPort; }
		inline void setLocalTransportType(resip::TransportType _eTransportType){ m_stLocalTuple.m_eTransportType= _eTransportType; }
		inline const NetTuple_t & getLocalTuple() const{ return m_stLocalTuple; }
		inline void setNodeLocalId(unsigned int _unNodeLocalId){ m_unNodeLocalId = _unNodeLocalId; }
		inline unsigned int getNodeLocalId() const{ return m_unNodeLocalId; }
		inline void setRemoteIp(const char * _pszRemoteIp){ m_stRemoteTuple.m_clsIpAddr = _pszRemoteIp; }
		inline void setRemotePort(int _nRemotePort){ m_stRemoteTuple.m_nPort = _nRemotePort; }
		inline void setRemoteTransportType(resip::TransportType _eTransportType){ m_stRemoteTuple.m_eTransportType= _eTransportType; }
		inline const NetTuple_t & getRemoteTuple() const{ return m_stRemoteTuple; }
		inline void setNodeRemoteId(unsigned int _unNodeRemoteId){ m_unNodeRemoteId = _unNodeRemoteId; }
		inline unsigned int getNodeRemoteId() const{ return m_unNodeRemoteId; }
		inline void setRouteId(unsigned int _unRouteId){ m_unRouteId = _unRouteId; }
		inline unsigned int getRouteId() const{ return m_unRouteId; }
		inline void setDSCP(int _nDSCP){ m_nDSCP = _nDSCP; }
		inline int getDSCP() const{ return m_nDSCP; }
		inline void setLocalCseq(unsigned int _unLocalCseq){ m_unLocalCseq = _unLocalCseq; }
		inline unsigned int getLocalCseq() const{ return m_unLocalCseq; }
		inline void setRemoteCseq(unsigned int _unRemoteCseq){ m_unRemoteCseq = _unRemoteCseq; }
		inline unsigned int getRemoteCseq() const{ return m_unRemoteCseq; }
		inline void setLocalAor(const resip::NameAddr & _rclsLocalAor){ m_clsLocalAor = _rclsLocalAor; }
		inline const resip::NameAddr & getLocalAor() const{ return m_clsLocalAor; }
		inline void setRemoteAor(const resip::NameAddr & _rclsRemoteAor){ m_clsRemoteAor = _rclsRemoteAor; }
		inline const resip::NameAddr & getRemoteAor() const{ return m_clsRemoteAor; }
		inline void setLocalContact(const resip::NameAddr & _rclsLocalContact){ m_clsLocalContact = _rclsLocalContact; }
		inline const resip::NameAddr & getLocalContact() const{ return m_clsLocalContact; }
		inline void setRemoteContact(const resip::NameAddr & _rclsRemoteContact){ m_clsRemoteContact = _rclsRemoteContact; }
		inline const resip::NameAddr & getRemoteContact() const{ return m_clsRemoteContact; }
		inline void setCallId(const resip::Data & _rclsCallId){ m_clsCallId = _rclsCallId; }
		inline const resip::Data & getCallId() const{ return m_clsCallId; }
		inline void addViaHeader(const resip::Via & _rclsVia){ m_listViaHeader.push_back(_rclsVia); }
		inline const List_Via_t & getViaHeaderList() const{ return m_listViaHeader; }
		inline void clearViaHeaderList(){ m_listViaHeader.clear(); }
		inline bool isLocalAddr(const resip::Data & _rclsIpAddr, int _nPort) const
		{
			if( _rclsIpAddr == m_stLocalTuple.m_clsIpAddr && _nPort == m_stLocalTuple.m_nPort )
				return true;
			else
				return false;
		}
		inline bool isLocalAddr(const resip::Data & _rclsIpAddr, int _nPort, const resip::Data & _rclsTransport) const
		{
			if( _rclsIpAddr == m_stLocalTuple.m_clsIpAddr
				&& _nPort == m_stLocalTuple.m_nPort
				&& resip::isEqualNoCase(_rclsTransport, resip::toData(m_stLocalTuple.m_eTransportType)) )
				return true;
			else
				return false;
		}
		bool getLocalContactAddr(resip::NameAddr & _rclsContactAddr) const;
		inline void setGarbageTime(const time_t & _rstTime){ m_GarbageTime = _rstTime; }
		void setGarbageTime();		
		inline time_t getGarbageTime() const{ return m_GarbageTime; }
		inline const eSipUtil::SyncTime * getTimeInfoArray() const{ return m_arrTimeInfo; }
		inline void resetTimeInfo(EDialogTimerType_t _eDialogTimerType)
		{
			if( isValidDialogTimerType(_eDialogTimerType) )
			{
				m_arrTimeInfo[_eDialogTimerType].reset();
				return;
			}
		}
		inline bool setTimeInfo(EDialogTimerType_t _eDialogTimerType, const struct timespec & _rstExpireTime)
		{
			if( isValidDialogTimerType(_eDialogTimerType) )
			{
				m_arrTimeInfo[_eDialogTimerType].setTime(_rstExpireTime);
				return true;
			}
			return false;
		}
		bool isInitRequest(resip::MethodTypes _eMethod) const;

	protected :
		Dialog();
		virtual ~Dialog();

		inline void setDialogMethodType(EDialogMethodType_t _eDialogMethodType){ m_eDialogMethodType = _eDialogMethodType; }

		EDialogMethodType_t m_eDialogMethodType; //not change
		int m_nDialogId; //not change

		int m_nTuId;
		EDialogInitDir_t m_eDialogInitDir;
 		int m_nSessionId;
		int m_nForkingIdx; //if term-InvDialog, it is set. If orig-InvDialog, it is 0		
		/*==== state information ====*/
		ESipState_t m_eSipCurrentState;
		ESipState_t m_eSipPrevState;
		/*==== transport information ====*/
		NetTuple_t m_stLocalTuple;		//when dialog is created, it is set
		unsigned int m_unNodeLocalId;
		NetTuple_t m_stRemoteTuple;	//when dialog is created, it is set
		unsigned int m_unNodeRemoteId;
		unsigned int m_unRouteId;
		int m_nDSCP;
		/*==== sip information ====*/
		unsigned int m_unLocalCseq;
		unsigned int m_unRemoteCseq;
		resip::Data m_clsCallId;
		mutable resip::NameAddr m_clsLocalAor;
		mutable resip::NameAddr m_clsRemoteAor;
		mutable resip::NameAddr m_clsLocalContact;
		mutable resip::NameAddr m_clsRemoteContact;
		/*==== 1. save all of header when receive REQ and delete saved header when send final RSP(orig dialog)
			     	  2. save topmost of header when send REQ and delete saved header when recv final RSP(term dialog) ====*/
		mutable List_Via_t m_listViaHeader;
		time_t m_GarbageTime;
		eSipUtil::SyncTime m_arrTimeInfo[E_DIALOG_TIMER_NUM];
		
	private :
		inline void setDialogId(int _nDialogId){ m_nDialogId = _nDialogId; }

		bool m_bUse;
		
		friend class DialogMgr;
		
};

class StandAloneDialog : public Dialog
{
	public :
		inline static bool validMethod(resip::MethodTypes _eResipMethodType)
		{
			if( isStandAloneDlgCreateMethod(_eResipMethodType) )
				return true;
			else
				return false;
		}

		virtual void reset();
		virtual const char * str(char * _pszPrint, unsigned int _unLen) const;
		virtual const char * strBrief(char * _pszPrint, unsigned int _unLen) const;
		virtual void copy(const Dialog & _rclsSrcDialog);
		virtual bool isMatch(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSipMsg,
											bool & _bNeedEarlyDialog, int & _nEarlyDialogIndex) const;
		virtual int setTimer(const resip::SipMessage * _pclsSipMsg, DialogSetTimerResult_t * _parrDialogSetTimerResult, bool _bSetGarbage);

		inline void setSipMethodType(resip::MethodTypes _eSipMethodType){ m_eSipMethodType = _eSipMethodType; }
		inline resip::MethodTypes getSipMethodType() const{ return m_eSipMethodType; }

	protected :
		StandAloneDialog();
		virtual ~StandAloneDialog();

	private :
		resip::MethodTypes m_eSipMethodType;

		friend class DialogMgr;

};

class SubscribeDialog : public Dialog
{
	public :
		enum
		{
			E_CONST_MAX_INDIALOG_REQ_SAVE_NUMBER = 3,
		};
		
		inline static bool validMethod(resip::MethodTypes _eResipMethodType)
		{
			if( isSubStateMethod(_eResipMethodType) || isSubOtherMethod(_eResipMethodType) )
				return true;
			else
				return false;
		}

		virtual void reset();
		virtual const char * str(char * _pszPrint, unsigned int _unLen) const;
		virtual const char * strBrief(char * _pszPrint, unsigned int _unLen) const;
		virtual void copy(const Dialog & _rclsSrcDialog);
		virtual bool isMatch(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSipMsg,
											bool & _bNeedEarlyDialog, int & _nEarlyDialogIndex) const;
		virtual int setTimer(const resip::SipMessage * _pclsSipMsg, DialogSetTimerResult_t * _parrDialogSetTimerResult, bool _bSetGarbage);

		inline void setInitial(bool _bInit){ m_bInitial = _bInit; }
		inline bool getInitial() const{ return m_bInitial; }
		inline void setSndReqUri(const resip::Uri & _rclsReqUri){ m_clsSndReqUri = _rclsReqUri; }
		inline const resip::Uri & getSndReqUri() const{ return m_clsSndReqUri; }
		inline void setSessionExpire(unsigned int _unSessionExpire){ m_unSessionExpire = _unSessionExpire; }
		inline unsigned int getSessionExpire() const{ return m_unSessionExpire; }
		inline const List_InDialogReqInfo_t & getInDialogReqInfoList() const{ return m_listInDialogReqInfo; }
		inline List_InDialogReqInfo_t & getInDialogReqInfoList(){ return m_listInDialogReqInfo; }
		inline void addRecordRoute(const resip::NameAddr & _rclsRR){ m_listRecordRoute.push_back(_rclsRR); }
		inline const List_RecordRoute_t & getRecordRouteList() const{ return m_listRecordRoute; }
		inline void clearRecordRouteList(){ m_listRecordRoute.clear(); }
		inline void addFrontRouteSet(const resip::NameAddr & _rclsRoute){ m_listRouteSet.push_front(_rclsRoute); }
		inline void addBackRouteSet(const resip::NameAddr & _rclsRoute){ m_listRouteSet.push_back(_rclsRoute); }
		inline const List_RouteSet_t & getRouteSetList() const{ return m_listRouteSet; }
		inline void clearRouteSetList(){ m_listRouteSet.clear(); }

	protected :
		SubscribeDialog();
		virtual ~SubscribeDialog();

	private :
		bool m_bInitial;
		/*==== sip information ====*/
		resip::Uri m_clsSndReqUri;
		unsigned int m_unSessionExpire;		//sec
		/*==== 1. save request when receive/send InDialogReq(non Subscribe) and delete saved request when send/receive final RSP ====*/
		List_InDialogReqInfo_t m_listInDialogReqInfo;
		/*==== 1. save all of header when receive REQ of InvDlg and delete saved header when send final RSP(orig dialog) ====*/
		List_RecordRoute_t m_listRecordRoute;
		/*==== 1. save route-set when receive INVITE 18x-2xx RSP ====*/
		List_RouteSet_t m_listRouteSet;

		friend class DialogMgr;

};

class InviteDialog : public Dialog
{
	public :
		enum
		{
			E_CONST_MAX_INDIALOG_REQ_SAVE_NUMBER = 50,
			E_CONST_MAX_TERM_EARLY_DIALOG_NUMBER = 17,
			E_CONST_MAX_ORIG_EARLY_DIALOG_NUMBER = E_CONST_MAX_TERM_EARLY_DIALOG_NUMBER*E_CONST_MAX_FORKING_NUMBER,
		};

		struct EarlyDialog
		{
			bool m_bUse;
			resip::NameAddr m_clsToAor;
			unsigned int m_unLocalCseq;
			unsigned int m_unRemoteCseq;
			unsigned int m_unRseq;
			ESipState_t m_eSipCurrentState;
			ESipState_t m_eSipPrevState;
			ESipState_t m_eSipStateForSdp;
			ESdpState_t m_eSdpCurrentState;

			EarlyDialog()
			{
				reset();
			}

			void reset()
			{
				m_bUse = false;
				m_clsToAor.reset();
				m_unLocalCseq = 0;
				m_unRemoteCseq = 0;
				m_unRseq = 0;
				m_eSipCurrentState = E_SIP_STATE_IDLE;
				m_eSipPrevState = E_SIP_STATE_IDLE;
				m_eSipStateForSdp = E_SIP_STATE_IDLE;
				m_eSdpCurrentState = E_SDP_STATE_IDLE;
			}
		};
		typedef struct EarlyDialog EarlyDialog_t;

		inline static bool validMethod(resip::MethodTypes _eResipMethodType)
		{
			if( isInvStateMethod(_eResipMethodType) || isInvOtherMethod(_eResipMethodType) )
				return true;
			else
				return false;
		}

		virtual void reset();
		virtual const char * str(char * _pszPrint, unsigned int _unLen) const;
		virtual const char * strBrief(char * _pszPrint, unsigned int _unLen) const;
		virtual void copy(const Dialog & _rclsSrcDialog);
		virtual bool isMatch(EFsmEventType_t _eFsmEventType, resip::MethodTypes _eSipMethodType, bool _bRequest,
											const resip::Data & _rclsCallId, const resip::Data & _rclsFromTag, const resip::Data & _rclsToTag,
											bool & _bNeedEarlyDialog, int & _nEarlyDialogIndex) const;
		virtual bool isMatch(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSipMsg,
											bool & _bNeedEarlyDialog, int & _nEarlyDialogIndex) const;
#if 0		
		bool isMatch(EFsmEventType_t _eFsmEventType, resip::MethodTypes _eSipMethodType, bool _bRequest,
									const resip::Data & _rclsCallId, const resip::Data & _rclsFromTag, const resip::Data & _rclsToTag,
									int _nEarlyDialogIndex) const;
		bool isMatch(EFsmEventType_t _eFsmEventType, const resip::SipMessage & _rclsSipMsg, int _nEarlyDialogIndex) const;
#endif		
		virtual int setTimer(const resip::SipMessage * _pclsSipMsg, DialogSetTimerResult_t * _parrDialogSetTimerResult, bool _bSetGarbage);

		inline void applyNextSdpState(ESdpState_t _eNextSdpState, resip::MethodTypes _eResipMethodType, int _nRspCode)
		{
			m_eSdpCurrentState = _eNextSdpState;
			if( _eResipMethodType == resip::UPDATE && _nRspCode == 0 )
				m_eSipStateForSdp = E_SIP_STATE_STANDALONE_UAS_REQ;
			else
				m_eSipStateForSdp = m_eSipCurrentState;
		}
		inline void setSdpCurrentState(ESdpState_t _eSdpState){ m_eSdpCurrentState = _eSdpState; }
		inline ESdpState_t getSdpCurrentState() const{ return m_eSdpCurrentState; }
		inline void setSipStateForSdp(ESipState_t _eSipState){ m_eSipStateForSdp = _eSipState; }
		inline ESipState_t getSipStateForSdp() const{ return m_eSipStateForSdp; }
		inline void setSndReqUri(const resip::Uri & _rclsReqUri){ m_clsSndReqUri = _rclsReqUri; }
		inline const resip::Uri & getSndReqUri() const{ return m_clsSndReqUri; }
		inline void setInviteCseq(unsigned int _unInviteCseq){ m_unInviteCseq = _unInviteCseq; }
		inline unsigned int getInviteCseq() const{ return m_unInviteCseq; }
		inline void setRseq(unsigned int _unRseq){ m_unRseq = _unRseq; }
		inline unsigned int getRseq() const{ return m_unRseq; }
		inline void setRefresher(bool _bRefresher){ m_bRefresher = _bRefresher; }
		inline bool IsRefresher() const{ return m_bRefresher; }
		inline void setSessionExpire(unsigned int _unSessionExpire){ m_unSessionExpire = _unSessionExpire; }
		inline unsigned int getSessionExpire() const{ return m_unSessionExpire; }
		inline void addRecordRoute(const resip::NameAddr & _rclsRR){ m_listRecordRoute.push_back(_rclsRR); }
		inline const List_RecordRoute_t & getRecordRouteList() const{ return m_listRecordRoute; }
		inline void clearRecordRouteList(){ m_listRecordRoute.clear(); }
		inline void addFrontRouteSet(const resip::NameAddr & _rclsRoute){ m_listRouteSet.push_front(_rclsRoute); }
		inline void addBackRouteSet(const resip::NameAddr & _rclsRoute){ m_listRouteSet.push_back(_rclsRoute); }
		inline const List_RouteSet_t & getRouteSetList() const{ return m_listRouteSet; }
		inline void clearRouteSetList(){ m_listRouteSet.clear(); }
		inline const EarlyDialog_t * getEarlyDialogArray() const{ return m_arrEarlyDialog; }
		inline EarlyDialog_t * getEarlyDialog(int _nEarlyDlgIdx)
		{
			if( _nEarlyDlgIdx < 0 || _nEarlyDlgIdx >= E_CONST_MAX_ORIG_EARLY_DIALOG_NUMBER )
				return NULL;

			return &(m_arrEarlyDialog[_nEarlyDlgIdx]);
		}
		inline bool getAvailEarlyDialog(const resip::NameAddr & _rclsToAor, int & _rnEarlyDlgIdx)
		{
			return getAvailEarlyDialog(&_rclsToAor, _rnEarlyDlgIdx);
		}
		bool getAvailEarlyDialog(const resip::NameAddr * _pclsToAor, int & _rnEarlyDlgIdx);
		inline bool setToAorEarlyDialog(const resip::NameAddr & _rclsToAor, int _nEarlyDlgIdx)
		{
			if( _nEarlyDlgIdx < 0 || _nEarlyDlgIdx >= E_CONST_MAX_ORIG_EARLY_DIALOG_NUMBER )
				return false;

#ifdef CCM_FIX_B2BUA_TOAOR		//20190722
			if( m_arrEarlyDialog[_nEarlyDlgIdx].m_clsToAor.uri().scheme().empty() )
				m_arrEarlyDialog[_nEarlyDlgIdx].m_clsToAor = _rclsToAor;
			else
				m_arrEarlyDialog[_nEarlyDlgIdx].m_clsToAor.param(resip::p_tag) = _rclsToAor.param(resip::p_tag);
#else
			m_arrEarlyDialog[_nEarlyDlgIdx].m_clsToAor = _rclsToAor;
#endif
			
			return true;
		}
		inline bool isUsingEarlyDialog(int _nEarlyDlgIdx)
		{
			if( _nEarlyDlgIdx < 0 || _nEarlyDlgIdx >= E_CONST_MAX_ORIG_EARLY_DIALOG_NUMBER )
				return false;
			return m_arrEarlyDialog[_nEarlyDlgIdx].m_bUse;
		}
		inline void resetEarlyDialog(int _nEarlyDlgIdx)
		{
			if( _nEarlyDlgIdx < 0 || _nEarlyDlgIdx >= E_CONST_MAX_ORIG_EARLY_DIALOG_NUMBER )
				return;
			m_arrEarlyDialog[_nEarlyDlgIdx].reset();
		}
		void resetEarlyDialogAll();
		void copyDialog2EarlyDialog(int _nEarlyDlgIdx);
		void copyEarlyDialog2Dialog(int _nEarlyDlgIdx);
		bool convertOppositeEarlyDialogIdx(int _nLocalEarlyDlgIdx, int & _nOppositeForkingIdx, int & _nOppositeEarlyDlgIdx);
		inline void setEarlyDialogResetFlag(bool _bReset){ m_bEarlyDialogResetFlag = _bReset; }
		inline bool getEarlyDialogResetFlag() const{ return m_bEarlyDialogResetFlag; }
		inline void setRsp4Retrans(const resip::SipMessage * _pclsRsp)
		{
			delete m_pclsRsp4Retransmission;
			m_pclsRsp4Retransmission = NULL;
			if( _pclsRsp )
				m_pclsRsp4Retransmission = (resip::SipMessage *)(_pclsRsp->clone());
		}
		inline const resip::SipMessage * getRsp4Retrans() const{ return m_pclsRsp4Retransmission; }
		inline void setTimerGInterval(const unsigned long & _rTimer){ m_ulTimerGInterval = _rTimer; }
		inline const unsigned long & getTimerGInterval() const{ return m_ulTimerGInterval; }
		inline void setTimerRelInterval(const unsigned long & _rTimer){ m_ulTimerRelInterval = _rTimer; }
		inline const unsigned long & getTimerRelInterval() const{ return m_ulTimerRelInterval; }
		inline const List_InDialogReqInfo_t & getInDialogReqInfoList() const{ return m_listInDialogReqInfo; }
		inline List_InDialogReqInfo_t & getInDialogReqInfoList(){ return m_listInDialogReqInfo; }
		bool checkBranch4Rsp(const resip::SipMessage & _rclsRsp) const;
		bool checkAcl4Req(const resip::SipMessage & _rclsReq) const;

	protected :
		InviteDialog();
		virtual ~InviteDialog();

		bool validate4EarlyDialog(resip::MethodTypes _eMethodType, const resip::Data & _rclsCheckTag, bool _bCheckRemote, int & _nEarlyDlgIdx) const;

	private :
		/*==== state information ====*/
		ESdpState_t m_eSdpCurrentState;
		ESipState_t m_eSipStateForSdp;		//when SDP-state is changed, this is set for SIP-state at time.
		/*==== sip information ====*/
		resip::Uri m_clsSndReqUri;
		unsigned int m_unInviteCseq;
		unsigned int m_unRseq;
		bool m_bRefresher;								//Self Session Refresh flag
		unsigned int m_unSessionExpire;		//sec. for alive timer
		/*==== 1. save request when receive/send InDialogReq(non Invite) and delete saved request when send/receive final RSP(not include replication data) ====*/
		List_InDialogReqInfo_t m_listInDialogReqInfo;
		/*==== 1. save all of header when receive REQ of InvDlg and delete saved header when send final RSP(orig dialog) ====*/
		List_RecordRoute_t m_listRecordRoute;
		/*==== 1. save route-set when receive INVITE 18x-2xx RSP ====*/
		List_RouteSet_t m_listRouteSet;
		/*==== early dialog information array ====*/
		EarlyDialog_t m_arrEarlyDialog[E_CONST_MAX_ORIG_EARLY_DIALOG_NUMBER];
		bool m_bEarlyDialogResetFlag;
		/*==== retransmission timer ==== */
		resip::SipMessage * m_pclsRsp4Retransmission;
		unsigned long m_ulTimerGInterval;
		unsigned long m_ulTimerRelInterval;

		friend class DialogMgr;
		
};

class DialogConfig
{
	public :
		DialogConfig();
		~DialogConfig();
		
		DialogConfig & operator=(const DialogConfig & _rstSrcDialogConfig);

		void setDialogType(EDialogType_t _eDialogType);
		inline EDialogType_t getDialogType() const
		{
			return m_eDialogType;
		}
		inline void setSipOwnUsername(const resip::Data & _rclsUsername){ m_clsSipOwnUserName = _rclsUsername; }
		inline const resip::Data & getSipOwnUsername() const{ return m_clsSipOwnUserName; }
		inline void setUseSessRefresh(bool _bUse){ m_bUseSessRefresh = _bUse; }
		inline bool getUseSessRefresh() const{ return m_bUseSessRefresh; }
		void setInvDialogMaxNumber(int _nMaxNumber);
		void setRegDialogMaxNumber(int _nMaxNumber);
		void setSubDialogMaxNumber(int _nMaxNumber);
		void setStandaloneDialogMaxNumber(int _nMaxNumber);
		inline int getDialogMaxNumber(EDialogMethodType_t _eDialogMethodType) const
		{
			if( isValidDialogMethodType(_eDialogMethodType) )
				return m_arrMaxDialogNumber[_eDialogMethodType];
			else
				return -1;
		}

	private :
		enum
		{
			E_CONST_DFT_TYPE = E_DIALOG_TYPE_UA,
			E_CONST_DFT_INV_NUMBER = 200000,
			E_CONST_DFT_REG_NUMBER = 1000,
			E_CONST_DFT_SUB_NUMBER = 1000,
			E_CONST_DFT_STANDALONE_NUMBER = 1000,
		};

		EDialogType_t m_eDialogType;
		resip::Data m_clsSipOwnUserName;
		bool m_bUseSessRefresh;
		int m_arrMaxDialogNumber[E_DIALOG_METHOD_TYPE_NUM];
	 
};

class DialogMgr
{
	public :
		enum
		{
			E_CONST_MAX_TAG_PREFIX_LEN = 50,
		};
		
		static DialogMgr & getInstance(const DialogConfig * _pclsDialogConfig = NULL, const char * _pszTagPrefix = NULL);
		~DialogMgr();

		const char * getTagPrefix() const;
		const char * getStackTagPrefix() const;

		inline EDialogType_t getDialogType() const{ return m_clsDialogConfig.getDialogType(); }
		const Dialog * findDialog(EDialogMethodType_t _eDialogMethodType, int _nDialogId) const;		
		Dialog * findDialog(EDialogMethodType_t _eDialogMethodType, int _nDialogId);
		bool popDialogIdFromIdMgr(EDialogMethodType_t _eDialogMethodType, int _nDialogId);
		Dialog * getAvailDialog(EDialogMethodType_t _eDialogMethodType);
		Dialog * getAvailDialog(EDialogMethodType_t _eDialogMethodType, int _nDialogId);
		void releaseDialog(EDialogMethodType_t _eDialogMethodType, int _nDialogId);		
		bool releaseDialogData(EDialogMethodType_t _eDialogMethodType, int _nDialogId);
		void releaseDialogId(EDialogMethodType_t _eDialogMethodType, int _nDialogId);
		void resetDialogIdAll();
		void resetDialogDataAll();
		inline unsigned int getMaxDialogNumber(EDialogMethodType_t _eDialogMethodType) const
		{
			return m_clsDialogConfig.getDialogMaxNumber(_eDialogMethodType);
		}
		inline unsigned int getAvailDialogNumber(EDialogMethodType_t _eDialogMethodType) const
		{
			if( !isValidDialogMethodType(_eDialogMethodType) )
				return 0;
			return m_parrDialogIdMgr[_eDialogMethodType]->getAvailableIdNum();
		}
		inline int getUsingDialogNumber(EDialogMethodType_t _eDialogMethodType) const
		{
			if( !isValidDialogMethodType(_eDialogMethodType) )
				return -1;
			return m_clsDialogConfig.getDialogMaxNumber(_eDialogMethodType) - m_parrDialogIdMgr[_eDialogMethodType]->getAvailableIdNum();
		}
		inline const DialogConfig & getDialogConfig() const
		{
			return m_clsDialogConfig;
		}
		inline DialogTimerConfig & getDialogTimerConfig()
		{
			return m_clsDialogTimerConfig;
		}
		inline const DialogTimerConfig & getDialogTimerConfig() const
		{
			return m_clsDialogTimerConfig;
		}

	private :
		DialogMgr(const DialogConfig * _pclsDialogConfig = NULL, const char * _pszTagPrefix = NULL);

		DialogConfig m_clsDialogConfig;
		DialogTimerConfig m_clsDialogTimerConfig;

		char m_szTagPrefix[E_CONST_MAX_TAG_PREFIX_LEN];
		char m_szStackTagPrefix[E_CONST_MAX_TAG_PREFIX_LEN];

		InviteDialog * m_arrInvDialog;
		StandAloneDialog * m_arrStandAloneDialog;
		SubscribeDialog * m_arrSubscribeDialog;
		//???? REGISTER array
		eSipUtil::IdMgr * m_parrDialogIdMgr[E_DIALOG_METHOD_TYPE_NUM];

};

#define D_DIALOG_MGR DialogMgr::getInstance()

}

#endif

