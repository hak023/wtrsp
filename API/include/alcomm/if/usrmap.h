#ifndef __AL_USRMAPPING_H_
#define __AL_USRMAPPING_H_


#include "aloxCommon.h"
#include <map>
//ses mapping
template < typename T_KEY,typename T_VALUE,typename T_COMPARE=std::less<T_KEY> >
//Inherited CUncopyable
class CUsrMap
   : private CUncopyable
{
private:
	///defines types
	typedef std::map<T_KEY,T_VALUE,T_COMPARE> ALUSRMAP;
   typedef std::pair<T_KEY,T_VALUE> VALUE_TYPE;
	typedef typename ALUSRMAP::iterator ALUSRMAP_IT;
public:
   ///contstructor
   CUsrMap()
   {
      m_pUserMap= new ALUSRMAP;
      m_pLock= new CCritSec;
   }
   ///distructor
   ~CUsrMap()
   {
   }
   ///Insert map
	bool InsertMap(const T_KEY& key,const T_VALUE& value)
   {
      m_pLock->Lock();
      int nBeforeSize=m_pUserMap->size();
      m_pUserMap->insert(VALUE_TYPE(key,value));	
      int nAfterSize=m_pUserMap->size();
      m_pLock->Unlock();
      if(nBeforeSize >= nAfterSize ) return false;
      return true;
   }
   ///delete map
	bool DeleteMap(const T_KEY& key)
   {
      m_pLock->Lock();
      if ( m_pUserMap->empty() ){
         m_pLock->Unlock();
         return false;
      }
      ALUSRMAP_IT it;
      it=m_pUserMap->find(key);
      if( it!=m_pUserMap->end()){
         m_pUserMap->erase(it++);
         m_pLock->Unlock();
         return true;
      }
      m_pLock->Unlock();
      return false;
   }
   ///find map with key, reference value
	bool FindMap(const T_KEY& key,T_VALUE& value,bool bMemcpy=false)
   {
      m_pLock->Lock();
      if ( m_pUserMap->empty() ){
         m_pLock->Unlock();
         return false;
      }
      ALUSRMAP_IT it;
      it=m_pUserMap->find(key);
      if( it!=m_pUserMap->end()){
         if ( bMemcpy ){
            memcpy(&value,&(it->second),sizeof(T_VALUE));
         }else{
            value=it->second;
         }
         m_pLock->Unlock();
         return true;
      }
      m_pLock->Unlock();
      return false;
   }
   ///find map with key, pointer value
	bool FindMap(const T_KEY& key,T_VALUE*& pOutValue)
   {
      m_pLock->Lock();
      if ( m_pUserMap->empty() ){
         m_pLock->Unlock();
         return false;
      }
      ALUSRMAP_IT it;
      it=m_pUserMap->find(key);
      if( it!=m_pUserMap->end()){
         pOutValue=&(it->second); 
         m_pLock->Unlock();
         return true;
      }
      m_pLock->Unlock();
      return false;
   }
   int GetCount(){
      m_pLock->Lock();
      int nCount=m_pUserMap->size();
      m_pLock->Unlock();
      return nCount;
   }
   ALUSRMAP* GetUserMap(){return m_pUserMap;}
   CCritSec* GetLock(){return m_pLock;}
protected:
	ALUSRMAP* m_pUserMap;
	CCritSec* m_pLock;
};
#endif
