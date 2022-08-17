/**
 * @file  eSipRteUsrMap.hxx
 * the sip route user map header
 *
 * @attention Copyright (c) 2014 POINT-I Co., Ltd, Seoul, South Korea \n\n
 * All rights are reserved.  This is unpublished proprietary source
 * code of POINT-I.\n The copyright notice does not evidence any actual
 * publication of such source code.
 *
 * @par Project : Point-I Service Platform
 * @par Last Changed : 
 * @par 2014.11.12   hc_yun   1.0     first written
 */

#ifndef __ESIP_CORE_SIP_ROUTE_USER_MAP__
#define __ESIP_CORE_SIP_ROUTE_USER_MAP__

#include "eSipRteUtil.hxx"
#include "rutil/RWMutex.hxx"

#include <tr1/unordered_map>
//#include <map>

namespace eSIP
{

//key bypass integer
struct MapKeyBypass_t{
   size_t operator()(int _nValue) const{
      return (size_t)_nValue;
   }
};

//ses mapping
template < typename T_KEY,typename T_VALUE,typename T_HASH=std::tr1::hash<T_KEY> >
class CUsrMap
{
private:
	///defines types
	typedef std::tr1::unordered_map<T_KEY,T_VALUE,T_HASH> ALUSRMAP;
   typedef std::pair<T_KEY,T_VALUE> VALUE_TYPE;
	typedef typename ALUSRMAP::iterator ALUSRMAP_IT;
public:
   ///contstructor
   CUsrMap()
   {
      m_pUserMap= new ALUSRMAP;
      m_pLock= new resip::RWMutex;
   }
   ///distructor
   ~CUsrMap()
   {
   }
   ///Insert map
	bool m_fnInsertMap(const T_KEY& key,const T_VALUE& value)
   {
      m_pLock->lock();
      int nBeforeSize=m_pUserMap->size();
      m_pUserMap->insert(VALUE_TYPE(key,value));	
      int nAfterSize=m_pUserMap->size();
      m_pLock->unlock();
      if(nBeforeSize >= nAfterSize ) return false;
      return true;
   }
   ///delete map
	bool m_fnDeleteMap(const T_KEY& key)
   {
      m_pLock->lock();
      if ( m_pUserMap->empty() ){
         m_pLock->unlock();
         return false;
      }
      ALUSRMAP_IT it;
      it=m_pUserMap->find(key);
      if( it!=m_pUserMap->end()){
         m_pUserMap->erase(it++);
         m_pLock->unlock();
         return true;
      }
      m_pLock->unlock();
      return false;
   }
   ///find map with key, reference value
	bool m_fnFindMap(const T_KEY& key,T_VALUE& value,bool bMemcpy=false)
   {
      //m_pLock->lock();
      m_pLock->readlock();
      if ( m_pUserMap->empty() ){
         m_pLock->unlock();
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
         m_pLock->unlock();
         return true;
      }
      m_pLock->unlock();
      return false;
   }
   void m_fnClear( )
   {
      m_pLock->lock();
      m_pUserMap->clear();
      m_pLock->unlock();
   }

   int m_fnGetCount(){
      m_pLock->lock();
      int nCount=m_pUserMap->size();
      m_pLock->unlock();
      return nCount;
   }
   ALUSRMAP* GetUserMap(){return m_pUserMap;}
   resip::RWMutex* GetLock(){return m_pLock;}
protected:
	ALUSRMAP* m_pUserMap;
	resip::RWMutex* m_pLock;
};


}; //end namespace
#endif
