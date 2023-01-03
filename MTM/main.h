#ifndef _VWTRSS_H_
#define _VWTRSS_H_
#include "Trsplog.hxx"

#define MAIN_SPEC_1K 1024
#define MAIN_SPEC_1M 1048576
#define MAIN_SPEC_1G 1073741824

#define MAIN_PROCESS_STACK_SIZE    8*MAIN_SPEC_1M
#define MAIN_THREAD_STACK_SIZE    80*MAIN_SPEC_1K

typedef enum
{
	E_VWTRSS_LOG_CATE_UTIL = 0,
	E_VWTRSS_LOG_CATE_APP,
	E_VWTRSS_LOG_CATE_TRSG
} EVwtrssLogCate_t;
void g_fnKillMyself();
int g_fnGetLog();
bool g_fnCheckLog( eSipUtil::ELogLevel_t _eLogLevel);
bool g_fnCheckTrsgLog( eSipUtil::ELogLevel_t _eLogLevel);
void g_fnSetLog(EVwtrssLogCate_t _eCate,eSipUtil::ELogLevel_t _eLv);

#define IFLOG(_eT,_format,args...) Log_(g_fnGetLog(), E_VWTRSS_LOG_CATE_APP, _eT, _format, ##args)
#define DLOG() Log_(g_fnGetLog(), E_VWTRSS_LOG_CATE_APP, eSipUtil::E_LOG_ERR, "%s %u",__func__,__LINE__)
#define SGLOG(_eT,_format,args...) Log_(g_fnGetLog(), E_VWTRSS_LOG_CATE_TRSG, _eT, _format, ##args)
#endif

