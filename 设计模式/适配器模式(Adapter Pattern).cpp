#include <iostream>
#include <string>

using namespace std;

/*****ԭϵͳA��Target*****/
class MangeSystemA
{
public:
	virtual void getName()
	{
		cout << "��ȡϵͳA��Ա��������" << endl;
	}
};

/*****Ҫ���ɵ�ϵͳB��Adaptee*****/
class MangeSystemB
{
public:
	virtual void getPeopleName()
	{
		cout << "��ȡϵͳB��Ա��������" << endl;
	}
};


/***��������Adapter***/
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

/****ϵͳA��������****/
int main()
{

	MangeSystemB *sysB = new MangeSystemB;
	//����������
	MangeSystemA *sysA = new AdapterSystem(sysB);
	sysA->getName();

	return 0;
}