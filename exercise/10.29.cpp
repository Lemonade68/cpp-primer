#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iterator>
#include<algorithm>

using namespace std;

void func_29(){
    ifstream input("../8_testing.txt");
    if(!input){
        cerr << "无法打开文件！" << endl;
        return;
    }
    istream_iterator<string> it(input), eof;
    ostream_iterator<string> out_iter(cout, " ");
    vector<string> vec(it, eof);
    copy(vec.begin(), vec.end(), out_iter);
    cout << endl;
}

void func_30(){
    ifstream input("../8_testing.txt");
    if(!input){
        cerr << "无法打开文件！" << endl;
        return;
    }
    istream_iterator<int> it(input), eof;
    ostream_iterator<int> out(cout, " ");
    vector<int> vec(it, eof);
    sort(vec.begin(), vec.end());
    
    copy(vec.begin(), vec.end(), out);              // 30
    cout << endl;
    unique_copy(vec.begin(), vec.end(), out);       // 31
    cout << endl;
}

void func_33(){
    string s1("../text/8_testing.txt"), s_odd("../text/10_odd.txt"), s_even("../text/10_even.txt");
    ifstream in(s1);
    ofstream out_odd(s_odd), out_even(s_even,ofstream::app);    //前面为截断，后面为尾部增添
    if(!in){
        cerr << "failed to open files!" << endl;
        return;
    }
    istream_iterator<int> it(in), eof;
    ostream_iterator<int> out1(out_odd, " "), out2(out_even, " ");
    vector<int> vec(it, eof);
    
    copy_if(vec.begin(), vec.end(), out1, [](const int &a)
            { return !(a % 2); });
    out_odd << endl;                                            //观察奇数部分，目前已运行3次，每次在尾部重写
    
    copy_if(vec.begin(), vec.end(), out2, [](const int &a)
            { return a % 2; });
    out_even << endl;                                           //观察偶数部分，每次都覆写
    
    cout << "Done!" << endl;
}


int main(){
    // func_29();
    // func_30();
    func_33();
    return 0;
}