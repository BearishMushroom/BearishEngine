#ifndef _BEARISH_CORE_JOBFACTORY_H_
#define _BEARISH_CORE_JOBFACTORY_H_

#include "../Types.h"
#include "ConcurrentQueue.h"
#include "Callback.h"
#include "Logger.h"

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <future>

namespace Bearish { namespace Core {
	class JobFactory {
	public:
		JobFactory() {
			_active = true;
			_numWorkers = std::thread::hardware_concurrency() - 2;
			Logger::Info("JobFactory created with %d worker threads.", _numWorkers);

			for (i32 i = 0; i < _numWorkers; i++) {
				_workers.push_back(std::thread(&JobFactory::DoWork, this));
			}
		}

		~JobFactory() {
			Terminate();
		}

		template<typename TIn, typename TOut>
		std::future<TOut> AddJob(Action<TIn*, TOut*> fn, TIn params) {
			std::future<TOut> fut;
			try {
				auto p = std::make_shared<std::promise<TOut>>();
				fut = p->get_future();

				Job job;

				auto jorb = [func = std::move(fn)](std::shared_ptr<std::promise<TOut>> prom, void* in, void* out) mutable -> void {
					func((TIn*)in, (TOut*)out);
					prom->set_value(*((TOut*)out));
				};

				job.function = std::bind(jorb, p, std::placeholders::_1, std::placeholders::_2);

				job.in = malloc(sizeof(TIn));
				memcpy(job.in, &params, sizeof(TIn));
				job.out = malloc(sizeof(TOut));

				_queue.Push(job);
			}
			catch (std::exception e) {
				printf(e.what());
				throw;
			}
			return fut;
		}

		template<typename TIn>
		void AddJob(Action<TIn*> fn, TIn params) {
			Job job;
			job.function = [fn](void* in, void* out) -> void {
				fn((TIn*)in);
			};

			job.in = malloc(sizeof(TIn));
			memcpy(job.in, &params, sizeof(TIn));
			job.out = 0;

			_queue.Push(job);
		}

		void AddJob(Action<> fn) {
			Job job;
			job.function = [fn](void* in, void* out) -> void {
				fn();
			};

			job.in = 0;
			job.out = 0;

			_queue.Push(job);
		}

		void Terminate() {
			if (_active) {
				_active = false;
				_queue.Release();
				for (i32 i = 0; i < _numWorkers; i++) {
					_workers.at(i).join();
				}
			}
		}
	private:
		struct Job {
			Action<void*, void*> function;
			void* in, *out;
		};

		void DoWork() {
			while (_active == true) {
				Job job = _queue.Pop();
				try {
					if (job.in || job.out || job.function.Size()) { // make sure we have a valid job
						job.function(job.in, job.out);
						if (job.in) free(job.in);
						if (job.out) free(job.out);
					}
				}
				catch (std::exception e) {
					printf(e.what());
					throw;
				}
			}
		}

		std::atomic<bool> _active;
		i32 _numWorkers;
		std::vector<std::thread> _workers;
		ConcurrentQueue<Job> _queue;
	};
} }

#endif // _BEARISH_CORE_JOBFACTORY_H_
