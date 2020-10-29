#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#define FD_SETSIZE 1024

#include <map>
#include <list>
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
	void StartServer(int port = 8000);
	static TCPServer* GetInstance()
	{
		static TCPServer tcpserver;
		return &tcpserver;
	}
	TimerManager& GetTimerManager()
	{
		return m_timer_manager;
	}
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
	void RecvMsgThread();
	void SendMsgThread();
	void RemoveClient(SOCKET socket);
public:
	void SendMsgToClient(SOCKET socket, std::shared_ptr<TCPPacket> ptcppacket);
private:
	using SendList = std::list<std::pair<SOCKET, std::shared_ptr<TCPPacket>>>;
	SendList										send_list;
	std::mutex										send_mtx;
	std::condition_variable_any						send_cond;
	std::mutex										client_mtx;
	std::mutex										start_mtx;
	std::mutex										accept_mtx;
	std::atomic_bool								close_flag;
	std::atomic_bool								start_flag;
	std::condition_variable_any						start_cond;
	std::map<SOCKET, TCPClient*>					accecpt_client;
private:
	std::thread										accept_thread;
	std::thread										recvmsg_thread;
	std::thread										sendmsg_thread;
};

#endif // !TCP_SERVER_H