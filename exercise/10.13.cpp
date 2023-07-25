#include<iostream>
#include<string>
#include<algorithm>
#include<vector>

bool func(const std::string &s){
    return s.size() >= 5;
}

int main(){
    std::vector<std::string> words
        {"i", "have", "a", "pencil", "yourfather", "have", "a", "pencil", "as well"};
    auto it = std::partition(words.begin(), words.end(), func);
    auto beg = words.begin();
    for (; beg != it;++beg){
        std::cout << *beg << std::endl;
        ++beg;
    }
    return 0;
}