#include "mylog.h"

#include <chrono>
#include <time.h>
#include <iostream>

#include <stdio.h> 
using namespace std;
using namespace std::chrono;

#include "tcpserver.h"

MyLog tcplog("TCPServer", "../log");

int main()
{
	TCPServer server;


	//{
	//	/// 初始化socket  
	//	WSADATA wsaData;
	//	WORD version = MAKEWORD(2, 2);
	//	int result = 0;
	//	result = WSAStartup(version, &wsaData);
	//	if (0 != result)
	//	{
	//		std::cout << "WSAStartup() error." << std::endl;
	//		return -1;
	//	}

	//	/// 创建侦听socket   
	//	SOCKET socketListen;
	//	socketListen = socket(AF_INET, SOCK_STREAM, 0);
	//	if (socketListen == INVALID_SOCKET)
	//	{
	//		WSACleanup();
	//		std::cout << "socket() error." << std::endl;
	//		return -1;
	//	}

	//	/// 服务器地址结构   
	//	sockaddr_in svrAddress;
	//	svrAddress.sin_family = AF_INET;
	//	svrAddress.sin_addr.s_addr = INADDR_ANY;
	//	svrAddress.sin_port = htons(8000);

	//	/// 绑定服务器套接字   
	//	result = bind(socketListen, (sockaddr*)&svrAddress, sizeof(svrAddress));
	//	if (result == SOCKET_ERROR)
	//	{
	//		closesocket(socketListen);
	//		WSACleanup();
	//		std::cout << "bind() error." << std::endl;
	//		return -1;
	//	}

	//	/// 开启监听  
	//	result = listen(socketListen, 5);
	//	if (result == SOCKET_ERROR)
	//	{
	//		closesocket(socketListen);
	//		WSACleanup();
	//		std::cout << "listen() error." << std::endl;
	//		return -1;
	//	}
	//	std::cout << "服务器启动成功，监听端口：" << ntohs(svrAddress.sin_port) << std::endl;

	//	/// select模型   
	//	fd_set m_sock_set;
	//	FD_ZERO(&m_sock_set);

	//	FD_SET(socketListen, &m_sock_set); // 将socketListen加入套接字集合中   

	//	while (true)
	//	{
	//		fd_set readSet;
	//		FD_ZERO(&readSet);                    //初始化可读队列
	//		readSet = m_sock_set;

	//		result = select(0, &readSet, NULL, NULL, NULL);
	//		if (result == SOCKET_ERROR)
	//		{
	//			std::cout << "select() error." << std::endl;
	//			break;
	//		}

	//		if (FD_ISSET(socketListen, &readSet))
	//		{
	//			sockaddr_in clientAddr;
	//			int len = sizeof(clientAddr);

	//			SOCKET clientSocket = accept(socketListen, (sockaddr*)&clientAddr, &len);
	//			if (clientSocket == INVALID_SOCKET)
	//			{
	//				std::cout << "accept() error." << std::endl;
	//				break;
	//			}
	//			FD_SET(clientSocket, &m_sock_set);   /// 将新创建的套接字加入到集合中   

	//			char ipAddress[16] = { 0 };
	//			inet_ntop(AF_INET, &clientAddr, ipAddress, 16);
	//			std::cout << "有新的连接[" << ipAddress << ":" << ntohs(clientAddr.sin_port)
	//				<< "], 目前客户端的数量为：" << m_sock_set.fd_count - 1 << std::endl;       //去掉一个侦听套接字

	//			continue;
	//		}

	//		for (u_int i = 0; i < m_sock_set.fd_count; ++i)
	//		{
	//			SOCKET socket = m_sock_set.fd_array[i];

	//			sockaddr_in clientAddr;
	//			int len = sizeof(clientAddr);
	//			getpeername(socket, (struct sockaddr *)&clientAddr, &len);      //获得与套接口相连的远程协议地址
	//			char ipAddress[16] = { 0 };
	//			inet_ntop(AF_INET, &clientAddr, ipAddress, 16);

	//			/// 可读性监视，可读性指有连接到来、有数据到来、连接已关闭、重置或终止  
	//			if (FD_ISSET(socket, &readSet))
	//			{
	//				char bufRecv[100];
	//				result = recv(socket, bufRecv, 100, 0);
	//				if (result == SOCKET_ERROR)
	//				{
	//					DWORD err = WSAGetLastError();
	//					if (err == WSAECONNRESET)       /// 客户端的socket没有被正常关闭,即没有调用closesocket  
	//					{
	//						std::cout << "客户端[" << ipAddress << ":" << ntohs(clientAddr.sin_port) << "]被强行关闭, ";
	//					}
	//					else
	//					{
	//						std::cout << "recv() error，" << std::endl;
	//					}

	//					closesocket(socket);
	//					FD_CLR(socket, &m_sock_set);

	//					std::cout << "目前客户端的数量为：" << m_sock_set.fd_count - 1 << std::endl;
	//					break;
	//				}
	//				else if (result == 0)               /// 客户端的socket调用closesocket正常关闭  
	//				{
	//					closesocket(socket);
	//					FD_CLR(socket, &m_sock_set);

	//					std::cout << "客户端[" << ipAddress << ":" << ntohs(clientAddr.sin_port)
	//						<< "]已经退出，目前客户端的数量为：" << m_sock_set.fd_count - 1 << std::endl;
	//					break;
	//				}

	//				bufRecv[result] = '\0';
	//				std::cout << "来自客户端[" << ipAddress << ":" << ntohs(clientAddr.sin_port)
	//					<< "]的消息：" << bufRecv << std::endl;

	//				char szSend[256] = "朕已经收到，你可以滚了！";
	//				send(socket, szSend, strlen(szSend), 0);
	//			}
	//		}
	//	}

	//	for (u_int i = 0; i < m_sock_set.fd_count; ++i)
	//	{
	//		SOCKET socket = m_sock_set.fd_array[i];
	//		closesocket(socket);
	//	}

	//	WSACleanup();
	//	return 0;
	//}


	MyLog molog("GameMain", "../log");
	molog.SaveLog(LOG_DEBUG, "%s吃%s", "ning", "shi");
	//while (1)
	//{
	//	molog.SaveLog(LOG_ERROR, "%s吃%s", "ning", "shi");
	//	molog.SaveLog(LOG_WARN, "%s吃%s", "ning", "shi");
	//	molog.SaveLog(LOG_INFO, "%s吃%s", "ning", "shi");
	//	molog.SaveLog(LOG_PROTO, "%s吃%s", "ning", "shi");
	//	molog.SaveLog(LOG_DEBUG, "%s吃%s", "ning", "shi");
	//}

	uint64_t timestamp(duration_cast<milliseconds>(chrono::system_clock::now().time_since_epoch()).count()); // 获取时间戳（毫秒）

	uint64_t milli = timestamp + 8 * 60 * 60 * 1000; // 转为东八区北京时间
	auto mTime = milliseconds(milli);
	auto tp = time_point<system_clock, milliseconds>(mTime);
	auto tt = system_clock::to_time_t(tp);
	tm now;
	gmtime_s(&now, &tt);
	printf("%4d年%02d月%02d日 %02d:%02d:%02d.%03d\n", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, int(timestamp % 1000));

	cin.get();

	return 0;
}