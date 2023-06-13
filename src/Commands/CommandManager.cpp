#include "Commands/CommandManager.hpp"

void CommandManager::AddCommand(std::unique_ptr<ICommand> command)
{
	if (command == nullptr)
		return;
	m_Commands.push(std::move(command));
}

void CommandManager::Tick()
{
	while (!m_Commands.empty())
	{
		auto& command = m_Commands.front();
		command->Execute();
		m_Commands.pop();
	}
}