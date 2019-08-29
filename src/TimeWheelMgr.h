#pragma once

enum Granularity : uint64_t
{
	MS = 1,
	Second = 1000,
	Minute = 60 * Second,
};

#define TIME_WHELL_COUNT 2	// 二级时间轮

// 最高精度 1000 毫秒
#define TW_RESOLUTION 1000

class Timer;
class TimeWheel;
class TimeWheelMgr {
	friend class TimeWheel;
	TimeWheel* tws[TIME_WHELL_COUNT];
public:
	TimeWheelMgr();
	~TimeWheelMgr();

	/// 由外部用最高精度TW_RESOLUTION 驱动
	void tick();

	void AddTimer(Timer& timer);
	void KillTimer(Timer& timer);
	
};
