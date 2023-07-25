#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<functional>
#include<iterator>

using namespace std;
using namespace std::placeholders;

int main(){
    // istream_iterator<int> in_iter(cin), eof;
    // vector<int> vec(in_iter, eof);
    istream_iterator<string> it(cin), eof;
    ostream_iterator<string> out(cout, " ");
    
    vector<string> vec(it, eof);

    copy(vec.begin(), vec.end(), out);  //使用copy来输出的简单写法
    cout << endl;

    for(auto e : vec)       //输出时使用ostream_iterator的写法
        *out++ = e;
    cout << endl;

    return 0;
}
