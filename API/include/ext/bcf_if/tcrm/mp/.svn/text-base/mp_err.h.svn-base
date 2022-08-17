#ifndef __MP_ERR_H__
#define __MP_ERR_H__

struct   MPReason {
   enum REASON {
      resNoErr         = 0x00,
   
      //msg
      resInvalidMsg           = 0x10,   
      resInvalidMsgLen        = 0x11,
      resInvalidMsgType       = 0x12,
      resInvalidMsgSubType    = 0x13,
      resInvalidMsgSesId      = 0x14,
      resInvalidMsgAppId      = 0x15,
      resInvalidMsgBoardId    = 0x16,
      resInvalidMsgDupAppId   = 0x17,  // only SetupApp
      resInvalidMsgDupBoardId = 0x18,  // only SetupBoard      
      resInvalidMsgParam      = 0x19,
      resInvalidMsgFileFormat = 0x1a,
      resInvalidMsgMediaType  = 0x1b,
      
      //status
      resInvalidSts           = 0x20,
      resInvalidStsNotReady   = 0x21,
      resInvalidStsNoSession  = 0x22,
      resInvalidStsIdleSes    = 0x23,
      resInvalidStsBusySes    = 0x24,
      resInvalidStsIdleSesErr = 0x25,
      resInvalidStsAlreadyDone= 0x26,           
      resInvalidStsOperationFail = 0x27,
      resInvalidStsNewSts        = 0x28,
      resInvalidStsNoGroup		= 0x29,
      //file 
      resFileIOErr            = 0x30, 
      resFileIOErrOpen        = 0x31,
      resFileIOErrOpenAudio   = 0x32,
      resFileIOErrOpenVideo   = 0x33, 
      resFileIOErrRead        = 0x34, 
      resFileIOErrReadAudio   = 0x35, 
      resFileIOErrReadVideo   = 0x36, 
      resFileIOErrWrite       = 0x37, 
      resFileIOErrWriteAudio  = 0x38, 
      resFileIOErrWriteVideo  = 0x39, 
      resFileReadDone			= 0x3a,
      resFileIOReadTimeout		= 0x3b,
      resFileIOWriteTimeout	= 0x3c,
      
      resNetIOErr             = 0x40,
      resNetIOErrTcp          = 0x41,
      resNetIOErrTcpSnd      = 0x42,
      resNetIOErrTcpRcv      = 0x43,
      resNetIOErrRtp          = 0x44,
      resNetIOErrRtpSnd      = 0x45,
      resNetIOErrRtpRcv      = 0x46,

      resSockErrOpen          = 0x50,
      resSockErrOpenAudio     = 0x51,
      resSockErrOpenVideo     = 0x52,
		resSConvErr					= 0x60,
		resSConvErrOpenAudio		= 0x61,
		resSConvErrOpenVideo		= 0x62,
		resSConvErrReadAudio		= 0x63,
		resSConvErrReadVideo		= 0x64,
		resSConvNoData				= 0x65,
		
		resRTPErr					= 0x70,
		resRTPErrRecvVideo		= 0x71,
		resRTPErrRecvAudio		= 0x72,
		resRTPErrSendVideo		= 0x73,
		resRTPErrSendAudio		= 0x74,
		resRTPRecvTimeout			= 0x75,
		resRTPSendTimeout			= 0x76,
		resRTPInvalidMediaType  = 0x77,
		resRTPInvalidParam			= 0x78,
		resRTPNoData					= 0x79,
		
		resTCErr					= 0x80,
		resTCErrStartAudio		= 0x81,
		resTCErrStartVideo		= 0x82,
		resTCErrOperationFail	= 0x83,
		resTCInvalidParam			= 0x84,
		resTCNoData					= 0x85,
		
		resROOMErr					= 0x90,
		
		resGroupErr					= 0xa0,
		resGroupInvalidParam		= 0xa1,
		
		resJobErr					= 0xb0,
		resJobErrCreate			= 0xb1,
		resSteLoop					= 0xFE,
      resErrEtc               = 0xFF,       
   };

   static const char * str(unsigned int nRes) {
      switch(nRes & 0x000000FF) {
      case resNoErr                 :
   	   return "Ok"             ;
      //msg
      case resInvalidMsg            :
   	   return "Err:InvalidMsg"        ;
      case resInvalidMsgLen         :
   	   return "Err:InvalidMsgLen"     ;
      case resInvalidMsgType        :
   	   return "Err:InvalidMsgType"    ;
      case resInvalidMsgSubType     :
   	   return "Err:InvalidMsgSubType" ;
      case resInvalidMsgSesId       :
   	   return "Err:InvalidMsgSesId"   ;
      case resInvalidMsgAppId       :
   	   return "Err:InvalidMsgAppId"   ;
      case resInvalidMsgBoardId       :
   	   return "Err:InvalidMsgBoardId"   ;
      case resInvalidMsgDupAppId    :
   	   return "Err:InvalidMsgDupAppId";
      case resInvalidMsgDupBoardId  :
   	   return "Err:InvalidMsgDupBoardId";
      case resInvalidMsgParam  :
   	   return "Err:InvalidMsgParam";
      case resInvalidMsgFileFormat  :
   	   return "Err:InvalidMsgFileFormat";
      case resInvalidMsgMediaType  :
   	   return "Err:InvalidMsgMediaType";
      
      
      //status                   
      case resInvalidSts            :
   	   return "Err:InvalidSts"        ;
      case resInvalidStsNotReady    :
   	   return "Err:InvalidStsNotReady";
      case resInvalidStsNoSession   :
   	   return "Err:InvalidStsNoSession";
      case resInvalidStsIdleSes     :
   	   return "Err:InvalidStsIdleSes" ;
      case resInvalidStsBusySes     :
   	   return "Err:InvalidStsBusySes" ;
      case resInvalidStsIdleSesErr  :
   	   return "Err:InvalidStsIdleSesErr";
      case resInvalidStsAlreadyDone :
   	   return "Err:InvalidStsAlreadyDone";
      case resInvalidStsOperationFail :
         return "Err:InvalidStsOperationFail";
      case resInvalidStsNewSts           :
         return "Err:InvalidStsNewSts";
		case resInvalidStsNoGroup   :
   	   return "Err:InvalidStsNoGroup";
      //file                  
      case resFileIOErr             :
   	   return "Err:FileIOErr"         ;
      case resFileIOErrOpenAudio    :
   	   return "Err:FileIOErrOpenAudio";
      case resFileIOErrOpen         :
   	   return "Err:FileIOErrOpen";
      case resFileIOErrOpenVideo    :
   	   return "Err:FileIOErrOpenVideo";
      case resFileIOErrRead         :
   	   return "Err:FileIOErrRead"     ;
      case resFileIOErrReadAudio    :
   	   return "Err:FileIOErrReadAudio";
      case resFileIOErrReadVideo    :
   	   return "Err:FileIOErrReadVideo";
      case resFileIOErrWrite        :
   	   return "Err:FileIOErrWrite"    ;
      case resFileIOErrWriteAudio   :
   	   return "Err:FileIOErrWriteAudio";
      case resFileIOErrWriteVideo   :
   	   return "Err:FileIOErrWriteVideo";
   	case resFileReadDone				:
   		return "resFileReadDone";
      case resFileIOReadTimeout		:
      	return "resFileIOReadTimeout";
      case resFileIOWriteTimeout		:
      	return "resFileIOWriteTimeout";
   	
      
      //Net IO Err
      case resNetIOErr             :
   	   return "Err:NetIOErr"           ;
      case resNetIOErrTcp          :
   	   return "Err:NetIOErrTcp"        ;
      case resNetIOErrTcpSnd      :
   	   return "Err:NetIOErrTcpSnd"    ;
      case resNetIOErrTcpRcv      :
   	   return "Err:NetIOErrTcpRcv"    ;
      case resNetIOErrRtp          :
   	   return "Err:NetIOErrRtp"        ;
      case resNetIOErrRtpSnd      :
   	   return "Err:NetIOErrRtpSnd"    ;
      case resNetIOErrRtpRcv      :
   	   return "Err:NetIOErrRtpRcv"    ;

      case resSockErrOpen        :
         return "Err:SockErrOpen";
      case resSockErrOpenAudio   :
         return "Err:SockErrOpenAudio";
      case resSockErrOpenVideo   :
         return "Err:SockErrOpenVideo";

   	case resSConvErr					:
   		return "Err:resSConvErr";
		case resSConvErrOpenAudio		:
			return "Err:resSConvErrOpenAudio";
		case resSConvErrOpenVideo		:
			return "Err:resSConvErrOpenVideo";
		case resSConvErrReadAudio		:
			return "Err:resSConvErrReadAudio";
		case resSConvErrReadVideo		:
			return "Err:resSConvErrReadVideo";
		case resSConvNoData				:
			return "resSConvNoData";
		
		case resRTPErr					:
			return "Err:resRTPErr";
		case resRTPErrRecvVideo		:
			return "Err:resRTPErrRecvVideo";
		case resRTPErrRecvAudio		:
			return "Err:resRTPErrRecvAudio";
		case resRTPErrSendVideo		:
			return "Err:resRTPErrSendVideo";
		case resRTPErrSendAudio		:
			return "Err:resRTPErrSendAudio";
		case resRTPRecvTimeout		:
			return "Err:resRTPRecvTimeout";
		case resRTPSendTimeout		:
			return "resRTPSendTimeout";
		case resRTPInvalidMediaType :
			return "resRTPInvalidMediaType";
		case resRTPInvalidParam		:
			return "resRTPInvalidParam";
		case resRTPNoData				:
			return "resRTPNoData";
			
		case resTCErrStartAudio:
			return "resTCErrStartAudio";
		case resTCErrStartVideo:
			return "resTCErrStartVideo";
		case resTCErrOperationFail:
			return "resTCErrOperationFail";
		case resTCInvalidParam:
			return "resTCInvalidParam";
		case resTCNoData:
			return "resTCNoData";
			
		case resROOMErr:
			return "resROOMErr";
		case resGroupErr:
			return "resGroupErr";
		case resGroupInvalidParam:
			return "resGroupInvalidParam";
		case resJobErr					:
			return "resJobErr";
		case resJobErrCreate			:
			return "resJobErrCreate";
		case resSteLoop				:
			return "resSteLoop";
      case resErrEtc               :        		
   	   return "Err:Etc"             ;
      default                      :
        return "Err:Unknown"           ;
      }
      return "Err:Unknown";
   }
};
#endif
////@}
