#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
#include<sstream>
#include"HasPtr.h"
using namespace std;


// 可能的问题：对于正常的operator=定义方法   不打印sort
// 原因：对于sort而言，只有数据量大于一定值时才会使用快速排序，从而用到swap；
// 否则使用直接插入排序，不使用swap

int main() {
    ifstream in("text.txt");
    if (!in) {
        cerr << "无法打开输入文件" << endl;
        return -1;
    }
    vector<HasPtr> hvec;
    string line, word;
    while (getline(in, line)) {
        istringstream l_in(line);           // 构造字符串流，读取单词
        while (l_in >> word)
            hvec.push_back(word);
    }
    for (auto p : hvec)
        cout << *p << " ";
    cout << endl;
    sort(hvec.begin(), hvec.end());
    for (auto p : hvec)
        cout << *p << " ";
    cout << endl;

    return 0;
}