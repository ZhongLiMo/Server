//#include <iostream>
//#include <mutex>
//using namespace std;
//
//
//class Singleton 
//{
//private:
//	Singleton() {};
//	Singleton(const Singleton& other) = delete;
//public:
//	static Singleton* getInstance();
//	static Singleton* m_instance;
//};
//
//Singleton* Singleton::m_instance = NULL;
//Singleton* Singleton::getInstance()
//{
//	std::once_flag myflag;
//	std::call_once(myflag, []() { Singleton::m_instance = new Singleton; });
//	return m_instance;
//}
//
//int main()
//{
//	Singleton* instance = Singleton::getInstance();
//	return 0;
//}