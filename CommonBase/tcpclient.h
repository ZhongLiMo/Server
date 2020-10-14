#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <WS2tcpip.h>
#include <string>


class TCPClient
{
public:
	TCPClient(SOCKET socket);
	virtual ~TCPClient();
	void OnRecvData(const std::string& data);
	void OnConnected();
	void OnDisconnected();
	void SendMsg(const std::string& body);
private:
	SOCKET  m_socket;
	

};
















#endif // !TCP_CLIENT_H