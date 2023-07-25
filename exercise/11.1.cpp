#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<iterator>
#include<cctype>
#include<map>
#include<set>

using namespace std;


string &trans(string &s) {                      //string的函数还可以用下标计算
    for (int p = 0; p < s.size(); ++p) {
        if (s[p] >= 'A' && s[p] <= 'Z')
            s[p] += ('a' - 'A');
        else if (s[p] == ',' || s[p] == '.')
            s.erase(p, 1);                      // string.erase(pos,len)
    }
    return s;
}


void func_3(){
    map<string, size_t> word_count;
    set<string> exclude{"The", "But", "And", "Or", "An", "A",
                        "the", "but", "and", "or", "an", "a"};
    string word;
    while(cin>>word){
        // if(find(exclude.cbegin(),exclude.cend(),word) == exclude.cend())     //关联容器尽量别使用泛型算法，太慢
        if(exclude.find(word) == exclude.end())     //使用set的find成员函数
            ++word_count[word];
    }
    for(const auto &s:word_count)
        cout << s.first << " occurs " << s.second << (s.second > 1 ? " times" : " time") << endl;
}


void func_4(){
    map<string, size_t> word_count;
    set<string> exclude{"The", "But", "And", "Or", "An", "A",
                        "the", "but", "and", "or", "an", "a"};
    string word;
    while(cin>>word){
        // tolower前一定要加 ::  强调是c版本的tolower，不加的话编译器无法区分用std的还是c的
        // std::toupper接受的是int类型的一个值，而char可以转成int, string不行！
        transform(word.begin(), word.end(), word.begin(), ::tolower);   //::toupper不加():这是谓词！
        if(*word.rbegin() == '.' || *word.rbegin() == ',')              //注意string迭代器解引用返回的是char
            word.erase(word.end() - 1, word.end());                     //erase不接受反向迭代器作为参数
        
        if(exclude.find(word) == exclude.end())     
            ++word_count[word];
    }
    for(const auto &s : word_count)
        cout << s.first << " occurs " << s.second << (s.second > 1 ? " times" : " time") << endl;
}


void func_4_another_method() {
    // 统计每个单词在输入中出现的次数
    map<string, size_t> word_count;     // string 到 size_t 的空 map
    string word;
    while (cin >> word)
        ++word_count[trans(word)];      // 提取 word 的计数器并将其加 1
    for (const auto &w : word_count)    // 对 map 中的每个元素
        cout << w.first << " occurs " << w.second << ((w.second > 1) ? " times" : " time") << endl;
}


// 使用关联容器insert成员实现
void func_20(){
    string word;
    map<string, size_t> word_count;
    while(cin>>word){
        auto par = word_count.insert({trans(word), 1});
        if(!par.second)
            ++par.first->second;
        // ++word_count.insert({trans(word), 0}).first->second;        //上三行另一种写法：
    }
    for(const auto &s : word_count)
    cout << s.first << " occurs " << s.second << (s.second > 1 ? " times" : " time") << endl;
}


int main(){
    // func_3();
    // func_4();
    func_20();
    return 0;
}
