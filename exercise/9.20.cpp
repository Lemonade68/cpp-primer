#include<iostream>
#include<list>
#include<deque>

int main(){
    std::list<int> l{1, 2, 3, 4, 5, 6, 7};
    std::deque<int> odd, even;
    for (auto b = l.cbegin(); b != l.cend(); ++b){
        if(*b % 2 == 0)
            even.push_back(*b);
        else
            odd.push_back(*b);
    }
    for (const auto a : even){
        std::cout << a << std::endl;
    }
    for (const auto a : odd){
        std::cout << a << std::endl;
    }
    return 0;
}



// 判断奇数偶数另一个方法：对最后一位使用位运算
// if (*lb & 1)        // 查看最低位，1：奇数；0：偶数
//     odd_d.push_back(*lb);
// else
//     even_d.push_back(*lb);