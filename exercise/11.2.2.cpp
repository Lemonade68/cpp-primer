#include<iostream>
#include<sstream>           //sstream头文件里包含string头文件，因此没定义也没事
#include<fstream>
#include<list>
// #include<algorithm>      //string的erase函数定义在string类里
#include<iterator>
#include<map>
// #include<cctype>         //有了这个后，std::toupper才有效（前面std不省略时）

using namespace std;

void trans(string &s){                      //要修改，不传const
    for (int p = 0; p < s.size(); ++p){     //对string中每一个字符操作
        if(s[p] >= 'A' && s[p] <= 'Z')
            s[p] = tolower(s[p]);           //这里使用的是ctype.h的tolower（iostream里好像包含）
        else if(s[p]=='.' || s[p] ==',' || s[p]=='!')
            s.erase(p, 1);
    }
}

void func_9(){
    map<string, list<int>> mp;
    int line_ctr = 0;                       //表示行数
    string line,word;
    
    ifstream in("../text/11_words.txt");
    if(!in){
        cerr << "Failed to open file!" << endl;
        return;
    }

    while(getline(in, line)){
        ++line_ctr;
        istringstream is(line);             //对一行的string处理：使用istringstream
        while(is >> word){
            trans(word);
            mp[word].push_back(line_ctr);   //会默认初始化一个空list
        }
    }

    ostream_iterator<int> out(cout, " ");
    for(const auto &a : mp){                //是按字典序打印
        cout << "word: " << a.first << (a.second.size() > 1 ? " 's lines are: " : " 's line is: ");
        copy(a.second.begin(), a.second.end(), out);
        cout << endl;
    }
}

int main(){
    func_9();
    return 0;
}