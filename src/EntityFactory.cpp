#include "EntityFactory.hpp"

#include "ECS/ECSTypes.hpp"

#include "Components/TransformComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/ModelComponent.hpp"

#include "game.hpp"
#include "Components/GoalComponent.hpp"

// 1) Changed function parameters
// 2) Added ModelComponent to every entity type
// 3) No implementation for mouse position in 3D space in Input Manager

EntityId EntityFactory::CreateParty(const std::string model_path)
{
	auto& ecs = Game::Instance().GetECS();
	auto unit = ecs.CreateEntity();

	std::shared_ptr<Model> model = std::make_shared<Model>(LoadModel(model_path.c_str()));
	float model_scale = 0.05f;

    ecs.AddComponent<TransformComponent>(unit, {{0, 0, 0}});
	ecs.AddComponent<RenderComponent>(unit, {GREEN, 4.0f});
	ecs.AddComponent<ModelComponent>(unit, {model, model_scale});
	ecs.AddComponent<MovementComponent>(unit, {{0, 0, 0}, 20.0f});
	ecs.AddComponent<GoalComponent>(unit, {{0, 0, 0}, false});

	Game::Instance().State.parties.push_back(unit);
	return unit;
}

EntityId EntityFactory::CreateCity(const std::string model_path)
{
	auto& ecs = Game::Instance().GetECS();
	auto city = ecs.CreateEntity();

	std::shared_ptr<Model> model = std::make_shared<Model>(LoadModel(model_path.c_str()));
	float model_scale = 1.0f;

	ecs.AddComponent<TransformComponent>(city, {{0, 0, 0}});
	ecs.AddComponent<ModelComponent>(city, {model, model_scale});
	ecs.AddComponent<RenderComponent>(city, {RED, 8.0f});

	Game::Instance().State.cities.push_back(city);
	return city;
}

EntityId EntityFactory::CreateLair(const std::string model_path)
{
	auto& ecs = Game::Instance().GetECS();
	auto city = ecs.CreateEntity();

	std::shared_ptr<Model> model = std::make_shared<Model>(LoadModel(model_path.c_str()));
	float model_scale = 1.0f;

	ecs.AddComponent<TransformComponent>(city, {{0, 0, 0}});
	ecs.AddComponent<ModelComponent>(city, {model, model_scale});
	ecs.AddComponent<RenderComponent>(city, {PURPLE, 8.0f});

	Game::Instance().State.lairs.push_back(city);
	return city;
}

EntityId EntityFactory::CreateEnemy(const std::string model_path)
{
	auto& ecs = Game::Instance().GetECS();
	auto unit = ecs.CreateEntity();

	std::shared_ptr<Model> model = std::make_shared<Model>(LoadModel(model_path.c_str()));
	float model_scale = 1.0f;

	ecs.AddComponent<TransformComponent>(unit, {{0, 0, 0}});
	ecs.AddComponent<RenderComponent>(unit, {BLACK, 4.0f});
	ecs.AddComponent<ModelComponent>(unit, {model, model_scale});
	ecs.AddComponent<MovementComponent>(unit, {{0, 0, 0}, 20.0f});
	ecs.AddComponent<GoalComponent>(unit, {{0, 0, 0}, false});

	Game::Instance().State.enemies.push_back(unit);
	return unit;
}
