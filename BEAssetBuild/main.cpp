#include <stdio.h>
#include "Types.h"

#include <vector>
#include <fstream>
#include <map>

i32 main(i32 argc, c8** argv) {
	std::map<string, u64> timestamps;
	// Get the filename if provided, or default it.
	string filename = "assets.assetdef";
	if (argc >= 2) {
		filename = string(argv[1]);
	}

	string build = "assetbuild.def";
	string defs = "./asset/assets.def";
	std::ifstream buildfile(build);

	string line;
	if (!buildfile.fail()) {
		string a;
		u64 b;
		while (std::getline(buildfile, line)) {
			a = line.substr(line.find('"') + 1, line.find_last_of('"') - 1);
			b = (u64)std::stoull(line.substr(line.find_last_of('"') + 1, line.length() - line.find_last_of('"')));
			timestamps.emplace(a, b);
		}
	}

	std::ifstream assetfile(filename);
	std::ofstream deffile(defs);

	std::vector<string> uncompiled;

	while (std::getline(assetfile, line)) {
		string a = "", b = "", c = "";
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
				else if (c == "") c = name;
				pos = -1;
			}
		}

		string resPath = "./res/" + b;
		string outPath = "./asset/" + c;
		int fileType = -1;

		string ext = b.substr(b.find_last_of('.'));
		if (ext == ".png" || ext == ".jpg" || ext == ".tga" || ext == ".bmp" || ext == ".dds") {
			outPath += ".bet";
			fileType = 0;
		}
		else if (ext == ".obj" || ext == ".fbx" || ext == ".dae") {
			outPath += ".bem";
			fileType = 1;
		}

		struct stat st;
		stat(resPath.c_str(), &st);
		b8 shouldCompile = true;

		if (timestamps.find(b) != timestamps.end()) {
			if (timestamps[b] >= (u64)st.st_mtime) {
				shouldCompile = false;
				uncompiled.push_back("\"" + a + "\"" + "\"" + outPath + "\"" + "\n");
			}
			else {
				timestamps[b] = (u64)st.st_mtime;
			}
		}
		else {
			timestamps.emplace(b, (u64)st.st_mtime);
		}

		if (shouldCompile) {
			printf("Compiling: %s\n", b.c_str());
			deffile << "\"" << a << "\"" << "\"" << outPath << "\"" << "\n";
			string ext = b.substr(b.find_last_of('.'));

			if (fileType == 0) {
				string command = "BETBuild.exe " + resPath + " " + outPath;
				system(command.c_str());
			}
			else if (fileType == 1) {
				string command = "BEMBuild.exe " + resPath + " " + outPath;
				system(command.c_str());
			}
			else {
				printf("Error: Could not find compiler for file.\n");
			}
		}
	}

	std::ofstream times(build);

	for (auto& pair : timestamps) {
		times << "\"" << pair.first << "\"" << std::to_string(pair.second) << "\n";
	}

	for (auto& str : uncompiled) {
		deffile << str;
	}
	
	return 0;
}