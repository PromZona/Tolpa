#include "Commands/CreateOrcCommand.hpp"

#include "EntityFactory.hpp"
#include "game.hpp"
#include "Components/TransformComponent.hpp"

CreateOrcCommand::CreateOrcCommand() = default;

void CreateOrcCommand::Execute()
{
	auto& lairs = Game::Instance().State.OrcTribes;
	if (lairs.empty())
	{
		TraceLog(LOG_INFO, "No tribes available to spawn orc");
		return;
	}

	auto& ecs = Game::Instance().GetECS();
	auto lairPos = ecs.GetComponent<TransformComponent>(lairs[GetRandomValue(0, lairs.size() - 1)])->Position;
	EntityFactory::CreateOrc(lairPos);
}
