#include"Passenger.h"
#include"Elevator.h"
#include"Settings.h"
using namespace std;

size_t Passenger::pas_remained = N;
size_t Passenger::pas_code = 1;
// vector<Passenger> Passenger::wait_list;

vector<Passenger*> pas_wait_list;                   // 仍在等待的名单(直接值拷贝即可)  静态变量和全局变量需要在cpp中初始化
extern vector<Elevator> ele_vec;                  	// 初始化10个电梯，注意这边不能再有(ELE_NUM)，不然是定义了！  这里应该是声明



void push_wait_list(Passenger *p) {
	pas_wait_list.push_back(p);
}

void pop_wait_list(Passenger *p) {
	auto it = std::find_if(pas_wait_list.begin(), pas_wait_list.end(), [&p](Passenger *pas) { return pas->getcode() == p->getcode(); });
	if (it != pas_wait_list.end())
		pas_wait_list.erase(it);
}


//乘客选择电梯部分
void choose_elevator_first(Passenger& p) {
	vector<size_t> enabled_ele;                             //可以选择的电梯集合
	int i = 0;
	//如果该电梯可以到达该楼层，则添加到集合中
	for (; i < ELE_NUM; ++i)
		if (ele_vec[i].enabled_floor(p.dest_floor))
			enabled_ele.push_back(i);
	size_t sz = enabled_ele.size();
	p.ele_code = enabled_ele[rand() % sz];                  //随机生成初始选择的电梯编号(下标为0 ~ sz-1)
	ele_vec[p.ele_code].isselected = true;                  //让该电梯被选中
}

void choose_elevator(Passenger &p) {
	//有电梯未被选中且符合条件且最近
	vector<size_t> en_ele;
	size_t i = 0, least = 0, last = 0;                      //least表示所有电梯中距自己最近的楼层差值
	bool first = true, all_is_selected = true;
	for (; i < ELE_NUM; ++i) {
		if (ele_vec[i].enabled_floor(p.dest_floor) && ele_vec[i].enabled_floor(p.cur_floor)) {         //可以去到dest_floor，则加入集合
			en_ele.push_back(i);
			//如果该电梯未被选中或被选中但仍initial，且未被帧选中
			if ((!ele_vec[i].isselected || (ele_vec[i].isselected && ele_vec[i].initial)) && !ele_vec[i].frame_chose) {
				if (first) {                                  //如果是第一个,直接设置least
					first = false;
					last = i;
					ele_vec[i].frame_chose = true;
					all_is_selected = false;                //说明有电梯空闲
					least = abs(int(ele_vec[i].cur_floor) - int(p.cur_floor));      //强制类型转换下，abs不对size_t类型有效
					p.ele_code = i;
				}
				else {                                       //不是第一个，比较后设置least  
					int tmp = abs(int(ele_vec[i].cur_floor) - int(p.cur_floor));
					if (tmp < least) {
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
	if (all_is_selected)                                     //如果所有电梯都正在被使用
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


void Passenger::run(size_t cur_time, vector<Elevator>& ele_vec) {
	//已经到达目的地，则返回
	if (finishd)
		return;
	if (cur_time >= arrive_time)
		arrived = true;

	//如果在电梯上或仍未到现场：返回（前者交给电梯类处理）
	if (on_ele || !arrived)
		return;

	if (counter < wait_time) {                            //处理等待时间，完毕之后按下按钮
		++counter;
		return;
	}

	//记录下按下按钮的时间
	if (!press_button) {
		press_button = true;
		tmp_time = cur_time;
	}

	//选择新的电梯，设置新的ele_code(设置完之后直到下次等待前都不再重新选择楼层)
	if (!chosen_ele) {
		choose_elevator(*this);
		ele_vec[ele_code].initial = false;              //更改电梯初始状态
		chosen_ele = true;
		return;                                         //防止此次就直接上电梯，从而避免电梯一直处于未被选中的状态
	}
	// if(!ele_vec[ele_code].isselected)                   //如果又变成未被选中了，再改回去，下帧重新选择
		// chosen_ele = false;

	//选择的电梯不在同一层楼，等待
	if (ele_vec[ele_code].get_floor() != cur_floor)
		return;
	//若电梯正在楼层间，等待
	if (ele_vec[ele_code].between_floors == true)
		return;
	//若上电梯导致超重，等待
	if (ele_vec[ele_code].get_pasgr().size() + 1 > MAXPEOPLE)
		return;
	//若电梯与自己的要去的方向不同且电梯上有人，等待（没人说明是专门来接的，方向可能会不同，但可以上电梯）
	if (ele_vec[ele_code].dir_up() != pressup() && ele_vec[ele_code].pasgr.size() != 0)
		return;
	//如果方向不同且电梯无人，但自己不是电梯要接的第一个乘客，返回
	if (ele_vec[ele_code].dir_up() != pressup() && ele_vec[ele_code].pasgr.size() == 0 && ele_vec[ele_code].first_pas_code != code)
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