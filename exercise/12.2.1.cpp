#include<iostream>
#include<string>
#include<memory>
#include<cstring>
#define n 100

using namespace std;


void func_23(){
    const char *s1 = "Hello ";
    const char *s2 = "World!";
    char *c = new char[strlen(s1) + strlen(s2) + 1];    //+1是预留给\0结束符的
    strcpy(c, s1);
    strcat(c, s2);
    cout << c << endl;

    string st1{"Hello "};
    string st2{"World!"};
    strcpy(c, (st1 + st2).c_str());         //s1+s2转换成c风格字符串
    cout << c << endl;

    delete[] c;
}

void func_24(){
    size_t size;
    cout << "plz input the size:";
    cin >> size;
    cin.ignore();                           //忽略输入size后的一个字符（空格或回车），用cin.get()也可以
    // cin.get();
    char *r = new char[size + 1]();         //值初始化
    cin.get(r, size + 1);                   //cin.get传入的是char数组
    cout << "The input string is:\n" << r << endl;

    delete[] r;
}

void func_26(){
    // int n = 100;
    allocator<string> alloc;
    auto const p = alloc.allocate(n);       //指定首地址
    string s;
    string *q = p;                          //或者 auto q = p;                    
    while(cin>>s && q != p + n){
        alloc.construct(q++, s);            //注意是后置递增
    }
    const size_t size = q - p;
    for (size_t i = 0; i < size; ++i)
        cout << p[i] << endl;
        
    while(q!=p)
        alloc.destroy(--q);                 //注意先递减

    alloc.deallocate(p, n);
}


int main(){
    // func_23();
    // func_24();
    func_26();
    return 0;
}