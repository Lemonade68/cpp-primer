#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<iterator>
#include<list>

using namespace std;

void func_34(){
    vector<int> vec{1, 2, 3, 4, 5, 6};
    for (auto rbeg = vec.crbegin(); rbeg != vec.crend();++rbeg)
        cout << *rbeg << " ";
    cout << endl;
}

void func_35(){
    vector<int> vec{1, 2, 3, 4, 5, 6};
    for (auto end = vec.cend(); end != vec.cbegin(); )
        cout << *(--end) << " ";
    cout << endl;
}

void func_36(){
    // list不支持迭代器相减求距离运算，因此使用count来计数
    list<int> l{1, 2, 3, 0, 4, 0, 5, 0};
    auto pos = find(l.crbegin(), l.crend(), 0);
    ++pos;          //++后使得pos.base()返回与原pos相同位置的迭代器
    int count = 1;
    for (auto beg = l.begin(); beg != pos.base();++beg,++count);
    cout << "最后一个0在第" << count << "个位置" << endl;
}

void func_37(){
    vector<int> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    list<int> l;
    copy(vec.rbegin() + 3, vec.rend() - 2, back_inserter(l));   //注意要用back_inserter!!
    ostream_iterator<int> out(cout, " ");
    copy(l.begin(), l.end(), out);
}

int main(){
    // func_34();
    // func_35();
    // func_36();
    func_37();
    return 0;
}

