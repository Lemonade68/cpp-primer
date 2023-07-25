#include <iostream>

// 练习15.20
class Base {
public:
    void pub_mem();         // public 成员
protected:
    int prot_mem;           // protected 成员
private:
    int priv_mem;           // private 成员
};

struct Pub_Derv : public Base {
    // 正确：派生类能访问 protected 成员
    int f() { return prot_mem; }
    // 错误：private 成员对于派生类来说是不可访问的
    // char g() { return priv_mem; }

    void memfcn(Base &b) {
        b = *this;
        std::cout << "Pub_Derv" << std::endl;
    }
};

struct Priv_Derv : private Base {
    // private 不影响派生类的访问权限
    int f1() { return prot_mem; }

    void memfcn(Base &b) {
        b = *this;
        std::cout << "Priv_Derv" << std::endl;
    }
};

struct Prot_Derv : protected Base {
    int f2() { return prot_mem; }

    void memfcn(Base &b) {
        b = *this;
        std::cout << "Prot_Derv" << std::endl;
    }
};

struct Derived_from_Public : public Pub_Derv {
    // 正确：Base::prot_mem 在 Pub_Derv 中仍然是 protected 的
    int use_base() { return prot_mem; }

    void memfcn(Base &b) {
        b = *this;
        std::cout << "Derived_from_Public" << std::endl;
    }
};

struct Derived_from_Private : public Priv_Derv {
//     错误：Base::prot_mem 在 Priv_Derv 中仍然是 private 的
//     int use_base() { return prot_mem; }

// Base 无法访问。因为 Priv_Derv : private Base, Priv_Derv 以
// 私有方式继承的 Base；Priv_Derv 的派生类 Derived_from_Private
// 无权访问其直接基类 Priv_Derv 的私有成员
//    void memfcn(Base &b) {
//        b = *this;
//        std::cout << "Derived_from_Private" << std::endl;
//    }
};

struct Derived_from_Protected : public Prot_Derv {
    // 正确：Base::prot_mem 在 Prot_Derv 中仍然是 protected 的
    int use_base() { return prot_mem; }

    void memfcn(Base &b) {
        b = *this;
        std::cout << "Derived_from_Protected" << std::endl;
    }
};



int main() {
    Pub_Derv d1;            // 继承自 Base 的成员是 public 的
    Priv_Derv d2;           // 继承自 Base 的成员是 private 的
    Prot_Derv d3;           // 继承自 Base 的成员是 protected 的

    // 练习 15.18
    Derived_from_Public dd1;
    Derived_from_Private dd2;
    Derived_from_Protected dd3;
    Base base;
    Base *p = new Base;
    p = &d1;                // d1 的类型是 Pub_Derv
    // p = &d2;                // d2 的类型是 Priv_Derv
    // p = &d3;                // d3 的类型是 Prot_Derv
    p = &dd1;               // dd1 的类型是 Derived_from_Public
    // p = &dd2;               // dd2 的类型是 Derived_from_Private
    // p = &dd3;               // dd3 的类型是 Derived_from_Protected

    // 练习 15.19
    d1.memfcn(base);
    d2.memfcn(base);
    d3.memfcn(base);
    dd1.memfcn(base);
    // dd2.memfcn(base);
    dd3.memfcn(base);

    return 0;
}