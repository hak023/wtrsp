#include "aloxConf.h"

CALConf::CALConf()
   : m_comment('#'),
     m_pConf(new ALCONF_MAP),
     m_Fp(NULL)
{
}

CALConf::~CALConf()
{
   //delete
   ALOX_CLOSE_FP(m_Fp);
   //delete all
   DeleteAll();   
   delete m_pConf;
}

bool CALConf::Init(char* pFileName, char comment)
{
  m_comment=comment;
  if(!pFileName) return false;

  ALOX_CLOSE_FP(m_Fp);

  m_Fp=fopen(pFileName,"r");
  if(!m_Fp) return false;

 
  char linebuf[1024];
  char secName[128];
  char pairName[2][128];
  char* pTmp=NULL;
  int nRet=0;
  memset(secName,0x00,sizeof(secName));
  
  while(fgets(linebuf, 1024, m_Fp) != NULL)
  {
    if( linebuf[0]==m_comment ||
        linebuf[0]==' ' ||
        linebuf[0]==0x00 )
    {
      continue;
    } 
    CALUtil::LRTrim(linebuf);
   
    // reverse comment
    pTmp=NULL;
    pTmp=strrchr(linebuf,m_comment);
    if(pTmp) *pTmp=0x00;
   
    // comment
    pTmp=NULL;
    pTmp=strchr(linebuf,m_comment);
    if(pTmp) *pTmp=0x00;

    if(strlen(linebuf)==0) continue;

    //get sect
    if(linebuf[0]=='[')
    {
      pTmp=NULL;
      pTmp=strchr(linebuf,']');
      if(pTmp) *pTmp=0x00;
      strncpy(secName,linebuf+1,sizeof(secName)-2);
      continue;
    }

    //if not section name
    if(secName[0] == 0x00)  continue;

    //key value
    nRet=CALUtil::StrTokenFix(linebuf,(char*)"=",pairName,2);
    if(nRet !=0)  continue;
   
    SetValue(secName,pairName[0],pairName[1]);   
  }
  ALOX_CLOSE_FP(m_Fp);
  return true;
}

bool CALConf::SetValue(char* pSect,char* pKey,char* pValue)
{
  ALCONF_MAP_IT conf_it;
  //ALVALUE_MAP_IT v_it;

  std::string strRootKey(pSect);
  std::string strSubKey(pKey);
  std::string strValue(pValue);

 
  conf_it=m_pConf->find(strRootKey);
  //if exist
  if( conf_it != m_pConf->end() )
  {
    conf_it->second.insert(std::pair<std::string,std::string>(strSubKey,strValue));
  }
  else
  {
    ALVALUE_MAP valueMap;
    valueMap.insert(std::pair<std::string,std::string>(strSubKey,strValue)); 
    m_pConf->insert(std::pair<std::string,ALVALUE_MAP>(strRootKey,valueMap));
  } 
  return true;
}
bool CALConf::GetValue(const char* pSect, const char* pKey, char* pValue)
{
  ALCONF_MAP_IT conf_it;
  ALVALUE_MAP_IT v_it;
 
  std::string strRootKey(pSect);
  std::string strSubKey(pKey);

  conf_it=m_pConf->find(strRootKey);
  if(  conf_it != m_pConf->end() )
  {
    v_it=conf_it->second.find(strSubKey);
    if( v_it != conf_it->second.end() )
    {
      strcpy(pValue,(v_it->second).c_str());
      return true;
    }

  }
  return false;
}
bool CALConf::IsCheckDigit(char* str, int nLen)
{
   for( int i =0 ; i < nLen ; i++)
   {
      if ( !(*(str+i) >= '0' &&  *(str+i) <= '9') )
      {
         return false;
      }
   }
   return true;
}

bool CALConf::IsCheckHexaDigit(char* str, int nLen)
{
   for( int i =0 ; i < nLen ; i++)
   {
      if ( !((*(str+i) >= '0' &&  *(str+i) <= '9') ||
           (*(str+i) >= 'a' &&  *(str+i) <= 'f') ||
           (*(str+i) >= 'A' &&  *(str+i) <= 'F')) )
      {
         return false;
      }
   }
   return true;
}

int CALConf::GetValueInt(const char* pSect, const char* pKey)
{

   ALCONF_MAP_IT conf_it;
   ALVALUE_MAP_IT v_it;

   std::string strRootKey(pSect);
   std::string strSubKey(pKey);

   char pValue[128];
   int strLen=0;
   conf_it=m_pConf->find(strRootKey);
   if(  conf_it != m_pConf->end() )
   {
      v_it=conf_it->second.find(strSubKey);
      if( v_it != conf_it->second.end() )
      {
         strcpy(pValue,(v_it->second).c_str());
         //len
         strLen=strlen(pValue);
         if( strLen <= 0 ) return -1;

         if ( strLen <= 2 )
         {
            if( !IsCheckDigit(pValue,strLen) ) return -1;
            return atoi(pValue);
         }else
         {
            //check 0x 
            if( strncasecmp(pValue, "0x", 2) == 0 ||
                strncasecmp(pValue, "0X", 2) == 0 )
            {
               if( !IsCheckHexaDigit(pValue+2,strLen-2) ) return -1;
               return axtoi(pValue);
            }
            else
            {
               if( !IsCheckDigit(pValue,strLen) ) return -1;
               return atoi(pValue);
            }
         }
      }
   }
   return -1;
}

void CALConf::PrintAll()
{
  ALCONF_MAP_IT conf_it;
  ALVALUE_MAP_IT v_it;

  for( conf_it=m_pConf->begin();conf_it!=m_pConf->end() ; conf_it++)
  {
    for(v_it = conf_it->second.begin() ;v_it != conf_it->second.end() ;v_it++)
    {
      printf("[%s] [%s] [%s]\n",
             (conf_it->first).c_str(),
             (v_it->first).c_str(),
             (v_it->second).c_str());
    }

  }
}

void CALConf::DeleteAll()
{
  if(m_pConf->empty() != true ) m_pConf->clear();
}
