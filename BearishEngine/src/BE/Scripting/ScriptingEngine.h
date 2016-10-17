#ifndef _BEARISH_SCRIPTING_ENGINE_H_
#define _BEARISH_SCRIPTING_ENGINE_H_

#include <stdio.h>
#include <stdarg.h>

#include <functional>

#include <BE/Types.h>
#include <BE/Core/Logger.h>

#include <lua.hpp>
#include <luabind\luabind.hpp>

namespace Bearish { namespace Scripting {
	class BEARISH_API LuaObject {
	public:
		LuaObject() {}
		LuaObject(luabind::object self) : _self(self) {}
		~LuaObject() {}

		template<typename Ret, typename... Args>
		inline Ret CallFunction(string name, Args... args) {
			try {
				return luabind::call_member<Ret>(_self, name.c_str(), args...);
			}
			catch (luabind::error e) {
				Core::Logger::Error("Lua: %s\n", lua_tostring(GetVM(), -1));
				Core::Logger::Error(e.what());
			}

			return Ret();
		}

		inline bool HasFunction(string name) {
			auto obj = _self[name.c_str()];
			
			return luabind::type(luabind::rawget(_self, name)) != LUA_TNIL || luabind::type(obj) != LUA_TNIL;
		}

		template<typename T>
		inline void Set(string name, T value) {
			_self[name] = value;
		}

		template<typename T>
		inline T Get(string name) {
			return luabind::object_cast<T>(luabind::rawget(_self, name));
		}

		inline bool Valid() {
			return _self.interpreter() && luabind::type(_self) != LUA_TNIL;
		}
	private:
		luabind::object _self;
	};

	static lua_State* L;
	static luabind::adl::object moonToLua, moonDoFile;

	static std::map<string, u32> _loadedMoons;
	static std::map<string, u32> _loadedLuas;

	BEARISH_API void InitLua();
	BEARISH_API void KillLua();

	BEARISH_API void RunFile(string file);
	BEARISH_API void RunString(string cmd);

	BEARISH_API void InitMoonScript();

	BEARISH_API string CompileMoon(string moon);

	BEARISH_API void RunMoon(string moon);

	BEARISH_API LuaObject DoMoonFile(string filename);

	BEARISH_API lua_State* GetVM();

	template<typename... Args>
	static LuaObject CreateInstance(string name, Args&&... args) {
		try {
			return LuaObject(luabind::call_function<luabind::object>(GetVM(), name.c_str(), args...));
		}
		catch (luabind::error e) {
			Core::Logger::Error("Lua: %s\n", lua_tostring(GetVM(), -1));
			Core::Logger::Error(e.what());
			return LuaObject();
		}
		catch (std::runtime_error e) {
			Core::Logger::Error(e.what());
			return LuaObject();

		}
	}

	BEARISH_API LuaObject CreateInstance(string func);

	BEARISH_API void UpdateScripts(); 
} }



#endif // _BEARISH_SCRIPTING_ENGINE_H_
