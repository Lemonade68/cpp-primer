#include<iostream>
#include<functional>
#include<cstring>
#include"14_Sales_data.h"

template<typename T>
int compare(const T& a, const T& b){
    if(std::less<T>()(a,b)) return -1;      //使用标准库函数对象less
    if(std::less<T>()(b,a)) return 1;
    return 0;
}

template<unsigned N, unsigned M>
int compare(const char (&p1)[N], const char (&p2)[M]){
    return strcmp(p1, p2);
}


//函数模板直接使用即可，编译器自动判断对应类型T
int main() {
    std::cout << compare(10, 1) << std::endl;
    std::cout << compare("hi", "mom") << std::endl;
    
    Sales_data s1("book1"), s2("book2");
    // std::cout << compare(s1, s2) << std::endl;   //sales_data中未定义<
    return 0;
}