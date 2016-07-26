#ifndef _BEARISH_CORE_SETTINGS_H_
#define _BEARISH_CORE_SETTINGS_H_

#include "../Types.h"
#include "IAllocatable.h"
#include <unordered_map>
#include "../Scripting/ScriptingEngine.h"
#include "../Utils.h"

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

	struct Setting {
		f64 number;
		string chars;
	};

	class Settings : IAllocatable<Settings> {
	public:
		static void Load() {
			if (!Util::DirectoryExists(SETTINGS_DIR)) {
				Util::CreateFolder(SETTINGS_DIR);
				WriteDefault();
			}
			else if (!Util::FileExists(SETTINGS_FILE)) {
				WriteDefault();
			}

			_map = Scripting::DoMoonFile(SETTINGS_FILE);
		}

		template<typename T>
		static T Get(string index) {
			return (T)_map.Get<T>(index);
		}
	private:
		static void WriteDefault() {
			FILE* f;
			fopen_s(&f, SETTINGS_FILE.c_str(), "w");
			fwrite(DEFAULT_FILE.c_str(), 1, DEFAULT_FILE.length(), f);
			fclose(f);
		}

		static Scripting::LuaObject _map;
	};
} }

#endif //_BEARISH_CORE_SETTINGS_H_