#include <BE\Core\Settings.h>

using namespace Bearish;
using namespace Core;

void Settings::Load() {
	if (!Util::DirectoryExists(SETTINGS_DIR)) {
		Util::CreateFolder(SETTINGS_DIR);
		WriteDefault();
	}
	else if (!Util::FileExists(SETTINGS_FILE)) {
		WriteDefault();
	}

	_map = new Scripting::LuaObject(Scripting::DoMoonFile(SETTINGS_FILE));
}

template<typename T>
T Settings::Get(string index) {
	return (T)_map->Get<T>(index);
}

template<>
i32 BEARISH_API Settings::Get(string index) {
	return (i32)_map->Get<i32>(index);
}

template<>
f32 BEARISH_API Settings::Get(string index) {
	return (f32)_map->Get<f32>(index);
}

void Settings::WriteDefault() {
	FILE* f;
	fopen_s(&f, SETTINGS_FILE.c_str(), "w");
	fwrite(DEFAULT_FILE.c_str(), 1, DEFAULT_FILE.length(), f);
	fclose(f);
}

Scripting::LuaObject* Settings::_map;