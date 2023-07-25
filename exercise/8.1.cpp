#include<iostream>
#include<fstream>
#include<string>

std::istream &func(std::istream &is){
    std::string buf;
    while(is >> buf){
        std::cout << buf << std::endl;
    }
    is.clear();
    return is;
}


int main(){
    // std::cout << "输入数据  直到ctrl+z为止" << std::endl;
    // func(std::cin);
    std::ifstream in;
    std::string ifile("../text/8_testing.txt");        //相对路径
    in.open(ifile);
    if(in)
        std::cout << "succ" << std::endl;
    else
        std::cout << "fail" << std::endl;
    return 0;
}