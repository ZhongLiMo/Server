#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <string>
#include <WS2tcpip.h>

#include "tcppacket.h"

class TCPClient
{
public:
	TCPClient(const SOCKET& socket, const std::string& ip);
	virtual ~TCPClient();
public:
	const std::string& GetIP() const { return m_ip; };
	void SendMsg(std::shared_ptr<TCPPacket> ptcppacket);
	void OnRecvData(std::shared_ptr<TCPPacket> ptcppacket);
private:
	void OnConnected();
	void OnDisconnected();
private:
	SOCKET  m_socket;
	std::string m_ip;
};

#endif // !TCP_CLIENT_H