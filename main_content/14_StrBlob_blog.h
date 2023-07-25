#ifndef TEST_STRBLOB_H
#define TEST_STRBLOB_H

//.h文件中内容

#include <vector>
#include <string>
#include <initializer_list>
#include <memory>
#include <stdexcept>

using std::string;
using std::vector;
using std::initializer_list;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
using std::out_of_range;
using std::runtime_error;

// 对于 StrBlob 中的友元声明来说，此前置声明是必要的
class StrBlobPtr;
class ConstStrBlobPtr;

//===========================================
//
//    StrBlob - custom vector<string>
//
//===========================================
class StrBlob {
    friend class StrBlobPtr;
    friend class ConstStrBlobPtr;
    // 练习 14.16
    friend bool operator==(const StrBlob &lhs, const StrBlob &rhs);
    friend bool operator!=(const StrBlob &lhs, const StrBlob &rhs);
    // 练习 14.18
    friend bool operator<(const StrBlob &s1, const StrBlob &s2);
    friend bool operator<=(const StrBlob &s1, const StrBlob &s2);
    friend bool operator>(const StrBlob &s1, const StrBlob &s2);
    friend bool operator>=(const StrBlob &s1, const StrBlob &s2);

public:
    typedef vector<string>::size_type size_type;
    StrBlob();
    StrBlob(initializer_list<string> il);
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    // 添加和删除元素
    void push_back(const string &t) { data->push_back(t); }
    void pop_back();
    // 元素访问
    string& front();
    const string& front() const;
    string& back();
    const string& back() const;

    // 提供给 StrBlobPtr 的接口
    // 返回指向首元素和尾后元素的 StrBlobPtr
    StrBlobPtr begin();     // 定义 StrBlobPtr 后才能定义这两个函数
    StrBlobPtr end();
    // 提供给 ConstStrBlobPtr 的接口
    // 返回指向首元素和尾后元素的 ConstStrBlobPtr
    ConstStrBlobPtr cbegin() const;     // 定义 ConstStrBlobPtr 后才能定义这两个函数
    ConstStrBlobPtr cend() const;
    // 练习 14.26
    string &operator[](size_t);
    const string &operator[](size_t) const;

private:
    shared_ptr<vector<string>> data;
    // 如果 data[i] 不合法，抛出一个异常
    void check(size_type i, const string &msg) const;
};
bool operator==(const StrBlob &lhs, const StrBlob &rhs);
bool operator!=(const StrBlob &lhs, const StrBlob &rhs);
bool operator<(const StrBlob &s1, const StrBlob &s2);
bool operator<=(const StrBlob &s1, const StrBlob &s2);
bool operator>(const StrBlob &s1, const StrBlob &s2);
bool operator>=(const StrBlob &s1, const StrBlob &s2);


//===================================================
//
//    StrBlobPtr - custom iterator of StrBlob
//
//===================================================
class StrBlobPtr {
    // 练习 14.16
    friend bool operator==(const StrBlobPtr &lhs, const StrBlobPtr &rhs);
    friend bool operator!=(const StrBlobPtr &lhs, const StrBlobPtr &rhs);
    // 练习 14.18
    friend bool operator<(const StrBlobPtr &s1, const StrBlobPtr &s2);
    friend bool operator<=(const StrBlobPtr &s1, const StrBlobPtr &s2);
    friend bool operator>(const StrBlobPtr &s1, const StrBlobPtr &s2);
    friend bool operator>=(const StrBlobPtr &s1, const StrBlobPtr &s2);

public:
    StrBlobPtr() : curr(0) {}
    StrBlobPtr(StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}

    string& deref() const;
    StrBlobPtr& incr();     // 前缀递增
    StrBlobPtr& decr();     // 后缀递减
    // 练习 14.26
    string &operator[](size_t);
    const string &operator[](size_t) const;
    // 练习 14.27
    StrBlobPtr &operator++();       // 前置运算符
    StrBlobPtr &operator--();
    StrBlobPtr &operator++(int);    // 后置运算符
    StrBlobPtr &operator--(int);
    // 练习 14.28
    StrBlobPtr &operator+=(size_t);
    StrBlobPtr &operator-=(size_t);
    StrBlobPtr operator+(size_t) const;
    StrBlobPtr operator-(size_t) const;
    // 练习 14.30
    string &operator*() const;
    string *operator->() const;

private:
    // 若检查成功，check 返回一个指向 vector 的 shared_ptr
    shared_ptr<vector<string>> check(size_t, const string&) const;
    // 保存一个 weak_ptr，意味着底层 vector 可能会被销毁
    weak_ptr<vector<string>> wptr;
    size_t curr;            // 在数组中的当前位置
};
bool operator==(const StrBlobPtr &lhs, const StrBlobPtr &rhs);
bool operator!=(const StrBlobPtr &lhs, const StrBlobPtr &rhs);
bool operator<(const StrBlobPtr &s1, const StrBlobPtr &s2);
bool operator<=(const StrBlobPtr &s1, const StrBlobPtr &s2);
bool operator>(const StrBlobPtr &s1, const StrBlobPtr &s2);
bool operator>=(const StrBlobPtr &s1, const StrBlobPtr &s2);


//=========================================================
//
//    ConstStrBlobPtr - custom const_iterator of StrBlob
//
//=========================================================
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










//.cpp文件
// #include "StrBlob.h"

/*
 * StrBlob
 */
StrBlob::StrBlob() : data(make_shared<vector<string>>()) { }
StrBlob::StrBlob(initializer_list <string> il) :
        data(make_shared<vector<string>>(il)) { }

void StrBlob::check(vector<string>::size_type i, const string &msg) const {
    if (i >= data->size())
        throw out_of_range(msg);
}

string& StrBlob::front() {
    // 如果 vector 为空，check 会抛出一个异常
    check(0, "front on empty StrBlob");
    return data->front();
}

// const 版本 front
const string& StrBlob::front() const {
    check(0, "front on empty StrBlob");
    return data->front();
}

string& StrBlob::back() {
    check(0, "back on empty StrBlob");
    return data->back();
}

// const 版本 back
const string& StrBlob::back() const {
    check(0, "back on empty StrBlob");
    return data->back();
}

void StrBlob::pop_back() {
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}

bool operator==(const StrBlob &lhs, const StrBlob&rhs) {
    return lhs.data == rhs.data;            // 所指向的 vector 相等
}

bool operator!=(const StrBlob &lhs, const StrBlob&rhs) {
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

string& StrBlob::operator[](size_t n) {
    check(n, "out of range");
    return data->at(n);
}

const string& StrBlob::operator[](size_t n) const {
    check(n, "out of range");
    return data->at(n);
}


/*
 * StrBlobPtr
 */
shared_ptr<vector<string>> StrBlobPtr::check(size_t i, const string &msg) const {
    auto ret = wptr.lock(); // vector 还存在吗？
    if (!ret)
        throw runtime_error("unbound StrBlobPtr");
    if (i >= ret->size())
        throw out_of_range(msg);
    return ret;             // 否则，返回指向 vector 的 shared_ptr
}

string& StrBlobPtr::deref() const {
    auto p = check(curr, "dereference past end");
    return (*p)[curr];      // (*P) 是对象所指向的 vector
}

// 前缀递增：返回递增后的对象的引用
StrBlobPtr& StrBlobPtr::incr() {
    // 如果 curr 已经指向容器的尾后位置，就不能递增它
    check(curr, "increment past end of StrBlobPtr");
    ++curr;                 // 推进当前位置
    return *this;
}

// 前缀递减：返回递减后的对象的引用
StrBlobPtr& StrBlobPtr::decr() {
    // 如果 curr 已经为 0，递减它会产生一个非法下标
    --curr;                 // 递减当前位置
    check(curr, "decrement past begin of StrBlobPtr");
    return *this;
}

// StrBlob 的 begin 和 end 成员的定义
StrBlobPtr StrBlob::begin() {
    return StrBlobPtr(*this);
}
StrBlobPtr StrBlob::end() {
    auto ret = StrBlobPtr(*this, data->size());
    return ret;
}

// StrBlobPtr 的比较操作
bool operator==(const StrBlobPtr &lhs, const StrBlobPtr &rhs) {
    auto l = lhs.wptr.lock(), r = rhs.wptr.lock();
    // 若底层的 vector 是同一个
    if (l == r)
        // 则两个指针都是空，或者指向相同元素时，它们相等
        return (!r || lhs.curr == rhs.curr);
    else
        return false;       // 若指向不同 vector，则不可能相等
}

bool operator!=(const StrBlobPtr &lhs, const StrBlobPtr &rhs) {
    return !(lhs == rhs);
}

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

bool operator>(const StrBlobPtr &s1, const StrBlobPtr &s2) {
    return s2 < s1;
}

bool operator<=(const StrBlobPtr &s1, const StrBlobPtr &s2) {
    return !(s2 < s1);
}

bool operator>=(const StrBlobPtr &s1, const StrBlobPtr &s2) {
    return !(s1 < s2);
}

string& StrBlobPtr::operator[](size_t n) {
    auto p = check(n, "dereference out of range");
    return (*p)[n];
}

const string& StrBlobPtr::operator[](size_t n) const {
    auto p = check(n, "dereference out of range");
    return (*p)[n];
}

// 前置版本：返回递增/递减对象的引用
StrBlobPtr& StrBlobPtr::operator++() {
    // 如果 curr 已经指向了容器的尾后位置，则无法递增它
    check(curr, "increment past end of StrBlobPtr");
    ++curr;         // 将 curr 在当前状态下向前移动一个元素
    return *this;
}
StrBlobPtr& StrBlobPtr::operator--() {
    // 如果 curr 是 0，则继续递减它将产生一个无效下标
    --curr;         // 将 curr 在当前状态下后前移动一个元素
    check(curr, "decrement past begin of StrBlobPtr");
    return *this;
}

// 后置版本：递增/递减对象的值但是返回原值
StrBlobPtr& StrBlobPtr::operator++(int) {
    // 此处无需检查有效性，调用前置递增运算时才需要检查
    StrBlobPtr ret = *this;         // 记录当前的值
    ++*this;        // 向前移动一个元素，前置++需要检查递增的有效性
    return ret;     // 返回之前记录的状态
}
StrBlobPtr& StrBlobPtr::operator--(int) {
    // 此处无需检查有效性，调用前置递减运算时才需要检查
    StrBlobPtr ret = *this;         // 记录当前的值
    ++*this;        // 向后移动一个元素，前置--需要检查递减的有效性
    return ret;     // 返回之前记录的状态
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

string& StrBlobPtr::operator*() const {
    auto p = check(curr, "dereference past end");
    return (*p)[curr];      // (*p) 是对象所指的 vector
}

string* StrBlobPtr::operator->() const {
    // 将实际工作委托给解引用运算符
    return & this->operator*();
}


/*
 * ConstStrBlobPtr
 */
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



#endif //TEST_STRBLOB_H
