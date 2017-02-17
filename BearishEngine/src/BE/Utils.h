#ifndef _BEARISH_UTILS_H_
#define _BEARISH_UTILS_H_

#include <BE/Core/Keyboard.h>
#include <BE/Types.h>
#include <time.h>

namespace Bearish { namespace Util {
	static string KeyToString(Core::Key key) {
		// If is ASCII
		if ((i32)key <= (i32)Core::Key::Z && (i32)key >= (i32)Core::Key::A) {
			return string(1, (c8)key + 32);
		}

		if ((i32)key <= (i32)Core::Key::Nine && (i32)key >= (i32)Core::Key::Zero) {
			return string(1, (c8)key);
		}

		// Convert numpad numbers to regular numbers
		if ((i32)key >= (i32)Core::Key::NumpadZero && (i32)key <= (i32)Core::Key::NumpadNine) {
			return string(1, (c8)((i32)key - ((i32)Core::Key::NumpadZero - (i32)Core::Key::Zero)));
		}

		if (key == Core::Key::Spacebar) return " ";
		if (key == Core::Key::Tab) return "   ";
		if (key == Core::Key::Period) return ".";
		if (key == Core::Key::Comma) return ",";
		if (key == Core::Key::Minus) return "-";
		if (key == Core::Key::Plus) return "+";

		return "";
	}

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
		usize pos = text.find(delim);
		usize initialPos = 0;
		result.clear();

		while (pos != string::npos && pos < text.length() - 1) {
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

	static void CreateFolder(string name) {
		CreateDirectory(name.c_str(), 0);
	}

	static bool FileExists(string name) {
		DWORD dwAttrib = GetFileAttributes(name.c_str());

		return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
			!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	static bool DirectoryExists(string name) {
		DWORD dwAttrib = GetFileAttributes(name.c_str());

		return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
			(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	static f32 Round2P(f32 value) {
		return floor(value * 100 + 0.5) / 100;
	}

	enum class FileType {
		Invalid, Text, Binary
	};

	struct File {
		c8* content;
		u32 length;
		FileType type;
	};

	static File ReadFile(string path, FileType type) {
		std::ifstream fs(path, std::ios::ate | (type == FileType::Binary ? std::ios::binary : 0));
		if (!fs.is_open()) {
			Core::Logger::Error("Failed to open file %s.", path.c_str());
			return {0, 0, FileType::Invalid};
		}

		u32 size = fs.tellg();
		c8* buffer = new c8[size];

		fs.seekg(0);
		fs.read(buffer, size);
		fs.close();

		return {
			buffer,
			size,
			type
		};
	}
} }
#endif