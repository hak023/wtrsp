#ifndef __AMP__MT_COMMON_TYPE_DEF_H__
#define __AMP__MT_COMMON_TYPE_DEF_H__

#include <stdio.h>
#include <string.h>
#include "amp_common.h"

#if defined(_WIN32)
#ifndef strcasecmp
    #define strcasecmp(x,y) stricmp((x),(y))
#endif
#ifndef strncasecmp
    #define strncasecmp(x,y,z) strnicmp((x),(y),(z))
#endif
#else
    #define stricmp(x,y) strcasecmp((x),(y))
    #define strnicmp(x,y,z) strncasecmp((x),(y),(z))
#endif
 


namespace AMP 
{

typedef enum {
   CTI_NONE    = -1,
   CTI_UNKNOWN = 0,
   CTI_H263     ,
   CTI_PCM      ,
   CTI_G7231    ,
   CTI_AMR_IF2  ,
   CTI_AMR_RFC  , //RFC44
   CTI_AMR_RFC53,
   CTI_AMR_WB   , //wspark
   CTI_G726_40  ,
   CTI_G726_32  ,
   CTI_G726_24  ,
   CTI_G726_16  ,
   CTI_G711A    ,
   CTI_G711U    ,
   CTI_G729I    ,
   CTI_G729A    ,
   CTI_EVRC     ,
   CTI_QCELP13K ,
   CTI_MP3      ,
   CTI_AAC      ,
   CTI_MPEG4AAC ,
   CTI_QDM2     ,
   CTI_AC3      ,
   CTI_WMA1     ,
   CTI_WMA2     ,
   CTI_WMA2_PRO ,
   CTI_WMA2_LL  ,
   CTI_WMA2_SP  ,
   CTI_MPEG1    ,
   CTI_MPEG2    ,
   CTI_MPEG4    ,
   CTI_H261     ,
   CTI_H264     ,
   CTI_WMV1     ,
   CTI_WMV2     ,
   CTI_WMV3     ,
   CTI_MSMPEG4V1,
   CTI_MSMPEG4V2,
   CTI_MSMPEG4V3,
   CTI_FLV1     ,
   CTI_FLASHSV  ,
   CTI_FLV_VP6  ,
   CTI_SVQ1     ,
   CTI_SVQ3     ,
   CTI_HEAAC    ,
   CTI_JPEG     ,
   CTI_BMP      ,
   CTI_YV12     ,
   CTI_YUV420   ,
   CTI_YUY2     ,
   CTI_RGB24    ,
   CTI_RGB32    ,
   CTI_NOVOICE  ,
   CTI_MAX
} CODEC_TYPE;

inline const char* GetCodecTypeName(CODEC_TYPE t) {
   switch(t) {
   case CTI_NONE          : return "NONE"       ;
   case CTI_UNKNOWN       : return "UNKNOWN"    ;
	case CTI_H263          : return "H263"       ;
	case CTI_PCM           : return "PCM"        ;
	case CTI_G7231         : return "G7231"      ;
	case CTI_AMR_IF2       : return "AMR_IF2"    ;
	case CTI_AMR_RFC       : return "AMR_RFC"    ;
	case CTI_AMR_RFC53     : return "AMR_RFC53"  ;
	case CTI_AMR_WB		  : return "AMR_WB"		;
	case CTI_G726_40       : return "G726_40"    ;
	case CTI_G726_32       : return "G726_32"    ;
	case CTI_G726_24       : return "G726_24"    ;
	case CTI_G726_16       : return "G726_16"    ;
	case CTI_G711A         : return "G711A"      ;
	case CTI_G711U         : return "G711U"      ;
	case CTI_G729I         : return "G729I"      ;
	case CTI_G729A         : return "G729A"      ;
	case CTI_EVRC          : return "EVRC"       ;
	case CTI_QCELP13K      : return "QCP13K"     ;
	case CTI_MP3           : return "MP3"        ;
	case CTI_AAC           : return "AAC"        ;
	case CTI_HEAAC         : return "HEAAC"      ;
   case CTI_MPEG4AAC      : return "MPEG4AAC"   ;
   case CTI_QDM2          : return "QDM2"       ;
	case CTI_AC3           : return "AC3"        ;
	case CTI_WMA1          : return "WMA1"       ;
	case CTI_WMA2          : return "WMA2"       ;
	case CTI_WMA2_PRO      : return "WMA2_PRO"   ;
	case CTI_WMA2_LL       : return "WMA2_LL"    ;
	case CTI_WMA2_SP       : return "WMA2_SP"    ;
	case CTI_MPEG1         : return "MPEG1"      ;
	case CTI_MPEG2         : return "MPEG2"      ;
	case CTI_MPEG4         : return "MPEG4"      ;
	case CTI_H261          : return "H261"       ;
	case CTI_H264          : return "H264"       ;
	case CTI_WMV1          : return "WMV1"       ;
	case CTI_WMV2          : return "WMV2"       ;
	case CTI_WMV3          : return "WMV3"       ;
   case CTI_MSMPEG4V1     : return "MSMPEG4V1"  ;
   case CTI_MSMPEG4V2     : return "MSMPEG4V2"  ;
   case CTI_MSMPEG4V3     : return "MSMPEG4V3"  ;
   case CTI_FLV1          : return "FLV1"       ;
   case CTI_FLASHSV       : return "FLASHSV"    ;
   case CTI_FLV_VP6       : return "FLV_VP6"    ;
   case CTI_SVQ1          : return "SVQ1"       ;
   case CTI_SVQ3          : return "SVQ3"       ;
   case CTI_JPEG          : return "JPEG"       ;
   case CTI_BMP           : return "BMP"        ;
   case CTI_YV12          : return "YV12"       ;
   case CTI_YUV420        : return "YUV420"     ;
   case CTI_YUY2          : return "YUY2"       ;
   case CTI_RGB24         : return "RGB24"      ;
   case CTI_RGB32         : return "RGB32"      ;
   case CTI_NOVOICE       : return "NOVOICE"    ;
	default                : return "INVALID"    ;
	};
}

inline CODEC_TYPE GetCodecTypeFromStr(const char* sCodec) {
   for(int i=CTI_NONE;i < CTI_MAX; i++) {
      if(strncmp(GetCodecTypeName((CODEC_TYPE)i), sCodec,
         strlen(GetCodecTypeName((CODEC_TYPE)i))) == 0) {
         return (CODEC_TYPE)i;
      }
   }
   return CTI_UNKNOWN;
}


inline bool IsVoiceCodec(CODEC_TYPE t) {
   switch(t) {
	   case CTI_G7231    :
	   case CTI_AMR_IF2  :
	   case CTI_AMR_RFC  :
	   case CTI_AMR_RFC53:
	   case CTI_AMR_WB	: // 2011.03.09 AMRWB , wspark
	   case CTI_G726_40  :
	   case CTI_G726_32  :
	   case CTI_G726_24  :
	   case CTI_G726_16  :
	   case CTI_G711A    :
	   case CTI_G711U    :
	   case CTI_G729I    :
	   case CTI_G729A    :
	   case CTI_EVRC     :
	   case CTI_QCELP13K :
	   case CTI_NOVOICE  : return true;
      default           : return false;
   }
   return false;
}


typedef	enum  {
   HAVCT_UNKNOWN     = 0,
   HAVCT_AVI         = 0x0010,
   HAVCT_3GP         = 0x0020,
   HAVCT_MP4         = 0x0021, // 2013.06.12 add by lyunix 
   HAVCT_ASF         = 0x0030,
   HAVCT_FLV         = 0x0040,
   HAVCT_MPEGPS      = 0x0050,
   HAVCT_JPEG        = 0x0060,
   HAVCT_BMP         = 0x0070,

   HAVCT_WAV         = 0x0110,
   HAVCT_MP3         = 0x0120,
   HAVCT_AAC         = 0x0131,
   HAVCT_HEAAC       = 0x0132,
   HAVCT_AC3         = 0x0140,

   //HAVCT_PCM .. ADPCM ... RAW
   HAVCT_G711A       = 0x0211,
   HAVCT_G711U       = 0x0212,
   HAVCT_PCM16       = 0x0213,
   HAVCT_VOX         = 0x0214,
   HAVCT_G7231_53    = 0x0221,
   HAVCT_G7231_63    = 0x0222,
   HAVCT_G729A       = 0x0231,
   HAVCT_G729I       = 0x0232,
	// 2011.07.26 add AMR Raw muxer demuxer
   HAVCT_AMR         = 0x0241,
	
	// 2013.06.03 add by DongHyun Kang G726
	HAVCT_G726_40     = 0x0251,
	HAVCT_G726_32     = 0x0252,
	HAVCT_G726_24     = 0x0253,
	HAVCT_G726_16     = 0x0254,

   HAVCT_MMC_MP4     = 0x0311,
   HAVCT_MMC_RTP     = 0x0312,
   HAVCT_RFF         = 0x0313, // 2011.06.01 RTP File Format text base
   HAVCT_RAW         = 0x0400, // used for all raw-format Muxers.
} HAVC_TYPE;

inline const char* GetAVCTypeName(HAVC_TYPE nType)  {
   switch(nType) {
	case HAVCT_UNKNOWN      : return "UNKNOWN"    ;   
	case HAVCT_AVI          : return "AVI"        ;   
	case HAVCT_3GP          : return "3GP"        ;   
	case HAVCT_MP4				: return "MP4"			 ;//2013.06.11 add by DongHyun Kang
	case HAVCT_ASF          : return "ASF"        ;   
	case HAVCT_FLV          : return "FLV"        ;   
	case HAVCT_MPEGPS       : return "MPEGPS"     ;   
	case HAVCT_JPEG         : return "JPEG"       ;   
	case HAVCT_BMP          : return "BMP"        ;   
	case HAVCT_WAV          : return "WAV"        ;   
	case HAVCT_MP3          : return "MP3"        ;   
	case HAVCT_AAC          : return "AAC"        ;   
	case HAVCT_HEAAC        : return "HEAAC"      ;   
	case HAVCT_AC3          : return "AC3"        ; 
      
   //HAVCT_PCM .. ADPCM ... RAW
	case HAVCT_G711A        : return "G711A"      ; 
	case HAVCT_G711U        : return "G711U"      ; 
	case HAVCT_PCM16        : return "PCM16"      ; 
	case HAVCT_VOX          : return "VOX"        ; 
	case HAVCT_G7231_53     : return "G7231_53"   ; 
	case HAVCT_G7231_63     : return "G7231_63"   ; 
	case HAVCT_G729A        : return "G729A"      ; 
	case HAVCT_G729I        : return "G729I"      ; 
	// 2011.07.26 
	case HAVCT_AMR          : return "AMR"        ; 

	// 2013.06.03 add by DongHyun Kang G726
	case HAVCT_G726_40		: return "G726_40"	 ;
	case HAVCT_G726_32		: return "G726_32"	 ;
	case HAVCT_G726_24		: return "G726_24"	 ;
	case HAVCT_G726_16		: return "G726_16"	 ;

	case HAVCT_MMC_MP4      : return "MMC_MP4"    ;   
	case HAVCT_MMC_RTP      : return "MMC_RTP"    ;   
	case HAVCT_RFF          : return "RFF"        ;  // 2011.06.01 
	case HAVCT_RAW          : return "RAW"        ;   
   default                 : return "INVALID"    ;
   };
}


// 2011.06.01 edit by minimei7
// strcmp -> strncasecmp
inline HAVC_TYPE GetAVCTypeFromStr(const char* fmt) {
   if(strncasecmp(fmt, "AVI",       3)==0) return HAVCT_AVI;
   if(strncasecmp(fmt, "3GP",       3)==0) return HAVCT_3GP;
   if(strncasecmp(fmt, "MP4",       3)==0) return HAVCT_MP4; // 2013.06.12 add by lyunix 
   if(strncasecmp(fmt, "ASF",       3)==0) return HAVCT_ASF;
   if(strncasecmp(fmt, "FLV",       3)==0) return HAVCT_FLV;
   if(strncasecmp(fmt, "MPEGPS",    6)==0) return HAVCT_MPEGPS;
   if(strncasecmp(fmt, "MMC_MP4",   7)==0) return HAVCT_MMC_MP4;
   if(strncasecmp(fmt, "MMC_RTP",   7)==0) return HAVCT_MMC_RTP;
   if(strncasecmp(fmt, "RFF",       3)==0) return HAVCT_RFF; // 2011.06.01
   if(strncasecmp(fmt, "JPEG",      4)==0) return HAVCT_JPEG;
   if(strncasecmp(fmt, "BMP",       4)==0) return HAVCT_BMP;
   if(strncasecmp(fmt, "WAV",       3)==0) return HAVCT_WAV;
   if(strncasecmp(fmt, "MP3",       3)==0) return HAVCT_MP3;
   if(strncasecmp(fmt, "AAC",       3)==0) return HAVCT_AAC;
   if(strncasecmp(fmt, "HEAAC",     5)==0) return HAVCT_HEAAC;
   if(strncasecmp(fmt, "AC3",       3)==0) return HAVCT_AC3;

   //HAVCT_PCM .. ADPCM ... RAW
   if(strncasecmp(fmt, "G711A",     5)==0) return HAVCT_G711A;
   if(strncasecmp(fmt, "G711U",     5)==0) return HAVCT_G711U;
   if(strncasecmp(fmt, "PCM16",     5)==0) return HAVCT_PCM16;
   if(strncasecmp(fmt, "VOX",       3)==0) return HAVCT_VOX;

   if(strncasecmp(fmt, "G7231_53",  8)==0) return HAVCT_G7231_53;
   if(strncasecmp(fmt, "G7231_63",  8)==0) return HAVCT_G7231_63;
   if(strncasecmp(fmt, "G729A",     5)==0) return HAVCT_G729A;
   if(strncasecmp(fmt, "G729I",     5)==0) return HAVCT_G729I;
	// 2011.07.26
   if(strncasecmp(fmt, "AMR",       5)==0) return HAVCT_AMR;

	// 2013.06.03 add by DongHyun Kang G726
	if(strncasecmp(fmt, "G726_40",   7)==0) return HAVCT_G726_40;
	if(strncasecmp(fmt, "G726_32",   7)==0) return HAVCT_G726_32;
	if(strncasecmp(fmt, "G726_24",   7)==0) return HAVCT_G726_24;
	if(strncasecmp(fmt, "G726_16",   7)==0) return HAVCT_G726_16;

   if(strncasecmp(fmt, "RAW",       3)==0) return HAVCT_RAW;
   return HAVCT_UNKNOWN;
}


typedef	enum {
	FSTREAM_TYPE_UNKNOWN = 0,
	FSTREAM_TYPE_AUDIO   = 1,
	FSTREAM_TYPE_VIDEO   = 2,
	FSTREAM_TYPE_EOS     = 3,
	FSTREAM_TYPE_ERROR   = 4
} FStreamType;

//enum {AVC_MAX_EXTRA_DATA_SIZE = 512};
enum {AVC_MAX_EXTRA_DATA_SIZE = 1024*80};

typedef struct AVCAudioInfo{
   CODEC_TYPE     nCodecID;
   unsigned int   uFrameCnt;  	 // # frames in file
   unsigned int   uPlayTime;  	 // 총 플레이 시간 (단위 : ms)
   unsigned int   uSampleRate;
   unsigned int   uBitPerSample; 
   unsigned int   uBitRate;
   unsigned int   uTimeScale;        // sample rate
   unsigned int   uChannelCnt;
   unsigned int   uBlockAlign;
   unsigned int   uExtraSize;
   unsigned char  pExtraData[AVC_MAX_EXTRA_DATA_SIZE];
	bool operator==(const AVCAudioInfo& aif)
	{
		if(nCodecID == aif.nCodecID &&
			uFrameCnt == aif.uFrameCnt &&
			uSampleRate == aif.uSampleRate &&
			uBitPerSample == aif.uBitPerSample &&
			uBitRate == aif.uBitRate &&
			uTimeScale == aif.uTimeScale)
			return true;
		return false;
	}
	char* str(char* szBuf,int nLen)
	{
		sprintf(szBuf,"codecid:%d %s framecnt:%d playtime:%d samplerate:%d bitpersample:%d bitrate:%d timesclae:%d channelcnt:%d "
			"blockalign:%d",
			nCodecID,GetCodecTypeName(nCodecID),uFrameCnt,uPlayTime,uSampleRate,uBitPerSample,uBitRate,uTimeScale,uChannelCnt,
			uBlockAlign);
		return szBuf;
	}
} AVCAudioInfo;

typedef struct AVCVideoInfo{
   CODEC_TYPE     nCodecID;
   unsigned int   uFrameCnt;  	 // # frames in file
   unsigned int   uPlayTime;  	 // 총 플레이 시간 (단위 : ms)
   double         dFrameRate;		 // # of frames per second
   unsigned int   uBitRate;       // bps
   unsigned int   uTimeScale;		 // sample rate
   unsigned int   uWidth;
   unsigned int   uHeight;
   unsigned int   uMaxFrameSize;
   unsigned int   uIFramePeriod;  // in second.
   unsigned int   uCBRPeriod;     // in second.
   unsigned int   uExtraSize;
   unsigned char  pExtraData[AVC_MAX_EXTRA_DATA_SIZE];
	bool operator==(const AVCVideoInfo& vif)
	{
		if(nCodecID == vif.nCodecID &&
			uFrameCnt == vif.uFrameCnt &&
			uPlayTime == vif.uPlayTime &&
			dFrameRate == vif.dFrameRate &&
			uBitRate == vif.uBitRate &&
			uTimeScale == vif.uTimeScale &&
			uWidth == vif.uWidth &&
			uHeight == vif.uHeight &&
			uMaxFrameSize == vif.uMaxFrameSize)
			return true;
		return false;
	}
	char* str(char* szBuf,int nLen)
	{
		sprintf(szBuf,"codecid:%d %s framecnt:%d playtime:%d dframerate:%f bitrate:%d timesclae:%d width:%d"
			"height:%d maxframesize:%d iframeperiod:%d cbrperiod:%d",
			nCodecID,GetCodecTypeName(nCodecID),uFrameCnt,uPlayTime,dFrameRate,uBitRate,uTimeScale,uWidth,
			uHeight,uMaxFrameSize,uIFramePeriod,uCBRPeriod);
		return szBuf;
	}
} AVCVideoInfo;

   
}; //namespace AMP 

#endif /*__MT_COMMON_TYPE_DEF_H__*/
