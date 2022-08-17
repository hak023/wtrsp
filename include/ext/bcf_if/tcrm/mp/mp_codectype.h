/*
*/
#ifndef RTP_CODECTYPE_H
#define RTP_CODECTYPE_H

#include <string>

typedef enum {
   CTI_NONE    = -1,   
   CTI_UNKNOWN = 0,
   CTI_H263     ,
   CTI_PCM      ,
   CTI_G7231    ,
   CTI_AMR_IF2  ,
   CTI_AMR_RFC  , //RFC44 
   CTI_AMR_RFC53,
   CTI_AMR_WB,
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
	case CTI_AMR_WB		  : return "AMR-WB"		;
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
	   case CTI_AMR_WB   :
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
typedef  enum  { 
   HAVCT_UNKNOWN     = 0,    
   HAVCT_AVI         = 0x0010, 
   HAVCT_3GP         = 0x0020, 
   HAVCT_ASF         = 0x0030, 
   HAVCT_FLV         = 0x0040, 
   HAVCT_MPEGPS      = 0x0050, 
   HAVCT_JPEG        = 0x0060, 

   HAVCT_WAV         = 0x0110, 
   HAVCT_MP3         = 0x0120, 
   HAVCT_AAC         = 0x0131, 
   HAVCT_HEAAC       = 0x0132, 
   HAVCT_AC3         = 0x0140, 

   //HAVCT_PCM .. ADPCM ... RAW
   HAVCT_G711A       = 0x0211, 
   HAVCT_G711U       = 0x0212, 
   HAVCT_G7231_53    = 0x0221, 
   HAVCT_G7231_63    = 0x0222, 
   HAVCT_G729A       = 0x0231, 
   HAVCT_G729I       = 0x0232, 
	HAVCT_AMR         = 0x0241,//11.07.29
   HAVCT_MMC_MP4     = 0x0311, 
   HAVCT_MMC_RTP     = 0x0312, 
   HAVCT_RFF         = 0x0313, 
   HAVCT_RAW         = 0x0400, // used for all raw-format Muxers.
} HAVC_TYPE;

inline const char* GetAVCTypeName(HAVC_TYPE nType)  {
   switch(nType) {
   case HAVCT_UNKNOWN      : return "UNKNOWN"    ;
   case HAVCT_AVI          : return "AVI"        ;
   case HAVCT_3GP          : return "3GP"        ;
   case HAVCT_ASF          : return "ASF"        ;
   case HAVCT_FLV          : return "FLV"        ;
   case HAVCT_MPEGPS       : return "MPEGPS"     ;
   case HAVCT_JPEG         : return "JPEG"       ;
   case HAVCT_WAV          : return "WAV"        ;
   case HAVCT_MP3          : return "MP3"        ;
   case HAVCT_AAC          : return "AAC"        ;
   case HAVCT_HEAAC        : return "HEAAC"      ;
   case HAVCT_AC3          : return "AC3"        ;

   //HAVCT_PCM .. ADPCM ... RAW
   case HAVCT_G711A        : return "G711A"      ;
   case HAVCT_G711U        : return "G711U"      ;
   case HAVCT_G7231_53     : return "G7231_53"   ;
   case HAVCT_G7231_63     : return "G7231_63"   ;
   case HAVCT_G729A        : return "G729A"      ;
   case HAVCT_G729I        : return "G729I"      ;
  	case HAVCT_AMR          : return "AMR"        ;//11.07.29
 
   case HAVCT_MMC_MP4      : return "MMC_MP4"    ;
   case HAVCT_MMC_RTP      : return "MMC_RTP"    ;
   case HAVCT_RFF          : return "RFF"    ;   
   case HAVCT_RAW          : return "RAW"        ;
   default                 : return "INVALID"    ;
   };
}

inline HAVC_TYPE GetAVCTypeFromStr(const char* fmt) {
   std::string str = fmt;
   for(uint i=0;i<str.length();i++) if(islower(str[i])) str[i] += ('A' - 'a');
   if(str.substr(0,3)=="AVI"       ) return HAVCT_AVI;
   if(str.substr(0,3)=="3GP"       ) return HAVCT_3GP;
   if(str.substr(0,3)=="ASF"       ) return HAVCT_ASF;
   if(str.substr(0,3)=="FLV"       ) return HAVCT_FLV;
   if(str.substr(0,6)=="MPEGPS"    ) return HAVCT_MPEGPS;
   if(str.substr(0,7)=="MMC_MP4"   ) return HAVCT_MMC_MP4;
   if(str.substr(0,7)=="MMC_RTP"   ) return HAVCT_MMC_RTP;
   if(str.substr(0,3)=="RFF"       ) return HAVCT_RFF;
   if(str.substr(0,4)=="JPEG"      ) return HAVCT_JPEG;
   if(str.substr(0,3)=="WAV"       ) return HAVCT_WAV;
   if(str.substr(0,3)=="MP3"       ) return HAVCT_MP3;
   if(str.substr(0,3)=="AAC"       ) return HAVCT_AAC;
   if(str.substr(0,3)=="HEAAC"     ) return HAVCT_HEAAC;
   if(str.substr(0,3)=="AC3"       ) return HAVCT_AC3;
   //HAVCT_PCM .. ADPCM ... RAW    
   if(str.substr(0,5)=="G711A"     ) return HAVCT_G711A;
   if(str.substr(0,5)=="G711U"     ) return HAVCT_G711U;
   if(str.substr(0,8)=="G7231_53"  ) return HAVCT_G7231_53;
   if(str.substr(0,8)=="G7231_63"  ) return HAVCT_G7231_63;
   if(str.substr(0,5)=="G729A"     ) return HAVCT_G729A;
   if(str.substr(0,5)=="G729I"     ) return HAVCT_G729I;
	if(str.substr(0,3)=="AMR"       ) return HAVCT_AMR;
   if(str.substr(0,3)=="RAW"       ) return HAVCT_RAW;
   return HAVCT_UNKNOWN;
}
#endif //RTP_CODECTYPE_H
////@}
