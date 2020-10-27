// CommandModel.h�ļ�
#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ����ʦ��
class BarbecueMaster
{
public:
	void MakeMutton()
	{
		std::cout << "�������" << std::endl;
	}
	void MakeChiken()
	{
		std::cout << "�������" << std::endl;
	}
};
// ������
class Command
{
protected:
	BarbecueMaster * m_master;
public:
	Command(BarbecueMaster * p)
	{
		m_master = p;
	}
	virtual void executeCmd() = 0;
};

class CommandMutton : public Command
{
public:
	CommandMutton(BarbecueMaster * p) : Command(p) {}
	void executeCmd()
	{
		m_master->MakeMutton();
	}
};

class CommandChiken : public Command
{
public:
	CommandChiken(BarbecueMaster * p) : Command(p) {}
	void executeCmd()
	{
		m_master->MakeChiken();
	}
};
// ����Ա
class Waiter
{
private:
	std::vector<Command *> m_vec;
public:
	~Waiter()
	{
		for (auto it = m_vec.begin(); it != m_vec.end(); it++)
		{
			delete(*it);
		}
		m_vec.clear();
	}
	void add(Command * p)
	{
		m_vec.push_back(p);
	}
	void remove(Command * p)
	{
		auto it = find(m_vec.begin(), m_vec.end(), p);
		if (it != m_vec.end())
			m_vec.erase(it);
	}
	void submitCmd()
	{
		for (auto it = m_vec.cbegin(); it != m_vec.cend(); it++)
		{
			(*it)->executeCmd();
		}
	}
};


int main()
{
	using namespace std;
	// ����ģʽ
	Waiter * pWaiter = new Waiter();
	BarbecueMaster * pMaster = new BarbecueMaster();
	pWaiter->add(new CommandChiken(pMaster));
	pWaiter->add(new CommandMutton(pMaster));
	pWaiter->submitCmd();
	delete pMaster;
	delete pWaiter;
	return 0;
}
