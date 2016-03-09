#ifndef _BEARISH_CORE_ASSETS_H_
#define _BEARISH_CORE_ASSETS_H_

#include "../Types.h"
#include <fstream>
#include <unordered_map>

namespace Bearish { namespace Core {
	class Asset {
	public:
		static void LoadAssetDefinitions() {
			static string filename = "asset/assets.def";
			
			std::ifstream assetfile(filename);
			string line;

			while (std::getline(assetfile, line)) {
				string a = "", b = "";
				i32 pos = -1;
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

		static string Get(string assetName) {
			return _values.at(assetName);
		}
	private:
		Asset() {}
		~Asset() {}

		static std::unordered_map<string, string> _values;
	};
} }

#endif // _BEARISH_CORE_ASSETS_H_