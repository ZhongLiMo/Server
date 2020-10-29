#include "tcpclient.h"
#include "mylog.h"
#include "tcpserver.h"

extern MyLog tcplog;

TCPClient::TCPClient(const SOCKET& socket, const std::string& ip) : m_socket(socket), m_ip(ip)
{
	OnConnected();
}
TCPClient::~TCPClient()
{
	OnDisconnected();
}
bool TCPClient::OnRecvMsg(std::shared_ptr<TCPPacket> ptcppacket)
{
	tcplog.SaveLog(LOG_INFO, "client IP(%s) recv cmd(%d) datd(%s).", m_ip.c_str(), ptcppacket->safe_check(), ptcppacket->body.c_str());
	std::shared_ptr<TCPPacket> psend = TCPPacket::CreateNew();
	psend->pack_packet("ning jie biao chi shi", ptcppacket->safe_check());
	SendMsg(psend);
	return true;
}
void TCPClient::OnConnected()
{
	tcplog.SaveLog(LOG_INFO, "client login IP(%s).", m_ip.c_str());
}
void TCPClient::OnDisconnected()
{
	tcplog.SaveLog(LOG_INFO, "client logout IP(%s).", m_ip.c_str());
}
void TCPClient::SendMsg(std::shared_ptr<TCPPacket> ptcppacket)
{
	TCPServer::GetInstance()->SendMsgToClient(m_socket, ptcppacket);
}