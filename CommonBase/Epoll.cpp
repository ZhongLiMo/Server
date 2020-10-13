#include "Epoll.h"

#include <sys/epoll.h>	//epoll


namespace Net
{

Epoll::Epoll() : m_epfd(-1)
{

}

Epoll::~Epoll()
{
	destroy();
}

int Epoll::create()
{
    m_epfd = epoll_create(10000);
    if (m_epfd == -1)
    {
		return -1;
    }
	return 0;
}

void Epoll::destroy()
{
	if (m_epfd != -1)
	{
		close(m_epfd);
	}
}

int Epoll::listen(struct epoll_event* events, int size)
{
	return epoll_wait(m_epfd, events, size, -1);
}

int Epoll::addListener(int fd)
{
	struct epoll_event event;
	event.data.ptr = NULL;
    event.events = EPOLLIN|EPOLLET;
	return epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &event);
}

int Epoll::removeListener(int fd)
{
	struct epoll_event event;
	event.data.ptr = NULL;
    event.events = 0;
	return epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, &event);
}

}  //namespace Net