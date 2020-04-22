#include "Timer.h"
#include "TimeWheel.h"
#include <cassert>
#include <iostream>
using namespace std;

#define MIN_WHEEL_SIZE 1
#define MAX_WHEEL_SIZE 4096

TimeWheel::TimeWheel(TimeWheelMgr& _mgr, uint16_t _index, std::string _name, uint16_t _wheel_size, Granularity _granularity) : mgr(_mgr), index(_index), name(_name), wheel_size(_wheel_size), granularity(_granularity)
{
	spokes = new tw_links[wheel_size];
	for (size_t i = 0; i < wheel_size; i++)	// wheel_size 个空的双链表
	{
		tw_links& spoke = spokes[i];
		spoke.next = &spoke;
		spoke.prev = &spoke;
	}
}

TimeWheel::~TimeWheel() {
	// 如果先于timer析构，则要先分离timer
	for (size_t i = 0; i < wheel_size; ++i)
	{
		tw_links* spoke = spokes + i;
		Timer* timer = (Timer*)spoke->next;
		while ((tw_links*)timer != spoke)
		{
			timer->stop();
			if ( timer->own())
				delete timer;
			timer = (Timer*)spoke->next;
		}
	}
	delete[] spokes;
}

void TimeWheel::update() {
	m_ticks += TimeWheelMgr::TW_RESOLUTION;
	while (m_ticks >= granularity)
	{
		m_ticks -= granularity;
		tick();
	}
}

void TimeWheel::tick() {
	bool tigger_next_level = false;
	if (++spoke_index == wheel_size) {	// 跑一圈，要执行下一层
		spoke_index = 0;
		tigger_next_level = true;
	}
	//cout << "tick()：index=" << index << ",spoke_index=" << spoke_index << endl;	// 这里index大概率是0
	/*if (tigger_next_level) {	// 驱动下一级
		uint16_t idx = index + 1;
		if (idx < TIME_WHELL_COUNT)
		{
			cout << "触发下一级：index=" << index << endl;	// 这里index大概率是0
			mgr.tws[idx]->tick();
		}
	}*/

	tw_links* spoke = spokes + spoke_index;
	Timer* timer = (Timer*)spoke->next;
	while ((tw_links*)timer != spoke)
	{
		tw_links* next = timer->links.next;
		
		if (timer->rotation_count != 0)
		{
			--timer->rotation_count;
		}
		else {
			timer->delay %= granularity;
			timer->stop();
			if (timer->delay > mgr.tws[0]->granularity)
			{
				cout << "迁移：index=" << index << ",spoke_index=" << spoke_index << endl;
				mgr.AddTimer(*timer);	// 下一级会往前一级迁移
			}
			else {
				cout << "到期：index=" << index << ",spoke_index=" << spoke_index << endl;	// 这里index大概率是0
				if (-1 != timer->repeat)
				{
					--timer->repeat;
				}
				if (timer->repeat && timer->periodic_delay > 0)	// 重复执行
				{
					timer->delay += timer->periodic_delay;
					mgr.AddTimer(*timer);
				}
				if (timer->callback)
					timer->callback();
				if (!timer->repeat && timer->own())
					delete timer;
			}
		}
		timer = (Timer*)next;
	}
}

void TimeWheel::AddTimer(Timer& timer, uint32_t ticks) {
	timer.rotation_count = ticks / wheel_size;
	uint32_t td = ticks % wheel_size;
	uint32_t cursor = (spoke_index + td) % wheel_size;
	tw_links& spoke = spokes[cursor];

	tw_links* prev = spoke.prev;
	timer.links.next = &spoke;
	timer.links.prev = prev;
	prev->next = (tw_links*)& timer;
	spoke.prev = (tw_links*)& timer;
}
