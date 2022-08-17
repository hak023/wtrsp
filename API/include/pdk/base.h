
#ifndef _BASE_H_
#define _BASE_H_

#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef _LINUX_
	#include <sys/time.h>
#endif

#include <unistd.h>
#include <sys/types.h>

#include "types.h"

class CObject;
   class CFile;

class CString;
class CTime;
struct CFileStatus;
class CDumpContext;
class CArchive;

struct CRuntimeClass
{
// Attributes
   LPCSTR m_lpszClassName;
   PDK32 m_nObjectSize;
   PDKUINT m_wSchema;
   CObject *(*m_pfnCreateObject)();
   CRuntimeClass *(*m_pfnGetBaseClass)();
   LPCSTR GetClassName() { return m_lpszClassName; }

// Operations
   CObject *CreateObject();
   bool IsDerivedFrom(const CRuntimeClass *pBaseClass) const;
   static CRuntimeClass *FromName(LPCSTR pszClassName);


// Implementation
   void Store(CArchive& ar) const;
   static CRuntimeClass *Load(CArchive& ar, PDKUINT *pwSchemaNum);

   CRuntimeClass *m_pNextClass; // linked list of registered classes
};

/////////////////////////////////////////////////////////////////////////////

struct CStringData
{
	PDK32 nRefs;
	PDK32 nDataLength;
	PDK32 nAllocLength;
#if 0
	PDK32 nPAD01;
#endif
	char *data() { return (char *)(this + 1); }
};

const CString &GetEmptyString();

class CString
{
public:
	CString();
	CString(const CString &stringSrc);
	CString(const char *pszData);
	~CString();

   int GetLength() const;
   bool IsEmpty() const;
   void Empty();

   char GetAt(int nIndex) const;
   char operator[](int nIndex) const;
   void SetAt(int nIndex, char ch);
   operator LPCSTR() const;
   operator LPSTR() const;

	const CString &operator=(LPCSTR psz);
   const CString &operator=(const CString &stringSrc);
   const CString &operator=(char ch);

	const CString &operator+=(char ch);
	const CString &operator+=(LPCSTR psz);
	const CString &operator+=(const CString &string);

   int Compare(LPCSTR lpsz) const;

	char *GetBuffer(int nMinBufLength);
   char *GetBufferSetLength(int nNewLength);

   int Find(char ch) const;
   int Find(char ch, int nStart) const;
   int Find(LPCSTR lpszSub) const;
   int Find(LPCSTR lpszSub, int nStart) const;
//@~ Add
	LPCSTR Find(char ch, int *nPos) const;
	LPCSTR Find(char ch, int nStart, int *nPos) const;
//~@
   // Add Replace Function by khiin(2011.11.29 by minimei7)
   int Replace(LPCSTR lpszOld, LPCSTR lpszNew);
   int Replace(LPCSTR lpszOld, LPCSTR lpszNew, int nStart);
   // by khiin

   bool TrimLeft(char chTarget);
   bool TrimRight(char chTarget);

	////////////////////////////////////////////////////////////
	//- fish Ãß°¡ 20070802
   void Mid(CString &strDest, int nFirst, int nCount = 0) const;
   void Left(CString &strDest, int nCount) const;
   void Right(CString &strDest, int nCount) const;
   void ReleaseBuffer(int nNewLength = -1);
   void CopyBeforeWrite();
   ////////////////////////////////////////////////////////////
   
   CString Mid(int nFirst, int nCount = 0) const;
   CString Left(int nCount) const;
   CString Right(int nCount) const;

   void Format(LPCSTR pFormat, ...);

#ifdef DEBUG
   friend CDumpContext& operator<<(CDumpContext& dc, const CString& string);
#endif
   friend CArchive& operator<<(CArchive& ar, const CString& string);
   friend CArchive& operator>>(CArchive& ar, CString& string);

protected:
	char *m_pchData;

protected:
	CStringData *GetData() const;
	void Init();
	void AllocBuffer(int nLen);
   void AllocCopy(CString &dest, int nCopyLen,
                  int nCopyIndex, int nExtraLen) const;
	void ConcatCopy(int nSrc1Len, LPCSTR pszSrc1Data,
	                int nSrc2Len, LPCSTR pszSrc2Data);
	void ConcatInPlace(int nSrcLen, LPCSTR pszSrcData);
   void AssignCopy(int nSrcLen, LPCSTR pszSrcData);
	void Release();
	static void Release(CStringData *pData);
	static void FreeData(CStringData *pData);
};

inline int CString::GetLength() const
   { return GetData()->nDataLength; }
inline bool CString::IsEmpty() const
   { return GetData()->nDataLength == 0; }
inline char CString::GetAt(int nIndex) const
{
   assert(nIndex >= 0);
   assert(nIndex < GetData()->nDataLength);
   return m_pchData[nIndex];
}
inline char CString::operator[](int nIndex) const
{
   assert(nIndex >= 0);
   assert(nIndex < GetData()->nDataLength);
   return m_pchData[nIndex];
}
inline CString::operator LPCSTR() const
   { return m_pchData; }
inline CString::operator LPSTR() const
   { return m_pchData; }
inline int CString::Compare(LPCSTR lpsz) const
	{ return strcmp(m_pchData, lpsz); }

#if 0 // this is very dangerous
inline CString operator+(const CString &s1, const CString &s2)
	{  CString s3(s1); s3 += s2; return s3; }
inline CString operator+(const CString &s1, LPCSTR s2)
	{  CString s3(s1); s3 += s2; return s3; }
inline CString operator+(LPCSTR s1, const CString &s2)
	{  CString s3(s1); s3 += s2; return s3; }
#endif

// Compare helpers
inline bool operator==(const CString& s1, const CString& s2)
   { return s1.Compare(s2) == 0; }
inline bool operator==(const CString& s1, LPCSTR s2)
   { return s1.Compare(s2) == 0; }
inline bool operator==(LPCSTR s1, const CString& s2)
   { return s2.Compare(s1) == 0; }
inline bool operator!=(const CString& s1, const CString& s2)
   { return s1.Compare(s2) != 0; }
inline bool operator!=(const CString& s1, LPCSTR s2)
   { return s1.Compare(s2) != 0; }
inline bool operator!=(LPCSTR s1, const CString& s2)
   { return s2.Compare(s1) != 0; }
inline bool operator<(const CString& s1, const CString& s2)
   { return s1.Compare(s2) < 0; }
inline bool operator<(const CString& s1, LPCSTR s2)
   { return s1.Compare(s2) < 0; }
inline bool operator<(LPCSTR s1, const CString& s2)
   { return s2.Compare(s1) > 0; }
inline bool operator>(const CString& s1, const CString& s2)
   { return s1.Compare(s2) > 0; }
inline bool operator>(const CString& s1, LPCSTR s2)
   { return s1.Compare(s2) > 0; }
inline bool operator>(LPCSTR s1, const CString& s2)
   { return s2.Compare(s1) < 0; }
inline bool operator<=(const CString& s1, const CString& s2)
   { return s1.Compare(s2) <= 0; }
inline bool operator<=(const CString& s1, LPCSTR s2)
   { return s1.Compare(s2) <= 0; }
inline bool operator<=(LPCSTR s1, const CString& s2)
   { return s2.Compare(s1) >= 0; }
inline bool operator>=(const CString& s1, const CString& s2)
   { return s1.Compare(s2) >= 0; }
inline bool operator>=(const CString& s1, LPCSTR s2)
   { return s1.Compare(s2) >= 0; }
inline bool operator>=(LPCSTR s1, const CString& s2)
   { return s2.Compare(s1) <= 0; }

/////////////////////////////////////////////////////////////////////////////

class CObject
{
public:
   virtual CRuntimeClass *GetRuntimeClass() const;
	//virtual LPCSTR CObject::GetClassName() const;
	virtual LPCSTR GetClassName() const;
   virtual ~CObject();

   void *operator new(size_t nSize);
   void *operator new(size_t, void *p);
   void operator delete(void *p);
   void operator delete(void *p, void *pPlace);

protected:
   CObject();

private:
   CObject(const CObject &objectSrc);
   void operator=(const CObject &objectSrc);

public:
   bool IsSerializable() const;
   bool IsKindOf(const CRuntimeClass *pClass) const;

   virtual void Serialize(CArchive &ar);

#ifdef DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext &dc) const;
#endif

public:
   static const CRuntimeClass classCObject;
   static CRuntimeClass *_GetBaseClass();
};

inline CObject::CObject()
   { }
inline CObject::~CObject()
   { }
inline void *CObject::operator new(size_t nSize)
   { return ::operator new(nSize); }
inline void *CObject::operator new(size_t, void *p)
   { return p; }
inline void CObject::operator delete(void* p)
	{ ::operator delete(p); }
inline void CObject::operator delete(void* p, void*)
	{ ::operator delete(p); }
inline void CObject::Serialize(CArchive &ar)
   { }

//////////////////////////////////////////////////////
// Helper macros
//////////////////////////////////////////////////////

#define RUNTIME_CLASS(class_name) \
   ((CRuntimeClass *)(&class_name::class##class_name))
#define ASSERT_KINDOF(class_name, object) \
   assert((object)->IsKindOf(RUNTIME_CLASS(class_name)))

#define DECLARE_DYNAMIC(class_name) \
protected: \
   static CRuntimeClass *_GetBaseClass(); \
public: \
   static const CRuntimeClass class##class_name; \
   virtual CRuntimeClass *GetRuntimeClass() const; \

#define _DECLARE_DYNAMIC(class_name) \
protected: \
   static CRuntimeClass *_GetBaseClass(); \
public: \
   static CRuntimeClass class##class_name; \
   virtual CRuntimeClass *GetRuntimeClass() const; \

#define DECLARE_DYNCREATE(class_name) \
   DECLARE_DYNAMIC(class_name) \
   static CObject *CreateObject();

#define _DECLARE_DYNCREATE(class_name) \
   _DECLARE_DYNAMIC(class_name) \
   static CObject *CreateObject();

#define DECLARE_SERIAL(class_name) \
   _DECLARE_DYNCREATE(class_name) \
   friend CArchive& operator>>(CArchive& ar, class_name* &pOb);

void _ClassInit(CRuntimeClass *pNewClass);
struct _CLASSINIT
   { _CLASSINIT(CRuntimeClass *pNewClass) { _ClassInit(pNewClass); } };

#define IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
   CRuntimeClass *class_name::_GetBaseClass() \
      { return RUNTIME_CLASS(base_class_name); } \
   const CRuntimeClass class_name::class##class_name = { \
      #class_name, sizeof(class class_name), wSchema, pfnNew, \
         &class_name::_GetBaseClass, NULL }; \
   CRuntimeClass *class_name::GetRuntimeClass() const \
      { return RUNTIME_CLASS(class_name); } \

#define _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
   CRuntimeClass *class_name::_GetBaseClass() \
      { return RUNTIME_CLASS(base_class_name); } \
   CRuntimeClass class_name::class##class_name = { \
      #class_name, sizeof(class class_name), wSchema, pfnNew, \
         &class_name::_GetBaseClass, NULL }; \
   CRuntimeClass *class_name::GetRuntimeClass() const \
      { return RUNTIME_CLASS(class_name); } \

#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
   IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xffff, NULL)

#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
   CObject *class_name::CreateObject() { return new class_name; } \
   IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xffff, \
      class_name::CreateObject)

#define IMPLEMENT_SERIAL(class_name, base_class_name, wSchema) \
   CObject* class_name::CreateObject() \
      { return new class_name; } \
   _IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, \
      class_name::CreateObject) \
   _CLASSINIT _init_##class_name(RUNTIME_CLASS(class_name)); \
   CArchive& operator>>(CArchive& ar, class_name* &pOb) \
      { pOb = (class_name*) ar.ReadObject(RUNTIME_CLASS(class_name)); \
         return ar; } \

#define VERSIONABLE_SCHEMA  (0x80000000)

/////////////////////////////////////////////////////////////////////////////

class CException
{
public:
	CException(const char *pFormat, ...);
	virtual ~CException();
	LPCSTR Cause();
protected:
	CString m_strCause;
};

inline CException::CException(const char *pFormat, ...)
{
	char *psz = m_strCause.GetBufferSetLength(1024 * 20);

	va_list ap;
	va_start(ap, pFormat);
	vsprintf(psz, pFormat, ap);
	va_end(ap);
}

inline CException::~CException()
{
}

inline LPCSTR CException::Cause()
{
	return (LPCSTR)m_strCause;
}

///////////////////////////////////////////////////////////////////////////////

class CFile : public CObject
{
   DECLARE_DYNAMIC(CFile)

public:
   enum OpenFlags {
      modeRead =        0x0000,
      modeWrite =       0x0001,
      modeReadWrite =   0x0002,
      modeCreate =      0x1000,
      modeNoTruncate =  0x2000,
#if 1 // added by kmj
      modeNoAppend   =  0x5000,
#endif
      typeText =        0x4000,
      typeBinary = (int)0x8000
   };

   enum SeekPosition {
      begin = SEEK_SET,
      current = SEEK_CUR,
      end = SEEK_END
   };

   enum { hFileNull = -1 };

   CFile();
   CFile(int hFile);
   CFile(LPCSTR lpszFileName, PDKUINT nOpenFlags, mode_t rmode=0);

   int m_hFile;
   operator int() const;

   virtual PDK32U GetPosition() const;
   bool GetStatus(CFileStatus &rStatus) const;
   virtual CString GetFileName() const;

   virtual bool Open(LPCSTR lpszFileName, PDKUINT nOpenFlags, mode_t rmode=0);

   static void Rename(LPCSTR lpszOldName, LPCSTR lpszNewName);
   static void Remove(LPCSTR lpszFileName);
   static bool GetStatus(LPCSTR lpszFileName, CFileStatus &rStatus);

   PDK32 SeekToEnd();
   void SeekToBegin();

   PDK32U ReadHuge(void *lpBuffer, PDK32U dwCount);
   void WriteHuge(const void *lpBuffer, PDK32U dwCount);

   virtual CFile *Duplicate() const;

   virtual PDK32 Seek(PDK32 lOff, PDKUINT nFrom);
   virtual PDK32U GetLength() const;

   virtual PDKUINT Read(void *lpBuf, PDKUINT nCount);
   virtual PDKUINT Write(const void *lpBuf, PDKUINT nCount);

   virtual void Flush();
   virtual void Close();
   virtual void Abort();

public:
   virtual ~CFile();
#ifdef DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext &dc) const;
#endif

   enum BufferCommand { bufferRead, bufferWrite, bufferCommit, bufferCheck };
   virtual PDKUINT GetBufferPtr(PDKUINT nCommand, PDKUINT nCount = 0,
                void **ppBufStart = NULL, void **ppBufMax = NULL);

protected:
   bool m_bCloseOnDelete;
   CString m_strFileName;
};

inline CFile::operator int() const
   { return m_hFile; }
inline PDK32U CFile::ReadHuge(void *lpBuffer, PDK32U dwCount)
   { return (PDK32U)Read(lpBuffer, (PDKUINT)dwCount); }
inline void CFile::WriteHuge(const void *lpBuffer, PDK32U dwCount)
   { Write(lpBuffer, (PDKUINT)dwCount); }
inline PDK32 CFile::SeekToEnd()
   { return Seek(0, CFile::end); }
inline void CFile::SeekToBegin()
   { Seek(0, CFile::begin); }

/////////////////////////////////////////////////////////////////////////////

class CTime
{
public:
   static CTime GetCurrentTime();

   CTime();
   CTime(time_t time);
   CTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST = -1);
   CTime(const CTime &timeSrc);

   struct tm *GetLocalTm(struct tm *ptm) const;

   const CTime &operator=(const CTime &timeSrc);
   const CTime &operator=(time_t t);

   time_t GetTime() const;
   int GetYear() const;
   int GetMonth() const; // month of year(1 = Jan)
   int GetDay() const;
   int GetHour() const;
   int GetMinute() const;
   int GetSecond() const;
   int GetDayOfWeek() const; // 1=Sun, 2=Mon, ..., 7=Sat

   bool operator==(CTime time) const;
   bool operator!=(CTime time) const;
   bool operator<(CTime time) const;
   bool operator>(CTime time) const;
   bool operator<=(CTime time) const;
   bool operator>=(CTime time) const;

   CString Format(LPCSTR pFormat) const;
	bool PFormat(LPCSTR pszTime, LPCSTR pFormat);

#ifdef DEBUG
   friend CDumpContext &operator<<(CDumpContext &dc, CTime time);
#endif
	friend CArchive& operator<<(CArchive& ar, CTime time);
	friend CArchive& operator>>(CArchive& ar, CTime& rtime);

protected:
   time_t m_time;
};

#ifdef DEBUG
inline CTime::CTime()
   { }
inline CTime::CTime(const CTime& timeSrc)
   { m_time = timeSrc.m_time; }
#endif
inline CTime::CTime(time_t time)
   { m_time = time; }
inline const CTime& CTime::operator=(const CTime& timeSrc)
   { m_time = timeSrc.m_time; return *this; }
inline const CTime& CTime::operator=(time_t t)
   { m_time = t; return *this; }
inline time_t CTime::GetTime() const
   { return m_time; }
inline int CTime::GetYear() const
   { struct tm tmTemp; return GetLocalTm(&tmTemp)->tm_year + 1900; }
inline int CTime::GetMonth() const
   { struct tm tmTemp; return GetLocalTm(&tmTemp)->tm_mon + 1; }
inline int CTime::GetDay() const
   { struct tm tmTemp; return GetLocalTm(&tmTemp)->tm_mday; }
inline int CTime::GetHour() const
   { struct tm tmTemp; return GetLocalTm(&tmTemp)->tm_hour; }
inline int CTime::GetMinute() const
   { struct tm tmTemp; return GetLocalTm(&tmTemp)->tm_min; }
inline int CTime::GetSecond() const
   { struct tm tmTemp; return GetLocalTm(&tmTemp)->tm_sec; }
inline int CTime::GetDayOfWeek() const
   { struct tm tmTemp; return GetLocalTm(&tmTemp)->tm_wday + 1; }
inline bool CTime::operator==(CTime time) const
   { return m_time == time.m_time; }
inline bool CTime::operator!=(CTime time) const
   { return m_time != time.m_time; }
inline bool CTime::operator<(CTime time) const
   { return m_time < time.m_time; }
inline bool CTime::operator>(CTime time) const
   { return m_time > time.m_time; }
inline bool CTime::operator<=(CTime time) const
   { return m_time <= time.m_time; }
inline bool CTime::operator>=(CTime time) const
   { return m_time >= time.m_time; }

/////////////////////////////////////////////////////////////////////////////

class CTime_ : public CTime
{
public:
   CTime_();
   CString Format(LPCSTR pFormat) const;

protected:
   long m_usec;
   int m_msec;
};

/////////////////////////////////////////////////////////////////////////////

struct CFileStatus
{
   CTime m_ctime;     // creation date/time of file
   CTime m_mtime;     // last modification date/time of file
   CTime m_atime;     // last access date/time of file
   PDK32 m_size;      // logical size of file in bytes
   PDK8 m_szFullName[PATH_MAX]; // absolute path name
                                // PATH_MAX: defined in limits.h
#ifdef DEBUG
   void Dump(CDumpContext &dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

class CDumpContext
{
public:
#ifdef DEBUG
   static void EnableTrace(bool bEnable = true);
#endif
   CDumpContext(CFile *pFile = NULL);

   int GetDepth() const; // 0: this object, 1: children objects
   void SetDepth(int nNewDepth);

   CDumpContext &operator<<(LPCSTR lpsz);
   CDumpContext &operator<<(PDK8U by);
   CDumpContext &operator<<(PDK16U w);
   CDumpContext &operator<<(PDKUINT u);
   CDumpContext &operator<<(PDK32 l);
   CDumpContext &operator<<(PDK32U dw);
   CDumpContext &operator<<(float f);
   CDumpContext &operator<<(double d);
   CDumpContext &operator<<(int n);
   CDumpContext &operator<<(bool b);
   CDumpContext &operator<<(const CObject *pOb);
   CDumpContext &operator<<(const CObject &ob);
   CDumpContext &operator<<(const void *lp);
   void HexDump(LPCSTR lpszLine, PDK8U *pby, int nBytes, int nWidth);
   void Flush();

protected:
   CDumpContext(const CDumpContext &dcSrc);
   void operator=(const CDumpContext &dcSrc);
   void OutputString(LPCSTR lpsz);

   int m_nDepth;
#ifdef DEBUG
   static bool m_bTraceEnabled;
#endif

public:
   CFile *m_pFile;
};

inline int CDumpContext::GetDepth() const
   { return m_nDepth; }
inline void CDumpContext::SetDepth(int nNewDepth)
   { m_nDepth = nNewDepth; }

void OutputDebugString(LPCSTR lpsz);

/////////////////////////////////////////////////////////////////////////////

class CPtrArray;
class CMapPtrToPtr;

class CStream;

class CArchive
{
public:
   enum Mode { store = 0, load = 1 };
   enum { nBufSizeMin = 128 };
   enum { nGrowSize = 64 };
   enum { nHashSize = 137 };
   enum { nBlockSize = 16 };

   CArchive(CFile *pFile, PDKUINT nMode, int nBufSize = 4096);
   CArchive(CStream *pStream, PDKUINT nMode, int nBufSize = 4096);
   ~CArchive();

   bool IsLoading() const;
   bool IsStoring() const;

   CFile *GetFile() const;
   CStream *GetStream() const;
   void SetObjectSchema(PDKUINT nSchema);

   void Flush();
   void Close();
   void Abort();

   PDKUINT Read(void *pBuf, PDKUINT nMax);
   void Write(const void *pBuf, PDKUINT nMax);

   void WriteString(LPCSTR lpsz);
   LPSTR ReadString(LPSTR lpsz, PDKUINT nMax);
   bool ReadString(CString &rString);

   void CheckCount();
   PDK32U ReadCount();
   void WriteCount(PDK32U dwCount);

   void FillBuffer(PDKUINT nBytesNeeded);

	friend CArchive& operator<<(CArchive& ar, const CObject* pOb);

	friend CArchive& operator>>(CArchive& ar, CObject*& pOb);
	friend CArchive& operator>>(CArchive& ar, const CObject*& pOb);

	// insertion operations
	CArchive& operator<<(PDK8U by);
	CArchive& operator<<(PDK16U w);
	CArchive& operator<<(PDK32 l);
	CArchive& operator<<(PDK32U dw);
	CArchive& operator<<(float f);
	CArchive& operator<<(double d);

	CArchive& operator<<(int i);
	CArchive& operator<<(short w);
	CArchive& operator<<(char ch);
	CArchive& operator<<(unsigned u);
   CArchive& operator<<(bool b);

	// extraction operations
	CArchive& operator>>(PDK8U& by);
	CArchive& operator>>(PDK16U& w);
	CArchive& operator>>(PDK32& l);
	CArchive& operator>>(PDK32U& dw);
	CArchive& operator>>(float& f);
	CArchive& operator>>(double& d);

	CArchive& operator>>(int& i);
	CArchive& operator>>(short& w);
	CArchive& operator>>(char& ch);
	CArchive& operator>>(unsigned& u);
   CArchive& operator>>(bool& b);

	CObject* ReadObject(const CRuntimeClass* pClass);
	void WriteObject(const CObject* pOb);
   void MapObject(const CObject *pOb);

   void WriteClass(const CRuntimeClass *pClassRef);
   CRuntimeClass *ReadClass(const CRuntimeClass *pClassRefRequested,
                            PDKUINT *pSchema, PDK32U *pObTag);

public:
   PDKUINT m_nObjectSchema;
   CString m_strFileName;

protected:
   CArchive(const CArchive& arSrc);
   void operator=(const CArchive& arSrc);

   PDKUINT m_nMode;
   CFile *m_pFile;
   CStream *m_pStream;
   int m_nBufSize;
   PDK8U *m_pBufStart;
   PDK8U *m_pBufCur;
   PDK8U *m_pBufMax;

   PDKUINT m_nMapCount;
   union {
      CPtrArray *m_pLoadArray;
      CMapPtrToPtr *m_pStoreMap;
   };
   CMapPtrToPtr *m_pSchemaMap;

   PDKUINT m_nGrowSize;
   PDKUINT m_nHashSize;
};

inline CArchive::CArchive(const CArchive& /* arSrc */)
	{ }
inline void CArchive::operator=(const CArchive& /* arSrc */)
	{ }
inline CArchive& operator<<(CArchive& ar, const CObject* pOb)
	{ ar.WriteObject(pOb); return ar; }
inline CArchive& operator>>(CArchive& ar, CObject*& pOb)
	{ pOb = ar.ReadObject(NULL); return ar; }
inline CArchive& operator>>(CArchive& ar, const CObject*& pOb)
	{ pOb = ar.ReadObject(NULL); return ar; }

inline bool CArchive::IsLoading() const
   { return (m_nMode & CArchive::load) != 0; }
inline bool CArchive::IsStoring() const
   { return (m_nMode & CArchive::load) == 0; }
inline CFile *CArchive::GetFile() const
   { return m_pFile; }
inline CStream *CArchive::GetStream() const
   { return (CStream *)m_pStream; }
inline void CArchive::SetObjectSchema(PDKUINT nSchema)
   { m_nObjectSchema = nSchema; }
inline CArchive& CArchive::operator<<(PDK8U by)
   { if ((m_pBufCur + sizeof(PDK8U)) > m_pBufMax) Flush();
     *(PDK8U *)m_pBufCur = by; m_pBufCur += sizeof(PDK8U); return *this; }
inline CArchive& CArchive::operator<<(PDK16U w)
   { if ((m_pBufCur + sizeof(PDK16U)) > m_pBufMax) Flush();
     memmove(m_pBufCur, (char *)&w, sizeof(PDK16U));
     m_pBufCur += sizeof(PDK16U); return *this; }
inline CArchive& CArchive::operator<<(PDK32 l)
   { if ((m_pBufCur + sizeof(PDK32)) > m_pBufMax) Flush();
     memmove(m_pBufCur, (char *)&l, sizeof(PDK32));
     m_pBufCur += sizeof(PDK32); return *this; }
inline CArchive& CArchive::operator<<(PDK32U dw)
   { if ((m_pBufCur + sizeof(PDK32U)) > m_pBufMax) Flush();
     memmove(m_pBufCur, (char *)&dw, sizeof(PDK32U));
     m_pBufCur += sizeof(PDK32U); return *this; }
inline CArchive& CArchive::operator<<(float f)
   { if ((m_pBufCur + sizeof(float)) > m_pBufMax) Flush();
     memmove(m_pBufCur, (char *)&f, sizeof(float));
     m_pBufCur += sizeof(float); return *this; }
inline CArchive& CArchive::operator<<(double d)
   { if ((m_pBufCur + sizeof(double)) > m_pBufMax) Flush();
     memmove(m_pBufCur, (char *)&d, sizeof(double));
     m_pBufCur += sizeof(double); return *this; }
inline CArchive& CArchive::operator<<(int i)
   { return CArchive::operator<<((PDK32)i); }
inline CArchive& CArchive::operator<<(short w)
   { return CArchive::operator<<((PDK16U)w); }
inline CArchive& CArchive::operator<<(char ch)
   { return CArchive::operator<<((PDK8U)ch); }
inline CArchive& CArchive::operator<<(unsigned u)
   { return CArchive::operator<<((PDK32)u); }
inline CArchive& CArchive::operator<<(bool b)
   { return CArchive::operator<<((PDK32)b); }

inline CArchive& CArchive::operator>>(PDK8U& by)
   { if ((m_pBufCur + sizeof(PDK8U)) > m_pBufMax)
         FillBuffer(sizeof(PDK8U) - (PDKUINT)(m_pBufMax - m_pBufCur));
     by = *(PDK8U*)m_pBufCur; m_pBufCur += sizeof(PDK8U); return *this; }
inline CArchive& CArchive::operator>>(PDK16U& w)
   { if ((m_pBufCur + sizeof(PDK16U)) > m_pBufMax)
         FillBuffer(sizeof(PDK16U) - (PDKUINT)(m_pBufMax - m_pBufCur));
     memmove((char *)&w, m_pBufCur, sizeof(PDK16U));
     m_pBufCur += sizeof(PDK16U); return *this; }
inline CArchive& CArchive::operator>>(PDK32& l)
   { if ((m_pBufCur + sizeof(PDK32)) > m_pBufMax)
         FillBuffer(sizeof(PDK32) - (PDKUINT)(m_pBufMax - m_pBufCur));
     memmove((char *)&l, m_pBufCur, sizeof(PDK32));
     m_pBufCur += sizeof(PDK32); return *this; }
inline CArchive& CArchive::operator>>(PDK32U& dw)
   { if ((m_pBufCur + sizeof(PDK32U)) > m_pBufMax)
         FillBuffer(sizeof(PDK32U) - (PDKUINT)(m_pBufMax - m_pBufCur));
     memmove((char *)&dw, m_pBufCur, sizeof(PDK32U));
     m_pBufCur += sizeof(PDK32U); return *this; }
inline CArchive& CArchive::operator>>(float& f)
   { if ((m_pBufCur + sizeof(float)) > m_pBufMax)
         FillBuffer(sizeof(float) - (PDKUINT)(m_pBufMax - m_pBufCur));
     memmove((char *)&f, m_pBufCur, sizeof(float));
     m_pBufCur += sizeof(float); return *this; }
inline CArchive& CArchive::operator>>(double& d)
   { if ((m_pBufCur + sizeof(double)) > m_pBufMax)
	      FillBuffer(sizeof(double) - (PDKUINT)(m_pBufMax - m_pBufCur));
     memmove((char *)&d, m_pBufCur, sizeof(double));
     m_pBufCur += sizeof(double); return *this; }
inline CArchive& CArchive::operator>>(int& i)
   { return CArchive::operator>>((PDK32&)i); }
inline CArchive& CArchive::operator>>(short& w)
   { return CArchive::operator>>((PDK16U&)w); }
inline CArchive& CArchive::operator>>(char& ch)
   { return CArchive::operator>>((PDK8U&)ch); }
inline CArchive& CArchive::operator>>(unsigned& u)
   { return CArchive::operator>>((PDK32&)u); }
inline CArchive& CArchive::operator>>(bool& b)
#if 0
   { return CArchive::operator>>((PDK32&)b); }
#else
   { PDK32 i; *this=CArchive::operator>>(i); b=(bool)i; return *this; }
#endif
/////////////////////////////////////////////////////////////////////////////

struct CPlex
{
   CPlex *pNext;

   void *data() { return this + 1; }
   static CPlex *Create(CPlex *&head, PDKUINT nMax, PDKUINT cbElement);
   void FreeDataChain();
};

#endif
