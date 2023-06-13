#include "EntityFactory.hpp"

#include "game.hpp"

#include "Components/TransformComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/ModelComponent.hpp"

#include "game.hpp"
#include "Components/GoalComponent.hpp"

EntityId EntityFactory::CreateHuman(Vector3 position)
{
	auto& ecs = Game::Instance().GetECS();
	auto unit = ecs.CreateEntity();

    ecs.AddComponent<TransformComponent>(unit, {{position}, {0, 0, 0}, 0});
	ecs.AddComponent<RenderComponent>(unit, {GREEN, 4.0f});
	ecs.AddComponent<ModelComponent>(unit, {ModelType::PARTY, 0.2f});
	ecs.AddComponent<MovementComponent>(unit, {{0, 0, 0}, 20.0f});
	ecs.AddComponent<GoalComponent>(unit, {{0, 0, 0}, {}, false, 0});

	Game::Instance().State.Humans.push_back(unit);
	return unit;
}

EntityId EntityFactory::CreateCity(Vector3 position)
{
	auto& ecs = Game::Instance().GetECS();
	auto city = ecs.CreateEntity();

	ecs.AddComponent<TransformComponent>(city, {{position}, {0, 0, 0}, 0});
	ecs.AddComponent<ModelComponent>(city, {ModelType::CITY, 1.0f});
	ecs.AddComponent<RenderComponent>(city, {RED, 8.0f});

	Game::Instance().State.HumanCities.push_back(city);
	return city;
}

EntityId EntityFactory::CreateOrcsTribe(Vector3 position)
{
	auto& ecs = Game::Instance().GetECS();
	auto city = ecs.CreateEntity();

	ecs.AddComponent<TransformComponent>(city, {{position}, {0, 0, 0}, 0});
	ecs.AddComponent<ModelComponent>(city, {ModelType::LAIR, 1.0f});
	ecs.AddComponent<RenderComponent>(city, {PURPLE, 8.0f});

	Game::Instance().State.OrcTribes.push_back(city);
	return city;
}

EntityId EntityFactory::CreateOrc(Vector3 position)
{
	auto& ecs = Game::Instance().GetECS();
	auto unit = ecs.CreateEntity();
	
	ecs.AddComponent<TransformComponent>(unit, {{position}, {0, 0, 0}, 0});
	ecs.AddComponent<RenderComponent>(unit, {BLACK, 4.0f});
	ecs.AddComponent<ModelComponent>(unit, {ModelType::ENEMY, 1.0f});
	ecs.AddComponent<MovementComponent>(unit, {{0, 0, 0}, 20.0f});
	ecs.AddComponent<GoalComponent>(unit, {{0, 0, 0}, {}, false, 0});

	Game::Instance().State.Orcs.push_back(unit);
	return unit;
}
