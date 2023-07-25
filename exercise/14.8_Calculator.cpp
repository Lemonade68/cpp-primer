#include<iostream>
#include<string>
#include<functional>
#include<map>

//普通函数
int add(int i, int j) { return i + j; }

//lambda，产生一个未命名的函数对象类
auto mod = [](int i, int j){ return i % j; };

//函数对象类
struct divide{
    int operator()(int d1, int d2) { return d1 / d2; }
};

// 列举了可调用对象与二元运算符对应关系的表格
// 所有可调用对象都必须接受两个 int，返回一个 int
// 其中的元素可以是函数指针、函数对象或者 lambda
std::map<std::string, std::function<int(int, int)>> binops = {
    {"+", add},                                 // 函数指针
    {"-", std::minus<int>()},                   // 标准库函数对象
    {"*", [] (int i, int j) { return i * j; }}, // 未命名的 lambda
    {"/", divide()},                            // 用户定义的函数对象
    {"%", mod}                                  // 命名了的 lambda 对象
};


int main(){
    while(true){
        std::cout << "请输入：op1 op op2" << std::endl;
        int a, b;
        std::string c;
        std::cin >> a >> c >> b;
        //binops[c]返回一个function，function重载了operator()，使这两个数传递给保存的可调用对象
        std::cout << binops[c](a, b) << std::endl;      
    }
    return 0;
}