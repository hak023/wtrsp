
#ifndef _RPDK_BASE_H_
#define _RPDK_BASE_H_


#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#	include <sys/time.h>
#	include <unistd.h>
#	include <sys/types.h>
#include "r_types.h"

namespace RED
{
class CFile;
class CString;
class CTime;
class CFileStatus;

/////////////////////////////////////////////////////////////////////////////

const CString &GetEmptyString();

struct CStringData
{
   PDKLONG nRefs;
   PDKLONG nDataLength;
   PDKLONG nAllocLength;
#if 0
   PDKLONG nPAD01;
#endif
   char *data() { return (char *)(this + 1); }
};

class DLLPORT CString
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

   bool TrimLeft(char chTarget);
   bool TrimRight(char chTarget);

   void Mid(CString &strDest, int nFirst, int nCount = 0) const;
   void Left(CString &strDest, int nCount) const;
   void Right(CString &strDest, int nCount) const;

   void Format(LPCSTR pFormat, ...);

//protected:
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

class DLLPORT CXException
{
public:
	CXException(const char *pFormat, ...);
	virtual ~CXException();
	LPCSTR Cause();
protected:
	CString m_strCause;
};

inline CXException::CXException(const char *pFormat, ...)
{
	char *psz = m_strCause.GetBufferSetLength(1024 * 20);

	va_list ap;
	va_start(ap, pFormat);
	vsprintf(psz, pFormat, ap);
	va_end(ap);
}

inline CXException::~CXException()
{
}

inline LPCSTR CXException::Cause()
{
	return (LPCSTR)m_strCause;
}


/////////////////////////////////////////////////////////////////////////////

class DLLPORT CTime
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

class DLLPORT CTime_ : public CTime
{
public:
   CTime_();
   CString Format(LPCSTR pFormat) const;

protected:
   long m_usec;
   int m_msec;
};

///////////////////////////////////////////////////////////////////////////////
struct CFileStatus
{
   CTime m_ctime;     // creation date/time of file
   CTime m_mtime;     // last modification date/time of file
   CTime m_atime;     // last access date/time of file
   PDKLONG m_size;      // logical size of file in bytes
   PDK8 m_szFullName[PATH_MAX]; // absolute path name
                                // PATH_MAX: defined in limits.h
};

class DLLPORT CFile
{
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

   virtual PDKULONG GetPosition() const;
   bool GetStatus(CFileStatus &rStatus) const;
   virtual CString GetFileName() const;

   virtual bool Open(LPCSTR lpszFileName, PDKUINT nOpenFlags, mode_t rmode=0);

   static void Rename(LPCSTR lpszOldName, LPCSTR lpszNewName);
   static void Remove(LPCSTR lpszFileName);
   static bool GetStatus(LPCSTR lpszFileName, CFileStatus &rStatus);

   PDKLONG SeekToEnd();
   void SeekToBegin();

   PDKULONG ReadHuge(void *lpBuffer, PDKULONG dwCount);
   void WriteHuge(const void *lpBuffer, PDKULONG dwCount);

   virtual CFile *Duplicate() const;

   virtual PDKLONG Seek(PDKLONG lOff, PDKUINT nFrom);
   virtual PDKULONG GetLength() const;

   virtual PDKUINT Read(void *lpBuf, PDKUINT nCount);
   virtual PDKUINT Write(const void *lpBuf, PDKUINT nCount);

   virtual void Flush();
   virtual void Close();
   virtual void Abort();

public:
   virtual ~CFile();
   enum BufferCommand { bufferRead, bufferWrite, bufferCommit, bufferCheck };
   virtual PDKUINT GetBufferPtr(PDKUINT nCommand, PDKUINT nCount = 0,
                void **ppBufStart = NULL, void **ppBufMax = NULL);

protected:
   bool m_bCloseOnDelete;
   CString m_strFileName;
};

inline CFile::operator int() const
   { return m_hFile; }
inline PDKULONG CFile::ReadHuge(void *lpBuffer, PDKULONG dwCount)
   { return (PDKULONG)Read(lpBuffer, (PDKUINT)dwCount); }
inline void CFile::WriteHuge(const void *lpBuffer, PDKULONG dwCount)
   { Write(lpBuffer, (PDKUINT)dwCount); }
inline PDKLONG CFile::SeekToEnd()
   { return Seek(0, CFile::end); }
inline void CFile::SeekToBegin()
   { Seek(0, CFile::begin); }

/////////////////////////////////////////////////////////////////////////////
// global helpers (can be overridden)

template<class TYPE>
inline void ConstructElements(TYPE* pElements, int nCount)
{
   // first do bit-wise zero initialization
   memset((void*)pElements, 0, nCount * sizeof(TYPE));

   // then call the constructor(s)
   for (; nCount--; pElements++)
      pElements = (TYPE*)malloc(sizeof(TYPE));
      /*::new((void*)pElements) TYPE;*/
}

template<class TYPE>
inline void DestructElements(TYPE* pElements, int nCount)
{
   // call the destructor(s)
   for (; nCount--; pElements++)
      pElements->~TYPE();
}

template<class TYPE>
inline void CopyElements(TYPE* pDest, const TYPE* pSrc, int nCount)
{
   // default is element-copy using assignment
   while (nCount--)
      *pDest++ = *pSrc++;
}

/////////////////////////////////////////////////////////////////////////////
// CArray<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE>
class DLLPORT CArray
{
public:
// Construction
   CArray();

// Attributes
   int GetSize() const;
   int GetUpperBound() const;
   void SetSize(int nNewSize, int nGrowBy = -1); 

// Operations
   // Clean up
   void FreeExtra();
   void RemoveAll();

   // Accessing elements
   TYPE GetAt(int nIndex) const;
   void SetAt(int nIndex, ARG_TYPE newElement);
   TYPE& ElementAt(int nIndex);

   // Direct Access to the element data (may return NULL)
   const TYPE* GetData() const;
   TYPE* GetData();

   // Potentially growing the array
   void SetAtGrow(int nIndex, ARG_TYPE newElement);
   int Add(ARG_TYPE newElement);
   int Append(const CArray& src);
   void Copy(const CArray& src);

   // overloaded operator helpers
   TYPE operator[](int nIndex) const;
   TYPE& operator[](int nIndex);
   // Added by KMJ
   const CArray<TYPE,ARG_TYPE> &operator=(const CArray &o); 

   // Operations that move elements around
   void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
   void RemoveAt(int nIndex, int nCount = 1);
   void InsertAt(int nStartIndex, CArray* pNewArray);

// Implementation
protected:
   TYPE* m_pData;   // the actual array of data
   int m_nSize;     // # of elements (upperBound - 1)
   int m_nMaxSize;  // max allocated
   int m_nGrowBy;   // grow amount

public:
   ~CArray();
};

/////////////////////////////////////////////////////////////////////////////
// CArray<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
inline int CArray<TYPE, ARG_TYPE>::GetSize() const
   { return m_nSize; }
template<class TYPE, class ARG_TYPE>
inline int CArray<TYPE, ARG_TYPE>::GetUpperBound() const
   { return m_nSize-1; }
template<class TYPE, class ARG_TYPE>
inline void CArray<TYPE, ARG_TYPE>::RemoveAll()
   { SetSize(0, -1); }
template<class TYPE, class ARG_TYPE>
inline TYPE CArray<TYPE, ARG_TYPE>::GetAt(int nIndex) const
   { ASSERT(nIndex >= 0 && nIndex < m_nSize);
      return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
inline void CArray<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE newElement)
   { ASSERT(nIndex >= 0 && nIndex < m_nSize);
      m_pData[nIndex] = newElement; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CArray<TYPE, ARG_TYPE>::ElementAt(int nIndex)
   { ASSERT(nIndex >= 0 && nIndex < m_nSize);
      return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE>
inline const TYPE* CArray<TYPE, ARG_TYPE>::GetData() const
   { return (const TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
inline TYPE* CArray<TYPE, ARG_TYPE>::GetData()
   { return (TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE>
inline int CArray<TYPE, ARG_TYPE>::Add(ARG_TYPE newElement)
   { int nIndex = m_nSize;
      SetAtGrow(nIndex, newElement);
      return nIndex; }
template<class TYPE, class ARG_TYPE>
inline TYPE CArray<TYPE, ARG_TYPE>::operator[](int nIndex) const
   { return GetAt(nIndex); }
template<class TYPE, class ARG_TYPE>
inline TYPE& CArray<TYPE, ARG_TYPE>::operator[](int nIndex)
   { return ElementAt(nIndex); }
template<class TYPE, class ARG_TYPE>
inline const CArray<TYPE,ARG_TYPE> &
CArray<TYPE,ARG_TYPE>::operator=(const CArray &o)
{
   RemoveAll();
   SetSize(o.GetSize());
   for (int i = 0; i < o.GetSize(); i++) 
      (*this)[i] = o[i];
   return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CArray<TYPE, ARG_TYPE> out-of-line functions
         
template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::CArray() 
{     
   m_pData = NULL;
   m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

template<class TYPE, class ARG_TYPE>
CArray<TYPE, ARG_TYPE>::~CArray()
{
   ASSERT_VALID(this);

   if (m_pData != NULL) {
      DestructElements<TYPE>(m_pData, m_nSize);
      delete[] (PDK8U*)m_pData;
   }
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::SetSize(int nNewSize, int nGrowBy)
{
   ASSERT_VALID(this);
   ASSERT(nNewSize >= 0);

   if (nGrowBy != -1)
      m_nGrowBy = nGrowBy;  // set new size

   if (nNewSize == 0) {
      // shrink to nothing
      if (m_pData != NULL) {
         DestructElements<TYPE>(m_pData, m_nSize);
         delete[] (PDK8U*)m_pData;
         m_pData = NULL;
      }
      m_nSize = m_nMaxSize = 0;
   }
   else if (m_pData == NULL) {
      // create one with exact size
#ifdef SIZE_T_MAX
      ASSERT(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow
#endif
      m_pData = (TYPE*) new PDK8U[nNewSize * sizeof(TYPE)];
      ConstructElements<TYPE>(m_pData, nNewSize);
      m_nSize = m_nMaxSize = nNewSize;
   }
   else if (nNewSize <= m_nMaxSize) {
      // it fits
      if (nNewSize > m_nSize) {
         // initialize the new elements
         ConstructElements<TYPE>(&m_pData[m_nSize], nNewSize-m_nSize);
      }
      else if (m_nSize > nNewSize) {
         // destroy the old elements
         DestructElements<TYPE>(&m_pData[nNewSize], m_nSize-nNewSize);
      }
      m_nSize = nNewSize;
   }
   else {
      // otherwise, grow array
      int nGrowBy = m_nGrowBy;
      if (nGrowBy == 0) {
         // heuristically determine growth when nGrowBy == 0
         //  (this avoids heap fragmentation in many situations)
         nGrowBy = m_nSize / 8;
         nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
      }
      int nNewMax;
      if (nNewSize < m_nMaxSize + nGrowBy)
         nNewMax = m_nMaxSize + nGrowBy;  // granularity
      else
         nNewMax = nNewSize;  // no slush

      ASSERT(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
      ASSERT(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
      TYPE* pNewData = (TYPE*) new PDK8U[nNewMax * sizeof(TYPE)];

      // copy new data from old
      memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

      // construct remaining elements
		ASSERT(nNewSize > m_nSize);
      ConstructElements<TYPE>(&pNewData[m_nSize], nNewSize-m_nSize);

      // get rid of old stuff (note: no destructors called)
      delete[] (PDK8U*)m_pData;
      m_pData = pNewData;
      m_nSize = nNewSize;
      m_nMaxSize = nNewMax;
   }
}

template<class TYPE, class ARG_TYPE>
int CArray<TYPE, ARG_TYPE>::Append(const CArray& src)
{
   ASSERT_VALID(this);
   ASSERT(this != &src);   // cannot append to itself

   int nOldSize = m_nSize;
   SetSize(m_nSize + src.m_nSize);
   CopyElements<TYPE>(m_pData + nOldSize, src.m_pData, src.m_nSize);
   return nOldSize;
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::Copy(const CArray& src)
{
   ASSERT_VALID(this);
   ASSERT(this != &src);   // cannot append to itself

   SetSize(src.m_nSize);
   CopyElements<TYPE>(m_pData, src.m_pData, src.m_nSize);
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::FreeExtra()
{
   ASSERT_VALID(this);

   if (m_nSize != m_nMaxSize) {
      // shrink to desired size
#ifdef SIZE_T_MAX
      ASSERT(m_nSize <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
      TYPE* pNewData = NULL;
      if (m_nSize != 0) {
         pNewData = (TYPE*) new PDK8U[m_nSize * sizeof(TYPE)];
         // copy new data from old
         memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));
      }

      // get rid of old stuff (note: no destructors called)
      delete[] (PDK8U*)m_pData;
      m_pData = pNewData;
      m_nMaxSize = m_nSize;
   }
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::SetAtGrow(int nIndex, ARG_TYPE newElement)
{
   ASSERT_VALID(this);
   ASSERT(nIndex >= 0);

   if (nIndex >= m_nSize)
      SetSize(nIndex+1, -1);
   m_pData[nIndex] = newElement;
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::InsertAt(int nIndex, ARG_TYPE newElement, int nCount /*=1*/)
{
   ASSERT_VALID(this);
   ASSERT(nIndex >= 0);    // will expand to meet need
   ASSERT(nCount > 0);     // zero or negative size not allowed

   if (nIndex >= m_nSize) {
      // adding after the end of the array
      SetSize(nIndex + nCount, -1);   // grow so nIndex is valid
   }
   else {
      // inserting in the middle of the array
      int nOldSize = m_nSize;
      SetSize(m_nSize + nCount, -1);  // grow it to new size
      // destroy intial data before copying over it
      DestructElements<TYPE>(&m_pData[nOldSize], nCount);
      // shift old data up to fill gap
      memmove(&m_pData[nIndex+nCount], &m_pData[nIndex],
              (nOldSize-nIndex) * sizeof(TYPE));

      // re-init slots we copied from
      ConstructElements<TYPE>(&m_pData[nIndex], nCount);
   }

   // insert new value in the gap
   ASSERT(nIndex + nCount <= m_nSize);
   while (nCount--)
      m_pData[nIndex++] = newElement;
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount)
{
   ASSERT_VALID(this);
   ASSERT(nIndex >= 0);
   ASSERT(nCount >= 0);
   ASSERT(nIndex + nCount <= m_nSize);

   // just remove a range
   int nMoveCount = m_nSize - (nIndex + nCount);
   DestructElements<TYPE>(&m_pData[nIndex], nCount);
   if (nMoveCount)
      memmove(&m_pData[nIndex], &m_pData[nIndex + nCount],
              nMoveCount * sizeof(TYPE));
   m_nSize -= nCount;
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::InsertAt(int nStartIndex, CArray* pNewArray)
{
   ASSERT_VALID(this);
   ASSERT(pNewArray != NULL);
   ASSERT_VALID(pNewArray);
   ASSERT(nStartIndex >= 0);

   if (pNewArray->GetSize() > 0) {
      InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
      for (int i = 0; i < pNewArray->GetSize(); i++)
         SetAt(nStartIndex + i, pNewArray->GetAt(i));
   }
}

struct CPlex
{
   CPlex *pNext;

   void *data() { return this + 1; }
   static CPlex *Create(CPlex *&head, PDKUINT nMax, PDKUINT cbElement);
   void FreeDataChain();
};

};//namespace RED
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
#endif
