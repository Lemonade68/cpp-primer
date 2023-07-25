#include<iostream>
#include<string>
#include<algorithm>

class PrintString{
public:
    PrintString(std::ostream &o = std::cout, char c = ' '): 
        os(o),sep(c){}
    void operator()(const std::string &s) { os << s << sep; }

private:
    std::ostream &os;           //由于不能拷贝流对象，因此是引用
    char sep;                   //分隔符
};


//调用：
// std::string s("hello world");
// PrintString printer;
// printer(s);
// PrintString errors(std::cerr, '\n');
// errors(s);


//常用于作为泛型算法的实参（与lambda相同）
// std::for_each(vs.begin(), vs.end(), PrintString(cerr, '\n'));