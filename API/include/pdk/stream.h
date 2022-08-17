#ifndef __STREAM_H__
#define __STREAM_H__

#include <stdlib.h>
#include "base.h"

typedef enum {
	soBeginning,
	soCurrent,
	soEnd
} TSeekOrigin;

class CStream : public CObject
{
	DECLARE_SERIAL(CStream)
public:
	CStream() { }
	virtual ~CStream() { }
	inline PDKUINT GetPosition() { return Seek(0, soCurrent); }
	inline void SetPosition(PDKUINT Pos) { Seek(Pos, soBeginning); }
	PDKUINT GetSize();
	inline PDKUINT GetCount() { return 0; }
	inline virtual void SetSize(PDKUINT Size) { return; }
	inline virtual PDKUINT Read(void *Buffer, PDKUINT Count) { return 0; }
	inline virtual PDKUINT Write(const void *Buffer, PDKUINT Count) { return 0; }
	inline virtual PDKINT Seek(PDKUINT Offset, TSeekOrigin Origin) { return 0; }
	inline virtual PDKINT SeekToEnd() { return 0; }
	inline virtual PDK8U *GetData(PDKUINT &Count) { return 0; }
	inline virtual PDK8U *Rewind(PDKUINT &Count) { return 0; }
	inline virtual void SeekToBegin() { }
	inline virtual void Serialize(CArchive &ar) { }
	void ReadBuffer(PDK8U *Buffer, PDKUINT Count);
	void WriteBuffer(PDK8U *Buffer, PDKUINT Count);
};

class CMemoryStream : public CStream
{
	DECLARE_SERIAL(CMemoryStream)
public:
	CMemoryStream() :
		m_pMemory(NULL), m_nSize(0), m_nPosition(0), m_nCapacity(0) { }
	virtual ~CMemoryStream() { if (m_pMemory) free(m_pMemory); }
	PDKUINT GetCount() { return m_nSize - m_nPosition; }
	PDKUINT Read(void *Buffer, PDKUINT Count);
	virtual PDKUINT Write(const void *Buffer, PDKUINT Count);
	PDKINT Seek(PDKUINT Offset, TSeekOrigin Origin);
	PDKINT SeekToEnd();
	void SeekToBegin();
	void SetSize(PDKUINT Size);
	PDK8U *GetMemory() { return m_pMemory; }
	PDK8U *GetData(PDKUINT &Count);
	PDK8U *Rewind(PDKUINT &Count);
	PDKUINT GetCapacity() { return m_nCapacity; }
	virtual void Clear();
	void Serialize(CArchive &ar);
protected:
	void SetMemory(PDK8U *Ptr, PDKUINT Size) { m_pMemory = Ptr; m_nSize = Size; }
	PDK8U *Realloc(PDKUINT &Capacity);
private:
	void SetCapacity(PDKUINT Capacity);
	PDK8U 		*m_pMemory;
	PDKUINT		 m_nSize;
	PDKUINT		 m_nPosition;
	PDKUINT		 m_nCapacity;
};

#endif // __STREAM_H__
