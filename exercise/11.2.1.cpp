#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<utility>       

using namespace std;


//体会添加关键字时为何要find，而添加值时不需要使用find!

void add_family(map<string, vector<pair<string, string>>> &families, const string &family) {
    if (families.find(family) == families.end())
        families[family] = vector<pair<string,string>>();
}

// 上面函数体中更简单的版本如下：借助默认初始化
// families[family];
// 已存在family: 访问而不进行任何操作
// 不存在family: 默认初始化构造值（生成空vector）

void add_child(map<string, vector<pair<string, string>>> &families, const string &family, const string &child, const string &birth) {
    families[family].push_back({child,birth});
}

//更新为11.14要求的版本
void func_7_se(){
    map<string, vector<pair<string, string>>> families;
    add_family(families, "张");
    add_child(families, "张", "强", "2002.12.20");
    add_child(families, "张", "三", "2001.12.20");
    add_child(families, "王", "五", "2003.12.20");
    add_family(families, "王");
    for (auto f : families) {
        cout << f.first << "家的孩子：";
        for (auto c : f.second)
            cout << f.first << c.first << " " << c.second << "; ";
        cout << endl;
    }
}

void func_7(){
    map<string, vector<string>> m = {{"A", {"A1", "A2"}},       // 注意这边vector初始化时要加{} 
                                     {"B", {"B1"}},
                                     {"C", {"C1", "C2", "C3"}}};
    string family,child;
    
    while(cin>>family && family != "Q"){
        if(m.find(family) == m.end()){                          // map成员的find函数, 不然的话下行操作会清空原有数据
            m[family] = vector<string>();                       // 对map[key]成员操作：有就操作，没有就创建新元素
        }
    }
    
    while(cin>>family>>child && family!="Q" && child!="Q"){
        m[family].push_back(child);
    }
    
    for(const auto &s : m){
        cout << "Family: " << s.first << (s.second.size() > 1 ? " Children: " : " Child: ");
        for(const auto &a : s.second)
            cout << a << " ";
        cout << endl;
    }
}


void func_8(){
    //每次添加前使用find来查找下
}


int main(){
    // func_7();
    func_7_se();
    return 0;
}

