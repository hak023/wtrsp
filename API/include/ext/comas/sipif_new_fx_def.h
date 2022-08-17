 
#ifndef SIPIF_NEW_FX_DEF_H
#define SIPIF_NEW_FX_DEF_H

/* --------------------------------------------- */
/* - category - */
/* --------------------------------------------- */
/*==== Common Category of Init/Ping/Sip/Term ====*/
#define SIPFLEXIF_C_com 					"COMMON"
/*==== additional Category of Sip ====*/
#define SIPFLEXIF_C_sipHdr 				"HDR"
#define SIPFLEXIF_C_sipBody	 		"BODY"

/* --------------------------------------------- */
/* - key (not include SIP Header) - */
/* --------------------------------------------- */
//SIPFLEXIF_C_com (Init/Ping/Sip/Term)
#define SIPFLEXIF_K_compId				"CID"
#define SIPFLEXIF_K_compSessId		"CSID"
#define SIPFLEXIF_K_asId						"AID"
#define SIPFLEXIF_K_asSessId				"ASID"
#define SIPFLEXIF_K_msgType				"MTYPE"
#define SIPFLEXIF_K_reasonCode		"RC"
//SIPFLEXIF_C_com (Init)
#define SIPFLEXIF_K_maxSessCnt		"MAXCNT"
//SIPFLEXIF_C_com (Ping)
#define SIPFLEXIF_K_enable					"ENABLE"
#define SIPFLEXIF_K_curSessCnt			"CURCNT"
//SIPFLEXIF_C_com (Init/Ping Rsp)
#define SIPFLEXIF_K_reqResult			"RESULT"
//SIPFLEXIF_C_com (Sip)
#define SIPFLEXIF_K_tuId						"TID"
#define SIPFLEXIF_K_sessType				"STYPE"
#define SIPFLEXIF_K_dlgType				"DTYPE"
#define SIPFLEXIF_K_sipDir					"DIR"
#define SIPFLEXIF_K_sipMethod			"MET"
#define SIPFLEXIF_K_sipRspCode			"RPC"
#define SIPFLEXIF_K_caller					"CR"
#define SIPFLEXIF_K_called					"CD"
#define SIPFLEXIF_K_localIp					"LIP"
#define SIPFLEXIF_K_localPort				"LPORT"
#define SIPFLEXIF_K_remoteIp				"RIP"
#define SIPFLEXIF_K_remotePort			"RPORT"
#define SIPFLEXIF_K_trsp						"TRSP"
#define SIPFLEXIF_K_reqUri					"RURI"
#define SIPFLEXIF_K_callState				"CS"
//SIPFLEXIF_C_sipBody
#define SIPFLEXIF_K_content				"CONTENT"

/* --------------------------------------------- */
/* - predefined value - */
/* --------------------------------------------- */
/*==== type of type ====*/
#define SIPFLEXIF_V_MSGTYPE_initReq					"InitReq"
#define SIPFLEXIF_V_MSGTYPE_initRsp 					"InitRsp"
#define SIPFLEXIF_V_MSGTYPE_pingReq 				"PingReq"
#define SIPFLEXIF_V_MSGTYPE_pingRsp 				"PingRsp"
#define SIPFLEXIF_V_MSGTYPE_sip			 				"Sip"
#define SIPFLEXIF_V_MSGTYPE_internalTerm		"InternalTerm"
/*==== type of result ====*/
#define SIPFLEXIF_V_REASON_succ									"0"
#define SIPFLEXIF_V_REASON_failInvalidCompId			"1"
#define SIPFLEXIF_V_REASON_failInvalidAppId				"2"
#define SIPFLEXIF_V_REASON_failUnknownTuple			"3"
#define SIPFLEXIF_V_REASON_failInvalidStatus			"4"
#define SIPFLEXIF_V_REASON_failInternalErr				"5"
/*==== direction of sip ====*/
#define SIPFLEXIF_V_SIPDIR_incoming		"income"
#define SIPFLEXIF_V_SIPDIR_outgoing		"outgo"
/*==== type of session ====*/
#define SIPFLEXIF_V_SESSTYPE_invite		"inv"
#define SIPFLEXIF_V_SESSTYPE_regi			"reg"
#define SIPFLEXIF_V_SESSTYPE_sub			"sub"
#define SIPFLEXIF_V_SESSTYPE_stand		"stand"
/*==== state of call ====*/
#define SIPFLEXIF_V_CALLSTATE_init				"init"
#define SIPFLEXIF_V_CALLSTATE_establish	"est"
/*==== type of dialog ====*/
#define SIPFLEXIF_V_DLGTYPE_in			"inDlg"
#define SIPFLEXIF_V_DLGTYPE_out			"outDlg"
/*==== type of transport ====*/
#define SIPFLEXIF_V_TRSP_tls			"tls"
#define SIPFLEXIF_V_TRSP_tcp 		"tcp"
#define SIPFLEXIF_V_TRSP_udp		"udp"
#define SIPFLEXIF_V_TRSP_sctp		"sctp"
#define SIPFLEXIF_V_TRSP_dccp		"dccp"
#define SIPFLEXIF_V_TRSP_dtls		"dtls"
/*==== method of sip ====*/
#define SIPFLEXIF_V_SIPMETHOD_ACK						"ACK"
#define	SIPFLEXIF_V_SIPMETHOD_BYE					"BYE"
#define	SIPFLEXIF_V_SIPMETHOD_CANCEL				"CANCEL"
#define	SIPFLEXIF_V_SIPMETHOD_INVITE	 			"INVITE"
#define	SIPFLEXIF_V_SIPMETHOD_NOTIFY				"NOTIFY"
#define	SIPFLEXIF_V_SIPMETHOD_OPTIONS			"OPTIONS"
#define	SIPFLEXIF_V_SIPMETHOD_REFER 				"REFER"
#define	SIPFLEXIF_V_SIPMETHOD_REGISTER			"REGISTER"
#define	SIPFLEXIF_V_SIPMETHOD_SUBSCRIBE		"SUBSCRIBE"
#define	SIPFLEXIF_V_SIPMETHOD_MESSAGE			"MESSAGE"
#define	SIPFLEXIF_V_SIPMETHOD_INFO					"INFO"
#define	SIPFLEXIF_V_SIPMETHOD_PRACK				"PRACK"
#define	SIPFLEXIF_V_SIPMETHOD_PUBLISH			"PUBLISH"
#define	SIPFLEXIF_V_SIPMETHOD_SERVICE			"SERVICE"
#define	SIPFLEXIF_V_SIPMETHOD_UPDATE				"UPDATE"

#endif

