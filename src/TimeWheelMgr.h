#pragma once

#include "Timer.h"

#include <cstdint>

/// 下级一定要是上级的倍数
enum Granularity : uint64_t
{
	MS		= 1,		// 1 毫秒
	MS100	= 100 * MS,	// 100 毫秒
	Second	= 10 * MS100,
	Minute	= 60 * Second,
	Hour	= 60 * Minute,
	Day		= 24 * Hour,
	Week	= 7 * Day,
};

class TimeWheel;
class TimeWheelMgr {

	enum {
		TIME_WHELL_COUNT = 6,	// 三级时间轮

		// 最高精度 1000 毫秒, 普通应用，可以了
		// 如果是游戏，可以降为100或者10毫秒，然后在秒轮签名插入响应的毫秒轮
		TW_RESOLUTION = 100,
	};
	friend class TimeWheel;
	TimeWheel* tws[TIME_WHELL_COUNT];
	int64_t last_tick{};
	int64_t _times{};	// 开机以来，跑了多少个 TW_RESOLUTION
public:
	TimeWheelMgr();
	~TimeWheelMgr();

	/// 由外部用驱动
	/// @return 距离下次更新还有多少毫秒
	int64_t update();
	int64_t times()const { return _times; }
	void AddTimer(Timer& timer);
	void KillTimer(Timer& timer);
	
};
