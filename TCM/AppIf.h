#ifndef APP_IF_H
#define APP_IF_H
#include "flexIf.hxx"
#include "string.hxx"
#include "rutil/Random.hxx"

#define TRSE_HEADER_SIZE 30
#define TRSE_HEADER_KSIP 23

#define TRSS_HEADER_SIZE 30
#define TRSS_HEADER_KSIP 23

#define DEF_UNKNOWN "UNKNOWN"

/* Body Type Define(String) */
#define DEF_BODY_ESTABLISH_SESSION_REQ "EstablishSessionRequest"
#define DEF_BODY_ESTABLISH_SESSION_RES "EstablishSessionResponse"
#define DEF_BODY_CLOSE_SESSION_REQ "CloseSessionRequest"
#define DEF_BODY_CLOSE_SESSION_RES "CloseSessionResponse"
#define DEF_BODY_LINK_TEST_REQ "LinkTestRequest"
#define DEF_BODY_LINK_TEST_RES "LinkTestResponse"
#define DEF_BODY_CREATE_JOB_REQ "CreateJobRequest"
#define DEF_BODY_CREATE_JOB_RES "CreateJobResponse"
#define DEF_JOB_STATUS_CHANGED_NOTIFY "JobStatusChangedNotify"
#define DEF_JOB_STATUS_CHANGED_NOTIFY_CREATED "JobStatusChangedNotify_Created"
#define DEF_JOB_STATUS_CHANGED_NOTIFY_WAITING "JobStatusChangedNotify_Waiting"
#define DEF_JOB_STATUS_CHANGED_NOTIFY_JOB_STARTED "JobStatusChangedNotify_JobStarted"
#define DEF_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STARTED "JobStatusChangedNotify_TC_Started"
#define DEF_JOB_STATUS_CHANGED_NOTIFY_TRANSCODING_STOPPED "JobStatusChangedNotify_TC_Stopped"
#define DEF_JOB_STATUS_CHANGED_NOTIFY_JOB_STOPPED "JobStatusChangedNotify_JobStopped"
#define DEF_JOB_STATUS_CHANGED_NOTIFY_JOB_DESTROYED "JobStatusChangedNotify_Destroyed"

inline eSipUtil::KString g_fnMakeTrseKey(unsigned int _unTid, eSipUtil::KString _clsSessionID)
{
   eSipUtil::KString clsKey;
   eSipUtil::KString clsTid(_unTid);
   eSipUtil::KString::m_fnStrnCat((eSipUtil::KSTR)clsKey,clsKey.m_unLen,"%s-%d", (eSipUtil::KSTR)_clsSessionID, (eSipUtil::KUINT)clsTid);
   return clsKey;
}
inline eSipUtil::KString g_fnMakeTrssKey(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID)
{
   eSipUtil::KString clsKey;
   eSipUtil::KString clsTid(_unTid);
   eSipUtil::KString::m_fnStrnCat((eSipUtil::KSTR)clsKey,clsKey.m_unLen,"%s-%d-%s", (eSipUtil::KSTR)_clsSessionID, (eSipUtil::KUINT)clsTid, (eSipUtil::KSTR)_clsJobID);
   return clsKey;
}
inline eSipUtil::KString g_fnGetUUID()
{
	eSipUtil::KString uuid("{");

	uuid << (resip::Random::getCryptoRandomHex(4)).c_str(); // time-low
	uuid << "-";

	uuid << (resip::Random::getCryptoRandomHex(2)).c_str(); // time-mid
	uuid << "-";

	resip::Data time_hi_and_version = resip::Random::getCryptoRandom(2);
	time_hi_and_version[0] &= 0x0f;
	time_hi_and_version[0] |= 0x40;
	uuid << (time_hi_and_version.hex()).c_str();
	uuid << "-";

	resip::Data clock_seq_hi_and_reserved = resip::Random::getCryptoRandom(1);
	clock_seq_hi_and_reserved[0] &= 0x3f;
	clock_seq_hi_and_reserved[0] |= 0x40;
	uuid << (clock_seq_hi_and_reserved.hex()).c_str();
	uuid << (resip::Random::getCryptoRandomHex(1)).c_str(); // clock-seq-low
	uuid << "-";

	uuid << (resip::Random::getCryptoRandomHex(6)).c_str(); // node
	uuid << "}";
	return uuid;
}
typedef enum
{
    E_JOB_RES_OK = 0,
    E_TRSE_TCP_SESSION_CLOSE,
    E_TRSE_TCP_RESPONSE_FAILED,
    E_TRSS_TCP_REQUEST_FAILED,
    E_TRSS_SESSION_CLOSE,
    E_TRSE_MSG_UNCOMP_FAILED,
    E_IP_ACCESS_FAILED,
    E_ID_PASSWORD_FAILED,
    E_JOB_CREATED_TIMEOUT,
    E_JOB_TRANSCODE_TIMEOUT,
    E_TCP_BINARY_DECODING_FAILED,
    E_TCP_BINARY_ENCODING_FAILED,
    E_SOURCE_CONTENT_ERR,
    E_TARGET_CONTENT_ERR,
    E_TRANSCODING_ERR,
    E_XML_PARSE_ERR,
    E_JOB_IS_LIMITED,
    E_INTERNAL_NAS_ACCESS_ERR,
    E_TRANS_PROC_RUN_FAILED,
    E_TRANSCODING_FAILED,
    E_NOT_SUPPORT_AUDIO_DECODEC_TYPE,
    E_NOT_SUPPORT_VIDEO_DECODEC_TYPE,
    E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE,
    E_NOT_SUPPORT_VIDEO_ENCODEC_TYPE,
    E_MEDIA_ENCODING_ERROR,
    E_MEDIA_DECODING_ERROR,
    E_AUDIO_DECODING_ERROR,
    E_VIDEO_DECODING_ERROR,
    E_CONTAINER_OPEN_FAILED,
    E_METADATA_WRITE_FAILED,
    E_LICENSE_KEY_INVALID,
    E_PARAMETER_INVALID,
    E_TARGET_SIZE_ERROR,
    E_NAS_ACCESS_FAILED,
    E_SRC_FILE_COPY_ERROR,
    E_TARGET_FILE_COPY_ERROR,
    E_NO_SRC_NAS_CODE,
    E_NO_TARGET_NAS_CODE
}ETrsgCodeSet_t;

inline const char * g_fnGetCdrCode(ETrsgCodeSet_t _eSt)
{
    if(_eSt == E_JOB_RES_OK)                          return "20000000";
    else if(_eSt == E_TRSE_TCP_SESSION_CLOSE)         return "30001001";
    else if(_eSt == E_TRSE_TCP_RESPONSE_FAILED)       return "30001002";
    else if(_eSt == E_TRSS_TCP_REQUEST_FAILED)        return "30001003";
    else if(_eSt == E_TRSS_SESSION_CLOSE)             return "30001004";
    else if(_eSt == E_TRSE_MSG_UNCOMP_FAILED)         return "30001005";
    else if(_eSt == E_IP_ACCESS_FAILED)               return "30001006";
    else if(_eSt == E_ID_PASSWORD_FAILED)             return "30001007";
    else if(_eSt == E_JOB_CREATED_TIMEOUT)            return "30001008";
    else if(_eSt == E_JOB_TRANSCODE_TIMEOUT)          return "30001009";
    else if(_eSt == E_TCP_BINARY_DECODING_FAILED)     return "30001010";
    else if(_eSt == E_TCP_BINARY_ENCODING_FAILED)     return "30001011";
    else if(_eSt == E_SOURCE_CONTENT_ERR)             return "30001012";
    else if(_eSt == E_TARGET_CONTENT_ERR)             return "30001013";
    else if(_eSt == E_TRANSCODING_ERR)                return "30001014";
    else if(_eSt == E_XML_PARSE_ERR)                  return "30001015";
    else if(_eSt == E_JOB_IS_LIMITED)                 return "30001016";
    else if(_eSt == E_INTERNAL_NAS_ACCESS_ERR)        return "30001017";
    else if(_eSt == E_TRANS_PROC_RUN_FAILED)          return "30002008";
    else if(_eSt == E_TRANSCODING_FAILED)             return "40003001";//한줄로그 규격서에 없음. 추가요청 필요함.
    else if(_eSt == E_NOT_SUPPORT_AUDIO_DECODEC_TYPE) return "40003002";
    else if(_eSt == E_NOT_SUPPORT_VIDEO_DECODEC_TYPE) return "40003003";
    else if(_eSt == E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE) return "40003004";
    else if(_eSt == E_NOT_SUPPORT_VIDEO_ENCODEC_TYPE) return "40003005";
    else if(_eSt == E_MEDIA_ENCODING_ERROR)           return "40003006";
    else if(_eSt == E_MEDIA_DECODING_ERROR)           return "40003007";
    else if(_eSt == E_AUDIO_DECODING_ERROR)           return "40003008";
    else if(_eSt == E_VIDEO_DECODING_ERROR)           return "40003009";
    else if(_eSt == E_CONTAINER_OPEN_FAILED)          return "40003010";
    else if(_eSt == E_METADATA_WRITE_FAILED)          return "40003011";
    else if(_eSt == E_LICENSE_KEY_INVALID)            return "40003012";
    else if(_eSt == E_PARAMETER_INVALID)              return "40003013";
    else if(_eSt == E_TARGET_SIZE_ERROR)              return "40003016";
    else if(_eSt == E_NAS_ACCESS_FAILED)              return "40004001";
    else if(_eSt == E_SRC_FILE_COPY_ERROR)            return "40004002";
    else if(_eSt == E_TARGET_FILE_COPY_ERROR)         return "40004003";
    else if(_eSt == E_NO_SRC_NAS_CODE)                return "40004004";
    else if(_eSt == E_NO_TARGET_NAS_CODE)             return "40004005";
    else                                              return "40003001";//default 는 t/c fail 로 취급.
}
inline const char * g_fnGetTrsgCode(ETrsgCodeSet_t _eSt)
{
   if(_eSt == E_JOB_RES_OK)                          return "0000";
   else if(_eSt == E_TRSE_TCP_SESSION_CLOSE)         return "1001";
   else if(_eSt == E_TRSE_TCP_RESPONSE_FAILED)       return "1002";
   else if(_eSt == E_TRSS_TCP_REQUEST_FAILED)        return "1003";
   else if(_eSt == E_TRSS_SESSION_CLOSE)             return "1004";
   else if(_eSt == E_TRSE_MSG_UNCOMP_FAILED)         return "1005";
   else if(_eSt == E_IP_ACCESS_FAILED)               return "1006";
   else if(_eSt == E_ID_PASSWORD_FAILED)             return "1007";
   else if(_eSt == E_JOB_CREATED_TIMEOUT)            return "1008";
   else if(_eSt == E_JOB_TRANSCODE_TIMEOUT)          return "1009";
   else if(_eSt == E_TCP_BINARY_DECODING_FAILED)     return "1010";
   else if(_eSt == E_TCP_BINARY_ENCODING_FAILED)     return "1011";
   else if(_eSt == E_SOURCE_CONTENT_ERR)             return "1012";
   else if(_eSt == E_TARGET_CONTENT_ERR)             return "1013";
   else if(_eSt == E_TRANSCODING_ERR)                return "1014";
   else if(_eSt == E_XML_PARSE_ERR)                  return "1015";
   else if(_eSt == E_JOB_IS_LIMITED)                 return "1016";
   else if(_eSt == E_INTERNAL_NAS_ACCESS_ERR)        return "1017";
   else if(_eSt == E_TRANS_PROC_RUN_FAILED)          return "2008";
   else if(_eSt == E_TRANSCODING_FAILED)             return "3001";//한줄로그 규격서에 없음. 추가요청 필요함.
   else if(_eSt == E_NOT_SUPPORT_AUDIO_DECODEC_TYPE) return "3002";
   else if(_eSt == E_NOT_SUPPORT_VIDEO_DECODEC_TYPE) return "3003";
   else if(_eSt == E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE) return "3004";
   else if(_eSt == E_NOT_SUPPORT_VIDEO_ENCODEC_TYPE) return "3005";
   else if(_eSt == E_MEDIA_ENCODING_ERROR)           return "3006";
   else if(_eSt == E_MEDIA_DECODING_ERROR)           return "3007";
   else if(_eSt == E_AUDIO_DECODING_ERROR)           return "3008";
   else if(_eSt == E_VIDEO_DECODING_ERROR)           return "3009";
   else if(_eSt == E_CONTAINER_OPEN_FAILED)          return "3010";
   else if(_eSt == E_METADATA_WRITE_FAILED)          return "3011";
   else if(_eSt == E_LICENSE_KEY_INVALID)            return "3012";
   else if(_eSt == E_PARAMETER_INVALID)              return "3013";
   else if(_eSt == E_TARGET_SIZE_ERROR)              return "3016";
   else if(_eSt == E_NAS_ACCESS_FAILED)              return "4001";
   else if(_eSt == E_SRC_FILE_COPY_ERROR)            return "4002";
   else if(_eSt == E_TARGET_FILE_COPY_ERROR)         return "4003";
   else if(_eSt == E_NO_SRC_NAS_CODE)                return "4004";
   else if(_eSt == E_NO_TARGET_NAS_CODE)             return "4005";
   else                                              return "3001";//default 는 t/c fail 로 취급.
}
inline const int g_fnGetEnumIdx(eSipUtil::CmpString _clsCmp)
{
    if(_clsCmp == "0000") return E_JOB_RES_OK;
    else if(_clsCmp == "1001") return E_TRSE_TCP_SESSION_CLOSE;
    else if(_clsCmp == "1002") return E_TRSE_TCP_RESPONSE_FAILED;
    else if(_clsCmp == "1003") return E_TRSS_TCP_REQUEST_FAILED;
    else if(_clsCmp == "1004") return E_TRSS_SESSION_CLOSE;
    else if(_clsCmp == "1005") return E_TRSE_MSG_UNCOMP_FAILED;
    else if(_clsCmp == "1006") return E_IP_ACCESS_FAILED;
    else if(_clsCmp == "1007") return E_ID_PASSWORD_FAILED;
    else if(_clsCmp == "1008") return E_JOB_CREATED_TIMEOUT;
    else if(_clsCmp == "1009") return E_JOB_TRANSCODE_TIMEOUT;
    else if(_clsCmp == "1010") return E_TCP_BINARY_DECODING_FAILED;
    else if(_clsCmp == "1011") return E_TCP_BINARY_ENCODING_FAILED;
    else if(_clsCmp == "1012") return E_SOURCE_CONTENT_ERR;
    else if(_clsCmp == "1013") return E_TARGET_CONTENT_ERR;
    else if(_clsCmp == "1014") return E_TRANSCODING_ERR;
    else if(_clsCmp == "1015") return E_XML_PARSE_ERR;
    else if(_clsCmp == "1016") return E_JOB_IS_LIMITED;
    else if(_clsCmp == "1017") return E_INTERNAL_NAS_ACCESS_ERR;
    else if(_clsCmp == "2008") return E_TRANS_PROC_RUN_FAILED;
    else if(_clsCmp == "3001") return E_TRANSCODING_FAILED;
    else if(_clsCmp == "3002") return E_NOT_SUPPORT_AUDIO_DECODEC_TYPE;
    else if(_clsCmp == "3003") return E_NOT_SUPPORT_VIDEO_DECODEC_TYPE;
    else if(_clsCmp == "3004") return E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE;
    else if(_clsCmp == "3005") return E_NOT_SUPPORT_VIDEO_ENCODEC_TYPE;
    else if(_clsCmp == "3006") return E_MEDIA_ENCODING_ERROR;
    else if(_clsCmp == "3007") return E_MEDIA_DECODING_ERROR;
    else if(_clsCmp == "3008") return E_AUDIO_DECODING_ERROR;
    else if(_clsCmp == "3009") return E_VIDEO_DECODING_ERROR;
    else if(_clsCmp == "3010") return E_CONTAINER_OPEN_FAILED;
    else if(_clsCmp == "3011") return E_METADATA_WRITE_FAILED;
    else if(_clsCmp == "3012") return E_LICENSE_KEY_INVALID;
    else if(_clsCmp == "3013") return E_PARAMETER_INVALID;
    else if(_clsCmp == "3016") return E_TARGET_SIZE_ERROR;
    else if(_clsCmp == "4001") return E_NAS_ACCESS_FAILED;
    else if(_clsCmp == "4002") return E_SRC_FILE_COPY_ERROR;
    else if(_clsCmp == "4003") return E_TARGET_FILE_COPY_ERROR;
    else if(_clsCmp == "4004") return E_NO_SRC_NAS_CODE;
    else if(_clsCmp == "4005") return E_NO_TARGET_NAS_CODE;
    else return E_TRANSCODING_FAILED;//default 는 t/c fail 로 취급.
}
inline const char * g_fnGetTrsgCodeDesc(eSipUtil::CmpString _clsCmp)
{
    if(_clsCmp == "0000")      return "OK";
    else if(_clsCmp == "1001") return "TRSE_TCP_SESSION_CLOSE";
    else if(_clsCmp == "1002") return "TRSE_TCP_RESPONSE_FAILED";
    else if(_clsCmp == "1003") return "TRSS_TCP_REQUEST_FAILED";
    else if(_clsCmp == "1004") return "TRSS_TCP_SESSION_CLOSE";
    else if(_clsCmp == "1005") return "TRSE_MSG_UNCOMP_FAILED";
    else if(_clsCmp == "1006") return "IP_ACCESS_FAILED";
    else if(_clsCmp == "1007") return "ID_PASSWORD_FAILED";
    else if(_clsCmp == "1008") return "JOB_CREATED_TIMEOUT";
    else if(_clsCmp == "1009") return "JOB_TRANSCODE_TIMEOUT";
    else if(_clsCmp == "1010") return "TCP_BINARY_DECODING_FAILED";
    else if(_clsCmp == "1011") return "TCP_BINARY_ENCODING_FAILED";
    else if(_clsCmp == "1012") return "SOURCECONTENT_TAG_DOES_NOT_EXIST";
    else if(_clsCmp == "1013") return "TARGETCONTENT_TAG_DOES_NOT_EXIST";
    else if(_clsCmp == "1014") return "TRANSCODING_TAG_DOES_NOT_EXIST";
    else if(_clsCmp == "1015") return "XML_PARSE_ERR";
    else if(_clsCmp == "1016") return "JOB_IS_LIMITED";
    else if(_clsCmp == "1017") return "INTERNAL_NAS_ACCESS_ERR";
    else if(_clsCmp == "2008") return "TRANS_PROC_RUN_FAILED";
    else if(_clsCmp == "3001") return "TRANSCODING_FAILED";    // 규격서에 없음
    else if(_clsCmp == "3002") return "NOT_SUPPORTED_AUDIO_DECODEC_TYPE";
    else if(_clsCmp == "3003") return "NOT_SUPPORTED_VIDEO_DECODEC_TYPE";
    else if(_clsCmp == "3004") return "NOT_SUPPORTED_AUDIO_ENCODEC_TYPE";
    else if(_clsCmp == "3005") return "NOT_SUPPORTED_VIDEO_ENCODEC_TYPE";
    else if(_clsCmp == "3006") return "ENCODING_ERROR_WRONG_OUTPUT";
    else if(_clsCmp == "3007") return "DECODING_ERROR_WRONG_INPUT";
    else if(_clsCmp == "3008") return "ACODEC_DECODING_FAILED";
    else if(_clsCmp == "3009") return "VCODEC_DECODING_FAILED";
    else if(_clsCmp == "3010") return "CONTAINER_OPEN_FAILED";
    else if(_clsCmp == "3011") return "METADATA_WRITING_FAILED";
    else if(_clsCmp == "3012") return "LICENSE_KEY_INVALID";
    else if(_clsCmp == "3013") return "PARAMETER_INVALID";
    else if(_clsCmp == "3016") return "TARGET_SIZE_ERROR";
    else if(_clsCmp == "4001") return "NAS_ACCESS_FAILED";
    else if(_clsCmp == "4002") return "SOURCE_FILE_COPY_FAILED";
    else if(_clsCmp == "4003") return "OUTPUT_FILE_COPY_FAILED";
    else if(_clsCmp == "4004") return "NO_SOURCE_NAS_CODE";
    else if(_clsCmp == "4005") return "NO_TARGET_NAS_CODE";
    else                       return "TRANSCODING_FAILED";//default 는 t/c fail 로 취급.
}
#endif
