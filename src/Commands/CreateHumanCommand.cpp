#include "Commands/CreateHumanCommand.hpp"
#include "EntityFactory.hpp"
#include "Logger/Logger.hpp"

CreateHumanCommand::CreateHumanCommand()
{
	m_Position = {0, 0};
}

CreateHumanCommand::CreateHumanCommand(Vector2 position)
{
	m_Position = position;
}

void CreateHumanCommand::Execute()
{
	EntityFactory::CreateHuman(m_Position);
	Logger::Log(Info, "Human created at (%.1f, %.1f)", m_Position.x, m_Position.y);
}
