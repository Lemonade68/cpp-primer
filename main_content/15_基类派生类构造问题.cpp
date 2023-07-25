#include<iostream>
using namespace std;

class Sequence{
public:
    Sequence():num(0) { }
    Sequence(int a ) : num(a) { } 
    virtual void print() = 0;
protected : 
    int num;
};

class Increment : public Sequence{
public:
    Increment(int a) { num = a; }
    // Increment(int a) : num(a) {}     //会报错：基类构造函数已经使用，基类成员已经构造过了，不能重构

    void print() override {
        std::cout << num << "\t";
        ++num;
    }
};

class Power : public Sequence{
public:
    Power(int a) : Sequence(a) {}
    void print() override {
        std::cout << num << "\t";
        num *= num;
    }
};

class Decrement:public Sequence{
public:
    Decrement(int a) : Sequence(a) {}
    void print() override {
        std::cout << num << "\t";
        --num;
    }
};

int main(){
    Sequence *spi = new Increment(2);
    Sequence *spp = new Power(3);
    Sequence *spd = new Decrement(4);
    for (int i = 0; i < 3; ++i){
        spi->print();
        spp->print();
        spd->print();
        std::cout << std::endl;
    }
    return 0;
}