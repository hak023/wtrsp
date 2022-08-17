
#ifndef _BASE_LIST_H_
#define _BASE_LIST_H_

#include <assert.h>
#include <new>
#include <stdio.h>
#include "types.h"
#include "base.h"

// abstract iteration position
struct __POSITION { };
typedef __POSITION* POSITION;

#define BEFORE_START_POSITION ((POSITION)-1L)

class CSimpleList
{
public:
   CSimpleList(int nNextOffset = 0);
   void Construct(int nNextOffset);

   bool IsEmpty() const;
   void AddHead(void *p);
   void RemoveAll();
   void *GetHead() const;
   void *GetNext(void *p) const;
   bool Remove(void *p);

   void *m_pHead;
   size_t m_nNextOffset;

   void **GetNextPtr(void *p) const;
};

inline CSimpleList::CSimpleList(int nNextOffset)
   { m_pHead = NULL; m_nNextOffset = nNextOffset; }
inline void CSimpleList::Construct(int nNextOffset)
   { assert(m_pHead == NULL); m_nNextOffset = nNextOffset; }
inline bool CSimpleList::IsEmpty() const
   { return m_pHead == NULL; }
inline void **CSimpleList::GetNextPtr(void *p) const
   { assert(p != NULL); return (void **)((PDK8U *)p + m_nNextOffset); }
inline void CSimpleList::RemoveAll()
   { m_pHead = NULL; }
inline void *CSimpleList::GetHead() const
   { return m_pHead; }
inline void *CSimpleList::GetNext(void *prevElement) const
   { return *GetNextPtr(prevElement); }

/////////////////////////////////////////////////////////////////////////////

template<class TYPE>
class CTypedSimpleList : public CSimpleList
{
public:
	CTypedSimpleList(int nNextOffset = 0)
		: CSimpleList(nNextOffset) { }
	void AddHead(TYPE p)
		{ CSimpleList::AddHead(p); }
	TYPE GetHead()
		{ return (TYPE)CSimpleList::GetHead(); }
	TYPE GetNext(TYPE p)
		{ return (TYPE)CSimpleList::GetNext(p); }
	bool Remove(TYPE p)
		{ return CSimpleList::Remove((TYPE)p); }
	operator TYPE()
		{ return (TYPE)CSimpleList::GetHead(); }
};

/////////////////////////////////////////////////////////////////////////////

class CPtrList : public CObject
{
   DECLARE_DYNAMIC(CPtrList)

protected:
   struct CNode {
      CNode *pNext;
      CNode *pPrev;
      void *data;
   };

public:
// Construction
   CPtrList(int nBlockSize = 10);

// Attributes (head and tail)
   // count of elements
   int GetCount() const;
   bool IsEmpty() const;

   // peek at head or tail
   void *& GetHead();
   void *GetHead() const;
   void *& GetTail();
   void *GetTail() const;

// Operations
   // get head or tail (and remove it) - don't call on empty list!
   void *RemoveHead();
   void *RemoveTail();

   // add before head or after tail
   POSITION AddHead(void *newElement);
   POSITION AddTail(void *newElement);

   // add another list of elements before head or after tail
   void AddHead(CPtrList *pNewList);
   void AddTail(CPtrList *pNewList);

   // remove all elements
   void RemoveAll();

   // iteration
   POSITION GetHeadPosition() const;
   POSITION GetTailPosition() const;
   void *&GetNext(POSITION &rPosition); // return *Position++
   void *GetNext(POSITION &rPosition) const; // return *Position++
   void *&GetPrev(POSITION &rPosition); // return *Position--
   void *GetPrev(POSITION &rPosition) const; // return *Position--

   // getting/modifying an element at a given position
   void *&GetAt(POSITION position);
   void *GetAt(POSITION position) const;
   void SetAt(POSITION pos, void *newElement);

   void RemoveAt(POSITION position);

   // inserting before or after a given position
   POSITION InsertBefore(POSITION position, void *newElement);
   POSITION InsertAfter(POSITION position, void *newElement);

   // helper functions (note: O(n) speed)
   POSITION Find(void *searchValue, POSITION startAfter = NULL) const;
	               // defaults to starting at the HEAD
	               // return NULL if not found
   POSITION FindIndex(int nIndex) const;
	               // get the 'nIndex'th element (may return NULL)

// Implementation
protected:
   CNode *m_pNodeHead;
   CNode *m_pNodeTail;
   int m_nCount;
   CNode *m_pNodeFree;
   struct CPlex *m_pBlocks;
   int m_nBlockSize;

   CNode *NewNode(CNode *, CNode *);
   void FreeNode(CNode *);

public:
   ~CPtrList();
#ifdef DEBUG
   void Dump(CDumpContext&) const;
   void AssertValid() const;
#endif
   // local typedefs for class templates
   typedef void *BASE_TYPE;
   typedef void *BASE_ARG_TYPE;
};

inline int CPtrList::GetCount() const
	{ return m_nCount; }
inline bool CPtrList::IsEmpty() const
	{ return m_nCount == 0; }
inline void*& CPtrList::GetHead()
	{ assert(m_pNodeHead != NULL); return m_pNodeHead->data; }
inline void* CPtrList::GetHead() const
	{ assert(m_pNodeHead != NULL); return m_pNodeHead->data; }
inline void*& CPtrList::GetTail()
	{ assert(m_pNodeTail != NULL); return m_pNodeTail->data; }
inline void* CPtrList::GetTail() const
	{ assert(m_pNodeTail != NULL); return m_pNodeTail->data; }
inline POSITION CPtrList::GetHeadPosition() const
	{ return (POSITION) m_pNodeHead; }
inline POSITION CPtrList::GetTailPosition() const
	{ return (POSITION) m_pNodeTail; }
inline void*& CPtrList::GetNext(POSITION& rPosition) // return *Position++
	{ CNode* pNode = (CNode*) rPosition;
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
inline void* CPtrList::GetNext(POSITION& rPosition) const // return *Position++
	{ CNode* pNode = (CNode*) rPosition;
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
inline void*& CPtrList::GetPrev(POSITION& rPosition) // return *Position--
	{ CNode* pNode = (CNode*) rPosition;
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
inline void* CPtrList::GetPrev(POSITION& rPosition) const // return *Position--
	{ CNode* pNode = (CNode*) rPosition;
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
inline void*& CPtrList::GetAt(POSITION position)
	{ CNode* pNode = (CNode*) position; return pNode->data; }
inline void* CPtrList::GetAt(POSITION position) const
	{ CNode* pNode = (CNode*) position; return pNode->data; }
inline void CPtrList::SetAt(POSITION pos, void* newElement)
	{ CNode* pNode = (CNode*) pos; pNode->data = newElement; }

/////////////////////////////////////////////////////////////////////////////

class CObList : public CObject
{
   DECLARE_SERIAL(CObList)

protected:
   struct CNode {
      CNode* pNext;
      CNode* pPrev;
      CObject* data;
   };

public:
// Construction
   CObList(int nBlockSize = 10);

// Attributes (head and tail)
   // count of elements
   int GetCount() const;
   bool IsEmpty() const;

   // peek at head or tail
   CObject*& GetHead();
   CObject* GetHead() const;
   CObject*& GetTail();
   CObject* GetTail() const;

// Operations
   // get head or tail (and remove it) - don't call on empty list!
   CObject* RemoveHead();
   CObject* RemoveTail();

   // add before head or after tail
   POSITION AddHead(CObject* newElement);
   POSITION AddTail(CObject* newElement);

   // add another list of elements before head or after tail
   void AddHead(CObList* pNewList);
   void AddTail(CObList* pNewList);

   // remove all elements
   void RemoveAll();

   // iteration
   POSITION GetHeadPosition() const;
   POSITION GetTailPosition() const;
   CObject*& GetNext(POSITION& rPosition); // return *Position++
   CObject* GetNext(POSITION& rPosition) const; // return *Position++
   CObject*& GetPrev(POSITION& rPosition); // return *Position--
   CObject* GetPrev(POSITION& rPosition) const; // return *Position--

   // getting/modifying an element at a given position
   CObject*& GetAt(POSITION position);
   CObject* GetAt(POSITION position) const;
   void SetAt(POSITION pos, CObject* newElement);

   void RemoveAt(POSITION position);

   // inserting before or after a given position
   POSITION InsertBefore(POSITION position, CObject* newElement);
   POSITION InsertAfter(POSITION position, CObject* newElement);

   // helper functions (note: O(n) speed)
   POSITION Find(CObject* searchValue, POSITION startAfter = NULL) const;
                  // defaults to starting at the HEAD
                  // return NULL if not found
   POSITION FindIndex(int nIndex) const;
                  // get the 'nIndex'th element (may return NULL)

// Implementation
protected:
   CNode* m_pNodeHead;
   CNode* m_pNodeTail;
   int m_nCount;
   CNode* m_pNodeFree;
   struct CPlex* m_pBlocks;
   int m_nBlockSize;

   CNode* NewNode(CNode*, CNode*);
   void FreeNode(CNode*);

public:
   ~CObList();
   void Serialize(CArchive&);
#ifdef DEBUG
   void Dump(CDumpContext&) const;
   void AssertValid() const;
#endif

   // local typedefs for class templates
   typedef CObject* BASE_TYPE;
   typedef CObject* BASE_ARG_TYPE;
};

inline int CObList::GetCount() const
   { return m_nCount; }
inline bool CObList::IsEmpty() const
   { return m_nCount == 0; }
inline CObject*& CObList::GetHead()
   { assert(m_pNodeHead != NULL);
      return m_pNodeHead->data; }
inline CObject* CObList::GetHead() const
   { assert(m_pNodeHead != NULL);
      return m_pNodeHead->data; }
inline CObject*& CObList::GetTail()
   { assert(m_pNodeTail != NULL);
      return m_pNodeTail->data; }
inline CObject* CObList::GetTail() const
   { assert(m_pNodeTail != NULL);
      return m_pNodeTail->data; }
inline POSITION CObList::GetHeadPosition() const
   { return (POSITION) m_pNodeHead; }
inline POSITION CObList::GetTailPosition() const
   { return (POSITION) m_pNodeTail; }
inline CObject*& CObList::GetNext(POSITION& rPosition) // return *Position++
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pNext;
      return pNode->data; }
inline CObject* CObList::GetNext(POSITION& rPosition) const //return *Position++
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pNext;
      return pNode->data; }
inline CObject*& CObList::GetPrev(POSITION& rPosition) // return *Position--
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pPrev;
      return pNode->data; }
inline CObject* CObList::GetPrev(POSITION& rPosition) const //return *Position--
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pPrev;
      return pNode->data; }
inline CObject*& CObList::GetAt(POSITION position)
   { CNode* pNode = (CNode*) position;
      return pNode->data; }
inline CObject* CObList::GetAt(POSITION position) const
   { CNode* pNode = (CNode*) position;
      return pNode->data; }
inline void CObList::SetAt(POSITION pos, CObject* newElement)
   { CNode* pNode = (CNode*) pos;
      pNode->data = newElement; }

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrList<BASE_CLASS, TYPE>

template<class BASE_CLASS, class TYPE>
class _CTypedPtrList : public BASE_CLASS
{
public:
// Construction
   _CTypedPtrList(int nBlockSize = 10)
      : BASE_CLASS(nBlockSize) { }

   // peek at head or tail
   TYPE& GetHead()
      { return (TYPE&)BASE_CLASS::GetHead(); }
   TYPE GetHead() const
      { return (TYPE)BASE_CLASS::GetHead(); }
   TYPE& GetTail()
      { return (TYPE&)BASE_CLASS::GetTail(); }
   TYPE GetTail() const
      { return (TYPE)BASE_CLASS::GetTail(); }

   // get head or tail (and remove it) - don't call on empty list!
   TYPE RemoveHead()
   	{ return (TYPE)BASE_CLASS::RemoveHead(); }
   TYPE RemoveTail()
      { return (TYPE)BASE_CLASS::RemoveTail(); }

   // iteration
   TYPE& GetNext(POSITION& rPosition)
      { return (TYPE&)BASE_CLASS::GetNext(rPosition); }
   TYPE GetNext(POSITION& rPosition) const
      { return (TYPE)BASE_CLASS::GetNext(rPosition); }
   TYPE& GetPrev(POSITION& rPosition)
      { return (TYPE&)BASE_CLASS::GetPrev(rPosition); }
   TYPE GetPrev(POSITION& rPosition) const
      { return (TYPE)BASE_CLASS::GetPrev(rPosition); }

   // getting/modifying an element at a given position
   TYPE& GetAt(POSITION position)
      { return (TYPE&)BASE_CLASS::GetAt(position); }
   TYPE GetAt(POSITION position) const
      { return (TYPE)BASE_CLASS::GetAt(position); }
   void SetAt(POSITION pos, TYPE newElement)
      { BASE_CLASS::SetAt(pos, newElement); }
};

template<class BASE_CLASS, class TYPE>
class CTypedPtrList : public _CTypedPtrList<BASE_CLASS, TYPE>
{
public:
// Construction
   CTypedPtrList(int nBlockSize = 10)
      : _CTypedPtrList<BASE_CLASS, TYPE>(nBlockSize) { }

   // add before head or after tail
   POSITION AddHead(TYPE newElement)
      { return BASE_CLASS::AddHead(newElement); }
   POSITION AddTail(TYPE newElement)
      { return BASE_CLASS::AddTail(newElement); }

   // add another list of elements before head or after tail
   void AddHead(CTypedPtrList<BASE_CLASS, TYPE>* pNewList)
      { BASE_CLASS::AddHead(pNewList); }
   void AddTail(CTypedPtrList<BASE_CLASS, TYPE>* pNewList)
      { BASE_CLASS::AddTail(pNewList); }
};

// need specialized version for CObList because of AddHead/Tail ambiguity
template<> class CTypedPtrList<CObList, CObList*>
   : public _CTypedPtrList<CObList, CObList*>
{
public:
// Construction
   CTypedPtrList(int nBlockSize = 10)
      : _CTypedPtrList<CObList, CObList*>(nBlockSize) { }

   // add before head or after tail
   POSITION AddHead(CObList *newElement)
    { return _CTypedPtrList<CObList, CObList*>::AddHead((CObject*)newElement); }
   POSITION AddTail(CObList *newElement)
	 { return _CTypedPtrList<CObList, CObList*>::AddTail((CObject*)newElement); }

   // add another list of elements before head or after tail
   void AddHead(CTypedPtrList<CObList, CObList *>* pNewList)
      { _CTypedPtrList<CObList, CObList*>::AddHead(pNewList); }
   void AddTail(CTypedPtrList<CObList, CObList *>* pNewList)
      { _CTypedPtrList<CObList, CObList*>::AddTail(pNewList); }
};

// need specialized version for CPtrList because of AddHead/Tail ambiguity
template<> class CTypedPtrList<CPtrList, CPtrList *>
   : public _CTypedPtrList<CPtrList, CPtrList *>
{
public:
// Construction
   CTypedPtrList(int nBlockSize = 10)
      : _CTypedPtrList<CPtrList, CPtrList*>(nBlockSize) { }

   // add before head or after tail
   POSITION AddHead(CPtrList *newElement)
     { return _CTypedPtrList<CPtrList, CPtrList*>::AddHead((void*)newElement); }
   POSITION AddTail(CPtrList *newElement)
     { return _CTypedPtrList<CPtrList, CPtrList*>::AddTail((void*)newElement); }

   // add another list of elements before head or after tail
   void AddHead(CTypedPtrList<CPtrList, CPtrList *>* pNewList)
      { _CTypedPtrList<CPtrList, CPtrList*>::AddHead(pNewList); }
   void AddTail(CTypedPtrList<CPtrList, CPtrList *>* pNewList)
      { _CTypedPtrList<CPtrList, CPtrList*>::AddTail(pNewList); }
};

////////////////////////////////////////////////////////////////////////////

class CPtrArray : public CObject
{
   DECLARE_DYNAMIC(CPtrArray)

public:
// Construction
   CPtrArray();

// Attributes
   int GetSize() const;
   int GetUpperBound() const;
   void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
   // Clean up
   void FreeExtra();
   void RemoveAll();

   // Accessing elements
   void* GetAt(int nIndex) const;
   void SetAt(int nIndex, void* newElement);

   void*& ElementAt(int nIndex);

   // Direct Access to the element data (may return NULL)
   const void** GetData() const;
   void** GetData();

   // Potentially growing the array
   void SetAtGrow(int nIndex, void* newElement);

   int Add(void* newElement);

   int Append(const CPtrArray& src);
   void Copy(const CPtrArray& src);

   // overloaded operator helpers
   void* operator[](int nIndex) const;
   void*& operator[](int nIndex);

   // Operations that move elements around
   void InsertAt(int nIndex, void* newElement, int nCount = 1);

   void RemoveAt(int nIndex, int nCount = 1);
   void InsertAt(int nStartIndex, CPtrArray* pNewArray);

// Implementation
protected:
   void** m_pData;   // the actual array of data
   int m_nSize;     // # of elements (upperBound - 1)
   int m_nMaxSize;  // max allocated
   int m_nGrowBy;   // grow amount

public:
   ~CPtrArray();
#ifdef DEBUG
   void Dump(CDumpContext&) const;
   void AssertValid() const;
#endif

protected:
   // local typedefs for class templates
   typedef void* BASE_TYPE;
   typedef void* BASE_ARG_TYPE;
};

inline int CPtrArray::GetSize() const
	{ return m_nSize; }
inline int CPtrArray::GetUpperBound() const
	{ return m_nSize-1; }
inline void CPtrArray::RemoveAll()
	{ SetSize(0); }
inline void* CPtrArray::GetAt(int nIndex) const
	{ assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
inline void CPtrArray::SetAt(int nIndex, void* newElement)
	{ assert(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }

inline void*& CPtrArray::ElementAt(int nIndex)
	{ assert(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
inline const void** CPtrArray::GetData() const
	{ return (const void**)m_pData; }
inline void** CPtrArray::GetData()
	{ return (void**)m_pData; }
inline int CPtrArray::Add(void* newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }

inline void* CPtrArray::operator[](int nIndex) const
	{ return GetAt(nIndex); }
inline void*& CPtrArray::operator[](int nIndex)
	{ return ElementAt(nIndex); }

/////////////////////////////////////////////////////////////////////////////

class CMapPtrToPtr : public CObject
{
   DECLARE_DYNAMIC(CMapPtrToPtr)

protected:
   // Association
   struct CAssoc {
      CAssoc* pNext;
      void* key;
      void* value;
   };

public:
// Construction
   CMapPtrToPtr(int nBlockSize = 10);

// Attributes
   // number of elements
   int GetCount() const;
   bool IsEmpty() const;

   // Lookup
   bool Lookup(void* key, void*& rValue) const;

// Operations
   // Lookup and add if not there
   void*& operator[](void* key);

   // add a new (key, value) pair
   void SetAt(void* key, void* newValue);

   // removing existing (key, ?) pair
   bool RemoveKey(void* key);
   void RemoveAll();

   // iterating all (key, value) pairs
   POSITION GetStartPosition() const;
   void GetNextAssoc(POSITION& rNextPosition, void*& rKey, void*& rValue) const;

   // advanced features for derived classes
   PDKUINT GetHashTableSize() const;
   void InitHashTable(PDKUINT hashSize, bool bAllocNow = true);

// Overridables: special non-virtual (see map implementation for details)
   // Routine used to user-provided hash keys
   PDKUINT HashKey(void* key) const;

// Implementation
protected:
   CAssoc** m_pHashTable;
   PDKUINT m_nHashTableSize;
   int m_nCount;
   CAssoc* m_pFreeList;
   struct CPlex* m_pBlocks;
   int m_nBlockSize;

   CAssoc* NewAssoc();
   void FreeAssoc(CAssoc*);
   CAssoc* GetAssocAt(void*, PDKUINT&) const;

public:
   ~CMapPtrToPtr();
#ifdef DEBUG
   void Dump(CDumpContext&) const;
   void AssertValid() const;
#endif

   void* GetValueAt(void* key) const;

protected:
   // local typedefs for CTypedPtrMap class template
   typedef void* BASE_KEY;
   typedef void* BASE_ARG_KEY;
   typedef void* BASE_VALUE;
   typedef void* BASE_ARG_VALUE;
};

inline int CMapPtrToPtr::GetCount() const
	{ return m_nCount; }
inline bool CMapPtrToPtr::IsEmpty() const
	{ return m_nCount == 0; }
inline void CMapPtrToPtr::SetAt(void* key, void* newValue)
	{ (*this)[key] = newValue; }
inline POSITION CMapPtrToPtr::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
inline PDKUINT CMapPtrToPtr::GetHashTableSize() const
	{ return m_nHashTableSize; }

/////////////////////////////////////////////////////////////////////////////

class CMapStringToPtr : public CObject
{
   DECLARE_DYNAMIC(CMapStringToPtr)

protected:
   // Association
   struct CAssoc {
      CAssoc* pNext;
      PDKUINT nHashValue;  // needed for efficient iteration
      CString key;
      void* value;
   };

public:
// Construction
   CMapStringToPtr(int nBlockSize = 10);

// Attributes
   // number of elements
   int GetCount() const;
   bool IsEmpty() const;

   // Lookup
   bool Lookup(LPCSTR key, void*& rValue) const;
   bool LookupKey(LPCSTR key, LPCSTR& rKey) const;

// Operations
   // Lookup and add if not there
   void*& operator[](LPCSTR key);

   // add a new (key, value) pair
   void SetAt(LPCSTR key, void* newValue);

   // removing existing (key, ?) pair
   bool RemoveKey(LPCSTR key);
   void RemoveAll();

   // iterating all (key, value) pairs
   POSITION GetStartPosition() const;
   void GetNextAssoc(POSITION& rNextPosition,
                     CString& rKey, void*& rValue) const;

   // advanced features for derived classes
   PDKUINT GetHashTableSize() const;
   void InitHashTable(PDKUINT hashSize, bool bAllocNow = true);

// Overridables: special non-virtual (see map implementation for details)
   // Routine used to user-provided hash keys
   PDKUINT HashKey(LPCSTR key) const;

// Implementation
protected:
   CAssoc** m_pHashTable;
   PDKUINT m_nHashTableSize;
   int m_nCount;
   CAssoc* m_pFreeList;
   struct CPlex* m_pBlocks;
   int m_nBlockSize;

   CAssoc* NewAssoc();
   void FreeAssoc(CAssoc*);
   CAssoc* GetAssocAt(LPCSTR, PDKUINT&) const;

public:
   ~CMapStringToPtr();
#ifdef DEBUG
   void Dump(CDumpContext&) const;
   void AssertValid() const;
#endif

protected:
   // local typedefs for CTypedPtrMap class template
   typedef CString BASE_KEY;
   typedef LPCSTR BASE_ARG_KEY;
   typedef void* BASE_VALUE;
   typedef void* BASE_ARG_VALUE;
};

inline int CMapStringToPtr::GetCount() const
   { return m_nCount; }
inline bool CMapStringToPtr::IsEmpty() const
   { return m_nCount == 0; }
inline void CMapStringToPtr::SetAt(LPCSTR key, void* newValue)
   { (*this)[key] = newValue; }
inline POSITION CMapStringToPtr::GetStartPosition() const
   { return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
inline PDKUINT CMapStringToPtr::GetHashTableSize() const
   { return m_nHashTableSize; }

/////////////////////////////////////////////////////////////////////////////

class CMapStringToOb : public CObject
{
   DECLARE_SERIAL(CMapStringToOb)

protected:
   // Association
   struct CAssoc {
      CAssoc* pNext;
      PDKUINT nHashValue;  // needed for efficient iteration
      CString key;
      CObject* value;
   };

public:
// Construction
   CMapStringToOb(int nBlockSize = 10);

// Attributes
   // number of elements
   int GetCount() const;
   bool IsEmpty() const;

   // Lookup
   bool Lookup(LPCSTR key, CObject*& rValue) const;
   bool LookupKey(LPCSTR key, LPCSTR& rKey) const;

// Operations
   // Lookup and add if not there
   CObject*& operator[](LPCSTR key);

   // add a new (key, value) pair
   void SetAt(LPCSTR key, CObject* newValue);

   // removing existing (key, ?) pair
   bool RemoveKey(LPCSTR key);
   void RemoveAll();

   // iterating all (key, value) pairs
   POSITION GetStartPosition() const;
   void GetNextAssoc(POSITION& rNextPosition,
                     CString& rKey, CObject*& rValue) const;

   // advanced features for derived classes
   PDKUINT GetHashTableSize() const;
   void InitHashTable(PDKUINT hashSize, bool bAllocNow = true);

// Overridables: special non-virtual (see map implementation for details)
   // Routine used to user-provided hash keys
   PDKUINT HashKey(LPCSTR key) const;

// Implementation
protected:
   CAssoc** m_pHashTable;
   PDKUINT m_nHashTableSize;
   int m_nCount;
   CAssoc* m_pFreeList;
   struct CPlex* m_pBlocks;
   int m_nBlockSize;

   CAssoc* NewAssoc();
   void FreeAssoc(CAssoc*);
   CAssoc* GetAssocAt(LPCSTR, PDKUINT&) const;

public:
   ~CMapStringToOb();

   void Serialize(CArchive&);
#ifdef DEBUG
   void Dump(CDumpContext&) const;
   void AssertValid() const;
#endif

protected:
   // local typedefs for CTypedPtrMap class template
   typedef CString BASE_KEY;
   typedef LPCSTR BASE_ARG_KEY;
   typedef CObject* BASE_VALUE;
   typedef CObject* BASE_ARG_VALUE;
};

inline int CMapStringToOb::GetCount() const
	{ return m_nCount; }
inline bool CMapStringToOb::IsEmpty() const
	{ return m_nCount == 0; }
inline void CMapStringToOb::SetAt(LPCSTR key, CObject* newValue)
	{ (*this)[key] = newValue; }
inline POSITION CMapStringToOb::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
inline PDKUINT CMapStringToOb::GetHashTableSize() const
	{ return m_nHashTableSize; }

/////////////////////////////////////////////////////////////////////////////

class CMapStringToString : public CObject
{
   DECLARE_SERIAL(CMapStringToString)

protected:
   // Association
   struct CAssoc {
      CAssoc* pNext;
      PDKUINT nHashValue;  // needed for efficient iteration
      CString key;
      CString value;
   };

public:
// Construction
   CMapStringToString(int nBlockSize = 10);

// Attributes
   // number of elements
   int GetCount() const;
   bool IsEmpty() const;

   // Lookup
   bool Lookup(LPCSTR key, CString& rValue) const;
   bool LookupKey(LPCSTR key, LPCSTR& rKey) const;

// Operations
   // Lookup and add if not there
   CString& operator[](LPCSTR key);

   // add a new (key, value) pair
   void SetAt(LPCSTR key, LPCSTR newValue);

   // removing existing (key, ?) pair
   bool RemoveKey(LPCSTR key);
   void RemoveAll();

   // iterating all (key, value) pairs
   POSITION GetStartPosition() const;
   void GetNextAssoc(POSITION& rNextPosition,
                     CString& rKey, CString& rValue) const;

   // advanced features for derived classes
   PDKUINT GetHashTableSize() const;
   void InitHashTable(PDKUINT hashSize, bool bAllocNow = true);

// Overridables: special non-virtual (see map implementation for details)
   // Routine used to user-provided hash keys
   PDKUINT HashKey(LPCSTR key) const;

// Implementation
protected:
   CAssoc** m_pHashTable;
   PDKUINT m_nHashTableSize;
   int m_nCount;
   CAssoc* m_pFreeList;
   struct CPlex* m_pBlocks;
   int m_nBlockSize;

   CAssoc* NewAssoc();
   void FreeAssoc(CAssoc*);
   CAssoc* GetAssocAt(LPCSTR, PDKUINT&) const;

public:
   ~CMapStringToString();

   void Serialize(CArchive&);
#ifdef DEBUG
   void Dump(CDumpContext&) const;
   void AssertValid() const;
#endif

protected:
   // local typedefs for CTypedPtrMap class template
   typedef CString BASE_KEY;
   typedef LPCSTR BASE_ARG_KEY;
   typedef CString BASE_VALUE;
   typedef LPCSTR BASE_ARG_VALUE;
};

inline int CMapStringToString::GetCount() const
   { return m_nCount; }
inline bool CMapStringToString::IsEmpty() const
   { return m_nCount == 0; }
inline void CMapStringToString::SetAt(LPCSTR key, LPCSTR newValue)
   { (*this)[key] = newValue; }
inline POSITION CMapStringToString::GetStartPosition() const
   { return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
inline PDKUINT CMapStringToString::GetHashTableSize() const
   { return m_nHashTableSize; }

/////////////////////////////////////////////////////////////////////////////

class CStringList : public CObject
{
   DECLARE_SERIAL(CStringList)

protected:
   struct CNode {
      CNode* pNext;
      CNode* pPrev;
      CString data;
   };

public:
// Construction
   CStringList(int nBlockSize = 10);

// Attributes (head and tail)
   // count of elements
   int GetCount() const;
   bool IsEmpty() const;

   // peek at head or tail
   CString& GetHead();
   CString GetHead() const;
   CString& GetTail();
   CString GetTail() const;

// Operations
   // get head or tail (and remove it) - don't call on empty list!
   CString RemoveHead();
   CString RemoveTail();

   // add before head or after tail
   POSITION AddHead(LPCSTR newElement);
   POSITION AddTail(LPCSTR newElement);

   POSITION AddHead(const CString& newElement);
   POSITION AddTail(const CString& newElement);

   // add another list of elements before head or after tail
   void AddHead(CStringList* pNewList);
   void AddTail(CStringList* pNewList);

   // remove all elements
   void RemoveAll();

   // iteration
   POSITION GetHeadPosition() const;
   POSITION GetTailPosition() const;
   CString& GetNext(POSITION& rPosition); // return *Position++
   CString GetNext(POSITION& rPosition) const; // return *Position++
   CString& GetPrev(POSITION& rPosition); // return *Position--
   CString GetPrev(POSITION& rPosition) const; // return *Position--

   // getting/modifying an element at a given position
   CString& GetAt(POSITION position);
   CString GetAt(POSITION position) const;
   void SetAt(POSITION pos, LPCSTR newElement);

   void SetAt(POSITION pos, const CString& newElement);

   void RemoveAt(POSITION position);

   // inserting before or after a given position
   POSITION InsertBefore(POSITION position, LPCSTR newElement);
   POSITION InsertAfter(POSITION position, LPCSTR newElement);

   POSITION InsertBefore(POSITION position, const CString& newElement);
   POSITION InsertAfter(POSITION position, const CString& newElement);


   // helper functions (note: O(n) speed)
   POSITION Find(LPCSTR searchValue, POSITION startAfter = NULL) const;
      // defaults to starting at the HEAD
      // return NULL if not found
   POSITION FindIndex(int nIndex) const;
      // get the 'nIndex'th element (may return NULL)

// Implementation
protected:
   CNode* m_pNodeHead;
   CNode* m_pNodeTail;
   int m_nCount;
   CNode* m_pNodeFree;
   struct CPlex* m_pBlocks;
   int m_nBlockSize;

   CNode* NewNode(CNode*, CNode*);
   void FreeNode(CNode*);

public:
   ~CStringList();

   void Serialize(CArchive&);
#ifdef DEBUG
   void Dump(CDumpContext&) const;
   void AssertValid() const;
#endif
   // local typedefs for class templates
   typedef CString BASE_TYPE;
   typedef LPCSTR BASE_ARG_TYPE;
};

inline int CStringList::GetCount() const
   { return m_nCount; }
inline bool CStringList::IsEmpty() const
   { return m_nCount == 0; }
inline CString& CStringList::GetHead()
   { assert(m_pNodeHead != NULL); return m_pNodeHead->data; }
inline CString CStringList::GetHead() const
   { assert(m_pNodeHead != NULL); return m_pNodeHead->data; }
inline CString& CStringList::GetTail()
   { assert(m_pNodeTail != NULL); return m_pNodeTail->data; }
inline CString CStringList::GetTail() const
   { assert(m_pNodeTail != NULL); return m_pNodeTail->data; }
inline POSITION CStringList::GetHeadPosition() const
   { return (POSITION) m_pNodeHead; }
inline POSITION CStringList::GetTailPosition() const
   { return (POSITION) m_pNodeTail; }
inline CString& CStringList::GetNext(POSITION& rPosition) // return *Position++
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pNext;
      return pNode->data; }
inline CString CStringList::GetNext(POSITION& rPosition) const // return *Position++
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pNext;
      return pNode->data; }
inline CString& CStringList::GetPrev(POSITION& rPosition) // return *Position--
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pPrev;
      return pNode->data; }
inline CString CStringList::GetPrev(POSITION& rPosition) const // return *Position--
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pPrev;
      return pNode->data; }
inline CString& CStringList::GetAt(POSITION position)
   { CNode* pNode = (CNode*) position; return pNode->data; }
inline CString CStringList::GetAt(POSITION position) const
   { CNode* pNode = (CNode*) position; return pNode->data; }
inline void CStringList::SetAt(POSITION pos, LPCSTR newElement)
   { CNode* pNode = (CNode*) pos; pNode->data = newElement; }
inline void CStringList::SetAt(POSITION pos, const CString& newElement)
   { CNode* pNode = (CNode*) pos; pNode->data = newElement; }

/////////////////////////////////////////////////////////////////////////////
// global helpers (can be overridden)

template<class TYPE>
inline void ConstructElements(TYPE* pElements, int nCount)
{
	// first do bit-wise zero initialization
	memset((void*)pElements, 0, nCount * sizeof(TYPE));

	// then call the constructor(s)
	for (; nCount--; pElements++)
		::new((void*)pElements) TYPE;
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

template<class TYPE>
void SerializeElements(CArchive& ar, TYPE* pElements, int nCount)
{
	// default is bit-wise read/write
	if (ar.IsStoring())
		ar.Write((void*)pElements, nCount * sizeof(TYPE));
	else
		ar.Read((void*)pElements, nCount * sizeof(TYPE));
}

#ifdef DEBUG
template<class TYPE>
void DumpElements(CDumpContext& dc, const TYPE* pElements, int nCount)
{
	// default does nothing
}
#endif

template<class TYPE, class ARG_TYPE>
bool CompareElements(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	return *pElement1 == *pElement2;
}

template<class ARG_KEY>
inline PDKUINT HashKey(ARG_KEY key)
{
	// default identity hash - works for most primitive values
	return ((PDKUINT)(void*)(PDK32U)key) >> 4;
}

// special versions for CString
template<> void ConstructElements<CString> (CString* pElements, int nCount);
template<> void DestructElements<CString> (CString* pElements, int nCount);
template<> void CopyElements<CString> (CString* pDest, const CString* pSrc, int nCount);
template<> void SerializeElements<CString> (CArchive& ar, CString* pElements, int nCount);
template<> PDKUINT HashKey<LPCSTR> (LPCSTR key);

/////////////////////////////////////////////////////////////////////////////
// CArray<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE>
class CArray : public CObject
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
   void Serialize(CArchive&);
#ifdef DEBUG
   void Dump(CDumpContext&) const;
   void AssertValid() const;
#endif
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

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::Serialize(CArchive& ar)
{
   ASSERT_VALID(this);

   CObject::Serialize(ar);
   if (ar.IsStoring()) {
      ar.WriteCount(m_nSize);
   }
   else {
      PDK32U nOldSize = ar.ReadCount();
      SetSize(nOldSize, -1);
   }
   SerializeElements<TYPE>(ar, m_pData, m_nSize);
}

#ifdef DEBUG
template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::Dump(CDumpContext& dc) const
{
   CObject::Dump(dc);

   dc << "with " << m_nSize << " elements";
   if (dc.GetDepth() > 0) {
      dc << "\n";
      DumpElements<TYPE>(dc, m_pData, m_nSize);
   }

   dc << "\n";
}

template<class TYPE, class ARG_TYPE>
void CArray<TYPE, ARG_TYPE>::AssertValid() const
{
   CObject::AssertValid();

   if (m_pData == NULL) {
      ASSERT(m_nSize == 0);
      ASSERT(m_nMaxSize == 0);
   }
   else {
      ASSERT(m_nSize >= 0);
      ASSERT(m_nMaxSize >= 0);
      ASSERT(m_nSize <= m_nMaxSize);
   }
}
#endif // DEBUG

/////////////////////////////////////////////////////////////////////////////
// CList<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE>
class CList : public CObject
{
protected:
   struct CNode {
      CNode* pNext;
      CNode* pPrev;
      TYPE data;
   };

public:
// Construction
   CList(int nBlockSize = 10);

// Attributes (head and tail)
   // count of elements
   int GetCount() const;
   bool IsEmpty() const;

   // peek at head or tail
   TYPE& GetHead();
   TYPE GetHead() const;
   TYPE& GetTail();
   TYPE GetTail() const;

// Operations
   // get head or tail (and remove it) - don't call on empty list !
   TYPE RemoveHead();
   TYPE RemoveTail();

   // add before head or after tail
   POSITION AddHead(ARG_TYPE newElement);
   POSITION AddTail(ARG_TYPE newElement);

   // add another list of elements before head or after tail
   void AddHead(CList* pNewList);
   void AddTail(CList* pNewList);

   // remove all elements
   void RemoveAll();

   // iteration
   POSITION GetHeadPosition() const;
   POSITION GetTailPosition() const;
   TYPE& GetNext(POSITION& rPosition); // return *Position++
   TYPE GetNext(POSITION& rPosition) const; // return *Position++
   TYPE& GetPrev(POSITION& rPosition); // return *Position--
   TYPE GetPrev(POSITION& rPosition) const; // return *Position--

   // getting/modifying an element at a given position
   TYPE& GetAt(POSITION position);
   TYPE GetAt(POSITION position) const;
   void SetAt(POSITION pos, ARG_TYPE newElement);
   void RemoveAt(POSITION position);

   // inserting before or after a given position
   POSITION InsertBefore(POSITION position, ARG_TYPE newElement);
   POSITION InsertAfter(POSITION position, ARG_TYPE newElement);

   // helper functions (note: O(n) speed)
   POSITION Find(ARG_TYPE searchValue, POSITION startAfter = NULL) const;
      // defaults to starting at the HEAD, return NULL if not found
   POSITION FindIndex(int nIndex) const;
      // get the 'nIndex'th element (may return NULL)

// Implementation
protected:
   CNode* m_pNodeHead;
   CNode* m_pNodeTail;
   int m_nCount;
   CNode* m_pNodeFree;
   struct CPlex* m_pBlocks;
   int m_nBlockSize;

   CNode* NewNode(CNode*, CNode*);
   void FreeNode(CNode*);

public:
   ~CList();
   void Serialize(CArchive&);
#ifdef DEBUG
   void Dump(CDumpContext&) const;
   void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CList<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE>
inline int CList<TYPE, ARG_TYPE>::GetCount() const
   { return m_nCount; }
template<class TYPE, class ARG_TYPE>
inline bool CList<TYPE, ARG_TYPE>::IsEmpty() const
   { return m_nCount == 0; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CList<TYPE, ARG_TYPE>::GetHead()
   { assert(m_pNodeHead != NULL); return m_pNodeHead->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE CList<TYPE, ARG_TYPE>::GetHead() const
   { assert(m_pNodeHead != NULL); return m_pNodeHead->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CList<TYPE, ARG_TYPE>::GetTail()
   { assert(m_pNodeTail != NULL); return m_pNodeTail->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE CList<TYPE, ARG_TYPE>::GetTail() const
   { assert(m_pNodeTail != NULL); return m_pNodeTail->data; }
template<class TYPE, class ARG_TYPE>
inline POSITION CList<TYPE, ARG_TYPE>::GetHeadPosition() const
   { return (POSITION) m_pNodeHead; }
template<class TYPE, class ARG_TYPE>
inline POSITION CList<TYPE, ARG_TYPE>::GetTailPosition() const
   { return (POSITION) m_pNodeTail; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CList<TYPE, ARG_TYPE>::GetNext(POSITION& rPosition)
// return *Position++
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pNext;
      return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE CList<TYPE, ARG_TYPE>::GetNext(POSITION& rPosition) const
// return *Position++
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pNext;
      return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CList<TYPE, ARG_TYPE>::GetPrev(POSITION& rPosition)
// return *Position--
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pPrev;
      return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE CList<TYPE, ARG_TYPE>::GetPrev(POSITION& rPosition) const
// return *Position--
   { CNode* pNode = (CNode*) rPosition;
      rPosition = (POSITION) pNode->pPrev;
      return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE& CList<TYPE, ARG_TYPE>::GetAt(POSITION position)
   { CNode* pNode = (CNode*) position;
      return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline TYPE CList<TYPE, ARG_TYPE>::GetAt(POSITION position) const
   { CNode* pNode = (CNode*) position;
      return pNode->data; }
template<class TYPE, class ARG_TYPE>
inline void CList<TYPE, ARG_TYPE>::SetAt(POSITION pos, ARG_TYPE newElement)
   { CNode* pNode = (CNode*) pos;
      pNode->data = newElement; }

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::CList(int nBlockSize)
{
   assert(nBlockSize > 0);

   m_nCount = 0;
   m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
   m_pBlocks = NULL;
   m_nBlockSize = nBlockSize;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveAll()
{
   assert(this != NULL);

   // destroy elements
   CNode* pNode;
   for (pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
      DestructElements<TYPE>(&pNode->data, 1);

   m_nCount = 0;
   m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
   m_pBlocks->FreeDataChain();
   m_pBlocks = NULL;
}

template<class TYPE, class ARG_TYPE>
CList<TYPE, ARG_TYPE>::~CList()
{
   RemoveAll();
   assert(m_nCount == 0);
}

/////////////////////////////////////////////////////////////////////////////
// Node helpers
//
// Implementation note: CNode's are stored in CPlex blocks and
//  chained together. Free blocks are maintained in a singly linked list
//  using the 'pNext' member of CNode with 'm_pNodeFree' as the head.
//  Used blocks are maintained in a doubly linked list using both 'pNext'
//  and 'pPrev' as links and 'm_pNodeHead' and 'm_pNodeTail'
//   as the head/tail.
//
// We never free a CPlex block unless the List is destroyed or RemoveAll()
//  is used - so the total number of CPlex blocks may grow large depending
//  on the maximum past size of the list.
//

template<class TYPE, class ARG_TYPE>
typename CList<TYPE, ARG_TYPE>::CNode*
CList<TYPE, ARG_TYPE>::NewNode(CList::CNode* pPrev, CList::CNode* pNext)
{
   if (m_pNodeFree == NULL) {
      // add another block
      CPlex* pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CNode));

      // chain them into free list
      CNode* pNode = (CNode*) pNewBlock->data();
      // free in reverse order to make it easier to debug
      pNode += m_nBlockSize - 1;
      for (int i = m_nBlockSize-1; i >= 0; i--, pNode--) {
         pNode->pNext = m_pNodeFree;
         m_pNodeFree = pNode;
      }
   }
   assert(m_pNodeFree != NULL);  // we must have something

   typename CList::CNode* pNode = m_pNodeFree;
   m_pNodeFree = m_pNodeFree->pNext;
   pNode->pPrev = pPrev;
   pNode->pNext = pNext;
   m_nCount++;
   assert(m_nCount > 0);  // make sure we don't overflow

   ConstructElements<TYPE>(&pNode->data, 1);
   return pNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::FreeNode(CList::CNode* pNode)
{
   DestructElements<TYPE>(&pNode->data, 1);
   pNode->pNext = m_pNodeFree;
   m_pNodeFree = pNode;
   m_nCount--;
   assert(m_nCount >= 0);  // make sure we don't underflow

   // if no more elements, cleanup completely
   if (m_nCount == 0)
      RemoveAll();
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::AddHead(ARG_TYPE newElement)
{
   assert(this != NULL);

   CNode* pNewNode = NewNode(NULL, m_pNodeHead);
   pNewNode->data = newElement;
   if (m_pNodeHead != NULL)
      m_pNodeHead->pPrev = pNewNode;
   else
      m_pNodeTail = pNewNode;
   m_pNodeHead = pNewNode;
   return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::AddTail(ARG_TYPE newElement)
{
   assert(this != NULL);

   CNode* pNewNode = NewNode(m_pNodeTail, NULL);
   pNewNode->data = newElement;
   if (m_pNodeTail != NULL)
      m_pNodeTail->pNext = pNewNode;
   else
      m_pNodeHead = pNewNode;
   m_pNodeTail = pNewNode;
   return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AddHead(CList* pNewList)
{
   assert(this != NULL);
   assert(pNewList != NULL);

   // add a list of same elements to head (maintain order)
   POSITION pos = pNewList->GetTailPosition();
   while (pos != NULL)
      AddHead(pNewList->GetPrev(pos));
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AddTail(CList* pNewList)
{
   assert(this != NULL);
   assert(pNewList != NULL);

   // add a list of same elements
   POSITION pos = pNewList->GetHeadPosition();
   while (pos != NULL)
      AddTail(pNewList->GetNext(pos));
}

template<class TYPE, class ARG_TYPE>
TYPE CList<TYPE, ARG_TYPE>::RemoveHead()
{
   assert(this != NULL);
   assert(m_pNodeHead != NULL);  // don't call on empty list !!!

   CNode* pOldNode = m_pNodeHead;
   TYPE returnValue = pOldNode->data;

   m_pNodeHead = pOldNode->pNext;
   if (m_pNodeHead != NULL)
      m_pNodeHead->pPrev = NULL;
   else
      m_pNodeTail = NULL;
   FreeNode(pOldNode);
   return returnValue;
}

template<class TYPE, class ARG_TYPE>
TYPE CList<TYPE, ARG_TYPE>::RemoveTail()
{
   assert(this != NULL);
   assert(m_pNodeTail != NULL);  // don't call on empty list !!!

   CNode* pOldNode = m_pNodeTail;
   TYPE returnValue = pOldNode->data;

   m_pNodeTail = pOldNode->pPrev;
   if (m_pNodeTail != NULL)
      m_pNodeTail->pNext = NULL;
   else
      m_pNodeHead = NULL;
   FreeNode(pOldNode);
   return returnValue;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::InsertBefore(POSITION position, ARG_TYPE newElement)
{
   assert(this != NULL);

   if (position == NULL)
      return AddHead(newElement); // insert before nothing -> head of the list

   // Insert it before position
   CNode* pOldNode = (CNode*) position;
   CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
   pNewNode->data = newElement;

   if (pOldNode->pPrev != NULL)
      pOldNode->pPrev->pNext = pNewNode;
   else {
      assert(pOldNode == m_pNodeHead);
      m_pNodeHead = pNewNode;
   }
   pOldNode->pPrev = pNewNode;
   return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::InsertAfter(POSITION position, ARG_TYPE newElement)
{
   assert(this != NULL);

   if (position == NULL)
      return AddTail(newElement); // insert after nothing -> tail of the list

   // Insert it before position
   CNode* pOldNode = (CNode*) position;
   CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
   pNewNode->data = newElement;

   if (pOldNode->pNext != NULL)
      pOldNode->pNext->pPrev = pNewNode;
   else {
      assert(pOldNode == m_pNodeTail);
      m_pNodeTail = pNewNode;
   }
   pOldNode->pNext = pNewNode;
   return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::RemoveAt(POSITION position)
{
   assert(this != NULL);

   CNode* pOldNode = (CNode*) position;

   // remove pOldNode from list
   if (pOldNode == m_pNodeHead)
      m_pNodeHead = pOldNode->pNext;
   else
      pOldNode->pPrev->pNext = pOldNode->pNext;
   if (pOldNode == m_pNodeTail)
      m_pNodeTail = pOldNode->pPrev;
   else
      pOldNode->pNext->pPrev = pOldNode->pPrev;
   FreeNode(pOldNode);
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::FindIndex(int nIndex) const
{
   assert(this != NULL);

   if (nIndex >= m_nCount || nIndex < 0)
      return NULL;  // went too far

   CNode* pNode = m_pNodeHead;
   while (nIndex--)
      pNode = pNode->pNext;

   return (POSITION) pNode;
}

template<class TYPE, class ARG_TYPE>
POSITION CList<TYPE, ARG_TYPE>::Find(ARG_TYPE searchValue, POSITION startAfter) const
{
   assert(this != NULL);

   CNode* pNode = (CNode*) startAfter;
   if (pNode == NULL)
      pNode = m_pNodeHead;  // start at head
   else
      pNode = pNode->pNext;  // start after the one specified

   for (; pNode != NULL; pNode = pNode->pNext)
      if (CompareElements<TYPE>(&pNode->data, &searchValue))
         return (POSITION)pNode;
   return NULL;
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::Serialize(CArchive& ar)
{
   assert(this != NULL);

   CObject::Serialize(ar);

   if (ar.IsStoring()) {
      ar.WriteCount(m_nCount);
      for (CNode* pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
         SerializeElements<TYPE>(ar, &pNode->data, 1);
   }
   else {
      PDK32U nNewCount = ar.ReadCount();
      while (nNewCount--) {
         TYPE newData;
         SerializeElements<TYPE>(ar, &newData, 1);
         AddTail(newData);
      }
   }
}

#ifdef DEBUG
template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::Dump(CDumpContext& dc) const
{
   CObject::Dump(dc);

   dc << "with " << m_nCount << " elements";
   if (dc.GetDepth() > 0) {
   	POSITION pos = GetHeadPosition();
   	while (pos != NULL) {
   		dc << "\n";
   		DumpElements<TYPE>(dc, &((CList*)this)->GetNext(pos), 1);
   	}
   }

   dc << "\n";
}

template<class TYPE, class ARG_TYPE>
void CList<TYPE, ARG_TYPE>::AssertValid() const
{
	CObject::AssertValid();

	if (m_nCount == 0) {
		// empty list
		assert(m_pNodeHead == NULL);
		assert(m_pNodeTail == NULL);
	}
	else {
		// non-empty list
		assert(m_pNodeHead != NULL);
		assert(m_pNodeTail != NULL);
	}
}
#endif //DEBUG

#endif
