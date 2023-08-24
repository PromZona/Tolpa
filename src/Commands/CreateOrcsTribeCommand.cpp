#include "Commands/CreateOrcsTribeCommand.hpp"
#include "EntityFactory.hpp"

CreateOrcsTribeCommand::CreateOrcsTribeCommand()
{
	m_Position = {0, 0, 0};
}

CreateOrcsTribeCommand::CreateOrcsTribeCommand(Vector3 position)
{
	m_Position = position;
}

void CreateOrcsTribeCommand::Execute()
{
	EntityFactory::CreateOrcsTribe(m_Position);
}