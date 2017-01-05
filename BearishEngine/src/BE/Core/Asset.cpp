#include "Asset.h"
#include "Model.h"
#include <BE/Graphics/Texture/Texture2D.h>
#include <BE/Graphics/Material.h>
#include <BE\Core\Settings.h>

using namespace Bearish;
using namespace Core;
using namespace Graphics;

std::unordered_map<string, Texture2D*> Asset<Texture2D>::_loaded;
std::unordered_map<string, Texture*> Asset<Texture>::_loaded;

std::unordered_map<string, Model*>     Asset<Model>::_loaded;
std::unordered_map<string, Material*>  Asset<Material>::_loaded;

std::unordered_map<string, string> ResourceMap::_values;
/*
void ResourceMap::LoadAssetDefinitions() {
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
}*/