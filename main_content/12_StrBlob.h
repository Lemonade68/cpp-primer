#ifndef TEST_STRBLOB_H
#define TEST_STRBLOB_H

#include<vector>
#include<string>
#include<memory>
#include<exception>

using std::initializer_list;
using std::make_shared;
using std::out_of_range;
using std::runtime_error;
using std::shared_ptr;
using std::string;
using std::vector;
using std::weak_ptr;


// 对StrBlob中的友元来说，此前置声明是必要的
class StrBlobPtr;
class StrBlob{
public:
    typedef vector<string>::size_type size_type;
    friend class StrBlobPtr;

public:
    StrBlob();
    StrBlob(initializer_list<string> il);
    size_type size() { return data->size(); }
    bool empty() { return data->empty(); }
    // 添加和删除元素
    void push_back(const string &t) { data->push_back(t); }
    void push_back(string &&t) { data->push_back(std::move(t)); }
    void pop_back();

    void push_back(const string &t) const;              //不使用的原因：站在使用者的角度应该处理的是const vector
    void pop_back(const string &t) const;               //具体见笔记上的链接解释

    // 元素访问
    string& front();
    string& back();
    const string& front() const;                        //对常量对象的重载
    const string& back() const;

    // 提供给StrBlobPtr的接口
    // 返回指向首元素和尾后元素的StrBlobPtr
    StrBlobPtr begin();                                 //定义StrBlobPtr后才能定义这两个函数
    StrBlobPtr end();                                   //StrBlob类结束后就就会编译函数体，如果这里定义的话会报错
    StrBlobPtr begin() const;                           //对常量对象的重载, cbegin才返回const的strblobptr
    StrBlobPtr end() const;


private:
    shared_ptr<vector<string>> data;
    // 如果data[i]不合法，抛出一个异常
    void check(size_type i, const string &msg) const;   //工具函数，检测是否在合法范围内，msg描述了错误内容
};



//.cpp中的内容

StrBlob::StrBlob():
    data(make_shared<vector<string>>()) { }             //在动态内存上默认分配一个空的vector

StrBlob::StrBlob(initializer_list<string> il):
    data(make_shared<vector<string>>(il)) { }           //在动态内存上使用列表初始化创建一个vector

void StrBlob::check(size_type i, const string &msg) const{
    if(i>=data->size())
        throw out_of_range(msg);
}

string& StrBlob::front(){
    check(0, "front on empty StrBlob");
    return data->front();
}

const string& StrBlob::front()const{
    check(0, "front on empty StrBlob");
    return data->front();
}

string& StrBlob::back(){
    check(0, "back on empty StrBlob");
    return data->back();
}

const string& StrBlob::back()const{
    check(0, "back on empty StrBlob");
    return data->back();
}

void StrBlob::pop_back(){
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}


// 由于只有一个shared_ptr成员，因此拷贝、赋值和销毁的都是这个数据成员，
// 从而实现多个StrBlob类指向一个vector的目的


// 核查指针类StrBlobPtr, 对访问一个不存在元素的尝试，StrBolobPtr会抛出异常
class StrBlobPtr{
    friend bool eq(const StrBlobPtr &, const StrBlobPtr &);

public:
    StrBlobPtr():curr(0){ }
    StrBlobPtr(StrBlob &a, size_t sz = 0):
            wptr(a.data),curr(sz){}
    StrBlobPtr(const StrBlob &a, size_t sz = 0):
            wptr(a.data),curr(sz){}
    string& deref() const;
    StrBlobPtr& incr();
    StrBlobPtr& decr();

private:
    size_t curr;
    weak_ptr<vector<string>> wptr;                      //注意：使用weak_ptr前需要检查
    shared_ptr<vector<string>> check(size_t, const string &) const;
};

bool eq(const StrBlobPtr &, const StrBlobPtr &);



//.cpp文件内容
shared_ptr<vector<string>> StrBlobPtr::check(size_t i, const string &msg) const{
    auto ret = wptr.lock();
    if(!ret)
        throw runtime_error("unbound StrBlobPtr");
    if(i>=ret->size())
        throw out_of_range(msg);
    return ret;                                         //否则，返回指向vector的shared_ptr
}

string& StrBlobPtr::deref() const{
    auto ret = check(curr, "dereference past end");
    return (*ret)[curr];
}

StrBlobPtr& StrBlobPtr::incr() {
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;                                       //前置递增：返回自身引用作为左值
}

StrBlobPtr& StrBlobPtr::decr(){
    --curr;
    check(curr, "decrement past begin of SrtBlobPtr");     //博客上的错误版本(确实写错了)
    return *this;
    // if(curr == 0)
        // throw out_of_range("decrement past begin of StrBlobPtr");
}

//StrBlob的begin和end成员的定义
StrBlobPtr StrBlob::begin(){
    return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end(){
    return StrBlobPtr(*this, data->size());
}

StrBlobPtr StrBlob::begin()const{
    return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end()const{
    return StrBlobPtr(*this, data->size());
}


//StrBlobPtr的比较操作
bool eq(const StrBlobPtr &lhs, const StrBlobPtr &rhs){
    auto l = lhs.wptr.lock(), r = rhs.wptr.lock();
    //若底层的vector是同一个
    if(l==r)
        return (!r || lhs.curr == rhs.curr);            //两指针都为空，或指向相同元素时，相等
    else
        return false;                                   //指向不同元素则可肯定不等
}

bool neq(const StrBlobPtr &lhs, const StrBlobPtr &rhs){
    return !eq(lhs, rhs);
}


#endif                                  //TEST_STRBLOB_H