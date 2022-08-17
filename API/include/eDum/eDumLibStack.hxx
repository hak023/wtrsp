
#ifndef DUMLIB_STACK_HXX
#define DUMLIB_STACK_HXX

#include "resip/stack/TransactionUser.hxx"
#include "resip/stack/EventStackThread.hxx"
#include "resip/stack/SipStack.hxx"
#include "resip/stack/SipMessage.hxx"
#include "resip/stack/eSipHandler.hxx"
#include "rutil/TransportType.hxx"
#include "rutil/Data.hxx"
#include "rutil/Timer.hxx"

namespace eDum
{

struct ResipStackConfig
{
	unsigned int m_unStackNumber;
	unsigned int m_unSocketRcvBuffSize;
	unsigned int m_unSocketSndBuffSize;

	resip::CustomOptions m_stResipCustomOption;

	ResipStackConfig()
		: m_unStackNumber(1), m_unSocketRcvBuffSize(0), m_unSocketSndBuffSize(0)
	{
	}
};
typedef struct ResipStackConfig ResipStackConfig_t;

class ResipStack
{
	public :
		enum
		{
			E_CONST_INVALID_STACK = -1,
		};
		
		inline int getStackId() const{ return m_nStackId; }
		void registerTu(resip::TransactionUser & _rclsTu);
		void unregisterTu(resip::TransactionUser & _rclsTu);
		bool addTransport4Slb(const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		void delTransport4Slb(const eSipUtil::Net5Tuple_t & _rstNet5Tuple);
		bool addTransport(resip::TransportType _eTransportType,
												int _nLocalPort,
												resip::IpVersion _eIpVersion,
												resip::StunSetting _eStun,
												const resip::Data & _rclsIpAddress);
		/*====================================
		send SIP message to network via stack.
		_rclsSipMsg is copy within this function. Therefore caller must be deleted _rclsSipMsg.

		1. argument : _rclsSipMsg(sending SIP), _pclsTu(owning TU)
		====================================*/
		inline bool sendToWire(const resip::SipMessage & _rclsSipMsg, resip::TransactionUser * _pclsTu, unsigned int & _runCurrentSize, unsigned int & _runMaxSize)
		{
				return m_pStack->send(_rclsSipMsg, _pclsTu, &_runCurrentSize, &_runMaxSize);
		}
		inline bool sendToWire(const resip::SipMessage & _rclsSipMsg, const resip::Tuple & _rclsTargetTuple, resip::TransactionUser * _pclsTu, unsigned int & _runCurrentSize, unsigned int & _runMaxSize)
		{
				return m_pStack->sendTo(_rclsSipMsg, _rclsTargetTuple, _pclsTu, &_runCurrentSize, &_runMaxSize);
		}
		/*====================================
		send SIP message to network via stack.
		_pclsSipMsg must be allocated dynamically. It's memory is released by stack. Therefore caller is not responsible for deleting _pclsSipMsg.
		Note that after invoke this function, caller must not use _pclsSipMsg. 
		Because thread of transactionController treat _pclsSipMsg, _pclsSipMsg can be treated several thread.

		1. argument : _rclsSipMsg(sending SIP), _pclsTu(owning TU)
		====================================*/
		inline bool sendToWire(resip::SipMessage * _pclsSipMsg, resip::TransactionUser * _pclsTu, unsigned int & _runCurrentSize, unsigned int & _runMaxSize)
		{
			std::auto_ptr< resip::SipMessage > autoPtrSipMsg(_pclsSipMsg);
			return m_pStack->send(autoPtrSipMsg, _pclsTu, &_runCurrentSize, &_runMaxSize);
		}
		inline bool sendToWire(resip::SipMessage * _pclsSipMsg, const resip::Tuple & _rclsTargetTuple, resip::TransactionUser * _pclsTu, unsigned int & _runCurrentSize, unsigned int & _runMaxSize)
		{
			std::auto_ptr< resip::SipMessage > autoPtrSipMsg(_pclsSipMsg);
			return m_pStack->sendTo(autoPtrSipMsg, _rclsTargetTuple, _pclsTu, &_runCurrentSize, &_runMaxSize);
		}
		/*====================================
		send SIP message to network via stack.
		_rclsSipMsg must be allocated dynamically. It's memory is released by stack. Therefore caller is not responsible for deleting _rclsSipMsg.
		Note that after invoke this function, caller must not use _pclsSipMsg. 
		Because thread of transactionController treat _rclsSipMsg, _rclsSipMsg can be treated several thread.

		1. argument : _rclsSipMsg(sending SIP), _pclsTu(owning TU)
		====================================*/
		inline bool sendToWire(std::auto_ptr< resip::SipMessage > & _rclsSipMsg, resip::TransactionUser * _pclsTu, unsigned int & _runCurrentSize, unsigned int & _runMaxSize)
		{
			return m_pStack->send(_rclsSipMsg, _pclsTu, &_runCurrentSize, &_runMaxSize);
		}
		/*==== Callback ====*/
		void setDelTransport4SlbCb(resip::PfuncDelTransport4Slb _pfnCb);
		void setConnected4NonBlock4SlbCb(resip::PfuncConnected4NonBlock4Slb _pfnCb);
		void setRcvManageMsg4SlbCb(resip::PfuncRcvLbManageMsg4Slb _pfnCb);
		void setStatHandler(resip::ExternalStatsHandler * _pclsHandler);
		void setGenToTag4RspCallback(resip::PfunGenerateStackTag _pfnGenTagCb);		
		void setRcvTraceHandler(resip::PfuncRcvTraceCallback _pfnRcvTraceCb);
		void setSndTraceHandler(resip::PfuncSndTraceCallback _pfnSndTraceCb);
		void setCheckOverloadCallback(resip::PfuncCheckOverloadCallback _pfnCheckOverloadCb);
		void setCheckSipCallback(resip::PfuncCheckSipCallback _pfnCheckSipCb);
		void setCheckOverload4SlbCallback(resip::PfuncCheckOverload4SlbCallback _pfnCheckOverloadCb);
		void setCheckSip4SlbCallback(resip::PfuncCheckSip4SlbCallback _pfnCheckSipCb);
		void setFaultMsgCallback(resip::PfuncFaultMsgCallback _pfnFaultCb);
		void setTransactionBalanceCallback1(resip::PfuncTransctionBalanceCallback1_t _pfnTranscBalanceCb);
		void setTransactionBalanceCallback2(resip::PfuncTransctionBalanceCallback2_t _pfnTranscBalanceCb);
		void setHangMonAddCallback(resip::PfuncHangMonAdd _pfnHangMonAddCb);
		void setHangMonTouchCallback(resip::PfuncHangMonTouch _pfnHangMonTouchCb);
		void setQueueAlarmAddCallback(resip::PfuncQueueAlarmAdd _pfnQueueAlarmAddCb);
		void setQueueAlarmDropCallback(resip::PfuncQueueAlarmDrop _pfnQueueAlarmDropCb);
		void setStatMonCallback(resip::PfuncStatMon _pfnStatMonCb);

		/*==== etc ====*/
		void abandonServerTransaction(const resip::Data & _rclsTid, int _nTcId);
		void cancelClientInviteTransaction(const resip::Data & _rclsTid, int _nTcId);
		void setStackMonitorInterval(unsigned int _unInterval);
		void setStackDiscardStrayRsp(bool _bDiscard);
		bool receivedRetrans(const resip::SipMessage & _rclsSipMsg);
		resip::eSipHandler * getSipHandler();
		inline resip::TransportMgrWithSlb * getTransportMgr4Slb()
		{
			if( m_pStack )
				return m_pStack->getTransportMgr4Slb();
			else
				return NULL;
		}
		
	private :
		ResipStack();
		~ResipStack();

		void reset(const resip::CustomOptions & _rstResipStackCustomOption);
		void construct(const resip::CustomOptions & _rstResipStackCustomOption);
		void destroy();
		void start();
		void stop();
		inline void setStackId(int _nStackId){ m_nStackId = _nStackId; }
		
		int m_nStackId;
		resip::FdPollGrp * m_pStackPollGrp;
		resip::EventThreadInterruptor * m_pStackIntrrupt;
		resip::SipStack * m_pStack;
		resip::EventStackThread * m_pStackThread;

		friend class ResipStackMgr;

};

class ResipStackMgr
{
	public :	
		static ResipStackMgr & getInstance(const ResipStackConfig_t * _pstResipStackCfg = NULL, unsigned int _unStatMonInterval = 0);
		~ResipStackMgr();

		void start();
		void stop();
		void reset();
		/*====================================
		find stack

		1. return : found stack
		2. argument : _nStackId(finding stack-id. range is 0 ~ m_nStackNumber-1)
		====================================*/
		const ResipStack * findStack(int _nStackId) const;
		ResipStack * findStack(int _nStackId);
		unsigned int getStackNumber() const;
		inline const ResipStackConfig_t & getResipStackConfig() const{ return m_stResipStackCfg; }

	private :
		ResipStackMgr(const ResipStackConfig_t * _pstResipStackCfg, unsigned int _unStatMonInterval);

		ResipStack * m_arrResipStack;
		ResipStackConfig_t m_stResipStackCfg;
		unsigned int m_unStatMonIntervalMs;
	
};

#define D_STACK_MGR ResipStackMgr::getInstance()

}

#endif

