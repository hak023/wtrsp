#ifndef __MSGQUEUE_H__
#define __MSGQUEUE_H__

/******************************************************************************
* SYNOPSIS: Kernel tuning parameters for message queues
* DETAIL DESCRIPTION:
* 
* This is a quick description of the tunable message queue parameters
* available in Solaris 2.X.
* 
* The tunable parameters are documented in the manuals:
* 
*   Solaris 2.1: SunOS 5.1 Administering Security, Performance, and 
*   Accounting (Appendix A)
*   Solaris 2.2: SunOS 5.2 Administering Security, Performance, and 
*   Accounting (Appendix A)
*   Solaris 2.3:  Solaris 2.3  Administering Security, Performance, and
*   Accounting (Appendix A)
*   Solaris 2.4:  Security, Performance, and Accounting Administration
*   (Appendix A)
*   Solaris 2.5: System Administration Guide, Volume 2 (Chapter 73)
* 
* 
* The variables are:
* 
*         msginfo_msgmap (100)    Number of entries in the message map
*         msginfo_msgmax (2048)   Maximum message size
*         msginfo_msgmnb (4096)   Maximum bytes on queue
*         msginfo_msgmni (50)     Number of message queue identifiers
*         msginfo_msgssz (8)      Segment size of a message
*         msginfo_msgtql (40)     Number of system message headers
*         msginfo_msgseg (1024)   Number of message segments 
*     (MUST BE < 32768)
* 
* These can be set in /etc/system using the syntax:
* 
*         set msgsys:X = N
* e.g.,
*       set msgsys:msginfo_msgseg=4096
* 
* In the rest of this document, the "msginfo_" part of the name above
* will be omitted for brevity.
* 
* msgmax: ( default 2048 )
* 
*   The parameter "msgmax" is the maximum number of bytes that can be in
*   any one message. msgsnd() will give the error EINVAL if a message is
*   longer than this.
* 
* msgmnb: ( default 4096 )
* 
*   The parameter "msgmnb" is the maximum number of message bytes that
*   can be on any one queue at a given time. If a message would cause a
*   queue to grow larger than this, that msgsnd() would either block
*   until space was available, or would give the error EAGAIN if user
*   specified non-blocking mode. For obvious reasons "msgmnb" must be at
*   least as large as "msgmax", however, it is probably good to set it to
*   some integral number of "msgmax." This way, a few messages of the
*   maximum size could be queued at any time. However, making "msgmnb"
*   too large compared to the total bytes available for messages (see
*   below) may allow one queue to hog the message system.
* 
* msgmni: ( default 50 )
* 
*   The parameter "msgmni" is the number of message queue ids available
*   to the system. Each message queue requires one id. msgget() gives
*   the error ENOSPC if all the ids have been used up.
* 
* msgtql: ( default 40 )
* 
*   The parameter "msgtql" is the number of message queue headers
*   available to the system. Each queued but as yet unread message
*   requires one header. If none are available, msgsnd() will either
*   block or give the error EAGAIN if non-blocking mode has been
*   specified.
* 
* msgssz & msgseg:  ( defaults, 8 and 1024, respectively )
* 
*   The parameters "msgssz" and "msgseg" together specify the total
*   number of bytes available for all messages on all queues. The reason
*   for two parameters is that the system allocates space to messages in
*   discrete units called segments. Each segment contains "msgssz"
*   bytes, and the system allocates "msgseg" of these segments.
*   Therefore, the total number of bytes available on all queues is
*   (msgssz * msgseg).
* 
* msgmap:  ( default 100 )
* 
*   The parameter "msgmap" specifies the number of entries in the map
*   that allocates message segments to messages. Each contiguous block
*   of free segments requires one entry in the map. When segments are
*   freed, if they are adjacent to another block of free segments, the
*   two entries are combined. When a message is queued, a contiguous
*   block of segments is required to hold the message. If many messages
*   of varying lengths are being sent, it is possible that there is no
*   single block of free segments available. Thus it is possible to "run
*   out" of free space even if not all of the message arena is currently
*   being used. If user gets a message on the console like: "msgmap:
*   rmap ovflo, lost ...", what has happened is that all the entries in
*   the map are in use and when the system tried to free a message's
*   segments, it couldn't create a new entry in the map. When this
*   occurs, the system discards that space, which means that it is
*   permanently lost to the system until the message system is
*   reinitialized.
******************************************************************************/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "jthread.h"
#include "logger.h"

#define PERMIT 0660

typedef enum {
	MSG_SEND_TYPE = 1,
	MSG_RECV_TYPE,
	MSG_NONE_TYPE = 0xfffe
} MSG_QUEUE_TYPE;

class CQThread;
typedef bool (*PMsgQueueRecv)(void *pOwner, void *pBuffer, int nLen);
template <class P, class T> class CMsgQueue : public CThread
{
protected:
	typedef struct {
		long mtype;
		T    mbody;
	} MSGELEM, *PMSGELEM; 

	PMSGELEM				m_pSendElem;
	PMSGELEM				m_pRecvElem;
	int					m_nQueueId;
	MSG_QUEUE_TYPE		m_nSendId;
	MSG_QUEUE_TYPE		m_nRecvId;
	PMsgQueueRecv		m_pMsgQueueRecv;
	CQThread				*m_pOwner;
	CLogger				*m_pLogger;

public:
	CMsgQueue() :
		  m_pSendElem(new MSGELEM),
		  m_pRecvElem(new MSGELEM),
		  m_nQueueId(-1),
		  m_nSendId(MSG_SEND_TYPE),
		  m_nRecvId(MSG_RECV_TYPE),
		  m_pMsgQueueRecv(NULL),
		  m_pOwner(NULL),
		  m_pLogger(CLogger::GetInstance())
	{
	}
	~CMsgQueue() {
		delete m_pSendElem;
		delete m_pRecvElem;
	}
	int GetQueueId() { return m_nQueueId; }
	void SetSendId(MSG_QUEUE_TYPE nId) { m_nSendId = nId; }
	MSG_QUEUE_TYPE GetSendId() { return m_nSendId; }
	void SetRecvId(MSG_QUEUE_TYPE nId) { m_nRecvId = nId; }
	MSG_QUEUE_TYPE GetRecvId() { return m_nRecvId; }
	void SetOwner(CQThread *pOwner) { m_pOwner = pOwner; }
	CQThread *GetOwner() { return m_pOwner; }
	void Remove() { msgctl(m_nQueueId, IPC_RMID, 0); }
	bool Start(const char *path, int id, PMsgQueueRecv pCallBack = NULL)
	{
		m_pMsgQueueRecv = pCallBack;
		key_t key = ftok(path, id);
		m_nQueueId = msgget(key, IPC_CREAT|PERMIT);
		if (m_nQueueId == -1) {
			m_pLogger->Log(__FILE__, __LINE__, CLogger::Error,
								"msgget error[%d:%s]", errno, strerror(errno));
			exit(-1);
		}
		return CThread::Create();
	}
	int Send(P elem, int type = -1) {
		if (type == -1) type = m_nSendId;
		m_pSendElem->mtype = type;

		memcpy(&m_pSendElem->mbody, elem, sizeof(T));
		int nLen = msgsnd(m_nQueueId, (struct msgbuf *)m_pSendElem, sizeof(T), 0);
		if (nLen == -1) {
			m_pLogger->Log(CLogger::Error,
								"msgsnd error[%d:%s]", errno, strerror(errno));
		}
		return nLen;
	}
	void *ThreadProc() {
		while (!DoExit()) {
			m_pRecvElem->mtype = m_nRecvId;

			int nLen = msgrcv(m_nQueueId, (struct msgbuf *)m_pRecvElem,
									sizeof(T), m_pRecvElem->mtype, 0);
			if (nLen == -1) {
				if (errno == EINTR)
					continue;

				m_pLogger->Log(__FILE__, __LINE__, CLogger::Error,
									"msgrcv error[%d:%s]", errno, strerror(errno));
				if (errno == E2BIG)
					m_pLogger->Log(CLogger::Error,
										"Recv Size[%d:%d]",
										sizeof(m_pRecvElem->mbody),
										sizeof(T));
				continue;
			}
			if (m_pMsgQueueRecv)
				m_pMsgQueueRecv((void *)m_pOwner,
									(void *)&m_pRecvElem->mbody, sizeof(T));
		}
		return NULL;
	}
};

#endif // __MSGQUEUE_H__
