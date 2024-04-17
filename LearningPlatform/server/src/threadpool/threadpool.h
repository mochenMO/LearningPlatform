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
	std::queue<std::function<void()>> m_taskQueue;    // 注意：如果定义的返回值是void，那么可以将任意返回值的函数作为任务函数，但也因此实际无法接收返回值，如果需要，则请额外多传一个参数作为返回值
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

	template<typename _Fn, typename... _Args>   // 模板函数要写在头文件中
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



