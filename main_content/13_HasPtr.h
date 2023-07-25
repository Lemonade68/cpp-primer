#ifndef has_ptr
#define has_ptr

#include<iostream>
#include<string>

using std::cout;
using std::endl;
using std::string;

class HasPtr{
    friend void swap(HasPtr&, HasPtr&);

public:
    HasPtr(const string &s = string()) : ps(new string(s)),i(0) { }
    HasPtr(const HasPtr &);
    HasPtr &operator=(const HasPtr &);
    HasPtr &operator=(const string &);      // 赋予新 string
    string &operator*();                    // 解引用
    bool operator<(const HasPtr &) const;
    ~HasPtr();

private:
    string *ps;
    int i;
};


inline void swap(HasPtr &hp1, HasPtr &hp2){
    using std::swap;
    cout << "swap " << *hp1.ps << " and " << *hp2.ps << endl;
    swap(hp1.ps, hp2.ps);   //交换指针，而不是交换string数据
    swap(hp1.i, hp2.i);     //交换int成员
}

HasPtr::HasPtr(const HasPtr &p):
    ps(new string(*p.ps)),i(p.i){ }

HasPtr& HasPtr::operator=(const HasPtr &p){
    // ps = new string(*p.ps);                 //不对！原来的ps会被丢失而没有delete
    string *new_ps = new string(*p.ps);
    delete ps;
    ps = new_ps;
    i = p.i;
    return *this;
}

HasPtr& HasPtr::operator=(const string &rhs) {
    *ps = rhs;
    return *this;
}

string& HasPtr::operator*() {
    return *ps;
}

bool HasPtr::operator<(const HasPtr &hp) const{
    return *ps < *hp.ps;
}

HasPtr::~HasPtr(){
    delete ps;          //直接分配的内存记得delete掉
}


#endif