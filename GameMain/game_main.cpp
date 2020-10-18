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
	TCPServer::GetInstance()->StartServer();

	MyLog molog("GameMain", "../log");
	molog.SaveLog(LOG_DEBUG, "%s³Ô%s", "ning", "shi");
	
	cin.get();

	return 0;
}