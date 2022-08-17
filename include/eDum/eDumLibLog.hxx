
#ifndef DUMLIB_LOG_HXX
#define DUMLIB_LOG_HXX

#include "rutil/Logger.hxx"

namespace eDum
{

#define StackCoreLog(args_) StackAppLog(3, 0, args_)
#define DebugCoreLog(args_) DebugAppLog(3, 0, args_)
#define InfoCoreLog(args_) InfoAppLog(3, 0, args_)
#define WarningCoreLog(args_) WarningAppLog(3, 0, args_)
#define ErrCoreLog(args_) ErrAppLog(3, 0, args_)
#define CritCoreLog(args_) CritAppLog(3, 0, args_)

}

#endif

