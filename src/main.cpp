#include "Timer.h"
#include "TimeWheelMgr.h"

#include <Windows.h>
#include <cstdint>
#include <iostream>
using namespace std;


int main(){
	TimeWheelMgr timerMgr;

	uint64_t interval = 10 * Granularity::Second;
	Timer timer(interval, []() {
		cout << "10秒定时器,tick=" << GetTickCount64() << endl;
	});
	timer.repeat = -1;
	timer.periodic_delay = interval;
	timerMgr.AddTimer(timer);

	interval = 30 * Granularity::Second;
	Timer timer1(interval, []() {
		cout << "30秒定时器,tick=" << GetTickCount64() << endl;
	});
	timer1.repeat = -1;
	timer1.periodic_delay = interval;
	timerMgr.AddTimer(timer1);

	interval = 30 * Granularity::Second + 1 * Granularity::Minute;
	Timer timer2(interval, []() {
		cout << "1分30秒定时器,tick=" << GetTickCount64() << endl;
	});
	timer2.repeat = -1;
	timer2.periodic_delay = interval;
	timerMgr.AddTimer(timer2);

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
