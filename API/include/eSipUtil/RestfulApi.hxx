#ifndef _RESTFUL_API_H
#define _RESTFUL_API_H
#include "JsonK.hxx"
namespace eSipUtil
{
//-----------> Restful Message Class
class RestfulParam : public JsonObject
{
	public:
		RestfulParam();
		virtual ~RestfulParam();
		bool EMPTY();
		//-----> My Data
		KString & KEY();
		KString & VAL();
		//-----> Get Child Param
		RestfulParam & GET(const char * _pszParam);
		//-----> Set Child Param
		RestfulParam & SET(const char * _pszParam);
		//-----> Del Child Param
		void DEL(const char * _pszParam);
		//-----> Add Array Child Param, Set List Object Attribute
		RestfulParam & ARR();
		//-----> Get Array Child
		RestfulParam & operator[](unsigned int _unIndex);
		RestfulParam & INDEX(unsigned int _unIndex);
		//-----> Get Child Nums
		unsigned int NUMS();
};
class RestfulMsg : public JsonK
{
	public:
		RestfulMsg();
		virtual ~RestfulMsg();
		bool PARSE(const char * _pszJson);
		//-----> Get Child Param
		RestfulParam & GET(const char * _pszParam);
		//-----> Set Child Param
		RestfulParam & SET(const char * _pszParam);
		//-----> Del Child Param
		void DEL(const char * _pszParam);
		//-----> Get Array Child
		RestfulParam & operator[](unsigned int _unIndex);
		//-----> Get Child Nums
		unsigned int NUMS();
		//-----> Get Build Json
		char * STR();
		KString m_szMsg;
};
}

#endif