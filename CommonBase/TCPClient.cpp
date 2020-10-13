#include "TCPClient.h"

#include <error.h>

#include "TCPServer.h"


namespace Net
{

extern TCPServer tcpServer;

TCPClient::TCPClient() : m_fd(-1)
{

}

TCPClient::~TCPClient()
{
    if (m_fd != -1)
    {
        close(m_fd);
    }
}

int TCPClient::handleReadData()
{
    int ret;
    while (isOnline())
    {
        ret = read(m_fd, &m_readBuf[m_readPos], READ_BUF_SIZE - m_readPos);
        if (ret < 0) 
        {
            if (errno == EAGAIN || errno == EINPROGRESS || errno == EINTR) 
            {
                return 0;
            }
            
            tcpServer.deleteClient(m_fd);
            return -1;
        }
        if (ret == 0) 
        {
            
            tcpServer.deleteClient(m_fd);
            return -1;
        }
        m_readPos += ret;
        if (m_packHead.cmd == 0)
        {
            if (m_readPos == sizeof(m_packHead))
            {
                memcpy_s(m_packHead, READ_BUF_SIZE, m_readBuf, READ_BUF_SIZE);
                if (m_packHead.length > READ_BUF_SIZE || m_packHead.length < 0)
                {
                    tcpServer.deleteClient(m_fd);
                    return -1;
                }
                m_readPos = 0;
                if (m_packHead.length == 0) 
                {
                    dispatch();
                }
            }
        }
        else
        {
            if (m_packHead.length > 0)
            {
                m_body.append(m_readBuf, ret);
            }
            if (m_body.length() == m_packHead.length) 
            {
                dispatch();
            }
        }
    }
    return 0;
}

void TCPClient::resetReadState()
{
    m_readPos = 0;
    m_body.clear();
    m_packHead.clear();
}

int TCPClient::dispatch()
{
    struct timeval begin, end;
    gettimeofday(&begin, NULL);
    printf("cmd:%d  dada:%s", m_packHead.cmd, m_body.c_str());
    gettimeofday(&end, NULL);
    resetReadState();
    int usetime = 1000000 * (end.tv_sec - begin.tv_sec) + end.tv_usec - begin.tv_usec;
    if(usetime >= 1000000)
    {
        
    }
    return 0;
}

int TCPClient::send()
{

}

int TCPClient::offline()
{
    m_fd = -1;
}

bool TCPClient::isOnline()
{
    return (-1 == m_fd);
}


}  //namespace Net