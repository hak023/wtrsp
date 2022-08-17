
#ifndef _EVENT_DEFINITION_H_
#define _EVENT_DEFINITION_H_

#include <sys/types.h>

#define MAX_EVENTINFO_SIZE (8 * 1024)
#define EVENTINFO_HEADER_LENGTH (sizeof(XTMMSGHDR) + sizeof(ARSLINE))
#define MAX_VALUE_SIZE (MAX_EVENTINFO_SIZE - EVENTINFO_HEADER_LENGTH)

#if defined(LINUX) && !defined(uint16_t)
#	include <stdint.h>
#endif

#ifndef XTMADDR_DEF
#	define XTMADDR_DEF
typedef struct {
   uint16_t usZoneId;
   uint16_t usSysId;
   uint16_t usParentId;
   uint16_t usChildId;
   uint32_t ulTmpPid;

	 void hton()
 	{
		usZoneId = htons(usZoneId);
		usSysId = htons(usSysId);
		usParentId = htons(usParentId);
		usChildId = htons(usChildId);
		ulTmpPid = htonl(ulTmpPid);
 	}
} XTMADDR, *PXTMADDR;

#define XtmAddr(st, Zone, Sys, Parent, Child) \
   ((st).unZoneId = (Zone), (st).usSysId = (Sys), \
    (st).usParentId = (Parent), (st).usChildId = (Child), (st))

typedef struct {
   unsigned int nMagicCookie;
   int nMsgLen;
   XTMADDR stReceiver;
   XTMADDR stSender;
   short snHopCnt;
   unsigned short usTransactionId;
   short snSerNo;
   unsigned short _reserved;
   unsigned short usType;
   unsigned short usSubType;

	void hton()
	{
		nMagicCookie = htonl(nMagicCookie);
		nMsgLen = htonl(nMsgLen);
		stReceiver.hton();
		stSender.hton();
		snHopCnt = htons(snHopCnt);
		usTransactionId = htons(usTransactionId);
		snSerNo = htons(snSerNo);
		_reserved = htons(_reserved);
		usType = htons(usType);
		usSubType = htons(usSubType);		
	}
} XTMMSGHDR, *PXTMMSGHDR;

#endif // XTMADDR_DEF

typedef struct {
   uint16_t usAreaId;
   uint16_t usSystemId;
   uint16_t usBoardId;
   uint16_t usTrunkId;
   uint16_t usChannelId;
   uint16_t usReserved[3];

	 void hton()
 	{
		usAreaId = htons(usAreaId);
		usSystemId = htons(usSystemId);
		usBoardId = htons(usBoardId);
		usTrunkId = htons(usTrunkId);
		usChannelId = htons(usChannelId);
		usReserved[0] = htons(usReserved[0]);
		usReserved[1] = htons(usReserved[1]);
		usReserved[2] = htons(usReserved[2]);
 	}
} ARSLINE, *PARSLINE;

typedef struct {
   XTMMSGHDR h;
   ARSLINE line;
   char Value[MAX_VALUE_SIZE];
} EVENTINFO, *PEVENTINFO;

#define EVENTINFO_LENGTH(e)        ((e)->h.nMsgLen)
#define EVENTINFO_VALUE_LENGTH(e)  ((e)->h.nMsgLen - EVENTINFO_HEADER_LENGTH)
#define EVENTINFO_MAGIC(e)         ((e)->h.nMagicCookie)
#define EVENTINFO_RECEIVER(e)      ((e)->h.stReceiver.usSysId)
#define EVENTINFO_SENDER(e)        ((e)->h.stSender.usSysId)
#define EVENTINFO_RECV_PARENTID(e) ((e)->h.stReceiver.usParentId)
#define EVENTINFO_TYPE(e)          ((e)->h.usType)
#define EVENTINFO_SUBTYPE(e)       ((e)->h.usSubType)
#ifdef NEW_VERSION
#define EVENTINFO_TYPES(e)         ((uint32_t)((e)->h.usType))
#endif
#define EVENTINFO_AREAID(e)        ((e)->line.usAreaId)
#define EVENTINFO_SYSTEMID(e)      ((e)->line.usSystemId)
#define EVENTINFO_CLIENTID(e)      ((e)->line.usBoardId)
#define EVENTINFO_BOARD(e)         ((e)->line.usBoardId)
#define EVENTINFO_TRUNK(e)         ((e)->line.usTrunkId)
#define EVENTINFO_CHANNEL(e)       ((e)->line.usChannelId)
#if 20071023
#define EVENTINFO_SEQID(e)			((e)->h.snSerNo)
#endif

#endif
