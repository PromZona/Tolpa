#pragma once

#include <string>

enum LogType
{
	Info,
	Warning,
	Error,
	Raylib
};

struct LogMessage
{
	std::string Message;
	LogType LogType;
};