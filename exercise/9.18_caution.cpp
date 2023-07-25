#include<iostream>
#include<string>
#include<deque>

int main(){
    std::string s;
    std::deque<std::string> q;
    while(std::cin >> s){
        q.push_back(s);
    }
    auto b = q.cbegin();

    // 不好的习惯！！！！！    如果涉及添加和删除，则下面的循环会报错 
    auto e = q.cend();                           


    // for (auto b = q.cbegin(), auto e = q.cend(); b != e; ++b)    //错误！
    for (; b != e; ++b)      //注意：for循环中第一个分号前不能声明两个不同类型的元素（指针和引用可以）
        std::cout << *b << std::endl;
    return 0;
}


//或者init-statement中不声明e也可

// for (deque<string>::const_iterator db = sd.cbegin(); db != sd.cend(); ++db)
        // cout << *db << endl;