#pragma once

#include"Settings.h"
//#include"Passenger.h"		//不能相互包含，见elevator.cpp

#include<vector>
using std::vector;

#include<cstddef>
using std::size_t;

#include<algorithm>
using std::find_if;

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


//size_t Elevator::elevator_code = 0;							// 编号为0-9，静态变量和全局变量需要在cpp中初始化
