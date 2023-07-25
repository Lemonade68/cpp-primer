// 这边只有练习13.13

#include <iostream>
#include <vector>

using namespace std;

struct X {
    X() { cout << "构造函数 X()" << endl; }
    X(const X &) { cout << "拷贝构造函数 X(const X &)" << endl; }
    X &operator=(const X &rhs) {
        cout << "拷贝赋值运算符 =(const X &)" << endl;
        return *this;
    }
    ~X() { cout << "析构函数 ~X()" << endl; }
};

void f1(X x) { }
void f2(X &x) { }

int main() {
    cout << "局部变量：" << endl;
    X x;
    cout << endl;

    cout << "非引用参数传递：" << endl;
    f1(x);
    cout << endl;

    cout << "引用参数传递：" << endl;
    f2(x);
    cout << endl;

    cout << "动态分配：" << endl;
    X *px = new X;                  //使用了默认构造函数？
    cout << endl;

    cout << "添加到容器中：" << endl;
    vector<X> vx;
    vx.push_back(x);
    cout << endl;

    cout << "释放动态分配对象：" << endl;
    delete px;
    cout << endl;

    cout << "拷贝初始化和赋值：" << endl;
    X y = x;
    y = x;
    cout << endl;

    cout << "程序结束！" << endl;

    // 最后三个析构函数对应  y,vector中的x元素,x
    return 0;
}
