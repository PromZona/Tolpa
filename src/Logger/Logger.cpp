#include "Logger/Logger.hpp"

std::vector<LogMessage> Logger::m_messages;

void Logger::RaylibLoggingRedirect(int logLevel, const char* text, va_list args)
{
	Logger::Log(LogType::Raylib, text, args);
}
