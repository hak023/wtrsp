#ifndef _STLMAP_H
#define _STLMAP_H
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include <iterator>
#include <list>
#include <tr1/unordered_map>

namespace eSipUtil
{
typedef std::list<unsigned long> StlListLong_t;
typedef StlListLong_t::iterator ListItr_t;
typedef std::tr1::unordered_map<unsigned int, unsigned long> StlMapInt_t;
typedef std::tr1::unordered_map<std::string, unsigned long> StlMapStr_t;
typedef StlMapInt_t::iterator MapIntItr_t;
typedef StlMapStr_t::iterator MapStrItr_t;
typedef bool (*PFuncSortList_t)(unsigned long, unsigned long);   
/*
<Ascending Ordering> Call Back Fuction Sample
bool s_fnCbkSort(unsigned long _pvUser1, unsigned long _pvUser2)
{
	UserClass * pclsUser1 = (UserClass *)__pvUser1;
	UserClass * pclsUser1 = (UserClass *)__pvUser2;
	if(pclsUser1->m_nID < pclsUser1->m_nID) return true;
	return false;
}
*/
class Iterator
{
	public:
		Iterator(){m_unCnt = 0;}
		~Iterator(){}
		ListItr_t & m_fnGetNextList()
		{
			m_unCnt++; return m_stItorList;
		}
		MapIntItr_t & m_fnGetNextMapInt()
		{
			m_unCnt++; return m_stMapItorInit;
		}
		MapStrItr_t & m_fnGetNextMapStr()
		{
			m_unCnt++; return m_stMapItorStr;
		}
		ListItr_t m_stItorList;
		MapIntItr_t m_stMapItorInit;
		MapStrItr_t m_stMapItorStr;
		unsigned int m_unCnt;
};
typedef unsigned int STLKEYNUM;
typedef char * STLKEYSTR;

/******************* Base Object of (StlList or StlMap) ***************************************/ 
class StlObject
{
	public:
		StlObject(){m_uiKey=0;m_pszKey = NULL;}
		virtual ~StlObject(){if(m_pszKey) delete [] m_pszKey;}
		bool operator==(unsigned int _uiKey)
		{
			if(m_uiKey == _uiKey) return true;
			return false;
		}
		bool operator==(const char * _pszKey)
		{
			if(_pszKey==NULL) return false;
			if(strcmp(m_pszKey,_pszKey)==0) return true;
			return false;
		}
		operator STLKEYNUM(){return m_uiKey;}
		operator STLKEYSTR(){return m_pszKey;}
		void m_fnSetKey(unsigned int _uiKey){m_uiKey = _uiKey;}
		void m_fnSetKey(const char * _pszKey)
		{
			if(m_pszKey) delete [] m_pszKey;
			m_pszKey = m_fnCreateCopy(_pszKey);
		}
		unsigned int m_uiKey;
		char *m_pszKey;
	private:
		char * m_fnCreateCopy(const char * _pszSrc)
		{
			if(_pszSrc == NULL) return NULL;
			unsigned int unLen = strlen(_pszSrc);
			char *pszTmp = new char [unLen+1];
			strcpy(pszTmp,_pszSrc);pszTmp[unLen] = 0x00;
			return pszTmp;
		}
};
/******************************* RU stl list ********************************************/
class StlList
{
	public:
		StlList();
		~StlList();
		bool  m_fnPushBack(StlObject *_pclsObj,bool _bChk=false);
		bool  m_fnPushFront(StlObject *_pclsObj,bool _bChk=false);
		bool m_fnInsertIndex(StlObject * _pclsObj,unsigned int _uiIndex,bool _bChk);
		void * m_fnFindNode(unsigned int _uiKey);
		void * m_fnFindNode(const char * _pszKey);
		void * m_fnFindNode(StlObject *_pclsObj);
		void * m_fnGetIndex(unsigned int _uiIndex);
		bool m_fnDelHead();
		bool m_fnDelNode(unsigned int _uiKey);
		bool m_fnDelNode(const char * _pszKey);
		bool m_fnDelTail();
		bool m_fnClear();
		void * m_fnBegin(ListItr_t & _rclsItem);
		void * m_fnNext(ListItr_t & _rclsItem);
		bool m_fnIsEnd(ListItr_t & _rclsItem);
		void * m_fnGetNext(Iterator & _rclsItor);
		void m_fnSort(PFuncSortList_t _pfn);  //ascending order ==>  if(user1 < user2)  return true;
		void m_fnReverse();
		unsigned int m_fnGetSize();
		StlListLong_t m_listR;
	private:
		ListItr_t m_listItor;
};
class StlMap
{
	public:
		StlMap();
		~StlMap();
		void * m_fnFindMap(unsigned int _nKey);
		void * m_fnFindMap(const char * _pszKey);
		bool m_fnSetMap(unsigned int _nKey, StlObject * _pclsObj);  
		bool m_fnSetMap(const char * _pszKey,StlObject * _pclsObj);
		bool m_fnDelMap(unsigned int _nKey);
		bool m_fnDelMap(const char * _pszKey);
		void m_fnClear();
		void * m_fnBeginI(MapIntItr_t & _rclsItem);
		void * m_fnNextI(MapIntItr_t & _rclsItem);
		bool m_fnIsEndI(MapIntItr_t & _rclsItem);
		void * m_fnBeginS(MapStrItr_t & _rclsItem);
		void * m_fnNextS(MapStrItr_t & _rclsItem);
		bool m_fnIsEndS(MapStrItr_t & _rclsItem);
		void * m_fnGetNextI(Iterator & _rclsItor);
		void * m_fnGetNextS(Iterator & _rclsItor);
		unsigned int m_fnGetSizeInt();
		unsigned int m_fnGetSizeStr();
		StlMapInt_t m_mapR;
		StlMapStr_t m_mapS;
	private:
		MapIntItr_t m_mapItorInt;
		MapStrItr_t m_mapItorStr;
};

}
#endif

