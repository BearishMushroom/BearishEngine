#ifndef _BEARISH_CORE_SHARED_H_
#define _BEARISH_CORE_SHARED_H_

#include <BE/Types.h>
#include "IAllocatable.h"

namespace Bearish { namespace Core {
	template<typename T>
	class BEARISH_API Shared : public IAllocatable<Shared<T>> {
	public:
		Shared() { _refs = 0; _data = 0; }
		Shared(Shared<T>&& other) { _data = other._data; _refs = ++other._refs; }
		
		Shared(const Shared<T>& other) {
			Shared<T>& o = const_cast<Shared<T>&>(other);
			_data = o._data;
			_refs = ++o._refs;
		}

		Shared(T* data) { _data = data; _refs = 1; }

		~Shared() {
			if (_data) {
				_refs--;
				if (_refs == 0) {
					delete _data;
				}
			}
		}

		bool IsLast() { return _refs == 1; }

		void operator=(Shared& other) { _data = other._data; _refs = ++other._refs; }
		void operator=(T* data) { _data = data; _refs = 1; }

		T* Get() const { return _data; }
		T* operator->() { return _data; }
	private:
		T* _data;
		i32 _refs;
	};
} }


#endif // _BEARISH_CORE_SHARED_H_
