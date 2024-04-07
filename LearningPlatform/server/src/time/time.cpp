#include "time.h"

using namespace mochen::time;


Timer::Timer()
{
	QueryPerformanceFrequency(&m_frequency);
}

LONGLONG Timer::getFrequency()
{
	return m_frequency.QuadPart;
}

LONGLONG Timer::getCount()
{
	QueryPerformanceCounter(&m_count);   // 注意：该函数是线程安全的
	return m_count.QuadPart;
}


