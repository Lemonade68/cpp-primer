#include<iostream>
#include<memory>

using namespace std;

struct connection{ };
struct destination{ };
connection connect(destination *pd){
    cout << "connection begins" << endl;
    return connection();
}
void disconnect(connection c){
    cout << "connection ends" << endl;
}

void f_new(destination &d){
    cout << "manage connect directly" << endl;
    connection c = connect(&d);
    //假设这里忘记调用disconnect来关闭连接
    cout << "f_new finished" << endl;
}

void end_connection(connection *p){
    disconnect(*p);
}

void f_s_p(destination &d){
    cout << "manage connect with shared_ptr" << endl;
    connection c = connect(&d);
    shared_ptr<connection> p(&c, end_connection);       //注意这边的声明格式   1 取址  2 deleter
    cout << "f_shared_ptr finished" << endl;
}

void f_sp_15(destination &d){
    cout << "manage connect with shared_ptr" << endl;
    connection c = connect(&d);
    shared_ptr<connection> p(&c, [](connection *p)      
                             { end_connection(p); });   //使用lambda版本
    cout << "f_shared_ptr finished" << endl;
}

void func_14(){
    destination d;
    f_new(d);
    cout << endl;
    f_s_p(d);
}

void func_15(){
    destination d;
    f_new(d);
    cout << endl;
    f_sp_15(d);
}

int main(){
    // func_14();
    func_15();
    return 0;
}