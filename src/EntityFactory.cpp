#include "EntityFactory.hpp"

#include "EntityManager.hpp"
#include "Entity.hpp"

#include "Components/TransformComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/RenderComponent.hpp"

#include "game.hpp"
#include "Components/GoalComponent.hpp"


Entity& EntityFactory::CreateParty()
{
	  auto& entityManager = EntityManager::Instance();
	  auto& unit = entityManager.CreateEntity("party");

    int transformIndex;
    auto& transformComponent = unit.AddComponent<TransformComponent>(transformIndex);
    transformComponent.Position = {0, 0};

    auto& renderComponent = unit.AddComponent<RenderComponent>();
    renderComponent.TransformComponentIndex = transformIndex;
    renderComponent.Radius = 4;
    renderComponent.Color = GREEN;

    auto& movementComponent = unit.AddComponent<MovementComponent>();
    movementComponent.PositionComponentIndex = transformIndex;
    movementComponent.Direction = {0, 0};
    movementComponent.Speed = 20.0f;

    auto& goalComponent = unit.AddComponent<GoalComponent>();
    goalComponent.IsActive = false;


	  Game::Instance().State.parties.push_back(unit.id);
	  return unit;
}

Entity& EntityFactory::CreateCity()
{
	auto& entityManager = EntityManager::Instance();
	auto& city = entityManager.CreateEntity("City");

	int transformIndex;
	auto& transformComponent = city.AddComponent<TransformComponent>(transformIndex);
	transformComponent.Position = { 0, 0 };

	auto& renderComponent = city.AddComponent<RenderComponent>();
	renderComponent.TransformComponentIndex = transformIndex;
	renderComponent.Radius = 8;
	renderComponent.Color = RED;

	Game::Instance().State.cities.push_back(city.id);
	return city;
}

Entity& EntityFactory::CreateLair()
{
	auto& entityManager = EntityManager::Instance();
	auto& city = entityManager.CreateEntity("Lair");

	int transformIndex;
	auto& transformComponent = city.AddComponent<TransformComponent>(transformIndex);
	transformComponent.Position = { 0, 0 };

	auto& renderComponent = city.AddComponent<RenderComponent>();
	renderComponent.TransformComponentIndex = transformIndex;
	renderComponent.Radius = 8;
	renderComponent.Color = PURPLE;

	Game::Instance().State.lairs.push_back(city.id);
	return city;
}
