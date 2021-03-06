#include "mylog.h"

#include <chrono>
#include <time.h>
#include <iostream>
#include <stdio.h> 
using namespace std;
using namespace std::chrono;

#include "tcpserver.h"
#include "timer.h"
#include "mysqldb.hpp"
#include "costtime.hpp"

const char user[] = "root";
const char pswd[] = "123456";
const char host[] = "localhost";
const char database[] = "test";
const char tableName[] = "test_user";

int main()
{
	CostTime costtime(COST_TIME_CONSTRUCTOR);
	{
		CostTime costtime(__LINE__, "TCPServer->StartServer");
		TCPServer::GetInstance()->StartServer();
	}
	{
		CostTime costtime(__LINE__, "DBHandle->Connect");
		DBHandle->Connect(host, user, pswd, database);
	}

	TimerManager manager;
	Timer timer(manager, [] {
		cout << 666 << endl;
	});
	Timer timer2(manager, [] {
		cout << 777 << endl;
	});

	timer.StartTimer(100, true);
	timer2.StartTimer(200, true);
	while (1)
	{
		manager.OnTimer();
	}

	
	//std::this_thread::sleep_for(std::chrono::seconds(3));
	cin.get();
	return 0;
}


//class Test
//{
//public:
//	Test() :n(0) {}
//	void test(int a)
//	{
//		n += a;
//		tcplog.SaveLog(LOG_DEBUG, "对象调用地址: %p n:%d", this, n);
//	}
//	int n;
//};

//std::thread testthread([]
//{
//	TimerManager manager;
//	Test test;
//	tcplog.SaveLog(LOG_DEBUG, "对象真实地址: %p", &test);
//	Timer timer(manager, &Test::test, &test, 1);

//	timer.StartTimer(100, true);
//	int n = 0;
//	while (1)
//	{
//		n++;
//		manager.OnTimer();
//		if (n == 2000000)
//		{
//			timer.StopTimer();
//			return;
//		}
//	}
//});
//testthread.detach();

/*
	enum TEST_USER
{
	TEST_USER_ID,
	TEST_USER_NAME,
	TEST_USER_NICKNAME,
	TEST_USER_SEX,
	TEST_USER_MAX,
};

	{
		typedef DBRecord<TEST_USER, TEST_USER_MAX, tableName> Record;
		typedef DBTble<Record> Table;
		Table userTable;
		{
			CostTime costtime(tcplog, __LINE__, "DBHandle->Select");
			DBHandle->Select(userTable, tableName);
		}

		//select
		std::cout << "after select" << std::endl;

		auto ite = userTable.begin();
		for (; ite != userTable.end(); ++ite)
		{
			for (unsigned int i = 0; i < (ite->second)->Size(); ++i)
			{
				std::cout << (*ite->second)[i] << "\t";
			}
			std::cout << std::endl;
		}

		//update
		if (userTable.begin() != userTable.end())
		{
			auto userRecord = userTable.begin()->second;
			userRecord->SetString(TEST_USER_NICKNAME, "Jack");
			userRecord->Update();
		}
		std::cout << std::endl << "after update" << std::endl; ite = userTable.begin();
		for (; ite != userTable.end(); ++ite)
		{
			for (unsigned int i = 0; i < (ite->second)->Size(); ++i)
			{
				std::cout << (*ite->second)[i] << "\t";
			}
			std::cout << std::endl;
		}

		//delete
		Key key = 0;
		if (userTable.begin() != userTable.end())
		{
			auto userRecord = userTable.begin()->second;
			key = userRecord->GetKey();
			userTable.DeleteRecord(key);
		}
		std::cout << std::endl << "after delete" << std::endl; ite = userTable.begin();
		for (; ite != userTable.end(); ++ite)
		{
			for (unsigned int i = 0; i < (ite->second)->Size(); ++i)
			{
				std::cout << (*ite->second)[i] << "\t";
			}
			std::cout << std::endl;
		}

		//insert
		auto userRecord = Record::CreateNew(userTable.GetNewKey());
		userRecord->SetString(TEST_USER_NAME, "Libai");
		userRecord->SetString(TEST_USER_NICKNAME, "XiaoBai");
		userRecord->SetInt(TEST_USER_SEX, 1);
		userTable.InsertRecord(userRecord);

		std::cout << std::endl << "after insert" << std::endl; ite = userTable.begin();
		for (; ite != userTable.end(); ++ite)
		{
			for (unsigned int i = 0; i < (ite->second)->Size(); ++i)
			{
				std::cout << (*ite->second)[i] << "\t";
			}
			std::cout << std::endl;
		}
	}

*/