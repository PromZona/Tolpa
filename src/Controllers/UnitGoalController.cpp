#include "Controllers/UnitGoalController.hpp"

#include <vector>

#include "Components/GoalComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/MovementComponent.hpp"

#include "game.hpp"

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"

UnitGoalController::UnitGoalController() = default;
UnitGoalController::~UnitGoalController() = default;

void UnitGoalController::Tick(float deltaTime)
{
	auto& ecs = Game::Instance().GetECS();
	auto& cities = Game::Instance().State.cities;

	auto archetypes = ecs.GetRequiredArchetypes(Archetype);

	for (auto archetype : archetypes)
	{
		auto& moves = archetype->GetComponents<MovementComponent>();
		auto& trans = archetype->GetComponents<TransformComponent>();
		auto& goals = archetype->GetComponents<GoalComponent>();
		std::size_t size = moves.size();
		for (int i = 0; i < size; i++)
		{
			auto& goalComp = goals[i];
			auto& movementComp = moves[i];
			auto& transformComp = trans[i];

			if (Vector2Distance(goalComp.GoalPosition, transformComp.Position) < 0.5f)
			{
				goalComp.IsActive = false;
			}

			if (!goalComp.IsActive)
			{
				EntityId cityId = cities[GetRandomValue(0, cities.size() - 1)];
				Vector2 cityPosition = ecs.GetComponent<TransformComponent>(cityId)->Position;

				goalComp.GoalPosition = cityPosition;
				goalComp.IsActive = true;

				movementComp.Direction = Vector2Normalize(Vector2Subtract(cityPosition, transformComp.Position));
			}
		}
	}
}