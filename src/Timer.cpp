#include "Timer.h"


Timer::Timer(uint64_t interval, Callback&& _callback, bool _own ) 
	: delay(interval), periodic_delay(interval), callback(_callback), repeat(-1), m_own(_own)
{

}

Timer::Timer(uint64_t _delay, uint64_t _periodic_delay, Callback&& _callback, bool _own ) 
	: delay(_delay), periodic_delay(_periodic_delay), callback(_callback), m_own(_own)
{

}

Timer::~Timer() {
	stop();
}

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