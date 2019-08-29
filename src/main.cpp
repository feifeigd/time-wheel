#include "Timer.h"
#include "TimeWheelMgr.h"

#include <Windows.h>
#include <cstdint>
#include <iostream>
using namespace std;


int main(){
	TimeWheelMgr timerMgr;

	Timer timer(10 * 1000, []() {
		cout << "10秒定时器,tick=" << GetTickCount64() << endl;
	});
	timer.repeat = -1;
	timer.periodic_delay = 10 * 1000;
	timerMgr.AddTimer(timer);

	Timer timer1(30 * 1000, []() {
		cout << "30秒定时器,tick=" << GetTickCount64() << endl;
	});
	timer1.repeat = -1;
	timer1.periodic_delay = 30 * 1000;
	timerMgr.AddTimer(timer1);

	uint64_t last_tick = GetTickCount64();
	cout << "start tick:" << last_tick << endl;
	uint64_t second = 0;
	while(true){
		uint64_t now = GetTickCount64();
		while (now > last_tick + TW_RESOLUTION)
		{
			cout << "second=" << ++second << ",tick=" << GetTickCount64() << endl;
			timerMgr.tick();
			last_tick += TW_RESOLUTION;
		}
		Sleep(66);	// 1000ms / 15 帧 = 66.66
	}
	return 0;
}
