#ifndef _BEARISH_CORE_SHARED_H_
#define _BEARISH_CORE_SHARED_H_

#include "../Types.h"
#include "IAllocatable.h"

namespace Bearish { namespace Core {
	template<typename T>
	class Shared : public IAllocatable<Shared<T>> {
	public:
		Shared() { _refs = 0; _data = 0; }
		Shared(Shared<T>&& other) { _data = other._data; _refs = ++other._refs; }
		Shared(T* data) { _data = data; _refs = 1; }

		~Shared() {
			if (_data) {
				_refs--;
				if (_refs == 0) {
					delete _data;
				}
			}
		}

		void operator=(Shared& other) { _data = other._data; _refs = ++other._refs; }
		void operator=(T* data) { _data = data; _refs = 1; }

		T* operator->() { return _data; }
	private:
		T* _data;
		i32 _refs;
	};
} }


#endif // _BEARISH_CORE_SHARED_H_
