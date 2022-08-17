
#ifndef COM_AS_ALARM_IF_H
#define COM_AS_ALARM_IF_H

#include "eSipAlarm.hxx"

namespace ComAs
{

class ComAsAlarmMgr
{
	public :
		static ComAsAlarmMgr & getInstance();
		~ComAsAlarmMgr();

		bool m_fnInit();
		void m_fnProcAsStatusAlarm(bool _bClear, const char * _pszNodeName, const char * _pszTuple);

		static void m_fnSendAlarm(const eSIP::AlarmSendData_t & _rstAlarmSndData);
		
	private :
		ComAsAlarmMgr();
		
		int m_nUdpFd;
};

}

#endif
