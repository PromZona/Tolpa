#include "Commands/CreateCityCommand.hpp"
#include "EntityFactory.hpp"

CreateCityCommand::CreateCityCommand()
{
	m_Position = {0, 0};
}

CreateCityCommand::CreateCityCommand(Vector2 position)
{
	m_Position = position;
}

void CreateCityCommand::Execute()
{
	EntityFactory::CreateCity(m_Position);
}
