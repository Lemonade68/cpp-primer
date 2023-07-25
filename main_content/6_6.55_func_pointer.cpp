#include<iostream>
#include<vector>


// std::vector<decltype(func) *> vec;
std::vector<int (*)(int, int)> vec;

typedef int (*PF)(int a, int b);               //括号一定要记得加，  不然就是返回int*的函数了
// using PF = int (*)(int a, int b);

int func1(int a,int b){
    return a + b;
}
int func2(int a,int b){
    return a - b;
}
int func3(int a,int b){
    return a * b;
}
int func4(int a,int b){
    return a / b;
}

void compute(int a, int b , PF p){
    std::cout << p(a, b) << std::endl;
}


int main(){
    // int (*pf1)(int a, int b) = func1;
    // int (*pf2)(int a, int b) = func2;
    // int (*pf3)(int a, int b) = func3;
    // int (*pf4)(int a, int b) = func4;

    PF pf1 = func1, pf2 = func2, pf3 = func3, pf4 = func4;
    // decltype(func1) *p1 = func1, *pf2 = func2, *pf3 = func3, *pf4 = func4; 

    vec.push_back(pf1);
    vec.push_back(pf2);
    vec.push_back(pf3);
    vec.push_back(pf4);

    int a = 5, b = 10;

    for(auto x : vec){
        std::cout << x(a, b) << std::endl;
    }

    return 0;
}