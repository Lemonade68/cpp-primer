#include <string>
#include <iostream>

//练习15.26

class Quote {
public:
    Quote() {
        std::cout << "Quote Default Constructor" << std::endl;
    }
    Quote(const std::string &book, double sales_price)
        : bookNo(book), price(sales_price) {
        std::cout << "Quote Constructor taking two parameters" << std::endl;
    }
    std::string isbn() const { return bookNo; }
    // 返回给定数量的书籍的销售总额
    // 派生类负责改写并使用不同的折扣计算算法
    virtual double net_price(std::size_t n) const { return n * price; }
    virtual ~Quote() {          // 对析构函数进行动态绑定
        std::cout << "Quote Destructor" << std::endl;
    }

    Quote(const Quote &rhs) : bookNo(rhs.bookNo), price(rhs.price) {
        std::cout << "Quote Copy Constructor" << std::endl;
    }

    Quote &operator=(const Quote &rhs) {
        std::cout << "Quote Copy assignment operator" << std::endl;

        price = rhs.price;
        bookNo = rhs.bookNo;
        return *this;
    }

    Quote(Quote &&rhs) noexcept
        : bookNo(std::move(rhs.bookNo)), price(std::move(rhs.price)) {
        std::cout << "Quote Move Constructor" << std::endl;
    }

    Quote &operator=(Quote &&rhs) noexcept {
        std::cout << "Quote Move assignment operator" << std::endl;

        bookNo = std::move(rhs.bookNo);
        price = std::move(rhs.price);
        return *this;
    }

private:
    std::string bookNo;                 // 书籍的 ISBN 编号

protected:
    double price = 0.0;                 // 代表普通状态下不打折的价格
};

// 计算并打印销售给定数量的某种书籍所得的费用
double print_total(std::ostream &os, const Quote &item, std::size_t n) {
    // 根据传入 item 形参的对象类型调用 Quote::net_price
    // 或者 Bulk_quote::net_price
    double ret = item.net_price(n);
    os << "ISBN: " << item.isbn()
       << " # sold: " << n << " total due: " << ret << std::endl;
    return ret;
}


class Bulk_quote : public Quote {
public:
    Bulk_quote() {
        std::cout << "Bulk_quote Default Constructor" << std::endl;
    }
    Bulk_quote(const std::string &book, double price,
            std::size_t qty, double disc) :
            Quote(book, price), min_qty(qty), discount(disc) {
        std::cout << "Bulk_quote Constructor taking four parameters" << std::endl;
    }
    Bulk_quote(const Bulk_quote &rhs) : Quote(rhs), min_qty(rhs.min_qty),
        discount(rhs.discount) {
        std::cout << "Bulk_quote Copy Constructor" << std::endl;
    }
    Bulk_quote &operator=(const Bulk_quote &rhs) {
        std::cout << "Bulk_quote Copy assignment operator" << std::endl;

        Quote::operator=(rhs);
        min_qty = rhs.min_qty;
        discount = rhs.discount;
        return *this;
    }
    Bulk_quote(Bulk_quote &&rhs) noexcept : Quote(std::move(rhs)),
        min_qty(std::move(rhs.min_qty)), discount(std::move(rhs.discount)) {
        std::cout << "Bulk_quote Move Constructor" << std::endl;
    }
    Bulk_quote &operator=(Bulk_quote &&rhs) noexcept {
        std::cout << "Bulk_quote Move assignment operator" << std::endl;

        Quote::operator=(rhs);
        min_qty = std::move(rhs.min_qty);
        discount = std::move(rhs.discount);
        return *this;
    }
    virtual ~Bulk_quote() {
        std::cout << "Bulk_quote Destructor" << std::endl;
    }
    // 覆盖基类中的函数版本以实现一种新的折扣策略
    double net_price(std::size_t) const override;

private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};

double Bulk_quote::net_price(std::size_t cnt) const {
    if (cnt >= min_qty)
        return cnt * (1 - discount) * price;
    else
        return cnt * price;
}


int main() {
    Bulk_quote bulk("978-7-121-15535-2", 100, 2, 0.2);    // 派生类对象
    std::cout << std::endl;

    std::cout << "------Bulk_quote bulk1 = bulk------" << std::endl;
    Bulk_quote bulk1 = bulk;
    std::cout << std::endl;

    std::cout << "------Bulk_quote bulk2, bulk2 = bulk------" << std::endl;
    Bulk_quote bulk2;
    bulk2 = bulk;
    std::cout << std::endl;

    std::cout << "------Bulk_quote bulk3 = std::move(bulk)------" << std::endl;
    Bulk_quote bulk3 = std::move(bulk);
    std::cout << std::endl;

    std::cout << "------Quote quote1 = bulk------" << std::endl;
    Quote quote1 = bulk;
    std::cout << std::endl;

    std::cout << "------Quote quote2 = std::move(bulk)------" << std::endl;
    Quote quote2 = std::move(bulk);
    std::cout << std::endl;

    return 0;
}


// 最后析构的结果前两个是bulk和bulk1，后面依次是 bulk2, bulk3, quote1, quote2的析构函数
