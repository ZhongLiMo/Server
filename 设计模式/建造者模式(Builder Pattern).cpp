#include <string>
#include <iostream>
using namespace std;

//车
class Car
{
public:
	void setEngine(string engine) { m_Engine = engine; }
	void setWheel(string wheel) { m_Wheel = wheel; }
	void setBody(string body) { m_Body = body; }
	void disPlay() { cout << m_Engine << " " << m_Wheel << " " << m_Body << endl; }
private:
	string  m_Engine;
	string  m_Wheel;
	string  m_Body;
};
//建造者模式抽象基类
class IBuilder
{
public:
	IBuilder() : car(new Car()) { }
	virtual ~IBuilder() { delete car; car = NULL; }
	Car* getCar() { return car; };
public:
	virtual void assembleEngine() = 0;
	virtual void assembleWheel() = 0;
	virtual void assembleBody() = 0;
protected:
	Car* car;
};
//A型车组装流程
class ACarBuilder : public IBuilder
{
public:
	ACarBuilder() : IBuilder() { }
	virtual void assembleEngine() { car->setEngine("engineA"); };
	virtual void assembleWheel() { car->setWheel("whellA"); }
	virtual void assembleBody() { car->setBody("bodyA"); };
};
//B型车组装流程
class BCarBuilder : public IBuilder
{
public:
	BCarBuilder() : IBuilder() { }
	virtual void assembleEngine() { car->setEngine("engineB"); };
	virtual void assembleWheel() { car->setWheel("wheelB"); }
	virtual void assembleBody() { car->setBody("bodyB"); };
};
class Assemble
{
public:
	void Creat(IBuilder* builder)
	{
		builder->assembleEngine();
		builder->assembleBody();
		builder->assembleWheel();
	}
};
int main()
{
	Assemble m_assemble;
	IBuilder* acarbuilder = new ACarBuilder();
	m_assemble.Creat(acarbuilder);
	acarbuilder->getCar()->disPlay();
	delete acarbuilder;
	IBuilder* bcarbuilder = new BCarBuilder();
	m_assemble.Creat(bcarbuilder);
	bcarbuilder->getCar()->disPlay();
	delete bcarbuilder;
	return 0;
}