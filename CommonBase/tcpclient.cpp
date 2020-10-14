#include "tcpclient.h"

#include "mylog.h"
#include "tcpserver.h"


extern MyLog tcplog;
extern TCPServer tcpServer;


TCPClient::TCPClient(SOCKET socket) : m_socket(socket)
{
}
TCPClient::~TCPClient()
{
}
void TCPClient::OnRecvData(const std::string& data)
{
	sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	int len = sizeof(clientAddr);
	getpeername(m_socket, (struct sockaddr *)&clientAddr, &len);
	char ipAddress[16] = { 0 };
	inet_ntop(AF_INET, &clientAddr, ipAddress, 16);
	tcplog.SaveLog(LOG_INFO, "recv client IP(%s:%d) msg(%s).", ipAddress, ntohs(clientAddr.sin_port), data.c_str());
	SendMsg("ning jie biao chi shi");
}
void TCPClient::OnConnected()
{

}
void TCPClient::OnDisconnected()
{
	
}
void TCPClient::SendMsg(const std::string& body)
{
	send(m_socket, body.c_str(), body.length(), 0);
}