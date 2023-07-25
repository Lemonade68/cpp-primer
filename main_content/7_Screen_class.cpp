#include<iostream>
#include<string>
#include<vector>

class Screen{
    friend class Window_mgr;                                //声明 + 设置友元

public:
    typedef std::string::size_type pos;                     //可以供用户使用(要加上作用域)
    // using pos = std::string::size_type;

    Screen() = default;
    Screen(pos ht, pos wd):
        height(ht), width(wd), contents(ht * wd, ' ') { }
    Screen(pos ht, pos wd, char c):
        height(ht), width(wd), contents(ht * wd, c) { }     //注意string对象初始化，全屏声明成一个字符的重复


    char get() const { return contents[cursor]; }           //读光标处的字符  隐式内联
    inline char get(pos ht, pos wd) const;                  //显式内联以及函数重载
    Screen &move(pos r, pos c);                             //之后设为内联

    void some_member() const;

    Screen &set(char);
    Screen &set(pos, pos, char);

    //根据对象是否是const重载了display函数
    Screen &display(std::ostream &os) { 
        do_display(os);
        return *this;
    }
    const Screen &display(std::ostream &os) const{
        do_display(os);
        return *this;
    }

    void setHeight(pos);

private:
    pos cursor = 0;                     //光标
    pos height = 0, width = 0;          //屏幕高和宽
    std::string contents;
    mutable size_t access_ctr;          //mutable 表示可变数据成员
    void do_display(std::ostream &os) const { os << contents; }     //该函数的意义：方便日后的操作
};


char Screen::get(pos r, pos c) const {
    pos row = r * width;
    return contents[row + c];
}

inline Screen &Screen::move(pos r, pos c){                  
    pos row = r * width;
    cursor = row + c;                                       //光标移动到指定位置
    return *this;
}

void Screen::some_member()const{
    ++access_ctr;
}

inline Screen &Screen::set(char c){
    contents[cursor] = c;
    return *this;
}

inline Screen &Screen::set(pos r, pos c, char ch){
    contents[r * width + c] = ch;
    return *this;
}

//成员函数定义在类外部时还需考虑定义前全局作用域中的声明      接  setHeight 函数
Screen::pos verify(Screen::pos);
void Screen::setHeight(pos var){
    height = verify(var);
}



class Window_mgr{                                           
public:
    using ScreenIndex = std::vector<Screen>::size_type;     //类型成员

    void Clear(ScreenIndex);
    ScreenIndex addScreen(const Screen &);

private:
    std::vector<Screen> screens{Screen(24, 80, ' ')};       //利用Screen的构造函数创建一个screen对象，赋给vector

};

void Window_mgr::Clear(ScreenIndex i){
    Screen &s = screens[i];                                 //注意是引用，不是赋值！
    s.contents = std::string(s.height * s.width, ' ');      //清屏
}

Window_mgr::ScreenIndex Window_mgr::addScreen(const Screen &s){     //注意这边作用域的添加与否
    screens.push_back(s);
    return screens.size() - 1;                              //返回编号
}



int main() {
    Screen::pos ht = 24, wd = 80;

    Screen myScreen(5, 5, 'X');
    myScreen.move(4, 0).set('#').display(std::cout);
    std::cout << "\n";
    myScreen.display(std::cout);
    std::cout << "\n";

    return 0;
}
