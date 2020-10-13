#include "TCPServer.h"


namespace Net
{

//只有一个线程会访问，不需要锁
static const int EVENTS_SIZE = 100;
static struct epoll_event events[EVENTS_SIZE];

TCPServer::TCPServer() : m_fd(-1), m_port(-1)
{

}

TCPServer::~TCPServer()
{
    if (m_fd != -1)
    {
        close(m_fd);
    }
}

int TCPServer::initServer(std::string ip, int port)
{
    m_ip = ip;
    m_port = port;
    int ret = bindListen();
    if (ret != 0)
    {
        return ret;
    }
    ret = epoll();
    if (ret != 0)
    {
        return ret;
    }
    return 0;
}

void TCPServer::deleteClient(int fd)
{
    std::map<int, std::shared_ptr<TCPClient>>::iterator ite = m_fdClientMap.find(fd);
    if (ite != m_fdClientMap.end())
    {
        ite->second->offline();
        m_fdClientMap.erase(ite);
        m_epoll.removeListener(fd);
    }
}

int TCPServer::bindListen()
{
    struct sockaddr_in addr;
    m_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (m_fd < 0)
    {
		return -1;
	}
    bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);
	addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
	if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        close(m_fd);
		return -2;
	}
	int on = 1;
	if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
		close(m_fd);
		return -3;
	}
	if (bind(m_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
		close(m_fd);
		return -4;
	}
	fcntl(m_fd, F_SETFL, fcntl(m_fd, F_GETFL, 0)|O_NONBLOCK);
	listen(m_fd, 10000);
    return 0;
}

int TCPServer::epoll()
{
    int ret = m_epoll.create();
    if (ret != 0)
    {
        return ret;
    }
    ret = m_epoll.addListener(m_fd);
    if (ret != 0)
    {
        return ret;
    }
    std::thread epollThread(&TCPServer::doEpoll, std::ref(this));
    epollthread.detach();
    return 0;
}

void TCPServer::doEpoll()
{
    while (1)
    {
        int num = m_epoll.listen(events, EVENTS_SIZE);
        if (num > 0)
        {
            for (int i = 0; i < num; ++i)
            {
                if (events[i].events&EPOLLIN)
                {
                    if (events[i].data.fd == m_fd)
                    {
                        handleAccept();
                        //std::thread acceptThread(&TCPServer::handleAccept, std::ref(this));
                        //acceptThread.detach();
                    }
                    else
                    {
                        handleReadData(events[i].data.fd);
                        //std::thread readDataThread(&TCPServer::handleReadData, std::ref(this), events[i].data.fd);
                        //readDataThread.detach();
                    }
                }
            }
        }
    }
}

int TCPServer::handleAccept()
{
    
}

int TCPServer::handleReadData(int fd)
{
    std::map<int, std::shared_ptr<TCPClient>>::iterator ite = m_fdClientMap.find(fd);
    if (ite != m_fdClientMap.end())
    {
        ite->second->handleReadData();
    }
}

}  //namespace Net