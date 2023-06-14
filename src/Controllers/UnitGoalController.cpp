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
	auto& cities = Game::Instance().State.HumanCities;
	auto& navMeshTriangles = Game::Instance().GetNavGrid().GetTriangles();

	if (cities.empty())
		return;

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

			if (goalComp.IsActive)
			{
				if (goalComp.steps == goalComp.PathToGoal.size())
				{
					transformComp.Position = navMeshTriangles[GetRandomValue(0, navMeshTriangles.size() - 1)].middlePoint;
					goalComp.IsActive = false;
				}
			}

			if (!goalComp.IsActive)
			{
				EntityId cityId = cities[GetRandomValue(0, cities.size() - 1)];
				Vector3 cityPosition = ecs.GetComponent<TransformComponent>(cityId)->Position;

				goalComp.PathToGoal = Game::Instance().GetNavGrid().FindPath(transformComp.Position, cityPosition);
				goalComp.steps = 0;
				goalComp.GoalPosition = goalComp.PathToGoal[0];
				goalComp.IsActive = true;
			}
			else
			{
				if (Vector3Distance(goalComp.GoalPosition, transformComp.Position) < 0.5f)
				{
					transformComp.Position = goalComp.GoalPosition;
					goalComp.steps++;
					goalComp.GoalPosition = goalComp.PathToGoal[goalComp.steps];
				}

				movementComp.Direction = Vector3Normalize(Vector3Subtract(goalComp.GoalPosition, 
																		  transformComp.Position));
			}
		}
	}
}