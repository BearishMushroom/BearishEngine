#include "JobFactory.h"

using namespace Bearish;
using namespace Core;

std::atomic<bool>                JobFactory::_active;
i32                              JobFactory::_numWorkers;
std::vector<std::thread>         JobFactory::_workers;
ConcurrentQueue<JobFactory::Job> JobFactory::_queue;

void JobFactory::Initialize() {
	_active = true;
	_numWorkers = std::thread::hardware_concurrency() - 2;
	Logger::Info("JobFactory created with %d worker threads.", _numWorkers);

	for (i32 i = 0; i < _numWorkers; i++) {
		_workers.push_back(std::thread(&JobFactory::DoWork));
	}
}

void JobFactory::DoWork() {
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

void JobFactory::Terminate() {
	if (_active) {
		_active = false;
		_queue.Release();
		for (i32 i = 0; i < _numWorkers; i++) {
			_workers.at(i).join();
		}
	}
}

void JobFactory::AddJob(Action<> fn) {
	Job job;
	job.function = [fn](void* in, void* out) -> void {
		fn();
	};

	job.in = 0;
	job.out = 0;

	_queue.Push(job);
}