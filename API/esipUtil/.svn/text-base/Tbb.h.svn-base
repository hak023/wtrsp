#ifndef ESIP_TBB_H
#define ESIP_TBB_H
#ifdef TBB_USE
#include "log.hxx"
#include "lock.hxx"
#include "timeUtil.hxx"
#include "StlMap.hxx"
#ifndef _MSC_VER
#include "task_scheduler_init.h"
#include "blocked_range.h"
#include "concurrent_queue.h"
#include "parallel_for.h"
#include "parallel_while.h"
#include "parallel_do.h"
#include <functional>
#endif
namespace eSipUtil
{
#ifndef _MSC_VER
/******************************* Tbb Task Init ********************************************/
class TbbLib
{
	public:
		TbbLib(){}
		tbb::task_scheduler_init m_Init;
};
/******************************* Static Array Parrallel ASync Proc ******************************/
//순서 보장X,  동기 처리 O
class TbbFor
{
	public:
		typedef void (*PFuncFor_t)(void * _pvUser,unsigned long _nCnt);
		TbbFor(){}
		~TbbFor(){}
		bool m_fnProc(void * _pvUser,PFuncFor_t _pfn,unsigned long _ulStart,unsigned long _ulMax)
		{
			if(_pfn)
			{
				tbb::parallel_for(tbb::blocked_range<unsigned long>(_ulStart,_ulMax),
											TbbForProc(_pvUser,_pfn),tbb::auto_partitioner());
				return true;
			}
			return false;
		}
	private:
		class TbbForProc
		{
			public:
				eSipUtil::RwMutex m_clsLock;
				TbbForProc(void * _pvUser,PFuncFor_t _pfn)
				{
					m_pfnWork = _pfn; m_pvUser = _pvUser;
				}
				~TbbForProc(){}
				void operator()(tbb::blocked_range<unsigned long>&r) const
				{
					if(m_pfnWork)
					{
						for(unsigned long i = (unsigned long)r.begin(); i != (unsigned long)r.end(); ++i )
						{
							m_pfnWork(m_pvUser,i);
						}
					}
				}
				PFuncFor_t m_pfnWork;
				void * m_pvUser;
		};
};
/********************************
//Sample Code
#define DEF_MAX_TBB_LOOP 10000000
int g_nNum[DEF_MAX_TBB_LOOP];
static void s_fnTbbWork(void * _pvUser, unsigned long _ulCnt)   // User Object , Array Index
{
	int * nNums = (int*)_pvUser;         // type casting user array object
	g_nNum[_ulcnt] = _ulCnt;
	char sz[10240];memset(sz,0x00,10240);
}
void g_fnTestTbbFor()
{
	//Normal Loop Code
	for(unsigned long i=0;i<DEF_MAX_TBB_LOOP;i++)
	{
		g_nNum[i]=i;
		char sz[10240];memset(sz,0x00,10240);
	}
	// Tbb Loop Code
	TbbFor clsTask;clsTask.m_fnProc(&g_nNum[0], s_fnTbbWork,0,DEF_MAX_TBB_LOOP);
}
********************************/
/*************************** Stl List Parrallel ASync Proc **************************************/
//순서보장 X, 동기처리 O, 고정된 List 사용
class TbbWhile
{
	public:
		typedef void (*PFuncWhile_t)(void * _pclsUser,eSipUtil::StlObject * _pclsObj);
		TbbWhile(){}
		~TbbWhile(){}
		bool m_fnProc(void * _pvUser,eSipUtil::StlList * _pclsList,PFuncWhile_t _pfn)
		{
			m_clsBody.m_pclsUser = _pvUser;
			if(_pclsList && _pfn)
			{
				m_clsStream.m_plistProc = _pclsList;
				m_clsBody.m_pfnProc = _pfn;
				tbb::parallel_while<TbbWhileBody> w;
				w.run(m_clsStream,m_clsBody);
				return true;
			}
			return false;
		}
	private:
		class TbbWhileStream
		{
			public:
				TbbWhileStream(){m_plistProc=NULL;}
				~TbbWhileStream(){}
				bool pop_if_present(eSipUtil::StlObject *& _ppclsItem)
				{
					if(m_plistProc==NULL) return false;
					eSipUtil::StlObject * pclsFind = (eSipUtil::StlObject*)m_plistProc->m_fnGetNext(m_Itor);
					if(pclsFind) 
					{
						_ppclsItem = pclsFind; return true;
					}
					else return false;
				}
				eSipUtil::StlList * m_plistProc;
				eSipUtil::Iterator m_Itor;
		};
		class TbbWhileBody
		{
			public:
				TbbWhileBody(){m_pfnProc = NULL;m_pclsUser=NULL;}
				void operator()(eSipUtil::StlObject * _pclsItem) const
				{
					if(m_pfnProc) m_pfnProc(m_pclsUser,_pclsItem);
				}
				typedef eSipUtil::StlObject * argument_type;
				PFuncWhile_t m_pfnProc;
				void * m_pclsUser;
		};
		TbbWhileStream m_clsStream;
		TbbWhileBody m_clsBody;
};
/******************************** Tbb Parallel Do Proc *************************************/
//순서보장 X, 동기처리 O, 동적으로 List Increasing 가능
class TbbDo
{
	public:
		typedef void (*PFuncDo_t)(void * _pvUser,eSipUtil::StlObject *  _pclsItem);
		TbbDo(){}
		~TbbDo(){}
		bool m_fnProc(void * _pvUser,eSipUtil::StlList * _pclsList,PFuncDo_t _pfn)
		{
			if(_pfn && _pclsList)
			{
				tbb::parallel_do(_pclsList->m_listR.begin(),_pclsList->m_listR.end(),TbbDoProc(_pvUser,_pfn));
				return true;
			}
			return false;
		}
	private:
		class TbbDoProc
		{
			public:
				TbbDoProc(void * _pvUser,PFuncDo_t _pfn)
				{
					m_pfnWork = _pfn; m_pvUser = _pvUser;
				}
				~TbbDoProc(){}
				void operator()(const unsigned long & _ulVal) const
				{
					if(m_pfnWork && m_pvUser)
					{
						m_pfnWork(m_pvUser,(eSipUtil::StlObject*)_ulVal);
					}
				}
				PFuncDo_t m_pfnWork;
				void * m_pvUser;
		};
};
#endif
}
#endif
#endif
