#include "Controllers/UnitGoalController.hpp"

#include <vector>

#include "Components/GoalComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/MovementComponent.hpp"

#include "EntityManager.hpp"

#include "game.hpp"

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"

UnitGoalController::UnitGoalController() = default;
UnitGoalController::~UnitGoalController() = default;

void UnitGoalController::Tick(float deltaTime)
{
	auto& entityManager = EntityManager::Instance();
	auto& parties = Game::Instance().State.parties;
	auto& cities = Game::Instance().State.cities;

	for (auto& partyId : parties)
	{
		auto& unit = entityManager.GetEntity(partyId);
		auto& goalComp = unit.GetComponent<GoalComponent>();
		auto& movementComp = unit.GetComponent<MovementComponent>();
		auto& transformComp = unit.GetComponent<TransformComponent>();

        if (Vector2Distance(goalComp.GoalPosition, transformComp.Position) < 0.5f)
        {
            goalComp.IsActive = false;
        }

        if (!goalComp.IsActive)
        {
            int cityId = cities[GetRandomValue(0, cities.size() - 1)];
            Vector2 cityPosition = entityManager.GetEntity(cityId).GetComponent<TransformComponent>().Position;

            goalComp.GoalPosition = cityPosition;
            goalComp.IsActive = true;

            movementComp.Direction = Vector2Normalize(Vector2Subtract(cityPosition, transformComp.Position));
        }
    }
}