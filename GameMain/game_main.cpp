#include "mylog.h"

#include <chrono>
#include <time.h>
#include <iostream>
#include <stdio.h> 
using namespace std;
using namespace std::chrono;

#include "tcpserver.h"
#include "timer.h"

MyLog tcplog("TCPServer", "../log");

class Test
{
public:
	Test():n(0){}
	void test(int a)
	{
		n += a;
		tcplog.SaveLog(LOG_DEBUG, "Test: %p n:%d" , this, n);
	}
	int n;
};


int main()
{
	//TCPServer::GetInstance()->StartServer();

	//MyLog molog("GameMain", "../log");
	//molog.SaveLog(LOG_DEBUG, "%s³Ô%s", "ning", "shi");

	std::thread testthread([] 
	{
		TimerManager manager;
		Test test;
		tcplog.SaveLog(LOG_DEBUG, "Test true add: %p", &test);
		Timer timer(manager, &Test::test, &test, 10);
		//Timer timer(manager, [] {
		//	cout << 10 << endl;
		//});
		timer.StartTimer(500, true);
		int n = 0;
		while (1)
		{
			n++;
			manager.OnTimer();
			if (n == 50000000)
			{
				timer.StopTimer();
			}
		}
	});
	cin.get();

	return 0;
}