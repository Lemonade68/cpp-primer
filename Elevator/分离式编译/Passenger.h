#pragma once

#include"Settings.h"
#include"Elevator.h"

#include<vector>
using std::vector;

#include<cstddef>
using std::size_t;

#include<algorithm>
using std::find_if;

class Passenger {
	friend void choose_elevator_first(Passenger &);                 // 乘客选择电梯，返回电梯编号
	friend void choose_elevator(Passenger &);                       // 乘客选择电梯，返回电梯编号，并设置等待时间
	friend class Elevator;
	static size_t pas_code;

public:
	static size_t pas_remained;                                     // 剩余未到达目的地乘客数
	Passenger();

	void run(size_t, vector<Elevator>&);                            // 每秒判断状态
	bool finished() const { return finishd; };                      // 判断是否到达目的地楼层
	bool pressup() const { return dest_floor > cur_floor; }         // 是否按下向上键

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
void choose_elevator_first(Passenger &);       // 乘客选择电梯，返回电梯编号，并设置等待时间
void choose_elevator(Passenger &);             // 乘客选择电梯，返回电梯编号，并设置等待时间
//需要在cpp中初始化，不能在头文件中初始化
//size_t Passenger::pas_remained = N;
//size_t Passenger::pas_code = 1;
// vector<Passenger> Passenger::wait_list;

