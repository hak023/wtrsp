#ifndef _ALOX_CONFIG_H_
#define _ALOX_CONFIG_H_

#include "aloxUtil.h"
#include <map>
#include <string>



class CALConf
{
private:
   typedef std::map<std::string,std::string> ALVALUE_MAP;
   typedef std::map<std::string,ALVALUE_MAP> ALCONF_MAP;


   typedef std::map<std::string,std::string>::iterator ALVALUE_MAP_IT;
   typedef std::map<std::string,ALVALUE_MAP>::iterator ALCONF_MAP_IT;
public:
    CALConf();
    virtual ~CALConf();
    bool Init(char* pFileName, char comment='#');
    bool GetValue(const char* pSect, const char* pKey, char* pValue);
    void PrintAll();
    void DeleteAll();

    int GetValueInt(const char* pSect, const char* pKey);
    static bool IsCheckDigit(char* str, int nLen);
    static bool IsCheckHexaDigit(char* str, int nLen);
protected:
    char        m_comment;
    ALCONF_MAP* m_pConf;
    FILE* m_Fp;
   
protected:
    bool SetValue(char* pSect,char* pKey,char* pValue);
};






#endif
