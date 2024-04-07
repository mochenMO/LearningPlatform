#include "threadpool.h"

using namespace mochen::threadpool;

ThreadPool::ThreadPool() : m_taskQueue(), m_threadArray(), m_mutex(), m_isStop(false)
{

}


ThreadPool::~ThreadPool()
{
	InterlockedExchange8((char*)&m_isStop, true);
	for (auto it = m_threadArray.begin(); it != m_threadArray.end(); ++it) {
		it->join();
	}
}


void ThreadPool::threadFuntion()
{
	std::function<void()> function;
	bool isEmpty;

	while(1)
	{
		m_mutex.lock(); 
		isEmpty = m_taskQueue.empty();
		if (isEmpty == false) {
			function = m_taskQueue.front();
			m_taskQueue.pop();
		}
		m_mutex.unlock();

		if (isEmpty == false) {
			function();
			continue;
		}
		if (m_isStop == true) {
			return;
		}
	}
}


void ThreadPool::startup()
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);  // ��ȡϵͳ�������Ϣ������CPU����������������Ҫ����ϸ�ĺ�������Ϣ�������߼�����������ʹ�ú��� GetLogicalProcessorInformation 
	m_maxSize = systemInfo.dwNumberOfProcessors * 2;    // ע�⣺����̨ͨʽ���У�һ�����̵��߳�����һ�㲻������CPU���������������

	for (int i = 0; i < m_maxSize; ++i) {
		std::thread t(&ThreadPool::threadFuntion, this);
		m_threadArray.push_back(std::move(t));    // std::thread ֻ�ܽ����ƶ�����
	}
}
