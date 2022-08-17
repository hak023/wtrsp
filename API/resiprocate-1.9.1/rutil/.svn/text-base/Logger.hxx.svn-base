#ifndef RESIP_Logger_hxx
#define RESIP_Logger_hxx

#include "rutil/Log.hxx"
#include "rutil/Lock.hxx"
#include "rutil/DataStream.hxx"
#include "rutil/Data.hxx"
#include "rutil/Subsystem.hxx"

#ifdef ESIP_IN_RESIP
#include "rutil/Mutex.hxx"
#include "eSipUtil/log.hxx"
#endif

#ifdef WIN32
#include <windows.h>
#endif 


/**
   @file Defines a set of logging macros, one for each level of logging.

   <h2>Simple usage</h2>

   Each source file which uses logging facilities, must set RESIPROCATE_SUBSYSTEM
   preprocessor define to one of resip subsystems. This define will be used by
   logging macros to mark log entries, generated from this source file, with
   appropriate subsystem tag. For the list of available resip subsystems refer
   to static variables list in resip::Subsystem class. Note, that those standard
   resip::Subsystem variables are just the most commonly used ones. Nothing
   prevents you from creating your own resip::Subsystem instance and set
   RESIPROCATE_SUBSYSTEM to point to it. That custom resip::Subsystem variable
   can even be local to a file or private class member variable, you just must
   ensure that it exist in all places you use logging this file. Creating own
   resip::Subsystem will allow you to create own subsystem tag and set specific
   logging level for it.

   Once you have RESIPROCATE_SUBSYSTEM defined you can start logging with any
   of StackLog(), DebugLog(), InfoLog(), WarningLog(), ErrLog(), CritLog(). These
   preprocessor macros provide a convenient way to log your data. Look at this
   piece of code as an example:

<code>
#include Logger.hxx
#define RESIPROCATE_SUBSYSTEM resip::Subsystem::SIP
   ...
   DebugLog(<< "hi there " << mix << 4 << types);  // note leading << and no endl
</code>


   <h2>Initialization</h2>

   Logging may be used without (or prior to) initialization, in which case all
   log data will be printed right to console (to std::cout). Likely, you will
   want to use more advanced logging features like output to syslog or a file.
   In this case you need to call Log::initialize() with appropriate parameters.
   E.g., following example tells logger to write data to a file with the name
   "resip.log". It will write only entries with logging priority Log::Info or
   higher. Application name is taken from argv[0], and no external logger is
   specified (last parameter is NULL).

<code>
   Log::initialize(Log::File, Log::Info, argv[0], "resip.log", NULL);
</code>

   Refer to Log::Type for a list of possible logging types and to Log::Level
   for a list of available logging levels.


   <h2>External loggers</h2>

   Sometimes you may need to provide your own, application specific way of
   logging. E.g. you may want to output log data to a GUI window. This may be
   implemented using external loggers. To create an external logger you just
   need to inherit from ExternalLogger class and pass an object of your external
   logger to Log::initialize() as the last parameter.

   
   <h2Subsystem-specific logging level></h2>

   You may set logging level to output for a specific subsystem inside resip.
   The recommended way to do this is with <code>Log::setLevel(level, subsystem)</code>
   static function. If you set a concrete logging level for the subsystem, it
   will be used instead of all other logging level settings, like global logging
   level setting and thread local logger level setting. To set subsystem-specific
   logging level back to default logging level, call
   <code>Log::setLevel(Log::None, subsystem)</code>


   <h2>Thread local loggers</h2>

   If your application uses several threads for resip, e.g. uses separate thread
   for each resip instance, then you may want to split your log streams to be
   separate too. E.g. you may use different log files for different instances.
   In this case you need to use thread local loggers.

   First, you need to create a thread local logger with Log::localLoggerCreate()
   static function call with desired parameters (they're closely following
   Log::initialize() meaning). You will receive LocalLoggerId in response which
   you will use later to refer to created local logger. If you need to change
   local logger's parameters later, you should use Log::localLoggerReinitialize()
   static function. And when you're done with it, free it with Log::localLoggerRemove()
   static function. To actually use a created local logger, you need to call
   Log::setThreadLocalLogger() from the target thread context. If you need to
   remove thread local logger from a thread, just call
   <code>Log::setThreadLocalLogger(0)</code>

   Note, that thread local logger may be safely used from multiple threads.
   So if each of your resip instances have two threads, both of them can just
   share the same local logger - just pass its LocalLoggerId to them both.


   <h2>Still not sure?</h2>

   If you still can't get something, just look how it is used in existing code.
   One particular place to look into is rutil/test/testLogger.cxx which is
   a unittest for logging facility.
*/
#define DELIM 
// unconditionally output to cerr -- easily change back and forth
#define CerrLog(args_)                                                  \
	resip::Log::tags(resip::Log::StdErr, RESIPROCATE_SUBSYSTEM,           \
                   __FILE__, __LINE__, resipCerr) << ' ' << '|' << ' '  \
          args_ << std::endl;
static inline bool
genericLogCheckLevel(resip::Log::Level level, const resip::Subsystem& sub)
{
   return resip::Log::isLogging(level, sub);
}
#ifdef ESIP_IN_RESIP   // >>> cwkim change log guard
static inline bool
genericLogCheckLevel(resip::Log::Level level)
{
   return resip::Log::isLogging(level);
}
static inline bool
genericAppLogCheckLevel(resip::Log::AppType _eAppType, int _nSubId, resip::Log::Level level)
{
   return resip::Log::isAppLogging(_eAppType, _nSubId, level);
}
class NewLog : public resip::Log
{
	public:
		NewLog(){}
		~NewLog(){}
		static resip::Mutex & getAppMutex(AppType _eAppType){ return _arrAppMutex[_eAppType]; }
		static resip::Mutex & getMutex(){ return _mutex; }
		static void fnBuildGuard(unsigned int _unMax, char *_szDst,resip::Log::Level _eLevel)
		{
			memset(_szDst,0x00,_unMax);
			fnGuardTime(_unMax,_szDst);
			strcat(_szDst,fnStringLogGuard(_eLevel));
		}
		static unsigned int fnGuardTime(unsigned int _unMax,char *_szDst)
		{
			struct timeval tval;
			struct tm tmTemp;
			gettimeofday(&tval, NULL);
			time_t m_time = (time_t)tval.tv_sec;
			long m_usec = tval.tv_usec;
			int m_msec = (int)((m_usec + 500) / 1000);
			_szDst[0] = '[';
			if (localtime_r(&m_time, &tmTemp) == NULL) _szDst[1] = '\0';
			if (strftime(&_szDst[1], _unMax, "%T", &tmTemp) == 0) _szDst[1] = '\0';
			int n = strlen(_szDst);
			if (n > 0)
			sprintf(_szDst + n, ".%03d", m_msec);
			n = strlen(_szDst);
			_szDst[n] = ']'; n++;
			return (unsigned int)n;
		} 
		static const char * fnStringLogGuard(resip::Log::Level _eLog)
		{
			if(_eLog == resip::Log::Crit) return "[CRIT   ][SIP] ";
			else if(_eLog == resip::Log::Err) return "[ERR    ][SIP] ";
			else if(_eLog == resip::Log::Warning) return "[WARNING][SIP] ";
			else if(_eLog == resip::Log::Info) return   "[INFO   ][SIP] ";
			else if(_eLog == resip::Log::Debug) return "[DEBUG  ][SIP] ";
			else if(_eLog == resip::Log::Stack) return "[STACK  ][SIP] ";
			else if(_eLog == resip::Log::StdErr) return "[STDERR ][SIP] ";
			else if(_eLog == resip::Log::Bogus) return "[BOGUS  ][SIP] ";
			else return "[NONE   ][SIP] ";
		}
};
inline eSipUtil::ELogLevel_t g_fnStackConvLevel(resip::Log::Level _eLv)
{
	switch(_eLv)
	{
		case resip::Log::None: return eSipUtil::E_LOG_DISABLE;
		case resip::Log::Crit: return eSipUtil::E_LOG_ERR;
		case resip::Log::Err: return eSipUtil::E_LOG_ERR;
		case resip::Log::Warning: return eSipUtil::E_LOG_WARN;
		case resip::Log::Info: return eSipUtil::E_LOG_INFO;
		case resip::Log::Debug: return eSipUtil::E_LOG_DEBUG;
		case resip::Log::Stack: return eSipUtil::E_LOG_STACK;
		case resip::Log::StdErr: return eSipUtil::E_LOG_ERR;
		case resip::Log::Bogus: return eSipUtil::E_LOG_ERR;
		default: return eSipUtil::E_LOG_DEBUG;
	}
}
#define LogStream(_eLevel,args_)\
do\
{\
if(genericLogCheckLevel(_eLevel))\
{\
resip::Lock clsLock(NewLog::getMutex());\
std::ostream& clsStream =  resip::Log::Instance(2);\
char szLog[256];NewLog::fnBuildGuard(256,szLog,_eLevel);\
clsStream <<szLog;\
clsStream args_;\
clsStream<<std::endl;\
}\
}while(false)
#if 0
#define LogAppStream(appType, subId, _eLevel, args_)\
do\
{\
if(genericAppLogCheckLevel(appType, subId, _eLevel))\
{\
resip::Lock clsLock(NewLog::getAppMutex(appType));\
std::ostream& clsStream =  resip::Log::appInstance(appType, 2);\
char szLog[256];NewLog::fnBuildGuard(256,szLog,_eLevel);\
clsStream <<szLog;\
clsStream args_;\
clsStream<<std::endl;\
}\
}while(false)
#else
#define LogAppStream(appType, subId, _eLevel, args_)\
do\
{\
eSipUtil::ELogLevel_t eCvLv = g_fnStackConvLevel(_eLevel);\
if( eSipUtil::AsyncFileLog::m_fnCheckLogLevel(appType, subId, eCvLv) )\
{\
resip::Data clsStr(10240, resip::Data::Preallocate);\
resip::DataStream clsStm(clsStr); clsStm args_; \
clsStm.flush();\
char * p = clsStr.m_fnAutoPtr();\
eSipUtil::AsyncFileLog::m_fnLog(appType, subId, eCvLv, p);\
}\
}while(false)
#endif
#if 0
#define StackLog(args_) LogStream(resip::Log::Stack, args_)
#define DebugLog(args_) LogStream(resip::Log::Debug, args_)
#define InfoLog(args_) LogStream(resip::Log::Info, args_)
#define WarningLog(args_) LogStream(resip::Log::Warning, args_)
#define ErrLog(args_) LogStream(resip::Log::Err, args_)
#define CritLog(args_) LogStream(resip::Log::Crit, args_)
#else
#define StackLog(args_) LogAppStream(2,0,resip::Log::Stack, args_)
#define DebugLog(args_) LogAppStream(2,0,resip::Log::Debug, args_)
#define InfoLog(args_) LogAppStream(2,0,resip::Log::Info, args_)
#define WarningLog(args_) LogAppStream(2,0,resip::Log::Warning, args_)
#define ErrLog(args_) LogAppStream(2,0,resip::Log::Err, args_)
#define CritLog(args_) LogAppStream(2,0,resip::Log::Crit, args_)

#define DumpLog(args_) LogAppStream(1,0,resip::Log::Err, args_)
#endif
#define StackAppLog(appType, subId, args_) LogAppStream(appType, subId, resip::Log::Stack, args_)
#define DebugAppLog(appType, subId, args_) LogAppStream(appType, subId, resip::Log::Debug, args_)
#define InfoAppLog(appType, subId, args_) LogAppStream(appType, subId, resip::Log::Info, args_)
#define WarningAppLog(appType, subId, args_) LogAppStream(appType, subId, resip::Log::Warning, args_)
#define ErrAppLog(appType, subId, args_) LogAppStream(appType, subId, resip::Log::Err, args_)
#define CritAppLog(appType, subId, args_) LogAppStream(appType, subId, resip::Log::Crit, args_)
#else
// do/while allows a {} block in an expression
#define GenericLog(system_, level_, args_)                              \
   do                                                                   \
   {                                                                    \
      if (genericLogCheckLevel(level_, system_))                        \
      {                                                                 \
         resip::Log::Guard _resip_log_guard(level_, system_, __FILE__, __LINE__); \
         _resip_log_guard.asStream()  args_;                            \
      }                                                                 \
   } while (false)

#define StackLog(args_)                                                         \
GenericLog(RESIPROCATE_SUBSYSTEM, resip::Log::Stack, args_)

#define DebugLog(args_) \
GenericLog(RESIPROCATE_SUBSYSTEM, resip::Log::Debug, args_)

#define InfoLog(args_) \
GenericLog(RESIPROCATE_SUBSYSTEM, resip::Log::Info, args_)

#define WarningLog(args_) \
GenericLog(RESIPROCATE_SUBSYSTEM, resip::Log::Warning, args_)

#define ErrLog(args_) \
GenericLog(RESIPROCATE_SUBSYSTEM, resip::Log::Err, args_)

#define CritLog(args_) \
GenericLog(RESIPROCATE_SUBSYSTEM, resip::Log::Crit, args_)
#endif

#ifdef NO_DEBUG
// Suppress debug logging at compile time
#undef DebugLog
#define DebugLog(args_)
#undef StackLog 
#define StackLog(args_)
#endif

namespace resip
{
/// DEPRECATED! Left for backward compatibility.
typedef Log GenericLogImpl;
}

#endif

/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */
