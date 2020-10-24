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
#include "cost_time.hpp"

MyLog tcplog("TCPServer", "../log");
MyLog mysqllog("MYSQL", "../log");

const char tableName[] = "test_user";

enum TEST_USER
{
	TEST_USER_ID,
	TEST_USER_NAME,
	TEST_USER_NICKNAME,
	TEST_USER_SEX,
	TEST_USER_MAX,
};

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
	//MysqlDB::GetInstance()->Connect();

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

	const char user[] = "root";
	const char pswd[] = "123456";
	const char host[] = "localhost";
	const char database[] = "test";
	{
		{
			CostTime costtime(tcplog, __LINE__, "DBHandle->Connect");
			DBHandle->Connect(host, user, pswd, database);
		}
		typedef DBRecord<TEST_USER, TEST_USER_MAX, tableName> UserRecord;
		DBTble<UserRecord> userTable;

		//select
		std::cout << "after select" << std::endl;
		{
			CostTime costtime(tcplog, __LINE__, "DBHandle->Select");
			DBHandle->Select(userTable, tableName, "id>1", "id", "DESC");
		}
		auto ite = userTable.begin();
		for (; ite != userTable.end(); ++ite)
		{
			for (unsigned int i = 0; i < (ite->second)->Size(); ++i)
			{
				std::cout << (*ite->second)[i] << "\t";
			}
			std::cout << std::endl;
		}

		////update
		//if (userTable.begin() != userTable.end())
		//{
		//	auto userRecord = userTable.begin()->second;
		//	userRecord->SetString(TEST_USER_NICKNAME, "Jack");
		//	userRecord->Update();
		//}
		//std::cout << std::endl << "after update" << std::endl; ite = userTable.begin();
		//for (; ite != userTable.end(); ++ite)
		//{
		//	for (unsigned int i = 0; i < (ite->second)->Size(); ++i)
		//	{
		//		std::cout << (*ite->second)[i] << "\t";
		//	}
		//	std::cout << std::endl;
		//}

		////delete
		//Key key = 0;
		//if (userTable.begin() != userTable.end())
		//{
		//	auto userRecord = userTable.begin()->second;
		//	key = userRecord->GetKey();
		//	userTable.DeleteRecord(key);
		//}
		//std::cout << std::endl << "after delete" << std::endl; ite = userTable.begin();
		//for (; ite != userTable.end(); ++ite)
		//{
		//	for (unsigned int i = 0; i < (ite->second)->Size(); ++i)
		//	{
		//		std::cout << (*ite->second)[i] << "\t";
		//	}
		//	std::cout << std::endl;
		//}

		////insert
		//if (key)
		//{
		//	auto userRecord = UserRecord::CreateNew(key);
		//	userRecord->SetString(TEST_USER_NAME, "Libai");
		//	userRecord->SetString(TEST_USER_NICKNAME, "XiaoBai");
		//	userRecord->SetInt(TEST_USER_SEX, 1);
		//	userTable.InsertRecord(userRecord);
		//}
		//std::cout << std::endl << "after insert" << std::endl; ite = userTable.begin();
		//for (; ite != userTable.end(); ++ite)
		//{
		//	for (unsigned int i = 0; i < (ite->second)->Size(); ++i)
		//	{
		//		std::cout << (*ite->second)[i] << "\t";
		//	}
		//	std::cout << std::endl;
		//}
	}

	

	cin.get();
	return 0;
}