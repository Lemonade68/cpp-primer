#include"Elevator.h"
#include"Passenger.h"										//这里include  不然报错c2061：   因为两个头文件不能相互包含
#include"Settings.h"

size_t Elevator::elevator_code = 0;							// 编号为0-9，静态变量和全局变量需要在cpp中初始化

extern vector<Passenger*> pas_wait_list;                    // 仍在等待的名单
vector<Elevator> ele_vec(ELE_NUM);							// 初始化10个电梯



//第3次上机新增
bool Elevator::enabled_floor(const size_t &floor) {
	switch (code) {                                          // 对电梯的code进行判断传入的楼层是否可达
	case 0: case 1:
		return true;
	case 2: case 3:
		if (floor == 1 || floor >= 25)
			return true;
		else
			return false;
	case 4: case 5:
		if (floor <= 25)
			return true;
		else
			return false;
	case 6: case 7:
		if (floor == 1 || (floor % 2 == 0))
			return true;
		else
			return false;
	case 8: case 9:
		if (floor % 2 == 1)
			return true;
		else
			return false;
	default:
		return true;
	}
}


// 乘客类来进行使用
// 上来的乘客一定是顺着电梯方向的
void Elevator::push_pasgr(Passenger& p) {
	pasgr.push_back(&p);
	isempty = false;

	//如果在selectedfloor列表上，从selectedfloor上将其删除
	auto it = std::find_if(ele_vec[p.ele_code].selectedfloor.begin(), ele_vec[p.ele_code].selectedfloor.end(), [&p](const size_t &a) {return a == p.cur_floor; });
	if (it != ele_vec[p.ele_code].selectedfloor.end())
		ele_vec[p.ele_code].selectedfloor.erase(it);

	if (p.code == first_pas_code) {               //如果是该电梯空乘客时第一个按下按钮的乘客
		set = true;                             //解锁
		// first_pas_code = 0;                     //置零
	}

	//如果目前电梯只有1人，那么电梯运行方向就是该乘客要去的方向
	if (pasgr.size() == 1) {
		direction_up = p.pressup();
		first_dir_up = p.pressup();
	}

	//新增乘客数+1
	++new_pas_num;
}
//注意修改电梯信息empty，判断是否修改方向，选中，下一个停靠层


//电梯类进行使用，返回下去的人数，并设置该乘客下一次的数据，每次run时使用即可
void Elevator::pop_pasgr() {
	static size_t count = 0;                                    //下去乘客的人数
	count = 0;
	auto it = std::find_if(pasgr.begin(), pasgr.end(), [this](Passenger *p) {return cur_floor == p->dest_floor; });
	while (it != pasgr.end()) {
		++count;

		if ((*it)->code == first_pas_code)                           //第一位乘客下电梯，则该数重置为0
			first_pas_code = 0;
		//设置 *下次的* 状态
		(*it)->took_time++;                                         //这是第几次
		(*it)->on_ele = false;
		(*it)->chosen_ele = false;                                  //电梯重置为还未选择
		(*it)->ele_code = ELE_NUM;                                  //类似于置位，防止有其他事件发生

		// (*it)->wait_time = rand() % 111 + 10;                    //设计下次的随机等待时间
		(*it)->wait_time = rand() % 11 + 10;                        //设计下次的随机等待时间（先按10-20算）
		(*it)->counter = 0;                                         //计数器清零
		if ((*it)->took_time < (*it)->L) {                            //次数小于L次，随机生成下次的dest_floor（包括第L次）
			(*it)->dest_floor = rand() % 39 + 2;
			while ((*it)->cur_floor == (*it)->dest_floor)            //如果随机到这层，重新随机
				(*it)->dest_floor = rand() % 39 + 2;
		}
		else if ((*it)->took_time == (*it)->L) {                      //第L次下电梯，设置L+1次的状态
			(*it)->dest_floor = 1;
		}
		else {                                                       //第L+1次下电梯
			(*it)->wait_time = 0;                                   //状态清0
			(*it)->finishd = true;                                  //乘客到达1层，结束仿真
		}

		if (!(*it)->finished())                                      //如果仿真还没结束，再加回wait_list 
			pas_wait_list.push_back(*it);
		else if ((*it)->cur_floor == (*it)->dest_floor)              //如果isfinished，且已经到达目的楼层（1层）
			--Passenger::pas_remained;                              //剩余的乘客数-1

		pasgr.erase(it);
		it = std::find_if(pasgr.begin(), pasgr.end(), [this](Passenger *p) {return cur_floor == p->dest_floor; });
	}
	lev_pas_num = count;                                            //记录 下电梯的乘客数
}


void Elevator::refresh() {
	for (auto &p : pasgr) {
		p->cur_floor = cur_floor;
	}
}


//更新new_pas_num 和 lev_pas_num
//设置下一时刻电梯的状态

void Elevator::run(size_t cur_time) {
	// 这边不能设置static count = 0         有问题！！     //所有类的对象在调用该函数时共享这个变量

	//每一时刻开始，刷新帧选中
	frame_chose = false;

	if (pasgr.size() != 0)
		initial = false;
	if (initial) {                                        //电梯处于初始状态，直接返回
		++resting_time;
		return;
	}
	check(pas_wait_list);                               //检查该时刻是否有人需要电梯来接
	if (!isselected) {                                    //没有则返回
		waiting_pas = true;
		between_floors = false;
		++resting_time;                                 //等待时间++
		return;
	}

	//如果没有退出，即为被选中
	++running_time;                                     //运行时间++

	//01 判断是否为等待部分
	if (waiting_pas) {
		// cout << code << " wp" << endl;
		pop_pasgr();                                                //先让乘客下去，得到下去的人数，上来的人数由乘客类提供
		wait_time += (new_pas_num + lev_pas_num) * T;               //设置等待时间

		//中间层状态        
		if (new_pas_num + lev_pas_num == 0 && wait_time == 0) {       //如果这一时刻既不上来乘客也不下去乘客且不需要再等待  直接往下一层走
			waiting_pas = false;
			between_floors = true;
			count = 0;
			goto FLAG;                                  //进入上升或者下降循环
		}

		//等待层状态
		if (count >= wait_time) {                         //等待完成，执行下一步
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
	else if (between_floors) {
	FLAG:
		// cout << code << " bf" << endl;
		wait_time = S;                                  //到达下一层的时间
		if (count >= wait_time) {                         //这边改成==会有问题？     为啥
			if (direction_up) {
				++cur_floor;
				refresh();
			}
			else {
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
void Elevator::check(const vector<Passenger*> &wait_list) {
	// 判断是否还有乘客
	static bool sign;                                                                           //以及是否与该乘客同楼层
	sign = false;
	if (pasgr.size() == 0) {
		//搜索wait_list中是否有乘客需要自己来接，优先朝第一个按下按钮的乘客（使用push_back，后进来的wait_list在后面）
		for (const auto &p : wait_list) {
			if (p->counter >= p->wait_time && p->ele_code == code) {                              //乘客选中电梯且乘客已经等待完毕（此时按下按钮），找第一个按按钮的乘客
				if (cur_floor == p->cur_floor) {
					wait_time = 2;                                                              //如果在这层：电梯不停等待
					if (set) {
						sign = true;
						first_pas_code = p->code;                                               //记录该乘客的编号
						direction_up = p->pressup();
						first_dir_up = p->pressup();                                            //设置该乘客将要去的方向
					}
				}
				else {
					if (set) {
						first_pas_code = p->code;
						direction_up = (cur_floor > p->cur_floor) ? false : true;               //设置方向
						first_dir_up = p->pressup();
					}
				}
				//若还未添加过，则添加
				auto it = std::find_if(selectedfloor.begin(), selectedfloor.end(), [&p](const size_t &a) {return a == p->cur_floor; });
				if (it == selectedfloor.end())
					selectedfloor.push_back(p->cur_floor);
				set = false;
			}
		}
		if (selectedfloor.size() == 0) {
			set = true;                                     //如果一直没被选中,set不断还原为true，如果只在乘客类加的话，加入乘客前set为false，加不进去，而只有进去了才能变成true
			isselected = false;
			count = 0;
			wait_time = 0;
			waiting_pas = true;
			between_floors = false;
		}
		else {
			if (sign) {
				isselected = true;
				waiting_pas = true;
				between_floors = false;
			}
			else {
				isselected = true;
				waiting_pas = false;
				between_floors = true;                      //设置成false : 以便在当前层的乘客可以直接上来（配合run中的更改waiting_pas状态使用） 
			}
		}
	}
}
//*****************************************************

