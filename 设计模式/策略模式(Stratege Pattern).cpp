#include <iostream>
#include <string>

using namespace std;

class Company
{
public:
	string name;
	virtual void Management() = 0;
};
class Company1 :public Company
{
public:
	Company1(string n)
	{
		name = n;
	}
	void printcompany()
	{
		cout << "������" << name << endl;
	}
	virtual void Management()
	{
		cout << "�й��ֹ�˾:��Ӫģʽ1�ľ���ʵ��..." << endl;
	}
};
class Company2 :public Company
{
public:
	Company2(string n)
	{
		name = n;
	}
	void printcompany()
	{
		cout << "������" << name << endl;
	}
	virtual void Management()
	{
		cout << "�����ֹ�˾:��Ӫģʽ2�ľ���ʵ��..." << endl;
	}
};
class Context
{
public:
	Company *com;
	Context(Company *c)
	{
		com = c;
	}
	void printcompany()
	{
		com->Management();
	}
};
int main()
{
	Company *china = new Company1("����");
	Company *america = new Company2("����");
	Context a1(china);
	Context a2(america);
	a1.printcompany();
	cout << "=================================================" << endl;
	a2.printcompany();
	cout << "=================================================" << endl;
}