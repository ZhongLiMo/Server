#include <iostream>
using namespace std;

class Subsystem1
{
public:
	Subsystem1() {};
	~Subsystem1() {};
	void Operation() { cout << "Subsystem1::Operation" << endl; };
};
class Subsystem2
{
public:
	Subsystem2() {};
	~Subsystem2() {};
	void Operation() { cout << "Subsystem2::Operation" << endl; };
};
class Facade
{
public:
	Facade() {};
	~Facade() {};
	void OperationWrapper() { _subsys1.Operation();  _subsys2.Operation(); };
private:
	Subsystem1 _subsys1;
	Subsystem2 _subsys2;
};
int main()
{
	Facade pFacade;
	pFacade.OperationWrapper();
	return 0;
}