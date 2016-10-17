#ifndef _BEARISH_CORE_JOBFACTORY_H_
#define _BEARISH_CORE_JOBFACTORY_H_

#include <BE/Types.h>
#include <BE/Core/ConcurrentQueue.h>
#include <BE/Core/Callback.h>
#include <BE/Core/Logger.h>

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <future>

namespace Bearish { namespace Core {
	class BEARISH_API JobFactory {
	public:
		JobFactory() {
			Initialize();
		}

		~JobFactory() {
			Terminate();
		}

		static void Initialize();

		template<typename TIn, typename TOut>
		static std::future<TOut> AddJob(Action<TIn*, TOut*> fn, TIn params) {
			std::future<TOut> fut;
			try {
				auto p = Shared<std::promise<TOut>>(new std::promise<TOut>);
				fut = p->get_future();

				Job job;

				auto jorb = [func = std::move(fn)](Shared<std::promise<TOut>> prom, void* in, void* out) mutable -> void {
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
		static void AddJob(Action<TIn*> fn, TIn params) {
			Job job;
			job.function = [fn](void* in, void* out) -> void {
				fn((TIn*)in);
			};

			job.in = malloc(sizeof(TIn));
			memcpy(job.in, &params, sizeof(TIn));
			job.out = 0;

			_queue.Push(job);
		}

		static void AddJob(Action<> fn);

		static void Terminate();
		
		struct Job {
			Action<void*, void*> function;
			void* in, *out;
		};
	private:

		static void DoWork();

		static std::atomic<bool> _active;
		static i32 _numWorkers;
		static std::vector<std::thread> _workers;
		static ConcurrentQueue<Job> _queue;
	};
} }

#endif // _BEARISH_CORE_JOBFACTORY_H_
