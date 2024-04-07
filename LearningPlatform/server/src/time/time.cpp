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
	QueryPerformanceCounter(&m_count);   // ע�⣺�ú������̰߳�ȫ��
	return m_count.QuadPart;
}


