#ifndef __ALOX_USER_POOL_HPP__
#define __ALOX_USER_POOL_HPP__


#include "aloxCommon.h"
#include "ses_traits.h"
#include <map>
#include <deque>

#define ALOX_INIT_POOLSIZE   (1024)
//session mapping
template < typename SesType, 
           int T_InitSize=ALOX_INIT_POOLSIZE >
class cuser_pool
   : private CUncopyable
{
public:
   ///session element
   typedef SesType pool_element;
   ///session element ptr
   typedef pool_element* pool_element_ptr;
   ///session traits
   typedef CSesTraits<pool_element> traits_type;
   ///session id type
   typedef typename traits_type::id_type id_type;

private:

   ///the using element map
   typedef std::map<id_type, pool_element_ptr > using_map;

   ///the not using deque
   typedef std::deque<pool_element_ptr> not_using_deque;

   ///the using element map value type
   typedef std::pair<id_type, pool_element_ptr > value_type;

   ///the using element map iterator
   typedef typename using_map::iterator using_map_it;

   ///the not using deque iterator
   typedef typename not_using_deque::iterator not_using_deque_it;


public:
   ///constructor
   cuser_pool()
      : m_nInitMaxCnt(T_InitSize),
        m_pUsingMap(new using_map),
        m_pNotUsingDeque(new not_using_deque),
        m_pLock(new CCritSec) 
   {

   }
   ///distructor
   ~cuser_pool()
   {
      delete m_pLock;
      delete m_pUsingMap;
      delete m_pNotUsingDeque;
   }

   ///all delete
   void all_delete()
   {

      using_map_it it;
      pool_element_ptr pUserObj=NULL;

      for(it=m_pUsingMap->begin(); it!=m_pUsingMap->end() ;it++)
      {
         pUserObj=it->second;
         pUserObj->stop();
         delete pUserObj;
      }

      not_using_deque_it it2;
      for(it2=m_pNotUsingDeque->begin(); it2!=m_pNotUsingDeque->end() ;it2++)
      {
         pUserObj=(pool_element_ptr)(*it2);
         pUserObj->stop();
         delete pUserObj;
      }
   }

   ///initialize
   bool initialize(int nMaxCnt=0)
   {
      //set max count
      if( nMaxCnt > 0 ) m_nInitMaxCnt=nMaxCnt;

      m_pLock->Lock();

      //clear
      m_pUsingMap->clear();
      m_pNotUsingDeque->clear();

      //insert vector & not using user map
      id_type element_id;
      for(int i=0; i < m_nInitMaxCnt ; i++){
         pool_element_ptr pUserObj = NULL;
         pUserObj=new pool_element;
         if( pUserObj == NULL ){
            m_pLock->Unlock();
            return false;
         }
         //type-casting
         element_id=id_type(i);
         pUserObj->init();
         pUserObj->set_sesid(element_id);

         m_pNotUsingDeque->push_back(pUserObj);
      }

      //check the size  
      if ( m_pNotUsingDeque->size() != (unsigned int)m_nInitMaxCnt )
      {
         m_pLock->Unlock();
         return false;
      }

      m_pLock->Unlock();
      return true;
   }

   ///get session
   int  get_session(pool_element_ptr& out_ptr)
   {

      not_using_deque_it not_using_it;
      int i_ret=0;

      int i_before_size=0;
      int i_after_size=0;

      id_type element_id;
      ////////////////////LOCK////////////////
      m_pLock->Lock();

      //dummy loop
      do{
         //check empty. user is full
         if (m_pNotUsingDeque->empty()){
            i_ret=0;
            break;
         }

         //get not using elemenet
         not_using_it=m_pNotUsingDeque->begin();
         out_ptr=(pool_element_ptr)(*not_using_it);
         element_id=out_ptr->get_sesid();

         //insert using map
         i_before_size=m_pUsingMap->size();
         m_pUsingMap->insert(value_type(element_id,out_ptr));
         i_after_size=m_pUsingMap->size();

         if ( i_before_size >= i_after_size ){
            i_ret=-1;
            break;
         }
         //erase not using map
         m_pNotUsingDeque->pop_front();
      }while(0); //end do
      ////////////////////UNLOCK////////////////
      m_pLock->Unlock();
      i_ret=i_after_size;

      return i_ret;  
   }

   ///delete session
   void del_session(id_type element_id)
   {
      using_map_it using_it;
      m_pLock->Lock(); 

      if ( m_pUsingMap->empty() ){
         m_pLock->Unlock();
         return;
      }
      //find
      using_it=m_pUsingMap->find(element_id);
      if( using_it == m_pUsingMap->end() ){
         m_pLock->Unlock();
         return;
      }

      //session clear
      pool_element_ptr out_ptr=using_it->second;
      out_ptr->clear();

      //insert not using session map
      m_pNotUsingDeque->push_back(out_ptr);

      //erase
      m_pUsingMap->erase(using_it++);  

      m_pLock->Unlock();
   }

   ///find session
   bool find_session(id_type element_id,pool_element_ptr& out_ptr)
   {
      using_map_it using_it;
      //find
      m_pLock->Lock();
      if ( m_pUsingMap->empty() ){
         m_pLock->Unlock();
         return false;
      }
      //find
      using_it=m_pUsingMap->find(element_id);
      if( using_it != m_pUsingMap->end() ){
         out_ptr=(using_it->second);
         m_pLock->Unlock();
         return true;
      }
      m_pLock->Unlock();
      return false;
   }

   ///increase pool 
   bool increase_pool(int i_plus_num)
   {
      if( i_plus_num <= 0  ) return false;

      m_pLock->Lock();
      int i_current_max_idx=m_nInitMaxCnt;
      id_type element_id;
      bool b_ret=false;
      do{
         //insert vector, NotUsingSesMap
         for(int i=0; i < i_plus_num ; i++){
            pool_element_ptr pUserObj = NULL;
            pUserObj=new pool_element;
            if( pUserObj == 0  ){
               m_pLock->Unlock();
               return false;
            }
            //element id
            element_id=id_type(i_current_max_idx+i);
            pUserObj->init();
            //set
            pUserObj->set_sesid(element_id);

            //insert not using queue
            m_pNotUsingDeque->push_back(pUserObj);
         }//end for
         m_nInitMaxCnt+=i_plus_num;
         b_ret=true;
      }while(0);

      m_pLock->Unlock();
      return b_ret;
   }

   ///get the using pool size
   int  get_using_size()
   {
      m_pLock->Lock(); 
      int i_size=m_pUsingMap->size();
      m_pLock->Unlock(); 
      return i_size;
   }

   ///get the notusing pool size
   int  get_notusing_size(){
      m_pLock->Lock(); 
      int i_size=m_pNotUsingDeque->size();
#if 0
      int i_2size=m_pUsingMap->size();
      if ( i_size+i_2size != m_nInitMaxCnt)
      {
         printf("ERR!!!! %d/%d\n",i_size+i_2size,m_nInitMaxCnt);
         LOGGER(CLogger::Error,"ERR_YUN!!!! %d/%d\n",i_size+i_2size,m_nInitMaxCnt);
      }
#endif
      m_pLock->Unlock(); 
      return i_size;
   };
protected:
   int               m_nInitMaxCnt;
   using_map*        m_pUsingMap;
   not_using_deque*  m_pNotUsingDeque;
   CCritSec*         m_pLock;
};


#endif
