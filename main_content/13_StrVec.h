#ifndef STRVEC
#define STRVEC

#include<iostream>

#include<string>
using std::string;

#include<memory>
using std::allocator;
using std::uninitialized_copy;

#include<utility>
using std::pair;

using std::make_move_iterator;

#include<initializer_list>
using std::initializer_list;

#include<algorithm>
using std::equal;
using std::for_each;

class StrVec{
public:
    StrVec():elements(nullptr), first_free(nullptr), cap(nullptr){ }
    StrVec(const StrVec &);
    StrVec(StrVec &&) noexcept;     //移动构造函数不需要const，且noexcept在声明和定义时都需要加
    ~StrVec();
    
public:
    StrVec &operator=(const StrVec &);
    StrVec &operator=(StrVec &&);

    void push_back(const string &);     //拷贝元素
    void push_back(string &&);          //移动元素
    const size_t size() const{ return first_free - elements; }
    const size_t capacity() const{ return cap - elements; }
    string *begin() { return elements; }
    string *end() { return cap; }
    const string *begin() const { return elements; }
    const string *end() const { return cap; }

    //练习13.39
    void reserve(size_t n){ if(n>capacity()) reallocate(n); }       //预留一部分空间
    void resize(size_t);
    void resize(size_t, const string &);

    //练习13.40
    StrVec(initializer_list<string>);

private:
    static allocator<string> alloc;     //分配元素，共用一个即可
    
    // 添加元素的函数使用
    void chk_n_alloc() { if(size() == capacity())  reallocate(); }      
    
    //工具函数，被拷贝构造函数，赋值运算符和析构函数使用
    pair<string *, string *> alloc_n_copy(const string *, const string *);
    void free();                //销毁元素，释放内存
    void reallocate();          //获得更多内存，并拷贝已有元素

    string *elements;           //指向数组首元素的指针
    string *first_free;         //指向数组第一个空闲元素的指针
    string *cap;                //指向数组尾后位置的指针
    
    void reallocate(size_t);    //重载形式
};





//剩下部分应该放在cpp文件中

//静态成员变量要在cpp文件中定义
allocator<string> StrVec::alloc;

void StrVec::push_back(const string &s){
    chk_n_alloc();              //确保有空间容纳新元素
    alloc.construct(first_free++, s);       
}

void StrVec::push_back(string &&s){
    chk_n_alloc();
    alloc.construct(first_free++, std::move(s));    //注意这边不传s，因为s也是一个左值
}

pair<string*, string*> StrVec::alloc_n_copy(const string *b, const string *e){      //n - and
    //分配空间保存给定范围中的元素
    auto data = alloc.allocate(e - b);
    //初始化并返回一个pair，由data（首地址）和uninitialized_copy的返回值（尾后指针）构成
    return {data, uninitialized_copy(b, e, data)};
}

void StrVec::free(){
    //不能传递给deallocate一个空指针，如果elements为空，则什么也不做
    if(elements){
        for (auto p = first_free; p != elements; )      //不能使用range for，直接修改元素
            alloc.destroy(--p);
        //使用for_each和lambda函数版本
        // for_each(elements, first_free, [this](string &s) {alloc.destroy(&s);});
        alloc.deallocate(elements, capacity());
    }
}

StrVec::StrVec(const StrVec &s){
    auto newdata = alloc_n_copy(s.begin(), s.end());
    elements = newdata.first;
    first_free = cap = newdata.second;
}

StrVec::StrVec(StrVec &&s) noexcept : elements(s.elements),first_free(s.first_free),cap(s.cap){
    s.elements = s.first_free = s.cap = nullptr;
}

StrVec::~StrVec(){
    free();
}

StrVec& StrVec::operator=(const StrVec &rhs){
    auto newdata = alloc_n_copy(rhs.begin(), rhs.end());        //这一步优先：解决自赋值问题
    free();
    elements = newdata.first;
    first_free = cap = newdata.second;
    return *this;
}

StrVec& StrVec::operator=(StrVec &&rhs){
    if(this != &rhs){                   //直接检测自赋值
        free();
        elements = rhs.elements;
        first_free = rhs.first_free;
        cap = rhs.cap;
        rhs.elements = rhs.first_free = rhs.cap = nullptr;
    }
    return *this;
}

void StrVec::reallocate(){
    auto newcapacity = size() ? 2 * size() : 1;     //分配两倍的大小，若为空，则分配大小为1的内存
    auto newdata = alloc.allocate(newcapacity);     //分配新内存

    //将数据从旧内存移动到新内存
    auto dest = newdata;                //指向新数组中的下一个空闲位置
    auto elem = elements;               //指向旧数组中的下一个元素
    for (size_t i = 0; i != size(); ++i){
        alloc.construct(dest++, std::move(*elem++));        //std::move返回一个右值引用，construct利用右值引用达成移动的效果
    }
    free();                             //移动完元素就是放旧内存空间
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;
}

void StrVec::reallocate(size_t newcapacity){
    auto newdata = alloc.allocate(newcapacity);

    auto dest = newdata;                //指向目的地，供construct第一个参数使用
    auto elem = elements;               

    for (size_t i = 0; i < size(); ++i)
        alloc.construct(dest++, std::move(*elem++));
    free();

    elements = newdata;
    first_free = elem;
    cap = elements + newcapacity;
}


//使用移动迭代器版本的reallocate
void StrVec::reallocate(){
    auto newcapacity = size() ? 2 * size() : 1;
    auto first = alloc.allocate(newcapacity);

    auto last = uninitialized_copy(make_move_iterator(begin()), make_move_iterator(end()), first);
    free();

    elements = first;
    first_free = last;
    cap = elements + newcapacity;
}


void StrVec::resize(size_t count){
    resize(count, string());            //利用自己的重载版本
}

void StrVec::resize(size_t count, const string &s){
    if(count > size()){
        if(count > capacity())
            reserve(count * 2);         //预留多余的空间
        for (auto i = size(); i != count; ++i)
            alloc.construct(first_free++, s);
    }
    else if(count < size()){
        while(first_free != elements + count)
            alloc.destroy(--first_free);
    }
}

StrVec::StrVec(initializer_list<string> il){
    auto newdata = alloc_n_copy(il.begin(), il.end());
    elements = newdata.first;
    cap = first_free = newdata.second;
}


#endif