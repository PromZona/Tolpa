#pragma once

#include <queue>
#include <memory>

class ICommand
{
public:
	virtual void Execute() = 0;
	virtual ~ICommand() = default;
};

class CommandManager
{
public:
	CommandManager() = default;
	~CommandManager() = default;

	void AddCommand(std::unique_ptr<ICommand> command);
	void Tick();

private:
	std::queue<std::unique_ptr<ICommand>> m_Commands;
};
