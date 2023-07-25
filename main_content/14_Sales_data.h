#ifndef TEST_SALES_DATA_H
#define TEST_SALES_DATA_H


//.h部分
#include <iostream>
#include <string>
using std::string;

// added overloaded input, output, addition, and compound-assignment operators
class Sales_data {
    //注意这边定义为非成员函数
    friend std::istream &operator>>(std::istream&, Sales_data&);        // input，输入改变状态，因此是引用
    friend std::ostream &operator<<(std::ostream&, const Sales_data&);  // output，输出不改变状态，因此是const引用
    friend Sales_data operator+(const Sales_data&, const Sales_data&);  // addition

public:
    Sales_data(const std::string &s, unsigned n, double p)
        : bookNo(s), units_sold(n), revenue(n * p) { }
    Sales_data() : Sales_data("", 0, 0.0f) { }
    Sales_data(const std::string &s) : Sales_data(s, 0, 0.0f) { }
    Sales_data(std::istream&);

    Sales_data &operator+=(const Sales_data&);          // compound-assignment
    std::string isbn() const { return bookNo; }

    Sales_data &operator=(const string &);

private:
    double avg_price() const;

    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0;
};

// 除了友元声明，还要有实在的声明
std::istream &operator>>(std::istream&, Sales_data&);
std::ostream &operator<<(std::ostream&, const Sales_data&);
Sales_data operator+(const Sales_data&, const Sales_data&);

// 成员函数不需要重新声明，而友元需要
// Sales_data& Sales_data::operator=(const string &);

inline
double Sales_data::avg_price() const {
    return units_sold ? revenue / units_sold : 0;
}


//.cpp部分

Sales_data::Sales_data(std::istream &is) : Sales_data() {
    is >> *this;
}

//赋值运算返回左值
Sales_data& Sales_data::operator+=(const Sales_data &rhs) {
    units_sold += rhs.units_sold;
    revenue += rhs.revenue;
    return *this;
}

std::istream &operator>>(std::istream &is, Sales_data &item) {
    double price = 0.0;
    is >> item.bookNo >> item.units_sold >> price;
    //一次性输入完后再检验
    if (is)
        item.revenue = price * item.units_sold;
    else
        item = Sales_data();
    return is;
}

std::ostream &operator<<(std::ostream &os, const Sales_data &item) {
    //注意这边没有endl，这里定义时不需要刷新输出buf区
    os << item.isbn() << " " << item.units_sold << " " << item.revenue
       << " " << item.avg_price();
    return os;
}


//算数运算返回右值（可以返回局部变量的值），最好定义一个临时变量，因为传进来的是const
Sales_data operator+(const Sales_data &lhs, const Sales_data &rhs) {
    Sales_data sum = lhs;
    sum += rhs;
    return sum;
}

Sales_data& Sales_data::operator=(const string &isbn){
    bookNo = isbn;
    return *this;
}

#endif //TEST_SALES_DATA_H
