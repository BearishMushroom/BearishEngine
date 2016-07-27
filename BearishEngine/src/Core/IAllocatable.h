#ifndef _BEARISH_CORE_IALLOCATABLE_H_
#define _BEARISH_CORE_IALLOCATABLE_H_

#include "BlockAllocator.h"
#include "../Types.h"

namespace Bearish { namespace Core {
	template<class T>
	class IAllocatable {
	public:
		static void* operator new(usize size) {
			return BlockAllocator<sizeof(T)>::Instance().Allocate();
		}

		static void operator delete(void* data, usize size) {
			BlockAllocator<sizeof(T)>::Instance().Free(data);
		}
	};
}}

#endif