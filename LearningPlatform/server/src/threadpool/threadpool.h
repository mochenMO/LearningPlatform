#pragma once
#ifndef _MOCHEN_THREADPOOL_H_
#define _MOCHEN_THREADPOOL_H_


#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>

#include <Windows.h>

namespace mochen
{

namespace threadpool
{


class ThreadPool
{
private:
	std::queue<std::function<void()>> m_taskQueue;    // ע�⣺�������ķ���ֵ��void����ô���Խ����ⷵ��ֵ�ĺ�����Ϊ����������Ҳ���ʵ���޷����շ���ֵ�������Ҫ���������ഫһ��������Ϊ����ֵ
	std::vector<std::thread> m_threadArray;
	std::mutex m_mutex;
	int m_maxSize;
	bool m_isStop;
public:
	ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) noexcept = delete;

	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) noexcept = delete;

	~ThreadPool();

	void threadFunction();

	void startup();

	template<typename _Fn, typename... _Args>   // ģ�庯��Ҫд��ͷ�ļ���
	void addTask(_Fn&& _f, _Args&& ..._args)
	{
		auto temp = std::bind(_f, std::forward<_Args>(_args)...);
		m_mutex.lock();
		m_taskQueue.push(temp);
		m_mutex.unlock();
	}
};





};

};




#endif // !_MOCHEN_THREADPOOL_H_



