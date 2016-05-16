#ifndef _BEARISH_CORE_TIMER_H_
#define _BEARISH_CORE_TIMER_H_

#include "IAllocatable.h"
#include "../Types.h"
#include <Windows.h>

namespace Bearish { namespace Core {
	class Timer : public IAllocatable<Timer> {
	public:
		Timer();
		~Timer();

		void Start();
		const f32 Loop();
		const f32 LoopMS();
		const f32 Delta() const;
		const f32 DeltaMS() const;
	private:
		LARGE_INTEGER _start;
		f64 _freq;
	};
} }
#endif