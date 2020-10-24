#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <WS2tcpip.h>

#include <string>

#include "tcppacket.h"

class TCPClient
{
public:
	TCPClient(const SOCKET& socket, const std::string& ip);
	virtual ~TCPClient();
public:
	const std::string& GetIP() const { return m_ip; };
	void SendMsg(std::shared_ptr<TCPPacket> ptcppacket);
	bool OnRecvData(std::shared_ptr<TCPPacket> ptcppacket);
private:
	void OnConnected();
	void OnDisconnected();
private:
	SOCKET  m_socket;
	std::string m_ip;
};

#endif // !TCP_CLIENT_H