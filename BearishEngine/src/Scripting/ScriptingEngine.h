#ifndef _BEARISH_SCRIPTING_ENGINE_H_
#define _BEARISH_SCRIPTING_ENGINE_H_

#include <stdio.h>
#include <stdarg.h>

#include <functional>

#include "../Types.h"
#include "../Core/Logger.h"

#include <lua.hpp>
#include <luabind\luabind.hpp>

namespace Bearish { namespace Scripting {
	class LuaObject {
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
				Core::Logger::Error("Lua: %s\n", lua_tostring(Scripting::L, -1));
				Core::Logger::Error(e.what());
			}
		}

		inline bool HasFunction(string name) {
			auto obj = _self[name.c_str()];
			
			return luabind::type(obj) != LUA_TNIL;
		}

		inline bool Valid() {
			return luabind::type(_self) != LUA_TNIL;
		}
	private:
		luabind::object _self;
	};

	static lua_State* L;
	static luabind::adl::object moonToLua, moonDoFile;

	static void InitLua() {
		L = luaL_newstate();
		luaL_openlibs(L);
		luabind::open(L);
	}


	static void KillLua() {
		lua_close(L);
	}

	static void RunFile(string file) {
		if (luaL_dofile(L, file.c_str())) {
			Core::Logger::Error("Error running script: %s\nLua: %s\n", file.c_str(), lua_tostring(L, -1));
		}
	}

	static void RunString(string cmd) {
		if (luaL_dostring(L, cmd.c_str())) {
			Core::Logger::Error("Error running string: %s\nLua: %s\n", cmd.c_str(), lua_tostring(L, -1));
		}
	}

	static void InitMoonScript() {
		luabind::object moon = luabind::call_function<luabind::object>(Scripting::L, "require", "moonscript");
		moonToLua = luabind::rawget(moon, "to_lua");
		moonDoFile = luabind::rawget(moon, "dofile");
	}

	static string CompileMoon(string moon) {
		return luabind::call_function<string>(moonToLua, moon);
	}

	static void RunMoon(string moon) {
		string str = CompileMoon(moon);
		Core::Logger::Info("%s", str.c_str());
		RunString(str);
	}

	static void DoMoonFile(string filename) {
		try {
			luabind::call_function<void>(moonDoFile, filename);
		}
		catch (luabind::error e) {
			Core::Logger::Error("Lua: %s\n", lua_tostring(Scripting::L, -1));
			Core::Logger::Error(e.what());
		}
	}

	template<typename... Args>
	static LuaObject CreateInstance(string name, Args&&... args) {
		return LuaObject(luabind::call_function<luabind::object>(Scripting::L, name.c_str(), args...));
	}

	static LuaObject CreateInstance(string func) {
		luaL_loadstring(L, ("return " + func).c_str());
		luabind::object obj(luabind::from_stack(L, -1));
		return LuaObject(obj());
	}
} }

#endif // _BEARISH_SCRIPTING_ENGINE_H_
