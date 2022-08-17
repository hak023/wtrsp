
#ifndef ESIPUTIL_CONDITION_H
#define ESIPUTIL_CONDITION_H

#include <pthread.h>

namespace eSipUtil
{

class Mutex;

class Condition
{
	public:
		Condition();
		virtual ~Condition();

		void wait(Mutex & _rclsMutex);
		bool wait(Mutex & _rclsMutex, unsigned int _unMs);
		void signal();
		void broadcast();

	private:
		mutable  pthread_cond_t m_ConditionId;

};

}

#endif

