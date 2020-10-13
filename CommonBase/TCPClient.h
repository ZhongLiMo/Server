#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H


namespace Net
{

#pragma pack(push)
#pragma pack(1)
struct PackHead
{
    int length;
    int cmd;
    PackHead()
    {
        length = 0;
        cmd = 0;
    }
    void clear()
    {
        length = 0;
        cmd = 0;
    }
};
#pragma pack(pop)

const int READ_BUF_SIZE = 8*1024;

class TCPClient
{
public:
    TCPClient();
    virtual ~TCPClient();

    int handleReadData();
    void resetReadState();
    virtual int dispatch();
    int send();
    int offline();
    bool isOnline();

private:
    TCPClient& operator=(const TCPClient&) = delete;
    TCPClient(const TCPClient&) = delete;

private:
    int m_fd;
    
    static int m_readPos;
    static std::string m_body;
    static PackHead m_packHead;
    static char m_readBuf[READ_BUF_SIZE];
};

}  //namespace Net


#endif  //TCP_CLIENT_H