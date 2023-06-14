#pragma once

#include <vector>
#include "LogTypes.hpp"

class Logger
{
public:
	template<typename... Args>
	static void Log(LogType logType, const std::string& format, Args... args) {
		size_t size = snprintf(nullptr, 0, format.c_str(), args...);
		std::string result(size + 1, '\0');
		snprintf(&result[0], size + 1, format.c_str(), args...);
		result.resize(size);
		m_messages.push_back({ result, logType});
	}

	static void RaylibLoggingRedirect(int logLevel, const char *text, va_list args);

	static const std::vector<LogMessage>& GetLog() { return m_messages; }

private:
	static std::vector<LogMessage> m_messages;
};
