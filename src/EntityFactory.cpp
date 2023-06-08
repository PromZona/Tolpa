#include "EntityFactory.hpp"

#include "ECS/ECSTypes.hpp"

#include "Components/TransformComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/ModelComponent.hpp"

#include "game.hpp"
#include "Components/GoalComponent.hpp"

EntityId EntityFactory::CreateParty()
{
	auto& ecs = Game::Instance().GetECS();
	auto unit = ecs.CreateEntity();

    ecs.AddComponent<TransformComponent>(unit, {{0, 0, 0}});
	ecs.AddComponent<RenderComponent>(unit, {GREEN, 4.0f});
	ecs.AddComponent<ModelComponent>(unit, {ModelType::PARTY, 0.2f});
	ecs.AddComponent<MovementComponent>(unit, {{0, 0, 0}, 20.0f});
	ecs.AddComponent<GoalComponent>(unit, {{0, 0, 0}, false});

	Game::Instance().State.parties.push_back(unit);
	return unit;
}

EntityId EntityFactory::CreateCity()
{
	auto& ecs = Game::Instance().GetECS();
	auto city = ecs.CreateEntity();

	ecs.AddComponent<TransformComponent>(city, {{0, 0, 0}});
	ecs.AddComponent<ModelComponent>(city, {ModelType::CITY, 1.0f});
	ecs.AddComponent<RenderComponent>(city, {RED, 8.0f});

	Game::Instance().State.cities.push_back(city);
	return city;
}

EntityId EntityFactory::CreateLair()
{
	auto& ecs = Game::Instance().GetECS();
	auto city = ecs.CreateEntity();

	ecs.AddComponent<TransformComponent>(city, {{0, 0, 0}});
	ecs.AddComponent<ModelComponent>(city, {ModelType::LAIR, 1.0f});
	ecs.AddComponent<RenderComponent>(city, {PURPLE, 8.0f});

	Game::Instance().State.lairs.push_back(city);
	return city;
}

EntityId EntityFactory::CreateEnemy()
{
	auto& ecs = Game::Instance().GetECS();
	auto unit = ecs.CreateEntity();
	
	ecs.AddComponent<TransformComponent>(unit, {{0, 0, 0}});
	ecs.AddComponent<RenderComponent>(unit, {BLACK, 4.0f});
	ecs.AddComponent<ModelComponent>(unit, {ModelType::ENEMY, 1.0f});
	ecs.AddComponent<MovementComponent>(unit, {{0, 0, 0}, 20.0f});
	ecs.AddComponent<GoalComponent>(unit, {{0, 0, 0}, false});

	Game::Instance().State.enemies.push_back(unit);
	return unit;
}
