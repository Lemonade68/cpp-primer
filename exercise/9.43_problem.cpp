#include <iostream>
#include <vector>
#include <string>

using namespace std;

void replace_string(string &s, const string &oldVal, const string &newVal) {
    // 这边最后也可以用oldVal.size()
    // std::distance用于求出两个迭代器间的距离
    for (string::iterator beg = s.begin(); distance(beg, s.end()) >= distance(oldVal.begin(), oldVal.end()); ) {
        if (string{beg, beg + oldVal.size()} == oldVal) {
            beg = s.erase(beg, beg + oldVal.size());
            beg = s.insert(beg, newVal.cbegin(), newVal.cend());

            // 用这一句就会抛出错误????
            // s.replace(beg, beg + oldVal.size(), newVal);    // 该句也可用上边两句代替
            
            advance(beg, newVal.size());            // std::advance  将迭代器前移newval.size()个单位
        } else
            ++beg;
    }
}

int main() {
    string s = "tho thru tho!";
    replace_string(s, "thru", "through");
    cout << s << endl;

    replace_string(s, "tho", "though");
    cout << s << endl;

    replace_string(s, "through", "");
    cout << s << endl;

    return 0;
}
