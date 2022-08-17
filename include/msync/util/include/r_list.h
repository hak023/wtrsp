#ifndef _RMAP_SS_H_
#define _RMAP_SS_H_

#include <assert.h>
/*#include <new.h>*/
#include <stdio.h>
#include <stdlib.h>
#include "r_types.h"
#include "r_base.h"

namespace RED
{

struct __POSITION { }; 
typedef __POSITION* POSITION;

#define BEFORE_START_POSITION ((POSITION)-1L)

class DLLPORT CMapStringToString
{
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
};//namespace RED

#endif
