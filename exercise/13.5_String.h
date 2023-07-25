#ifndef TEST_STRING_H
#define TEST_STRING_H

//重点：对string使用string类的操作，对const char*使用strlen这类的操作

//.h文件

#include <iostream>
using std::ostream;

#include <memory>
using std::allocator;
using std::uninitialized_copy;
using std::uninitialized_fill_n;

#include<algorithm>
using std::equal;
using std::lexicographical_compare;

class String {
    friend String operator+(const String&, const String&);
    friend String add(const String&, const String&);
    friend ostream &operator<<(ostream&, const String&);
    friend ostream &print(ostream&, const String&);
    
    friend bool operator==(const String &, const String &);
    friend bool operator!=(const String &, const String &);
    friend bool operator<(const String &s1, const String &s2);
    friend bool operator<=(const String &s1, const String &s2);
    friend bool operator>(const String &s1, const String &s2);
    friend bool operator>=(const String &s1, const String &s2);

public:
    String() : sz(0), p(0) { }
    // cp points to null terminated array, allocate new
    // memory & copy the array
    String(const char *cp) : sz(strlen(cp)), p(a.allocate(sz)) {
        uninitialized_copy(cp, cp + sz, p);
    }
    String(size_t n, char c) : sz(n), p(a.allocate(n)) {
        uninitialized_fill_n(p, sz, c);
    }

    // copy constructor: allocate a new copy of the characters in s
    String(const String &s) : sz(s.sz), p(a.allocate(s.sz)) {
        uninitialized_copy(s.p, s.p + sz, p);
    }

    String(String &&s)noexcept:sz(s.sz),p(s.p){
        s.sz = 0;
        s.p = nullptr;
    }

    String &operator=(String &&) noexcept;

    // allocate a new copy of the data in the right-hand operand;
    // deletes the memory used by the left-hand operand
    String &operator=(const String&);

    // unconditionally delete the memory because each String has
    // its own memory
    ~String() { free(); }

public:
    // additional assignment operators
    String &operator=(const char*);         // car = "Studebaker"
    String &operator=(char);                // model = 'T'

    const char *begin() { return p; }
    const char *begin() const { return p; }
    const char *end() { return p + sz; }
    const char *end() const { return p + sz; }

    size_t size() const { return sz; }
    void swap(String&);

    // 练习 14.26
    char &operator[](size_t n) { return p[n]; }
    const char &operator[](size_t n) const { return p[n]; }

private:
    static allocator<char> a;
    size_t sz;
    char *p;

    void free();
};

String make_plural(size_t ctr, const String&, const String&);

inline
void swap(String &s1, String &s2) {
    s1.swap(s2);
}
//还要声明一下
String operator+(const String&, const String&);
String add(const String&, const String&);
ostream &operator<<(ostream&, const String&);
ostream &print(ostream&, const String&);
bool operator==(const String &, const String &);
bool operator!=(const String &, const String &);
bool operator<(const String &s1, const String &s2);
bool operator<=(const String &s1, const String &s2);
bool operator>(const String &s1, const String &s2);
bool operator>=(const String &s1, const String &s2);

//.cpp文件

#include <algorithm>
using std::for_each;

// define the static allocator member，静态成员变量要在cpp中定义
allocator<char> String::a;

void String::free() {
    if (p) {
        //this传入的是指针，因此修改的是原来的对象
        for_each(p, p + sz, [this] (char &c) { a.destroy(&c); });       
        a.deallocate(p, sz);
    }
}

void String::swap(String &s) {
    // using std::swap;
    // swap(p, s.p);
    // swap(sz, s.sz);

    char *tmp = p;
    p = s.p;
    s.p = tmp;

    size_t cnt = sz;
    sz = s.sz;
    s.sz = cnt;
}

// copy-assignment operator
String& String::operator=(const String &rhs) {
    // copying the right-hand operand before deleting
    // the left handles self-assignment
    char *newp = a.allocate(rhs.sz);    // copy the underlying string from rhs
    uninitialized_copy(rhs.p, rhs.p + rhs.sz, newp);
    free();         // free the memory used by the left-hand operand
    p = newp;       // p now points to the newly allocated string
    sz = rhs.sz;    // update the size
    return *this;
}

String& String::operator=(const char *cp) {
    free();         // free the memory used by the left-hand operand
    p = a.allocate(sz = strlen(cp));
    uninitialized_copy(cp, cp + sz, p);
    return *this;
}

String& String::operator=(char c) {
    free();         // free the memory used by the left-hand operand
    p = a.allocate(sz = 1);
    *p = c;
    return *this;
}

// named functions for operators
ostream &print(ostream &os, const String &s) {
    const char *p = s.begin();
    while (p != s.end())
        os << *p++;
    return os;
}

String add(const String &lhs, const String &rhs) {
    String ret;
    ret.sz = rhs.size() + lhs.size();       // size of the combined String
    ret.p = String::a.allocate(ret.sz);     // allocate new space
    uninitialized_copy(lhs.begin(), lhs.end(), ret.p);  // copy the operands
    uninitialized_copy(rhs.begin(), rhs.end(), ret.p + lhs.sz);
    return ret;
}

// return plural version of word if ctr isn't 1
String make_plural(size_t ctr, const String &word, const String &ending) {
    return (ctr != 1) ? add(word, ending) : word;
}

// chapter 14 will explain overloaded operators
ostream &operator<<(ostream &os, const String &s) {
    return print(os, s);
}

String operator+(const String &lhs, const String &rhs) {
    return add(lhs, rhs);
}

String& String::operator=(String &&rhs)noexcept{
    if(this != &rhs){
        free();
        p = rhs.p;
        sz = rhs.sz;

        rhs.p = nullptr;
        rhs.sz = 0;
    }
    return *this;
}

// bool operator==(const String &lhs, const String &rhs){
//     if(lhs.size() != rhs.size())
//         return false;

//     for (auto it1 = lhs.begin(), it2 = rhs.begin(); it1 != lhs.end(), it2 != rhs.end(); ++it1, ++it2)
//         if(*it1 != *it2)
//             return false;

//     return true;
// }


//学习这种写法
bool operator==(const String &lhs, const String &rhs){
    return (lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin()));
}

bool operator!=(const String &lhs, const String &rhs){
    return !(lhs == rhs);
}

// lexicographical_compare : 字典序比较两个序列（前一区间是否小于后一区间）
bool operator<(const String &s1, const String &s2) {
    return lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end());
}

bool operator>(const String &s1, const String &s2) {
    return s2 < s1;
}

bool operator<=(const String &s1, const String &s2) {
    return !(s2 < s1);
}

bool operator>=(const String &s1, const String &s2) {
    return !(s1 < s2);
}

#endif //TEST_STRING_H
