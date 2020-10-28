#include "tcpserver.h"
#include "mylog.h"
#include "tcppacket.h"

extern MyLog tcplog;

TCPServer::TCPServer() : close_flag(false), start_flag(false),
	accept_thread(&TCPServer::AcceptThread, this), 
	recvmsg_thread(&TCPServer::RecvmsgThread, this)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) tcplog.SaveLog(LOG_FATAL, "WSAStartup() error.");
	m_socket = get_server_socket();
}
TCPServer::~TCPServer()
{
	CloseServer();
}
void TCPServer::StartServer(int port)
{
	static std::once_flag once;
	std::call_once(once, [&]
	{
		m_port = port;
		bind_and_listen();
		start_flag = true;
		start_cond.notify_all();
	});
}
int TCPServer::cur_client_num()
{
	return m_client_set.fd_count;
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
SOCKET TCPServer::get_server_socket()
{
	SOCKET socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (socket == INVALID_SOCKET)
	{
		WSACleanup();
		tcplog.SaveLog(LOG_FATAL, "socket() failed error[%d].", WSAGetLastError());
	}
	set_socket(socket, true);
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
void TCPServer::CloseServer()
{
	if (!close_flag)
	{
		close_flag = true;
		start_flag = true;
		start_cond.notify_all();
		accept_thread.join();
		recvmsg_thread.join();
		for (u_int i = 0; i < m_client_set.fd_count; ++i)
		{
			closesocket(m_client_set.fd_array[i]);
		}
		FD_ZERO(&m_client_set);
		closesocket(m_socket);
		WSACleanup();
		while (!m_client_map.empty())
		{
			delete m_client_map.begin()->second;
			m_client_map.erase(m_client_map.begin());
		}
		while (!accecpt_client.empty())
		{
			delete accecpt_client.begin()->second;
			accecpt_client.erase(accecpt_client.begin());
		}
	}
}
void TCPServer::AcceptThread()
{
	start_mtx.lock();
	start_cond.wait(start_mtx, [this] {return !!start_flag; });
	start_mtx.unlock();
	fd_set accept_set;
	struct timeval timeout = { 1, 0 };
	while (!close_flag)
	{
		FD_ZERO(&accept_set);
		FD_SET(m_socket, &accept_set);
		int result = select(FD_SETSIZE, &accept_set, NULL, NULL, &timeout);
		if (result == SOCKET_ERROR)
		{
			tcplog.SaveLog(LOG_ERROR, "select() error.");
		}
		else if (result > 0)
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
				TCPClient* pp(new TCPClient(clientSocket, inet_ntoa(clientAddr.sin_addr)));
				tcplog.SaveLog(LOG_INFO, "new client login IP(%s).", pp->GetIP().c_str());
				accept_mtx.lock();
				accecpt_client.insert(std::make_pair(clientSocket, pp));
				accept_mtx.unlock();
			}
		}
	}
}
void TCPServer::RecvmsgThread()
{
	start_mtx.lock();
	start_cond.wait(start_mtx, [this] {return !!start_flag; });
	start_mtx.unlock();
	fd_set read_set;
	int cur_read_len = 0;
	char pheader[TCP_HEAD_LEN];
	char read_buf[1024 * 100];
	struct timeval timeout = {0, 0};
	while (!close_flag)
	{
		m_timer_manager.OnTimer();
		accept_mtx.lock();
		if (!accecpt_client.empty())
		{
			for (std::map<SOCKET, TCPClient*>::iterator ite = accecpt_client.begin(); ite != accecpt_client.end(); ++ite)
			{
				FD_SET(ite->first, &m_client_set);
			}
			m_client_map.insert(accecpt_client.begin(), accecpt_client.end());
			accecpt_client.clear();
		}
		accept_mtx.unlock();
		if (!m_client_set.fd_count) continue;
		read_set = m_client_set;
		int result = select(FD_SETSIZE, &read_set, NULL, NULL, &timeout);
		if (result == SOCKET_ERROR)
		{
			tcplog.SaveLog(LOG_ERROR, "select() error.");
			continue;
		}
		else if (result > 0)
		{
			tcplog.SaveLog(LOG_INFO, "RecvmsgThread cur client num[%d]", cur_client_num());
			for (u_int i = 0; i < m_client_set.fd_count;)
			{
				if (!FD_ISSET(m_client_set.fd_array[i], &read_set) || m_client_set.fd_array[i] == INVALID_SOCKET)
				{
					++i;
					continue;
				}
				SOCKET socket = m_client_set.fd_array[i];
				if (m_client_map.find(socket) == m_client_map.end())
				{
					closesocket(socket);
					FD_CLR(socket, &m_client_set);
					tcplog.SaveLog(LOG_ERROR, "socket not find in map");
					++i;
					continue;
				}

				cur_read_len = 0;
				memset(pheader, 0, TCP_HEAD_LEN);
				memset(read_buf, 0, sizeof(read_buf));

				while (true)
				{
					if (cur_read_len < TCP_HEAD_LEN) result = recv(socket, pheader, TCP_HEAD_LEN - cur_read_len, 0);
					else if (cur_read_len == TCP_HEAD_LEN && reinterpret_cast<TCPHeader*>(pheader)->length == 0) goto ON_RECV_MSG;
					else result = recv(socket, read_buf, sizeof(read_buf) - cur_read_len - TCP_HEAD_LEN, 0);

					if (result == SOCKET_ERROR)
					{
						if (errno == EAGAIN  || errno == EINTR || errno == EINPROGRESS || errno == EWOULDBLOCK)
						{
							++i;
							tcplog.SaveLog(LOG_WARN, "recv() error.");
							continue;
						}
						DWORD err = WSAGetLastError();
						if (err == WSAECONNRESET) tcplog.SaveLog(LOG_INFO, "client IP(%s) shutdown.", m_client_map.find(socket)->second->GetIP().c_str());
						else tcplog.SaveLog(LOG_INFO, "recv() error.");
						closesocket(socket);
						FD_CLR(socket, &m_client_set);
						delete m_client_map[socket];
						m_client_map.erase(socket);
						break;
					}
					else if (result == 0)
					{
						closesocket(socket);
						FD_CLR(socket, &m_client_set);
						delete m_client_map[socket];
						m_client_map.erase(socket);
						break;
					}
					else
					{
						cur_read_len += result;
						if (cur_read_len == (TCP_HEAD_LEN + reinterpret_cast<TCPHeader*>(pheader)->length))
						{
						ON_RECV_MSG:
							std::shared_ptr<TCPPacket> ptcppacket = TCPPacket::CreateNew();
							if (ptcppacket->save_packet(reinterpret_cast<TCPHeader*>(pheader), read_buf)
								&& m_client_map.find(socket)->second->OnRecvData(ptcppacket))
							{
								++i;
							}
							else
							{
								closesocket(socket);
								FD_CLR(socket, &m_client_set);
								delete m_client_map[socket];
								m_client_map.erase(socket);
								tcplog.SaveLog(LOG_ERROR, "client send length not right.");
							}
							break;
						}
					}
				}
			}
		}
	}
}