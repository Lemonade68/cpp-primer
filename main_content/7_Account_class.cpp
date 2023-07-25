#include<string>
#include<iostream>


class Account{
public:
    void calculate(){
        amount += amount * interestRate;
    }
    static double rate(){               //成员函数可直接访问静态成员
        return interestRate;
    }
    static void rate(double);

private:
    std::string owner;
    double amount;
    static double interestRate;
    static double initRate();
};


//类外定义static成员函数
void Account::rate(double newRate){
    interestRate = newRate;
}

//定义并初始化一个静态成员
double Account::interestRate = initRate();          //类名后所有部分都在类的作用域内了
