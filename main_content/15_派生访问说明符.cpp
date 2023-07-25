//展示了派生访问说明符对派生类对象的作用

#include<iostream>
using namespace std;

class A{
public:
    int pub_mem = 0;
private:
    int pri_mem = 1;
protected:
    int pro_mem = 2;

};

struct Pub : public A{
    int f1() { return pub_mem; }
    // int f2() { return pri_mem; }        //不可访问
    int f3() { return pro_mem; }
};

struct Pri : private A{
    int f1() { return pub_mem; }
    // int f2() { return pri_mem; }        //不可访问
    int f3() { return pro_mem; }
};

int main() {
    Pub pub;
    Pri pri;

    // f1 f3对于二者而言都是public的，且两个类都可以访问A中的public和protected成员
    // 但是对于A中的派生到两个类中的成员：
    // pub_mem：对Pub而言等于公共成员，对象pub可以访问； 对于Pri而言等于私有成员，对象pri不可访问
    // pri_mem：对Pub和Pri都是私有成员，对象pub和pri都不可访问
    // pro_mem：根据protected的定义，对象pub和pri都不可访问
    
    cout << pub.f1() << " " << pub.f3() << " " << endl;
    cout << pub.pub_mem << endl;            //可以访问
    // cout << pub.pri_mem << endl;            //不可访问
    // cout << pub.pro_mem << endl;            //不可访问

    cout << pri.f1() << " " << pri.f3() << " " << endl;
    // cout << pri.pub_mem << endl;            //不可访问
    // cout << pri.pri_mem << endl;            //不可访问
    // cout << pri.pro_mem << endl;            //不可访问
    
    return 0;
}