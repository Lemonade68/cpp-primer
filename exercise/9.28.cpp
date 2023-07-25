#include<iostream>
#include<string>
#include<forward_list>
#include<vector>
using namespace std;

void func(forward_list<string> &l, const string &s1, const string &s2){
    auto pre = l.before_begin();
    auto cur = l.begin();
    while(cur != l.end()){
        if(*cur == s1){
            cur = l.insert_after(cur, s2);
            ++pre;
            return;
        }
        else{
            ++cur;
            ++pre;
        }

    }
    cur = l.insert_after(pre, s2);          // 这里传的是pre, cur已经是end了（最后元素的后一个）
}

int main(){
    vector<string> vec;
    string s;
    cout << "输入一串单词：" << endl;
    while(cin>>s && s!="Q")
        vec.push_back(s);
    forward_list<string> l(vec.cbegin(),vec.cend());    // 不用列表初始化的方式 
    string s1("good"),s2("XDU");
    cout << "输入两个单词s1,s2:" << endl;            // 为什么上面不用Q，输入ctrl+z时  不会给输入就直接出结果？
    cin >> s1 >> s2;
    func(l, s1, s2);
    for(auto a: l)
        cout << a << " ";
    cout << endl;
    return 0;
}