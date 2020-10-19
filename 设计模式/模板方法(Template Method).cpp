//#include <iostream>
//using namespace std;
//
//
//class Method
//{
//public:
//	void Domethod()
//	{
//		Step1();
//		if (Step2())
//		{
//			Step3();
//		}
//		else
//		{
//			Step4();
//		}
//	}
//protected:
//	virtual bool Step2() { return false; }
//private:
//	void Step1() {}
//	void Step3() {}
//	void Step4() {}
//};
//
//class MyMethod : public Method
//{
//protected:
//	virtual bool Step2() { return true; }
//};
//
//int main()
//{
//	Method* method = new MyMethod;
//	method->Domethod();
//	delete method;
//	return 0;
//}