#ifndef hppl
#define hppl

#include<iostream>
#include<string>

using std::string;

class HasPtr{
public:
    //构造函数分配新的string和新的计数器，并将计数器置为1
    HasPtr(const string &s = string()) : ps(new string(s)),i(0),use(new std::size_t(1)) { }
    //拷贝构造函数拷贝所有三个数据成员，并递增计数器
    HasPtr(const HasPtr &p) : ps(p.ps), i(p.i), use(p.use) { ++*use; }
    HasPtr &operator=(HasPtr &);
    ~HasPtr();

private:
    string *ps;
    int i;
    std::size_t *use;       //动态成员，虽然是直接分配的，但也需要拷贝！
};

HasPtr& HasPtr::operator=(HasPtr &p){
    ++*p.use;               //    放最上面的原因：处理自赋值情况
    if(--*use == 0){        //如果这个放前面，则对应自赋值会出现问题
        delete ps;
        delete use;
    }
    ps = p.ps;
    i = p.i;
    use = p.use;            //计数器也要拷贝
    return *this;
}

HasPtr::~HasPtr(){
    if(--*use == 0){
        delete ps;          //释放string内存
        delete use;         //释放计数器内存
    }
}

#endif