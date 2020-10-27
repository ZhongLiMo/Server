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
		cout << "负责人" << name << endl;
	}
	virtual void Management()
	{
		cout << "中国分公司:经营模式1的具体实现..." << endl;
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
		cout << "负责人" << name << endl;
	}
	virtual void Management()
	{
		cout << "美国分公司:经营模式2的具体实现..." << endl;
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
	Company *china = new Company1("张三");
	Company *america = new Company2("张三");
	Context a1(china);
	Context a2(america);
	a1.printcompany();
	cout << "=================================================" << endl;
	a2.printcompany();
	cout << "=================================================" << endl;
}