
#ifndef ESIPUTIL_EPOLL_H
#define ESIPUTIL_EPOLL_H

#include <sys/epoll.h>

namespace eSipUtil
{

struct Epoll_event
{
	int m_nEventNumber; 							//-1(err), 0(none of event fd), 0<(the number of event fd)
	struct epoll_event * 	m_pEpollEvents;		//don't must be free
	
	Epoll_event()
		:m_nEventNumber(0), m_pEpollEvents(NULL)
	{
	}
};
typedef struct Epoll_event Epoll_event_t;

class Epoll
{
	public:
		Epoll(int _nEpollMaxSize);
		~Epoll();

		bool reset();
		bool addFD(int _nFd, int _nFlag);
		bool modFD(int _nFd, int _nFlag);
		bool delFD(int _nFd);
		int wait(Epoll_event_t & _rstEpollEvent, int _nMiliTimeout = -1);
		int getEpollFD() const;

	private:
		int m_nEpollMaxSize;		//Maximum number of the FD recognized FD when events are happened
		int m_nEpollFD;
		struct epoll_event * 	m_pEpollEvents;
		
};


}

#endif

