#include <iostream>
#include <string>
#include <vector>

using namespace std;

class STMemento
{
private:
	int iVitality;
public:
	STMemento() {}
	STMemento(int iVitality)
	{
		this->iVitality = iVitality;
	}

	int GetVitality() const
	{
		return this->iVitality;
	}
};

class STOriginator
{
private:
	int iVitality;
	string name;
public:
	STOriginator(string strName, int iVit) : iVitality(iVit), name(strName)
	{

	}

	STMemento* SaveState()
	{
		return new STMemento(iVitality);
	}

	void RecoverState(const STMemento* stMemento)
	{
		this->iVitality = stMemento->GetVitality();
	}

	void SetVitality(int iVit)
	{
		this->iVitality = iVit;
	}

	void Show()
	{
		cout << "Name: " << name << endl;
		cout << "Live: " << iVitality << endl;
	}
};

class STCareTaker
{
private:
	vector<STMemento*> vecStMemento;

public:
	void AddMemento(STMemento* stMemento)
	{
		vecStMemento.push_back(stMemento);
	}

	STMemento* GetMemento(int Iindex)
	{
		if (Iindex >= vecStMemento.size())
			return NULL;
		else
			return vecStMemento[Iindex];
	}
};

int main(int argc, char* argv[])
{
	STOriginator* pstOriginator = new STOriginator("xxx", 100);
	cout << "Ô­Ê¼×´Ì¬£º " << endl;
	pstOriginator->Show();

	STCareTaker* pstCareTaker = new STCareTaker();
	pstCareTaker->AddMemento(pstOriginator->SaveState());

	pstOriginator->SetVitality(50);
	cout << "Õ½¶·ºó×´Ì¬£º " << endl;
	pstOriginator->Show();

	pstOriginator->RecoverState(pstCareTaker->GetMemento(0));
	cout << "¹éµµºó×´Ì¬£º " << endl;
	pstOriginator->Show();

	return 0;
}