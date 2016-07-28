#ifndef _BEARISH_CORE_IALIGNED_H_
#define _BEARISH_CORE_IALIGNED_H_

#include "BlockAllocator.h"
#include "../Types.h"

namespace Bearish {
	namespace Core {
		template<usize ALIGN>
		class IAligned {
		public:
			static void* operator new(usize size) {
#if defined WIN32
				return _aligned_malloc(size, ALIGN);
#elif defined __linux__
				return memalign(ALIGN, size);
#elif defined __MACH__
				return malloc(size);
#else // use page-aligned memory for other systems
				return valloc(size);
#endif
			}

			static void operator delete(void* data, usize size) {
#if defined WIN32
				return _aligned_free(v);
#else
				return free(v);
#endif
			}

			static void* operator new[](usize size) {
#if defined WIN32
				return _aligned_malloc(size, ALIGN);
#elif defined __linux__
				return memalign(ALIGN, size);
#elif defined __MACH__
				return malloc(size);
#else // use page-aligned memory for other systems
				return valloc(size);
#endif
			}

			static void operator delete[](void* data, usize size) {
#if defined WIN32
				return _aligned_free(v);
#else
				return free(v);
#endif
			}
		};
	}
}

#endif