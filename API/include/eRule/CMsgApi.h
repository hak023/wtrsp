#ifndef RESIP_CMSGAPI_HXX
#define RESIP_CMSGAPI_HXX
#include "CAppParser.h"

/****************** Test Sample Code ****************************************************/
extern resip::Data TestAPIMsg;
void fnMsgApiSample();
//--------> MergeHeader    Ex>  history-info: <>,<>,<>
void fnMsgApiMergeSingleLine(resip::SipMessage *_pclsMsg,  // source resip Message
										const char * _pszName); // Header Name
//-------> Get Header Raw Data
 resip::HeaderFieldValueList* fnMsgApiGetHeaderValueList(resip::SipMessage *_pclsMsg,  // source resip Message
															const char * _pszName);       // Header Name
//-------> Is Exist Header 
bool fnMsgApiIsExistHeader(resip::SipMessage *_pclsMsg, const char * _pszName);    // Is Exist Headers
/****************** resip Message Get Functions ********************************************/
//-------> Get First Line
void fnMsgApiGetFirstLine(resip::SipMessage *_pclsSipMsg,        // source resip Message
								bool * _bRetRequest,                     // result : requeset or response
								int * _pnRetRspCode,	                   // result : response code
								resip::MethodTypes * _peMethod,    // result : method enum , req=>request, rsp=>seq method
								resip::Data * _pclsRetMethod);       // result : method String
//-------> Get First Line Raw Data
void fnMsgApiGetFirstRawData(resip::SipMessage * _pclsSipMsg, RUString &_rclsResult);
//-------> Get Request Uri String
const char * fnMsgApiGetRequestUri(resip::SipMessage * _pclsSipMsg,RUString &_rclsResult);
//-------> Get Header MultiLine
void fnMsgApiGetHeaderLine(resip::SipMessage *_pclsMsg,  // source resip Message 
							const char * _pszName,                 // Header Name
							RUArrayString *_pclsResultString);   // Result Multi String
//--------> Get Header Param
bool fnMsgApiGetParam(resip::SipMessage *_pclsMsg,          // source resip Message 
						const char * _pszHeader,                     // Header Name
						const char * _pszParam,                      // Header Param
						RUArrayString *_pclsResultString);         // Result Multi String
//--------> Get Header Uri Param
bool fnMsgApiGetUriParam(resip::SipMessage *_pclsMsg,       // source resip Message 
							const char * _pszHeader,                // Header Name
							const char * _pszParam,                  // Header Uri Param
							RUArrayString *_pclsResultString);    // Result Multi String
//--------> Get Header Uri Full String							
void fnMsgApiGetUriFullString(resip::SipMessage * _pclsMsg,      // source resip Message 
										const char * _pszHeader,    // Header Name
										char * _pszOut,                  // Result String
										unsigned int _unOutMax);    // Result Buffer size of
//--------> Get Header DisplayName
bool fnMsgApiGetUriDisplay(resip::SipMessage *_pclsMsg,     // source resip Message 
							const char * _pszHeader,                 // Header Name
							RUArrayString *_pclsResultString);    // Result Multi String
//--------> Get Header Uri UserName
bool fnMsgApiGetUriUserName(resip::SipMessage *_pclsMsg,  // source resip Message 
								const char * _pszHeader,            // Header Name
								RUArrayString *_pclsResultString); // Result Multi String
//--------> Get Header Uri Host
bool fnMsgApiGetUriHost(resip::SipMessage *_pclsMsg,              // source resip Message 
						const char * _pszHeader,                          // Header Name
						RUArrayString *_pclsResultString);              // Result Multi String
//--------> Get Header Uri Port
bool fnMsgApiGetUriPort(resip::SipMessage *_pclsMsg,              // source resip Message 
						const char * _pszHeader,                          // Header Name
						RUArrayString *_pclsResultString);             // Result Multi String
//--------> Get Header Uri Scheme
bool fnMsgApiGetUriScheme(resip::SipMessage *_pclsMsg,        // source resip Message 
							const char * _pszHeader,                     // Header Name
							RUArrayString *_pclsResultString);        // Result Multi String
//--------> Get Via Transport
bool fnMsgApiGetViaTransport(resip::SipMessage *_pclsMsg,    // source resip Message 
							RUArrayString *_pclsResultString);        // Result Multi String
//--------> Get Via Host
bool fnMsgApiGetViaHost(resip::SipMessage *_pclsMsg,              // source resip Message 
							RUArrayString *_pclsResultString);        // Result Multi String
//--------> Get Via Port
bool fnMsgApiGetViaPort(resip::SipMessage *_pclsMsg,               // source resip Message 
							RUArrayString *_pclsResultString);        // Result Multi String
							

/************************** resip Message Encode Functions ********************************/
void fnMsgApiSetFirstLine(resip::SipMessage *_pclsSipMsg,        // source resip Message
								int  _nRspCode,	                        // result : response code
								resip::Data * _pclsMethod);           // result : method String
//--------> Set Manual Position 
void fnMsgApiAddPosition(RUArrayBools * _pclsPos,                 // source position array         
								unsigned int _uiPos);                     // position enum
//--------> Add Header 
void fnMsgApiAddHeader(resip::SipMessage *_pclsSrcMsg,           // source resip Message                
						const char * _pszH,                                    // Header Name
						const char * _pszV,                                    // Header Value String
						EMsgPos_t _ePos);                                     // Multi Header Position (First/ Last)
//--------> Add Header Param
void fnMsgApiAddParam(resip::SipMessage *_pclsSrcMsg,           // source resip Message
						const char * _pszH,                                   // Header Name
						const char * _pszP,                                   // Header Parameter
						const char * _pszV,                                   // Header Parameter Value String
						EMsgPos_t _ePos,                                     // Multi Header Position (First/ Last)
						RUArrayBools * _pclsMutiPos=NULL);           // Multi Position (Ext - T,F,F,T)
//--------> Add Header Uri Param
void fnMsgApiAddUriParam(resip::SipMessage *_pclsSrcMsg,     // source resip Message
						const char * _pszH,                                   // Header Name
						const char * _pszP,                                    // Header Parameter
						const char * _pszV,                                   // Header Parameter Value String
						EMsgPos_t _ePos,                                      // Multi Header Position (First/ Last)
						RUArrayBools * _pclsMutiPos=NULL);           // Multi Position (Ext - T,F,F,T)
//--------> Add Header DisplayName
void fnMsgApiAddUriDisplay(resip::SipMessage *_pclsSrcMsg,     // source resip Message
									const char * _pszH,                 // Header Name
									const char * _pszV,                 // Header Parameter Value String
									EMsgPos_t _ePos,                    // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL); // Multi Position (Ext - T,F,F,T)
//--------> Add Header Uri UserName
void fnMsgApiAddUriUserName(resip::SipMessage *_pclsSrcMsg,     // source resip Message
									const char * _pszH,                 // Header Name
									const char * _pszV,                 // Header Parameter Value String
									EMsgPos_t _ePos,                    // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL); // Multi Position (Ext - T,F,F,T)
//--------> Add Header Uri Host
void fnMsgApiAddUriHost(resip::SipMessage *_pclsSrcMsg,         // source resip Message
									const char * _pszH,                 // Header Name
									const char * _pszV,                 // Header Parameter Value String
									EMsgPos_t _ePos,                    // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL);           // Multi Position (Ext - T,F,F,T)
//--------> Add Header Uri Port
void fnMsgApiAddUriPort(resip::SipMessage *_pclsSrcMsg,          // source resip Message
									const char * _pszH,                 // Header Name
									const char * _pszV,                 // Header Parameter Value String
									EMsgPos_t _ePos,                    // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL); // Multi Position (Ext - T,F,F,T)
//--------> Add Header Uri Scheme
void fnMsgApiAddUriScheme(resip::SipMessage *_pclsSrcMsg,     // source resip Message
									const char * _pszH,                 // Header Name
									const char * _pszV,                 // Header Parameter Value String
									EMsgPos_t _ePos,                    // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL); // Multi Position (Ext - T,F,F,T)
//--------> Add Via Transport
void fnMsgApiAddViaTransport(resip::SipMessage *_pclsSrcMsg, // source resip Message
									const char * _pszV,                 // via transport Value String
									EMsgPos_t _ePos,                    // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL); // Multi Position (Ext - T,F,F,T)
//--------> Add Via Host
void fnMsgApiAddViaHost(resip::SipMessage *_pclsSrcMsg,         // source resip Message
									const char * _pszV,                 // via Host Value String
									EMsgPos_t _ePos,                    // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL); // Multi Position (Ext - T,F,F,T)
//--------> Add Via port
void fnMsgApiAddViaPort(resip::SipMessage *_pclsSrcMsg,         // source resip Message
									const char * _pszV,                 // via Port Value String
									EMsgPos_t _ePos,                    // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL);// Multi Position (Ext - T,F,F,T)


//--------> Remove Header
void fnMsgApiRemoveHeader(resip::SipMessage *_pclsSrcMsg,  // source resip Message
									const char * _pszH,                // Header Name
									EMsgPos_t _ePos,                  // Multi Header Position (First/ Last)
									RUArrayBools * _pclsMutiPos=NULL);// Multi Position (Ext - T,F,F,T)
//--------> Remove Header Parameter
void fnMsgApiRemoveParam(resip::SipMessage *_pclsSrcMsg,   // source resip Message
									const char * _pszH,                // Header Name
									const char * _pszP,                // Header Parameter
									EMsgPos_t _ePos,                  // Multi Header Position (First/ Last)
									RUArrayBools * _pclsMutiPos=NULL); // Multi Position (Ext - T,F,F,T)
//--------> Remove Header Uri Parameter
void fnMsgApiRemoveUriParam(resip::SipMessage *_pclsSrcMsg,// source resip Message
									const char * _pszH,                // Header Name
									const char * _pszP,                // Header Parameter
									EMsgPos_t _ePos,                  // Multi Header Position (First/ Last)
									RUArrayBools * _pclsMutiPos=NULL);           // Multi Position (Ext - T,F,F,T)
//--------> Remove Header DisplayName
void fnMsgApiRemoveUriDisplay(resip::SipMessage *_pclsSrcMsg, // source resip Message
									const char * _pszH,                     // Header Name
									EMsgPos_t _ePos,                        // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL); // Multi Position (Ext - T,F,F,T)
//--------> Remove Header Uri UserName
void fnMsgApiRemoveUriUserName(resip::SipMessage *_pclsSrcMsg,// source resip Message
										const char * _pszH,                 // Header Name
										EMsgPos_t _ePos,                    // Multi Header Position
										RUArrayBools * _pclsMutiPos=NULL); // Multi Position (Ext - T,F,F,T)
//--------> Remove Header Uri Host
void fnMsgApiRemoveUriHost(resip::SipMessage *_pclsSrcMsg,  // source resip Message
									const char * _pszH,                 // Header Name
									EMsgPos_t _ePos,                    // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL);           // Multi Position (Ext - T,F,F,T)
//--------> Remove Header Uri Port
void fnMsgApiRemoveUriPort(resip::SipMessage *_pclsSrcMsg,    // source resip Message
									const char * _pszH,                 // Header Name
									EMsgPos_t _ePos,                    // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL);  // Multi Position (Ext - T,F,F,T)
//--------> Remove Header Uri Scheme
void fnMsgApiRemoveUriScheme(resip::SipMessage *_pclsSrcMsg, // source resip Message
									const char * _pszH,                 // Header Name
									EMsgPos_t _ePos,                    // Multi Header Position
									RUArrayBools * _pclsMutiPos=NULL); // Multi Position (Ext - T,F,F,T)
//--------> Shift Header Parameter
void fnMsgApiShiftParam(resip::SipMessage * _pclsMsg,                // source resip Message
									const char * _pszH,                   // Header Name
									const char * _pszP,                   // Header Parameter
									EMsgPos_t _ePos);                     // Multi Header Position (Shift Up/Donw)
//--------> Shift Header Uri Parameter
void fnMsgApiShiftUriParam(resip::SipMessage * _pclsMsg,           // source resip Message
									const char * _pszH,                   // Header Name
									const char * _pszP,                   // Header Parameter
									EMsgPos_t _ePos);                     // Multi Header Position (Shift Up/Donw)

// --------> Special History-Info Proc
void fnMsgApiConvHexToChar(char * _pszSrc);   // only 3B(;), 3D(=)
void fnMsgApiInternalHistoryInfoScreen(resip::SipMessage * _pclsMsg);
void fnMsgApiExternalHistoryInfoScreen(resip::SipMessage * _pclsMsg);
#endif

