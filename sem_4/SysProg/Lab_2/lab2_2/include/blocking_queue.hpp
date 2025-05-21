#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class BlockingQueue {
public:
	void push(T item) {
		{
			std::lock_guard<std::mutex> lk(mutex_);
			queue_.push(std::move(item));
		}
		cv_.notify_one();
	}

	// Блокирующий pop
	T pop() {
		std::unique_lock<std::mutex> lk(mutex_);
		cv_.wait(lk, [&]{ return !queue_.empty(); });
		T item = std::move(queue_.front());
		queue_.pop();
		return item;
	}

private:
	std::queue<T>           queue_;
	std::mutex              mutex_;
	std::condition_variable cv_;
};
