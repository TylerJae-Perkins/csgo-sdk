#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <string_view>
#include <Windows.h>

enum ELogType {
	LOG_MESSAGE = 0,
	LOG_WARNING,
	LOG_ERROR
};

namespace Logger {
	void LogToConsole(std::string_view strString, ELogType log, bool bCreateNewLine = true);
}

//log_type = int | 0 = Message, 1 = Warning, 2 = Error
#define LOG(str, log_type) Logger::LogToConsole(str, log_type);
#define LOG_NO_BREAK(str, log_type) Logger::LogToConsole(str, log_type, false);

#endif