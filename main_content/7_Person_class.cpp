#include<iostream>
#include<string>
#include<vector>


class Person{
private:
    std::string Name;        //姓名
    std::string Address;     //地址

public:
//默认构造函数、接受两个实参的构造函数、从标准输入流读取数据的构造函数
    Person() = default;
    Person(const std::string &name, const std::string &add):
        Name(name),Address(add) { }
    explicit Person(std::istream &is) { read(is, *this); }

    std::string get_name() const { return Name; }                       //不改变数据成员的值  用常量成员函数   加const
    std::string get_address() const { return Address; }

    std::istream &read(std::istream &is, Person &p);             
    std::ostream &print(std::ostream &os, const Person &p);             //const别忘加
};


std::istream &Person::read(std::istream &is, Person &p){                //类外定义成员函数：作用域要加，使用get_name接口(类外无法访问数据成员)       
    is >> p.Name >> p.Address;
    return is;
}

std::ostream &Person::print(std::ostream &os, const Person &p){         //类外定义成员函数：作用域要加，使用get_name接口
    os << p.Name << " " << p.Address;
    return os;
}
