#include "tcpserver.h"

#include "mylog.h"

extern MyLog tcplog;

TCPServer::TCPServer(int port) : m_port(port)
{
	bind_and_listen();
	wait_client();
}
TCPServer::~TCPServer()
{
	for (u_int i = 0; i < m_sock_set.fd_count; ++i)
	{
		closesocket(m_sock_set.fd_array[i]);
	}
	WSACleanup();
}
void TCPServer::bind_and_listen()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result) tcplog.SaveLog(LOG_FATAL, "WSAStartup() error.");
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		WSACleanup();
		tcplog.SaveLog(LOG_FATAL, "socket() failed error[%d].", WSAGetLastError());
	}
	set_socket(m_socket);
	SOCKADDR_IN sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_port);
	sin.sin_addr.s_addr = INADDR_ANY;
	if (bind(m_socket, (LPSOCKADDR)&sin, sizeof(sin)))
	{
		WSACleanup();
		closesocket(m_socket);
		tcplog.SaveLog(LOG_FATAL, "bind() failed error[%d].", WSAGetLastError());
	}
	if (listen(m_socket, SOMAXCONN))
	{
		WSACleanup();
		closesocket(m_socket);
		tcplog.SaveLog(LOG_FATAL, "listen() failed error[%d].", WSAGetLastError());
	}
	tcplog.SaveLog(LOG_INFO, "Server start OK! listen port:%d.", m_port);
}
void TCPServer::wait_client()
{
	FD_ZERO(&m_sock_set);
	FD_SET(m_socket, &m_sock_set);
	while (true)
	{
		m_read_set = m_sock_set;
		int result = select(10000, &m_read_set, NULL, NULL, NULL);
		if (result == SOCKET_ERROR)
		{
			tcplog.SaveLog(LOG_ERROR, "select() error.");
			continue;
		}
		else if (result > 0)
		{
			if (FD_ISSET(m_socket, &m_read_set))
			{
				sockaddr_in clientAddr;
				memset(&clientAddr, 0, sizeof(clientAddr));
				int len = sizeof(clientAddr);
				SOCKET clientSocket = accept(m_socket, (sockaddr*)&clientAddr, &len);
				if (clientSocket == INVALID_SOCKET)
				{
					tcplog.SaveLog(LOG_ERROR, "accept() error.");
				}
				else
				{
					set_socket(clientSocket);
					FD_SET(clientSocket, &m_sock_set);
					char ipAddress[16] = { 0 };
					inet_ntop(AF_INET, &clientAddr, ipAddress, 16);
					tcplog.SaveLog(LOG_INFO, "new client login IP(%s:%d) total client num:%d.", ipAddress, ntohs(clientAddr.sin_port), cur_client_num());
					m_client_map.insert(std::make_pair(clientSocket, new TCPClient(clientSocket)));
					m_client_map[clientSocket]->OnConnected();
					if (1 == result) continue;
				}
			}
			for (u_int i = 0; i < m_sock_set.fd_count;)
			{
				if (m_sock_set.fd_array[i] == m_socket ||
					m_sock_set.fd_array[i] == INVALID_SOCKET ||
					!FD_ISSET(m_sock_set.fd_array[i], &m_read_set))
				{
					++i;
					continue;
				}
				SOCKET socket = m_sock_set.fd_array[i];
				memset(m_read_buf, 0, sizeof(m_read_buf));
				result = recv(socket, m_read_buf, sizeof(m_read_buf), 0);

				sockaddr_in clientAddr;
				memset(&clientAddr, 0, sizeof(clientAddr));
				int len = sizeof(clientAddr);
				getpeername(socket, (struct sockaddr *)&clientAddr, &len);
				char ipAddress[16] = { 0 };
				inet_ntop(AF_INET, &clientAddr, ipAddress, 16);

				if (result == SOCKET_ERROR)
				{
					DWORD err = WSAGetLastError();
					if (err == WSAECONNRESET) tcplog.SaveLog(LOG_INFO, "client IP(%s:%d) shutdown.", ipAddress, ntohs(clientAddr.sin_port));
					else tcplog.SaveLog(LOG_INFO, "recv() error.", ipAddress, ntohs(clientAddr.sin_port));
					closesocket(socket);
					FD_CLR(socket, &m_sock_set);
					if (m_client_map.find(socket) == m_client_map.end())
					{
						tcplog.SaveLog(LOG_ERROR, "socket not find in map");
					}
					else
					{
						m_client_map.find(socket)->second->OnDisconnected();
						m_client_map.erase(socket);
					}
				}
				else if (result == 0)
				{
					closesocket(socket);
					FD_CLR(socket, &m_sock_set);
					tcplog.SaveLog(LOG_INFO, "client IP(%s:%d) logout.", ipAddress, ntohs(clientAddr.sin_port));
					if (m_client_map.find(socket) == m_client_map.end())
					{
						tcplog.SaveLog(LOG_ERROR, "socket not find in map");
					}
					else
					{
						m_client_map.find(socket)->second->OnDisconnected();
						m_client_map.erase(socket);
					}
				}
				else
				{
					if (m_client_map.find(socket) == m_client_map.end())
					{
						tcplog.SaveLog(LOG_ERROR, "socket not find in map");
						closesocket(socket);
						FD_CLR(socket, &m_sock_set);
					}
					else
					{
						m_client_map.find(socket)->second->OnRecvData(m_read_buf);
						++i;
					}
				}
			}
		}
	}
}
void TCPServer::set_socket(SOCKET& socket)
{
	int optval = 1;
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)))
	{
		WSACleanup();
		closesocket(socket);
		tcplog.SaveLog(LOG_FATAL, "setsockopt(SO_REUSEADDR) failed error[%d].", WSAGetLastError());
	}
	optval = 1;
	if (setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (char *)&optval, sizeof(optval)))
	{
		WSACleanup();
		closesocket(socket);
		tcplog.SaveLog(LOG_FATAL, "setsockopt(SO_KEEPALIVE) failed error[%d].", WSAGetLastError());
	}
	optval = 32 * 1024;
	if (setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&optval, sizeof(optval)))
	{
		WSACleanup();
		closesocket(socket);
		tcplog.SaveLog(LOG_FATAL, "setsockopt(SO_SNDBUF) failed error[%d].", WSAGetLastError());
	}
	unsigned long a = 1;
	if (ioctlsocket(socket, FIONBIO, &a))
	{
		WSACleanup();
		closesocket(socket);
		tcplog.SaveLog(LOG_FATAL, "ioctlsocket() failed error[%d].", WSAGetLastError());
	}
}
int TCPServer::cur_client_num()
{
	return m_sock_set.fd_count - 1;
}