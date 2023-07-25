#include<iostream>
#include<vector>
#include<string>
#include<ctime>
#include<cmath>
#include<Windows.h>
#include<algorithm>

#include"Elevator.h"
#include"Passenger.h"
#include"Settings.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

extern vector<Passenger*> pas_wait_list;				//全局和静态变量应该在某个cpp文件中定义，其他地方使用extern声明
extern vector<Elevator> ele_vec;						// 初始化10个电梯, 注意这边不能再有(ELE_NUM)，不然是定义了！  这里应该是声明

int main() {
	srand((unsigned)time(NULL));                        //设置随机数种子
	vector<Passenger> pasgr_vec(N);                     //初始化N个乘客，电梯已经在一开始全局声明了

	cout << "Simulation starts, passengers' information are as follows:" << endl << endl;
	for (const auto &p : pasgr_vec)
		cout << p.getcode() << "--" << p.getdest() << " arrives at " << p.get_arrive_time() << " , selects elevator " << p.get_ele_num() << endl;
	cout << endl;
	for (const auto & e : ele_vec) {
		cout << "Elevator " << e.get_code() << " (cur_floor: " << e.get_floor() << ") Passengers:\t";
		for (const auto & p : e.get_pasgr())
			cout << p->getcode() << "--" << p->getdest() << " ;\t";
		cout << endl;
	}
	system("pause");                                    //操作以继续

	size_t timer = 0;                                   //计时器

	while (Passenger::pas_remained != 0) {                //进行时间循环，当没有乘客需要坐电梯时结束仿真
		system("cls");
		//运行部分: 先乘客, 后电梯
		for (auto &p : pasgr_vec)
			if (!p.finished())                           //如果还没结束，接着运行该乘客
				p.run(timer, ele_vec);
		for (auto &e : ele_vec)
			e.run(timer);

		//打印部分
		for (const auto & e : ele_vec) {
			//调试时显示状态用
			cout << "Elevator " << e.get_code() << ((e.get_pasgr().size() == 0 && e.get_seltd_floor_num() == 0) ? " - " : (e.dir_up() ? " ↑ " : " ↓ ")) << "\t"
				<< "(cur_floor:" << e.get_floor() << ")" << "\tfirst_pas_code: " << e.get_fpc()
				<< "\twait_time: " << e.get_waittime() << "\t Passengers: \t";
			for (const auto & p : e.get_pasgr())
				cout << p->getcode() << "--" << p->getdest() << ";\t";
			cout << endl;
			//调试用代码
			cout << "isselected:" << (e.selected() ? " true" : " false") << "\twaiting_pas:" << (e.wp() ? " true" : " false") << "\tbetween_floor:" << (e.get_bf() ? " true" : " false") << "\tset: " << (e.getset() ? " true" : " false") << endl;
			cout << endl;
		}
		cout << endl;
		cout << "Remained Passengers' List:" << endl;
		if (pas_wait_list.size() == 0)
			cout << "None!" << endl << endl;
		else {
			for (const auto &p : pas_wait_list) {
				cout << p->getcode() << "\tarrives at " << p->get_arrive_time() << "\tselects elevator ";
				if (p->get_counter() < p->get_wait_time())
					cout << "/";
				else if (p->get_ele_num() == ELE_NUM)
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
		if (MODE == 1)
			system("pause");                                    //手动播放
		else if (MODE == 2)
			Sleep(1000 / play_speed);                             //自动播放，可以更改播放速度
		else {
			cerr << "MODE FAULT!" << endl;
			return -1;
		}

	}

	//结束后，打印各个电梯的运行时间和等待时间，以及乘客按下按钮后的等待时间
	cout << endl;
	for (const auto & e : ele_vec)
		cout << "Elevator " << e.get_code() << "\trunning time: " << e.get_run_time() << "  \tresting time: " << e.get_rest_time() - 1 << endl;
	cout << endl;
	for (const auto &p : pasgr_vec) {
		cout << "Passenger " << p.getcode() << "\twaiting time:\t";
		for (const auto &a : p.get_vec())
			cout << a << ", ";
		cout << endl;
	}
	cout << endl;
	cout << "Finished! :D" << endl;
	Sleep(5000);
	return 0;
}