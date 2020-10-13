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
	//	/// ��ʼ��socket  
	//	WSADATA wsaData;
	//	WORD version = MAKEWORD(2, 2);
	//	int result = 0;
	//	result = WSAStartup(version, &wsaData);
	//	if (0 != result)
	//	{
	//		std::cout << "WSAStartup() error." << std::endl;
	//		return -1;
	//	}

	//	/// ��������socket   
	//	SOCKET socketListen;
	//	socketListen = socket(AF_INET, SOCK_STREAM, 0);
	//	if (socketListen == INVALID_SOCKET)
	//	{
	//		WSACleanup();
	//		std::cout << "socket() error." << std::endl;
	//		return -1;
	//	}

	//	/// ��������ַ�ṹ   
	//	sockaddr_in svrAddress;
	//	svrAddress.sin_family = AF_INET;
	//	svrAddress.sin_addr.s_addr = INADDR_ANY;
	//	svrAddress.sin_port = htons(8000);

	//	/// �󶨷������׽���   
	//	result = bind(socketListen, (sockaddr*)&svrAddress, sizeof(svrAddress));
	//	if (result == SOCKET_ERROR)
	//	{
	//		closesocket(socketListen);
	//		WSACleanup();
	//		std::cout << "bind() error." << std::endl;
	//		return -1;
	//	}

	//	/// ��������  
	//	result = listen(socketListen, 5);
	//	if (result == SOCKET_ERROR)
	//	{
	//		closesocket(socketListen);
	//		WSACleanup();
	//		std::cout << "listen() error." << std::endl;
	//		return -1;
	//	}
	//	std::cout << "�����������ɹ��������˿ڣ�" << ntohs(svrAddress.sin_port) << std::endl;

	//	/// selectģ��   
	//	fd_set m_sock_set;
	//	FD_ZERO(&m_sock_set);

	//	FD_SET(socketListen, &m_sock_set); // ��socketListen�����׽��ּ�����   

	//	while (true)
	//	{
	//		fd_set readSet;
	//		FD_ZERO(&readSet);                    //��ʼ���ɶ�����
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
	//			FD_SET(clientSocket, &m_sock_set);   /// ���´������׽��ּ��뵽������   

	//			char ipAddress[16] = { 0 };
	//			inet_ntop(AF_INET, &clientAddr, ipAddress, 16);
	//			std::cout << "���µ�����[" << ipAddress << ":" << ntohs(clientAddr.sin_port)
	//				<< "], Ŀǰ�ͻ��˵�����Ϊ��" << m_sock_set.fd_count - 1 << std::endl;       //ȥ��һ�������׽���

	//			continue;
	//		}

	//		for (u_int i = 0; i < m_sock_set.fd_count; ++i)
	//		{
	//			SOCKET socket = m_sock_set.fd_array[i];

	//			sockaddr_in clientAddr;
	//			int len = sizeof(clientAddr);
	//			getpeername(socket, (struct sockaddr *)&clientAddr, &len);      //������׽ӿ�������Զ��Э���ַ
	//			char ipAddress[16] = { 0 };
	//			inet_ntop(AF_INET, &clientAddr, ipAddress, 16);

	//			/// �ɶ��Լ��ӣ��ɶ���ָ�����ӵ����������ݵ����������ѹرա����û���ֹ  
	//			if (FD_ISSET(socket, &readSet))
	//			{
	//				char bufRecv[100];
	//				result = recv(socket, bufRecv, 100, 0);
	//				if (result == SOCKET_ERROR)
	//				{
	//					DWORD err = WSAGetLastError();
	//					if (err == WSAECONNRESET)       /// �ͻ��˵�socketû�б������ر�,��û�е���closesocket  
	//					{
	//						std::cout << "�ͻ���[" << ipAddress << ":" << ntohs(clientAddr.sin_port) << "]��ǿ�йر�, ";
	//					}
	//					else
	//					{
	//						std::cout << "recv() error��" << std::endl;
	//					}

	//					closesocket(socket);
	//					FD_CLR(socket, &m_sock_set);

	//					std::cout << "Ŀǰ�ͻ��˵�����Ϊ��" << m_sock_set.fd_count - 1 << std::endl;
	//					break;
	//				}
	//				else if (result == 0)               /// �ͻ��˵�socket����closesocket�����ر�  
	//				{
	//					closesocket(socket);
	//					FD_CLR(socket, &m_sock_set);

	//					std::cout << "�ͻ���[" << ipAddress << ":" << ntohs(clientAddr.sin_port)
	//						<< "]�Ѿ��˳���Ŀǰ�ͻ��˵�����Ϊ��" << m_sock_set.fd_count - 1 << std::endl;
	//					break;
	//				}

	//				bufRecv[result] = '\0';
	//				std::cout << "���Կͻ���[" << ipAddress << ":" << ntohs(clientAddr.sin_port)
	//					<< "]����Ϣ��" << bufRecv << std::endl;

	//				char szSend[256] = "���Ѿ��յ�������Թ��ˣ�";
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
	molog.SaveLog(LOG_DEBUG, "%s��%s", "ning", "shi");
	//while (1)
	//{
	//	molog.SaveLog(LOG_ERROR, "%s��%s", "ning", "shi");
	//	molog.SaveLog(LOG_WARN, "%s��%s", "ning", "shi");
	//	molog.SaveLog(LOG_INFO, "%s��%s", "ning", "shi");
	//	molog.SaveLog(LOG_PROTO, "%s��%s", "ning", "shi");
	//	molog.SaveLog(LOG_DEBUG, "%s��%s", "ning", "shi");
	//}

	uint64_t timestamp(duration_cast<milliseconds>(chrono::system_clock::now().time_since_epoch()).count()); // ��ȡʱ��������룩

	uint64_t milli = timestamp + 8 * 60 * 60 * 1000; // תΪ����������ʱ��
	auto mTime = milliseconds(milli);
	auto tp = time_point<system_clock, milliseconds>(mTime);
	auto tt = system_clock::to_time_t(tp);
	tm now;
	gmtime_s(&now, &tt);
	printf("%4d��%02d��%02d�� %02d:%02d:%02d.%03d\n", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, int(timestamp % 1000));

	cin.get();

	return 0;
}