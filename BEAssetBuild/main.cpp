#include <stdio.h>
#include "Types.h"

#include <vector>
#include <fstream>
#include <map>
#include <regex>

void ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

bool MatchTextWithWildcards(const string& text, string wildcardPattern) {
	// Escape all regex special chars
	EscapeRegex(wildcardPattern);

	// Convert chars '*?' back to their regex equivalents
	ReplaceAll(wildcardPattern, "\\?", ".");
	ReplaceAll(wildcardPattern, "\\*", ".*");

	std::regex pattern(wildcardPattern);

	return std::regex_match(text, pattern);
}

void EscapeRegex(string &regex) {
	ReplaceAll(regex, "\\", "\\\\");
	ReplaceAll(regex, "^", "\\^");
	ReplaceAll(regex, ".", "\\.");
	ReplaceAll(regex, "$", "\\$");
	ReplaceAll(regex, "|", "\\|");
	ReplaceAll(regex, "(", "\\(");
	ReplaceAll(regex, ")", "\\)");
	ReplaceAll(regex, "[", "\\[");
	ReplaceAll(regex, "]", "\\]");
	ReplaceAll(regex, "*", "\\*");
	ReplaceAll(regex, "+", "\\+");
	ReplaceAll(regex, "?", "\\?");
	ReplaceAll(regex, "/", "\\/");
}

#include <Windows.h>
std::vector<string> GetFilesInFolder(string folder, bool subfolders = true) {
	std::vector<string> names;
	c8 searchPath[MAX_PATH];
	sprintf_s(searchPath, "%s", folder.c_str());

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(searchPath, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			names.push_back(folder + fd.cFileName);
		} while (FindNextFile(hFind, &fd));

		FindClose(hFind);
	}

	if (subfolders) {
		strcpy_s(searchPath, MAX_PATH, folder.c_str());
		strcat_s(searchPath, MAX_PATH, "\\*");
		hFind = FindFirstFile(searchPath, &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && string(fd.cFileName) != "." && string(fd.cFileName) != "..") {
					auto dir = GetFilesInFolder(folder + string(fd.cFileName) + "/", subfolders);
					for (auto d : dir) {
						names.push_back(d);
					}
				}
			} while (FindNextFile(hFind, &fd));

			FindClose(hFind);
		}
	}

	return names;
}

void CreateFolder(string name) {
	CreateDirectory(name.c_str(), 0);
}

bool FileExists(string name) {
	DWORD dwAttrib = GetFileAttributes(name.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool DirectoryExists(string name) {
	DWORD dwAttrib = GetFileAttributes(name.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

std::vector<string> GetFiles(std::string expression) {

}

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
				pos = (i32)line.find('"');
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