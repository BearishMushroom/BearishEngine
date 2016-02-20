#ifndef _BEARISH_CORE_TIMER_H_
#define _BEARISH_CORE_TIMER_H_

#include "IAllocatable.h"
#include "../Types.h"
#include <glfw3.h>

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
		f32 _lastTime;
	};
} }
#endif