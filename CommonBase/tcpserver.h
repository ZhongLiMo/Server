#ifndef TCP_SERVER_H
#define TCP_SERVER_H


#include <map>
#include <memory>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#include "tcpclient.h"


class TCPServer
{
public:
	TCPServer(int port = 8000);
	virtual ~TCPServer();
private:
	void bind_and_listen();
	void wait_client();
	void set_socket(SOCKET& socket);
	int cur_client_num();
private:
	int		m_port;
	SOCKET  m_socket;
	fd_set m_sock_set;
	fd_set m_read_set;
	char	m_read_buf[1024 * 100];
	std::map<SOCKET, std::shared_ptr<TCPClient>> m_client_map;
};













#endif // !TCP_SERVER_H