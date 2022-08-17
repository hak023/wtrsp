
#ifndef COM_AS_TRACE_IF_HXX
#define COM_AS_TRACE_IF_HXX

#include "eSipTrace.hxx"

namespace ComAs
{

class ComAsTraceMgr
{
	public :
		static ComAsTraceMgr & getInstance();
		~ComAsTraceMgr();

		bool m_fnInit();
		int m_fnGetUdpFd() const;

		static void m_fnSendTrace(const eSIP::TraceResult & _rstTraceResult);
		
	private :
		ComAsTraceMgr();
		
		int m_nUdpFd;
};

}

#endif

