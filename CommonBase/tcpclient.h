#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <WS2tcpip.h>
#include <string>

#include "tcppacket.h"


class TCPClient
{
public:
	TCPClient(SOCKET socket);
	virtual ~TCPClient();
	void OnRecvData(std::shared_ptr<TCPPacket> ptcppacket);
	void OnConnected();
	void OnDisconnected();
	void SendMsg(std::shared_ptr<TCPPacket> ptcppacket);
private:
	SOCKET  m_socket;
	

};
















#endif // !TCP_CLIENT_H