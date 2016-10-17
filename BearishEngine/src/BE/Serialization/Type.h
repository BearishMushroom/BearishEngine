#ifndef _BEARISH_SERILIZATION_TYPE_H_
#define _BEARISH_SERILIZATION_TYPE_H_

#include <functional>
#include <map>


namespace Bearish { namespace Serilization {
	class TypeFactory {
		static std::map<std::string, std::function<void*()>> _factories;
	public:
		template<class T>
		static T* Create(const std::string& className) {
			return (T*)_factories.at(className)();
		}

		template<class T>
		static void Declare(const std::string objectName, std::function<T*()> factory) {
			_factories[objectName] = [factory]() { return static_cast<void*>(factory()); };
		}
	};


	template<class T>
	struct Type {
		static constexpr char const* name = "";
	};

	#define BE_SERILIZATION_TYPE(TYPE) \
	template<> \
	struct ::Bearish::Serilization::Type<TYPE> { \
		static constexpr char const* name = #TYPE; \
		Type() { \
			::Bearish::Serilization::TypeFactory::Declare(#TYPE, std::function<TYPE*()>([]() -> TYPE* { return new TYPE(); })); \
		} \
	}; static Type<TYPE> SERILIZATION_##TYPE##_INIT;

	template<typename T>
	static T* CreateObject() {
		return TypeFactory::Create<T>(Type<T>::name);
	}

	static void* CreateObject(std::string name) {
		return TypeFactory::Create<void>(name);
	}
} }

#endif // _BEARISH_SERILIZATION_TYPE_H_