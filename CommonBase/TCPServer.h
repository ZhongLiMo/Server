#ifndef TCP_SERVER_H
#define TCP_SERVER_H


#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

class TCPServer
{
public:
	TCPServer(int port = 8000);
	virtual ~TCPServer();
private:
	void bind_and_listen();
	void wait_client();
private:
	int		m_port;
	SOCKET  m_socket;
	fd_set m_sock_set;
	fd_set m_read_set;
};













#endif // !TCP_SERVER_H