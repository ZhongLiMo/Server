//#include <iostream>
//using namespace std;
//
//
//class ICalculate
//{
//public:
//	virtual void Calculate(int a, int b) = 0;
//};
//class CalculateAdd: public ICalculate
//{
//public:
//	virtual void Calculate(int a, int b) { cout << (a + b) << endl; }
//};
//class CalculateSubtract : public ICalculate
//{
//public:
//	virtual void Calculate(int a, int b) { cout << (a - b) << endl; }
//};
//class CalculateMultiply : public ICalculate
//{
//public:
//	virtual void Calculate(int a, int b) { cout << (a * b) << endl; }
//};
//class CalculateDivide : public ICalculate
//{
//public:
//	virtual void Calculate(int a, int b)
//	{
//		if (b)
//			cout << (a / b) << endl;
//		else
//			cout << 0 << endl;
//	}
//};
//
//int main()
//{
//	ICalculate* calculate = new CalculateAdd;
//	calculate->Calculate(10, 10);
//	delete calculate;
//	calculate = new CalculateSubtract;
//	calculate->Calculate(10, 10);
//	delete calculate;
//	calculate = new CalculateMultiply;
//	calculate->Calculate(10, 10);
//	delete calculate;
//	calculate = new CalculateDivide;
//	calculate->Calculate(10, 10);
//	delete calculate;
//	calculate = NULL;
//	return 0;
//}