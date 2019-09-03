
#include "TimeWheelMgr.h"

#include <Windows.h>	// Sleep
#include <cstdint>
#include <iostream>
using namespace std;

void print_now() {
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	printf("%4d/%02d/%02d %02d:%02d:%02d.%03d \n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
}

int main(){
	TimeWheelMgr timerMgr;

	uint64_t interval = 10 * Granularity::Second;
	/*Timer timer(interval, [&]() {		
		print_now();
		cout << "times=" << timerMgr.times() << ",10秒定时器" << endl;
	});
	timerMgr.AddTimer(timer);

	interval = 30 * Granularity::Second;
	Timer timer1(interval, [&]() {
		print_now();
		cout << "times=" << timerMgr.times() << ",30秒定时器" << endl;
	});
	timerMgr.AddTimer(timer1);*/

	interval = 30 * Granularity::Second + 1 * Granularity::Minute;
	Timer timer2(interval, [&]() {
		print_now();
		cout << "times=" << timerMgr.times() << ",1分30秒定时器" << endl;
	});
	 timerMgr.AddTimer(timer2);

	interval = 30 * Granularity::Second + 1 * Granularity::Hour;
	Timer timer3(interval, [&]() {
		print_now();
		cout << "times=" << timerMgr.times() << ",1时30秒定时器" << endl;
	});
	timerMgr.AddTimer(timer3);

	interval = 30 * Granularity::Second + 1 * Granularity::Day;
	Timer timer4(interval, [&]() {
		print_now();
		cout << "times=" << timerMgr.times() << ",1天30秒定时器" << endl;
	});
	timerMgr.AddTimer(timer4);

	interval = 30 * Granularity::Second + 1 * Granularity::Week;
	Timer timer5(interval, [&]() {
		print_now();
		cout << "times=" << timerMgr.times() << ",1星期30秒定时器" << endl;
	});
	timerMgr.AddTimer(timer5);

	print_now();
	while(true){		
		int64_t next_tick = timerMgr.update();
		if(next_tick > 0)
			Sleep(next_tick);
	}
	return 0;
}
