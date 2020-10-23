#include <iostream>
using namespace std;


class ICalculate 
{
public:
	virtual void Calculate(int a, int b) = 0;
};
class CalculateAdd : public ICalculate 
{
public:
	virtual void Calculate(int a, int b) { cout << (a + b) << endl; }
};
class CalculateSubtract : public ICalculate 
{
public:
	virtual void Calculate(int a, int b) { cout << (a - b) << endl; }
};
class CalculateMultiply : public ICalculate 
{
public:
	virtual void Calculate(int a, int b) { cout << (a * b) << endl; }
};
class CalculateDivide : public ICalculate 
{
public:
	virtual void Calculate(int a, int b) 
	{
		if (b)
			cout << (a / b) << endl;
		else
			cout << 0 << endl;
	}
};

class ICalculateFactory
{
public:
	virtual ICalculate* CreateCalculate() = 0;
};
class CalculateAddFactory : public ICalculateFactory
{
public:
	virtual ICalculate* CreateCalculate() { return new CalculateAdd(); }
};
class CalculateSubtractFactory : public ICalculateFactory
{
public:
	virtual ICalculate* CreateCalculate() { return new CalculateSubtract(); }
};
class CalculateMultiplyFactory : public ICalculateFactory
{
public:
	virtual ICalculate* CreateCalculate() { return new CalculateMultiply(); }
};
class CalculateDivideFactory : public ICalculateFactory
{
public:
	virtual ICalculate* CreateCalculate() { return new CalculateDivide(); }
};

int main()
{
	ICalculateFactory* factory = new CalculateAddFactory;
	ICalculate* calculate = factory->CreateCalculate();
	calculate->Calculate(10, 10);
	delete factory;
	delete calculate;
	factory = new CalculateSubtractFactory;
	calculate = factory->CreateCalculate();
	calculate->Calculate(10, 10);
	delete factory;
	delete calculate;
	factory = new CalculateMultiplyFactory;
	calculate = factory->CreateCalculate();
	calculate->Calculate(10, 10);
	delete factory;
	delete calculate;
	factory = new CalculateDivideFactory;
	calculate = factory->CreateCalculate();
	calculate->Calculate(10, 10);
	delete factory;
	delete calculate;
	return 0;
}