#include <iostream>
#include <string>

using namespace std;

/*****原系统A：Target*****/
class MangeSystemA
{
public:
	virtual void getName()
	{
		cout << "获取系统A中员工的名称" << endl;
	}
};

/*****要集成的系统B：Adaptee*****/
class MangeSystemB
{
public:
	virtual void getPeopleName()
	{
		cout << "获取系统B中员工的名称" << endl;
	}
};


/***适配器：Adapter***/
class AdapterSystem : public MangeSystemA
{
public:
	AdapterSystem(MangeSystemB *pSysB) :m_pSysB(pSysB) {}
	virtual void getName()
	{
		m_pSysB->getPeopleName();
	}
private:
	MangeSystemB *m_pSysB;
};

/****系统A工作场景****/
int main()
{

	MangeSystemB *sysB = new MangeSystemB;
	//创建适配器
	MangeSystemA *sysA = new AdapterSystem(sysB);
	sysA->getName();

	return 0;
}