#ifndef _BEARISH_CORE_CALLBACK_H_
#define _BEARISH_CORE_CALLBACK_H_

#include <BE/Types.h>

namespace Bearish { namespace Core {
	template<typename Return, typename... Args>
	class BEARISH_API Callback {
	public:
		using Function = std::function<Return(Args...)>;
		Callback() {}
		~Callback() {}

		inline void operator+= (Function func) {
			_functions.push_back(func);
		}

		inline void operator-= (Function func) {
			auto comp = func.target<Return(Args...)>();
			for (std::vector<Function>::iterator it = _functions.begin(); it != _functions.end(); it++) {
				auto comp2 = (*it).target<Return(Args...)>();
				if (comp == comp2) {
					_functions.erase(it);
					break;
				}
			}
		}

		inline std::vector<Return> operator() (Args... args) const {
			std::vector<Return> returnValues;
			for (auto& f : _functions) {
				returnValues.push_back(f(args...));
			}
			return returnValues;
		}

		inline Function operator[] (u32 pos) {
			return _functions.at(pos);
		}

		inline u32 Size() const {
			return _functions.size();
		}
	private:
		std::vector<Function> _functions;
	};

	template<typename... Args>
	class BEARISH_API Action {
	public:
		using Function = std::function<void(Args...)>;
		Action() {}

		template<class Fn>
		Action(Fn func) { _functions.push_back(func); }

		~Action() {}

		inline void operator+= (Function func) {
			_functions.push_back(func);
		}

		inline void operator-= (Function func) {
			auto comp = func.target<void(Args...)>();
			for (std::vector<Function>::iterator it = _functions.begin(); it != _functions.end(); it++) {
				auto comp2 = (*it).target<void(Args...)>();
				if (comp == comp2) {
					_functions.erase(it);
					break;
				}
			}
		}

		inline void operator() (Args... args) const {
			for (auto& f : _functions) {
				f(args...);
			}
		}

		inline const Function operator[] (u32 pos) {
			return _functions.at(pos);
		}

		inline u32 Size() const {
			return _functions.size();
		}
	private:
		std::vector<Function> _functions;
	};
} }

#endif // _BEARISH_CORE_CALLBACK_H_
