#include "tcpclient.h"

#include "mylog.h"


extern MyLog tcplog;


TCPClient::TCPClient(SOCKET socket) : m_socket(socket)
{
}
TCPClient::~TCPClient()
{
}
void TCPClient::OnRecvData(std::shared_ptr<TCPPacket> ptcppacket)
{
	sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	int len = sizeof(clientAddr);
	getpeername(m_socket, (struct sockaddr *)&clientAddr, &len);
	char ipAddress[16] = { 0 };
	inet_ntop(AF_INET, &clientAddr, ipAddress, 16);
	tcplog.SaveLog(LOG_INFO, "recv client IP(%s:%d) cmd(%d) datd(%s).", ipAddress, ntohs(clientAddr.sin_port), ptcppacket->safe_check(), ptcppacket->body.c_str());
	std::shared_ptr<TCPPacket> psend(new TCPPacket);
	psend->body = "ning jie biao chi shi";
	psend->pack();
	SendMsg(psend);
}
void TCPClient::OnConnected()
{

}
void TCPClient::OnDisconnected()
{
	
}
void TCPClient::SendMsg(std::shared_ptr<TCPPacket> ptcppacket)
{
	
	send(m_socket, ptcppacket->data.c_str(), ptcppacket->data.length(), 0);
}