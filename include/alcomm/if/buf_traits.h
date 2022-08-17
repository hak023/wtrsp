#ifndef __ALOX_BUFFER_TRAITS_HPP__
#define __ALOX_BUFFER_TRAITS_HPP__

//define buffer
template<unsigned short T_usSize>
struct al_buffer_traits
{
   ///type define 
   typedef unsigned int id_type;

   //user data
   unsigned int         ses_id;
   unsigned short       us_cap_len;
   unsigned short       us_real_len;
   char*                p_data;

   ///constructor
   al_buffer_traits()
      : ses_id(0),
        us_cap_len(T_usSize),
        us_real_len(us_cap_len)
   {
   }
   ///distructor
   ~al_buffer_traits()
   {

   }
   ///initialize
   void init()
   {
      p_data=new char[us_cap_len];
      clear();
   }

   ///stop
   void stop()
   {
      us_real_len=0;
      us_cap_len=0;
      if(p_data){
         delete[] p_data;
         p_data=NULL;
      }
   }
   ///clear
   void clear()
   {
      memset(p_data,0x00,sizeof(char)*us_cap_len);
      us_real_len=0;
   }
   ///set ses id
   void set_sesid(unsigned int& id)
   {
      ses_id=id;
   }
   ///get ses id
   unsigned int get_sesid()
   {
      return ses_id;
   }

   ///increase buffer
   void set_buffer(int len,char* in_data)
   {
      if( us_cap_len < len)
      {
         delete[] p_data;
         p_data=NULL;
         us_cap_len=len;
         p_data=new char[us_cap_len]; 
      }
      us_real_len=len;
      memcpy(p_data,in_data,us_real_len);
   }
   ///get buffer size
   void get_buffer_size()
   {
      return us_cap_len;
   }
   ///get data size
   void get_data_size()
   {
      return us_real_len;
   }
};

#undef SVC_SES_TYPE_CLEAR
#undef SVC_SES_TYPE_SET_SESID
#undef SVC_SES_TYPE_GET_SESID
#undef ALOX_INIT_USRBUFFER_SIZE

#endif
