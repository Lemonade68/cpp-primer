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
class ConstStrBlobPtr;

class StrBlob{
public:
    friend class StrBlobPtr;
    friend class ConstStrBlobPtr;
    friend bool operator==(const StrBlob &, const StrBlob &);
    friend bool operator!=(const StrBlob &, const StrBlob &);
    friend bool operator<(const StrBlob &s1, const StrBlob &s2);
    friend bool operator<=(const StrBlob &s1, const StrBlob &s2);
    friend bool operator>(const StrBlob &s1, const StrBlob &s2);
    friend bool operator>=(const StrBlob &s1, const StrBlob &s2);

public:
    typedef vector<string>::size_type size_type;
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
    
    // 提供给 ConstStrBlobPtr 的接口
    // 返回指向首元素和尾后元素的 ConstStrBlobPtr
    ConstStrBlobPtr cbegin() const;                     // 定义 ConstStrBlobPtr 后才能定义这两个函数
    ConstStrBlobPtr cend() const;
    
    // StrBlobPtr cbegin() const;                           //对常量对象的重载, cbegin才返回const的strblobptr
    // StrBlobPtr cend() const;

    //练习14.26
    string &operator[](size_t n);
    const string &operator[](size_t n) const;

private:
    shared_ptr<vector<string>> data;
    // 如果data[i]不合法，抛出一个异常
    void check(size_type i, const string &msg) const;   //工具函数，检测是否在合法范围内，msg描述了错误内容
};

bool operator==(const StrBlob &, const StrBlob &);
bool operator!=(const StrBlob &, const StrBlob &);
bool operator<(const StrBlob &s1, const StrBlob &s2);
bool operator<=(const StrBlob &s1, const StrBlob &s2);
bool operator>(const StrBlob &s1, const StrBlob &s2);
bool operator>=(const StrBlob &s1, const StrBlob &s2);



//.cpp中的内容

bool operator==(const StrBlob &lhs, const StrBlob &rhs){
    return lhs.data == rhs.data;
}

//使用上面的==来定义
bool operator!=(const StrBlob &lhs, const StrBlob &rhs){
    return !(lhs == rhs);
}

bool operator<(const StrBlob &s1, const StrBlob &s2) {
    return *s1.data < *s2.data;
}

bool operator<=(const StrBlob &s1, const StrBlob &s2) {
    return *s1.data <= *s2.data;
}

bool operator>(const StrBlob &s1, const StrBlob &s2) {
    return *s1.data > *s2.data;
}

bool operator>=(const StrBlob &s1, const StrBlob &s2) {
    return *s1.data >= *s2.data;
}


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


string& StrBlob::operator[](size_t n){
    check(n, "out of range");
    return data->at(n);
}

const string& StrBlob::operator[](size_t n)const{
    check(n, "out of range");
    return data->at(n);
}




// 核查指针类StrBlobPtr, 对访问一个不存在元素的尝试，StrBolobPtr会抛出异常
class StrBlobPtr{
    friend bool eq(const StrBlobPtr &, const StrBlobPtr &);
    friend bool operator==(const StrBlobPtr &, const StrBlobPtr &);
    friend bool operator!=(const StrBlobPtr &, const StrBlobPtr &);
    friend bool operator<(const StrBlobPtr &, const StrBlobPtr &);
    friend bool operator<=(const StrBlobPtr &, const StrBlobPtr &);
    friend bool operator>(const StrBlobPtr &, const StrBlobPtr &);
    friend bool operator>=(const StrBlobPtr &, const StrBlobPtr &);

public:
    StrBlobPtr():curr(0){ }
    StrBlobPtr(StrBlob &a, size_t sz = 0) : wptr(a.data),curr(sz){}
    StrBlobPtr(const StrBlob &a, size_t sz = 0) : wptr(a.data),curr(sz){}
    string& deref() const;
    StrBlobPtr& incr();
    StrBlobPtr& decr();

    const string &operator[](size_t) const;
    //递增和递减运算符，注意无法仅通过返回类型区分函数重载
    StrBlobPtr &operator++();                           //前置版本，没有参数，返回引用
    StrBlobPtr &operator--();
    StrBlobPtr operator++(int);                         //后置版本，int用来区分，返回右值
    StrBlobPtr operator--(int);             
    
    // 练习 14.28
    StrBlobPtr &operator+=(size_t);
    StrBlobPtr &operator-=(size_t);
    StrBlobPtr operator+(size_t) const;
    StrBlobPtr operator-(size_t) const;

    //练习 14.30
    string &operator*() const;                          //对于非常量对象：返回值可修改
    string *operator->() const;

private:
    size_t curr;
    weak_ptr<vector<string>> wptr;                      //注意：使用weak_ptr前需要检查
    shared_ptr<vector<string>> check(size_t, const string &) const;
};

bool eq(const StrBlobPtr &, const StrBlobPtr &);
bool operator==(const StrBlobPtr &, const StrBlobPtr &);
bool operator!=(const StrBlobPtr &, const StrBlobPtr &);
bool operator<(const StrBlobPtr &s1, const StrBlobPtr &s2);
bool operator<=(const StrBlobPtr &s1, const StrBlobPtr &s2);
bool operator>(const StrBlobPtr &s1, const StrBlobPtr &s2);
bool operator>=(const StrBlobPtr &s1, const StrBlobPtr &s2);



//指向常量对象的constStrblobptr
class ConstStrBlobPtr {
    // 练习 14.16
    friend bool operator==(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs);
    friend bool operator!=(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs);
    // 练习 14.18
    friend bool operator<(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2);
    friend bool operator<=(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2);
    friend bool operator>(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2);
    friend bool operator>=(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2);

public:
    ConstStrBlobPtr() : curr(0) {}
    ConstStrBlobPtr(const StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}

    string& deref() const;
    ConstStrBlobPtr& incr();     // 前缀递增
    ConstStrBlobPtr& decr();     // 后缀递减
    // 练习 14.26
    const string &operator[](size_t) const;
    // 练习 14.27
    ConstStrBlobPtr &operator++();       // 前置运算符
    ConstStrBlobPtr &operator--();
    ConstStrBlobPtr &operator++(int);    // 后置运算符
    ConstStrBlobPtr &operator--(int);
    // 练习 14.28
    ConstStrBlobPtr &operator+=(size_t);
    ConstStrBlobPtr &operator-=(size_t);
    ConstStrBlobPtr operator+(size_t) const;
    ConstStrBlobPtr operator-(size_t) const;
    // 练习 14.30
    const string &operator*() const;
    const string *operator->() const;

private:
    // 若检查成功，check 返回一个指向 vector 的 shared_ptr
    shared_ptr<vector<string>> check(size_t, const string&) const;
    // 保存一个 weak_ptr，意味着底层 vector 可能会被销毁
    weak_ptr<vector<string>> wptr;
    size_t curr;            // 在数组中的当前位置
};
bool operator==(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs);
bool operator!=(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs);
bool operator<(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2);
bool operator<=(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2);
bool operator>(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2);
bool operator>=(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2);





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


//操作符重载版本
bool operator==(const StrBlobPtr &lhs, const StrBlobPtr &rhs){
    //weak_ptr本身不支持operator==操作
    auto l = lhs.wptr.lock(), r = rhs.wptr.lock();
    if(l == r)
        return (!r || lhs.curr == rhs.curr);    //都为空则相等，或者不为空且另一个数据成员相等也相等
    else
        return false;                           //指向不同对象肯定不相等
}

bool operator!=(const StrBlobPtr &lhs, const StrBlobPtr &rhs){
    return !(lhs == rhs);
}


// 指针的<比较：首先指向同一个序列，然后比较前后顺序
bool operator<(const StrBlobPtr &s1, const StrBlobPtr &s2) {
    auto l = s1.wptr.lock(), r = s2.wptr.lock();
    if (l == r) {
        if (!r)
            return false;               // 两个指针都为空，认为是相等
        return (s1.curr < s2.curr);     // 指向相同 vector，比较指针位置
    } else {
        return false;                   // 指向不同 vector 时，不能比较
    }
}

// 学习这个思想，当对象不包含内置的 < 比较时，定义一个<，然后囊括 > / <= / >= 
bool operator>(const StrBlobPtr &s1, const StrBlobPtr &s2) {
    return s2 < s1;
}

// 这边尤其学习
bool operator<=(const StrBlobPtr &s1, const StrBlobPtr &s2) {
    return !(s2 < s1);
}

bool operator>=(const StrBlobPtr &s1, const StrBlobPtr &s2) {
    return !(s1 < s2);
}

const string& StrBlobPtr::operator[](size_t n) const {
    auto p = check(n, "dereference out of range");
    return (*p)[n];
}

//前置版本：返回对象的引用
StrBlobPtr& StrBlobPtr::operator++(){
    //如果已经指向容器的尾后位置，则无法继续递增它
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}

StrBlobPtr& StrBlobPtr::operator--(){
    //如果curr是0，则继续递减会导致一个无效下标
    --curr;
    check(curr, "decrement past begin of StrBlobPtr");
    return *this;
}

//后置版本
StrBlobPtr StrBlobPtr::operator++(int){
    auto ret = *this;                   //记录当前值
    ++*this;                            //当前对象++，且不需要检查是否越界，因为使用的是重载的前置++
    return ret;
}

StrBlobPtr StrBlobPtr::operator--(int){
    //无需检验有效性，调用前置递增时才需要检查
    auto ret = *this;
    --*this;
    return ret;
}

StrBlobPtr& StrBlobPtr::operator+=(size_t n) {
    curr += n;
    check(curr, "increment past end of StrBlobPtr");
    return *this;
}

StrBlobPtr& StrBlobPtr::operator-=(size_t n) {
    curr -= n;
    check(curr, "decrement past begin of StrBlobPtr");
    return *this;
}

//无需检验：+= 和 -= 已经检查过
StrBlobPtr StrBlobPtr::operator+(size_t n) const {
    StrBlobPtr ret = *this;
    ret += n;
    return ret;
}

StrBlobPtr StrBlobPtr::operator-(size_t n) const {
    StrBlobPtr ret = *this;
    ret -= n;
    return ret;
}

string& StrBlobPtr::operator*() const{
    auto data = check(curr, "dereference out of range");
    return (*data)[curr];
}

string* StrBlobPtr::operator->() const{
    return & this->operator*();                     //this显式写出来清晰
}




//ConstStrBlobPtr的定义，与上面只有解引用部分不同（返回不可改变的常量）

shared_ptr<vector<string>> ConstStrBlobPtr::check(size_t i, const string &msg) const {
    auto ret = wptr.lock(); // vector 还存在吗？
    if (!ret)
        throw runtime_error("unbound ConstStrBlobPtr");
    if (i >= ret->size())
        throw out_of_range(msg);
    return ret;             // 否则，返回指向 vector 的 shared_ptr
}

string& ConstStrBlobPtr::deref() const {
    auto p = check(curr, "dereference past end");
    return (*p)[curr];      // (*P) 是对象所指向的 vector
}

// 前缀递增：返回递增后的对象的引用
ConstStrBlobPtr& ConstStrBlobPtr::incr() {
    // 如果 curr 已经指向容器的尾后位置，就不能递增它
    check(curr, "increment past end of ConstStrBlobPtr");
    ++curr;                 // 推进当前位置
    return *this;
}

// 前缀递减：返回递减后的对象的引用
ConstStrBlobPtr& ConstStrBlobPtr::decr() {
    // 如果 curr 已经为 0，递减它会产生一个非法下标
    --curr;                 // 递减当前位置
    check(-1, "decrement past begin of ConstStrBlobPtr");
    return *this;
}

// StrBlob 的 begin 和 end 成员的定义
ConstStrBlobPtr StrBlob::cbegin() const {
    return ConstStrBlobPtr(*this);
}
ConstStrBlobPtr StrBlob::cend() const {
    auto ret = ConstStrBlobPtr(*this, data->size());
    return ret;
}

// ConstStrBlobPtr 的比较操作
bool operator==(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs) {
    auto l = lhs.wptr.lock(), r = rhs.wptr.lock();
    // 若底层的 vector 是同一个
    if (l == r)
        // 则两个指针都是空，或者指向相同元素时，它们相等
        return (!r || lhs.curr == rhs.curr);
    else
        return false;       // 若指向不同 vector，则不可能相等
}

bool operator!=(const ConstStrBlobPtr &lhs, const ConstStrBlobPtr &rhs) {
    return !(lhs == rhs);
}

bool operator<(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2) {
    auto l = s1.wptr.lock(), r = s2.wptr.lock();
    if (l == r) {
        if (!r)
            return false;               // 两个指针都为空，认为是相等
        return (s1.curr < s2.curr);     // 指向相同 vector，比较指针位置
    } else {
        return false;                   // 指向不同 vector 时，不能比较
    }
}

bool operator>(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2) {
    return s2 < s1;
}

bool operator<=(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2) {
    return !(s2 < s1);
}

bool operator>=(const ConstStrBlobPtr &s1, const ConstStrBlobPtr &s2) {
    return !(s1 < s2);
}

const string& ConstStrBlobPtr::operator[](size_t n) const {
    auto p = check(n, "dereference out of range");
    return (*p)[n];
}

// 前置版本：返回递增/递减对象的引用
ConstStrBlobPtr& ConstStrBlobPtr::operator++() {
    // 如果 curr 已经指向了容器的尾后位置，则无法递增它
    check(curr, "increment past end of ConstStrBlobPtr");
    ++curr;         // 将 curr 在当前状态下向前移动一个元素
    return *this;
}
ConstStrBlobPtr& ConstStrBlobPtr::operator--() {
    // 如果 curr 是 0，则继续递减它将产生一个无效下标
    --curr;         // 将 curr 在当前状态下后前移动一个元素
    check(curr, "decrement past begin of ConstStrBlobPtr");
    return *this;
}

// 后置版本：递增/递减对象的值但是返回原值
ConstStrBlobPtr& ConstStrBlobPtr::operator++(int) {
    // 此处无需检查有效性，调用前置递增运算时才需要检查
    ConstStrBlobPtr ret = *this;         // 记录当前的值
    ++*this;        // 向前移动一个元素，前置++需要检查递增的有效性
    return ret;     // 返回之前记录的状态
}
ConstStrBlobPtr& ConstStrBlobPtr::operator--(int) {
    // 此处无需检查有效性，调用前置递减运算时才需要检查
    ConstStrBlobPtr ret = *this;         // 记录当前的值
    ++*this;        // 向后移动一个元素，前置--需要检查递减的有效性
    return ret;     // 返回之前记录的状态
}

ConstStrBlobPtr& ConstStrBlobPtr::operator+=(size_t n) {
    curr += n;
    check(curr, "increment past end of ConstStrBlobPtr");
    return *this;
}

ConstStrBlobPtr& ConstStrBlobPtr::operator-=(size_t n) {
    curr -= n;
    check(curr, "decrement past begin of ConstStrBlobPtr");
    return *this;
}

ConstStrBlobPtr ConstStrBlobPtr::operator+(size_t n) const {
    ConstStrBlobPtr ret = *this;
    ret += n;
    return ret;
}

ConstStrBlobPtr ConstStrBlobPtr::operator-(size_t n) const {
    ConstStrBlobPtr ret = *this;
    ret -= n;
    return ret;
}

const string& ConstStrBlobPtr::operator*() const {
    auto p = check(curr, "dereference past end");
    return (*p)[curr];      // (*p) 是对象所指的 vector
}

const string* ConstStrBlobPtr::operator->() const {
    // 将实际工作委托给解引用运算符
    return & this->operator*();
}




#endif                                  //TEST_STRBLOB_H