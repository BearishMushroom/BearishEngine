#ifndef _BEARISH_CORE_IALLOCATABLE_H_
#define _BEARISH_CORE_IALLOCATABLE_H_

#include <BE/Core/BlockAllocator.h>
#include <BE/Types.h>

namespace Bearish { namespace Core {
	template<class T>
	class BEARISH_API IAllocatable {
	public:
		static void* operator new(usize size) {
			//return BlockAllocator<sizeof(T)>::Instance().Allocate();
			return malloc(size);
		}

		static void operator delete(void* data, usize size) {
			//BlockAllocator<sizeof(T)>::Instance().Free(data);
			free(data);
		}
	};
}}

#endif