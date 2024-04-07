#pragma once
#ifndef _MOCHEN_TIME_H_
#define _MOCHEN_TIME_H_

#include <Windows.h>

namespace mochen
{

namespace time
{

class Timer   // 是一个单例
{
private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_count;
public:
	Timer();
	~Timer() = default;
	
	Timer(const Timer&) = delete;
	Timer(Timer&&) noexcept = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) noexcept = delete;

	LONGLONG getFrequency();
	LONGLONG getCount();
};




};

};





#endif // !_MOCHEN_TIME_H_
