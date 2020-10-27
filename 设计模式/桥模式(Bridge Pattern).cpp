#include <iostream>


using namespace std;


// ============================================
// ǹе����
class Gun_base {
public:
	virtual ~Gun_base() {}

	virtual void fire() const = 0;
};


// ��ǹ
class Pistol : public Gun_base {
public:
	void fire() const {
		cout << "��ǹ���䡪�� biu biu biu��" << endl;
	}
};


// ��ǹ
class Rifle : public Gun_base {
public:
	void fire() const {
		cout << "��ǹ���䡪�� tu tu tu��" << endl;
	}
};


// �ѻ�ǹ
class Awp :public Gun_base {
public:
	void fire() const {
		cout << "�ѻ�ǹ���䡪�� peng��" << endl;
	}
};
// ============================================


// ============================================
// սʿ����
class Fighter_base {
public:
	virtual ~Fighter_base() {}

	virtual void fight(Gun_base* gun) const = 0;
};


// սʿA
class Fighter_A : public Fighter_base {
public:
	void fight(Gun_base* gun) const {
		cout << "����սʿA�� �������� ";
		gun->fire();
	}
};


// սʿB
class Fighter_B : public Fighter_base {
public:
	void fight(Gun_base* gun) const {
		cout << "����սʿB�� �������� ";
		gun->fire();
	}
};


// սʿC
class Fighter_C : public Fighter_base {
public:
	void fight(Gun_base* gun) const {
		cout << "����սʿC�� �������� ";
		gun->fire();
	}
};
// ============================================


int main() {
	// ��������ǹ
	Gun_base* gun_01 = new Pistol();
	Gun_base* gun_02 = new Rifle();
	Gun_base* gun_03 = new Awp();

	// ��������սʿ
	Fighter_base* A = new Fighter_A();
	Fighter_base* B = new Fighter_B();
	Fighter_base* C = new Fighter_C();

	// ��ͬսʿ���䲻ͬǹеչʾ
	C->fight(gun_02);

	delete gun_01;
	delete gun_02;
	delete gun_03;

	delete A;
	delete B;
	delete C;
}
