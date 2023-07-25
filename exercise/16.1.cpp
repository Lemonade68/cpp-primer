#include<iostream>
#include<vector>
using namespace std;

//练习16.4 编写行为类似find算法的函数模板
template<typename I, typename T>
I Find(I b, I e, T n){      //对应begin,end,要查找的值
    for (; b != e; ++b){
        if(*b == n)
            return b;
    }
    return e;
}

//16.5 可以处理任何类型与任何长度的数组
template<typename T, size_t N>
void print(const T (&arr)[N]){  // N自动获取传入的arr的长度
    for(auto elem : arr)
        cout << elem << endl;
}

//16.6 定义begin和end
template <typename T, size_t N>
const T *begin(const T (&arr)[N]) {
    return arr;
}

template <typename T, size_t N>
const T *end(const T (&arr)[N]) {
    return arr + N;
}

//16.7
template <typename T, size_t N>
constexpr int SizeOfArray(const T (&arr)[N]) {
    return N;
}




int main(){
    //16.4
    vector<int> v1{1, 2, 3, 4, 5};
    auto it = Find(v1.begin(), v1.end(), 6);
    if(it == v1.end())
        cout << "failed to find" << endl;
    else
        cout << *it << endl;

    //16.5
    int arr[5] = {1, 2, 3, 4, 5};
    print(arr);

    //
    return 0;
}