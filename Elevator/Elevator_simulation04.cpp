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
#define MAX_Times 3         // 乘坐的次数随机区间上限    5次演示时间短点
#define play_speed 100        // 倍速播放（*n）
#define MODE 2              // 播放模式：1为手动播放， 2为自动播放

using std::cerr;
using std::cout;
using std::endl;
using std::vector;


//前置声明部分
class Passenger;

class Elevator{
    friend void choose_elevator_first(Passenger &);       // 乘客选择电梯，返回电梯编号，并设置等待时间
    friend void choose_elevator(Passenger &);             // 乘客选择电梯，返回电梯编号，并设置等待时间
    friend class Passenger;
    static size_t elevator_code;

public:
    Elevator() : isselected(true), code(elevator_code++), first_dir_up(true), set(true), 
                 isempty(true), direction_up(true), between_floors(false), waiting_pas(true), 
                 cur_floor(1),initial(true),wait_time(0), frame_chose(false) { }

    bool selected() const { return isselected; }
    bool overweighing() const { return pasgr.size() > MAXPEOPLE ? true : false; }             //是否超重
    bool empty() const{ return isempty; }
    bool dir_up() const{ return direction_up; }
    bool get_bf() const { return between_floors; }
    
    //*调试用   
    bool wp() const { return waiting_pas; }
    bool getset() const { return set; }
    const size_t &get_fpc() const { return first_pas_code; }
    const size_t &get_count() const { return count; }
    const size_t& get_waittime() const { return wait_time; }
    //*调试用
    
    const size_t& get_floor() const { return cur_floor; }
    const size_t& get_code() const{ return code; }
    
    size_t get_seltd_floor_num() const { return selectedfloor.size(); }
    const vector<Passenger*> &get_pasgr() const { return pasgr; }
    
    void run(size_t);                                                   // 每一秒进行判断状态
    void check (const vector<Passenger*>&);                             // 如果现在没有乘客，判断是否有乘客需要自己去接

    //第4次新增
    const size_t &get_run_time() const { return running_time; }               
    const size_t &get_rest_time() const { return resting_time; }  

private:
    bool isselected;                        // 电梯是否已被选中
    bool isempty;                           // 电梯是否为空
    bool direction_up;                      // 电梯方向，真为向上，假向下
    bool between_floors;                    // 是否正在上升或下降过程（楼间）
    bool waiting_pas;                       // 是否正在等待乘客上下电梯
    bool initial;                           // 是否为初始状态（还未接待乘客）

    size_t code;                            // 电梯编号
    size_t count;                           // 计数  默认为0
    size_t wait_time;                       // 上下楼、等候乘客上下电梯时使用   初始化为0
    size_t cur_floor;                       // 目前停留楼层
    size_t dest_floor;                      // 下一个停靠层（电梯没人而去接人时的状态）

    size_t new_pas_num;                     // 该层新上来的乘客数  初始置0
    size_t lev_pas_num;                     // 该层下去的乘客数  初始置0

    vector<Passenger*> pasgr;               // 乘客vec，默认初始化为空   传指针，使后面方面修改原值
    //selected_floor可以优化成set，从而减少每次比较的时间
    vector<size_t> selectedfloor;           // 电梯为空时要去接的乘客的楼层，默认初始化为空

    void push_pasgr(Passenger&);            // 添加乘客，设置new_pas_num                               
    void pop_pasgr();                       // 删除乘客，设置lev_pas_num

    //工具函数
    void refresh();                         // 用于动态更新电梯中的乘客的当前楼层

    //第2次新增
    bool first_dir_up;                      // 第一个按下按钮的人的乘坐方向
    bool set;                               // 判断是否是第一个按下按钮的人，初始为true，后续为false
    size_t first_pas_code;                  // 记录第一个按下按钮的乘客的编号，初始置为0
    
    //第3次新增
    bool frame_chose;                       // 帧选中，用于区分同一时刻乘客先后按下按钮的情况
    bool enabled_floor(const size_t &);     // 传入楼层，判断是否可以停靠，可以停靠则返回true

    //第4次新增
    size_t running_time;                    // 总计运行时间，默认为0
    size_t resting_time;                    // 总计休息时间，默认为0

};
size_t Elevator::elevator_code = 0;         // 编号为0-9  分离编译时要写到cpp文件中，不能写在.h文件中



class Passenger{
    friend void choose_elevator_first(Passenger &);                 // 乘客选择电梯，返回电梯编号
    friend void choose_elevator(Passenger &);                       // 乘客选择电梯，返回电梯编号，并设置等待时间
    friend class Elevator;
    static size_t pas_code;

public:
    static size_t pas_remained;                                     // 剩余未到达目的地乘客数
    Passenger();

    void run(size_t, vector<Elevator>&);                            // 每秒判断状态
    bool finished() const { return finishd; };                      // 判断是否完成所有任务
    
    const size_t &getcode() const { return code; }
    const size_t &getdest() const { return dest_floor; }
    const size_t &getcur() const { return cur_floor; }
    const size_t &get_ele_num() const { return ele_code; }
    const size_t &get_arrive_time() const { return arrive_time; }
    const size_t &get_wait_time() const { return wait_time; }
    const size_t &get_L() const { return L; }
    const size_t &get_took_time() const { return took_time; }
    const size_t &get_counter() const { return counter; }

    //第4次新增
    vector<size_t> get_vec() const { return pas_resting_time_vec; }

private:
    bool arrived;                                   // 初始时是否已经到达1层                        
    bool on_ele;                                    // 是否在电梯上
    bool finishd;                                   // 乘客结束仿真
    size_t code;                                    // 乘客编号
    size_t dest_floor;                              // 目标楼层（后续可更改   注意不能随机到当前的楼层）  
    size_t cur_floor;                               // 目前楼层（跟随电梯）   
    size_t arrive_time;                             // 乘客等待电梯时间（包括第一次）
    size_t ele_code;                                // 将乘坐的电梯的编号

    bool pressup() const { return dest_floor > cur_floor; }     // 是否按下向上键
    
    //第2次上机新增
    size_t L;                                       // 要乘坐的电梯的次数
    size_t took_time;                               // 乘坐过的电梯的次数，初始化为0
    size_t wait_time;                               // 在楼层上等待的时间
    size_t counter;                                 // 等待时的计数器，初始化为0

    //第3次上机新增
    bool chosen_ele;                                // 等待结束后是否已经选择好了电梯

    // 工具函数
    // size_t cal_wait_time(const Elevator &);      // 计算等待时间

    //第4次上机新增
    vector<size_t> pas_resting_time_vec;            // 乘客发出乘客要求后的等待时间的集合
    size_t tmp_time;                                // 记录发出要求的时间，初始=arrive_time
    bool press_button;                              // 是否按下按钮
};
size_t Passenger::pas_remained = N;
size_t Passenger::pas_code = 1;
// vector<Passenger> Passenger::wait_list;


vector<Passenger*> pas_wait_list;                   // 仍在等待的名单
vector<Elevator> ele_vec(ELE_NUM);                  // 初始化10个电梯


void push_wait_list(Passenger *p){
    pas_wait_list.push_back(p);
}

void pop_wait_list(Passenger *p){
    auto it = std::find_if(pas_wait_list.begin(), pas_wait_list.end(),[&p](Passenger *pas){ return pas->getcode() == p->getcode();});
    if(it != pas_wait_list.end())
        pas_wait_list.erase(it);
}


//第3次上机新增
bool Elevator::enabled_floor(const size_t &floor){
    switch (code){                                          // 对电梯的code进行判断传入的楼层是否可达
    case 0: case 1:
        return true;
    case 2: case 3:
        if(floor == 1 || floor >=25)
            return true;
        else
            return false;
    case 4: case 5:
        if(floor <= 25)
            return true;
        else
            return false;
    case 6: case 7:
        if(floor == 1 || (floor % 2 == 0))
            return true;
        else
            return false;
    case 8: case 9:
        if(floor % 2 == 1)
            return true;
        else
            return false;
    default:
        return true;
    }
}


// 乘客类来进行使用
// 上来的乘客一定是顺着电梯方向的
void Elevator::push_pasgr(Passenger& p){
    pasgr.push_back(&p);
    isempty = false;
    
    //如果在selectedfloor列表上，从selectedfloor上将其删除
    auto it = std::find_if(ele_vec[p.ele_code].selectedfloor.begin(),ele_vec[p.ele_code].selectedfloor.end(),[&p](const size_t &a){return a == p.cur_floor;});
    if(it != ele_vec[p.ele_code].selectedfloor.end())
        ele_vec[p.ele_code].selectedfloor.erase(it);

    if(p.code == first_pas_code){               //如果是该电梯空乘客时第一个按下按钮的乘客
        set = true;                             //解锁
        // first_pas_code = 0;                     //置零
    }

    //如果目前电梯只有1人，那么电梯运行方向就是该乘客要去的方向
    if(pasgr.size() == 1){                      
        direction_up = p.pressup();
        first_dir_up = p.pressup();
    }

    //新增乘客数+1
    ++new_pas_num;          
}
//注意修改电梯信息empty，判断是否修改方向，选中，下一个停靠层


//电梯类进行使用，返回下去的人数，并设置该乘客下一次的数据，每次run时使用即可
void Elevator::pop_pasgr(){
    static size_t count = 0;                                    //下去乘客的人数
    count = 0;
    auto it = std::find_if(pasgr.begin(),pasgr.end(),[this](Passenger *p){return cur_floor == p->dest_floor;});
    while(it != pasgr.end()){
        ++count;

        if((*it)->code == first_pas_code)                           //第一位乘客下电梯，则该数重置为0
            first_pas_code = 0; 
        //设置 *下次的* 状态
        (*it)->took_time++;                                         //这是第几次
        (*it)->on_ele = false;
        (*it)->chosen_ele = false;                                  //电梯重置为还未选择
        (*it)->ele_code = ELE_NUM;                                  //类似于置位，防止有其他事件发生

        // (*it)->wait_time = rand() % 111 + 10;                    //设计下次的随机等待时间
        (*it)->wait_time = rand() % 11 + 10;                        //设计下次的随机等待时间（先按10-20算）
        (*it)->counter = 0;                                         //计数器清零
        if((*it)->took_time < (*it)->L){                            //次数小于L次，随机生成下次的dest_floor（包括第L次）
            (*it)->dest_floor = rand() % 39 + 2;
            while((*it)->cur_floor == (*it)->dest_floor)            //如果随机到这层，重新随机
                (*it)->dest_floor = rand() % 39 + 2;
        }
        else if((*it)->took_time == (*it)->L){                      //第L次下电梯，设置L+1次的状态
            (*it)->dest_floor = 1;
        }
        else{                                                       //第L+1次下电梯
            (*it)->wait_time = 0;                                   //状态清0
            (*it)->finishd = true;                                  //乘客到达1层，结束仿真
        }

        if(!(*it)->finished())                                      //如果仿真还没结束，再加回wait_list 
            pas_wait_list.push_back(*it);
        else if((*it)->cur_floor == (*it)->dest_floor)              //如果isfinished，且已经到达目的楼层（1层）
            --Passenger::pas_remained;                              //剩余的乘客数-1
        
        pasgr.erase(it);
        it = std::find_if(pasgr.begin(),pasgr.end(),[this](Passenger *p){return cur_floor == p->dest_floor;});
    }
    lev_pas_num = count;                                            //记录 下电梯的乘客数
}


void Elevator::refresh(){
    for(auto &p : pasgr){
        p->cur_floor = cur_floor;
    }
}


//更新new_pas_num 和 lev_pas_num
//设置下一时刻电梯的状态

void Elevator::run(size_t cur_time){
    // 这边不能设置static count = 0         有问题！！     //所有类的对象在调用该函数时共享这个变量
    
    //每一时刻开始，刷新帧选中
    frame_chose = false;

    if(pasgr.size() != 0)
        initial = false;
    if(initial){                                        //电梯处于初始状态，直接返回
        ++resting_time;
        return;
    }
    check(pas_wait_list);                               //检查该时刻是否有人需要电梯来接
    if(!isselected){                                    //没有则返回
        waiting_pas = true;
        between_floors = false;
        ++resting_time;                                 //等待时间++
        return;
    }

    //如果没有退出，即为被选中
    ++running_time;                                     //运行时间++

    //01 判断是否为等待部分
    if(waiting_pas){
        // cout << code << " wp" << endl;
        pop_pasgr();                                                //先让乘客下去，得到下去的人数，上来的人数由乘客类提供
        wait_time += (new_pas_num + lev_pas_num) * T;               //设置等待时间

        //中间层状态        
        if(new_pas_num + lev_pas_num == 0 && wait_time == 0){       //如果这一时刻既不上来乘客也不下去乘客且不需要再等待  直接往下一层走
            waiting_pas = false;
            between_floors = true;
            count = 0;
            goto FLAG;                                  //进入上升或者下降循环
        }

        //等待层状态
        if(count >= wait_time){                         //等待完成，执行下一步
            count = 0;
            new_pas_num = 0;                            //清空状态
            lev_pas_num = 0;                            //清空状态
            waiting_pas = false;
            between_floors = true;
            return;
        }
        ++count;                                        //每一秒count + 1
        new_pas_num = 0;
        lev_pas_num = 0;
        return;
    }
    //02 判断是否为上升、下降部分
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
    static bool sign;                                                                           //以及是否与该乘客同楼层
    sign = false;
    if(pasgr.size() == 0){
        //搜索wait_list中是否有乘客需要自己来接，优先朝第一个按下按钮的乘客（使用push_back，后进来的wait_list在后面）
        for(const auto &p : wait_list){
            if(p->counter >= p->wait_time && p->ele_code == code){                              //乘客选中电梯且乘客已经等待完毕（此时按下按钮），找第一个按按钮的乘客
                if(cur_floor == p->cur_floor){
                    wait_time = 2;                                                              //如果在这层：电梯不停等待
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
            set = true;                                     //如果一直没被选中,set不断还原为true，如果只在乘客类加的话，加入乘客前set为false，加不进去，而只有进去了才能变成true
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
                between_floors = true;                      //设置成false : 以便在当前层的乘客可以直接上来（配合run中的更改waiting_pas状态使用） 
            }
        }
    }
}
//*****************************************************


//乘客选择电梯部分
void choose_elevator_first(Passenger& p){
    vector<size_t> enabled_ele;                             //可以选择的电梯集合
    int i = 0;
    //如果该电梯可以到达该楼层，则添加到集合中
    for (; i < ELE_NUM; ++i)
        if(ele_vec[i].enabled_floor(p.dest_floor))        
            enabled_ele.push_back(i);
    size_t sz = enabled_ele.size();
    p.ele_code = enabled_ele[rand() % sz];                  //随机生成初始选择的电梯编号(下标为0 ~ sz-1)
    ele_vec[p.ele_code].isselected = true;                  //让该电梯被选中
}


void choose_elevator(Passenger &p){
    //有电梯未被选中且符合条件且最近
    vector<size_t> en_ele;
    size_t i = 0, least = 0, last = 0;                      //least表示所有电梯中距自己最近的楼层差值
    bool first = true, all_is_selected = true;
    for (; i < ELE_NUM; ++i){
        if(ele_vec[i].enabled_floor(p.dest_floor) && ele_vec[i].enabled_floor(p.cur_floor)){         //可以到当前楼层，且可以去到dest_floor，则加入集合
            en_ele.push_back(i);
            //如果该电梯未被选中或被选中但仍initial，且未被帧选中
            if ((!ele_vec[i].isselected || (ele_vec[i].isselected && ele_vec[i].initial))&& !ele_vec[i].frame_chose){            
                if(first){                                  //如果是第一个,直接设置least
                    first = false;
                    last = i;
                    ele_vec[i].frame_chose = true;
                    all_is_selected = false;                //说明有电梯空闲
                    least = abs(int(ele_vec[i].cur_floor) - int(p.cur_floor));      //强制类型转换下，abs不对size_t类型有效
                    p.ele_code = i;
                }
                else{                                       //不是第一个，比较后设置least  
                    int tmp = abs(int(ele_vec[i].cur_floor) - int(p.cur_floor));
                    if(tmp < least){
                        least = tmp;
                        p.ele_code = i;
                        ele_vec[last].frame_chose = false;
                        ele_vec[i].frame_chose = true;
                        last = i;
                    }
                }
            }
        }
    }
    //都在运行的话随机选择一个可以去的电梯
    if(all_is_selected)                                     //如果所有电梯都正在被使用
        p.ele_code = en_ele[rand() % (en_ele.size())];      //随机在可以去到的电梯中选择一个等待
}


//初始化乘客对象
Passenger::Passenger() : arrived(false), code(pas_code++), cur_floor(1), on_ele(false), finishd(false), chosen_ele(true), press_button(false) {
    arrive_time = rand() % M;                               //生成0-M的到达时间
    dest_floor = rand() % 39 + 2;                           //生成2-40的随机目标楼层
    //电梯规则更改后，乘客初始选择电梯：
    choose_elevator_first(*this);
    L = rand() % MAX_Times + 1;                             //随机生成要乘坐的次数，10太多，生成3次以内
    push_wait_list(this);                                   //加入等候名单
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

    //记录下按下按钮的时间
    if(!press_button){
        press_button = true;
        tmp_time = cur_time;
    }

    //选择新的电梯，设置新的ele_code(设置完之后直到下次等待前都不再重新选择楼层)
    if(!chosen_ele){
        choose_elevator(*this);
        ele_vec[ele_code].initial = false;              //更改电梯初始状态
        chosen_ele = true;
        return;                                         //防止此次就直接上电梯，从而避免电梯一直处于未被选中的状态
    }
    // if(!ele_vec[ele_code].isselected)                   //如果又变成未被选中了，再改回去，下帧重新选择
        // chosen_ele = false;

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

    //记录下按下按钮到上电梯的等待时间
    pas_resting_time_vec.push_back(cur_time - tmp_time);
    press_button = false;

    ele_vec[ele_code].push_pasgr(*this);                //把自己插入到电梯类中
    on_ele = true;
}


int main(){
    srand((unsigned)time(NULL));                        //设置随机数种子
    vector<Passenger> pasgr_vec(N);                     //初始化N个乘客，电梯已经在一开始全局声明了
    
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
    system("pause");                                    //操作以继续

    size_t timer = 0;                                   //计时器
    
    while(Passenger::pas_remained != 0){                //进行时间循环，当没有乘客需要坐电梯时结束仿真
        system("cls");
        //运行部分: 先乘客, 后电梯
        for(auto &p : pasgr_vec)
            if(!p.finished())                           //如果还没结束，接着运行该乘客
                p.run(timer, ele_vec);
        for(auto &e : ele_vec)
            e.run(timer);
        
        //打印部分
        for(const auto & e : ele_vec){
            //调试时显示状态用
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
            for(const auto &p : pas_wait_list){
                cout << p->getcode() << "\tarrives at " << p->get_arrive_time() << "\tselects elevator ";
                if(p->get_counter() < p->get_wait_time())
                    cout << "/";
                else if(p->get_ele_num() == ELE_NUM)
                    cout << "/";
                else
                    cout << p->get_ele_num();
                cout << "\tcurrent_floor: " << p->getcur() << "\tdest_floor: " << p->getdest() << "\t  wait_time: " << p->get_wait_time() << "\t  Total_took_time_L: " << p->get_L() << "\ttook_time: " << p->get_took_time()
                     << "\tis_ready:" << ((p->get_counter() >= p->get_wait_time()) ? " Yes " : " No ") << endl;
            }
            cout << endl;
        }

        cout << "Number of Remained Passengers: " << Passenger::pas_remained << endl;
        cout << endl;
        cout << "current time: " << timer << endl;
        cout << endl;
        ++timer;

        //播放
        if(MODE == 1)
            system("pause");                                    //手动播放
        else if(MODE == 2)
            Sleep(1000/play_speed);                             //自动播放，可以更改播放速度
        else{
            cerr << "MODE FAULT!" << endl;
            return -1;
        }

    }

    //结束后，打印各个电梯的运行时间和等待时间，以及乘客按下按钮后的等待时间
    cout << endl;
    for(const auto & e : ele_vec)
        cout << "Elevator " << e.get_code() << "\trunning time: " << e.get_run_time() << "  \tresting time: " << e.get_rest_time() - 1 << endl;
    cout << endl;
    for(const auto &p: pasgr_vec){
        cout << "Passenger " << p.getcode() << "\twaiting time:\t";
        for(const auto &a : p.get_vec())
            cout << a << ", ";
        cout << endl;
    }
    cout << endl;
    cout << "Finished! :D" << endl;
    return 0;
}