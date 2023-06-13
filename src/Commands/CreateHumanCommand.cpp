#include "Commands/CreateHumanCommand.hpp"
#include "EntityFactory.hpp"

CreateHumanCommand::CreateHumanCommand()
{
	m_Position = {0, 0, 0};
}

CreateHumanCommand::CreateHumanCommand(Vector3 position)
{
	m_Position = position;
}

void CreateHumanCommand::Execute()
{
	EntityFactory::CreateHuman(m_Position);
}
