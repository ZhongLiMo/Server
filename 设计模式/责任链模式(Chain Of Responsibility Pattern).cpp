#include <iostream>
#include <string>
using namespace std;

// 请求
class Request
{
public:
	int m_nNumber;
};

// 管理者
class Manager
{
public:
	Manager(string temp) { name = temp; }
	void SetSuccessor(Manager* temp) { manager = temp; }
	virtual void GetRequest(Request* request) = 0;
protected:
	Manager* manager;
	string name;
};

// 经理
class CommonManager : public Manager
{
public:
	CommonManager(string strTemp) : Manager(strTemp) {}
	virtual void GetRequest(Request* request);
};

void CommonManager::GetRequest(Request* request)
{
	if (request->m_nNumber >= 0 && request->m_nNumber < 1000)
	{
		cout << name << " 处理了请求: " << request->m_nNumber << endl;
	}
	else
	{
		manager->GetRequest(request);
	}
}

// 总监
class Majordomo : public Manager
{
public:
	Majordomo(string strTemp) : Manager(strTemp) {}
	virtual void GetRequest(Request* request);
};

void Majordomo::GetRequest(Request* request)
{
	if (request->m_nNumber <= 5000)
	{
		cout << name << " 处理了请求: " << request->m_nNumber << endl;
	}
	else
	{
		manager->GetRequest(request);
	}
}

//总经理  
class GeneralManager : public Manager
{
public:
	GeneralManager(string name) :Manager(name) {}
	virtual void GetRequest(Request* request)  //总经理可以处理所有请求  
	{
		cout << name << " 处理了请求: " << request->m_nNumber << endl;
	}
};

int main() {

	Manager* common = new CommonManager("张经理");
	Manager* major = new Majordomo("李总监");
	GeneralManager* general = new GeneralManager("赵总");
	common->SetSuccessor(major);
	major->SetSuccessor(general);
	Request* rq = new Request();

	rq->m_nNumber = 999;
	common->GetRequest(rq);

	rq->m_nNumber = 4999;
	common->GetRequest(rq);

	rq->m_nNumber = 6999;
	common->GetRequest(rq);

	delete rq;
	delete major;
	delete common;
	delete general;
	return 0;
}