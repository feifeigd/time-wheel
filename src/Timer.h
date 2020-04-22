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

	uint64_t	delay;	///< 多少毫秒之后执行，相对于TimeWheelMgr::last_tick
	uint8_t		rotation_count{};
	using Callback = std::function<void()>;
	Callback callback;
	bool m_own;	// 要释放内存

	bool running()const;
	void stop();

public:
	Timer(uint64_t interval, Callback&& _callback, bool _own = false);
	Timer(uint64_t _delay, uint64_t _periodic_delay,Callback&& _callback, bool _own = false);
	~Timer();

	bool own()const { return m_own; }
	int16_t repeat = 1;	///< -1表示无限次
	uint64_t periodic_delay = 0;
};
