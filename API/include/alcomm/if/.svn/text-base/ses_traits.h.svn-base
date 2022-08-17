#ifndef __ALOX_SESSION_TRAITS_HPP__
#define __ALOX_SESSION_TRAITS_HPP__

///the component session traits
template <typename SesType>
struct CSesTraits
{
   typedef typename SesType::id_type   id_type;
};

///the common interface to access a session information
template <typename SesType, 
   typename SesTraits=CSesTraits<SesType> >
class CSesInfo
{
public:
   ///the implementation type.
   typedef SesType   impl_type;
   ///the session traits type.
   typedef SesTraits traits_type;
   ///the session id type. 
   typedef typename traits_type::id_type id_type;

public:
   ///clear 
   void clear(){
      m_Implementaion.clear();
   }
   ///set the session id
   void set_sesid(id_type& id)
   {
      m_Implementaion.set_sesid(id);
   }
   ///get the session id
   id_type& get_sesid( )
   {
      return m_Implementaion.get_sesid();
   }
   ///get implementation
   impl_type* get_impl(){
      return &m_Implementaion;
   }
protected:
   impl_type  m_Implementaion;
};

#endif
