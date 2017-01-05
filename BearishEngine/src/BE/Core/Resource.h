#ifndef _BEARISH_CORE_RESOURCE_H_
#define _BEARISH_CORE_RESOURCE_H_

#include <BE\Types.h>
#include <BE\Core\Shared.h>
#include <BE\Core\Callback.h>
#include <BE\Core\JobFactory.h>
#include <atomic>

#include <BE/Serialization/Serialization.h>

namespace Bearish { namespace Core {
	template<typename T>
	class Resource {
	public:
		Resource() {
			_name = "";
			_isLoaded = Shared<std::atomic<bool>>(new std::atomic<bool>(false));
			_resource = Shared<T*>(new T*);
		}

		Resource(string name) {
			_name = name;
			_isLoaded = Shared<std::atomic<bool>>(new std::atomic<bool>(false));
			_resource = Shared<T*>(new T*);
		}

		Resource(string name, Shared<T*> res) {
			_isLoaded = Shared<std::atomic<bool>>(new std::atomic<bool>(true));
			_name = name;
			_resource = res;
		}

		Resource(const Resource<T>& o) {
			_name = o._name;
			_isLoaded = Shared<std::atomic<bool>>(o._isLoaded);
			_resource = Shared<T*>(o._resource);
		}

		bool IsLoaded() const { return *_isLoaded.Get(); }
		void IsLoaded(bool v) { _isLoaded->store(v); }

		T* Get() { return *_resource.operator->(); }
		void Set(T* res) { *_resource.operator->() = res; }

		T* operator->() { return Get(); }

		void WaitForLoad() {
			while (*_isLoaded.operator->() == false) {
				// Force the loader to tick.
				ResourceManager::Update();
				JobFactory::Update();
			}
		}

		static Resource<T> Get(string name, Action<Resource<T>&> end = [](Resource<T>& l) {});

		template<typename Archive>
		void save(Archive& ar) const {
			ar(CEREAL_NVP(_name));
		}

		template<typename Archive>
		void load(Archive& ar) {
			ar(_name);
			*this = Get(_name);
		}
	private:
		static std::map<string, Shared<T*>> _loaded;

		string _name;
		Shared<std::atomic<bool>> _isLoaded;
		Shared<T*> _resource;
	};
} }

#endif // _BEARISH_CORE_RESOURCE_H_
