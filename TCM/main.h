#ifndef _VWTRSG_H_
#define _VWTRSG_H_
#include "log.hxx"

#define MAIN_SPEC_1K 1024
#define MAIN_SPEC_1M 1048576
#define MAIN_SPEC_1G 1073741824

#define MAIN_PROCESS_STACK_SIZE    8*MAIN_SPEC_1M
#define MAIN_THREAD_STACK_SIZE    80*MAIN_SPEC_1K

typedef enum
{
	E_VWTRSG_LOG_CATE_UTIL = 0,
	E_VWTRSG_LOG_CATE_APP,
	E_VWTRSG_LOG_CATE_TRSE,
	E_VWTRSG_LOG_CATE_TRSS
}EVwtrsgLogCate_t;
void g_fnKillMyself();
int g_fnGetLog();
bool g_fnCheckLog( eSipUtil::ELogLevel_t _eLogLevel);
bool g_fnCheckTrseLog( eSipUtil::ELogLevel_t _eLogLevel);
bool g_fnCheckTrssLog( eSipUtil::ELogLevel_t _eLogLevel);

void g_fnSetLog(EVwtrsgLogCate_t _eCate,eSipUtil::ELogLevel_t _eLv);

#define IFLOG(_eT,_format,args...) Log_(g_fnGetLog(), E_VWTRSG_LOG_CATE_APP, _eT, _format, ##args)
#define SELOG(_eT,_format,args...) Log_(g_fnGetLog(), E_VWTRSG_LOG_CATE_TRSE, _eT, _format, ##args)
#define SSLOG(_eT,_format,args...) Log_(g_fnGetLog(), E_VWTRSG_LOG_CATE_TRSS, _eT, _format, ##args)
#define DLOG() Log_(g_fnGetLog(), E_VWTRSG_LOG_CATE_APP, eSipUtil::E_LOG_ERR, "%s %u",__func__,__LINE__)
#endif

