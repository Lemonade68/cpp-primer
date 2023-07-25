// 面向对象版本的见opp文件夹

#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<memory>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::istringstream;
using std::map;
using std::ostream;
using std::set;
using std::shared_ptr;
using std::string;
using std::vector;


class QueryResult;

class TextQuery{
public:
    using line_no = vector<string>::size_type;          //类型别名，表示行号，即vector中的下标
    TextQuery() = default;
    TextQuery(ifstream &);                              //读写IO类对象会改变其状态，因此不可以使用const来修饰 
    QueryResult query(const string &) const;

private:
    shared_ptr<vector<string>> file;                    //输入文件
    map< string, shared_ptr<set<line_no>> > wm;         //word_map
};

TextQuery::TextQuery(ifstream &is): file(new vector<string>) {
    string text;
    while(getline(is,text)){
        file->push_back(text);
        int n = file->size() - 1;                       //当前行号
        istringstream line(text);
        string word;
        while(line>>word){
            auto &lines = wm[word];                     //使用lines作为别称
            if(!lines)                                  //第一次遇见word时，wm[word]值初始化为空指针
                lines.reset(new set<line_no>);
            lines->insert(n);                           //因为是set，所以如果一个单词多次出现在同一行，insert不会重复插入
        }
    }
}



class QueryResult{                                  //一个单词对应的所有查询结果类
    friend ostream &print(ostream &, const QueryResult &);
public:
    QueryResult(string s, shared_ptr<set<TextQuery::line_no>> p, shared_ptr<vector<string>> f):
        sought(s),lines(p),file(f) { }              //体会这边复制智能指针的妙处

private: 
    string sought;   //查询单词
    shared_ptr<set<TextQuery::line_no>> lines;      //出现的行号set
    shared_ptr<vector<string>> file;                //输入文件对应文本
};


QueryResult TextQuery::query(const string &sought)const{            //用sought来在map中定义行号set
    static shared_ptr<set<line_no>> nodata(new set<line_no>);       //对应找不到到的单词
    //使用find查找单词：避免将单词添加到wm中
    auto loc = wm.find(sought);
    if(loc == wm.end())
        return QueryResult(sought, nodata, file);           //未找到的情况，返回一个空的行号set的shared_ptr
    else
        return QueryResult(sought, loc->second, file);      //找到，返回wm的pair中的shared_ptr
}


inline string make_plural(size_t ctr, const string &word, const string &ending){
    return ctr > 1 ? word + ending : word;
}

ostream &print(ostream &os, const QueryResult &qr){
    //这边用匿名函数返回"word"为什么不行？
    os << endl;
    os << qr.sought << " occurs " << qr.lines->size() 
        << make_plural(qr.lines->size()," time","s") << endl;
    for(auto num : *qr.lines)                               //这边qr.lines是一个shared_ptr，解引用之后才是set
        os << "\t(line " << num + 1 << ")  " << *(qr.file->begin() + num) << endl;
    return os;
}


void runQueries(std::ifstream &infile){
    TextQuery tq(infile);
    while(true){
        cout << "enter word to look for, or q to quit: ";
        string s;
        if(!(cin>>s) || s == "q")
            break;
        print(cout, tq.query(s)) << endl;
    }
    cout << "Thank you! Have a nice day :D" << endl;
}


int main(){
    ifstream infile("../text/12_testing.txt");
    if(!infile){
        std::cerr << "failed to open file" << endl;
        return -1;
    }
    runQueries(infile);
    return 0;
}