#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#define FD_SETSIZE 1024

#include <map>
#include <memory>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "tcpclient.h"


class TCPServer
{
public:
	static TCPServer* GetInstance();
	void StartServer(int port = 8000);
private:
	TCPServer();
	virtual ~TCPServer();
	TCPServer(const TCPServer&) = delete;
private:
	void bind_and_listen();
	int cur_client_num();
	SOCKET get_socket(bool is_server = false);
	void set_socket(SOCKET& socket, bool is_server = false);
private:
	int		m_port;
	SOCKET  m_socket;
	fd_set m_client_set;
	std::map<SOCKET, std::shared_ptr<TCPClient>> m_client_map;
private:
	void CloseServer();
	void AcceptThread();
	void RecvmsgThread();
private:
	std::mutex start_mtx;
	std::mutex accept_mtx;
	std::thread accept_thread;
	std::thread recvmsg_thread;
	std::atomic<bool> close_flag;
	std::condition_variable_any start_cond;
	std::map<SOCKET, std::shared_ptr<TCPClient>> accecpt_client;
};

#endif // !TCP_SERVER_H