#ifndef TEST_QUOTE_H
#define TEST_QUOTE_H

#include<iostream>
#include<string>
#include<memory>
#include<set>

class Quote{
public:
    //如果派生类需要移动操作的话，应在基类中显式地声明合成移动操作，以及拷贝操作

    Quote() = default;
    Quote(const Quote &) = default;
    Quote(Quote &&) = default;
    Quote &operator=(const Quote &) = default;
    Quote &operator=(Quote &&) = default;
    virtual ~Quote() = default;                 //对析构函数进行动态绑定(显式使用)
    
    Quote(const std::string &book, double sales_price):
                    bookNo(book),price(sales_price) { }

    std::string isbn() const { return bookNo; }
    
    //派生类负责改写并使用不同的折扣计算算法
    virtual double net_price(std::size_t n) const { return n * price; }

    //练习15.11
    virtual void debug();

    //Basket类中add_item函数所使用的虚函数,返回当前对象的一份动态分配拷贝
    virtual Quote *clone() const & { return new Quote(*this); }
    virtual Quote *clone() && { return new Quote(std::move(*this)); }       //注意*this是个左值

private:
    std::string bookNo;
protected:
    double price = 0.0;
};

inline void Quote::debug(){
    std::cout << "Parameters: " << bookNo << " " << price << std::endl;
}



class Disc_quote : public Quote{
public:
    Disc_quote() = default;
    Disc_quote(const std::string &book, double price, std::size_t qty, double disc):
                Quote(book,price),quantity(qty),discount(disc) { }
    double net_price(std::size_t) const = 0;        //没写override也可以（保证函数名、形参一样即可）
protected:
    std::size_t quantity = 0;                       //折扣适用的购买量
    double discount = 0.0;                          //折扣
};




class Bulk_quote : public Disc_quote{
public:
    // Bulk_quote() = default;

    //练习15.27   使用using声明来构造派生类的构造函数
    using Disc_quote::Disc_quote;

    Bulk_quote(const std::string &, double, std::size_t, double);
    
    //覆写的虚函数
    double net_price(std::size_t n) const override;

    //练习15.11
    void debug() override;

    //Basket类中需要的函数，覆写上面的基类的虚函数
    Bulk_quote *clone() const & override { return new Bulk_quote(*this); }
    Bulk_quote *clone() && override { return new Bulk_quote(std::move(*this)); }
};


//构造函数：利用基类的构造函数来构造基类部分
Bulk_quote::Bulk_quote(const std::string &book, double p, std::size_t qty, double disc):
            Disc_quote(book, price, qty, disc) { }

double Bulk_quote::net_price(std::size_t n) const{
    if (n >= quantity)
        return n * (1 - discount) * price;
    else
        return n * price;
}

void Bulk_quote::debug(){
    Quote::debug();
    std::cout << quantity << " " << discount << std::endl;
}



double print_total(std::ostream &os, const Quote &item, std::size_t n) {
    // 根据传入 item 形参的对象类型调用 Quote::net_price
    // 或者 Bulk_quote::net_price
    double ret = item.net_price(n);
    os << "ISBN: " << item.isbn()
       << " # sold: " << n << " total due: " << ret << std::endl;
    return ret;
}


//15.8 Basket类
class Basket{
public:
    //使用合成的默认构造函数和拷贝控制成员
    
    //往购物篮中加入书籍
    void add_item(const Quote &);           //左值拷贝版本
    void add_item(Quote &&);                //右值引用版本
    //打印每本书的总价和购物篮中所有书的总价
    double total_receipt(std::ostream &) const;

private:
    //该函数用于比较shared_ptr，multiset成员会使用它
    static bool compare(const std::shared_ptr<Quote> &lhs, const std::shared_ptr<Quote> &rhs){
        return lhs->isbn() < rhs->isbn();
    }
    //multiset保存多个报价，按照compare成员排序
    std::multiset<std::shared_ptr<Quote>, decltype(compare)*> items{compare};
};


double Basket::total_receipt(std::ostream &os) const{
    double sum = 0.0;                   //保存实时计算出的总价格
    //iter指向isbn相同的一批元素中的第一个
    //upper_bound返回一个迭代器，该迭代器指向这批元素的尾后位置
    for (auto iter = items.cbegin(); iter != items.cend(); iter = items.upper_bound(*iter)){
        sum += print_total(os, **iter, items.count(*iter));
    }
    os << "Total Sale: " << sum << std::endl;            //打印最终总价格
    return sum;
}


//上面Quote和Bulk_quote新添加的函数clone记得看下
void Basket::add_item(const Quote &sale){
    items.insert(std::shared_ptr<Quote>(sale.clone()));     //sale.clone返回new的Quote*，转换为shared_ptr
}

void Basket::add_item(Quote &&sale){
    items.insert(std::shared_ptr<Quote>(std::move(sale.clone())));
}



#endif