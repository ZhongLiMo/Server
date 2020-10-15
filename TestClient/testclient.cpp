#include <iostream>  
#include <WS2tcpip.h>  
#include <WinSock2.H>  
using namespace std;
#pragma comment(lib, "ws2_32.lib")  
#include "tcppacket.h"

#define SERVER_ADDRESS      "127.0.0.1"  
#define SERVER_PORT         8000  

#define SOCKET_NUM          120       /// �ͻ���socket�ĸ���,�޸ĸ�ֵ���Ըı����ӵ��������Ŀͻ��˸���  

SOCKET socketClient;

char g_szBuf[512] = "";

HANDLE hMutex;

void Input();

void Recv();

DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
);

DWORD WINAPI SendDataThreadProc(
	_In_ LPVOID lpParameter
);

int main()
{
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) return 1;

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		std::cout << "WSAStartup() error." << std::endl;
		return -1;
	}

	socketClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketClient == INVALID_SOCKET)
	{
		WSACleanup();
		std::cout << "socket() error." << std::endl;
		return -1;
	}


	SOCKADDR_IN server;
	memset(&server, 0, sizeof(SOCKADDR_IN));
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	inet_pton(server.sin_family, SERVER_ADDRESS, &server.sin_addr);

	err = connect(socketClient, (struct sockaddr *)&server, sizeof(SOCKADDR_IN));
	if (err == SOCKET_ERROR)
	{
		std::cout << "connect() error." << std::endl;
		closesocket(socketClient);
		WSACleanup();
		return -1;
	}

	hMutex = CreateMutex(NULL, false, NULL);
	if (NULL == hMutex)
	{
		printf("�����������ʧ�ܣ�\n");
		return -1;
	}

	DWORD dwThreadId = 0;

	/*HANDLE hThread = CreateThread(NULL, NULL, &SendDataThreadProc, NULL, 0, &dwThreadId);
	if (NULL == hThread)
	{
	printf("���������߳�ʧ�ܣ�\n");
	return -1;
	}*/


	HANDLE hThread2 = CreateThread(NULL, NULL, &ThreadProc, NULL, 0, &dwThreadId);
	if (NULL == hThread2)
	{
		printf("���������߳�ʧ�ܣ�\n");
		return -1;
	}




	Input();
	//    Recv();

	closesocket(socketClient);
	WSACleanup();

	return 0;
}

void Input()
{
	while (true)
	{
		WaitForSingleObject(hMutex, INFINITE);
		char szInput[256] = "";
		printf("Me: cmd mg\n");
		unsigned int cmd = 0;
		scanf("%d %[^\n]", &cmd, szInput);
		if (strcmp(szInput, "exit") == 0)
		{
			break;
		}
		else
		{
			std::shared_ptr<TCPPacket> ptcppacket(new TCPPacket);
			ptcppacket->body = szInput;
			ptcppacket->pack(cmd);
			send(socketClient, ptcppacket->data.c_str(), ptcppacket->data.length(), 0);
		}
		//        printf("\n");
		ReleaseMutex(hMutex);

	}
}

void Recv()
{
	while (true)
	{

		printf("Recv Form Server alllen[%d] cmd[%d] bodylen[%d] msg[%s]\n\n", recv(socketClient, g_szBuf, sizeof(g_szBuf), 0), 
			reinterpret_cast<Pheader*>(g_szBuf)->cmd, reinterpret_cast<Pheader*>(g_szBuf)->length, g_szBuf + sizeof(Pheader));
		printf("Me Recv Form Server:%s\n", g_szBuf);


	}
}

DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
{

	while (true)
	{
		WaitForSingleObject(hMutex, INFINITE);
		printf("Recv Form Server alllen[%d] cmd[%d] bodylen[%d] msg[%s]\n\n", recv(socketClient, g_szBuf, sizeof(g_szBuf), 0),
			reinterpret_cast<Pheader*>(g_szBuf)->cmd, reinterpret_cast<Pheader*>(g_szBuf)->length, g_szBuf + sizeof(Pheader));
		ReleaseMutex(hMutex);
	}

	return 0;
}

DWORD WINAPI SendDataThreadProc(_In_ LPVOID lpParameter)
{

	while (true)
	{
		WaitForSingleObject(hMutex, INFINITE);
		char szInput[256] = "";
		printf("Me: cmd mg\n");
		unsigned int cmd = 0;
		scanf("%d %[^\n]", &cmd, szInput);
		if (strcmp(szInput, "exit") == 0)
		{
			break;
		}
		else
		{
			std::shared_ptr<TCPPacket> ptcppacket(new TCPPacket);
			ptcppacket->body = szInput;
			ptcppacket->pack(cmd);
			send(socketClient, ptcppacket->data.c_str(), ptcppacket->data.length(), 0);

		}
		printf("\n");
		ReleaseMutex(hMutex);
	}


	return 0;
}