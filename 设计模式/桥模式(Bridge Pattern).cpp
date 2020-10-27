#include <iostream>


using namespace std;


// ============================================
// 枪械基类
class Gun_base {
public:
	virtual ~Gun_base() {}

	virtual void fire() const = 0;
};


// 手枪
class Pistol : public Gun_base {
public:
	void fire() const {
		cout << "手枪发射—— biu biu biu！" << endl;
	}
};


// 步枪
class Rifle : public Gun_base {
public:
	void fire() const {
		cout << "步枪发射—— tu tu tu！" << endl;
	}
};


// 狙击枪
class Awp :public Gun_base {
public:
	void fire() const {
		cout << "狙击枪发射—— peng！" << endl;
	}
};
// ============================================


// ============================================
// 战士基类
class Fighter_base {
public:
	virtual ~Fighter_base() {}

	virtual void fight(Gun_base* gun) const = 0;
};


// 战士A
class Fighter_A : public Fighter_base {
public:
	void fight(Gun_base* gun) const {
		cout << "我是战士A！ ———— ";
		gun->fire();
	}
};


// 战士B
class Fighter_B : public Fighter_base {
public:
	void fight(Gun_base* gun) const {
		cout << "我是战士B！ ———— ";
		gun->fire();
	}
};


// 战士C
class Fighter_C : public Fighter_base {
public:
	void fight(Gun_base* gun) const {
		cout << "我是战士C！ ———— ";
		gun->fire();
	}
};
// ============================================


int main() {
	// 创建三种枪
	Gun_base* gun_01 = new Pistol();
	Gun_base* gun_02 = new Rifle();
	Gun_base* gun_03 = new Awp();

	// 创建三种战士
	Fighter_base* A = new Fighter_A();
	Fighter_base* B = new Fighter_B();
	Fighter_base* C = new Fighter_C();

	// 不同战士搭配不同枪械展示
	C->fight(gun_02);

	delete gun_01;
	delete gun_02;
	delete gun_03;

	delete A;
	delete B;
	delete C;
}
