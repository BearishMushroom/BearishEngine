#ifndef _BEARISH_CORE_SETTINGS_H_
#define _BEARISH_CORE_SETTINGS_H_

#include <unordered_map>

#include <BE/Types.h>
#include <BE/Utils.h>
#include <BE/Core/IAllocatable.h>
#include <BE/Scripting/ScriptingEngine.h>

namespace Bearish { namespace Core {
	const string SETTINGS_FILE = "./config/settings.moon";
	const string SETTINGS_DIR = "./config/";

	const string DEFAULT_FILE = R"(
settings =
  resolution_x: 1280
  resolution_y: 720
  shadows: 4
  ssao: 1

settings
)";

	struct BEARISH_API Setting {
		f64 number;
		string chars;
	};

	class BEARISH_API Settings : IAllocatable<Settings> {
	public:
		static void Load();

		template<typename T>
		static T Get(string index);
	private:
		static void WriteDefault();

		static Scripting::LuaObject* _map;
	};
} }

#endif //_BEARISH_CORE_SETTINGS_H_