
#ifndef RESIP_ESIP_STACK_MONITOR_HXX
#define RESIP_ESIP_STACK_MONITOR_HXX

#ifdef ESIP_IN_RESIP

#include "rutil/Data.hxx"
#include "rutil/ThreadIf.hxx"
#include "rutil/TimeLimitFifo.hxx"

namespace resip
{

class SipStack;

class StackMonitorThread : public ThreadIf
{
	public :
		StackMonitorThread(SipStack & _rclsSipStack);
		~StackMonitorThread();

		virtual void thread();

		void process();
		void stop();
		void setInterval(unsigned int _unInterval);

	private :
		TimeLimitFifo<int> m_clsFifo;
		SipStack & m_rclsSipStack;
		unsigned int m_unInterval;		//milisecond

};

}

#endif

#endif

