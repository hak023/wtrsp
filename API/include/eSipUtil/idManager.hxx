
#ifndef ESIPUTIL_ID_MGR_H
#define ESIPUTIL_ID_MGR_H

#include <pthread.h>

namespace eSipUtil
{

class IdMgr
{
	public :
		IdMgr(int _nIdMaxNum);
		~IdMgr();

		int getAvailableId();
		bool getAvailableId(int _nId);
		bool releaseId(int _nId);
		inline unsigned int getAvailableIdNum() const{ return  m_unIdAvailNum; }
		bool reset();

	private :
		enum
		{
			E_CONST_HEAD_ID = -1,
			E_CONST_TAIL_ID = -2,
		};
		
		typedef struct
		{
			int m_nId;
			int m_nPrevId;
			int m_nNextId;
			bool m_bUse;
		}IdNode_t;

		bool pushId(int _nId);
		bool popId(int _nId);

		int m_nIdMaxNum;
		unsigned int m_unIdAvailNum;
		IdNode_t m_stHeadNode;
		IdNode_t m_stTailNode;
		IdNode_t * m_parrIdNode;
		pthread_mutex_t m_Lock;

};

}

#endif

