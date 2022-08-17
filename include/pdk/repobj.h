
#ifndef __SIP_REPLICATION_H__
#define __SIP_REPLICATION_H__

#include "base.h"
#include "stream.h"
#include "udpserver.h"
#include "sharedmemory.h"

#define CI_OBJ_REPLICA	0xabba

void ObjToStream(CObject *pObject, CMemoryStream *pStream);
void StreamToObj(CObject *&pObject, CMemoryStream *pStream);
void MemoryToObj(CObject *&pObject, LPSTR ptr, PDKINT nLen);

///////////////////////////////////////////////////////////////////////////////
template <class T> class CReplicaInterface : public CShmQInterface<T>
{
public:
   CReplicaInterface(PDK16U nId, char *pPath) :
		CShmQInterface<T>(pPath), m_nSenderId(nId) { }
	CReplicaInterface(PDK16U nId, char *pPath, int nBufferLen) :
		CShmQInterface<T>(pPath, nBufferLen), m_nSenderId(nId) { }
	CReplicaInterface(PDK16U nId, int nKey) :
		CShmQInterface<T>(nKey), m_nSenderId(nId) { }
	CReplicaInterface(PDK16U nId, int nKey, int nBufferLen) :
		CShmQInterface<T>(nKey, nBufferLen), m_nSenderId(nId) { }

	bool Add(T *pEvent);
	bool AddObject(PDK16U type, PDK16U stype, CObject *pObject);
	PDK16U GetSenderId() { return m_nSenderId; }
protected:
	PDK16U				m_nSenderId;
};

///////////////////////////////////////////////////////////////////////////////
template <class T> class CReplicaReceiver : public CShmQThread<T>
{
public:
   CReplicaReceiver(char *pPath) :
		CShmQThread<T>(pPath) { }
	CReplicaReceiver(char *pPath, int nBufferLen) :
		CShmQThread<T>(pPath, nBufferLen) { }
	CReplicaReceiver(int nKey) :
		CShmQThread<T>(nKey) { }
	CReplicaReceiver(int nKey, int nBufferLen) :
		CShmQThread<T>(nKey, nBufferLen) { }
	void SetOwner(CThread *pOwner) { m_pOwner = pOwner; }
	CThread *GetOwner() { return m_pOwner; }
	static void GetObject(T *, CObject *&);
protected:
	virtual void *ThreadProc();
	CThread							*m_pOwner;
	CLogger							*m_pLogger;
};

///////////////////////////////////////////////////////////////////////////////
class CReplica : public CIUdpServer
{
	DECLARE_DYNAMIC(CReplica)
public:
	~CReplica();
	bool Start(LPCSTR pszAddr, PDKINT nPort, LPCSTR pszDest, PDKINT nDest, PDKINT keyShm);
	virtual bool AddInterface(PDKINT nId, PDKINT keyShm);
	virtual int RecvHandle(void *Buffer, PDKINT nLen);
	virtual int SendHandle(void *Buffer, PDKINT nLen);
	virtual int SendData(void *ptr, PDKINT nLen);
	virtual int SendData(void *ptr, PDKINT nLen, PDK16U nType, PDK16U nSubType = 0);
	virtual int SendObject(CObject *ptr);
protected:
	virtual void SendObjectHandle(CObject *pObject, PEVENTINFO pEvent) { }
	static int AfterReceive(void *ptr, void *pBuffer, void *pLen);
	static int BeforeSend(void *ptr, void *pBuffer, void *pLen);

	CString				 m_strDestAddr;
	PDKINT				 m_nDestPort;
	CMapPtrToPtr		 m_MapInterface;
	CReplicaReceiver<EVENTINFO>	*m_pReplicaReceiver;
};

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline bool CReplicaInterface<T>::Add(T *pEvent)
{
	EVENTINFO_SENDER(pEvent) = m_nSenderId;
	return CShmQInterface<T>::Add(pEvent);
}

template <class T>
inline bool CReplicaInterface<T>::AddObject(PDK16U type, PDK16U stype,
	CObject *pObject)
{
	CMemoryStream mStream;
	ObjToStream(pObject, &mStream);

	bool bRet = CShmQInterface<T>::AddEvent(type, stype, (char *)mStream.GetMemory(),
		(PDKINT)mStream.GetSize());
	if (bRet == false) {
		fprintf(stderr, "CReplicaInterface::AddObject AddEvent Error\n");
	}
	return bRet;
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline void CReplicaReceiver<T>::GetObject(T *pInfo, CObject *&pObject)
{
	::MemoryToObj(pObject, pInfo->Value, EVENTINFO_VALUE_LENGTH(pInfo));
}

template <class T>
inline void *CReplicaReceiver<T>::ThreadProc()
{
	m_pLogger = CLogger::GetInstance();

	while (!CThread::DoExit()) {
		T *pEvent = CShmQInterface<T>::Remove();
		assert(pEvent != NULL);

		switch (EVENTINFO_TYPES(pEvent)) {
			case CI_QUIT_THREAD: {
				break;
			}
			case CI_OBJ_REPLICA: {
				if (m_pOwner == NULL)
					break;

				CRuntimeClass *pClass = m_pOwner->GetRuntimeClass();
				if (pClass->IsDerivedFrom(RUNTIME_CLASS(CQThread)) == true) {
					((CQThread *)m_pOwner)->Add(pEvent);
				} else
				if (pClass->IsDerivedFrom(RUNTIME_CLASS(CThread)) == true) {
					if (m_pOwner->IsKindOf(RUNTIME_CLASS(CReplica))) {
						((CReplica *)m_pOwner)->SendData(pEvent,
							EVENTINFO_LENGTH(pEvent));
					}
				} else
				{
				}
				break;
			}
			default: {
				m_pLogger->Log(CLogger::Error,
									"CReplicaReceiver::ThreadProc Error"
									"[Unknown Event %x]",
									EVENTINFO_TYPES(pEvent));
			}
		}
	}
	return NULL;
}

#endif //__SIP_REPLICATION_H__
