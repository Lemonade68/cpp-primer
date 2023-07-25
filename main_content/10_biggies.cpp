//将单词序列去重后按字数稳定排序，最后打印出长度大于输入长度的单词个数
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<functional>                    //bind函数与placeholders命名空间

using namespace std;
using namespace std::placeholders;      //使用占位符

void prt(const vector<string> &s){
    for(const auto &a : s)
        cout << a << " ";
    cout << endl;
}

void elimDups(vector<string> &s){
    sort(s.begin(), s.end());
    auto it = unique(s.begin(), s.end());
    s.erase(it, s.end());
}

string make_plural(int count, const string &s1, const string &s2){
    return count > 1 ? s1 + s2 : s1;
}

bool check_size(const string &s, string::size_type sz){
    return s.size() >= sz;
}


// find_if版本，找到第一个谓词返回true的（只接受一元谓词）
void biggies_findif(vector<string> &words, vector<string>::size_type sz){
    elimDups(words);
    prt(words);
    stable_sort(words.begin(), words.end(),
                [](const string &s1, const string &s2)
                { return s1.size() < s2.size(); });
    prt(words);

    //计算数目
    
    //使用lambda版本
    // auto it = find_if(words.begin(), words.end(), 
                    // [sz](const string &s)
                    //   { return s.size() >= sz; });
    
    //使用bind函数与check_size函数
    auto it = find_if(words.begin(), words.end(),
                      bind(check_size, _1, sz));


    auto count = words.end() - it;
    cout << count << " " << make_plural(count, "word", "s") << " of length " << sz << " or longer" << endl;

    //使用for_each函数打印对应单词
    for_each(it, words.end(), 
            [](const string &s)
             { cout << s << " "; });
    cout << endl;
}


// partition版本，前后划分
void biggies_partition(vector<string> &words, vector<string>::size_type sz){
    elimDups(words);
    prt(words);

    // partition不需要stable_sort！

    //计算数目    用partition也可以！
    auto it = stable_partition(words.begin(), words.end(), 
                    [sz](const string &s)            
                     { return s.size() >= sz; });
    auto count = it - words.begin();
    cout << count << " " << make_plural(count, "word", "s") << " of length " << sz << " or longer" << endl;

    //打印对应单词
    for_each(words.begin(), it, 
            [](const string &s)
             { cout << s << " "; });
    cout << endl;   

}

void func_24(const vector<string> &s, const string &str){
    auto it = find_if(s.begin(), s.end(), bind(check_size, _1, str.size()));
    cout << *it << endl;
}

int main(){
    vector<string> vec{"the", "quick", "red", "fox", "jumps", "over", "the", "slow", "red", "turtle"};
    biggies_findif(vec, 5);
    cout << "-----------------------------------" << endl;
    biggies_partition(vec, 4);
    return 0;
}   