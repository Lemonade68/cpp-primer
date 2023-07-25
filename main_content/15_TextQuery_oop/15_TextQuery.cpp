#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<memory>
#include<algorithm>
#include<iterator>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::istringstream;
using std::make_shared;
using std::map;
using std::ostream;
using std::set;
using std::set_intersection;
using std::shared_ptr;
using std::string;
using std::vector;


class QueryResult;

// 传入文件的同时，生成文件对应的word_map供之后使用，即：map<word,word出现的行的集合的指针>
class TextQuery{
public:
    using line_no = vector<string>::size_type;          //类型别名，表示行号，即vector中的下标
    TextQuery() = default;
    TextQuery(ifstream &);                              //读写IO类对象会改变其状态，因此不可以使用const来修饰 
    QueryResult query(const string &) const;            //在word_map中查找传入的单词，返回QueryResult对象

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
            auto &lines = wm[word];                     //使用lines作为别称，lines对应的是一个shared_ptr，可以使用new来初始化
            if(!lines)                                  //第一次遇见word时，将wm[word]值初始化为空指针
                lines.reset(new set<line_no>);
            lines->insert(n);                           //因为是set，所以如果一个单词多次出现在同一行，insert不会重复插入
        }
    }
}



class QueryResult{                                      //一个单词对应的所有查询结果类
    friend ostream &print(ostream &, const QueryResult &);
public:
    typedef set<TextQuery::line_no>::const_iterator line_it;
    QueryResult(string s, shared_ptr<set<TextQuery::line_no>> p, shared_ptr<vector<string>> f):
        sought(s),lines(p),file(f) { }                  //体会这边复制智能指针的妙处
    
    //后面所需的新的成员
    line_it begin() const { return lines->begin(); }
    line_it end() const { return lines->end(); }
    shared_ptr<vector<string>> get_file() { return file; }

private: 
    string sought;                                      //查询单词
    shared_ptr<set<TextQuery::line_no>> lines;          //出现的行号set
    shared_ptr<vector<string>> file;                    //输入文件对应文本
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


//15.9内容

// 1. 接口类与抽象基类 

class Query;

//Query_base类，是抽象基类，具体的查询类型从中派生，所有成员都是private的(对用户隐藏)
class Query_base{
    friend class Query;                     //Query需要调用Query_base的虚函数
protected:
    using line_no = TextQuery::line_no;     //用于eval函数
    virtual ~Query_base() = default;        //隐式地被用于派生类的析构函数中
private:
    //eval返回与当前Query匹配的QueryResult    e-value 求值
    virtual QueryResult eval(const TextQuery &) const = 0;
    //rep是表示查询的一个string
    virtual std::string rep() const = 0;
};


//这是一个管理Query_base继承体系的接口类
class Query{
    //这些运算符需要访问shared_ptr的构造函数，而该函数是私有的
    friend Query operator~(const Query &);
    friend Query operator|(const Query &, const Query &);
    friend Query operator&(const Query &, const Query &);
public:
    Query(const string &);                              //构建一个新的WordQuery（该类还未定义）
    //接口函数，调用对应的Query_base操作
    QueryResult eval(const TextQuery &t) const { return q->eval(t); }
    string rep() const { return q->rep(); }
private:
    Query(shared_ptr<Query_base> query) : q(query){ }   //不希望用户随便定义Query_base对象，放私有，供下面运算符使用
    shared_ptr<Query_base> q;                           //类型为抽象基类，这样可以调用派生类的虚函数
};

//输出运算符重载
ostream& operator<<(ostream &os, const Query &query){
    //Query::rep()通过它的Query_base指针对rep()进行了虚调用
    return os << query.rep();
}




// 2.派生类

//word_query类：在给定的text_query对象上实际执行查询的唯一操作
class WordQuery : public Query_base{
    friend class Query;                                 //Query使用WordQuery构造函数
    WordQuery(const string &s) : query_word(s){ }
    //具体的类：word_query将定义所有继承而来的纯虚函数
    QueryResult eval(const TextQuery &t) const override { return t.query(query_word); }
    string rep() const override { return query_word; }
    string query_word;                  //要查找的单词
};

inline Query::Query(const string &s) : q(new WordQuery(s)) { }


//NotQuery类及~运算符：~生成一个NotQuery，其中保存一个需要对其取反的Query
class NotQuery : public Query_base{
    friend Query operator~(const Query &);
    NotQuery(const Query &q) : query(q){ }
    //具体的类：not_query将定义所有继承而来的纯虚函数
    string rep() const override { return "~(" + query.rep() + ")"; }
    QueryResult eval(const TextQuery &) const override;
    Query query;                    //可能是单个，也可能是binaryquery的派生类，因此统一使用Query（使用Query的话省去了指针操作），即使用接口类来简化自己的类
};

Query operator~(const Query &operand){
    return shared_ptr<Query_base>(new NotQuery(operand));   //使用接受shared_ptr的Query构造函数
}


//BinaryQuery类
class BinaryQuery : public Query_base{
protected:
    BinaryQuery(const Query &l, const Query &r, string s): lhs(l), rhs(r), opSym(s) { }     //Query_base没有数据成员，不需要构造函数初始化
    //抽象类, BinaryQuery不定义eval，而是继承eval这个纯虚函数；不能创建BinaryQuery对象
    string rep() const override { return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")"; }
    Query lhs, rhs;                 //左右侧运算对象
    string opSym;                   //运算符名字
};


//AndQuery类
class AndQuery : public BinaryQuery{
    friend Query operator&(const Query &, const Query &);
    AndQuery(const Query &left, const Query &right) : BinaryQuery(left,right, "&") { }
    //具体的类：AndQuery继承了rep并定义了eval这个纯虚函数
    QueryResult eval(const TextQuery &) const override;
};

Query operator&(const Query &lhs, const Query &rhs){
    return shared_ptr<Query_base>(new AndQuery(lhs, rhs));  //使用接受shared_ptr的Query构造函数
}


//OrQuery类
class OrQuery : public BinaryQuery{
    friend Query operator|(const Query &, const Query &);
    OrQuery(const Query &left, const Query &right) : BinaryQuery(left, right, "|") { }
    //具体的类：OrQuery继承rep并定义eval
    QueryResult eval(const TextQuery &) const override;
};

Query operator|(const Query &lhs, const Query &rhs){
    return shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}



//eval函数

//OrQuery::eval
QueryResult OrQuery::eval(const TextQuery &text) const{
    //通过Query成员lhs和rhs进行的虚调用
    //调用eval返回每个运算对象的QueryResult
    auto right = rhs.eval(text), left = lhs.eval(text);
    //将左侧运算对象拷贝到结果set中
    auto ret_lines = make_shared<set<line_no>>(left.begin(), left.end());
    //插入右侧对象的行号
    ret_lines->insert(right.begin(), right.end());
    //返回一个新的QueryResult表示lhs和rhs的并集
    return QueryResult(rep(), ret_lines, left.get_file());      //right.get_file()也可以
}


//AndQuery::eval
QueryResult AndQuery::eval(const TextQuery &text) const{
    //通过Query运算对象进行的虚调用，以获得运算对象的查询结果set
    auto left = lhs.eval(text), right = rhs.eval(text);
    //保存left和right交集的set
    auto ret_lines = make_shared<set<line_no>>();
    //将两个范围的交集写入一个目的迭代器中
    //本次调用的目的迭代器向ret添加元素
    set_intersection(left.begin(), left.end(), right.begin(), right.end(), std::inserter(*ret_lines, ret_lines->begin()));
    return QueryResult(rep(), ret_lines, left.get_file());
}


//NotQuery::eval
QueryResult NotQuery::eval(const TextQuery& text)const{
    //通过Query运算对象对eval进行虚调用
    auto result = query.eval(text);
    //开始时结果set为空
    auto ret_lines = make_shared<set<line_no>>();
    //我们必须在运算对象出现的所有行中进行迭代
    auto beg = result.begin(), end = result.end();
    //对于输入文件的每一行，如果该行不在result当中，则将其添加到ret_lines
    auto sz = result.get_file()->size();
    for (size_t n = 0; n != sz; ++n){
        //如果我们还没有处理完result的所有行
        //检查当前行是否存在
        if(beg == end || *beg != n)     //因为set默认是从小到大排序
            ret_lines->insert(n);       //如果不在result中，添加这一行
        else if(beg != end)
            ++beg;
    }
    return QueryResult(rep(), ret_lines, result.get_file());
}
