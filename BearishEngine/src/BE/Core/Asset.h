#ifndef _BEARISH_CORE_ASSETS_H_
#define _BEARISH_CORE_ASSETS_H_

#include <BE/Types.h>

#include <fstream>
#include <unordered_map>

namespace Bearish { namespace Core {
	class BEARISH_API ResourceMap {
	public:
		static std::unordered_map<string, string> _values;
		static void LoadAssetDefinitions() {
			static string filename = "asset/assets.def";

			std::ifstream assetfile(filename);
			string line;

			while (std::getline(assetfile, line)) {
				string a = "", b = "";
				usize pos = -1;
				while (line.find('"') != -1) {
					if (pos == -1) {
						pos = line.find('"');
					}
					else {
						string name = line.substr(pos + 1, line.find('"', pos + 1) - pos - 1);
						line = line.substr(line.find('"', pos + 1) + 1);

						if (a == "") a = name;
						else if (b == "") b = name;
						pos = -1;
					}
				}

				_values.emplace(a, b);
			}
		}
	};

	template<typename T>
	class BEARISH_API Asset {
	public:
		static bool IsLoaded(string asset) {
			return _loaded.find(asset) != _loaded.end();
		}

		static T* Load(string asset) {
			_loaded.emplace(asset, new T(GetPath(asset)));
			T* ret = _loaded.at(asset);
			ret->SetAssetName(asset);
			return ret;
		}

		static T* Get(string asset) {
			if (IsLoaded(asset)) {
				return _loaded.at(asset);
			}
			else {
				return Load(asset);
			}
		}

		static string GetPath(string assetName) {
			if (assetName.at(0) == '@') return assetName.substr(1);

			return ResourceMap::_values.at(assetName);
		}
	private:
		Asset() {}
		~Asset() {}

		static std::unordered_map<string, T*> _loaded;
	};
} }

#endif // _BEARISH_CORE_ASSETS_H_