#ifndef APP_IF_H
#define APP_IF_H
#include "flexIf.hxx"
#include "string.hxx"
#include "rutil/Random.hxx"

#define TRSG_HEADER_SIZE 30
#define TRSG_HEADER_KSIP 22

#define DEF_UNKNOWN "UNKNOWN"

/* Body Type Define(String) */
#define DEF_BODY_ESTABLISH_SESSION_REQ "EstablishSessionRequest"
#define DEF_BODY_ESTABLISH_SESSION_RES "EstablishSessionResponse"
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

inline eSipUtil::KString g_fnMakeTrsgKey(unsigned int _unTid, eSipUtil::KString _clsJobID, eSipUtil::KString _clsSessionID)
{
   eSipUtil::KString clsKey;
   eSipUtil::KString clsTid(_unTid);
   eSipUtil::KString::m_fnStrnCat((eSipUtil::KSTR)clsKey,10240,"%s-%d-%s", (eSipUtil::KSTR)_clsSessionID, (eSipUtil::KUINT)clsTid, (eSipUtil::KSTR)_clsJobID);
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
	E_TC_RES_OK = 0,
	E_JOB_TRANSCODE_TIMEOUT,
	E_JOB_IS_LIMITED,
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
	E_TARGET_SIZE_INAVLID,
	E_NAS_ACCESS_FAILED,
	E_NO_SRC_NAS_CODE,
	E_NO_TARGET_NAS_CODE
}ETrssCodeSet_t;
inline const char * g_fnGetTrssCode(ETrssCodeSet_t _eSt)
{
	if(_eSt == E_TC_RES_OK) return "0000";
	else if(_eSt == E_JOB_TRANSCODE_TIMEOUT) return "1009";
	else if(_eSt == E_JOB_IS_LIMITED) return "1016";
	else if(_eSt == E_TRANSCODING_FAILED) return "3001";
	else if(_eSt == E_NOT_SUPPORT_AUDIO_DECODEC_TYPE) return "3002";
	else if(_eSt == E_NOT_SUPPORT_VIDEO_DECODEC_TYPE) return "3003";
	else if(_eSt == E_NOT_SUPPORT_AUDIO_ENCODEC_TYPE) return "3004";
	else if(_eSt == E_NOT_SUPPORT_VIDEO_ENCODEC_TYPE) return "3005";
	else if(_eSt == E_MEDIA_ENCODING_ERROR) return "3006";
	else if(_eSt == E_MEDIA_DECODING_ERROR) return "3007";
	else if(_eSt == E_AUDIO_DECODING_ERROR) return "3008";
	else if(_eSt == E_VIDEO_DECODING_ERROR) return "3009";
	else if(_eSt == E_CONTAINER_OPEN_FAILED) return "3010";
	else if(_eSt == E_METADATA_WRITE_FAILED) return "3011";
	else if(_eSt == E_LICENSE_KEY_INVALID) return "3012";
	else if(_eSt == E_PARAMETER_INVALID) return "3013";
	else if(_eSt == E_TARGET_SIZE_INAVLID) return "3016";
	else if(_eSt == E_NAS_ACCESS_FAILED) return "4001";
	else if(_eSt == E_NO_SRC_NAS_CODE) return "4004";
	else if(_eSt == E_NO_TARGET_NAS_CODE) return "4005";
	else return "9999";

}
inline const char * g_fnGetTrssCodeDesc(eSipUtil::CmpString _clsCmp)
{
   if(_clsCmp == "0000")      return "OK";
   else if(_clsCmp == "1009") return "JOB_TRANSCODE_TIMEOUT";
   else if(_clsCmp == "1016") return "JOB_IS_LIMITED";
   else if(_clsCmp == "3001") return "TRANSCODING_FAILED";		// 규격서에 없음
   else if(_clsCmp == "3002") return "NOT_SUPPORTED_AUDIO_DECODEC_TYPE";
   else if(_clsCmp == "3003") return "NOT_SUPPORTED_VIDEO_DECODEC_TYPE";
   else if(_clsCmp == "3004") return "NOT_SUPPORTED_AUDIO_ENCODEC_TYPE"; // target
   else if(_clsCmp == "3005") return "NOT_SUPPORTED_VIDEO_ENCODEC_TYPE";
   else if(_clsCmp == "3006") return "MEDIA_ENCODING_ERROR";
   else if(_clsCmp == "3007") return "MEDIA_DECODING_ERROR";
   else if(_clsCmp == "3008") return "ACODEC_DECODING_FAILED";
   else if(_clsCmp == "3009") return "VCODEC_DECODING_FAILED";
   else if(_clsCmp == "3010") return "CONTAINER_OPEN_FAILED";
   else if(_clsCmp == "3011") return "METADATA_WRITING_FAILED";
   else if(_clsCmp == "3012") return "LICENSE_KEY_INVALID";
   else if(_clsCmp == "3013") return "PARAMETER_INVALID";
   else if(_clsCmp == "3016") return "TARGET_SIZE_ERROR";
   else if(_clsCmp == "4001") return "NAS_ACCESS_FAILED";
   else if(_clsCmp == "4004") return "NO_SOURCE_NAS_CODE";
   else if(_clsCmp == "4005") return "NO_TARGET_NAS_CODE";
   else                       return "UNKNOWN";
}

#endif
