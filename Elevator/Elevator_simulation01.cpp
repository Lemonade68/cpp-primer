//乘客类管理什么时候上电梯，电梯类管理什么时候下乘客

#include<iostream>
#include<vector>
#include<string>
#include<ctime>
#include<cmath>
#include<Windows.h>
#include<algorithm>

#define MAXPEOPLE 10        // 电梯最大载客量
#define ELE_NUM 10          // 电梯数
#define N 12                // 乘客数
#define M 10                // 到达的时间段长度(秒)
#define S 3                 // 电梯有人时S秒每层
#define T 2                 // 每个乘客上下电梯的时间
#define play_speed 10       // 倍速播放（*n）

// enum elevator_status{halt,waiting,runing};

using std::cout;
using std::endl;
using std::vector;


class Passenger;

class Elevator{
    size_t friend choose_elevator(Passenger &, vector<Elevator> &);     // 乘客选择电梯，返回电梯编号，并设置等待时间
    friend class Passenger;
    static size_t ele_code;

public:
    Elevator() : isselected(true), code(ele_code++), isempty(true), direction_up(true), between_floors(false), waiting_pas(true), cur_floor(1),initial(true), wait_time(0) { }
    void run(size_t);                               //每一秒进行判断状态

public:
    bool overweighing() const
        { return pasgr.size() > MAXPEOPLE ? true : false; }             //是否超重
    bool empty() const{ return isempty; }
    bool dir_up() const{ return direction_up; }
    //调试用   
    bool selected() const { return isselected; }
    bool wp() const { return waiting_pas; }
    const size_t &get_count() const { return count; }


    const size_t &get_waittime() const { return wait_time; }
    const size_t& get_floor() const { return cur_floor; }
    const size_t& get_code() const{ return code; }
    bool get_bf() const { return between_floors; }
    const vector<Passenger*> &get_pasgr() const { return pasgr; }
    void push_pasgr(Passenger&);            // 添加乘客，设置new_pas_num                               
    void pop_pasgr();                       // 删除乘客，设置lev_pas_num
    void check (const vector<Passenger>&);                           // 如果现在没有乘客，判断是否有乘客需要自己去接


private:
    size_t code;                            // 电梯编号
    bool isselected;                        // 电梯是否已被选中
    bool isempty;                           // 电梯是否为空
    bool direction_up;                      // 电梯方向，真为向上，假向下
    bool between_floors;                    // 是否正在上升或下降过程（楼间）
    bool waiting_pas;                       // 是否正在等待乘客上下电梯
    bool initial;
    size_t count;                           // 计数  默认为0
    size_t wait_time;                       // 上下楼、等候乘客上下电梯时使用   初始化为0
    size_t cur_floor;                       // 目前停留楼层
    size_t dest_floor;                      // 下一个停靠层（电梯没人而去接人时的状态）

    size_t new_pas_num;                     // 该层新上来的乘客数  初始置0
    size_t lev_pas_num;                     // 该层下去的乘客数  初始置0

    vector<Passenger*> pasgr;               // 乘客vec，默认初始化为空   传指针，使后面方面修改原值
    //selected_floor可以优化成set，从而减少每次比较的时间
    vector<size_t> selectedfloor;           // 电梯为空时要去接的乘客的楼层，默认初始化为空

    //工具函数
    void refresh();                         // 用于动态更新电梯中的乘客的当前楼层
};
size_t Elevator::ele_code = 0;              // 编号为0-9



class Passenger{
    friend class Elevator;
    size_t friend choose_elevator(Passenger&, vector<Elevator>&);   // 乘客选择电梯，返回电梯编号
    static size_t pas_code;
public:
    Passenger();
    void run(size_t, vector<Elevator>&);                            // 每秒判断状态

public:
    // static vector<Passenger> wait_list;          // 正在等电梯的乘客名单
    static size_t pas_remained;                     // 剩余未到达目的地乘客数
    bool finished() const;                          // 判断是否到达目的地楼层
    bool pressup() const { return dest_floor > cur_floor; }     // 是否按下向上键
    // bool onboard() const { return on_ele; }                  //是否在电梯上
    const size_t &getcode() const { return code; }
    const size_t &getdest() const { return dest_floor; }
    const size_t &get_ele_num() const { return ele_code; }
    const size_t &get_wait_time() const { return wait_time; }

private:
    bool arrived;                                   // 初始时是否已经到达1层                        
    size_t code;                                    // 乘客编号
    size_t dest_floor;                              // 目标楼层（后续可更改）
    size_t cur_floor;                               // 目前楼层（跟随电梯）   
    size_t wait_time;                               // 乘客等待电梯时间（包括第一次）
    size_t ele_code;                                // 将乘坐的电梯的编号
    bool on_ele;                                    // 是否在电梯上
    bool finishd;                                   // 乘客结束仿真
    
    // 工具函数
    // size_t cal_wait_time(const Elevator &);      // 计算等待时间
};
size_t Passenger::pas_remained = N;
size_t Passenger::pas_code = 1;
// vector<Passenger> Passenger::wait_list;



vector<Passenger> pas_wait_list;                    // 仍在等待的名单(直接值拷贝即可)




void push_wait_list(Passenger &p){
    pas_wait_list.push_back(p);
}

void pop_wait_list(const Passenger &p){
    auto it = std::find_if(pas_wait_list.begin(), pas_wait_list.end(),[&p](const Passenger &pas){ return pas.getcode() == p.getcode();});
    if(it != pas_wait_list.end())
        pas_wait_list.erase(it);
}



//当有人上电梯时，在所有人要去的楼层中选择电梯下一个停靠的楼层
size_t choosefloor(const vector<size_t> &vec, bool is_up){
    if(is_up)
        return *std::min_element(vec.begin(), vec.end());
    else
        return *std::max_element(vec.begin(), vec.end());
}


//乘客类来进行使用
void Elevator::push_pasgr(Passenger& p){
    pasgr.push_back(&p);
    isempty = false;
    //上来的乘客一定是顺着电梯方向的，需要比较 要去的楼层和已选中的楼层的大小
    // selectedfloor.push_back(p.dest_floor);
    //如果目前电梯只有1人，那么电梯运行方向就是该乘客要去的方向
    if(pasgr.size() == 1)
        direction_up = p.pressup();    
    //在电梯的所有乘客中选择下一个停靠的楼层         
    // dest_floor = choosefloor(selectedfloor, direction_up);
    //新增乘客数+1
    ++new_pas_num;          
}
//注意修改电梯信息empty，判断是否修改方向，选中，下一个停靠层


//电梯类进行使用，返回下去的人数，每次run时使用即可
void Elevator::pop_pasgr(){
    static size_t count = 0;                        //下去乘客的人数
    count = 0;
    auto it = std::find_if(pasgr.begin(),pasgr.end(),[this](Passenger *p){return cur_floor == p->dest_floor;});
    while(it != pasgr.end()){
        ++count;
        (*it)->on_ele = false;

//*****************************************************
        (*it)->finishd = true;                 //乘客到达最终目标楼层，结束仿真   后续要进行更改
//*****************************************************

        if(!(*it)->finished())                 //如果仿真还没结束，再加回wait_list 
            pas_wait_list.push_back(*(*it));
        else
            --Passenger::pas_remained;         //如果到达终点，剩余的乘客数-1
        pasgr.erase(it);
        it = std::find_if(pasgr.begin(),pasgr.end(),[this](Passenger *p){return cur_floor == p->dest_floor;});
    }

    // if(count > 0){                              //在选择的destfloor中删除该层
    //     for (int i = 0; i < count; ++i){
    //         auto iter = find_if(selectedfloor.begin(), selectedfloor.end(), [this](const size_t &a) { return a == this->cur_floor; });
    //         selectedfloor.erase(iter);
    //     }
    // }
        
    lev_pas_num = count;                        //记录 下电梯的乘客数
}
//注意修改isempty等属性


void Elevator::refresh(){
    for(auto &p : pasgr){
        p->cur_floor = cur_floor;
    }
}


//*****************************************************

//更新new_pas_num 和 lev_pas_num
//设置下一时刻电梯的状态

void Elevator::run(size_t cur_time){
    // 这边不能设置static count = 0         有问题！！

    if(pasgr.size() != 0)
        initial = false;
    if(initial)                             //电梯处于初始状态，直接返回
        return;

    if(!pasgr.size())
        check(pas_wait_list);               //检查该时刻是否有人需要电梯来接
    if(!isselected)                         //没有则返回
        return;

    //判断是否为等待部分
    if(waiting_pas){
        // cout << code << " wp" << endl;
        pop_pasgr();                            //先让乘客下去，得到下去的人数，上来的人数由乘客类提供
        wait_time += (new_pas_num + lev_pas_num) * T;   //设置等待时间

        //中间层状态        
        if(new_pas_num + lev_pas_num == 0 && wait_time == 0){     //如果这一时刻既不上来乘客也不下去乘客且不需要再等待  直接往下一层走
            waiting_pas = false;
            between_floors = true;
            count = 0;
            goto FLAG;                          //进入上升或者下降循环
        }

        //等待层状态
        if(count >= wait_time){                 //等待完成，执行下一步
            count = 0;
            wait_time = 0;
            new_pas_num = 0;                    //清空状态
            lev_pas_num = 0;                    //清空状态
            waiting_pas = false;
            between_floors = true;
            //判断此时电梯中是否有人
            check(pas_wait_list);
            return;
        }
        ++count;                                //每一秒count + 1
        new_pas_num = 0;
        lev_pas_num = 0;
        return;
    }
    //判断是否为上升、下降部分
    else if(between_floors){
    FLAG :
        // cout << code << " bf" << endl;
        wait_time = S;                                  //到达下一层的时间
        if(count >= wait_time){                         //这边改成==会有问题？     为啥
            if(direction_up){
                ++cur_floor;
                refresh();
            }
            else{
                --cur_floor;
                refresh();
            }
            wait_time = 0;
            count = 0;
            new_pas_num = 0;
            lev_pas_num = 0;
            between_floors = false;
            waiting_pas = true;
            return;
        }
        ++count;
        return;
    }
    // 记得最先更新所有乘客的目前楼层，设置在楼层间的状态，isempty状态
}





//*****************************************************
//更改当前电梯的isselected, selected_floor, direction_up状态（假装电梯里有人按了selected_floor）
//目前缺陷：当电梯上下同时有人发出申请时，会先去最后一个那里，可能不符合逻辑
void Elevator::check(const vector<Passenger> &wait_list){
    // 判断是否还有乘客
    if(pasgr.size() == 0){
        //搜索wait_list中是否有乘客需要自己来接
        for(const auto &p : wait_list){
            if(p.ele_code == code){
                //若还未添加过，则添加
                auto it = std::find_if(selectedfloor.begin(),selectedfloor.end(),[&p](const size_t &a){return a == p.cur_floor;});
                if(it == selectedfloor.end()){
                    selectedfloor.push_back(p.cur_floor);
                    direction_up = (cur_floor > p.cur_floor) ? false : true;
                }
            }
        }
        if(selectedfloor.size() == 0){
            isselected = false;
            waiting_pas = true;
            between_floors = false;
        }
        else{
            isselected = true;
            waiting_pas = false;
            between_floors = true;
        }
    }
}
//*****************************************************



//初始化乘客对象
Passenger::Passenger() : arrived(false), code(pas_code++), cur_floor(1), on_ele(false), finishd(false) {
    wait_time = rand() % M;                     //生成0-M的到达时间
    dest_floor = rand() % 39 + 2;               //生成2-40的随机目标楼层
    ele_code = rand() % 10;                     //随机选择一部电梯        第一次   choose_elevator后续使用
    push_wait_list(*this);                      //加入等候名单
}       


void Passenger::run(size_t cur_time, vector<Elevator>& ele_vec){
    //已经到达目的地，则返回
    if(finishd)
        return;
    if(cur_time >= wait_time)
        arrived = true;
    //如果在电梯上或仍未到现场：返回（前者交给电梯类处理）
    if(on_ele || !arrived)
        return;                                 
    //选择的电梯不在同一层楼，等待
    if(ele_vec[ele_code].get_floor() != cur_floor)
        return;
    //若电梯正在楼层间，等待
    if(ele_vec[ele_code].between_floors == true)
        return;
    //若上电梯导致超重，等待
    if(ele_vec[ele_code].get_pasgr().size() + 1 > MAXPEOPLE)  
        return; 
    //若电梯与自己的要去的方向不同且电梯上有人，等待（没人说明是专门来接的，方向可能会不同，但可以上电梯）
    if(ele_vec[ele_code].dir_up() != pressup() && ele_vec[ele_code].pasgr.size()!=0)   
        return;
    ele_vec[ele_code].push_pasgr(*this);        //不然就把自己插入到电梯类中
    on_ele = true;
    pop_wait_list(*this);                       //从等候名单删除
    //如果在selectedfloor列表上，将其删除
    if(ele_vec[ele_code].pasgr.size() == 1){
        auto it = std::find_if(ele_vec[ele_code].selectedfloor.begin(),ele_vec[ele_code].selectedfloor.end(),[this](const size_t &a){return a == cur_floor;});
        if(it != ele_vec[ele_code].selectedfloor.end())
            ele_vec[ele_code].selectedfloor.erase(it);
    }
}

// ************************************************************     后续要改
bool Passenger::finished() const{                   
    return finishd;
}
// ************************************************************


size_t choose_elevator(Passenger& pasgr, vector<Elevator> &vec){
    pasgr.ele_code = rand() % 10;       // 乘客乘坐的电梯号码
    return 0;
    //判断有没有电梯可以坐：
    // bool has_ele = false;           //是否有空的电梯
    // for(const auto &e : vec){
    //     if(e.empty())
    //         has_ele = true;
    //     else if()
    // }
    // for(auto &e : vec){
    //     if(e.empty() && !e.isselected){
    //         has_ele = true;
    //         e.isselected = true;
    //         pasgr.wait_time = (e.cur_floor - pasgr.cur_floor)*;
    //         pasgr.ele_code = e.code;
    //         e.push_pasgr(pasgr);
    //         e.dest_floor = pasgr.dest_floor;
    //     }
    // }
    // if(!has_ele){
    //     pasgr.ele_code;             //选择电梯编号      还没写
    //     pasgr.wait_time = pasgr.cal_wait_time(vec[pasgr.ele_code]);      //计算等待时间
    //     Passenger::wait_list.push_back(pasgr);                           //加入等待名单
    // }
}


int main(){
    srand((unsigned)time(NULL));                //设置随机数种子

    vector<Elevator> ele_vec(10);               //初始化10个电梯
    vector<Passenger> pasgr_vec(N);             //初始化N个乘客
    // for(auto &p : pasgr_vec)
        // choose_elevator(p, ele_vec);            //选择每个乘客所要乘坐的电梯
    cout << "Simulation starts, passengers' information are as follows:" << endl << endl;
    for(const auto &p : pasgr_vec)
        cout << p.getcode() << "--" << p.getdest() <<" arrives at "<<p.get_wait_time()<< " , selects elevator " << p.get_ele_num() << endl;
    cout << endl;
    for(const auto & e : ele_vec){
        cout << "Elevator " << e.get_code() << " (cur_floor: " << e.get_floor() << ") Passengers:\t";
        for(const auto & p : e.get_pasgr())
            cout << p->getcode() << "--" << p->getdest() << " ;\t";
        cout << endl;
    }
    system("pause");                            //操作以继续

    size_t timer = 0;                           //计时器
    while(Passenger::pas_remained != 0){        //进行时间循环，当没有乘客需要坐电梯时结束仿真
        system("cls");
        for(auto &p : pasgr_vec)
            p.run(timer, ele_vec);
        for(auto &e : ele_vec)
            e.run(timer);



        for(const auto & e : ele_vec){
            cout << "Elevator " << e.get_code() << (e.dir_up() ? " ↑ " : " ↓ ") << "\t"
                 << "(cur_floor:" << e.get_floor() << ")"
                 << "\twait_time: " << e.get_waittime() << "\t Passengers: \t";
            for(const auto & p : e.get_pasgr())
                cout << p->getcode() << "--" << p->getdest() << ";\t";
            cout << endl;
            //调试用代码
            // cout << "\tisselected:" << (e.selected() ? " true " : " false ") << "\twaiting_pas:" << (e.wp() ? " true " : " false ") << "\tbetween_floor:" << (e.bf() ? " true " : " false ") << endl;
            // cout << " waiting_time "<<e.get_waittime() << endl;
        }
        cout << endl;
        cout << "Remained Passengers' List:" << endl;
        if(pas_wait_list.size() == 0)
            cout << "None!" << endl << endl;
        else{
            for(const auto &p : pas_wait_list)
                cout << p.getcode() << "--" << p.getdest() << "\tarrives at " << p.get_wait_time() << "\tselects elevator " << p.get_ele_num() << endl;
            cout << endl;
        }
        cout << "Number of Remained Passengers: " << Passenger::pas_remained << endl;
        cout << endl;
        cout << "current time: " << timer << endl;
        cout << endl;
        system("pause");
        // Sleep(1000/play_speed);                            //后续进行修改，更改播放速度
        ++timer;
    }

    cout << "Finished! :D" << endl;
    return 0;
}