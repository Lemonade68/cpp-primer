#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<utility>

using namespace std;

void func_12(){
    vector<pair<string, int>> vec;
    string word;
    int num;
    while(cin>>word>>num){
        vec.push_back({word, num});
        // vec.push_back(pair<string,int>(word,num));
        // vec.push_back(make_pair(word,num));
    }
    for(const auto &a : vec)
        cout << a.first << " : " << a.second << endl;
}

int main(){
    func_12();
    return 0;
}

