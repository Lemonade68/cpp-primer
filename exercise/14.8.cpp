#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<sstream>
#include<fstream>
#include<functional>
using namespace std;
// using namespace std::placeholders;

//34
struct Test_34{
    int operator()(bool j, int a, int b) { return j ? a : b; }
};


//35
class GetInput{
public:
    GetInput(istream &in = cin):is(in){}
    string operator()()const{
        string str;
        getline(is, str);
        return (is ? str : string());
    }

private:
    istream &is;
};

void func_35(){
    GetInput gt;
    vector<string> vec;
    for (string tmp; !(tmp = gt()).empty(); )
        vec.push_back(tmp);
    for(auto a : vec)
        cout << a << " , ";
    cout << endl;
}




//36
class IsEqual {
    int value;

public:
    IsEqual(int v) : value(v) { }
    bool operator()(int elem) { return elem == value; }
};

void func_36() {
    std::vector<int> ivec = {3, 2, 1, 4, 3, 7, 8, 6};
    std::replace_if(ivec.begin(), ivec.end(), IsEqual(3), 5);
    for (int i : ivec)
        std::cout << i << " ";
    std::cout << std::endl;
}




//38
class LengthComp{
public:
    LengthComp(int a) : i(a){}
    bool operator()(const string &s) { return s.length() == i; }
    //注：length和size没有区别

private:
    int i;
};

void func_38(){
    ifstream in("../text/14_test.txt");
    if(!in){
        cerr << "failed to open file!" << endl;
        return;
    }
    string line, word;
    vector<string> vec;
    while(getline(in, line)){
        istringstream is(line);
        while(is>>word)
            vec.push_back(word);
    }

    for (int i = 1; i <= 10; ++i){
        LengthComp lc(i);
        cout << "len: " << i << " cnt: " << count_if(vec.begin(), vec.end(), lc) << endl;
    }
}




//39
class strlenbetween{
private:    
    int minlen;
    int maxlen;

public:
    strlenbetween(int minLen, int maxLen):minlen(minLen),maxlen(maxLen){}
    bool operator()(const string &s) { return s.length() >= minlen && s.length() <= maxlen; }
};

class strlenbiggerthan{
private:    
    int minlen;

public:
    strlenbiggerthan(int minLen):minlen(minLen){}
    bool operator()(const string &s) { return s.length() >= minlen; }
};


void func_39(){
    ifstream in("../text/14_test.txt");
    if(!in){
        cerr << "failed to open file!" << endl;
        return;
    }
    string line, word;
    vector<string> vec;
    while(getline(in, line)){
        istringstream is(line);
        while(is>>word)
            vec.push_back(word);
    }

    strlenbetween sb(1, 9);
    strlenbiggerthan sbig(10);
    cout << "1-9: " << count_if(vec.begin(), vec.end(), sb) << endl;
    cout << ">=10: " << count_if(vec.begin(), vec.end(), sbig) << endl;
}




//42
void func_42(){
    using std::placeholders::_1;

    //统计大于1024的值有多少
    vector<int> ivec{1000, 2000, 3000, 4000, 5000};
    cout << count_if(ivec.begin(), ivec.end(), bind(greater<int>(), _1, 1024)) << endl;

    //找到第一个不等于 pooth 的字符串
    vector<string> svec{"pooth", "pooth", "abc", "pooth"};
    cout << count_if(svec.begin(), svec.end(), bind(not_equal_to<string>(), _1, "pooth")) << endl;

    // 将所有的值乘以 2
    std::transform(ivec.begin(), ivec.end(), ivec.begin(), std::bind(std::multiplies<int>(), _1, 2));
    for (const auto &i : ivec)
        std::cout << i << " ";
    std::cout << std::endl;
}




//43
void func_43(){
    int n;
    std::cin >> n;
    std::modulus<int> mod;
    std::vector<int> ivec{1, 2, 4, 5, 10};
    auto predicator = [&] (int i) { return 0 == mod(n, i); };       // 隐式引用捕获使用到的函数体中的实体
    auto is_divisible = std::all_of(ivec.begin(), ivec.end(), predicator);
    std::cout << (is_divisible ? "Yes" : "No");
    std::cout << std::endl;
}


int main(){
    // func_38();
    // func_39();
    // func_42();
    func_43();
    return 0;
}