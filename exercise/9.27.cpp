#include<iostream>
#include<forward_list>

int main(){
    std::forward_list<int> l{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto pre = l.before_begin();
    auto cur = l.begin();
    while(cur != l.end()){
        if(*cur & 1)        //Œª‘ÀÀ„
            cur = l.erase_after(pre);
        else{
            ++cur;
            ++pre;
        }
    }
    for(auto a:l)
        std::cout << a << " ";
    std::cout << std::endl;
    return 0;
}