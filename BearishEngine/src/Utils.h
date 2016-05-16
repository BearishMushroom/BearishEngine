#ifndef _BEARISH_UTILS_H_
#define _BEARISH_UTILS_H_

#include "Types.h"
#include <time.h>

namespace Bearish { namespace Util {	
	static void SeedRandom() {
		srand((u32)time(NULL));
	}

	static f32 Random() {
		return (f32)rand() / (f32)RAND_MAX + 1.f;
	}

	static f32 Clamp(f32 value, f32 min, f32 max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	static std::vector<string> SplitString(const string &text, const string& delim) {
		std::vector<string> result;
		u32 pos = text.find(delim);
		u32 initialPos = 0;
		result.clear();

		while (pos != string::npos) {
			result.push_back(text.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = text.find(delim, initialPos);
		}

		result.push_back(text.substr(initialPos, pos < text.size() ? pos : text.size() - initialPos));

		return result;
	}

	// TODO: Port for X11 / MacOS.
	static std::vector<string> GetFilesInFolder(string folder, string ext = "*", bool subfolders = true) {
		std::vector<string> names;
		c8 searchPath[MAX_PATH];
		sprintf_s(searchPath, (string("%s/*.") + ext).c_str(), folder.c_str());

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
						auto dir = GetFilesInFolder(folder + string(fd.cFileName) + "/", ext, subfolders);
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
} }
#endif