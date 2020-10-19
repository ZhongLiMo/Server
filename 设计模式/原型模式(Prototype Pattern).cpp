//#include <iostream>
//using namespace std;
//
//class IPrototype
//{
//public:
//	virtual void ShowClass() = 0;
//	virtual IPrototype* Copy() = 0;
//};
//class ClassA: public IPrototype
//{
//public:
//	virtual void ShowClass() { cout << "ClassA" << endl; }
//	virtual IPrototype* Copy() { return new ClassA; }
//};
//class ClassB : public IPrototype
//{
//public:
//	virtual void ShowClass() { cout << "ClassB" << endl; }
//	virtual IPrototype* Copy() { return new ClassB; }
//};
//class ClassC : public IPrototype
//{
//public:
//	virtual void ShowClass() { cout << "ClassC" << endl; }
//	virtual IPrototype* Copy() { return new ClassC; }
//};
//
//int main()
//{
//	IPrototype* calculate = NULL;
//	IPrototype* copy = NULL;
//	calculate = new ClassA;
//	copy = calculate->Copy();
//	calculate->ShowClass();
//	copy->ShowClass();
//	delete calculate;
//	delete copy;
//	calculate = new ClassB;
//	copy = calculate->Copy();
//	calculate->ShowClass();
//	copy->ShowClass();
//	delete calculate;
//	delete copy;
//	calculate = new ClassC;
//	copy = calculate->Copy();
//	calculate->ShowClass();
//	copy->ShowClass();
//	delete calculate;
//	delete copy;
//	calculate = NULL;
//	copy = NULL;
//	return 0;
//}