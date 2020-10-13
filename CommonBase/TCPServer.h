#ifndef TCP_SERVER_H
#define TCP_SERVER_H


#include <map>      //map
#include <memory>   //shared_ptr
#include <mutex>    //mutex

#include "Epoll.h"
#include "TCPClient.h"

namespace Net
{

class TCPServer
{
public:
    TCPServer();
    virtual ~TCPServer();

    int initServer(std::string ip, int port);
    void deleteClient(int fd);

private:
    TCPServer& operator=(const TCPServer&) = delete;
    TCPServer(const TCPServer&) = delete;

    int bindListen();
    int epoll();
    void doEpoll();
    int handleAccept();
    int handleReadData(int fd);

private:
    int m_fd;
    int m_port;
    Epoll m_epoll;
    std::mutex mutex;
    std::string m_ip;
    std::map<int, std::vector<std::string>> m_fdSendBuf;
    std::map<int, std::shared_ptr<TCPClient>> m_fdClientMap;
};

}  //namespace Net


#endif  //TCP_SERVER_H