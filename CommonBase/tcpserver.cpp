#include "tcpserver.h"

#include "mylog.h"
#include "tcppacket.h"

extern MyLog tcplog;

TCPServer::TCPServer(int port) : m_port(port)
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result) tcplog.SaveLog(LOG_FATAL, "WSAStartup() error.");
	m_socket = get_socket(true);
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
SOCKET TCPServer::get_socket(bool is_server)
{
	SOCKET socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (socket == INVALID_SOCKET)
	{
		if (is_server)
		{
			WSACleanup();
			tcplog.SaveLog(LOG_FATAL, "socket() failed error[%d].", WSAGetLastError());
		}
		else
		{
			tcplog.SaveLog(LOG_ERROR, "socket() failed error[%d].", WSAGetLastError());
			return INVALID_SOCKET;
		}
	}
	set_socket(socket, is_server);
	return socket;
}
void TCPServer::set_socket(SOCKET& socket, bool is_server)
{
	if (INVALID_SOCKET == socket) return;
	int optval = 1;
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)))
	{
		closesocket(socket);
		if (is_server)
		{
			WSACleanup();
			tcplog.SaveLog(LOG_FATAL, "setsockopt(SO_REUSEADDR) failed error[%d].", WSAGetLastError());
		}
		else
		{
			tcplog.SaveLog(LOG_ERROR, "setsockopt(SO_REUSEADDR) failed error[%d].", WSAGetLastError());
			socket = INVALID_SOCKET;
			return;
		}
	}
	optval = 1;
	if (setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (char *)&optval, sizeof(optval)))
	{
		closesocket(socket);
		if (is_server)
		{
			WSACleanup();
			tcplog.SaveLog(LOG_FATAL, "setsockopt(SO_KEEPALIVE) failed error[%d].", WSAGetLastError());
		}
		else
		{
			tcplog.SaveLog(LOG_ERROR, "setsockopt(SO_KEEPALIVE) failed error[%d].", WSAGetLastError());
			socket = INVALID_SOCKET;
			return;
		}
	}
	optval = 32 * 1024;
	if (setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&optval, sizeof(optval)))
	{
		closesocket(socket);
		if (is_server)
		{
			WSACleanup();
			tcplog.SaveLog(LOG_FATAL, "setsockopt(SO_SNDBUF) failed error[%d].", WSAGetLastError());
		}
		else
		{
			tcplog.SaveLog(LOG_ERROR, "setsockopt(SO_SNDBUF) failed error[%d].", WSAGetLastError());
			socket = INVALID_SOCKET;
			return;
		}
	}
	unsigned long a = 1;
	if (ioctlsocket(socket, FIONBIO, &a))
	{
		closesocket(socket);
		if (is_server)
		{
			WSACleanup();
			tcplog.SaveLog(LOG_FATAL, "ioctlsocket() failed error[%d].", WSAGetLastError());
		}
		else
		{
			tcplog.SaveLog(LOG_ERROR, "ioctlsocket() failed error[%d].", WSAGetLastError());
			socket = INVALID_SOCKET;
			return;
		}
	}
}
void TCPServer::wait_client()
{
	FD_ZERO(&m_sock_set);
	FD_SET(m_socket, &m_sock_set);
	int cur_read_len = 0;
	char pheader[TCP_HEAD_LEN];
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
				set_socket(clientSocket);
				if (clientSocket == INVALID_SOCKET)
				{
					tcplog.SaveLog(LOG_ERROR, "new clientSocket error.");
				}
				else
				{
					FD_SET(clientSocket, &m_sock_set);
					tcplog.SaveLog(LOG_INFO, "total client num:%d.", cur_client_num());
					m_client_map.insert(std::make_pair(clientSocket, new TCPClient(clientSocket, inet_ntoa(clientAddr.sin_addr))));
				}
				if (1 == result) continue;
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
				cur_read_len = 0;
				memset(pheader, 0, sizeof(pheader));
				memset(m_read_buf, 0, sizeof(m_read_buf));
				SOCKET socket = m_sock_set.fd_array[i];
				while (true)
				{
					if (cur_read_len < sizeof(pheader)) result = recv(socket, pheader, sizeof(pheader) - cur_read_len, 0);
					else if (cur_read_len == sizeof(pheader) && reinterpret_cast<TCPHeader*>(pheader)->length == 0) goto ON_RECV_MSG;
					else result = recv(socket, m_read_buf, sizeof(m_read_buf) - cur_read_len - sizeof(pheader), 0);
					cur_read_len = cur_read_len + result;
					if (result == SOCKET_ERROR)
					{
						if (errno == EAGAIN || errno == EINPROGRESS || errno == EINTR || errno == EWOULDBLOCK)
						{
							++i;
							tcplog.SaveLog(LOG_WARN, "recv() error.");
							continue;
						}
						DWORD err = WSAGetLastError();
						if (err == WSAECONNRESET) tcplog.SaveLog(LOG_INFO, "client shutdown total client num:%d.", cur_client_num() - 1);
						else tcplog.SaveLog(LOG_INFO, "recv() error.");
						closesocket(socket);
						FD_CLR(socket, &m_sock_set);
						if (m_client_map.find(socket) == m_client_map.end())
						{
							tcplog.SaveLog(LOG_ERROR, "socket not find in map");
						}
						else
						{
							m_client_map.erase(socket);
						}
						break;
					}
					else if (result == 0)
					{
						closesocket(socket);
						FD_CLR(socket, &m_sock_set);
						if (m_client_map.find(socket) == m_client_map.end())
						{
							tcplog.SaveLog(LOG_ERROR, "socket not find in map");
						}
						else
						{
							m_client_map.erase(socket);
						}
						break;
					}
					else
					{
						if (cur_read_len == (sizeof(pheader) + reinterpret_cast<TCPHeader*>(pheader)->length))
						{
						ON_RECV_MSG:
							std::shared_ptr<TCPPacket> ptcppacket = TCPPacket::CreateNew();
							if (ptcppacket->save_packet(reinterpret_cast<TCPHeader*>(pheader), m_read_buf))
							{
								if (m_client_map.find(socket) == m_client_map.end())
								{
									tcplog.SaveLog(LOG_ERROR, "socket not find in map");
									closesocket(socket);
									FD_CLR(socket, &m_sock_set);
								}
								else
								{
									m_client_map.find(socket)->second->OnRecvData(ptcppacket);
									++i;
								}
							}
							else
							{
								closesocket(socket);
								FD_CLR(socket, &m_sock_set);
								tcplog.SaveLog(LOG_ERROR, "client send length not right.");
								if (m_client_map.find(socket) == m_client_map.end())
								{
									tcplog.SaveLog(LOG_ERROR, "socket not find in map");
								}
								else
								{
									m_client_map.erase(socket);
								}
							}
							break;
						}
					}
				}
			}
		}
	}
}
int TCPServer::cur_client_num()
{
	return m_sock_set.fd_count - 1;
}