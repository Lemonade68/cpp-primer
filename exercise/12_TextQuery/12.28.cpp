//不使用类的版本

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>                  // 要使用 EXIT_FAILURE
#include "./make_plural.h"          // ./当前目录    ../父目录

using namespace std;

using line_no = vector<string>::size_type;
using wmType = map<string, set<line_no>>;
using wmIter = wmType::const_iterator;
using lineIter = wmType::mapped_type::const_iterator;
vector<string> file;                // 文件每行内容
wmType wm;                          // 单词到行号 set 的映射



string cleanup_str(const string &word) {        //全转成小写，并忽略括号的影响
    string ret;
    for (string::const_iterator it = word.begin(); it != word.end(); ++it) {
        if (!ispunct(*it))
            ret += tolower(*it);
    }
    return ret;
}

void input_text(ifstream &is) {
    string text;
    while (getline(is, text)) {     // 对文件中每一行
        file.push_back(text);       // 保存此行文本
        int n = file.size() - 1;    // 当前行号
        istringstream line(text);   // 将行文本分解为单词
        string word;
        while (line >> word) {      // 对行中每个单词
            // 将当前行号插入到其行号 set 中
            // 如果单词不在 wm 中，以之为下标在 wm 中添加一项
            wm[cleanup_str(word)].insert(n);
        }
    }
}

ostream &query_and_print(const string &sought, ostream &os) {
    // 使用 find 而不是下标运算符来查找单词，避免将单词添加到 wm 中！
    wmIter loc = wm.find(cleanup_str(sought));
    if (loc == wm.end()) {          // 未找到
        os << sought << " 出现了 0 次" << endl;
    } else {
        wmType::mapped_type lines = loc->second;   // 行号 set
        os << sought << " 出现了 " << lines.size() << " 次" << endl;
        for (line_no num : lines)
            os << "\t（第 " << num + 1 << " 行）"
               << *(file.begin() + num) << endl;
    }
    return os;
}

void runQueries(ifstream &infile) {
    // infile 是一个 ifstream，指向我们要查询的文件
    input_text(infile);     // 读入文本并建立查询 map
    // 与用户交互：提示用户输入要查询的单词，完成查询并打印结果
    do {
        cout << "enter word to look for, or q to quit: ";
        string s;
        // 若遇到文件尾或用户输入了 q 时循环终止
        if (!(cin >> s) || s == "q")
            break;
        query_and_print(s, cout) << endl;
    } while (true);
}

// 程序接受唯一的命令行参数，表示文本文件名
int main(int argc, char **argv) {
    // 打开要查询的文件
    ifstream infile("../../text/12_testing.txt");
    // 打开文件失败，程序异常退出
    if (!infile) {
        cerr << "No input file!" << endl;
        return EXIT_FAILURE;
    }
    runQueries(infile);
    return 0;
}
