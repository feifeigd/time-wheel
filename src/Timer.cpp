#include "Timer.h"


Timer::Timer(uint64_t _delay, Callback&& _callback) : delay(_delay), callback(_callback)
{

}

Timer::~Timer() {
	stop();
}

//void Timer::prepare() {
//
//}

bool Timer::running()const {
	return !!links.next;
}

void Timer::stop() {
	tw_links* next = links.next;
	if (next) {
		tw_links* prev = links.prev;
		next->prev = prev;
		prev->next = next;
		links.next = nullptr;
		links.prev = nullptr;
	}
}