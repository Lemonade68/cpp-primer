#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<numeric>

void printstring(const std::vector<std::string> &s){
    for(const auto &a : s)
        std::cout << a << std::endl;
}

bool isShorter(const std::string &s1, const std::string &s2){       //×÷Î½´Ê
    return s1.size() < s2.size();
}


void eliminateDups(std::vector<std::string> &words){
    std::sort(words.begin(), words.end());                          //×ÖµäÐòÅÅÐò
    auto it = std::unique(words.begin(), words.end());              //È¥ÖØ
    words.erase(it, words.end());                                   //É¾³ý
    std::stable_sort(words.begin(), words.end(), isShorter);
}


int main(){
    std::vector<std::string> s
        {"i", "have", "a", "pen", "you", "have", "a", "pen", "too"};
    eliminateDups(s);
    printstring(s);
    return 0;
}