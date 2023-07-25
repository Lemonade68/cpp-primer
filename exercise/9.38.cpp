#include <iostream>
#include <vector>

using namespace std;

void func_9_3_8() {
    vector<int> ivec;
    // size 应该为 0；capacity 的值依赖于具体实现
    cout << "ivec: size: " << ivec.size()
         << " capacity: " << ivec.capacity() << endl;
    // ivec: size: 0 capacity: 0

    // 向 ivec 添加 24 个元素
    for (vector<int>::size_type ix = 0; ix != 24; ++ix)
        ivec.push_back(ix);
    // size 应该为 24；capacity 应该大于等于 24，具体值依赖于标准库实现
    cout << "ivec: size: " << ivec.size()
         << " capacity: " << ivec.capacity() << endl;
    // ivec: size: 24 capacity: 32

    ivec.reserve(50);       // 将 capacity 至少设定为 50，可能会更大
    // size 应该为 24；capacity 应该大于等于 50，具体值依赖于标准库实现
    cout << "ivec: size: " << ivec.size()
         << " capacity: " << ivec.capacity() << endl;
    // ivec: size: 24 capacity: 50

    // 添加元素用光多余容量
    while (ivec.size() != ivec.capacity())
        ivec.push_back(0);
    // capacity 应该未改变，size 和 capacity 相等
    cout << "ivec: size: " << ivec.size()
         << " capacity: " << ivec.capacity() << endl;
    // ivec: size: 50 capacity: 50

    ivec.push_back(42);     // 在添加一个元素
    // size 应该为 51；capacity 应该大于等于 51，具体值依赖于标准库实现
    cout << "ivec: size: " << ivec.size()
         << " capacity: " << ivec.capacity() << endl;
    // ivec: size: 51 capacity: 100

    ivec.shrink_to_fit();   // 要求归还内存
    // size 应该未改变（51）；capacity 的值依赖于具体实现
    cout << "ivec: size: " << ivec.size()
         << " capacity: " << ivec.capacity() << endl;
    // ivec: size: 51 capacity: 51
}


void func_9_3_9() {
    vector<string> svec;
    svec.reserve(1024);
    // size 应该为 0；capacity 的值依赖于具体实现
    cout << "svec: size: " << svec.size()
         << " capacity: " << svec.capacity() << endl;

    // 读入 256 个词
    int count = 0;
    while (count != 1024) {
        svec.push_back("C++");
        ++count;
    }
    // capacity 应该未改变，size 和 capacity 相等
    cout << "svec: size: " << svec.size()
         << " capacity: " << svec.capacity() << endl;
    
    // resize 向 svec 末尾添加当前字符串数量一半那么多
    // 的新字符串，它们的值都是空串。若空间不够，会分配
    // 足够容纳这些新字符串的内存空间
    svec.resize(svec.size() + svec.size() / 2);
    // capacity 应该未改变，size 和 capacity 相等
    cout << "svec: size: " << svec.size()
         << " capacity: " << svec.capacity() << endl;

}


int main(){
    func_9_3_8();
    cout << "-----------------------------" << endl;
    func_9_3_9();
    return 0;
}
