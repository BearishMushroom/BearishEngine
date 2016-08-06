#ifndef _BEARISH_CORE_CONCURRENTQUEUE_H_
#define _BEARISH_CORE_CONCURRENTQUEUE_H_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace Bearish { namespace Core {
	template <typename T>
	class ConcurrentQueue {
	public:
		T Pop() {
			std::unique_lock<std::mutex> mlock(_mutex);
			while (_queue.empty() && _alive) {
				_cond.wait(mlock);
			}

			if (!_alive) {
				return T();
			}

			auto val = _queue.front();
			_queue.pop();
			return val;
		}

		void Push(const T& item) {
			std::unique_lock<std::mutex> mlock(_mutex);
			_queue.push(item);
			mlock.unlock();
			_cond.notify_one();
		}

		void Release() { // Releases all waiting threads.
			std::unique_lock<std::mutex> lock(_mutex);
			_alive = false;
			lock.unlock();
			_cond.notify_all();
		}

		ConcurrentQueue() : _alive(true) {}
		ConcurrentQueue(const ConcurrentQueue&) = delete;            // disable copying
		ConcurrentQueue& operator=(const ConcurrentQueue&) = delete; // disable assignment
	private:
		std::queue<T>  _queue;
		bool _alive;
		std::mutex _mutex;
		std::condition_variable _cond;
	};
} }
#endif // _BEARISH_CORE_JOBQUEUE_H_