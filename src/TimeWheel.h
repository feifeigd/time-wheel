#pragma once
#include "TimeWheelMgr.h"
#include <string>

struct tw_links;
class TimeWheelMgr;
class TimeWheel {
	std::string		name;
	uint16_t		wheel_size;		// 时间轮有多少格
	uint16_t		spoke_index = 0;	// 当前执行的索引	
	Granularity		granularity;	// 粒度，单位毫秒, 一个tick的毫秒数
	TimeWheelMgr&	mgr;
	uint16_t		index;	// 在TimeWheelMgr中的索引
	friend class TimeWheelMgr;	

	tw_links* spokes;
	void tick();
	/// @param ticks 放进延迟多少格子的链表处
	void AddTimer(Timer& timer, uint64_t ticks);
public:
	TimeWheel(TimeWheelMgr& _mgr, uint16_t _index, std::string _name, uint16_t _wheel_size, Granularity _granularity);
};
