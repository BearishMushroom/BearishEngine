#include "Logger.h"

using namespace Bearish;
using namespace Core;

Logger::Logger() {
	_file = new std::ofstream("BearishEngineLog.txt", std::ios::app);
}

Logger::~Logger() {
	_file->close();
	delete _file;
}

void Logger::Log(string toLog) {
	std::cout << toLog;
	*_file << toLog;
}