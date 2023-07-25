#ifndef MESSAGE
#define MESSAGE

#include<string>
using std::string;

#include<set>
using std::set;

#include<iostream>
using std::cerr;
using std::endl;

class Folder;                       //前置声明必须有！不然下面Message里面的函数Folder会显示未声明

class Message{
    friend class Folder;
    friend void swap(Message &, Message &);

public:
    //folders被隐式初始化为空集合
    explicit Message(const string &str = " "): contents(str){}
    //拷贝控制成员，用于管理指向本Message的指针
    Message(const Message &);
    Message(Message &&);
    Message &operator=(const Message &);
    Message &operator=(Message &&);
    ~Message();
    //从给定Folder集合中添加/删除本Message
    void save(Folder &);
    void remove(Folder &);

    void debug_print();             //打印内容，供主函数使用

private:
    string contents;                // 实际文本消息
    set<Folder *> folders;          //包含本Message的Folder
  
    //拷贝构造函数、拷贝赋值运算符和析构函数所使用的工具函数
    //将本Message添加到指向参数的Folder中
    void add_to_Folders(const Message &);
    void remove_from_Folders();

    //给Folders类使用的工具函数
    void addFldr(Folder *f) { folders.insert(f); }
    void remFldr(Folder *f) { folders.erase(f); }

    //给移动构造和赋值运算符使用的工具函数
    void move_Folders(Message *);           //更新指向原Message的Folders
};



class Folder{
    friend class Message;
    friend void swap(Message &, Message &);

public:
    Folder() = default;
    Folder(const Folder &);
    Folder &operator=(const Folder &);
    ~Folder();

    void save(Message &);
    void remove(Message &);

    void debug_print();             //打印内容，供主函数使用

private:
    set<Message *> messages;

    //供拷贝构造函数、析构函数和拷贝赋值运算符使用的工具函数
    void add_to_msgs(const Folder &);                   // 一个添加到给定对象的messages集合中（因为是拷贝）
    void remove_from_msgs();                            // 一个将自身从messages集合中删除

    void addMsg(Message *m) { messages.insert(m); }
    void remMsg(Message *m) { messages.erase(m); }
};


//注意未声明noexcept，因为向set中插入元素可能会抛出异常
void Message::move_Folders(Message *m){
    folders = std::move(m->folders);                    //使用set的移动赋值运算符
    for(auto f :folders){                               //改变Folder中保存的Message指针
        f->remMsg(m);
        f->addMsg(this);
    }
    m->folders.clear();
}


Message::Message(const Message &m) : contents(m.contents),folders(m.folders){
    add_to_Folders(m);
}

Message::Message(Message &&m):contents(m.contents){
    move_Folders(&m);           //移动folders并更新Folder指针
}


void Message::save(Folder &f){
    folders.insert(&f);
    f.addMsg(this);
}

void Message::remove(Folder &f){
    folders.erase(&f);
    f.remMsg(this);
}

//将本message添加到指向m的Folder中
void Message::add_to_Folders(const Message &m){         //这个const是因为set中对象的地址并没有改变
    //使用range for没问题的原因：这里改的是成员指向的地方的对象，而成员本身（地址）没有变
    //如果害怕出问题的话可以按正常的来写
    for(auto f : m.folders)                             
        f->addMsg(this);        //添加一个指向本message的指针
}

void Message::remove_from_Folders(){
    for(auto f : folders)
        f->remMsg(this);
}

Message::~Message(){
    remove_from_Folders();
}

//也执行拷贝构造函数和析构函数的行为，且需要注意自赋值问题
Message& Message::operator=(const Message &m){
    remove_from_Folders();
    contents = m.contents;
    folders = m.folders;
    add_to_Folders(m);                      //先add再remove的话，如果是自赋值，两者地址一样，会全被删掉
    return *this;
}

//移动赋值运算符直接检查自赋值情况
Message& Message::operator=(Message &&rhs){
    if(this != &rhs){                       //直接检查自赋值
        remove_from_Folders();
        contents = std::move(rhs.contents);
        move_Folders(&rhs);                 //重置Folders指向本Message
    }
    return *this;
}



void swap(Message &lhs, Message &rhs){      //lhs是赋值的目的地，rhs是赋值的源头（=左右）
    using std::swap;                        //不严格要求这样写，但是个好习惯

    for(auto f : lhs.folders)
        f->remMsg(&lhs);
    for(auto f : rhs.folders)
        f->remMsg(&rhs);

    swap(lhs.contents, rhs.contents);
    swap(lhs.folders, rhs.folders);

    for(auto f : lhs.folders)
        f->addMsg(&lhs);
    for (auto f : rhs.folders)
        f->addMsg(&rhs);
}







void Folder::add_to_msgs(const Folder &f){
    for(auto m : f.messages)
        m->addFldr(this);
}

Folder::Folder(const Folder &f) : messages(f.messages) { 
    add_to_msgs(f); 
}

void Folder::remove_from_msgs(){
    while (!messages.empty())
        (*messages.begin())->remove(*this);             //因为remove时会一起把这个message.begin()给删除掉
}

Folder::~Folder(){
    remove_from_msgs();
}

Folder& Folder::operator=(const Folder &rhs){
    remove_from_msgs();
    messages = rhs.messages;
    add_to_msgs(rhs);
    return *this;
}

void Folder::save(Message &m){
    messages.insert(&m);
    m.addFldr(this);
}

void Folder::remove(Message &m){
    messages.erase(&m);
    m.remFldr(this);
}


void Folder::debug_print(){
    cerr << "Folder contains: " << messages.size() << " messages " << endl;
    int ctr = 1;
    for(const auto &m : messages)
        cerr << "Message " << ctr++ << ":\n\t" << m->contents << endl;
}

void Message::debug_print(){
    cerr << "Message:\n\t" << contents << endl;
    cerr << "Appears in " << folders.size() << " Folders " << endl;
}


#endif