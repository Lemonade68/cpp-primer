#ifndef TEST_STRBLOB_H
#define TEST_STRBLOB_H

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
class StrBlob {
    friend class StrBlobPtr;
public:
    typedef vector<string>::size_type size_type;
    StrBlob();
    StrBlob(initializer_list<string> il);
    // new add code
    StrBlob(vector<string> *p);
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
    StrBlobPtr begin() const;     // 定义 StrBlobPtr 后才能定义这两个函数
    StrBlobPtr end() const;

private:
    shared_ptr<vector<string>> data;
    // 如果 data[i] 不合法，抛出一个异常
    void check(size_type i, const string &msg) const;
};

inline StrBlob::StrBlob() : data(make_shared<vector<string>>()) { }
inline StrBlob::StrBlob(initializer_list <string> il) :
        data(make_shared<vector<string>>(il)) { }
// new add code
inline StrBlob::StrBlob(vector<string> *p) : data(p) { }

inline void StrBlob::check(vector<string>::size_type i, const string &msg) const {
    if (i >= data->size())
        throw out_of_range(msg);
}

inline string& StrBlob::front() {
    // 如果 vector 为空，check 会抛出一个异常
    check(0, "front on empty StrBlob");
    return data->front();
}

// const 版本 front
inline const string& StrBlob::front() const {
    check(0, "front on empty StrBlob");
    return data->front();
}

inline string& StrBlob::back() {
    check(0, "back on empty StrBlob");
    return data->back();
}

// const 版本 back
inline const string& StrBlob::back() const {
    check(0, "back on empty StrBlob");
    return data->back();
}

inline void StrBlob::pop_back() {
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}


class StrBlobPtr {
    friend bool eq(const StrBlobPtr &, const StrBlobPtr &);

public:
    StrBlobPtr() : curr(0) {}
    StrBlobPtr(StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}
    StrBlobPtr(const StrBlob &a, size_t sz = 0) : wptr(a.data), curr(sz) {}

    string& deref() const;
    // new add code
    string& deref(int off) const;
    StrBlobPtr& incr();     // 前缀递增
    StrBlobPtr& decr();     // 后缀递减
private:
    // 若检查成功，check 返回一个指向 vector 的 shared_ptr
    shared_ptr<vector<string>> check(size_t, const string&) const;
    // 保存一个 weak_ptr，意味着底层 vector 可能会被销毁
    weak_ptr<vector<string>> wptr;
    size_t curr;            // 在数组中的当前位置
};

inline shared_ptr<vector<string>> StrBlobPtr::check(size_t i, const string &msg) const {
    auto ret = wptr.lock(); // vector 还存在吗？
    if (!ret)
        throw runtime_error("unbound StrBlobPtr");
    if (i >= ret->size())
        throw out_of_range(msg);
    return ret;             // 否则，返回指向 vector 的 shared_ptr
}

inline string& StrBlobPtr::deref() const {
    auto p = check(curr, "dereference past end");
    return (*p)[curr];      // (*P) 是对象所指向的 vector
}

// new add code
inline string& StrBlobPtr::deref(int off) const {
    auto p = check(curr + off, "dereference past end");
    return (*p)[curr + off];// (*P) 是对象所指向的 vector
}

// 前缀递增：返回递增后的对象的引用
inline StrBlobPtr& StrBlobPtr::incr() {
    // 如果 curr 已经指向容器的尾后位置，就不能递增它
    check(curr, "increment past end of StrBlobPtr");
    ++curr;                 // 推进当前位置
    return *this;
}

// 前缀递减：返回递减后的对象的引用           这边decre有问题  看main_content里面的
inline StrBlobPtr& StrBlobPtr::decr() {
    // 如果 curr 已经为 0，递减它会产生一个非法下标
    --curr;                 // 递减当前位置
    check(-1, "decrement past begin of StrBlobPtr");
    return *this;
}

// StrBlob 的 begin 和 end 成员的定义
inline StrBlobPtr StrBlob::begin() const {
    return StrBlobPtr(*this);
}
inline StrBlobPtr StrBlob::end() const {
    auto ret = StrBlobPtr(*this, data->size());
    return ret;
}

// StrBlobPtr 的比较操作
inline bool eq(const StrBlobPtr &lhs, const StrBlobPtr &rhs) {
    auto l = lhs.wptr.lock(), r = rhs.wptr.lock();
    // 若底层的 vector 是同一个
    if (l == r)
        // 则两个指针都是空，或者指向相同元素时，它们相等
        return (!r || lhs.curr == rhs.curr);
    else
        return false;       // 若指向不同 vector，则不可能相等
}

inline bool neq(const StrBlobPtr &lhs, const StrBlobPtr &rhs) {
    return !eq(lhs, rhs);
}

#endif //TEST_STRBLOB_H
