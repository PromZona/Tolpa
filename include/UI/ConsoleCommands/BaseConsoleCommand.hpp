#pragma once

#include <string>

class BaseConsoleCommand
{
public:
	virtual void BaseConsoleCommand();
	virtual void ProcessCommand(const std::stringstream&);

	std::string CommandName;
	std::string ArgsHint;
};
