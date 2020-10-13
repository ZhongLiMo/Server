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
	WORD version = MAKEWORD(2, 2);
	int result = WSAStartup(version, &wsaData);
	if (result) tcplog.SaveLog(LOG_FATAL, "WSAStartup() error.");
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		WSACleanup();
		tcplog.SaveLog(LOG_FATAL, "socket() error.");
	}
	sockaddr_in svrAddress;
	svrAddress.sin_family = AF_INET;
	svrAddress.sin_addr.s_addr = INADDR_ANY;
	svrAddress.sin_port = htons(m_port);
	result = bind(m_socket, (sockaddr*)&svrAddress, sizeof(svrAddress));
	if (result == SOCKET_ERROR)
	{
		closesocket(m_socket);
		WSACleanup();
		tcplog.SaveLog(LOG_FATAL, "bind() error.");
	}
	result = listen(m_socket, 5);
	if (result == SOCKET_ERROR)
	{
		closesocket(m_socket);
		WSACleanup();
		tcplog.SaveLog(LOG_FATAL, "listen() error.");
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
		int result = select(100000, &m_read_set, NULL, NULL, NULL);
		if (result == SOCKET_ERROR)
		{
			tcplog.SaveLog(LOG_ERROR, "select() error.");
			continue;
		}
		if (FD_ISSET(m_socket, &m_read_set))
		{
			sockaddr_in clientAddr;
			int len = sizeof(clientAddr);
			SOCKET clientSocket = accept(m_socket, (sockaddr*)&clientAddr, &len);
			if (clientSocket == INVALID_SOCKET)
			{
				tcplog.SaveLog(LOG_ERROR, "accept() error.");
			}
			else
			{
				FD_SET(clientSocket, &m_sock_set);
				char ipAddress[16] = { 0 };
				inet_ntop(AF_INET, &clientAddr, ipAddress, 16);
				tcplog.SaveLog(LOG_INFO, "new client login IP(%s:%d) total num:%d.", ipAddress, ntohs(clientAddr.sin_port), m_sock_set.fd_count - 1);
				continue;
			}
		}
		for (u_int i = 0; i < m_sock_set.fd_count;)
		{
			SOCKET socket = m_sock_set.fd_array[i];
			sockaddr_in clientAddr;
			int len = sizeof(clientAddr);
			getpeername(socket, (struct sockaddr *)&clientAddr, &len);
			char ipAddress[16] = { 0 };
			inet_ntop(AF_INET, &clientAddr, ipAddress, 16);
			if (FD_ISSET(socket, &m_read_set))
			{
				char bufRecv[100];
				result = recv(socket, bufRecv, 100, 0);
				if (result == SOCKET_ERROR)
				{
					DWORD err = WSAGetLastError();
					if (err == WSAECONNRESET) tcplog.SaveLog(LOG_INFO, "client IP(%s:%d) shutdown.", ipAddress, ntohs(clientAddr.sin_port));
					else tcplog.SaveLog(LOG_INFO, "recv() error.", ipAddress, ntohs(clientAddr.sin_port));
					closesocket(socket);
					FD_CLR(socket, &m_sock_set);
				}
				else if (result == 0)
				{
					closesocket(socket);
					FD_CLR(socket, &m_sock_set);
					tcplog.SaveLog(LOG_INFO, "client IP(%s:%d) logout.", ipAddress, ntohs(clientAddr.sin_port));
				}
				else
				{
					bufRecv[result] = '\0';
					tcplog.SaveLog(LOG_INFO, "recv client IP(%s:%d) msg(%s).", ipAddress, ntohs(clientAddr.sin_port), bufRecv);
					char szSend[256] = "朕已经收到，你可以滚了！";
					send(socket, szSend, strlen(szSend), 0);
					++i;
				}
			}
		}
	}
}