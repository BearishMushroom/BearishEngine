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
	static lua_State* L;
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
} }

#endif // _BEARISH_SCRIPTING_ENGINE_H_
