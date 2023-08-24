#pragma once

#include <memory>
#include <set>
#include <functional>
#include "Commands/CommandManager.hpp"
#include "Logger/LogTypes.hpp"

class ConsoleGui
{
public:
	ConsoleGui();
	~ConsoleGui();

	void Draw();
	void ShowOrHide();
private:
	bool FilterLogMessage(const LogMessage& message);
	void SendCommand(const char* commandInput);

	void PushColor(LogType logType);
	void PopColor();

	bool m_showConsoleWindow = false;
	bool m_autoScroll = true;
	bool m_showInfoLog = true;
	bool m_showWarningLog = true;
	bool m_showErrorLog = true;
	char m_ConsoleInputBuf[256];

	std::set<std::string> m_commands;
	std::unordered_map<std::string, std::function<void(const std::stringstream&)>> m_commandToFunction;
};
