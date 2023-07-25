#include <iostream>
#include "14_StrBlob_blog.h"

using namespace std;

/*
 * MyClass 类定义了一个指向 StrBlobPtr 的指针
 */
class MyClass {
public:
    MyClass() = default;
    MyClass(StrBlobPtr *p) : pointer(p) {}

    StrBlobPtr &operator*() { return *this->pointer; };
    StrBlobPtr *operator->() { return &this->operator*(); };

private:
    StrBlobPtr *pointer = nullptr;
};

int main() {
    StrBlob a1 = {"hi", "bye", "now"};
    StrBlobPtr p1(a1);               // p1 指向 a1 中的 vector
    MyClass p(&p1);

    //这边就是StrBlobPtr的->
    cout << (*p)->front() << endl;
    cout << (*p)->back() << endl;
    
    //下面这个好好分析：第一个->是myclass里的，第二个->是Strblobptr里的
    //一开始p->operator->()变成*(p.pointer).operator->()，返回值是一个string*，再->front()
    cout << p->operator->()->front();
    cout << p->operator->()->back() << endl;

    return 0;
}
