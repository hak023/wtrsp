
#ifndef __APP_COMMSG_H_
#define __APP_COMMSG_H_

#include <stdio.h>
#include <string>

#ifdef _MSC_VER
   #include <WINSOCK2.H>
#else 
   #include <netinet/in.h>
#endif

#ifdef WIN32
#ifndef _WINSOCKAPI_

#define _WINSOCKAPI_
#endif
#endif

//@union_compses
#ifndef _LINUX_
#define V2TDM_BIG_ENDIAN 
#endif

struct AppComMsgHdr
{
   enum { APP_MAX_DESCNAME = 32 };
   ///defines the first element
	unsigned int uiMagicCode;						;
   unsigned int uiMsgLen;                   	; //including header, the message length
   unsigned short usType                     ; //the message type
	unsigned short usSubType						;
	unsigned short usAppId							;
	unsigned short usAppSesId						;
   unsigned short usReasonCode               ;
	unsigned short usReserved						;
	char szDesc[APP_MAX_DESCNAME]					;
   char* data(){
      return (char*)(this+1);
   }
   
   //converts from host byte order to network byte order
   void hton(){
      uiMsgLen=htonl(uiMsgLen);
      usType=htons(usType);
      usReasonCode=htons(usReasonCode);
   }

   //converts from network byte order to host byte order
   void ntoh(){
      uiMsgLen=ntohl(uiMsgLen);
      usType=ntohs(usType);
      usReasonCode=ntohs(usReasonCode);
   }
};

///defines common body
enum APP_COMMON_TYPE
{
   APP_SETUP_APPS = 0xF001,   //no body,
   APP_ALIVE      = 0xF002,   //no body,
   APP_OOS_CHECK  = 0xF003,   //no body,
	APP_INTERNAL	= 0xF004
};

enum APP_COMMON_SUBTYPE
{
	APP_TCP_CONN,
	APP_TCP_DISC,
	APP_REQ,
	APP_RES,
	APP_INFO
};
#endif
