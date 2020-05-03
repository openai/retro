#pragma once

#include <mutex>
#include <condition_variable>
#include <deque>

template <typename T, typename  Container = std::deque<T>>
class BlockingQueue
{
private:
	std::mutex m_mutex;
	std::condition_variable m_condition;
	Container m_queue;
public:
	void push(T const& value)
	{
		{
			std::unique_lock<std::mutex> lock(this->m_mutex);
			m_queue.push_front(value);
		}
		this->m_condition.notify_one();
	}

	void pushBack(T const& value)
	{
		{
			std::unique_lock<std::mutex> lock(this->m_mutex);
			m_queue.push_back(value);
		}
		this->m_condition.notify_one();
	}

	T pop()
	{
		std::unique_lock<std::mutex> lock(this->m_mutex);
		this->m_condition.wait(lock, [this]{ return !this->m_queue.empty(); });
		T rc(std::move(this->m_queue.back()));
		this->m_queue.pop_back();
		return rc;
	}

	bool tryPop(T & v, std::chrono::milliseconds dur)
	{
		std::unique_lock<std::mutex> lock(this->m_mutex);
		if (!this->m_condition.wait_for(lock, dur, [this]{ return !this->m_queue.empty(); })) {
			return false;
		}
		v = std::move(this->m_queue.back());
		this->m_queue.pop_back();
		return true;
	}

	size_t size()
	{
		return m_queue.size();
	}
};
