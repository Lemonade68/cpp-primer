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
#define M 5                 // 到达的时间段长度
#define S 3                 // 电梯有人时S秒每层
#define T 2                 // 每个乘客上下电梯的时间
#define play_speed 2        // 倍速播放（*n）
#define MAX_Times 3         // 乘坐的次数随机区间上限    5次演示时间短点

// enum elevator_status{halt,waiting,runing};

using std::cout;
using std::endl;
using std::vector;


class Passenger;

class Elevator{
    friend size_t choose_elevator(Passenger &, vector<Elevator> &);     // 乘客选择电梯，返回电梯编号，并设置等待时间
    friend class Passenger;
    static size_t elevator_code;

public:
    Elevator() : isselected(true), code(elevator_code++), first_dir_up(true), set(true), isempty(true), direction_up(true), between_floors(false), waiting_pas(true), cur_floor(1),initial(true),wait_time(0) { }
    void run(size_t);                                                   //每一秒进行判断状态

public:
    bool overweighing() const
        { return pasgr.size() > MAXPEOPLE ? true : false; }             //是否超重
    bool empty() const{ return isempty; }
    bool dir_up() const{ return direction_up; }
    //调试用   
    bool selected() const { return isselected; }
    bool wp() const { return waiting_pas; }
    const size_t &get_count() const { return count; }
    bool getset() const { return set; }
    const size_t &get_fpc() const { return first_pas_code; }

    const size_t &get_waittime() const { return wait_time; }
    const size_t& get_floor() const { return cur_floor; }
    const size_t& get_code() const{ return code; }
    size_t get_seltd_floor_num() const { return selectedfloor.size(); }
    bool get_bf() const { return between_floors; }
    const vector<Passenger*> &get_pasgr() const { return pasgr; }
    void push_pasgr(Passenger&, vector<Elevator>&);                     // 添加乘客，设置new_pas_num                               
    void pop_pasgr();                                                   // 删除乘客，设置lev_pas_num
    void check (const vector<Passenger*>&);                             // 如果现在没有乘客，判断是否有乘客需要自己去接

private:
    size_t code;                            // 电梯编号
    bool isselected;                        // 电梯是否已被选中
    bool isempty;                           // 电梯是否为空
    bool direction_up;                      // 电梯方向，真为向上，假向下
    bool between_floors;                    // 是否正在上升或下降过程（楼间）
    bool waiting_pas;                       // 是否正在等待乘客上下电梯
    bool initial;                           // 是否为初始状态（还未接待乘客）
    
    //第2次新加
    bool first_dir_up;                      // 第一个按下按钮的人的乘坐方向
    bool set;                               // 判断是否是第一个按下按钮的人，初始为true，后续为false
    size_t first_pas_code;                  // 记录第一个按下按钮的乘客的编号，初始置为0

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
    void reverse_dir();
};
size_t Elevator::elevator_code = 0;         // 编号为0-9  分离编译时要写到cpp文件中，不能写在.h文件中



class Passenger{
    friend class Elevator;
    friend size_t choose_elevator(Passenger&, vector<Elevator>&);   // 乘客选择电梯，返回电梯编号
    static size_t pas_code;
public:
    Passenger();
    void run(size_t, vector<Elevator>&);                            // 每秒判断状态

public:
    // static vector<Passenger> wait_list;                          // 正在等电梯的乘客名单
    static size_t pas_remained;                                     // 剩余未到达目的地乘客数
    bool finished() const { return finishd; };                      // 判断是否到达目的地楼层
    bool pressup() const { return dest_floor > cur_floor; }         // 是否按下向上键
    // bool onboard() const { return on_ele; }                      //是否在电梯上
    const size_t &getcode() const { return code; }
    const size_t &getdest() const { return dest_floor; }
    const size_t &getcur() const { return cur_floor; }
    const size_t &get_ele_num() const { return ele_code; }
    const size_t &get_arrive_time() const { return arrive_time; }
    const size_t &get_wait_time() const { return wait_time; }
    const size_t &get_L() const { return L; }
    const size_t &get_took_time() const { return took_time; }
    const size_t &get_counter() const { return counter; }


private:
    bool arrived;                                   // 初始时是否已经到达1层                        
    bool on_ele;                                    // 是否在电梯上
    bool finishd;                                   // 乘客结束仿真
    size_t code;                                    // 乘客编号
    size_t dest_floor;                              // 目标楼层（后续可更改   注意不能随机到当前的楼层）  
    size_t cur_floor;                               // 目前楼层（跟随电梯）   
    size_t arrive_time;                             // 乘客等待电梯时间（包括第一次）
    size_t ele_code;                                // 将乘坐的电梯的编号
    
    //第二次上机添加
    size_t L;                                       // 要乘坐的电梯的次数
    size_t took_time;                               // 乘坐过的电梯的次数，初始化为0
    size_t wait_time;                               // 在楼层上等待的时间
    size_t counter;                                 // 等待时的计数器，初始化为0

    // 工具函数
    // size_t cal_wait_time(const Elevator &);      // 计算等待时间
};
size_t Passenger::pas_remained = N;
size_t Passenger::pas_code = 1;
// vector<Passenger> Passenger::wait_list;


vector<Passenger*> pas_wait_list;                    // 仍在等待的名单


void push_wait_list(Passenger *p){
    pas_wait_list.push_back(p);
}

void pop_wait_list(Passenger *p){
    auto it = std::find_if(pas_wait_list.begin(), pas_wait_list.end(),[&p](Passenger *pas){ return pas->getcode() == p->getcode();});
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
void Elevator::push_pasgr(Passenger& p, vector<Elevator> &ele_vec){
    pasgr.push_back(&p);
    isempty = false;
    //上来的乘客一定是顺着电梯方向的，需要比较 要去的楼层和已选中的楼层的大小
    // selectedfloor.push_back(p.dest_floor);
    //如果目前电梯只有1人，那么电梯运行方向就是该乘客要去的方向
    
    // if(selectedfloor.size() == 0)            //后续更改
    
    //如果在selectedfloor列表上，从selectedfloor上将其删除
    auto it = std::find_if(ele_vec[p.ele_code].selectedfloor.begin(),ele_vec[p.ele_code].selectedfloor.end(),[&p](const size_t &a){return a == p.cur_floor;});
    if(it != ele_vec[p.ele_code].selectedfloor.end())
        ele_vec[p.ele_code].selectedfloor.erase(it);

    if(p.code == first_pas_code){               //如果是该电梯空乘客时第一个按下按钮的乘客
        set = true;                             //解锁
        // first_pas_code = 0;                     //置零
    }
    if(pasgr.size() == 1){                      
        direction_up = p.pressup();
        first_dir_up = p.pressup();
    }
    //在电梯的所有乘客中选择下一个停靠的楼层         
    // dest_floor = choosefloor(selectedfloor, direction_up);
    //新增乘客数+1
    ++new_pas_num;          
}
//注意修改电梯信息empty，判断是否修改方向，选中，下一个停靠层


//电梯类进行使用，返回下去的人数，并设置该乘客下一次的数据，每次run时使用即可
void Elevator::pop_pasgr(){
    static size_t count = 0;                    //下去乘客的人数
    count = 0;
    auto it = std::find_if(pasgr.begin(),pasgr.end(),[this](Passenger *p){return cur_floor == p->dest_floor;});
    while(it != pasgr.end()){
        ++count;

        //设置 *下次的* 状态
        (*it)->took_time++;                                     //这是第几次
        (*it)->on_ele = false;

        // (*it)->wait_time = rand() % 111 + 10;                   //设计下次的随机等待时间
        (*it)->wait_time = rand() % 11 + 10;                    //设计下次的随机等待时间（先按10-20算）
        (*it)->counter = 0;                                     //计数器清零
        if((*it)->took_time < (*it)->L){                        //次数小于L次，随机生成下次的dest_floor（包括第L次）
            (*it)->dest_floor = rand() % 39 + 2;
            while((*it)->cur_floor == (*it)->dest_floor)        //如果随机到这层，重新随机
                (*it)->dest_floor = rand() % 39 + 2;
        }
        else if((*it)->took_time == (*it)->L){                  //第L次下电梯，设置L+1次的状态
            (*it)->dest_floor = 1;
        }
        else{                                                   //第L+1次下电梯
            (*it)->wait_time = 0;                               //状态清0
            (*it)->finishd = true;                              //乘客到达1层，结束仿真
        }

        if(!(*it)->finished())                                  //如果仿真还没结束，再加回wait_list 
            pas_wait_list.push_back(*it);
        else if((*it)->cur_floor == (*it)->dest_floor)          //如果isfinished，且已经到达目的楼层（1层）
            --Passenger::pas_remained;                          //剩余的乘客数-1
        
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

//修bug用的
void Elevator::reverse_dir(){
    if(direction_up == true)
        direction_up = false;
    else
        direction_up = true;
}


//更新new_pas_num 和 lev_pas_num
//设置下一时刻电梯的状态

void Elevator::run(size_t cur_time){
    // 这边不能设置static count = 0         有问题！！     //所有类的对象在调用该函数时共享这个变量

    if(pasgr.size() != 0)
        initial = false;
    if(initial)                                         //电梯处于初始状态，直接返回
        return;

    check(pas_wait_list);                               //检查该时刻是否有人需要电梯来接
    if(!isselected){                                    //没有则返回
        waiting_pas = true;
        between_floors = false;
        return;
    }

    //判断是否为等待部分
    if(waiting_pas){
        // cout << code << " wp" << endl;
        pop_pasgr();                                                //先让乘客下去，得到下去的人数，上来的人数由乘客类提供
        wait_time += (new_pas_num + lev_pas_num) * T;               //设置等待时间

        //中间层状态        
        if(new_pas_num + lev_pas_num == 0 && wait_time == 0){       //如果这一时刻既不上来乘客也不下去乘客且不需要再等待  直接往下一层走
            waiting_pas = false;
            between_floors = true;
            count = 0;
            goto FLAG;                          //进入上升或者下降循环
        }

        //等待层状态
        if(count >= wait_time){                 //等待完成，执行下一步
            count = 0;
            new_pas_num = 0;                    //清空状态
            lev_pas_num = 0;                    //清空状态
            waiting_pas = false;
            between_floors = true;
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
void Elevator::check(const vector<Passenger*> &wait_list){
    // 判断是否还有乘客
    static bool sign;                                                                    //以及是否与该乘客同楼层
    sign = false;
    if(pasgr.size() == 0){
        //搜索wait_list中是否有乘客需要自己来接，优先朝第一个按下按钮的乘客（使用push_back，后进来的wait_list在后面）
        for(const auto &p : wait_list){
            if(p->counter >= p->wait_time && p->ele_code == code){                              //乘客选中电梯且乘客已经等待完毕（此时按下按钮），找第一个按按钮的乘客
                if(cur_floor == p->cur_floor){
                    wait_time = 2;                                                              //如果在这层：电梯等待，防止先上下一层再返回
                    if(set){
                        sign = true;
                        first_pas_code = p->code;                                               //记录该乘客的编号
                        direction_up = p->pressup();
                        first_dir_up = p->pressup();                                            //设置该乘客将要去的方向
                    }
                }
                else{
                    if(set){
                        first_pas_code = p->code;
                        direction_up = (cur_floor > p->cur_floor) ? false : true;               //设置方向
                        first_dir_up = p->pressup();
                    }
                }
                //若还未添加过，则添加
                auto it = std::find_if(selectedfloor.begin(),selectedfloor.end(),[&p](const size_t &a){return a == p->cur_floor;});
                if(it == selectedfloor.end())
                    selectedfloor.push_back(p->cur_floor);
                set = false;
            }
        }
        if(selectedfloor.size() == 0){
            set = true;                                 //如果一直没被选中,set不断还原为true，如果只在乘客类加的话，加入乘客前set为false，加不进去，而只有进去了才能变成true
            isselected = false;
            count = 0;
            wait_time = 0;
            waiting_pas = true;
            between_floors = false;
        }
        else{
            if(sign){
                isselected = true;
                waiting_pas = true;
                between_floors = false;
            }else{
                isselected = true;
                waiting_pas = false;
                between_floors = true;                 //设置成false : 以便在当前层的乘客可以直接上来（配合run中的更改waiting_pas状态使用） 
            }
        }
    }
}
//*****************************************************

//初始化乘客对象
Passenger::Passenger() : arrived(false), code(pas_code++), cur_floor(1), on_ele(false), finishd(false) {
    arrive_time = rand() % M;                       //生成0-M的到达时间
    dest_floor = rand() % 39 + 2;                   //生成2-40的随机目标楼层
    ele_code = rand() % 10;                         //随机选择一部电梯        第一次   choose_elevator后续使用
    L = rand() % MAX_Times + 1;                     //随机生成要乘坐的次数     10太多，生成3次以内
    push_wait_list(this);                           //加入等候名单
}       


void Passenger::run(size_t cur_time, vector<Elevator>& ele_vec){
    //已经到达目的地，则返回
    if(finishd)
        return;
    if(cur_time >= arrive_time)
        arrived = true;
    
    //如果在电梯上或仍未到现场：返回（前者交给电梯类处理）
    if(on_ele || !arrived)
        return;                                 
    
    if(counter < wait_time){                            //处理等待时间，完毕之后按下按钮
        ++counter;
        return;
    }
    
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
    if(ele_vec[ele_code].dir_up() != pressup() && ele_vec[ele_code].pasgr.size() != 0)   
        return;
    //如果方向不同且电梯无人，但自己不是电梯要接的第一个乘客，返回
    if(ele_vec[ele_code].dir_up() != pressup() && ele_vec[ele_code].pasgr.size() == 0 && ele_vec[ele_code].first_pas_code != code)
        return;
    //如果电梯经过自己且和自己方向相同，但是在接第一个乘客后要去的方向和自己相反，则先不上电梯
    if (ele_vec[ele_code].first_dir_up != pressup())
        return;

    counter = 0;                                        //counter清零
    wait_time = 0;                                      //wait_time清零
    pop_wait_list(this);                                //从等候名单删除


    ele_vec[ele_code].push_pasgr(*this, ele_vec);       //把自己插入到电梯类中
    on_ele = true;
}


size_t choose_elevator(Passenger& pasgr, vector<Elevator> &vec){
    pasgr.ele_code = rand() % 10;                       // 乘客乘坐的电梯号码
    return 0;
    //判断有没有电梯可以坐：
    // bool has_ele = false;                            //是否有空的电梯
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
    srand((unsigned)time(NULL));                    //设置随机数种子

    vector<Elevator> ele_vec(10);                   //初始化10个电梯
    vector<Passenger> pasgr_vec(N);                 //初始化N个乘客
    // for(auto &p : pasgr_vec)
        // choose_elevator(p, ele_vec);             //选择每个乘客所要乘坐的电梯
    cout << "Simulation starts, passengers' information are as follows:" << endl << endl;
    for(const auto &p : pasgr_vec)
        cout << p.getcode() << "--" << p.getdest() <<" arrives at "<<p.get_arrive_time()<< " , selects elevator " << p.get_ele_num() << endl;
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
            if(!p.finished())                   //如果还没结束，接着运行该乘客
                p.run(timer, ele_vec);
        for(auto &e : ele_vec)
            e.run(timer);



        for(const auto & e : ele_vec){
            //调试时显示状态用
            // cout << "Elevator " << e.get_code() << " has "<<e.get_seltd_floor_num()<<" passengers to take "<< ((e.get_pasgr().size() == 0 && e.get_seltd_floor_num() == 0) ? " - " : (e.dir_up() ? " ↑ " : " ↓ ")) << "\t"
                //  << "(cur_floor:" << e.get_floor() << ")" << "\t direction_up:"<<(e.dir_up()?" true ":" false ")
            cout << "Elevator " << e.get_code() << ((e.get_pasgr().size() == 0 && e.get_seltd_floor_num() == 0) ? " - " : (e.dir_up() ? " ↑ " : " ↓ ")) << "\t"
                 << "(cur_floor:" << e.get_floor() << ")" << "\tfirst_pas_code: " << e.get_fpc() 
                 << "\twait_time: " << e.get_waittime() << "\t Passengers: \t";
            for(const auto & p : e.get_pasgr())
                cout << p->getcode() << "--" << p->getdest() << ";\t";
            cout << endl;
            //调试用代码
            cout << "isselected:" << (e.selected() ? " true" : " false") << "\twaiting_pas:" << (e.wp() ? " true" : " false") << "\tbetween_floor:" << (e.get_bf() ? " true" : " false") <<"\tset: "<<(e.getset() ? " true" : " false") << endl;
            cout << endl;
        }
        cout << endl;
        cout << "Remained Passengers' List:" << endl;
        if(pas_wait_list.size() == 0)
            cout << "None!" << endl << endl;
        else{
            for(const auto &p : pas_wait_list)
                cout << p->getcode() << "\tarrives at " << p->get_arrive_time() << "\tselects elevator " << p->get_ele_num()
                     << "\tcurrent_floor: " << p->getcur() << "\tdest_floor: " << p->getdest() << "\t  wait_time: " << p->get_wait_time() << "\t  Total_took_time_L: " << p->get_L() << "\ttook_time: " << p->get_took_time()
                     << "\tis_ready:" << ((p->get_counter() >= p->get_wait_time()) ? " Yes " : " No ") << endl;
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