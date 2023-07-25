#include<iostream>
#include<string>
#include<vector>

int main(){
    std::vector<char> vec{'a', 'b', 'c', 'd'};
    
    //迭代器不是指针！
    // std::string s(vec.cbegin(), 4);

    // data成员函数返回vector容器的首地址
    std::string s(vec.data(), vec.size());
    
    s.push_back('e');       // push_back只支持单元素（字符）
    s.append("fgh");        // append支持单元素与多元素（可以是字符串）
    s.append("i");

    std::cout << s << std::endl;
    return 0;
}