#pragma once
#include "TimeWheelMgr.h"
#include <string>

struct tw_links;

class TimeWheel {
	std::string		name;
	uint16_t		wheel_size;		// 时间轮有多少格, 最多一毫秒一格，一圈1000格
	uint16_t		spoke_index{};	// 当前执行的索引	
	Granularity		granularity;	// 粒度，单位毫秒, 一个tick的毫秒数
	TimeWheelMgr&	mgr;
	uint16_t const	index;	// 在TimeWheelMgr中的索引
	uint64_t		m_ticks{};
	friend class TimeWheelMgr;	

	tw_links* spokes;
	void update(); // 每次增加TW_RESOLUTION毫秒
	void tick();
	/// @param ticks 放进延迟多少格子的链表处
	void AddTimer(Timer& timer, uint32_t ticks);
public:
	TimeWheel(TimeWheelMgr& _mgr, uint16_t _index, std::string _name, uint16_t _wheel_size, Granularity _granularity);
	~TimeWheel();
};
