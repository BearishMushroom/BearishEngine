#ifndef _BEARISH_CORE_LOGGER_H_
#define _BEARISH_CORE_LOGGER_H_

#define BEARISH_DEBUG	  1  // Do we even print to the log?
#define BEARISH_VERBOSE	  0  // Prints file and line of every info piece.
#define BEARISH_WARNLEVEL 3  // 0 = Crashes only, 1 = Errors, 2 = Warnings, 3 = Info

#include "../Types.h"
#include <mutex>

#if BEARISH_DEBUG
#define BEARISH_ASSERT(x, m) \
		if(!(x)) { \
			Bearish::Core::Logger::Instance().Log("*************************\n"); \
			Bearish::Core::Logger::Instance().Log("     ASSERTION FAILED!   \n"); \
			Bearish::Core::Logger::Instance().Log("*************************\n"); \
			Bearish::Core::Logger::Instance().Log(std::string(m) + std::string("\n")); \
			BEARISH_ASSERT_PRINTFILE(); \
			*(int*)NULL = 1;\
		} 

#define BEARISH_ASSERT_PRINTFILE()	std::string name(__FILE__); \
								name = name.substr(name.find("\src") == std::string::npos ? 0 : name.find("\src"), name.length()); \
								Bearish::Core::Logger::Instance().FormatLog("(%s:%d)\n", name.c_str(), __LINE__);
#endif

namespace Bearish { 
	static void Assert(bool condition, string message) {
		if (!condition) {
			throw message;
		}
	}
	
	namespace Core {
	class Logger {
	private:
		std::mutex _mutex;
		std::ofstream* _file;
		Logger();
		~Logger();
	public:
		static Logger& Instance() {
			static Logger instance;
			return instance;
		}

		void Log(string toLog);

		template<typename... Args>
		void FormatLog(string format, Args&&... args) {
			std::unique_lock<std::mutex> lock(_mutex);
			c8 buffer[300000];
			i32 length = sprintf_s(buffer, format.c_str(), args...);

			if (length) {
				string str(buffer);
				Log(str);
			}
			else {
				Log("[BEARISH][ERROR] Failed to format \"" + format + "\"");
			}
		}

		template<typename... Args>
		static void Info(string format, Args&&... args) {
			Instance().FormatLog("[BEARISH][INFO] " + format + "\n", args...);
		}

		template<typename... Args>
		static void Warn(string format, Args&&... args) {
			Instance().FormatLog("[BEARISH][WARN] " + format + "\n", args...);
		}

		template<typename... Args>
		static void Error(string format, Args&&... args) {
			Instance().FormatLog("[BEARISH][ERROR] " + format + "\n", args...);
		}

		template<typename... Args>
		static void Fatal(string format, Args&&... args) {
			Instance().FormatLog("[BEARISH][FATAL] " + format + "\n", args...);
			Assert(false, "");
		}
	};
}}

#endif