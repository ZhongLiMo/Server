#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#define FD_SETSIZE 1024

#include <WS2tcpip.h>

#include <map>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "timer.h"
#include "tcpclient.h"

#define TCP_THREAD_TIMER_MANAGER TCPServer::GetInstance()->GetTimerManager()

class TCPServer
{
public:
	static TCPServer* GetInstance();
	void StartServer(int port = 8000);
	TimerManager& GetTimerManager();
private:
	TCPServer();
	virtual ~TCPServer();
	TCPServer(const TCPServer&) = delete;
private:
	int cur_client_num();
	void bind_and_listen();
	SOCKET get_server_socket();
	void set_socket(SOCKET& socket, bool is_server = false);
private:
	int												m_port;
	SOCKET											m_socket;
	fd_set											m_client_set;
	TimerManager									m_timer_manager;
	std::map<SOCKET, TCPClient*>					m_client_map;
private:
	void CloseServer();
	void AcceptThread();
	void RecvmsgThread();
private:
	std::mutex										start_mtx;
	std::mutex										accept_mtx;
	std::atomic_bool								close_flag;
	std::condition_variable_any						start_cond;
	std::map<SOCKET, TCPClient*>					accecpt_client;
private:
	std::thread										accept_thread;
	std::thread										recvmsg_thread;
};

#endif // !TCP_SERVER_H