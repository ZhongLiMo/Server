#ifndef EPOLL_H
#define EPOLL_H


struct epoll_event;

namespace Net
{

class Epoll
{
public:
	Epoll();
	~Epoll();
	
public:
	int create();
	void destroy();
	int listen(struct epoll_event* events, int size);
	int addListener(int fd);
	int removeListener(int fd);

private:
	Epoll(const Epoll&) = delete;
	Epoll& operator=(const Epoll&) = delete;
	
private:
    int m_epfd;
};

}  //namespace Net


#endif  //EPOLL_H