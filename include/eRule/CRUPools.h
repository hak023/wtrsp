#ifndef RUPOOLS_H
#define RUPOOLS_H
#include "CRuleUtiles.h"

/******************************** Static Pools Structor *************************************/
#define STATIC_POOL_DATA     1024
#define STATIC_POOL_MAX        10000
typedef struct StaticPool_t
{
	StaticPool_t(){m_unIdx=0;fnClear();}
	~StaticPool_t(){}
	void fnClear()
	{
		m_bUse=0;m_nNext=-1;m_unSTime=0;memset(m_arrData,0x00,STATIC_POOL_DATA);
	}
	void fnInit(unsigned int _unIndex){m_unIdx = _unIndex;}
	void fnAlloc(){m_bUse = 1;}
	void fnFree(bool bAll)
	{
		m_bUse = 0; m_nNext=-1;m_unSTime=0;if(bAll) memset(m_arrData,0x00,STATIC_POOL_DATA);
	}
	unsigned int m_unIdx;
	unsigned int m_unMax;
	unsigned int m_bUse;
	unsigned int m_unSTime;
	int m_nNext;
	char m_arrData[STATIC_POOL_DATA];
}StaticPool_t;
typedef struct StaticPools_t
{
	StaticPools_t(){}
	~StaticPools_t(){}
	int m_nHead;
	int m_nTail;
	StaticPool_t m_arrSession[STATIC_POOL_MAX];
	void fnInit()
	{
		for(unsigned int i=0;i<STATIC_POOL_MAX;i++)
		{
			m_arrSession[i].fnInit(i);
			if(i != (STATIC_POOL_MAX-1))
			{
				m_arrSession[i].m_nNext = i+1;
			}
		}
		m_nHead = 0;
		m_nTail = STATIC_POOL_MAX-1;
	}
	StaticPool_t * fnAlloc()
	{
		// Free Session List Deleted
		if(m_nHead == -1) return NULL;
		if(m_nHead >= STATIC_POOL_MAX) return NULL;
		StaticPool_t *pRet = &m_arrSession[m_nHead];
		if(pRet->m_bUse == 1) return NULL;
		pRet->fnAlloc();
		m_nHead = m_arrSession[m_nHead].m_nNext;
		if(m_nHead == -1)
		{
			m_nTail = -1;
		}
		return pRet;
	}
	bool fnFree(StaticPool_t *pFree,bool bClear)
	{
		// Free Session List Added
		if(pFree->m_bUse == 0) return false;
		pFree->fnFree(bClear);
		if(m_nTail == -1)
		{
			m_nHead = pFree->m_unIdx;m_nTail = pFree->m_unIdx;
		}
		else
		{
			m_arrSession[m_nTail].m_nNext = pFree->m_unIdx;m_nTail = pFree->m_unIdx;
		}
		return true;
	}
}StaticPools_t;
/***************************** Pools Data Class *******************************************/
class RUPool
{
	public:
		RUPool(){}
		~RUPool(){}
		void fnClear(){m_bUse = false;m_pvUser = NULL;m_unTime = 0;m_pclsNext = NULL;}
		bool m_bUse;
		unsigned int m_unIdx;
		RUPool * m_pclsNext;
		void * m_pvUser;
		unsigned int  m_unTime;
};
class RUPools
{
	public:
		RUPools();
		~RUPools();
		void fnCreatePool(unsigned int _unMax);
		RUPool *fnAllocPool();
		void fnFreePool(RUPool * _pclsPool);
		RUPool * m_arrPools;
		RUPool * m_pclsFreeHead;
		RUPool * m_pclsFreeTail;
	private:
		unsigned int m_unMax;
};
void RUPools_Test();
#endif

