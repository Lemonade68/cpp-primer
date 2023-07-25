#include<iostream>
#include<string>
#include<vector>



class Sales_data{
//友元声明（最好集中放在最前面或最后面）
    friend Sales_data add(const Sales_data &, const Sales_data &);
    friend std::ostream &print(std::ostream &, const Sales_data &);
    friend std::istream &read(std::istream &, Sales_data &);

private:
// 数据成员
    std::string bookNo;
    unsigned units_sold = 0;                        // unsigned单独使用 = unsigned int
    double revenue = 0.0;                           // 收入，上面的是书号和销售数
    double avg_price() const;                       // 过程函数，非接口

public:
//构造函数（设成public）
    Sales_data() = default;
    Sales_data(const std::string &s) : 
            bookNo(s) { }
    // explicit Sales_data(const std::string &s) : bookNo(s) { }        //拒绝使用隐式转换
    Sales_data(const std::string &s, unsigned n, double p) :
            bookNo(s), units_sold(n), revenue(p*n) { }
    Sales_data(std::istream &);

// 关于Sales_data对象的操作（成员接口函数）
    std::string isbn() const { return bookNo; }     // std::string isbn() const { return this->bookNo; }
    Sales_data &combine(const Sales_data &);        // 这两个函数定义在外部
};
// Sales_data的非成员接口函数 （这些操作不用作成员函数更符合直觉）
Sales_data add(const Sales_data &, const Sales_data &);
std::ostream &print(std::ostream &, const Sales_data &);
std::istream &read(std::istream &, Sales_data &);



//类外部定义成员函数    注意作用域    (类外定义成内联的函数)
inline double Sales_data::avg_price() const{
    if(units_sold)
        return revenue / units_sold;
    else
        return 0;
}

//定义一个返回this对象的函数
Sales_data& Sales_data::combine(const Sales_data &rhs){
    units_sold += rhs.units_sold;                   //rhs的成员加到this对象的成员上
    revenue += rhs.revenue;
    return *this;                                   //返回调用该函数的对象
}

//定义read和print函数（非成员函数，无需作用域）
std::istream &read(std::istream &is, Sales_data &item){
    double price = 0.0;
    is >> item.bookNo >> item.units_sold >> price;
    item.revenue = price * item.units_sold;
    return is;
}

std::ostream &print(std::ostream &os, const Sales_data &item){
    os << item.isbn() << " " << item.units_sold << " " 
       << item.revenue << " " << item.avg_price();
    return os;
}

//定义add函数
Sales_data add(const Sales_data &lhs, const Sales_data & rhs){
    Sales_data sum = lhs;
    sum.combine(rhs);                               //rhs数据成员加到sum中
    return sum;
}

//类外部定义构造函数
Sales_data::Sales_data(std::istream &is){
    read(is, *this);                                //对this指针的解引用就是当前的对象
}


int main(){
    std::string s = "109";
    Sales_data item;
    item.combine(s);                                //如果使用explicit版本的构造函数，则这句也会报错
    // item.combine("109");                            //要执行两步隐式转换，会报错
    return 0;
}