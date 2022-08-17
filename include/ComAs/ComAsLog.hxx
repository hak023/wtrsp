
#ifndef COM_AS_LOG_HXX
#define COM_AS_LOG_HXX

#include "eSipLog.hxx"

namespace ComAs
{

#define StackAsLog(args_) StackAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_CORE), 1, args_)
#define DebugAsLog(args_) DebugAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_CORE), 1, args_)
#define InfoAsLog(args_) InfoAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_CORE), 1, args_)
#define WarningAsLog(args_) WarningAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_CORE), 1, args_)
#define ErrAsLog(args_) ErrAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_CORE), 1, args_)
#define CritAsLog(args_) CritAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_CORE), 1, args_)

#define StackMmiLog(args_) StackAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_MMI), 0, args_)
#define DebugMmiLog(args_) DebugAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_MMI), 0, args_)
#define InfoMmiLog(args_) InfoAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_MMI), 0, args_)
#define WarningMmiLog(args_) WarningAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_MMI), 0, args_)
#define ErrMmiLog(args_) ErrAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_MMI), 0, args_)
#define CritMmiLog(args_) CritAppLog(eSIP::LogMgr::m_fnGetLogInstanceId(eSIP::LogMgr::E_LOG_APP_MMI), 0, args_)

}

#endif

