#include <iostream>
using namespace std;

class IBook
{
public:
	virtual void ShowBook() = 0;
};
class ChineseBook : public IBook
{
public:
	virtual void ShowBook() { cout << "��ʦ����������" << endl; };
};
class MathBook : public IBook
{
public:
	virtual void ShowBook() { cout << "��ʦ������ѧ��" << endl; };
};
class EnglishBook : public IBook
{
public:
	virtual void ShowBook() { cout << "��ʦ����Ӣ����" << endl; };
};

class ITeacher
{
public:
	virtual void Teach() = 0;
};
class ChineseTeacher : public ITeacher
{
public:
	virtual void Teach() { cout << "������ʦ���Ͽ�" << endl; };
};
class MathTeacher : public ITeacher
{
public:
	virtual void Teach() { cout << "��ѧ��ʦ���Ͽ�" << endl; };
};
class EnglishTeacher : public ITeacher
{
public:
	virtual void Teach() { cout << "Ӣ����ʦ���Ͽ�" << endl; };
};

class AbsFactory
{
public:
	virtual IBook* CreateBook() = 0;
	virtual ITeacher* CreateTeacher() = 0;
};
class ChineseAbsFactory : public AbsFactory
{
public:
	virtual IBook* CreateBook() { return new ChineseBook; }
	virtual ITeacher* CreateTeacher() { return new ChineseTeacher; }
};
class MathAbsFactory : public AbsFactory
{
public:
	virtual IBook* CreateBook() { return new MathBook; }
	virtual ITeacher* CreateTeacher() { return new MathTeacher; }
};
class EnglishAbsFactory : public AbsFactory
{
public:
	virtual IBook* CreateBook() { return new EnglishBook; }
	virtual ITeacher* CreateTeacher() { return new EnglishTeacher; }
};

void OnProcess(AbsFactory* factory)
{
	IBook* book = factory->CreateBook();
	ITeacher* teacher = factory->CreateTeacher();
	book->ShowBook();
	teacher->Teach();
	delete book;
	delete teacher;
}

int main()
{
	AbsFactory* factory = new ChineseAbsFactory;
	OnProcess(factory);
	delete factory;
	factory = new MathAbsFactory;
	OnProcess(factory);
	delete factory;
	factory = new EnglishAbsFactory;
	OnProcess(factory);
	delete factory;
	return 0;
}