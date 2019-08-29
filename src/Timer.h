#pragma once

#include <cstdint>
#include <functional>

/// 双链表
struct tw_links
{
	tw_links* next;
	tw_links* prev;
};

class TimeWheelMgr;
class Timer {
	tw_links links{};	// 链表指针放在最前面
	friend class TimeWheel;
	friend class TimeWheelMgr;

	uint64_t delay;	///< 多少毫秒之后执行
	uint64_t rotation_count = 0;
	using Callback = std::function<void()>;
	Callback callback;

	void prepare();
	bool running()const;
	void stop();

public:
	Timer(uint64_t _delay, Callback && _callback);
	~Timer();

	int16_t repeat = 1;	///< -1表示无限次
	uint64_t periodic_delay = 0;
};
