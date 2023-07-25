#include<iostream>
#include<memory>
#include<string>
#include<vector>

template<typename T> class Blob{
public:
    typedef T value_type;
    //注意这边typedef后跟typename的用法，表示让编译器知道vector<T>的size_type是一种类型
    //这里的vector<T>还是一种未知的类型
    typedef typename std::vector<T>::size_type size_type;
    // typedef std::vector<T>::size_type size_type;

    //构造函数
    Blob();
    Blob(std::initializer_list<T> il);
    //各项操作
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    void push_back(const T &t) { data->push_back(t); }
    void push_back(T &&t) { data->push_back(std::move(t)); }    //移动版本，注意使用move来返回右值引用
    void pop_back();
    //元素访问
    T &back();
    T &operator[](size_type i);

private:
    std::shared_ptr<std::vector<T>> data;
    //若data[i]无效，则抛出msg
    void check(size_type i, const std::string &msg) const;
};



//使用样例：
// Blob<int> ia;