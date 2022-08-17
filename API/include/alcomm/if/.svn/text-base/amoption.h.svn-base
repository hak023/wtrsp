#ifndef  __AM_OPTION_H__
#define  __AM_OPTION_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class CAMOption 
{
protected:
   char           m_szOpt[16];
   int            m_nCount;
   char     **    m_ppszValue;
public:
   CAMOption(char * szOpt) {
      strcpy(m_szOpt, szOpt);

      m_nCount = 0;
      m_ppszValue = NULL;
   }

   ~CAMOption() {
      if(m_ppszValue != NULL) {
         for(int i=0; i<m_nCount; i++) {
            delete [] m_ppszValue[i];
         }
         delete [] m_ppszValue;
      } 
   }

   void AddValue(char * pszValue) {
   
      char ** ppValue = new char *[m_nCount+1];
      char * pValue = new char[strlen(pszValue)+1];
      strcpy(pValue, pszValue);
      for(int i=0; i<m_nCount; i++) {
         ppValue[i] = m_ppszValue[i];
      }
      ppValue[m_nCount] = pValue;

      if(m_ppszValue != NULL) {
         delete [] m_ppszValue;
      } 
      m_ppszValue = ppValue;

      m_nCount++;
   }

   char * GetOpt() { return m_szOpt; }
   int  GetCount() { return m_nCount; }
   char * GetValue(int nIndex) { return m_ppszValue[nIndex]; }

   void Print() {
      printf(" %s : %d\n",  m_szOpt, m_nCount);
      for(int i=0; i<m_nCount; i++) {
         printf("\t[%d]: %s\n", i, m_ppszValue[i]);
      }
   }
};

class CAMArgs 
{
protected:
   int m_nCount;
   CAMOption        **  m_ppOptions;
public:
   CAMArgs() {
      m_nCount = 0;
      m_ppOptions = NULL;
   };

   ~CAMArgs() {
      if(m_ppOptions != NULL) {
         for(int i=0; i<m_nCount; i++) {
            delete m_ppOptions[i];
         }
         delete [] m_ppOptions;
      } 
   };

   bool Init(int argc, char ** argv) {
      CAMOption * pOpt = NULL;
      for(int i = 0; i<argc; i++) {
         if(*argv[i] == '-') {
            pOpt = AddOption(argv[i]);
         } else if(pOpt != NULL) {
            pOpt->AddValue(argv[i]);
         } else {
            return false;
         }
      }
      return true;
   }

   CAMOption * AddOption(char * pszOpt)
   {
      CAMOption ** ppOpt = new CAMOption *[m_nCount +1];
      CAMOption * pOpt = new CAMOption(pszOpt);

      
      for(int i=0; i<m_nCount; i++) {
         ppOpt[i] = m_ppOptions[i];
      }
      ppOpt[m_nCount] = pOpt;

      if(m_ppOptions != NULL) {
         delete [] m_ppOptions;
      } 
      m_ppOptions = ppOpt;

      m_nCount++;
      return pOpt;
   }

   int GetCount() { return m_nCount; } 

   CAMOption * GetOption(int nIndex) { return m_ppOptions[nIndex]; }

   CAMOption * GetOption(const char * pszOpt) { 
      for(int i=0; i<m_nCount; i++) {
         if(strcmp(m_ppOptions[i]->GetOpt(), pszOpt) == 0) {
            return m_ppOptions[i];
         }
      }
      return NULL;
   }

   void Print() {
      printf("Option : %d\n", m_nCount);

      for(int i=0; i<m_nCount; i++) {
         m_ppOptions[i]->Print();

      }   
   }
};

 
#endif // __AM_OPTION_H__

