#include "Timer.h"
#include "TimeWheel.h"
#include "TimeWheelMgr.h"
#include <cassert>
#include <chrono>

int64_t millseconds()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

TimeWheelMgr::TimeWheelMgr()
{
	tws[0] = new TimeWheel(*this, 0, "100ms", 10, Granularity::MS100);		// 100毫秒秒轮
	tws[1] = new TimeWheel(*this, 1, "second", 60, Granularity::Second);	// 轮
	tws[2] = new TimeWheel(*this, 2, "minute", 60, Granularity::Minute);	// 分轮
	tws[3] = new TimeWheel(*this, 3, "hour",   24, Granularity::Hour);		// 时轮
	tws[4] = new TimeWheel(*this, 4, "day",     7, Granularity::Day);		// 天轮
	tws[5] = new TimeWheel(*this, 5, "week",    4, Granularity::Week);		// 星期轮
}

TimeWheelMgr::~TimeWheelMgr() {
	for (TimeWheel* tw : tws) {
		delete tw;
	}
}

int64_t TimeWheelMgr::update() {
	int64_t now = millseconds();
	if (last_tick < 1)
		last_tick = now;
	while (now > last_tick + TW_RESOLUTION)
	{
		last_tick += TW_RESOLUTION;
		++_times;
		for (TimeWheel* tw : tws) {
			tw->update();
		}
	}
	return last_tick + TW_RESOLUTION - millseconds();
}

// 1年=366天= 366*24*3600*1000 = 31,622,400,000毫秒
void TimeWheelMgr::AddTimer(Timer& timer) {
	TimeWheel* tw = tws[0];
	assert(tw);

	KillTimer(timer);	// 如果之前就在链表里面，要从之前的链表脱离

	uint32_t ticks = 1;	// 往后延迟多少个格子，至少一格，不足一格也算一格
	for (size_t i = TIME_WHELL_COUNT - 1; i >= 0; --i)
	{
		tw = tws[i];
		if (!tw)continue;
		
		if (timer.delay < tw->granularity)
		{
			if (i)
			{
				continue;
			}
		}
		else {
			timer.delay += tw->m_ticks;
			ticks = uint32_t(timer.delay / tw->granularity);
		}
		if (ticks < 1) {
			ticks = 1;
			assert(false);
		}
		tw->AddTimer(timer, ticks);

		break;
	}
}

void TimeWheelMgr::KillTimer(Timer& timer) {
	timer.stop();
}
